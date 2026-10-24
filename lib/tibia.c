/* tibia.c - Instrumentum (vide include/tibia.h)
 *
 * Frequentia undae per exemplum computatur SOLUM dum nota movetur
 * (glissandum aut vibratus); aliter semel, in parare et in nota statim
 * posita - sinus/potentia per exemplum sumptus est quem nota tenta non
 * eget. Via eadem offline et in mixtura: exempla octetim eadem.
 */
#include "tibia.h"
#include "sors.h"
#include <math.h>
#include <string.h>

#define TIBIA_DUO_PI    6.28318530717958647692
#define TIBIA_CIRCULUS  4294967296.0   /* 2^32: phasis cycli unius */

nomen structura {
     constans character* titulus;
         TibiaParametra  parametra;
} TibiaPraeparatum;

/* forma, officium, involucrum {impetus, lapsus, tenor, remissio},
 * vibratus, celeritas, gravia, amplitudo; genus, decrementum,
 * claritas, ratio, index; partiales; vocalis, flatus, resonans,
 * acumen, motus */
hic_manens constans TibiaPraeparatum tibiae_praeparata[] = {
    /* aetherophonum: sinus, introitus et exitus lenes, vibratus
     * parvus (manus tremens) */
    { "aether",     { 3.0f, 0.5f,  { 0.04f,  0.0f,  1.0f, 0.25f },
                      0.12f, 5.5f, 0.0f, 0.5f,
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      { { 0.0f, 0.0f, 0.0f } },
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f } },
    /* chiptune: pulsus quartae, mollitus supra VI milia */
    { "quadrans",   { 0.0f, 0.25f, { 0.002f, 0.08f, 0.7f, 0.08f },
                      0.0f, 0.0f, 6000.0f, 0.35f,
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      { { 0.0f, 0.0f, 0.0f } },
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f } },
    { "triangulum", { 2.0f, 0.5f,  { 0.002f, 0.05f, 0.9f, 0.05f },
                      0.0f, 0.0f, 0.0f, 0.6f,
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      { { 0.0f, 0.0f, 0.0f } },
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f } },
    /* Karplus-Strong: involucrum plenum (chorda ipsa decrescit), manu
     * tacta remittit; -60 dB in 2.5 s (citharae similis), claritas
     * media; amplitudo plena - impulsus cito cadit */
    { "chorda",     { 0.0f, 0.5f,  { 0.0f,   0.0f,  1.0f, 0.08f },
                      0.0f, 0.0f, 0.0f, 1.0f,
                      1.0f, 2.5f, 0.5f, 0.0f, 0.0f,
                      { { 0.0f, 0.0f, 0.0f } },
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f } },
    /* FM: ratio 1.4 (inharmonica), index VI decrescens (1/e in 0.4 s),
     * impetus 2 ms, lapsus longus sine tenore */
    { "campana",    { 0.0f, 0.5f,  { 0.002f, 3.0f,  0.0f, 1.0f },
                      0.0f, 0.0f, 0.0f, 0.45f,
                      2.0f, 0.4f, 0.0f, 1.4f, 6.0f,
                      { { 0.0f, 0.0f, 0.0f } },
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f } },
    /* organum: registra 8' 4' 2 2/3' 2' (rationes 1 2 3 4), tenet;
     * impetus 30 ms (aer in fistula) */
    { "organum",    { 0.0f, 0.5f,  { 0.03f,  0.0f,  1.0f, 0.15f },
                      0.0f, 0.0f, 0.0f, 0.4f,
                      3.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      { { 1.0f, 1.0f, 0.0f }, { 2.0f, 0.6f, 0.0f },
                        { 3.0f, 0.4f, 0.0f },
                        { 4.0f, 0.3f, 0.0f } },
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f } },
    /* marimba: lamina accommodata 1 : 3.9 : 9.2, superiores citius
     * cadunt */
    { "marimba",    { 0.0f, 0.5f,  { 0.001f, 0.0f,  1.0f, 0.3f },
                      0.0f, 0.0f, 0.0f, 0.7f,
                      3.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      { { 1.0f, 1.0f, 0.6f }, { 3.9f, 0.5f, 0.15f },
                        { 9.2f, 0.2f, 0.05f } },
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f } },
    /* glockenspiel: lamina libera 1 : 2.76 : 5.40 : 8.93, longa */
    { "glockenspiel", { 0.0f, 0.5f, { 0.001f, 0.0f, 1.0f, 1.5f },
                      0.0f, 0.0f, 0.0f, 0.4f,
                      3.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      { { 1.0f, 1.0f, 1.5f }, { 2.76f, 0.6f, 0.8f },
                        { 5.40f, 0.3f, 0.4f },
                        { 8.93f, 0.15f, 0.2f } },
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f } },
    /* schola: serra per formantes vocalis a (tenor), impetus 80 ms,
     * vibratus lenis, flatus parvus (aer in voce) */
    { "schola",     { 1.0f, 0.5f,  { 0.08f,  0.0f,  1.0f, 0.25f },
                      0.12f, 5.0f, 0.0f, 0.5f,
                      4.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      { { 0.0f, 0.0f, 0.0f } },
                      0.0f, 0.03f, 0.0f, 0.0f, 0.0f } },
    /* fistula: partiales 1 2 3 debiles, flatus circa notam, impetus
     * 60 ms, vibratus parvus */
    { "fistula",    { 0.0f, 0.5f,  { 0.06f,  0.0f,  1.0f, 0.12f },
                      0.08f, 5.0f, 0.0f, 0.45f,
                      3.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      { { 1.0f, 1.0f, 0.0f }, { 2.0f, 0.25f, 0.0f },
                        { 3.0f, 0.08f, 0.0f } },
                      0.0f, 0.15f, 0.0f, 0.0f, 0.0f } },
    /* nubes: serra, impetus 0.8 s, colum resonans a CD cycli/s (Q 4)
     * per II octavas tenore apertum; apex ~2.3 x amplitudo
     * (resonantia) */
    { "nubes",      { 1.0f, 0.5f,  { 0.8f,   0.0f,  1.0f, 1.5f },
                      0.0f, 0.0f, 0.0f, 0.25f,
                      0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                      { { 0.0f, 0.0f, 0.0f } },
                      0.0f, 0.0f, 400.0f, 4.0f, 24.0f } }
};

#define TIBIAE_PRAEPARATA                                           \
    (magnitudo(tibiae_praeparata)                                   \
     / magnitudo(tibiae_praeparata[ZEPHYRUM]))

/* forma rotundata in [PULSUS, SINUS] - strepitus notam non habet */
interior SonusFormaUndae
_forma (
    f32 forma)
{
    s32 k = (s32)(forma + 0.5f);

    si (forma < 0.0f || k < ZEPHYRUM)
    {
        k = ZEPHYRUM;
    }
    si (k > (s32)SONUS_UNDA_SINUS)
    {
        k = (s32)SONUS_UNDA_SINUS;
    }
    redde (SonusFormaUndae)k;
}

/* genus rotundatum in [UNDA, FM] */
interior TibiaGenus
_genus (
    f32 genus)
{
    s32 k = (s32)(genus + 0.5f);

    si (genus < 0.0f || k < ZEPHYRUM)
    {
        k = ZEPHYRUM;
    }
    si (k > (s32)TIBIA_FORMANTES)
    {
        k = (s32)TIBIA_FORMANTES;
    }
    redde (TibiaGenus)k;
}

/* frequentia: unda (UNDA, FM portans); CHORDA longitudo morae (ad
 * lineam coercita) */
interior vacuum
_frequentiam_ponere (
    TibiaVox* v,
         f32  nota)
{
    f32 f = sonus_notae_frequentia(nota);

    sonus_unda_frequentiam_ponere(&v->unda, f);
    si (v->genus == TIBIA_CHORDA)
    {
        f32 mora = f > 0.0f ? (f32)SONUS_FREQUENTIA / f
                            : (f32)(TIBIA_LINEA - III);

        v->mora = mora > (f32)(TIBIA_LINEA - III)
                ? (f32)(TIBIA_LINEA - III) : (mora
                    < 2.0f ? 2.0f : mora);
    }
}

/* CHORDA: linea impulsu strepitus impleta (sors ex nota - eadem nota
 * eadem exempla), media nulla; detrimentum per cyclum ex decremento */
interior vacuum
_chordam_parare (
                     TibiaVox* v,
    constans TibiaParametra* p,
                          f32  nota)
{
    Sors sors;
     f64 media = 0.0;
     i32 n;
     i32 i;
     f32 f = sonus_notae_frequentia(nota);
     f32 t = p->decrementum > 0.0f ? p->decrementum : 1.5f;

    n = (i32)v->mora + II;
    sors_seminare(&sors, (i64)(nota * 1000.0f) ^ 0x6368ULL, ZEPHYRUM);
    per (i = ZEPHYRUM; i < n; i++)
    {
        v->linea[i]  = (f32)sors_proximum(&sors) / 2147483648.0f - 1.0f;
        media        += (f64)v->linea[i];
    }
    media /= (f64)n;
    per (i = ZEPHYRUM; i < n; i++)
    {
        v->linea[i] -= (f32)media;
    }
    v->scriptura    = n;
    v->claritas     = p->claritas < 0.0f ? 0.0f
                    : (p->claritas > 1.0f ? 1.0f : p->claritas);
    /* -60 dB post t secundas (f*t cycli), pensatis filtro et
     * interpolatione (ambo fundamentalem paulum minuunt) */
    si (f > 0.0f)
    {
        f64 w   = TIBIA_DUO_PI * (f64)f / (f64)SONUS_FREQUENTIA;
        f64 a   = 0.5 + 0.5 * (f64)v->claritas;
        f64 d   = (f64)v->mora - (1.0 - a);
        f64 fr  = d - floor(d);
        f64 lucrum_filtri  = sqrt(a * a + (1.0 - a) * (1.0 - a)
                       + 2.0 * a * (1.0 - a) * cos(w));
        f64 lucrum_fractionis  = sqrt((1.0 - fr) * (1.0 - fr) + fr * fr
                       + 2.0 * fr * (1.0 - fr) * cos(w));
        f64 g   = pow(10.0, -3.0 / ((f64)t * (f64)f))
            / (lucrum_filtri * lucrum_fractionis);

        v->detrimentum = (f32)(g < 0.99999 ? g : 0.99999);
    }
}

/* linea in loco fracto (linealiter); locus intra [0, TIBIA_LINEA) */
interior f32
_lineam_legere (
    constans TibiaVox* v,
                  f32  locus)
{
    i32 i;
    f32 fr;

    dum (locus < 0.0f)
    {
        locus += (f32)TIBIA_LINEA;
    }
    i   = (i32)locus;
    fr  = locus - (f32)i;
    redde (1.0f - fr) * v->linea[i % TIBIA_LINEA]
        + fr * v->linea[(i + I) % TIBIA_LINEA];
}

/* CHORDA: y = g (a x[n-D] + (1 - a) x[n-D-1]), a = 1/2 + claritas/2;
 * D = mora - (1 - a) ut periodus tota = mora (filtrum (1-a) moratur) */
interior f32
_chordae_proximum (
    TibiaVox* v)
{
    f32 a      = 0.5f + 0.5f * v->claritas;
    f32 locus  = (f32)v->scriptura - (v->mora - (1.0f - a));
    f32 y = v->detrimentum * (a * _lineam_legere(v, locus)
          + (1.0f - a) * _lineam_legere(v, locus - 1.0f));

    v->linea[v->scriptura]  = y;
    v->scriptura            = (v->scriptura + I) % TIBIA_LINEA;
    redde y;
}

/* FM: sin(portans + index sin(modulans)); modulans = ratio x portans;
 * index decrescit per exemplum */
interior f32
_modulationis_proximum (
    TibiaVox* v)
{
    f64 portans   = (f64)v->unda.phasis / TIBIA_CIRCULUS;
    f64 modulans  = (f64)v->phasis_modulans / TIBIA_CIRCULUS;
    f32 x = (f32)sin(TIBIA_DUO_PI * portans + (f64)v->index
            * sin(TIBIA_DUO_PI * modulans));

    v->unda.phasis      += v->unda.incrementum;
    v->phasis_modulans  += (i32)((f32)v->unda.incrementum * v->ratio);
    v->index            *= v->index_detrimentum;
    redde x;
}

/* PARTIALES: activi (ratio > 0, amplitudo > 0) in ordinem collecti;
 * amplitudines per summam divisae (apex <= 1); detrimentum per
 * exemplum ex decremento (0 = tenet). Nulli dati = fundamentalis
 * sola */
interior vacuum
_partiales_parare (
                     TibiaVox* v,
    constans TibiaParametra* p)
{
    f32 summa  = 0.0f;
    i32 n      = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < TIBIA_PARTIALES_NUMERUS; k++)
    {
        constans TibiaPartialis* q = &p->partiales[k];

        si (q->ratio > 0.0f && q->amplitudo > 0.0f)
        {
            v->rationes[n]      = q->ratio;
            v->amplitudines[n]  = q->amplitudo;
            v->detrimenta[n]    = q->decrementum > 0.0f
                ? (f32)exp(-1.0 / ((f64)q->decrementum
                                   * (f64)SONUS_FREQUENTIA)) : 1.0f;
            summa += q->amplitudo;
            n++;
        }
    }
    si (n == ZEPHYRUM)
    {
        v->rationes[ZEPHYRUM]      = 1.0f;
        v->amplitudines[ZEPHYRUM]  = 1.0f;
        v->detrimenta[ZEPHYRUM]    = 1.0f;

        summa  = 1.0f;
        n      = I;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        v->amplitudines[k] /= summa;
    }
    v->numerus_partialium = n;
}

/* PARTIALES: summa sinuum; phasis quaeque = incrementum undae x
 * ratio (glissandum et vibratus sequuntur). Partialis supra Nyquist
 * omittitur (phasis stat), sed decrescit ut ceteri */
interior f32
_partialium_proximum (
    TibiaVox* v)
{
    f64 x = 0.0;
    i32 k;

    per (k = ZEPHYRUM; k < v->numerus_partialium; k++)
    {
        f64 incrementum = (f64)v->unda.incrementum
                        * (f64)v->rationes[k];

        si (incrementum < TIBIA_CIRCULUS / 2.0)
        {
            x += (f64)v->amplitudines[k] * sin(TIBIA_DUO_PI
                 * (f64)v->phases[k] / TIBIA_CIRCULUS);
            v->phases[k] += (i32)incrementum;
        }
        v->amplitudines[k] *= v->detrimenta[k];
    }
    redde (f32)x;
}

/* formantes tenoris (Csound, "Formant Values" appendix): frequentia
 * (cycli/s), lucrum (dB ad primum), latitudo (cycli/s) */
nomen structura {
    f32 frequentia[TIBIA_FORMANTES_NUMERUS];
    f32 decibelia[TIBIA_FORMANTES_NUMERUS];
    f32 latitudo[TIBIA_FORMANTES_NUMERUS];
} TibiaVocalis;

hic_manens constans TibiaVocalis VOCALES[V] = {
    /* a */
    { { 650.0f, 1080.0f, 2650.0f, 2900.0f, 3250.0f },
      { 0.0f, -6.0f, -7.0f, -8.0f, -22.0f },
      { 80.0f, 90.0f, 120.0f, 130.0f, 140.0f } },
    /* e */
    { { 400.0f, 1700.0f, 2600.0f, 3200.0f, 3580.0f },
      { 0.0f, -14.0f, -12.0f, -14.0f, -20.0f },
      { 70.0f, 80.0f, 100.0f, 120.0f, 120.0f } },
    /* i */
    { { 290.0f, 1870.0f, 2800.0f, 3250.0f, 3540.0f },
      { 0.0f, -15.0f, -18.0f, -20.0f, -30.0f },
      { 40.0f, 90.0f, 100.0f, 120.0f, 120.0f } },
    /* o */
    { { 400.0f, 800.0f, 2600.0f, 2800.0f, 3000.0f },
      { 0.0f, -10.0f, -12.0f, -12.0f, -26.0f },
      { 40.0f, 80.0f, 100.0f, 120.0f, 120.0f } },
    /* u */
    { { 350.0f, 600.0f, 2700.0f, 2900.0f, 3300.0f },
      { 0.0f, -20.0f, -17.0f, -14.0f, -26.0f },
      { 40.0f, 60.0f, 100.0f, 120.0f, 120.0f } }
};

/* lucrum formantium commune: serra per cola media angusta multum
 * attenuatur (efficax ~0.1 x amplitudo sine eo); 2.4 = efficax
 * ~0.25 x amplitudo, apex < 1.2 x amplitudo per C3..D5 (mensuratum,
 * vide worklog) */
#define TIBIA_FORMANTIUM_LUCRUM 2.4f

/* FORMANTES: vocalis in [0, 4]; fractio inter vicinas linealiter
 * (frequentia, dB, latitudo) */
interior vacuum
_formantes_parare (
                     TibiaVox* v,
      constans TibiaParametra* p)
{
    f32 vocalis = p->vocalis < 0.0f ? 0.0f
                : (p->vocalis > 4.0f ? 4.0f : p->vocalis);
    i32 a = (i32)vocalis;
    i32 b = a < IV ? a + I : IV;
    f32 t = vocalis - (f32)a;
    i32 k;

    per (k = ZEPHYRUM; k < TIBIA_FORMANTES_NUMERUS; k++)
    {
        f32 f  = (1.0f - t) * VOCALES[a].frequentia[k]
               + t * VOCALES[b].frequentia[k];
        f32 db = (1.0f - t) * VOCALES[a].decibelia[k]
               + t * VOCALES[b].decibelia[k];
        f32 l  = (1.0f - t) * VOCALES[a].latitudo[k]
               + t * VOCALES[b].latitudo[k];

        sonus_resonans_parare(&v->formantes[k], SONUS_RESONANS_MEDIA,
                              f, f / l);
        v->lucra_formantium[k] = TIBIA_FORMANTIUM_LUCRUM
            * (f32)pow(10.0, (f64)db / 20.0);
    }
}

/* FORMANTES: unda per cola media parallela */
interior f32
_formantium_proximum (
    TibiaVox* v)
{
    f32 x = sonus_unda_proximum(&v->unda);
    f32 y = 0.0f;
    i32 k;

    per (k = ZEPHYRUM; k < TIBIA_FORMANTES_NUMERUS; k++)
    {
        y += v->lucra_formantium[k]
            * sonus_resonans_proximum(&v->formantes[k], x);
    }
    redde y;
}

/* FLATUS: colum medium circa notam (Q II); lucrum ut strepitus
 * aequalis (efficax 1/sqrt 3) post colum efficacem sinus (1/sqrt 2)
 * habeat: latitudo aequivalens strepitus = pi/2 x f/Q */
#define TIBIA_FLATUS_ACUMEN 2.0f

interior vacuum
_flatum_ponere (
    TibiaVox* v)
{
    f64 f = (f64)sonus_notae_frequentia(v->nota);
    f64 l = f / (f64)TIBIA_FLATUS_ACUMEN;

    l = l < 20.0 ? 20.0 : l;
    sonus_resonans_ponere(&v->colum_flatus, (f32)f,
        TIBIA_FLATUS_ACUMEN);
    v->lucrum_flatus = (f32)(0.70710678118654752
        / (0.57735026918962576 * sqrt(TIBIA_DUO_PI / 2.0 * l
                                      / (f64)SONUS_FREQUENTIA)));
}

/* cola viva per XXXII exempla: flatus notam sequitur, limes
 * resonantis involucrum (motus semitoniis) */
interior vacuum
_moderari (
    TibiaVox* v)
{
    si (v->reliqua_moderationis > ZEPHYRUM)
    {
        v->reliqua_moderationis--;
        redde;
    }
    v->reliqua_moderationis = XXXI;
    si (v->flatus > 0.0f)
    {
        _flatum_ponere(v);
    }
    si (v->resonans > 0.0f)
    {
        sonus_resonans_ponere(&v->colum_resonans, v->resonans
            * (f32)pow(2.0, (f64)(v->motus * v->involucrum_ultimum)
                       / 12.0), v->acumen);
    }
}

vacuum
tibia_vocem_parare (
                 TibiaVox* v,
    constans TibiaParametra* p,
                      f32  nota)
{
    f64 incrementum;

    memset(v, ZEPHYRUM, magnitudo(TibiaVox));
    v->genus = _genus(p->genus);
    sonus_unda_parare(&v->unda, _forma(p->forma), ZEPHYRUM);
    sonus_unda_officium_ponere(&v->unda, p->officium);
    sonus_involucrum_parare(&v->involucrum, &p->involucrum);
    sonus_involucrum_aperire(&v->involucrum);
    sonus_colum_parare(&v->colum, SONUS_COLUM_GRAVIA, p->gravia);
    v->amplitudo  = p->amplitudo;
    v->vibratus   = p->vibratus;
    incrementum   = (f64)p->vibratus_celeritas / (f64)SONUS_FREQUENTIA
                  * TIBIA_CIRCULUS;
    v->incrementum_vibrati = incrementum > 0.0 && incrementum
        < TIBIA_CIRCULUS ? (i32)incrementum : ZEPHYRUM;
    v->nota    = nota;
    v->meta    = nota;
    v->aperta  = VERUM;
    _frequentiam_ponere(v, nota);
    si (v->genus == TIBIA_CHORDA)
    {
        _chordam_parare(v, p, nota);
    }
    alioquin si (v->genus == TIBIA_MODULATIO)
    {
        f32 t = p->decrementum > 0.0f ? p->decrementum : 1.0f;

        v->ratio = p->ratio;
        v->index = p->index;
        v->index_detrimentum  = (f32)exp(-1.0 / ((f64)t
                                         * (f64)SONUS_FREQUENTIA));
    }
    alioquin si (v->genus == TIBIA_PARTIALES)
    {
        _partiales_parare(v, p);
    }
    alioquin si (v->genus == TIBIA_FORMANTES)
    {
        _formantes_parare(v, p);
    }
    v->flatus    = p->flatus < 0.0f ? 0.0f
                 : (p->flatus > 1.0f ? 1.0f : p->flatus);
    v->resonans  = p->resonans > 0.0f ? p->resonans : 0.0f;
    v->acumen    = p->acumen;
    v->motus     = p->motus;
    si (v->flatus > 0.0f)
    {
        sors_seminare(&v->sors_flatus, (i64)(nota * 1000.0f)
                      ^ 0x666CULL, ZEPHYRUM);
        sonus_resonans_parare(&v->colum_flatus, SONUS_RESONANS_MEDIA,
                              sonus_notae_frequentia(nota),
                              TIBIA_FLATUS_ACUMEN);
    }
    si (v->resonans > 0.0f)
    {
        sonus_resonans_parare(&v->colum_resonans, SONUS_RESONANS_GRAVIA,
                              v->resonans, v->acumen);
    }
}

vacuum
tibia_notam_ponere (
    TibiaVox* v,
         f32  nota,
         i32  quadra)
{
    si (quadra == ZEPHYRUM)
    {
        v->nota     = nota;
        v->meta     = nota;
        v->passus   = 0.0f;
        v->reliqua  = ZEPHYRUM;
        _frequentiam_ponere(v, nota);
        redde;
    }
    v->meta     = nota;
    v->passus   = (nota - v->nota) / (f32)quadra;
    v->reliqua  = quadra;
}

vacuum
tibia_portam_claudere (
    TibiaVox* v)
{
    si (v->aperta)
    {
        v->aperta = FALSUM;
        sonus_involucrum_claudere(&v->involucrum);
    }
}

f32
tibia_vocis_proximum (
    TibiaVox* v)
{
    f32 nota;
    f32 x;

    si (v->reliqua > ZEPHYRUM || v->vibratus != 0.0f)
    {
        si (v->reliqua > ZEPHYRUM)
        {
            v->reliqua--;
            /* ultimo passu meta EXACTA (sine residuo fluitante) */
            v->nota = v->reliqua == ZEPHYRUM ? v->meta
                                             : v->nota + v->passus;
        }
        nota = v->nota;
        si (v->vibratus != 0.0f)
        {
            nota += v->vibratus * (f32)sin(TIBIA_DUO_PI
                * (f64)v->phasis_vibrati / TIBIA_CIRCULUS);
            v->phasis_vibrati += v->incrementum_vibrati;
        }
        _frequentiam_ponere(v, nota);
    }
    x = v->genus == TIBIA_CHORDA ? _chordae_proximum(v)
        : v->genus == TIBIA_MODULATIO     ? _modulationis_proximum(v)
        : v->genus == TIBIA_PARTIALES     ? _partialium_proximum(v)
        : v->genus == TIBIA_FORMANTES     ? _formantium_proximum(v)
                                 : sonus_unda_proximum(&v->unda);
    si (v->flatus > 0.0f || v->resonans > 0.0f)
    {
        _moderari(v);
    }
    si (v->flatus > 0.0f)
    {
        f32 n = (f32)sors_proximum(&v->sors_flatus) / 2147483648.0f
              - 1.0f;

        x = (1.0f - v->flatus) * x + v->flatus * v->lucrum_flatus
            * sonus_resonans_proximum(&v->colum_flatus, n);
    }
    v->involucrum_ultimum = sonus_involucrum_proximum(&v->involucrum);

    x *= v->involucrum_ultimum;
    si (v->resonans > 0.0f)
    {
        x = sonus_resonans_proximum(&v->colum_resonans, x);
    }
    redde sonus_colum_proximum(&v->colum, x) * v->amplitudo;
}

b32
tibia_vox_finita (
    constans TibiaVox* v)
{
    redde !v->aperta && sonus_involucrum_quiescit(&v->involucrum);
}

constans TibiaParametra*
tibia_praeparatum (
    constans character* titulus)
{
    memoriae_index k;

    si (titulus == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < TIBIAE_PRAEPARATA; k++)
    {
        si (strcmp(tibiae_praeparata[k].titulus, titulus) == ZEPHYRUM)
        {
            redde &tibiae_praeparata[k].parametra;
        }
    }
    redde NIHIL;
}

constans character*
tibia_praeparati_titulus (
    i32 index)
{
    redde index < TIBIAE_PRAEPARATA ? tibiae_praeparata[index].titulus
                                    : NIHIL;
}

constans character*
tibia_generis_titulus (
    TibiaGenus genus)
{
    hic_manens constans character* constans GENERA[V] = {
        "unda", "chorda", "modulatio", "partiales", "formantes"
    };

    redde (i32)genus < V ? GENERA[genus] : NIHIL;
}
