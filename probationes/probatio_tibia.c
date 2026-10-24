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

    memset(&p, ZEPHYRUM, magnitudo(p));
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

/* FNV-1a super octetos exemplorum: vox aperta I s (glissandum LX ->
 * LXIV per 0.1 s), clausa 0.5 s */
interior i64
_vestigium (
    constans TibiaParametra* p)
{
    TibiaVox v;
         i64 h = 1469598103934665603ULL;
         i32 i;
         i32 j;

    tibia_vocem_parare(&v, p, 60.0f);
    tibia_notam_ponere(&v, 64.0f, 4800);
    per (i = ZEPHYRUM; i < SONUS_FREQUENTIA * III / II; i++)
    {
        insignatus character b[IV];
                         f32 x;

        si (i == SONUS_FREQUENTIA)
        {
            tibia_portam_claudere(&v);
        }
        x = tibia_vocis_proximum(&v);
        memcpy(b, &x, IV);
        per (j = ZEPHYRUM; j < IV; j++)
        {
            h ^= (i64)b[j];
            h *= 1099511628211ULL;
        }
    }
    redde h;
}

/* praeparata vetera (genus UNDA) octetim eadem ac ante genera nova
 * (vestigia 2026-10-23, ante CHORDA et FM computata) */
interior vacuum
_sectio_vetera (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM (_vestigium(tibia_praeparatum("aether"))
                 == 17283308480231307402ULL);
    CREDO_VERUM (_vestigium(tibia_praeparatum("quadrans"))
                 == 11789016554117580620ULL);
    CREDO_VERUM (_vestigium(tibia_praeparatum("triangulum"))
                 == 398703613805126966ULL);
}

/* periodus per autocorrelationem: spatium in [prima, ultima] maximus
 * super fenestram ab initio */
interior i32
_periodus (
    constans f32* x,
             i32  initium,
             i32  fenestra,
             i32  prima,
             i32  ultima)
{
    i32 spatium;
    i32 optimus = prima;
    f64 maximum = -1.0e30;

    per (spatium = prima; spatium <= ultima; spatium++)
    {
        f64 summa = 0.0;
        i32 i;

        per (i = initium; i < initium + fenestra; i++)
        {
            summa += (f64)x[i] * (f64)x[i + spatium];
        }
        si (summa > maximum)
        {
            maximum = summa;
            optimus = spatium;
        }
    }
    redde optimus;
}

/* correlatio normalizata ad spatium */
interior f64
_congruentia (
    constans f32* x,
             i32  initium,
             i32  fenestra,
             i32  spatium)
{
    f64 producta    = 0.0;
    f64 quadrata_a  = 0.0;
    f64 quadrata_b  = 0.0;
    i32 i;

    per (i = initium; i < initium + fenestra; i++)
    {
        producta    += (f64)x[i] * (f64)x[i + spatium];
        quadrata_a  += (f64)x[i] * (f64)x[i];
        quadrata_b  += (f64)x[i + spatium] * (f64)x[i + spatium];
    }
    redde quadrata_a > 0.0
        && quadrata_b > 0.0 ? producta
            / sqrt(quadrata_a * quadrata_b) : 0.0;
}

/* periodus fracta: maximum autocorrelationis cum interpolatione
 * parabolica circa spatium optimum */
interior f64
_periodus_fracta (
    constans f32* x,
             i32  initium,
             i32  fenestra,
             i32  prima,
             i32  ultima)
{
    i32 l = _periodus(x, initium, fenestra, prima, ultima);
    f64 a = _congruentia(x, initium, fenestra, l - I);
    f64 b = _congruentia(x, initium, fenestra, l);
    f64 d = _congruentia(x, initium, fenestra, l + I);
    f64 n = a - 2.0 * b + d;

    redde n != 0.0 ? (f64)l + 0.5 * (a - d) / n : (f64)l;
}

interior f64
_efficax (
    constans f32* x,
             i32  initium,
             i32  numerus)
{
    f64 s = 0.0;
    i32 i;

    per (i = initium; i < initium + numerus; i++)
    {
        s += (f64)x[i] * (f64)x[i];
    }
    redde sqrt(s / (f64)numerus);
}

/* vox aperta per n exempla in tabulam */
interior vacuum
_sonare (
    constans TibiaParametra* p,
                        f32  nota,
                        f32* tabula,
                        i32  n)
{
    TibiaVox v;
         i32 i;

    tibia_vocem_parare(&v, p, nota);
    per (i = ZEPHYRUM; i < n; i++)
    {
        tabula[i] = tibia_vocis_proximum(&v);
    }
}

/* Karplus-Strong: periodus = frequentia (A2, A4, C6), decrementum ad
 * -60 dB, determinismus, glissandum, porta clausa finit */
interior vacuum
_sectio_chorda (
    CredoContextus* c)
{
     constans TibiaParametra* p = tibia_praeparatum("chorda");
                    TibiaVox  v;
                         i32  i;
                         f64  initium;
                         f64  serum;

    (vacuum)c;
    CREDO_NECESSE (p != NIHIL
        && (s32)(p->genus + 0.5f) == TIBIA_CHORDA);
    /* A4: 48000 / 440 = 109.09 - etiam fracta (+-0.1: mora filtri
     * pensata) */
    _sonare(p, 69.0f, tabula_a, SECUNDA);
    CREDO_AEQUALIS_I32 (_periodus(tabula_a, SECUNDA / XX, M, C, CXX),
                        CIX);
    CREDO_VERUM (   fabs(_periodus_fracta(tabula_a, SECUNDA / XX, M, C,
                                          CXX) - 109.0909) < 0.1);
    /* A2: 436.4; C6: 45.87 */
    _sonare(p, 45.0f, tabula_a, SECUNDA);
    i = _periodus(tabula_a, SECUNDA / XX, MM, CDXX, CDL);
    CREDO_VERUM (i >= CDXXXV && i <= CDXXXVII);
    _sonare(p, 84.0f, tabula_a, SECUNDA);
    i = _periodus(tabula_a, SECUNDA / XX, D, XL, L);
    CREDO_VERUM (i >= XLV && i <= XLVI);
    /* decrementum: clivus stabilis (post 0.25 s - harmonici impulsus
     * celeriter cadunt): -30 dB per dimidium decrementi (+-3 dB) */
    _sonare(p, 69.0f, tabula_a, TABULA_MAXIMA);
    initium = _efficax(tabula_a, SECUNDA / IV, SECUNDA / C);
    serum   = _efficax(tabula_a, SECUNDA / IV
                   + (i32)(p->decrementum * (f32)SECUNDA / 2.0f),
                   SECUNDA / C);
    CREDO_VERUM (initium > 0.01);
    CREDO_VERUM (   20.0 * log10(serum / initium) < -27.0
                 && 20.0 * log10(serum / initium) > -33.0);
    /* determinismus: eadem nota eadem exempla; alia nota alia */
    _sonare(p, 69.0f, tabula_b, TABULA_MAXIMA);
    CREDO_VERUM (memcmp(tabula_a, tabula_b, magnitudo(tabula_a))
                 == ZEPHYRUM);
    _sonare(p, 70.0f, tabula_b, SECUNDA);
    CREDO_VERUM (memcmp(tabula_a, tabula_b, SECUNDA * magnitudo(f32))
                 != ZEPHYRUM);
    /* glissandum statim ad A5: periodus 54.5 */
    tibia_vocem_parare(&v, p, 69.0f);
    per (i = ZEPHYRUM; i < SECUNDA / X; i++)
    {
        (vacuum)tibia_vocis_proximum(&v);
    }
    tibia_notam_ponere(&v, 81.0f, ZEPHYRUM);
    per (i = ZEPHYRUM; i < SECUNDA / II; i++)
    {
        tabula_a[i] = tibia_vocis_proximum(&v);
    }
    i = _periodus(tabula_a, SECUNDA / X, M, XLV, LXV);
    CREDO_VERUM (i >= LIV && i <= LV);
    /* porta clausa: remissio, deinde finita */
    tibia_portam_claudere(&v);
    per (i = ZEPHYRUM; i < SECUNDA && !tibia_vox_finita(&v); i++)
    {
        (vacuum)tibia_vocis_proximum(&v);
    }
    CREDO_VERUM (tibia_vox_finita(&v));
}

/* claritas: plus energiae in differentiis (altae) cum claritas I */
interior vacuum
_sectio_claritas (
    CredoContextus* c)
{
    TibiaParametra p = *tibia_praeparatum("chorda");
               f64 clara;
               f64 obscura;
               i32 i;
               f64 d;

    (vacuum)c;
    p.claritas = 1.0f;
    _sonare(&p, 57.0f, tabula_a, SECUNDA / II);
    p.claritas = 0.0f;
    _sonare(&p, 57.0f, tabula_b, SECUNDA / II);
    clara    = 0.0;
    obscura  = 0.0;
    per (i = SECUNDA / XX + I; i < SECUNDA / X; i++)
    {
        d        = (f64)tabula_a[i] - (f64)tabula_a[i - I];
        clara    += d * d;
        d        = (f64)tabula_b[i] - (f64)tabula_b[i - I];
        obscura  += d * d;
    }
    CREDO_VERUM (   clara / _efficax(tabula_a, SECUNDA / XX, SECUNDA
        / XX)
                 > obscura / _efficax(tabula_b, SECUNDA / XX, SECUNDA
                     / XX));
}

/* FM: initio index magnus (transitus multi), post decrementum index
 * parvus - frequentia portantis per transitus; determinismus;
 * limites */
interior vacuum
_sectio_campana (
    CredoContextus* c)
{
     constans TibiaParametra* p = tibia_praeparatum("campana");
                         i32  i;
                         i32  primi  = ZEPHYRUM;
                         i32  seri   = ZEPHYRUM;
                         i32  t;
                         f32  summum = 0.0f;

    (vacuum)c;
    CREDO_NECESSE (p != NIHIL
        && (s32)(p->genus + 0.5f) == TIBIA_MODULATIO);
    _sonare(p, 69.0f, tabula_a, TABULA_MAXIMA);
    per (i = ZEPHYRUM; i < TABULA_MAXIMA; i++)
    {
        summum = (f32)fabs(tabula_a[i])
            > summum ? (f32)fabs(tabula_a[i])
                                                 : summum;
    }
    CREDO_VERUM (summum <= p->amplitudo + 1.0e-4f && summum > 0.05f);
    /* ratio inharmonica: initio non periodica in cyclo portantis
     * (ratio 1 periodica esset, correlatio ~1) */
    CREDO_VERUM (_congruentia(tabula_a, SECUNDA / C, M, CIX) < 0.9);
    /* transitus sursum: 0.1 s initio, 0.1 s post 4 decrementa */
    t = (i32)(4.0f * p->decrementum * (f32)SECUNDA);
    per (i = I; i < SECUNDA / X; i++)
    {
        si (tabula_a[i - I] < 0.0f && tabula_a[i] >= 0.0f)
        {
            primi++;
        }
        si (tabula_a[t + i - I] < 0.0f && tabula_a[t + i] >= 0.0f)
        {
            seri++;
        }
    }
    CREDO_VERUM (primi > seri + V);
    /* index ~0: sinus portantis, 44 transitus per 0.1 s (+-1) */
    CREDO_VERUM (seri >= XLIII && seri <= XLV);
    _sonare(p, 69.0f, tabula_b, TABULA_MAXIMA);
    CREDO_VERUM (memcmp(tabula_a, tabula_b, magnitudo(tabula_a))
                 == ZEPHYRUM);
}

/* amplitudo frequentiae unius (DFT singularis) in fenestra */
interior f64
_amplitudo_frequentiae (
    constans f32* x,
             i32  initium,
             i32  numerus,
             f64  frequentia)
{
    f64 re  = 0.0;
    f64 im  = 0.0;
    f64 w   = 6.283185307179586 * frequentia / (f64)SECUNDA;
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        re += (f64)x[initium + i] * cos(w * (f64)i);
        im -= (f64)x[initium + i] * sin(w * (f64)i);
    }
    redde 2.0 * sqrt(re * re + im * im) / (f64)numerus;
}

/* organum: partiales ad rationes 1 2 3 4 cum amplitudinibus tabulae,
 * nihil inter eas, tenet, apex <= amplitudo */
interior vacuum
_sectio_organum (
    CredoContextus* c)
{
     constans TibiaParametra* p = tibia_praeparatum("organum");
                         f64  a1;
                         i32  k;

    (vacuum)c;
    CREDO_NECESSE (p != NIHIL
                   && (s32)(p->genus + 0.5f) == TIBIA_PARTIALES);
    _sonare(p, 57.0f, tabula_a, SECUNDA);
    /* A3 = 220 Hz; fenestra 0.5 s (110 cycli integri) */
    a1 = _amplitudo_frequentiae(tabula_a, SECUNDA / IV, SECUNDA / II,
                                220.0);
    CREDO_VERUM (a1 > 0.05);
    per (k = I; k < IV; k++)
    {
        f64 inventa = _amplitudo_frequentiae(tabula_a, SECUNDA / IV,
            SECUNDA / II, 220.0 * (f64)(k + I));
        f64 exspectata = (f64)p->partiales[k].amplitudo
               / (f64)p->partiales[ZEPHYRUM].amplitudo;

        CREDO_VERUM (fabs(inventa / a1 - exspectata) < 0.05);
    }
    CREDO_VERUM (_amplitudo_frequentiae(tabula_a, SECUNDA / IV,
        SECUNDA / II, 330.0) < 0.01 * a1);
    /* tenet: efficax idem in 0.4 et 0.8 s */
    CREDO_VERUM (fabs(20.0 * log10(_efficax(tabula_a, SECUNDA * II / V,
        SECUNDA / X) / _efficax(tabula_a, SECUNDA * IV / V,
        SECUNDA / X))) < 0.5);
    CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, SECUNDA) <= p->amplitudo
        + 1.0e-4f);
}

/* marimba: partialis 3.9 citius cadit quam fundamentalis; fundamentalis
 * ad 1/e post decrementum suum (+-20%); determinismus */
interior vacuum
_sectio_lignea (
    CredoContextus* c)
{
     constans TibiaParametra* p = tibia_praeparatum("marimba");
                         f64  f = 261.6255653;   /* C4 */
                         f64  f1_primus;
                         f64  f2_primus;
                         f64  f1_serus;
                         f64  f2_serus;
                         f64  fund;
                         i32  t;

    (vacuum)c;
    CREDO_NECESSE (p != NIHIL);
    _sonare(p, 60.0f, tabula_a, TABULA_MAXIMA);
    f1_primus = _amplitudo_frequentiae(tabula_a, SECUNDA / C,
                                        SECUNDA / XX, f);
    f2_primus = _amplitudo_frequentiae(tabula_a, SECUNDA / C,
                                        SECUNDA / XX, 3.9 * f);
    f1_serus  = _amplitudo_frequentiae(tabula_a, SECUNDA / V,
                                        SECUNDA / XX, f);
    f2_serus  = _amplitudo_frequentiae(tabula_a, SECUNDA / V,
                                        SECUNDA / XX, 3.9 * f);
    CREDO_VERUM (f2_primus > 0.0 && f1_serus > 0.0);
    CREDO_VERUM (f2_serus / f1_serus < 0.5 * (f2_primus / f1_primus));
    t    = (i32)(p->partiales[ZEPHYRUM].decrementum * (f32)SECUNDA);
    fund = _amplitudo_frequentiae(tabula_a, SECUNDA / C + t,
                                  SECUNDA / XX, f) / f1_primus;
    CREDO_VERUM (fund > 0.8 / 2.718281828 && fund < 1.2 / 2.718281828);
    _sonare(p, 60.0f, tabula_b, TABULA_MAXIMA);
    CREDO_VERUM (memcmp(tabula_a, tabula_b, magnitudo(tabula_a))
                 == ZEPHYRUM);
}

/* glockenspiel in C8: partialis 8.93 supra Nyquist omittitur - finita,
 * apex <= amplitudo */
interior vacuum
_sectio_campanulae (
    CredoContextus* c)
{
     constans TibiaParametra* p = tibia_praeparatum("glockenspiel");
                         i32  i;
                         b32  finita = VERUM;

    (vacuum)c;
    CREDO_NECESSE (p != NIHIL);
    _sonare(p, 108.0f, tabula_a, SECUNDA);
    per (i = ZEPHYRUM; i < SECUNDA; i++)
    {
        finita = finita && tabula_a[i] == tabula_a[i]
                 && fabs(tabula_a[i]) < 10.0;
    }
    CREDO_VERUM (finita);
    CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, SECUNDA) <= p->amplitudo
        + 1.0e-4f);
    CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, SECUNDA) > 0.05f);
    /* sine aliasing: 8.93 x C8 (37.4 kHz) non reflectitur ad
     * 48 kHz - 37.4 kHz */
    {
        f64 f = 4186.009045;
        f64 fundamentum = _amplitudo_frequentiae(tabula_a, ZEPHYRUM,
            SECUNDA / XX, f);

        CREDO_VERUM (fundamentum > 0.05);
        CREDO_VERUM (_amplitudo_frequentiae(tabula_a, ZEPHYRUM,
            SECUNDA / XX, (f64)SECUNDA - 8.93 * f)
            < 0.01 * fundamentum);
    }
}

/* PARTIALES sine partialibus datis: fundamentalis sola (sinus) */
interior vacuum
_sectio_partiales_nulli (
    CredoContextus* c)
{
    TibiaParametra p;

    (vacuum)c;
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.genus             = (f32)TIBIA_PARTIALES;
    p.involucrum.tenor  = 1.0f;
    p.amplitudo         = 0.5f;
    _sonare(&p, 57.0f, tabula_a, SECUNDA);
    CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, SECUNDA) > 0.49f);
    CREDO_VERUM (_amplitudo_frequentiae(tabula_a, SECUNDA / IV,
        SECUNDA / II, 220.0) > 0.49);
    CREDO_VERUM (_amplitudo_frequentiae(tabula_a, SECUNDA / IV,
        SECUNDA / II, 440.0) < 0.001);
}

/* harmonicus fortissimus (1..n) notae f in fenestra 0.25..0.75 s */
interior i32
_harmonicus_fortissimus (
    constans f32* x,
             f64  f,
             i32  n)
{
    i32 h;
    i32 optimus = I;
    f64 maximum = -1.0;

    per (h = I; h <= n; h++)
    {
        f64 a = _amplitudo_frequentiae(x, SECUNDA / IV, SECUNDA / II,
                                       f * (f64)h);

        si (a > maximum)
        {
            maximum = a;
            optimus = h;
        }
    }
    redde optimus;
}

/* fractio energiae harmonicorum supra 1 kHz (harmonici 1..XL) */
interior f64
_fractio_acuta (
    constans f32* x,
             f64  f)
{
    f64 summa = 0.0;
    f64 acuta = 0.0;
    i32 h;

    per (h = I; h <= XL; h++)
    {
        f64 a = _amplitudo_frequentiae(x, SECUNDA / IV, SECUNDA / II,
                                       f * (f64)h);

        summa += a * a;
        si (f * (f64)h > 1000.0)
        {
            acuta += a * a;
        }
    }
    redde summa > 0.0 ? acuta / summa : 0.0;
}

/* efficax per colum resonans (genus, f, Q) in fenestra */
interior f64
_efficax_per_colum (
            constans f32* x,
                     i32  initium,
                     i32  numerus,
    SonusGenusResonantis  genus,
                     f32  frequentia,
                     f32  acumen)
{
    SonusResonans r;
              f64 s = 0.0;
              i32 i;

    sonus_resonans_parare(&r, genus, frequentia, acumen);
    per (i = ZEPHYRUM; i < initium + numerus; i++)
    {
        f32 y = sonus_resonans_proximum(&r, x[i]);

        si (i >= initium)
        {
            s += (f64)y * (f64)y;
        }
    }
    redde sqrt(s / (f64)numerus);
}

/* schola: vocalis a - harmonicus III (660 Hz, F1 650) fortissimus,
 * u - I aut II; a plus acutorum quam u; vocalis 3.5 inter o et u;
 * apex finitus per ambitum; determinismus */
interior vacuum
_sectio_schola (
    CredoContextus* c)
{
    constans TibiaParametra* p = tibia_praeparatum("schola");
             TibiaParametra  q;
                        f64  fa;
                        f64  fu;
                        i32  h;
                        i32  k;
               constans f32  notae[III] = { 48.0f, 60.0f, 74.0f };

    (vacuum)c;
    CREDO_NECESSE (p != NIHIL
                   && (s32)(p->genus + 0.5f) == TIBIA_FORMANTES);
    q           = *p;
    q.flatus    = 0.0f;
    q.vibratus  = 0.0f;
    _sonare(&q, 57.0f, tabula_a, SECUNDA);
    h   = _harmonicus_fortissimus(tabula_a, 220.0, VIII);
    fa  = _fractio_acuta(tabula_a, 220.0);
    CREDO_AEQUALIS_I32 (h, III);
    q.vocalis = 4.0f;
    _sonare(&q, 57.0f, tabula_a, SECUNDA);
    h   = _harmonicus_fortissimus(tabula_a, 220.0, VIII);
    fu  = _fractio_acuta(tabula_a, 220.0);
    CREDO_VERUM (h <= II);
    CREDO_VERUM (fa > 2.0 * fu);
    /* vocalis 3.5: F2 medius inter o (800, latitudo 80, -10 dB) et u
     * (600, 60, -20): 700 cycli/s, Q 10, -15 dB. (Fractio acuta non
     * monotona: harmonici in cola angusta cadunt aut non) */
    {
             TibiaVox v;
        SonusResonans r;

        q.vocalis = 3.5f;
        tibia_vocem_parare(&v, &q, 57.0f);
        sonus_resonans_parare(&r, SONUS_RESONANS_MEDIA, 700.0f, 10.0f);
        CREDO_VERUM (   v.formantes[I].b0 == r.b0
                     && v.formantes[I].a1 == r.a1
                     && v.formantes[I].a2 == r.a2);
        CREDO_F32_PROXIMUS (v.lucra_formantium[I]
            / v.lucra_formantium[ZEPHYRUM], 0.17783f, 0.0001f);
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        _sonare(p, notae[k], tabula_a, SECUNDA);
        CREDO_VERUM (_apex(tabula_a, ZEPHYRUM, SECUNDA)
                     < 1.2f * p->amplitudo);
        CREDO_VERUM (_efficax(tabula_a, SECUNDA / II, SECUNDA / IV)
                     > 0.05 * (f64)p->amplitudo);
    }
    _sonare(p, 57.0f, tabula_a, SECUNDA);
    _sonare(p, 57.0f, tabula_b, SECUNDA);
    CREDO_VERUM (memcmp(tabula_a, tabula_b, magnitudo(tabula_a))
                 == ZEPHYRUM);
}

/* fistula: flatus = strepitus additus (residuum contra flatum 0)
 * mensura flatus, circa notam (plus ad f quam ad 6f);
 * deterministicum */
interior vacuum
_sectio_fistula (
    CredoContextus* c)
{
    constans TibiaParametra* p = tibia_praeparatum("fistula");
             TibiaParametra  q;
                        f64  sonus_purus;
                        f64  residuum;
                        f64  ad_notam;
                        f64  procul;
                        i32  i;

    (vacuum)c;
    CREDO_NECESSE (p != NIHIL && p->flatus > 0.0f);
    q         = *p;
    q.flatus  = 0.0f;
    _sonare(&q, 69.0f, tabula_b, SECUNDA);
    _sonare(p, 69.0f, tabula_a, SECUNDA);
    sonus_purus = _efficax(tabula_b, SECUNDA / II, SECUNDA / IV);
    per (i = ZEPHYRUM; i < SECUNDA; i++)
    {
        tabula_a[i] -= (1.0f - p->flatus) * tabula_b[i];
    }
    residuum = _efficax(tabula_a, SECUNDA / II, SECUNDA / IV);
    /* residuum strepitus est, non sonus scalatus: incorrelatum */
    {
        f64 productum = 0.0;

        per (i = SECUNDA / II; i < SECUNDA / II + SECUNDA / IV; i++)
        {
            productum += (f64)tabula_a[i] * (f64)tabula_b[i];
        }
        productum /= (f64)(SECUNDA / IV) * residuum * sonus_purus;
        CREDO_VERUM (fabs(productum) < 0.2);
    }
    CREDO_VERUM (residuum > 0.5 * (f64)p->flatus * sonus_purus);
    CREDO_VERUM (residuum < 2.0 * (f64)p->flatus * sonus_purus);
    ad_notam = _efficax_per_colum(tabula_a, SECUNDA / II, SECUNDA / IV,
                                  SONUS_RESONANS_MEDIA, 440.0f, 4.0f);
    procul   = _efficax_per_colum(tabula_a, SECUNDA / II, SECUNDA / IV,
                                  SONUS_RESONANS_MEDIA, 2640.0f, 4.0f);
    CREDO_VERUM (ad_notam > 2.0 * procul);
    _sonare(p, 69.0f, tabula_a, SECUNDA);
    _sonare(p, 69.0f, tabula_b, SECUNDA);
    CREDO_VERUM (memcmp(tabula_a, tabula_b, magnitudo(tabula_a))
                 == ZEPHYRUM);
}

/* nubes: colum tenore apertum - fractio acuta (supra 1.2 kHz) serius
 * maior; motus 0 cum limite = obscurior quam sine limite */
interior vacuum
_sectio_nubes (
    CredoContextus* c)
{
    constans TibiaParametra* p = tibia_praeparatum("nubes");
             TibiaParametra  q;
                        f64  primum;
                        f64  serum;
                        f64  apertum;
                        f64  clausum;

    (vacuum)c;
    CREDO_NECESSE (p != NIHIL && p->resonans > 0.0f && p->motus > 0.0f);
    _sonare(p, 45.0f, tabula_a, TABULA_MAXIMA);
    primum = _efficax_per_colum(tabula_a, SECUNDA / XX, SECUNDA / X,
                 SONUS_RESONANS_ACUTA, 1200.0f, 0.7071f)
           / _efficax(tabula_a, SECUNDA / XX, SECUNDA / X);
    serum  = _efficax_per_colum(tabula_a, SECUNDA, SECUNDA / X,
                 SONUS_RESONANS_ACUTA, 1200.0f, 0.7071f)
           / _efficax(tabula_a, SECUNDA, SECUNDA / X);
    CREDO_VERUM (serum > 2.0 * primum);
    q                     = *p;
    q.motus               = 0.0f;
    q.involucrum.impetus  = 0.0f;
    _sonare(&q, 45.0f, tabula_a, SECUNDA);
    clausum = _efficax_per_colum(tabula_a, SECUNDA / II, SECUNDA / IV,
                  SONUS_RESONANS_ACUTA, 1200.0f, 0.7071f)
            / _efficax(tabula_a, SECUNDA / II, SECUNDA / IV);
    q.resonans  = 0.0f;
    _sonare(&q, 45.0f, tabula_a, SECUNDA);
    apertum = _efficax_per_colum(tabula_a, SECUNDA / II, SECUNDA / IV,
                  SONUS_RESONANS_ACUTA, 1200.0f, 0.7071f)
            / _efficax(tabula_a, SECUNDA / II, SECUNDA / IV);
    CREDO_VERUM (apertum > 3.0 * clausum);
    /* acumen: resonantia ad limitem (CD cycli/s) Q 4 contra 0.707 */
    {
        f64 acutum;
        f64 planum;

        q.resonans  = p->resonans;
        _sonare(&q, 45.0f, tabula_a, SECUNDA);
        acutum = _efficax_per_colum(tabula_a, SECUNDA / II, SECUNDA
            / IV,
                     SONUS_RESONANS_MEDIA, 400.0f, 4.0f);
        q.acumen    = 0.7071f;
        _sonare(&q, 45.0f, tabula_a, SECUNDA);
        planum = _efficax_per_colum(tabula_a, SECUNDA / II, SECUNDA
            / IV,
                     SONUS_RESONANS_MEDIA, 400.0f, 4.0f);
        CREDO_VERUM (acutum > 2.0 * planum);
    }
}

/* genus supra FORMANTES coercetur ad FORMANTES */
interior vacuum
_sectio_genus_coercitum (
    CredoContextus* c)
{
    TibiaParametra q = *tibia_praeparatum("schola");

    (vacuum)c;
    _sonare(&q, 57.0f, tabula_a, SECUNDA / X);
    q.genus = 9.0f;
    _sonare(&q, 57.0f, tabula_b, SECUNDA / X);
    CREDO_VERUM (memcmp(tabula_a, tabula_b, magnitudo(f32) * (size_t)(
        SECUNDA / X)) == ZEPHYRUM);
    q.genus = 0.0f;
    _sonare(&q, 57.0f, tabula_b, SECUNDA / X);
    CREDO_VERUM (memcmp(tabula_a, tabula_b, magnitudo(f32) * (size_t)(
        SECUNDA / X)) != ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tenet", _sectio_tenet, _parare, NIHIL, NIHIL, NIHIL },
    { "involucrum", _sectio_involucrum, _parare, NIHIL, NIHIL, NIHIL },
    { "motus", _sectio_motus, _parare, NIHIL, NIHIL, NIHIL },
    { "vibratus", _sectio_vibratus, _parare, NIHIL, NIHIL, NIHIL },
    { "color", _sectio_color, _parare, NIHIL, NIHIL, NIHIL },
    { "praeparata", _sectio_praeparata, _parare, NIHIL, NIHIL, NIHIL },
    { "vetera", _sectio_vetera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "chorda", _sectio_chorda, _parare, NIHIL, NIHIL, NIHIL },
    { "claritas", _sectio_claritas, _parare, NIHIL, NIHIL, NIHIL },
    { "campana", _sectio_campana, _parare, NIHIL, NIHIL, NIHIL },
    { "organum", _sectio_organum, _parare, NIHIL, NIHIL, NIHIL },
    { "marimba", _sectio_lignea, _parare, NIHIL, NIHIL, NIHIL },
    { "glockenspiel", _sectio_campanulae, _parare, NIHIL, NIHIL,
        NIHIL },
    { "partiales_nulli", _sectio_partiales_nulli, _parare, NIHIL,
        NIHIL, NIHIL },
    { "schola", _sectio_schola, _parare, NIHIL, NIHIL, NIHIL },
    { "fistula", _sectio_fistula, _parare, NIHIL, NIHIL, NIHIL },
    { "nubes", _sectio_nubes, _parare, NIHIL, NIHIL, NIHIL },
    { "genus_coercitum", _sectio_genus_coercitum, _parare, NIHIL,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tibia", SECTIONES);
}
