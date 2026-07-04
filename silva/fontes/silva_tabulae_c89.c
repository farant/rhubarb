/* silva_tabulae_c89.c - GENERATUM per silva_coquere ex grammatica/c89.stml
 * NOLI MANU MUTARE - regenera per silva/generare.sh.
 *
 * status: 305, productiones: 197, symbola: 134,
 * conflictus servati (GLR): 14
 */

#include "silva_tabulae_c89.h"

/* ==================================================
 * Symbola
 * ================================================== */

hic_manens constans SilvaTabSymbolum SILVA_C89_SYMBOLA[134] = {
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
    /* [15] */ { "TYPEDEF", SILVA_LEX_TYPEDEF, VERUM },
    /* [16] */ { "EXTERN", SILVA_LEX_EXTERN, VERUM },
    /* [17] */ { "STATIC", SILVA_LEX_STATIC, VERUM },
    /* [18] */ { "AUTO", SILVA_LEX_AUTO, VERUM },
    /* [19] */ { "REGISTER", SILVA_LEX_REGISTER, VERUM },
    /* [20] */ { "CONST", SILVA_LEX_CONST, VERUM },
    /* [21] */ { "VOLATILE", SILVA_LEX_VOLATILE, VERUM },
    /* [22] */ { "ELLIPSIS", SILVA_LEX_ELLIPSIS, VERUM },
    /* [23] */ { "STRUCT", SILVA_LEX_STRUCT, VERUM },
    /* [24] */ { "UNION", SILVA_LEX_UNION, VERUM },
    /* [25] */ { "ENUM", SILVA_LEX_ENUM, VERUM },
    /* [26] */ { "BRACE_APERTA", SILVA_LEX_BRACE_APERTA, VERUM },
    /* [27] */ { "BRACE_CLAUSA", SILVA_LEX_BRACE_CLAUSA, VERUM },
    /* [28] */ { "PLUS", SILVA_LEX_PLUS, VERUM },
    /* [29] */ { "MINUS", SILVA_LEX_MINUS, VERUM },
    /* [30] */ { "STAR", SILVA_LEX_STAR, VERUM },
    /* [31] */ { "SOLIDUS", SILVA_LEX_SOLIDUS, VERUM },
    /* [32] */ { "PERCENTUM", SILVA_LEX_PERCENTUM, VERUM },
    /* [33] */ { "AMPERSAND", SILVA_LEX_AMPERSAND, VERUM },
    /* [34] */ { "BARRA", SILVA_LEX_BARRA, VERUM },
    /* [35] */ { "CARET", SILVA_LEX_CARET, VERUM },
    /* [36] */ { "TILDE", SILVA_LEX_TILDE, VERUM },
    /* [37] */ { "EXCLAMATIO", SILVA_LEX_EXCLAMATIO, VERUM },
    /* [38] */ { "ET_ET", SILVA_LEX_ET_ET, VERUM },
    /* [39] */ { "VEL_VEL", SILVA_LEX_VEL_VEL, VERUM },
    /* [40] */ { "AEQUALIS_AEQUALIS", SILVA_LEX_AEQUALIS_AEQUALIS, VERUM },
    /* [41] */ { "NON_AEQUALIS", SILVA_LEX_NON_AEQUALIS, VERUM },
    /* [42] */ { "MINOR", SILVA_LEX_MINOR, VERUM },
    /* [43] */ { "MAIOR", SILVA_LEX_MAIOR, VERUM },
    /* [44] */ { "MINOR_AEQUALIS", SILVA_LEX_MINOR_AEQUALIS, VERUM },
    /* [45] */ { "MAIOR_AEQUALIS", SILVA_LEX_MAIOR_AEQUALIS, VERUM },
    /* [46] */ { "SINISTRORSUM", SILVA_LEX_SINISTRORSUM, VERUM },
    /* [47] */ { "DEXTRORSUM", SILVA_LEX_DEXTRORSUM, VERUM },
    /* [48] */ { "ASSIGNATIO", SILVA_LEX_ASSIGNATIO, VERUM },
    /* [49] */ { "PLUS_ASSIGNATIO", SILVA_LEX_PLUS_ASSIGNATIO, VERUM },
    /* [50] */ { "MINUS_ASSIGNATIO", SILVA_LEX_MINUS_ASSIGNATIO, VERUM },
    /* [51] */ { "STAR_ASSIGNATIO", SILVA_LEX_STAR_ASSIGNATIO, VERUM },
    /* [52] */ { "SOLIDUS_ASSIGNATIO", SILVA_LEX_SOLIDUS_ASSIGNATIO, VERUM },
    /* [53] */ { "PERCENTUM_ASSIGNATIO", SILVA_LEX_PERCENTUM_ASSIGNATIO, VERUM },
    /* [54] */ { "AMPERSAND_ASSIGNATIO", SILVA_LEX_AMPERSAND_ASSIGNATIO, VERUM },
    /* [55] */ { "BARRA_ASSIGNATIO", SILVA_LEX_BARRA_ASSIGNATIO, VERUM },
    /* [56] */ { "CARET_ASSIGNATIO", SILVA_LEX_CARET_ASSIGNATIO, VERUM },
    /* [57] */ { "SINISTRORSUM_ASSIGNATIO", SILVA_LEX_SINISTRORSUM_ASSIGNATIO, VERUM },
    /* [58] */ { "DEXTRORSUM_ASSIGNATIO", SILVA_LEX_DEXTRORSUM_ASSIGNATIO, VERUM },
    /* [59] */ { "INCREMENTUM", SILVA_LEX_INCREMENTUM, VERUM },
    /* [60] */ { "DECREMENTUM", SILVA_LEX_DECREMENTUM, VERUM },
    /* [61] */ { "PUNCTUM", SILVA_LEX_PUNCTUM, VERUM },
    /* [62] */ { "SAGITTA", SILVA_LEX_SAGITTA, VERUM },
    /* [63] */ { "QUAESTIO", SILVA_LEX_QUAESTIO, VERUM },
    /* [64] */ { "COLON", SILVA_LEX_COLON, VERUM },
    /* [65] */ { "COMMA", SILVA_LEX_COMMA, VERUM },
    /* [66] */ { "SEMICOLON", SILVA_LEX_SEMICOLON, VERUM },
    /* [67] */ { "PAREN_APERTA", SILVA_LEX_PAREN_APERTA, VERUM },
    /* [68] */ { "PAREN_CLAUSA", SILVA_LEX_PAREN_CLAUSA, VERUM },
    /* [69] */ { "QUADRA_APERTA", SILVA_LEX_QUADRA_APERTA, VERUM },
    /* [70] */ { "QUADRA_CLAUSA", SILVA_LEX_QUADRA_CLAUSA, VERUM },
    /* [71] */ { "EOF", SILVA_LEX_EOF, VERUM },
    /* [72] */ { "elementa", SILVA_LEX_EOF, FALSUM },
    /* [73] */ { "elementum", SILVA_LEX_EOF, FALSUM },
    /* [74] */ { "declaratio", SILVA_LEX_EOF, FALSUM },
    /* [75] */ { "atomum-non-primitivum", SILVA_LEX_EOF, FALSUM },
    /* [76] */ { "atomum-decl-non-primitivum", SILVA_LEX_EOF, FALSUM },
    /* [77] */ { "specificatores-decl", SILVA_LEX_EOF, FALSUM },
    /* [78] */ { "sd-post-primitivum", SILVA_LEX_EOF, FALSUM },
    /* [79] */ { "sd-post-aliud", SILVA_LEX_EOF, FALSUM },
    /* [80] */ { "sd-post-tag", SILVA_LEX_EOF, FALSUM },
    /* [81] */ { "atomum-tag", SILVA_LEX_EOF, FALSUM },
    /* [82] */ { "typus-primitivus-atomum", SILVA_LEX_EOF, FALSUM },
    /* [83] */ { "declaratores", SILVA_LEX_EOF, FALSUM },
    /* [84] */ { "declarator-initiatus", SILVA_LEX_EOF, FALSUM },
    /* [85] */ { "initiator", SILVA_LEX_EOF, FALSUM },
    /* [86] */ { "congeries-elementa", SILVA_LEX_EOF, FALSUM },
    /* [87] */ { "species-structurae", SILVA_LEX_EOF, FALSUM },
    /* [88] */ { "species-unionis", SILVA_LEX_EOF, FALSUM },
    /* [89] */ { "membra", SILVA_LEX_EOF, FALSUM },
    /* [90] */ { "membrum", SILVA_LEX_EOF, FALSUM },
    /* [91] */ { "membra-declaratores", SILVA_LEX_EOF, FALSUM },
    /* [92] */ { "membrum-declarator", SILVA_LEX_EOF, FALSUM },
    /* [93] */ { "species-enumerationis", SILVA_LEX_EOF, FALSUM },
    /* [94] */ { "enumeratores", SILVA_LEX_EOF, FALSUM },
    /* [95] */ { "enumerator-unus", SILVA_LEX_EOF, FALSUM },
    /* [96] */ { "declarator", SILVA_LEX_EOF, FALSUM },
    /* [97] */ { "qualificatores", SILVA_LEX_EOF, FALSUM },
    /* [98] */ { "declarator-directus", SILVA_LEX_EOF, FALSUM },
    /* [99] */ { "mensura-aciei", SILVA_LEX_EOF, FALSUM },
    /* [100] */ { "parametra", SILVA_LEX_EOF, FALSUM },
    /* [101] */ { "parametra-plena", SILVA_LEX_EOF, FALSUM },
    /* [102] */ { "parametrum", SILVA_LEX_EOF, FALSUM },
    /* [103] */ { "sententia-expressionis", SILVA_LEX_EOF, FALSUM },
    /* [104] */ { "expressio", SILVA_LEX_EOF, FALSUM },
    /* [105] */ { "assignatio", SILVA_LEX_EOF, FALSUM },
    /* [106] */ { "ternarius", SILVA_LEX_EOF, FALSUM },
    /* [107] */ { "disiunctio", SILVA_LEX_EOF, FALSUM },
    /* [108] */ { "coniunctio", SILVA_LEX_EOF, FALSUM },
    /* [109] */ { "vel-bitorum", SILVA_LEX_EOF, FALSUM },
    /* [110] */ { "aut-bitorum", SILVA_LEX_EOF, FALSUM },
    /* [111] */ { "et-bitorum", SILVA_LEX_EOF, FALSUM },
    /* [112] */ { "aequalitas", SILVA_LEX_EOF, FALSUM },
    /* [113] */ { "comparatio", SILVA_LEX_EOF, FALSUM },
    /* [114] */ { "translatio", SILVA_LEX_EOF, FALSUM },
    /* [115] */ { "additivum", SILVA_LEX_EOF, FALSUM },
    /* [116] */ { "multiplicativum", SILVA_LEX_EOF, FALSUM },
    /* [117] */ { "conversio", SILVA_LEX_EOF, FALSUM },
    /* [118] */ { "unarium", SILVA_LEX_EOF, FALSUM },
    /* [119] */ { "species-typi", SILVA_LEX_EOF, FALSUM },
    /* [120] */ { "specificatores", SILVA_LEX_EOF, FALSUM },
    /* [121] */ { "sq-post-primitivum", SILVA_LEX_EOF, FALSUM },
    /* [122] */ { "sq-post-aliud", SILVA_LEX_EOF, FALSUM },
    /* [123] */ { "sq-post-tag", SILVA_LEX_EOF, FALSUM },
    /* [124] */ { "verbum-primitivum", SILVA_LEX_EOF, FALSUM },
    /* [125] */ { "verba-primitiva", SILVA_LEX_EOF, FALSUM },
    /* [126] */ { "declarator-abstractus", SILVA_LEX_EOF, FALSUM },
    /* [127] */ { "abstractus-directus", SILVA_LEX_EOF, FALSUM },
    /* [128] */ { "postfixum", SILVA_LEX_EOF, FALSUM },
    /* [129] */ { "argumenta", SILVA_LEX_EOF, FALSUM },
    /* [130] */ { "argumenta-plena", SILVA_LEX_EOF, FALSUM },
    /* [131] */ { "chordae", SILVA_LEX_EOF, FALSUM },
    /* [132] */ { "primarium", SILVA_LEX_EOF, FALSUM },
    /* [133] */ { "__initium__", SILVA_LEX_EOF, FALSUM }
};

/* ==================================================
 * Productiones
 * ================================================== */

hic_manens constans SilvaTabProductio SILVA_C89_PRODUCTIONES[197] = {
    /* P0  elementa (0 symbola) */ { 72, 0, NIHIL },
    /* P1  elementa (2 symbola) */ { 72, 2, "elementa-appendere" },
    /* P2  elementum (1 symbola) */ { 73, 1, NIHIL },
    /* P3  elementum (1 symbola) */ { 73, 1, NIHIL },
    /* P4  declaratio (3 symbola) */ { 74, 3, "declaratio-plena" },
    /* P5  declaratio (2 symbola) */ { 74, 2, "declaratio-nuda" },
    /* P6  atomum-non-primitivum (1 symbola) */ { 75, 1, "typus-nominatus" },
    /* P7  atomum-non-primitivum (1 symbola) */ { 75, 1, NIHIL },
    /* P8  atomum-non-primitivum (1 symbola) */ { 75, 1, NIHIL },
    /* P9  atomum-decl-non-primitivum (1 symbola) */ { 76, 1, NIHIL },
    /* P10 atomum-decl-non-primitivum (1 symbola) */ { 76, 1, NIHIL },
    /* P11 atomum-decl-non-primitivum (1 symbola) */ { 76, 1, NIHIL },
    /* P12 atomum-decl-non-primitivum (1 symbola) */ { 76, 1, NIHIL },
    /* P13 atomum-decl-non-primitivum (1 symbola) */ { 76, 1, NIHIL },
    /* P14 atomum-decl-non-primitivum (1 symbola) */ { 76, 1, NIHIL },
    /* P15 specificatores-decl (1 symbola) */ { 77, 1, NIHIL },
    /* P16 specificatores-decl (1 symbola) */ { 77, 1, NIHIL },
    /* P17 specificatores-decl (1 symbola) */ { 77, 1, NIHIL },
    /* P18 sd-post-primitivum (1 symbola) */ { 78, 1, "sd-pp-primum" },
    /* P19 sd-post-primitivum (2 symbola) */ { 78, 2, "sd-pp-ex-alio" },
    /* P20 sd-post-primitivum (2 symbola) */ { 78, 2, "sd-pp-ex-tag" },
    /* P21 sd-post-aliud (1 symbola) */ { 79, 1, "sd-pa-primum" },
    /* P22 sd-post-aliud (2 symbola) */ { 79, 2, "sd-pa-ex-primitivo" },
    /* P23 sd-post-aliud (2 symbola) */ { 79, 2, "sd-pa-ex-alio" },
    /* P24 sd-post-aliud (2 symbola) */ { 79, 2, "sd-pa-ex-tag" },
    /* P25 sd-post-tag (1 symbola) */ { 80, 1, "sd-pt-primum" },
    /* P26 sd-post-tag (2 symbola) */ { 80, 2, "sd-pt-ex-primitivo" },
    /* P27 sd-post-tag (2 symbola) */ { 80, 2, "sd-pt-ex-alio" },
    /* P28 sd-post-tag (2 symbola) */ { 80, 2, "sd-pt-ex-tag" },
    /* P29 atomum-tag (1 symbola) */ { 81, 1, NIHIL },
    /* P30 atomum-tag (1 symbola) */ { 81, 1, NIHIL },
    /* P31 atomum-tag (1 symbola) */ { 81, 1, NIHIL },
    /* P32 typus-primitivus-atomum (1 symbola) */ { 82, 1, "typus-primitivus" },
    /* P33 declaratores (1 symbola) */ { 83, 1, "declaratores-primum" },
    /* P34 declaratores (3 symbola) */ { 83, 3, "declaratores-appendere" },
    /* P35 declarator-initiatus (3 symbola) */ { 84, 3, "declarator-initiatus" },
    /* P36 declarator-initiatus (1 symbola) */ { 84, 1, NIHIL },
    /* P37 initiator (1 symbola) */ { 85, 1, NIHIL },
    /* P38 initiator (3 symbola) */ { 85, 3, "congeries" },
    /* P39 congeries-elementa (1 symbola) */ { 86, 1, "congeries-primum" },
    /* P40 congeries-elementa (3 symbola) */ { 86, 3, "congeries-appendere" },
    /* P41 congeries-elementa (2 symbola) */ { 86, 2, "congeries-cauda" },
    /* P42 species-structurae (5 symbola) */ { 87, 5, "structura-plena" },
    /* P43 species-structurae (4 symbola) */ { 87, 4, "structura-anonyma" },
    /* P44 species-structurae (2 symbola) */ { 87, 2, "structura-nominata" },
    /* P45 species-unionis (5 symbola) */ { 88, 5, "unio-plena" },
    /* P46 species-unionis (4 symbola) */ { 88, 4, "unio-anonyma" },
    /* P47 species-unionis (2 symbola) */ { 88, 2, "unio-nominata" },
    /* P48 membra (0 symbola) */ { 89, 0, NIHIL },
    /* P49 membra (2 symbola) */ { 89, 2, "membra-appendere" },
    /* P50 membrum (3 symbola) */ { 90, 3, "membrum" },
    /* P51 membra-declaratores (1 symbola) */ { 91, 1, "membra-decl-primum" },
    /* P52 membra-declaratores (3 symbola) */ { 91, 3, "membra-decl-appendere" },
    /* P53 membrum-declarator (1 symbola) */ { 92, 1, NIHIL },
    /* P54 membrum-declarator (3 symbola) */ { 92, 3, "campus" },
    /* P55 membrum-declarator (2 symbola) */ { 92, 2, "campus-sine-declaratore" },
    /* P56 species-enumerationis (5 symbola) */ { 93, 5, "enumeratio-plena" },
    /* P57 species-enumerationis (4 symbola) */ { 93, 4, "enumeratio-anonyma" },
    /* P58 species-enumerationis (2 symbola) */ { 93, 2, "enumeratio-nominata" },
    /* P59 enumeratores (1 symbola) */ { 94, 1, "enumeratores-primum" },
    /* P60 enumeratores (3 symbola) */ { 94, 3, "enumeratores-appendere" },
    /* P61 enumerator-unus (1 symbola) */ { 95, 1, "enumerator" },
    /* P62 enumerator-unus (3 symbola) */ { 95, 3, "enumerator-valoratus" },
    /* P63 declarator (3 symbola) */ { 96, 3, "declarator-monstrator" },
    /* P64 declarator (1 symbola) */ { 96, 1, NIHIL },
    /* P65 qualificatores (0 symbola) */ { 97, 0, NIHIL },
    /* P66 qualificatores (2 symbola) */ { 97, 2, "qualificatores-const" },
    /* P67 qualificatores (2 symbola) */ { 97, 2, "qualificatores-volatile" },
    /* P68 declarator-directus (1 symbola) */ { 98, 1, "declarator-titulus" },
    /* P69 declarator-directus (3 symbola) */ { 98, 3, "declarator-parenthesis" },
    /* P70 declarator-directus (4 symbola) */ { 98, 4, "declarator-aciei" },
    /* P71 declarator-directus (3 symbola) */ { 98, 3, "declarator-aciei-vacua" },
    /* P72 declarator-directus (4 symbola) */ { 98, 4, "declarator-functionis" },
    /* P73 mensura-aciei (1 symbola) */ { 99, 1, NIHIL },
    /* P74 parametra (0 symbola) */ { 100, 0, NIHIL },
    /* P75 parametra (1 symbola) */ { 100, 1, NIHIL },
    /* P76 parametra-plena (1 symbola) */ { 101, 1, "parametra-primum" },
    /* P77 parametra-plena (3 symbola) */ { 101, 3, "parametra-appendere" },
    /* P78 parametra-plena (3 symbola) */ { 101, 3, "parametra-ellipsis" },
    /* P79 parametrum (1 symbola) */ { 102, 1, "parametrum-nudum" },
    /* P80 parametrum (2 symbola) */ { 102, 2, "parametrum-declaratum" },
    /* P81 parametrum (2 symbola) */ { 102, 2, "parametrum-abstractum" },
    /* P82 sententia-expressionis (2 symbola) */ { 103, 2, "sententia-expr" },
    /* P83 expressio (3 symbola) */ { 104, 3, "virgula-series" },
    /* P84 expressio (1 symbola) */ { 104, 1, NIHIL },
    /* P85 assignatio (3 symbola) */ { 105, 3, "assignatio-simplex" },
    /* P86 assignatio (3 symbola) */ { 105, 3, "assignatio-plus" },
    /* P87 assignatio (3 symbola) */ { 105, 3, "assignatio-minus" },
    /* P88 assignatio (3 symbola) */ { 105, 3, "assignatio-stella" },
    /* P89 assignatio (3 symbola) */ { 105, 3, "assignatio-solidus" },
    /* P90 assignatio (3 symbola) */ { 105, 3, "assignatio-percentum" },
    /* P91 assignatio (3 symbola) */ { 105, 3, "assignatio-ampersand" },
    /* P92 assignatio (3 symbola) */ { 105, 3, "assignatio-barra" },
    /* P93 assignatio (3 symbola) */ { 105, 3, "assignatio-caret" },
    /* P94 assignatio (3 symbola) */ { 105, 3, "assignatio-sinistrorsum" },
    /* P95 assignatio (3 symbola) */ { 105, 3, "assignatio-dextrorsum" },
    /* P96 assignatio (1 symbola) */ { 105, 1, NIHIL },
    /* P97 ternarius (5 symbola) */ { 106, 5, "ternarius-conditio" },
    /* P98 ternarius (1 symbola) */ { 106, 1, NIHIL },
    /* P99 disiunctio (3 symbola) */ { 107, 3, "binarium-disiunctio" },
    /* P100 disiunctio (1 symbola) */ { 107, 1, NIHIL },
    /* P101 coniunctio (3 symbola) */ { 108, 3, "binarium-coniunctio" },
    /* P102 coniunctio (1 symbola) */ { 108, 1, NIHIL },
    /* P103 vel-bitorum (3 symbola) */ { 109, 3, "binarium-vel" },
    /* P104 vel-bitorum (1 symbola) */ { 109, 1, NIHIL },
    /* P105 aut-bitorum (3 symbola) */ { 110, 3, "binarium-aut" },
    /* P106 aut-bitorum (1 symbola) */ { 110, 1, NIHIL },
    /* P107 et-bitorum (3 symbola) */ { 111, 3, "binarium-et" },
    /* P108 et-bitorum (1 symbola) */ { 111, 1, NIHIL },
    /* P109 aequalitas (3 symbola) */ { 112, 3, "binarium-aequalis" },
    /* P110 aequalitas (3 symbola) */ { 112, 3, "binarium-non-aequalis" },
    /* P111 aequalitas (1 symbola) */ { 112, 1, NIHIL },
    /* P112 comparatio (3 symbola) */ { 113, 3, "binarium-minor" },
    /* P113 comparatio (3 symbola) */ { 113, 3, "binarium-maior" },
    /* P114 comparatio (3 symbola) */ { 113, 3, "binarium-minor-aequalis" },
    /* P115 comparatio (3 symbola) */ { 113, 3, "binarium-maior-aequalis" },
    /* P116 comparatio (1 symbola) */ { 113, 1, NIHIL },
    /* P117 translatio (3 symbola) */ { 114, 3, "binarium-sinistrorsum" },
    /* P118 translatio (3 symbola) */ { 114, 3, "binarium-dextrorsum" },
    /* P119 translatio (1 symbola) */ { 114, 1, NIHIL },
    /* P120 additivum (3 symbola) */ { 115, 3, "binarium-plus" },
    /* P121 additivum (3 symbola) */ { 115, 3, "binarium-minus" },
    /* P122 additivum (1 symbola) */ { 115, 1, NIHIL },
    /* P123 multiplicativum (3 symbola) */ { 116, 3, "binarium-stella" },
    /* P124 multiplicativum (3 symbola) */ { 116, 3, "binarium-solidus" },
    /* P125 multiplicativum (3 symbola) */ { 116, 3, "binarium-percentum" },
    /* P126 multiplicativum (1 symbola) */ { 116, 1, NIHIL },
    /* P127 conversio (4 symbola) */ { 117, 4, "conversio-typus" },
    /* P128 conversio (1 symbola) */ { 117, 1, NIHIL },
    /* P129 unarium (2 symbola) */ { 118, 2, "unarium-incrementum" },
    /* P130 unarium (2 symbola) */ { 118, 2, "unarium-decrementum" },
    /* P131 unarium (2 symbola) */ { 118, 2, "unarium-ampersand" },
    /* P132 unarium (2 symbola) */ { 118, 2, "unarium-stella" },
    /* P133 unarium (2 symbola) */ { 118, 2, "unarium-plus" },
    /* P134 unarium (2 symbola) */ { 118, 2, "unarium-minus" },
    /* P135 unarium (2 symbola) */ { 118, 2, "unarium-tilde" },
    /* P136 unarium (2 symbola) */ { 118, 2, "unarium-exclamatio" },
    /* P137 unarium (2 symbola) */ { 118, 2, "magnitudo-expressionis" },
    /* P138 unarium (4 symbola) */ { 118, 4, "magnitudo-typi" },
    /* P139 unarium (1 symbola) */ { 118, 1, NIHIL },
    /* P140 species-typi (2 symbola) */ { 119, 2, "species-typi-plena" },
    /* P141 species-typi (1 symbola) */ { 119, 1, "species-typi-nuda" },
    /* P142 specificatores (1 symbola) */ { 120, 1, NIHIL },
    /* P143 specificatores (1 symbola) */ { 120, 1, NIHIL },
    /* P144 specificatores (1 symbola) */ { 120, 1, NIHIL },
    /* P145 sq-post-primitivum (1 symbola) */ { 121, 1, "sq-pp-primum" },
    /* P146 sq-post-primitivum (2 symbola) */ { 121, 2, "sq-pp-ex-alio" },
    /* P147 sq-post-primitivum (2 symbola) */ { 121, 2, "sq-pp-ex-tag" },
    /* P148 sq-post-aliud (1 symbola) */ { 122, 1, "sq-pa-primum" },
    /* P149 sq-post-aliud (2 symbola) */ { 122, 2, "sq-pa-ex-primitivo" },
    /* P150 sq-post-aliud (2 symbola) */ { 122, 2, "sq-pa-ex-alio" },
    /* P151 sq-post-aliud (2 symbola) */ { 122, 2, "sq-pa-ex-tag" },
    /* P152 sq-post-tag (1 symbola) */ { 123, 1, "sq-pt-primum" },
    /* P153 sq-post-tag (2 symbola) */ { 123, 2, "sq-pt-ex-primitivo" },
    /* P154 sq-post-tag (2 symbola) */ { 123, 2, "sq-pt-ex-alio" },
    /* P155 sq-post-tag (2 symbola) */ { 123, 2, "sq-pt-ex-tag" },
    /* P156 verbum-primitivum (1 symbola) */ { 124, 1, NIHIL },
    /* P157 verbum-primitivum (1 symbola) */ { 124, 1, NIHIL },
    /* P158 verbum-primitivum (1 symbola) */ { 124, 1, NIHIL },
    /* P159 verbum-primitivum (1 symbola) */ { 124, 1, NIHIL },
    /* P160 verbum-primitivum (1 symbola) */ { 124, 1, NIHIL },
    /* P161 verbum-primitivum (1 symbola) */ { 124, 1, NIHIL },
    /* P162 verbum-primitivum (1 symbola) */ { 124, 1, NIHIL },
    /* P163 verbum-primitivum (1 symbola) */ { 124, 1, NIHIL },
    /* P164 verbum-primitivum (1 symbola) */ { 124, 1, NIHIL },
    /* P165 verba-primitiva (1 symbola) */ { 125, 1, "verba-primitiva-primum" },
    /* P166 verba-primitiva (2 symbola) */ { 125, 2, "verba-primitiva-appendere" },
    /* P167 declarator-abstractus (2 symbola) */ { 126, 2, "declarator-abstractus-stella" },
    /* P168 declarator-abstractus (3 symbola) */ { 126, 3, "declarator-abstractus-catena" },
    /* P169 declarator-abstractus (1 symbola) */ { 126, 1, NIHIL },
    /* P170 abstractus-directus (3 symbola) */ { 127, 3, "abstractus-parenthesis" },
    /* P171 abstractus-directus (4 symbola) */ { 127, 4, "abstractus-aciei" },
    /* P172 abstractus-directus (3 symbola) */ { 127, 3, "abstractus-aciei-sine" },
    /* P173 abstractus-directus (3 symbola) */ { 127, 3, "abstractus-aciei-vacua" },
    /* P174 abstractus-directus (2 symbola) */ { 127, 2, "abstractus-aciei-nuda" },
    /* P175 abstractus-directus (4 symbola) */ { 127, 4, "abstractus-functionis" },
    /* P176 abstractus-directus (3 symbola) */ { 127, 3, "abstractus-functionis-sine" },
    /* P177 postfixum (4 symbola) */ { 128, 4, "subscriptio-index" },
    /* P178 postfixum (4 symbola) */ { 128, 4, "vocatio-argumenta" },
    /* P179 postfixum (3 symbola) */ { 128, 3, "accessus-punctum" },
    /* P180 postfixum (3 symbola) */ { 128, 3, "accessus-sagitta" },
    /* P181 postfixum (2 symbola) */ { 128, 2, "postcrementum-incrementum" },
    /* P182 postfixum (2 symbola) */ { 128, 2, "postcrementum-decrementum" },
    /* P183 postfixum (1 symbola) */ { 128, 1, NIHIL },
    /* P184 argumenta (0 symbola) */ { 129, 0, NIHIL },
    /* P185 argumenta (1 symbola) */ { 129, 1, NIHIL },
    /* P186 argumenta-plena (1 symbola) */ { 130, 1, "argumenta-primum" },
    /* P187 argumenta-plena (3 symbola) */ { 130, 3, "argumenta-appendere" },
    /* P188 chordae (1 symbola) */ { 131, 1, "chordae-prima" },
    /* P189 chordae (2 symbola) */ { 131, 2, "chordae-appendere" },
    /* P190 primarium (1 symbola) */ { 132, 1, "primarium-identificator" },
    /* P191 primarium (1 symbola) */ { 132, 1, "primarium-integer" },
    /* P192 primarium (1 symbola) */ { 132, 1, "primarium-fluitans" },
    /* P193 primarium (1 symbola) */ { 132, 1, "primarium-character" },
    /* P194 primarium (1 symbola) */ { 132, 1, "primarium-chorda" },
    /* P195 primarium (3 symbola) */ { 132, 3, "primarium-parenthesis" },
    /* P196 __initium__ (2 symbola) */ { 133, 2, NIHIL }
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
    {   5, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* VOID: reducere P0 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* CHAR: reducere P0 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* SHORT: reducere P0 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INT: reducere P0 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* LONG: reducere P0 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* FLOAT_KW: reducere P0 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* DOUBLE: reducere P0 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* SIGNED: reducere P0 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* UNSIGNED: reducere P0 */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* SIZEOF: reducere P0 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* TYPEDEF: reducere P0 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* EXTERN: reducere P0 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* STATIC: reducere P0 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* AUTO: reducere P0 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* REGISTER: reducere P0 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* CONST: reducere P0 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* VOLATILE: reducere P0 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* STRUCT: reducere P0 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* UNION: reducere P0 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* ENUM: reducere P0 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* PLUS: reducere P0 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* MINUS: reducere P0 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* STAR: reducere P0 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* AMPERSAND: reducere P0 */
    {  36, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* TILDE: reducere P0 */
    {  37, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* EXCLAMATIO: reducere P0 */
    {  59, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INCREMENTUM: reducere P0 */
    {  60, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* DECREMENTUM: reducere P0 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* PAREN_APERTA: reducere P0 */
    {  71, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* EOF: reducere P0 */
    /* --- status 1 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,   7, 0 },  /* IDENTIFICATOR: transponere -> 7 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    {  71, SILVA_TAB_ACTIO_TRANSPONERE,  70, 0 },  /* EOF: transponere -> 70 */
    /* --- status 2 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* IDENTIFICATOR: reducere P1 elementa-appendere */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* INTEGER: reducere P1 elementa-appendere */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* FLOAT: reducere P1 elementa-appendere */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* CHARACTER_LIT: reducere P1 elementa-appendere */
    {   4, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* STRING_LIT: reducere P1 elementa-appendere */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* VOID: reducere P1 elementa-appendere */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* CHAR: reducere P1 elementa-appendere */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* SHORT: reducere P1 elementa-appendere */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* INT: reducere P1 elementa-appendere */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* LONG: reducere P1 elementa-appendere */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* FLOAT_KW: reducere P1 elementa-appendere */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* DOUBLE: reducere P1 elementa-appendere */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* SIGNED: reducere P1 elementa-appendere */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* UNSIGNED: reducere P1 elementa-appendere */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* SIZEOF: reducere P1 elementa-appendere */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* TYPEDEF: reducere P1 elementa-appendere */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* EXTERN: reducere P1 elementa-appendere */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* STATIC: reducere P1 elementa-appendere */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* AUTO: reducere P1 elementa-appendere */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* REGISTER: reducere P1 elementa-appendere */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* CONST: reducere P1 elementa-appendere */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* VOLATILE: reducere P1 elementa-appendere */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* STRUCT: reducere P1 elementa-appendere */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* UNION: reducere P1 elementa-appendere */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* ENUM: reducere P1 elementa-appendere */
    {  28, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* PLUS: reducere P1 elementa-appendere */
    {  29, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* MINUS: reducere P1 elementa-appendere */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* STAR: reducere P1 elementa-appendere */
    {  33, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* AMPERSAND: reducere P1 elementa-appendere */
    {  36, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* TILDE: reducere P1 elementa-appendere */
    {  37, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* EXCLAMATIO: reducere P1 elementa-appendere */
    {  59, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* INCREMENTUM: reducere P1 elementa-appendere */
    {  60, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* DECREMENTUM: reducere P1 elementa-appendere */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* PAREN_APERTA: reducere P1 elementa-appendere */
    {  71, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* EOF: reducere P1 elementa-appendere */
    /* --- status 3 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* IDENTIFICATOR: reducere P2 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* INTEGER: reducere P2 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* FLOAT: reducere P2 */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* CHARACTER_LIT: reducere P2 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* STRING_LIT: reducere P2 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* VOID: reducere P2 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* CHAR: reducere P2 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* SHORT: reducere P2 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* INT: reducere P2 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* LONG: reducere P2 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* FLOAT_KW: reducere P2 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* DOUBLE: reducere P2 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* SIGNED: reducere P2 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* UNSIGNED: reducere P2 */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* SIZEOF: reducere P2 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* TYPEDEF: reducere P2 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* EXTERN: reducere P2 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* STATIC: reducere P2 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* AUTO: reducere P2 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* REGISTER: reducere P2 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* CONST: reducere P2 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* VOLATILE: reducere P2 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* STRUCT: reducere P2 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* UNION: reducere P2 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* ENUM: reducere P2 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* PLUS: reducere P2 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* MINUS: reducere P2 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* STAR: reducere P2 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* AMPERSAND: reducere P2 */
    {  36, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* TILDE: reducere P2 */
    {  37, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* EXCLAMATIO: reducere P2 */
    {  59, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* INCREMENTUM: reducere P2 */
    {  60, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* DECREMENTUM: reducere P2 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* PAREN_APERTA: reducere P2 */
    {  71, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* EOF: reducere P2 */
    /* --- status 4 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* IDENTIFICATOR: reducere P3 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* INTEGER: reducere P3 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* FLOAT: reducere P3 */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* CHARACTER_LIT: reducere P3 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* STRING_LIT: reducere P3 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* VOID: reducere P3 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* CHAR: reducere P3 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* SHORT: reducere P3 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* INT: reducere P3 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* LONG: reducere P3 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* FLOAT_KW: reducere P3 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* DOUBLE: reducere P3 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* SIGNED: reducere P3 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* UNSIGNED: reducere P3 */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* SIZEOF: reducere P3 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* TYPEDEF: reducere P3 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* EXTERN: reducere P3 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* STATIC: reducere P3 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* AUTO: reducere P3 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* REGISTER: reducere P3 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* CONST: reducere P3 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* VOLATILE: reducere P3 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* STRUCT: reducere P3 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* UNION: reducere P3 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* ENUM: reducere P3 */
    {  28, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* PLUS: reducere P3 */
    {  29, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* MINUS: reducere P3 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* STAR: reducere P3 */
    {  33, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* AMPERSAND: reducere P3 */
    {  36, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* TILDE: reducere P3 */
    {  37, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* EXCLAMATIO: reducere P3 */
    {  59, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* INCREMENTUM: reducere P3 */
    {  60, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* DECREMENTUM: reducere P3 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* PAREN_APERTA: reducere P3 */
    {  71, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* EOF: reducere P3 */
    /* --- status 5 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* IDENTIFICATOR: transponere -> 76 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  74, 0 },  /* STAR: transponere -> 74 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  77, 0 },  /* PAREN_APERTA: transponere -> 77 */
    /* --- status 6 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* IDENTIFICATOR: reducere P17 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* STAR: reducere P17 */
    {  66, SILVA_TAB_ACTIO_TRANSPONERE,  78, 0 },  /* SEMICOLON: transponere -> 78 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* PAREN_APERTA: reducere P17 */
    /* --- status 7 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* IDENTIFICATOR: reducere P6 typus-nominatus */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* VOID: reducere P6 typus-nominatus */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* CHAR: reducere P6 typus-nominatus */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* SHORT: reducere P6 typus-nominatus */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* INT: reducere P6 typus-nominatus */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* LONG: reducere P6 typus-nominatus */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* FLOAT_KW: reducere P6 typus-nominatus */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* DOUBLE: reducere P6 typus-nominatus */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* SIGNED: reducere P6 typus-nominatus */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* UNSIGNED: reducere P6 typus-nominatus */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* TYPEDEF: reducere P6 typus-nominatus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* EXTERN: reducere P6 typus-nominatus */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STATIC: reducere P6 typus-nominatus */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* AUTO: reducere P6 typus-nominatus */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* REGISTER: reducere P6 typus-nominatus */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* CONST: reducere P6 typus-nominatus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* VOLATILE: reducere P6 typus-nominatus */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STRUCT: reducere P6 typus-nominatus */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* UNION: reducere P6 typus-nominatus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* ENUM: reducere P6 typus-nominatus */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PLUS: reducere P190 primarium-identificator */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MINUS: reducere P190 primarium-identificator */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STAR: reducere P6 typus-nominatus */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* STAR: reducere P190 primarium-identificator */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SOLIDUS: reducere P190 primarium-identificator */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PERCENTUM: reducere P190 primarium-identificator */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* AMPERSAND: reducere P190 primarium-identificator */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* BARRA: reducere P190 primarium-identificator */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* CARET: reducere P190 primarium-identificator */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* ET_ET: reducere P190 primarium-identificator */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* VEL_VEL: reducere P190 primarium-identificator */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* AEQUALIS_AEQUALIS: reducere P190 primarium-identificator */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* NON_AEQUALIS: reducere P190 primarium-identificator */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MINOR: reducere P190 primarium-identificator */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MAIOR: reducere P190 primarium-identificator */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MINOR_AEQUALIS: reducere P190 primarium-identificator */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MAIOR_AEQUALIS: reducere P190 primarium-identificator */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SINISTRORSUM: reducere P190 primarium-identificator */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* DEXTRORSUM: reducere P190 primarium-identificator */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* ASSIGNATIO: reducere P190 primarium-identificator */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PLUS_ASSIGNATIO: reducere P190 primarium-identificator */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MINUS_ASSIGNATIO: reducere P190 primarium-identificator */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* STAR_ASSIGNATIO: reducere P190 primarium-identificator */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P190 primarium-identificator */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P190 primarium-identificator */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P190 primarium-identificator */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* BARRA_ASSIGNATIO: reducere P190 primarium-identificator */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* CARET_ASSIGNATIO: reducere P190 primarium-identificator */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P190 primarium-identificator */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P190 primarium-identificator */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* INCREMENTUM: reducere P190 primarium-identificator */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* DECREMENTUM: reducere P190 primarium-identificator */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PUNCTUM: reducere P190 primarium-identificator */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SAGITTA: reducere P190 primarium-identificator */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* QUAESTIO: reducere P190 primarium-identificator */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* COMMA: reducere P190 primarium-identificator */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SEMICOLON: reducere P190 primarium-identificator */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* PAREN_APERTA: reducere P6 typus-nominatus */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PAREN_APERTA: reducere P190 primarium-identificator */
    {  68, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* PAREN_CLAUSA: reducere P6 typus-nominatus */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PAREN_CLAUSA: reducere P190 primarium-identificator */
    {  69, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* QUADRA_APERTA: reducere P6 typus-nominatus */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* QUADRA_APERTA: reducere P190 primarium-identificator */
    /* --- status 8 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* IDENTIFICATOR: reducere P7 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* VOID: reducere P7 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* CHAR: reducere P7 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* SHORT: reducere P7 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* INT: reducere P7 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* LONG: reducere P7 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* FLOAT_KW: reducere P7 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* DOUBLE: reducere P7 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* SIGNED: reducere P7 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* UNSIGNED: reducere P7 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* TYPEDEF: reducere P7 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* EXTERN: reducere P7 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* STATIC: reducere P7 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* AUTO: reducere P7 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* REGISTER: reducere P7 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* CONST: reducere P7 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* VOLATILE: reducere P7 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* STRUCT: reducere P7 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* UNION: reducere P7 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* ENUM: reducere P7 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* STAR: reducere P7 */
    {  64, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* COLON: reducere P7 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* COMMA: reducere P7 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* PAREN_APERTA: reducere P7 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* PAREN_CLAUSA: reducere P7 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* QUADRA_APERTA: reducere P7 */
    /* --- status 9 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* IDENTIFICATOR: reducere P8 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* VOID: reducere P8 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* CHAR: reducere P8 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* SHORT: reducere P8 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* INT: reducere P8 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* LONG: reducere P8 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* FLOAT_KW: reducere P8 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* DOUBLE: reducere P8 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* SIGNED: reducere P8 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* UNSIGNED: reducere P8 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* TYPEDEF: reducere P8 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* EXTERN: reducere P8 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* STATIC: reducere P8 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* AUTO: reducere P8 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* REGISTER: reducere P8 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* CONST: reducere P8 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* VOLATILE: reducere P8 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* STRUCT: reducere P8 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* UNION: reducere P8 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* ENUM: reducere P8 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* STAR: reducere P8 */
    {  64, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* COLON: reducere P8 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* COMMA: reducere P8 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* PAREN_APERTA: reducere P8 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* PAREN_CLAUSA: reducere P8 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* QUADRA_APERTA: reducere P8 */
    /* --- status 10 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* IDENTIFICATOR: reducere P9 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* VOID: reducere P9 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* CHAR: reducere P9 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* SHORT: reducere P9 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* INT: reducere P9 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* LONG: reducere P9 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* FLOAT_KW: reducere P9 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* DOUBLE: reducere P9 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* SIGNED: reducere P9 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* UNSIGNED: reducere P9 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* TYPEDEF: reducere P9 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* EXTERN: reducere P9 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* STATIC: reducere P9 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* AUTO: reducere P9 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* REGISTER: reducere P9 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* CONST: reducere P9 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* VOLATILE: reducere P9 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* STRUCT: reducere P9 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* UNION: reducere P9 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* ENUM: reducere P9 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* STAR: reducere P9 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* COMMA: reducere P9 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* PAREN_APERTA: reducere P9 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* PAREN_CLAUSA: reducere P9 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* QUADRA_APERTA: reducere P9 */
    /* --- status 11 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* IDENTIFICATOR: reducere P10 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* VOID: reducere P10 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* CHAR: reducere P10 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* SHORT: reducere P10 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* INT: reducere P10 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* LONG: reducere P10 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* FLOAT_KW: reducere P10 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* DOUBLE: reducere P10 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* SIGNED: reducere P10 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* UNSIGNED: reducere P10 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* TYPEDEF: reducere P10 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* EXTERN: reducere P10 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* STATIC: reducere P10 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* AUTO: reducere P10 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* REGISTER: reducere P10 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* CONST: reducere P10 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* VOLATILE: reducere P10 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* STRUCT: reducere P10 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* UNION: reducere P10 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* ENUM: reducere P10 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* STAR: reducere P10 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* COMMA: reducere P10 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* PAREN_APERTA: reducere P10 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* PAREN_CLAUSA: reducere P10 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* QUADRA_APERTA: reducere P10 */
    /* --- status 12 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* IDENTIFICATOR: reducere P11 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* VOID: reducere P11 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* CHAR: reducere P11 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* SHORT: reducere P11 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* INT: reducere P11 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* LONG: reducere P11 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* FLOAT_KW: reducere P11 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* DOUBLE: reducere P11 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* SIGNED: reducere P11 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* UNSIGNED: reducere P11 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* TYPEDEF: reducere P11 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* EXTERN: reducere P11 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* STATIC: reducere P11 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* AUTO: reducere P11 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* REGISTER: reducere P11 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* CONST: reducere P11 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* VOLATILE: reducere P11 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* STRUCT: reducere P11 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* UNION: reducere P11 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* ENUM: reducere P11 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* STAR: reducere P11 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* COMMA: reducere P11 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* PAREN_APERTA: reducere P11 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* PAREN_CLAUSA: reducere P11 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* QUADRA_APERTA: reducere P11 */
    /* --- status 13 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* IDENTIFICATOR: reducere P12 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* VOID: reducere P12 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* CHAR: reducere P12 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* SHORT: reducere P12 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* INT: reducere P12 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* LONG: reducere P12 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* FLOAT_KW: reducere P12 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* DOUBLE: reducere P12 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* SIGNED: reducere P12 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* UNSIGNED: reducere P12 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* TYPEDEF: reducere P12 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* EXTERN: reducere P12 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* STATIC: reducere P12 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* AUTO: reducere P12 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* REGISTER: reducere P12 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* CONST: reducere P12 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* VOLATILE: reducere P12 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* STRUCT: reducere P12 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* UNION: reducere P12 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* ENUM: reducere P12 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* STAR: reducere P12 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* COMMA: reducere P12 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* PAREN_APERTA: reducere P12 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* PAREN_CLAUSA: reducere P12 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* QUADRA_APERTA: reducere P12 */
    /* --- status 14 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* IDENTIFICATOR: reducere P13 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* VOID: reducere P13 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* CHAR: reducere P13 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* SHORT: reducere P13 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* INT: reducere P13 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* LONG: reducere P13 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* FLOAT_KW: reducere P13 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* DOUBLE: reducere P13 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* SIGNED: reducere P13 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* UNSIGNED: reducere P13 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* TYPEDEF: reducere P13 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* EXTERN: reducere P13 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* STATIC: reducere P13 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* AUTO: reducere P13 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* REGISTER: reducere P13 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* CONST: reducere P13 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* VOLATILE: reducere P13 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* STRUCT: reducere P13 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* UNION: reducere P13 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* ENUM: reducere P13 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* STAR: reducere P13 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* COMMA: reducere P13 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* PAREN_APERTA: reducere P13 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* PAREN_CLAUSA: reducere P13 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* QUADRA_APERTA: reducere P13 */
    /* --- status 15 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* IDENTIFICATOR: reducere P14 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* VOID: reducere P14 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* CHAR: reducere P14 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* SHORT: reducere P14 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* INT: reducere P14 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* LONG: reducere P14 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* FLOAT_KW: reducere P14 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* DOUBLE: reducere P14 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* SIGNED: reducere P14 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* UNSIGNED: reducere P14 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* TYPEDEF: reducere P14 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* EXTERN: reducere P14 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* STATIC: reducere P14 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* AUTO: reducere P14 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* REGISTER: reducere P14 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* CONST: reducere P14 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* VOLATILE: reducere P14 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* STRUCT: reducere P14 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* UNION: reducere P14 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* ENUM: reducere P14 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* STAR: reducere P14 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* COMMA: reducere P14 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PAREN_APERTA: reducere P14 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PAREN_CLAUSA: reducere P14 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* QUADRA_APERTA: reducere P14 */
    /* --- status 16 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* IDENTIFICATOR: reducere P15 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* STAR: reducere P15 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* COMMA: reducere P15 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PAREN_APERTA: reducere P15 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PAREN_CLAUSA: reducere P15 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* QUADRA_APERTA: reducere P15 */
    /* --- status 17 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* IDENTIFICATOR: reducere P16 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* STAR: reducere P16 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* COMMA: reducere P16 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* PAREN_APERTA: reducere P16 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* PAREN_CLAUSA: reducere P16 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* QUADRA_APERTA: reducere P16 */
    /* --- status 18 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* IDENTIFICATOR: reducere P18 sd-pp-primum */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* TYPEDEF: reducere P18 sd-pp-primum */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* EXTERN: reducere P18 sd-pp-primum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* STATIC: reducere P18 sd-pp-primum */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* AUTO: reducere P18 sd-pp-primum */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* REGISTER: reducere P18 sd-pp-primum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* CONST: reducere P18 sd-pp-primum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* VOLATILE: reducere P18 sd-pp-primum */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* STRUCT: reducere P18 sd-pp-primum */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* UNION: reducere P18 sd-pp-primum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* ENUM: reducere P18 sd-pp-primum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* STAR: reducere P18 sd-pp-primum */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* COMMA: reducere P18 sd-pp-primum */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* PAREN_APERTA: reducere P18 sd-pp-primum */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* PAREN_CLAUSA: reducere P18 sd-pp-primum */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  18, 0 },  /* QUADRA_APERTA: reducere P18 sd-pp-primum */
    /* --- status 19 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* IDENTIFICATOR: reducere P21 sd-pa-primum */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* VOID: reducere P21 sd-pa-primum */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* CHAR: reducere P21 sd-pa-primum */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* SHORT: reducere P21 sd-pa-primum */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* INT: reducere P21 sd-pa-primum */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* LONG: reducere P21 sd-pa-primum */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* FLOAT_KW: reducere P21 sd-pa-primum */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* DOUBLE: reducere P21 sd-pa-primum */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* SIGNED: reducere P21 sd-pa-primum */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* UNSIGNED: reducere P21 sd-pa-primum */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* TYPEDEF: reducere P21 sd-pa-primum */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* EXTERN: reducere P21 sd-pa-primum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* STATIC: reducere P21 sd-pa-primum */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* AUTO: reducere P21 sd-pa-primum */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* REGISTER: reducere P21 sd-pa-primum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* CONST: reducere P21 sd-pa-primum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* VOLATILE: reducere P21 sd-pa-primum */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* STRUCT: reducere P21 sd-pa-primum */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* UNION: reducere P21 sd-pa-primum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* ENUM: reducere P21 sd-pa-primum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* STAR: reducere P21 sd-pa-primum */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* COMMA: reducere P21 sd-pa-primum */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* PAREN_APERTA: reducere P21 sd-pa-primum */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* PAREN_CLAUSA: reducere P21 sd-pa-primum */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  21, 0 },  /* QUADRA_APERTA: reducere P21 sd-pa-primum */
    /* --- status 20 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* IDENTIFICATOR: reducere P25 sd-pt-primum */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* VOID: reducere P25 sd-pt-primum */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* CHAR: reducere P25 sd-pt-primum */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* SHORT: reducere P25 sd-pt-primum */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* INT: reducere P25 sd-pt-primum */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* LONG: reducere P25 sd-pt-primum */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* FLOAT_KW: reducere P25 sd-pt-primum */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* DOUBLE: reducere P25 sd-pt-primum */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* SIGNED: reducere P25 sd-pt-primum */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* UNSIGNED: reducere P25 sd-pt-primum */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* TYPEDEF: reducere P25 sd-pt-primum */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* EXTERN: reducere P25 sd-pt-primum */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* STATIC: reducere P25 sd-pt-primum */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* AUTO: reducere P25 sd-pt-primum */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* REGISTER: reducere P25 sd-pt-primum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* CONST: reducere P25 sd-pt-primum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* VOLATILE: reducere P25 sd-pt-primum */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* STRUCT: reducere P25 sd-pt-primum */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* UNION: reducere P25 sd-pt-primum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* ENUM: reducere P25 sd-pt-primum */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* STAR: reducere P25 sd-pt-primum */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* COMMA: reducere P25 sd-pt-primum */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* SEMICOLON: reducere P25 sd-pt-primum */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* PAREN_APERTA: reducere P25 sd-pt-primum */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* PAREN_CLAUSA: reducere P25 sd-pt-primum */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  25, 0 },  /* QUADRA_APERTA: reducere P25 sd-pt-primum */
    /* --- status 21 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* IDENTIFICATOR: reducere P29 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* VOID: reducere P29 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* CHAR: reducere P29 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* SHORT: reducere P29 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* INT: reducere P29 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* LONG: reducere P29 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* FLOAT_KW: reducere P29 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* DOUBLE: reducere P29 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* SIGNED: reducere P29 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* UNSIGNED: reducere P29 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* TYPEDEF: reducere P29 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* EXTERN: reducere P29 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* STATIC: reducere P29 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* AUTO: reducere P29 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* REGISTER: reducere P29 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* CONST: reducere P29 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* VOLATILE: reducere P29 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* STRUCT: reducere P29 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* UNION: reducere P29 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* ENUM: reducere P29 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* STAR: reducere P29 */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* COLON: reducere P29 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* COMMA: reducere P29 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* SEMICOLON: reducere P29 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* PAREN_APERTA: reducere P29 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* PAREN_CLAUSA: reducere P29 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  29, 0 },  /* QUADRA_APERTA: reducere P29 */
    /* --- status 22 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* IDENTIFICATOR: reducere P30 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* VOID: reducere P30 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* CHAR: reducere P30 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* SHORT: reducere P30 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* INT: reducere P30 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* LONG: reducere P30 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* FLOAT_KW: reducere P30 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* DOUBLE: reducere P30 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* SIGNED: reducere P30 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* UNSIGNED: reducere P30 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* TYPEDEF: reducere P30 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* EXTERN: reducere P30 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* STATIC: reducere P30 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* AUTO: reducere P30 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* REGISTER: reducere P30 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* CONST: reducere P30 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* VOLATILE: reducere P30 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* STRUCT: reducere P30 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* UNION: reducere P30 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* ENUM: reducere P30 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* STAR: reducere P30 */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* COLON: reducere P30 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* COMMA: reducere P30 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* SEMICOLON: reducere P30 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* PAREN_APERTA: reducere P30 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* PAREN_CLAUSA: reducere P30 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  30, 0 },  /* QUADRA_APERTA: reducere P30 */
    /* --- status 23 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* IDENTIFICATOR: reducere P31 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* VOID: reducere P31 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* CHAR: reducere P31 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* SHORT: reducere P31 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* INT: reducere P31 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* LONG: reducere P31 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* FLOAT_KW: reducere P31 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* DOUBLE: reducere P31 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* SIGNED: reducere P31 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* UNSIGNED: reducere P31 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* TYPEDEF: reducere P31 */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* EXTERN: reducere P31 */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* STATIC: reducere P31 */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* AUTO: reducere P31 */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* REGISTER: reducere P31 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* CONST: reducere P31 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* VOLATILE: reducere P31 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* STRUCT: reducere P31 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* UNION: reducere P31 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* ENUM: reducere P31 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* STAR: reducere P31 */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* COLON: reducere P31 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* COMMA: reducere P31 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* SEMICOLON: reducere P31 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* PAREN_APERTA: reducere P31 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* PAREN_CLAUSA: reducere P31 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  31, 0 },  /* QUADRA_APERTA: reducere P31 */
    /* --- status 24 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* IDENTIFICATOR: reducere P32 typus-primitivus */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* TYPEDEF: reducere P32 typus-primitivus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* EXTERN: reducere P32 typus-primitivus */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* STATIC: reducere P32 typus-primitivus */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* AUTO: reducere P32 typus-primitivus */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* REGISTER: reducere P32 typus-primitivus */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* CONST: reducere P32 typus-primitivus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* VOLATILE: reducere P32 typus-primitivus */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* STRUCT: reducere P32 typus-primitivus */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* UNION: reducere P32 typus-primitivus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* ENUM: reducere P32 typus-primitivus */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* STAR: reducere P32 typus-primitivus */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* COLON: reducere P32 typus-primitivus */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* COMMA: reducere P32 typus-primitivus */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* PAREN_APERTA: reducere P32 typus-primitivus */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* PAREN_CLAUSA: reducere P32 typus-primitivus */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  32, 0 },  /* QUADRA_APERTA: reducere P32 typus-primitivus */
    /* --- status 25 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  89, 0 },  /* IDENTIFICATOR: transponere -> 89 */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE,  90, 0 },  /* BRACE_APERTA: transponere -> 90 */
    /* --- status 26 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  91, 0 },  /* IDENTIFICATOR: transponere -> 91 */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE,  92, 0 },  /* BRACE_APERTA: transponere -> 92 */
    /* --- status 27 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  93, 0 },  /* IDENTIFICATOR: transponere -> 93 */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE,  94, 0 },  /* BRACE_APERTA: transponere -> 94 */
    /* --- status 28 --- */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE,  96, 0 },  /* COMMA: transponere -> 96 */
    {  66, SILVA_TAB_ACTIO_TRANSPONERE,  95, 0 },  /* SEMICOLON: transponere -> 95 */
    /* --- status 29 --- */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* COLON: reducere P84 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* COMMA: reducere P84 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* SEMICOLON: reducere P84 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* PAREN_CLAUSA: reducere P84 */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  84, 0 },  /* QUADRA_CLAUSA: reducere P84 */
    /* --- status 30 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* BRACE_CLAUSA: reducere P128 */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* PLUS: reducere P128 */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MINUS: reducere P128 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* STAR: reducere P128 */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* SOLIDUS: reducere P128 */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* PERCENTUM: reducere P128 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* AMPERSAND: reducere P128 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* BARRA: reducere P128 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* CARET: reducere P128 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* ET_ET: reducere P128 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* VEL_VEL: reducere P128 */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* AEQUALIS_AEQUALIS: reducere P128 */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* NON_AEQUALIS: reducere P128 */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MINOR: reducere P128 */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MAIOR: reducere P128 */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MINOR_AEQUALIS: reducere P128 */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MAIOR_AEQUALIS: reducere P128 */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* SINISTRORSUM: reducere P128 */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* DEXTRORSUM: reducere P128 */
    {  48, SILVA_TAB_ACTIO_TRANSPONERE,  97, 0 },  /* ASSIGNATIO: transponere -> 97 */
    {  49, SILVA_TAB_ACTIO_TRANSPONERE,  98, 0 },  /* PLUS_ASSIGNATIO: transponere -> 98 */
    {  50, SILVA_TAB_ACTIO_TRANSPONERE,  99, 0 },  /* MINUS_ASSIGNATIO: transponere -> 99 */
    {  51, SILVA_TAB_ACTIO_TRANSPONERE, 100, 0 },  /* STAR_ASSIGNATIO: transponere -> 100 */
    {  52, SILVA_TAB_ACTIO_TRANSPONERE, 101, 0 },  /* SOLIDUS_ASSIGNATIO: transponere -> 101 */
    {  53, SILVA_TAB_ACTIO_TRANSPONERE, 102, 0 },  /* PERCENTUM_ASSIGNATIO: transponere -> 102 */
    {  54, SILVA_TAB_ACTIO_TRANSPONERE, 103, 0 },  /* AMPERSAND_ASSIGNATIO: transponere -> 103 */
    {  55, SILVA_TAB_ACTIO_TRANSPONERE, 104, 0 },  /* BARRA_ASSIGNATIO: transponere -> 104 */
    {  56, SILVA_TAB_ACTIO_TRANSPONERE, 105, 0 },  /* CARET_ASSIGNATIO: transponere -> 105 */
    {  57, SILVA_TAB_ACTIO_TRANSPONERE, 106, 0 },  /* SINISTRORSUM_ASSIGNATIO: transponere -> 106 */
    {  58, SILVA_TAB_ACTIO_TRANSPONERE, 107, 0 },  /* DEXTRORSUM_ASSIGNATIO: transponere -> 107 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* QUAESTIO: reducere P128 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* COLON: reducere P128 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* COMMA: reducere P128 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* SEMICOLON: reducere P128 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* PAREN_CLAUSA: reducere P128 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* QUADRA_CLAUSA: reducere P128 */
    /* --- status 31 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* BRACE_CLAUSA: reducere P96 */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* COLON: reducere P96 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* COMMA: reducere P96 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* SEMICOLON: reducere P96 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* PAREN_CLAUSA: reducere P96 */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  96, 0 },  /* QUADRA_CLAUSA: reducere P96 */
    /* --- status 32 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  98, 0 },  /* BRACE_CLAUSA: reducere P98 */
    {  39, SILVA_TAB_ACTIO_TRANSPONERE, 109, 0 },  /* VEL_VEL: transponere -> 109 */
    {  63, SILVA_TAB_ACTIO_TRANSPONERE, 108, 0 },  /* QUAESTIO: transponere -> 108 */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  98, 0 },  /* COLON: reducere P98 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  98, 0 },  /* COMMA: reducere P98 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  98, 0 },  /* SEMICOLON: reducere P98 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  98, 0 },  /* PAREN_CLAUSA: reducere P98 */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  98, 0 },  /* QUADRA_CLAUSA: reducere P98 */
    /* --- status 33 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 100, 0 },  /* BRACE_CLAUSA: reducere P100 */
    {  38, SILVA_TAB_ACTIO_TRANSPONERE, 110, 0 },  /* ET_ET: transponere -> 110 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 100, 0 },  /* VEL_VEL: reducere P100 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 100, 0 },  /* QUAESTIO: reducere P100 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 100, 0 },  /* COLON: reducere P100 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 100, 0 },  /* COMMA: reducere P100 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 100, 0 },  /* SEMICOLON: reducere P100 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 100, 0 },  /* PAREN_CLAUSA: reducere P100 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 100, 0 },  /* QUADRA_CLAUSA: reducere P100 */
    /* --- status 34 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 102, 0 },  /* BRACE_CLAUSA: reducere P102 */
    {  34, SILVA_TAB_ACTIO_TRANSPONERE, 111, 0 },  /* BARRA: transponere -> 111 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 102, 0 },  /* ET_ET: reducere P102 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 102, 0 },  /* VEL_VEL: reducere P102 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 102, 0 },  /* QUAESTIO: reducere P102 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 102, 0 },  /* COLON: reducere P102 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 102, 0 },  /* COMMA: reducere P102 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 102, 0 },  /* SEMICOLON: reducere P102 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 102, 0 },  /* PAREN_CLAUSA: reducere P102 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 102, 0 },  /* QUADRA_CLAUSA: reducere P102 */
    /* --- status 35 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* BRACE_CLAUSA: reducere P104 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* BARRA: reducere P104 */
    {  35, SILVA_TAB_ACTIO_TRANSPONERE, 112, 0 },  /* CARET: transponere -> 112 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* ET_ET: reducere P104 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* VEL_VEL: reducere P104 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* QUAESTIO: reducere P104 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* COLON: reducere P104 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* COMMA: reducere P104 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* SEMICOLON: reducere P104 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* PAREN_CLAUSA: reducere P104 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 104, 0 },  /* QUADRA_CLAUSA: reducere P104 */
    /* --- status 36 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* BRACE_CLAUSA: reducere P106 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE, 113, 0 },  /* AMPERSAND: transponere -> 113 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* BARRA: reducere P106 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* CARET: reducere P106 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* ET_ET: reducere P106 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* VEL_VEL: reducere P106 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* QUAESTIO: reducere P106 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* COLON: reducere P106 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* COMMA: reducere P106 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* SEMICOLON: reducere P106 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* PAREN_CLAUSA: reducere P106 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 106, 0 },  /* QUADRA_CLAUSA: reducere P106 */
    /* --- status 37 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* BRACE_CLAUSA: reducere P108 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* AMPERSAND: reducere P108 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* BARRA: reducere P108 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* CARET: reducere P108 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* ET_ET: reducere P108 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* VEL_VEL: reducere P108 */
    {  40, SILVA_TAB_ACTIO_TRANSPONERE, 114, 0 },  /* AEQUALIS_AEQUALIS: transponere -> 114 */
    {  41, SILVA_TAB_ACTIO_TRANSPONERE, 115, 0 },  /* NON_AEQUALIS: transponere -> 115 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* QUAESTIO: reducere P108 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* COLON: reducere P108 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* COMMA: reducere P108 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* SEMICOLON: reducere P108 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* PAREN_CLAUSA: reducere P108 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 108, 0 },  /* QUADRA_CLAUSA: reducere P108 */
    /* --- status 38 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* BRACE_CLAUSA: reducere P111 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* AMPERSAND: reducere P111 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* BARRA: reducere P111 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* CARET: reducere P111 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* ET_ET: reducere P111 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* VEL_VEL: reducere P111 */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* AEQUALIS_AEQUALIS: reducere P111 */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* NON_AEQUALIS: reducere P111 */
    {  42, SILVA_TAB_ACTIO_TRANSPONERE, 116, 0 },  /* MINOR: transponere -> 116 */
    {  43, SILVA_TAB_ACTIO_TRANSPONERE, 117, 0 },  /* MAIOR: transponere -> 117 */
    {  44, SILVA_TAB_ACTIO_TRANSPONERE, 118, 0 },  /* MINOR_AEQUALIS: transponere -> 118 */
    {  45, SILVA_TAB_ACTIO_TRANSPONERE, 119, 0 },  /* MAIOR_AEQUALIS: transponere -> 119 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* QUAESTIO: reducere P111 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* COLON: reducere P111 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* COMMA: reducere P111 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* SEMICOLON: reducere P111 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* PAREN_CLAUSA: reducere P111 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 111, 0 },  /* QUADRA_CLAUSA: reducere P111 */
    /* --- status 39 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* BRACE_CLAUSA: reducere P116 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* AMPERSAND: reducere P116 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* BARRA: reducere P116 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* CARET: reducere P116 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* ET_ET: reducere P116 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* VEL_VEL: reducere P116 */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* AEQUALIS_AEQUALIS: reducere P116 */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* NON_AEQUALIS: reducere P116 */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* MINOR: reducere P116 */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* MAIOR: reducere P116 */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* MINOR_AEQUALIS: reducere P116 */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* MAIOR_AEQUALIS: reducere P116 */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE, 120, 0 },  /* SINISTRORSUM: transponere -> 120 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE, 121, 0 },  /* DEXTRORSUM: transponere -> 121 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* QUAESTIO: reducere P116 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* COLON: reducere P116 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* COMMA: reducere P116 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* SEMICOLON: reducere P116 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* PAREN_CLAUSA: reducere P116 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 116, 0 },  /* QUADRA_CLAUSA: reducere P116 */
    /* --- status 40 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* BRACE_CLAUSA: reducere P119 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE, 122, 0 },  /* PLUS: transponere -> 122 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE, 123, 0 },  /* MINUS: transponere -> 123 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* AMPERSAND: reducere P119 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* BARRA: reducere P119 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* CARET: reducere P119 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* ET_ET: reducere P119 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* VEL_VEL: reducere P119 */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* AEQUALIS_AEQUALIS: reducere P119 */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* NON_AEQUALIS: reducere P119 */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* MINOR: reducere P119 */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* MAIOR: reducere P119 */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* MINOR_AEQUALIS: reducere P119 */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* MAIOR_AEQUALIS: reducere P119 */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* SINISTRORSUM: reducere P119 */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* DEXTRORSUM: reducere P119 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* QUAESTIO: reducere P119 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* COLON: reducere P119 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* COMMA: reducere P119 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* SEMICOLON: reducere P119 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* PAREN_CLAUSA: reducere P119 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 119, 0 },  /* QUADRA_CLAUSA: reducere P119 */
    /* --- status 41 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* BRACE_CLAUSA: reducere P122 */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* PLUS: reducere P122 */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* MINUS: reducere P122 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE, 124, 0 },  /* STAR: transponere -> 124 */
    {  31, SILVA_TAB_ACTIO_TRANSPONERE, 125, 0 },  /* SOLIDUS: transponere -> 125 */
    {  32, SILVA_TAB_ACTIO_TRANSPONERE, 126, 0 },  /* PERCENTUM: transponere -> 126 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* AMPERSAND: reducere P122 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* BARRA: reducere P122 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* CARET: reducere P122 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* ET_ET: reducere P122 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* VEL_VEL: reducere P122 */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* AEQUALIS_AEQUALIS: reducere P122 */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* NON_AEQUALIS: reducere P122 */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* MINOR: reducere P122 */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* MAIOR: reducere P122 */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* MINOR_AEQUALIS: reducere P122 */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* MAIOR_AEQUALIS: reducere P122 */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* SINISTRORSUM: reducere P122 */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* DEXTRORSUM: reducere P122 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* QUAESTIO: reducere P122 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* COLON: reducere P122 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* COMMA: reducere P122 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* SEMICOLON: reducere P122 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* PAREN_CLAUSA: reducere P122 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 122, 0 },  /* QUADRA_CLAUSA: reducere P122 */
    /* --- status 42 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* BRACE_CLAUSA: reducere P126 */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* PLUS: reducere P126 */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* MINUS: reducere P126 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* STAR: reducere P126 */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* SOLIDUS: reducere P126 */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* PERCENTUM: reducere P126 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* AMPERSAND: reducere P126 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* BARRA: reducere P126 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* CARET: reducere P126 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* ET_ET: reducere P126 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* VEL_VEL: reducere P126 */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* AEQUALIS_AEQUALIS: reducere P126 */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* NON_AEQUALIS: reducere P126 */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* MINOR: reducere P126 */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* MAIOR: reducere P126 */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* MINOR_AEQUALIS: reducere P126 */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* MAIOR_AEQUALIS: reducere P126 */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* SINISTRORSUM: reducere P126 */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* DEXTRORSUM: reducere P126 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* QUAESTIO: reducere P126 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* COLON: reducere P126 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* COMMA: reducere P126 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* SEMICOLON: reducere P126 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* PAREN_CLAUSA: reducere P126 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 126, 0 },  /* QUADRA_CLAUSA: reducere P126 */
    /* --- status 43 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,   7, 0 },  /* IDENTIFICATOR: transponere -> 7 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 44 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 138, 0 },  /* PAREN_APERTA: transponere -> 138 */
    /* --- status 45 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 138, 0 },  /* PAREN_APERTA: transponere -> 138 */
    /* --- status 46 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 47 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 48 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 49 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 50 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 51 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 52 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 148, 0 },  /* PAREN_APERTA: transponere -> 148 */
    /* --- status 53 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* BRACE_CLAUSA: reducere P139 */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* PLUS: reducere P139 */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* MINUS: reducere P139 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* STAR: reducere P139 */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* SOLIDUS: reducere P139 */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* PERCENTUM: reducere P139 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* AMPERSAND: reducere P139 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* BARRA: reducere P139 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* CARET: reducere P139 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* ET_ET: reducere P139 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* VEL_VEL: reducere P139 */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* AEQUALIS_AEQUALIS: reducere P139 */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* NON_AEQUALIS: reducere P139 */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* MINOR: reducere P139 */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* MAIOR: reducere P139 */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* MINOR_AEQUALIS: reducere P139 */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* MAIOR_AEQUALIS: reducere P139 */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* SINISTRORSUM: reducere P139 */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* DEXTRORSUM: reducere P139 */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* ASSIGNATIO: reducere P139 */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* PLUS_ASSIGNATIO: reducere P139 */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* MINUS_ASSIGNATIO: reducere P139 */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* STAR_ASSIGNATIO: reducere P139 */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P139 */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P139 */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P139 */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* BARRA_ASSIGNATIO: reducere P139 */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* CARET_ASSIGNATIO: reducere P139 */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P139 */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P139 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE, 153, 0 },  /* INCREMENTUM: transponere -> 153 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE, 154, 0 },  /* DECREMENTUM: transponere -> 154 */
    {  61, SILVA_TAB_ACTIO_TRANSPONERE, 151, 0 },  /* PUNCTUM: transponere -> 151 */
    {  62, SILVA_TAB_ACTIO_TRANSPONERE, 152, 0 },  /* SAGITTA: transponere -> 152 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* QUAESTIO: reducere P139 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* COLON: reducere P139 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* COMMA: reducere P139 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* SEMICOLON: reducere P139 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 150, 0 },  /* PAREN_APERTA: transponere -> 150 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* PAREN_CLAUSA: reducere P139 */
    {  69, SILVA_TAB_ACTIO_TRANSPONERE, 149, 0 },  /* QUADRA_APERTA: transponere -> 149 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 139, 0 },  /* QUADRA_CLAUSA: reducere P139 */
    /* --- status 54 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* IDENTIFICATOR: reducere P156 */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* VOID: reducere P156 */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* CHAR: reducere P156 */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* SHORT: reducere P156 */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* INT: reducere P156 */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* LONG: reducere P156 */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* FLOAT_KW: reducere P156 */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* DOUBLE: reducere P156 */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* SIGNED: reducere P156 */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* UNSIGNED: reducere P156 */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* TYPEDEF: reducere P156 */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* EXTERN: reducere P156 */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* STATIC: reducere P156 */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* AUTO: reducere P156 */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* REGISTER: reducere P156 */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* CONST: reducere P156 */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* VOLATILE: reducere P156 */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* STRUCT: reducere P156 */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* UNION: reducere P156 */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* ENUM: reducere P156 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* STAR: reducere P156 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* COLON: reducere P156 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* COMMA: reducere P156 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* PAREN_APERTA: reducere P156 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* PAREN_CLAUSA: reducere P156 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 156, 0 },  /* QUADRA_APERTA: reducere P156 */
    /* --- status 55 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* IDENTIFICATOR: reducere P157 */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* VOID: reducere P157 */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* CHAR: reducere P157 */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* SHORT: reducere P157 */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* INT: reducere P157 */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* LONG: reducere P157 */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* FLOAT_KW: reducere P157 */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* DOUBLE: reducere P157 */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* SIGNED: reducere P157 */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* UNSIGNED: reducere P157 */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* TYPEDEF: reducere P157 */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* EXTERN: reducere P157 */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* STATIC: reducere P157 */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* AUTO: reducere P157 */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* REGISTER: reducere P157 */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* CONST: reducere P157 */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* VOLATILE: reducere P157 */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* STRUCT: reducere P157 */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* UNION: reducere P157 */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* ENUM: reducere P157 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* STAR: reducere P157 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* COLON: reducere P157 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* COMMA: reducere P157 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* PAREN_APERTA: reducere P157 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* PAREN_CLAUSA: reducere P157 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 157, 0 },  /* QUADRA_APERTA: reducere P157 */
    /* --- status 56 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* IDENTIFICATOR: reducere P158 */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* VOID: reducere P158 */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* CHAR: reducere P158 */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* SHORT: reducere P158 */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* INT: reducere P158 */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* LONG: reducere P158 */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* FLOAT_KW: reducere P158 */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* DOUBLE: reducere P158 */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* SIGNED: reducere P158 */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* UNSIGNED: reducere P158 */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* TYPEDEF: reducere P158 */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* EXTERN: reducere P158 */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* STATIC: reducere P158 */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* AUTO: reducere P158 */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* REGISTER: reducere P158 */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* CONST: reducere P158 */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* VOLATILE: reducere P158 */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* STRUCT: reducere P158 */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* UNION: reducere P158 */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* ENUM: reducere P158 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* STAR: reducere P158 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* COLON: reducere P158 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* COMMA: reducere P158 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* PAREN_APERTA: reducere P158 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* PAREN_CLAUSA: reducere P158 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 158, 0 },  /* QUADRA_APERTA: reducere P158 */
    /* --- status 57 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* IDENTIFICATOR: reducere P159 */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* VOID: reducere P159 */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* CHAR: reducere P159 */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* SHORT: reducere P159 */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* INT: reducere P159 */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* LONG: reducere P159 */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* FLOAT_KW: reducere P159 */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* DOUBLE: reducere P159 */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* SIGNED: reducere P159 */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* UNSIGNED: reducere P159 */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* TYPEDEF: reducere P159 */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* EXTERN: reducere P159 */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* STATIC: reducere P159 */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* AUTO: reducere P159 */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* REGISTER: reducere P159 */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* CONST: reducere P159 */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* VOLATILE: reducere P159 */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* STRUCT: reducere P159 */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* UNION: reducere P159 */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* ENUM: reducere P159 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* STAR: reducere P159 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* COLON: reducere P159 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* COMMA: reducere P159 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* PAREN_APERTA: reducere P159 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* PAREN_CLAUSA: reducere P159 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 159, 0 },  /* QUADRA_APERTA: reducere P159 */
    /* --- status 58 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* IDENTIFICATOR: reducere P160 */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* VOID: reducere P160 */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* CHAR: reducere P160 */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* SHORT: reducere P160 */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* INT: reducere P160 */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* LONG: reducere P160 */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* FLOAT_KW: reducere P160 */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* DOUBLE: reducere P160 */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* SIGNED: reducere P160 */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* UNSIGNED: reducere P160 */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* TYPEDEF: reducere P160 */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* EXTERN: reducere P160 */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* STATIC: reducere P160 */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* AUTO: reducere P160 */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* REGISTER: reducere P160 */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* CONST: reducere P160 */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* VOLATILE: reducere P160 */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* STRUCT: reducere P160 */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* UNION: reducere P160 */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* ENUM: reducere P160 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* STAR: reducere P160 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* COLON: reducere P160 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* COMMA: reducere P160 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* PAREN_APERTA: reducere P160 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* PAREN_CLAUSA: reducere P160 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 160, 0 },  /* QUADRA_APERTA: reducere P160 */
    /* --- status 59 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* IDENTIFICATOR: reducere P161 */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* VOID: reducere P161 */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* CHAR: reducere P161 */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* SHORT: reducere P161 */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* INT: reducere P161 */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* LONG: reducere P161 */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* FLOAT_KW: reducere P161 */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* DOUBLE: reducere P161 */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* SIGNED: reducere P161 */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* UNSIGNED: reducere P161 */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* TYPEDEF: reducere P161 */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* EXTERN: reducere P161 */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* STATIC: reducere P161 */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* AUTO: reducere P161 */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* REGISTER: reducere P161 */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* CONST: reducere P161 */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* VOLATILE: reducere P161 */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* STRUCT: reducere P161 */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* UNION: reducere P161 */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* ENUM: reducere P161 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* STAR: reducere P161 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* COLON: reducere P161 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* COMMA: reducere P161 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* PAREN_APERTA: reducere P161 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* PAREN_CLAUSA: reducere P161 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 161, 0 },  /* QUADRA_APERTA: reducere P161 */
    /* --- status 60 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* IDENTIFICATOR: reducere P162 */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* VOID: reducere P162 */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* CHAR: reducere P162 */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* SHORT: reducere P162 */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* INT: reducere P162 */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* LONG: reducere P162 */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* FLOAT_KW: reducere P162 */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* DOUBLE: reducere P162 */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* SIGNED: reducere P162 */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* UNSIGNED: reducere P162 */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* TYPEDEF: reducere P162 */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* EXTERN: reducere P162 */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* STATIC: reducere P162 */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* AUTO: reducere P162 */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* REGISTER: reducere P162 */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* CONST: reducere P162 */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* VOLATILE: reducere P162 */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* STRUCT: reducere P162 */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* UNION: reducere P162 */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* ENUM: reducere P162 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* STAR: reducere P162 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* COLON: reducere P162 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* COMMA: reducere P162 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* PAREN_APERTA: reducere P162 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* PAREN_CLAUSA: reducere P162 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 162, 0 },  /* QUADRA_APERTA: reducere P162 */
    /* --- status 61 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* IDENTIFICATOR: reducere P163 */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* VOID: reducere P163 */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* CHAR: reducere P163 */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* SHORT: reducere P163 */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* INT: reducere P163 */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* LONG: reducere P163 */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* FLOAT_KW: reducere P163 */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* DOUBLE: reducere P163 */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* SIGNED: reducere P163 */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* UNSIGNED: reducere P163 */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* TYPEDEF: reducere P163 */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* EXTERN: reducere P163 */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* STATIC: reducere P163 */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* AUTO: reducere P163 */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* REGISTER: reducere P163 */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* CONST: reducere P163 */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* VOLATILE: reducere P163 */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* STRUCT: reducere P163 */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* UNION: reducere P163 */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* ENUM: reducere P163 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* STAR: reducere P163 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* COLON: reducere P163 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* COMMA: reducere P163 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* PAREN_APERTA: reducere P163 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* PAREN_CLAUSA: reducere P163 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 163, 0 },  /* QUADRA_APERTA: reducere P163 */
    /* --- status 62 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* IDENTIFICATOR: reducere P164 */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* VOID: reducere P164 */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* CHAR: reducere P164 */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* SHORT: reducere P164 */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* INT: reducere P164 */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* LONG: reducere P164 */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* FLOAT_KW: reducere P164 */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* DOUBLE: reducere P164 */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* SIGNED: reducere P164 */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* UNSIGNED: reducere P164 */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* TYPEDEF: reducere P164 */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* EXTERN: reducere P164 */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* STATIC: reducere P164 */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* AUTO: reducere P164 */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* REGISTER: reducere P164 */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* CONST: reducere P164 */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* VOLATILE: reducere P164 */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* STRUCT: reducere P164 */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* UNION: reducere P164 */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* ENUM: reducere P164 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* STAR: reducere P164 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* COLON: reducere P164 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* COMMA: reducere P164 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* PAREN_APERTA: reducere P164 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* PAREN_CLAUSA: reducere P164 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 164, 0 },  /* QUADRA_APERTA: reducere P164 */
    /* --- status 63 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* IDENTIFICATOR: reducere P165 verba-primitiva-primum */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* VOID: reducere P165 verba-primitiva-primum */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* CHAR: reducere P165 verba-primitiva-primum */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* SHORT: reducere P165 verba-primitiva-primum */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* INT: reducere P165 verba-primitiva-primum */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* LONG: reducere P165 verba-primitiva-primum */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* FLOAT_KW: reducere P165 verba-primitiva-primum */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* DOUBLE: reducere P165 verba-primitiva-primum */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* SIGNED: reducere P165 verba-primitiva-primum */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* UNSIGNED: reducere P165 verba-primitiva-primum */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* TYPEDEF: reducere P165 verba-primitiva-primum */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* EXTERN: reducere P165 verba-primitiva-primum */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* STATIC: reducere P165 verba-primitiva-primum */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* AUTO: reducere P165 verba-primitiva-primum */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* REGISTER: reducere P165 verba-primitiva-primum */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* CONST: reducere P165 verba-primitiva-primum */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* VOLATILE: reducere P165 verba-primitiva-primum */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* STRUCT: reducere P165 verba-primitiva-primum */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* UNION: reducere P165 verba-primitiva-primum */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* ENUM: reducere P165 verba-primitiva-primum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* STAR: reducere P165 verba-primitiva-primum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* COLON: reducere P165 verba-primitiva-primum */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* COMMA: reducere P165 verba-primitiva-primum */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* PAREN_APERTA: reducere P165 verba-primitiva-primum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* PAREN_CLAUSA: reducere P165 verba-primitiva-primum */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 165, 0 },  /* QUADRA_APERTA: reducere P165 verba-primitiva-primum */
    /* --- status 64 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* BRACE_CLAUSA: reducere P183 */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* PLUS: reducere P183 */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* MINUS: reducere P183 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* STAR: reducere P183 */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* SOLIDUS: reducere P183 */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* PERCENTUM: reducere P183 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* AMPERSAND: reducere P183 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* BARRA: reducere P183 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* CARET: reducere P183 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* ET_ET: reducere P183 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* VEL_VEL: reducere P183 */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* AEQUALIS_AEQUALIS: reducere P183 */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* NON_AEQUALIS: reducere P183 */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* MINOR: reducere P183 */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* MAIOR: reducere P183 */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* MINOR_AEQUALIS: reducere P183 */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* MAIOR_AEQUALIS: reducere P183 */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* SINISTRORSUM: reducere P183 */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* DEXTRORSUM: reducere P183 */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* ASSIGNATIO: reducere P183 */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* PLUS_ASSIGNATIO: reducere P183 */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* MINUS_ASSIGNATIO: reducere P183 */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* STAR_ASSIGNATIO: reducere P183 */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P183 */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P183 */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P183 */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* BARRA_ASSIGNATIO: reducere P183 */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* CARET_ASSIGNATIO: reducere P183 */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P183 */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P183 */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* INCREMENTUM: reducere P183 */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* DECREMENTUM: reducere P183 */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* PUNCTUM: reducere P183 */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* SAGITTA: reducere P183 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* QUAESTIO: reducere P183 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* COLON: reducere P183 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* COMMA: reducere P183 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* SEMICOLON: reducere P183 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* PAREN_APERTA: reducere P183 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* PAREN_CLAUSA: reducere P183 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* QUADRA_APERTA: reducere P183 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 183, 0 },  /* QUADRA_CLAUSA: reducere P183 */
    /* --- status 65 --- */
    {   4, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* STRING_LIT: reducere P188 chordae-prima */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* BRACE_CLAUSA: reducere P188 chordae-prima */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* PLUS: reducere P188 chordae-prima */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* MINUS: reducere P188 chordae-prima */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* STAR: reducere P188 chordae-prima */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* SOLIDUS: reducere P188 chordae-prima */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* PERCENTUM: reducere P188 chordae-prima */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* AMPERSAND: reducere P188 chordae-prima */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* BARRA: reducere P188 chordae-prima */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* CARET: reducere P188 chordae-prima */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* ET_ET: reducere P188 chordae-prima */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* VEL_VEL: reducere P188 chordae-prima */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* AEQUALIS_AEQUALIS: reducere P188 chordae-prima */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* NON_AEQUALIS: reducere P188 chordae-prima */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* MINOR: reducere P188 chordae-prima */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* MAIOR: reducere P188 chordae-prima */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* MINOR_AEQUALIS: reducere P188 chordae-prima */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* MAIOR_AEQUALIS: reducere P188 chordae-prima */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* SINISTRORSUM: reducere P188 chordae-prima */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* DEXTRORSUM: reducere P188 chordae-prima */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* ASSIGNATIO: reducere P188 chordae-prima */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* PLUS_ASSIGNATIO: reducere P188 chordae-prima */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* MINUS_ASSIGNATIO: reducere P188 chordae-prima */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* STAR_ASSIGNATIO: reducere P188 chordae-prima */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P188 chordae-prima */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P188 chordae-prima */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P188 chordae-prima */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* BARRA_ASSIGNATIO: reducere P188 chordae-prima */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* CARET_ASSIGNATIO: reducere P188 chordae-prima */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P188 chordae-prima */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P188 chordae-prima */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* INCREMENTUM: reducere P188 chordae-prima */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* DECREMENTUM: reducere P188 chordae-prima */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* PUNCTUM: reducere P188 chordae-prima */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* SAGITTA: reducere P188 chordae-prima */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* QUAESTIO: reducere P188 chordae-prima */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* COLON: reducere P188 chordae-prima */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* COMMA: reducere P188 chordae-prima */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* SEMICOLON: reducere P188 chordae-prima */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* PAREN_APERTA: reducere P188 chordae-prima */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* PAREN_CLAUSA: reducere P188 chordae-prima */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* QUADRA_APERTA: reducere P188 chordae-prima */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 188, 0 },  /* QUADRA_CLAUSA: reducere P188 chordae-prima */
    /* --- status 66 --- */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE, 155, 0 },  /* STRING_LIT: transponere -> 155 */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* BRACE_CLAUSA: reducere P194 primarium-chorda */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* PLUS: reducere P194 primarium-chorda */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* MINUS: reducere P194 primarium-chorda */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* STAR: reducere P194 primarium-chorda */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* SOLIDUS: reducere P194 primarium-chorda */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* PERCENTUM: reducere P194 primarium-chorda */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* AMPERSAND: reducere P194 primarium-chorda */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* BARRA: reducere P194 primarium-chorda */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* CARET: reducere P194 primarium-chorda */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* ET_ET: reducere P194 primarium-chorda */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* VEL_VEL: reducere P194 primarium-chorda */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* AEQUALIS_AEQUALIS: reducere P194 primarium-chorda */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* NON_AEQUALIS: reducere P194 primarium-chorda */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* MINOR: reducere P194 primarium-chorda */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* MAIOR: reducere P194 primarium-chorda */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* MINOR_AEQUALIS: reducere P194 primarium-chorda */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* MAIOR_AEQUALIS: reducere P194 primarium-chorda */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* SINISTRORSUM: reducere P194 primarium-chorda */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* DEXTRORSUM: reducere P194 primarium-chorda */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* ASSIGNATIO: reducere P194 primarium-chorda */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* PLUS_ASSIGNATIO: reducere P194 primarium-chorda */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* MINUS_ASSIGNATIO: reducere P194 primarium-chorda */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* STAR_ASSIGNATIO: reducere P194 primarium-chorda */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P194 primarium-chorda */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P194 primarium-chorda */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P194 primarium-chorda */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* BARRA_ASSIGNATIO: reducere P194 primarium-chorda */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* CARET_ASSIGNATIO: reducere P194 primarium-chorda */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P194 primarium-chorda */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P194 primarium-chorda */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* INCREMENTUM: reducere P194 primarium-chorda */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* DECREMENTUM: reducere P194 primarium-chorda */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* PUNCTUM: reducere P194 primarium-chorda */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* SAGITTA: reducere P194 primarium-chorda */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* QUAESTIO: reducere P194 primarium-chorda */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* COLON: reducere P194 primarium-chorda */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* COMMA: reducere P194 primarium-chorda */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* SEMICOLON: reducere P194 primarium-chorda */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* PAREN_APERTA: reducere P194 primarium-chorda */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* PAREN_CLAUSA: reducere P194 primarium-chorda */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* QUADRA_APERTA: reducere P194 primarium-chorda */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 194, 0 },  /* QUADRA_CLAUSA: reducere P194 primarium-chorda */
    /* --- status 67 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* BRACE_CLAUSA: reducere P191 primarium-integer */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* PLUS: reducere P191 primarium-integer */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* MINUS: reducere P191 primarium-integer */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* STAR: reducere P191 primarium-integer */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* SOLIDUS: reducere P191 primarium-integer */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* PERCENTUM: reducere P191 primarium-integer */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* AMPERSAND: reducere P191 primarium-integer */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* BARRA: reducere P191 primarium-integer */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* CARET: reducere P191 primarium-integer */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* ET_ET: reducere P191 primarium-integer */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* VEL_VEL: reducere P191 primarium-integer */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* AEQUALIS_AEQUALIS: reducere P191 primarium-integer */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* NON_AEQUALIS: reducere P191 primarium-integer */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* MINOR: reducere P191 primarium-integer */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* MAIOR: reducere P191 primarium-integer */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* MINOR_AEQUALIS: reducere P191 primarium-integer */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* MAIOR_AEQUALIS: reducere P191 primarium-integer */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* SINISTRORSUM: reducere P191 primarium-integer */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* DEXTRORSUM: reducere P191 primarium-integer */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* ASSIGNATIO: reducere P191 primarium-integer */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* PLUS_ASSIGNATIO: reducere P191 primarium-integer */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* MINUS_ASSIGNATIO: reducere P191 primarium-integer */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* STAR_ASSIGNATIO: reducere P191 primarium-integer */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P191 primarium-integer */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P191 primarium-integer */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P191 primarium-integer */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* BARRA_ASSIGNATIO: reducere P191 primarium-integer */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* CARET_ASSIGNATIO: reducere P191 primarium-integer */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P191 primarium-integer */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P191 primarium-integer */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* INCREMENTUM: reducere P191 primarium-integer */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* DECREMENTUM: reducere P191 primarium-integer */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* PUNCTUM: reducere P191 primarium-integer */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* SAGITTA: reducere P191 primarium-integer */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* QUAESTIO: reducere P191 primarium-integer */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* COLON: reducere P191 primarium-integer */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* COMMA: reducere P191 primarium-integer */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* SEMICOLON: reducere P191 primarium-integer */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* PAREN_APERTA: reducere P191 primarium-integer */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* PAREN_CLAUSA: reducere P191 primarium-integer */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* QUADRA_APERTA: reducere P191 primarium-integer */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 191, 0 },  /* QUADRA_CLAUSA: reducere P191 primarium-integer */
    /* --- status 68 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* BRACE_CLAUSA: reducere P192 primarium-fluitans */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* PLUS: reducere P192 primarium-fluitans */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* MINUS: reducere P192 primarium-fluitans */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* STAR: reducere P192 primarium-fluitans */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* SOLIDUS: reducere P192 primarium-fluitans */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* PERCENTUM: reducere P192 primarium-fluitans */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* AMPERSAND: reducere P192 primarium-fluitans */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* BARRA: reducere P192 primarium-fluitans */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* CARET: reducere P192 primarium-fluitans */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* ET_ET: reducere P192 primarium-fluitans */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* VEL_VEL: reducere P192 primarium-fluitans */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* AEQUALIS_AEQUALIS: reducere P192 primarium-fluitans */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* NON_AEQUALIS: reducere P192 primarium-fluitans */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* MINOR: reducere P192 primarium-fluitans */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* MAIOR: reducere P192 primarium-fluitans */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* MINOR_AEQUALIS: reducere P192 primarium-fluitans */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* MAIOR_AEQUALIS: reducere P192 primarium-fluitans */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* SINISTRORSUM: reducere P192 primarium-fluitans */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* DEXTRORSUM: reducere P192 primarium-fluitans */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* ASSIGNATIO: reducere P192 primarium-fluitans */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* PLUS_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* MINUS_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* STAR_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* BARRA_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* CARET_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P192 primarium-fluitans */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* INCREMENTUM: reducere P192 primarium-fluitans */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* DECREMENTUM: reducere P192 primarium-fluitans */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* PUNCTUM: reducere P192 primarium-fluitans */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* SAGITTA: reducere P192 primarium-fluitans */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* QUAESTIO: reducere P192 primarium-fluitans */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* COLON: reducere P192 primarium-fluitans */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* COMMA: reducere P192 primarium-fluitans */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* SEMICOLON: reducere P192 primarium-fluitans */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* PAREN_APERTA: reducere P192 primarium-fluitans */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* PAREN_CLAUSA: reducere P192 primarium-fluitans */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* QUADRA_APERTA: reducere P192 primarium-fluitans */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 192, 0 },  /* QUADRA_CLAUSA: reducere P192 primarium-fluitans */
    /* --- status 69 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* BRACE_CLAUSA: reducere P193 primarium-character */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* PLUS: reducere P193 primarium-character */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* MINUS: reducere P193 primarium-character */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* STAR: reducere P193 primarium-character */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* SOLIDUS: reducere P193 primarium-character */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* PERCENTUM: reducere P193 primarium-character */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* AMPERSAND: reducere P193 primarium-character */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* BARRA: reducere P193 primarium-character */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* CARET: reducere P193 primarium-character */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* ET_ET: reducere P193 primarium-character */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* VEL_VEL: reducere P193 primarium-character */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* AEQUALIS_AEQUALIS: reducere P193 primarium-character */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* NON_AEQUALIS: reducere P193 primarium-character */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* MINOR: reducere P193 primarium-character */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* MAIOR: reducere P193 primarium-character */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* MINOR_AEQUALIS: reducere P193 primarium-character */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* MAIOR_AEQUALIS: reducere P193 primarium-character */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* SINISTRORSUM: reducere P193 primarium-character */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* DEXTRORSUM: reducere P193 primarium-character */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* ASSIGNATIO: reducere P193 primarium-character */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* PLUS_ASSIGNATIO: reducere P193 primarium-character */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* MINUS_ASSIGNATIO: reducere P193 primarium-character */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* STAR_ASSIGNATIO: reducere P193 primarium-character */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P193 primarium-character */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P193 primarium-character */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P193 primarium-character */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* BARRA_ASSIGNATIO: reducere P193 primarium-character */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* CARET_ASSIGNATIO: reducere P193 primarium-character */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P193 primarium-character */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P193 primarium-character */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* INCREMENTUM: reducere P193 primarium-character */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* DECREMENTUM: reducere P193 primarium-character */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* PUNCTUM: reducere P193 primarium-character */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* SAGITTA: reducere P193 primarium-character */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* QUAESTIO: reducere P193 primarium-character */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* COLON: reducere P193 primarium-character */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* COMMA: reducere P193 primarium-character */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* SEMICOLON: reducere P193 primarium-character */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* PAREN_APERTA: reducere P193 primarium-character */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* PAREN_CLAUSA: reducere P193 primarium-character */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* QUADRA_APERTA: reducere P193 primarium-character */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 193, 0 },  /* QUADRA_CLAUSA: reducere P193 primarium-character */
    /* --- status 70 --- */
    {  -1, SILVA_TAB_ACTIO_ACCIPERE,   0, 0 },  /* $: accipere */
    /* --- status 71 --- */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE, 157, 0 },  /* COMMA: transponere -> 157 */
    {  66, SILVA_TAB_ACTIO_TRANSPONERE, 156, 0 },  /* SEMICOLON: transponere -> 156 */
    /* --- status 72 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* COMMA: reducere P33 declaratores-primum */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  33, 0 },  /* SEMICOLON: reducere P33 declaratores-primum */
    /* --- status 73 --- */
    {  48, SILVA_TAB_ACTIO_TRANSPONERE, 158, 0 },  /* ASSIGNATIO: transponere -> 158 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* COMMA: reducere P36 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  36, 0 },  /* SEMICOLON: reducere P36 */
    /* --- status 74 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* IDENTIFICATOR: reducere P65 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* CONST: reducere P65 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* VOLATILE: reducere P65 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* STAR: reducere P65 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* PAREN_APERTA: reducere P65 */
    /* --- status 75 --- */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  64, 0 },  /* ASSIGNATIO: reducere P64 */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  64, 0 },  /* COLON: reducere P64 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  64, 0 },  /* COMMA: reducere P64 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  64, 0 },  /* SEMICOLON: reducere P64 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 161, 0 },  /* PAREN_APERTA: transponere -> 161 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  64, 0 },  /* PAREN_CLAUSA: reducere P64 */
    {  69, SILVA_TAB_ACTIO_TRANSPONERE, 160, 0 },  /* QUADRA_APERTA: transponere -> 160 */
    /* --- status 76 --- */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* ASSIGNATIO: reducere P68 declarator-titulus */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* COLON: reducere P68 declarator-titulus */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* COMMA: reducere P68 declarator-titulus */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* SEMICOLON: reducere P68 declarator-titulus */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* PAREN_APERTA: reducere P68 declarator-titulus */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* PAREN_CLAUSA: reducere P68 declarator-titulus */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* QUADRA_APERTA: reducere P68 declarator-titulus */
    /* --- status 77 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* IDENTIFICATOR: transponere -> 76 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  74, 0 },  /* STAR: transponere -> 74 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  77, 0 },  /* PAREN_APERTA: transponere -> 77 */
    /* --- status 78 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* IDENTIFICATOR: reducere P5 declaratio-nuda */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* INTEGER: reducere P5 declaratio-nuda */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* FLOAT: reducere P5 declaratio-nuda */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* CHARACTER_LIT: reducere P5 declaratio-nuda */
    {   4, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* STRING_LIT: reducere P5 declaratio-nuda */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* VOID: reducere P5 declaratio-nuda */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* CHAR: reducere P5 declaratio-nuda */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* SHORT: reducere P5 declaratio-nuda */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* INT: reducere P5 declaratio-nuda */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* LONG: reducere P5 declaratio-nuda */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* FLOAT_KW: reducere P5 declaratio-nuda */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* DOUBLE: reducere P5 declaratio-nuda */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* SIGNED: reducere P5 declaratio-nuda */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* UNSIGNED: reducere P5 declaratio-nuda */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* SIZEOF: reducere P5 declaratio-nuda */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* TYPEDEF: reducere P5 declaratio-nuda */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* EXTERN: reducere P5 declaratio-nuda */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* STATIC: reducere P5 declaratio-nuda */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* AUTO: reducere P5 declaratio-nuda */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* REGISTER: reducere P5 declaratio-nuda */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* CONST: reducere P5 declaratio-nuda */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* VOLATILE: reducere P5 declaratio-nuda */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* STRUCT: reducere P5 declaratio-nuda */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* UNION: reducere P5 declaratio-nuda */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* ENUM: reducere P5 declaratio-nuda */
    {  28, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* PLUS: reducere P5 declaratio-nuda */
    {  29, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* MINUS: reducere P5 declaratio-nuda */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* STAR: reducere P5 declaratio-nuda */
    {  33, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* AMPERSAND: reducere P5 declaratio-nuda */
    {  36, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* TILDE: reducere P5 declaratio-nuda */
    {  37, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* EXCLAMATIO: reducere P5 declaratio-nuda */
    {  59, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* INCREMENTUM: reducere P5 declaratio-nuda */
    {  60, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* DECREMENTUM: reducere P5 declaratio-nuda */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* PAREN_APERTA: reducere P5 declaratio-nuda */
    {  71, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* EOF: reducere P5 declaratio-nuda */
    /* --- status 79 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* IDENTIFICATOR: reducere P6 typus-nominatus */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* VOID: reducere P6 typus-nominatus */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* CHAR: reducere P6 typus-nominatus */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* SHORT: reducere P6 typus-nominatus */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* INT: reducere P6 typus-nominatus */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* LONG: reducere P6 typus-nominatus */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* FLOAT_KW: reducere P6 typus-nominatus */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* DOUBLE: reducere P6 typus-nominatus */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* SIGNED: reducere P6 typus-nominatus */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* UNSIGNED: reducere P6 typus-nominatus */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* TYPEDEF: reducere P6 typus-nominatus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* EXTERN: reducere P6 typus-nominatus */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STATIC: reducere P6 typus-nominatus */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* AUTO: reducere P6 typus-nominatus */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* REGISTER: reducere P6 typus-nominatus */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* CONST: reducere P6 typus-nominatus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* VOLATILE: reducere P6 typus-nominatus */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STRUCT: reducere P6 typus-nominatus */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* UNION: reducere P6 typus-nominatus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* ENUM: reducere P6 typus-nominatus */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STAR: reducere P6 typus-nominatus */
    {  64, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* COLON: reducere P6 typus-nominatus */
    {  65, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* COMMA: reducere P6 typus-nominatus */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* PAREN_APERTA: reducere P6 typus-nominatus */
    {  68, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* PAREN_CLAUSA: reducere P6 typus-nominatus */
    {  69, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* QUADRA_APERTA: reducere P6 typus-nominatus */
    /* --- status 80 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* IDENTIFICATOR: reducere P20 sd-pp-ex-tag */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* TYPEDEF: reducere P20 sd-pp-ex-tag */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* EXTERN: reducere P20 sd-pp-ex-tag */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* STATIC: reducere P20 sd-pp-ex-tag */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* AUTO: reducere P20 sd-pp-ex-tag */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* REGISTER: reducere P20 sd-pp-ex-tag */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* CONST: reducere P20 sd-pp-ex-tag */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* VOLATILE: reducere P20 sd-pp-ex-tag */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* STRUCT: reducere P20 sd-pp-ex-tag */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* UNION: reducere P20 sd-pp-ex-tag */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* ENUM: reducere P20 sd-pp-ex-tag */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* STAR: reducere P20 sd-pp-ex-tag */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* COMMA: reducere P20 sd-pp-ex-tag */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* PAREN_APERTA: reducere P20 sd-pp-ex-tag */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* PAREN_CLAUSA: reducere P20 sd-pp-ex-tag */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  20, 0 },  /* QUADRA_APERTA: reducere P20 sd-pp-ex-tag */
    /* --- status 81 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* IDENTIFICATOR: reducere P24 sd-pa-ex-tag */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* VOID: reducere P24 sd-pa-ex-tag */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* CHAR: reducere P24 sd-pa-ex-tag */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* SHORT: reducere P24 sd-pa-ex-tag */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* INT: reducere P24 sd-pa-ex-tag */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* LONG: reducere P24 sd-pa-ex-tag */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* FLOAT_KW: reducere P24 sd-pa-ex-tag */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* DOUBLE: reducere P24 sd-pa-ex-tag */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* SIGNED: reducere P24 sd-pa-ex-tag */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* UNSIGNED: reducere P24 sd-pa-ex-tag */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* TYPEDEF: reducere P24 sd-pa-ex-tag */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* EXTERN: reducere P24 sd-pa-ex-tag */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* STATIC: reducere P24 sd-pa-ex-tag */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* AUTO: reducere P24 sd-pa-ex-tag */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* REGISTER: reducere P24 sd-pa-ex-tag */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* CONST: reducere P24 sd-pa-ex-tag */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* VOLATILE: reducere P24 sd-pa-ex-tag */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* STRUCT: reducere P24 sd-pa-ex-tag */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* UNION: reducere P24 sd-pa-ex-tag */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* ENUM: reducere P24 sd-pa-ex-tag */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* STAR: reducere P24 sd-pa-ex-tag */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* COMMA: reducere P24 sd-pa-ex-tag */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* PAREN_APERTA: reducere P24 sd-pa-ex-tag */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* PAREN_CLAUSA: reducere P24 sd-pa-ex-tag */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  24, 0 },  /* QUADRA_APERTA: reducere P24 sd-pa-ex-tag */
    /* --- status 82 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* IDENTIFICATOR: reducere P28 sd-pt-ex-tag */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* VOID: reducere P28 sd-pt-ex-tag */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* CHAR: reducere P28 sd-pt-ex-tag */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* SHORT: reducere P28 sd-pt-ex-tag */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* INT: reducere P28 sd-pt-ex-tag */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* LONG: reducere P28 sd-pt-ex-tag */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* FLOAT_KW: reducere P28 sd-pt-ex-tag */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* DOUBLE: reducere P28 sd-pt-ex-tag */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* SIGNED: reducere P28 sd-pt-ex-tag */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* UNSIGNED: reducere P28 sd-pt-ex-tag */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* TYPEDEF: reducere P28 sd-pt-ex-tag */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* EXTERN: reducere P28 sd-pt-ex-tag */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* STATIC: reducere P28 sd-pt-ex-tag */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* AUTO: reducere P28 sd-pt-ex-tag */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* REGISTER: reducere P28 sd-pt-ex-tag */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* CONST: reducere P28 sd-pt-ex-tag */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* VOLATILE: reducere P28 sd-pt-ex-tag */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* STRUCT: reducere P28 sd-pt-ex-tag */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* UNION: reducere P28 sd-pt-ex-tag */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* ENUM: reducere P28 sd-pt-ex-tag */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* STAR: reducere P28 sd-pt-ex-tag */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* COMMA: reducere P28 sd-pt-ex-tag */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* SEMICOLON: reducere P28 sd-pt-ex-tag */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* PAREN_APERTA: reducere P28 sd-pt-ex-tag */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* PAREN_CLAUSA: reducere P28 sd-pt-ex-tag */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  28, 0 },  /* QUADRA_APERTA: reducere P28 sd-pt-ex-tag */
    /* --- status 83 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* IDENTIFICATOR: reducere P22 sd-pa-ex-primitivo */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* VOID: reducere P22 sd-pa-ex-primitivo */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* CHAR: reducere P22 sd-pa-ex-primitivo */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* SHORT: reducere P22 sd-pa-ex-primitivo */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* INT: reducere P22 sd-pa-ex-primitivo */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* LONG: reducere P22 sd-pa-ex-primitivo */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* FLOAT_KW: reducere P22 sd-pa-ex-primitivo */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* DOUBLE: reducere P22 sd-pa-ex-primitivo */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* SIGNED: reducere P22 sd-pa-ex-primitivo */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* UNSIGNED: reducere P22 sd-pa-ex-primitivo */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* TYPEDEF: reducere P22 sd-pa-ex-primitivo */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* EXTERN: reducere P22 sd-pa-ex-primitivo */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* STATIC: reducere P22 sd-pa-ex-primitivo */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* AUTO: reducere P22 sd-pa-ex-primitivo */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* REGISTER: reducere P22 sd-pa-ex-primitivo */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* CONST: reducere P22 sd-pa-ex-primitivo */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* VOLATILE: reducere P22 sd-pa-ex-primitivo */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* STRUCT: reducere P22 sd-pa-ex-primitivo */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* UNION: reducere P22 sd-pa-ex-primitivo */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* ENUM: reducere P22 sd-pa-ex-primitivo */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* STAR: reducere P22 sd-pa-ex-primitivo */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* COMMA: reducere P22 sd-pa-ex-primitivo */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* PAREN_APERTA: reducere P22 sd-pa-ex-primitivo */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* PAREN_CLAUSA: reducere P22 sd-pa-ex-primitivo */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  22, 0 },  /* QUADRA_APERTA: reducere P22 sd-pa-ex-primitivo */
    /* --- status 84 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* IDENTIFICATOR: reducere P26 sd-pt-ex-primitivo */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* VOID: reducere P26 sd-pt-ex-primitivo */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* CHAR: reducere P26 sd-pt-ex-primitivo */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* SHORT: reducere P26 sd-pt-ex-primitivo */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* INT: reducere P26 sd-pt-ex-primitivo */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* LONG: reducere P26 sd-pt-ex-primitivo */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* FLOAT_KW: reducere P26 sd-pt-ex-primitivo */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* DOUBLE: reducere P26 sd-pt-ex-primitivo */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* SIGNED: reducere P26 sd-pt-ex-primitivo */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* UNSIGNED: reducere P26 sd-pt-ex-primitivo */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* TYPEDEF: reducere P26 sd-pt-ex-primitivo */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* EXTERN: reducere P26 sd-pt-ex-primitivo */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* STATIC: reducere P26 sd-pt-ex-primitivo */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* AUTO: reducere P26 sd-pt-ex-primitivo */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* REGISTER: reducere P26 sd-pt-ex-primitivo */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* CONST: reducere P26 sd-pt-ex-primitivo */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* VOLATILE: reducere P26 sd-pt-ex-primitivo */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* STRUCT: reducere P26 sd-pt-ex-primitivo */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* UNION: reducere P26 sd-pt-ex-primitivo */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* ENUM: reducere P26 sd-pt-ex-primitivo */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* STAR: reducere P26 sd-pt-ex-primitivo */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* COMMA: reducere P26 sd-pt-ex-primitivo */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* SEMICOLON: reducere P26 sd-pt-ex-primitivo */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* PAREN_APERTA: reducere P26 sd-pt-ex-primitivo */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* PAREN_CLAUSA: reducere P26 sd-pt-ex-primitivo */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  26, 0 },  /* QUADRA_APERTA: reducere P26 sd-pt-ex-primitivo */
    /* --- status 85 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* IDENTIFICATOR: reducere P19 sd-pp-ex-alio */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* TYPEDEF: reducere P19 sd-pp-ex-alio */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* EXTERN: reducere P19 sd-pp-ex-alio */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* STATIC: reducere P19 sd-pp-ex-alio */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* AUTO: reducere P19 sd-pp-ex-alio */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* REGISTER: reducere P19 sd-pp-ex-alio */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* CONST: reducere P19 sd-pp-ex-alio */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* VOLATILE: reducere P19 sd-pp-ex-alio */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* STRUCT: reducere P19 sd-pp-ex-alio */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* UNION: reducere P19 sd-pp-ex-alio */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* ENUM: reducere P19 sd-pp-ex-alio */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* STAR: reducere P19 sd-pp-ex-alio */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* COMMA: reducere P19 sd-pp-ex-alio */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* PAREN_APERTA: reducere P19 sd-pp-ex-alio */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* PAREN_CLAUSA: reducere P19 sd-pp-ex-alio */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  19, 0 },  /* QUADRA_APERTA: reducere P19 sd-pp-ex-alio */
    /* --- status 86 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* IDENTIFICATOR: reducere P23 sd-pa-ex-alio */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* VOID: reducere P23 sd-pa-ex-alio */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* CHAR: reducere P23 sd-pa-ex-alio */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* SHORT: reducere P23 sd-pa-ex-alio */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* INT: reducere P23 sd-pa-ex-alio */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* LONG: reducere P23 sd-pa-ex-alio */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* FLOAT_KW: reducere P23 sd-pa-ex-alio */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* DOUBLE: reducere P23 sd-pa-ex-alio */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* SIGNED: reducere P23 sd-pa-ex-alio */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* UNSIGNED: reducere P23 sd-pa-ex-alio */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* TYPEDEF: reducere P23 sd-pa-ex-alio */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* EXTERN: reducere P23 sd-pa-ex-alio */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* STATIC: reducere P23 sd-pa-ex-alio */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* AUTO: reducere P23 sd-pa-ex-alio */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* REGISTER: reducere P23 sd-pa-ex-alio */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* CONST: reducere P23 sd-pa-ex-alio */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* VOLATILE: reducere P23 sd-pa-ex-alio */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* STRUCT: reducere P23 sd-pa-ex-alio */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* UNION: reducere P23 sd-pa-ex-alio */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* ENUM: reducere P23 sd-pa-ex-alio */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* STAR: reducere P23 sd-pa-ex-alio */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* COMMA: reducere P23 sd-pa-ex-alio */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* PAREN_APERTA: reducere P23 sd-pa-ex-alio */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* PAREN_CLAUSA: reducere P23 sd-pa-ex-alio */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  23, 0 },  /* QUADRA_APERTA: reducere P23 sd-pa-ex-alio */
    /* --- status 87 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* IDENTIFICATOR: reducere P27 sd-pt-ex-alio */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* VOID: reducere P27 sd-pt-ex-alio */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* CHAR: reducere P27 sd-pt-ex-alio */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* SHORT: reducere P27 sd-pt-ex-alio */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* INT: reducere P27 sd-pt-ex-alio */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* LONG: reducere P27 sd-pt-ex-alio */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* FLOAT_KW: reducere P27 sd-pt-ex-alio */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* DOUBLE: reducere P27 sd-pt-ex-alio */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* SIGNED: reducere P27 sd-pt-ex-alio */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* UNSIGNED: reducere P27 sd-pt-ex-alio */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* TYPEDEF: reducere P27 sd-pt-ex-alio */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* EXTERN: reducere P27 sd-pt-ex-alio */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* STATIC: reducere P27 sd-pt-ex-alio */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* AUTO: reducere P27 sd-pt-ex-alio */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* REGISTER: reducere P27 sd-pt-ex-alio */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* CONST: reducere P27 sd-pt-ex-alio */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* VOLATILE: reducere P27 sd-pt-ex-alio */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* STRUCT: reducere P27 sd-pt-ex-alio */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* UNION: reducere P27 sd-pt-ex-alio */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* ENUM: reducere P27 sd-pt-ex-alio */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* STAR: reducere P27 sd-pt-ex-alio */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* COMMA: reducere P27 sd-pt-ex-alio */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* SEMICOLON: reducere P27 sd-pt-ex-alio */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* PAREN_APERTA: reducere P27 sd-pt-ex-alio */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* PAREN_CLAUSA: reducere P27 sd-pt-ex-alio */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  27, 0 },  /* QUADRA_APERTA: reducere P27 sd-pt-ex-alio */
    /* --- status 88 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* IDENTIFICATOR: reducere P166 verba-primitiva-appendere */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* VOID: reducere P166 verba-primitiva-appendere */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* CHAR: reducere P166 verba-primitiva-appendere */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* SHORT: reducere P166 verba-primitiva-appendere */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* INT: reducere P166 verba-primitiva-appendere */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* LONG: reducere P166 verba-primitiva-appendere */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* FLOAT_KW: reducere P166 verba-primitiva-appendere */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* DOUBLE: reducere P166 verba-primitiva-appendere */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* SIGNED: reducere P166 verba-primitiva-appendere */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* UNSIGNED: reducere P166 verba-primitiva-appendere */
    {  15, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* TYPEDEF: reducere P166 verba-primitiva-appendere */
    {  16, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* EXTERN: reducere P166 verba-primitiva-appendere */
    {  17, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* STATIC: reducere P166 verba-primitiva-appendere */
    {  18, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* AUTO: reducere P166 verba-primitiva-appendere */
    {  19, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* REGISTER: reducere P166 verba-primitiva-appendere */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* CONST: reducere P166 verba-primitiva-appendere */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* VOLATILE: reducere P166 verba-primitiva-appendere */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* STRUCT: reducere P166 verba-primitiva-appendere */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* UNION: reducere P166 verba-primitiva-appendere */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* ENUM: reducere P166 verba-primitiva-appendere */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* STAR: reducere P166 verba-primitiva-appendere */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* COLON: reducere P166 verba-primitiva-appendere */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* COMMA: reducere P166 verba-primitiva-appendere */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* PAREN_APERTA: reducere P166 verba-primitiva-appendere */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* PAREN_CLAUSA: reducere P166 verba-primitiva-appendere */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 166, 0 },  /* QUADRA_APERTA: reducere P166 verba-primitiva-appendere */
    /* --- status 89 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* IDENTIFICATOR: reducere P44 structura-nominata */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* VOID: reducere P44 structura-nominata */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* CHAR: reducere P44 structura-nominata */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* SHORT: reducere P44 structura-nominata */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* INT: reducere P44 structura-nominata */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* LONG: reducere P44 structura-nominata */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* FLOAT_KW: reducere P44 structura-nominata */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* DOUBLE: reducere P44 structura-nominata */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* SIGNED: reducere P44 structura-nominata */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* UNSIGNED: reducere P44 structura-nominata */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* TYPEDEF: reducere P44 structura-nominata */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* EXTERN: reducere P44 structura-nominata */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* STATIC: reducere P44 structura-nominata */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* AUTO: reducere P44 structura-nominata */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* REGISTER: reducere P44 structura-nominata */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* CONST: reducere P44 structura-nominata */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* VOLATILE: reducere P44 structura-nominata */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* STRUCT: reducere P44 structura-nominata */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* UNION: reducere P44 structura-nominata */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* ENUM: reducere P44 structura-nominata */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE, 163, 0 },  /* BRACE_APERTA: transponere -> 163 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* STAR: reducere P44 structura-nominata */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* COLON: reducere P44 structura-nominata */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* COMMA: reducere P44 structura-nominata */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* SEMICOLON: reducere P44 structura-nominata */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* PAREN_APERTA: reducere P44 structura-nominata */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* PAREN_CLAUSA: reducere P44 structura-nominata */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  44, 0 },  /* QUADRA_APERTA: reducere P44 structura-nominata */
    /* --- status 90 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* IDENTIFICATOR: reducere P48 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* VOID: reducere P48 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CHAR: reducere P48 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SHORT: reducere P48 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* INT: reducere P48 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* LONG: reducere P48 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* FLOAT_KW: reducere P48 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* DOUBLE: reducere P48 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SIGNED: reducere P48 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* UNSIGNED: reducere P48 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CONST: reducere P48 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* VOLATILE: reducere P48 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* STRUCT: reducere P48 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* UNION: reducere P48 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* ENUM: reducere P48 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* BRACE_CLAUSA: reducere P48 */
    /* --- status 91 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* IDENTIFICATOR: reducere P47 unio-nominata */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* VOID: reducere P47 unio-nominata */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* CHAR: reducere P47 unio-nominata */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* SHORT: reducere P47 unio-nominata */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* INT: reducere P47 unio-nominata */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* LONG: reducere P47 unio-nominata */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* FLOAT_KW: reducere P47 unio-nominata */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* DOUBLE: reducere P47 unio-nominata */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* SIGNED: reducere P47 unio-nominata */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* UNSIGNED: reducere P47 unio-nominata */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* TYPEDEF: reducere P47 unio-nominata */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* EXTERN: reducere P47 unio-nominata */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* STATIC: reducere P47 unio-nominata */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* AUTO: reducere P47 unio-nominata */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* REGISTER: reducere P47 unio-nominata */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* CONST: reducere P47 unio-nominata */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* VOLATILE: reducere P47 unio-nominata */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* STRUCT: reducere P47 unio-nominata */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* UNION: reducere P47 unio-nominata */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* ENUM: reducere P47 unio-nominata */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE, 165, 0 },  /* BRACE_APERTA: transponere -> 165 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* STAR: reducere P47 unio-nominata */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* COLON: reducere P47 unio-nominata */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* COMMA: reducere P47 unio-nominata */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* SEMICOLON: reducere P47 unio-nominata */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* PAREN_APERTA: reducere P47 unio-nominata */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* PAREN_CLAUSA: reducere P47 unio-nominata */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  47, 0 },  /* QUADRA_APERTA: reducere P47 unio-nominata */
    /* --- status 92 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* IDENTIFICATOR: reducere P48 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* VOID: reducere P48 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CHAR: reducere P48 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SHORT: reducere P48 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* INT: reducere P48 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* LONG: reducere P48 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* FLOAT_KW: reducere P48 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* DOUBLE: reducere P48 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SIGNED: reducere P48 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* UNSIGNED: reducere P48 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CONST: reducere P48 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* VOLATILE: reducere P48 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* STRUCT: reducere P48 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* UNION: reducere P48 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* ENUM: reducere P48 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* BRACE_CLAUSA: reducere P48 */
    /* --- status 93 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* IDENTIFICATOR: reducere P58 enumeratio-nominata */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* VOID: reducere P58 enumeratio-nominata */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* CHAR: reducere P58 enumeratio-nominata */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* SHORT: reducere P58 enumeratio-nominata */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* INT: reducere P58 enumeratio-nominata */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* LONG: reducere P58 enumeratio-nominata */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* FLOAT_KW: reducere P58 enumeratio-nominata */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* DOUBLE: reducere P58 enumeratio-nominata */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* SIGNED: reducere P58 enumeratio-nominata */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* UNSIGNED: reducere P58 enumeratio-nominata */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* TYPEDEF: reducere P58 enumeratio-nominata */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* EXTERN: reducere P58 enumeratio-nominata */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* STATIC: reducere P58 enumeratio-nominata */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* AUTO: reducere P58 enumeratio-nominata */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* REGISTER: reducere P58 enumeratio-nominata */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* CONST: reducere P58 enumeratio-nominata */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* VOLATILE: reducere P58 enumeratio-nominata */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* STRUCT: reducere P58 enumeratio-nominata */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* UNION: reducere P58 enumeratio-nominata */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* ENUM: reducere P58 enumeratio-nominata */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE, 167, 0 },  /* BRACE_APERTA: transponere -> 167 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* STAR: reducere P58 enumeratio-nominata */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* COLON: reducere P58 enumeratio-nominata */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* COMMA: reducere P58 enumeratio-nominata */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* SEMICOLON: reducere P58 enumeratio-nominata */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* PAREN_APERTA: reducere P58 enumeratio-nominata */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* PAREN_CLAUSA: reducere P58 enumeratio-nominata */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  58, 0 },  /* QUADRA_APERTA: reducere P58 enumeratio-nominata */
    /* --- status 94 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 170, 0 },  /* IDENTIFICATOR: transponere -> 170 */
    /* --- status 95 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* IDENTIFICATOR: reducere P82 sententia-expr */
    {   1, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* INTEGER: reducere P82 sententia-expr */
    {   2, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* FLOAT: reducere P82 sententia-expr */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* CHARACTER_LIT: reducere P82 sententia-expr */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* STRING_LIT: reducere P82 sententia-expr */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* VOID: reducere P82 sententia-expr */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* CHAR: reducere P82 sententia-expr */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* SHORT: reducere P82 sententia-expr */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* INT: reducere P82 sententia-expr */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* LONG: reducere P82 sententia-expr */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* FLOAT_KW: reducere P82 sententia-expr */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* DOUBLE: reducere P82 sententia-expr */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* SIGNED: reducere P82 sententia-expr */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* UNSIGNED: reducere P82 sententia-expr */
    {  14, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* SIZEOF: reducere P82 sententia-expr */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* TYPEDEF: reducere P82 sententia-expr */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* EXTERN: reducere P82 sententia-expr */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* STATIC: reducere P82 sententia-expr */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* AUTO: reducere P82 sententia-expr */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* REGISTER: reducere P82 sententia-expr */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* CONST: reducere P82 sententia-expr */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* VOLATILE: reducere P82 sententia-expr */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* STRUCT: reducere P82 sententia-expr */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* UNION: reducere P82 sententia-expr */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* ENUM: reducere P82 sententia-expr */
    {  28, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* PLUS: reducere P82 sententia-expr */
    {  29, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* MINUS: reducere P82 sententia-expr */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* STAR: reducere P82 sententia-expr */
    {  33, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* AMPERSAND: reducere P82 sententia-expr */
    {  36, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* TILDE: reducere P82 sententia-expr */
    {  37, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* EXCLAMATIO: reducere P82 sententia-expr */
    {  59, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* INCREMENTUM: reducere P82 sententia-expr */
    {  60, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* DECREMENTUM: reducere P82 sententia-expr */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* PAREN_APERTA: reducere P82 sententia-expr */
    {  71, SILVA_TAB_ACTIO_REDUCERE,  82, 0 },  /* EOF: reducere P82 sententia-expr */
    /* --- status 96 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 97 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 98 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 99 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 100 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 101 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 102 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 103 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 104 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 105 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 106 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 107 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 108 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 109 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 110 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 111 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 112 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 113 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 114 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 115 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 116 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 117 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 118 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 119 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 120 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 121 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 122 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 123 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 124 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 125 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 126 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 127 --- */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE,  96, 0 },  /* COMMA: transponere -> 96 */
    {  68, SILVA_TAB_ACTIO_TRANSPONERE, 202, 0 },  /* PAREN_CLAUSA: transponere -> 202 */
    /* --- status 128 --- */
    {  68, SILVA_TAB_ACTIO_TRANSPONERE, 203, 0 },  /* PAREN_CLAUSA: transponere -> 203 */
    /* --- status 129 --- */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE, 205, 0 },  /* STAR: transponere -> 205 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 207, 0 },  /* PAREN_APERTA: transponere -> 207 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 141, 0 },  /* PAREN_CLAUSA: reducere P141 species-typi-nuda */
    {  69, SILVA_TAB_ACTIO_TRANSPONERE, 208, 0 },  /* QUADRA_APERTA: transponere -> 208 */
    /* --- status 130 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 142, 0 },  /* IDENTIFICATOR: reducere P142 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 142, 0 },  /* STAR: reducere P142 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 142, 0 },  /* COLON: reducere P142 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 142, 0 },  /* PAREN_APERTA: reducere P142 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 142, 0 },  /* PAREN_CLAUSA: reducere P142 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 142, 0 },  /* QUADRA_APERTA: reducere P142 */
    /* --- status 131 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 143, 0 },  /* IDENTIFICATOR: reducere P143 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 143, 0 },  /* STAR: reducere P143 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 143, 0 },  /* COLON: reducere P143 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 143, 0 },  /* PAREN_APERTA: reducere P143 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 143, 0 },  /* PAREN_CLAUSA: reducere P143 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 143, 0 },  /* QUADRA_APERTA: reducere P143 */
    /* --- status 132 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 144, 0 },  /* IDENTIFICATOR: reducere P144 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 144, 0 },  /* STAR: reducere P144 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 144, 0 },  /* COLON: reducere P144 */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 144, 0 },  /* PAREN_APERTA: reducere P144 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 144, 0 },  /* PAREN_CLAUSA: reducere P144 */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 144, 0 },  /* QUADRA_APERTA: reducere P144 */
    /* --- status 133 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* IDENTIFICATOR: reducere P145 sq-pp-primum */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* CONST: reducere P145 sq-pp-primum */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* VOLATILE: reducere P145 sq-pp-primum */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* STRUCT: reducere P145 sq-pp-primum */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* UNION: reducere P145 sq-pp-primum */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* ENUM: reducere P145 sq-pp-primum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* STAR: reducere P145 sq-pp-primum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* COLON: reducere P145 sq-pp-primum */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* PAREN_APERTA: reducere P145 sq-pp-primum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* PAREN_CLAUSA: reducere P145 sq-pp-primum */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 145, 0 },  /* QUADRA_APERTA: reducere P145 sq-pp-primum */
    /* --- status 134 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* IDENTIFICATOR: reducere P148 sq-pa-primum */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* VOID: reducere P148 sq-pa-primum */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* CHAR: reducere P148 sq-pa-primum */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* SHORT: reducere P148 sq-pa-primum */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* INT: reducere P148 sq-pa-primum */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* LONG: reducere P148 sq-pa-primum */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* FLOAT_KW: reducere P148 sq-pa-primum */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* DOUBLE: reducere P148 sq-pa-primum */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* SIGNED: reducere P148 sq-pa-primum */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* UNSIGNED: reducere P148 sq-pa-primum */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* CONST: reducere P148 sq-pa-primum */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* VOLATILE: reducere P148 sq-pa-primum */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* STRUCT: reducere P148 sq-pa-primum */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* UNION: reducere P148 sq-pa-primum */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* ENUM: reducere P148 sq-pa-primum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* STAR: reducere P148 sq-pa-primum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* COLON: reducere P148 sq-pa-primum */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* PAREN_APERTA: reducere P148 sq-pa-primum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* PAREN_CLAUSA: reducere P148 sq-pa-primum */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 148, 0 },  /* QUADRA_APERTA: reducere P148 sq-pa-primum */
    /* --- status 135 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* IDENTIFICATOR: reducere P152 sq-pt-primum */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* VOID: reducere P152 sq-pt-primum */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* CHAR: reducere P152 sq-pt-primum */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* SHORT: reducere P152 sq-pt-primum */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* INT: reducere P152 sq-pt-primum */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* LONG: reducere P152 sq-pt-primum */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* FLOAT_KW: reducere P152 sq-pt-primum */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* DOUBLE: reducere P152 sq-pt-primum */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* SIGNED: reducere P152 sq-pt-primum */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* UNSIGNED: reducere P152 sq-pt-primum */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* CONST: reducere P152 sq-pt-primum */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* VOLATILE: reducere P152 sq-pt-primum */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* STRUCT: reducere P152 sq-pt-primum */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* UNION: reducere P152 sq-pt-primum */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* ENUM: reducere P152 sq-pt-primum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* STAR: reducere P152 sq-pt-primum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* COLON: reducere P152 sq-pt-primum */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* PAREN_APERTA: reducere P152 sq-pt-primum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* PAREN_CLAUSA: reducere P152 sq-pt-primum */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 152, 0 },  /* QUADRA_APERTA: reducere P152 sq-pt-primum */
    /* --- status 136 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* BRACE_CLAUSA: reducere P129 unarium-incrementum */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* PLUS: reducere P129 unarium-incrementum */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* MINUS: reducere P129 unarium-incrementum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* STAR: reducere P129 unarium-incrementum */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* SOLIDUS: reducere P129 unarium-incrementum */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* PERCENTUM: reducere P129 unarium-incrementum */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* AMPERSAND: reducere P129 unarium-incrementum */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* BARRA: reducere P129 unarium-incrementum */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* CARET: reducere P129 unarium-incrementum */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* ET_ET: reducere P129 unarium-incrementum */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* VEL_VEL: reducere P129 unarium-incrementum */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* AEQUALIS_AEQUALIS: reducere P129 unarium-incrementum */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* NON_AEQUALIS: reducere P129 unarium-incrementum */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* MINOR: reducere P129 unarium-incrementum */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* MAIOR: reducere P129 unarium-incrementum */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* MINOR_AEQUALIS: reducere P129 unarium-incrementum */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* MAIOR_AEQUALIS: reducere P129 unarium-incrementum */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* SINISTRORSUM: reducere P129 unarium-incrementum */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* DEXTRORSUM: reducere P129 unarium-incrementum */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* ASSIGNATIO: reducere P129 unarium-incrementum */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* PLUS_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* MINUS_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* STAR_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* BARRA_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* CARET_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P129 unarium-incrementum */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* QUAESTIO: reducere P129 unarium-incrementum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* COLON: reducere P129 unarium-incrementum */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* COMMA: reducere P129 unarium-incrementum */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* SEMICOLON: reducere P129 unarium-incrementum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* PAREN_CLAUSA: reducere P129 unarium-incrementum */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 129, 0 },  /* QUADRA_CLAUSA: reducere P129 unarium-incrementum */
    /* --- status 137 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* BRACE_CLAUSA: reducere P190 primarium-identificator */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PLUS: reducere P190 primarium-identificator */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MINUS: reducere P190 primarium-identificator */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* STAR: reducere P190 primarium-identificator */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SOLIDUS: reducere P190 primarium-identificator */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PERCENTUM: reducere P190 primarium-identificator */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* AMPERSAND: reducere P190 primarium-identificator */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* BARRA: reducere P190 primarium-identificator */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* CARET: reducere P190 primarium-identificator */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* ET_ET: reducere P190 primarium-identificator */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* VEL_VEL: reducere P190 primarium-identificator */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* AEQUALIS_AEQUALIS: reducere P190 primarium-identificator */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* NON_AEQUALIS: reducere P190 primarium-identificator */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MINOR: reducere P190 primarium-identificator */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MAIOR: reducere P190 primarium-identificator */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MINOR_AEQUALIS: reducere P190 primarium-identificator */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MAIOR_AEQUALIS: reducere P190 primarium-identificator */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SINISTRORSUM: reducere P190 primarium-identificator */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* DEXTRORSUM: reducere P190 primarium-identificator */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* ASSIGNATIO: reducere P190 primarium-identificator */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PLUS_ASSIGNATIO: reducere P190 primarium-identificator */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* MINUS_ASSIGNATIO: reducere P190 primarium-identificator */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* STAR_ASSIGNATIO: reducere P190 primarium-identificator */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P190 primarium-identificator */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P190 primarium-identificator */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P190 primarium-identificator */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* BARRA_ASSIGNATIO: reducere P190 primarium-identificator */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* CARET_ASSIGNATIO: reducere P190 primarium-identificator */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P190 primarium-identificator */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P190 primarium-identificator */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* INCREMENTUM: reducere P190 primarium-identificator */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* DECREMENTUM: reducere P190 primarium-identificator */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PUNCTUM: reducere P190 primarium-identificator */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SAGITTA: reducere P190 primarium-identificator */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* QUAESTIO: reducere P190 primarium-identificator */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* COLON: reducere P190 primarium-identificator */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* COMMA: reducere P190 primarium-identificator */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* SEMICOLON: reducere P190 primarium-identificator */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PAREN_APERTA: reducere P190 primarium-identificator */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* PAREN_CLAUSA: reducere P190 primarium-identificator */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* QUADRA_APERTA: reducere P190 primarium-identificator */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 190, 0 },  /* QUADRA_CLAUSA: reducere P190 primarium-identificator */
    /* --- status 138 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 139 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* BRACE_CLAUSA: reducere P130 unarium-decrementum */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* PLUS: reducere P130 unarium-decrementum */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* MINUS: reducere P130 unarium-decrementum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* STAR: reducere P130 unarium-decrementum */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* SOLIDUS: reducere P130 unarium-decrementum */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* PERCENTUM: reducere P130 unarium-decrementum */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* AMPERSAND: reducere P130 unarium-decrementum */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* BARRA: reducere P130 unarium-decrementum */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* CARET: reducere P130 unarium-decrementum */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* ET_ET: reducere P130 unarium-decrementum */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* VEL_VEL: reducere P130 unarium-decrementum */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* AEQUALIS_AEQUALIS: reducere P130 unarium-decrementum */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* NON_AEQUALIS: reducere P130 unarium-decrementum */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* MINOR: reducere P130 unarium-decrementum */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* MAIOR: reducere P130 unarium-decrementum */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* MINOR_AEQUALIS: reducere P130 unarium-decrementum */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* MAIOR_AEQUALIS: reducere P130 unarium-decrementum */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* SINISTRORSUM: reducere P130 unarium-decrementum */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* DEXTRORSUM: reducere P130 unarium-decrementum */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* ASSIGNATIO: reducere P130 unarium-decrementum */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* PLUS_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* MINUS_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* STAR_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* BARRA_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* CARET_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P130 unarium-decrementum */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* QUAESTIO: reducere P130 unarium-decrementum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* COLON: reducere P130 unarium-decrementum */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* COMMA: reducere P130 unarium-decrementum */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* SEMICOLON: reducere P130 unarium-decrementum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* PAREN_CLAUSA: reducere P130 unarium-decrementum */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 130, 0 },  /* QUADRA_CLAUSA: reducere P130 unarium-decrementum */
    /* --- status 140 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* BRACE_CLAUSA: reducere P128 */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* PLUS: reducere P128 */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MINUS: reducere P128 */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* STAR: reducere P128 */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* SOLIDUS: reducere P128 */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* PERCENTUM: reducere P128 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* AMPERSAND: reducere P128 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* BARRA: reducere P128 */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* CARET: reducere P128 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* ET_ET: reducere P128 */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* VEL_VEL: reducere P128 */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* AEQUALIS_AEQUALIS: reducere P128 */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* NON_AEQUALIS: reducere P128 */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MINOR: reducere P128 */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MAIOR: reducere P128 */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MINOR_AEQUALIS: reducere P128 */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MAIOR_AEQUALIS: reducere P128 */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* SINISTRORSUM: reducere P128 */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* DEXTRORSUM: reducere P128 */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* ASSIGNATIO: reducere P128 */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* PLUS_ASSIGNATIO: reducere P128 */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* MINUS_ASSIGNATIO: reducere P128 */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* STAR_ASSIGNATIO: reducere P128 */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P128 */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P128 */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P128 */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* BARRA_ASSIGNATIO: reducere P128 */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* CARET_ASSIGNATIO: reducere P128 */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P128 */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P128 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* QUAESTIO: reducere P128 */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* COLON: reducere P128 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* COMMA: reducere P128 */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* SEMICOLON: reducere P128 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* PAREN_CLAUSA: reducere P128 */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 128, 0 },  /* QUADRA_CLAUSA: reducere P128 */
    /* --- status 141 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* BRACE_CLAUSA: reducere P131 unarium-ampersand */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* PLUS: reducere P131 unarium-ampersand */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* MINUS: reducere P131 unarium-ampersand */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* STAR: reducere P131 unarium-ampersand */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* SOLIDUS: reducere P131 unarium-ampersand */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* PERCENTUM: reducere P131 unarium-ampersand */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* AMPERSAND: reducere P131 unarium-ampersand */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* BARRA: reducere P131 unarium-ampersand */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* CARET: reducere P131 unarium-ampersand */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* ET_ET: reducere P131 unarium-ampersand */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* VEL_VEL: reducere P131 unarium-ampersand */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* AEQUALIS_AEQUALIS: reducere P131 unarium-ampersand */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* NON_AEQUALIS: reducere P131 unarium-ampersand */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* MINOR: reducere P131 unarium-ampersand */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* MAIOR: reducere P131 unarium-ampersand */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* MINOR_AEQUALIS: reducere P131 unarium-ampersand */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* MAIOR_AEQUALIS: reducere P131 unarium-ampersand */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* SINISTRORSUM: reducere P131 unarium-ampersand */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* DEXTRORSUM: reducere P131 unarium-ampersand */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* ASSIGNATIO: reducere P131 unarium-ampersand */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* PLUS_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* MINUS_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* STAR_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* BARRA_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* CARET_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P131 unarium-ampersand */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* QUAESTIO: reducere P131 unarium-ampersand */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* COLON: reducere P131 unarium-ampersand */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* COMMA: reducere P131 unarium-ampersand */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* SEMICOLON: reducere P131 unarium-ampersand */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* PAREN_CLAUSA: reducere P131 unarium-ampersand */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 131, 0 },  /* QUADRA_CLAUSA: reducere P131 unarium-ampersand */
    /* --- status 142 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* BRACE_CLAUSA: reducere P132 unarium-stella */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* PLUS: reducere P132 unarium-stella */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* MINUS: reducere P132 unarium-stella */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* STAR: reducere P132 unarium-stella */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* SOLIDUS: reducere P132 unarium-stella */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* PERCENTUM: reducere P132 unarium-stella */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* AMPERSAND: reducere P132 unarium-stella */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* BARRA: reducere P132 unarium-stella */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* CARET: reducere P132 unarium-stella */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* ET_ET: reducere P132 unarium-stella */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* VEL_VEL: reducere P132 unarium-stella */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* AEQUALIS_AEQUALIS: reducere P132 unarium-stella */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* NON_AEQUALIS: reducere P132 unarium-stella */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* MINOR: reducere P132 unarium-stella */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* MAIOR: reducere P132 unarium-stella */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* MINOR_AEQUALIS: reducere P132 unarium-stella */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* MAIOR_AEQUALIS: reducere P132 unarium-stella */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* SINISTRORSUM: reducere P132 unarium-stella */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* DEXTRORSUM: reducere P132 unarium-stella */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* ASSIGNATIO: reducere P132 unarium-stella */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* PLUS_ASSIGNATIO: reducere P132 unarium-stella */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* MINUS_ASSIGNATIO: reducere P132 unarium-stella */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* STAR_ASSIGNATIO: reducere P132 unarium-stella */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P132 unarium-stella */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P132 unarium-stella */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P132 unarium-stella */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* BARRA_ASSIGNATIO: reducere P132 unarium-stella */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* CARET_ASSIGNATIO: reducere P132 unarium-stella */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P132 unarium-stella */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P132 unarium-stella */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* QUAESTIO: reducere P132 unarium-stella */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* COLON: reducere P132 unarium-stella */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* COMMA: reducere P132 unarium-stella */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* SEMICOLON: reducere P132 unarium-stella */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* PAREN_CLAUSA: reducere P132 unarium-stella */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 132, 0 },  /* QUADRA_CLAUSA: reducere P132 unarium-stella */
    /* --- status 143 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* BRACE_CLAUSA: reducere P133 unarium-plus */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* PLUS: reducere P133 unarium-plus */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* MINUS: reducere P133 unarium-plus */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* STAR: reducere P133 unarium-plus */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* SOLIDUS: reducere P133 unarium-plus */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* PERCENTUM: reducere P133 unarium-plus */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* AMPERSAND: reducere P133 unarium-plus */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* BARRA: reducere P133 unarium-plus */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* CARET: reducere P133 unarium-plus */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* ET_ET: reducere P133 unarium-plus */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* VEL_VEL: reducere P133 unarium-plus */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* AEQUALIS_AEQUALIS: reducere P133 unarium-plus */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* NON_AEQUALIS: reducere P133 unarium-plus */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* MINOR: reducere P133 unarium-plus */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* MAIOR: reducere P133 unarium-plus */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* MINOR_AEQUALIS: reducere P133 unarium-plus */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* MAIOR_AEQUALIS: reducere P133 unarium-plus */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* SINISTRORSUM: reducere P133 unarium-plus */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* DEXTRORSUM: reducere P133 unarium-plus */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* ASSIGNATIO: reducere P133 unarium-plus */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* PLUS_ASSIGNATIO: reducere P133 unarium-plus */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* MINUS_ASSIGNATIO: reducere P133 unarium-plus */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* STAR_ASSIGNATIO: reducere P133 unarium-plus */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P133 unarium-plus */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P133 unarium-plus */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P133 unarium-plus */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* BARRA_ASSIGNATIO: reducere P133 unarium-plus */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* CARET_ASSIGNATIO: reducere P133 unarium-plus */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P133 unarium-plus */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P133 unarium-plus */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* QUAESTIO: reducere P133 unarium-plus */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* COLON: reducere P133 unarium-plus */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* COMMA: reducere P133 unarium-plus */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* SEMICOLON: reducere P133 unarium-plus */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* PAREN_CLAUSA: reducere P133 unarium-plus */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 133, 0 },  /* QUADRA_CLAUSA: reducere P133 unarium-plus */
    /* --- status 144 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* BRACE_CLAUSA: reducere P134 unarium-minus */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* PLUS: reducere P134 unarium-minus */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* MINUS: reducere P134 unarium-minus */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* STAR: reducere P134 unarium-minus */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* SOLIDUS: reducere P134 unarium-minus */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* PERCENTUM: reducere P134 unarium-minus */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* AMPERSAND: reducere P134 unarium-minus */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* BARRA: reducere P134 unarium-minus */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* CARET: reducere P134 unarium-minus */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* ET_ET: reducere P134 unarium-minus */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* VEL_VEL: reducere P134 unarium-minus */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* AEQUALIS_AEQUALIS: reducere P134 unarium-minus */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* NON_AEQUALIS: reducere P134 unarium-minus */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* MINOR: reducere P134 unarium-minus */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* MAIOR: reducere P134 unarium-minus */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* MINOR_AEQUALIS: reducere P134 unarium-minus */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* MAIOR_AEQUALIS: reducere P134 unarium-minus */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* SINISTRORSUM: reducere P134 unarium-minus */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* DEXTRORSUM: reducere P134 unarium-minus */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* ASSIGNATIO: reducere P134 unarium-minus */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* PLUS_ASSIGNATIO: reducere P134 unarium-minus */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* MINUS_ASSIGNATIO: reducere P134 unarium-minus */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* STAR_ASSIGNATIO: reducere P134 unarium-minus */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P134 unarium-minus */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P134 unarium-minus */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P134 unarium-minus */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* BARRA_ASSIGNATIO: reducere P134 unarium-minus */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* CARET_ASSIGNATIO: reducere P134 unarium-minus */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P134 unarium-minus */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P134 unarium-minus */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* QUAESTIO: reducere P134 unarium-minus */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* COLON: reducere P134 unarium-minus */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* COMMA: reducere P134 unarium-minus */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* SEMICOLON: reducere P134 unarium-minus */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* PAREN_CLAUSA: reducere P134 unarium-minus */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 134, 0 },  /* QUADRA_CLAUSA: reducere P134 unarium-minus */
    /* --- status 145 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* BRACE_CLAUSA: reducere P135 unarium-tilde */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* PLUS: reducere P135 unarium-tilde */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* MINUS: reducere P135 unarium-tilde */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* STAR: reducere P135 unarium-tilde */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* SOLIDUS: reducere P135 unarium-tilde */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* PERCENTUM: reducere P135 unarium-tilde */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* AMPERSAND: reducere P135 unarium-tilde */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* BARRA: reducere P135 unarium-tilde */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* CARET: reducere P135 unarium-tilde */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* ET_ET: reducere P135 unarium-tilde */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* VEL_VEL: reducere P135 unarium-tilde */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* AEQUALIS_AEQUALIS: reducere P135 unarium-tilde */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* NON_AEQUALIS: reducere P135 unarium-tilde */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* MINOR: reducere P135 unarium-tilde */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* MAIOR: reducere P135 unarium-tilde */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* MINOR_AEQUALIS: reducere P135 unarium-tilde */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* MAIOR_AEQUALIS: reducere P135 unarium-tilde */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* SINISTRORSUM: reducere P135 unarium-tilde */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* DEXTRORSUM: reducere P135 unarium-tilde */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* ASSIGNATIO: reducere P135 unarium-tilde */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* PLUS_ASSIGNATIO: reducere P135 unarium-tilde */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* MINUS_ASSIGNATIO: reducere P135 unarium-tilde */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* STAR_ASSIGNATIO: reducere P135 unarium-tilde */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P135 unarium-tilde */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P135 unarium-tilde */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P135 unarium-tilde */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* BARRA_ASSIGNATIO: reducere P135 unarium-tilde */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* CARET_ASSIGNATIO: reducere P135 unarium-tilde */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P135 unarium-tilde */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P135 unarium-tilde */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* QUAESTIO: reducere P135 unarium-tilde */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* COLON: reducere P135 unarium-tilde */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* COMMA: reducere P135 unarium-tilde */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* SEMICOLON: reducere P135 unarium-tilde */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* PAREN_CLAUSA: reducere P135 unarium-tilde */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 135, 0 },  /* QUADRA_CLAUSA: reducere P135 unarium-tilde */
    /* --- status 146 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* BRACE_CLAUSA: reducere P136 unarium-exclamatio */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* PLUS: reducere P136 unarium-exclamatio */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* MINUS: reducere P136 unarium-exclamatio */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* STAR: reducere P136 unarium-exclamatio */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* SOLIDUS: reducere P136 unarium-exclamatio */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* PERCENTUM: reducere P136 unarium-exclamatio */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* AMPERSAND: reducere P136 unarium-exclamatio */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* BARRA: reducere P136 unarium-exclamatio */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* CARET: reducere P136 unarium-exclamatio */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* ET_ET: reducere P136 unarium-exclamatio */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* VEL_VEL: reducere P136 unarium-exclamatio */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* AEQUALIS_AEQUALIS: reducere P136 unarium-exclamatio */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* NON_AEQUALIS: reducere P136 unarium-exclamatio */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* MINOR: reducere P136 unarium-exclamatio */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* MAIOR: reducere P136 unarium-exclamatio */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* MINOR_AEQUALIS: reducere P136 unarium-exclamatio */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* MAIOR_AEQUALIS: reducere P136 unarium-exclamatio */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* SINISTRORSUM: reducere P136 unarium-exclamatio */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* DEXTRORSUM: reducere P136 unarium-exclamatio */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* PLUS_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* MINUS_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* STAR_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* BARRA_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* CARET_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P136 unarium-exclamatio */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* QUAESTIO: reducere P136 unarium-exclamatio */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* COLON: reducere P136 unarium-exclamatio */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* COMMA: reducere P136 unarium-exclamatio */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* SEMICOLON: reducere P136 unarium-exclamatio */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* PAREN_CLAUSA: reducere P136 unarium-exclamatio */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 136, 0 },  /* QUADRA_CLAUSA: reducere P136 unarium-exclamatio */
    /* --- status 147 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* BRACE_CLAUSA: reducere P137 magnitudo-expressionis */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* PLUS: reducere P137 magnitudo-expressionis */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* MINUS: reducere P137 magnitudo-expressionis */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* STAR: reducere P137 magnitudo-expressionis */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* SOLIDUS: reducere P137 magnitudo-expressionis */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* PERCENTUM: reducere P137 magnitudo-expressionis */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* AMPERSAND: reducere P137 magnitudo-expressionis */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* BARRA: reducere P137 magnitudo-expressionis */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* CARET: reducere P137 magnitudo-expressionis */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* ET_ET: reducere P137 magnitudo-expressionis */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* VEL_VEL: reducere P137 magnitudo-expressionis */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* AEQUALIS_AEQUALIS: reducere P137 magnitudo-expressionis */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* NON_AEQUALIS: reducere P137 magnitudo-expressionis */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* MINOR: reducere P137 magnitudo-expressionis */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* MAIOR: reducere P137 magnitudo-expressionis */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* MINOR_AEQUALIS: reducere P137 magnitudo-expressionis */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* MAIOR_AEQUALIS: reducere P137 magnitudo-expressionis */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* SINISTRORSUM: reducere P137 magnitudo-expressionis */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* DEXTRORSUM: reducere P137 magnitudo-expressionis */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* PLUS_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* MINUS_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* STAR_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* BARRA_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* CARET_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P137 magnitudo-expressionis */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* QUAESTIO: reducere P137 magnitudo-expressionis */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* COLON: reducere P137 magnitudo-expressionis */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* COMMA: reducere P137 magnitudo-expressionis */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* SEMICOLON: reducere P137 magnitudo-expressionis */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* PAREN_CLAUSA: reducere P137 magnitudo-expressionis */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 137, 0 },  /* QUADRA_CLAUSA: reducere P137 magnitudo-expressionis */
    /* --- status 148 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,   7, 0 },  /* IDENTIFICATOR: transponere -> 7 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 149 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 150 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 184, 0 },  /* PAREN_CLAUSA: reducere P184 */
    /* --- status 151 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 222, 0 },  /* IDENTIFICATOR: transponere -> 222 */
    /* --- status 152 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 223, 0 },  /* IDENTIFICATOR: transponere -> 223 */
    /* --- status 153 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* BRACE_CLAUSA: reducere P181 postcrementum-incrementum */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* PLUS: reducere P181 postcrementum-incrementum */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* MINUS: reducere P181 postcrementum-incrementum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* STAR: reducere P181 postcrementum-incrementum */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* SOLIDUS: reducere P181 postcrementum-incrementum */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* PERCENTUM: reducere P181 postcrementum-incrementum */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* AMPERSAND: reducere P181 postcrementum-incrementum */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* BARRA: reducere P181 postcrementum-incrementum */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* CARET: reducere P181 postcrementum-incrementum */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* ET_ET: reducere P181 postcrementum-incrementum */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* VEL_VEL: reducere P181 postcrementum-incrementum */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* AEQUALIS_AEQUALIS: reducere P181 postcrementum-incrementum */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* NON_AEQUALIS: reducere P181 postcrementum-incrementum */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* MINOR: reducere P181 postcrementum-incrementum */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* MAIOR: reducere P181 postcrementum-incrementum */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* MINOR_AEQUALIS: reducere P181 postcrementum-incrementum */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* MAIOR_AEQUALIS: reducere P181 postcrementum-incrementum */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* SINISTRORSUM: reducere P181 postcrementum-incrementum */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* DEXTRORSUM: reducere P181 postcrementum-incrementum */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* PLUS_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* MINUS_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* STAR_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* BARRA_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* CARET_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P181 postcrementum-incrementum */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* INCREMENTUM: reducere P181 postcrementum-incrementum */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* DECREMENTUM: reducere P181 postcrementum-incrementum */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* PUNCTUM: reducere P181 postcrementum-incrementum */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* SAGITTA: reducere P181 postcrementum-incrementum */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* QUAESTIO: reducere P181 postcrementum-incrementum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* COLON: reducere P181 postcrementum-incrementum */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* COMMA: reducere P181 postcrementum-incrementum */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* SEMICOLON: reducere P181 postcrementum-incrementum */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* PAREN_APERTA: reducere P181 postcrementum-incrementum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* PAREN_CLAUSA: reducere P181 postcrementum-incrementum */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* QUADRA_APERTA: reducere P181 postcrementum-incrementum */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 181, 0 },  /* QUADRA_CLAUSA: reducere P181 postcrementum-incrementum */
    /* --- status 154 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* BRACE_CLAUSA: reducere P182 postcrementum-decrementum */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* PLUS: reducere P182 postcrementum-decrementum */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* MINUS: reducere P182 postcrementum-decrementum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* STAR: reducere P182 postcrementum-decrementum */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* SOLIDUS: reducere P182 postcrementum-decrementum */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* PERCENTUM: reducere P182 postcrementum-decrementum */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* AMPERSAND: reducere P182 postcrementum-decrementum */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* BARRA: reducere P182 postcrementum-decrementum */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* CARET: reducere P182 postcrementum-decrementum */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* ET_ET: reducere P182 postcrementum-decrementum */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* VEL_VEL: reducere P182 postcrementum-decrementum */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* AEQUALIS_AEQUALIS: reducere P182 postcrementum-decrementum */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* NON_AEQUALIS: reducere P182 postcrementum-decrementum */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* MINOR: reducere P182 postcrementum-decrementum */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* MAIOR: reducere P182 postcrementum-decrementum */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* MINOR_AEQUALIS: reducere P182 postcrementum-decrementum */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* MAIOR_AEQUALIS: reducere P182 postcrementum-decrementum */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* SINISTRORSUM: reducere P182 postcrementum-decrementum */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* DEXTRORSUM: reducere P182 postcrementum-decrementum */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* PLUS_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* MINUS_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* STAR_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* BARRA_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* CARET_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P182 postcrementum-decrementum */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* INCREMENTUM: reducere P182 postcrementum-decrementum */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* DECREMENTUM: reducere P182 postcrementum-decrementum */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* PUNCTUM: reducere P182 postcrementum-decrementum */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* SAGITTA: reducere P182 postcrementum-decrementum */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* QUAESTIO: reducere P182 postcrementum-decrementum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* COLON: reducere P182 postcrementum-decrementum */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* COMMA: reducere P182 postcrementum-decrementum */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* SEMICOLON: reducere P182 postcrementum-decrementum */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* PAREN_APERTA: reducere P182 postcrementum-decrementum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* PAREN_CLAUSA: reducere P182 postcrementum-decrementum */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* QUADRA_APERTA: reducere P182 postcrementum-decrementum */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 182, 0 },  /* QUADRA_CLAUSA: reducere P182 postcrementum-decrementum */
    /* --- status 155 --- */
    {   4, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* STRING_LIT: reducere P189 chordae-appendere */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* BRACE_CLAUSA: reducere P189 chordae-appendere */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* PLUS: reducere P189 chordae-appendere */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* MINUS: reducere P189 chordae-appendere */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* STAR: reducere P189 chordae-appendere */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* SOLIDUS: reducere P189 chordae-appendere */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* PERCENTUM: reducere P189 chordae-appendere */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* AMPERSAND: reducere P189 chordae-appendere */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* BARRA: reducere P189 chordae-appendere */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* CARET: reducere P189 chordae-appendere */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* ET_ET: reducere P189 chordae-appendere */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* VEL_VEL: reducere P189 chordae-appendere */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* AEQUALIS_AEQUALIS: reducere P189 chordae-appendere */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* NON_AEQUALIS: reducere P189 chordae-appendere */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* MINOR: reducere P189 chordae-appendere */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* MAIOR: reducere P189 chordae-appendere */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* MINOR_AEQUALIS: reducere P189 chordae-appendere */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* MAIOR_AEQUALIS: reducere P189 chordae-appendere */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* SINISTRORSUM: reducere P189 chordae-appendere */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* DEXTRORSUM: reducere P189 chordae-appendere */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* ASSIGNATIO: reducere P189 chordae-appendere */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* PLUS_ASSIGNATIO: reducere P189 chordae-appendere */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* MINUS_ASSIGNATIO: reducere P189 chordae-appendere */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* STAR_ASSIGNATIO: reducere P189 chordae-appendere */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P189 chordae-appendere */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P189 chordae-appendere */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P189 chordae-appendere */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* BARRA_ASSIGNATIO: reducere P189 chordae-appendere */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* CARET_ASSIGNATIO: reducere P189 chordae-appendere */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P189 chordae-appendere */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P189 chordae-appendere */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* INCREMENTUM: reducere P189 chordae-appendere */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* DECREMENTUM: reducere P189 chordae-appendere */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* PUNCTUM: reducere P189 chordae-appendere */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* SAGITTA: reducere P189 chordae-appendere */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* QUAESTIO: reducere P189 chordae-appendere */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* COLON: reducere P189 chordae-appendere */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* COMMA: reducere P189 chordae-appendere */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* SEMICOLON: reducere P189 chordae-appendere */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* PAREN_APERTA: reducere P189 chordae-appendere */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* PAREN_CLAUSA: reducere P189 chordae-appendere */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* QUADRA_APERTA: reducere P189 chordae-appendere */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 189, 0 },  /* QUADRA_CLAUSA: reducere P189 chordae-appendere */
    /* --- status 156 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* IDENTIFICATOR: reducere P4 declaratio-plena */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* INTEGER: reducere P4 declaratio-plena */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* FLOAT: reducere P4 declaratio-plena */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* CHARACTER_LIT: reducere P4 declaratio-plena */
    {   4, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* STRING_LIT: reducere P4 declaratio-plena */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* VOID: reducere P4 declaratio-plena */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* CHAR: reducere P4 declaratio-plena */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* SHORT: reducere P4 declaratio-plena */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* INT: reducere P4 declaratio-plena */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* LONG: reducere P4 declaratio-plena */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* FLOAT_KW: reducere P4 declaratio-plena */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* DOUBLE: reducere P4 declaratio-plena */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* SIGNED: reducere P4 declaratio-plena */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* UNSIGNED: reducere P4 declaratio-plena */
    {  14, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* SIZEOF: reducere P4 declaratio-plena */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* TYPEDEF: reducere P4 declaratio-plena */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* EXTERN: reducere P4 declaratio-plena */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* STATIC: reducere P4 declaratio-plena */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* AUTO: reducere P4 declaratio-plena */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* REGISTER: reducere P4 declaratio-plena */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* CONST: reducere P4 declaratio-plena */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* VOLATILE: reducere P4 declaratio-plena */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* STRUCT: reducere P4 declaratio-plena */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* UNION: reducere P4 declaratio-plena */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* ENUM: reducere P4 declaratio-plena */
    {  28, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* PLUS: reducere P4 declaratio-plena */
    {  29, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* MINUS: reducere P4 declaratio-plena */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* STAR: reducere P4 declaratio-plena */
    {  33, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* AMPERSAND: reducere P4 declaratio-plena */
    {  36, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* TILDE: reducere P4 declaratio-plena */
    {  37, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* EXCLAMATIO: reducere P4 declaratio-plena */
    {  59, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* INCREMENTUM: reducere P4 declaratio-plena */
    {  60, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* DECREMENTUM: reducere P4 declaratio-plena */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* PAREN_APERTA: reducere P4 declaratio-plena */
    {  71, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* EOF: reducere P4 declaratio-plena */
    /* --- status 157 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* IDENTIFICATOR: transponere -> 76 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  74, 0 },  /* STAR: transponere -> 74 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  77, 0 },  /* PAREN_APERTA: transponere -> 77 */
    /* --- status 158 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE, 227, 0 },  /* BRACE_APERTA: transponere -> 227 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 159 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* IDENTIFICATOR: transponere -> 76 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE, 229, 0 },  /* CONST: transponere -> 229 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE, 230, 0 },  /* VOLATILE: transponere -> 230 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  74, 0 },  /* STAR: transponere -> 74 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  77, 0 },  /* PAREN_APERTA: transponere -> 77 */
    /* --- status 160 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    {  70, SILVA_TAB_ACTIO_TRANSPONERE, 232, 0 },  /* QUADRA_CLAUSA: transponere -> 232 */
    /* --- status 161 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* PAREN_CLAUSA: reducere P74 */
    /* --- status 162 --- */
    {  68, SILVA_TAB_ACTIO_TRANSPONERE, 239, 0 },  /* PAREN_CLAUSA: transponere -> 239 */
    /* --- status 163 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* IDENTIFICATOR: reducere P48 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* VOID: reducere P48 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CHAR: reducere P48 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SHORT: reducere P48 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* INT: reducere P48 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* LONG: reducere P48 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* FLOAT_KW: reducere P48 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* DOUBLE: reducere P48 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SIGNED: reducere P48 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* UNSIGNED: reducere P48 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CONST: reducere P48 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* VOLATILE: reducere P48 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* STRUCT: reducere P48 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* UNION: reducere P48 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* ENUM: reducere P48 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* BRACE_CLAUSA: reducere P48 */
    /* --- status 164 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  27, SILVA_TAB_ACTIO_TRANSPONERE, 241, 0 },  /* BRACE_CLAUSA: transponere -> 241 */
    /* --- status 165 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* IDENTIFICATOR: reducere P48 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* VOID: reducere P48 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CHAR: reducere P48 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SHORT: reducere P48 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* INT: reducere P48 */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* LONG: reducere P48 */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* FLOAT_KW: reducere P48 */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* DOUBLE: reducere P48 */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* SIGNED: reducere P48 */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* UNSIGNED: reducere P48 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* CONST: reducere P48 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* VOLATILE: reducere P48 */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* STRUCT: reducere P48 */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* UNION: reducere P48 */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* ENUM: reducere P48 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  48, 0 },  /* BRACE_CLAUSA: reducere P48 */
    /* --- status 166 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  27, SILVA_TAB_ACTIO_TRANSPONERE, 245, 0 },  /* BRACE_CLAUSA: transponere -> 245 */
    /* --- status 167 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 170, 0 },  /* IDENTIFICATOR: transponere -> 170 */
    /* --- status 168 --- */
    {  27, SILVA_TAB_ACTIO_TRANSPONERE, 247, 0 },  /* BRACE_CLAUSA: transponere -> 247 */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE, 248, 0 },  /* COMMA: transponere -> 248 */
    /* --- status 169 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* BRACE_CLAUSA: reducere P59 enumeratores-primum */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  59, 0 },  /* COMMA: reducere P59 enumeratores-primum */
    /* --- status 170 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  61, 0 },  /* BRACE_CLAUSA: reducere P61 enumerator */
    {  48, SILVA_TAB_ACTIO_TRANSPONERE, 249, 0 },  /* ASSIGNATIO: transponere -> 249 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  61, 0 },  /* COMMA: reducere P61 enumerator */
    /* --- status 171 --- */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* COLON: reducere P83 virgula-series */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* COMMA: reducere P83 virgula-series */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* SEMICOLON: reducere P83 virgula-series */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* PAREN_CLAUSA: reducere P83 virgula-series */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  83, 0 },  /* QUADRA_CLAUSA: reducere P83 virgula-series */
    /* --- status 172 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* BRACE_CLAUSA: reducere P85 assignatio-simplex */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* COLON: reducere P85 assignatio-simplex */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* COMMA: reducere P85 assignatio-simplex */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* SEMICOLON: reducere P85 assignatio-simplex */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* PAREN_CLAUSA: reducere P85 assignatio-simplex */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  85, 0 },  /* QUADRA_CLAUSA: reducere P85 assignatio-simplex */
    /* --- status 173 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  86, 0 },  /* BRACE_CLAUSA: reducere P86 assignatio-plus */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  86, 0 },  /* COLON: reducere P86 assignatio-plus */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  86, 0 },  /* COMMA: reducere P86 assignatio-plus */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  86, 0 },  /* SEMICOLON: reducere P86 assignatio-plus */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  86, 0 },  /* PAREN_CLAUSA: reducere P86 assignatio-plus */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  86, 0 },  /* QUADRA_CLAUSA: reducere P86 assignatio-plus */
    /* --- status 174 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  87, 0 },  /* BRACE_CLAUSA: reducere P87 assignatio-minus */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  87, 0 },  /* COLON: reducere P87 assignatio-minus */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  87, 0 },  /* COMMA: reducere P87 assignatio-minus */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  87, 0 },  /* SEMICOLON: reducere P87 assignatio-minus */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  87, 0 },  /* PAREN_CLAUSA: reducere P87 assignatio-minus */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  87, 0 },  /* QUADRA_CLAUSA: reducere P87 assignatio-minus */
    /* --- status 175 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  88, 0 },  /* BRACE_CLAUSA: reducere P88 assignatio-stella */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  88, 0 },  /* COLON: reducere P88 assignatio-stella */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  88, 0 },  /* COMMA: reducere P88 assignatio-stella */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  88, 0 },  /* SEMICOLON: reducere P88 assignatio-stella */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  88, 0 },  /* PAREN_CLAUSA: reducere P88 assignatio-stella */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  88, 0 },  /* QUADRA_CLAUSA: reducere P88 assignatio-stella */
    /* --- status 176 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  89, 0 },  /* BRACE_CLAUSA: reducere P89 assignatio-solidus */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  89, 0 },  /* COLON: reducere P89 assignatio-solidus */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  89, 0 },  /* COMMA: reducere P89 assignatio-solidus */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  89, 0 },  /* SEMICOLON: reducere P89 assignatio-solidus */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  89, 0 },  /* PAREN_CLAUSA: reducere P89 assignatio-solidus */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  89, 0 },  /* QUADRA_CLAUSA: reducere P89 assignatio-solidus */
    /* --- status 177 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* BRACE_CLAUSA: reducere P90 assignatio-percentum */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* COLON: reducere P90 assignatio-percentum */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* COMMA: reducere P90 assignatio-percentum */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* SEMICOLON: reducere P90 assignatio-percentum */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* PAREN_CLAUSA: reducere P90 assignatio-percentum */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  90, 0 },  /* QUADRA_CLAUSA: reducere P90 assignatio-percentum */
    /* --- status 178 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* BRACE_CLAUSA: reducere P91 assignatio-ampersand */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* COLON: reducere P91 assignatio-ampersand */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* COMMA: reducere P91 assignatio-ampersand */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* SEMICOLON: reducere P91 assignatio-ampersand */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* PAREN_CLAUSA: reducere P91 assignatio-ampersand */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  91, 0 },  /* QUADRA_CLAUSA: reducere P91 assignatio-ampersand */
    /* --- status 179 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* BRACE_CLAUSA: reducere P92 assignatio-barra */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* COLON: reducere P92 assignatio-barra */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* COMMA: reducere P92 assignatio-barra */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* SEMICOLON: reducere P92 assignatio-barra */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* PAREN_CLAUSA: reducere P92 assignatio-barra */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  92, 0 },  /* QUADRA_CLAUSA: reducere P92 assignatio-barra */
    /* --- status 180 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* BRACE_CLAUSA: reducere P93 assignatio-caret */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* COLON: reducere P93 assignatio-caret */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* COMMA: reducere P93 assignatio-caret */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* SEMICOLON: reducere P93 assignatio-caret */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* PAREN_CLAUSA: reducere P93 assignatio-caret */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  93, 0 },  /* QUADRA_CLAUSA: reducere P93 assignatio-caret */
    /* --- status 181 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* BRACE_CLAUSA: reducere P94 assignatio-sinistrorsum */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* COLON: reducere P94 assignatio-sinistrorsum */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* COMMA: reducere P94 assignatio-sinistrorsum */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* SEMICOLON: reducere P94 assignatio-sinistrorsum */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* PAREN_CLAUSA: reducere P94 assignatio-sinistrorsum */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  94, 0 },  /* QUADRA_CLAUSA: reducere P94 assignatio-sinistrorsum */
    /* --- status 182 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* BRACE_CLAUSA: reducere P95 assignatio-dextrorsum */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* COLON: reducere P95 assignatio-dextrorsum */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* COMMA: reducere P95 assignatio-dextrorsum */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* SEMICOLON: reducere P95 assignatio-dextrorsum */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* PAREN_CLAUSA: reducere P95 assignatio-dextrorsum */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  95, 0 },  /* QUADRA_CLAUSA: reducere P95 assignatio-dextrorsum */
    /* --- status 183 --- */
    {  64, SILVA_TAB_ACTIO_TRANSPONERE, 250, 0 },  /* COLON: transponere -> 250 */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE,  96, 0 },  /* COMMA: transponere -> 96 */
    /* --- status 184 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  99, 0 },  /* BRACE_CLAUSA: reducere P99 binarium-disiunctio */
    {  38, SILVA_TAB_ACTIO_TRANSPONERE, 110, 0 },  /* ET_ET: transponere -> 110 */
    {  39, SILVA_TAB_ACTIO_REDUCERE,  99, 0 },  /* VEL_VEL: reducere P99 binarium-disiunctio */
    {  63, SILVA_TAB_ACTIO_REDUCERE,  99, 0 },  /* QUAESTIO: reducere P99 binarium-disiunctio */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  99, 0 },  /* COLON: reducere P99 binarium-disiunctio */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  99, 0 },  /* COMMA: reducere P99 binarium-disiunctio */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  99, 0 },  /* SEMICOLON: reducere P99 binarium-disiunctio */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  99, 0 },  /* PAREN_CLAUSA: reducere P99 binarium-disiunctio */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  99, 0 },  /* QUADRA_CLAUSA: reducere P99 binarium-disiunctio */
    /* --- status 185 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 101, 0 },  /* BRACE_CLAUSA: reducere P101 binarium-coniunctio */
    {  34, SILVA_TAB_ACTIO_TRANSPONERE, 111, 0 },  /* BARRA: transponere -> 111 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 101, 0 },  /* ET_ET: reducere P101 binarium-coniunctio */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 101, 0 },  /* VEL_VEL: reducere P101 binarium-coniunctio */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 101, 0 },  /* QUAESTIO: reducere P101 binarium-coniunctio */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 101, 0 },  /* COLON: reducere P101 binarium-coniunctio */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 101, 0 },  /* COMMA: reducere P101 binarium-coniunctio */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 101, 0 },  /* SEMICOLON: reducere P101 binarium-coniunctio */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 101, 0 },  /* PAREN_CLAUSA: reducere P101 binarium-coniunctio */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 101, 0 },  /* QUADRA_CLAUSA: reducere P101 binarium-coniunctio */
    /* --- status 186 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* BRACE_CLAUSA: reducere P103 binarium-vel */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* BARRA: reducere P103 binarium-vel */
    {  35, SILVA_TAB_ACTIO_TRANSPONERE, 112, 0 },  /* CARET: transponere -> 112 */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* ET_ET: reducere P103 binarium-vel */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* VEL_VEL: reducere P103 binarium-vel */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* QUAESTIO: reducere P103 binarium-vel */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* COLON: reducere P103 binarium-vel */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* COMMA: reducere P103 binarium-vel */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* SEMICOLON: reducere P103 binarium-vel */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* PAREN_CLAUSA: reducere P103 binarium-vel */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 103, 0 },  /* QUADRA_CLAUSA: reducere P103 binarium-vel */
    /* --- status 187 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* BRACE_CLAUSA: reducere P105 binarium-aut */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE, 113, 0 },  /* AMPERSAND: transponere -> 113 */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* BARRA: reducere P105 binarium-aut */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* CARET: reducere P105 binarium-aut */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* ET_ET: reducere P105 binarium-aut */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* VEL_VEL: reducere P105 binarium-aut */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* QUAESTIO: reducere P105 binarium-aut */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* COLON: reducere P105 binarium-aut */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* COMMA: reducere P105 binarium-aut */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* SEMICOLON: reducere P105 binarium-aut */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* PAREN_CLAUSA: reducere P105 binarium-aut */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 105, 0 },  /* QUADRA_CLAUSA: reducere P105 binarium-aut */
    /* --- status 188 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* BRACE_CLAUSA: reducere P107 binarium-et */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* AMPERSAND: reducere P107 binarium-et */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* BARRA: reducere P107 binarium-et */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* CARET: reducere P107 binarium-et */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* ET_ET: reducere P107 binarium-et */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* VEL_VEL: reducere P107 binarium-et */
    {  40, SILVA_TAB_ACTIO_TRANSPONERE, 114, 0 },  /* AEQUALIS_AEQUALIS: transponere -> 114 */
    {  41, SILVA_TAB_ACTIO_TRANSPONERE, 115, 0 },  /* NON_AEQUALIS: transponere -> 115 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* QUAESTIO: reducere P107 binarium-et */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* COLON: reducere P107 binarium-et */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* COMMA: reducere P107 binarium-et */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* SEMICOLON: reducere P107 binarium-et */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* PAREN_CLAUSA: reducere P107 binarium-et */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 107, 0 },  /* QUADRA_CLAUSA: reducere P107 binarium-et */
    /* --- status 189 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* BRACE_CLAUSA: reducere P109 binarium-aequalis */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* AMPERSAND: reducere P109 binarium-aequalis */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* BARRA: reducere P109 binarium-aequalis */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* CARET: reducere P109 binarium-aequalis */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* ET_ET: reducere P109 binarium-aequalis */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* VEL_VEL: reducere P109 binarium-aequalis */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* AEQUALIS_AEQUALIS: reducere P109 binarium-aequalis */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* NON_AEQUALIS: reducere P109 binarium-aequalis */
    {  42, SILVA_TAB_ACTIO_TRANSPONERE, 116, 0 },  /* MINOR: transponere -> 116 */
    {  43, SILVA_TAB_ACTIO_TRANSPONERE, 117, 0 },  /* MAIOR: transponere -> 117 */
    {  44, SILVA_TAB_ACTIO_TRANSPONERE, 118, 0 },  /* MINOR_AEQUALIS: transponere -> 118 */
    {  45, SILVA_TAB_ACTIO_TRANSPONERE, 119, 0 },  /* MAIOR_AEQUALIS: transponere -> 119 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* QUAESTIO: reducere P109 binarium-aequalis */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* COLON: reducere P109 binarium-aequalis */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* COMMA: reducere P109 binarium-aequalis */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* SEMICOLON: reducere P109 binarium-aequalis */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* PAREN_CLAUSA: reducere P109 binarium-aequalis */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 109, 0 },  /* QUADRA_CLAUSA: reducere P109 binarium-aequalis */
    /* --- status 190 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* BRACE_CLAUSA: reducere P110 binarium-non-aequalis */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* AMPERSAND: reducere P110 binarium-non-aequalis */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* BARRA: reducere P110 binarium-non-aequalis */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* CARET: reducere P110 binarium-non-aequalis */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* ET_ET: reducere P110 binarium-non-aequalis */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* VEL_VEL: reducere P110 binarium-non-aequalis */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* AEQUALIS_AEQUALIS: reducere P110 binarium-non-aequalis */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* NON_AEQUALIS: reducere P110 binarium-non-aequalis */
    {  42, SILVA_TAB_ACTIO_TRANSPONERE, 116, 0 },  /* MINOR: transponere -> 116 */
    {  43, SILVA_TAB_ACTIO_TRANSPONERE, 117, 0 },  /* MAIOR: transponere -> 117 */
    {  44, SILVA_TAB_ACTIO_TRANSPONERE, 118, 0 },  /* MINOR_AEQUALIS: transponere -> 118 */
    {  45, SILVA_TAB_ACTIO_TRANSPONERE, 119, 0 },  /* MAIOR_AEQUALIS: transponere -> 119 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* QUAESTIO: reducere P110 binarium-non-aequalis */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* COLON: reducere P110 binarium-non-aequalis */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* COMMA: reducere P110 binarium-non-aequalis */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* SEMICOLON: reducere P110 binarium-non-aequalis */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* PAREN_CLAUSA: reducere P110 binarium-non-aequalis */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 110, 0 },  /* QUADRA_CLAUSA: reducere P110 binarium-non-aequalis */
    /* --- status 191 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* BRACE_CLAUSA: reducere P112 binarium-minor */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* AMPERSAND: reducere P112 binarium-minor */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* BARRA: reducere P112 binarium-minor */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* CARET: reducere P112 binarium-minor */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* ET_ET: reducere P112 binarium-minor */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* VEL_VEL: reducere P112 binarium-minor */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* AEQUALIS_AEQUALIS: reducere P112 binarium-minor */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* NON_AEQUALIS: reducere P112 binarium-minor */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* MINOR: reducere P112 binarium-minor */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* MAIOR: reducere P112 binarium-minor */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* MINOR_AEQUALIS: reducere P112 binarium-minor */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* MAIOR_AEQUALIS: reducere P112 binarium-minor */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE, 120, 0 },  /* SINISTRORSUM: transponere -> 120 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE, 121, 0 },  /* DEXTRORSUM: transponere -> 121 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* QUAESTIO: reducere P112 binarium-minor */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* COLON: reducere P112 binarium-minor */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* COMMA: reducere P112 binarium-minor */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* SEMICOLON: reducere P112 binarium-minor */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* PAREN_CLAUSA: reducere P112 binarium-minor */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 112, 0 },  /* QUADRA_CLAUSA: reducere P112 binarium-minor */
    /* --- status 192 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* BRACE_CLAUSA: reducere P113 binarium-maior */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* AMPERSAND: reducere P113 binarium-maior */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* BARRA: reducere P113 binarium-maior */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* CARET: reducere P113 binarium-maior */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* ET_ET: reducere P113 binarium-maior */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* VEL_VEL: reducere P113 binarium-maior */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* AEQUALIS_AEQUALIS: reducere P113 binarium-maior */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* NON_AEQUALIS: reducere P113 binarium-maior */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* MINOR: reducere P113 binarium-maior */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* MAIOR: reducere P113 binarium-maior */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* MINOR_AEQUALIS: reducere P113 binarium-maior */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* MAIOR_AEQUALIS: reducere P113 binarium-maior */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE, 120, 0 },  /* SINISTRORSUM: transponere -> 120 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE, 121, 0 },  /* DEXTRORSUM: transponere -> 121 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* QUAESTIO: reducere P113 binarium-maior */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* COLON: reducere P113 binarium-maior */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* COMMA: reducere P113 binarium-maior */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* SEMICOLON: reducere P113 binarium-maior */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* PAREN_CLAUSA: reducere P113 binarium-maior */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 113, 0 },  /* QUADRA_CLAUSA: reducere P113 binarium-maior */
    /* --- status 193 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* BRACE_CLAUSA: reducere P114 binarium-minor-aequalis */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* AMPERSAND: reducere P114 binarium-minor-aequalis */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* BARRA: reducere P114 binarium-minor-aequalis */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* CARET: reducere P114 binarium-minor-aequalis */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* ET_ET: reducere P114 binarium-minor-aequalis */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* VEL_VEL: reducere P114 binarium-minor-aequalis */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* AEQUALIS_AEQUALIS: reducere P114 binarium-minor-aequalis */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* NON_AEQUALIS: reducere P114 binarium-minor-aequalis */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* MINOR: reducere P114 binarium-minor-aequalis */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* MAIOR: reducere P114 binarium-minor-aequalis */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* MINOR_AEQUALIS: reducere P114 binarium-minor-aequalis */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* MAIOR_AEQUALIS: reducere P114 binarium-minor-aequalis */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE, 120, 0 },  /* SINISTRORSUM: transponere -> 120 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE, 121, 0 },  /* DEXTRORSUM: transponere -> 121 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* QUAESTIO: reducere P114 binarium-minor-aequalis */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* COLON: reducere P114 binarium-minor-aequalis */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* COMMA: reducere P114 binarium-minor-aequalis */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* SEMICOLON: reducere P114 binarium-minor-aequalis */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* PAREN_CLAUSA: reducere P114 binarium-minor-aequalis */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 114, 0 },  /* QUADRA_CLAUSA: reducere P114 binarium-minor-aequalis */
    /* --- status 194 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* BRACE_CLAUSA: reducere P115 binarium-maior-aequalis */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* AMPERSAND: reducere P115 binarium-maior-aequalis */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* BARRA: reducere P115 binarium-maior-aequalis */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* CARET: reducere P115 binarium-maior-aequalis */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* ET_ET: reducere P115 binarium-maior-aequalis */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* VEL_VEL: reducere P115 binarium-maior-aequalis */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* AEQUALIS_AEQUALIS: reducere P115 binarium-maior-aequalis */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* NON_AEQUALIS: reducere P115 binarium-maior-aequalis */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* MINOR: reducere P115 binarium-maior-aequalis */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* MAIOR: reducere P115 binarium-maior-aequalis */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* MINOR_AEQUALIS: reducere P115 binarium-maior-aequalis */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* MAIOR_AEQUALIS: reducere P115 binarium-maior-aequalis */
    {  46, SILVA_TAB_ACTIO_TRANSPONERE, 120, 0 },  /* SINISTRORSUM: transponere -> 120 */
    {  47, SILVA_TAB_ACTIO_TRANSPONERE, 121, 0 },  /* DEXTRORSUM: transponere -> 121 */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* QUAESTIO: reducere P115 binarium-maior-aequalis */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* COLON: reducere P115 binarium-maior-aequalis */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* COMMA: reducere P115 binarium-maior-aequalis */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* SEMICOLON: reducere P115 binarium-maior-aequalis */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* PAREN_CLAUSA: reducere P115 binarium-maior-aequalis */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 115, 0 },  /* QUADRA_CLAUSA: reducere P115 binarium-maior-aequalis */
    /* --- status 195 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* BRACE_CLAUSA: reducere P117 binarium-sinistrorsum */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE, 122, 0 },  /* PLUS: transponere -> 122 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE, 123, 0 },  /* MINUS: transponere -> 123 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* AMPERSAND: reducere P117 binarium-sinistrorsum */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* BARRA: reducere P117 binarium-sinistrorsum */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* CARET: reducere P117 binarium-sinistrorsum */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* ET_ET: reducere P117 binarium-sinistrorsum */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* VEL_VEL: reducere P117 binarium-sinistrorsum */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* AEQUALIS_AEQUALIS: reducere P117 binarium-sinistrorsum */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* NON_AEQUALIS: reducere P117 binarium-sinistrorsum */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* MINOR: reducere P117 binarium-sinistrorsum */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* MAIOR: reducere P117 binarium-sinistrorsum */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* MINOR_AEQUALIS: reducere P117 binarium-sinistrorsum */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* MAIOR_AEQUALIS: reducere P117 binarium-sinistrorsum */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* SINISTRORSUM: reducere P117 binarium-sinistrorsum */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* DEXTRORSUM: reducere P117 binarium-sinistrorsum */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* QUAESTIO: reducere P117 binarium-sinistrorsum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* COLON: reducere P117 binarium-sinistrorsum */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* COMMA: reducere P117 binarium-sinistrorsum */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* SEMICOLON: reducere P117 binarium-sinistrorsum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* PAREN_CLAUSA: reducere P117 binarium-sinistrorsum */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 117, 0 },  /* QUADRA_CLAUSA: reducere P117 binarium-sinistrorsum */
    /* --- status 196 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* BRACE_CLAUSA: reducere P118 binarium-dextrorsum */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE, 122, 0 },  /* PLUS: transponere -> 122 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE, 123, 0 },  /* MINUS: transponere -> 123 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* AMPERSAND: reducere P118 binarium-dextrorsum */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* BARRA: reducere P118 binarium-dextrorsum */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* CARET: reducere P118 binarium-dextrorsum */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* ET_ET: reducere P118 binarium-dextrorsum */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* VEL_VEL: reducere P118 binarium-dextrorsum */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* AEQUALIS_AEQUALIS: reducere P118 binarium-dextrorsum */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* NON_AEQUALIS: reducere P118 binarium-dextrorsum */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* MINOR: reducere P118 binarium-dextrorsum */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* MAIOR: reducere P118 binarium-dextrorsum */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* MINOR_AEQUALIS: reducere P118 binarium-dextrorsum */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* MAIOR_AEQUALIS: reducere P118 binarium-dextrorsum */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* SINISTRORSUM: reducere P118 binarium-dextrorsum */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* DEXTRORSUM: reducere P118 binarium-dextrorsum */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* QUAESTIO: reducere P118 binarium-dextrorsum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* COLON: reducere P118 binarium-dextrorsum */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* COMMA: reducere P118 binarium-dextrorsum */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* SEMICOLON: reducere P118 binarium-dextrorsum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* PAREN_CLAUSA: reducere P118 binarium-dextrorsum */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 118, 0 },  /* QUADRA_CLAUSA: reducere P118 binarium-dextrorsum */
    /* --- status 197 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* BRACE_CLAUSA: reducere P120 binarium-plus */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* PLUS: reducere P120 binarium-plus */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* MINUS: reducere P120 binarium-plus */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE, 124, 0 },  /* STAR: transponere -> 124 */
    {  31, SILVA_TAB_ACTIO_TRANSPONERE, 125, 0 },  /* SOLIDUS: transponere -> 125 */
    {  32, SILVA_TAB_ACTIO_TRANSPONERE, 126, 0 },  /* PERCENTUM: transponere -> 126 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* AMPERSAND: reducere P120 binarium-plus */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* BARRA: reducere P120 binarium-plus */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* CARET: reducere P120 binarium-plus */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* ET_ET: reducere P120 binarium-plus */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* VEL_VEL: reducere P120 binarium-plus */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* AEQUALIS_AEQUALIS: reducere P120 binarium-plus */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* NON_AEQUALIS: reducere P120 binarium-plus */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* MINOR: reducere P120 binarium-plus */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* MAIOR: reducere P120 binarium-plus */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* MINOR_AEQUALIS: reducere P120 binarium-plus */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* MAIOR_AEQUALIS: reducere P120 binarium-plus */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* SINISTRORSUM: reducere P120 binarium-plus */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* DEXTRORSUM: reducere P120 binarium-plus */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* QUAESTIO: reducere P120 binarium-plus */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* COLON: reducere P120 binarium-plus */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* COMMA: reducere P120 binarium-plus */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* SEMICOLON: reducere P120 binarium-plus */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* PAREN_CLAUSA: reducere P120 binarium-plus */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 120, 0 },  /* QUADRA_CLAUSA: reducere P120 binarium-plus */
    /* --- status 198 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* BRACE_CLAUSA: reducere P121 binarium-minus */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* PLUS: reducere P121 binarium-minus */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* MINUS: reducere P121 binarium-minus */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE, 124, 0 },  /* STAR: transponere -> 124 */
    {  31, SILVA_TAB_ACTIO_TRANSPONERE, 125, 0 },  /* SOLIDUS: transponere -> 125 */
    {  32, SILVA_TAB_ACTIO_TRANSPONERE, 126, 0 },  /* PERCENTUM: transponere -> 126 */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* AMPERSAND: reducere P121 binarium-minus */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* BARRA: reducere P121 binarium-minus */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* CARET: reducere P121 binarium-minus */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* ET_ET: reducere P121 binarium-minus */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* VEL_VEL: reducere P121 binarium-minus */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* AEQUALIS_AEQUALIS: reducere P121 binarium-minus */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* NON_AEQUALIS: reducere P121 binarium-minus */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* MINOR: reducere P121 binarium-minus */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* MAIOR: reducere P121 binarium-minus */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* MINOR_AEQUALIS: reducere P121 binarium-minus */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* MAIOR_AEQUALIS: reducere P121 binarium-minus */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* SINISTRORSUM: reducere P121 binarium-minus */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* DEXTRORSUM: reducere P121 binarium-minus */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* QUAESTIO: reducere P121 binarium-minus */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* COLON: reducere P121 binarium-minus */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* COMMA: reducere P121 binarium-minus */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* SEMICOLON: reducere P121 binarium-minus */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* PAREN_CLAUSA: reducere P121 binarium-minus */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 121, 0 },  /* QUADRA_CLAUSA: reducere P121 binarium-minus */
    /* --- status 199 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* BRACE_CLAUSA: reducere P123 binarium-stella */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* PLUS: reducere P123 binarium-stella */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* MINUS: reducere P123 binarium-stella */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* STAR: reducere P123 binarium-stella */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* SOLIDUS: reducere P123 binarium-stella */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* PERCENTUM: reducere P123 binarium-stella */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* AMPERSAND: reducere P123 binarium-stella */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* BARRA: reducere P123 binarium-stella */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* CARET: reducere P123 binarium-stella */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* ET_ET: reducere P123 binarium-stella */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* VEL_VEL: reducere P123 binarium-stella */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* AEQUALIS_AEQUALIS: reducere P123 binarium-stella */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* NON_AEQUALIS: reducere P123 binarium-stella */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* MINOR: reducere P123 binarium-stella */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* MAIOR: reducere P123 binarium-stella */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* MINOR_AEQUALIS: reducere P123 binarium-stella */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* MAIOR_AEQUALIS: reducere P123 binarium-stella */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* SINISTRORSUM: reducere P123 binarium-stella */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* DEXTRORSUM: reducere P123 binarium-stella */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* QUAESTIO: reducere P123 binarium-stella */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* COLON: reducere P123 binarium-stella */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* COMMA: reducere P123 binarium-stella */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* SEMICOLON: reducere P123 binarium-stella */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* PAREN_CLAUSA: reducere P123 binarium-stella */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 123, 0 },  /* QUADRA_CLAUSA: reducere P123 binarium-stella */
    /* --- status 200 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* BRACE_CLAUSA: reducere P124 binarium-solidus */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* PLUS: reducere P124 binarium-solidus */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* MINUS: reducere P124 binarium-solidus */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* STAR: reducere P124 binarium-solidus */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* SOLIDUS: reducere P124 binarium-solidus */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* PERCENTUM: reducere P124 binarium-solidus */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* AMPERSAND: reducere P124 binarium-solidus */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* BARRA: reducere P124 binarium-solidus */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* CARET: reducere P124 binarium-solidus */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* ET_ET: reducere P124 binarium-solidus */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* VEL_VEL: reducere P124 binarium-solidus */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* AEQUALIS_AEQUALIS: reducere P124 binarium-solidus */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* NON_AEQUALIS: reducere P124 binarium-solidus */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* MINOR: reducere P124 binarium-solidus */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* MAIOR: reducere P124 binarium-solidus */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* MINOR_AEQUALIS: reducere P124 binarium-solidus */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* MAIOR_AEQUALIS: reducere P124 binarium-solidus */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* SINISTRORSUM: reducere P124 binarium-solidus */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* DEXTRORSUM: reducere P124 binarium-solidus */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* QUAESTIO: reducere P124 binarium-solidus */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* COLON: reducere P124 binarium-solidus */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* COMMA: reducere P124 binarium-solidus */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* SEMICOLON: reducere P124 binarium-solidus */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* PAREN_CLAUSA: reducere P124 binarium-solidus */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 124, 0 },  /* QUADRA_CLAUSA: reducere P124 binarium-solidus */
    /* --- status 201 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* BRACE_CLAUSA: reducere P125 binarium-percentum */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* PLUS: reducere P125 binarium-percentum */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* MINUS: reducere P125 binarium-percentum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* STAR: reducere P125 binarium-percentum */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* SOLIDUS: reducere P125 binarium-percentum */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* PERCENTUM: reducere P125 binarium-percentum */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* AMPERSAND: reducere P125 binarium-percentum */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* BARRA: reducere P125 binarium-percentum */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* CARET: reducere P125 binarium-percentum */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* ET_ET: reducere P125 binarium-percentum */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* VEL_VEL: reducere P125 binarium-percentum */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* AEQUALIS_AEQUALIS: reducere P125 binarium-percentum */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* NON_AEQUALIS: reducere P125 binarium-percentum */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* MINOR: reducere P125 binarium-percentum */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* MAIOR: reducere P125 binarium-percentum */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* MINOR_AEQUALIS: reducere P125 binarium-percentum */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* MAIOR_AEQUALIS: reducere P125 binarium-percentum */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* SINISTRORSUM: reducere P125 binarium-percentum */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* DEXTRORSUM: reducere P125 binarium-percentum */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* QUAESTIO: reducere P125 binarium-percentum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* COLON: reducere P125 binarium-percentum */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* COMMA: reducere P125 binarium-percentum */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* SEMICOLON: reducere P125 binarium-percentum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* PAREN_CLAUSA: reducere P125 binarium-percentum */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 125, 0 },  /* QUADRA_CLAUSA: reducere P125 binarium-percentum */
    /* --- status 202 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* BRACE_CLAUSA: reducere P195 primarium-parenthesis */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* PLUS: reducere P195 primarium-parenthesis */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* MINUS: reducere P195 primarium-parenthesis */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* STAR: reducere P195 primarium-parenthesis */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* SOLIDUS: reducere P195 primarium-parenthesis */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* PERCENTUM: reducere P195 primarium-parenthesis */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* AMPERSAND: reducere P195 primarium-parenthesis */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* BARRA: reducere P195 primarium-parenthesis */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* CARET: reducere P195 primarium-parenthesis */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* ET_ET: reducere P195 primarium-parenthesis */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* VEL_VEL: reducere P195 primarium-parenthesis */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* AEQUALIS_AEQUALIS: reducere P195 primarium-parenthesis */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* NON_AEQUALIS: reducere P195 primarium-parenthesis */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* MINOR: reducere P195 primarium-parenthesis */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* MAIOR: reducere P195 primarium-parenthesis */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* MINOR_AEQUALIS: reducere P195 primarium-parenthesis */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* MAIOR_AEQUALIS: reducere P195 primarium-parenthesis */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* SINISTRORSUM: reducere P195 primarium-parenthesis */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* DEXTRORSUM: reducere P195 primarium-parenthesis */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* PLUS_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* MINUS_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* STAR_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* BARRA_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* CARET_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P195 primarium-parenthesis */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* INCREMENTUM: reducere P195 primarium-parenthesis */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* DECREMENTUM: reducere P195 primarium-parenthesis */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* PUNCTUM: reducere P195 primarium-parenthesis */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* SAGITTA: reducere P195 primarium-parenthesis */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* QUAESTIO: reducere P195 primarium-parenthesis */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* COLON: reducere P195 primarium-parenthesis */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* COMMA: reducere P195 primarium-parenthesis */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* SEMICOLON: reducere P195 primarium-parenthesis */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* PAREN_APERTA: reducere P195 primarium-parenthesis */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* PAREN_CLAUSA: reducere P195 primarium-parenthesis */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* QUADRA_APERTA: reducere P195 primarium-parenthesis */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 195, 0 },  /* QUADRA_CLAUSA: reducere P195 primarium-parenthesis */
    /* --- status 203 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 204 --- */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 140, 0 },  /* PAREN_CLAUSA: reducere P140 species-typi-plena */
    /* --- status 205 --- */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* CONST: reducere P65 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* VOLATILE: reducere P65 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* STAR: reducere P65 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* PAREN_APERTA: reducere P65 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* PAREN_CLAUSA: reducere P65 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* QUADRA_APERTA: reducere P65 */
    /* --- status 206 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 169, 0 },  /* COMMA: reducere P169 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 254, 0 },  /* PAREN_APERTA: transponere -> 254 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 169, 0 },  /* PAREN_CLAUSA: reducere P169 */
    {  69, SILVA_TAB_ACTIO_TRANSPONERE, 253, 0 },  /* QUADRA_APERTA: transponere -> 253 */
    /* --- status 207 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE, 205, 0 },  /* STAR: transponere -> 205 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 207, 0 },  /* PAREN_APERTA: transponere -> 207 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* PAREN_CLAUSA: reducere P74 */
    {  69, SILVA_TAB_ACTIO_TRANSPONERE, 208, 0 },  /* QUADRA_APERTA: transponere -> 208 */
    /* --- status 208 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    {  70, SILVA_TAB_ACTIO_TRANSPONERE, 258, 0 },  /* QUADRA_CLAUSA: transponere -> 258 */
    /* --- status 209 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* IDENTIFICATOR: reducere P149 sq-pa-ex-primitivo */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* VOID: reducere P149 sq-pa-ex-primitivo */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* CHAR: reducere P149 sq-pa-ex-primitivo */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* SHORT: reducere P149 sq-pa-ex-primitivo */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* INT: reducere P149 sq-pa-ex-primitivo */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* LONG: reducere P149 sq-pa-ex-primitivo */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* FLOAT_KW: reducere P149 sq-pa-ex-primitivo */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* DOUBLE: reducere P149 sq-pa-ex-primitivo */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* SIGNED: reducere P149 sq-pa-ex-primitivo */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* UNSIGNED: reducere P149 sq-pa-ex-primitivo */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* CONST: reducere P149 sq-pa-ex-primitivo */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* VOLATILE: reducere P149 sq-pa-ex-primitivo */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* STRUCT: reducere P149 sq-pa-ex-primitivo */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* UNION: reducere P149 sq-pa-ex-primitivo */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* ENUM: reducere P149 sq-pa-ex-primitivo */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* STAR: reducere P149 sq-pa-ex-primitivo */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* COLON: reducere P149 sq-pa-ex-primitivo */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* PAREN_APERTA: reducere P149 sq-pa-ex-primitivo */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* PAREN_CLAUSA: reducere P149 sq-pa-ex-primitivo */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 149, 0 },  /* QUADRA_APERTA: reducere P149 sq-pa-ex-primitivo */
    /* --- status 210 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* IDENTIFICATOR: reducere P153 sq-pt-ex-primitivo */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* VOID: reducere P153 sq-pt-ex-primitivo */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* CHAR: reducere P153 sq-pt-ex-primitivo */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* SHORT: reducere P153 sq-pt-ex-primitivo */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* INT: reducere P153 sq-pt-ex-primitivo */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* LONG: reducere P153 sq-pt-ex-primitivo */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* FLOAT_KW: reducere P153 sq-pt-ex-primitivo */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* DOUBLE: reducere P153 sq-pt-ex-primitivo */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* SIGNED: reducere P153 sq-pt-ex-primitivo */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* UNSIGNED: reducere P153 sq-pt-ex-primitivo */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* CONST: reducere P153 sq-pt-ex-primitivo */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* VOLATILE: reducere P153 sq-pt-ex-primitivo */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* STRUCT: reducere P153 sq-pt-ex-primitivo */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* UNION: reducere P153 sq-pt-ex-primitivo */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* ENUM: reducere P153 sq-pt-ex-primitivo */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* STAR: reducere P153 sq-pt-ex-primitivo */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* COLON: reducere P153 sq-pt-ex-primitivo */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* PAREN_APERTA: reducere P153 sq-pt-ex-primitivo */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* PAREN_CLAUSA: reducere P153 sq-pt-ex-primitivo */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 153, 0 },  /* QUADRA_APERTA: reducere P153 sq-pt-ex-primitivo */
    /* --- status 211 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* IDENTIFICATOR: reducere P146 sq-pp-ex-alio */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* CONST: reducere P146 sq-pp-ex-alio */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* VOLATILE: reducere P146 sq-pp-ex-alio */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* STRUCT: reducere P146 sq-pp-ex-alio */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* UNION: reducere P146 sq-pp-ex-alio */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* ENUM: reducere P146 sq-pp-ex-alio */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* STAR: reducere P146 sq-pp-ex-alio */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* COLON: reducere P146 sq-pp-ex-alio */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* PAREN_APERTA: reducere P146 sq-pp-ex-alio */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* PAREN_CLAUSA: reducere P146 sq-pp-ex-alio */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 146, 0 },  /* QUADRA_APERTA: reducere P146 sq-pp-ex-alio */
    /* --- status 212 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* IDENTIFICATOR: reducere P150 sq-pa-ex-alio */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* VOID: reducere P150 sq-pa-ex-alio */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* CHAR: reducere P150 sq-pa-ex-alio */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* SHORT: reducere P150 sq-pa-ex-alio */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* INT: reducere P150 sq-pa-ex-alio */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* LONG: reducere P150 sq-pa-ex-alio */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* FLOAT_KW: reducere P150 sq-pa-ex-alio */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* DOUBLE: reducere P150 sq-pa-ex-alio */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* SIGNED: reducere P150 sq-pa-ex-alio */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* UNSIGNED: reducere P150 sq-pa-ex-alio */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* CONST: reducere P150 sq-pa-ex-alio */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* VOLATILE: reducere P150 sq-pa-ex-alio */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* STRUCT: reducere P150 sq-pa-ex-alio */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* UNION: reducere P150 sq-pa-ex-alio */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* ENUM: reducere P150 sq-pa-ex-alio */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* STAR: reducere P150 sq-pa-ex-alio */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* COLON: reducere P150 sq-pa-ex-alio */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* PAREN_APERTA: reducere P150 sq-pa-ex-alio */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* PAREN_CLAUSA: reducere P150 sq-pa-ex-alio */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 150, 0 },  /* QUADRA_APERTA: reducere P150 sq-pa-ex-alio */
    /* --- status 213 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* IDENTIFICATOR: reducere P154 sq-pt-ex-alio */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* VOID: reducere P154 sq-pt-ex-alio */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* CHAR: reducere P154 sq-pt-ex-alio */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* SHORT: reducere P154 sq-pt-ex-alio */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* INT: reducere P154 sq-pt-ex-alio */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* LONG: reducere P154 sq-pt-ex-alio */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* FLOAT_KW: reducere P154 sq-pt-ex-alio */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* DOUBLE: reducere P154 sq-pt-ex-alio */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* SIGNED: reducere P154 sq-pt-ex-alio */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* UNSIGNED: reducere P154 sq-pt-ex-alio */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* CONST: reducere P154 sq-pt-ex-alio */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* VOLATILE: reducere P154 sq-pt-ex-alio */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* STRUCT: reducere P154 sq-pt-ex-alio */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* UNION: reducere P154 sq-pt-ex-alio */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* ENUM: reducere P154 sq-pt-ex-alio */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* STAR: reducere P154 sq-pt-ex-alio */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* COLON: reducere P154 sq-pt-ex-alio */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* PAREN_APERTA: reducere P154 sq-pt-ex-alio */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* PAREN_CLAUSA: reducere P154 sq-pt-ex-alio */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 154, 0 },  /* QUADRA_APERTA: reducere P154 sq-pt-ex-alio */
    /* --- status 214 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* IDENTIFICATOR: reducere P147 sq-pp-ex-tag */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* CONST: reducere P147 sq-pp-ex-tag */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* VOLATILE: reducere P147 sq-pp-ex-tag */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* STRUCT: reducere P147 sq-pp-ex-tag */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* UNION: reducere P147 sq-pp-ex-tag */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* ENUM: reducere P147 sq-pp-ex-tag */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* STAR: reducere P147 sq-pp-ex-tag */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* COLON: reducere P147 sq-pp-ex-tag */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* PAREN_APERTA: reducere P147 sq-pp-ex-tag */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* PAREN_CLAUSA: reducere P147 sq-pp-ex-tag */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 147, 0 },  /* QUADRA_APERTA: reducere P147 sq-pp-ex-tag */
    /* --- status 215 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* IDENTIFICATOR: reducere P151 sq-pa-ex-tag */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* VOID: reducere P151 sq-pa-ex-tag */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* CHAR: reducere P151 sq-pa-ex-tag */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* SHORT: reducere P151 sq-pa-ex-tag */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* INT: reducere P151 sq-pa-ex-tag */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* LONG: reducere P151 sq-pa-ex-tag */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* FLOAT_KW: reducere P151 sq-pa-ex-tag */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* DOUBLE: reducere P151 sq-pa-ex-tag */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* SIGNED: reducere P151 sq-pa-ex-tag */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* UNSIGNED: reducere P151 sq-pa-ex-tag */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* CONST: reducere P151 sq-pa-ex-tag */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* VOLATILE: reducere P151 sq-pa-ex-tag */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* STRUCT: reducere P151 sq-pa-ex-tag */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* UNION: reducere P151 sq-pa-ex-tag */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* ENUM: reducere P151 sq-pa-ex-tag */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* STAR: reducere P151 sq-pa-ex-tag */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* COLON: reducere P151 sq-pa-ex-tag */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* PAREN_APERTA: reducere P151 sq-pa-ex-tag */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* PAREN_CLAUSA: reducere P151 sq-pa-ex-tag */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 151, 0 },  /* QUADRA_APERTA: reducere P151 sq-pa-ex-tag */
    /* --- status 216 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* IDENTIFICATOR: reducere P155 sq-pt-ex-tag */
    {   5, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* VOID: reducere P155 sq-pt-ex-tag */
    {   6, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* CHAR: reducere P155 sq-pt-ex-tag */
    {   7, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* SHORT: reducere P155 sq-pt-ex-tag */
    {   8, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* INT: reducere P155 sq-pt-ex-tag */
    {   9, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* LONG: reducere P155 sq-pt-ex-tag */
    {  10, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* FLOAT_KW: reducere P155 sq-pt-ex-tag */
    {  11, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* DOUBLE: reducere P155 sq-pt-ex-tag */
    {  12, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* SIGNED: reducere P155 sq-pt-ex-tag */
    {  13, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* UNSIGNED: reducere P155 sq-pt-ex-tag */
    {  20, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* CONST: reducere P155 sq-pt-ex-tag */
    {  21, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* VOLATILE: reducere P155 sq-pt-ex-tag */
    {  23, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* STRUCT: reducere P155 sq-pt-ex-tag */
    {  24, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* UNION: reducere P155 sq-pt-ex-tag */
    {  25, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* ENUM: reducere P155 sq-pt-ex-tag */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* STAR: reducere P155 sq-pt-ex-tag */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* COLON: reducere P155 sq-pt-ex-tag */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* PAREN_APERTA: reducere P155 sq-pt-ex-tag */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* PAREN_CLAUSA: reducere P155 sq-pt-ex-tag */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 155, 0 },  /* QUADRA_APERTA: reducere P155 sq-pt-ex-tag */
    /* --- status 217 --- */
    {  68, SILVA_TAB_ACTIO_TRANSPONERE, 259, 0 },  /* PAREN_CLAUSA: transponere -> 259 */
    /* --- status 218 --- */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE,  96, 0 },  /* COMMA: transponere -> 96 */
    {  70, SILVA_TAB_ACTIO_TRANSPONERE, 260, 0 },  /* QUADRA_CLAUSA: transponere -> 260 */
    /* --- status 219 --- */
    {  68, SILVA_TAB_ACTIO_TRANSPONERE, 261, 0 },  /* PAREN_CLAUSA: transponere -> 261 */
    /* --- status 220 --- */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE, 262, 0 },  /* COMMA: transponere -> 262 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 185, 0 },  /* PAREN_CLAUSA: reducere P185 */
    /* --- status 221 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 186, 0 },  /* COMMA: reducere P186 argumenta-primum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 186, 0 },  /* PAREN_CLAUSA: reducere P186 argumenta-primum */
    /* --- status 222 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* BRACE_CLAUSA: reducere P179 accessus-punctum */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* PLUS: reducere P179 accessus-punctum */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* MINUS: reducere P179 accessus-punctum */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* STAR: reducere P179 accessus-punctum */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* SOLIDUS: reducere P179 accessus-punctum */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* PERCENTUM: reducere P179 accessus-punctum */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* AMPERSAND: reducere P179 accessus-punctum */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* BARRA: reducere P179 accessus-punctum */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* CARET: reducere P179 accessus-punctum */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* ET_ET: reducere P179 accessus-punctum */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* VEL_VEL: reducere P179 accessus-punctum */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* AEQUALIS_AEQUALIS: reducere P179 accessus-punctum */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* NON_AEQUALIS: reducere P179 accessus-punctum */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* MINOR: reducere P179 accessus-punctum */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* MAIOR: reducere P179 accessus-punctum */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* MINOR_AEQUALIS: reducere P179 accessus-punctum */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* MAIOR_AEQUALIS: reducere P179 accessus-punctum */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* SINISTRORSUM: reducere P179 accessus-punctum */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* DEXTRORSUM: reducere P179 accessus-punctum */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* ASSIGNATIO: reducere P179 accessus-punctum */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* PLUS_ASSIGNATIO: reducere P179 accessus-punctum */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* MINUS_ASSIGNATIO: reducere P179 accessus-punctum */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* STAR_ASSIGNATIO: reducere P179 accessus-punctum */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P179 accessus-punctum */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P179 accessus-punctum */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P179 accessus-punctum */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* BARRA_ASSIGNATIO: reducere P179 accessus-punctum */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* CARET_ASSIGNATIO: reducere P179 accessus-punctum */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P179 accessus-punctum */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P179 accessus-punctum */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* INCREMENTUM: reducere P179 accessus-punctum */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* DECREMENTUM: reducere P179 accessus-punctum */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* PUNCTUM: reducere P179 accessus-punctum */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* SAGITTA: reducere P179 accessus-punctum */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* QUAESTIO: reducere P179 accessus-punctum */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* COLON: reducere P179 accessus-punctum */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* COMMA: reducere P179 accessus-punctum */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* SEMICOLON: reducere P179 accessus-punctum */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* PAREN_APERTA: reducere P179 accessus-punctum */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* PAREN_CLAUSA: reducere P179 accessus-punctum */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* QUADRA_APERTA: reducere P179 accessus-punctum */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 179, 0 },  /* QUADRA_CLAUSA: reducere P179 accessus-punctum */
    /* --- status 223 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* BRACE_CLAUSA: reducere P180 accessus-sagitta */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* PLUS: reducere P180 accessus-sagitta */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* MINUS: reducere P180 accessus-sagitta */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* STAR: reducere P180 accessus-sagitta */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* SOLIDUS: reducere P180 accessus-sagitta */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* PERCENTUM: reducere P180 accessus-sagitta */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* AMPERSAND: reducere P180 accessus-sagitta */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* BARRA: reducere P180 accessus-sagitta */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* CARET: reducere P180 accessus-sagitta */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* ET_ET: reducere P180 accessus-sagitta */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* VEL_VEL: reducere P180 accessus-sagitta */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* AEQUALIS_AEQUALIS: reducere P180 accessus-sagitta */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* NON_AEQUALIS: reducere P180 accessus-sagitta */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* MINOR: reducere P180 accessus-sagitta */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* MAIOR: reducere P180 accessus-sagitta */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* MINOR_AEQUALIS: reducere P180 accessus-sagitta */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* MAIOR_AEQUALIS: reducere P180 accessus-sagitta */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* SINISTRORSUM: reducere P180 accessus-sagitta */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* DEXTRORSUM: reducere P180 accessus-sagitta */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* ASSIGNATIO: reducere P180 accessus-sagitta */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* PLUS_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* MINUS_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* STAR_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* BARRA_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* CARET_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P180 accessus-sagitta */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* INCREMENTUM: reducere P180 accessus-sagitta */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* DECREMENTUM: reducere P180 accessus-sagitta */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* PUNCTUM: reducere P180 accessus-sagitta */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* SAGITTA: reducere P180 accessus-sagitta */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* QUAESTIO: reducere P180 accessus-sagitta */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* COLON: reducere P180 accessus-sagitta */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* COMMA: reducere P180 accessus-sagitta */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* SEMICOLON: reducere P180 accessus-sagitta */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* PAREN_APERTA: reducere P180 accessus-sagitta */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* PAREN_CLAUSA: reducere P180 accessus-sagitta */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* QUADRA_APERTA: reducere P180 accessus-sagitta */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 180, 0 },  /* QUADRA_CLAUSA: reducere P180 accessus-sagitta */
    /* --- status 224 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* COMMA: reducere P34 declaratores-appendere */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  34, 0 },  /* SEMICOLON: reducere P34 declaratores-appendere */
    /* --- status 225 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* COMMA: reducere P35 declarator-initiatus */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  35, 0 },  /* SEMICOLON: reducere P35 declarator-initiatus */
    /* --- status 226 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* BRACE_CLAUSA: reducere P37 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* COMMA: reducere P37 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  37, 0 },  /* SEMICOLON: reducere P37 */
    /* --- status 227 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE, 227, 0 },  /* BRACE_APERTA: transponere -> 227 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 228 --- */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  63, 0 },  /* ASSIGNATIO: reducere P63 declarator-monstrator */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  63, 0 },  /* COLON: reducere P63 declarator-monstrator */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  63, 0 },  /* COMMA: reducere P63 declarator-monstrator */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  63, 0 },  /* SEMICOLON: reducere P63 declarator-monstrator */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  63, 0 },  /* PAREN_CLAUSA: reducere P63 declarator-monstrator */
    /* --- status 229 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* IDENTIFICATOR: reducere P66 qualificatores-const */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* CONST: reducere P66 qualificatores-const */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* VOLATILE: reducere P66 qualificatores-const */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* STAR: reducere P66 qualificatores-const */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* COMMA: reducere P66 qualificatores-const */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* PAREN_APERTA: reducere P66 qualificatores-const */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* PAREN_CLAUSA: reducere P66 qualificatores-const */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  66, 0 },  /* QUADRA_APERTA: reducere P66 qualificatores-const */
    /* --- status 230 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* IDENTIFICATOR: reducere P67 qualificatores-volatile */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* CONST: reducere P67 qualificatores-volatile */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* VOLATILE: reducere P67 qualificatores-volatile */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* STAR: reducere P67 qualificatores-volatile */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* COMMA: reducere P67 qualificatores-volatile */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* PAREN_APERTA: reducere P67 qualificatores-volatile */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* PAREN_CLAUSA: reducere P67 qualificatores-volatile */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  67, 0 },  /* QUADRA_APERTA: reducere P67 qualificatores-volatile */
    /* --- status 231 --- */
    {  70, SILVA_TAB_ACTIO_TRANSPONERE, 265, 0 },  /* QUADRA_CLAUSA: transponere -> 265 */
    /* --- status 232 --- */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* ASSIGNATIO: reducere P71 declarator-aciei-vacua */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* COLON: reducere P71 declarator-aciei-vacua */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* COMMA: reducere P71 declarator-aciei-vacua */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* SEMICOLON: reducere P71 declarator-aciei-vacua */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* PAREN_APERTA: reducere P71 declarator-aciei-vacua */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* PAREN_CLAUSA: reducere P71 declarator-aciei-vacua */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  71, 0 },  /* QUADRA_APERTA: reducere P71 declarator-aciei-vacua */
    /* --- status 233 --- */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  73, 0 },  /* QUADRA_CLAUSA: reducere P73 */
    /* --- status 234 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* IDENTIFICATOR: reducere P17 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* STAR: reducere P17 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* COMMA: reducere P17 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* PAREN_APERTA: reducere P17 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* PAREN_CLAUSA: reducere P17 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* QUADRA_APERTA: reducere P17 */
    /* --- status 235 --- */
    {  68, SILVA_TAB_ACTIO_TRANSPONERE, 266, 0 },  /* PAREN_CLAUSA: transponere -> 266 */
    /* --- status 236 --- */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE, 267, 0 },  /* COMMA: transponere -> 267 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  75, 0 },  /* PAREN_CLAUSA: reducere P75 */
    /* --- status 237 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* COMMA: reducere P76 parametra-primum */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  76, 0 },  /* PAREN_CLAUSA: reducere P76 parametra-primum */
    /* --- status 238 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* IDENTIFICATOR: transponere -> 76 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE, 268, 0 },  /* STAR: transponere -> 268 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* COMMA: reducere P79 parametrum-nudum */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 269, 0 },  /* PAREN_APERTA: transponere -> 269 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  79, 0 },  /* PAREN_CLAUSA: reducere P79 parametrum-nudum */
    {  69, SILVA_TAB_ACTIO_TRANSPONERE, 208, 0 },  /* QUADRA_APERTA: transponere -> 208 */
    /* --- status 239 --- */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* ASSIGNATIO: reducere P69 declarator-parenthesis */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* COLON: reducere P69 declarator-parenthesis */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* COMMA: reducere P69 declarator-parenthesis */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* SEMICOLON: reducere P69 declarator-parenthesis */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* PAREN_APERTA: reducere P69 declarator-parenthesis */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* PAREN_CLAUSA: reducere P69 declarator-parenthesis */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  69, 0 },  /* QUADRA_APERTA: reducere P69 declarator-parenthesis */
    /* --- status 240 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  27, SILVA_TAB_ACTIO_TRANSPONERE, 272, 0 },  /* BRACE_CLAUSA: transponere -> 272 */
    /* --- status 241 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* IDENTIFICATOR: reducere P43 structura-anonyma */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* VOID: reducere P43 structura-anonyma */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* CHAR: reducere P43 structura-anonyma */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* SHORT: reducere P43 structura-anonyma */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* INT: reducere P43 structura-anonyma */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* LONG: reducere P43 structura-anonyma */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* FLOAT_KW: reducere P43 structura-anonyma */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* DOUBLE: reducere P43 structura-anonyma */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* SIGNED: reducere P43 structura-anonyma */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* UNSIGNED: reducere P43 structura-anonyma */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* TYPEDEF: reducere P43 structura-anonyma */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* EXTERN: reducere P43 structura-anonyma */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* STATIC: reducere P43 structura-anonyma */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* AUTO: reducere P43 structura-anonyma */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* REGISTER: reducere P43 structura-anonyma */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* CONST: reducere P43 structura-anonyma */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* VOLATILE: reducere P43 structura-anonyma */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* STRUCT: reducere P43 structura-anonyma */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* UNION: reducere P43 structura-anonyma */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* ENUM: reducere P43 structura-anonyma */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* STAR: reducere P43 structura-anonyma */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* COLON: reducere P43 structura-anonyma */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* COMMA: reducere P43 structura-anonyma */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* SEMICOLON: reducere P43 structura-anonyma */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* PAREN_APERTA: reducere P43 structura-anonyma */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* PAREN_CLAUSA: reducere P43 structura-anonyma */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  43, 0 },  /* QUADRA_APERTA: reducere P43 structura-anonyma */
    /* --- status 242 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* IDENTIFICATOR: reducere P49 membra-appendere */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* VOID: reducere P49 membra-appendere */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* CHAR: reducere P49 membra-appendere */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SHORT: reducere P49 membra-appendere */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* INT: reducere P49 membra-appendere */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* LONG: reducere P49 membra-appendere */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* FLOAT_KW: reducere P49 membra-appendere */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* DOUBLE: reducere P49 membra-appendere */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* SIGNED: reducere P49 membra-appendere */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* UNSIGNED: reducere P49 membra-appendere */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* CONST: reducere P49 membra-appendere */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* VOLATILE: reducere P49 membra-appendere */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* STRUCT: reducere P49 membra-appendere */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* UNION: reducere P49 membra-appendere */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* ENUM: reducere P49 membra-appendere */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  49, 0 },  /* BRACE_CLAUSA: reducere P49 membra-appendere */
    /* --- status 243 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* IDENTIFICATOR: transponere -> 76 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  74, 0 },  /* STAR: transponere -> 74 */
    {  64, SILVA_TAB_ACTIO_TRANSPONERE, 276, 0 },  /* COLON: transponere -> 276 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  77, 0 },  /* PAREN_APERTA: transponere -> 77 */
    /* --- status 244 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  27, SILVA_TAB_ACTIO_TRANSPONERE, 277, 0 },  /* BRACE_CLAUSA: transponere -> 277 */
    /* --- status 245 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* IDENTIFICATOR: reducere P46 unio-anonyma */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* VOID: reducere P46 unio-anonyma */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* CHAR: reducere P46 unio-anonyma */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* SHORT: reducere P46 unio-anonyma */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* INT: reducere P46 unio-anonyma */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* LONG: reducere P46 unio-anonyma */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* FLOAT_KW: reducere P46 unio-anonyma */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* DOUBLE: reducere P46 unio-anonyma */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* SIGNED: reducere P46 unio-anonyma */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* UNSIGNED: reducere P46 unio-anonyma */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* TYPEDEF: reducere P46 unio-anonyma */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* EXTERN: reducere P46 unio-anonyma */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* STATIC: reducere P46 unio-anonyma */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* AUTO: reducere P46 unio-anonyma */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* REGISTER: reducere P46 unio-anonyma */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* CONST: reducere P46 unio-anonyma */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* VOLATILE: reducere P46 unio-anonyma */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* STRUCT: reducere P46 unio-anonyma */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* UNION: reducere P46 unio-anonyma */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* ENUM: reducere P46 unio-anonyma */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* STAR: reducere P46 unio-anonyma */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* COLON: reducere P46 unio-anonyma */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* COMMA: reducere P46 unio-anonyma */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* SEMICOLON: reducere P46 unio-anonyma */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* PAREN_APERTA: reducere P46 unio-anonyma */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* PAREN_CLAUSA: reducere P46 unio-anonyma */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  46, 0 },  /* QUADRA_APERTA: reducere P46 unio-anonyma */
    /* --- status 246 --- */
    {  27, SILVA_TAB_ACTIO_TRANSPONERE, 278, 0 },  /* BRACE_CLAUSA: transponere -> 278 */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE, 248, 0 },  /* COMMA: transponere -> 248 */
    /* --- status 247 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* IDENTIFICATOR: reducere P57 enumeratio-anonyma */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* VOID: reducere P57 enumeratio-anonyma */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* CHAR: reducere P57 enumeratio-anonyma */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* SHORT: reducere P57 enumeratio-anonyma */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* INT: reducere P57 enumeratio-anonyma */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* LONG: reducere P57 enumeratio-anonyma */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* FLOAT_KW: reducere P57 enumeratio-anonyma */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* DOUBLE: reducere P57 enumeratio-anonyma */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* SIGNED: reducere P57 enumeratio-anonyma */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* UNSIGNED: reducere P57 enumeratio-anonyma */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* TYPEDEF: reducere P57 enumeratio-anonyma */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* EXTERN: reducere P57 enumeratio-anonyma */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* STATIC: reducere P57 enumeratio-anonyma */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* AUTO: reducere P57 enumeratio-anonyma */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* REGISTER: reducere P57 enumeratio-anonyma */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* CONST: reducere P57 enumeratio-anonyma */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* VOLATILE: reducere P57 enumeratio-anonyma */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* STRUCT: reducere P57 enumeratio-anonyma */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* UNION: reducere P57 enumeratio-anonyma */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* ENUM: reducere P57 enumeratio-anonyma */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* STAR: reducere P57 enumeratio-anonyma */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* COLON: reducere P57 enumeratio-anonyma */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* COMMA: reducere P57 enumeratio-anonyma */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* SEMICOLON: reducere P57 enumeratio-anonyma */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* PAREN_APERTA: reducere P57 enumeratio-anonyma */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* PAREN_CLAUSA: reducere P57 enumeratio-anonyma */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  57, 0 },  /* QUADRA_APERTA: reducere P57 enumeratio-anonyma */
    /* --- status 248 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 170, 0 },  /* IDENTIFICATOR: transponere -> 170 */
    /* --- status 249 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 250 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 251 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* BRACE_CLAUSA: reducere P127 conversio-typus */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* PLUS: reducere P127 conversio-typus */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* MINUS: reducere P127 conversio-typus */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* STAR: reducere P127 conversio-typus */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* SOLIDUS: reducere P127 conversio-typus */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* PERCENTUM: reducere P127 conversio-typus */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* AMPERSAND: reducere P127 conversio-typus */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* BARRA: reducere P127 conversio-typus */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* CARET: reducere P127 conversio-typus */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* ET_ET: reducere P127 conversio-typus */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* VEL_VEL: reducere P127 conversio-typus */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* AEQUALIS_AEQUALIS: reducere P127 conversio-typus */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* NON_AEQUALIS: reducere P127 conversio-typus */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* MINOR: reducere P127 conversio-typus */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* MAIOR: reducere P127 conversio-typus */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* MINOR_AEQUALIS: reducere P127 conversio-typus */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* MAIOR_AEQUALIS: reducere P127 conversio-typus */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* SINISTRORSUM: reducere P127 conversio-typus */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* DEXTRORSUM: reducere P127 conversio-typus */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* ASSIGNATIO: reducere P127 conversio-typus */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* PLUS_ASSIGNATIO: reducere P127 conversio-typus */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* MINUS_ASSIGNATIO: reducere P127 conversio-typus */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* STAR_ASSIGNATIO: reducere P127 conversio-typus */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P127 conversio-typus */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P127 conversio-typus */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P127 conversio-typus */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* BARRA_ASSIGNATIO: reducere P127 conversio-typus */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* CARET_ASSIGNATIO: reducere P127 conversio-typus */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P127 conversio-typus */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P127 conversio-typus */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* QUAESTIO: reducere P127 conversio-typus */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* COLON: reducere P127 conversio-typus */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* COMMA: reducere P127 conversio-typus */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* SEMICOLON: reducere P127 conversio-typus */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* PAREN_CLAUSA: reducere P127 conversio-typus */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 127, 0 },  /* QUADRA_CLAUSA: reducere P127 conversio-typus */
    /* --- status 252 --- */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE, 229, 0 },  /* CONST: transponere -> 229 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE, 230, 0 },  /* VOLATILE: transponere -> 230 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE, 205, 0 },  /* STAR: transponere -> 205 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 207, 0 },  /* PAREN_APERTA: transponere -> 207 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 167, 0 },  /* PAREN_CLAUSA: reducere P167 declarator-abstractus-stella */
    {  69, SILVA_TAB_ACTIO_TRANSPONERE, 208, 0 },  /* QUADRA_APERTA: transponere -> 208 */
    /* --- status 253 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    {  70, SILVA_TAB_ACTIO_TRANSPONERE, 284, 0 },  /* QUADRA_CLAUSA: transponere -> 284 */
    /* --- status 254 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* PAREN_CLAUSA: reducere P74 */
    /* --- status 255 --- */
    {  68, SILVA_TAB_ACTIO_TRANSPONERE, 286, 0 },  /* PAREN_CLAUSA: transponere -> 286 */
    /* --- status 256 --- */
    {  68, SILVA_TAB_ACTIO_TRANSPONERE, 287, 0 },  /* PAREN_CLAUSA: transponere -> 287 */
    /* --- status 257 --- */
    {  70, SILVA_TAB_ACTIO_TRANSPONERE, 288, 0 },  /* QUADRA_CLAUSA: transponere -> 288 */
    /* --- status 258 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 174, 0 },  /* COMMA: reducere P174 abstractus-aciei-nuda */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 174, 0 },  /* PAREN_APERTA: reducere P174 abstractus-aciei-nuda */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 174, 0 },  /* PAREN_CLAUSA: reducere P174 abstractus-aciei-nuda */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 174, 0 },  /* QUADRA_APERTA: reducere P174 abstractus-aciei-nuda */
    /* --- status 259 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* BRACE_CLAUSA: reducere P138 magnitudo-typi */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* PLUS: reducere P138 magnitudo-typi */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* MINUS: reducere P138 magnitudo-typi */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* STAR: reducere P138 magnitudo-typi */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* SOLIDUS: reducere P138 magnitudo-typi */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* PERCENTUM: reducere P138 magnitudo-typi */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* AMPERSAND: reducere P138 magnitudo-typi */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* BARRA: reducere P138 magnitudo-typi */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* CARET: reducere P138 magnitudo-typi */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* ET_ET: reducere P138 magnitudo-typi */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* VEL_VEL: reducere P138 magnitudo-typi */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* AEQUALIS_AEQUALIS: reducere P138 magnitudo-typi */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* NON_AEQUALIS: reducere P138 magnitudo-typi */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* MINOR: reducere P138 magnitudo-typi */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* MAIOR: reducere P138 magnitudo-typi */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* MINOR_AEQUALIS: reducere P138 magnitudo-typi */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* MAIOR_AEQUALIS: reducere P138 magnitudo-typi */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* SINISTRORSUM: reducere P138 magnitudo-typi */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* DEXTRORSUM: reducere P138 magnitudo-typi */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* ASSIGNATIO: reducere P138 magnitudo-typi */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* PLUS_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* MINUS_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* STAR_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* BARRA_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* CARET_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P138 magnitudo-typi */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* QUAESTIO: reducere P138 magnitudo-typi */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* COLON: reducere P138 magnitudo-typi */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* COMMA: reducere P138 magnitudo-typi */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* SEMICOLON: reducere P138 magnitudo-typi */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* PAREN_CLAUSA: reducere P138 magnitudo-typi */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 138, 0 },  /* QUADRA_CLAUSA: reducere P138 magnitudo-typi */
    /* --- status 260 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* BRACE_CLAUSA: reducere P177 subscriptio-index */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* PLUS: reducere P177 subscriptio-index */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* MINUS: reducere P177 subscriptio-index */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* STAR: reducere P177 subscriptio-index */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* SOLIDUS: reducere P177 subscriptio-index */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* PERCENTUM: reducere P177 subscriptio-index */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* AMPERSAND: reducere P177 subscriptio-index */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* BARRA: reducere P177 subscriptio-index */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* CARET: reducere P177 subscriptio-index */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* ET_ET: reducere P177 subscriptio-index */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* VEL_VEL: reducere P177 subscriptio-index */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* AEQUALIS_AEQUALIS: reducere P177 subscriptio-index */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* NON_AEQUALIS: reducere P177 subscriptio-index */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* MINOR: reducere P177 subscriptio-index */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* MAIOR: reducere P177 subscriptio-index */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* MINOR_AEQUALIS: reducere P177 subscriptio-index */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* MAIOR_AEQUALIS: reducere P177 subscriptio-index */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* SINISTRORSUM: reducere P177 subscriptio-index */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* DEXTRORSUM: reducere P177 subscriptio-index */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* ASSIGNATIO: reducere P177 subscriptio-index */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* PLUS_ASSIGNATIO: reducere P177 subscriptio-index */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* MINUS_ASSIGNATIO: reducere P177 subscriptio-index */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* STAR_ASSIGNATIO: reducere P177 subscriptio-index */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P177 subscriptio-index */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P177 subscriptio-index */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P177 subscriptio-index */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* BARRA_ASSIGNATIO: reducere P177 subscriptio-index */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* CARET_ASSIGNATIO: reducere P177 subscriptio-index */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P177 subscriptio-index */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P177 subscriptio-index */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* INCREMENTUM: reducere P177 subscriptio-index */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* DECREMENTUM: reducere P177 subscriptio-index */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* PUNCTUM: reducere P177 subscriptio-index */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* SAGITTA: reducere P177 subscriptio-index */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* QUAESTIO: reducere P177 subscriptio-index */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* COLON: reducere P177 subscriptio-index */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* COMMA: reducere P177 subscriptio-index */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* SEMICOLON: reducere P177 subscriptio-index */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* PAREN_APERTA: reducere P177 subscriptio-index */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* PAREN_CLAUSA: reducere P177 subscriptio-index */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* QUADRA_APERTA: reducere P177 subscriptio-index */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 177, 0 },  /* QUADRA_CLAUSA: reducere P177 subscriptio-index */
    /* --- status 261 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* BRACE_CLAUSA: reducere P178 vocatio-argumenta */
    {  28, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* PLUS: reducere P178 vocatio-argumenta */
    {  29, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* MINUS: reducere P178 vocatio-argumenta */
    {  30, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* STAR: reducere P178 vocatio-argumenta */
    {  31, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* SOLIDUS: reducere P178 vocatio-argumenta */
    {  32, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* PERCENTUM: reducere P178 vocatio-argumenta */
    {  33, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* AMPERSAND: reducere P178 vocatio-argumenta */
    {  34, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* BARRA: reducere P178 vocatio-argumenta */
    {  35, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* CARET: reducere P178 vocatio-argumenta */
    {  38, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* ET_ET: reducere P178 vocatio-argumenta */
    {  39, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* VEL_VEL: reducere P178 vocatio-argumenta */
    {  40, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* AEQUALIS_AEQUALIS: reducere P178 vocatio-argumenta */
    {  41, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* NON_AEQUALIS: reducere P178 vocatio-argumenta */
    {  42, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* MINOR: reducere P178 vocatio-argumenta */
    {  43, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* MAIOR: reducere P178 vocatio-argumenta */
    {  44, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* MINOR_AEQUALIS: reducere P178 vocatio-argumenta */
    {  45, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* MAIOR_AEQUALIS: reducere P178 vocatio-argumenta */
    {  46, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* SINISTRORSUM: reducere P178 vocatio-argumenta */
    {  47, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* DEXTRORSUM: reducere P178 vocatio-argumenta */
    {  48, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  49, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* PLUS_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  50, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* MINUS_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  51, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* STAR_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  52, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* SOLIDUS_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  53, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* PERCENTUM_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  54, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* AMPERSAND_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  55, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* BARRA_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  56, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* CARET_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  57, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* SINISTRORSUM_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  58, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* DEXTRORSUM_ASSIGNATIO: reducere P178 vocatio-argumenta */
    {  59, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* INCREMENTUM: reducere P178 vocatio-argumenta */
    {  60, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* DECREMENTUM: reducere P178 vocatio-argumenta */
    {  61, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* PUNCTUM: reducere P178 vocatio-argumenta */
    {  62, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* SAGITTA: reducere P178 vocatio-argumenta */
    {  63, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* QUAESTIO: reducere P178 vocatio-argumenta */
    {  64, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* COLON: reducere P178 vocatio-argumenta */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* COMMA: reducere P178 vocatio-argumenta */
    {  66, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* SEMICOLON: reducere P178 vocatio-argumenta */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* PAREN_APERTA: reducere P178 vocatio-argumenta */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* PAREN_CLAUSA: reducere P178 vocatio-argumenta */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* QUADRA_APERTA: reducere P178 vocatio-argumenta */
    {  70, SILVA_TAB_ACTIO_REDUCERE, 178, 0 },  /* QUADRA_CLAUSA: reducere P178 vocatio-argumenta */
    /* --- status 262 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 263 --- */
    {  27, SILVA_TAB_ACTIO_TRANSPONERE, 290, 0 },  /* BRACE_CLAUSA: transponere -> 290 */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE, 291, 0 },  /* COMMA: transponere -> 291 */
    /* --- status 264 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* BRACE_CLAUSA: reducere P39 congeries-primum */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  39, 0 },  /* COMMA: reducere P39 congeries-primum */
    /* --- status 265 --- */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* ASSIGNATIO: reducere P70 declarator-aciei */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* COLON: reducere P70 declarator-aciei */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* COMMA: reducere P70 declarator-aciei */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* SEMICOLON: reducere P70 declarator-aciei */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* PAREN_APERTA: reducere P70 declarator-aciei */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* PAREN_CLAUSA: reducere P70 declarator-aciei */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  70, 0 },  /* QUADRA_APERTA: reducere P70 declarator-aciei */
    /* --- status 266 --- */
    {  48, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* ASSIGNATIO: reducere P72 declarator-functionis */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* COLON: reducere P72 declarator-functionis */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* COMMA: reducere P72 declarator-functionis */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* SEMICOLON: reducere P72 declarator-functionis */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* PAREN_APERTA: reducere P72 declarator-functionis */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* PAREN_CLAUSA: reducere P72 declarator-functionis */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  72, 0 },  /* QUADRA_APERTA: reducere P72 declarator-functionis */
    /* --- status 267 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  79, 0 },  /* IDENTIFICATOR: transponere -> 79 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  22, SILVA_TAB_ACTIO_TRANSPONERE, 293, 0 },  /* ELLIPSIS: transponere -> 293 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    /* --- status 268 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* IDENTIFICATOR: reducere P65 */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* CONST: reducere P65 */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* VOLATILE: reducere P65 */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* STAR: reducere P65 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* COMMA: reducere P65 */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* PAREN_APERTA: reducere P65 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* PAREN_CLAUSA: reducere P65 */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  65, 0 },  /* QUADRA_APERTA: reducere P65 */
    /* --- status 269 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 295, 0 },  /* IDENTIFICATOR: transponere -> 295 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  54, 0 },  /* VOID: transponere -> 54 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  55, 0 },  /* CHAR: transponere -> 55 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  56, 0 },  /* SHORT: transponere -> 56 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  57, 0 },  /* INT: transponere -> 57 */
    {   9, SILVA_TAB_ACTIO_TRANSPONERE,  58, 0 },  /* LONG: transponere -> 58 */
    {  10, SILVA_TAB_ACTIO_TRANSPONERE,  59, 0 },  /* FLOAT_KW: transponere -> 59 */
    {  11, SILVA_TAB_ACTIO_TRANSPONERE,  60, 0 },  /* DOUBLE: transponere -> 60 */
    {  12, SILVA_TAB_ACTIO_TRANSPONERE,  61, 0 },  /* SIGNED: transponere -> 61 */
    {  13, SILVA_TAB_ACTIO_TRANSPONERE,  62, 0 },  /* UNSIGNED: transponere -> 62 */
    {  15, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* TYPEDEF: transponere -> 11 */
    {  16, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* EXTERN: transponere -> 12 */
    {  17, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* STATIC: transponere -> 13 */
    {  18, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* AUTO: transponere -> 14 */
    {  19, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* REGISTER: transponere -> 15 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* CONST: transponere -> 8 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE,   9, 0 },  /* VOLATILE: transponere -> 9 */
    {  23, SILVA_TAB_ACTIO_TRANSPONERE,  25, 0 },  /* STRUCT: transponere -> 25 */
    {  24, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* UNION: transponere -> 26 */
    {  25, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* ENUM: transponere -> 27 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE, 268, 0 },  /* STAR: transponere -> 268 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 269, 0 },  /* PAREN_APERTA: transponere -> 269 */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  74, 0 },  /* PAREN_CLAUSA: reducere P74 */
    {  69, SILVA_TAB_ACTIO_TRANSPONERE, 208, 0 },  /* QUADRA_APERTA: transponere -> 208 */
    /* --- status 270 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* COMMA: reducere P80 parametrum-declaratum */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  80, 0 },  /* PAREN_CLAUSA: reducere P80 parametrum-declaratum */
    /* --- status 271 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* COMMA: reducere P81 parametrum-abstractum */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  81, 0 },  /* PAREN_CLAUSA: reducere P81 parametrum-abstractum */
    /* --- status 272 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* IDENTIFICATOR: reducere P42 structura-plena */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* VOID: reducere P42 structura-plena */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* CHAR: reducere P42 structura-plena */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* SHORT: reducere P42 structura-plena */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* INT: reducere P42 structura-plena */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* LONG: reducere P42 structura-plena */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* FLOAT_KW: reducere P42 structura-plena */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* DOUBLE: reducere P42 structura-plena */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* SIGNED: reducere P42 structura-plena */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* UNSIGNED: reducere P42 structura-plena */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* TYPEDEF: reducere P42 structura-plena */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* EXTERN: reducere P42 structura-plena */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* STATIC: reducere P42 structura-plena */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* AUTO: reducere P42 structura-plena */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* REGISTER: reducere P42 structura-plena */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* CONST: reducere P42 structura-plena */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* VOLATILE: reducere P42 structura-plena */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* STRUCT: reducere P42 structura-plena */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* UNION: reducere P42 structura-plena */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* ENUM: reducere P42 structura-plena */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* STAR: reducere P42 structura-plena */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* COLON: reducere P42 structura-plena */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* COMMA: reducere P42 structura-plena */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* SEMICOLON: reducere P42 structura-plena */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* PAREN_APERTA: reducere P42 structura-plena */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* PAREN_CLAUSA: reducere P42 structura-plena */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  42, 0 },  /* QUADRA_APERTA: reducere P42 structura-plena */
    /* --- status 273 --- */
    {  65, SILVA_TAB_ACTIO_TRANSPONERE, 297, 0 },  /* COMMA: transponere -> 297 */
    {  66, SILVA_TAB_ACTIO_TRANSPONERE, 296, 0 },  /* SEMICOLON: transponere -> 296 */
    /* --- status 274 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* COMMA: reducere P51 membra-decl-primum */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  51, 0 },  /* SEMICOLON: reducere P51 membra-decl-primum */
    /* --- status 275 --- */
    {  64, SILVA_TAB_ACTIO_TRANSPONERE, 298, 0 },  /* COLON: transponere -> 298 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* COMMA: reducere P53 */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  53, 0 },  /* SEMICOLON: reducere P53 */
    /* --- status 276 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 277 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* IDENTIFICATOR: reducere P45 unio-plena */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* VOID: reducere P45 unio-plena */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* CHAR: reducere P45 unio-plena */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* SHORT: reducere P45 unio-plena */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* INT: reducere P45 unio-plena */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* LONG: reducere P45 unio-plena */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* FLOAT_KW: reducere P45 unio-plena */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* DOUBLE: reducere P45 unio-plena */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* SIGNED: reducere P45 unio-plena */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* UNSIGNED: reducere P45 unio-plena */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* TYPEDEF: reducere P45 unio-plena */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* EXTERN: reducere P45 unio-plena */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* STATIC: reducere P45 unio-plena */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* AUTO: reducere P45 unio-plena */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* REGISTER: reducere P45 unio-plena */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* CONST: reducere P45 unio-plena */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* VOLATILE: reducere P45 unio-plena */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* STRUCT: reducere P45 unio-plena */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* UNION: reducere P45 unio-plena */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* ENUM: reducere P45 unio-plena */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* STAR: reducere P45 unio-plena */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* COLON: reducere P45 unio-plena */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* COMMA: reducere P45 unio-plena */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* SEMICOLON: reducere P45 unio-plena */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* PAREN_APERTA: reducere P45 unio-plena */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* PAREN_CLAUSA: reducere P45 unio-plena */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  45, 0 },  /* QUADRA_APERTA: reducere P45 unio-plena */
    /* --- status 278 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* IDENTIFICATOR: reducere P56 enumeratio-plena */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* VOID: reducere P56 enumeratio-plena */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* CHAR: reducere P56 enumeratio-plena */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* SHORT: reducere P56 enumeratio-plena */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* INT: reducere P56 enumeratio-plena */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* LONG: reducere P56 enumeratio-plena */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* FLOAT_KW: reducere P56 enumeratio-plena */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* DOUBLE: reducere P56 enumeratio-plena */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* SIGNED: reducere P56 enumeratio-plena */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* UNSIGNED: reducere P56 enumeratio-plena */
    {  15, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* TYPEDEF: reducere P56 enumeratio-plena */
    {  16, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* EXTERN: reducere P56 enumeratio-plena */
    {  17, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* STATIC: reducere P56 enumeratio-plena */
    {  18, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* AUTO: reducere P56 enumeratio-plena */
    {  19, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* REGISTER: reducere P56 enumeratio-plena */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* CONST: reducere P56 enumeratio-plena */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* VOLATILE: reducere P56 enumeratio-plena */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* STRUCT: reducere P56 enumeratio-plena */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* UNION: reducere P56 enumeratio-plena */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* ENUM: reducere P56 enumeratio-plena */
    {  30, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* STAR: reducere P56 enumeratio-plena */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* COLON: reducere P56 enumeratio-plena */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* COMMA: reducere P56 enumeratio-plena */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* SEMICOLON: reducere P56 enumeratio-plena */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* PAREN_APERTA: reducere P56 enumeratio-plena */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* PAREN_CLAUSA: reducere P56 enumeratio-plena */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  56, 0 },  /* QUADRA_APERTA: reducere P56 enumeratio-plena */
    /* --- status 279 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* BRACE_CLAUSA: reducere P60 enumeratores-appendere */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  60, 0 },  /* COMMA: reducere P60 enumeratores-appendere */
    /* --- status 280 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  62, 0 },  /* BRACE_CLAUSA: reducere P62 enumerator-valoratus */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  62, 0 },  /* COMMA: reducere P62 enumerator-valoratus */
    /* --- status 281 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* BRACE_CLAUSA: reducere P97 ternarius-conditio */
    {  64, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* COLON: reducere P97 ternarius-conditio */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* COMMA: reducere P97 ternarius-conditio */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* SEMICOLON: reducere P97 ternarius-conditio */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* PAREN_CLAUSA: reducere P97 ternarius-conditio */
    {  70, SILVA_TAB_ACTIO_REDUCERE,  97, 0 },  /* QUADRA_CLAUSA: reducere P97 ternarius-conditio */
    /* --- status 282 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 168, 0 },  /* COMMA: reducere P168 declarator-abstractus-catena */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 168, 0 },  /* PAREN_CLAUSA: reducere P168 declarator-abstractus-catena */
    /* --- status 283 --- */
    {  70, SILVA_TAB_ACTIO_TRANSPONERE, 300, 0 },  /* QUADRA_CLAUSA: transponere -> 300 */
    /* --- status 284 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 173, 0 },  /* COMMA: reducere P173 abstractus-aciei-vacua */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 173, 0 },  /* PAREN_APERTA: reducere P173 abstractus-aciei-vacua */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 173, 0 },  /* PAREN_CLAUSA: reducere P173 abstractus-aciei-vacua */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 173, 0 },  /* QUADRA_APERTA: reducere P173 abstractus-aciei-vacua */
    /* --- status 285 --- */
    {  68, SILVA_TAB_ACTIO_TRANSPONERE, 301, 0 },  /* PAREN_CLAUSA: transponere -> 301 */
    /* --- status 286 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 170, 0 },  /* COMMA: reducere P170 abstractus-parenthesis */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 170, 0 },  /* PAREN_APERTA: reducere P170 abstractus-parenthesis */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 170, 0 },  /* PAREN_CLAUSA: reducere P170 abstractus-parenthesis */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 170, 0 },  /* QUADRA_APERTA: reducere P170 abstractus-parenthesis */
    /* --- status 287 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 176, 0 },  /* COMMA: reducere P176 abstractus-functionis-sine */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 176, 0 },  /* PAREN_APERTA: reducere P176 abstractus-functionis-sine */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 176, 0 },  /* PAREN_CLAUSA: reducere P176 abstractus-functionis-sine */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 176, 0 },  /* QUADRA_APERTA: reducere P176 abstractus-functionis-sine */
    /* --- status 288 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 172, 0 },  /* COMMA: reducere P172 abstractus-aciei-sine */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 172, 0 },  /* PAREN_APERTA: reducere P172 abstractus-aciei-sine */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 172, 0 },  /* PAREN_CLAUSA: reducere P172 abstractus-aciei-sine */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 172, 0 },  /* QUADRA_APERTA: reducere P172 abstractus-aciei-sine */
    /* --- status 289 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 187, 0 },  /* COMMA: reducere P187 argumenta-appendere */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 187, 0 },  /* PAREN_CLAUSA: reducere P187 argumenta-appendere */
    /* --- status 290 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* BRACE_CLAUSA: reducere P38 congeries */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* COMMA: reducere P38 congeries */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  38, 0 },  /* SEMICOLON: reducere P38 congeries */
    /* --- status 291 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  26, SILVA_TAB_ACTIO_TRANSPONERE, 227, 0 },  /* BRACE_APERTA: transponere -> 227 */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* BRACE_CLAUSA: reducere P41 congeries-cauda */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  41, 0 },  /* COMMA: reducere P41 congeries-cauda */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 292 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  77, 0 },  /* COMMA: reducere P77 parametra-appendere */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  77, 0 },  /* PAREN_CLAUSA: reducere P77 parametra-appendere */
    /* --- status 293 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  78, 0 },  /* COMMA: reducere P78 parametra-ellipsis */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  78, 0 },  /* PAREN_CLAUSA: reducere P78 parametra-ellipsis */
    /* --- status 294 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* IDENTIFICATOR: transponere -> 76 */
    {  20, SILVA_TAB_ACTIO_TRANSPONERE, 229, 0 },  /* CONST: transponere -> 229 */
    {  21, SILVA_TAB_ACTIO_TRANSPONERE, 230, 0 },  /* VOLATILE: transponere -> 230 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE, 268, 0 },  /* STAR: transponere -> 268 */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 167, 0 },  /* COMMA: reducere P167 declarator-abstractus-stella */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE, 269, 0 },  /* PAREN_APERTA: transponere -> 269 */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 167, 0 },  /* PAREN_CLAUSA: reducere P167 declarator-abstractus-stella */
    {  69, SILVA_TAB_ACTIO_TRANSPONERE, 208, 0 },  /* QUADRA_APERTA: transponere -> 208 */
    /* --- status 295 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* IDENTIFICATOR: reducere P6 typus-nominatus */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* VOID: reducere P6 typus-nominatus */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* CHAR: reducere P6 typus-nominatus */
    {   7, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* SHORT: reducere P6 typus-nominatus */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* INT: reducere P6 typus-nominatus */
    {   9, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* LONG: reducere P6 typus-nominatus */
    {  10, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* FLOAT_KW: reducere P6 typus-nominatus */
    {  11, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* DOUBLE: reducere P6 typus-nominatus */
    {  12, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* SIGNED: reducere P6 typus-nominatus */
    {  13, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* UNSIGNED: reducere P6 typus-nominatus */
    {  15, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* TYPEDEF: reducere P6 typus-nominatus */
    {  16, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* EXTERN: reducere P6 typus-nominatus */
    {  17, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STATIC: reducere P6 typus-nominatus */
    {  18, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* AUTO: reducere P6 typus-nominatus */
    {  19, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* REGISTER: reducere P6 typus-nominatus */
    {  20, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* CONST: reducere P6 typus-nominatus */
    {  21, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* VOLATILE: reducere P6 typus-nominatus */
    {  23, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STRUCT: reducere P6 typus-nominatus */
    {  24, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* UNION: reducere P6 typus-nominatus */
    {  25, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* ENUM: reducere P6 typus-nominatus */
    {  30, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STAR: reducere P6 typus-nominatus */
    {  65, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* COMMA: reducere P6 typus-nominatus */
    {  67, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* PAREN_APERTA: reducere P6 typus-nominatus */
    {  67, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* PAREN_APERTA: reducere P68 declarator-titulus */
    {  68, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* PAREN_CLAUSA: reducere P6 typus-nominatus */
    {  68, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* PAREN_CLAUSA: reducere P68 declarator-titulus */
    {  69, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* QUADRA_APERTA: reducere P6 typus-nominatus */
    {  69, SILVA_TAB_ACTIO_REDUCERE,  68, 0 },  /* QUADRA_APERTA: reducere P68 declarator-titulus */
    /* --- status 296 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* IDENTIFICATOR: reducere P50 membrum */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* VOID: reducere P50 membrum */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* CHAR: reducere P50 membrum */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* SHORT: reducere P50 membrum */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* INT: reducere P50 membrum */
    {   9, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* LONG: reducere P50 membrum */
    {  10, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* FLOAT_KW: reducere P50 membrum */
    {  11, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* DOUBLE: reducere P50 membrum */
    {  12, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* SIGNED: reducere P50 membrum */
    {  13, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* UNSIGNED: reducere P50 membrum */
    {  20, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* CONST: reducere P50 membrum */
    {  21, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* VOLATILE: reducere P50 membrum */
    {  23, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* STRUCT: reducere P50 membrum */
    {  24, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* UNION: reducere P50 membrum */
    {  25, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* ENUM: reducere P50 membrum */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  50, 0 },  /* BRACE_CLAUSA: reducere P50 membrum */
    /* --- status 297 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  76, 0 },  /* IDENTIFICATOR: transponere -> 76 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  74, 0 },  /* STAR: transponere -> 74 */
    {  64, SILVA_TAB_ACTIO_TRANSPONERE, 276, 0 },  /* COLON: transponere -> 276 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  77, 0 },  /* PAREN_APERTA: transponere -> 77 */
    /* --- status 298 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE, 137, 0 },  /* IDENTIFICATOR: transponere -> 137 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  67, 0 },  /* INTEGER: transponere -> 67 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,  68, 0 },  /* FLOAT: transponere -> 68 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  69, 0 },  /* CHARACTER_LIT: transponere -> 69 */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  65, 0 },  /* STRING_LIT: transponere -> 65 */
    {  14, SILVA_TAB_ACTIO_TRANSPONERE,  52, 0 },  /* SIZEOF: transponere -> 52 */
    {  28, SILVA_TAB_ACTIO_TRANSPONERE,  48, 0 },  /* PLUS: transponere -> 48 */
    {  29, SILVA_TAB_ACTIO_TRANSPONERE,  49, 0 },  /* MINUS: transponere -> 49 */
    {  30, SILVA_TAB_ACTIO_TRANSPONERE,  47, 0 },  /* STAR: transponere -> 47 */
    {  33, SILVA_TAB_ACTIO_TRANSPONERE,  46, 0 },  /* AMPERSAND: transponere -> 46 */
    {  36, SILVA_TAB_ACTIO_TRANSPONERE,  50, 0 },  /* TILDE: transponere -> 50 */
    {  37, SILVA_TAB_ACTIO_TRANSPONERE,  51, 0 },  /* EXCLAMATIO: transponere -> 51 */
    {  59, SILVA_TAB_ACTIO_TRANSPONERE,  44, 0 },  /* INCREMENTUM: transponere -> 44 */
    {  60, SILVA_TAB_ACTIO_TRANSPONERE,  45, 0 },  /* DECREMENTUM: transponere -> 45 */
    {  67, SILVA_TAB_ACTIO_TRANSPONERE,  43, 0 },  /* PAREN_APERTA: transponere -> 43 */
    /* --- status 299 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* COMMA: reducere P55 campus-sine-declaratore */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  55, 0 },  /* SEMICOLON: reducere P55 campus-sine-declaratore */
    /* --- status 300 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 171, 0 },  /* COMMA: reducere P171 abstractus-aciei */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 171, 0 },  /* PAREN_APERTA: reducere P171 abstractus-aciei */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 171, 0 },  /* PAREN_CLAUSA: reducere P171 abstractus-aciei */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 171, 0 },  /* QUADRA_APERTA: reducere P171 abstractus-aciei */
    /* --- status 301 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE, 175, 0 },  /* COMMA: reducere P175 abstractus-functionis */
    {  67, SILVA_TAB_ACTIO_REDUCERE, 175, 0 },  /* PAREN_APERTA: reducere P175 abstractus-functionis */
    {  68, SILVA_TAB_ACTIO_REDUCERE, 175, 0 },  /* PAREN_CLAUSA: reducere P175 abstractus-functionis */
    {  69, SILVA_TAB_ACTIO_REDUCERE, 175, 0 },  /* QUADRA_APERTA: reducere P175 abstractus-functionis */
    /* --- status 302 --- */
    {  27, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* BRACE_CLAUSA: reducere P40 congeries-appendere */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  40, 0 },  /* COMMA: reducere P40 congeries-appendere */
    /* --- status 303 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* COMMA: reducere P52 membra-decl-appendere */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  52, 0 },  /* SEMICOLON: reducere P52 membra-decl-appendere */
    /* --- status 304 --- */
    {  65, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* COMMA: reducere P54 campus */
    {  66, SILVA_TAB_ACTIO_REDUCERE,  54, 0 },  /* SEMICOLON: reducere P54 campus */
};

/* ==================================================
 * Goto
 * ================================================== */

hic_manens constans SilvaTabGoto SILVA_C89_GOTO[] = {
    /* --- status 0 --- */
    {  72,   1 },  /* elementa -> 1 */
    /* --- status 1 --- */
    {  73,   2 },  /* elementum -> 2 */
    {  74,   4 },  /* declaratio -> 4 */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  19 },  /* atomum-decl-non-primitivum -> 19 */
    {  77,   5 },  /* specificatores-decl -> 5 */
    {  78,  16 },  /* sd-post-primitivum -> 16 */
    {  79,  17 },  /* sd-post-aliud -> 17 */
    {  80,   6 },  /* sd-post-tag -> 6 */
    {  81,  20 },  /* atomum-tag -> 20 */
    {  82,  18 },  /* typus-primitivus-atomum -> 18 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 103,   3 },  /* sententia-expressionis -> 3 */
    { 104,  28 },  /* expressio -> 28 */
    { 105,  29 },  /* assignatio -> 29 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 5 --- */
    {  83,  71 },  /* declaratores -> 71 */
    {  84,  72 },  /* declarator-initiatus -> 72 */
    {  96,  73 },  /* declarator -> 73 */
    {  98,  75 },  /* declarator-directus -> 75 */
    /* --- status 6 --- */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  81 },  /* atomum-decl-non-primitivum -> 81 */
    {  81,  82 },  /* atomum-tag -> 82 */
    {  82,  80 },  /* typus-primitivus-atomum -> 80 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 16 --- */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  83 },  /* atomum-decl-non-primitivum -> 83 */
    {  81,  84 },  /* atomum-tag -> 84 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    /* --- status 17 --- */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  86 },  /* atomum-decl-non-primitivum -> 86 */
    {  81,  87 },  /* atomum-tag -> 87 */
    {  82,  85 },  /* typus-primitivus-atomum -> 85 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 24 --- */
    { 124,  88 },  /* verbum-primitivum -> 88 */
    /* --- status 43 --- */
    {  75, 134 },  /* atomum-non-primitivum -> 134 */
    {  81, 135 },  /* atomum-tag -> 135 */
    {  82, 133 },  /* typus-primitivus-atomum -> 133 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 104, 127 },  /* expressio -> 127 */
    { 105,  29 },  /* assignatio -> 29 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 119, 128 },  /* species-typi -> 128 */
    { 120, 129 },  /* specificatores -> 129 */
    { 121, 130 },  /* sq-post-primitivum -> 130 */
    { 122, 131 },  /* sq-post-aliud -> 131 */
    { 123, 132 },  /* sq-post-tag -> 132 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 44 --- */
    { 118, 136 },  /* unarium -> 136 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 45 --- */
    { 118, 139 },  /* unarium -> 139 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 46 --- */
    { 117, 141 },  /* conversio -> 141 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 47 --- */
    { 117, 142 },  /* conversio -> 142 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 48 --- */
    { 117, 143 },  /* conversio -> 143 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 49 --- */
    { 117, 144 },  /* conversio -> 144 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 50 --- */
    { 117, 145 },  /* conversio -> 145 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 51 --- */
    { 117, 146 },  /* conversio -> 146 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 52 --- */
    { 118, 147 },  /* unarium -> 147 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 74 --- */
    {  97, 159 },  /* qualificatores -> 159 */
    /* --- status 77 --- */
    {  96, 162 },  /* declarator -> 162 */
    {  98,  75 },  /* declarator-directus -> 75 */
    /* --- status 90 --- */
    {  89, 164 },  /* membra -> 164 */
    /* --- status 92 --- */
    {  89, 166 },  /* membra -> 166 */
    /* --- status 94 --- */
    {  94, 168 },  /* enumeratores -> 168 */
    {  95, 169 },  /* enumerator-unus -> 169 */
    /* --- status 96 --- */
    { 105, 171 },  /* assignatio -> 171 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 97 --- */
    { 105, 172 },  /* assignatio -> 172 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 98 --- */
    { 105, 173 },  /* assignatio -> 173 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 99 --- */
    { 105, 174 },  /* assignatio -> 174 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 100 --- */
    { 105, 175 },  /* assignatio -> 175 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 101 --- */
    { 105, 176 },  /* assignatio -> 176 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 102 --- */
    { 105, 177 },  /* assignatio -> 177 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 103 --- */
    { 105, 178 },  /* assignatio -> 178 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 104 --- */
    { 105, 179 },  /* assignatio -> 179 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 105 --- */
    { 105, 180 },  /* assignatio -> 180 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 106 --- */
    { 105, 181 },  /* assignatio -> 181 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 107 --- */
    { 105, 182 },  /* assignatio -> 182 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 108 --- */
    { 104, 183 },  /* expressio -> 183 */
    { 105,  29 },  /* assignatio -> 29 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 109 --- */
    { 108, 184 },  /* coniunctio -> 184 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 110 --- */
    { 109, 185 },  /* vel-bitorum -> 185 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 111 --- */
    { 110, 186 },  /* aut-bitorum -> 186 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 112 --- */
    { 111, 187 },  /* et-bitorum -> 187 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 113 --- */
    { 112, 188 },  /* aequalitas -> 188 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 114 --- */
    { 113, 189 },  /* comparatio -> 189 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 115 --- */
    { 113, 190 },  /* comparatio -> 190 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 116 --- */
    { 114, 191 },  /* translatio -> 191 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 117 --- */
    { 114, 192 },  /* translatio -> 192 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 118 --- */
    { 114, 193 },  /* translatio -> 193 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 119 --- */
    { 114, 194 },  /* translatio -> 194 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 120 --- */
    { 115, 195 },  /* additivum -> 195 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 121 --- */
    { 115, 196 },  /* additivum -> 196 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 122 --- */
    { 116, 197 },  /* multiplicativum -> 197 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 123 --- */
    { 116, 198 },  /* multiplicativum -> 198 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 124 --- */
    { 117, 199 },  /* conversio -> 199 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 125 --- */
    { 117, 200 },  /* conversio -> 200 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 126 --- */
    { 117, 201 },  /* conversio -> 201 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 129 --- */
    { 126, 204 },  /* declarator-abstractus -> 204 */
    { 127, 206 },  /* abstractus-directus -> 206 */
    /* --- status 130 --- */
    {  75, 209 },  /* atomum-non-primitivum -> 209 */
    {  81, 210 },  /* atomum-tag -> 210 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    /* --- status 131 --- */
    {  75, 212 },  /* atomum-non-primitivum -> 212 */
    {  81, 213 },  /* atomum-tag -> 213 */
    {  82, 211 },  /* typus-primitivus-atomum -> 211 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 132 --- */
    {  75, 215 },  /* atomum-non-primitivum -> 215 */
    {  81, 216 },  /* atomum-tag -> 216 */
    {  82, 214 },  /* typus-primitivus-atomum -> 214 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 138 --- */
    { 104, 127 },  /* expressio -> 127 */
    { 105,  29 },  /* assignatio -> 29 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 148 --- */
    {  75, 134 },  /* atomum-non-primitivum -> 134 */
    {  81, 135 },  /* atomum-tag -> 135 */
    {  82, 133 },  /* typus-primitivus-atomum -> 133 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 104, 127 },  /* expressio -> 127 */
    { 105,  29 },  /* assignatio -> 29 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 119, 217 },  /* species-typi -> 217 */
    { 120, 129 },  /* specificatores -> 129 */
    { 121, 130 },  /* sq-post-primitivum -> 130 */
    { 122, 131 },  /* sq-post-aliud -> 131 */
    { 123, 132 },  /* sq-post-tag -> 132 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 149 --- */
    { 104, 218 },  /* expressio -> 218 */
    { 105,  29 },  /* assignatio -> 29 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 150 --- */
    { 105, 221 },  /* assignatio -> 221 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 129, 219 },  /* argumenta -> 219 */
    { 130, 220 },  /* argumenta-plena -> 220 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 157 --- */
    {  84, 224 },  /* declarator-initiatus -> 224 */
    {  96,  73 },  /* declarator -> 73 */
    {  98,  75 },  /* declarator-directus -> 75 */
    /* --- status 158 --- */
    {  85, 225 },  /* initiator -> 225 */
    { 105, 226 },  /* assignatio -> 226 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 159 --- */
    {  96, 228 },  /* declarator -> 228 */
    {  98,  75 },  /* declarator-directus -> 75 */
    /* --- status 160 --- */
    {  99, 231 },  /* mensura-aciei -> 231 */
    { 106, 233 },  /* ternarius -> 233 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 161 --- */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  19 },  /* atomum-decl-non-primitivum -> 19 */
    {  77, 238 },  /* specificatores-decl -> 238 */
    {  78,  16 },  /* sd-post-primitivum -> 16 */
    {  79,  17 },  /* sd-post-aliud -> 17 */
    {  80, 234 },  /* sd-post-tag -> 234 */
    {  81,  20 },  /* atomum-tag -> 20 */
    {  82,  18 },  /* typus-primitivus-atomum -> 18 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 100, 235 },  /* parametra -> 235 */
    { 101, 236 },  /* parametra-plena -> 236 */
    { 102, 237 },  /* parametrum -> 237 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 163 --- */
    {  89, 240 },  /* membra -> 240 */
    /* --- status 164 --- */
    {  75, 134 },  /* atomum-non-primitivum -> 134 */
    {  81, 135 },  /* atomum-tag -> 135 */
    {  82, 133 },  /* typus-primitivus-atomum -> 133 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  90, 242 },  /* membrum -> 242 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 120, 243 },  /* specificatores -> 243 */
    { 121, 130 },  /* sq-post-primitivum -> 130 */
    { 122, 131 },  /* sq-post-aliud -> 131 */
    { 123, 132 },  /* sq-post-tag -> 132 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 165 --- */
    {  89, 244 },  /* membra -> 244 */
    /* --- status 166 --- */
    {  75, 134 },  /* atomum-non-primitivum -> 134 */
    {  81, 135 },  /* atomum-tag -> 135 */
    {  82, 133 },  /* typus-primitivus-atomum -> 133 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  90, 242 },  /* membrum -> 242 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 120, 243 },  /* specificatores -> 243 */
    { 121, 130 },  /* sq-post-primitivum -> 130 */
    { 122, 131 },  /* sq-post-aliud -> 131 */
    { 123, 132 },  /* sq-post-tag -> 132 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 167 --- */
    {  94, 246 },  /* enumeratores -> 246 */
    {  95, 169 },  /* enumerator-unus -> 169 */
    /* --- status 203 --- */
    { 117, 251 },  /* conversio -> 251 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 205 --- */
    {  97, 252 },  /* qualificatores -> 252 */
    /* --- status 207 --- */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  19 },  /* atomum-decl-non-primitivum -> 19 */
    {  77, 238 },  /* specificatores-decl -> 238 */
    {  78,  16 },  /* sd-post-primitivum -> 16 */
    {  79,  17 },  /* sd-post-aliud -> 17 */
    {  80, 234 },  /* sd-post-tag -> 234 */
    {  81,  20 },  /* atomum-tag -> 20 */
    {  82,  18 },  /* typus-primitivus-atomum -> 18 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 100, 256 },  /* parametra -> 256 */
    { 101, 236 },  /* parametra-plena -> 236 */
    { 102, 237 },  /* parametrum -> 237 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    { 126, 255 },  /* declarator-abstractus -> 255 */
    { 127, 206 },  /* abstractus-directus -> 206 */
    /* --- status 208 --- */
    {  99, 257 },  /* mensura-aciei -> 257 */
    { 106, 233 },  /* ternarius -> 233 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 227 --- */
    {  85, 264 },  /* initiator -> 264 */
    {  86, 263 },  /* congeries-elementa -> 263 */
    { 105, 226 },  /* assignatio -> 226 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 234 --- */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  81 },  /* atomum-decl-non-primitivum -> 81 */
    {  81,  82 },  /* atomum-tag -> 82 */
    {  82,  80 },  /* typus-primitivus-atomum -> 80 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 238 --- */
    {  96, 270 },  /* declarator -> 270 */
    {  98,  75 },  /* declarator-directus -> 75 */
    { 126, 271 },  /* declarator-abstractus -> 271 */
    { 127, 206 },  /* abstractus-directus -> 206 */
    /* --- status 240 --- */
    {  75, 134 },  /* atomum-non-primitivum -> 134 */
    {  81, 135 },  /* atomum-tag -> 135 */
    {  82, 133 },  /* typus-primitivus-atomum -> 133 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  90, 242 },  /* membrum -> 242 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 120, 243 },  /* specificatores -> 243 */
    { 121, 130 },  /* sq-post-primitivum -> 130 */
    { 122, 131 },  /* sq-post-aliud -> 131 */
    { 123, 132 },  /* sq-post-tag -> 132 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 243 --- */
    {  91, 273 },  /* membra-declaratores -> 273 */
    {  92, 274 },  /* membrum-declarator -> 274 */
    {  96, 275 },  /* declarator -> 275 */
    {  98,  75 },  /* declarator-directus -> 75 */
    /* --- status 244 --- */
    {  75, 134 },  /* atomum-non-primitivum -> 134 */
    {  81, 135 },  /* atomum-tag -> 135 */
    {  82, 133 },  /* typus-primitivus-atomum -> 133 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  90, 242 },  /* membrum -> 242 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 120, 243 },  /* specificatores -> 243 */
    { 121, 130 },  /* sq-post-primitivum -> 130 */
    { 122, 131 },  /* sq-post-aliud -> 131 */
    { 123, 132 },  /* sq-post-tag -> 132 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 248 --- */
    {  95, 279 },  /* enumerator-unus -> 279 */
    /* --- status 249 --- */
    { 106, 280 },  /* ternarius -> 280 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 250 --- */
    { 106, 281 },  /* ternarius -> 281 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 252 --- */
    { 126, 282 },  /* declarator-abstractus -> 282 */
    { 127, 206 },  /* abstractus-directus -> 206 */
    /* --- status 253 --- */
    {  99, 283 },  /* mensura-aciei -> 283 */
    { 106, 233 },  /* ternarius -> 233 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 254 --- */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  19 },  /* atomum-decl-non-primitivum -> 19 */
    {  77, 238 },  /* specificatores-decl -> 238 */
    {  78,  16 },  /* sd-post-primitivum -> 16 */
    {  79,  17 },  /* sd-post-aliud -> 17 */
    {  80, 234 },  /* sd-post-tag -> 234 */
    {  81,  20 },  /* atomum-tag -> 20 */
    {  82,  18 },  /* typus-primitivus-atomum -> 18 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 100, 285 },  /* parametra -> 285 */
    { 101, 236 },  /* parametra-plena -> 236 */
    { 102, 237 },  /* parametrum -> 237 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 262 --- */
    { 105, 289 },  /* assignatio -> 289 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 267 --- */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  19 },  /* atomum-decl-non-primitivum -> 19 */
    {  77, 238 },  /* specificatores-decl -> 238 */
    {  78,  16 },  /* sd-post-primitivum -> 16 */
    {  79,  17 },  /* sd-post-aliud -> 17 */
    {  80, 234 },  /* sd-post-tag -> 234 */
    {  81,  20 },  /* atomum-tag -> 20 */
    {  82,  18 },  /* typus-primitivus-atomum -> 18 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    { 102, 292 },  /* parametrum -> 292 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    /* --- status 268 --- */
    {  97, 294 },  /* qualificatores -> 294 */
    /* --- status 269 --- */
    {  75,  10 },  /* atomum-non-primitivum -> 10 */
    {  76,  19 },  /* atomum-decl-non-primitivum -> 19 */
    {  77, 238 },  /* specificatores-decl -> 238 */
    {  78,  16 },  /* sd-post-primitivum -> 16 */
    {  79,  17 },  /* sd-post-aliud -> 17 */
    {  80, 234 },  /* sd-post-tag -> 234 */
    {  81,  20 },  /* atomum-tag -> 20 */
    {  82,  18 },  /* typus-primitivus-atomum -> 18 */
    {  87,  21 },  /* species-structurae -> 21 */
    {  88,  22 },  /* species-unionis -> 22 */
    {  93,  23 },  /* species-enumerationis -> 23 */
    {  96, 162 },  /* declarator -> 162 */
    {  98,  75 },  /* declarator-directus -> 75 */
    { 100, 256 },  /* parametra -> 256 */
    { 101, 236 },  /* parametra-plena -> 236 */
    { 102, 237 },  /* parametrum -> 237 */
    { 124,  63 },  /* verbum-primitivum -> 63 */
    { 125,  24 },  /* verba-primitiva -> 24 */
    { 126, 255 },  /* declarator-abstractus -> 255 */
    { 127, 206 },  /* abstractus-directus -> 206 */
    /* --- status 276 --- */
    { 106, 299 },  /* ternarius -> 299 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 291 --- */
    {  85, 302 },  /* initiator -> 302 */
    { 105, 226 },  /* assignatio -> 226 */
    { 106,  31 },  /* ternarius -> 31 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118,  30 },  /* unarium -> 30 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    /* --- status 294 --- */
    {  96, 228 },  /* declarator -> 228 */
    {  98,  75 },  /* declarator-directus -> 75 */
    { 126, 282 },  /* declarator-abstractus -> 282 */
    { 127, 206 },  /* abstractus-directus -> 206 */
    /* --- status 297 --- */
    {  92, 303 },  /* membrum-declarator -> 303 */
    {  96, 275 },  /* declarator -> 275 */
    {  98,  75 },  /* declarator-directus -> 75 */
    /* --- status 298 --- */
    { 106, 304 },  /* ternarius -> 304 */
    { 107,  32 },  /* disiunctio -> 32 */
    { 108,  33 },  /* coniunctio -> 33 */
    { 109,  34 },  /* vel-bitorum -> 34 */
    { 110,  35 },  /* aut-bitorum -> 35 */
    { 111,  36 },  /* et-bitorum -> 36 */
    { 112,  37 },  /* aequalitas -> 37 */
    { 113,  38 },  /* comparatio -> 38 */
    { 114,  39 },  /* translatio -> 39 */
    { 115,  40 },  /* additivum -> 40 */
    { 116,  41 },  /* multiplicativum -> 41 */
    { 117,  42 },  /* conversio -> 42 */
    { 118, 140 },  /* unarium -> 140 */
    { 128,  53 },  /* postfixum -> 53 */
    { 131,  66 },  /* chordae -> 66 */
    { 132,  64 },  /* primarium -> 64 */
    { -1, -1 }   /* terminator */
};

/* ==================================================
 * Status (offset + numerus in series planas)
 * ================================================== */

hic_manens constans SilvaTabStatus SILVA_C89_STATUS[305] = {
    /* status  0 */ {    0, 35,    0,  1 },
    /* status  1 */ {   35, 35,    1, 34 },
    /* status  2 */ {   70, 35,   35,  0 },
    /* status  3 */ {  105, 35,   35,  0 },
    /* status  4 */ {  140, 35,   35,  0 },
    /* status  5 */ {  175,  3,   35,  4 },
    /* status  6 */ {  178, 24,   39,  9 },
    /* status  7 */ {  202, 63,   48,  0 },
    /* status  8 */ {  265, 26,   48,  0 },
    /* status  9 */ {  291, 26,   48,  0 },
    /* status 10 */ {  317, 25,   48,  0 },
    /* status 11 */ {  342, 25,   48,  0 },
    /* status 12 */ {  367, 25,   48,  0 },
    /* status 13 */ {  392, 25,   48,  0 },
    /* status 14 */ {  417, 25,   48,  0 },
    /* status 15 */ {  442, 25,   48,  0 },
    /* status 16 */ {  467, 17,   48,  6 },
    /* status 17 */ {  484, 26,   54,  9 },
    /* status 18 */ {  510, 16,   63,  0 },
    /* status 19 */ {  526, 25,   63,  0 },
    /* status 20 */ {  551, 26,   63,  0 },
    /* status 21 */ {  577, 27,   63,  0 },
    /* status 22 */ {  604, 27,   63,  0 },
    /* status 23 */ {  631, 27,   63,  0 },
    /* status 24 */ {  658, 26,   63,  1 },
    /* status 25 */ {  684,  2,   64,  0 },
    /* status 26 */ {  686,  2,   64,  0 },
    /* status 27 */ {  688,  2,   64,  0 },
    /* status 28 */ {  690,  2,   64,  0 },
    /* status 29 */ {  692,  5,   64,  0 },
    /* status 30 */ {  697, 36,   64,  0 },
    /* status 31 */ {  733,  6,   64,  0 },
    /* status 32 */ {  739,  8,   64,  0 },
    /* status 33 */ {  747,  9,   64,  0 },
    /* status 34 */ {  756, 10,   64,  0 },
    /* status 35 */ {  766, 11,   64,  0 },
    /* status 36 */ {  777, 12,   64,  0 },
    /* status 37 */ {  789, 14,   64,  0 },
    /* status 38 */ {  803, 18,   64,  0 },
    /* status 39 */ {  821, 20,   64,  0 },
    /* status 40 */ {  841, 22,   64,  0 },
    /* status 41 */ {  863, 25,   64,  0 },
    /* status 42 */ {  888, 25,   64,  0 },
    /* status 43 */ {  913, 29,   64, 31 },
    /* status 44 */ {  942, 15,   95,  4 },
    /* status 45 */ {  957, 15,   99,  4 },
    /* status 46 */ {  972, 15,  103,  5 },
    /* status 47 */ {  987, 15,  108,  5 },
    /* status 48 */ { 1002, 15,  113,  5 },
    /* status 49 */ { 1017, 15,  118,  5 },
    /* status 50 */ { 1032, 15,  123,  5 },
    /* status 51 */ { 1047, 15,  128,  5 },
    /* status 52 */ { 1062, 15,  133,  4 },
    /* status 53 */ { 1077, 42,  137,  0 },
    /* status 54 */ { 1119, 26,  137,  0 },
    /* status 55 */ { 1145, 26,  137,  0 },
    /* status 56 */ { 1171, 26,  137,  0 },
    /* status 57 */ { 1197, 26,  137,  0 },
    /* status 58 */ { 1223, 26,  137,  0 },
    /* status 59 */ { 1249, 26,  137,  0 },
    /* status 60 */ { 1275, 26,  137,  0 },
    /* status 61 */ { 1301, 26,  137,  0 },
    /* status 62 */ { 1327, 26,  137,  0 },
    /* status 63 */ { 1353, 26,  137,  0 },
    /* status 64 */ { 1379, 42,  137,  0 },
    /* status 65 */ { 1421, 43,  137,  0 },
    /* status 66 */ { 1464, 43,  137,  0 },
    /* status 67 */ { 1507, 42,  137,  0 },
    /* status 68 */ { 1549, 42,  137,  0 },
    /* status 69 */ { 1591, 42,  137,  0 },
    /* status 70 */ { 1633,  1,  137,  0 },
    /* status 71 */ { 1634,  2,  137,  0 },
    /* status 72 */ { 1636,  2,  137,  0 },
    /* status 73 */ { 1638,  3,  137,  0 },
    /* status 74 */ { 1641,  5,  137,  1 },
    /* status 75 */ { 1646,  7,  138,  0 },
    /* status 76 */ { 1653,  7,  138,  0 },
    /* status 77 */ { 1660,  3,  138,  2 },
    /* status 78 */ { 1663, 35,  140,  0 },
    /* status 79 */ { 1698, 26,  140,  0 },
    /* status 80 */ { 1724, 16,  140,  0 },
    /* status 81 */ { 1740, 25,  140,  0 },
    /* status 82 */ { 1765, 26,  140,  0 },
    /* status 83 */ { 1791, 25,  140,  0 },
    /* status 84 */ { 1816, 26,  140,  0 },
    /* status 85 */ { 1842, 16,  140,  0 },
    /* status 86 */ { 1858, 25,  140,  0 },
    /* status 87 */ { 1883, 26,  140,  0 },
    /* status 88 */ { 1909, 26,  140,  0 },
    /* status 89 */ { 1935, 28,  140,  0 },
    /* status 90 */ { 1963, 16,  140,  1 },
    /* status 91 */ { 1979, 28,  141,  0 },
    /* status 92 */ { 2007, 16,  141,  1 },
    /* status 93 */ { 2023, 28,  142,  0 },
    /* status 94 */ { 2051,  1,  142,  2 },
    /* status 95 */ { 2052, 35,  144,  0 },
    /* status 96 */ { 2087, 15,  144, 17 },
    /* status 97 */ { 2102, 15,  161, 17 },
    /* status 98 */ { 2117, 15,  178, 17 },
    /* status 99 */ { 2132, 15,  195, 17 },
    /* status 100 */ { 2147, 15,  212, 17 },
    /* status 101 */ { 2162, 15,  229, 17 },
    /* status 102 */ { 2177, 15,  246, 17 },
    /* status 103 */ { 2192, 15,  263, 17 },
    /* status 104 */ { 2207, 15,  280, 17 },
    /* status 105 */ { 2222, 15,  297, 17 },
    /* status 106 */ { 2237, 15,  314, 17 },
    /* status 107 */ { 2252, 15,  331, 17 },
    /* status 108 */ { 2267, 15,  348, 18 },
    /* status 109 */ { 2282, 15,  366, 14 },
    /* status 110 */ { 2297, 15,  380, 13 },
    /* status 111 */ { 2312, 15,  393, 12 },
    /* status 112 */ { 2327, 15,  405, 11 },
    /* status 113 */ { 2342, 15,  416, 10 },
    /* status 114 */ { 2357, 15,  426,  9 },
    /* status 115 */ { 2372, 15,  435,  9 },
    /* status 116 */ { 2387, 15,  444,  8 },
    /* status 117 */ { 2402, 15,  452,  8 },
    /* status 118 */ { 2417, 15,  460,  8 },
    /* status 119 */ { 2432, 15,  468,  8 },
    /* status 120 */ { 2447, 15,  476,  7 },
    /* status 121 */ { 2462, 15,  483,  7 },
    /* status 122 */ { 2477, 15,  490,  6 },
    /* status 123 */ { 2492, 15,  496,  6 },
    /* status 124 */ { 2507, 15,  502,  5 },
    /* status 125 */ { 2522, 15,  507,  5 },
    /* status 126 */ { 2537, 15,  512,  5 },
    /* status 127 */ { 2552,  2,  517,  0 },
    /* status 128 */ { 2554,  1,  517,  0 },
    /* status 129 */ { 2555,  4,  517,  2 },
    /* status 130 */ { 2559, 12,  519,  5 },
    /* status 131 */ { 2571, 21,  524,  8 },
    /* status 132 */ { 2592, 21,  532,  8 },
    /* status 133 */ { 2613, 11,  540,  0 },
    /* status 134 */ { 2624, 20,  540,  0 },
    /* status 135 */ { 2644, 20,  540,  0 },
    /* status 136 */ { 2664, 36,  540,  0 },
    /* status 137 */ { 2700, 42,  540,  0 },
    /* status 138 */ { 2742, 15,  540, 18 },
    /* status 139 */ { 2757, 36,  558,  0 },
    /* status 140 */ { 2793, 36,  558,  0 },
    /* status 141 */ { 2829, 36,  558,  0 },
    /* status 142 */ { 2865, 36,  558,  0 },
    /* status 143 */ { 2901, 36,  558,  0 },
    /* status 144 */ { 2937, 36,  558,  0 },
    /* status 145 */ { 2973, 36,  558,  0 },
    /* status 146 */ { 3009, 36,  558,  0 },
    /* status 147 */ { 3045, 36,  558,  0 },
    /* status 148 */ { 3081, 29,  558, 31 },
    /* status 149 */ { 3110, 15,  589, 18 },
    /* status 150 */ { 3125, 16,  607, 19 },
    /* status 151 */ { 3141,  1,  626,  0 },
    /* status 152 */ { 3142,  1,  626,  0 },
    /* status 153 */ { 3143, 42,  626,  0 },
    /* status 154 */ { 3185, 42,  626,  0 },
    /* status 155 */ { 3227, 43,  626,  0 },
    /* status 156 */ { 3270, 35,  626,  0 },
    /* status 157 */ { 3305,  3,  626,  3 },
    /* status 158 */ { 3308, 16,  629, 18 },
    /* status 159 */ { 3324,  5,  647,  2 },
    /* status 160 */ { 3329, 16,  649, 17 },
    /* status 161 */ { 3345, 21,  666, 16 },
    /* status 162 */ { 3366,  1,  682,  0 },
    /* status 163 */ { 3367, 16,  682,  1 },
    /* status 164 */ { 3383, 16,  683, 13 },
    /* status 165 */ { 3399, 16,  696,  1 },
    /* status 166 */ { 3415, 16,  697, 13 },
    /* status 167 */ { 3431,  1,  710,  2 },
    /* status 168 */ { 3432,  2,  712,  0 },
    /* status 169 */ { 3434,  2,  712,  0 },
    /* status 170 */ { 3436,  3,  712,  0 },
    /* status 171 */ { 3439,  5,  712,  0 },
    /* status 172 */ { 3444,  6,  712,  0 },
    /* status 173 */ { 3450,  6,  712,  0 },
    /* status 174 */ { 3456,  6,  712,  0 },
    /* status 175 */ { 3462,  6,  712,  0 },
    /* status 176 */ { 3468,  6,  712,  0 },
    /* status 177 */ { 3474,  6,  712,  0 },
    /* status 178 */ { 3480,  6,  712,  0 },
    /* status 179 */ { 3486,  6,  712,  0 },
    /* status 180 */ { 3492,  6,  712,  0 },
    /* status 181 */ { 3498,  6,  712,  0 },
    /* status 182 */ { 3504,  6,  712,  0 },
    /* status 183 */ { 3510,  2,  712,  0 },
    /* status 184 */ { 3512,  9,  712,  0 },
    /* status 185 */ { 3521, 10,  712,  0 },
    /* status 186 */ { 3531, 11,  712,  0 },
    /* status 187 */ { 3542, 12,  712,  0 },
    /* status 188 */ { 3554, 14,  712,  0 },
    /* status 189 */ { 3568, 18,  712,  0 },
    /* status 190 */ { 3586, 18,  712,  0 },
    /* status 191 */ { 3604, 20,  712,  0 },
    /* status 192 */ { 3624, 20,  712,  0 },
    /* status 193 */ { 3644, 20,  712,  0 },
    /* status 194 */ { 3664, 20,  712,  0 },
    /* status 195 */ { 3684, 22,  712,  0 },
    /* status 196 */ { 3706, 22,  712,  0 },
    /* status 197 */ { 3728, 25,  712,  0 },
    /* status 198 */ { 3753, 25,  712,  0 },
    /* status 199 */ { 3778, 25,  712,  0 },
    /* status 200 */ { 3803, 25,  712,  0 },
    /* status 201 */ { 3828, 25,  712,  0 },
    /* status 202 */ { 3853, 42,  712,  0 },
    /* status 203 */ { 3895, 15,  712,  5 },
    /* status 204 */ { 3910,  1,  717,  0 },
    /* status 205 */ { 3911,  6,  717,  1 },
    /* status 206 */ { 3917,  4,  718,  0 },
    /* status 207 */ { 3921, 24,  718, 18 },
    /* status 208 */ { 3945, 16,  736, 17 },
    /* status 209 */ { 3961, 20,  753,  0 },
    /* status 210 */ { 3981, 20,  753,  0 },
    /* status 211 */ { 4001, 11,  753,  0 },
    /* status 212 */ { 4012, 20,  753,  0 },
    /* status 213 */ { 4032, 20,  753,  0 },
    /* status 214 */ { 4052, 11,  753,  0 },
    /* status 215 */ { 4063, 20,  753,  0 },
    /* status 216 */ { 4083, 20,  753,  0 },
    /* status 217 */ { 4103,  1,  753,  0 },
    /* status 218 */ { 4104,  2,  753,  0 },
    /* status 219 */ { 4106,  1,  753,  0 },
    /* status 220 */ { 4107,  2,  753,  0 },
    /* status 221 */ { 4109,  2,  753,  0 },
    /* status 222 */ { 4111, 42,  753,  0 },
    /* status 223 */ { 4153, 42,  753,  0 },
    /* status 224 */ { 4195,  2,  753,  0 },
    /* status 225 */ { 4197,  2,  753,  0 },
    /* status 226 */ { 4199,  3,  753,  0 },
    /* status 227 */ { 4202, 16,  753, 19 },
    /* status 228 */ { 4218,  5,  772,  0 },
    /* status 229 */ { 4223,  8,  772,  0 },
    /* status 230 */ { 4231,  8,  772,  0 },
    /* status 231 */ { 4239,  1,  772,  0 },
    /* status 232 */ { 4240,  7,  772,  0 },
    /* status 233 */ { 4247,  1,  772,  0 },
    /* status 234 */ { 4248, 26,  772,  9 },
    /* status 235 */ { 4274,  1,  781,  0 },
    /* status 236 */ { 4275,  2,  781,  0 },
    /* status 237 */ { 4277,  2,  781,  0 },
    /* status 238 */ { 4279,  6,  781,  4 },
    /* status 239 */ { 4285,  7,  785,  0 },
    /* status 240 */ { 4292, 16,  785, 13 },
    /* status 241 */ { 4308, 27,  798,  0 },
    /* status 242 */ { 4335, 16,  798,  0 },
    /* status 243 */ { 4351,  4,  798,  4 },
    /* status 244 */ { 4355, 16,  802, 13 },
    /* status 245 */ { 4371, 27,  815,  0 },
    /* status 246 */ { 4398,  2,  815,  0 },
    /* status 247 */ { 4400, 27,  815,  0 },
    /* status 248 */ { 4427,  1,  815,  1 },
    /* status 249 */ { 4428, 15,  816, 16 },
    /* status 250 */ { 4443, 15,  832, 16 },
    /* status 251 */ { 4458, 36,  848,  0 },
    /* status 252 */ { 4494,  6,  848,  2 },
    /* status 253 */ { 4500, 16,  850, 17 },
    /* status 254 */ { 4516, 21,  867, 16 },
    /* status 255 */ { 4537,  1,  883,  0 },
    /* status 256 */ { 4538,  1,  883,  0 },
    /* status 257 */ { 4539,  1,  883,  0 },
    /* status 258 */ { 4540,  4,  883,  0 },
    /* status 259 */ { 4544, 36,  883,  0 },
    /* status 260 */ { 4580, 42,  883,  0 },
    /* status 261 */ { 4622, 42,  883,  0 },
    /* status 262 */ { 4664, 15,  883, 17 },
    /* status 263 */ { 4679,  2,  900,  0 },
    /* status 264 */ { 4681,  2,  900,  0 },
    /* status 265 */ { 4683,  7,  900,  0 },
    /* status 266 */ { 4690,  7,  900,  0 },
    /* status 267 */ { 4697, 21,  900, 14 },
    /* status 268 */ { 4718,  8,  914,  1 },
    /* status 269 */ { 4726, 24,  915, 20 },
    /* status 270 */ { 4750,  2,  935,  0 },
    /* status 271 */ { 4752,  2,  935,  0 },
    /* status 272 */ { 4754, 27,  935,  0 },
    /* status 273 */ { 4781,  2,  935,  0 },
    /* status 274 */ { 4783,  2,  935,  0 },
    /* status 275 */ { 4785,  3,  935,  0 },
    /* status 276 */ { 4788, 15,  935, 16 },
    /* status 277 */ { 4803, 27,  951,  0 },
    /* status 278 */ { 4830, 27,  951,  0 },
    /* status 279 */ { 4857,  2,  951,  0 },
    /* status 280 */ { 4859,  2,  951,  0 },
    /* status 281 */ { 4861,  6,  951,  0 },
    /* status 282 */ { 4867,  2,  951,  0 },
    /* status 283 */ { 4869,  1,  951,  0 },
    /* status 284 */ { 4870,  4,  951,  0 },
    /* status 285 */ { 4874,  1,  951,  0 },
    /* status 286 */ { 4875,  4,  951,  0 },
    /* status 287 */ { 4879,  4,  951,  0 },
    /* status 288 */ { 4883,  4,  951,  0 },
    /* status 289 */ { 4887,  2,  951,  0 },
    /* status 290 */ { 4889,  3,  951,  0 },
    /* status 291 */ { 4892, 18,  951, 18 },
    /* status 292 */ { 4910,  2,  969,  0 },
    /* status 293 */ { 4912,  2,  969,  0 },
    /* status 294 */ { 4914,  8,  969,  4 },
    /* status 295 */ { 4922, 28,  973,  0 },
    /* status 296 */ { 4950, 16,  973,  0 },
    /* status 297 */ { 4966,  4,  973,  3 },
    /* status 298 */ { 4970, 15,  976, 16 },
    /* status 299 */ { 4985,  2,  992,  0 },
    /* status 300 */ { 4987,  4,  992,  0 },
    /* status 301 */ { 4991,  4,  992,  0 },
    /* status 302 */ { 4995,  2,  992,  0 },
    /* status 303 */ { 4997,  2,  992,  0 },
    /* status 304 */ { 4999,  2,  992,  0 }
};

/* ==================================================
 * Tabula cocta integra
 * ================================================== */

constans SilvaTabulaCocta SILVA_C89_TABULA = {
    SILVA_C89_SYMBOLA, 134,
    72,  /* terminalia */
    SILVA_C89_PRODUCTIONES, 197,
    SILVA_C89_STATUS, 305,
    SILVA_C89_ACTIONES, 5001,
    SILVA_C89_GOTO, 992,
    133,  /* initium (augmentatum) */
    196,  /* productio augmentata */
    14   /* conflictus servati */
};

/* ==================================================
 * Registrum generum (layouts locorum)
 * ================================================== */

hic_manens constans SilvaTabLocus SILVA_C89_LOCI[] = {
    /* declaratio */
    { "specificatores", SILVA_LOCUS_LISTA_MIXTA },
    { "declaratores", SILVA_LOCUS_LISTA_MIXTA },
    { "tok_terminator", SILVA_LOCUS_TOKEN },
    /* typus-nominatus */
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    /* typus-primitivus */
    { "tok_verba", SILVA_LOCUS_LISTA_MIXTA },
    /* declarator-initiatus */
    { "declarator", SILVA_LOCUS_NODUS },
    { "tok_operator", SILVA_LOCUS_TOKEN },
    { "initiator", SILVA_LOCUS_NODUS },
    /* congeries */
    { "tok_aperta", SILVA_LOCUS_TOKEN },
    { "elementa", SILVA_LOCUS_LISTA_MIXTA },
    { "tok_clausa", SILVA_LOCUS_TOKEN },
    /* structura */
    { "tok_verbum", SILVA_LOCUS_TOKEN },
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    { "tok_aperta", SILVA_LOCUS_TOKEN },
    { "membra", SILVA_LOCUS_LISTA_MIXTA },
    { "tok_clausa", SILVA_LOCUS_TOKEN },
    /* unio */
    { "tok_verbum", SILVA_LOCUS_TOKEN },
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    { "tok_aperta", SILVA_LOCUS_TOKEN },
    { "membra", SILVA_LOCUS_LISTA_MIXTA },
    { "tok_clausa", SILVA_LOCUS_TOKEN },
    /* membrum */
    { "specificatores", SILVA_LOCUS_LISTA_MIXTA },
    { "declaratores", SILVA_LOCUS_LISTA_MIXTA },
    { "tok_terminator", SILVA_LOCUS_TOKEN },
    /* campus */
    { "declarator", SILVA_LOCUS_NODUS },
    { "tok_colon", SILVA_LOCUS_TOKEN },
    { "latitudo", SILVA_LOCUS_NODUS },
    /* enumeratio */
    { "tok_verbum", SILVA_LOCUS_TOKEN },
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    { "tok_aperta", SILVA_LOCUS_TOKEN },
    { "enumeratores", SILVA_LOCUS_LISTA_MIXTA },
    { "tok_clausa", SILVA_LOCUS_TOKEN },
    /* enumerator */
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    { "tok_operator", SILVA_LOCUS_TOKEN },
    { "valor", SILVA_LOCUS_NODUS },
    /* declarator-monstrator */
    { "tok_stella", SILVA_LOCUS_TOKEN },
    { "qualificatores", SILVA_LOCUS_LISTA_MIXTA },
    { "internum", SILVA_LOCUS_NODUS },
    /* declarator-titulus */
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    /* parenthesis */
    { "tok_apertum", SILVA_LOCUS_TOKEN },
    { "internum", SILVA_LOCUS_NODUS },
    { "tok_clausum", SILVA_LOCUS_TOKEN },
    /* declarator-aciei */
    { "internum", SILVA_LOCUS_NODUS },
    { "tok_apertum", SILVA_LOCUS_TOKEN },
    { "mensura", SILVA_LOCUS_NODUS },
    { "tok_clausum", SILVA_LOCUS_TOKEN },
    /* declarator-functionis */
    { "internum", SILVA_LOCUS_NODUS },
    { "tok_apertum", SILVA_LOCUS_TOKEN },
    { "parametra", SILVA_LOCUS_LISTA_MIXTA },
    { "tok_clausum", SILVA_LOCUS_TOKEN },
    /* parametrum */
    { "specificatores", SILVA_LOCUS_LISTA_MIXTA },
    { "declarator", SILVA_LOCUS_NODUS },
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
    /* declarator-abstractus */
    { "tok_stella", SILVA_LOCUS_TOKEN },
    { "qualificatores", SILVA_LOCUS_LISTA_MIXTA },
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
    /* [ 0] */ { "declaratio", 0, 3 },
    /* [ 1] */ { "typus-nominatus", 3, 1 },
    /* [ 2] */ { "typus-primitivus", 4, 1 },
    /* [ 3] */ { "declarator-initiatus", 5, 3 },
    /* [ 4] */ { "congeries", 8, 3 },
    /* [ 5] */ { "structura", 11, 5 },
    /* [ 6] */ { "unio", 16, 5 },
    /* [ 7] */ { "membrum", 21, 3 },
    /* [ 8] */ { "campus", 24, 3 },
    /* [ 9] */ { "enumeratio", 27, 5 },
    /* [10] */ { "enumerator", 32, 3 },
    /* [11] */ { "declarator-monstrator", 35, 3 },
    /* [12] */ { "declarator-titulus", 38, 1 },
    /* [13] */ { "parenthesis", 39, 3 },
    /* [14] */ { "declarator-aciei", 42, 4 },
    /* [15] */ { "declarator-functionis", 46, 4 },
    /* [16] */ { "parametrum", 50, 2 },
    /* [17] */ { "sententia-expressionis", 52, 2 },
    /* [18] */ { "virgula", 54, 3 },
    /* [19] */ { "assignatio", 57, 3 },
    /* [20] */ { "ternarius", 60, 5 },
    /* [21] */ { "binarium", 65, 3 },
    /* [22] */ { "conversio", 68, 4 },
    /* [23] */ { "unarium", 72, 2 },
    /* [24] */ { "magnitudo-expressionis", 74, 2 },
    /* [25] */ { "magnitudo-typi", 76, 4 },
    /* [26] */ { "species-typi", 80, 2 },
    /* [27] */ { "declarator-abstractus", 82, 3 },
    /* [28] */ { "subscriptio", 85, 4 },
    /* [29] */ { "vocatio", 89, 4 },
    /* [30] */ { "accessus", 93, 3 },
    /* [31] */ { "postcrementum", 96, 2 },
    /* [32] */ { "folium-identificator", 98, 1 },
    /* [33] */ { "folium-integer", 99, 1 },
    /* [34] */ { "folium-fluitans", 100, 1 },
    /* [35] */ { "folium-character", 101, 1 },
    /* [36] */ { "folium-chorda", 102, 1 },
    /* [37] */ { "error", 103, 1 },
    /* [38] */ { "ambiguus", 104, 2 },
    /* [39] */ { "conditionalis", 106, 2 },
    /* [40] */ { "ramus-sumptus", 108, 3 },
    /* [41] */ { "ramus-omissus", 111, 3 },
    { NIHIL, 0, 0 }   /* terminator */
};

constans SilvaRegistrumCoctum SILVA_C89_REGISTRUM = {
    SILVA_C89_GENERA, 42,
    SILVA_C89_LOCI, 114
};

/* ==================================================
 * Accessores (probati: genus + fines)
 * ================================================== */

SilvaValor
silva_c89_declaratio_specificatores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_declaratio_declaratores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_declaratio_tok_terminator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
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
silva_c89_declarator_initiatus_declarator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_declarator_initiatus_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_declarator_initiatus_initiator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_congeries_tok_aperta (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CONGERIES
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_congeries_elementa (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CONGERIES
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_congeries_tok_clausa (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CONGERIES
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_structura_tok_verbum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_STRUCTURA
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_structura_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_STRUCTURA
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_structura_tok_aperta (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_STRUCTURA
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_structura_membra (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_STRUCTURA
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_structura_tok_clausa (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_STRUCTURA
        || 4 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[4];
}

SilvaValor
silva_c89_unio_tok_verbum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_UNIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_unio_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_UNIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_unio_tok_aperta (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_UNIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_unio_membra (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_UNIO
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_unio_tok_clausa (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_UNIO
        || 4 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[4];
}

SilvaValor
silva_c89_membrum_specificatores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_MEMBRUM
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_membrum_declaratores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_MEMBRUM
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_membrum_tok_terminator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_MEMBRUM
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_campus_declarator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CAMPUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_campus_tok_colon (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CAMPUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_campus_latitudo (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_CAMPUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_enumeratio_tok_verbum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ENUMERATIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_enumeratio_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ENUMERATIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_enumeratio_tok_aperta (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ENUMERATIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_enumeratio_enumeratores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ENUMERATIO
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_enumeratio_tok_clausa (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ENUMERATIO
        || 4 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[4];
}

SilvaValor
silva_c89_enumerator_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ENUMERATOR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_enumerator_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ENUMERATOR
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_enumerator_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_ENUMERATOR
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_declarator_monstrator_tok_stella (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_declarator_monstrator_qualificatores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_declarator_monstrator_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_declarator_titulus_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS
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
silva_c89_declarator_aciei_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_declarator_aciei_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_declarator_aciei_mensura (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_declarator_aciei_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_declarator_functionis_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_declarator_functionis_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_declarator_functionis_parametra (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_c89_declarator_functionis_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS
        || 3 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[3];
}

SilvaValor
silva_c89_parametrum_specificatores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_PARAMETRUM
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_c89_parametrum_declarator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_PARAMETRUM
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

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
silva_c89_declarator_abstractus_qualificatores (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_c89_declarator_abstractus_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
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
    casus 3:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 4:  /* declaratio-plena */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 5:  /* declaratio-nuda */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 6:  /* typus-nominatus */
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
    casus 7:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 8:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 9:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 10:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 11:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 12:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 13:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 14:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 15:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 16:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 17:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 18:  /* sd-pp-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 19:  /* sd-pp-ex-alio */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 20:  /* sd-pp-ex-tag */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 21:  /* sd-pa-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 22:  /* sd-pa-ex-primitivo */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 23:  /* sd-pa-ex-alio */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 24:  /* sd-pa-ex-tag */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 25:  /* sd-pt-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 26:  /* sd-pt-ex-primitivo */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 27:  /* sd-pt-ex-alio */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 28:  /* sd-pt-ex-tag */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 29:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 30:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 31:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 32:  /* typus-primitivus */
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
    casus 33:  /* declaratores-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 34:  /* declaratores-appendere */
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
    casus 35:  /* declarator-initiatus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_INITIATUS, 3);
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
    casus 36:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 37:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 38:  /* congeries */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_CONGERIES, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 39:  /* congeries-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 40:  /* congeries-appendere */
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
    casus 41:  /* congeries-cauda */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 42:  /* structura-plena */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_STRUCTURA, 5);
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
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[3], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 4, valores[4], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 43:  /* structura-anonyma */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_STRUCTURA, 5);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[2], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 4, valores[3], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 44:  /* structura-nominata */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_STRUCTURA, 5);
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
        redde silva_valor_nodus(nodus);
    }
    casus 45:  /* unio-plena */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNIO, 5);
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
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[3], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 4, valores[4], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 46:  /* unio-anonyma */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNIO, 5);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[2], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 4, valores[3], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 47:  /* unio-nominata */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_UNIO, 5);
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
        redde silva_valor_nodus(nodus);
    }
    casus 48:  /* transitus */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        redde lista;
    }
    casus 49:  /* membra-appendere */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 50:  /* membrum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_MEMBRUM, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 51:  /* membra-decl-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 52:  /* membra-decl-appendere */
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
    casus 53:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 54:  /* campus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_CAMPUS, 3);
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
    casus 55:  /* campus-sine-declaratore */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_CAMPUS, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 56:  /* enumeratio-plena */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ENUMERATIO, 5);
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
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[3], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 4, valores[4], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 57:  /* enumeratio-anonyma */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ENUMERATIO, 5);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[2], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 4, valores[3], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 58:  /* enumeratio-nominata */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ENUMERATIO, 5);
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
        redde silva_valor_nodus(nodus);
    }
    casus 59:  /* enumeratores-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 60:  /* enumeratores-appendere */
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
    casus 61:  /* enumerator */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ENUMERATOR, 3);
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
    casus 62:  /* enumerator-valoratus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_ENUMERATOR, 3);
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
        redde silva_valor_nodus(nodus);
    }
    casus 63:  /* declarator-monstrator */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_MONSTRATOR, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 64:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 65:  /* transitus */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        redde lista;
    }
    casus 66:  /* qualificatores-const */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 67:  /* qualificatores-volatile */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 68:  /* declarator-titulus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_TITULUS, 1);
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
    casus 69:  /* declarator-parenthesis */
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
    casus 70:  /* declarator-aciei */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI, 4);
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
    casus 71:  /* declarator-aciei-vacua */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI, 4);
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
        si (!silva_nodus_ponere(nodus, 3, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 72:  /* declarator-functionis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS, 4);
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
    casus 73:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 74:  /* transitus */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        redde lista;
    }
    casus 75:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 76:  /* parametra-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 77:  /* parametra-appendere */
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
    casus 78:  /* parametra-ellipsis */
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
    casus 79:  /* parametrum-nudum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_PARAMETRUM, 2);
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
    casus 80:  /* parametrum-declaratum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_PARAMETRUM, 2);
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
    casus 81:  /* parametrum-abstractum */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_PARAMETRUM, 2);
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
    casus 82:  /* sententia-expr */
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
    casus 83:  /* virgula-series */
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
    casus 84:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 85:  /* assignatio-simplex */
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
    casus 86:  /* assignatio-plus */
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
    casus 87:  /* assignatio-minus */
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
    casus 88:  /* assignatio-stella */
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
    casus 89:  /* assignatio-solidus */
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
    casus 90:  /* assignatio-percentum */
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
    casus 91:  /* assignatio-ampersand */
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
    casus 92:  /* assignatio-barra */
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
    casus 93:  /* assignatio-caret */
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
    casus 94:  /* assignatio-sinistrorsum */
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
    casus 95:  /* assignatio-dextrorsum */
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
    casus 96:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 97:  /* ternarius-conditio */
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
    casus 98:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 99:  /* binarium-disiunctio */
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
    casus 100:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 101:  /* binarium-coniunctio */
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
    casus 102:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 103:  /* binarium-vel */
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
    casus 104:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 105:  /* binarium-aut */
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
    casus 106:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 107:  /* binarium-et */
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
    casus 108:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 109:  /* binarium-aequalis */
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
    casus 110:  /* binarium-non-aequalis */
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
    casus 111:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 112:  /* binarium-minor */
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
    casus 113:  /* binarium-maior */
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
    casus 114:  /* binarium-minor-aequalis */
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
    casus 115:  /* binarium-maior-aequalis */
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
    casus 116:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 117:  /* binarium-sinistrorsum */
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
    casus 118:  /* binarium-dextrorsum */
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
    casus 119:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 120:  /* binarium-plus */
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
    casus 121:  /* binarium-minus */
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
    casus 122:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 123:  /* binarium-stella */
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
    casus 124:  /* binarium-solidus */
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
    casus 125:  /* binarium-percentum */
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
    casus 126:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 127:  /* conversio-typus */
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
    casus 128:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 129:  /* unarium-incrementum */
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
    casus 130:  /* unarium-decrementum */
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
    casus 131:  /* unarium-ampersand */
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
    casus 132:  /* unarium-stella */
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
    casus 133:  /* unarium-plus */
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
    casus 134:  /* unarium-minus */
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
    casus 135:  /* unarium-tilde */
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
    casus 136:  /* unarium-exclamatio */
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
    casus 137:  /* magnitudo-expressionis */
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
    casus 138:  /* magnitudo-typi */
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
    casus 139:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 140:  /* species-typi-plena */
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
    casus 141:  /* species-typi-nuda */
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
    casus 142:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 143:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 144:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 145:  /* sq-pp-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 146:  /* sq-pp-ex-alio */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 147:  /* sq-pp-ex-tag */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 148:  /* sq-pa-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 149:  /* sq-pa-ex-primitivo */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 150:  /* sq-pa-ex-alio */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 151:  /* sq-pa-ex-tag */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 152:  /* sq-pt-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 153:  /* sq-pt-ex-primitivo */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 154:  /* sq-pt-ex-alio */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 155:  /* sq-pt-ex-tag */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 156:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 157:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 158:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 159:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 160:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 161:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 162:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 163:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 164:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 165:  /* verba-primitiva-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 166:  /* verba-primitiva-appendere */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 167:  /* declarator-abstractus-stella */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 168:  /* declarator-abstractus-catena */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ABSTRACTUS, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[1], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[2], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 169:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 170:  /* abstractus-parenthesis */
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
    casus 171:  /* abstractus-aciei */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI, 4);
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
    casus 172:  /* abstractus-aciei-sine */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI, 4);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[1], SILVA_LOCUS_NODUS))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 173:  /* abstractus-aciei-vacua */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI, 4);
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
        si (!silva_nodus_ponere(nodus, 3, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 174:  /* abstractus-aciei-nuda */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_ACIEI, 4);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[1], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 175:  /* abstractus-functionis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS, 4);
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
    casus 176:  /* abstractus-functionis-sine */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS, 4);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 1, valores[0], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 2, valores[1], SILVA_LOCUS_LISTA_MIXTA))
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 3, valores[2], SILVA_LOCUS_TOKEN))
        {
            redde silva_valor_nihil();
        }
        redde silva_valor_nodus(nodus);
    }
    casus 177:  /* subscriptio-index */
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
    casus 178:  /* vocatio-argumenta */
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
    casus 179:  /* accessus-punctum */
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
    casus 180:  /* accessus-sagitta */
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
    casus 181:  /* postcrementum-incrementum */
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
    casus 182:  /* postcrementum-decrementum */
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
    casus 183:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 184:  /* transitus */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        redde lista;
    }
    casus 185:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 186:  /* argumenta-primum */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 187:  /* argumenta-appendere */
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
    casus 188:  /* chordae-prima */
    {
        SilvaValor lista = silva_valor_lista_nova(piscina);
        lista = silva_valor_lista_appendere(piscina, lista, valores[0]);
        redde lista;
    }
    casus 189:  /* chordae-appendere */
    {
        SilvaValor lista = valores[0];
        si (lista.genus != SILVA_VALOR_LISTA)
        {
            redde silva_valor_nihil();
        }
        lista = silva_valor_lista_appendere(piscina, lista, valores[1]);
        redde lista;
    }
    casus 190:  /* primarium-identificator */
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
    casus 191:  /* primarium-integer */
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
    casus 192:  /* primarium-fluitans */
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
    casus 193:  /* primarium-character */
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
    casus 194:  /* primarium-chorda */
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
    casus 195:  /* primarium-parenthesis */
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
    casus 196:  /* augmentata */
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
