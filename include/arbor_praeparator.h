#ifndef ARBOR_PRAEPARATOR_H
#define ARBOR_PRAEPARATOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "arbor_lexema.h"

/* ==================================================
 * ARBOR PRAEPARATOR - C Preprocessor
 *
 * Tractat directivas (#include, #define, #if, etc.)
 * Expandit macros cum tracking originis.
 *
 * PHILOSOPHIA:
 * - Origo tracking pro source mapping
 * - Nested expansion chains conservatae
 * - Separatus a parser (two-pass)
 * - Full #if expression evaluation
 *
 * EXEMPLUM:
 *   ArborPraeparator* pp = arbor_praeparator_creare(piscina, intern);
 *   arbor_praeparator_addere_via(pp, "/usr/include");
 *   arbor_praeparator_definire(pp, "DEBUG", "1");
 *   Xar* expandita = arbor_praeparator_processare(pp, "main.c");
 * ================================================== */

/* ==================================================
 * Macro Expansion Origin Chain
 *
 * Tracks provenance of tokens through macro expansion
 * and #include processing. Enables source mapping back
 * to original code.
 * ================================================== */

nomen structura ArborOrigo {
    chorda*                  nomen_macro;       /* Which macro expanded (NULL if #include) */
    chorda*                  via_file;          /* File path (for #include tracking) */
    i32                      linea_originalis;
    i32                      columna_originalis;
    i32                      expansio_index;    /* Nesting level (0=original source) */
    structura ArborOrigo*    pater;             /* Parent expansion (NULL if root) */
} ArborOrigo;

/* ==================================================
 * Token with Origin Tracking
 * ================================================== */

nomen structura {
    ArborLexema*    lexema;
    ArborOrigo*     origo;    /* NULL if from original source (not macro/include) */
} ArborLexemaOrigo;

/* ==================================================
 * Directive Types
 * ================================================== */

nomen enumeratio {
    ARBOR_DIRECTIVA_INCLUDE,
    ARBOR_DIRECTIVA_DEFINE,
    ARBOR_DIRECTIVA_UNDEF,
    ARBOR_DIRECTIVA_IF,
    ARBOR_DIRECTIVA_IFDEF,
    ARBOR_DIRECTIVA_IFNDEF,
    ARBOR_DIRECTIVA_ELIF,
    ARBOR_DIRECTIVA_ELSE,
    ARBOR_DIRECTIVA_ENDIF,
    ARBOR_DIRECTIVA_LINE,
    ARBOR_DIRECTIVA_ERROR,
    ARBOR_DIRECTIVA_PRAGMA,
    ARBOR_DIRECTIVA_UNKNOWN
} ArborDirectivaGenus;

/* ==================================================
 * Preprocessor Mode
 *
 * PROCESSARE: Normal mode - expand macros, follow includes
 * PRESERVARE: Preserve mode - keep directives as tokens for fidelis formatting
 * HYBRID:     Learn macros from includes, but preserve original tokens
 * ================================================== */

nomen enumeratio {
    ARBOR_PP_MODUS_PROCESSARE,    /* Normal: expand macros, follow includes */
    ARBOR_PP_MODUS_PRESERVARE,    /* Preserve: keep directives as nodes */
    ARBOR_PP_MODUS_HYBRID         /* Hybrid: learn macros, preserve tokens */
} ArborPPModus;

/* ==================================================
 * Macro Definition
 * ================================================== */

nomen structura {
    chorda*         titulus;         /* Macro name (interned) */
    b32             est_functio;     /* Function-like macro */
    b32             est_variadic;    /* Has ... parameter */
    Xar*            parametra;       /* Xar of chorda* (param names) */
    Xar*            corpus;          /* Xar of ArborLexema* (body tokens) */
    chorda          via_file;        /* Where defined */
    i32             linea_def;
} ArborMacroDefinitio;

/* ==================================================
 * Preprocessor State (opaque)
 * ================================================== */

nomen structura ArborPraeparator ArborPraeparator;

/* ==================================================
 * Error Types (reused from arbor_syntaxis)
 * ================================================== */

nomen enumeratio {
    ARBOR_PP_ERROR_FILE_NOT_FOUND,
    ARBOR_PP_ERROR_SYNTAX,
    ARBOR_PP_ERROR_UNTERMINATED_CONDITIONAL,
    ARBOR_PP_ERROR_UNEXPECTED_DIRECTIVE,
    ARBOR_PP_ERROR_INVALID_EXPRESSION,
    ARBOR_PP_ERROR_MACRO_RECURSION,
    ARBOR_PP_ERROR_INCLUDE_DEPTH
} ArborPPErrorGenus;

nomen structura {
    ArborPPErrorGenus   genus;
    chorda              nuntius;
    chorda              via_file;
    i32                 linea;
    i32                 columna;
} ArborPPError;

/* ==================================================
 * API - Creation and Configuration
 * ================================================== */

/* Creare praeparator */
ArborPraeparator* arbor_praeparator_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* Ponere modum (PROCESSARE vel PRESERVARE) */
vacuum arbor_praeparator_ponere_modum(
    ArborPraeparator*     pp,
    ArborPPModus          modus);

/* Obtinere modum */
ArborPPModus arbor_praeparator_obtinere_modum(
    ArborPraeparator*     pp);

/* Addere via include (search path) */
vacuum arbor_praeparator_addere_via(
    ArborPraeparator*     pp,
    constans character*   via);

/* Definire macro (e.g., from command line -D) */
vacuum arbor_praeparator_definire(
    ArborPraeparator*     pp,
    constans character*   nomen_macro,
    constans character*   valor);

/* Undefinire macro (e.g., from command line -U) */
vacuum arbor_praeparator_undefinire(
    ArborPraeparator*     pp,
    constans character*   nomen_macro);

/* ==================================================
 * API - Processing
 * ================================================== */

/* Processare file, return expanded tokens with origins
 * Reads file, lexes, preprocesses recursively.
 * Returns Xar of ArborLexemaOrigo.
 */
Xar* arbor_praeparator_processare(
    ArborPraeparator*     pp,
    constans character*   via_file);

/* Processare lexemata jam habita
 * For when you already have tokens (e.g., from embedded source).
 * Returns Xar of ArborLexemaOrigo.
 */
Xar* arbor_praeparator_processare_lexemata(
    ArborPraeparator*     pp,
    Xar*                  lexemata,
    constans character*   via_file);

/* ==================================================
 * API - Query
 * ================================================== */

/* Obtinere macro definition (for hover info, debugging) */
ArborMacroDefinitio* arbor_praeparator_obtinere_macro(
    ArborPraeparator*     pp,
    chorda*               nomen_macro);

/* Check if macro is defined */
b32 arbor_praeparator_est_definitum(
    ArborPraeparator*     pp,
    chorda*               nomen_macro);

/* Obtinere errores */
Xar* arbor_praeparator_errores(ArborPraeparator* pp);

/* Obtinere keyword macros (for hybrid mode)
 * Returns TabulaDispersa mapping macro name (chorda*) -> keyword token type (i32)
 * E.g., "hic_manens" -> ARBOR_LEXEMA_STATIC
 * Caller should NOT free the table.
 */
TabulaDispersa* arbor_praeparator_obtinere_keyword_macros(ArborPraeparator* pp);

/* Obtinere typedef nomina (for hybrid mode)
 * Returns TabulaDispersa of typedef names learned from included files.
 * E.g., "i8", "i32", etc.
 * Caller should NOT free the table.
 */
TabulaDispersa* arbor_praeparator_obtinere_typedef_nomina(ArborPraeparator* pp);

/* ==================================================
 * API - Utilities
 * ================================================== */

/* Obtinere nomen directiva (pro debugging) */
constans character* arbor_directiva_genus_nomen(ArborDirectivaGenus genus);

/* Format error for display */
chorda arbor_pp_error_formare(Piscina* piscina, ArborPPError* err);

#endif /* ARBOR_PRAEPARATOR_H */
