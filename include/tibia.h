/* tibia.h - Instrumentum: nota cum porta, altitudine viva
 *
 * Sonitus EFFECTUS est (longitudo in parametris eius); tibia NOTAM
 * tenet: porta aperta sonat donec clauditur, deinde remissio. Altitudo
 * viva in notis MIDI fractis (LXIX = A4; fractiones licent -
 * aetherophonum sine gradibus): tibia_notam_ponere glissat per quadra
 * data linealiter in SEMITONIIS, id est logarithmice in frequentia -
 * glissando aequale auditui. Lucrum vivum NON hic: mixtura id per
 * vocem habet (MIXTURA_IUSSUM_VOCEM_PONERE cum clivo).
 *
 * Patch minimus (sonus-spec S5a, ab aetherophono tractus): unda,
 * involucrum ADSR (sonus.h), vibratus, colum gravium. Parametra omnia
 * f32 ut sonitus (schema uniforme; patches STML S6). Purus: valor in
 * memoria vocantis, sine piscina, sine statu globali.
 *
 * USUS:
 *   TibiaVox v;
 *   tibia_vocem_parare(&v, tibia_praeparatum("aether"), 69.0f);
 *   ... exitus[i] = tibia_vocis_proximum(&v);
 *   tibia_notam_ponere(&v, 81.0f, SONUS_FREQUENTIA / X);  (* octava *)
 *   tibia_portam_claudere(&v);
 *   dum (!tibia_vox_finita(&v)) ...
 */
/* <aedilis corpus="lib/tibia.c"/> */
#ifndef TIBIA_H
#define TIBIA_H

#include "latina.h"
#include "sonus.h"

nomen structura {
                    f32 forma;       /* SonusFormaUndae 0..3 (rotundata;
                                      * strepitus non) */
                    f32 officium;    /* pulsus, 0..1 */
    SonusInvolucriForma involucrum;  /* impetus, lapsus, tenor,
                                      * remissio */
                    f32 vibratus;    /* profunditas, semitonia */
                    f32 vibratus_celeritas;  /* cycli/s */
                    f32 gravia;      /* limes coli gravium, cycli/s (0 =
                                      * nihil) */
                    f32 amplitudo;   /* lucrum lineare */
} TibiaParametra;

nomen structura {
          SonusUnda unda;
    SonusInvolucrum involucrum;
         SonusColum colum;
                f32 amplitudo;
                f32 vibratus;
                i32 phasis_vibrati;      /* 2^32 = cyclus (ut unda) */
                i32 incrementum_vibrati;
                f32 nota;                /* currens */
                f32 meta;
                f32 passus;
                i32 reliqua;             /* quadra glissandi */
                b32 aperta;              /* porta */
} TibiaVox;

/* porta APERTA: impetus incipit ad notam datam */
vacuum
tibia_vocem_parare (
                   TibiaVox* v,
    constans TibiaParametra* p,
                        f32  nota);

/* nota nova per 'quadra' linealiter in semitoniis; 0 = statim.
 * Glissandum currens substituitur a valore CURRENTE (sine saltu) */
vacuum
tibia_notam_ponere (
    TibiaVox* v,
         f32  nota,
         i32  quadra);

/* remissio a valore currente; iterum = nihil */
vacuum
tibia_portam_claudere (
    TibiaVox* v);

/* exemplum proximum in [-1, 1] x amplitudo */
f32
tibia_vocis_proximum (
    TibiaVox* v);

/* porta clausa et involucrum quietum: vox liberari potest. Porta
 * aperta numquam finita */
b32
tibia_vox_finita (
    constans TibiaVox* v);

/* "aether" (sinus, impetus lenis, vibratus parvus), "quadrans" (pulsus
 * XXV%, chiptune), "triangulum" (bassus); NIHIL si ignotum */
constans TibiaParametra*
tibia_praeparatum (
    constans character* titulus);

#endif /* TIBIA_H */
