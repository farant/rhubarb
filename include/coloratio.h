#ifndef COLORATIO_H
#define COLORATIO_H

/* ==================================================
 * COLORATIO - Syntax Highlighting Module
 *
 * Produces a per-cell color index array from TabulaCharacterum.
 * Separates highlighting logic from rendering for testability.
 *
 * ARCHITECTURA:
 *   TabulaCharacterum → Coloratio → Pagina (renders)
 *
 * PRINCIPIA:
 * - Coloratio est pure computational (no rendering)
 * - Pagina possidet Coloratio, sed Coloratio potest
 *   instiari independenter pro unit testing
 * - Supportat multi-line blocks (e.g., <sputnik>...</sputnik>)
 *
 * EXEMPLUM:
 *   Coloratio* col = coloratio_creare(piscina, 68, 56);
 *   coloratio_ponere_regulas(col, COLORATIO_REGULA_COMMANDA);
 *   coloratio_computare(col, &tabula);
 *   u8 index = coloratio_obtinere(col, linea, columna);
 *
 * ================================================== */

#include "latina.h"
#include "piscina.h"
#include "tabula_characterum.h"

/* Forward declaration */
structura RegistrumCommandi;


/* ==================================================
 * Color Index Categories
 * ================================================== */

/* Semantic color indices - mapped to actual colors by theme */
enumeratio {
    COLORATIO_DEFALTA     = 0,   /* Normal text */
    COLORATIO_COMMANDUM   = 1,   /* $command */
    COLORATIO_TAGUM       = 2,   /* <tag> */
    COLORATIO_CLAVIS      = 3,   /* Keywords: sit, si, dum, per */
    COLORATIO_CHORDA      = 4,   /* String literals "..." */
    COLORATIO_NUMERUS     = 5,   /* Numbers */
    COLORATIO_COMMENTUM   = 6,   /* // comments */
    COLORATIO_OPERANS     = 7,   /* Operators */
    COLORATIO_PROPRIETAS  = 8,   /* Property names in objects */
    COLORATIO_ATTRIBUTUM  = 9,   /* Attribute names in tags */
    COLORATIO_VALOR_ATTR  = 10,  /* Attribute values */
    COLORATIO_LINK        = 11,  /* #hashtag links */
    COLORATIO_TYPUS       = 12,  /* Type annotation colons (: and ::) */
    COLORATIO_NUMERUS_MAX = 16   /* Reserved space for expansion */
} ColoratioIndex;


/* ==================================================
 * Line State (for multi-line blocks)
 * ================================================== */

/* Status per linea - tracks context at end of line */
enumeratio {
    STATUS_LINEA_NORMALIS   = 0,  /* Normal text */
    STATUS_LINEA_IN_SPUTNIK = 1,  /* Inside <sputnik> block */
    STATUS_LINEA_IN_STML    = 2,  /* Inside other STML content */
    STATUS_LINEA_IN_CHORDA  = 3,  /* Inside multi-line string */
    STATUS_LINEA_IN_COMMENT = 4   /* Inside block comment */
} StatusLinea;


/* ==================================================
 * Rule Flags
 * ================================================== */

#define COLORATIO_REGULA_COMMANDA   0x01  /* $command highlighting */
#define COLORATIO_REGULA_SPUTNIK    0x02  /* Sputnik syntax within blocks */
#define COLORATIO_REGULA_STML       0x04  /* STML/XML tags */


/* ==================================================
 * Coloratio Structure
 * ================================================== */

nomen structura {
    Piscina* piscina;

    /* Dimensiones (must match TabulaCharacterum) */
    i32 latitudo;
    i32 altitudo;

    /* Color index per cell - parallel to TabulaCharacterum.cellulae */
    i8* indices;  /* [altitudo * latitudo], row-major */

    /* Active highlighting rules */
    i32 regulae;  /* Bitmask of COLORATIO_REGULA_* */

    /* Multi-line state tracking */
    i8* status_lineae;  /* [altitudo] - state at END of each line */

    /* Optional: command registry for validating $commands
     * Si NIHIL, all $word patterns highlighted
     * Si non NIHIL, only registered commands highlighted */
    structura RegistrumCommandi* reg_commandi;
} Coloratio;


/* Macro pro accessu directo */
#define coloratio_index(coloratio, linea, columna) \
    ((coloratio)->indices[(linea) * (coloratio)->latitudo + (columna)])


/* ==================================================
 * Creation
 * ================================================== */

/* Creare coloratio cum dimensionibus specificatis
 *
 * piscina: memory pool
 * latitudo: columns (must match TabulaCharacterum)
 * altitudo: rows (must match TabulaCharacterum)
 *
 * Reddit: Coloratio*, vel NIHIL si error
 */
Coloratio*
coloratio_creare(
    Piscina* piscina,
    i32 latitudo,
    i32 altitudo);

/* Creare coloratio ex TabulaCharacterum (copies dimensions)
 *
 * piscina: memory pool
 * tabula: source character table
 *
 * Reddit: Coloratio*, vel NIHIL si error
 */
Coloratio*
coloratio_creare_ex_tabula(
    Piscina* piscina,
    constans TabulaCharacterum* tabula);


/* ==================================================
 * Configuration
 * ================================================== */

/* Ponere regulae activae (replaces existing)
 *
 * coloratio: target
 * regulae: bitmask of COLORATIO_REGULA_*
 */
vacuum
coloratio_ponere_regulas(
    Coloratio* coloratio,
    i32 regulae);

/* Addere singulam regulam
 *
 * coloratio: target
 * regula: single COLORATIO_REGULA_* flag
 */
vacuum
coloratio_addere_regulam(
    Coloratio* coloratio,
    i32 regula);

/* Removere singulam regulam
 *
 * coloratio: target
 * regula: single COLORATIO_REGULA_* flag
 */
vacuum
coloratio_removere_regulam(
    Coloratio* coloratio,
    i32 regula);

/* Ponere registrum commandi pro validatione $commands
 *
 * Si reg est NIHIL, omnes $word patterns colorantur.
 * Si reg non est NIHIL, solum commanda registrata colorantur.
 *
 * coloratio: target
 * reg: command registry (vel NIHIL)
 */
vacuum
coloratio_ponere_registrum(
    Coloratio* coloratio,
    structura RegistrumCommandi* reg);


/* ==================================================
 * Processing
 * ================================================== */

/* Computare indices ex tabula characterum
 *
 * Main entry point. Scans entire tabula and populates indices.
 * Uses two-pass algorithm:
 *   Pass 1: Compute line-ending states for block tracking
 *   Pass 2: Color each line knowing its context
 *
 * coloratio: target (indices will be populated)
 * tabula: source character data
 */
vacuum
coloratio_computare(
    Coloratio* coloratio,
    constans TabulaCharacterum* tabula);

/* Vacare omnes indices ad COLORATIO_DEFALTA
 *
 * coloratio: target
 */
vacuum
coloratio_vacare(
    Coloratio* coloratio);


/* ==================================================
 * Query
 * ================================================== */

/* Obtinere index coloris ad positionem
 *
 * coloratio: source
 * linea: row (0 to altitudo-1)
 * columna: column (0 to latitudo-1)
 *
 * Reddit: color index (0 to COLORATIO_NUMERUS_MAX-1)
 */
i8
coloratio_obtinere(
    constans Coloratio* coloratio,
    i32 linea,
    i32 columna);

/* Obtinere status lineae ad finem lineae
 *
 * coloratio: source
 * linea: row (0 to altitudo-1)
 *
 * Reddit: StatusLinea value
 */
i8
coloratio_obtinere_status_lineae(
    constans Coloratio* coloratio,
    i32 linea);


#endif /* COLORATIO_H */
