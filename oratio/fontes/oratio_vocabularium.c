/* oratio_vocabularium.c - Vide oratio_vocabularium.h. */

#include "oratio_vocabularium.h"
#include "nuntium.h"
#include "friatio.h"
#include <string.h>


/* ==================================================
 * Partes orationis
 * ================================================== */

hic_manens constans character* TITULI_PARTIUM[] = {
    "X", "N", "PRON", "PACK", "ADJ", "NUM", "ADV", "V", "VPAR",
    "SUPINE", "PREP", "CONJ", "INTERJ", "TACKON", "PREFIX", "SUFFIX"
};

OratioPars
oratio_pars_ex_titulo (
    chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < (i32)ORATIO_PARS_NUMERUS_PARTIUM; i++)
    {
        i32 l = (i32)strlen(TITULI_PARTIUM[i]);

        si (   titulus.mensura == l
            && memcmp(titulus.datum, TITULI_PARTIUM[i], (size_t)l)
                == ZEPHYRUM)
        {
            redde (OratioPars)i;
        }
    }
    redde ORATIO_PARS_NUMERUS_PARTIUM;
}

constans character*
oratio_pars_titulus (
    OratioPars pars)
{
    si ((i32)pars >= (i32)ORATIO_PARS_NUMERUS_PARTIUM)
    {
        redde "?";
    }
    redde TITULI_PARTIUM[pars];
}

/* partes cum declinatione et variante in recordis */
interior b32
_declinata (
    OratioPars pars)
{
    redde (b32)(pars == ORATIO_PARS_N || pars == ORATIO_PARS_PRON
        || pars == ORATIO_PARS_PACK || pars == ORATIO_PARS_ADJ
        || pars == ORATIO_PARS_NUM || pars == ORATIO_PARS_V
        || pars == ORATIO_PARS_VPAR || pars == ORATIO_PARS_SUPINE);
}


/* ==================================================
 * Chordae, lineae, lexemata
 * ================================================== */

interior chorda
_chorda (
     i8* datum,
    i32  mensura)
{
    chorda c;

    c.datum    = datum;
    c.mensura  = mensura;
    redde c;
}

/* literae constantes ut chorda: indicator constans per unionem lavatur
 * (chorda datum non constans fert; nihil hic scribitur) */
interior chorda
_ex_literis (
    constans character* literae)
{
    unio {
        constans character* c;
                        i8* m;
    } u;

    u.c = literae;
    redde _chorda(u.m, (i32)strlen(literae));
}

interior chorda
_vacua (vacuum)
{
    redde _chorda(NIHIL, ZEPHYRUM);
}

interior b32
_spatium (
    i8 c)
{
    redde (b32)(c == ' ' || c == '\t');
}

interior chorda
_tondere (
    chorda c)
{
    i32 a = ZEPHYRUM;
    i32 b = c.mensura;

    dum (a < b && _spatium(c.datum[a]))
    {
        a = a + I;
    }
    dum (b > a && _spatium(c.datum[b - I]))
    {
        b = b - I;
    }
    redde _chorda(c.datum + a, b - a);
}

interior chorda
_tondere_dextra (
    chorda c)
{
    i32 b = c.mensura;

    dum (b > ZEPHYRUM && _spatium(c.datum[b - I]))
    {
        b = b - I;
    }
    redde _chorda(c.datum, b);
}

/* linea proxima [cursor, '\n'), '\r' finali amoto; FALSUM in fine */
interior b32
_linea_proxima (
    chorda  fons,
       i32* cursor,
       i32* numerus,
    chorda* linea)
{
    i32 a = *cursor;
    i32 b;

    si (a >= fons.mensura)
    {
        redde FALSUM;
    }
    b = a;
    dum (b < fons.mensura && fons.datum[b] != '\n')
    {
        b = b + I;
    }
    *cursor   = b < fons.mensura ? b + I : b;
    *numerus  = *numerus + I;
    si (b > a && fons.datum[b - I] == '\r')
    {
        b = b - I;
    }
    *linea = _chorda(fons.datum + a, b - a);
    redde VERUM;
}

/* lexemata spatiis separata in Xar de chorda */
interior b32
_lexemata (
    chorda  linea,
       Xar* exitus)
{
    i32 i = ZEPHYRUM;

    dum (i < linea.mensura)
    {
        i32 a;

        dum (i < linea.mensura && _spatium(linea.datum[i]))
        {
            i = i + I;
        }
        si (i >= linea.mensura)
        {
            frange;
        }
        a = i;
        dum (i < linea.mensura && !_spatium(linea.datum[i]))
        {
            i = i + I;
        }
        {
            chorda* locus = (chorda*)xar_addere(exitus);

            si (locus == NIHIL)
            {
                redde FALSUM;
            }
            *locus = _chorda(linea.datum + a, i - a);
        }
    }
    redde VERUM;
}

interior chorda
_lexema (
     Xar* lexemata,
     i32  i)
{
    redde *(chorda*)xar_obtinere(lexemata, i);
}

/* lexemata [ab, ad) spatio uno iuncta (copia in piscina) */
interior chorda
_iungere (
     Piscina* piscina,
         Xar* lexemata,
         i32  ab,
         i32  ad)
{
    i32  mensura = ZEPHYRUM;
    i32  k;
     i8* datum;
    i32  o = ZEPHYRUM;

    si (ad <= ab)
    {
        redde _vacua();
    }
    per (k = ab; k < ad; k++)
    {
        mensura = mensura + _lexema(lexemata, k).mensura + I;
    }
    datum = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (datum == NIHIL)
    {
        redde _vacua();
    }
    per (k = ab; k < ad; k++)
    {
        chorda l = _lexema(lexemata, k);

        si (k > ab)
        {
            datum[o]  = ' ';
            o         = o + I;
        }
        memcpy(datum + o, l.datum, (size_t)l.mensura);
        o = o + l.mensura;
    }
    redde _chorda(datum, o);
}

interior b32
_numerus_ex (
    chorda  c,
       i32* exitus)
{
    i32 i;
    i32 v = ZEPHYRUM;

    si (c.mensura == ZEPHYRUM || c.mensura > (i32)VI)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        i8 d = c.datum[i];

        si (d < '0' || d > '9')
        {
            redde FALSUM;
        }
        v = v * (i32)X + (i32)(d - '0');
    }
    *exitus = v;
    redde VERUM;
}

interior b32
_littera_una (
    chorda c)
{
    redde (b32)(c.mensura == I && c.datum[ZEPHYRUM] >= 'A'
        && c.datum[ZEPHYRUM] <= 'Z');
}

interior vacuum
_vitium (
    OratioVocabulariumVitium* v,
          constans character* plagula,
                         i32  linea,
                         s32  offset,
          constans character* causa)
{
    si (v != NIHIL)
    {
        v->plagula  = plagula;
        v->linea    = linea;
        v->offset   = offset;
        v->causa    = causa;
    }
}


/* ==================================================
 * DICTLINE.GEN - columnae fixae (FONTES.md)
 * ================================================== */

#define DL_STIRPS_MENSURA   18
#define DL_PARS_AB          76
#define DL_PARS_AD          100
#define DL_TRAN_AB          100
#define DL_SENSUS_AB        110

interior b32
_dictline_legere (
                     Piscina* piscina,
                      chorda  fons,
                         Xar* stirpes,
    OratioVocabulariumVitium* vitium)
{
       i32  cursor   = ZEPHYRUM;
       i32  numerus  = ZEPHYRUM;
    chorda  linea;
       Xar* lexemata = xar_creare(piscina, (i32)magnitudo(chorda));

    si (lexemata == NIHIL)
    {
        redde FALSUM;
    }
    dum (_linea_proxima(fons, &cursor, &numerus, &linea))
    {
        OratioStirps r;
                 i32 k;
                 i32 n;

        si (linea.mensura == ZEPHYRUM && cursor >= fons.mensura)
        {
            frange;   /* linea nova finalis */
        }
        si (linea.mensura < (i32)DL_SENSUS_AB)
        {
            _vitium(vitium, "DICTLINE.GEN", numerus, ZEPHYRUM,
                "linea brevior CX columnis");
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < (i32)IV; k++)
        {
            si (linea.datum[(k + I) * (DL_STIRPS_MENSURA + I) - I]
                != ' ')
            {
                _vitium(vitium, "DICTLINE.GEN", numerus, ZEPHYRUM,
                    "spatium post stirpem deest (columnae XIX/XXXVIII/LVII/LXXVI)");
                redde FALSUM;
            }
            r.stirpes[k] = _tondere(_chorda(linea.datum
                + k * (DL_STIRPS_MENSURA + I), DL_STIRPS_MENSURA));
        }
        per (k = ZEPHYRUM; k < (i32)V; k++)
        {
            i8 c = linea.datum[DL_TRAN_AB + k * (i32)II];

            si (   c < 'A' || c > 'Z'
                || (k < (i32)IV
                    && linea.datum[DL_TRAN_AB + k * (i32)II + I]
                    != ' '))
            {
                _vitium(vitium, "DICTLINE.GEN", numerus, ZEPHYRUM,
                    "codices translationis (columnae CI-CIX) non cadunt");
                redde FALSUM;
            }
        }
        si (linea.datum[DL_SENSUS_AB - I] != ' ')
        {
            _vitium(vitium, "DICTLINE.GEN", numerus, ZEPHYRUM,
                "spatium ante sensum (columna CX) deest");
            redde FALSUM;
        }
        xar_vacare(lexemata);
        si (!_lexemata(_chorda(linea.datum + DL_PARS_AB,
                DL_PARS_AD - DL_PARS_AB), lexemata))
        {
            redde FALSUM;
        }
        n = xar_numerus(lexemata);
        si (n == ZEPHYRUM)
        {
            _vitium(vitium, "DICTLINE.GEN", numerus, ZEPHYRUM,
                "pars orationis deest");
            redde FALSUM;
        }
        r.pars = oratio_pars_ex_titulo(_lexema(lexemata, ZEPHYRUM));
        si (r.pars >= ORATIO_PARS_NUMERUS_PARTIUM)
        {
            _vitium(vitium, "DICTLINE.GEN", numerus, ZEPHYRUM,
                "pars orationis ignota");
            redde FALSUM;
        }
        r.declinatio  = ZEPHYRUM;
        r.varians     = ZEPHYRUM;
        si (_declinata(r.pars))
        {
            si (   n < (i32)III
                || !_numerus_ex(_lexema(lexemata, I), &r.declinatio)
                || !_numerus_ex(_lexema(lexemata, (i32)II), &r.varians))
            {
                _vitium(vitium, "DICTLINE.GEN", numerus, ZEPHYRUM,
                    "declinatio/varians non numeri");
                redde FALSUM;
            }
            r.species = _iungere(piscina, lexemata, (i32)III, n);
        }
        alioquin
        {
            r.species = _iungere(piscina, lexemata, I, n);
        }
        r.aetas       = (character)linea.datum[DL_TRAN_AB];
        r.area        = (character)linea.datum[DL_TRAN_AB + (i32)II];
        r.geo         = (character)linea.datum[DL_TRAN_AB + (i32)IV];
        r.frequentia  = (character)linea.datum[DL_TRAN_AB + (i32)VI];
        r.fons        = (character)linea.datum[DL_TRAN_AB + (i32)VIII];
        r.sensus     = _tondere_dextra(_chorda(linea.datum
            + DL_SENSUS_AB,
            linea.mensura - DL_SENSUS_AB));
        r.linea      = numerus;
        {
            OratioStirps* locus = (OratioStirps*)xar_addere(stirpes);

            si (locus == NIHIL)
            {
                redde FALSUM;
            }
            *locus = r;
        }
    }
    redde VERUM;
}


/* ==================================================
 * INFLECTS.LAT - lexemata; cauda: clavis mensura [finis] aetas frequentia
 * ================================================== */

interior chorda
_sine_commentario (
    chorda linea)
{
    i32 i;

    per (i = ZEPHYRUM; i + I < linea.mensura; i++)
    {
        si (linea.datum[i] == '-' && linea.datum[i + I] == '-')
        {
            redde _chorda(linea.datum, i);
        }
    }
    redde linea;
}

interior b32
_inflects_legere (
                     Piscina* piscina,
                      chorda  fons,
                         Xar* flexiones,
    OratioVocabulariumVitium* vitium)
{
       i32  cursor   = ZEPHYRUM;
       i32  numerus  = ZEPHYRUM;
    chorda  linea;
       Xar* lexemata = xar_creare(piscina, (i32)magnitudo(chorda));

    si (lexemata == NIHIL)
    {
        redde FALSUM;
    }
    dum (_linea_proxima(fons, &cursor, &numerus, &linea))
    {
        OratioFlexio r;
                 i32 n;
                 i32 ab;
                 i32 ad;
              chorda aetas;
              chorda frequentia;

        linea = _tondere(_sine_commentario(linea));
        si (linea.mensura == ZEPHYRUM)
        {
            perge;
        }
        xar_vacare(lexemata);
        si (!_lexemata(linea, lexemata))
        {
            redde FALSUM;
        }
        n = xar_numerus(lexemata);
        si (n < (i32)V)
        {
            _vitium(vitium, "INFLECTS.LAT", numerus, ZEPHYRUM,
                "recordum brevius V lexematis");
            redde FALSUM;
        }
        r.pars = oratio_pars_ex_titulo(_lexema(lexemata, ZEPHYRUM));
        si (r.pars >= ORATIO_PARS_NUMERUS_PARTIUM)
        {
            _vitium(vitium, "INFLECTS.LAT", numerus, ZEPHYRUM,
                "pars orationis ignota");
            redde FALSUM;
        }
        frequentia  = _lexema(lexemata, n - I);
        aetas       = _lexema(lexemata, n - (i32)II);
        si (!_littera_una(aetas) || !_littera_una(frequentia))
        {
            _vitium(vitium, "INFLECTS.LAT", numerus, ZEPHYRUM,
                "aetas/frequentia non litterae singulae");
            redde FALSUM;
        }
        r.aetas       = (character)aetas.datum[ZEPHYRUM];
        r.frequentia  = (character)frequentia.datum[ZEPHYRUM];
        si (_numerus_ex(_lexema(lexemata, n - (i32)III), &r.mensura))
        {
            /* sine fine: ... clavis 0 aetas frequentia */
            si (r.mensura != ZEPHYRUM)
            {
                _vitium(vitium, "INFLECTS.LAT", numerus, ZEPHYRUM,
                    "mensura finis sine fine non ZEPHYRUM");
                redde FALSUM;
            }
            r.finis  = _vacua();
            ad       = n - (i32)IV;
        }
        alioquin
        {
            r.finis = _lexema(lexemata, n - (i32)III);
            si (   n < (i32)VI
                || !_numerus_ex(_lexema(lexemata, n - (i32)IV),
                &r.mensura)
                || r.mensura != r.finis.mensura)
            {
                _vitium(vitium, "INFLECTS.LAT", numerus, ZEPHYRUM,
                    "mensura finis fini non congruit");
                redde FALSUM;
            }
            ad = n - (i32)V;
        }
        si (   ad < I || !_numerus_ex(_lexema(lexemata, ad), &r.clavis)
            || r.clavis > (i32)IX)
        {
            _vitium(vitium, "INFLECTS.LAT", numerus, ZEPHYRUM,
                "clavis stirpis non numerus 0-9");
            redde FALSUM;
        }
        r.declinatio  = ZEPHYRUM;
        r.varians     = ZEPHYRUM;
        ab            = I;
        si (_declinata(r.pars))
        {
            si (   ad < (i32)III
                || !_numerus_ex(_lexema(lexemata, I), &r.declinatio)
                || !_numerus_ex(_lexema(lexemata, (i32)II), &r.varians))
            {
                _vitium(vitium, "INFLECTS.LAT", numerus, ZEPHYRUM,
                    "declinatio/varians non numeri");
                redde FALSUM;
            }
            ab = (i32)III;
        }
        r.accidentia  = _iungere(piscina, lexemata, ab, ad);
        r.linea       = numerus;
        {
            OratioFlexio* locus = (OratioFlexio*)xar_addere(flexiones);

            si (locus == NIHIL)
            {
                redde FALSUM;
            }
            *locus = r;
        }
    }
    redde VERUM;
}


/* ==================================================
 * ADDONS.LAT / UNIQUES.LAT - terna
 * ================================================== */

interior b32
_commentarium_aut_vacua (
    chorda linea)
{
    chorda t = _tondere(linea);

    redde (b32)(t.mensura == ZEPHYRUM
        || (t.mensura >= (i32)II && t.datum[ZEPHYRUM] == '-'
            && t.datum[I] == '-'));
}

interior b32
_addons_legere (
                     Piscina* piscina,
                      chorda  fons,
                         Xar* additamenta,
    OratioVocabulariumVitium* vitium)
{
    i32 cursor   = ZEPHYRUM;
    i32 numerus  = ZEPHYRUM;
    i32 gradus   = ZEPHYRUM;
    chorda linea;
    OratioAdditamentum r;
    Xar* lexemata = xar_creare(piscina, (i32)magnitudo(chorda));

    si (lexemata == NIHIL)
    {
        redde FALSUM;
    }
    memset(&r, ZEPHYRUM, magnitudo(r));
    dum (_linea_proxima(fons, &cursor, &numerus, &linea))
    {
        si (gradus == ZEPHYRUM)
        {
            OratioPars genus;

            si (_commentarium_aut_vacua(linea))
            {
                perge;
            }
            xar_vacare(lexemata);
            si (!_lexemata(linea, lexemata))
            {
                redde FALSUM;
            }
            genus = xar_numerus(lexemata) >= (i32)II
                ? oratio_pars_ex_titulo(_lexema(lexemata, ZEPHYRUM))
                : ORATIO_PARS_NUMERUS_PARTIUM;
            si (   genus != ORATIO_PARS_PREFIX
                && genus != ORATIO_PARS_SUFFIX
                && genus != ORATIO_PARS_TACKON)
            {
                _vitium(vitium, "ADDONS.LAT", numerus, ZEPHYRUM,
                    "caput PREFIX/SUFFIX/TACKON cum forma exspectatum");
                redde FALSUM;
            }
            r.genus  = genus;
            r.forma  = _lexema(lexemata, I);
            r.linea  = numerus;
            gradus   = I;
        }
        alioquin si (gradus == I)
        {
            xar_vacare(lexemata);
            si (!_lexemata(linea, lexemata))
            {
                redde FALSUM;
            }
            si (xar_numerus(lexemata) == ZEPHYRUM)
            {
                _vitium(vitium, "ADDONS.LAT", numerus, ZEPHYRUM,
                    "recordum additamenti vacuum");
                redde FALSUM;
            }
            r.descriptio = _iungere(piscina, lexemata, ZEPHYRUM,
                xar_numerus(lexemata));
            gradus = (i32)II;
        }
        alioquin
        {
            OratioAdditamentum* locus;

            r.sensus  = _tondere_dextra(linea);
            locus     = (OratioAdditamentum*)xar_addere(additamenta);
            si (locus == NIHIL)
            {
                redde FALSUM;
            }
            *locus = r;
            gradus = ZEPHYRUM;
        }
    }
    si (gradus != ZEPHYRUM)
    {
        _vitium(vitium, "ADDONS.LAT", numerus, ZEPHYRUM,
            "additamentum ultimum truncatum");
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_uniques_legere (
                     Piscina* piscina,
                      chorda  fons,
                         Xar* unica,
    OratioVocabulariumVitium* vitium)
{
    i32 cursor   = ZEPHYRUM;
    i32 numerus  = ZEPHYRUM;
    i32 gradus   = ZEPHYRUM;
    chorda linea;
    OratioUnicum r;
    Xar* lexemata = xar_creare(piscina, (i32)magnitudo(chorda));

    si (lexemata == NIHIL)
    {
        redde FALSUM;
    }
    memset(&r, ZEPHYRUM, magnitudo(r));
    dum (_linea_proxima(fons, &cursor, &numerus, &linea))
    {
        si (gradus == ZEPHYRUM)
        {
            si (_commentarium_aut_vacua(linea))
            {
                perge;
            }
            xar_vacare(lexemata);
            si (!_lexemata(linea, lexemata))
            {
                redde FALSUM;
            }
            si (xar_numerus(lexemata) != I)
            {
                _vitium(vitium, "UNIQUES.LAT", numerus, ZEPHYRUM,
                    "forma unica (lexema unum) exspectata");
                redde FALSUM;
            }
            r.forma  = _lexema(lexemata, ZEPHYRUM);
            r.linea  = numerus;
            gradus   = I;
        }
        alioquin si (gradus == I)
        {
            xar_vacare(lexemata);
            si (!_lexemata(linea, lexemata))
            {
                redde FALSUM;
            }
            si (xar_numerus(lexemata) == ZEPHYRUM)
            {
                _vitium(vitium, "UNIQUES.LAT", numerus, ZEPHYRUM,
                    "recordum unici vacuum");
                redde FALSUM;
            }
            r.descriptio = _iungere(piscina, lexemata, ZEPHYRUM,
                xar_numerus(lexemata));
            gradus = (i32)II;
        }
        alioquin
        {
            OratioUnicum* locus;

            r.sensus  = _tondere_dextra(linea);
            locus     = (OratioUnicum*)xar_addere(unica);
            si (locus == NIHIL)
            {
                redde FALSUM;
            }
            *locus = r;
            gradus = ZEPHYRUM;
        }
    }
    si (gradus != ZEPHYRUM)
    {
        _vitium(vitium, "UNIQUES.LAT", numerus, ZEPHYRUM,
            "unicum ultimum truncatum");
        redde FALSUM;
    }
    redde VERUM;
}


/* ==================================================
 * Emissio nuntii
 * ================================================== */

interior b32
_chordam_scribere (
    NuntiumScriptor* s,
                i32  tag,
             chorda  c)
{
    si (c.mensura == ZEPHYRUM)
    {
        redde VERUM;   /* absens = vacua */
    }
    redde nuntium_scribere_chorda(s, tag, c);
}

interior b32
_nidum_scribere (
    NuntiumScriptor* exterior,
    NuntiumScriptor* interior_s,
                i32  tag)
{
    chorda datum = nuntium_scriptor_datum(interior_s);

    si (nuntium_scriptor_error(interior_s))
    {
        redde FALSUM;
    }
    redde nuntium_scribere_nuntium(exterior, tag, datum);
}

b32
oratio_vocabularium_coquere (
                              Piscina* piscina,
    constans OratioVocabulariumFontes* fontes,
                   constans character* fons_titulus,
                               chorda* exitus,
             OratioVocabulariumCensus* census,
             OratioVocabulariumVitium* vitium)
{
    Xar* stirpes    = xar_creare(piscina,
        (i32)magnitudo(OratioStirps));
    Xar* flexiones  = xar_creare(piscina,
        (i32)magnitudo(OratioFlexio));
    Xar* additamenta = xar_creare(piscina,
        (i32)magnitudo(OratioAdditamentum));
    Xar* unica       = xar_creare(piscina,
        (i32)magnitudo(OratioUnicum));
    NuntiumScriptor* ext;
    NuntiumScriptor* nid;
                i32  i;

    _vitium(vitium, "", ZEPHYRUM, ZEPHYRUM, "");
    si (   stirpes     == NIHIL || flexiones == NIHIL
        || additamenta == NIHIL
        || unica       == NIHIL)
    {
        redde FALSUM;
    }
    si (   !_dictline_legere(piscina, fontes->dictline, stirpes, vitium)
        || !_inflects_legere(piscina, fontes->inflects, flexiones,
        vitium)
        || !_addons_legere(piscina, fontes->addons, additamenta, vitium)
        || !_uniques_legere(piscina, fontes->uniques, unica, vitium))
    {
        redde FALSUM;
    }
    ext = nuntium_scriptor_creare(piscina, (i32)8388608);
    nid = nuntium_scriptor_creare(piscina, (i32)512);
    si (ext == NIHIL || nid == NIHIL)
    {
        redde FALSUM;
    }

    /* CAPUT */
    nuntium_scriptor_purgare(nid);
    nuntium_scribere_varint(nid, I, (i64)ORATIO_VOCABULARIUM_VERSIO);
        nuntium_scribere_chorda(nid, (i32)II,
            _ex_literis(fons_titulus));

    nuntium_scribere_varint(nid, (i32)III, (i64)xar_numerus(stirpes));
    nuntium_scribere_varint(nid, (i32)IV, (i64)xar_numerus(flexiones));
    nuntium_scribere_varint(nid, (i32)V, (i64)xar_numerus(additamenta));
    nuntium_scribere_varint(nid, (i32)VI, (i64)xar_numerus(unica));
    si (!_nidum_scribere(ext, nid, (i32)ORATIO_VOC_CAPUT))
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < xar_numerus(stirpes); i++)
    {
        constans OratioStirps* r = (constans OratioStirps*)xar_obtinere(
            stirpes, i);
        i32 k;

        nuntium_scriptor_purgare(nid);
        per (k = ZEPHYRUM; k < (i32)IV; k++)
        {
            _chordam_scribere(nid, k + I, r->stirpes[k]);
        }
        nuntium_scribere_varint(nid, (i32)V, (i64)r->pars);
        nuntium_scribere_varint(nid, (i32)VI, (i64)r->declinatio);
        nuntium_scribere_varint(nid, (i32)VII, (i64)r->varians);
        _chordam_scribere(nid, (i32)VIII, r->species);
        nuntium_scribere_varint(nid, (i32)IX, (i64)(i8)r->aetas);
        nuntium_scribere_varint(nid, (i32)X, (i64)(i8)r->area);
        nuntium_scribere_varint(nid, (i32)XI, (i64)(i8)r->geo);
        nuntium_scribere_varint(nid, (i32)XII, (i64)(i8)r->frequentia);
        nuntium_scribere_varint(nid, (i32)XIII, (i64)(i8)r->fons);
        _chordam_scribere(nid, (i32)XIV, r->sensus);
        nuntium_scribere_varint(nid, (i32)XV, (i64)r->linea);
        si (!_nidum_scribere(ext, nid, (i32)ORATIO_VOC_STIRPS))
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(flexiones); i++)
    {
        constans OratioFlexio* r = (constans OratioFlexio*)xar_obtinere(
            flexiones, i);

        nuntium_scriptor_purgare(nid);
        nuntium_scribere_varint(nid, I, (i64)r->pars);
        nuntium_scribere_varint(nid, (i32)II, (i64)r->declinatio);
        nuntium_scribere_varint(nid, (i32)III, (i64)r->varians);
        _chordam_scribere(nid, (i32)IV, r->accidentia);
        nuntium_scribere_varint(nid, (i32)V, (i64)r->clavis);
        nuntium_scribere_varint(nid, (i32)VI, (i64)r->mensura);
        _chordam_scribere(nid, (i32)VII, r->finis);
        nuntium_scribere_varint(nid, (i32)VIII, (i64)(i8)r->aetas);
        nuntium_scribere_varint(nid, (i32)IX, (i64)(i8)r->frequentia);
        nuntium_scribere_varint(nid, (i32)X, (i64)r->linea);
        si (!_nidum_scribere(ext, nid, (i32)ORATIO_VOC_FLEXIO))
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(additamenta); i++)
    {
        constans OratioAdditamentum* r =
            (constans OratioAdditamentum*)xar_obtinere(additamenta, i);

        nuntium_scriptor_purgare(nid);
        nuntium_scribere_varint(nid, I, (i64)r->genus);
        _chordam_scribere(nid, (i32)II, r->forma);
        _chordam_scribere(nid, (i32)III, r->descriptio);
        _chordam_scribere(nid, (i32)IV, r->sensus);
        nuntium_scribere_varint(nid, (i32)V, (i64)r->linea);
        si (!_nidum_scribere(ext, nid, (i32)ORATIO_VOC_ADDITAMENTUM))
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(unica); i++)
    {
        constans OratioUnicum* r = (constans OratioUnicum*)xar_obtinere(
            unica, i);

        nuntium_scriptor_purgare(nid);
        _chordam_scribere(nid, I, r->forma);
        _chordam_scribere(nid, (i32)II, r->descriptio);
        _chordam_scribere(nid, (i32)III, r->sensus);
        nuntium_scribere_varint(nid, (i32)IV, (i64)r->linea);
        si (!_nidum_scribere(ext, nid, (i32)ORATIO_VOC_UNICUM))
        {
            redde FALSUM;
        }
    }
    si (nuntium_scriptor_error(ext))
    {
        redde FALSUM;
    }
    *exitus = nuntium_scriptor_datum(ext);
    si (census != NIHIL)
    {
        census->stirpes       = xar_numerus(stirpes);
        census->flexiones     = xar_numerus(flexiones);
        census->additamenta   = xar_numerus(additamenta);
        census->unica         = xar_numerus(unica);
        census->versio        = (i32)ORATIO_VOCABULARIUM_VERSIO;
                census->fons  = _ex_literis(fons_titulus);

    }
    redde VERUM;
}


/* ==================================================
 * Decoctio
 * ================================================== */

interior b32
_saltare (
    NuntiumLector* l,
              i32  wire)
{
    redde nuntium_saltare_field(l, wire);
}

b32
oratio_vocabularium_stirps_legere (
         Piscina* piscina,
          chorda  nidus,
    OratioStirps* r)
{
    NuntiumLector* l = nuntium_lector_creare(piscina, nidus);
              i32  tag;
              i32  wire;
              b32  pars_lecta = FALSUM;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    memset(r, ZEPHYRUM, magnitudo(*r));
    dum (nuntium_legere_field(l, &tag, &wire))
    {
        commutatio (tag)
        {
            casus I: casus II: casus III: casus IV:
                r->stirpes[tag - I] = nuntium_legere_chorda(l);
                frange;
            casus V:
                r->pars = (OratioPars)nuntium_legere_varint(l);
                pars_lecta = VERUM;
                frange;
            casus VI:   r->declinatio =
                            (i32)nuntium_legere_varint(l); frange;
            casus VII:  r->varians    =
                            (i32)nuntium_legere_varint(l); frange;
            casus VIII: r->species    =
                            nuntium_legere_chorda(l);      frange;
            casus IX:   r->aetas      =
                            (character)nuntium_legere_varint(l); frange;
            casus X:    r->area       =
                            (character)nuntium_legere_varint(l); frange;
            casus XI:   r->geo        =
                            (character)nuntium_legere_varint(l); frange;
            casus XII:  r->frequentia =
                            (character)nuntium_legere_varint(l); frange;
            casus XIII: r->fons       =
                            (character)nuntium_legere_varint(l); frange;
            casus XIV:  r->sensus     =
                            nuntium_legere_chorda(l);      frange;
            casus XV:   r->linea      =
                            (i32)nuntium_legere_varint(l); frange;
            ordinarius:
                si (!_saltare(l, wire))
                {
                    redde FALSUM;
                }
        }
    }
    redde (b32)(!nuntium_lector_error(l) && pars_lecta
        && (i32)r->pars < (i32)ORATIO_PARS_NUMERUS_PARTIUM
        && r->linea > ZEPHYRUM);
}

b32
oratio_vocabularium_flexio_legere (
         Piscina* piscina,
          chorda  nidus,
    OratioFlexio* r)
{
    NuntiumLector* l = nuntium_lector_creare(piscina, nidus);
              i32  tag;
              i32  wire;
              b32  pars_lecta = FALSUM;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    memset(r, ZEPHYRUM, magnitudo(*r));
    dum (nuntium_legere_field(l, &tag, &wire))
    {
        commutatio (tag)
        {
            casus I:
                r->pars = (OratioPars)nuntium_legere_varint(l);
                pars_lecta = VERUM;
                frange;
            casus II:   r->declinatio =
                            (i32)nuntium_legere_varint(l); frange;
            casus III:  r->varians    =
                            (i32)nuntium_legere_varint(l); frange;
            casus IV:   r->accidentia =
                            nuntium_legere_chorda(l);      frange;
            casus V:    r->clavis     =
                            (i32)nuntium_legere_varint(l); frange;
            casus VI:   r->mensura    =
                            (i32)nuntium_legere_varint(l); frange;
            casus VII:  r->finis      =
                            nuntium_legere_chorda(l);      frange;
            casus VIII: r->aetas      =
                            (character)nuntium_legere_varint(l); frange;
            casus IX:   r->frequentia =
                            (character)nuntium_legere_varint(l); frange;
            casus X:    r->linea      =
                            (i32)nuntium_legere_varint(l); frange;
            ordinarius:
                si (!_saltare(l, wire))
                {
                    redde FALSUM;
                }
        }
    }
    redde (b32)(!nuntium_lector_error(l) && pars_lecta
        && (i32)r->pars < (i32)ORATIO_PARS_NUMERUS_PARTIUM
        && r->mensura == r->finis.mensura && r->linea > ZEPHYRUM);
}

b32
oratio_vocabularium_additamentum_legere (
               Piscina* piscina,
                chorda  nidus,
    OratioAdditamentum* r)
{
    NuntiumLector* l = nuntium_lector_creare(piscina, nidus);
              i32  tag;
              i32  wire;
              b32  genus_lectum = FALSUM;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    memset(r, ZEPHYRUM, magnitudo(*r));
    dum (nuntium_legere_field(l, &tag, &wire))
    {
        commutatio (tag)
        {
            casus I:
                r->genus = (OratioPars)nuntium_legere_varint(l);
                genus_lectum = VERUM;
                frange;
            casus II:  r->forma      =
                           nuntium_legere_chorda(l);      frange;
            casus III: r->descriptio =
                           nuntium_legere_chorda(l);      frange;
            casus IV:  r->sensus     =
                           nuntium_legere_chorda(l);      frange;
            casus V:   r->linea      =
                           (i32)nuntium_legere_varint(l); frange;
            ordinarius:
                si (!_saltare(l, wire))
                {
                    redde FALSUM;
                }
        }
    }
    redde (b32)(!nuntium_lector_error(l) && genus_lectum
        && r->forma.mensura > ZEPHYRUM && r->linea > ZEPHYRUM);
}

b32
oratio_vocabularium_unicum_legere (
         Piscina* piscina,
          chorda  nidus,
    OratioUnicum* r)
{
    NuntiumLector* l = nuntium_lector_creare(piscina, nidus);
              i32  tag;
              i32  wire;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    memset(r, ZEPHYRUM, magnitudo(*r));
    dum (nuntium_legere_field(l, &tag, &wire))
    {
        commutatio (tag)
        {
            casus I:   r->forma      =
                           nuntium_legere_chorda(l);      frange;
            casus II:  r->descriptio =
                           nuntium_legere_chorda(l);      frange;
            casus III: r->sensus     =
                           nuntium_legere_chorda(l);      frange;
            casus IV:  r->linea      =
                           (i32)nuntium_legere_varint(l); frange;
            ordinarius:
                si (!_saltare(l, wire))
                {
                    redde FALSUM;
                }
        }
    }
    redde (b32)(!nuntium_lector_error(l) && r->forma.mensura > ZEPHYRUM
        && r->linea > ZEPHYRUM);
}

interior b32
_caput_legere (
                     Piscina* piscina,
                      chorda  nidus,
    OratioVocabulariumCensus* c)
{
    NuntiumLector* l = nuntium_lector_creare(piscina, nidus);
              i32  tag;
              i32  wire;

    si (l == NIHIL)
    {
        redde FALSUM;
    }
    memset(c, ZEPHYRUM, magnitudo(*c));
    dum (nuntium_legere_field(l, &tag, &wire))
    {
        commutatio (tag)
        {
            casus I:   c->versio      =
                           (i32)nuntium_legere_varint(l); frange;
            casus II:  c->fons        =
                           nuntium_legere_chorda(l);      frange;
            casus III: c->stirpes     =
                           (i32)nuntium_legere_varint(l); frange;
            casus IV:  c->flexiones   =
                           (i32)nuntium_legere_varint(l); frange;
            casus V:   c->additamenta =
                           (i32)nuntium_legere_varint(l); frange;
            casus VI:  c->unica       =
                           (i32)nuntium_legere_varint(l); frange;
            ordinarius:
                si (!_saltare(l, wire))
                {
                    redde FALSUM;
                }
        }
    }
    redde (b32)(!nuntium_lector_error(l)
        && c->versio == (i32)ORATIO_VOCABULARIUM_VERSIO);
}

b32
oratio_vocabularium_recensere (
                     Piscina* piscina,
                      chorda  tabula,
    OratioVocabulariumCensus* census,
    OratioVocabulariumVitium* vitium)
{
    NuntiumLector* l = nuntium_lector_creare(piscina, tabula);
    OratioVocabulariumCensus caput;
        i32 tag;
    i32 wire;
    s32 ordo          = ZEPHYRUM;
    b32 caput_lectum  = FALSUM;

    i32 stirpes      = ZEPHYRUM;
    i32 flexiones    = ZEPHYRUM;
    i32 additamenta  = ZEPHYRUM;
    i32 unica        = ZEPHYRUM;

    _vitium(vitium, "la.bin", ZEPHYRUM, ZEPHYRUM, "");
    si (l == NIHIL)
    {
        redde FALSUM;
    }
    memset(&caput, ZEPHYRUM, magnitudo(caput));
    dum (nuntium_legere_field(l, &tag, &wire))
    {
        chorda nidus;

        si (wire != (i32)NUNTIUM_WIRE_LENGTH_DELIMITED)
        {
            _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
                "campus summus non nidus");
            redde FALSUM;
        }
        nidus = nuntium_legere_nuntium(l);
        si (nuntium_lector_error(l))
        {
            _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
                "nidus truncatus");
            redde FALSUM;
        }
        si (ordo == ZEPHYRUM && tag != (i32)ORATIO_VOC_CAPUT)
        {
            _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
                "caput primum deest");
            redde FALSUM;
        }
        commutatio (tag)
        {
            casus ORATIO_VOC_CAPUT:
            {
                si (   caput_lectum
                    || !_caput_legere(piscina, nidus, &caput))
                {
                    _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
                        "caput iteratum aut versio ignota");
                    redde FALSUM;
                }
                caput_lectum = VERUM;
                frange;
            }
            casus ORATIO_VOC_STIRPS:
            {
                OratioStirps r;

                si (!oratio_vocabularium_stirps_legere(piscina, nidus,
                    &r))
                {
                    _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
                        "stirps non decoquitur");
                    redde FALSUM;
                }
                stirpes = stirpes + I;
                frange;
            }
            casus ORATIO_VOC_FLEXIO:
            {
                OratioFlexio r;

                si (!oratio_vocabularium_flexio_legere(piscina, nidus,
                    &r))
                {
                    _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
                        "flexio non decoquitur");
                    redde FALSUM;
                }
                flexiones = flexiones + I;
                frange;
            }
            casus ORATIO_VOC_ADDITAMENTUM:
            {
                OratioAdditamentum r;

                si (!oratio_vocabularium_additamentum_legere(piscina,
                    nidus,
                        &r))
                {
                    _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
                        "additamentum non decoquitur");
                    redde FALSUM;
                }
                additamenta = additamenta + I;
                frange;
            }
            casus ORATIO_VOC_UNICUM:
            {
                OratioUnicum r;

                si (!oratio_vocabularium_unicum_legere(piscina, nidus,
                    &r))
                {
                    _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
                        "unicum non decoquitur");
                    redde FALSUM;
                }
                unica = unica + I;
                frange;
            }
            ordinarius:
                _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
                    "tag ignotus");
                redde FALSUM;
        }
        ordo = ordo + I;
    }
    si (nuntium_lector_error(l))
    {
        _vitium(vitium, "la.bin", ZEPHYRUM, ordo, "fluxus corruptus");
        redde FALSUM;
    }
    si (   !caput_lectum || caput.stirpes != stirpes
        || caput.flexiones   != flexiones
        || caput.additamenta != additamenta
        || caput.unica       != unica)
    {
        _vitium(vitium, "la.bin", ZEPHYRUM, ordo,
            "numeri capitis recordis non congruunt");
        redde FALSUM;
    }
    si (census != NIHIL)
    {
        *census = caput;
    }
    redde VERUM;
}


/* ==================================================
 * Sigillum
 * ================================================== */

chorda
oratio_vocabularium_sigillum (
    Piscina* piscina,
     chorda  tabula)
{
    i8 digestum[SHA256_DIGEST_MENSURA];
    hic_manens constans character HEX[] = "0123456789abcdef";
    i8* hex = (i8*)piscina_allocare(piscina, (memoriae_index)XVI);
    i32 i;

    si (hex == NIHIL)
    {
        redde _vacua();
    }
    sha256_friare(tabula.datum, tabula.mensura, digestum);
    per (i = ZEPHYRUM; i < (i32)VIII; i++)
    {
        hex[i * (i32)II]      = (i8)HEX[(digestum[i] >> IV) & 0x0F];
        hex[i * (i32)II + I]  = (i8)HEX[digestum[i] & 0x0F];
    }
    redde _chorda(hex, (i32)XVI);
}
