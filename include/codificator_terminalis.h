/* codificator_terminalis.h - Eventus -> octeti terminalis (eventus B6a;
 * latus codificandi moduli 003)
 *
 * Inversum interpretis (interpres_terminalis): quod terminalis
 * programmati mitteret pro eventu dato, sub modis quos programma
 * petivit. PURUM: modi argumentum sunt (emulator statum modorum
 * possidebit), nulla allocatio praeter aedificatorem.
 *
 * ORACULUM: Ghostty (../ghostty @ 12752b2, src/input/key_encode.zig);
 * formae canonicae eius: in formis 'u' et '~' pressio ':1' omittitur,
 * in formis specialibus (CSI 1;m:1 A) includitur; in solutione nullus
 * textus, ergo nulla clavis mutata (alternata).
 *
 * FLUXUS: clavis cum TEXTU sequente (SCRIPTA) una series est ('A' in
 * legacy, textus in campo kitty) - codificator eventa consumpta
 * reddit. Quod modus exprimere nequit (solutio in legacy, clavis
 * modificans sine OMNES) nihil scribit, sed consumitur.
 *
 * Nondum (cum emulatore, modulus 006): DECCKM (sagittae SS3),
 * tabula numerica applicationis, modifyOtherKeys gradus II, X10.
 */

#ifndef CODIFICATOR_TERMINALIS_H
#define CODIFICATOR_TERMINALIS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "chorda_aedificator.h"
#include "eventus.h"

/* Mus quem programma petivit (B6a-ii) */
#define CODIFICATOR_MUS_NULLUS   ZEPHYRUM
#define CODIFICATOR_MUS_PRESSIO  I     /* ?1000 */
#define CODIFICATOR_MUS_TRACTUS  II    /* ?1002 */
#define CODIFICATOR_MUS_OMNIS    III   /* ?1003 */

nomen structura {
    i32 kitty_vexilla;      /* INTERPRES_KITTY_* (0 = legacy) */
    i32 mus;                /* CODIFICATOR_MUS_*; forma SGR (?1006) */
    b32 glutinum;           /* ?2004 */
    b32 focus;              /* ?1004 */
    s32 cellula_latitudo;   /* Modulus: pixela nostra per cellulam */
    s32 cellula_altitudo;
} CodificatorModi;

/* Eventa[0..numerus) a capite codificare: eventus primus (et TEXTUS
 * sequens si clavis eum generavit) in aedificatorem. Redde numerum
 * consumptum (>= I si numerus >= I); octeti nulli = inexpressibile. */
i32
codificator_eventa (
    constans CodificatorModi* modi,
            constans Eventus* eventa,
                         i32  numerus,
           ChordaAedificator* aedificator);

#endif /* CODIFICATOR_TERMINALIS_H */
