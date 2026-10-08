/* norma_stml.c - facies STML normae (norma-spec-2; norma-plan-3: A3
 * lector, A4 scriptor). Canon primus iudicat (norma.canon infixus),
 * lector sensum. */
#include "norma_stml.h"
#include "norma_canon.h"
#include "stml.h"
#include "canon.h"
#include "internamentum.h"
#include "chorda_aedificator.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ====================================================================
 * A. LECTOR
 * ==================================================================== */

/* norma nominata: status 0 nondum, I in aedificatione, II facta */
nomen structura {
        chorda  titulus;    /* nomen (canon genus nomen) */
     StmlNodus* nodus;      /* elementum norma */
         Norma* facta;
           i32  status;
} NominataLectoris;

nomen structura {
                           Piscina* p;
                            chorda  fons;
    constans NormaGignensNominatum* gignentes;
                               i32  numerus_gignentium;
                               Xar* vitia;
                               Xar* notae;
                               Xar* nominatae;   /* NominataLectoris */
                               Xar* catena;      /* i32: in aedificatione */
} Lector;

interior constans character* constans _typi[] = {
    "liberum", "nullum", "boolean", "integer", "numerus", "textus",
    "tabulatum", "objectum", "discrimen", "ad", NIHIL
};

interior b32
_est (
             StmlNodus* n,
    constans character* titulus)
{
    redde n && n->genus == STML_NODUS_ELEMENTUM && n->titulus
        && chorda_aequalis_literis(*n->titulus, titulus);
}

interior b32
_est_typus (
    StmlNodus* n)
{
    i32 i;

    per (i = 0; _typi[i]; i++)
    {
        si (_est(n, _typi[i]))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior StmlNodus*
_filius (
    StmlNodus* n,
          i32  i)
{
    redde *(StmlNodus**)xar_obtinere(n->liberi, i);
}

/* columna 1-basata ex positu nodi (octeti post lineam novam) */
interior i32
_columna (
       Lector* lector,
    StmlNodus* n)
{
    i32 i;

    si (!n || n->linea == 0)
    {
        redde 0;
    }
    i = n->positus_initium;
    dum (i > 0 && lector->fons.datum[i - I] != '\n')
    {
        i--;
    }
    redde n->positus_initium - i + I;
}

interior vacuum
_vitium (
                 Lector* lector,
              StmlNodus* nodus,
         NormaStmlCausa  causa,
     constans character* nuntius)
{
    NormaStmlVitium* v = (NormaStmlVitium*)xar_addere(
        causa == NORMA_STML_GIGNENS_SINE_REGISTRO ? lector->notae
                                                  : lector->vitia);

    v->linea    = nodus ? nodus->linea : 0;
    v->columna  = _columna(lector, nodus);
    v->causa    = causa;
    v->nuntius  = chorda_ex_literis(nuntius, lector->p);
}

/* attributum ut literae C propriae (NIHIL si abest) */
interior character*
_attributum (
                 Lector* lector,
              StmlNodus* n,
     constans character* titulus)
{
    chorda* c = stml_attributum_capere(n, titulus);

    redde c ? chorda_ut_cstr(*c, lector->p) : NIHIL;
}

interior b32
_verum (
                 Lector* lector,
              StmlNodus* n,
     constans character* titulus,
                    b32  praestitutum)
{
    character* s = _attributum(lector, n, titulus);

    redde s ? strcmp(s, "verum") == 0 : praestitutum;
}

interior NormaModus
_modus (
       Lector* lector,
    StmlNodus* n)
{
    character* s = _attributum(lector, n, "modus");

    si (s && strcmp(s, "apertum") == 0)
    {
        redde NORMA_APERTUM;
    }
    si (s && strcmp(s, "notandum") == 0)
    {
        redde NORMA_NOTANDUM;
    }
    redde NORMA_CLAUSUM;
}

interior b32
_integrum (
                 Lector* lector,
              StmlNodus* n,
     constans character* s,
                    s64  infimum,
                    s64  summum,
                    s64* exitus)
{
    character* finis;
       longus  v;
    character  nuntius[CCLVI];

    errno  = 0;
    v      = strtol(s, &finis, X);
    si (   errno == ERANGE || finis == s || *finis != '\0'
        || (s64)v < infimum || (s64)v > summum)
    {
        sprintf(nuntius, "finis pravus: '%.100s'", s);
        _vitium(lector, n, NORMA_STML_FINIS_PRAVUS, nuntius);
        redde FALSUM;
    }
    *exitus = (s64)v;
    redde VERUM;
}

interior b32
_fluitans (
                 Lector* lector,
              StmlNodus* n,
     constans character* s,
                    f64* exitus)
{
    character* finis;
          f64  v;
    character  nuntius[CCLVI];

    errno  = 0;
    v      = strtod(s, &finis);
    si (   finis == s || *finis != '\0' || errno == ERANGE
        || v     != v || v - v != 0.0)
    {
        sprintf(nuntius,
            "finis pravus (f64 finitus postulatur): '%.100s'",
            s);
        _vitium(lector, n, NORMA_STML_FINIS_PRAVUS, nuntius);
        redde FALSUM;
    }
    *exitus = v;
    redde VERUM;
}

/* par finium (R3): 0 neuter, I ambo, II vitium iam positum */
interior i32
_par (
                 Lector*  lector,
              StmlNodus*  n,
     constans character*  a,
     constans character*  b,
              character** sa,
              character** sb)
{
    character nuntius[CCLVI];

    *sa = _attributum(lector, n, a);
    *sb = _attributum(lector, n, b);
    si (!*sa && !*sb)
    {
        redde 0;
    }
    si (!*sa || !*sb)
    {
        sprintf(nuntius, "'%s' sine '%s' (fines bini; norma-plan-3 R3)",
            *sa ? a : b, *sa ? b : a);
        _vitium(lector, n, NORMA_STML_FINIS_DIMIDIATUS, nuntius);
        redde II;
    }
    redde I;
}

interior vacuum
_inversi (
       Lector* lector,
    StmlNodus* n)
{
    _vitium(lector, n, NORMA_STML_FINES_INVERSI, "minimum > maximum");
}

interior vacuum
_intra_integrum (
       Lector* lector,
    StmlNodus* n,
        Norma* norma)
{
    character *sa, *sb;
          s64  a, b;

    si (_par(lector, n, "minimum", "maximum", &sa, &sb) != I)
    {
        redde;
    }
    si (   !_integrum(lector, n, sa, (s64)LONG_MIN, (s64)LONG_MAX, &a)
        || !_integrum(lector, n, sb, (s64)LONG_MIN, (s64)LONG_MAX, &b))
    {
        redde;
    }
    si (a > b)
    {
        _inversi(lector, n);
        redde;
    }
    norma_intra(norma, a, b);
}

interior vacuum
_intra_fluitans (
       Lector* lector,
    StmlNodus* n,
        Norma* norma)
{
    character *sa, *sb;
          f64  a, b;

    si (_par(lector, n, "minimum", "maximum", &sa, &sb) != I)
    {
        redde;
    }
    si (!_fluitans(lector, n, sa, &a) || !_fluitans(lector, n, sb, &b))
    {
        redde;
    }
    si (a > b)
    {
        _inversi(lector, n);
        redde;
    }
    norma_intra_fluitans(norma, a, b);
}

interior vacuum
_longitudo (
       Lector* lector,
    StmlNodus* n,
        Norma* norma)
{
    character *sa, *sb;
          s64  a, b;

    si (_par(lector, n, "longitudo_minima", "longitudo_maxima", &sa,
        &sb)
        != I)
    {
        redde;
    }
    si (   !_integrum(lector, n, sa, 0, (s64)0xFFFFFFFFL, &a)
        || !_integrum(lector, n, sb, 0, (s64)0xFFFFFFFFL, &b))
    {
        redde;
    }
    si (a > b)
    {
        _inversi(lector, n);
        redde;
    }
    norma_longitudo(norma, (i32)a, (i32)b);
}

interior b32
_spatium (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior vacuum
_electio (
       Lector* lector,
    StmlNodus* n,
        Norma* norma)
{
              character* e       = _attributum(lector, n, "electio");
                    Xar* licita  = xar_creare(lector->p,
                                       (i32)magnitudo(character*));
                    i32   ex_liberis = 0;
                    i32   i;
     constans character** tabula;

    per (i = 0; i < xar_numerus(n->liberi); i++)
    {
        StmlNodus* f = _filius(n, i);

        si (_est(f, "licitum"))
        {
            *(character**)xar_addere(licita) = chorda_ut_cstr(
                stml_textus_valor(f, lector->p), lector->p);
            ex_liberis++;
        }
    }
    si (e && ex_liberis > 0)
    {
        _vitium(lector, n, NORMA_STML_ELECTIO_DUPLEX,
            "electio= et <licitum> simul");
        redde;
    }
    si (e)
    {
        character* s = e;

        dum (*s)
        {
            dum (_spatium(*s))
            {
                s++;
            }
            si (!*s)
            {
                frange;
            }
            *(character**)xar_addere(licita) = s;
            dum (*s && !_spatium(*s))
            {
                s++;
            }
            si (*s)
            {
                *s = '\0';
                s++;
            }
        }
        si (xar_numerus(licita) == 0)
        {
            _vitium(lector, n, NORMA_STML_ELECTIO_VACUA,
                "electio sine valore");
            redde;
        }
    }
    si (xar_numerus(licita) == 0)
    {
        redde;
    }
    tabula = (constans character**)piscina_allocare(lector->p,
        (memoriae_index)((xar_numerus(licita) + I)
                         * magnitudo(character*)));
    per (i = 0; i < xar_numerus(licita); i++)
    {
        tabula[i] = *(character**)xar_obtinere(licita, i);
    }
    tabula[i] = NIHIL;
    norma_electio(norma, tabula);
}

interior vacuum
_descriptio (
       Lector* lector,
    StmlNodus* n,
        Norma* norma)
{
    i32 i;

    per (i = 0; i < xar_numerus(n->liberi); i++)
    {
        si (_est(_filius(n, i), "descriptio"))
        {
            norma_descriptio(norma, chorda_ut_cstr(stml_textus_valor(
                _filius(n, i), lector->p), lector->p));
        }
    }
}

interior vacuum
_gignens (
       Lector* lector,
    StmlNodus* n,
        Norma* norma)
{
    character* g = _attributum(lector, n, "gignens");
          i32  i;
    character  nuntius[CCLVI];

    si (!g)
    {
        redde;
    }
    norma_gignens_titulus(norma, g);
    si (!lector->gignentes)
    {
        sprintf(nuntius,
            "gignens '%.100s' sine registro: titulus servatus",
            g);
        _vitium(lector, n, NORMA_STML_GIGNENS_SINE_REGISTRO, nuntius);
        redde;
    }
    per (i = 0; i < lector->numerus_gignentium; i++)
    {
        si (strcmp(lector->gignentes[i].titulus, g) == 0)
        {
            norma_gignens(norma, lector->gignentes[i].functio,
                lector->gignentes[i].datum);
            redde;
        }
    }
    sprintf(nuntius, "gignens ignotum: '%.100s'", g);
    _vitium(lector, n, NORMA_STML_GIGNENS_IGNOTUM, nuntius);
}

interior Norma*
_struere (
       Lector* lector,
    StmlNodus* n);

/* typus filius unicus continentis (norma, campus, tabulatum) */
interior Norma*
_typus_unicus (
       Lector* lector,
    StmlNodus* continens)
{
    StmlNodus* typus    = NIHIL;
          i32  numerus  = 0;
          i32  i;
    character  nuntius[CCLVI];

    per (i = 0; i < xar_numerus(continens->liberi); i++)
    {
        si (_est_typus(_filius(continens, i)))
        {
            typus = _filius(continens, i);
            numerus++;
        }
    }
    si (numerus != I)
    {
        sprintf(nuntius, "<%.*s> typum unum postulat, %u invenit",
            (integer)continens->titulus->mensura,
            (constans character*)continens->titulus->datum,
            (insignatus integer)numerus);
        _vitium(lector, continens, NORMA_STML_LIBERI_TYPI, nuntius);
        redde NIHIL;
    }
    redde _struere(lector, typus);
}

interior i32
_index_nominatae (
                 Lector* lector,
     constans character* titulus)
{
    i32 i;

    per (i = 0; i < xar_numerus(lector->nominatae); i++)
    {
        si (chorda_aequalis_literis(((NominataLectoris*)xar_obtinere(
                lector->nominatae, i))->titulus, titulus))
        {
            redde i;
        }
    }
    redde xar_numerus(lector->nominatae);
}

interior NominataLectoris*
_nominatam (
    Lector* lector,
       i32  i)
{
    redde (NominataLectoris*)xar_obtinere(lector->nominatae, i);
}

interior vacuum
_circulus (
       Lector* lector,
    StmlNodus* citans,
          i32  index)
{
    ChordaAedificator* a = chorda_aedificator_creare(lector->p, CCLVI);
                  i32  i;
                  b32  intra = FALSUM;
               chorda  c;

    chorda_aedificator_appendere_literis(a, "circulus: ");
    per (i = 0; i < xar_numerus(lector->catena); i++)
    {
        i32 k = *(i32*)xar_obtinere(lector->catena, i);

        si (k == index)
        {
            intra = VERUM;
        }
        si (intra)
        {
            chorda_aedificator_appendere_chorda(a,
                _nominatam(lector, k)->titulus);
            chorda_aedificator_appendere_literis(a, " -> ");
        }
    }
    chorda_aedificator_appendere_chorda(a, _nominatam(lector,
        index)->titulus);
    c = chorda_aedificator_finire(a);
    _vitium(lector, citans, NORMA_STML_CIRCULUS,
        chorda_ut_cstr(c, lector->p));
}

interior Norma*
_nominata (
       Lector* lector,
          i32  index,
    StmlNodus* citans)
{
    NominataLectoris* nl = _nominatam(lector, index);

    si (nl->status == II)
    {
        redde nl->facta;
    }
    si (nl->status == I)
    {
        _circulus(lector, citans, index);
        redde NIHIL;
    }
    nl->status = I;
    *(i32*)xar_addere(lector->catena) = index;
    nl->facta = _typus_unicus(lector, nl->nodus);
    xar_removere_ultimum(lector->catena);
    nl->status = II;
    redde nl->facta;
}

interior Norma*
_struere (
       Lector* lector,
    StmlNodus* n)
{
        Norma* norma = NIHIL;
    character* s;
          i32  i;

    si (_est(n, "ad"))
    {
        s = _attributum(lector, n, "norma");
        i = _index_nominatae(lector, s ? s : "");
        si (i == xar_numerus(lector->nominatae))
        {
            _vitium(lector, n, NORMA_STML_CANON, "citatio irrita");
            redde NIHIL;   /* canon id iam clamavit; custodia */
        }
        redde _nominata(lector, i, n);
    }
    si (_est(n, "liberum"))
    {
        norma = norma_liberum(lector->p);
    }
    alioquin si (_est(n, "nullum"))
    {
        norma = norma_nullum(lector->p);
    }
    alioquin si (_est(n, "boolean"))
    {
        norma = norma_boolean(lector->p);
    }
    alioquin si (_est(n, "integer"))
    {
        norma = norma_integer(lector->p);
        _intra_integrum(lector, n, norma);
    }
    alioquin si (_est(n, "numerus"))
    {
        norma = norma_numerus(lector->p);
        _intra_fluitans(lector, n, norma);
    }
    alioquin si (_est(n, "textus"))
    {
        norma = norma_textus(lector->p);
        _longitudo(lector, n, norma);
        s = _attributum(lector, n, "forma");
        si (s)
        {
            norma_forma(norma, s);
        }
        _electio(lector, n, norma);
    }
    alioquin si (_est(n, "tabulatum"))
    {
        Norma* e = _typus_unicus(lector, n);

        norma = norma_tabulatum(lector->p,
            e ? e : norma_liberum(lector->p));
        _longitudo(lector, n, norma);
    }
    alioquin si (_est(n, "objectum"))
    {
        norma = norma_modus(norma_objectum(lector->p), _modus(lector,
            n));
        per (i = 0; i < xar_numerus(n->liberi); i++)
        {
            StmlNodus* c = _filius(n, i);
                Norma* valor;

            si (!_est(c, "campus"))
            {
                perge;
            }
            valor = _typus_unicus(lector, c);
            si (valor)
            {
                norma_campus(norma, _attributum(lector, c, "titulus"),
                    valor,
                    _verum(lector, c, "requiritur", VERUM));
            }
        }
    }
    alioquin si (_est(n, "discrimen"))
    {
        norma = norma_modus(norma_discrimen(lector->p,
            _attributum(lector, n, "clavis")), _modus(lector, n));
        per (i = 0; i < xar_numerus(n->liberi); i++)
        {
             StmlNodus* va = _filius(n, i);
                   i32  k;

            si (!_est(va, "variatio"))
            {
                perge;
            }
            per (k = 0; k < xar_numerus(va->liberi); k++)
            {
                si (_est(_filius(va, k), "objectum"))
                {
                    Norma* o = _struere(lector, _filius(va, k));

                    si (o)
                    {
                        norma_variatio(norma, _attributum(lector, va,
                            "valor"),
                            o);
                    }
                }
            }
        }
    }
    si (!norma)
    {
        redde NIHIL;
    }
    _descriptio(lector, n, norma);
    _gignens(lector, n, norma);
    si (_verum(lector, n, "aut_nullum", FALSUM))
    {
        norma_aut_nullum(norma);
    }
    redde norma;
}

/* nodus pravus (custodia: canon claves duplicatas iam vetat) */
interior b32
_pravum (
    constans Norma* n,
            chorda* nuntius)
{
    NormaVisus v = norma_visus(n);
           i32 i;

    si (!n)
    {
        redde FALSUM;
    }
    si (v.error_schematis.mensura > 0)
    {
        *nuntius = v.error_schematis;
        redde VERUM;
    }
    si (v.elementum && _pravum(v.elementum, nuntius))
    {
        redde VERUM;
    }
    per (i = 0; v.campi && i < xar_numerus(v.campi); i++)
    {
        si (_pravum(((NormaCampus*)xar_obtinere(v.campi, i))->valor,
                nuntius))
        {
            redde VERUM;
        }
    }
    per (i = 0; v.variationes && i < xar_numerus(v.variationes); i++)
    {
        si (_pravum(((NormaVariatio*)xar_obtinere(v.variationes,
                i))->objectum, nuntius))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

NormaStmlLectio
norma_stml_legere (
                               chorda  fons,
       constans NormaGignensNominatum* gignentes,
                                  i32  numerus_gignentium,
                              Piscina* piscina)
{
        NormaStmlLectio  l;
                 Lector  lector;
    InternamentumChorda* in;
           StmlResultus  r;
                  Canon* canon;
                 chorda  causa;
                    Xar* cv;
                    i32  i;
              character  nuntius[DXII];

    memset(&l, 0, magnitudo(l));
    memset(&lector, 0, magnitudo(lector));
    lector.p                   = piscina;
    lector.fons                = fons;
    lector.gignentes           = gignentes;
    lector.numerus_gignentium  = numerus_gignentium;
    lector.vitia = xar_creare(piscina,
        (i32)magnitudo(NormaStmlVitium));
    lector.notae = xar_creare(piscina,
        (i32)magnitudo(NormaStmlVitium));
    lector.nominatae = xar_creare(piscina,
        (i32)magnitudo(NominataLectoris));
    lector.catena  = xar_creare(piscina, (i32)magnitudo(i32));
    l.normae       = xar_creare(piscina, (i32)magnitudo(NormaNominata));
    l.vitia        = lector.vitia;
    l.notae        = lector.notae;

    in  = internamentum_creare(piscina);
    r   = stml_legere(fons, piscina, in);
    si (!r.successus)
    {
        NormaStmlVitium* v = (NormaStmlVitium*)xar_addere(lector.vitia);

        v->linea    = r.linea_erroris;
        v->columna  = r.columna_erroris;
        v->causa    = NORMA_STML_FRACTUM;
        v->nuntius  = r.error;
        redde l;
    }
    canon = canon_legere(norma_canon_textus(piscina), piscina, in,
        &causa);
    si (!canon)
    {
        sprintf(nuntius, "canon infixus fractus: %.*s",
            (integer)(causa.mensura > CC ? CC : causa.mensura),
            (constans character*)causa.datum);
        _vitium(&lector, NIHIL, NORMA_STML_CANON, nuntius);
        redde l;
    }
    cv = canon_iudicare(canon, r.radix, piscina);
    per (i = 0; i < xar_numerus(cv); i++)
    {
        CanonVitium* c = (CanonVitium*)xar_obtinere(cv, i);

        sprintf(nuntius, "%s%s%.*s%s%.*s", canon_nuntius(c->genus),
            c->elementum ? ": <" : "",
            c->elementum ? (integer)c->elementum->mensura : 0,
            c->elementum ? (constans character*)c->elementum->datum : "",
            c->detail ? "> " : (c->elementum ? ">" : ""),
            c->detail ? (integer)c->detail->mensura : 0,
            c->detail ? (constans character*)c->detail->datum : "");
        _vitium(&lector, c->nodus, NORMA_STML_CANON, nuntius);
    }
    si (xar_numerus(lector.vitia) > 0)
    {
        redde l;
    }
    per (i = 0; i < xar_numerus(r.elementum_radix->liberi); i++)
    {
        StmlNodus* f = _filius(r.elementum_radix, i);

        si (_est(f, "norma"))
        {
            NominataLectoris* nl = (NominataLectoris*)xar_addere(
                lector.nominatae);
                   character* t  = _attributum(&lector, f, "titulus");

            nl->titulus  = chorda_ex_literis(t, piscina);
            nl->nodus    = f;
            nl->facta    = NIHIL;
            nl->status   = 0;
        }
    }
    per (i = 0; i < xar_numerus(lector.nominatae); i++)
    {
        chorda pravum;

        _nominata(&lector, i, NIHIL);
        si (_pravum(_nominatam(&lector, i)->facta, &pravum))
        {
            sprintf(nuntius, "schema pravum: %.*s",
                (integer)(pravum.mensura > CC ? CC : pravum.mensura),
                (constans character*)pravum.datum);
            _vitium(&lector, _nominatam(&lector, i)->nodus,
                NORMA_STML_CANON,
                nuntius);
        }
    }
    si (xar_numerus(lector.vitia) > 0)
    {
        redde l;
    }
    per (i = 0; i < xar_numerus(lector.nominatae); i++)
    {
        NormaNominata* nn = (NormaNominata*)xar_addere(l.normae);

        nn->titulus  = _nominatam(&lector, i)->titulus;
        nn->norma    = _nominatam(&lector, i)->facta;
    }
    l.successus = VERUM;
    redde l;
}

Norma*
norma_stml_quaerere (
    constans NormaStmlLectio* lectio,
          constans character* titulus)
{
    i32 i;

    si (!lectio || !lectio->normae || !titulus)
    {
        redde NIHIL;
    }
    per (i = 0; i < xar_numerus(lectio->normae); i++)
    {
        NormaNominata* nn = (NormaNominata*)xar_obtinere(lectio->normae,
            i);

        si (chorda_aequalis_literis(nn->titulus, titulus))
        {
            redde nn->norma;
        }
    }
    redde NIHIL;
}
