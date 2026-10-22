/* tibia.c - Instrumentum (vide include/tibia.h)
 *
 * Frequentia undae per exemplum computatur SOLUM dum nota movetur
 * (glissandum aut vibratus); aliter semel, in parare et in nota statim
 * posita - sinus/potentia per exemplum sumptus est quem nota tenta non
 * eget. Via eadem offline et in mixtura: exempla octetim eadem.
 */
#include "tibia.h"
#include <math.h>
#include <string.h>

#define TIBIA_DUO_PI    6.28318530717958647692
#define TIBIA_CIRCULUS  4294967296.0   /* 2^32: phasis cycli unius */

nomen structura {
     constans character* titulus;
         TibiaParametra  parametra;
} TibiaPraeparatum;

/* forma, officium, involucrum {impetus, lapsus, tenor, remissio},
 * vibratus, celeritas, gravia, amplitudo */
hic_manens constans TibiaPraeparatum tibiae_praeparata[] = {
    /* aetherophonum: sinus, introitus et exitus lenes, vibratus
     * parvus (manus tremens) */
    { "aether",     { 3.0f, 0.5f,  { 0.04f,  0.0f,  1.0f, 0.25f },
                      0.12f, 5.5f, 0.0f, 0.5f } },
    /* chiptune: pulsus quartae, mollitus supra VI milia */
    { "quadrans",   { 0.0f, 0.25f, { 0.002f, 0.08f, 0.7f, 0.08f },
                      0.0f, 0.0f, 6000.0f, 0.35f } },
    { "triangulum", { 2.0f, 0.5f,  { 0.002f, 0.05f, 0.9f, 0.05f },
                      0.0f, 0.0f, 0.0f, 0.6f } }
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

interior vacuum
_frequentiam_ponere (
    TibiaVox* v,
         f32  nota)
{
    sonus_unda_frequentiam_ponere(&v->unda,
        sonus_notae_frequentia(nota));
}

vacuum
tibia_vocem_parare (
                 TibiaVox* v,
    constans TibiaParametra* p,
                      f32  nota)
{
    f64 incrementum;

    memset(v, ZEPHYRUM, magnitudo(TibiaVox));
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
    x = sonus_unda_proximum(&v->unda)
        * sonus_involucrum_proximum(&v->involucrum);
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
