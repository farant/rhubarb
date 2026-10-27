/* sonitus.c - Vox effectuum soni (vide include/sonitus.h)
 *
 * Portatio solarii synth.c super primitiva sonus: involucrum = ADSR
 * (lapsus 0, tenor 1, porta clausa fine morae), ictus factor proprius
 * per moram evanescens; altitudo per exemplum ex clivo, acceleratione,
 * vibratu, saltu computata. Strepitus Sortem PROPRIAM in unda habet
 * (series 0 seminis), ergo varietas strepitum non transponit; varietas
 * seriem I trahit, ne sortes eius primis valoribus strepitus aequales
 * sint (decorrelatio, non mutatio audibilis - planta 'series communis'
 * viridis manet, 2026-10-20). Vide lib/sonitus.worklog.md.
 */
#include "sonitus.h"
#include <math.h>
#include <stddef.h>
#include <string.h>

#define SONITUS_DUO_PI 6.28318530717958647692

/* perturbationes maximae varietatis I (solarium, auditu temperatae) */
#define SONITUS_VARIETAS_FREQUENTIAE 0.16f
#define SONITUS_VARIETAS_REMISSIONIS 0.35f
#define SONITUS_VARIETAS_OFFICII     0.10f

#define SONITUS_CAMPUS(c, imum, summum, u) \
    { #c, offsetof(SonitusParametra, c), imum, summum, u }

hic_manens constans SonitusCampus CAMPI[XX] = {
    SONITUS_CAMPUS(forma, 0.0f, 4.0f, ""),
    SONITUS_CAMPUS(impetus, 0.0f, 10.0f, "s"),
    SONITUS_CAMPUS(mora, 0.0f, 10.0f, "s"),
    SONITUS_CAMPUS(ictus, 0.0f, 1.0f, ""),
    SONITUS_CAMPUS(remissio, 0.0f, 10.0f, "s"),
    SONITUS_CAMPUS(frequentia, 1.0f, 20000.0f, "Hz"),
    SONITUS_CAMPUS(clivus, -24.0f, 24.0f, "oct/s"),
    SONITUS_CAMPUS(acceleratio, -48.0f, 48.0f, "oct/s2"),
    SONITUS_CAMPUS(vibratus, 0.0f, 12.0f, "semitonia"),
    SONITUS_CAMPUS(vibratus_celeritas, 0.0f, 50.0f, "Hz"),
    SONITUS_CAMPUS(saltus, 0.125f, 8.0f, ""),
    SONITUS_CAMPUS(saltus_tempus, 0.0f, 10.0f, "s"),
    SONITUS_CAMPUS(officium, 0.0f, 1.0f, ""),
    SONITUS_CAMPUS(officium_clivus, -10.0f, 10.0f, "/s"),
    SONITUS_CAMPUS(gravia, 0.0f, 24000.0f, "Hz"),
    SONITUS_CAMPUS(acuta, 0.0f, 24000.0f, "Hz"),
    SONITUS_CAMPUS(gradus, 0.0f, 256.0f, ""),
    SONITUS_CAMPUS(retentio, 0.0f, 64.0f, "exempla"),
    SONITUS_CAMPUS(amplitudo, 0.0f, 1.0f, ""),
    SONITUS_CAMPUS(varietas, 0.0f, 1.0f, "")
};

/* Praeparata solarii (synth.c, 'tuned by ear via synthtest') -
 * valores octetim portati, ordine camporum. Nomina Latina: blip
 * punctum, step passus, whoosh susurrus, thump fragor, wind ventus,
 * laser radius, crackle crepitus, water rivus. Ultimum, tonus, NOSTRUM
 * est (non solarii): vox cantus, altitudine fixa (cantus.h). */
nomen structura {
     constans character* titulus;
       SonitusParametra  p;
} SonitusPraeparatum;

hic_manens constans SonitusPraeparatum PRAEPARATA[] = {
    /*              forma impet  mora   ictus  remis  freq     cliv  accel vib  vibc  salt  saltt  offic offcl grav     acuta   grad ret  ampl   var */
    { "punctum",  { 0.0f, 0.0f,  0.045f, 0.2f, 0.09f,  880.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.5f, 0.05f, 0.5f, 0.0f, 0.0f,    0.0f,
            0.0f, 0.0f, 0.40f, 0.0f } },
    { "passus",   { 4.0f, 0.002f, 0.018f, 0.0f, 0.08f,  320.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.5f, 0.0f, 1400.0f, 220.0f,
            0.0f, 0.0f, 0.33f, 0.5f } },
    { "susurrus", { 4.0f, 0.10f, 0.05f,  0.0f, 0.28f,  180.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.5f, 0.0f, 800.0f,  250.0f,
            0.0f, 0.0f, 0.30f, 0.3f } },
    { "fragor",   { 3.0f, 0.002f, 0.04f,  0.6f, 0.28f,  110.0f, -3.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.5f, 0.0f, 300.0f,  0.0f,
            0.0f, 0.0f, 0.60f, 0.25f } },
    { "ventus",   { 4.0f, 0.40f, 1.60f,  0.0f, 0.50f,  60.0f,  0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.5f, 0.0f, 450.0f,  0.0f,
            0.0f, 0.0f, 0.25f, 0.0f } },
    { "radius",   { 0.0f, 0.0f,  0.04f,  0.3f, 0.18f, 1400.0f, -9.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.18f, 0.3f, 0.0f,    0.0f,
            0.0f, 0.0f, 0.35f, 0.2f } },
    /* formae iterabilis (impetus/remissio 0): sputum flammae -
     * strepitus lente retentus, ad registrum ignis colatus */
    { "crepitus", { 4.0f, 0.0f,  3.0f,   0.0f, 0.0f,   18.0f,  0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.5f, 0.0f, 2000.0f, 400.0f,
            0.0f, 0.0f, 0.50f, 0.0f } },
    /* formae iterabilis: rivulus - strepitus subtilis, gravia infima
     * et acuta summa abscisa, ut aqua mota sonet, non ventus */
    { "rivus",    { 4.0f, 0.0f,  2.0f,   0.0f, 0.0f,   900.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.5f, 0.0f, 2600.0f, 520.0f,
            0.0f, 0.0f, 0.30f, 0.0f } },
    /* vox cantus: pulsus dimidius purus, impetus brevis contra crepitum,
     * altitudo FIXA - nota sonat quae scripta est; mora a cantu
     * (tenere) ponitur */
    { "tonus",    { 0.0f, 0.003f, 0.10f, 0.0f, 0.06f,  440.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  0.5f, 0.0f, 0.0f,    0.0f,
            0.0f, 0.0f, 0.35f, 0.0f } }
};

#define SONITUS_PRAEPARATORUM \
    ((i32)(magnitudo(PRAEPARATA) / magnitudo(PRAEPARATA[ZEPHYRUM])))

interior f32
_coercere (
    f32 v,
    f32 imum,
    f32 summum)
{
    si (v < imum)
    {
        redde imum;
    }
    si (v > summum)
    {
        redde summum;
    }
    redde v;
}


    /* ==================================================
     * SCHEMA
     * ================================================== */

i32
sonitus_campi_numerus (
    vacuum)
{
    redde XX;
}

constans SonitusCampus*
sonitus_campus (
    i32 index)
{
    si (index >= XX)
    {
        redde NIHIL;
    }
    redde &CAMPI[index];
}

f32
sonitus_campum_legere (
    constans SonitusParametra* p,
                          i32  index)
{
    si (index >= XX)
    {
        redde 0.0f;
    }
    redde *(constans f32*)((constans character*)p
        + CAMPI[index].offset);
}

vacuum
sonitus_campum_ponere (
    SonitusParametra* p,
                 i32  index,
                 f32  valor)
{
    si (index >= XX)
    {
        redde;
    }
    *(f32*)((character*)p + CAMPI[index].offset) =
        _coercere(valor, CAMPI[index].minimum, CAMPI[index].maximum);
}


    /* ==================================================
     * PRAEPARATA
     * ================================================== */

i32
sonitus_praeparatorum_numerus (
    vacuum)
{
    redde SONITUS_PRAEPARATORUM;
}

constans character*
sonitus_praeparati_titulus (
    i32 index)
{
    si (index >= SONITUS_PRAEPARATORUM)
    {
        redde NIHIL;
    }
    redde PRAEPARATA[index].titulus;
}

constans SonitusParametra*
sonitus_praeparatum (
    constans character* titulus)
{
    i32 k;

    si (titulus == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < SONITUS_PRAEPARATORUM; k++)
    {
        si (strcmp(PRAEPARATA[k].titulus, titulus) == ZEPHYRUM)
        {
            redde &PRAEPARATA[k].p;
        }
    }
    redde NIHIL;
}


    /* ==================================================
     * VOX
     * ================================================== */

/* parametra coercita, deinde varietas seminis - sortes TRES semper
 * trahuntur (etiam varietate 0), ut series ab varietate non pendeat.
 * Una via pro voce et longitudine: eaedem secundae, idem finis */
interior vacuum
_resolvere (
    constans SonitusParametra* p,
                          i64  semen,
             SonitusParametra* exitus)
{
    Sors s;
     i32 k;
     f32 r_frequentiae;
     f32 r_remissionis;
     f32 r_officii;
     f32 v;

    per (k = ZEPHYRUM; k < XX; k++)
    {
        sonitus_campum_ponere(exitus, k, sonitus_campum_legere(p, k));
    }
    sors_seminare(&s, semen, I);
    r_frequentiae  = sors_f32(&s) * 2.0f - 1.0f;
    r_remissionis  = sors_f32(&s) * 2.0f - 1.0f;
    r_officii      = sors_f32(&s) * 2.0f - 1.0f;
    v              = exitus->varietas;
    exitus->frequentia *= 1.0f + v * SONITUS_VARIETAS_FREQUENTIAE
                                 * r_frequentiae;
    exitus->remissio   *= 1.0f + v * SONITUS_VARIETAS_REMISSIONIS
                                 * r_remissionis;
    exitus->officium    = _coercere(exitus->officium
                                    + v * SONITUS_VARIETAS_OFFICII
                                        * r_officii, 0.0f, 1.0f);
}

/* ROTUNDATUM: 0.01f = 0.00999999977, truncatio CDLXXIX exempla daret
 * pro CDLXXX */
interior i32
_exempla (
    f32 secundae)
{
    redde (i32)((f64)secundae * (f64)SONUS_FREQUENTIA + 0.5);
}

vacuum
sonitus_vocem_parare (
                   SonitusVox* vox,
    constans SonitusParametra* p,
                          i64  semen)
{
    SonusInvolucriForma forma;

    _resolvere(p, semen, &vox->p);
    sonus_unda_parare(&vox->unda,
                      (SonusFormaUndae)(i32)(vox->p.forma + 0.5f),
                      semen);
    sonus_unda_frequentiam_ponere(&vox->unda, vox->p.frequentia);
    sonus_unda_officium_ponere(&vox->unda, vox->p.officium);
    forma.impetus   = vox->p.impetus;
    forma.lapsus    = 0.0f;
    forma.tenor     = 1.0f;
    forma.remissio  = vox->p.remissio;
    sonus_involucrum_parare(&vox->involucrum, &forma);
    sonus_involucrum_aperire(&vox->involucrum);
    sonus_colum_parare(&vox->colum_gravium, SONUS_COLUM_GRAVIA,
                       vox->p.gravia);
    sonus_colum_parare(&vox->colum_acutorum, SONUS_COLUM_ACUTA,
                       vox->p.acuta);
    sonus_retentio_parare(&vox->retentio, (i32)vox->p.retentio);
    vox->exemplum     = ZEPHYRUM;
    vox->finis_morae  = _exempla(vox->p.impetus + vox->p.mora);
    vox->finis       = _exempla(vox->p.impetus + vox->p.mora
                                + vox->p.remissio);
}

f32
sonitus_vocis_proximum (
    SonitusVox* vox)
{
     constans SonitusParametra* p = &vox->p;
                           f32  t;
                           f32  octavae;
                           f32  frequentia;
                           f32  lucrum;
                           f32  x;
                           i32  initium_morae;

    si (vox->exemplum >= vox->finis)
    {
        redde 0.0f;
    }
    si (vox->exemplum == vox->finis_morae)
    {
        sonus_involucrum_claudere(&vox->involucrum);
    }
    t = (f32)((f64)vox->exemplum / (f64)SONUS_FREQUENTIA);

    /* altitudo: glissando exponentiale (octavae = quomodo altitudo
     * movetur), vibratus in semitoniis, saltus unus */
    octavae = p->clivus * t + 0.5f * p->acceleratio * t * t;
    si (p->vibratus != 0.0f)
    {
        octavae += p->vibratus
                 * (f32)sin(SONITUS_DUO_PI * (f64)(p->vibratus_celeritas * t))
                 / 12.0f;
    }
    frequentia = p->frequentia * (f32)pow(2.0, (f64)octavae);
    si (p->saltus_tempus > 0.0f && t >= p->saltus_tempus)
    {
        frequentia *= p->saltus;
    }
    sonus_unda_frequentiam_ponere(&vox->unda,
                                  _coercere(frequentia, 1.0f,
                                  20000.0f));
    si (p->officium_clivus != 0.0f)
    {
        sonus_unda_officium_ponere(&vox->unda,
                                   p->officium
                                       + p->officium_clivus * t);
    }

    /* ictus: lucrum 1 + ictus initio morae, linea ad 1 fine morae */
    lucrum         = 1.0f;
    initium_morae  = _exempla(p->impetus);
    si (   p->ictus > 0.0f && vox->exemplum >= initium_morae
        && vox->exemplum < vox->finis_morae)
    {
        lucrum += p->ictus
                * (1.0f - (f32)(vox->exemplum - initium_morae)
                          / (f32)(vox->finis_morae - initium_morae));
    }

    x = sonus_unda_proximum(&vox->unda)
        * sonus_involucrum_proximum(&vox->involucrum) * lucrum;
    x = sonus_colum_proximum(&vox->colum_gravium, x);
    x = sonus_colum_proximum(&vox->colum_acutorum, x);
    x = sonus_comminuere(x, (i32)p->gradus);
    x = sonus_retentio_proximum(&vox->retentio, x);
    vox->exemplum++;
    redde _coercere(x * p->amplitudo, -1.0f, 1.0f);
}

b32
sonitus_vox_finita (
    constans SonitusVox* vox)
{
    redde vox->exemplum >= vox->finis;
}


    /* ==================================================
     * OFFLINE
     * ================================================== */

i32
sonitus_longitudo (
    constans SonitusParametra* p,
                          i64  semen)
{
    SonitusParametra r;

    _resolvere(p, semen, &r);
    redde _exempla(r.impetus + r.mora + r.remissio);
}

i32
sonitus_reddere (
    constans SonitusParametra* p,
                          i64  semen,
                          f32* exitus,
                          i32  maximum)
{
    SonitusVox vox;
           i32 n = ZEPHYRUM;

    sonitus_vocem_parare(&vox, p, semen);
    dum (!sonitus_vox_finita(&vox) && n < maximum)
    {
        exitus[n] = sonitus_vocis_proximum(&vox);
        n++;
    }
    redde n;
}

i32
sonitus_reddere_sine_sutura (
    constans SonitusParametra* p,
                          i64  semen,
                          f32* exitus,
                          i32  maximum,
                          i32  transitus)
{
    i32 n = sonitus_reddere(p, semen, exitus, maximum);
    i32 longitudo;
    i32 k;

    si (transitus >= n)
    {
        redde n;
    }
    longitudo = n - transitus;
    per (k = ZEPHYRUM; k < transitus; k++)
    {
        f32 pondus = (f32)k / (f32)transitus;

        exitus[k] = exitus[k] * pondus
                  + exitus[longitudo + k] * (1.0f - pondus);
    }
    redde longitudo;
}
