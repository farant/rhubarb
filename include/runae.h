/* runae.h - Nucleus Unicode (acervus textus, stratum primum)
 *
 * Proprietates runarum (codepoints) ex TABULIS GENERATIS e datis
 * Unicode fixis (probationes/fixa/unicode/<versio>/,
 * tools/runae_generare.sh).
 * Lapis primus: LATITUDO in cellulis terminalis. Postea hic (cum
 * trahuntur): rupturae graphematum (UAX #29), normalizatio, casus,
 * rupturae linearum, bidi. Locale (collatio, formae) NUMQUAM hic.
 *
 * Purum: nulla allocatio, nullus status mutabilis, totale.
 * Unitates in nominibus: octeti (bytes), runa (codepoint), graphema
 * (cluster), latitudo (cellulae) - numquam 'n' nudum.
 */

#ifndef RUNAE_H
#define RUNAE_H

#include "latina.h"

/* Versio datorum Unicode tabularum - contractus, non ornamentum */
#define RUNAE_VERSIO "15.1.0"

/* Latitudo runae in cellulis: 0, 1 aut 2. Regula Ghostty (uucode
 * wcwidth_standalone + wcwidth_zero_in_grapheme): 0 regimina (Cc), Cs,
 * Zl, Zp, Default_Ignorable (praeter U+00AD = 1), Mn, Me, iamo Hangul
 * V/T; 2 East_Asian_Width W/F et Regional_Indicator; Mc = 1;
 * modificatores emoji et Prepend latitudinem suam servant; ceterae 1.
 * TOTALIS: runa invalida (< 0 aut > U+10FFFF) = 1 (U+FFFD pingitur). */
i32
runae_latitudo (
    s32 runa);


/* ==================================================
 * Graphemata (UAX #29 15.1, graphemata extensa)
 * ================================================== */

/* Status rupturae a vocante possessus (nulla allocatio): paritas
 * indicatorum regionum, series Extended_Pictographic Extend* (ZWJ),
 * series InCB consonans [extend/linker]* (linker visus). Initiandus
 * per runae_rupturam_initiare ante primam vocationem. */
nomen structura {
    i32 status;
} RunaeRuptura;

vacuum
runae_rupturam_initiare (
    RunaeRuptura* ruptura);

/* VERUM si limes graphematis inter prior et runa. Vocanda SEQUENTER
 * per omnes paria contigua (status priorem quisque vocatione
 * accipit). UAX #29 pura (GB3-GB13, GB9c); runa invalida utrimque
 * rumpit (ut Control). */
b32
runae_rumpitur (
             s32  prior,
             s32  runa,
    RunaeRuptura* ruptura);

/* Politica latitudinis graphematum: GRAPHEMATUM = regula Ghostty
 * (modus 2027, ordinaria); SIMPLEX = ut Terminal.app (mensuratum
 * 2026-09-28, aspectibus duobus): ZWJ pictographa NON iungit (quodque
 * emoji graphema suum, latitudo <= II). Cetera eadem - etiam signum
 * spatians (Mc) amplificat: Terminal.app hi II cellulas dat. */
nomen enumeratio {
    RUNAE_POLITICA_GRAPHEMATUM = 0,
    RUNAE_POLITICA_SIMPLEX
} RunaePolitica;

/* Graphema primum octetorum UTF-8 [initium, finis): reddit
 * indicatorem post id et latitudinem eius (0-II) in *latitudo.
 * Segmentatio per runae_rumpitur; latitudo regula Ghostty
 * (graphemeWidth): runae primae latitudo; VS16/VS15 post basim
 * variationis emoji = II/I (aliter nihil); runa sequens non nulla in
 * graphemate = II. Series UTF-8 invalida = graphema suum, latitudo I.
 * NON fluens: graphema integrum aut finem logicum praebe.
 * initium >= finis: reddit initium, latitudo 0. */
constans i8*
runae_graphema_proximum (
    constans i8* initium,
    constans i8* finis,
            i32* latitudo);

/* Idem sub politica data (SIMPLEX: vide supra). runae_graphema_proximum
 * = politica GRAPHEMATUM. */
constans i8*
runae_graphema_ex_politica (
      constans i8* initium,
      constans i8* finis,
    RunaePolitica  politica,
              i32* latitudo);

#endif /* RUNAE_H */
