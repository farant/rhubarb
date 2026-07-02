/* silva_tabulae_imparilis.c - GENERATUM per silva_coquere ex grammatica/sceletum_imparilis.stml
 * NOLI MANU MUTARE - regenera per silva/generare.sh.
 *
 * status: 28, productiones: 19, symbola: 20,
 * conflictus servati (GLR): 1
 */

#include "silva_tabulae_imparilis.h"

/* ==================================================
 * Symbola
 * ================================================== */

hic_manens constans SilvaTabSymbolum SILVA_IMPARILIS_SYMBOLA[20] = {
    /* [ 0] */ { "IDENTIFICATOR", SILVA_LEX_IDENTIFICATOR, VERUM },
    /* [ 1] */ { "INTEGER", SILVA_LEX_INTEGER, VERUM },
    /* [ 2] */ { "INT", SILVA_LEX_INT, VERUM },
    /* [ 3] */ { "STAR", SILVA_LEX_STAR, VERUM },
    /* [ 4] */ { "PLUS", SILVA_LEX_PLUS, VERUM },
    /* [ 5] */ { "SEMICOLON", SILVA_LEX_SEMICOLON, VERUM },
    /* [ 6] */ { "PAREN_APERTA", SILVA_LEX_PAREN_APERTA, VERUM },
    /* [ 7] */ { "PAREN_CLAUSA", SILVA_LEX_PAREN_CLAUSA, VERUM },
    /* [ 8] */ { "EOF", SILVA_LEX_EOF, VERUM },
    /* [ 9] */ { "sententiae", SILVA_LEX_EOF, FALSUM },
    /* [10] */ { "sententia", SILVA_LEX_EOF, FALSUM },
    /* [11] */ { "mandatum", SILVA_LEX_EOF, FALSUM },
    /* [12] */ { "declaratio", SILVA_LEX_EOF, FALSUM },
    /* [13] */ { "typus", SILVA_LEX_EOF, FALSUM },
    /* [14] */ { "declarator", SILVA_LEX_EOF, FALSUM },
    /* [15] */ { "sententia-expressionis", SILVA_LEX_EOF, FALSUM },
    /* [16] */ { "expressio", SILVA_LEX_EOF, FALSUM },
    /* [17] */ { "terminus", SILVA_LEX_EOF, FALSUM },
    /* [18] */ { "factor", SILVA_LEX_EOF, FALSUM },
    /* [19] */ { "__initium__", SILVA_LEX_EOF, FALSUM }
};

/* ==================================================
 * Productiones
 * ================================================== */

hic_manens constans SilvaTabProductio SILVA_IMPARILIS_PRODUCTIONES[19] = {
    /* P0  sententiae (0 symbola) */ { 9, 0, NIHIL },
    /* P1  sententiae (2 symbola) */ { 9, 2, "sententiae-appendere" },
    /* P2  sententia (1 symbola) */ { 10, 1, NIHIL },
    /* P3  sententia (1 symbola) */ { 10, 1, NIHIL },
    /* P4  mandatum (1 symbola) */ { 11, 1, NIHIL },
    /* P5  declaratio (3 symbola) */ { 12, 3, "declaratio-simplex" },
    /* P6  typus (1 symbola) */ { 13, 1, "typus-int" },
    /* P7  typus (1 symbola) */ { 13, 1, "typus-nomen" },
    /* P8  declarator (1 symbola) */ { 14, 1, "declarator-nomen" },
    /* P9  declarator (2 symbola) */ { 14, 2, "declarator-stella" },
    /* P10 sententia-expressionis (2 symbola) */ { 15, 2, "sententia-expr" },
    /* P11 expressio (3 symbola) */ { 16, 3, "expressio-plus" },
    /* P12 expressio (1 symbola) */ { 16, 1, NIHIL },
    /* P13 terminus (3 symbola) */ { 17, 3, "terminus-star" },
    /* P14 terminus (1 symbola) */ { 17, 1, NIHIL },
    /* P15 factor (1 symbola) */ { 18, 1, "factor-identificator" },
    /* P16 factor (1 symbola) */ { 18, 1, "factor-integer" },
    /* P17 factor (3 symbola) */ { 18, 3, "factor-parenthesis" },
    /* P18 __initium__ (2 symbola) */ { 19, 2, NIHIL }
};

/* ==================================================
 * Actiones (laminae per statum in SILVA_IMPARILIS_STATUS)
 * ================================================== */

hic_manens constans SilvaTabActio SILVA_IMPARILIS_ACTIONES[] = {
    /* --- status 0 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* IDENTIFICATOR: reducere P0 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INTEGER: reducere P0 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INT: reducere P0 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* PAREN_APERTA: reducere P0 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* EOF: reducere P0 */
    /* --- status 1 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,   8, 0 },  /* IDENTIFICATOR: transponere -> 8 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* INTEGER: transponere -> 12 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,   7, 0 },  /* INT: transponere -> 7 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* PAREN_APERTA: transponere -> 13 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  14, 0 },  /* EOF: transponere -> 14 */
    /* --- status 2 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* IDENTIFICATOR: reducere P1 sententiae-appendere */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* INTEGER: reducere P1 sententiae-appendere */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* INT: reducere P1 sententiae-appendere */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* PAREN_APERTA: reducere P1 sententiae-appendere */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   1, 0 },  /* EOF: reducere P1 sententiae-appendere */
    /* --- status 3 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* IDENTIFICATOR: reducere P2 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* INTEGER: reducere P2 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* INT: reducere P2 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* PAREN_APERTA: reducere P2 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   2, 0 },  /* EOF: reducere P2 */
    /* --- status 4 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* IDENTIFICATOR: reducere P3 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* INTEGER: reducere P3 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* INT: reducere P3 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* PAREN_APERTA: reducere P3 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   3, 0 },  /* EOF: reducere P3 */
    /* --- status 5 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* IDENTIFICATOR: reducere P4 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* INTEGER: reducere P4 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* INT: reducere P4 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* PAREN_APERTA: reducere P4 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* EOF: reducere P4 */
    /* --- status 6 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  16, 0 },  /* IDENTIFICATOR: transponere -> 16 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  17, 0 },  /* STAR: transponere -> 17 */
    /* --- status 7 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* IDENTIFICATOR: reducere P6 typus-int */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STAR: reducere P6 typus-int */
    /* --- status 8 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* IDENTIFICATOR: reducere P7 typus-nomen */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* STAR: reducere P7 typus-nomen */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* STAR: reducere P15 factor-identificator */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PLUS: reducere P15 factor-identificator */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* SEMICOLON: reducere P15 factor-identificator */
    /* --- status 9 --- */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PLUS: transponere -> 19 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  18, 0 },  /* SEMICOLON: transponere -> 18 */
    /* --- status 10 --- */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* STAR: transponere -> 20 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* PLUS: reducere P12 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* SEMICOLON: reducere P12 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* PAREN_CLAUSA: reducere P12 */
    /* --- status 11 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* STAR: reducere P14 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PLUS: reducere P14 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* SEMICOLON: reducere P14 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PAREN_CLAUSA: reducere P14 */
    /* --- status 12 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* STAR: reducere P16 factor-integer */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* PLUS: reducere P16 factor-integer */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* SEMICOLON: reducere P16 factor-integer */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* PAREN_CLAUSA: reducere P16 factor-integer */
    /* --- status 13 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* IDENTIFICATOR: transponere -> 22 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* INTEGER: transponere -> 12 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* PAREN_APERTA: transponere -> 13 */
    /* --- status 14 --- */
    {  -1, SILVA_TAB_ACTIO_ACCIPERE,   0, 0 },  /* $: accipere */
    /* --- status 15 --- */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  23, 0 },  /* SEMICOLON: transponere -> 23 */
    /* --- status 16 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* SEMICOLON: reducere P8 declarator-nomen */
    /* --- status 17 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  16, 0 },  /* IDENTIFICATOR: transponere -> 16 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  17, 0 },  /* STAR: transponere -> 17 */
    /* --- status 18 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* IDENTIFICATOR: reducere P10 sententia-expr */
    {   1, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* INTEGER: reducere P10 sententia-expr */
    {   2, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* INT: reducere P10 sententia-expr */
    {   6, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* PAREN_APERTA: reducere P10 sententia-expr */
    {   8, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* EOF: reducere P10 sententia-expr */
    /* --- status 19 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* IDENTIFICATOR: transponere -> 22 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* INTEGER: transponere -> 12 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* PAREN_APERTA: transponere -> 13 */
    /* --- status 20 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* IDENTIFICATOR: transponere -> 22 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* INTEGER: transponere -> 12 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* PAREN_APERTA: transponere -> 13 */
    /* --- status 21 --- */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* PLUS: transponere -> 19 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  27, 0 },  /* PAREN_CLAUSA: transponere -> 27 */
    /* --- status 22 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* STAR: reducere P15 factor-identificator */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PLUS: reducere P15 factor-identificator */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* SEMICOLON: reducere P15 factor-identificator */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PAREN_CLAUSA: reducere P15 factor-identificator */
    /* --- status 23 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* IDENTIFICATOR: reducere P5 declaratio-simplex */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* INTEGER: reducere P5 declaratio-simplex */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* INT: reducere P5 declaratio-simplex */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* PAREN_APERTA: reducere P5 declaratio-simplex */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* EOF: reducere P5 declaratio-simplex */
    /* --- status 24 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* SEMICOLON: reducere P9 declarator-stella */
    /* --- status 25 --- */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  20, 0 },  /* STAR: transponere -> 20 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* PLUS: reducere P11 expressio-plus */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* SEMICOLON: reducere P11 expressio-plus */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* PAREN_CLAUSA: reducere P11 expressio-plus */
    /* --- status 26 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* STAR: reducere P13 terminus-star */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* PLUS: reducere P13 terminus-star */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* SEMICOLON: reducere P13 terminus-star */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* PAREN_CLAUSA: reducere P13 terminus-star */
    /* --- status 27 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* STAR: reducere P17 factor-parenthesis */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* PLUS: reducere P17 factor-parenthesis */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* SEMICOLON: reducere P17 factor-parenthesis */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  17, 0 },  /* PAREN_CLAUSA: reducere P17 factor-parenthesis */
};

/* ==================================================
 * Goto
 * ================================================== */

hic_manens constans SilvaTabGoto SILVA_IMPARILIS_GOTO[] = {
    /* --- status 0 --- */
    {   9,   1 },  /* sententiae -> 1 */
    /* --- status 1 --- */
    {  10,   2 },  /* sententia -> 2 */
    {  11,   4 },  /* mandatum -> 4 */
    {  12,   3 },  /* declaratio -> 3 */
    {  13,   6 },  /* typus -> 6 */
    {  15,   5 },  /* sententia-expressionis -> 5 */
    {  16,   9 },  /* expressio -> 9 */
    {  17,  10 },  /* terminus -> 10 */
    {  18,  11 },  /* factor -> 11 */
    /* --- status 6 --- */
    {  14,  15 },  /* declarator -> 15 */
    /* --- status 13 --- */
    {  16,  21 },  /* expressio -> 21 */
    {  17,  10 },  /* terminus -> 10 */
    {  18,  11 },  /* factor -> 11 */
    /* --- status 17 --- */
    {  14,  24 },  /* declarator -> 24 */
    /* --- status 19 --- */
    {  17,  25 },  /* terminus -> 25 */
    {  18,  11 },  /* factor -> 11 */
    /* --- status 20 --- */
    {  18,  26 },  /* factor -> 26 */
    { -1, -1 }   /* terminator */
};

/* ==================================================
 * Status (offset + numerus in series planas)
 * ================================================== */

hic_manens constans SilvaTabStatus SILVA_IMPARILIS_STATUS[28] = {
    /* status  0 */ {    0,  5,    0,  1 },
    /* status  1 */ {    5,  5,    1,  8 },
    /* status  2 */ {   10,  5,    9,  0 },
    /* status  3 */ {   15,  5,    9,  0 },
    /* status  4 */ {   20,  5,    9,  0 },
    /* status  5 */ {   25,  5,    9,  0 },
    /* status  6 */ {   30,  2,    9,  1 },
    /* status  7 */ {   32,  2,   10,  0 },
    /* status  8 */ {   34,  5,   10,  0 },
    /* status  9 */ {   39,  2,   10,  0 },
    /* status 10 */ {   41,  4,   10,  0 },
    /* status 11 */ {   45,  4,   10,  0 },
    /* status 12 */ {   49,  4,   10,  0 },
    /* status 13 */ {   53,  3,   10,  3 },
    /* status 14 */ {   56,  1,   13,  0 },
    /* status 15 */ {   57,  1,   13,  0 },
    /* status 16 */ {   58,  1,   13,  0 },
    /* status 17 */ {   59,  2,   13,  1 },
    /* status 18 */ {   61,  5,   14,  0 },
    /* status 19 */ {   66,  3,   14,  2 },
    /* status 20 */ {   69,  3,   16,  1 },
    /* status 21 */ {   72,  2,   17,  0 },
    /* status 22 */ {   74,  4,   17,  0 },
    /* status 23 */ {   78,  5,   17,  0 },
    /* status 24 */ {   83,  1,   17,  0 },
    /* status 25 */ {   84,  4,   17,  0 },
    /* status 26 */ {   88,  4,   17,  0 },
    /* status 27 */ {   92,  4,   17,  0 }
};

/* ==================================================
 * Tabula cocta integra
 * ================================================== */

constans SilvaTabulaCocta SILVA_IMPARILIS_TABULA = {
    SILVA_IMPARILIS_SYMBOLA, 20,
    9,  /* terminalia */
    SILVA_IMPARILIS_PRODUCTIONES, 19,
    SILVA_IMPARILIS_STATUS, 28,
    SILVA_IMPARILIS_ACTIONES, 96,
    SILVA_IMPARILIS_GOTO, 17,
    19,  /* initium (augmentatum) */
    18,  /* productio augmentata */
    1   /* conflictus servati */
};

/* ==================================================
 * Registrum generum (layouts locorum)
 * ================================================== */

hic_manens constans SilvaTabLocus SILVA_IMPARILIS_LOCI[] = {
    /* declaratio */
    { "typus", SILVA_LOCUS_NODUS },
    { "declarator", SILVA_LOCUS_NODUS },
    { "tok_terminator", SILVA_LOCUS_TOKEN },
    /* typus-primitivus */
    { "tok_verbum", SILVA_LOCUS_TOKEN },
    /* typus-nominatus */
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    /* declarator-titulus */
    { "tok_titulus", SILVA_LOCUS_TOKEN },
    /* declarator-monstrator */
    { "tok_stella", SILVA_LOCUS_TOKEN },
    { "internum", SILVA_LOCUS_NODUS },
    /* sententia-expressionis */
    { "expressio", SILVA_LOCUS_NODUS },
    { "tok_terminator", SILVA_LOCUS_TOKEN },
    /* binarium */
    { "sinister", SILVA_LOCUS_NODUS },
    { "tok_operator", SILVA_LOCUS_TOKEN },
    { "dexter", SILVA_LOCUS_NODUS },
    /* folium-identificator */
    { "tok_valor", SILVA_LOCUS_TOKEN },
    /* folium-integer */
    { "tok_valor", SILVA_LOCUS_TOKEN },
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
    { NIHIL, -1 }   /* terminator */
};

hic_manens constans SilvaTabGenus SILVA_IMPARILIS_GENERA[] = {
    /* [ 0] */ { "declaratio", 0, 3 },
    /* [ 1] */ { "typus-primitivus", 3, 1 },
    /* [ 2] */ { "typus-nominatus", 4, 1 },
    /* [ 3] */ { "declarator-titulus", 5, 1 },
    /* [ 4] */ { "declarator-monstrator", 6, 2 },
    /* [ 5] */ { "sententia-expressionis", 8, 2 },
    /* [ 6] */ { "binarium", 10, 3 },
    /* [ 7] */ { "folium-identificator", 13, 1 },
    /* [ 8] */ { "folium-integer", 14, 1 },
    /* [ 9] */ { "parenthesis", 15, 3 },
    /* [10] */ { "error", 18, 1 },
    /* [11] */ { "ambiguus", 19, 2 },
    /* [12] */ { "conditionalis", 21, 1 },
    { NIHIL, 0, 0 }   /* terminator */
};

constans SilvaRegistrumCoctum SILVA_IMPARILIS_REGISTRUM = {
    SILVA_IMPARILIS_GENERA, 13,
    SILVA_IMPARILIS_LOCI, 22
};

/* ==================================================
 * Accessores (probati: genus + fines)
 * ================================================== */

SilvaValor
silva_imparilis_declaratio_typus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_DECLARATIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_declaratio_declarator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_DECLARATIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_imparilis_declaratio_tok_terminator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_DECLARATIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_imparilis_typus_primitivus_tok_verbum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_TYPUS_PRIMITIVUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_typus_nominatus_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_TYPUS_NOMINATUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_declarator_titulus_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_DECLARATOR_TITULUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_declarator_monstrator_tok_stella (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_DECLARATOR_MONSTRATOR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_declarator_monstrator_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_DECLARATOR_MONSTRATOR
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_imparilis_sententia_expressionis_expressio (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_SENTENTIA_EXPRESSIONIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_sententia_expressionis_tok_terminator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_SENTENTIA_EXPRESSIONIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_imparilis_binarium_sinister (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_BINARIUM
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_binarium_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_BINARIUM
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_imparilis_binarium_dexter (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_BINARIUM
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_imparilis_folium_identificator_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_FOLIUM_IDENTIFICATOR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_folium_integer_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_FOLIUM_INTEGER
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_parenthesis_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_PARENTHESIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_parenthesis_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_PARENTHESIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_imparilis_parenthesis_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_PARENTHESIS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_imparilis_error_tokens (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_ERROR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_ambiguus_interpretationes (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_AMBIGUUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_imparilis_ambiguus_canonica (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_AMBIGUUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_imparilis_conditionalis_rami (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_IMPARILIS_GENUS_CONDITIONALIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

/* ==================================================
 * Constructio ex reductione (PURA - S26; pater numquam - S27)
 * ================================================== */

SilvaValor
silva_imparilis_construere (
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
    casus 1:  /* sententiae-appendere */
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
    casus 4:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 5:  /* declaratio-simplex */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_DECLARATIO, 3);
        si (nodus == NIHIL)
        {
            redde silva_valor_nihil();
        }
        si (!silva_nodus_ponere(nodus, 0, valores[0], SILVA_LOCUS_NODUS))
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
    casus 6:  /* typus-int */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_TYPUS_PRIMITIVUS, 1);
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
    casus 7:  /* typus-nomen */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_TYPUS_NOMINATUS, 1);
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
    casus 8:  /* declarator-nomen */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_DECLARATOR_TITULUS, 1);
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
    casus 9:  /* declarator-stella */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_DECLARATOR_MONSTRATOR, 2);
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
    casus 10:  /* sententia-expr */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_SENTENTIA_EXPRESSIONIS, 2);
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
    casus 11:  /* expressio-plus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_BINARIUM, 3);
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
    casus 12:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 13:  /* terminus-star */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_BINARIUM, 3);
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
    casus 14:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 15:  /* factor-identificator */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_FOLIUM_IDENTIFICATOR, 1);
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
    casus 16:  /* factor-integer */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_FOLIUM_INTEGER, 1);
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
    casus 17:  /* factor-parenthesis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_IMPARILIS_GENUS_PARENTHESIS, 3);
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
    casus 18:  /* augmentata */
        redde valores[ZEPHYRUM];
    ordinarius:
        redde silva_valor_nihil();
    }
}

/* ==================================================
 * Fabrica ambigui (spec-v2 par 12)
 * ================================================== */

SilvaValor
silva_imparilis_ambiguum_fabricare (
    Piscina*   piscina,
    SilvaValor interpretationes,
    s32        canonica)
{
    SilvaNodus* nodus = silva_nodus_creare(piscina,
        (s32)SILVA_IMPARILIS_GENUS_AMBIGUUS, 2);
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
