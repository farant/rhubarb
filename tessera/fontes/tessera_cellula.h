/* tessera_cellula.h - Cellula, stilus, colores, signa (Phase A)
 *
 * SIGNUM = octeti UTF-8 COMPACTI in i32 (spec-v2 par 1.3): octetus
 * primus in LSB, 1-4 octeti, 0 = cellula vacua (spatium emittitur).
 * ASCII: valor compactus == codepoint (< 0x80) - hospiti pellucidum.
 * Sine codificatore, sine decodificatore: scriptio limites runarum
 * ambulat (utf8_proxima_runa), emissio octetos effundit.
 *
 * COLOR = 0x00RRGGBB; TESSERA_COLOR_NATIVUS = defalta terminalis
 * (SGR reditio nuda eam dat - emissio nihil addit).
 *
 * ORNAMENTA = sex tuta (SGR singuli): crassum 1, obscurum 2,
 * cursivum 3, sublineatum 4, inversum 7, transfixum 9.
 */

#ifndef TESSERA_CELLULA_H
#define TESSERA_CELLULA_H

#include "latina.h"
#include "chorda_aedificator.h"

/* ==================================================
 * Colores
 * ================================================== */

#define TESSERA_COLOR_NATIVUS 0xFF000000

/* ==================================================
 * Ornamenta (fasciculus bitorum - sex tuta)
 * ================================================== */

#define TESSERA_ORNAMENTUM_CRASSUM     0x01
#define TESSERA_ORNAMENTUM_OBSCURUM    0x02
#define TESSERA_ORNAMENTUM_CURSIVUM    0x04
#define TESSERA_ORNAMENTUM_SUBLINEATUM 0x08
#define TESSERA_ORNAMENTUM_INVERSUM    0x10
#define TESSERA_ORNAMENTUM_TRANSFIXUM  0x20

/* ==================================================
 * Cellula + stilus
 * ================================================== */

nomen structura {
    i32 signum;          /* UTF-8 compactum; 0 = vacuum */
    i32 color_litterae;
    i32 color_fundi;
    i32 ornamenta;
} TesseraCellula;        /* 16 octeti - ordinatum IV alloca! */

nomen structura {
    i32 color_litterae;
    i32 color_fundi;
    i32 ornamenta;
} TesseraStilus;

TesseraStilus tessera_stilus (i32 color_litterae, i32 color_fundi,
    i32 ornamenta);
TesseraStilus tessera_stilus_nativus (vacuum);
b32 tessera_stilus_aequalis (TesseraStilus a, TesseraStilus b);

/* ==================================================
 * Signum compactum
 * ================================================== */

/* Octetos 1-4 compingere (LSB primus); 0 octeti aut nimis -> 0 */
i32 tessera_signum_ex_octetis (constans i8* octeti, i32 numerus);

/* Numerus octetorum signi; 0 pro vacuo */
i32 tessera_signum_mensura (i32 signum);

/* Octetos signi in aedificatorem effundere; vacuum -> ' ' */
vacuum tessera_signum_scribere (ChordaAedificator* aed, i32 signum);

/* ==================================================
 * Signa linearum (constanta compacta)
 * H/V horizontale/verticale; anguli SS/SD/IS/ID =
 * superior sinister/dexter, inferior sinister/dexter
 * ================================================== */

#define TESSERA_SIGNUM_SIMPLEX_H  0x8094E2  /* U+2500 */
#define TESSERA_SIGNUM_SIMPLEX_V  0x8294E2  /* U+2502 */
#define TESSERA_SIGNUM_SIMPLEX_SS 0x8C94E2  /* U+250C */
#define TESSERA_SIGNUM_SIMPLEX_SD 0x9094E2  /* U+2510 */
#define TESSERA_SIGNUM_SIMPLEX_IS 0x9494E2  /* U+2514 */
#define TESSERA_SIGNUM_SIMPLEX_ID 0x9894E2  /* U+2518 */

#define TESSERA_SIGNUM_DUPLEX_H   0x9095E2  /* U+2550 */
#define TESSERA_SIGNUM_DUPLEX_V   0x9195E2  /* U+2551 */
#define TESSERA_SIGNUM_DUPLEX_SS  0x9495E2  /* U+2554 */
#define TESSERA_SIGNUM_DUPLEX_SD  0x9795E2  /* U+2557 */
#define TESSERA_SIGNUM_DUPLEX_IS  0x9A95E2  /* U+255A */
#define TESSERA_SIGNUM_DUPLEX_ID  0x9D95E2  /* U+255D */

#define TESSERA_SIGNUM_ROTUNDATUM_SS 0xAD95E2  /* U+256D */
#define TESSERA_SIGNUM_ROTUNDATUM_SD 0xAE95E2  /* U+256E */
#define TESSERA_SIGNUM_ROTUNDATUM_ID 0xAF95E2  /* U+256F */
#define TESSERA_SIGNUM_ROTUNDATUM_IS 0xB095E2  /* U+2570 */

nomen enumeratio {
    TESSERA_LINEA_SIMPLEX = 0,
    TESSERA_LINEA_DUPLEX,
    TESSERA_LINEA_ROTUNDATA   /* anguli rotundati, latera simplicia */
} TesseraLineaGenus;

#endif /* TESSERA_CELLULA_H */
