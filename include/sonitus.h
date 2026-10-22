/* sonitus.h - Vox effectuum soni: parametra 'sfxr' super sonus
 *
 * Machina UNA, schema UNUM: omnis sonitus eadem viginti parametra
 * habet, et sonitus nominatus (praeparatum) est positio plena eorum -
 * forma ipsa 'sfxr' (DrPetter 2007) et solarii synth.c, nunc super
 * primitiva sonus.h (unda, involucrum, cola, retentio, comminutio).
 *
 * VOX est via UNA: sonitus_reddere vocem parat et exempla in tabulam
 * trahit; mixtura (S2) eandem vocem in reditu auditus trahet. Ergo
 * sonus offline et vivus octetim idem est (spec §IV.1).
 *
 * Parametra omnia f32 DE INDUSTRIA (etiam forma, gradus, retentio):
 * schema uniforme quod pagina soni (spec S-postea) legit, quod
 * 'mutatio' fortuita (sfxr 'mutate') et interpolatio inter praeparata
 * tractant, et quod STML (patch) scribit - forma ad enumerationem in
 * sonitus_vocem_parare rotundatur et coercetur.
 *
 * VARIETAS: semen strepitum SEMPER regit et, varietate > 0,
 * frequentiam, remissionem et officium perturbat - variatio per ictum
 * quam exemplum scriptum facere non potest. Idem semen + eadem
 * parametra = exempla octetim eadem.
 *
 * USUS:
 *   constans SonitusParametra* p = sonitus_praeparatum("laser");
 *   i32 n = sonitus_longitudo(p, semen);
 *   f32* x = piscina_allocare(piscina, n * magnitudo(f32));
 *   (vacuum)sonitus_reddere(p, semen, x, n);
 *
 *   vivum (S2):  sonitus_vocem_parare(&vox, p, semen);
 *                dum (!sonitus_vox_finita(&vox))
 *                    exitus[i++] = sonitus_vocis_proximum(&vox);
 */
/* <aedilis corpus="lib/sonitus.c"/> */
#ifndef SONITUS_H
#define SONITUS_H

#include "latina.h"
#include "sonus.h"


/* ================================================================
 * PARAMETRA - viginti, ordine schematis (sonitus_campus)
 * ================================================================ */

nomen structura {
    f32 forma;               /* SonusFormaUndae 0..4 (rotundata) */
    /* involucrum: impetus -> mora plena (cum ictu) -> remissio */
    f32 impetus;             /* s, 0 -> 1 */
    f32 mora;                /* s ad plenum ('sustain' sfxr = TEMPUS,
                              * non gradus) */
    f32 ictus;               /* 0..1 lucrum extra initio morae, per
                              * moram evanescens ('punch') */
    f32 remissio;            /* s, 1 -> 0 */
    /* altitudo */
    f32 frequentia;          /* cycli/s initio (strepitus: altitudo
                              * crepitus) */
    f32 clivus;              /* octavae per secundum (glissando) */
    f32 acceleratio;         /* octavae per secundum^2 (clivi mutatio) */
    f32 vibratus;            /* profunditas, semitonia */
    f32 vibratus_celeritas;  /* cycli/s */
    f32 saltus;              /* ratio frequentiae post saltum (1 =
                              * nullus) - arpeggio unius saltus */
    f32 saltus_tempus;       /* s ante saltum (0 = numquam) */
    /* timbre */
    f32 officium;            /* pulsus, 0..1 */
    f32 officium_clivus;     /* mutatio officii per secundum */
    f32 gravia;              /* limes coli gravium, cycli/s (0 = nihil) */
    f32 acuta;               /* limes coli acutorum, cycli/s (0 =
                              * nihil) */
    /* asperitas digitalis */
    f32 gradus;              /* comminutio, gradus (< II = nihil) */
    f32 retentio;            /* exempla tenenda (< II = nihil) */
    /* exitus */
    f32 amplitudo;           /* lucrum lineare */
    f32 varietas;            /* 0..1: quantum semen perturbat */
} SonitusParametra;


/* ================================================================
 * SCHEMA - campi enumerabiles (pagina soni, patches STML, mutatio)
 * ================================================================ */

nomen structura {
    constans character* titulus;   /* "frequentia" - idem ac campus C */
         memoriae_index offset;    /* offsetof(SonitusParametra, ...) */
                    f32 minimum;
                    f32 maximum;
    constans character* unitas;    /* "s", "Hz", "oct/s", "semitonia",
                                    * "" */
} SonitusCampus;

i32
sonitus_campi_numerus (
    vacuum);

/* NIHIL extra [0, numerus) */
constans SonitusCampus*
sonitus_campus (
    i32 index);

/* valor campi per indicem (offset schematis); index extra = 0 / nihil */
f32
sonitus_campum_legere (
    constans SonitusParametra* p,
                          i32  index);

/* valor ad [minimum, maximum] campi coercetur */
vacuum
sonitus_campum_ponere (
    SonitusParametra* p,
                 i32  index,
                 f32  valor);


/* ================================================================
 * PRAEPARATA - sonitus nominati (tabula C; STML postea, spec §X.2)
 * ================================================================ */

i32
sonitus_praeparatorum_numerus (
    vacuum);

/* NIHIL extra fines */
constans character*
sonitus_praeparati_titulus (
    i32 index);

/* NIHIL si titulus ignotus */
constans SonitusParametra*
sonitus_praeparatum (
    constans character* titulus);


/* ================================================================
 * VOX - status unius sonitus sonantis
 * ================================================================ */

nomen structura {
    SonitusParametra p;           /* post varietatem et coercitionem */
           SonusUnda unda;
     SonusInvolucrum involucrum;
          SonusColum colum_gravium;
          SonusColum colum_acutorum;
       SonusRetentio retentio;
                 i32 exemplum;    /* ab initio */
                 i32 finis_morae; /* exemplum quo porta clauditur */
                 i32 finis;       /* exempla tota (= sonitus_longitudo) */
} SonitusVox;

vacuum
sonitus_vocem_parare (
                   SonitusVox* vox,
    constans SonitusParametra* p,
                          i64  semen);

/* exemplum proximum in [-1, 1]; post finem 0 */
f32
sonitus_vocis_proximum (
    SonitusVox* vox);

/* exempla omnia reddita (exemplum >= finis): vox liberari potest.
 * Finis numeratus, non involucrum quiescens - clivus remissionis
 * fluitans exemplo uno errare potest, longitudo numquam */
b32
sonitus_vox_finita (
    constans SonitusVox* vox);


/* ================================================================
 * OFFLINE
 * ================================================================ */

/* exempla quae sonitus_reddere scribet: (impetus + mora + remissio) x
 * SONUS_FREQUENTIA post varietatem seminis (ergo semen requiritur) */
i32
sonitus_longitudo (
    constans SonitusParametra* p,
                          i64  semen);

/* vocem parat et exempla trahit usque ad finem aut 'maximum'; reddit
 * exempla scripta */
i32
sonitus_reddere (
    constans SonitusParametra* p,
                          i64  semen,
                          f32* exitus,
                          i32  maximum);

/* sonitus SINE SUTURA (ventus, crepitus ignis): reddit, deinde caudam
 * in caput 'transitus' exemplis miscet et abscindit - exemplum ultimum
 * primum continuat. Reddit longitudinem iterabilem (scripta minus
 * transitus); brevior quam transitus = sicut est. Parametra formae
 * iterabilis (impetus 0, remissio 0): evanescentia lucro mixturae
 * pertinet, non corpori circuli */
i32
sonitus_reddere_sine_sutura (
    constans SonitusParametra* p,
                          i64  semen,
                          f32* exitus,
                          i32  maximum,
                          i32  transitus);

#endif /* SONITUS_H */
