/* stilus_terminalis.h - Stilus cellulae et SGR (modulus 004; planum
 * project-specs/stilus-terminalis-plan.md)
 *
 * Exemplar stili (colores litterae / fundi / sublineae, ornamenta,
 * genus sublineae) et SGR in utramque partem. PURUS: nulla allocatio
 * praeter aedificatorem. Oraculum: Ghostty (sgr.zig, style.zig @
 * 12752b2).
 *
 * CODIFICATIO CANONICA (reditio plena): ESC [ 0, deinde ;1 ;2 ;3,
 * sublinea (;4 simplex, ;4:n ceterae), ;5 ;7 ;8 ;9 ;53, deinde
 * colores (;38 / ;48 / ;58: 2;r;g;b aut 5;n), deinde 'm'. Subset
 * tesserae (1 2 3 4 7 9, littera, fundus) octetim idem ac olim.
 * Plus quam XXIV parametra: series altera (sine 0) - terminalis seriem
 * nimis longam totam abicit (reditus T2 id invenit).
 * Colores tabulae semper 5;n (non 30-37 / 90-97).
 */

#ifndef STILUS_TERMINALIS_H
#define STILUS_TERMINALIS_H

/* <aedilis nexus="purus"/> - clausura sine regula nexus (nulla
 * framework; eventus A1b: bin/aedilis --nexus-purus) */

#include "latina.h"
#include "chorda_aedificator.h"
#include "series_terminalis.h"

nomen enumeratio {
    STILUS_COLOR_NATIVUS = ZEPHYRUM,   /* defalta terminalis */
    STILUS_COLOR_TABULA,               /* index 0-255 */
    STILUS_COLOR_RGB                   /* 0x00RRGGBB */
} StilusColorGenus;

nomen structura {
    StilusColorGenus genus;
                 i32 valor;            /* index aut 0x00RRGGBB */
} StilusColor;

/* Ornamenta (numerus SGR in commentario) */
#define STILUS_CRASSUM     0x001   /* 1 */
#define STILUS_OBSCURUM    0x002   /* 2 (faint) */
#define STILUS_CURSIVUM    0x004   /* 3 */
#define STILUS_NICTANS     0x008   /* 5 (blink) */
#define STILUS_INVERSUM    0x010   /* 7 */
#define STILUS_INVISIBILE  0x020   /* 8 */
#define STILUS_TRANSFIXUM  0x040   /* 9 */
#define STILUS_SUPERLINEA  0x080   /* 53 (overline) */

nomen enumeratio {
    STILUS_SUBLINEA_NULLA = ZEPHYRUM,  /* 24, 4:0 */
    STILUS_SUBLINEA_SIMPLEX,           /* 4, 4:1 */
    STILUS_SUBLINEA_DUPLEX,            /* 4:2 (21 apud quosdam) */
    STILUS_SUBLINEA_UNDULATA,          /* 4:3 (curly) */
    STILUS_SUBLINEA_PUNCTATA,          /* 4:4 */
    STILUS_SUBLINEA_LINEOLATA          /* 4:5 */
} StilusSublinea;

nomen structura {
       StilusColor color_litterae;
       StilusColor color_fundi;
       StilusColor color_sublineae;
               i32 ornamenta;          /* STILUS_* */
    StilusSublinea sublinea;
} StilusTerminalis;

/* Parametra per seriem SGR: plura terminalis (Ghostty MAX_PARAMS)
 * seriem totam abicit - codificatio in series plures dividit (greges
 * integri) */
#define STILUS_PARAMETRA_MAXIMA   SERIES_PARAMETRA_MAXIMA

/* Codificatio colorum RGB */
#define STILUS_CODIFICATIO_PLENA  ZEPHYRUM   /* 38;2;r;g;b */
#define STILUS_CODIFICATIO_CCLVI  I          /* RGB quantizatum: 5;n */

/* Stilus nativus: colores NATIVI, ornamenta nulla. */
vacuum
stilus_nativus (
    StilusTerminalis* st);

b32
stilus_aequalis (
    constans StilusTerminalis* a,
    constans StilusTerminalis* b);

/* SGR in aedificatorem. prior NIHIL = reditio plena (canonica). prior
 * datus (v1): nihil si aequales, alioquin reditio plena - recta, nondum
 * minima (differentia minima postea, mensurata). */
vacuum
stilus_codificare (
            ChordaAedificator* exitus,
    constans StilusTerminalis* prior,
    constans StilusTerminalis* novus,
                          i32  codificatio);

/* Decodificare (T2): lexema CSI 'm' (series_terminalis) in stilum
 * applicare - separatores ';' et ':' (etiam mixti), 38;5;n,
 * 38;2;r;g;b, 38:2::r:g:b, 38:2:r:g:b, 30-37/40-47/90-97/100-107,
 * reseta (22 crassum ET obscurum). Ignota praetereuntur et numerantur
 * (redde numerum), numquam fatalia; forma coloris truncata reliqua
 * consumit. Lexema non SGR (privatum, intermedia: '>4;2m' xterm) nihil
 * mutat. Oraculum: Ghostty sgr.zig. */
i32
stilus_applicare (
    constans SeriesLexema* lexema,
         StilusTerminalis* st);

/* Tabula CCLVI xterm defalta: index -> 0x00RRGGBB (XVI ANSI xterm,
 * cubus VI x VI x VI gradibus 0/95/135/175/215/255, XXIV grisei
 * 8 + 10k). Index extra 0-255: 0. */
i32
stilus_tabulae_color (
    s32 index);

/* RGB -> index CCLVI proximus (XVI-CCLV) per distantiam quadratam:
 * cubus aut griseus (aequalitas: cubus). Algorithmus tesserae
 * (_cclvi, quadrans Q4) translatus. */
i32
stilus_quantizare (
    i32 rgb);

#endif /* STILUS_TERMINALIS_H */
