/* probatio_tibia.c - Probationes instrumenti (include/tibia.h)
 *
 * Frequentia per transitus nullos ascendentes INTERPOLATOS (tempus
 * fractum transitus): media per fenestram aut momentanea circa
 * exemplum (tres cycli). Sinus purus (sine vibratu, involucrum plenum)
 * ut frequentia legatur sine ambiguitate.
 *
 * tenet: porta aperta numquam finita, 440 cycli/s. involucrum: impetus,
 * tenor, remissio post portam clausam, deinde finita et silentium.
 * glissandum: LXIX -> LXXXI in 0.5 s - in medio 622 (medium
 * GEOMETRICUM: semitonia linealiter), non 660; post 880; nullus saltus
 * exempli; statim et a valore currente. vibratus: intra profunditatem,
 * utrumque extremum attingit. praeparata et determinismus.
 */
#include "latina.h"
#include "credo.h"
#include "tibia.h"
#include <math.h>
#include <string.h>

#define SECUNDA SONUS_FREQUENTIA
#define TABULA_MAXIMA (II * SONUS_FREQUENTIA)

hic_manens f32 tabula_a[TABULA_MAXIMA];
hic_manens f32 tabula_b[TABULA_MAXIMA];

interior vacuum
_parare (
    CredoContextus* c)
{
    (vacuum)c;
    memset(tabula_a, ZEPHYRUM, magnitudo(tabula_a));
    memset(tabula_b, ZEPHYRUM, magnitudo(tabula_b));
}

/* sinus purus: involucrum plenum statim, sine vibratu et colo */
interior TibiaParametra
_pura (
    vacuum)
{
    TibiaParametra p;

    p.forma                = (f32)SONUS_UNDA_SINUS;
    p.officium             = 0.5f;
    p.involucrum.impetus   = 0.0f;
    p.involucrum.lapsus    = 0.0f;
    p.involucrum.tenor     = 1.0f;
    p.involucrum.remissio  = 0.0f;
    p.vibratus             = 0.0f;
    p.vibratus_celeritas   = 0.0f;
    p.gravia               = 0.0f;
    p.amplitudo            = 1.0f;
    redde p;
}

interior vacuum
_reddere (
    TibiaVox* v,
         f32* x,
         i32  a,
         i32  b)
{
    i32 k;

    per (k = a; k < b; k++)
    {
        x[k] = tibia_vocis_proximum(v);
    }
}

/* transitus ascendens inter k-1 et k: tempus fractum; -1 si nullus */
interior f64
_transitus (
    constans f32* x,
             i32  k)
{
    si (x[k - I] < 0.0f && x[k] >= 0.0f)
    {
        redde (f64)(k - I) + (f64)x[k - I]
            / ((f64)x[k - I] - (f64)x[k]);
    }
    redde -1.0;
}

/* frequentia media in [a, b): (ultimus - primus) / (numerus - 1) */
interior f64
_frequentia_media (
    constans f32* x,
             i32  a,
             i32  b)
{
    f64 primus   = -1.0;
    f64 ultimus  = -1.0;
    i32 numerus  = ZEPHYRUM;
    i32 k;

    per (k = a + I; k < b; k++)
    {
        f64 t = _transitus(x, k);

        si (t >= 0.0)
        {
            si (primus < 0.0)
            {
                primus = t;
            }
            ultimus = t;
            numerus++;
        }
    }
    si (numerus < II)
    {
        redde 0.0;
    }
    redde (f64)SONUS_FREQUENTIA * (f64)(numerus - I)
        / (ultimus - primus);
}

/* frequentia momentanea: tres cycli circa exemplum 'centrum' */
interior f64
_frequentia_circa (
    constans f32* x,
             i32  n,
             i32  centrum)
{
    f64 t[IV];
    i32 j = ZEPHYRUM;
    i32 k;

    /* transitus ultimus ante centrum, deinde tres post */
    per (k = centrum; k > I && j == ZEPHYRUM; k--)
    {
        si (_transitus(x, k) >= 0.0)
        {
            t[j++] = _transitus(x, k);
        }
    }
    per (k = centrum + I; k < n && j < IV; k++)
    {
        si (_transitus(x, k) >= 0.0)
        {
            t[j++] = _transitus(x, k);
        }
    }
    si (j < IV)
    {
        redde 0.0;
    }
    redde (f64)SONUS_FREQUENTIA * III / (t[III] - t[ZEPHYRUM]);
}

/* |exemplum| maximum in [a, b) */
interior f32
_apex (
    constans f32* x,
             i32  a,
             i32  b)
{
    f32 m = 0.0f;
    i32 k;

    per (k = a; k < b; k++)
    {
        f32 y = x[k] < 0.0f ? -x[k] : x[k];

        si (y > m)
        {
            m = y;
        }
    }
    redde m;
}

/* saltus maximus inter exempla vicina in [a, b) */
interior f32
_saltus (
    constans f32* x,
             i32  a,
             i32  b)
{
    f32 m = 0.0f;
    i32 k;

    per (k = a + I; k < b; k++)
    {
        f32 d = x[k] - x[k - I];

        d = d < 0.0f ? -d : d;
        si (d > m)
        {
            m = d;
        }
    }
    redde m;
}

/* clivus maximus sinus frequentiae f: 2 pi f / SONUS_FREQUENTIA */
interior f32
_saltus_sinus (
    f64 f)
{
    redde (f32)(6.283185307179586 * f / (f64)SONUS_FREQUENTIA) * 1.02f;
}


    /* ==================================================
     * TENET: porta aperta sonat, numquam finita
     * ================================================== */

interior vacuum
_sectio_tenet (
    CredoContextus* c)
{
    TibiaParametra p = _pura();
          TibiaVox v;
               f64 f;
               b32 finita = FALSUM;
               i32 k;

    (vacuum)c;
    tibia_vocem_parare(&v, &p, 69.0f);
    per (k = ZEPHYRUM; k < TABULA_MAXIMA; k++)
    {
        tabula_a[k]  = tibia_vocis_proximum(&v);
        finita       = finita || tibia_vox_finita(&v);
    }
    CREDO_FALSUM (finita);
    f = _frequentia_media(tabula_a, SECUNDA, TABULA_MAXIMA);
    CREDO_VERUM (f > 439.5 && f < 440.5);
    CREDO_VERUM (_apex(tabula_a, SECUNDA, TABULA_MAXIMA) > 0.99f);
    CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, TABULA_MAXIMA) <= 1.0f);
    /* fractiones: LXIX.5 = quartus toni super A4 */
    tibia_vocem_parare(&v, &p, 69.5f);
    _reddere(&v, tabula_b, ZEPHYRUM, SECUNDA);
    f = _frequentia_media(tabula_b, ZEPHYRUM, SECUNDA);
    CREDO_VERUM (f > 452.4 && f < 453.4);
}


    /* ==================================================
     * INVOLUCRUM: impetus, tenor, remissio, finita
     * ================================================== */

interior vacuum
_sectio_involucrum (
    CredoContextus* c)
{
    TibiaParametra p = _pura();
          TibiaVox v;
               i32 clausura = SECUNDA / V;           /* 0.2 s */
               i32 remissio = SECUNDA / XX;          /* 0.05 s */
               i32 k;
               b32 finita_ante = FALSUM;

    (vacuum)c;
    p.involucrum.impetus   = 0.01f;
    p.involucrum.lapsus    = 0.02f;
    p.involucrum.tenor     = 0.5f;
    p.involucrum.remissio  = 0.05f;
    tibia_vocem_parare(&v, &p, 81.0f);
    per (k = ZEPHYRUM; k < clausura; k++)
    {
        tabula_a[k] = tibia_vocis_proximum(&v);
        finita_ante = finita_ante || tibia_vox_finita(&v);
    }
    CREDO_FALSUM (finita_ante);
    /* impetus: initio parvum, crescit */
    CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, LX) < 0.2f);
    CREDO_VERUM (_apex(tabula_a, CC, CD) > _apex(tabula_a, ZEPHYRUM,
                 CC));
    /* tenor post impetum et lapsum (0.03 s) */
    CREDO_VERUM (_apex(tabula_a, SECUNDA / X, clausura) > 0.48f);
    CREDO_VERUM (_apex(tabula_a, SECUNDA / X, clausura) < 0.52f);
    /* porta clausa: remissio, deinde finita et silentium */
    tibia_portam_claudere(&v);
    tibia_portam_claudere(&v);
    CREDO_FALSUM (tibia_vox_finita(&v));
    _reddere(&v, tabula_a, clausura, clausura + remissio + C);
    CREDO_VERUM (tibia_vox_finita(&v));
    CREDO_VERUM (_apex(tabula_a, clausura, clausura + CC) > 0.4f);
    CREDO_VERUM (_apex(tabula_a, clausura + remissio + L,
                       clausura + remissio + C) == 0.0f);
}


    /* ==================================================
     * MOTUS NOTAE: semitonia linealiter, sine saltu
     * ================================================== */

interior vacuum
_sectio_motus (
    CredoContextus* c)
{
    TibiaParametra p = _pura();
          TibiaVox v;
               i32 initium = SECUNDA / X;        /* 0.1 s */
               i32 spatium = SECUNDA / II;       /* 0.5 s */
               f64 f;

    (vacuum)c;
    tibia_vocem_parare(&v, &p, 69.0f);
    _reddere(&v, tabula_a, ZEPHYRUM, initium);
    tibia_notam_ponere(&v, 81.0f, spatium);
    _reddere(&v, tabula_a, initium, TABULA_MAXIMA);
    f = _frequentia_media(tabula_a, ZEPHYRUM, initium);
    CREDO_VERUM (f > 439.5 && f < 440.5);
    /* medium: nota LXXV = 622.25, non (440 + 880) / 2 = 660 */
    f = _frequentia_circa(tabula_a, TABULA_MAXIMA,
        initium + spatium / II);
    CREDO_VERUM (f > 619.0 && f < 626.0);
    f = _frequentia_media(tabula_a, initium + spatium + C,
        TABULA_MAXIMA);
    CREDO_VERUM (f > 879.5 && f < 880.5);
    CREDO_VERUM (_saltus(tabula_a, ZEPHYRUM, TABULA_MAXIMA)
        <= _saltus_sinus(880.0));

    /* statim: phasis continua, frequentia nova */
    tibia_vocem_parare(&v, &p, 69.0f);
    _reddere(&v, tabula_b, ZEPHYRUM, initium);
    tibia_notam_ponere(&v, 93.0f, ZEPHYRUM);
    _reddere(&v, tabula_b, initium, SECUNDA);
    f = _frequentia_media(tabula_b, initium + X, SECUNDA);
    CREDO_VERUM (f > 1759.0 && f < 1761.0);
    CREDO_VERUM (_saltus(tabula_b, ZEPHYRUM, SECUNDA)
        <= _saltus_sinus(1760.0));

    /* in medio glissandi: nota nova a valore CURRENTE */
    tibia_vocem_parare(&v, &p, 69.0f);
    tibia_notam_ponere(&v, 81.0f, spatium);
    _reddere(&v, tabula_b, ZEPHYRUM, spatium / II);
    tibia_notam_ponere(&v, 69.0f, spatium / II);
    _reddere(&v, tabula_b, spatium / II, SECUNDA);
    /* reditus: in medio eius nota LXXII (523.25) */
    f = _frequentia_circa(tabula_b, SECUNDA,
        spatium / II + spatium / IV);
    CREDO_VERUM (f > 520.0 && f < 527.0);
    f = _frequentia_media(tabula_b, spatium + C, SECUNDA);
    CREDO_VERUM (f > 439.5 && f < 440.5);
    CREDO_VERUM (_saltus(tabula_b, ZEPHYRUM, SECUNDA)
        <= _saltus_sinus(880.0));
}


    /* ==================================================
     * VIBRATUS: intra profunditatem, extrema attingit
     * ================================================== */

interior vacuum
_sectio_vibratus (
    CredoContextus* c)
{
    TibiaParametra p = _pura();
          TibiaVox v;
               f64 minimum   = 1.0e9;
               f64 maximum   = 0.0;
               f64 superior  = 440.0 * pow(2.0, 1.0 / 12.0);
               f64 inferior  = 440.0 * pow(2.0, -1.0 / 12.0);
               i32 k;

    (vacuum)c;
    p.vibratus            = 1.0f;
    p.vibratus_celeritas  = 5.0f;
    tibia_vocem_parare(&v, &p, 69.0f);
    _reddere(&v, tabula_a, ZEPHYRUM, SECUNDA);
    per (k = SECUNDA / XX; k < SECUNDA - SECUNDA / XX; k += CCXL)
    {
        f64 f = _frequentia_circa(tabula_a, SECUNDA, k);

        minimum = f < minimum ? f : minimum;
        maximum = f > maximum ? f : maximum;
    }
    CREDO_VERUM (maximum < superior + 3.0);
    CREDO_VERUM (minimum > inferior - 3.0);
    CREDO_VERUM (maximum > 440.0 * pow(2.0, 0.85 / 12.0));
    CREDO_VERUM (minimum < 440.0 * pow(2.0, -0.85 / 12.0));
}


    /* ==================================================
     * COLOR: amplitudo, colum gravium, forma
     * ================================================== */

interior vacuum
_sectio_color (
    CredoContextus* c)
{
    TibiaParametra p = _pura();
          TibiaVox v;
               f32 plenus;

    (vacuum)c;
    p.amplitudo = 0.5f;
    tibia_vocem_parare(&v, &p, 69.0f);
    _reddere(&v, tabula_a, ZEPHYRUM, SECUNDA / X);
    CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, SECUNDA / X) > 0.49f);
    CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, SECUNDA / X) <= 0.5f);
    /* pulsus sine colo: plenus +-1; cum colo gravium (CC) mollior */
    p        = _pura();
    p.forma  = (f32)SONUS_UNDA_PULSUS;
    tibia_vocem_parare(&v, &p, 81.0f);
    _reddere(&v, tabula_a, ZEPHYRUM, SECUNDA / X);
    plenus = _apex(tabula_a, SECUNDA / XX, SECUNDA / X);
    CREDO_VERUM (plenus > 0.99f);
    p.gravia = 200.0f;
    tibia_vocem_parare(&v, &p, 81.0f);
    _reddere(&v, tabula_a, ZEPHYRUM, SECUNDA / X);
    CREDO_VERUM (_apex(tabula_a, SECUNDA / XX, SECUNDA / X) < plenus
                 * 0.7f);
    /* strepitus notam non habet: forma IV ut sinus (III) */
    {
        i32 k;
        b32 pares = VERUM;

        p        = _pura();
        p.forma  = 4.0f;
        tibia_vocem_parare(&v, &p, 69.0f);
        _reddere(&v, tabula_a, ZEPHYRUM, SECUNDA / X);
        p.forma  = 3.0f;
        tibia_vocem_parare(&v, &p, 69.0f);
        _reddere(&v, tabula_b, ZEPHYRUM, SECUNDA / X);
        per (k = ZEPHYRUM; k < SECUNDA / X; k++)
        {
            si (tabula_a[k] != tabula_b[k])
            {
                pares = FALSUM;
            }
        }
        CREDO_VERUM (pares);
    }
}


    /* ==================================================
     * PRAEPARATA et DETERMINISMUS
     * ================================================== */

interior vacuum
_sectio_praeparata (
    CredoContextus* c)
{
    constans TibiaParametra* aether;
    constans TibiaParametra* quadrans;
                   TibiaVox  v;
                        i32  k;
                        b32  pares = VERUM;

    (vacuum)c;
    aether    = tibia_praeparatum("aether");
    quadrans  = tibia_praeparatum("quadrans");
    CREDO_NECESSE (aether != NIHIL && quadrans != NIHIL);
    CREDO_VERUM (tibia_praeparatum("triangulum") != NIHIL);
    CREDO_VERUM (tibia_praeparatum("nullum") == NIHIL);
    CREDO_VERUM (tibia_praeparatum("") == NIHIL);
    CREDO_VERUM (aether->forma == (f32)SONUS_UNDA_SINUS);
    CREDO_VERUM (aether->vibratus > 0.0f);
    CREDO_VERUM (aether->involucrum.impetus > 0.0f);
    CREDO_VERUM (quadrans->forma == (f32)SONUS_UNDA_PULSUS);
    CREDO_VERUM (quadrans->officium == 0.25f);
    /* idem bis: exempla octetim eadem */
    tibia_vocem_parare(&v, aether, 64.0f);
    _reddere(&v, tabula_a, ZEPHYRUM, SECUNDA / II);
    tibia_notam_ponere(&v, 71.0f, SECUNDA / X);
    _reddere(&v, tabula_a, SECUNDA / II, SECUNDA);
    tibia_vocem_parare(&v, aether, 64.0f);
    _reddere(&v, tabula_b, ZEPHYRUM, SECUNDA / II);
    tibia_notam_ponere(&v, 71.0f, SECUNDA / X);
    _reddere(&v, tabula_b, SECUNDA / II, SECUNDA);
    per (k = ZEPHYRUM; k < SECUNDA; k++)
    {
        si (tabula_a[k] != tabula_b[k])
        {
            pares = FALSUM;
        }
    }
    CREDO_VERUM (pares);
    CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, SECUNDA) > 0.0f);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tenet", _sectio_tenet, _parare, NIHIL, NIHIL, NIHIL },
    { "involucrum", _sectio_involucrum, _parare, NIHIL, NIHIL, NIHIL },
    { "motus", _sectio_motus, _parare, NIHIL, NIHIL, NIHIL },
    { "vibratus", _sectio_vibratus, _parare, NIHIL, NIHIL, NIHIL },
    { "color", _sectio_color, _parare, NIHIL, NIHIL, NIHIL },
    { "praeparata", _sectio_praeparata, _parare, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tibia", SECTIONES);
}
