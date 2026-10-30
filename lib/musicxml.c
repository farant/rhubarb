/* musicxml.c - lector MusicXML (caput include/musicxml.h)
 *
 * Super xml_documentum.h (visio cocta). Duo transitus: primus omnes
 * <divisions> colligit et divisionem operis computat (minimus
 * communis multiplex, M4); secundus partes per mensuras ambulat cum
 * cursore in divisionibus operis. Deinde linea temporis communis
 * (M15). Arcae .mxl: plan P3. Spec: project-specs/musicxml-spec.md.
 */

#include "musicxml.h"
#include "xml_documentum.h"
#include "materia_diagnosticum.h"
#include "xar.h"
#include <stdlib.h>
#include <string.h>

#define DIVISIONES_MAXIMAE ((s64)2147483647)


/* ==================================================
 * Lector et vitia
 * ================================================== */

nomen structura {
    Piscina* piscina;
        Xar* vitia;          /* MusicxmlVitium */
        s64  divisiones;     /* divisio operis */
} Lector;

interior vacuum
_vitium_locatum (
                  Lector* l,
        MusicxmlGravitas  gravitas,
     MusicxmlVitiumGenus  genus,
                     i32  linea,
                     i32  columna,
      constans character* causa,
                  chorda  adiectum)
{
       MusicxmlVitium* v = (MusicxmlVitium*)xar_addere(l->vitia);
    ChordaAedificator* a;

    si (v == NIHIL)
    {
        redde;
    }
    memset(v, ZEPHYRUM, magnitudo(MusicxmlVitium));
    v->gravitas  = gravitas;
    v->genus     = genus;
    v->linea     = linea;
    v->columna   = columna;
    a            = chorda_aedificator_creare(l->piscina, CXXVIII);
    si (a == NIHIL)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(a, causa);
    si (adiectum.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, adiectum);
    }
    v->causa = chorda_aedificator_finire(a);
}

interior vacuum
_vitium (
                  Lector* l,
        MusicxmlGravitas  gravitas,
     MusicxmlVitiumGenus  genus,
      constans XmlCoctus* nodus,
      constans character* causa,
                  chorda  adiectum)
{
    i32 linea    = ZEPHYRUM;
    i32 columna  = ZEPHYRUM;

    (vacuum)xml_locus(nodus, &linea, &columna);
    _vitium_locatum(l, gravitas, genus, linea, columna, causa,
        adiectum);
}

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}


/* ==================================================
 * Textus et numeri
 * ================================================== */

interior b32
_spatium (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

/* spatia utrimque abscissa (sine allocatione) */
interior chorda
_praecidere (
    chorda c)
{
    dum (c.mensura > ZEPHYRUM && _spatium((character)c.datum[ZEPHYRUM]))
    {
        c.datum++;
        c.mensura--;
    }
    dum (   c.mensura > ZEPHYRUM
         && _spatium((character)c.datum[c.mensura - I]))
    {
        c.mensura--;
    }
    redde c;
}

/* [+-]?cifrae; FALSUM si non numerus aut nimius */
interior b32
_integer (
    chorda  c,
      s64* n)
{
    s64 v      = ZEPHYRUM;
    s64 limes  = DIVISIONES_MAXIMAE * (s64)M;
    b32 minus  = FALSUM;
    i32 i      = ZEPHYRUM;

    c = _praecidere(c);
    si (   c.mensura > ZEPHYRUM
        && (c.datum[ZEPHYRUM] == '+' || c.datum[ZEPHYRUM] == '-'))
    {
        minus  = c.datum[ZEPHYRUM] == '-';
        i      = I;
    }
    si (i >= c.mensura)
    {
        redde FALSUM;
    }
    per (; i < c.mensura; i++)
    {
        si (c.datum[i] < '0' || c.datum[i] > '9' || v > limes)
        {
            redde FALSUM;
        }
        v = v * X + (s64)(c.datum[i] - '0');
    }
    *n = minus ? -v : v;
    redde VERUM;
}

/* [+-]?cifrae[.cifrae]; FALSUM si non numerus */
interior b32
_decimalis (
    chorda  c,
       f64* n)
{
    f64 v        = 0.0;
    f64 scala    = 1.0;
    b32 minus    = FALSUM;
    b32 punctum  = FALSUM;
    i32 cifrae   = ZEPHYRUM;
    i32 i        = ZEPHYRUM;

    c = _praecidere(c);
    si (   c.mensura > ZEPHYRUM
        && (c.datum[ZEPHYRUM] == '+' || c.datum[ZEPHYRUM] == '-'))
    {
        minus  = c.datum[ZEPHYRUM] == '-';
        i      = I;
    }
    per (; i < c.mensura; i++)
    {
        si (c.datum[i] == '.' && !punctum)
        {
            punctum = VERUM;
        }
        alioquin si (   c.datum[i] >= '0' && c.datum[i] <= '9'
                     && cifrae < XVIII)
        {
            v = v * 10.0 + (f64)(c.datum[i] - '0');
            si (punctum)
            {
                scala *= 10.0;
            }
            cifrae++;
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    si (cifrae == ZEPHYRUM)
    {
        redde FALSUM;
    }
    *n = (minus ? -v : v) / scala;
    redde VERUM;
}

/* textus filii (praecisus); FALSUM si filius abest */
interior b32
_textus_filii (
                 Lector* l,
     constans XmlCoctus* e,
     constans character* titulus,
                 chorda* t)
{
    constans XmlCoctus* f = xml_filius(e, titulus);

    *t = _vacua();
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    si (!xml_textus_totus(f, l->piscina, t))
    {
        *t = _vacua();
    }
    *t = _praecidere(*t);
    redde VERUM;
}

/* numerus filii: VERUM si adest et legitur; monitum NUMERUS_MALUS si
 * adest sed non legitur */
interior b32
_integer_filii (
                 Lector* l,
     constans XmlCoctus* e,
     constans character* titulus,
                    s64* n)
{
    chorda t;

    si (!_textus_filii(l, e, titulus, &t))
    {
        redde FALSUM;
    }
    si (!_integer(t, n))
    {
        _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS,
            xml_filius(e, titulus), "numerus illegibilis: ", t);
        redde FALSUM;
    }
    redde VERUM;
}

interior chorda
_attributum (
     constans XmlCoctus* e,
     constans character* titulus)
{
    chorda v;

    si (!xml_attributum(e, titulus, &v))
    {
        redde _vacua();
    }
    redde _praecidere(v);
}

interior vacuum*
_tabula (
              Lector* l,
        constans Xar* x,
                 i32  magnitudo_elementi,
                 i32* numerus)
{
    vacuum* t;
       i32  n = x != NIHIL ? xar_numerus(x) : ZEPHYRUM;

    *numerus = ZEPHYRUM;
    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    t = piscina_allocare(l->piscina,
            (memoriae_index)(n * magnitudo_elementi));
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    (vacuum)xar_copiare_ad_tabulam(x, t, ZEPHYRUM, n);
    *numerus = n;
    redde t;
}

interior s64
_divisor_communis (
    s64 a,
    s64 b)
{
    dum (b != ZEPHYRUM)
    {
        s64 t = a % b;

        a = b;
        b = t;
    }
    redde a < ZEPHYRUM ? -a : a;
}

/* gradus diatonicus litterae in semitoniis supra C; -I si non littera */
interior s32
_gradus (
    character littera)
{
    commutatio (littera)
    {
        casus 'C': redde ZEPHYRUM;
        casus 'D': redde II;
        casus 'E': redde IV;
        casus 'F': redde V;
        casus 'G': redde VII;
        casus 'A': redde IX;
        casus 'B': redde XI;
        ordinarius: redde -I;
    }
}

/* nota MIDI: alteratio rotundata (dimidium sursum) */
interior s32
_notam_computare (
    character littera,
          f64 alteratio,
          s32 octava)
{
    f64 a = alteratio + 0.5;
    s32 r = (s32)a;

    si (a < 0.0 && (f64)r != a)
    {
        r--;
    }
    redde XII * (octava + I) + _gradus(littera) + r;
}


/* ==================================================
 * Transitus primus: divisio operis
 * ================================================== */

/* FALSUM = multiplex nimius (vitium scriptum) */
interior b32
_divisiones_colligere (
                 Lector* l,
     constans XmlCoctus* radix)
{
     constans XmlCoctus* p;
     constans XmlCoctus* m;
     constans XmlCoctus* a;
     constans XmlCoctus* d;
                    s64  multiplex = I;

    per (p = xml_filius(radix, "part"); p != NIHIL;
         p = xml_frater(p, "part"))
    {
        per (m = xml_filius(p, "measure"); m != NIHIL;
             m = xml_frater(m, "measure"))
        {
            per (a = xml_filius(m, "attributes"); a != NIHIL;
                 a = xml_frater(a, "attributes"))
            {
                per (d = xml_filius(a, "divisions"); d != NIHIL;
                     d = xml_frater(d, "divisions"))
                {
                    chorda t;
                       s64 v;
                       s64 g;

                    si (   !xml_textus_totus(d, l->piscina, &t)
                        || !_integer(t, &v) || v <= ZEPHYRUM)
                    {
                        _vitium(l, MUSICXML_MONITUM,
                            MUSICXML_NUMERUS_MALUS, d,
                            "divisions illegibiles (praeteritae): ",
                            _praecidere(t));
                        perge;
                    }
                    g = _divisor_communis(multiplex, v);
                    si (multiplex / g > DIVISIONES_MAXIMAE / v)
                    {
                        _vitium(l, MUSICXML_VITIUM,
                            MUSICXML_DIVISIONES_NIMIAE, d,
                            "divisio operis ultra 2^31: ", t);
                        redde FALSUM;
                    }
                    multiplex = multiplex / g * v;
                }
            }
        }
    }
    l->divisiones = multiplex;
    redde VERUM;
}


/* ==================================================
 * Attributa
 * ================================================== */

nomen structura {
                chorda  vox;
                   s32  nota;
    constans XmlCoctus* nodus;
} LigaturaAperta;

nomen structura {
     s64  factor;          /* divisio operis / divisiones scriptae */
     Xar* ligaturae;       /* LigaturaAperta, per partem */
} StatusPartis;

interior vacuum
_clavem_legere (
                 Lector* l,
     constans XmlCoctus* k,
         MusicxmlClavis* clavis)
{
     constans XmlCoctus* f;
                 chorda  t;
                    s64  v;
                    Xar* litterae;
                    Xar* alterationes;

    memset(clavis, ZEPHYRUM, magnitudo(MusicxmlClavis));
    si (_integer_filii(l, k, "fifths", &v))
    {
        clavis->tradita = VERUM;
        clavis->quintae = (s32)v;
        (vacuum)_textus_filii(l, k, "mode", &clavis->modus);
        redde;
    }
    litterae      = xar_creare(l->piscina, (i32)magnitudo(character));
    alterationes  = xar_creare(l->piscina, (i32)magnitudo(f64));
    si (litterae == NIHIL || alterationes == NIHIL)
    {
        redde;
    }
    per (f = xml_filius(k, NIHIL); f != NIHIL; f = xml_frater(f, NIHIL))
    {
        si (chorda_aequalis_literis(xml_titulus(f), "key-step"))
        {
            character* c = (character*)xar_addere(litterae);
                  f64* a = (f64*)xar_addere(alterationes);

            si (c == NIHIL || a == NIHIL)
            {
                redde;
            }
            (vacuum)xml_textus_totus(f, l->piscina, &t);
            t = _praecidere(t);
            *c = t.mensura > ZEPHYRUM ? (character)t.datum[ZEPHYRUM]
                                      : '\0';
            *a = 0.0;
        }
        alioquin si (   chorda_aequalis_literis(xml_titulus(f),
                     "key-alter")
                     && xar_numerus(alterationes) > ZEPHYRUM)
        {
            f64* a = (f64*)xar_obtinere(alterationes,
                         xar_numerus(alterationes) - I);

            (vacuum)xml_textus_totus(f, l->piscina, &t);
            si (!_decimalis(t, a))
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS, f,
                    "key-alter illegibile: ", _praecidere(t));
            }
        }
    }
    clavis->litterae = (character*)_tabula(l, litterae,
        (i32)magnitudo(character), &clavis->numerus_graduum);
    clavis->alterationes = (f64*)_tabula(l, alterationes,
        (i32)magnitudo(f64), &clavis->numerus_graduum);
}

interior vacuum
_metrum_legere (
                 Lector* l,
     constans XmlCoctus* tm,
         MusicxmlMetrum* metrum)
{
     constans XmlCoctus* f;
                    Xar* partes;

    memset(metrum, ZEPHYRUM, magnitudo(MusicxmlMetrum));
    metrum->symbolum = _attributum(tm, "symbol");
    si (xml_filius(tm, "senza-misura") != NIHIL)
    {
        metrum->liberum = VERUM;
        redde;
    }
    partes = xar_creare(l->piscina, (i32)magnitudo(MusicxmlMetriPars));
    si (partes == NIHIL)
    {
        redde;
    }
    per (f = xml_filius(tm, NIHIL); f != NIHIL; f = xml_frater(f,
        NIHIL))
    {
        chorda t;

        si (chorda_aequalis_literis(xml_titulus(f), "beats"))
        {
            MusicxmlMetriPars* p =
                (MusicxmlMetriPars*)xar_addere(partes);
                          i32 i;
                          i32 initium = ZEPHYRUM;

            si (p == NIHIL)
            {
                redde;
            }
            memset(p, ZEPHYRUM, magnitudo(MusicxmlMetriPars));
            (vacuum)xml_textus_totus(f, l->piscina, &t);
            p->pulsus = _praecidere(t);
            /* summa partium '+' (metrum additivum) */
            per (i = ZEPHYRUM; i <= p->pulsus.mensura; i++)
            {
                si (i == p->pulsus.mensura || p->pulsus.datum[i] == '+')
                {
                    s64 v;

                    si (_integer(chorda_sectio(p->pulsus, initium, i),
                            &v) && v > ZEPHYRUM)
                    {
                        p->summa += (i32)v;
                    }
                    alioquin
                    {
                        _vitium(l, MUSICXML_MONITUM,
                            MUSICXML_NUMERUS_MALUS, f,
                            "beats illegibile: ", p->pulsus);
                        frange;
                    }
                    initium = i + I;
                }
            }
        }
        alioquin si (   chorda_aequalis_literis(xml_titulus(f),
                     "beat-type")
                     && xar_numerus(partes) > ZEPHYRUM)
        {
            MusicxmlMetriPars* p = (MusicxmlMetriPars*)xar_obtinere(
                partes, xar_numerus(partes) - I);
                          s64 v;

            (vacuum)xml_textus_totus(f, l->piscina, &t);
            si (_integer(t, &v) && v > ZEPHYRUM)
            {
                p->unitas = (i32)v;
            }
            alioquin
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS, f,
                    "beat-type illegibile: ", _praecidere(t));
            }
        }
    }
    metrum->partes = (MusicxmlMetriPars*)_tabula(l, partes,
        (i32)magnitudo(MusicxmlMetriPars), &metrum->numerus_partium);
}

interior b32
_attributa_legere (
                 Lector* l,
     constans XmlCoctus* e,
                    s64  positio,
           StatusPartis* sp,
      MusicxmlAttributa* t)
{
     constans XmlCoctus* f;
                    s64  v;
                    Xar* claviculae;

    memset(t, ZEPHYRUM, magnitudo(MusicxmlAttributa));
    t->positio  = positio;
    f           = xml_filius(e, "divisions");
    si (f != NIHIL)
    {
        chorda c;

        /* illegibiles iam in transitu primo nominatae */
        si (   xml_textus_totus(f, l->piscina, &c) && _integer(c, &v)
            && v > ZEPHYRUM)
        {
            t->divisiones  = v;
            sp->factor     = l->divisiones / v;
        }
    }
    f = xml_filius(e, "key");
    si (f != NIHIL)
    {
        t->clavis_adest = VERUM;
        _clavem_legere(l, f, &t->clavis);
    }
    f = xml_filius(e, "time");
    si (f != NIHIL)
    {
        t->metrum_adest = VERUM;
        _metrum_legere(l, f, &t->metrum);
    }
    si (_integer_filii(l, e, "staves", &v) && v > ZEPHYRUM)
    {
        t->baculi = (i32)v;
    }
    claviculae = xar_creare(l->piscina,
        (i32)magnitudo(MusicxmlClavicula));
    si (claviculae == NIHIL)
    {
        redde FALSUM;
    }
    per (f = xml_filius(e, "clef"); f != NIHIL; f = xml_frater(f,
        "clef"))
    {
        MusicxmlClavicula* k =
            (MusicxmlClavicula*)xar_addere(claviculae);
                   chorda s;

        si (k == NIHIL)
        {
            redde FALSUM;
        }
        memset(k, ZEPHYRUM, magnitudo(MusicxmlClavicula));
        k->baculum = I;
        si (_integer(_attributum(f, "number"), &v) && v > ZEPHYRUM)
        {
            k->baculum = (i32)v;
        }
        si (_textus_filii(l, f, "sign", &s) && s.mensura > ZEPHYRUM)
        {
            k->signum = (character)s.datum[ZEPHYRUM];
            si (chorda_aequalis_literis(s, "percussion"))
            {
                k->signum = 'P';
            }
            alioquin si (chorda_aequalis_literis(s, "TAB"))
            {
                k->signum = 'T';
            }
            alioquin si (chorda_aequalis_literis(s, "none"))
            {
                k->signum = 'N';
            }
        }
        si (_integer_filii(l, f, "line", &v))
        {
            k->linea = (i32)v;
        }
        si (_integer_filii(l, f, "clef-octave-change", &v))
        {
            k->octava_mutata = (s32)v;
        }
    }
    t->claviculae = (MusicxmlClavicula*)_tabula(l, claviculae,
        (i32)magnitudo(MusicxmlClavicula), &t->numerus_clavicularum);
    f = xml_filius(e, "transpose");
    si (f != NIHIL)
    {
        t->transpositio_adest = VERUM;
        si (_integer_filii(l, f, "diatonic", &v))
        {
            t->transpositio.diatonica = (s32)v;
        }
        si (_integer_filii(l, f, "chromatic", &v))
        {
            t->transpositio.chromatica = (s32)v;
        }
        si (_integer_filii(l, f, "octave-change", &v))
        {
            t->transpositio.octava_mutata = (s32)v;
        }
        t->transpositio.duplicata = xml_filius(f, "double") != NIHIL;
    }
    redde VERUM;
}


/* ==================================================
 * Notae
 * ================================================== */

nomen structura {
     s64 cursor;
     s64 maximum;
     b32 prior_adest;        /* nota prior (non acordi) in mensura */
     s64 prior_positio;
  chorda prior_vox;
} StatusMensurae;

/* duratio in divisionibus operis; FALSUM si abest aut illegibilis */
interior b32
_duratio (
                 Lector* l,
     constans XmlCoctus* e,
           StatusPartis* sp,
                    s64* d)
{
    s64 v;

    si (!_integer_filii(l, e, "duration", &v))
    {
        redde FALSUM;
    }
    si (v < ZEPHYRUM)
    {
        _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS, e,
            "duration negativa", _vacua());
        redde FALSUM;
    }
    *d = v * sp->factor;
    redde VERUM;
}

interior vacuum
_ligaturam_tractare (
                 Lector* l,
           StatusPartis* sp,
     constans XmlCoctus* e,
           MusicxmlNota* n)
{
    i32 i;

    si (n->genus != MUSICXML_NOTA_SONANS)
    {
        redde;
    }
    si (n->ligatura_finis)
    {
        b32 inventa = FALSUM;

        per (i = ZEPHYRUM; i < xar_numerus(sp->ligaturae); i++)
        {
            LigaturaAperta* a = (LigaturaAperta*)xar_obtinere(
                sp->ligaturae, i);

            si (a->nota == n->nota && chorda_aequalis(a->vox, n->vox))
            {
                /* ultima in locum eius, deinde ultima tollitur */
                *a = *(LigaturaAperta*)xar_obtinere(sp->ligaturae,
                    xar_numerus(sp->ligaturae) - I);
                (vacuum)xar_removere_ultimum(sp->ligaturae);
                inventa = VERUM;
                frange;
            }
        }
        si (!inventa)
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_LIGATURA_PENDENS, e,
                "tie stop sine initio", _vacua());
        }
    }
    si (n->ligatura_initium)
    {
        LigaturaAperta* a = (LigaturaAperta*)xar_addere(sp->ligaturae);

        si (a != NIHIL)
        {
            a->vox    = n->vox;
            a->nota   = n->nota;
            a->nodus  = e;
        }
    }
}

/* VERUM = nota addenda in *n */
interior b32
_notam_legere (
                 Lector* l,
     constans XmlCoctus* e,
           StatusPartis* sp,
         StatusMensurae* sm,
           MusicxmlNota* n)
{
     constans XmlCoctus* f;
                 chorda  t;
                    s64  v;
                    s64  d = ZEPHYRUM;
                    b32  acordum;

    memset(n, ZEPHYRUM, magnitudo(MusicxmlNota));
    (vacuum)xml_locus(e, &n->linea, &n->columna);
    n->gratia  = xml_filius(e, "grace") != NIHIL;
    n->parva   = xml_filius(e, "cue") != NIHIL;
    acordum    = xml_filius(e, "chord") != NIHIL;
    (vacuum)_textus_filii(l, e, "voice", &n->vox);
    si (_integer_filii(l, e, "staff", &v) && v > ZEPHYRUM)
    {
        n->baculum = (i32)v;
    }

    f = xml_filius(e, "pitch");
    si (f != NIHIL)
    {
        n->genus = MUSICXML_NOTA_SONANS;
        si (   !_textus_filii(l, f, "step", &t) || t.mensura != I
            || _gradus((character)t.datum[ZEPHYRUM]) < ZEPHYRUM)
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, e,
                "pitch sine step legibili (pausa facta)", _vacua());
            n->genus = MUSICXML_NOTA_PAUSA;
        }
        alioquin
        {
            n->littera = (character)t.datum[ZEPHYRUM];
        }
        si (   _textus_filii(l, f, "alter", &t)
            && !_decimalis(t, &n->alteratio))
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_NUMERUS_MALUS, e,
                "alter illegibile: ", t);
            n->alteratio = 0.0;
        }
        si (n->genus == MUSICXML_NOTA_SONANS)
        {
            si (   !_integer_filii(l, f, "octave", &v) || v < ZEPHYRUM
                || v > IX)
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, e,
                    "pitch sine octave legibili (pausa facta)",
                    _vacua());
                n->genus    = MUSICXML_NOTA_PAUSA;
                n->littera  = '\0';
            }
            alioquin
            {
                n->octava = (s32)v;
            }
        }
    }
    alioquin si ((f = xml_filius(e, "rest")) != NIHIL)
    {
        n->genus = MUSICXML_NOTA_PAUSA;
        n->mensura_tota = chorda_aequalis_literis(
            _attributum(f, "measure"), "yes");
    }
    alioquin si ((f = xml_filius(e, "unpitched")) != NIHIL)
    {
        n->genus = MUSICXML_NOTA_PERCUSSA;
        si (   _textus_filii(l, f, "display-step", &t) && t.mensura == I
            && _gradus((character)t.datum[ZEPHYRUM]) >= ZEPHYRUM)
        {
            n->littera = (character)t.datum[ZEPHYRUM];
        }
        si (_integer_filii(l, f, "display-octave", &v))
        {
            n->octava = (s32)v;
        }
        si ((f = xml_filius(e, "instrument")) != NIHIL)
        {
            n->instrumentum = _attributum(f, "id");
        }
    }
    alioquin
    {
        _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, e,
            "nota sine pitch, rest aut unpitched (omissa)", _vacua());
        redde FALSUM;
    }

    si (!n->gratia)
    {
        si (!_duratio(l, e, sp, &d))
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, e,
                "nota sine duration (omissa)", _vacua());
            redde FALSUM;
        }
    }
    n->longitudo = d;

    si (n->genus != MUSICXML_NOTA_PAUSA && n->littera != '\0')
    {
        n->nota = _notam_computare(n->littera, n->alteratio, n->octava);
    }
    n->littera_sonans    = n->littera;
    n->alteratio_sonans  = n->alteratio;
    n->octava_sonans     = n->octava;

    si (   acordum && sm->prior_adest && chorda_aequalis(sm->prior_vox,
            n->vox))
    {
        n->acordum = VERUM;
        n->positio = sm->prior_positio;
    }
    alioquin
    {
        si (acordum)
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_ACORDUM_PRIMUM, e,
                "chord in nota prima vocis (nota plana facta)",
                _vacua());
        }
        n->positio         = sm->cursor;
        sm->prior_adest    = VERUM;
        sm->prior_positio  = sm->cursor;
        sm->prior_vox      = n->vox;
        sm->cursor         += d;
    }
    si (n->positio + d > sm->maximum)
    {
        sm->maximum = n->positio + d;
    }

    per (f = xml_filius(e, "tie"); f != NIHIL; f = xml_frater(f, "tie"))
    {
        t = _attributum(f, "type");
        si (chorda_aequalis_literis(t, "start"))
        {
            n->ligatura_initium = VERUM;
        }
        alioquin si (chorda_aequalis_literis(t, "stop"))
        {
            n->ligatura_finis = VERUM;
        }
    }
    _ligaturam_tractare(l, sp, e, n);
    redde VERUM;
}


/* ==================================================
 * Mensurae et partes
 * ================================================== */

interior b32
_mensuram_legere (
                 Lector* l,
     constans XmlCoctus* me,
           StatusPartis* sp,
        MusicxmlMensura* m)
{
     constans XmlCoctus* f;
         StatusMensurae  sm;
                    Xar* notae;
                    Xar* attributa;

    memset(m, ZEPHYRUM, magnitudo(MusicxmlMensura));
    memset(&sm, ZEPHYRUM, magnitudo(StatusMensurae));
    (vacuum)xml_locus(me, &m->linea, &m->columna);
    m->numerus    = _attributum(me, "number");
    m->implicita  = chorda_aequalis_literis(_attributum(me, "implicit"),
                        "yes");
    notae      = xar_creare(l->piscina, (i32)magnitudo(MusicxmlNota));
    attributa  = xar_creare(l->piscina,
                     (i32)magnitudo(MusicxmlAttributa));
    si (notae == NIHIL || attributa == NIHIL)
    {
        redde FALSUM;
    }
    per (f = xml_filius(me, NIHIL); f != NIHIL; f = xml_frater(f,
        NIHIL))
    {
        chorda titulus = xml_titulus(f);
           s64 d;

        si (chorda_aequalis_literis(titulus, "note"))
        {
            MusicxmlNota n;

            si (_notam_legere(l, f, sp, &sm, &n))
            {
                MusicxmlNota* x = (MusicxmlNota*)xar_addere(notae);

                si (x == NIHIL)
                {
                    redde FALSUM;
                }
                *x = n;
            }
        }
        alioquin si (chorda_aequalis_literis(titulus, "backup"))
        {
            si (!_duratio(l, f, sp, &d))
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, f,
                    "backup sine duration (praeteritum)", _vacua());
                perge;
            }
            sm.cursor -= d;
            si (sm.cursor < ZEPHYRUM)
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_REGRESSUS_NIMIUS,
                    f,
                    "backup ante initium mensurae (coercitum)",
                    _vacua());
                sm.cursor = ZEPHYRUM;
            }
            sm.prior_adest = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(titulus, "forward"))
        {
            si (!_duratio(l, f, sp, &d))
            {
                _vitium(l, MUSICXML_MONITUM, MUSICXML_FILIUS_DEEST, f,
                    "forward sine duration (praeteritum)", _vacua());
                perge;
            }
            sm.cursor += d;
            si (sm.cursor > sm.maximum)
            {
                sm.maximum = sm.cursor;
            }
            sm.prior_adest = FALSUM;
        }
        alioquin si (chorda_aequalis_literis(titulus, "attributes"))
        {
            MusicxmlAttributa* t = (MusicxmlAttributa*)xar_addere(
                attributa);

            si (   t == NIHIL
                || !_attributa_legere(l, f, sm.cursor, sp, t))
            {
                redde FALSUM;
            }
        }
        /* cetera (direction, barline, print, harmony ...) plan P5 */
    }
    m->longitudo  = sm.maximum;
    m->notae      = (MusicxmlNota*)_tabula(l, notae,
                        (i32)magnitudo(MusicxmlNota),
                        &m->numerus_notarum);
    m->attributa  = (MusicxmlAttributa*)_tabula(l, attributa,
                        (i32)magnitudo(MusicxmlAttributa),
                        &m->numerus_attributorum);
    redde VERUM;
}

interior b32
_partem_legere (
                 Lector* l,
     constans XmlCoctus* pe,
           MusicxmlPars* p)
{
     constans XmlCoctus* me;
           StatusPartis  sp;
                    Xar* mensurae;
                    i32  i;

    sp.factor     = l->divisiones;
    sp.ligaturae  = xar_creare(l->piscina,
                        (i32)magnitudo(LigaturaAperta));
    mensurae      = xar_creare(l->piscina,
                        (i32)magnitudo(MusicxmlMensura));
    si (sp.ligaturae == NIHIL || mensurae == NIHIL)
    {
        redde FALSUM;
    }
    per (me = xml_filius(pe, "measure"); me != NIHIL;
         me = xml_frater(me, "measure"))
    {
        MusicxmlMensura* m = (MusicxmlMensura*)xar_addere(mensurae);

        si (m == NIHIL || !_mensuram_legere(l, me, &sp, m))
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(sp.ligaturae); i++)
    {
        LigaturaAperta* a = (LigaturaAperta*)xar_obtinere(sp.ligaturae,
            i);

        _vitium(l, MUSICXML_MONITUM, MUSICXML_LIGATURA_PENDENS,
            a->nodus,
            "tie start sine fine", _vacua());
    }
    p->mensurae = (MusicxmlMensura*)_tabula(l, mensurae,
        (i32)magnitudo(MusicxmlMensura), &p->numerus_mensurarum);
    redde VERUM;
}

/* index partium (part-list): MusicxmlPars sine mensuris, ordine indicis */
nomen structura {
          MusicxmlPars  pars;
    constans XmlCoctus* nodus;
} IndexPartis;

interior b32
_indicem_legere (
                 Lector* l,
     constans XmlCoctus* radix,
                    Xar* index)
{
    constans XmlCoctus* lista = xml_filius(radix, "part-list");
    constans XmlCoctus* sp;

    si (lista == NIHIL)
    {
        redde VERUM;
    }
    per (sp = xml_filius(lista, "score-part"); sp != NIHIL;
         sp = xml_frater(sp, "score-part"))
    {
                 IndexPartis* x = (IndexPartis*)xar_addere(index);
          constans XmlCoctus* f;
                         Xar* instrumenta;

        si (x == NIHIL)
        {
            redde FALSUM;
        }
        memset(x, ZEPHYRUM, magnitudo(IndexPartis));
        x->nodus    = sp;
        x->pars.id  = _attributum(sp, "id");
        (vacuum)_textus_filii(l, sp, "part-name", &x->pars.titulus);
        (vacuum)_textus_filii(l, sp, "part-abbreviation",
            &x->pars.abbreviatio);
        instrumenta = xar_creare(l->piscina,
            (i32)magnitudo(MusicxmlInstrumentum));
        si (instrumenta == NIHIL)
        {
            redde FALSUM;
        }
        per (f = xml_filius(sp, "score-instrument"); f != NIHIL;
             f = xml_frater(f, "score-instrument"))
        {
            MusicxmlInstrumentum* s = (MusicxmlInstrumentum*)xar_addere(
                instrumenta);

            si (s == NIHIL)
            {
                redde FALSUM;
            }
            memset(s, ZEPHYRUM, magnitudo(MusicxmlInstrumentum));
            s->id       = _attributum(f, "id");
            s->volumen  = -1.0;
            (vacuum)_textus_filii(l, f, "instrument-name", &s->titulus);
        }
        per (f = xml_filius(sp, "midi-instrument"); f != NIHIL;
             f = xml_frater(f, "midi-instrument"))
        {
            MusicxmlInstrumentum* s   = NIHIL;
                          chorda  id  = _attributum(f, "id");
                          chorda  t;
                             s64  v;
                             i32  i;

            per (i = ZEPHYRUM; i < xar_numerus(instrumenta); i++)
            {
                MusicxmlInstrumentum* q = (MusicxmlInstrumentum*)
                    xar_obtinere(instrumenta, i);

                si (chorda_aequalis(q->id, id))
                {
                    s = q;
                    frange;
                }
            }
            si (s == NIHIL)
            {
                s = (MusicxmlInstrumentum*)xar_addere(instrumenta);
                si (s == NIHIL)
                {
                    redde FALSUM;
                }
                memset(s, ZEPHYRUM, magnitudo(MusicxmlInstrumentum));
                s->id       = id;
                s->volumen  = -1.0;
            }
            si (_integer_filii(l, f, "midi-channel", &v))
            {
                s->canalis = (i32)v;
            }
            si (_integer_filii(l, f, "midi-program", &v))
            {
                s->programma = (i32)v;
            }
            si (_integer_filii(l, f, "midi-unpitched", &v))
            {
                s->percussio = (i32)v;
            }
            si (   _textus_filii(l, f, "volume", &t)
                && !_decimalis(t, &s->volumen))
            {
                s->volumen = -1.0;
            }
            si (   _textus_filii(l, f, "pan", &t)
                && !_decimalis(t, &s->positio))
            {
                s->positio = 0.0;
            }
        }
        x->pars.instrumenta = (MusicxmlInstrumentum*)_tabula(l,
            instrumenta, (i32)magnitudo(MusicxmlInstrumentum),
            &x->pars.numerus_instrumentorum);
    }
    redde VERUM;
}

/* partes ordine indicis; <part> sine indice post eas, ordine
 * documenti (M: index partium discors - monitum utrimque) */
interior b32
_partes_legere (
                 Lector* l,
     constans XmlCoctus* radix,
           MusicxmlOpus* opus)
{
                    Xar* index;
                    Xar* partes;
     constans XmlCoctus* pe;
                    i32  numerus_elementorum;
                    b32* usa;
                    i32  i;
                    i32  k;

    index   = xar_creare(l->piscina, (i32)magnitudo(IndexPartis));
    partes  = xar_creare(l->piscina, (i32)magnitudo(MusicxmlPars));
    si (   index == NIHIL || partes == NIHIL
        || !_indicem_legere(l, radix, index))
    {
        redde FALSUM;
    }
    numerus_elementorum = xml_filiorum_numerus(radix, "part");
    usa = (b32*)piscina_allocare(l->piscina,
        (memoriae_index)((numerus_elementorum
            + I) * (i32)magnitudo(b32)));
    si (usa == NIHIL)
    {
        redde FALSUM;
    }
    memset(usa, ZEPHYRUM,
        (size_t)(numerus_elementorum + I) * magnitudo(b32));

    per (i = ZEPHYRUM; i < xar_numerus(index); i++)
    {
         IndexPartis* x        = (IndexPartis*)xar_obtinere(index, i);
                 b32  inventa  = FALSUM;

        per (pe = xml_filius(radix, "part"), k = ZEPHYRUM; pe != NIHIL;
             pe = xml_frater(pe, "part"), k++)
        {
            si (   !usa[k] && chorda_aequalis(_attributum(pe, "id"),
                    x->pars.id))
            {
                MusicxmlPars* p = (MusicxmlPars*)xar_addere(partes);

                si (p == NIHIL)
                {
                    redde FALSUM;
                }
                *p       = x->pars;
                usa[k]   = VERUM;
                inventa  = VERUM;
                si (!_partem_legere(l, pe, p))
                {
                    redde FALSUM;
                }
                frange;
            }
        }
        si (!inventa)
        {
            _vitium(l, MUSICXML_MONITUM, MUSICXML_PARS_DISCORS,
                x->nodus,
                "pars in indice sine <part> (omissa): ", x->pars.id);
        }
    }
    per (pe = xml_filius(radix, "part"), k = ZEPHYRUM; pe != NIHIL;
         pe = xml_frater(pe, "part"), k++)
    {
        MusicxmlPars* p;

        si (usa[k])
        {
            perge;
        }
        p = (MusicxmlPars*)xar_addere(partes);
        si (p == NIHIL)
        {
            redde FALSUM;
        }
        memset(p, ZEPHYRUM, magnitudo(MusicxmlPars));
        p->id       = _attributum(pe, "id");
        p->titulus  = p->id;
        _vitium(l, MUSICXML_MONITUM, MUSICXML_PARS_DISCORS, pe,
            "<part> sine indice (servata): ", p->id);
        si (!_partem_legere(l, pe, p))
        {
            redde FALSUM;
        }
    }
    opus->partes = (MusicxmlPars*)_tabula(l, partes,
        (i32)magnitudo(MusicxmlPars), &opus->numerus_partium);
    redde VERUM;
}

/* M15: initium[k+1] = initium[k] + maxima longitudo mensurae k */
interior b32
_lineam_temporis_ponere (
           Lector* l,
     MusicxmlOpus* opus)
{
    i32 n = ZEPHYRUM;
    i32 p;
    i32 k;
    s64 initium = ZEPHYRUM;

    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
        si (opus->partes[p].numerus_mensurarum > n)
        {
            n = opus->partes[p].numerus_mensurarum;
        }
    }
    si (n == ZEPHYRUM)
    {
        redde VERUM;
    }
    opus->initia = (s64*)piscina_allocare(l->piscina,
        (memoriae_index)(n * (i32)magnitudo(s64)));
    si (opus->initia == NIHIL)
    {
        redde FALSUM;
    }
    opus->numerus_mensurarum = n;
    per (k = ZEPHYRUM; k < n; k++)
    {
        s64 maxima = ZEPHYRUM;

        per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
        {
            si (   k < opus->partes[p].numerus_mensurarum
                && opus->partes[p].mensurae[k].longitudo > maxima)
            {
                maxima = opus->partes[p].mensurae[k].longitudo;
            }
        }
        opus->initia[k] = initium;
        per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
        {
            MusicxmlMensura* m;

            si (k >= opus->partes[p].numerus_mensurarum)
            {
                perge;
            }
            m           = &opus->partes[p].mensurae[k];
            m->initium  = initium;
            si (m->longitudo < maxima)
            {
                ChordaAedificator* a = chorda_aedificator_creare(
                    l->piscina, LXIV);

                si (a == NIHIL)
                {
                    redde FALSUM;
                }
                chorda_aedificator_appendere_chorda(a,
                    opus->partes[p].id);
                chorda_aedificator_appendere_literis(a, ", mensura ");
                chorda_aedificator_appendere_chorda(a, m->numerus);
                _vitium_locatum(l, MUSICXML_MONITUM,
                    MUSICXML_MENSURA_BREVIOR, m->linea, m->columna,
                    "mensura brevior quam partium ceterarum (tempore "
                    "pausae impleta): pars ",
                    chorda_aedificator_finire(a));
            }
        }
        initium += maxima;
    }
    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
        si (opus->partes[p].numerus_mensurarum < n)
        {
            _vitium_locatum(l, MUSICXML_MONITUM,
                MUSICXML_MENSURA_BREVIOR, ZEPHYRUM, ZEPHYRUM,
                "pars mensuris paucioribus quam ceterae: ",
                opus->partes[p].id);
        }
    }
    opus->longitudo = initium;
    redde VERUM;
}


/* ==================================================
 * Documentum
 * ================================================== */

interior vacuum
_identificationem_legere (
                 Lector* l,
     constans XmlCoctus* radix,
           MusicxmlOpus* opus)
{
    constans XmlCoctus* f;
    constans XmlCoctus* c;

    f = xml_filius(radix, "work");
    si (f != NIHIL)
    {
        (vacuum)_textus_filii(l, f, "work-title",
            &opus->titulus_operis);
        (vacuum)_textus_filii(l, f, "work-number",
            &opus->numerus_operis);
    }
    (vacuum)_textus_filii(l, radix, "movement-title",
        &opus->titulus_motus);
    (vacuum)_textus_filii(l, radix, "movement-number",
        &opus->numerus_motus);
    f = xml_filius(radix, "identification");
    si (f == NIHIL)
    {
        redde;
    }
    per (c = xml_filius(f, "creator"); c != NIHIL;
         c = xml_frater(c, "creator"))
    {
        chorda genus = _attributum(c, "type");
        chorda t;

        si (!xml_textus_totus(c, l->piscina, &t))
        {
            perge;
        }
        t = _praecidere(t);
        si (   chorda_aequalis_literis(genus, "composer")
            && opus->compositor.mensura == ZEPHYRUM)
        {
            opus->compositor = t;
        }
        alioquin si (   (chorda_aequalis_literis(genus, "lyricist")
                      || chorda_aequalis_literis(genus, "poet"))
                     && opus->poeta.mensura == ZEPHYRUM)
        {
            opus->poeta = t;
        }
    }
    (vacuum)_textus_filii(l, f, "rights", &opus->ius);
}

interior b32
_documentum_legere (
          Lector* l,
          chorda  datum,
    MusicxmlOpus* opus)
{
           XmlDocumentum* d = xml_documentum_legere(datum, l->piscina);
      constans XmlCoctus* radix;
                  chorda  titulus;

    si (d == NIHIL)
    {
        redde FALSUM;
    }
    si (!xml_documentum_successus(d))
    {
        Xar* dg = xml_documentum_diagnostica(d);
        constans MateriaDiagnosticum* m = NIHIL;

        si (dg != NIHIL && xar_numerus(dg) > ZEPHYRUM)
        {
            m = (constans MateriaDiagnosticum*)xar_obtinere(dg,
                ZEPHYRUM);
        }
        _vitium_locatum(l, MUSICXML_VITIUM, MUSICXML_XML_MALUM,
            m != NIHIL && m->tractus.initium >= ZEPHYRUM
                ? m->tractus.linea : ZEPHYRUM,
            m != NIHIL && m->tractus.initium >= ZEPHYRUM
                ? m->tractus.columna : ZEPHYRUM,
            "XML non bene formatum: ",
            m != NIHIL && m->causa != NIHIL
                ? chorda_ex_literis(m->causa, l->piscina)
                : chorda_ex_literis("documentum vacuum", l->piscina));
        redde VERUM;
    }
    radix    = xml_documentum_radix(d);
    titulus  = xml_titulus(radix);
    si (chorda_aequalis_literis(titulus, "score-partwise"))
    {
        opus->forma = MUSICXML_FORMA_PARTIUM;
    }
    alioquin si (chorda_aequalis_literis(titulus, "opus"))
    {
        _vitium(l, MUSICXML_VITIUM, MUSICXML_OPUS_RADIX, radix,
            "radix <opus> (collectio partiturarum) non legitur",
            _vacua());
        redde VERUM;
    }
    alioquin
    {
        /* score-timewise: plan P5 */
        _vitium(l, MUSICXML_VITIUM, MUSICXML_NON_MUSICXML, radix,
            "radix non score-partwise: ", titulus);
        redde VERUM;
    }
    opus->versio = _attributum(radix, "version");
    _identificationem_legere(l, radix, opus);
    si (!_divisiones_colligere(l, radix))
    {
        redde VERUM;
    }
    opus->divisiones = l->divisiones;
    si (!_partes_legere(l, radix, opus))
    {
        redde FALSUM;
    }
    si (opus->numerus_partium == ZEPHYRUM)
    {
        _vitium(l, MUSICXML_VITIUM, MUSICXML_SINE_PARTIBUS, radix,
            "partitura sine partibus", _vacua());
        redde VERUM;
    }
    redde _lineam_temporis_ponere(l, opus);
}


/* ==================================================
 * API
 * ================================================== */

MusicxmlOpus*
musicxml_legere (
      chorda  datum,
     Piscina* piscina)
{
          Lector  l;
    MusicxmlOpus* opus;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    opus = (MusicxmlOpus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(MusicxmlOpus));
    si (opus == NIHIL)
    {
        redde NIHIL;
    }
    memset(opus, ZEPHYRUM, magnitudo(MusicxmlOpus));
    l.piscina     = piscina;
    l.divisiones  = I;
    l.vitia       = xar_creare(piscina, (i32)magnitudo(MusicxmlVitium));
    si (l.vitia == NIHIL)
    {
        redde NIHIL;
    }
    opus->divisiones = I;
    si (   datum.mensura    >= IV && datum.datum[ZEPHYRUM] == 'P'
        && datum.datum[I]   == 'K' && datum.datum[II] == III
        && datum.datum[III] == IV)
    {
        /* arca .mxl: plan P3 */
        _vitium_locatum(&l, MUSICXML_VITIUM, MUSICXML_ARCA_MALA,
            ZEPHYRUM, ZEPHYRUM, "arca .mxl nondum legitur", _vacua());
    }
    alioquin si (!_documentum_legere(&l, datum, opus))
    {
        redde NIHIL;
    }
    opus->vitia = (MusicxmlVitium*)_tabula(&l, l.vitia,
        (i32)magnitudo(MusicxmlVitium), &opus->numerus_vitiorum);
    redde opus;
}

b32
musicxml_successus (
    constans MusicxmlOpus* opus)
{
    i32 i;

    si (opus == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < opus->numerus_vitiorum; i++)
    {
        si (opus->vitia[i].gravitas == MUSICXML_VITIUM)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_s64_scribere (
    ChordaAedificator* a,
                  s64  n)
{
    character cifrae[XXIV];
          i32 k      = ZEPHYRUM;
          b32 minus  = n < ZEPHYRUM;

    si (minus)
    {
        n = -n;
    }
    fac
    {
        cifrae[k++]  = (character)('0' + (integer)(n % X));
        n            /= X;
    }
    dum (n > ZEPHYRUM && k < XXIII);
    si (minus)
    {
        chorda_aedificator_appendere_character(a, '-');
    }
    dum (k > ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, cifrae[--k]);
    }
}

interior vacuum
_vitia_lineis (
              ChordaAedificator* a,
        constans MusicxmlVitium* vitia,
                            i32  numerus,
             constans character* via)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(a, via);
        chorda_aedificator_appendere_character(a, ':');
        si (vitia[i].linea > ZEPHYRUM)
        {
            chorda_aedificator_appendere_i32(a, vitia[i].linea);
            chorda_aedificator_appendere_character(a, ':');
            chorda_aedificator_appendere_i32(a, vitia[i].columna);
            chorda_aedificator_appendere_character(a, ':');
        }
        chorda_aedificator_appendere_literis(a,
            vitia[i].gravitas == MUSICXML_VITIUM ? " vitium: "
                                                 : " monitum: ");
        chorda_aedificator_appendere_chorda(a, vitia[i].causa);
        chorda_aedificator_appendere_character(a, '\n');
    }
}

b32
musicxml_vitia_scribere (
    constans MusicxmlOpus* opus,
    constans MusicxmlOrdo* ordo,
       constans character* via,
        ChordaAedificator* exitus)
{
    si (opus == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    _vitia_lineis(exitus, opus->vitia, opus->numerus_vitiorum,
        via != NIHIL ? via : "");
    si (ordo != NIHIL)
    {
        _vitia_lineis(exitus, ordo->vitia, ordo->numerus_vitiorum,
            via != NIHIL ? via : "");
    }
    redde VERUM;
}


/* ==================================================
 * Tabula canonica (oraculum)
 * ================================================== */

nomen structura {
                      i32  pars;
                      s64  initium;
                      s64  longitudo;
    constans MusicxmlNota* nota;
                      b32  aperta;      /* ligatura initium pendens */
} LineaTabulae;

interior integer
_lineas_conferre (
    constans vacuum* a,
    constans vacuum* b)
{
    constans LineaTabulae* x = (constans LineaTabulae*)a;
    constans LineaTabulae* y = (constans LineaTabulae*)b;

    si (x->pars != y->pars)
    {
        redde x->pars < y->pars ? -I : I;
    }
    si (x->initium != y->initium)
    {
        redde x->initium < y->initium ? -I : I;
    }
    si (x->nota->nota != y->nota->nota)
    {
        redde x->nota->nota < y->nota->nota ? -I : I;
    }
    redde ZEPHYRUM;
}

/* quartae ut fractio reducta: "3/2", "1", "0" */
interior vacuum
_quartas_scribere (
    ChordaAedificator* a,
                  s64  v,
                  s64  divisiones)
{
    s64 g = _divisor_communis(v, divisiones);

    si (v == ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(a, '0');
        redde;
    }
    _s64_scribere(a, v / g);
    si (divisiones / g != I)
    {
        chorda_aedificator_appendere_character(a, '/');
        _s64_scribere(a, divisiones / g);
    }
}

/* alteratio: integra sine puncto, aliter decimalis sine zephyris */
interior vacuum
_alterationem_scribere (
    ChordaAedificator* a,
                  f64  x)
{
          s64 scala = (s64)X * X * X * X;
          s64 v;
          s64 fractio;
          s32 k;
    character cifrae[V];

    si (x < 0.0)
    {
        chorda_aedificator_appendere_character(a, '-');
        x = -x;
    }
    v = (s64)(x * (f64)scala + 0.5);
    _s64_scribere(a, v / scala);
    fractio = v % scala;
    si (fractio == ZEPHYRUM)
    {
        redde;
    }
    per (k = III; k >= ZEPHYRUM; k--)
    {
        cifrae[k]  = (character)('0' + (integer)(fractio % X));
        fractio    /= X;
    }
    k = IV;
    dum (k > ZEPHYRUM && cifrae[k - I] == '0')
    {
        k--;
    }
    chorda_aedificator_appendere_character(a, '.');
    per (fractio = ZEPHYRUM; fractio < (s64)k; fractio++)
    {
        chorda_aedificator_appendere_character(a, cifrae[fractio]);
    }
}

interior constans character*
_locus_syllabae (
    MusicxmlLocusSyllabae locus)
{
    commutatio (locus)
    {
        casus MUSICXML_SYLLABA_SINGULA: redde "single";
        casus MUSICXML_SYLLABA_INITIUM: redde "begin";
        casus MUSICXML_SYLLABA_MEDIA:   redde "middle";
        casus MUSICXML_SYLLABA_FINIS:   redde "end";
        ordinarius:                     redde "?";
    }
}

interior vacuum
_textum_tabulae (
    ChordaAedificator* a,
               chorda  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c = (character)t.datum[i];

        chorda_aedificator_appendere_character(a,
            (c == '\t' || c == '\n' || c == '\r'
                || c == '|') ? ' ' : c);
    }
}

b32
musicxml_tabulam_scribere (
    constans MusicxmlOpus* opus,
    constans MusicxmlOrdo* ordo,
        ChordaAedificator* exitus)
{
             Piscina* piscina;
                 Xar* lineae;
        LineaTabulae* tabula;
                 i32  numerus;
                 i32  p;
                 i32  v;
                 i32  i;
                 i32  numerus_visitationum;

    si (opus == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    piscina = piscina_generare_dynamicum("musicxml_tabula",
        (memoriae_index)(IV * M));
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    lineae = xar_creare(piscina, (i32)magnitudo(LineaTabulae));
    si (lineae == NIHIL)
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }
    numerus_visitationum = ordo != NIHIL ? ordo->numerus
                                         : opus->numerus_mensurarum;
    per (p = ZEPHYRUM; p < opus->numerus_partium; p++)
    {
         constans MusicxmlPars* pars   = &opus->partes[p];
                           i32  prima  = xar_numerus(lineae);

        per (v = ZEPHYRUM; v < numerus_visitationum; v++)
        {
            i32 k       = ordo
                != NIHIL ? ordo->visitationes[v].mensura : v;
            s64 initium = ordo != NIHIL ? ordo->visitationes[v].initium
                                        : opus->initia[v];
            constans MusicxmlMensura* m;

            si (k < ZEPHYRUM || k >= pars->numerus_mensurarum)
            {
                perge;
            }
            m = &pars->mensurae[k];
            per (i = ZEPHYRUM; i < m->numerus_notarum; i++)
            {
                 constans MusicxmlNota* n      = &m->notae[i];
                                   s64  ortus  = initium + n->positio;
                          LineaTabulae* x      = NIHIL;
                                   i32  j;

                si (n->genus == MUSICXML_NOTA_PAUSA)
                {
                    perge;
                }
                si (n->ligatura_finis)
                {
                    per (j = prima; j < xar_numerus(lineae); j++)
                    {
                        LineaTabulae* q = (LineaTabulae*)xar_obtinere(
                            lineae, j);

                        si (   q->aperta && q->nota->nota == n->nota
                            && chorda_aequalis(q->nota->vox, n->vox)
                            && q->initium + q->longitudo == ortus)
                        {
                            x = q;
                            frange;
                        }
                    }
                }
                si (x != NIHIL)
                {
                    x->longitudo  += n->longitudo;
                    x->aperta     = n->ligatura_initium;
                    perge;
                }
                x = (LineaTabulae*)xar_addere(lineae);
                si (x == NIHIL)
                {
                    piscina_destruere(piscina);
                    redde FALSUM;
                }
                x->pars       = p;
                x->initium    = ortus;
                x->longitudo  = n->longitudo;
                x->nota       = n;
                x->aperta     = n->ligatura_initium;
            }
        }
    }
    tabula = (LineaTabulae*)piscina_allocare(piscina,
        (memoriae_index)((xar_numerus(lineae) + I)
            * (i32)magnitudo(LineaTabulae)));
    si (tabula == NIHIL)
    {
        piscina_destruere(piscina);
        redde FALSUM;
    }
    numerus = xar_copiare_ad_tabulam(lineae, tabula, ZEPHYRUM,
        xar_numerus(lineae));
    qsort(tabula, (size_t)numerus, magnitudo(LineaTabulae),
        _lineas_conferre);

    chorda_aedificator_appendere_literis(exitus,
        "DATA musicxml tabula 1\n"
        "# pars\tinitium\tlongitudo\tmidi\tlittera\talteratio\tgratia"
        "\tversus\n");
    per (i = ZEPHYRUM; i < numerus; i++)
    {
         constans LineaTabulae* x = &tabula[i];
         constans MusicxmlNota* n = x->nota;
                           i32  s;

        chorda_aedificator_appendere_chorda(exitus,
            opus->partes[x->pars].id);
        chorda_aedificator_appendere_character(exitus, '\t');
        _quartas_scribere(exitus, x->initium, opus->divisiones);
        chorda_aedificator_appendere_character(exitus, '\t');
        _quartas_scribere(exitus, x->longitudo, opus->divisiones);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_s32(exitus, n->nota);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_character(exitus,
            n->littera_sonans != '\0' ? n->littera_sonans : '-');
        chorda_aedificator_appendere_character(exitus, '\t');
        _alterationem_scribere(exitus, n->alteratio_sonans);
        chorda_aedificator_appendere_character(exitus, '\t');
        chorda_aedificator_appendere_character(exitus,
            n->gratia ? '1' : '0');
        chorda_aedificator_appendere_character(exitus, '\t');
        si (n->numerus_syllabarum == ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(exitus, '-');
        }
        per (s = ZEPHYRUM; s < n->numerus_syllabarum; s++)
        {
            constans MusicxmlSyllaba* y = &n->syllabae[s];

            si (s > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(exitus, '|');
            }
            chorda_aedificator_appendere_i32(exitus, y->versus);
            chorda_aedificator_appendere_character(exitus, ':');
            chorda_aedificator_appendere_literis(exitus,
                _locus_syllabae(y->locus));
            chorda_aedificator_appendere_character(exitus, ':');
            _textum_tabulae(exitus, y->textus);
        }
        chorda_aedificator_appendere_character(exitus, '\n');
    }
    piscina_destruere(piscina);
    redde VERUM;
}
