/* silva_tabulae_c89.c - GENERATUM per silva_coquere ex grammatica/c89.stml
 * NOLI MANU MUTARE - regenera per silva/generare.sh.
 *
 * status: 157, productiones: 99, symbola: 89,
 * conflictus servati (GLR): 2
 */

#include "silva_tabulae_c89.h"

/* ==================================================
 * Symbola
 * ================================================== */

hic_manens constans SilvaTabSymbolum SILVA_C89_SYMBOLA[89] = {
    /* [ 0] */ { "IDENTIFICATOR", SILVA_LEX_IDENTIFICATOR, VERUM },
    /* [ 1] */ { "INTEGER", SILVA_LEX_INTEGER, VERUM },
    /* [ 2] */ { "FLOAT", SILVA_LEX_FLOAT, VERUM },
    /* [ 3] */ { "CHARACTER_LIT", SILVA_LEX_CHARACTER_LIT, VERUM },
    /* [ 4] */ { "STRING_LIT", SILVA_LEX_STRING_LIT, VERUM },
    /* [ 5] */ { "VOID", SILVA_LEX_VOID, VERUM },
    /* [ 6] */ { "CHAR", SILVA_LEX_CHAR, VERUM },
    /* [ 7] */ { "SHORT", SILVA_LEX_SHORT, VERUM },
    /* [ 8] */ { "INT", SILVA_LEX_INT, VERUM },
    /* [ 9] */ { "LONG", SILVA_LEX_LONG, VERUM },
    /* [10] */ { "FLOAT_KW", SILVA_LEX_FLOAT_KW, VERUM },
    /* [11] */ { "DOUBLE", SILVA_LEX_DOUBLE, VERUM },
    /* [12] */ { "SIGNED", SILVA_LEX_SIGNED, VERUM },
    /* [13] */ { "UNSIGNED", SILVA_LEX_UNSIGNED, VERUM },
    /* [14] */ { "SIZEOF", SILVA_LEX_SIZEOF, VERUM },
    /* [15] */ { "PLUS", SILVA_LEX_PLUS, VERUM },
    /* [16] */ { "MINUS", SILVA_LEX_MINUS, VERUM },
    /* [17] */ { "STAR", SILVA_LEX_STAR, VERUM },
    /* [18] */ { "SOLIDUS", SILVA_LEX_SOLIDUS, VERUM },
    /* [19] */ { "PERCENTUM", SILVA_LEX_PERCENTUM, VERUM },
    /* [20] */ { "AMPERSAND", SILVA_LEX_AMPERSAND, VERUM },
    /* [21] */ { "BARRA", SILVA_LEX_BARRA, VERUM },
    /* [22] */ { "CARET", SILVA_LEX_CARET, VERUM },
    /* [23] */ { "TILDE", SILVA_LEX_TILDE, VERUM },
    /* [24] */ { "EXCLAMATIO", SILVA_LEX_EXCLAMATIO, VERUM },
    /* [25] */ { "ET_ET", SILVA_LEX_ET_ET, VERUM },
    /* [26] */ { "VEL_VEL", SILVA_LEX_VEL_VEL, VERUM },
    /* [27] */ { "AEQUALIS_AEQUALIS", SILVA_LEX_AEQUALIS_AEQUALIS, VERUM },
    /* [28] */ { "NON_AEQUALIS", SILVA_LEX_NON_AEQUALIS, VERUM },
    /* [29] */ { "MINOR", SILVA_LEX_MINOR, VERUM },
    /* [30] */ { "MAIOR", SILVA_LEX_MAIOR, VERUM },
    /* [31] */ { "MINOR_AEQUALIS", SILVA_LEX_MINOR_AEQUALIS, VERUM },
    /* [32] */ { "MAIOR_AEQUALIS", SILVA_LEX_MAIOR_AEQUALIS, VERUM },
    /* [33] */ { "SINISTRORSUM", SILVA_LEX_SINISTRORSUM, VERUM },
    /* [34] */ { "DEXTRORSUM", SILVA_LEX_DEXTRORSUM, VERUM },
    /* [35] */ { "ASSIGNATIO", SILVA_LEX_ASSIGNATIO, VERUM },
    /* [36] */ { "PLUS_ASSIGNATIO", SILVA_LEX_PLUS_ASSIGNATIO, VERUM },
    /* [37] */ { "MINUS_ASSIGNATIO", SILVA_LEX_MINUS_ASSIGNATIO, VERUM },
    /* [38] */ { "STAR_ASSIGNATIO", SILVA_LEX_STAR_ASSIGNATIO, VERUM },
    /* [39] */ { "SOLIDUS_ASSIGNATIO", SILVA_LEX_SOLIDUS_ASSIGNATIO, VERUM },
    /* [40] */ { "PERCENTUM_ASSIGNATIO", SILVA_LEX_PERCENTUM_ASSIGNATIO, VERUM },
    /* [41] */ { "AMPERSAND_ASSIGNATIO", SILVA_LEX_AMPERSAND_ASSIGNATIO, VERUM },
    /* [42] */ { "BARRA_ASSIGNATIO", SILVA_LEX_BARRA_ASSIGNATIO, VERUM },
    /* [43] */ { "CARET_ASSIGNATIO", SILVA_LEX_CARET_ASSIGNATIO, VERUM },
    /* [44] */ { "SINISTRORSUM_ASSIGNATIO", SILVA_LEX_SINISTRORSUM_ASSIGNATIO, VERUM },
    /* [45] */ { "DEXTRORSUM_ASSIGNATIO", SILVA_LEX_DEXTRORSUM_ASSIGNATIO, VERUM },
    /* [46] */ { "INCREMENTUM", SILVA_LEX_INCREMENTUM, VERUM },
    /* [47] */ { "DECREMENTUM", SILVA_LEX_DECREMENTUM, VERUM },
    /* [48] */ { "PUNCTUM", SILVA_LEX_PUNCTUM, VERUM },
    /* [49] */ { "SAGITTA", SILVA_LEX_SAGITTA, VERUM },
    /* [50] */ { "QUAESTIO", SILVA_LEX_QUAESTIO, VERUM },
    /* [51] */ { "COLON", SILVA_LEX_COLON, VERUM },
    /* [52] */ { "COMMA", SILVA_LEX_COMMA, VERUM },
    /* [53] */ { "SEMICOLON", SILVA_LEX_SEMICOLON, VERUM },
    /* [54] */ { "PAREN_APERTA", SILVA_LEX_PAREN_APERTA, VERUM },
    /* [55] */ { "PAREN_CLAUSA", SILVA_LEX_PAREN_CLAUSA, VERUM },
    /* [56] */ { "QUADRA_APERTA", SILVA_LEX_QUADRA_APERTA, VERUM },
    /* [57] */ { "QUADRA_CLAUSA", SILVA_LEX_QUADRA_CLAUSA, VERUM },
    /* [58] */ { "EOF", SILVA_LEX_EOF, VERUM },
    /* [59] */ { "elementa", SILVA_LEX_EOF, FALSUM },
    /* [60] */ { "elementum", SILVA_LEX_EOF, FALSUM },
    /* [61] */ { "sententia-expressionis", SILVA_LEX_EOF, FALSUM },
    /* [62] */ { "expressio", SILVA_LEX_EOF, FALSUM },
    /* [63] */ { "assignatio", SILVA_LEX_EOF, FALSUM },
    /* [64] */ { "ternarius", SILVA_LEX_EOF, FALSUM },
    /* [65] */ { "disiunctio", SILVA_LEX_EOF, FALSUM },
    /* [66] */ { "coniunctio", SILVA_LEX_EOF, FALSUM },
    /* [67] */ { "vel-bitorum", SILVA_LEX_EOF, FALSUM },
    /* [68] */ { "aut-bitorum", SILVA_LEX_EOF, FALSUM },
    /* [69] */ { "et-bitorum", SILVA_LEX_EOF, FALSUM },
    /* [70] */ { "aequalitas", SILVA_LEX_EOF, FALSUM },
    /* [71] */ { "comparatio", SILVA_LEX_EOF, FALSUM },
    /* [72] */ { "translatio", SILVA_LEX_EOF, FALSUM },
    /* [73] */ { "additivum", SILVA_LEX_EOF, FALSUM },
    /* [74] */ { "multiplicativum", SILVA_LEX_EOF, FALSUM },
    /* [75] */ { "conversio", SILVA_LEX_EOF, FALSUM },
    /* [76] */ { "unarium", SILVA_LEX_EOF, FALSUM },
    /* [77] */ { "species-typi", SILVA_LEX_EOF, FALSUM },
    /* [78] */ { "specificatores", SILVA_LEX_EOF, FALSUM },
    /* [79] */ { "typus", SILVA_LEX_EOF, FALSUM },
    /* [80] */ { "verbum-primitivum", SILVA_LEX_EOF, FALSUM },
    /* [81] */ { "verba-primitiva", SILVA_LEX_EOF, FALSUM },
    /* [82] */ { "declarator-abstractus", SILVA_LEX_EOF, FALSUM },
    /* [83] */ { "postfixum", SILVA_LEX_EOF, FALSUM },
    /* [84] */ { "argumenta", SILVA_LEX_EOF, FALSUM },
    /* [85] */ { "argumenta-plena", SILVA_LEX_EOF, FALSUM },
    /* [86] */ { "chordae", SILVA_LEX_EOF, FALSUM },
    /* [87] */ { "primarium", SILVA_LEX_EOF, FALSUM },
    /* [88] */ { "__initium__", SILVA_LEX_EOF, FALSUM }
};

/* ==================================================
 * Productiones
 * ================================================== */

hic_manens constans SilvaTabProductio SILVA_C89_PRODUCTIONES[99] = {
    /* P0  elementa (0 symbola) */ { 59, 0, NIHIL },
    /* P1  elementa (2 symbola) */ { 59, 2, "elementa-appendere" },
    /* P2  elementum (1 symbola) */ { 60, 1, NIHIL },
    /* P3  sententia-expressionis (2 symbola) */ { 61, 2, "sententia-expr" },
    /* P4  expressio (3 symbola) */ { 62, 3, "virgula-series" },
    /* P5  expressio (1 symbola) */ { 62, 1, NIHIL },
    /* P6  assignatio (3 symbola) */ { 63, 3, "assignatio-simplex" },
    /* P7  assignatio (3 symbola) */ { 63, 3, "assignatio-plus" },
    /* P8  assignatio (3 symbola) */ { 63, 3, "assignatio-minus" },
    /* P9  assignatio (3 symbola) */ { 63, 3, "assignatio-stella" },
    /* P10 assignatio (3 symbola) */ { 63, 3, "assignatio-solidus" },
    /* P11 assignatio (3 symbola) */ { 63, 3, "assignatio-percentum" },
    /* P12 assignatio (3 symbola) */ { 63, 3, "assignatio-ampersand" },
    /* P13 assignatio (3 symbola) */ { 63, 3, "assignatio-barra" },
    /* P14 assignatio (3 symbola) */ { 63, 3, "assignatio-caret" },
    /* P15 assignatio (3 symbola) */ { 63, 3, "assignatio-sinistrorsum" },
    /* P16 assignatio (3 symbola) */ { 63, 3, "assignatio-dextrorsum" },
    /* P17 assignatio (1 symbola) */ { 63, 1, NIHIL },
    /* P18 ternarius (5 symbola) */ { 64, 5, "ternarius-conditio" },
    /* P19 ternarius (1 symbola) */ { 64, 1, NIHIL },
    /* P20 disiunctio (3 symbola) */ { 65, 3, "binarium-disiunctio" },
    /* P21 disiunctio (1 symbola) */ { 65, 1, NIHIL },
    /* P22 coniunctio (3 symbola) */ { 66, 3, "binarium-coniunctio" },
    /* P23 coniunctio (1 symbola) */ { 66, 1, NIHIL },
    /* P24 vel-bitorum (3 symbola) */ { 67, 3, "binarium-vel" },
    /* P25 vel-bitorum (1 symbola) */ { 67, 1, NIHIL },
    /* P26 aut-bitorum (3 symbola) */ { 68, 3, "binarium-aut" },
    /* P27 aut-bitorum (1 symbola) */ { 68, 1, NIHIL },
    /* P28 et-bitorum (3 symbola) */ { 69, 3, "binarium-et" },
    /* P29 et-bitorum (1 symbola) */ { 69, 1, NIHIL },
    /* P30 aequalitas (3 symbola) */ { 70, 3, "binarium-aequalis" },
    /* P31 aequalitas (3 symbola) */ { 70, 3, "binarium-non-aequalis" },
    /* P32 aequalitas (1 symbola) */ { 70, 1, NIHIL },
    /* P33 comparatio (3 symbola) */ { 71, 3, "binarium-minor" },
    /* P34 comparatio (3 symbola) */ { 71, 3, "binarium-maior" },
    /* P35 comparatio (3 symbola) */ { 71, 3, "binarium-minor-aequalis" },
    /* P36 comparatio (3 symbola) */ { 71, 3, "binarium-maior-aequalis" },
    /* P37 comparatio (1 symbola) */ { 71, 1, NIHIL },
    /* P38 translatio (3 symbola) */ { 72, 3, "binarium-sinistrorsum" },
    /* P39 translatio (3 symbola) */ { 72, 3, "binarium-dextrorsum" },
    /* P40 translatio (1 symbola) */ { 72, 1, NIHIL },
    /* P41 additivum (3 symbola) */ { 73, 3, "binarium-plus" },
    /* P42 additivum (3 symbola) */ { 73, 3, "binarium-minus" },
    /* P43 additivum (1 symbola) */ { 73, 1, NIHIL },
    /* P44 multiplicativum (3 symbola) */ { 74, 3, "binarium-stella" },
    /* P45 multiplicativum (3 symbola) */ { 74, 3, "binarium-solidus" },
    /* P46 multiplicativum (3 symbola) */ { 74, 3, "binarium-percentum" },
    /* P47 multiplicativum (1 symbola) */ { 74, 1, NIHIL },
    /* P48 conversio (4 symbola) */ { 75, 4, "conversio-typus" },
    /* P49 conversio (1 symbola) */ { 75, 1, NIHIL },
    /* P50 unarium (2 symbola) */ { 76, 2, "unarium-incrementum" },
    /* P51 unarium (2 symbola) */ { 76, 2, "unarium-decrementum" },
    /* P52 unarium (2 symbola) */ { 76, 2, "unarium-ampersand" },
    /* P53 unarium (2 symbola) */ { 76, 2, "unarium-stella" },
    /* P54 unarium (2 symbola) */ { 76, 2, "unarium-plus" },
    /* P55 unarium (2 symbola) */ { 76, 2, "unarium-minus" },
    /* P56 unarium (2 symbola) */ { 76, 2, "unarium-tilde" },
    /* P57 unarium (2 symbola) */ { 76, 2, "unarium-exclamatio" },
    /* P58 unarium (2 symbola) */ { 76, 2, "magnitudo-expressionis" },
    /* P59 unarium (4 symbola) */ { 76, 4, "magnitudo-typi" },
    /* P60 unarium (1 symbola) */ { 76, 1, NIHIL },
    /* P61 species-typi (2 symbola) */ { 77, 2, "species-typi-plena" },
    /* P62 species-typi (1 symbola) */ { 77, 1, "species-typi-nuda" },
    /* P63 specificatores (1 symbola) */ { 78, 1, "specificatores-primum" },
    /* P64 typus (1 symbola) */ { 79, 1, "typus-primitivus" },
    /* P65 typus (1 symbola) */ { 79, 1, "typus-nominatus" },
    /* P66 verbum-primitivum (1 symbola) */ { 80, 1, NIHIL },
    /* P67 verbum-primitivum (1 symbola) */ { 80, 1, NIHIL },
    /* P68 verbum-primitivum (1 symbola) */ { 80, 1, NIHIL },
    /* P69 verbum-primitivum (1 symbola) */ { 80, 1, NIHIL },
    /* P70 verbum-primitivum (1 symbola) */ { 80, 1, NIHIL },
    /* P71 verbum-primitivum (1 symbola) */ { 80, 1, NIHIL },
    /* P72 verbum-primitivum (1 symbola) */ { 80, 1, NIHIL },
    /* P73 verbum-primitivum (1 symbola) */ { 80, 1, NIHIL },
    /* P74 verbum-primitivum (1 symbola) */ { 80, 1, NIHIL },
    /* P75 verba-primitiva (1 symbola) */ { 81, 1, "verba-primitiva-primum" },
    /* P76 verba-primitiva (2 symbola) */ { 81, 2, "verba-primitiva-appendere" },
    /* P77 declarator-abstractus (1 symbola) */ { 82, 1, "declarator-abstractus-stella" },
    /* P78 declarator-abstractus (2 symbola) */ { 82, 2, "declarator-abstractus-catena" },
    /* P79 postfixum (4 symbola) */ { 83, 4, "subscriptio-index" },
    /* P80 postfixum (4 symbola) */ { 83, 4, "vocatio-argumenta" },
    /* P81 postfixum (3 symbola) */ { 83, 3, "accessus-punctum" },
    /* P82 postfixum (3 symbola) */ { 83, 3, "accessus-sagitta" },
    /* P83 postfixum (2 symbola) */ { 83, 2, "postcrementum-incrementum" },
    /* P84 postfixum (2 symbola) */ { 83, 2, "postcrementum-decrementum" },
    /* P85 postfixum (1 symbola) */ { 83, 1, NIHIL },
    /* P86 argumenta (0 symbola) */ { 84, 0, NIHIL },
    /* P87 argumenta (1 symbola) */ { 84, 1, NIHIL },
    /* P88 argumenta-plena (1 symbola) */ { 85, 1, "argumenta-primum" },
    /* P89 argumenta-plena (3 symbola) */ { 85, 3, "argumenta-appendere" },
    /* P90 chordae (1 symbola) */ { 86, 1, "chordae-prima" },
    /* P91 chordae (2 symbola) */ { 86, 2, "chordae-appendere" },
    /* P92 primarium (1 symbola) */ { 87, 1, "primarium-identificator" },
    /* P93 primarium (1 symbola) */ { 87, 1, "primarium-integer" },
    /* P94 primarium (1 symbola) */ { 87, 1, "primarium-fluitans" },
    /* P95 primarium (1 symbola) */ { 87, 1, "primarium-character" },
    /* P96 primarium (1 symbola) */ { 87, 1, "primarium-chorda" },
    /* P97 primarium (3 symbola) */ { 87, 3, "primarium-parenthesis" },
    /* P98 __initium__ (2 symbola) */ { 88, 2, NIHIL }
};

/* ==================================================
 * Actiones (laminae per statum in SILVA_C89_STATUS)
 * ================================================== */

hic_manens constans SilvaTabActio SILVA_C89_ACTIONES[] = {
    /* --- status 0 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* IDENTIFICATOR: reducere P0 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INTEGER: reducere P0 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* FLOAT: reducere P0 */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* CHARACTER_LIT: reducere P0 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* STRING_LIT: reducere P0 */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* SIZEOF: reducere P0 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* PLUS: reducere P0 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* MINUS: reducere P0 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* STAR: reducere P0 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* AMPERSAND: reducere P0 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* TILDE: reducere P0 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* EXCLAMATIO: reducere P0 */
    {  46, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INCREMENTUM: reducere P0 */
    {  47, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* DECREMENTUM: reducere P0 */
    {  54, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* PAREN_APERTA: reducere P0 */
    {  58, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* EOF: reducere P0 */
    /* --- status 1 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    {  58, SILVA_TAB_ACTIO_TRANSPONERE,  37, 0 },  /* EOF: transponere -> 37 */
    /* --- status 2 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* IDENTIFICATOR: reducere P1 elementa-appendere */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* INTEGER: reducere P1 elementa-appendere */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* FLOAT: reducere P1 elementa-appendere */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* CHARACTER_LIT: reducere P1 elementa-appendere */
    {   4, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* STRING_LIT: reducere P1 elementa-appendere */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* SIZEOF: reducere P1 elementa-appendere */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* PLUS: reducere P1 elementa-appendere */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* MINUS: reducere P1 elementa-appendere */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* STAR: reducere P1 elementa-appendere */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* AMPERSAND: reducere P1 elementa-appendere */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* TILDE: reducere P1 elementa-appendere */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* EXCLAMATIO: reducere P1 elementa-appendere */
    {  46, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* INCREMENTUM: reducere P1 elementa-appendere */
    {  47, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* DECREMENTUM: reducere P1 elementa-appendere */
    {  54, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* PAREN_APERTA: reducere P1 elementa-appendere */
    {  58, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* EOF: reducere P1 elementa-appendere */
    /* --- status 3 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* IDENTIFICATOR: reducere P2 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* INTEGER: reducere P2 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* FLOAT: reducere P2 */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* CHARACTER_LIT: reducere P2 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* STRING_LIT: reducere P2 */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* SIZEOF: reducere P2 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* PLUS: reducere P2 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* MINUS: reducere P2 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* STAR: reducere P2 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* AMPERSAND: reducere P2 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* TILDE: reducere P2 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* EXCLAMATIO: reducere P2 */
    {  46, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* INCREMENTUM: reducere P2 */
    {  47, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* DECREMENTUM: reducere P2 */
    {  54, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* PAREN_APERTA: reducere P2 */
    {  58, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* EOF: reducere P2 */
    /* --- status 4 --- */
    {  52, SILVA_TAB_ACTIO_TRANSPONERE,  39, 0 },  /* COMMA: transponere -> 39 */
    {  53, SILVA_TAB_ACTIO_TRANSPONERE,  38, 0 },  /* SEMICOLON: transponere -> 38 */
    /* --- status 5 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* COLON: reducere P5 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* COMMA: reducere P5 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* SEMICOLON: reducere P5 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* PAREN_CLAUSA: reducere P5 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* QUADRA_CLAUSA: reducere P5 */
    /* --- status 6 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* PLUS: reducere P49 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MINUS: reducere P49 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* STAR: reducere P49 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SOLIDUS: reducere P49 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* PERCENTUM: reducere P49 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* AMPERSAND: reducere P49 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* BARRA: reducere P49 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* CARET: reducere P49 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* ET_ET: reducere P49 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* VEL_VEL: reducere P49 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* AEQUALIS_AEQUALIS: reducere P49 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* NON_AEQUALIS: reducere P49 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MINOR: reducere P49 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MAIOR: reducere P49 */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MINOR_AEQUALIS: reducere P49 */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MAIOR_AEQUALIS: reducere P49 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SINISTRORSUM: reducere P49 */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* DEXTRORSUM: reducere P49 */
    {  35, SILVA_TAB_ACTIO_TRANSPONERE,  40, 0 },  /* ASSIGNATIO: transponere -> 40 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  41, 0 },  /* PLUS_ASSIGNATIO: transponere -> 41 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  42, 0 },  /* MINUS_ASSIGNATIO: transponere -> 42 */
    {  38, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* STAR_ASSIGNATIO: transponere -> 43 */
    {  39, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* SOLIDUS_ASSIGNATIO: transponere -> 44 */
    {  40, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* PERCENTUM_ASSIGNATIO: transponere -> 45 */
    {  41, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND_ASSIGNATIO: transponere -> 46 */
    {  42, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* BARRA_ASSIGNATIO: transponere -> 47 */
    {  43, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* CARET_ASSIGNATIO: transponere -> 48 */
    {  44, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* SINISTRORSUM_ASSIGNATIO: transponere -> 49 */
    {  45, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* DEXTRORSUM_ASSIGNATIO: transponere -> 50 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* QUAESTIO: reducere P49 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* COLON: reducere P49 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* COMMA: reducere P49 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SEMICOLON: reducere P49 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* PAREN_CLAUSA: reducere P49 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* QUADRA_CLAUSA: reducere P49 */
    /* --- status 7 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* COLON: reducere P17 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* COMMA: reducere P17 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* SEMICOLON: reducere P17 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* PAREN_CLAUSA: reducere P17 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* QUADRA_CLAUSA: reducere P17 */
    /* --- status 8 --- */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* VEL_VEL: transponere -> 52 */
    {  50, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* QUAESTIO: transponere -> 51 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* COLON: reducere P19 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* COMMA: reducere P19 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* SEMICOLON: reducere P19 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* PAREN_CLAUSA: reducere P19 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* QUADRA_CLAUSA: reducere P19 */
    /* --- status 9 --- */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  53, 0 },  /* ET_ET: transponere -> 53 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* VEL_VEL: reducere P21 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* QUAESTIO: reducere P21 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* COLON: reducere P21 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* COMMA: reducere P21 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* SEMICOLON: reducere P21 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* PAREN_CLAUSA: reducere P21 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* QUADRA_CLAUSA: reducere P21 */
    /* --- status 10 --- */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* BARRA: transponere -> 54 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* ET_ET: reducere P23 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* VEL_VEL: reducere P23 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* QUAESTIO: reducere P23 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* COLON: reducere P23 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* COMMA: reducere P23 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* SEMICOLON: reducere P23 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* PAREN_CLAUSA: reducere P23 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* QUADRA_CLAUSA: reducere P23 */
    /* --- status 11 --- */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* BARRA: reducere P25 */
    {  22, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CARET: transponere -> 55 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* ET_ET: reducere P25 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* VEL_VEL: reducere P25 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* QUAESTIO: reducere P25 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* COLON: reducere P25 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* COMMA: reducere P25 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* SEMICOLON: reducere P25 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* PAREN_CLAUSA: reducere P25 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* QUADRA_CLAUSA: reducere P25 */
    /* --- status 12 --- */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* AMPERSAND: transponere -> 56 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* BARRA: reducere P27 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* CARET: reducere P27 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* ET_ET: reducere P27 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* VEL_VEL: reducere P27 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* QUAESTIO: reducere P27 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* COLON: reducere P27 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* COMMA: reducere P27 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* SEMICOLON: reducere P27 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* PAREN_CLAUSA: reducere P27 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* QUADRA_CLAUSA: reducere P27 */
    /* --- status 13 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* AMPERSAND: reducere P29 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* BARRA: reducere P29 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* CARET: reducere P29 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* ET_ET: reducere P29 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* VEL_VEL: reducere P29 */
    {  27, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* AEQUALIS_AEQUALIS: transponere -> 57 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* NON_AEQUALIS: transponere -> 58 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* QUAESTIO: reducere P29 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* COLON: reducere P29 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* COMMA: reducere P29 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* SEMICOLON: reducere P29 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* PAREN_CLAUSA: reducere P29 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* QUADRA_CLAUSA: reducere P29 */
    /* --- status 14 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* AMPERSAND: reducere P32 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* BARRA: reducere P32 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* CARET: reducere P32 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* ET_ET: reducere P32 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* VEL_VEL: reducere P32 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* AEQUALIS_AEQUALIS: reducere P32 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* NON_AEQUALIS: reducere P32 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* MINOR: transponere -> 59 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* MAIOR: transponere -> 60 */
    {  31, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* MINOR_AEQUALIS: transponere -> 61 */
    {  32, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* MAIOR_AEQUALIS: transponere -> 62 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* QUAESTIO: reducere P32 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* COLON: reducere P32 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* COMMA: reducere P32 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* SEMICOLON: reducere P32 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* PAREN_CLAUSA: reducere P32 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* QUADRA_CLAUSA: reducere P32 */
    /* --- status 15 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* AMPERSAND: reducere P37 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* BARRA: reducere P37 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* CARET: reducere P37 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* ET_ET: reducere P37 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* VEL_VEL: reducere P37 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* AEQUALIS_AEQUALIS: reducere P37 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* NON_AEQUALIS: reducere P37 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* MINOR: reducere P37 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* MAIOR: reducere P37 */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* MINOR_AEQUALIS: reducere P37 */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* MAIOR_AEQUALIS: reducere P37 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  63, 0 },  /* SINISTRORSUM: transponere -> 63 */
    {  34, SILVA_TAB_ACTIO_TRANSPONERE,  64, 0 },  /* DEXTRORSUM: transponere -> 64 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* QUAESTIO: reducere P37 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* COLON: reducere P37 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* COMMA: reducere P37 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* SEMICOLON: reducere P37 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* PAREN_CLAUSA: reducere P37 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* QUADRA_CLAUSA: reducere P37 */
    /* --- status 16 --- */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* PLUS: transponere -> 65 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  66, 0 },  /* MINUS: transponere -> 66 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* AMPERSAND: reducere P40 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* BARRA: reducere P40 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* CARET: reducere P40 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* ET_ET: reducere P40 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* VEL_VEL: reducere P40 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* AEQUALIS_AEQUALIS: reducere P40 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* NON_AEQUALIS: reducere P40 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* MINOR: reducere P40 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* MAIOR: reducere P40 */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* MINOR_AEQUALIS: reducere P40 */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* MAIOR_AEQUALIS: reducere P40 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* SINISTRORSUM: reducere P40 */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* DEXTRORSUM: reducere P40 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* QUAESTIO: reducere P40 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* COLON: reducere P40 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* COMMA: reducere P40 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* SEMICOLON: reducere P40 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* PAREN_CLAUSA: reducere P40 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* QUADRA_CLAUSA: reducere P40 */
    /* --- status 17 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* PLUS: reducere P43 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* MINUS: reducere P43 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* STAR: transponere -> 67 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* SOLIDUS: transponere -> 68 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* PERCENTUM: transponere -> 69 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* AMPERSAND: reducere P43 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* BARRA: reducere P43 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* CARET: reducere P43 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* ET_ET: reducere P43 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* VEL_VEL: reducere P43 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* AEQUALIS_AEQUALIS: reducere P43 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* NON_AEQUALIS: reducere P43 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* MINOR: reducere P43 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* MAIOR: reducere P43 */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* MINOR_AEQUALIS: reducere P43 */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* MAIOR_AEQUALIS: reducere P43 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* SINISTRORSUM: reducere P43 */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* DEXTRORSUM: reducere P43 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* QUAESTIO: reducere P43 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* COLON: reducere P43 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* COMMA: reducere P43 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* SEMICOLON: reducere P43 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* PAREN_CLAUSA: reducere P43 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* QUADRA_CLAUSA: reducere P43 */
    /* --- status 18 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* PLUS: reducere P47 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* MINUS: reducere P47 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* STAR: reducere P47 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* SOLIDUS: reducere P47 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* PERCENTUM: reducere P47 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* AMPERSAND: reducere P47 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* BARRA: reducere P47 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* CARET: reducere P47 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* ET_ET: reducere P47 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* VEL_VEL: reducere P47 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* AEQUALIS_AEQUALIS: reducere P47 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* NON_AEQUALIS: reducere P47 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* MINOR: reducere P47 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* MAIOR: reducere P47 */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* MINOR_AEQUALIS: reducere P47 */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* MAIOR_AEQUALIS: reducere P47 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* SINISTRORSUM: reducere P47 */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* DEXTRORSUM: reducere P47 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* QUAESTIO: reducere P47 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* COLON: reducere P47 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* COMMA: reducere P47 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* SEMICOLON: reducere P47 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* PAREN_CLAUSA: reducere P47 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* QUADRA_CLAUSA: reducere P47 */
    /* --- status 19 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  75, 0 },  /* IDENTIFICATOR: transponere -> 75 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* VOID: transponere -> 76 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  77, 0 },  /* CHAR: transponere -> 77 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  78, 0 },  /* SHORT: transponere -> 78 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* INT: transponere -> 79 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  80, 0 },  /* LONG: transponere -> 80 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  81, 0 },  /* FLOAT_KW: transponere -> 81 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  82, 0 },  /* DOUBLE: transponere -> 82 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  83, 0 },  /* SIGNED: transponere -> 83 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  84, 0 },  /* UNSIGNED: transponere -> 84 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 20 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  87, 0 },  /* PAREN_APERTA: transponere -> 87 */
    /* --- status 21 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  87, 0 },  /* PAREN_APERTA: transponere -> 87 */
    /* --- status 22 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 23 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 24 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 25 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 26 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 27 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 28 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  97, 0 },  /* PAREN_APERTA: transponere -> 97 */
    /* --- status 29 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* PLUS: reducere P60 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* MINUS: reducere P60 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* STAR: reducere P60 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* SOLIDUS: reducere P60 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* PERCENTUM: reducere P60 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* AMPERSAND: reducere P60 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* BARRA: reducere P60 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* CARET: reducere P60 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* ET_ET: reducere P60 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* VEL_VEL: reducere P60 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* AEQUALIS_AEQUALIS: reducere P60 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* NON_AEQUALIS: reducere P60 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* MINOR: reducere P60 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* MAIOR: reducere P60 */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* MINOR_AEQUALIS: reducere P60 */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* MAIOR_AEQUALIS: reducere P60 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* SINISTRORSUM: reducere P60 */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* DEXTRORSUM: reducere P60 */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* ASSIGNATIO: reducere P60 */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* PLUS_ASSIGNATIO: reducere P60 */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* MINUS_ASSIGNATIO: reducere P60 */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* STAR_ASSIGNATIO: reducere P60 */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P60 */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P60 */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P60 */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* BARRA_ASSIGNATIO: reducere P60 */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* CARET_ASSIGNATIO: reducere P60 */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P60 */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P60 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE, 102, 0 },  /* INCREMENTUM: transponere -> 102 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE, 103, 0 },  /* DECREMENTUM: transponere -> 103 */
    {  48, SILVA_TAB_ACTIO_TRANSPONERE, 100, 0 },  /* PUNCTUM: transponere -> 100 */
    {  49, SILVA_TAB_ACTIO_TRANSPONERE, 101, 0 },  /* SAGITTA: transponere -> 101 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* QUAESTIO: reducere P60 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* COLON: reducere P60 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* COMMA: reducere P60 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* SEMICOLON: reducere P60 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  99, 0 },  /* PAREN_APERTA: transponere -> 99 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* PAREN_CLAUSA: reducere P60 */
    {  56, SILVA_TAB_ACTIO_TRANSPONERE,  98, 0 },  /* QUADRA_APERTA: transponere -> 98 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* QUADRA_CLAUSA: reducere P60 */
    /* --- status 30 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* PLUS: reducere P85 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* MINUS: reducere P85 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* STAR: reducere P85 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* SOLIDUS: reducere P85 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* PERCENTUM: reducere P85 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* AMPERSAND: reducere P85 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* BARRA: reducere P85 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* CARET: reducere P85 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* ET_ET: reducere P85 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* VEL_VEL: reducere P85 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* AEQUALIS_AEQUALIS: reducere P85 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* NON_AEQUALIS: reducere P85 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* MINOR: reducere P85 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* MAIOR: reducere P85 */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* MINOR_AEQUALIS: reducere P85 */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* MAIOR_AEQUALIS: reducere P85 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* SINISTRORSUM: reducere P85 */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* DEXTRORSUM: reducere P85 */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* ASSIGNATIO: reducere P85 */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* PLUS_ASSIGNATIO: reducere P85 */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* MINUS_ASSIGNATIO: reducere P85 */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* STAR_ASSIGNATIO: reducere P85 */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P85 */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P85 */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P85 */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* BARRA_ASSIGNATIO: reducere P85 */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* CARET_ASSIGNATIO: reducere P85 */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P85 */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P85 */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* INCREMENTUM: reducere P85 */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* DECREMENTUM: reducere P85 */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* PUNCTUM: reducere P85 */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* SAGITTA: reducere P85 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* QUAESTIO: reducere P85 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* COLON: reducere P85 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* COMMA: reducere P85 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* SEMICOLON: reducere P85 */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* PAREN_APERTA: reducere P85 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* PAREN_CLAUSA: reducere P85 */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* QUADRA_APERTA: reducere P85 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* QUADRA_CLAUSA: reducere P85 */
    /* --- status 31 --- */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* STRING_LIT: reducere P90 chordae-prima */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* PLUS: reducere P90 chordae-prima */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* MINUS: reducere P90 chordae-prima */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* STAR: reducere P90 chordae-prima */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* SOLIDUS: reducere P90 chordae-prima */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* PERCENTUM: reducere P90 chordae-prima */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* AMPERSAND: reducere P90 chordae-prima */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* BARRA: reducere P90 chordae-prima */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* CARET: reducere P90 chordae-prima */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* ET_ET: reducere P90 chordae-prima */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* VEL_VEL: reducere P90 chordae-prima */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* AEQUALIS_AEQUALIS: reducere P90 chordae-prima */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* NON_AEQUALIS: reducere P90 chordae-prima */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* MINOR: reducere P90 chordae-prima */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* MAIOR: reducere P90 chordae-prima */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* MINOR_AEQUALIS: reducere P90 chordae-prima */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* MAIOR_AEQUALIS: reducere P90 chordae-prima */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* SINISTRORSUM: reducere P90 chordae-prima */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* DEXTRORSUM: reducere P90 chordae-prima */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* ASSIGNATIO: reducere P90 chordae-prima */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* PLUS_ASSIGNATIO: reducere P90 chordae-prima */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* MINUS_ASSIGNATIO: reducere P90 chordae-prima */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* STAR_ASSIGNATIO: reducere P90 chordae-prima */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P90 chordae-prima */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P90 chordae-prima */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P90 chordae-prima */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* BARRA_ASSIGNATIO: reducere P90 chordae-prima */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* CARET_ASSIGNATIO: reducere P90 chordae-prima */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P90 chordae-prima */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P90 chordae-prima */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* INCREMENTUM: reducere P90 chordae-prima */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* DECREMENTUM: reducere P90 chordae-prima */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* PUNCTUM: reducere P90 chordae-prima */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* SAGITTA: reducere P90 chordae-prima */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* QUAESTIO: reducere P90 chordae-prima */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* COLON: reducere P90 chordae-prima */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* COMMA: reducere P90 chordae-prima */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* SEMICOLON: reducere P90 chordae-prima */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* PAREN_APERTA: reducere P90 chordae-prima */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* PAREN_CLAUSA: reducere P90 chordae-prima */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* QUADRA_APERTA: reducere P90 chordae-prima */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* QUADRA_CLAUSA: reducere P90 chordae-prima */
    /* --- status 32 --- */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE, 104, 0 },  /* STRING_LIT: transponere -> 104 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* PLUS: reducere P96 primarium-chorda */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* MINUS: reducere P96 primarium-chorda */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* STAR: reducere P96 primarium-chorda */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* SOLIDUS: reducere P96 primarium-chorda */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* PERCENTUM: reducere P96 primarium-chorda */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* AMPERSAND: reducere P96 primarium-chorda */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* BARRA: reducere P96 primarium-chorda */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* CARET: reducere P96 primarium-chorda */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* ET_ET: reducere P96 primarium-chorda */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* VEL_VEL: reducere P96 primarium-chorda */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* AEQUALIS_AEQUALIS: reducere P96 primarium-chorda */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* NON_AEQUALIS: reducere P96 primarium-chorda */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* MINOR: reducere P96 primarium-chorda */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* MAIOR: reducere P96 primarium-chorda */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* MINOR_AEQUALIS: reducere P96 primarium-chorda */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* MAIOR_AEQUALIS: reducere P96 primarium-chorda */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* SINISTRORSUM: reducere P96 primarium-chorda */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* DEXTRORSUM: reducere P96 primarium-chorda */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* ASSIGNATIO: reducere P96 primarium-chorda */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* PLUS_ASSIGNATIO: reducere P96 primarium-chorda */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* MINUS_ASSIGNATIO: reducere P96 primarium-chorda */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* STAR_ASSIGNATIO: reducere P96 primarium-chorda */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P96 primarium-chorda */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P96 primarium-chorda */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P96 primarium-chorda */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* BARRA_ASSIGNATIO: reducere P96 primarium-chorda */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* CARET_ASSIGNATIO: reducere P96 primarium-chorda */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P96 primarium-chorda */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P96 primarium-chorda */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* INCREMENTUM: reducere P96 primarium-chorda */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* DECREMENTUM: reducere P96 primarium-chorda */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* PUNCTUM: reducere P96 primarium-chorda */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* SAGITTA: reducere P96 primarium-chorda */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* QUAESTIO: reducere P96 primarium-chorda */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* COLON: reducere P96 primarium-chorda */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* COMMA: reducere P96 primarium-chorda */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* SEMICOLON: reducere P96 primarium-chorda */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* PAREN_APERTA: reducere P96 primarium-chorda */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* PAREN_CLAUSA: reducere P96 primarium-chorda */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* QUADRA_APERTA: reducere P96 primarium-chorda */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* QUADRA_CLAUSA: reducere P96 primarium-chorda */
    /* --- status 33 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PLUS: reducere P92 primarium-identificator */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MINUS: reducere P92 primarium-identificator */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* STAR: reducere P92 primarium-identificator */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SOLIDUS: reducere P92 primarium-identificator */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PERCENTUM: reducere P92 primarium-identificator */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* AMPERSAND: reducere P92 primarium-identificator */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* BARRA: reducere P92 primarium-identificator */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* CARET: reducere P92 primarium-identificator */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* ET_ET: reducere P92 primarium-identificator */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* VEL_VEL: reducere P92 primarium-identificator */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* AEQUALIS_AEQUALIS: reducere P92 primarium-identificator */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* NON_AEQUALIS: reducere P92 primarium-identificator */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MINOR: reducere P92 primarium-identificator */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MAIOR: reducere P92 primarium-identificator */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MINOR_AEQUALIS: reducere P92 primarium-identificator */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MAIOR_AEQUALIS: reducere P92 primarium-identificator */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SINISTRORSUM: reducere P92 primarium-identificator */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* DEXTRORSUM: reducere P92 primarium-identificator */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* ASSIGNATIO: reducere P92 primarium-identificator */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PLUS_ASSIGNATIO: reducere P92 primarium-identificator */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MINUS_ASSIGNATIO: reducere P92 primarium-identificator */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* STAR_ASSIGNATIO: reducere P92 primarium-identificator */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P92 primarium-identificator */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P92 primarium-identificator */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P92 primarium-identificator */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* BARRA_ASSIGNATIO: reducere P92 primarium-identificator */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* CARET_ASSIGNATIO: reducere P92 primarium-identificator */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P92 primarium-identificator */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P92 primarium-identificator */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* INCREMENTUM: reducere P92 primarium-identificator */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* DECREMENTUM: reducere P92 primarium-identificator */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PUNCTUM: reducere P92 primarium-identificator */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SAGITTA: reducere P92 primarium-identificator */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* QUAESTIO: reducere P92 primarium-identificator */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* COLON: reducere P92 primarium-identificator */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* COMMA: reducere P92 primarium-identificator */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SEMICOLON: reducere P92 primarium-identificator */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PAREN_APERTA: reducere P92 primarium-identificator */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PAREN_CLAUSA: reducere P92 primarium-identificator */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* QUADRA_APERTA: reducere P92 primarium-identificator */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* QUADRA_CLAUSA: reducere P92 primarium-identificator */
    /* --- status 34 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* PLUS: reducere P93 primarium-integer */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* MINUS: reducere P93 primarium-integer */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* STAR: reducere P93 primarium-integer */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* SOLIDUS: reducere P93 primarium-integer */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* PERCENTUM: reducere P93 primarium-integer */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* AMPERSAND: reducere P93 primarium-integer */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* BARRA: reducere P93 primarium-integer */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* CARET: reducere P93 primarium-integer */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* ET_ET: reducere P93 primarium-integer */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* VEL_VEL: reducere P93 primarium-integer */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* AEQUALIS_AEQUALIS: reducere P93 primarium-integer */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* NON_AEQUALIS: reducere P93 primarium-integer */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* MINOR: reducere P93 primarium-integer */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* MAIOR: reducere P93 primarium-integer */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* MINOR_AEQUALIS: reducere P93 primarium-integer */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* MAIOR_AEQUALIS: reducere P93 primarium-integer */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* SINISTRORSUM: reducere P93 primarium-integer */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* DEXTRORSUM: reducere P93 primarium-integer */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* ASSIGNATIO: reducere P93 primarium-integer */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* PLUS_ASSIGNATIO: reducere P93 primarium-integer */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* MINUS_ASSIGNATIO: reducere P93 primarium-integer */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* STAR_ASSIGNATIO: reducere P93 primarium-integer */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P93 primarium-integer */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P93 primarium-integer */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P93 primarium-integer */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* BARRA_ASSIGNATIO: reducere P93 primarium-integer */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* CARET_ASSIGNATIO: reducere P93 primarium-integer */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P93 primarium-integer */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P93 primarium-integer */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* INCREMENTUM: reducere P93 primarium-integer */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* DECREMENTUM: reducere P93 primarium-integer */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* PUNCTUM: reducere P93 primarium-integer */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* SAGITTA: reducere P93 primarium-integer */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* QUAESTIO: reducere P93 primarium-integer */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* COLON: reducere P93 primarium-integer */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* COMMA: reducere P93 primarium-integer */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* SEMICOLON: reducere P93 primarium-integer */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* PAREN_APERTA: reducere P93 primarium-integer */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* PAREN_CLAUSA: reducere P93 primarium-integer */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* QUADRA_APERTA: reducere P93 primarium-integer */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* QUADRA_CLAUSA: reducere P93 primarium-integer */
    /* --- status 35 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* PLUS: reducere P94 primarium-fluitans */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* MINUS: reducere P94 primarium-fluitans */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* STAR: reducere P94 primarium-fluitans */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* SOLIDUS: reducere P94 primarium-fluitans */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* PERCENTUM: reducere P94 primarium-fluitans */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* AMPERSAND: reducere P94 primarium-fluitans */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* BARRA: reducere P94 primarium-fluitans */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* CARET: reducere P94 primarium-fluitans */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* ET_ET: reducere P94 primarium-fluitans */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* VEL_VEL: reducere P94 primarium-fluitans */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* AEQUALIS_AEQUALIS: reducere P94 primarium-fluitans */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* NON_AEQUALIS: reducere P94 primarium-fluitans */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* MINOR: reducere P94 primarium-fluitans */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* MAIOR: reducere P94 primarium-fluitans */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* MINOR_AEQUALIS: reducere P94 primarium-fluitans */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* MAIOR_AEQUALIS: reducere P94 primarium-fluitans */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* SINISTRORSUM: reducere P94 primarium-fluitans */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* DEXTRORSUM: reducere P94 primarium-fluitans */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* ASSIGNATIO: reducere P94 primarium-fluitans */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* PLUS_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* MINUS_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* STAR_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* BARRA_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* CARET_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P94 primarium-fluitans */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* INCREMENTUM: reducere P94 primarium-fluitans */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* DECREMENTUM: reducere P94 primarium-fluitans */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* PUNCTUM: reducere P94 primarium-fluitans */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* SAGITTA: reducere P94 primarium-fluitans */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* QUAESTIO: reducere P94 primarium-fluitans */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* COLON: reducere P94 primarium-fluitans */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* COMMA: reducere P94 primarium-fluitans */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* SEMICOLON: reducere P94 primarium-fluitans */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* PAREN_APERTA: reducere P94 primarium-fluitans */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* PAREN_CLAUSA: reducere P94 primarium-fluitans */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* QUADRA_APERTA: reducere P94 primarium-fluitans */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* QUADRA_CLAUSA: reducere P94 primarium-fluitans */
    /* --- status 36 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* PLUS: reducere P95 primarium-character */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* MINUS: reducere P95 primarium-character */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* STAR: reducere P95 primarium-character */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* SOLIDUS: reducere P95 primarium-character */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* PERCENTUM: reducere P95 primarium-character */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* AMPERSAND: reducere P95 primarium-character */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* BARRA: reducere P95 primarium-character */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* CARET: reducere P95 primarium-character */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* ET_ET: reducere P95 primarium-character */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* VEL_VEL: reducere P95 primarium-character */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* AEQUALIS_AEQUALIS: reducere P95 primarium-character */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* NON_AEQUALIS: reducere P95 primarium-character */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* MINOR: reducere P95 primarium-character */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* MAIOR: reducere P95 primarium-character */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* MINOR_AEQUALIS: reducere P95 primarium-character */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* MAIOR_AEQUALIS: reducere P95 primarium-character */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* SINISTRORSUM: reducere P95 primarium-character */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* DEXTRORSUM: reducere P95 primarium-character */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* ASSIGNATIO: reducere P95 primarium-character */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* PLUS_ASSIGNATIO: reducere P95 primarium-character */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* MINUS_ASSIGNATIO: reducere P95 primarium-character */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* STAR_ASSIGNATIO: reducere P95 primarium-character */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P95 primarium-character */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P95 primarium-character */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P95 primarium-character */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* BARRA_ASSIGNATIO: reducere P95 primarium-character */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* CARET_ASSIGNATIO: reducere P95 primarium-character */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P95 primarium-character */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P95 primarium-character */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* INCREMENTUM: reducere P95 primarium-character */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* DECREMENTUM: reducere P95 primarium-character */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* PUNCTUM: reducere P95 primarium-character */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* SAGITTA: reducere P95 primarium-character */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* QUAESTIO: reducere P95 primarium-character */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* COLON: reducere P95 primarium-character */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* COMMA: reducere P95 primarium-character */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* SEMICOLON: reducere P95 primarium-character */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* PAREN_APERTA: reducere P95 primarium-character */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* PAREN_CLAUSA: reducere P95 primarium-character */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* QUADRA_APERTA: reducere P95 primarium-character */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* QUADRA_CLAUSA: reducere P95 primarium-character */
    /* --- status 37 --- */
    {  -1, SILVA_TAB_ACTIO_ACCIPERE,   0, 0 },  /* $: accipere */
    /* --- status 38 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* IDENTIFICATOR: reducere P3 sententia-expr */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* INTEGER: reducere P3 sententia-expr */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* FLOAT: reducere P3 sententia-expr */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* CHARACTER_LIT: reducere P3 sententia-expr */
    {   4, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* STRING_LIT: reducere P3 sententia-expr */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* SIZEOF: reducere P3 sententia-expr */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* PLUS: reducere P3 sententia-expr */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* MINUS: reducere P3 sententia-expr */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* STAR: reducere P3 sententia-expr */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* AMPERSAND: reducere P3 sententia-expr */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* TILDE: reducere P3 sententia-expr */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* EXCLAMATIO: reducere P3 sententia-expr */
    {  46, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* INCREMENTUM: reducere P3 sententia-expr */
    {  47, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* DECREMENTUM: reducere P3 sententia-expr */
    {  54, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* PAREN_APERTA: reducere P3 sententia-expr */
    {  58, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* EOF: reducere P3 sententia-expr */
    /* --- status 39 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 40 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 41 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 42 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 43 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 44 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 45 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 46 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 47 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 48 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 49 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 50 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 51 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 52 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 53 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 54 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 55 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 56 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 57 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 58 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 59 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 60 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 61 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 62 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 63 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 64 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 65 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 66 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 67 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 68 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 69 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 70 --- */
    {  52, SILVA_TAB_ACTIO_TRANSPONERE,  39, 0 },  /* COMMA: transponere -> 39 */
    {  55, SILVA_TAB_ACTIO_TRANSPONERE, 136, 0 },  /* PAREN_CLAUSA: transponere -> 136 */
    /* --- status 71 --- */
    {  55, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* PAREN_CLAUSA: transponere -> 137 */
    /* --- status 72 --- */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE, 139, 0 },  /* STAR: transponere -> 139 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  62, 0 },  /* PAREN_CLAUSA: reducere P62 species-typi-nuda */
    /* --- status 73 --- */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  63, 0 },  /* STAR: reducere P63 specificatores-primum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  63, 0 },  /* PAREN_CLAUSA: reducere P63 specificatores-primum */
    /* --- status 74 --- */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* VOID: transponere -> 76 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  77, 0 },  /* CHAR: transponere -> 77 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  78, 0 },  /* SHORT: transponere -> 78 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* INT: transponere -> 79 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  80, 0 },  /* LONG: transponere -> 80 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  81, 0 },  /* FLOAT_KW: transponere -> 81 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  82, 0 },  /* DOUBLE: transponere -> 82 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  83, 0 },  /* SIGNED: transponere -> 83 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  84, 0 },  /* UNSIGNED: transponere -> 84 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  64, 0 },  /* STAR: reducere P64 typus-primitivus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  64, 0 },  /* PAREN_CLAUSA: reducere P64 typus-primitivus */
    /* --- status 75 [CONFLICTUS SERVATUS] --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PLUS: reducere P92 primarium-identificator */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MINUS: reducere P92 primarium-identificator */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* STAR: reducere P65 typus-nominatus */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* STAR: reducere P92 primarium-identificator */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SOLIDUS: reducere P92 primarium-identificator */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PERCENTUM: reducere P92 primarium-identificator */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* AMPERSAND: reducere P92 primarium-identificator */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* BARRA: reducere P92 primarium-identificator */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* CARET: reducere P92 primarium-identificator */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* ET_ET: reducere P92 primarium-identificator */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* VEL_VEL: reducere P92 primarium-identificator */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* AEQUALIS_AEQUALIS: reducere P92 primarium-identificator */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* NON_AEQUALIS: reducere P92 primarium-identificator */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MINOR: reducere P92 primarium-identificator */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MAIOR: reducere P92 primarium-identificator */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MINOR_AEQUALIS: reducere P92 primarium-identificator */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MAIOR_AEQUALIS: reducere P92 primarium-identificator */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SINISTRORSUM: reducere P92 primarium-identificator */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* DEXTRORSUM: reducere P92 primarium-identificator */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* ASSIGNATIO: reducere P92 primarium-identificator */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PLUS_ASSIGNATIO: reducere P92 primarium-identificator */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* MINUS_ASSIGNATIO: reducere P92 primarium-identificator */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* STAR_ASSIGNATIO: reducere P92 primarium-identificator */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P92 primarium-identificator */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P92 primarium-identificator */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P92 primarium-identificator */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* BARRA_ASSIGNATIO: reducere P92 primarium-identificator */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* CARET_ASSIGNATIO: reducere P92 primarium-identificator */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P92 primarium-identificator */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P92 primarium-identificator */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* INCREMENTUM: reducere P92 primarium-identificator */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* DECREMENTUM: reducere P92 primarium-identificator */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PUNCTUM: reducere P92 primarium-identificator */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SAGITTA: reducere P92 primarium-identificator */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* QUAESTIO: reducere P92 primarium-identificator */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* COMMA: reducere P92 primarium-identificator */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PAREN_APERTA: reducere P92 primarium-identificator */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* PAREN_CLAUSA: reducere P65 typus-nominatus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PAREN_CLAUSA: reducere P92 primarium-identificator */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* QUADRA_APERTA: reducere P92 primarium-identificator */
    /* --- status 76 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* VOID: reducere P66 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* CHAR: reducere P66 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* SHORT: reducere P66 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* INT: reducere P66 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* LONG: reducere P66 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* FLOAT_KW: reducere P66 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* DOUBLE: reducere P66 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* SIGNED: reducere P66 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* UNSIGNED: reducere P66 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* STAR: reducere P66 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* PAREN_CLAUSA: reducere P66 */
    /* --- status 77 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* VOID: reducere P67 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* CHAR: reducere P67 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* SHORT: reducere P67 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* INT: reducere P67 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* LONG: reducere P67 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* FLOAT_KW: reducere P67 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* DOUBLE: reducere P67 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* SIGNED: reducere P67 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* UNSIGNED: reducere P67 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* STAR: reducere P67 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* PAREN_CLAUSA: reducere P67 */
    /* --- status 78 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* VOID: reducere P68 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* CHAR: reducere P68 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* SHORT: reducere P68 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* INT: reducere P68 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* LONG: reducere P68 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* FLOAT_KW: reducere P68 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* DOUBLE: reducere P68 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* SIGNED: reducere P68 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* UNSIGNED: reducere P68 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* STAR: reducere P68 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* PAREN_CLAUSA: reducere P68 */
    /* --- status 79 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* VOID: reducere P69 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* CHAR: reducere P69 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* SHORT: reducere P69 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* INT: reducere P69 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* LONG: reducere P69 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* FLOAT_KW: reducere P69 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* DOUBLE: reducere P69 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* SIGNED: reducere P69 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* UNSIGNED: reducere P69 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* STAR: reducere P69 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* PAREN_CLAUSA: reducere P69 */
    /* --- status 80 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* VOID: reducere P70 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* CHAR: reducere P70 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* SHORT: reducere P70 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* INT: reducere P70 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* LONG: reducere P70 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* FLOAT_KW: reducere P70 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* DOUBLE: reducere P70 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* SIGNED: reducere P70 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* UNSIGNED: reducere P70 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* STAR: reducere P70 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* PAREN_CLAUSA: reducere P70 */
    /* --- status 81 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* VOID: reducere P71 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* CHAR: reducere P71 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* SHORT: reducere P71 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* INT: reducere P71 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* LONG: reducere P71 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* FLOAT_KW: reducere P71 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* DOUBLE: reducere P71 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* SIGNED: reducere P71 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* UNSIGNED: reducere P71 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* STAR: reducere P71 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* PAREN_CLAUSA: reducere P71 */
    /* --- status 82 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* VOID: reducere P72 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* CHAR: reducere P72 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* SHORT: reducere P72 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* INT: reducere P72 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* LONG: reducere P72 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* FLOAT_KW: reducere P72 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* DOUBLE: reducere P72 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* SIGNED: reducere P72 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* UNSIGNED: reducere P72 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* STAR: reducere P72 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* PAREN_CLAUSA: reducere P72 */
    /* --- status 83 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* VOID: reducere P73 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* CHAR: reducere P73 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* SHORT: reducere P73 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* INT: reducere P73 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* LONG: reducere P73 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* FLOAT_KW: reducere P73 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* DOUBLE: reducere P73 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* SIGNED: reducere P73 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* UNSIGNED: reducere P73 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* STAR: reducere P73 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* PAREN_CLAUSA: reducere P73 */
    /* --- status 84 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* VOID: reducere P74 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* CHAR: reducere P74 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* SHORT: reducere P74 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* INT: reducere P74 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* LONG: reducere P74 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* FLOAT_KW: reducere P74 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* DOUBLE: reducere P74 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* SIGNED: reducere P74 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* UNSIGNED: reducere P74 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* STAR: reducere P74 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* PAREN_CLAUSA: reducere P74 */
    /* --- status 85 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* VOID: reducere P75 verba-primitiva-primum */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* CHAR: reducere P75 verba-primitiva-primum */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* SHORT: reducere P75 verba-primitiva-primum */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* INT: reducere P75 verba-primitiva-primum */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* LONG: reducere P75 verba-primitiva-primum */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* FLOAT_KW: reducere P75 verba-primitiva-primum */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* DOUBLE: reducere P75 verba-primitiva-primum */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* SIGNED: reducere P75 verba-primitiva-primum */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* UNSIGNED: reducere P75 verba-primitiva-primum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* STAR: reducere P75 verba-primitiva-primum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* PAREN_CLAUSA: reducere P75 verba-primitiva-primum */
    /* --- status 86 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* PLUS: reducere P50 unarium-incrementum */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* MINUS: reducere P50 unarium-incrementum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* STAR: reducere P50 unarium-incrementum */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* SOLIDUS: reducere P50 unarium-incrementum */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* PERCENTUM: reducere P50 unarium-incrementum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* AMPERSAND: reducere P50 unarium-incrementum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* BARRA: reducere P50 unarium-incrementum */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* CARET: reducere P50 unarium-incrementum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* ET_ET: reducere P50 unarium-incrementum */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* VEL_VEL: reducere P50 unarium-incrementum */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* AEQUALIS_AEQUALIS: reducere P50 unarium-incrementum */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* NON_AEQUALIS: reducere P50 unarium-incrementum */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* MINOR: reducere P50 unarium-incrementum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* MAIOR: reducere P50 unarium-incrementum */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* MINOR_AEQUALIS: reducere P50 unarium-incrementum */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* MAIOR_AEQUALIS: reducere P50 unarium-incrementum */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* SINISTRORSUM: reducere P50 unarium-incrementum */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* DEXTRORSUM: reducere P50 unarium-incrementum */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* ASSIGNATIO: reducere P50 unarium-incrementum */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* PLUS_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* MINUS_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* STAR_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* BARRA_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* CARET_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P50 unarium-incrementum */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* QUAESTIO: reducere P50 unarium-incrementum */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* COLON: reducere P50 unarium-incrementum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* COMMA: reducere P50 unarium-incrementum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* SEMICOLON: reducere P50 unarium-incrementum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* PAREN_CLAUSA: reducere P50 unarium-incrementum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* QUADRA_CLAUSA: reducere P50 unarium-incrementum */
    /* --- status 87 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 88 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* PLUS: reducere P51 unarium-decrementum */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* MINUS: reducere P51 unarium-decrementum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* STAR: reducere P51 unarium-decrementum */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* SOLIDUS: reducere P51 unarium-decrementum */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* PERCENTUM: reducere P51 unarium-decrementum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* AMPERSAND: reducere P51 unarium-decrementum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* BARRA: reducere P51 unarium-decrementum */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* CARET: reducere P51 unarium-decrementum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* ET_ET: reducere P51 unarium-decrementum */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* VEL_VEL: reducere P51 unarium-decrementum */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* AEQUALIS_AEQUALIS: reducere P51 unarium-decrementum */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* NON_AEQUALIS: reducere P51 unarium-decrementum */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* MINOR: reducere P51 unarium-decrementum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* MAIOR: reducere P51 unarium-decrementum */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* MINOR_AEQUALIS: reducere P51 unarium-decrementum */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* MAIOR_AEQUALIS: reducere P51 unarium-decrementum */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* SINISTRORSUM: reducere P51 unarium-decrementum */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* DEXTRORSUM: reducere P51 unarium-decrementum */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* ASSIGNATIO: reducere P51 unarium-decrementum */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* PLUS_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* MINUS_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* STAR_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* BARRA_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* CARET_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P51 unarium-decrementum */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* QUAESTIO: reducere P51 unarium-decrementum */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* COLON: reducere P51 unarium-decrementum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* COMMA: reducere P51 unarium-decrementum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* SEMICOLON: reducere P51 unarium-decrementum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* PAREN_CLAUSA: reducere P51 unarium-decrementum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* QUADRA_CLAUSA: reducere P51 unarium-decrementum */
    /* --- status 89 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* PLUS: reducere P49 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MINUS: reducere P49 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* STAR: reducere P49 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SOLIDUS: reducere P49 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* PERCENTUM: reducere P49 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* AMPERSAND: reducere P49 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* BARRA: reducere P49 */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* CARET: reducere P49 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* ET_ET: reducere P49 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* VEL_VEL: reducere P49 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* AEQUALIS_AEQUALIS: reducere P49 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* NON_AEQUALIS: reducere P49 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MINOR: reducere P49 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MAIOR: reducere P49 */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MINOR_AEQUALIS: reducere P49 */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MAIOR_AEQUALIS: reducere P49 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SINISTRORSUM: reducere P49 */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* DEXTRORSUM: reducere P49 */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* ASSIGNATIO: reducere P49 */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* PLUS_ASSIGNATIO: reducere P49 */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* MINUS_ASSIGNATIO: reducere P49 */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* STAR_ASSIGNATIO: reducere P49 */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P49 */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P49 */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P49 */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* BARRA_ASSIGNATIO: reducere P49 */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* CARET_ASSIGNATIO: reducere P49 */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P49 */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P49 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* QUAESTIO: reducere P49 */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* COLON: reducere P49 */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* COMMA: reducere P49 */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SEMICOLON: reducere P49 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* PAREN_CLAUSA: reducere P49 */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* QUADRA_CLAUSA: reducere P49 */
    /* --- status 90 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* PLUS: reducere P52 unarium-ampersand */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* MINUS: reducere P52 unarium-ampersand */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* STAR: reducere P52 unarium-ampersand */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* SOLIDUS: reducere P52 unarium-ampersand */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* PERCENTUM: reducere P52 unarium-ampersand */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* AMPERSAND: reducere P52 unarium-ampersand */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* BARRA: reducere P52 unarium-ampersand */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* CARET: reducere P52 unarium-ampersand */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* ET_ET: reducere P52 unarium-ampersand */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* VEL_VEL: reducere P52 unarium-ampersand */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* AEQUALIS_AEQUALIS: reducere P52 unarium-ampersand */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* NON_AEQUALIS: reducere P52 unarium-ampersand */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* MINOR: reducere P52 unarium-ampersand */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* MAIOR: reducere P52 unarium-ampersand */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* MINOR_AEQUALIS: reducere P52 unarium-ampersand */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* MAIOR_AEQUALIS: reducere P52 unarium-ampersand */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* SINISTRORSUM: reducere P52 unarium-ampersand */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* DEXTRORSUM: reducere P52 unarium-ampersand */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* ASSIGNATIO: reducere P52 unarium-ampersand */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* PLUS_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* MINUS_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* STAR_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* BARRA_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* CARET_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P52 unarium-ampersand */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* QUAESTIO: reducere P52 unarium-ampersand */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* COLON: reducere P52 unarium-ampersand */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* COMMA: reducere P52 unarium-ampersand */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* SEMICOLON: reducere P52 unarium-ampersand */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* PAREN_CLAUSA: reducere P52 unarium-ampersand */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* QUADRA_CLAUSA: reducere P52 unarium-ampersand */
    /* --- status 91 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* PLUS: reducere P53 unarium-stella */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* MINUS: reducere P53 unarium-stella */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* STAR: reducere P53 unarium-stella */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* SOLIDUS: reducere P53 unarium-stella */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* PERCENTUM: reducere P53 unarium-stella */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* AMPERSAND: reducere P53 unarium-stella */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* BARRA: reducere P53 unarium-stella */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* CARET: reducere P53 unarium-stella */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* ET_ET: reducere P53 unarium-stella */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* VEL_VEL: reducere P53 unarium-stella */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* AEQUALIS_AEQUALIS: reducere P53 unarium-stella */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* NON_AEQUALIS: reducere P53 unarium-stella */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* MINOR: reducere P53 unarium-stella */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* MAIOR: reducere P53 unarium-stella */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* MINOR_AEQUALIS: reducere P53 unarium-stella */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* MAIOR_AEQUALIS: reducere P53 unarium-stella */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* SINISTRORSUM: reducere P53 unarium-stella */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* DEXTRORSUM: reducere P53 unarium-stella */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* ASSIGNATIO: reducere P53 unarium-stella */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* PLUS_ASSIGNATIO: reducere P53 unarium-stella */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* MINUS_ASSIGNATIO: reducere P53 unarium-stella */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* STAR_ASSIGNATIO: reducere P53 unarium-stella */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P53 unarium-stella */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P53 unarium-stella */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P53 unarium-stella */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* BARRA_ASSIGNATIO: reducere P53 unarium-stella */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* CARET_ASSIGNATIO: reducere P53 unarium-stella */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P53 unarium-stella */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P53 unarium-stella */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* QUAESTIO: reducere P53 unarium-stella */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* COLON: reducere P53 unarium-stella */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* COMMA: reducere P53 unarium-stella */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* SEMICOLON: reducere P53 unarium-stella */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* PAREN_CLAUSA: reducere P53 unarium-stella */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* QUADRA_CLAUSA: reducere P53 unarium-stella */
    /* --- status 92 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* PLUS: reducere P54 unarium-plus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* MINUS: reducere P54 unarium-plus */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* STAR: reducere P54 unarium-plus */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* SOLIDUS: reducere P54 unarium-plus */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* PERCENTUM: reducere P54 unarium-plus */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* AMPERSAND: reducere P54 unarium-plus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* BARRA: reducere P54 unarium-plus */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* CARET: reducere P54 unarium-plus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* ET_ET: reducere P54 unarium-plus */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* VEL_VEL: reducere P54 unarium-plus */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* AEQUALIS_AEQUALIS: reducere P54 unarium-plus */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* NON_AEQUALIS: reducere P54 unarium-plus */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* MINOR: reducere P54 unarium-plus */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* MAIOR: reducere P54 unarium-plus */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* MINOR_AEQUALIS: reducere P54 unarium-plus */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* MAIOR_AEQUALIS: reducere P54 unarium-plus */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* SINISTRORSUM: reducere P54 unarium-plus */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* DEXTRORSUM: reducere P54 unarium-plus */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* ASSIGNATIO: reducere P54 unarium-plus */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* PLUS_ASSIGNATIO: reducere P54 unarium-plus */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* MINUS_ASSIGNATIO: reducere P54 unarium-plus */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* STAR_ASSIGNATIO: reducere P54 unarium-plus */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P54 unarium-plus */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P54 unarium-plus */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P54 unarium-plus */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* BARRA_ASSIGNATIO: reducere P54 unarium-plus */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* CARET_ASSIGNATIO: reducere P54 unarium-plus */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P54 unarium-plus */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P54 unarium-plus */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* QUAESTIO: reducere P54 unarium-plus */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* COLON: reducere P54 unarium-plus */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* COMMA: reducere P54 unarium-plus */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* SEMICOLON: reducere P54 unarium-plus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* PAREN_CLAUSA: reducere P54 unarium-plus */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* QUADRA_CLAUSA: reducere P54 unarium-plus */
    /* --- status 93 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* PLUS: reducere P55 unarium-minus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* MINUS: reducere P55 unarium-minus */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* STAR: reducere P55 unarium-minus */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* SOLIDUS: reducere P55 unarium-minus */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* PERCENTUM: reducere P55 unarium-minus */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* AMPERSAND: reducere P55 unarium-minus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* BARRA: reducere P55 unarium-minus */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* CARET: reducere P55 unarium-minus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* ET_ET: reducere P55 unarium-minus */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* VEL_VEL: reducere P55 unarium-minus */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* AEQUALIS_AEQUALIS: reducere P55 unarium-minus */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* NON_AEQUALIS: reducere P55 unarium-minus */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* MINOR: reducere P55 unarium-minus */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* MAIOR: reducere P55 unarium-minus */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* MINOR_AEQUALIS: reducere P55 unarium-minus */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* MAIOR_AEQUALIS: reducere P55 unarium-minus */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* SINISTRORSUM: reducere P55 unarium-minus */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* DEXTRORSUM: reducere P55 unarium-minus */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* ASSIGNATIO: reducere P55 unarium-minus */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* PLUS_ASSIGNATIO: reducere P55 unarium-minus */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* MINUS_ASSIGNATIO: reducere P55 unarium-minus */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* STAR_ASSIGNATIO: reducere P55 unarium-minus */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P55 unarium-minus */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P55 unarium-minus */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P55 unarium-minus */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* BARRA_ASSIGNATIO: reducere P55 unarium-minus */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* CARET_ASSIGNATIO: reducere P55 unarium-minus */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P55 unarium-minus */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P55 unarium-minus */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* QUAESTIO: reducere P55 unarium-minus */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* COLON: reducere P55 unarium-minus */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* COMMA: reducere P55 unarium-minus */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* SEMICOLON: reducere P55 unarium-minus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* PAREN_CLAUSA: reducere P55 unarium-minus */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* QUADRA_CLAUSA: reducere P55 unarium-minus */
    /* --- status 94 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* PLUS: reducere P56 unarium-tilde */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* MINUS: reducere P56 unarium-tilde */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* STAR: reducere P56 unarium-tilde */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* SOLIDUS: reducere P56 unarium-tilde */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* PERCENTUM: reducere P56 unarium-tilde */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* AMPERSAND: reducere P56 unarium-tilde */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* BARRA: reducere P56 unarium-tilde */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* CARET: reducere P56 unarium-tilde */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* ET_ET: reducere P56 unarium-tilde */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* VEL_VEL: reducere P56 unarium-tilde */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* AEQUALIS_AEQUALIS: reducere P56 unarium-tilde */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* NON_AEQUALIS: reducere P56 unarium-tilde */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* MINOR: reducere P56 unarium-tilde */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* MAIOR: reducere P56 unarium-tilde */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* MINOR_AEQUALIS: reducere P56 unarium-tilde */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* MAIOR_AEQUALIS: reducere P56 unarium-tilde */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* SINISTRORSUM: reducere P56 unarium-tilde */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* DEXTRORSUM: reducere P56 unarium-tilde */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* ASSIGNATIO: reducere P56 unarium-tilde */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* PLUS_ASSIGNATIO: reducere P56 unarium-tilde */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* MINUS_ASSIGNATIO: reducere P56 unarium-tilde */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* STAR_ASSIGNATIO: reducere P56 unarium-tilde */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P56 unarium-tilde */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P56 unarium-tilde */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P56 unarium-tilde */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* BARRA_ASSIGNATIO: reducere P56 unarium-tilde */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* CARET_ASSIGNATIO: reducere P56 unarium-tilde */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P56 unarium-tilde */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P56 unarium-tilde */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* QUAESTIO: reducere P56 unarium-tilde */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* COLON: reducere P56 unarium-tilde */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* COMMA: reducere P56 unarium-tilde */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* SEMICOLON: reducere P56 unarium-tilde */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* PAREN_CLAUSA: reducere P56 unarium-tilde */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* QUADRA_CLAUSA: reducere P56 unarium-tilde */
    /* --- status 95 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* PLUS: reducere P57 unarium-exclamatio */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* MINUS: reducere P57 unarium-exclamatio */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* STAR: reducere P57 unarium-exclamatio */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* SOLIDUS: reducere P57 unarium-exclamatio */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* PERCENTUM: reducere P57 unarium-exclamatio */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* AMPERSAND: reducere P57 unarium-exclamatio */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* BARRA: reducere P57 unarium-exclamatio */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* CARET: reducere P57 unarium-exclamatio */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* ET_ET: reducere P57 unarium-exclamatio */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* VEL_VEL: reducere P57 unarium-exclamatio */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* AEQUALIS_AEQUALIS: reducere P57 unarium-exclamatio */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* NON_AEQUALIS: reducere P57 unarium-exclamatio */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* MINOR: reducere P57 unarium-exclamatio */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* MAIOR: reducere P57 unarium-exclamatio */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* MINOR_AEQUALIS: reducere P57 unarium-exclamatio */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* MAIOR_AEQUALIS: reducere P57 unarium-exclamatio */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* SINISTRORSUM: reducere P57 unarium-exclamatio */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* DEXTRORSUM: reducere P57 unarium-exclamatio */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* PLUS_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* MINUS_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* STAR_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* BARRA_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* CARET_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P57 unarium-exclamatio */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* QUAESTIO: reducere P57 unarium-exclamatio */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* COLON: reducere P57 unarium-exclamatio */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* COMMA: reducere P57 unarium-exclamatio */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* SEMICOLON: reducere P57 unarium-exclamatio */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* PAREN_CLAUSA: reducere P57 unarium-exclamatio */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* QUADRA_CLAUSA: reducere P57 unarium-exclamatio */
    /* --- status 96 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* PLUS: reducere P58 magnitudo-expressionis */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* MINUS: reducere P58 magnitudo-expressionis */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* STAR: reducere P58 magnitudo-expressionis */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* SOLIDUS: reducere P58 magnitudo-expressionis */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* PERCENTUM: reducere P58 magnitudo-expressionis */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* AMPERSAND: reducere P58 magnitudo-expressionis */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* BARRA: reducere P58 magnitudo-expressionis */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* CARET: reducere P58 magnitudo-expressionis */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* ET_ET: reducere P58 magnitudo-expressionis */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* VEL_VEL: reducere P58 magnitudo-expressionis */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* AEQUALIS_AEQUALIS: reducere P58 magnitudo-expressionis */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* NON_AEQUALIS: reducere P58 magnitudo-expressionis */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* MINOR: reducere P58 magnitudo-expressionis */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* MAIOR: reducere P58 magnitudo-expressionis */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* MINOR_AEQUALIS: reducere P58 magnitudo-expressionis */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* MAIOR_AEQUALIS: reducere P58 magnitudo-expressionis */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* SINISTRORSUM: reducere P58 magnitudo-expressionis */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* DEXTRORSUM: reducere P58 magnitudo-expressionis */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* PLUS_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* MINUS_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* STAR_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* BARRA_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* CARET_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P58 magnitudo-expressionis */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* QUAESTIO: reducere P58 magnitudo-expressionis */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* COLON: reducere P58 magnitudo-expressionis */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* COMMA: reducere P58 magnitudo-expressionis */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* SEMICOLON: reducere P58 magnitudo-expressionis */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* PAREN_CLAUSA: reducere P58 magnitudo-expressionis */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* QUADRA_CLAUSA: reducere P58 magnitudo-expressionis */
    /* --- status 97 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  75, 0 },  /* IDENTIFICATOR: transponere -> 75 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* VOID: transponere -> 76 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  77, 0 },  /* CHAR: transponere -> 77 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  78, 0 },  /* SHORT: transponere -> 78 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* INT: transponere -> 79 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  80, 0 },  /* LONG: transponere -> 80 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  81, 0 },  /* FLOAT_KW: transponere -> 81 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  82, 0 },  /* DOUBLE: transponere -> 82 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  83, 0 },  /* SIGNED: transponere -> 83 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  84, 0 },  /* UNSIGNED: transponere -> 84 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 98 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 99 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  86, 0 },  /* PAREN_CLAUSA: reducere P86 */
    /* --- status 100 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 146, 0 },  /* IDENTIFICATOR: transponere -> 146 */
    /* --- status 101 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 147, 0 },  /* IDENTIFICATOR: transponere -> 147 */
    /* --- status 102 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* PLUS: reducere P83 postcrementum-incrementum */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* MINUS: reducere P83 postcrementum-incrementum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* STAR: reducere P83 postcrementum-incrementum */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* SOLIDUS: reducere P83 postcrementum-incrementum */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* PERCENTUM: reducere P83 postcrementum-incrementum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* AMPERSAND: reducere P83 postcrementum-incrementum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* BARRA: reducere P83 postcrementum-incrementum */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* CARET: reducere P83 postcrementum-incrementum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* ET_ET: reducere P83 postcrementum-incrementum */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* VEL_VEL: reducere P83 postcrementum-incrementum */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* AEQUALIS_AEQUALIS: reducere P83 postcrementum-incrementum */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* NON_AEQUALIS: reducere P83 postcrementum-incrementum */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* MINOR: reducere P83 postcrementum-incrementum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* MAIOR: reducere P83 postcrementum-incrementum */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* MINOR_AEQUALIS: reducere P83 postcrementum-incrementum */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* MAIOR_AEQUALIS: reducere P83 postcrementum-incrementum */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* SINISTRORSUM: reducere P83 postcrementum-incrementum */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* DEXTRORSUM: reducere P83 postcrementum-incrementum */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* PLUS_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* MINUS_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* STAR_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* BARRA_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* CARET_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P83 postcrementum-incrementum */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* INCREMENTUM: reducere P83 postcrementum-incrementum */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* DECREMENTUM: reducere P83 postcrementum-incrementum */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* PUNCTUM: reducere P83 postcrementum-incrementum */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* SAGITTA: reducere P83 postcrementum-incrementum */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* QUAESTIO: reducere P83 postcrementum-incrementum */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* COLON: reducere P83 postcrementum-incrementum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* COMMA: reducere P83 postcrementum-incrementum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* SEMICOLON: reducere P83 postcrementum-incrementum */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* PAREN_APERTA: reducere P83 postcrementum-incrementum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* PAREN_CLAUSA: reducere P83 postcrementum-incrementum */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* QUADRA_APERTA: reducere P83 postcrementum-incrementum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* QUADRA_CLAUSA: reducere P83 postcrementum-incrementum */
    /* --- status 103 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* PLUS: reducere P84 postcrementum-decrementum */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* MINUS: reducere P84 postcrementum-decrementum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* STAR: reducere P84 postcrementum-decrementum */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* SOLIDUS: reducere P84 postcrementum-decrementum */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* PERCENTUM: reducere P84 postcrementum-decrementum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* AMPERSAND: reducere P84 postcrementum-decrementum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* BARRA: reducere P84 postcrementum-decrementum */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* CARET: reducere P84 postcrementum-decrementum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* ET_ET: reducere P84 postcrementum-decrementum */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* VEL_VEL: reducere P84 postcrementum-decrementum */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* AEQUALIS_AEQUALIS: reducere P84 postcrementum-decrementum */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* NON_AEQUALIS: reducere P84 postcrementum-decrementum */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* MINOR: reducere P84 postcrementum-decrementum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* MAIOR: reducere P84 postcrementum-decrementum */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* MINOR_AEQUALIS: reducere P84 postcrementum-decrementum */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* MAIOR_AEQUALIS: reducere P84 postcrementum-decrementum */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* SINISTRORSUM: reducere P84 postcrementum-decrementum */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* DEXTRORSUM: reducere P84 postcrementum-decrementum */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* PLUS_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* MINUS_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* STAR_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* BARRA_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* CARET_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P84 postcrementum-decrementum */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* INCREMENTUM: reducere P84 postcrementum-decrementum */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* DECREMENTUM: reducere P84 postcrementum-decrementum */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* PUNCTUM: reducere P84 postcrementum-decrementum */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* SAGITTA: reducere P84 postcrementum-decrementum */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* QUAESTIO: reducere P84 postcrementum-decrementum */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* COLON: reducere P84 postcrementum-decrementum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* COMMA: reducere P84 postcrementum-decrementum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* SEMICOLON: reducere P84 postcrementum-decrementum */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* PAREN_APERTA: reducere P84 postcrementum-decrementum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* PAREN_CLAUSA: reducere P84 postcrementum-decrementum */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* QUADRA_APERTA: reducere P84 postcrementum-decrementum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* QUADRA_CLAUSA: reducere P84 postcrementum-decrementum */
    /* --- status 104 --- */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* STRING_LIT: reducere P91 chordae-appendere */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* PLUS: reducere P91 chordae-appendere */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* MINUS: reducere P91 chordae-appendere */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* STAR: reducere P91 chordae-appendere */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* SOLIDUS: reducere P91 chordae-appendere */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* PERCENTUM: reducere P91 chordae-appendere */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* AMPERSAND: reducere P91 chordae-appendere */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* BARRA: reducere P91 chordae-appendere */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* CARET: reducere P91 chordae-appendere */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* ET_ET: reducere P91 chordae-appendere */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* VEL_VEL: reducere P91 chordae-appendere */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* AEQUALIS_AEQUALIS: reducere P91 chordae-appendere */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* NON_AEQUALIS: reducere P91 chordae-appendere */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* MINOR: reducere P91 chordae-appendere */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* MAIOR: reducere P91 chordae-appendere */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* MINOR_AEQUALIS: reducere P91 chordae-appendere */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* MAIOR_AEQUALIS: reducere P91 chordae-appendere */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* SINISTRORSUM: reducere P91 chordae-appendere */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* DEXTRORSUM: reducere P91 chordae-appendere */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* ASSIGNATIO: reducere P91 chordae-appendere */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* PLUS_ASSIGNATIO: reducere P91 chordae-appendere */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* MINUS_ASSIGNATIO: reducere P91 chordae-appendere */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* STAR_ASSIGNATIO: reducere P91 chordae-appendere */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P91 chordae-appendere */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P91 chordae-appendere */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P91 chordae-appendere */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* BARRA_ASSIGNATIO: reducere P91 chordae-appendere */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* CARET_ASSIGNATIO: reducere P91 chordae-appendere */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P91 chordae-appendere */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P91 chordae-appendere */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* INCREMENTUM: reducere P91 chordae-appendere */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* DECREMENTUM: reducere P91 chordae-appendere */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* PUNCTUM: reducere P91 chordae-appendere */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* SAGITTA: reducere P91 chordae-appendere */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* QUAESTIO: reducere P91 chordae-appendere */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* COLON: reducere P91 chordae-appendere */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* COMMA: reducere P91 chordae-appendere */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* SEMICOLON: reducere P91 chordae-appendere */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* PAREN_APERTA: reducere P91 chordae-appendere */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* PAREN_CLAUSA: reducere P91 chordae-appendere */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* QUADRA_APERTA: reducere P91 chordae-appendere */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* QUADRA_CLAUSA: reducere P91 chordae-appendere */
    /* --- status 105 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* COLON: reducere P4 virgula-series */
    {  52, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* COMMA: reducere P4 virgula-series */
    {  53, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* SEMICOLON: reducere P4 virgula-series */
    {  55, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* PAREN_CLAUSA: reducere P4 virgula-series */
    {  57, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* QUADRA_CLAUSA: reducere P4 virgula-series */
    /* --- status 106 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* COLON: reducere P6 assignatio-simplex */
    {  52, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* COMMA: reducere P6 assignatio-simplex */
    {  53, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* SEMICOLON: reducere P6 assignatio-simplex */
    {  55, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* PAREN_CLAUSA: reducere P6 assignatio-simplex */
    {  57, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* QUADRA_CLAUSA: reducere P6 assignatio-simplex */
    /* --- status 107 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* COLON: reducere P7 assignatio-plus */
    {  52, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* COMMA: reducere P7 assignatio-plus */
    {  53, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* SEMICOLON: reducere P7 assignatio-plus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* PAREN_CLAUSA: reducere P7 assignatio-plus */
    {  57, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* QUADRA_CLAUSA: reducere P7 assignatio-plus */
    /* --- status 108 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* COLON: reducere P8 assignatio-minus */
    {  52, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* COMMA: reducere P8 assignatio-minus */
    {  53, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* SEMICOLON: reducere P8 assignatio-minus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* PAREN_CLAUSA: reducere P8 assignatio-minus */
    {  57, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* QUADRA_CLAUSA: reducere P8 assignatio-minus */
    /* --- status 109 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* COLON: reducere P9 assignatio-stella */
    {  52, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* COMMA: reducere P9 assignatio-stella */
    {  53, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* SEMICOLON: reducere P9 assignatio-stella */
    {  55, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* PAREN_CLAUSA: reducere P9 assignatio-stella */
    {  57, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* QUADRA_CLAUSA: reducere P9 assignatio-stella */
    /* --- status 110 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* COLON: reducere P10 assignatio-solidus */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* COMMA: reducere P10 assignatio-solidus */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* SEMICOLON: reducere P10 assignatio-solidus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* PAREN_CLAUSA: reducere P10 assignatio-solidus */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* QUADRA_CLAUSA: reducere P10 assignatio-solidus */
    /* --- status 111 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* COLON: reducere P11 assignatio-percentum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* COMMA: reducere P11 assignatio-percentum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* SEMICOLON: reducere P11 assignatio-percentum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* PAREN_CLAUSA: reducere P11 assignatio-percentum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* QUADRA_CLAUSA: reducere P11 assignatio-percentum */
    /* --- status 112 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* COLON: reducere P12 assignatio-ampersand */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* COMMA: reducere P12 assignatio-ampersand */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* SEMICOLON: reducere P12 assignatio-ampersand */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* PAREN_CLAUSA: reducere P12 assignatio-ampersand */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* QUADRA_CLAUSA: reducere P12 assignatio-ampersand */
    /* --- status 113 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* COLON: reducere P13 assignatio-barra */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* COMMA: reducere P13 assignatio-barra */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* SEMICOLON: reducere P13 assignatio-barra */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* PAREN_CLAUSA: reducere P13 assignatio-barra */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* QUADRA_CLAUSA: reducere P13 assignatio-barra */
    /* --- status 114 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* COLON: reducere P14 assignatio-caret */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* COMMA: reducere P14 assignatio-caret */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* SEMICOLON: reducere P14 assignatio-caret */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PAREN_CLAUSA: reducere P14 assignatio-caret */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* QUADRA_CLAUSA: reducere P14 assignatio-caret */
    /* --- status 115 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* COLON: reducere P15 assignatio-sinistrorsum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* COMMA: reducere P15 assignatio-sinistrorsum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* SEMICOLON: reducere P15 assignatio-sinistrorsum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PAREN_CLAUSA: reducere P15 assignatio-sinistrorsum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* QUADRA_CLAUSA: reducere P15 assignatio-sinistrorsum */
    /* --- status 116 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* COLON: reducere P16 assignatio-dextrorsum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* COMMA: reducere P16 assignatio-dextrorsum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* SEMICOLON: reducere P16 assignatio-dextrorsum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* PAREN_CLAUSA: reducere P16 assignatio-dextrorsum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* QUADRA_CLAUSA: reducere P16 assignatio-dextrorsum */
    /* --- status 117 --- */
    {  51, SILVA_TAB_ACTIO_TRANSPONERE, 148, 0 },  /* COLON: transponere -> 148 */
    {  52, SILVA_TAB_ACTIO_TRANSPONERE,  39, 0 },  /* COMMA: transponere -> 39 */
    /* --- status 118 --- */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  53, 0 },  /* ET_ET: transponere -> 53 */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* VEL_VEL: reducere P20 binarium-disiunctio */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* QUAESTIO: reducere P20 binarium-disiunctio */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* COLON: reducere P20 binarium-disiunctio */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* COMMA: reducere P20 binarium-disiunctio */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* SEMICOLON: reducere P20 binarium-disiunctio */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* PAREN_CLAUSA: reducere P20 binarium-disiunctio */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* QUADRA_CLAUSA: reducere P20 binarium-disiunctio */
    /* --- status 119 --- */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* BARRA: transponere -> 54 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* ET_ET: reducere P22 binarium-coniunctio */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* VEL_VEL: reducere P22 binarium-coniunctio */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* QUAESTIO: reducere P22 binarium-coniunctio */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* COLON: reducere P22 binarium-coniunctio */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* COMMA: reducere P22 binarium-coniunctio */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* SEMICOLON: reducere P22 binarium-coniunctio */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* PAREN_CLAUSA: reducere P22 binarium-coniunctio */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* QUADRA_CLAUSA: reducere P22 binarium-coniunctio */
    /* --- status 120 --- */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* BARRA: reducere P24 binarium-vel */
    {  22, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CARET: transponere -> 55 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* ET_ET: reducere P24 binarium-vel */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* VEL_VEL: reducere P24 binarium-vel */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* QUAESTIO: reducere P24 binarium-vel */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* COLON: reducere P24 binarium-vel */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* COMMA: reducere P24 binarium-vel */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* SEMICOLON: reducere P24 binarium-vel */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* PAREN_CLAUSA: reducere P24 binarium-vel */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* QUADRA_CLAUSA: reducere P24 binarium-vel */
    /* --- status 121 --- */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* AMPERSAND: transponere -> 56 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* BARRA: reducere P26 binarium-aut */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* CARET: reducere P26 binarium-aut */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* ET_ET: reducere P26 binarium-aut */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* VEL_VEL: reducere P26 binarium-aut */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* QUAESTIO: reducere P26 binarium-aut */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* COLON: reducere P26 binarium-aut */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* COMMA: reducere P26 binarium-aut */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* SEMICOLON: reducere P26 binarium-aut */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* PAREN_CLAUSA: reducere P26 binarium-aut */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* QUADRA_CLAUSA: reducere P26 binarium-aut */
    /* --- status 122 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* AMPERSAND: reducere P28 binarium-et */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* BARRA: reducere P28 binarium-et */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* CARET: reducere P28 binarium-et */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* ET_ET: reducere P28 binarium-et */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* VEL_VEL: reducere P28 binarium-et */
    {  27, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* AEQUALIS_AEQUALIS: transponere -> 57 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* NON_AEQUALIS: transponere -> 58 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* QUAESTIO: reducere P28 binarium-et */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* COLON: reducere P28 binarium-et */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* COMMA: reducere P28 binarium-et */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* SEMICOLON: reducere P28 binarium-et */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* PAREN_CLAUSA: reducere P28 binarium-et */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* QUADRA_CLAUSA: reducere P28 binarium-et */
    /* --- status 123 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* AMPERSAND: reducere P30 binarium-aequalis */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* BARRA: reducere P30 binarium-aequalis */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* CARET: reducere P30 binarium-aequalis */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* ET_ET: reducere P30 binarium-aequalis */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* VEL_VEL: reducere P30 binarium-aequalis */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* AEQUALIS_AEQUALIS: reducere P30 binarium-aequalis */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* NON_AEQUALIS: reducere P30 binarium-aequalis */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* MINOR: transponere -> 59 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* MAIOR: transponere -> 60 */
    {  31, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* MINOR_AEQUALIS: transponere -> 61 */
    {  32, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* MAIOR_AEQUALIS: transponere -> 62 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* QUAESTIO: reducere P30 binarium-aequalis */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* COLON: reducere P30 binarium-aequalis */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* COMMA: reducere P30 binarium-aequalis */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* SEMICOLON: reducere P30 binarium-aequalis */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* PAREN_CLAUSA: reducere P30 binarium-aequalis */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* QUADRA_CLAUSA: reducere P30 binarium-aequalis */
    /* --- status 124 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* AMPERSAND: reducere P31 binarium-non-aequalis */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* BARRA: reducere P31 binarium-non-aequalis */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* CARET: reducere P31 binarium-non-aequalis */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* ET_ET: reducere P31 binarium-non-aequalis */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* VEL_VEL: reducere P31 binarium-non-aequalis */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* AEQUALIS_AEQUALIS: reducere P31 binarium-non-aequalis */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* NON_AEQUALIS: reducere P31 binarium-non-aequalis */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* MINOR: transponere -> 59 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* MAIOR: transponere -> 60 */
    {  31, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* MINOR_AEQUALIS: transponere -> 61 */
    {  32, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* MAIOR_AEQUALIS: transponere -> 62 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* QUAESTIO: reducere P31 binarium-non-aequalis */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* COLON: reducere P31 binarium-non-aequalis */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* COMMA: reducere P31 binarium-non-aequalis */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* SEMICOLON: reducere P31 binarium-non-aequalis */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* PAREN_CLAUSA: reducere P31 binarium-non-aequalis */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* QUADRA_CLAUSA: reducere P31 binarium-non-aequalis */
    /* --- status 125 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* AMPERSAND: reducere P33 binarium-minor */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* BARRA: reducere P33 binarium-minor */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* CARET: reducere P33 binarium-minor */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* ET_ET: reducere P33 binarium-minor */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* VEL_VEL: reducere P33 binarium-minor */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* AEQUALIS_AEQUALIS: reducere P33 binarium-minor */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* NON_AEQUALIS: reducere P33 binarium-minor */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* MINOR: reducere P33 binarium-minor */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* MAIOR: reducere P33 binarium-minor */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* MINOR_AEQUALIS: reducere P33 binarium-minor */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* MAIOR_AEQUALIS: reducere P33 binarium-minor */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  63, 0 },  /* SINISTRORSUM: transponere -> 63 */
    {  34, SILVA_TAB_ACTIO_TRANSPONERE,  64, 0 },  /* DEXTRORSUM: transponere -> 64 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* QUAESTIO: reducere P33 binarium-minor */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* COLON: reducere P33 binarium-minor */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* COMMA: reducere P33 binarium-minor */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* SEMICOLON: reducere P33 binarium-minor */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* PAREN_CLAUSA: reducere P33 binarium-minor */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* QUADRA_CLAUSA: reducere P33 binarium-minor */
    /* --- status 126 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* AMPERSAND: reducere P34 binarium-maior */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* BARRA: reducere P34 binarium-maior */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* CARET: reducere P34 binarium-maior */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* ET_ET: reducere P34 binarium-maior */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* VEL_VEL: reducere P34 binarium-maior */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* AEQUALIS_AEQUALIS: reducere P34 binarium-maior */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* NON_AEQUALIS: reducere P34 binarium-maior */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* MINOR: reducere P34 binarium-maior */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* MAIOR: reducere P34 binarium-maior */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* MINOR_AEQUALIS: reducere P34 binarium-maior */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* MAIOR_AEQUALIS: reducere P34 binarium-maior */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  63, 0 },  /* SINISTRORSUM: transponere -> 63 */
    {  34, SILVA_TAB_ACTIO_TRANSPONERE,  64, 0 },  /* DEXTRORSUM: transponere -> 64 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* QUAESTIO: reducere P34 binarium-maior */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* COLON: reducere P34 binarium-maior */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* COMMA: reducere P34 binarium-maior */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* SEMICOLON: reducere P34 binarium-maior */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* PAREN_CLAUSA: reducere P34 binarium-maior */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* QUADRA_CLAUSA: reducere P34 binarium-maior */
    /* --- status 127 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* AMPERSAND: reducere P35 binarium-minor-aequalis */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* BARRA: reducere P35 binarium-minor-aequalis */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* CARET: reducere P35 binarium-minor-aequalis */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* ET_ET: reducere P35 binarium-minor-aequalis */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* VEL_VEL: reducere P35 binarium-minor-aequalis */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* AEQUALIS_AEQUALIS: reducere P35 binarium-minor-aequalis */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* NON_AEQUALIS: reducere P35 binarium-minor-aequalis */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* MINOR: reducere P35 binarium-minor-aequalis */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* MAIOR: reducere P35 binarium-minor-aequalis */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* MINOR_AEQUALIS: reducere P35 binarium-minor-aequalis */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* MAIOR_AEQUALIS: reducere P35 binarium-minor-aequalis */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  63, 0 },  /* SINISTRORSUM: transponere -> 63 */
    {  34, SILVA_TAB_ACTIO_TRANSPONERE,  64, 0 },  /* DEXTRORSUM: transponere -> 64 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* QUAESTIO: reducere P35 binarium-minor-aequalis */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* COLON: reducere P35 binarium-minor-aequalis */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* COMMA: reducere P35 binarium-minor-aequalis */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* SEMICOLON: reducere P35 binarium-minor-aequalis */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* PAREN_CLAUSA: reducere P35 binarium-minor-aequalis */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* QUADRA_CLAUSA: reducere P35 binarium-minor-aequalis */
    /* --- status 128 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* AMPERSAND: reducere P36 binarium-maior-aequalis */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* BARRA: reducere P36 binarium-maior-aequalis */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* CARET: reducere P36 binarium-maior-aequalis */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* ET_ET: reducere P36 binarium-maior-aequalis */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* VEL_VEL: reducere P36 binarium-maior-aequalis */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* AEQUALIS_AEQUALIS: reducere P36 binarium-maior-aequalis */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* NON_AEQUALIS: reducere P36 binarium-maior-aequalis */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* MINOR: reducere P36 binarium-maior-aequalis */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* MAIOR: reducere P36 binarium-maior-aequalis */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* MINOR_AEQUALIS: reducere P36 binarium-maior-aequalis */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* MAIOR_AEQUALIS: reducere P36 binarium-maior-aequalis */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  63, 0 },  /* SINISTRORSUM: transponere -> 63 */
    {  34, SILVA_TAB_ACTIO_TRANSPONERE,  64, 0 },  /* DEXTRORSUM: transponere -> 64 */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* QUAESTIO: reducere P36 binarium-maior-aequalis */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* COLON: reducere P36 binarium-maior-aequalis */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* COMMA: reducere P36 binarium-maior-aequalis */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* SEMICOLON: reducere P36 binarium-maior-aequalis */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* PAREN_CLAUSA: reducere P36 binarium-maior-aequalis */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* QUADRA_CLAUSA: reducere P36 binarium-maior-aequalis */
    /* --- status 129 --- */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* PLUS: transponere -> 65 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  66, 0 },  /* MINUS: transponere -> 66 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* AMPERSAND: reducere P38 binarium-sinistrorsum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* BARRA: reducere P38 binarium-sinistrorsum */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* CARET: reducere P38 binarium-sinistrorsum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* ET_ET: reducere P38 binarium-sinistrorsum */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* VEL_VEL: reducere P38 binarium-sinistrorsum */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* AEQUALIS_AEQUALIS: reducere P38 binarium-sinistrorsum */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* NON_AEQUALIS: reducere P38 binarium-sinistrorsum */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* MINOR: reducere P38 binarium-sinistrorsum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* MAIOR: reducere P38 binarium-sinistrorsum */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* MINOR_AEQUALIS: reducere P38 binarium-sinistrorsum */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* MAIOR_AEQUALIS: reducere P38 binarium-sinistrorsum */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* SINISTRORSUM: reducere P38 binarium-sinistrorsum */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* DEXTRORSUM: reducere P38 binarium-sinistrorsum */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* QUAESTIO: reducere P38 binarium-sinistrorsum */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* COLON: reducere P38 binarium-sinistrorsum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* COMMA: reducere P38 binarium-sinistrorsum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* SEMICOLON: reducere P38 binarium-sinistrorsum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* PAREN_CLAUSA: reducere P38 binarium-sinistrorsum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* QUADRA_CLAUSA: reducere P38 binarium-sinistrorsum */
    /* --- status 130 --- */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* PLUS: transponere -> 65 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  66, 0 },  /* MINUS: transponere -> 66 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* AMPERSAND: reducere P39 binarium-dextrorsum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* BARRA: reducere P39 binarium-dextrorsum */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* CARET: reducere P39 binarium-dextrorsum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* ET_ET: reducere P39 binarium-dextrorsum */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* VEL_VEL: reducere P39 binarium-dextrorsum */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* AEQUALIS_AEQUALIS: reducere P39 binarium-dextrorsum */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* NON_AEQUALIS: reducere P39 binarium-dextrorsum */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* MINOR: reducere P39 binarium-dextrorsum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* MAIOR: reducere P39 binarium-dextrorsum */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* MINOR_AEQUALIS: reducere P39 binarium-dextrorsum */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* MAIOR_AEQUALIS: reducere P39 binarium-dextrorsum */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* SINISTRORSUM: reducere P39 binarium-dextrorsum */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* DEXTRORSUM: reducere P39 binarium-dextrorsum */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* QUAESTIO: reducere P39 binarium-dextrorsum */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* COLON: reducere P39 binarium-dextrorsum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* COMMA: reducere P39 binarium-dextrorsum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* SEMICOLON: reducere P39 binarium-dextrorsum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* PAREN_CLAUSA: reducere P39 binarium-dextrorsum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* QUADRA_CLAUSA: reducere P39 binarium-dextrorsum */
    /* --- status 131 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* PLUS: reducere P41 binarium-plus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* MINUS: reducere P41 binarium-plus */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* STAR: transponere -> 67 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* SOLIDUS: transponere -> 68 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* PERCENTUM: transponere -> 69 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* AMPERSAND: reducere P41 binarium-plus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* BARRA: reducere P41 binarium-plus */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* CARET: reducere P41 binarium-plus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* ET_ET: reducere P41 binarium-plus */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* VEL_VEL: reducere P41 binarium-plus */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* AEQUALIS_AEQUALIS: reducere P41 binarium-plus */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* NON_AEQUALIS: reducere P41 binarium-plus */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* MINOR: reducere P41 binarium-plus */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* MAIOR: reducere P41 binarium-plus */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* MINOR_AEQUALIS: reducere P41 binarium-plus */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* MAIOR_AEQUALIS: reducere P41 binarium-plus */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* SINISTRORSUM: reducere P41 binarium-plus */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* DEXTRORSUM: reducere P41 binarium-plus */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* QUAESTIO: reducere P41 binarium-plus */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* COLON: reducere P41 binarium-plus */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* COMMA: reducere P41 binarium-plus */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* SEMICOLON: reducere P41 binarium-plus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* PAREN_CLAUSA: reducere P41 binarium-plus */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* QUADRA_CLAUSA: reducere P41 binarium-plus */
    /* --- status 132 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* PLUS: reducere P42 binarium-minus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* MINUS: reducere P42 binarium-minus */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* STAR: transponere -> 67 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* SOLIDUS: transponere -> 68 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* PERCENTUM: transponere -> 69 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* AMPERSAND: reducere P42 binarium-minus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* BARRA: reducere P42 binarium-minus */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* CARET: reducere P42 binarium-minus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* ET_ET: reducere P42 binarium-minus */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* VEL_VEL: reducere P42 binarium-minus */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* AEQUALIS_AEQUALIS: reducere P42 binarium-minus */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* NON_AEQUALIS: reducere P42 binarium-minus */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* MINOR: reducere P42 binarium-minus */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* MAIOR: reducere P42 binarium-minus */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* MINOR_AEQUALIS: reducere P42 binarium-minus */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* MAIOR_AEQUALIS: reducere P42 binarium-minus */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* SINISTRORSUM: reducere P42 binarium-minus */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* DEXTRORSUM: reducere P42 binarium-minus */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* QUAESTIO: reducere P42 binarium-minus */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* COLON: reducere P42 binarium-minus */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* COMMA: reducere P42 binarium-minus */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* SEMICOLON: reducere P42 binarium-minus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* PAREN_CLAUSA: reducere P42 binarium-minus */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* QUADRA_CLAUSA: reducere P42 binarium-minus */
    /* --- status 133 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* PLUS: reducere P44 binarium-stella */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* MINUS: reducere P44 binarium-stella */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* STAR: reducere P44 binarium-stella */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* SOLIDUS: reducere P44 binarium-stella */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* PERCENTUM: reducere P44 binarium-stella */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* AMPERSAND: reducere P44 binarium-stella */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* BARRA: reducere P44 binarium-stella */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* CARET: reducere P44 binarium-stella */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* ET_ET: reducere P44 binarium-stella */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* VEL_VEL: reducere P44 binarium-stella */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* AEQUALIS_AEQUALIS: reducere P44 binarium-stella */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* NON_AEQUALIS: reducere P44 binarium-stella */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* MINOR: reducere P44 binarium-stella */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* MAIOR: reducere P44 binarium-stella */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* MINOR_AEQUALIS: reducere P44 binarium-stella */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* MAIOR_AEQUALIS: reducere P44 binarium-stella */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* SINISTRORSUM: reducere P44 binarium-stella */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* DEXTRORSUM: reducere P44 binarium-stella */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* QUAESTIO: reducere P44 binarium-stella */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* COLON: reducere P44 binarium-stella */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* COMMA: reducere P44 binarium-stella */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* SEMICOLON: reducere P44 binarium-stella */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* PAREN_CLAUSA: reducere P44 binarium-stella */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* QUADRA_CLAUSA: reducere P44 binarium-stella */
    /* --- status 134 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* PLUS: reducere P45 binarium-solidus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* MINUS: reducere P45 binarium-solidus */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* STAR: reducere P45 binarium-solidus */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* SOLIDUS: reducere P45 binarium-solidus */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* PERCENTUM: reducere P45 binarium-solidus */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* AMPERSAND: reducere P45 binarium-solidus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* BARRA: reducere P45 binarium-solidus */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* CARET: reducere P45 binarium-solidus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* ET_ET: reducere P45 binarium-solidus */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* VEL_VEL: reducere P45 binarium-solidus */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* AEQUALIS_AEQUALIS: reducere P45 binarium-solidus */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* NON_AEQUALIS: reducere P45 binarium-solidus */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* MINOR: reducere P45 binarium-solidus */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* MAIOR: reducere P45 binarium-solidus */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* MINOR_AEQUALIS: reducere P45 binarium-solidus */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* MAIOR_AEQUALIS: reducere P45 binarium-solidus */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* SINISTRORSUM: reducere P45 binarium-solidus */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* DEXTRORSUM: reducere P45 binarium-solidus */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* QUAESTIO: reducere P45 binarium-solidus */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* COLON: reducere P45 binarium-solidus */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* COMMA: reducere P45 binarium-solidus */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* SEMICOLON: reducere P45 binarium-solidus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* PAREN_CLAUSA: reducere P45 binarium-solidus */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* QUADRA_CLAUSA: reducere P45 binarium-solidus */
    /* --- status 135 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* PLUS: reducere P46 binarium-percentum */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* MINUS: reducere P46 binarium-percentum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* STAR: reducere P46 binarium-percentum */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* SOLIDUS: reducere P46 binarium-percentum */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* PERCENTUM: reducere P46 binarium-percentum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* AMPERSAND: reducere P46 binarium-percentum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* BARRA: reducere P46 binarium-percentum */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* CARET: reducere P46 binarium-percentum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* ET_ET: reducere P46 binarium-percentum */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* VEL_VEL: reducere P46 binarium-percentum */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* AEQUALIS_AEQUALIS: reducere P46 binarium-percentum */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* NON_AEQUALIS: reducere P46 binarium-percentum */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* MINOR: reducere P46 binarium-percentum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* MAIOR: reducere P46 binarium-percentum */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* MINOR_AEQUALIS: reducere P46 binarium-percentum */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* MAIOR_AEQUALIS: reducere P46 binarium-percentum */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* SINISTRORSUM: reducere P46 binarium-percentum */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* DEXTRORSUM: reducere P46 binarium-percentum */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* QUAESTIO: reducere P46 binarium-percentum */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* COLON: reducere P46 binarium-percentum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* COMMA: reducere P46 binarium-percentum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* SEMICOLON: reducere P46 binarium-percentum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* PAREN_CLAUSA: reducere P46 binarium-percentum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* QUADRA_CLAUSA: reducere P46 binarium-percentum */
    /* --- status 136 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* PLUS: reducere P97 primarium-parenthesis */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* MINUS: reducere P97 primarium-parenthesis */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* STAR: reducere P97 primarium-parenthesis */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* SOLIDUS: reducere P97 primarium-parenthesis */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* PERCENTUM: reducere P97 primarium-parenthesis */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* AMPERSAND: reducere P97 primarium-parenthesis */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* BARRA: reducere P97 primarium-parenthesis */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* CARET: reducere P97 primarium-parenthesis */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* ET_ET: reducere P97 primarium-parenthesis */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* VEL_VEL: reducere P97 primarium-parenthesis */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* AEQUALIS_AEQUALIS: reducere P97 primarium-parenthesis */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* NON_AEQUALIS: reducere P97 primarium-parenthesis */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* MINOR: reducere P97 primarium-parenthesis */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* MAIOR: reducere P97 primarium-parenthesis */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* MINOR_AEQUALIS: reducere P97 primarium-parenthesis */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* MAIOR_AEQUALIS: reducere P97 primarium-parenthesis */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* SINISTRORSUM: reducere P97 primarium-parenthesis */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* DEXTRORSUM: reducere P97 primarium-parenthesis */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* PLUS_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* MINUS_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* STAR_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* BARRA_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* CARET_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P97 primarium-parenthesis */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* INCREMENTUM: reducere P97 primarium-parenthesis */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* DECREMENTUM: reducere P97 primarium-parenthesis */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* PUNCTUM: reducere P97 primarium-parenthesis */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* SAGITTA: reducere P97 primarium-parenthesis */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* QUAESTIO: reducere P97 primarium-parenthesis */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* COLON: reducere P97 primarium-parenthesis */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* COMMA: reducere P97 primarium-parenthesis */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* SEMICOLON: reducere P97 primarium-parenthesis */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* PAREN_APERTA: reducere P97 primarium-parenthesis */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* PAREN_CLAUSA: reducere P97 primarium-parenthesis */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* QUADRA_APERTA: reducere P97 primarium-parenthesis */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* QUADRA_CLAUSA: reducere P97 primarium-parenthesis */
    /* --- status 137 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 138 --- */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  61, 0 },  /* PAREN_CLAUSA: reducere P61 species-typi-plena */
    /* --- status 139 --- */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE, 139, 0 },  /* STAR: transponere -> 139 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  77, 0 },  /* PAREN_CLAUSA: reducere P77 declarator-abstractus-stella */
    /* --- status 140 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* VOID: reducere P76 verba-primitiva-appendere */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* CHAR: reducere P76 verba-primitiva-appendere */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* SHORT: reducere P76 verba-primitiva-appendere */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* INT: reducere P76 verba-primitiva-appendere */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* LONG: reducere P76 verba-primitiva-appendere */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* FLOAT_KW: reducere P76 verba-primitiva-appendere */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* DOUBLE: reducere P76 verba-primitiva-appendere */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* SIGNED: reducere P76 verba-primitiva-appendere */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* UNSIGNED: reducere P76 verba-primitiva-appendere */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* STAR: reducere P76 verba-primitiva-appendere */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* PAREN_CLAUSA: reducere P76 verba-primitiva-appendere */
    /* --- status 141 --- */
    {  55, SILVA_TAB_ACTIO_TRANSPONERE, 151, 0 },  /* PAREN_CLAUSA: transponere -> 151 */
    /* --- status 142 --- */
    {  52, SILVA_TAB_ACTIO_TRANSPONERE,  39, 0 },  /* COMMA: transponere -> 39 */
    {  57, SILVA_TAB_ACTIO_TRANSPONERE, 152, 0 },  /* QUADRA_CLAUSA: transponere -> 152 */
    /* --- status 143 --- */
    {  55, SILVA_TAB_ACTIO_TRANSPONERE, 153, 0 },  /* PAREN_CLAUSA: transponere -> 153 */
    /* --- status 144 --- */
    {  52, SILVA_TAB_ACTIO_TRANSPONERE, 154, 0 },  /* COMMA: transponere -> 154 */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  87, 0 },  /* PAREN_CLAUSA: reducere P87 */
    /* --- status 145 --- */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  88, 0 },  /* COMMA: reducere P88 argumenta-primum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  88, 0 },  /* PAREN_CLAUSA: reducere P88 argumenta-primum */
    /* --- status 146 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* PLUS: reducere P81 accessus-punctum */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* MINUS: reducere P81 accessus-punctum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* STAR: reducere P81 accessus-punctum */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* SOLIDUS: reducere P81 accessus-punctum */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* PERCENTUM: reducere P81 accessus-punctum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* AMPERSAND: reducere P81 accessus-punctum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* BARRA: reducere P81 accessus-punctum */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* CARET: reducere P81 accessus-punctum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* ET_ET: reducere P81 accessus-punctum */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* VEL_VEL: reducere P81 accessus-punctum */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* AEQUALIS_AEQUALIS: reducere P81 accessus-punctum */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* NON_AEQUALIS: reducere P81 accessus-punctum */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* MINOR: reducere P81 accessus-punctum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* MAIOR: reducere P81 accessus-punctum */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* MINOR_AEQUALIS: reducere P81 accessus-punctum */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* MAIOR_AEQUALIS: reducere P81 accessus-punctum */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* SINISTRORSUM: reducere P81 accessus-punctum */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* DEXTRORSUM: reducere P81 accessus-punctum */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* ASSIGNATIO: reducere P81 accessus-punctum */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* PLUS_ASSIGNATIO: reducere P81 accessus-punctum */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* MINUS_ASSIGNATIO: reducere P81 accessus-punctum */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* STAR_ASSIGNATIO: reducere P81 accessus-punctum */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P81 accessus-punctum */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P81 accessus-punctum */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P81 accessus-punctum */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* BARRA_ASSIGNATIO: reducere P81 accessus-punctum */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* CARET_ASSIGNATIO: reducere P81 accessus-punctum */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P81 accessus-punctum */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P81 accessus-punctum */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* INCREMENTUM: reducere P81 accessus-punctum */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* DECREMENTUM: reducere P81 accessus-punctum */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* PUNCTUM: reducere P81 accessus-punctum */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* SAGITTA: reducere P81 accessus-punctum */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* QUAESTIO: reducere P81 accessus-punctum */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* COLON: reducere P81 accessus-punctum */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* COMMA: reducere P81 accessus-punctum */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* SEMICOLON: reducere P81 accessus-punctum */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* PAREN_APERTA: reducere P81 accessus-punctum */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* PAREN_CLAUSA: reducere P81 accessus-punctum */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* QUADRA_APERTA: reducere P81 accessus-punctum */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* QUADRA_CLAUSA: reducere P81 accessus-punctum */
    /* --- status 147 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* PLUS: reducere P82 accessus-sagitta */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* MINUS: reducere P82 accessus-sagitta */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* STAR: reducere P82 accessus-sagitta */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* SOLIDUS: reducere P82 accessus-sagitta */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* PERCENTUM: reducere P82 accessus-sagitta */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* AMPERSAND: reducere P82 accessus-sagitta */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* BARRA: reducere P82 accessus-sagitta */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* CARET: reducere P82 accessus-sagitta */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* ET_ET: reducere P82 accessus-sagitta */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* VEL_VEL: reducere P82 accessus-sagitta */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* AEQUALIS_AEQUALIS: reducere P82 accessus-sagitta */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* NON_AEQUALIS: reducere P82 accessus-sagitta */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* MINOR: reducere P82 accessus-sagitta */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* MAIOR: reducere P82 accessus-sagitta */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* MINOR_AEQUALIS: reducere P82 accessus-sagitta */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* MAIOR_AEQUALIS: reducere P82 accessus-sagitta */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* SINISTRORSUM: reducere P82 accessus-sagitta */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* DEXTRORSUM: reducere P82 accessus-sagitta */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* ASSIGNATIO: reducere P82 accessus-sagitta */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* PLUS_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* MINUS_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* STAR_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* BARRA_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* CARET_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P82 accessus-sagitta */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* INCREMENTUM: reducere P82 accessus-sagitta */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* DECREMENTUM: reducere P82 accessus-sagitta */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* PUNCTUM: reducere P82 accessus-sagitta */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* SAGITTA: reducere P82 accessus-sagitta */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* QUAESTIO: reducere P82 accessus-sagitta */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* COLON: reducere P82 accessus-sagitta */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* COMMA: reducere P82 accessus-sagitta */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* SEMICOLON: reducere P82 accessus-sagitta */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* PAREN_APERTA: reducere P82 accessus-sagitta */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* PAREN_CLAUSA: reducere P82 accessus-sagitta */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* QUADRA_APERTA: reducere P82 accessus-sagitta */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* QUADRA_CLAUSA: reducere P82 accessus-sagitta */
    /* --- status 148 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 149 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* PLUS: reducere P48 conversio-typus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* MINUS: reducere P48 conversio-typus */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* STAR: reducere P48 conversio-typus */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SOLIDUS: reducere P48 conversio-typus */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* PERCENTUM: reducere P48 conversio-typus */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* AMPERSAND: reducere P48 conversio-typus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* BARRA: reducere P48 conversio-typus */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CARET: reducere P48 conversio-typus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* ET_ET: reducere P48 conversio-typus */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* VEL_VEL: reducere P48 conversio-typus */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* AEQUALIS_AEQUALIS: reducere P48 conversio-typus */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* NON_AEQUALIS: reducere P48 conversio-typus */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* MINOR: reducere P48 conversio-typus */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* MAIOR: reducere P48 conversio-typus */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* MINOR_AEQUALIS: reducere P48 conversio-typus */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* MAIOR_AEQUALIS: reducere P48 conversio-typus */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SINISTRORSUM: reducere P48 conversio-typus */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* DEXTRORSUM: reducere P48 conversio-typus */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* ASSIGNATIO: reducere P48 conversio-typus */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* PLUS_ASSIGNATIO: reducere P48 conversio-typus */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* MINUS_ASSIGNATIO: reducere P48 conversio-typus */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* STAR_ASSIGNATIO: reducere P48 conversio-typus */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P48 conversio-typus */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P48 conversio-typus */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P48 conversio-typus */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* BARRA_ASSIGNATIO: reducere P48 conversio-typus */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CARET_ASSIGNATIO: reducere P48 conversio-typus */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P48 conversio-typus */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P48 conversio-typus */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* QUAESTIO: reducere P48 conversio-typus */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* COLON: reducere P48 conversio-typus */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* COMMA: reducere P48 conversio-typus */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SEMICOLON: reducere P48 conversio-typus */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* PAREN_CLAUSA: reducere P48 conversio-typus */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* QUADRA_CLAUSA: reducere P48 conversio-typus */
    /* --- status 150 --- */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  78, 0 },  /* PAREN_CLAUSA: reducere P78 declarator-abstractus-catena */
    /* --- status 151 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* PLUS: reducere P59 magnitudo-typi */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* MINUS: reducere P59 magnitudo-typi */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* STAR: reducere P59 magnitudo-typi */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* SOLIDUS: reducere P59 magnitudo-typi */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* PERCENTUM: reducere P59 magnitudo-typi */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* AMPERSAND: reducere P59 magnitudo-typi */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* BARRA: reducere P59 magnitudo-typi */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* CARET: reducere P59 magnitudo-typi */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* ET_ET: reducere P59 magnitudo-typi */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* VEL_VEL: reducere P59 magnitudo-typi */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* AEQUALIS_AEQUALIS: reducere P59 magnitudo-typi */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* NON_AEQUALIS: reducere P59 magnitudo-typi */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* MINOR: reducere P59 magnitudo-typi */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* MAIOR: reducere P59 magnitudo-typi */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* MINOR_AEQUALIS: reducere P59 magnitudo-typi */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* MAIOR_AEQUALIS: reducere P59 magnitudo-typi */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* SINISTRORSUM: reducere P59 magnitudo-typi */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* DEXTRORSUM: reducere P59 magnitudo-typi */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* ASSIGNATIO: reducere P59 magnitudo-typi */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* PLUS_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* MINUS_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* STAR_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* BARRA_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* CARET_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P59 magnitudo-typi */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* QUAESTIO: reducere P59 magnitudo-typi */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* COLON: reducere P59 magnitudo-typi */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* COMMA: reducere P59 magnitudo-typi */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* SEMICOLON: reducere P59 magnitudo-typi */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* PAREN_CLAUSA: reducere P59 magnitudo-typi */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* QUADRA_CLAUSA: reducere P59 magnitudo-typi */
    /* --- status 152 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* PLUS: reducere P79 subscriptio-index */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* MINUS: reducere P79 subscriptio-index */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* STAR: reducere P79 subscriptio-index */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* SOLIDUS: reducere P79 subscriptio-index */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* PERCENTUM: reducere P79 subscriptio-index */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* AMPERSAND: reducere P79 subscriptio-index */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* BARRA: reducere P79 subscriptio-index */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* CARET: reducere P79 subscriptio-index */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* ET_ET: reducere P79 subscriptio-index */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* VEL_VEL: reducere P79 subscriptio-index */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* AEQUALIS_AEQUALIS: reducere P79 subscriptio-index */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* NON_AEQUALIS: reducere P79 subscriptio-index */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* MINOR: reducere P79 subscriptio-index */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* MAIOR: reducere P79 subscriptio-index */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* MINOR_AEQUALIS: reducere P79 subscriptio-index */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* MAIOR_AEQUALIS: reducere P79 subscriptio-index */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* SINISTRORSUM: reducere P79 subscriptio-index */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* DEXTRORSUM: reducere P79 subscriptio-index */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* ASSIGNATIO: reducere P79 subscriptio-index */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* PLUS_ASSIGNATIO: reducere P79 subscriptio-index */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* MINUS_ASSIGNATIO: reducere P79 subscriptio-index */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* STAR_ASSIGNATIO: reducere P79 subscriptio-index */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P79 subscriptio-index */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P79 subscriptio-index */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P79 subscriptio-index */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* BARRA_ASSIGNATIO: reducere P79 subscriptio-index */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* CARET_ASSIGNATIO: reducere P79 subscriptio-index */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P79 subscriptio-index */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P79 subscriptio-index */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* INCREMENTUM: reducere P79 subscriptio-index */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* DECREMENTUM: reducere P79 subscriptio-index */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* PUNCTUM: reducere P79 subscriptio-index */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* SAGITTA: reducere P79 subscriptio-index */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* QUAESTIO: reducere P79 subscriptio-index */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* COLON: reducere P79 subscriptio-index */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* COMMA: reducere P79 subscriptio-index */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* SEMICOLON: reducere P79 subscriptio-index */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* PAREN_APERTA: reducere P79 subscriptio-index */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* PAREN_CLAUSA: reducere P79 subscriptio-index */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* QUADRA_APERTA: reducere P79 subscriptio-index */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* QUADRA_CLAUSA: reducere P79 subscriptio-index */
    /* --- status 153 --- */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* PLUS: reducere P80 vocatio-argumenta */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* MINUS: reducere P80 vocatio-argumenta */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* STAR: reducere P80 vocatio-argumenta */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* SOLIDUS: reducere P80 vocatio-argumenta */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* PERCENTUM: reducere P80 vocatio-argumenta */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* AMPERSAND: reducere P80 vocatio-argumenta */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* BARRA: reducere P80 vocatio-argumenta */
    {  22, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* CARET: reducere P80 vocatio-argumenta */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* ET_ET: reducere P80 vocatio-argumenta */
    {  26, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* VEL_VEL: reducere P80 vocatio-argumenta */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* AEQUALIS_AEQUALIS: reducere P80 vocatio-argumenta */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* NON_AEQUALIS: reducere P80 vocatio-argumenta */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* MINOR: reducere P80 vocatio-argumenta */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* MAIOR: reducere P80 vocatio-argumenta */
    {  31, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* MINOR_AEQUALIS: reducere P80 vocatio-argumenta */
    {  32, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* MAIOR_AEQUALIS: reducere P80 vocatio-argumenta */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* SINISTRORSUM: reducere P80 vocatio-argumenta */
    {  34, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* DEXTRORSUM: reducere P80 vocatio-argumenta */
    {  35, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* PLUS_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* MINUS_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  38, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* STAR_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  40, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  41, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  42, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* BARRA_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  43, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* CARET_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  44, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  45, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P80 vocatio-argumenta */
    {  46, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* INCREMENTUM: reducere P80 vocatio-argumenta */
    {  47, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* DECREMENTUM: reducere P80 vocatio-argumenta */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* PUNCTUM: reducere P80 vocatio-argumenta */
    {  49, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* SAGITTA: reducere P80 vocatio-argumenta */
    {  50, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* QUAESTIO: reducere P80 vocatio-argumenta */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* COLON: reducere P80 vocatio-argumenta */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* COMMA: reducere P80 vocatio-argumenta */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* SEMICOLON: reducere P80 vocatio-argumenta */
    {  54, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* PAREN_APERTA: reducere P80 vocatio-argumenta */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* PAREN_CLAUSA: reducere P80 vocatio-argumenta */
    {  56, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* QUADRA_APERTA: reducere P80 vocatio-argumenta */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* QUADRA_CLAUSA: reducere P80 vocatio-argumenta */
    /* --- status 154 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  33, 0 },  /* IDENTIFICATOR: transponere -> 33 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  34, 0 },  /* INTEGER: transponere -> 34 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  35, 0 },  /* FLOAT: transponere -> 35 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  36, 0 },  /* CHARACTER_LIT: transponere -> 36 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  31, 0 },  /* STRING_LIT: transponere -> 31 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  28, 0 },  /* SIZEOF: transponere -> 28 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  24, 0 },  /* PLUS: transponere -> 24 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* MINUS: transponere -> 25 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* STAR: transponere -> 23 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* AMPERSAND: transponere -> 22 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* TILDE: transponere -> 26 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* EXCLAMATIO: transponere -> 27 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* INCREMENTUM: transponere -> 20 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* DECREMENTUM: transponere -> 21 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PAREN_APERTA: transponere -> 19 */
    /* --- status 155 --- */
    {  51, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* COLON: reducere P18 ternarius-conditio */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* COMMA: reducere P18 ternarius-conditio */
    {  53, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* SEMICOLON: reducere P18 ternarius-conditio */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* PAREN_CLAUSA: reducere P18 ternarius-conditio */
    {  57, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* QUADRA_CLAUSA: reducere P18 ternarius-conditio */
    /* --- status 156 --- */
    {  52, SILVA_TAB_ACTIO_REDUCERE,  89, 0 },  /* COMMA: reducere P89 argumenta-appendere */
    {  55, SILVA_TAB_ACTIO_REDUCERE,  89, 0 },  /* PAREN_CLAUSA: reducere P89 argumenta-appendere */
};

/* ==================================================
 * Goto
 * ================================================== */

hic_manens constans SilvaTabGoto SILVA_C89_GOTO[] = {
    /* --- status 0 --- */
    {  59,   1 },  /* elementa -> 1 */
    /* --- status 1 --- */
    {  60,   2 },  /* elementum -> 2 */
    {  61,   3 },  /* sententia-expressionis -> 3 */
    {  62,   4 },  /* expressio -> 4 */
    {  63,   5 },  /* assignatio -> 5 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 19 --- */
    {  62,  70 },  /* expressio -> 70 */
    {  63,   5 },  /* assignatio -> 5 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  77,  71 },  /* species-typi -> 71 */
    {  78,  72 },  /* specificatores -> 72 */
    {  79,  73 },  /* typus -> 73 */
    {  80,  85 },  /* verbum-primitivum -> 85 */
    {  81,  74 },  /* verba-primitiva -> 74 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 20 --- */
    {  76,  86 },  /* unarium -> 86 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 21 --- */
    {  76,  88 },  /* unarium -> 88 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 22 --- */
    {  75,  90 },  /* conversio -> 90 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 23 --- */
    {  75,  91 },  /* conversio -> 91 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 24 --- */
    {  75,  92 },  /* conversio -> 92 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 25 --- */
    {  75,  93 },  /* conversio -> 93 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 26 --- */
    {  75,  94 },  /* conversio -> 94 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 27 --- */
    {  75,  95 },  /* conversio -> 95 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 28 --- */
    {  76,  96 },  /* unarium -> 96 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 39 --- */
    {  63, 105 },  /* assignatio -> 105 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 40 --- */
    {  63, 106 },  /* assignatio -> 106 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 41 --- */
    {  63, 107 },  /* assignatio -> 107 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 42 --- */
    {  63, 108 },  /* assignatio -> 108 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 43 --- */
    {  63, 109 },  /* assignatio -> 109 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 44 --- */
    {  63, 110 },  /* assignatio -> 110 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 45 --- */
    {  63, 111 },  /* assignatio -> 111 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 46 --- */
    {  63, 112 },  /* assignatio -> 112 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 47 --- */
    {  63, 113 },  /* assignatio -> 113 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 48 --- */
    {  63, 114 },  /* assignatio -> 114 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 49 --- */
    {  63, 115 },  /* assignatio -> 115 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 50 --- */
    {  63, 116 },  /* assignatio -> 116 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 51 --- */
    {  62, 117 },  /* expressio -> 117 */
    {  63,   5 },  /* assignatio -> 5 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 52 --- */
    {  66, 118 },  /* coniunctio -> 118 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 53 --- */
    {  67, 119 },  /* vel-bitorum -> 119 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 54 --- */
    {  68, 120 },  /* aut-bitorum -> 120 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 55 --- */
    {  69, 121 },  /* et-bitorum -> 121 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 56 --- */
    {  70, 122 },  /* aequalitas -> 122 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 57 --- */
    {  71, 123 },  /* comparatio -> 123 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 58 --- */
    {  71, 124 },  /* comparatio -> 124 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 59 --- */
    {  72, 125 },  /* translatio -> 125 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 60 --- */
    {  72, 126 },  /* translatio -> 126 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 61 --- */
    {  72, 127 },  /* translatio -> 127 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 62 --- */
    {  72, 128 },  /* translatio -> 128 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 63 --- */
    {  73, 129 },  /* additivum -> 129 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 64 --- */
    {  73, 130 },  /* additivum -> 130 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 65 --- */
    {  74, 131 },  /* multiplicativum -> 131 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 66 --- */
    {  74, 132 },  /* multiplicativum -> 132 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 67 --- */
    {  75, 133 },  /* conversio -> 133 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 68 --- */
    {  75, 134 },  /* conversio -> 134 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 69 --- */
    {  75, 135 },  /* conversio -> 135 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 72 --- */
    {  82, 138 },  /* declarator-abstractus -> 138 */
    /* --- status 74 --- */
    {  80, 140 },  /* verbum-primitivum -> 140 */
    /* --- status 87 --- */
    {  62,  70 },  /* expressio -> 70 */
    {  63,   5 },  /* assignatio -> 5 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 97 --- */
    {  62,  70 },  /* expressio -> 70 */
    {  63,   5 },  /* assignatio -> 5 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  77, 141 },  /* species-typi -> 141 */
    {  78,  72 },  /* specificatores -> 72 */
    {  79,  73 },  /* typus -> 73 */
    {  80,  85 },  /* verbum-primitivum -> 85 */
    {  81,  74 },  /* verba-primitiva -> 74 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 98 --- */
    {  62, 142 },  /* expressio -> 142 */
    {  63,   5 },  /* assignatio -> 5 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 99 --- */
    {  63, 145 },  /* assignatio -> 145 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  84, 143 },  /* argumenta -> 143 */
    {  85, 144 },  /* argumenta-plena -> 144 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 137 --- */
    {  75, 149 },  /* conversio -> 149 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 139 --- */
    {  82, 150 },  /* declarator-abstractus -> 150 */
    /* --- status 148 --- */
    {  64, 155 },  /* ternarius -> 155 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,  89 },  /* unarium -> 89 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    /* --- status 154 --- */
    {  63, 156 },  /* assignatio -> 156 */
    {  64,   7 },  /* ternarius -> 7 */
    {  65,   8 },  /* disiunctio -> 8 */
    {  66,   9 },  /* coniunctio -> 9 */
    {  67,  10 },  /* vel-bitorum -> 10 */
    {  68,  11 },  /* aut-bitorum -> 11 */
    {  69,  12 },  /* et-bitorum -> 12 */
    {  70,  13 },  /* aequalitas -> 13 */
    {  71,  14 },  /* comparatio -> 14 */
    {  72,  15 },  /* translatio -> 15 */
    {  73,  16 },  /* additivum -> 16 */
    {  74,  17 },  /* multiplicativum -> 17 */
    {  75,  18 },  /* conversio -> 18 */
    {  76,   6 },  /* unarium -> 6 */
    {  83,  29 },  /* postfixum -> 29 */
    {  86,  32 },  /* chordae -> 32 */
    {  87,  30 },  /* primarium -> 30 */
    { -1, -1 }   /* terminator */
};

/* ==================================================
 * Status (offset + numerus in series planas)
 * ================================================== */

hic_manens constans SilvaTabStatus SILVA_C89_STATUS[157] = {
    /* status  0 */ {    0, 16,    0,  1 },
    /* status  1 */ {   16, 16,    1, 20 },
    /* status  2 */ {   32, 16,   21,  0 },
    /* status  3 */ {   48, 16,   21,  0 },
    /* status  4 */ {   64,  2,   21,  0 },
    /* status  5 */ {   66,  5,   21,  0 },
    /* status  6 */ {   71, 35,   21,  0 },
    /* status  7 */ {  106,  5,   21,  0 },
    /* status  8 */ {  111,  7,   21,  0 },
    /* status  9 */ {  118,  8,   21,  0 },
    /* status 10 */ {  126,  9,   21,  0 },
    /* status 11 */ {  135, 10,   21,  0 },
    /* status 12 */ {  145, 11,   21,  0 },
    /* status 13 */ {  156, 13,   21,  0 },
    /* status 14 */ {  169, 17,   21,  0 },
    /* status 15 */ {  186, 19,   21,  0 },
    /* status 16 */ {  205, 21,   21,  0 },
    /* status 17 */ {  226, 24,   21,  0 },
    /* status 18 */ {  250, 24,   21,  0 },
    /* status 19 */ {  274, 24,   21, 23 },
    /* status 20 */ {  298, 15,   44,  4 },
    /* status 21 */ {  313, 15,   48,  4 },
    /* status 22 */ {  328, 15,   52,  5 },
    /* status 23 */ {  343, 15,   57,  5 },
    /* status 24 */ {  358, 15,   62,  5 },
    /* status 25 */ {  373, 15,   67,  5 },
    /* status 26 */ {  388, 15,   72,  5 },
    /* status 27 */ {  403, 15,   77,  5 },
    /* status 28 */ {  418, 15,   82,  4 },
    /* status 29 */ {  433, 41,   86,  0 },
    /* status 30 */ {  474, 41,   86,  0 },
    /* status 31 */ {  515, 42,   86,  0 },
    /* status 32 */ {  557, 42,   86,  0 },
    /* status 33 */ {  599, 41,   86,  0 },
    /* status 34 */ {  640, 41,   86,  0 },
    /* status 35 */ {  681, 41,   86,  0 },
    /* status 36 */ {  722, 41,   86,  0 },
    /* status 37 */ {  763,  1,   86,  0 },
    /* status 38 */ {  764, 16,   86,  0 },
    /* status 39 */ {  780, 15,   86, 17 },
    /* status 40 */ {  795, 15,  103, 17 },
    /* status 41 */ {  810, 15,  120, 17 },
    /* status 42 */ {  825, 15,  137, 17 },
    /* status 43 */ {  840, 15,  154, 17 },
    /* status 44 */ {  855, 15,  171, 17 },
    /* status 45 */ {  870, 15,  188, 17 },
    /* status 46 */ {  885, 15,  205, 17 },
    /* status 47 */ {  900, 15,  222, 17 },
    /* status 48 */ {  915, 15,  239, 17 },
    /* status 49 */ {  930, 15,  256, 17 },
    /* status 50 */ {  945, 15,  273, 17 },
    /* status 51 */ {  960, 15,  290, 18 },
    /* status 52 */ {  975, 15,  308, 14 },
    /* status 53 */ {  990, 15,  322, 13 },
    /* status 54 */ { 1005, 15,  335, 12 },
    /* status 55 */ { 1020, 15,  347, 11 },
    /* status 56 */ { 1035, 15,  358, 10 },
    /* status 57 */ { 1050, 15,  368,  9 },
    /* status 58 */ { 1065, 15,  377,  9 },
    /* status 59 */ { 1080, 15,  386,  8 },
    /* status 60 */ { 1095, 15,  394,  8 },
    /* status 61 */ { 1110, 15,  402,  8 },
    /* status 62 */ { 1125, 15,  410,  8 },
    /* status 63 */ { 1140, 15,  418,  7 },
    /* status 64 */ { 1155, 15,  425,  7 },
    /* status 65 */ { 1170, 15,  432,  6 },
    /* status 66 */ { 1185, 15,  438,  6 },
    /* status 67 */ { 1200, 15,  444,  5 },
    /* status 68 */ { 1215, 15,  449,  5 },
    /* status 69 */ { 1230, 15,  454,  5 },
    /* status 70 */ { 1245,  2,  459,  0 },
    /* status 71 */ { 1247,  1,  459,  0 },
    /* status 72 */ { 1248,  2,  459,  1 },
    /* status 73 */ { 1250,  2,  460,  0 },
    /* status 74 */ { 1252, 11,  460,  1 },
    /* status 75 */ { 1263, 40,  461,  0 },
    /* status 76 */ { 1303, 11,  461,  0 },
    /* status 77 */ { 1314, 11,  461,  0 },
    /* status 78 */ { 1325, 11,  461,  0 },
    /* status 79 */ { 1336, 11,  461,  0 },
    /* status 80 */ { 1347, 11,  461,  0 },
    /* status 81 */ { 1358, 11,  461,  0 },
    /* status 82 */ { 1369, 11,  461,  0 },
    /* status 83 */ { 1380, 11,  461,  0 },
    /* status 84 */ { 1391, 11,  461,  0 },
    /* status 85 */ { 1402, 11,  461,  0 },
    /* status 86 */ { 1413, 35,  461,  0 },
    /* status 87 */ { 1448, 15,  461, 18 },
    /* status 88 */ { 1463, 35,  479,  0 },
    /* status 89 */ { 1498, 35,  479,  0 },
    /* status 90 */ { 1533, 35,  479,  0 },
    /* status 91 */ { 1568, 35,  479,  0 },
    /* status 92 */ { 1603, 35,  479,  0 },
    /* status 93 */ { 1638, 35,  479,  0 },
    /* status 94 */ { 1673, 35,  479,  0 },
    /* status 95 */ { 1708, 35,  479,  0 },
    /* status 96 */ { 1743, 35,  479,  0 },
    /* status 97 */ { 1778, 24,  479, 23 },
    /* status 98 */ { 1802, 15,  502, 18 },
    /* status 99 */ { 1817, 16,  520, 19 },
    /* status 100 */ { 1833,  1,  539,  0 },
    /* status 101 */ { 1834,  1,  539,  0 },
    /* status 102 */ { 1835, 41,  539,  0 },
    /* status 103 */ { 1876, 41,  539,  0 },
    /* status 104 */ { 1917, 42,  539,  0 },
    /* status 105 */ { 1959,  5,  539,  0 },
    /* status 106 */ { 1964,  5,  539,  0 },
    /* status 107 */ { 1969,  5,  539,  0 },
    /* status 108 */ { 1974,  5,  539,  0 },
    /* status 109 */ { 1979,  5,  539,  0 },
    /* status 110 */ { 1984,  5,  539,  0 },
    /* status 111 */ { 1989,  5,  539,  0 },
    /* status 112 */ { 1994,  5,  539,  0 },
    /* status 113 */ { 1999,  5,  539,  0 },
    /* status 114 */ { 2004,  5,  539,  0 },
    /* status 115 */ { 2009,  5,  539,  0 },
    /* status 116 */ { 2014,  5,  539,  0 },
    /* status 117 */ { 2019,  2,  539,  0 },
    /* status 118 */ { 2021,  8,  539,  0 },
    /* status 119 */ { 2029,  9,  539,  0 },
    /* status 120 */ { 2038, 10,  539,  0 },
    /* status 121 */ { 2048, 11,  539,  0 },
    /* status 122 */ { 2059, 13,  539,  0 },
    /* status 123 */ { 2072, 17,  539,  0 },
    /* status 124 */ { 2089, 17,  539,  0 },
    /* status 125 */ { 2106, 19,  539,  0 },
    /* status 126 */ { 2125, 19,  539,  0 },
    /* status 127 */ { 2144, 19,  539,  0 },
    /* status 128 */ { 2163, 19,  539,  0 },
    /* status 129 */ { 2182, 21,  539,  0 },
    /* status 130 */ { 2203, 21,  539,  0 },
    /* status 131 */ { 2224, 24,  539,  0 },
    /* status 132 */ { 2248, 24,  539,  0 },
    /* status 133 */ { 2272, 24,  539,  0 },
    /* status 134 */ { 2296, 24,  539,  0 },
    /* status 135 */ { 2320, 24,  539,  0 },
    /* status 136 */ { 2344, 41,  539,  0 },
    /* status 137 */ { 2385, 15,  539,  5 },
    /* status 138 */ { 2400,  1,  544,  0 },
    /* status 139 */ { 2401,  2,  544,  1 },
    /* status 140 */ { 2403, 11,  545,  0 },
    /* status 141 */ { 2414,  1,  545,  0 },
    /* status 142 */ { 2415,  2,  545,  0 },
    /* status 143 */ { 2417,  1,  545,  0 },
    /* status 144 */ { 2418,  2,  545,  0 },
    /* status 145 */ { 2420,  2,  545,  0 },
    /* status 146 */ { 2422, 41,  545,  0 },
    /* status 147 */ { 2463, 41,  545,  0 },
    /* status 148 */ { 2504, 15,  545, 16 },
    /* status 149 */ { 2519, 35,  561,  0 },
    /* status 150 */ { 2554,  1,  561,  0 },
    /* status 151 */ { 2555, 35,  561,  0 },
    /* status 152 */ { 2590, 41,  561,  0 },
    /* status 153 */ { 2631, 41,  561,  0 },
    /* status 154 */ { 2672, 15,  561, 17 },
    /* status 155 */ { 2687,  5,  578,  0 },
    /* status 156 */ { 2692,  2,  578,  0 }
};

/* ==================================================
 * Tabula cocta integra
 * ================================================== */

constans SilvaTabulaCocta SILVA_C89_TABULA = {
    SILVA_C89_SYMBOLA, 89,
    59,  /* terminalia */
    SILVA_C89_PRODUCTIONES, 99,
    SILVA_C89_STATUS, 157,
    SILVA_C89_ACTIONES, 2694,
    SILVA_C89_GOTO, 578,
    88,  /* initium (augmentatum) */
    98,  /* productio augmentata */
    2   /* conflictus servati */
};

/* ==================================================
 * Registrum generum (layouts locorum)
 * ================================================== */

hic_manens constans SilvaTabLocus SILVA_C89_LOCI[] = {
    /* sententia-expressionis */
    { "expressio", SILVA_LOCUS_NODUS },
    { "tok_terminator", SILVA_LOCUS_TOKEN },
    /* virgula */
    { "sinister", SILVA_LOCUS_NODUS },
    { "tok_operator", SILVA_LOCUS_TOKEN },
    { "dexter", SILVA_LOCUS_NODUS },
    /* assignatio */
    { "sinister", SILVA_LOCUS_NODUS },
    { "tok_operator", SILVA_LOCUS_TOKEN },
    { "dexter", SILVA_LOCUS_NODUS },
    /* ternarius */
    { "conditio", SILVA_LOCUS_NODUS },
    { "tok_quaestio", SILVA_LOCUS_TOKEN },
    { "verum", SILVA_LOCUS_NODUS },
    { "tok_colon", SILVA_LOCUS_TOKEN },
    { "falsum", SILVA_LOCUS_NODUS },
    /* binarium */
    { "sinister", SILVA_LOCUS_NODUS },
    { "tok_operator", SILVA_LOCUS_TOKEN },
    { "dexter", SILVA_LOCUS_NODUS },
    /* conversio */
    { "tok_apertum", SILVA_LOCUS_TOKEN },
    { "typus", SILVA_LOCUS_NODUS },
    { "tok_clausum", SILVA_LOCUS_TOKEN },
    { "internum", SILVA_LOCUS_NODUS },
    /* unarium */
    { "tok_operator", SILVA_LOCUS_TOKEN },
    { "internum", SILVA_LOCUS_NODUS },
    /* magnitudo-expressionis */
    { "tok_verbum", SILVA_LOCUS_TOKEN },
    { "internum", SILVA_LOCUS_NODUS },
    /* magnitudo-typi */
    { "tok_verbum", SILVA_LOCUS_TOKEN },
    { "tok_apertum", SILVA_LOCUS_TOKEN },
    { "typus", SILVA_LOCUS_NODUS },
    { "tok_clausum", SILVA_LOCUS_TOKEN },
    /* species-typi */
    { "specificatores", SILVA_LOCUS_LISTA_MIXTA },
    { "declarator", SILVA_LOCUS_NODUS },
    /* typus-primitivus */
    { "tok_verba", SILVA_LOCUS_LISTA_MIXTA },
    /* typus-nominatus */
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    /* declarator-abstractus */
    { "tok_stella", SILVA_LOCUS_TOKEN },
    { "internum", SILVA_LOCUS_NODUS },
    /* subscriptio */
    { "basis", SILVA_LOCUS_NODUS },
    { "tok_apertum", SILVA_LOCUS_TOKEN },
    { "index", SILVA_LOCUS_NODUS },
    { "tok_clausum", SILVA_LOCUS_TOKEN },
    /* vocatio */
    { "functio", SILVA_LOCUS_NODUS },
    { "tok_apertum", SILVA_LOCUS_TOKEN },
    { "argumenta", SILVA_LOCUS_LISTA_MIXTA },
    { "tok_clausum", SILVA_LOCUS_TOKEN },
    /* accessus */
    { "basis", SILVA_LOCUS_NODUS },
    { "tok_operator", SILVA_LOCUS_TOKEN },
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    /* postcrementum */
    { "internum", SILVA_LOCUS_NODUS },
    { "tok_operator", SILVA_LOCUS_TOKEN },
    /* folium-identificator */
    { "tok_valor", SILVA_LOCUS_TOKEN },
    /* folium-integer */
    { "tok_valor", SILVA_LOCUS_TOKEN },
    /* folium-fluitans */
    { "tok_valor", SILVA_LOCUS_TOKEN },
    /* folium-character */
    { "tok_valor", SILVA_LOCUS_TOKEN },
    /* folium-chorda */
    { "tok_valor", SILVA_LOCUS_LISTA_MIXTA },
    /* parenthesis */
    { "tok_apertum", SILVA_LOCUS_TOKEN },
    { "internum", SILVA_LOCUS_NODUS },
    { "tok_clausum", SILVA_LOCUS_TOKEN },
    /* error */
    { "tokens", SILVA_LOCUS_LISTA_TOKEN },
    /* ambiguus */
    { "interpretationes", SILVA_LOCUS_LISTA_NODUS },
    { "canonica", SILVA_LOCUS_INDEX },
    /* conditionalis */
    { "rami", SILVA_LOCUS_LISTA_NODUS },
    { "finis", SILVA_LOCUS_LISTA_TOKEN },
    /* ramus-sumptus */
    { "directiva", SILVA_LOCUS_LISTA_TOKEN },
    { "contentum", SILVA_LOCUS_LISTA_NODUS },
    { "conditio_id", SILVA_LOCUS_INDEX },
    /* ramus-omissus */
    { "directiva", SILVA_LOCUS_LISTA_TOKEN },
    { "cruda", SILVA_LOCUS_LISTA_TOKEN },
    { "conditio_id", SILVA_LOCUS_INDEX },
    { NIHIL, -1 }   /* terminator */
};

hic_manens constans SilvaTabGenus SILVA_C89_GENERA[] = {
    /* [ 0] */ { "sententia-expressionis", 0, 2 },
    /* [ 1] */ { "virgula", 2, 3 },
    /* [ 2] */ { "assignatio", 5, 3 },
    /* [ 3] */ { "ternarius", 8, 5 },
    /* [ 4] */ { "binarium", 13, 3 },
    /* [ 5] */ { "conversio", 16, 4 },
    /* [ 6] */ { "unarium", 20, 2 },
    /* [ 7] */ { "magnitudo-expressionis", 22, 2 },
    /* [ 8] */ { "magnitudo-typi", 24, 4 },
    /* [ 9] */ { "species-typi", 28, 2 },
    /* [10] */ { "typus-primitivus", 30, 1 },
    /* [11] */ { "typus-nominatus", 31, 1 },
    /* [12] */ { "declarator-abstractus", 32, 2 },
    /* [13] */ { "subscriptio", 34, 4 },
    /* [14] */ { "vocatio", 38, 4 },
    /* [15] */ { "accessus", 42, 3 },
    /* [16] */ { "postcrementum", 45, 2 },
    /* [17] */ { "folium-identificator", 47, 1 },
    /* [18] */ { "folium-integer", 48, 1 },
    /* [19] */ { "folium-fluitans", 49, 1 },
    /* [20] */ { "folium-character", 50, 1 },
    /* [21] */ { "folium-chorda", 51, 1 },
    /* [22] */ { "parenthesis", 52, 3 },
    /* [23] */ { "error", 55, 1 },
    /* [24] */ { "ambiguus", 56, 2 },
    /* [25] */ { "conditionalis", 58, 2 },
    /* [26] */ { "ramus-sumptus", 60, 3 },
    /* [27] */ { "ramus-omissus", 63, 3 },
    { NIHIL, 0, 0 }   /* terminator */
};

constans SilvaRegistrumCoctum SILVA_C89_REGISTRUM = {
    SILVA_C89_GENERA, 28,
    SILVA_C89_LOCI, 66
};

/* ==================================================
 * Accessores (probati: genus + fines)
 * ================================================== */

SilvaValor
silva_c89_sententia_expressionis_expressio (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_sententia_expressionis_tok_terminator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_virgula_sinister (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_VIRGULA
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_virgula_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_VIRGULA
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_virgula_dexter (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_VIRGULA
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_assignatio_sinister (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ASSIGNATIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_assignatio_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ASSIGNATIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_assignatio_dexter (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ASSIGNATIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_ternarius_conditio (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_TERNARIUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_ternarius_tok_quaestio (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_TERNARIUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_ternarius_verum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_TERNARIUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_ternarius_tok_colon (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_TERNARIUS
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_ternarius_falsum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_TERNARIUS
        || 4 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[4];
}

SilvaValor
silva_c89_binarium_sinister (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_BINARIUM
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_binarium_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_BINARIUM
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_binarium_dexter (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_BINARIUM
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_conversio_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CONVERSIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_conversio_typus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CONVERSIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_conversio_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CONVERSIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_conversio_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CONVERSIO
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_unarium_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_UNARIUM
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_unarium_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_UNARIUM
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_magnitudo_expressionis_tok_verbum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_magnitudo_expressionis_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_magnitudo_typi_tok_verbum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_magnitudo_typi_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_magnitudo_typi_typus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_magnitudo_typi_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_species_typi_specificatores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_SPECIES_TYPI
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_species_typi_declarator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_SPECIES_TYPI
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_typus_primitivus_tok_verba (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_TYPUS_PRIMITIVUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_typus_nominatus_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_TYPUS_NOMINATUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_declarator_abstractus_tok_stella (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_declarator_abstractus_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_subscriptio_basis (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_SUBSCRIPTIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_subscriptio_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_SUBSCRIPTIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_subscriptio_index (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_SUBSCRIPTIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_subscriptio_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_SUBSCRIPTIO
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_vocatio_functio (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_VOCATIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_vocatio_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_VOCATIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_vocatio_argumenta (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_VOCATIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_vocatio_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_VOCATIO
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_accessus_basis (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ACCESSUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_accessus_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ACCESSUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_accessus_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ACCESSUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_postcrementum_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_POSTCREMENTUM
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_postcrementum_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_POSTCREMENTUM
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_folium_identificator_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_folium_integer_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_FOLIUM_INTEGER
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_folium_fluitans_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_folium_character_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_folium_chorda_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_FOLIUM_CHORDA
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_parenthesis_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_PARENTHESIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_parenthesis_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_PARENTHESIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_parenthesis_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_PARENTHESIS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_error_tokens (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ERROR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_ambiguus_interpretationes (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_AMBIGUUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_ambiguus_canonica (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_AMBIGUUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_conditionalis_rami (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CONDITIONALIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_conditionalis_finis (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CONDITIONALIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_ramus_sumptus_directiva (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_ramus_sumptus_contentum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_ramus_sumptus_conditio_id (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_RAMUS_SUMPTUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_ramus_omissus_directiva (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_RAMUS_OMISSUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_ramus_omissus_cruda (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_RAMUS_OMISSUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_ramus_omissus_conditio_id (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_RAMUS_OMISSUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

/* ==================================================
 * Constructio ex reductione (PURA - S26; pater numquam - S27)
 * ================================================== */

SilvaValor
silva_c89_construere (
    Piscina*             piscina,
    s32                  productio,
    constans SilvaValor* valores)
{
    (vacuum)piscina;
    (vacuum)valores;
    commutatio (productio)
    {
    casus 0:  /* transitus */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        redde lista;
    }
    casus 1:  /* elementa-appendere */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 2:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 3:  /* sententia-expr */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 4:  /* virgula-series */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_VIRGULA, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 5:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 6:  /* assignatio-simplex */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 7:  /* assignatio-plus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 8:  /* assignatio-minus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 9:  /* assignatio-stella */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 10:  /* assignatio-solidus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 11:  /* assignatio-percentum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 12:  /* assignatio-ampersand */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 13:  /* assignatio-barra */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 14:  /* assignatio-caret */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 15:  /* assignatio-sinistrorsum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 16:  /* assignatio-dextrorsum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ASSIGNATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 17:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 18:  /* ternarius-conditio */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_TERNARIUS, 5);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[3], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 4, valores[4], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 19:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 20:  /* binarium-disiunctio */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 21:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 22:  /* binarium-coniunctio */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 23:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 24:  /* binarium-vel */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 25:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 26:  /* binarium-aut */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 27:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 28:  /* binarium-et */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 29:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 30:  /* binarium-aequalis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 31:  /* binarium-non-aequalis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 32:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 33:  /* binarium-minor */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 34:  /* binarium-maior */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 35:  /* binarium-minor-aequalis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 36:  /* binarium-maior-aequalis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 37:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 38:  /* binarium-sinistrorsum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 39:  /* binarium-dextrorsum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 40:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 41:  /* binarium-plus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 42:  /* binarium-minus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 43:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 44:  /* binarium-stella */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 45:  /* binarium-solidus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 46:  /* binarium-percentum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_BINARIUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 47:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 48:  /* conversio-typus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_CONVERSIO, 4);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[3], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 49:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 50:  /* unarium-incrementum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNARIUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 51:  /* unarium-decrementum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNARIUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 52:  /* unarium-ampersand */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNARIUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 53:  /* unarium-stella */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNARIUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 54:  /* unarium-plus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNARIUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 55:  /* unarium-minus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNARIUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 56:  /* unarium-tilde */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNARIUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 57:  /* unarium-exclamatio */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNARIUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 58:  /* magnitudo-expressionis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_MAGNITUDO_EXPRESSIONIS, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 59:  /* magnitudo-typi */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_MAGNITUDO_TYPI, 4);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[3], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 60:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 61:  /* species-typi-plena */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_SPECIES_TYPI, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 62:  /* species-typi-nuda */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_SPECIES_TYPI, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 63:  /* specificatores-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 64:  /* typus-primitivus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_TYPUS_PRIMITIVUS, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 65:  /* typus-nominatus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_TYPUS_NOMINATUS, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 66:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 67:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 68:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 69:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 70:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 71:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 72:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 73:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 74:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 75:  /* verba-primitiva-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 76:  /* verba-primitiva-appendere */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 77:  /* declarator-abstractus-stella */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 78:  /* declarator-abstractus-catena */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 79:  /* subscriptio-index */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_SUBSCRIPTIO, 4);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[3], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 80:  /* vocatio-argumenta */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_VOCATIO, 4);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[3], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 81:  /* accessus-punctum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ACCESSUS, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 82:  /* accessus-sagitta */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ACCESSUS, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 83:  /* postcrementum-incrementum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_POSTCREMENTUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 84:  /* postcrementum-decrementum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_POSTCREMENTUM, 2);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 85:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 86:  /* transitus */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        redde lista;
    }
    casus 87:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 88:  /* argumenta-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 89:  /* argumenta-appendere */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        lista = silva_valor_lista_appendere(piscina, lista, valores[2]);
        redde lista;
    }
    casus 90:  /* chordae-prima */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 91:  /* chordae-appendere */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 92:  /* primarium-identificator */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_FOLIUM_IDENTIFICATOR, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 93:  /* primarium-integer */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_FOLIUM_INTEGER, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 94:  /* primarium-fluitans */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_FOLIUM_FLUITANS, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 95:  /* primarium-character */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_FOLIUM_CHARACTER, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 96:  /* primarium-chorda */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_FOLIUM_CHORDA, 1);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 97:  /* primarium-parenthesis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_PARENTHESIS, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 98:  /* augmentata */
        redde valores[ZEPHYRUM];
    ordinarius:
        redde silva_valor_nihil();
    }
}

/* ==================================================
 * Fabrica ambigui (spec-v2 par 12)
 * ================================================== */

SilvaValor
silva_c89_ambiguum_fabricare (
    Piscina*   piscina,
    SilvaValor interpretationes,
    s32        canonica)
{
    SilvaNodus* nodus = silva_nodus_creare(piscina,
        (s32)SILVA_C89_GENUS_AMBIGUUS, 2);
    si (nodus == NIHIL)
    {
        redde silva_valor_nihil();
    }
    si (!silva_nodus_ponere(nodus, 0, interpretationes, SILVA_LOCUS_LISTA_NODUS))
    {
        redde silva_valor_nihil();
    }
    si (!silva_nodus_ponere(nodus, 1, silva_valor_index(canonica),
            SILVA_LOCUS_INDEX))
    {
        redde silva_valor_nihil();
    }
    redde silva_valor_nodus(nodus);
}
