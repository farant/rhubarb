/* silva_tabulae_sceleti.c - GENERATUM per silva_coquere ex grammatica/sceletum.stml
 * NOLI MANU MUTARE - regenera per silva/generare.sh.
 *
 * status: 27, productiones: 18, symbola: 19,
 * conflictus servati (GLR): 1
 */

#include "silva_tabulae_sceleti.h"

/* ==================================================
 * Symbola
 * ================================================== */

hic_manens constans SilvaTabSymbolum SILVA_SCELETUM_SYMBOLA[19] = {
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
    /* [11] */ { "declaratio", SILVA_LEX_EOF, FALSUM },
    /* [12] */ { "typus", SILVA_LEX_EOF, FALSUM },
    /* [13] */ { "declarator", SILVA_LEX_EOF, FALSUM },
    /* [14] */ { "sententia-expressionis", SILVA_LEX_EOF, FALSUM },
    /* [15] */ { "expressio", SILVA_LEX_EOF, FALSUM },
    /* [16] */ { "terminus", SILVA_LEX_EOF, FALSUM },
    /* [17] */ { "factor", SILVA_LEX_EOF, FALSUM },
    /* [18] */ { "__initium__", SILVA_LEX_EOF, FALSUM }
};

/* ==================================================
 * Productiones
 * ================================================== */

hic_manens constans SilvaTabProductio SILVA_SCELETUM_PRODUCTIONES[18] = {
    /* P0  sententiae (0 symbola) */ { 9, 0, NIHIL },
    /* P1  sententiae (2 symbola) */ { 9, 2, "sententiae-appendere" },
    /* P2  sententia (1 symbola) */ { 10, 1, NIHIL },
    /* P3  sententia (1 symbola) */ { 10, 1, NIHIL },
    /* P4  declaratio (3 symbola) */ { 11, 3, "declaratio-simplex" },
    /* P5  typus (1 symbola) */ { 12, 1, "typus-int" },
    /* P6  typus (1 symbola) */ { 12, 1, "typus-nomen" },
    /* P7  declarator (1 symbola) */ { 13, 1, "declarator-nomen" },
    /* P8  declarator (2 symbola) */ { 13, 2, "declarator-stella" },
    /* P9  sententia-expressionis (2 symbola) */ { 14, 2, "sententia-expr" },
    /* P10 expressio (3 symbola) */ { 15, 3, "expressio-plus" },
    /* P11 expressio (1 symbola) */ { 15, 1, NIHIL },
    /* P12 terminus (3 symbola) */ { 16, 3, "terminus-star" },
    /* P13 terminus (1 symbola) */ { 16, 1, NIHIL },
    /* P14 factor (1 symbola) */ { 17, 1, "factor-identificator" },
    /* P15 factor (1 symbola) */ { 17, 1, "factor-integer" },
    /* P16 factor (3 symbola) */ { 17, 3, "factor-parenthesis" },
    /* P17 __initium__ (2 symbola) */ { 18, 2, NIHIL }
};

/* ==================================================
 * Actiones (laminae per statum in SILVA_SCELETUM_STATUS)
 * ================================================== */

hic_manens constans SilvaTabActio SILVA_SCELETUM_ACTIONES[] = {
    /* --- status 0 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* IDENTIFICATOR: reducere P0 */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INTEGER: reducere P0 */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* INT: reducere P0 */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* PAREN_APERTA: reducere P0 */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   0, 0 },  /* EOF: reducere P0 */
    /* --- status 1 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,   7, 0 },  /* IDENTIFICATOR: transponere -> 7 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* INTEGER: transponere -> 11 */
    {   2, SILVA_TAB_ACTIO_TRANSPONERE,   6, 0 },  /* INT: transponere -> 6 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* PAREN_APERTA: transponere -> 12 */
    {   8, SILVA_TAB_ACTIO_TRANSPONERE,  13, 0 },  /* EOF: transponere -> 13 */
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
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* IDENTIFICATOR: transponere -> 15 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  16, 0 },  /* STAR: transponere -> 16 */
    /* --- status 6 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* IDENTIFICATOR: reducere P5 typus-int */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   5, 0 },  /* STAR: reducere P5 typus-int */
    /* --- status 7 [CONFLICTUS SERVATUS] --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* IDENTIFICATOR: reducere P6 typus-nomen */
    {   3, SILVA_TAB_ACTIO_REDUCERE,   6, 0 },  /* STAR: reducere P6 typus-nomen */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* STAR: reducere P14 factor-identificator */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PLUS: reducere P14 factor-identificator */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* SEMICOLON: reducere P14 factor-identificator */
    /* --- status 8 --- */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  18, 0 },  /* PLUS: transponere -> 18 */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  17, 0 },  /* SEMICOLON: transponere -> 17 */
    /* --- status 9 --- */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* STAR: transponere -> 19 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* PLUS: reducere P11 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* SEMICOLON: reducere P11 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  11, 0 },  /* PAREN_CLAUSA: reducere P11 */
    /* --- status 10 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* STAR: reducere P13 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* PLUS: reducere P13 */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* SEMICOLON: reducere P13 */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  13, 0 },  /* PAREN_CLAUSA: reducere P13 */
    /* --- status 11 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* STAR: reducere P15 factor-integer */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PLUS: reducere P15 factor-integer */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* SEMICOLON: reducere P15 factor-integer */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  15, 0 },  /* PAREN_CLAUSA: reducere P15 factor-integer */
    /* --- status 12 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* IDENTIFICATOR: transponere -> 21 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* INTEGER: transponere -> 11 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* PAREN_APERTA: transponere -> 12 */
    /* --- status 13 --- */
    {  -1, SILVA_TAB_ACTIO_ACCIPERE,   0, 0 },  /* $: accipere */
    /* --- status 14 --- */
    {   5, SILVA_TAB_ACTIO_TRANSPONERE,  22, 0 },  /* SEMICOLON: transponere -> 22 */
    /* --- status 15 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   7, 0 },  /* SEMICOLON: reducere P7 declarator-nomen */
    /* --- status 16 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  15, 0 },  /* IDENTIFICATOR: transponere -> 15 */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  16, 0 },  /* STAR: transponere -> 16 */
    /* --- status 17 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* IDENTIFICATOR: reducere P9 sententia-expr */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* INTEGER: reducere P9 sententia-expr */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* INT: reducere P9 sententia-expr */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* PAREN_APERTA: reducere P9 sententia-expr */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   9, 0 },  /* EOF: reducere P9 sententia-expr */
    /* --- status 18 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* IDENTIFICATOR: transponere -> 21 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* INTEGER: transponere -> 11 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* PAREN_APERTA: transponere -> 12 */
    /* --- status 19 --- */
    {   0, SILVA_TAB_ACTIO_TRANSPONERE,  21, 0 },  /* IDENTIFICATOR: transponere -> 21 */
    {   1, SILVA_TAB_ACTIO_TRANSPONERE,  11, 0 },  /* INTEGER: transponere -> 11 */
    {   6, SILVA_TAB_ACTIO_TRANSPONERE,  12, 0 },  /* PAREN_APERTA: transponere -> 12 */
    /* --- status 20 --- */
    {   4, SILVA_TAB_ACTIO_TRANSPONERE,  18, 0 },  /* PLUS: transponere -> 18 */
    {   7, SILVA_TAB_ACTIO_TRANSPONERE,  26, 0 },  /* PAREN_CLAUSA: transponere -> 26 */
    /* --- status 21 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* STAR: reducere P14 factor-identificator */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PLUS: reducere P14 factor-identificator */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* SEMICOLON: reducere P14 factor-identificator */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  14, 0 },  /* PAREN_CLAUSA: reducere P14 factor-identificator */
    /* --- status 22 --- */
    {   0, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* IDENTIFICATOR: reducere P4 declaratio-simplex */
    {   1, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* INTEGER: reducere P4 declaratio-simplex */
    {   2, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* INT: reducere P4 declaratio-simplex */
    {   6, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* PAREN_APERTA: reducere P4 declaratio-simplex */
    {   8, SILVA_TAB_ACTIO_REDUCERE,   4, 0 },  /* EOF: reducere P4 declaratio-simplex */
    /* --- status 23 --- */
    {   5, SILVA_TAB_ACTIO_REDUCERE,   8, 0 },  /* SEMICOLON: reducere P8 declarator-stella */
    /* --- status 24 --- */
    {   3, SILVA_TAB_ACTIO_TRANSPONERE,  19, 0 },  /* STAR: transponere -> 19 */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* PLUS: reducere P10 expressio-plus */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* SEMICOLON: reducere P10 expressio-plus */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  10, 0 },  /* PAREN_CLAUSA: reducere P10 expressio-plus */
    /* --- status 25 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* STAR: reducere P12 terminus-star */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* PLUS: reducere P12 terminus-star */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* SEMICOLON: reducere P12 terminus-star */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  12, 0 },  /* PAREN_CLAUSA: reducere P12 terminus-star */
    /* --- status 26 --- */
    {   3, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* STAR: reducere P16 factor-parenthesis */
    {   4, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* PLUS: reducere P16 factor-parenthesis */
    {   5, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* SEMICOLON: reducere P16 factor-parenthesis */
    {   7, SILVA_TAB_ACTIO_REDUCERE,  16, 0 },  /* PAREN_CLAUSA: reducere P16 factor-parenthesis */
};

/* ==================================================
 * Goto
 * ================================================== */

hic_manens constans SilvaTabGoto SILVA_SCELETUM_GOTO[] = {
    /* --- status 0 --- */
    {   9,   1 },  /* sententiae -> 1 */
    /* --- status 1 --- */
    {  10,   2 },  /* sententia -> 2 */
    {  11,   3 },  /* declaratio -> 3 */
    {  12,   5 },  /* typus -> 5 */
    {  14,   4 },  /* sententia-expressionis -> 4 */
    {  15,   8 },  /* expressio -> 8 */
    {  16,   9 },  /* terminus -> 9 */
    {  17,  10 },  /* factor -> 10 */
    /* --- status 5 --- */
    {  13,  14 },  /* declarator -> 14 */
    /* --- status 12 --- */
    {  15,  20 },  /* expressio -> 20 */
    {  16,   9 },  /* terminus -> 9 */
    {  17,  10 },  /* factor -> 10 */
    /* --- status 16 --- */
    {  13,  23 },  /* declarator -> 23 */
    /* --- status 18 --- */
    {  16,  24 },  /* terminus -> 24 */
    {  17,  10 },  /* factor -> 10 */
    /* --- status 19 --- */
    {  17,  25 },  /* factor -> 25 */
    { -1, -1 }   /* terminator */
};

/* ==================================================
 * Status (offset + numerus in series planas)
 * ================================================== */

hic_manens constans SilvaTabStatus SILVA_SCELETUM_STATUS[27] = {
    /* status  0 */ {    0,  5,    0,  1 },
    /* status  1 */ {    5,  5,    1,  7 },
    /* status  2 */ {   10,  5,    8,  0 },
    /* status  3 */ {   15,  5,    8,  0 },
    /* status  4 */ {   20,  5,    8,  0 },
    /* status  5 */ {   25,  2,    8,  1 },
    /* status  6 */ {   27,  2,    9,  0 },
    /* status  7 */ {   29,  5,    9,  0 },
    /* status  8 */ {   34,  2,    9,  0 },
    /* status  9 */ {   36,  4,    9,  0 },
    /* status 10 */ {   40,  4,    9,  0 },
    /* status 11 */ {   44,  4,    9,  0 },
    /* status 12 */ {   48,  3,    9,  3 },
    /* status 13 */ {   51,  1,   12,  0 },
    /* status 14 */ {   52,  1,   12,  0 },
    /* status 15 */ {   53,  1,   12,  0 },
    /* status 16 */ {   54,  2,   12,  1 },
    /* status 17 */ {   56,  5,   13,  0 },
    /* status 18 */ {   61,  3,   13,  2 },
    /* status 19 */ {   64,  3,   15,  1 },
    /* status 20 */ {   67,  2,   16,  0 },
    /* status 21 */ {   69,  4,   16,  0 },
    /* status 22 */ {   73,  5,   16,  0 },
    /* status 23 */ {   78,  1,   16,  0 },
    /* status 24 */ {   79,  4,   16,  0 },
    /* status 25 */ {   83,  4,   16,  0 },
    /* status 26 */ {   87,  4,   16,  0 }
};

/* ==================================================
 * Tabula cocta integra
 * ================================================== */

constans SilvaTabulaCocta SILVA_SCELETUM_TABULA = {
    SILVA_SCELETUM_SYMBOLA, 19,
    9,  /* terminalia */
    SILVA_SCELETUM_PRODUCTIONES, 18,
    SILVA_SCELETUM_STATUS, 27,
    SILVA_SCELETUM_ACTIONES, 91,
    SILVA_SCELETUM_GOTO, 16,
    18,  /* initium (augmentatum) */
    17,  /* productio augmentata */
    1   /* conflictus servati */
};

/* ==================================================
 * Registrum generum (layouts locorum)
 * ================================================== */

hic_manens constans SilvaTabLocus SILVA_SCELETUM_LOCI[] = {
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

hic_manens constans SilvaTabGenus SILVA_SCELETUM_GENERA[] = {
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
    /* [12] */ { "conditionalis", 21, 2 },
    /* [13] */ { "ramus-sumptus", 23, 3 },
    /* [14] */ { "ramus-omissus", 26, 3 },
    { NIHIL, 0, 0 }   /* terminator */
};

constans SilvaRegistrumCoctum SILVA_SCELETUM_REGISTRUM = {
    SILVA_SCELETUM_GENERA, 15,
    SILVA_SCELETUM_LOCI, 29
};

/* ==================================================
 * Accessores (probati: genus + fines)
 * ================================================== */

SilvaValor
silva_sceletum_declaratio_typus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATIO
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_declaratio_declarator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATIO
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_declaratio_tok_terminator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATIO
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_sceletum_typus_primitivus_tok_verbum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_TYPUS_PRIMITIVUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_typus_nominatus_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_declarator_titulus_tok_titulus (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATOR_TITULUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_declarator_monstrator_tok_stella (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_declarator_monstrator_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_sententia_expressionis_expressio (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_sententia_expressionis_tok_terminator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_binarium_sinister (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_BINARIUM
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_binarium_tok_operator (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_BINARIUM
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_binarium_dexter (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_BINARIUM
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_sceletum_folium_identificator_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_FOLIUM_IDENTIFICATOR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_folium_integer_tok_valor (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_FOLIUM_INTEGER
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_parenthesis_tok_apertum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_PARENTHESIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_parenthesis_internum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_PARENTHESIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_parenthesis_tok_clausum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_PARENTHESIS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_sceletum_error_tokens (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_ERROR
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_ambiguus_interpretationes (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_AMBIGUUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_ambiguus_canonica (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_AMBIGUUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_conditionalis_rami (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_CONDITIONALIS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_conditionalis_finis (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_CONDITIONALIS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_ramus_sumptus_directiva (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_SUMPTUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_ramus_sumptus_contentum (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_SUMPTUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_ramus_sumptus_conditio_id (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_SUMPTUS
        || 2 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[2];
}

SilvaValor
silva_sceletum_ramus_omissus_directiva (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_OMISSUS
        || 0 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[0];
}

SilvaValor
silva_sceletum_ramus_omissus_cruda (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_OMISSUS
        || 1 >= nodus->numerus_locorum)
    {
        redde silva_valor_nihil();
    }
    redde nodus->loci[1];
}

SilvaValor
silva_sceletum_ramus_omissus_conditio_id (constans SilvaNodus* nodus)
{
    si (nodus == NIHIL || nodus->genus != (s32)SILVA_SCELETUM_GENUS_RAMUS_OMISSUS
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
silva_sceletum_construere (
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
    casus 4:  /* declaratio-simplex */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_DECLARATIO, 3);
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
    casus 5:  /* typus-int */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_TYPUS_PRIMITIVUS, 1);
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
    casus 6:  /* typus-nomen */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_TYPUS_NOMINATUS, 1);
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
    casus 7:  /* declarator-nomen */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_DECLARATOR_TITULUS, 1);
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
    casus 8:  /* declarator-stella */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_DECLARATOR_MONSTRATOR, 2);
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
    casus 9:  /* sententia-expr */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_SENTENTIA_EXPRESSIONIS, 2);
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
    casus 10:  /* expressio-plus */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM, 3);
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
    casus 11:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 12:  /* terminus-star */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_BINARIUM, 3);
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
    casus 13:  /* transitus */
        redde valores[ZEPHYRUM];
    casus 14:  /* factor-identificator */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_FOLIUM_IDENTIFICATOR, 1);
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
    casus 15:  /* factor-integer */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_FOLIUM_INTEGER, 1);
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
    casus 16:  /* factor-parenthesis */
    {
        SilvaNodus* nodus = silva_nodus_creare(piscina,
            (s32)SILVA_SCELETUM_GENUS_PARENTHESIS, 3);
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
    casus 17:  /* augmentata */
        redde valores[ZEPHYRUM];
    ordinarius:
        redde silva_valor_nihil();
    }
}

/* ==================================================
 * Fabrica ambigui (spec-v2 par 12)
 * ================================================== */

SilvaValor
silva_sceletum_ambiguum_fabricare (
    Piscina*   piscina,
    SilvaValor interpretationes,
    s32        canonica)
{
    SilvaNodus* nodus = silva_nodus_creare(piscina,
        (s32)SILVA_SCELETUM_GENUS_AMBIGUUS, 2);
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
