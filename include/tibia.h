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
 * GENERA (TibiaParametra.genus, f32 ut cetera - schema uniforme):
 *   0 UNDA    unda (sonus.h) x involucrum, colum, vibratus (ut antea)
 *   1 CHORDA  Karplus-Strong: impulsus strepitus (sors, semen ex nota -
 *             WAV deterministicum) per lineam morae longitudinis
 *             frequentiae; media duorum exemplorum (filtrum) et
 *             decrementum per cyclum; claritas = pondus filtri
 *             (0 obscura .. 1 clara); morae fractio linealiter
 *             (glissandum licet); porta clausa = involucrum remittit
 *             (chorda manu tacta)
 *   2 FM      sinus portans + modulans (ratio x frequentia), index
 *             modulationis decrescens exponentialiter (campana:
 *             ratio inharmonica, index magnus initio)
 *   3 PARTIALES  summa sinuum: partialis quisque ratio (x frequentia
 *             notae), amplitudo, decrementum (secundae ad 1/e; 0 =
 *             manet - organum). Summa per amplitudinum summam
 *             dividitur (apex <= amplitudo). Ratio harmonica
 *             (organum) aut inharmonica (marimba 1 : 3.9 : 9.2;
 *             lamina libera 1 : 2.76 : 5.40 : 8.93 - glockenspiel).
 *             Partiales supra frequentiam Nyquist omittuntur.
 *   4 FORMANTES  unda (serra ordinaria) per V cola media parallela ad
 *             formantes vocalis (tabula tenoris Csound: a e i o u);
 *             vocalis fracta formantes inter vicinas linealiter ducit.
 *             Formantes fixi (vocalis), non notam sequuntur - vox
 *             cantans (schola)
 * Involucrum ADSR omnibus generibus; vibratus et glissandum omnibus.
 * Omnibus etiam: FLATUS (strepitus per colum medium circa notam,
 * deterministicus - semen ex nota) et COLUM RESONANS gravium (limes,
 * acumen = Q, motus = semitonia quibus involucrum limitem attollit:
 * pad subtractivus qui tenore aperitur).
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
#include "sors.h"

/* partiales maximi per instrumentum */
#define TIBIA_PARTIALES_NUMERUS VIII

/* formantes per vocem (FORMANTES) */
#define TIBIA_FORMANTES_NUMERUS V

nomen structura {
    f32 ratio;         /* 0 = finis tabulae */
    f32 amplitudo;
    f32 decrementum;   /* secundae ad 1/e; 0 = manet */
} TibiaPartialis;

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
                    f32 genus;       /* TibiaGenus 0..4 (rotundatum) */
                    f32 decrementum; /* CHORDA: secundae ad -60 dB;
                                      * FM: secundae indicis ad 1/e */
                    f32 claritas;    /* CHORDA: 0..1 */
                    f32 ratio;       /* FM: modulans / portans */
                    f32 index;       /* FM: index initio */
         TibiaPartialis partiales[TIBIA_PARTIALES_NUMERUS];
                                     /* PARTIALES; ratio 0 = finis */
                    f32 vocalis;     /* FORMANTES: 0..4 = a e i o u;
                                      * fractio formantes inter
                                      * vocales vicinas ducit */
                    f32 flatus;      /* omnibus: strepitus circa notam,
                                      * 0..1 */
                    f32 resonans;    /* omnibus: limes coli gravium
                                      * resonantis, cycli/s (0 =
                                      * nihil) */
                    f32 acumen;      /* Q coli resonantis (0 = 0.707) */
                    f32 motus;       /* semitonia quibus involucrum
                                      * limitem attollit (pad: tenor
                                      * aperit) */
} TibiaParametra;

nomen enumeratio {
    TIBIA_UNDA,
    TIBIA_CHORDA,
    TIBIA_MODULATIO,
    TIBIA_PARTIALES,
    TIBIA_FORMANTES
} TibiaGenus;

/* linea morae CHORDAE: notae usque ad ~23 Hz (F#0) */
#define TIBIA_LINEA MMXLVIII

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
         TibiaGenus genus;
                f32 linea[TIBIA_LINEA];  /* CHORDA: mora */
                i32 scriptura;           /* CHORDA: locus scribendi */
                f32 mora;                /* CHORDA: longitudo fracta */
                f32 detrimentum;         /* CHORDA: lucrum per cyclum */
                f32 claritas;
                i32 phasis_modulans;     /* FM (portans = unda) */
                f32 ratio;
                f32 index;               /* FM: currens */
                f32 index_detrimentum;   /* FM: per exemplum */
                i32 numerus_partialium;  /* PARTIALES: activi */
                i32 phases[TIBIA_PARTIALES_NUMERUS];
                f32 rationes[TIBIA_PARTIALES_NUMERUS];
                /* PARTIALES: amplitudines currentes (per summam
                 * divisae), detrimenta per exemplum */
                f32 amplitudines[TIBIA_PARTIALES_NUMERUS];
                f32 detrimenta[TIBIA_PARTIALES_NUMERUS];
      SonusResonans formantes[TIBIA_FORMANTES_NUMERUS];
                f32 lucra_formantium[TIBIA_FORMANTES_NUMERUS];
      SonusResonans colum_resonans;
      SonusResonans colum_flatus;
               Sors sors_flatus;
                f32 flatus;              /* exemplaria parametrorum */
                f32 lucrum_flatus;       /* strepitus ad efficacem
                                          * undae (per latitudinem) */
                f32 resonans;
                f32 acumen;
                f32 motus;
                f32 involucrum_ultimum;  /* motus: ultimum valor */
                i32 reliqua_moderationis; /* cola per XXXII exempla */
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
 * XXV%, chiptune), "triangulum" (bassus), "chorda" (Karplus-Strong:
 * citharae similis), "campana" (FM, ratio 1.4), "organum" (8' 4' 2 2/3'
 * 2'), "marimba", "glockenspiel" (PARTIALES), "schola" (FORMANTES,
 * vocalis a: cantus), "fistula" (partiales cum flatu), "nubes" (serra,
 * colum resonans tenore apertum: pad); NIHIL si ignotum */
constans TibiaParametra*
tibia_praeparatum (
    constans character* titulus);

#endif /* TIBIA_H */
