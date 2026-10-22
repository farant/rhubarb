/* sonus.h - Primitiva soni: undae, involucra, cola, retentio,
 * comminutio, altitudo
 *
 * Lateres ex quibus voces fiunt (sonitus, tibia - project-specs/
 * sonus-spec.md §V). Quodque primitivum est VALOR PURUS in memoria
 * vocantis (sicut Sors): sine piscina, sine statu globali, sine filis;
 * copiatur libere. Unum exemplum per vocationem (_proximum): eadem via
 * reddit offline (in tabulam, semel) et vivum (in reditu auditus,
 * parametris inter exempla mutatis - theremin).
 *
 * Frequentia exemplorum FIXA: SONUS_FREQUENTIA (XLVIII milia per
 * secundum, mono f32 in [-1, 1]). Exitus (auditus) ad frequentiam
 * instrumenti convertit.
 *
 * Undae SINE limite bandae (aliasing) DE INDUSTRIA: harmonicae
 * replicatae sunt asperitas 'chiptune' quam quaerimus. Formae limitatae
 * (polyBLEP) postea formae NOVAE erunt in enumeratione, non
 * rescriptio.
 *
 * DETERMINISMUS: eadem parametra + idem semen = exempla octetim eadem
 * in eodem binario et machina. Strepitus ex sors (integri soli);
 * sinus/potentiae ex <math.h> (libm) - inter machinas fortasse non
 * octetim (spec §VI-g).
 *
 * USUS:
 *   SonusUnda u;
 *   SonusInvolucrum inv;
 *   SonusInvolucriForma f = { 0.0f, 0.09f, 0.6f, 0.2f };
 *   sonus_unda_parare(&u, SONUS_UNDA_PULSUS, ZEPHYRUM);
 *   sonus_unda_frequentiam_ponere(&u, 880.0f);
 *   sonus_involucrum_parare(&inv, &f);
 *   sonus_involucrum_aperire(&inv);
 *   per (i = 0; i < n; i++) {
 *       si (i == tenor_finis) sonus_involucrum_claudere(&inv);
 *       exitus[i] = sonus_unda_proximum(&u)
 *                 * sonus_involucrum_proximum(&inv);
 *   }
 */
/* <aedilis corpus="lib/sonus.c"/> */
#ifndef SONUS_H
#define SONUS_H

#include "latina.h"
#include "sors.h"

/* exempla per secundum - interna, fixa (spec §X.5) */
#define SONUS_FREQUENTIA (XLVIII * M)


/* ================================================================
 * UNDA - oscillator
 *
 * Phasis = accumulator integer XXXII bitorum: 2^32 = cyclus unus, et
 * circumvolutio insignata in C definita est - ergo phasis numquam
 * errorem colligit et 'floor' non eget (via classica 'DDS',
 * synthesis digitalis directa).
 * ================================================================ */

nomen enumeratio {
    SONUS_UNDA_PULSUS = 0,  /* quadrata cum officio (duty) */
    SONUS_UNDA_SERRA,       /* -1 -> +1 per cyclum */
    SONUS_UNDA_TRIANGULUM,
    SONUS_UNDA_SINUS,
    SONUS_UNDA_STREPITUS    /* sors, valor novus octies per cyclum:
                             * frequentia = altitudo crepitus */
} SonusFormaUndae;

nomen structura {
    SonusFormaUndae forma;
                i32 phasis;       /* 2^32 = cyclus */
                i32 incrementum;  /* phasis per exemplum */
                i32 officium;     /* pulsus: phasis < officium -> +1 */
               Sors sors;         /* strepitus */
                f32 retentum;     /* strepitus: valor currens */
} SonusUnda;

/* phasis 0, frequentia 0 (silentium), officium dimidium; semen
 * strepitum solum regit (formae aliae id ignorant) */
vacuum
sonus_unda_parare (
          SonusUnda* u,
    SonusFormaUndae  forma,
                i64  semen);

/* frequentia in cyclis per secundum; ad [0, SONUS_FREQUENTIA/2]
 * coercetur. Phasis NON reponitur - mutatio sine crepitu (glissando,
 * vibrato, theremin) */
vacuum
sonus_unda_frequentiam_ponere (
    SonusUnda* u,
          f32  frequentia);

/* ratio officii pulsus in (0, 1); ad [0.01, 0.99] coercetur */
vacuum
sonus_unda_officium_ponere (
    SonusUnda* u,
          f32  ratio);

/* exemplum proximum in [-1, 1]; phasis provehitur. Frequentia 0 ->
 * 0 (silentium, non DC pulsus haerentis) */
f32
sonus_unda_proximum (
    SonusUnda* u);


/* ================================================================
 * INVOLUCRUM - ADSR (impetus, lapsus, tenor, remissio)
 *
 * Segmenta linearia (asperitas 'chiptune'; curvae postea ut parametrum
 * novum). Porta (gate) per aperire/claudere: impetus et remissio a
 * valore CURRENTE incipiunt, ergo vox repercussa non crepitat.
 * ================================================================ */

nomen structura {
    f32 impetus;   /* secundae, 0 -> 1 */
    f32 lapsus;    /* secundae, 1 -> tenor */
    f32 tenor;     /* gradus manens dum porta aperta, [0, 1] */
    f32 remissio;  /* secundae, valor currens -> 0 */
} SonusInvolucriForma;

nomen enumeratio {
    SONUS_INVOLUCRUM_QUIESCIT = 0,  /* 0 reddit; vox liberari potest */
    SONUS_INVOLUCRUM_IMPETUS,
    SONUS_INVOLUCRUM_LAPSUS,
    SONUS_INVOLUCRUM_TENOR,
    SONUS_INVOLUCRUM_REMISSIO
} SonusGradusInvolucri;

nomen structura {
     SonusInvolucriForma forma;
    SonusGradusInvolucri gradus;
                     f32 valor;
                     f32 passus;   /* mutatio per exemplum in gradu */
} SonusInvolucrum;

/* quiescens, valor 0; secundae negativae ut 0 (saltus statim) */
vacuum
sonus_involucrum_parare (
                 SonusInvolucrum* inv,
    constans SonusInvolucriForma* forma);

/* porta aperta: impetus a valore currente */
vacuum
sonus_involucrum_aperire (
    SonusInvolucrum* inv);

/* porta clausa: remissio a valore currente (etiam in medio impetu) */
vacuum
sonus_involucrum_claudere (
    SonusInvolucrum* inv);

/* valor proximus in [0, 1]; gradus provehitur */
f32
sonus_involucrum_proximum (
    SonusInvolucrum* inv);

b32
sonus_involucrum_quiescit (
    constans SonusInvolucrum* inv);


/* ================================================================
 * COLUM - filtrum unius poli (biquad postea, cum tibia id trahit)
 * ================================================================ */

nomen enumeratio {
    SONUS_COLUM_GRAVIA = 0,  /* gravia transmittit (low-pass): 'surdum' */
    SONUS_COLUM_ACUTA        /* acuta transmittit (high-pass): signum
                              * minus suis gravibus */
} SonusGenusColi;

nomen structura {
    SonusGenusColi genus;
               f32 coefficiens;  /* 0 = transit */
               f32 memoria;
} SonusColum;

/* frequentia limitis in cyclis per secundum; 0 = transit (nihil
 * colatur). Memoria reponitur */
vacuum
sonus_colum_parare (
        SonusColum* c,
    SonusGenusColi  genus,
               f32  frequentia);

/* limitem mutat SINE memoria reposita (verrere sine crepitu) */
vacuum
sonus_colum_frequentiam_ponere (
    SonusColum* c,
           f32  frequentia);

f32
sonus_colum_proximum (
    SonusColum* c,
           f32  x);


/* ================================================================
 * RETENTIO et COMMINUTIO - asperitas digitalis ('8-bit' ut parametra)
 * ================================================================ */

/* sample-and-hold: valor quisque 'spatium' exempla tenetur (decimatio
 * sine colo - aliasing de industria) */
nomen structura {
    i32 spatium;   /* 0 aut 1 = transit */
    i32 reliqua;
    f32 valor;
} SonusRetentio;

vacuum
sonus_retentio_parare (
    SonusRetentio* r,
              i32  spatium);

f32
sonus_retentio_proximum (
    SonusRetentio* r,
              f32  x);

/* amplitudo quantizata: passus 2/gradus per [-1, 1], ergo gradus + I
 * valores (XVI gradus = 'IV bita' fere); gradus < II = transit.
 * Functio pura, sine statu */
f32
sonus_comminuere (
    f32 x,
    i32 gradus);


/* ================================================================
 * ALTITUDO - semitonia et notae
 * ================================================================ */

/* ratio frequentiae: 2^(semitonia/XII) (XII -> 2.0, -XII -> 0.5) */
f32
sonus_ratio_semitoniorum (
    f32 semitonia);

/* nota in numeris MIDI (LXIX = A4 = CDXL cycli/s; fractiones = microtoni,
 * theremin sine gradibus) -> frequentia */
f32
sonus_notae_frequentia (
    f32 nota);

#endif /* SONUS_H */
