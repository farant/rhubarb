/* glr_quaestio.c - GLR Table Query Tool
 *
 * Instrumentum pro quaerendo in tabulis GLR parser.
 *
 * USUS:
 *   glr_quaestio state <N>        - Ostendere actiones status N
 *   glr_quaestio token <NOMEN>    - Invenire status cum lexema
 *   glr_quaestio goto <NT>        - Invenire goto entries pro non-terminali
 *   glr_quaestio rule <P>         - Ostendere regulam P
 *   glr_quaestio stats            - Ostendere statisticas tabularum
 */

#include "latina.h"
#include "arbor2_glr.h"
#include "arbor2_lexema.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================================================
 * Internal NT Values (must match arbor2_glr_tabula.c)
 * ================================================== */

#define INT_NT_EXPR           0
#define INT_NT_TERM           1
#define INT_NT_FACTOR         2
#define INT_NT_DECLARATIO     3
#define INT_NT_DECLARATOR     4
#define INT_NT_DECLARATION    5
#define INT_NT_SENTENTIA      6
#define INT_NT_CORPUS         7
#define INT_NT_ELENCHUS       8
#define INT_NT_SI             9
#define INT_NT_DUM            10
#define INT_NT_FAC            11
#define INT_NT_PER            12
#define INT_NT_EXPRESSIO_OPT  13
#define INT_NT_PARAM_LIST     14
#define INT_NT_PARAM_DECL     15
#define INT_NT_DEFINITIO      16
#define INT_NT_STRUCT_SPEC    17
#define INT_NT_STRUCT_MEMBERS 18
#define INT_NT_ENUM_SPEC      19
#define INT_NT_ENUM_LIST      20
#define INT_NT_ENUMERATOR     21
#define INT_NT_AEQUALITAS     22
#define INT_NT_COMPARATIO     23
#define INT_NT_CONIUNCTIO     24
#define INT_NT_DISIUNCTIO     25

/* ==================================================
 * Name Tables
 * ================================================== */

nomen structura {
    constans character* titulus;
    s32 valor;
} TitulusValor;

hic_manens TitulusValor LEXEMA_NOMINA[] = {
    { "AUTO",           ARBOR2_LEXEMA_AUTO },
    { "BREAK",          ARBOR2_LEXEMA_BREAK },
    { "CASE",           ARBOR2_LEXEMA_CASE },
    { "CHAR",           ARBOR2_LEXEMA_CHAR },
    { "CONST",          ARBOR2_LEXEMA_CONST },
    { "CONTINUE",       ARBOR2_LEXEMA_CONTINUE },
    { "DEFAULT",        ARBOR2_LEXEMA_DEFAULT },
    { "DO",             ARBOR2_LEXEMA_DO },
    { "DOUBLE",         ARBOR2_LEXEMA_DOUBLE },
    { "ELSE",           ARBOR2_LEXEMA_ELSE },
    { "ENUM",           ARBOR2_LEXEMA_ENUM },
    { "EXTERN",         ARBOR2_LEXEMA_EXTERN },
    { "FLOAT",          ARBOR2_LEXEMA_FLOAT },
    { "FOR",            ARBOR2_LEXEMA_FOR },
    { "GOTO",           ARBOR2_LEXEMA_GOTO },
    { "IF",             ARBOR2_LEXEMA_IF },
    { "INT",            ARBOR2_LEXEMA_INT },
    { "LONG",           ARBOR2_LEXEMA_LONG },
    { "REGISTER",       ARBOR2_LEXEMA_REGISTER },
    { "RETURN",         ARBOR2_LEXEMA_RETURN },
    { "SHORT",          ARBOR2_LEXEMA_SHORT },
    { "SIGNED",         ARBOR2_LEXEMA_SIGNED },
    { "SIZEOF",         ARBOR2_LEXEMA_SIZEOF },
    { "STATIC",         ARBOR2_LEXEMA_STATIC },
    { "STRUCT",         ARBOR2_LEXEMA_STRUCT },
    { "SWITCH",         ARBOR2_LEXEMA_SWITCH },
    { "TYPEDEF",        ARBOR2_LEXEMA_TYPEDEF },
    { "UNION",          ARBOR2_LEXEMA_UNION },
    { "UNSIGNED",       ARBOR2_LEXEMA_UNSIGNED },
    { "VOID",           ARBOR2_LEXEMA_VOID },
    { "VOLATILE",       ARBOR2_LEXEMA_VOLATILE },
    { "WHILE",          ARBOR2_LEXEMA_WHILE },
    { "IDENTIFICATOR",  ARBOR2_LEXEMA_IDENTIFICATOR },
    { "INTEGER",        ARBOR2_LEXEMA_INTEGER },
    { "FLOAT_LIT",      ARBOR2_LEXEMA_FLOAT_LIT },
    { "CHAR_LIT",       ARBOR2_LEXEMA_CHAR_LIT },
    { "STRING_LIT",     ARBOR2_LEXEMA_STRING_LIT },
    { "PLUS",           ARBOR2_LEXEMA_PLUS },
    { "MINUS",          ARBOR2_LEXEMA_MINUS },
    { "ASTERISCUS",     ARBOR2_LEXEMA_ASTERISCUS },
    { "SOLIDUS",        ARBOR2_LEXEMA_SOLIDUS },
    { "PERCENTUM",      ARBOR2_LEXEMA_PERCENTUM },
    { "AMPERSAND",      ARBOR2_LEXEMA_AMPERSAND },
    { "PIPA",           ARBOR2_LEXEMA_PIPA },
    { "CARET",          ARBOR2_LEXEMA_CARET },
    { "TILDE",          ARBOR2_LEXEMA_TILDE },
    { "EXCLAMATIO",     ARBOR2_LEXEMA_EXCLAMATIO },
    { "DUAMPERSAND",    ARBOR2_LEXEMA_DUAMPERSAND },
    { "DUPIPA",         ARBOR2_LEXEMA_DUPIPA },
    { "AEQUALIS",       ARBOR2_LEXEMA_AEQUALIS },
    { "NON_AEQUALIS",   ARBOR2_LEXEMA_NON_AEQUALIS },
    { "MINOR",          ARBOR2_LEXEMA_MINOR },
    { "MAIOR",          ARBOR2_LEXEMA_MAIOR },
    { "MINOR_AEQ",      ARBOR2_LEXEMA_MINOR_AEQ },
    { "MAIOR_AEQ",      ARBOR2_LEXEMA_MAIOR_AEQ },
    { "SINISTRUM",      ARBOR2_LEXEMA_SINISTRUM },
    { "DEXTRUM",        ARBOR2_LEXEMA_DEXTRUM },
    { "ASSIGNATIO",     ARBOR2_LEXEMA_ASSIGNATIO },
    { "PLUS_ASSIGN",    ARBOR2_LEXEMA_PLUS_ASSIGN },
    { "MINUS_ASSIGN",   ARBOR2_LEXEMA_MINUS_ASSIGN },
    { "MULT_ASSIGN",    ARBOR2_LEXEMA_MULT_ASSIGN },
    { "DIV_ASSIGN",     ARBOR2_LEXEMA_DIV_ASSIGN },
    { "MOD_ASSIGN",     ARBOR2_LEXEMA_MOD_ASSIGN },
    { "AND_ASSIGN",     ARBOR2_LEXEMA_AND_ASSIGN },
    { "OR_ASSIGN",      ARBOR2_LEXEMA_OR_ASSIGN },
    { "XOR_ASSIGN",     ARBOR2_LEXEMA_XOR_ASSIGN },
    { "SHL_ASSIGN",     ARBOR2_LEXEMA_SHL_ASSIGN },
    { "SHR_ASSIGN",     ARBOR2_LEXEMA_SHR_ASSIGN },
    { "DUPLUS",         ARBOR2_LEXEMA_DUPLUS },
    { "DUMINUS",        ARBOR2_LEXEMA_DUMINUS },
    { "PUNCTUM",        ARBOR2_LEXEMA_PUNCTUM },
    { "SAGITTA",        ARBOR2_LEXEMA_SAGITTA },
    { "COMMA",          ARBOR2_LEXEMA_COMMA },
    { "COLON",          ARBOR2_LEXEMA_COLON },
    { "SEMICOLON",      ARBOR2_LEXEMA_SEMICOLON },
    { "QUAESTIO",       ARBOR2_LEXEMA_QUAESTIO },
    { "PAREN_APERTA",   ARBOR2_LEXEMA_PAREN_APERTA },
    { "PAREN_CLAUSA",   ARBOR2_LEXEMA_PAREN_CLAUSA },
    { "BRACKET_APERTA", ARBOR2_LEXEMA_BRACKET_APERTA },
    { "BRACKET_CLAUSA", ARBOR2_LEXEMA_BRACKET_CLAUSA },
    { "BRACE_APERTA",   ARBOR2_LEXEMA_BRACE_APERTA },
    { "BRACE_CLAUSA",   ARBOR2_LEXEMA_BRACE_CLAUSA },
    { "HASH",           ARBOR2_LEXEMA_HASH },
    { "HASH_HASH",      ARBOR2_LEXEMA_HASH_HASH },
    { "ELLIPSIS",       ARBOR2_LEXEMA_ELLIPSIS },
    { "NOVA_LINEA",     ARBOR2_LEXEMA_NOVA_LINEA },
    { "EOF",            ARBOR2_LEXEMA_EOF },
    { "ERROR",          ARBOR2_LEXEMA_ERROR },
    { NIHIL, -1 }
};

hic_manens TitulusValor NT_NOMINA[] = {
    { "EXPR",           INT_NT_EXPR },
    { "EXPRESSIO",      INT_NT_EXPR },
    { "TERM",           INT_NT_TERM },
    { "TERMINUS",       INT_NT_TERM },
    { "FACTOR",         INT_NT_FACTOR },
    { "DECLARATIO",     INT_NT_DECLARATIO },
    { "DECLARATOR",     INT_NT_DECLARATOR },
    { "DECLARATION",    INT_NT_DECLARATION },
    { "SENTENTIA",      INT_NT_SENTENTIA },
    { "CORPUS",         INT_NT_CORPUS },
    { "ELENCHUS",       INT_NT_ELENCHUS },
    { "SI",             INT_NT_SI },
    { "DUM",            INT_NT_DUM },
    { "FAC",            INT_NT_FAC },
    { "PER",            INT_NT_PER },
    { "EXPRESSIO_OPT",  INT_NT_EXPRESSIO_OPT },
    { "PARAM_LIST",     INT_NT_PARAM_LIST },
    { "PARAM_DECL",     INT_NT_PARAM_DECL },
    { "DEFINITIO",      INT_NT_DEFINITIO },
    { "STRUCT_SPEC",    INT_NT_STRUCT_SPEC },
    { "STRUCT_MEMBERS", INT_NT_STRUCT_MEMBERS },
    { "ENUM_SPEC",      INT_NT_ENUM_SPEC },
    { "ENUM_LIST",      INT_NT_ENUM_LIST },
    { "ENUMERATOR",     INT_NT_ENUMERATOR },
    { "AEQUALITAS",     INT_NT_AEQUALITAS },
    { "COMPARATIO",     INT_NT_COMPARATIO },
    { "CONIUNCTIO",     INT_NT_CONIUNCTIO },
    { "DISIUNCTIO",     INT_NT_DISIUNCTIO },
    { NIHIL, -1 }
};


/* ==================================================
 * Parsing
 * ================================================== */

interior s32
parsere_lexema_titulus(constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; LEXEMA_NOMINA[i].titulus != NIHIL; i++)
    {
        si (strcmp(LEXEMA_NOMINA[i].titulus, titulus) == ZEPHYRUM)
        {
            redde LEXEMA_NOMINA[i].valor;
        }
    }

    redde -I;
}

interior s32
parsere_nt_titulus(constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; NT_NOMINA[i].titulus != NIHIL; i++)
    {
        si (strcmp(NT_NOMINA[i].titulus, titulus) == ZEPHYRUM)
        {
            redde NT_NOMINA[i].valor;
        }
    }

    redde -I;
}


/* ==================================================
 * Callbacks
 * ================================================== */

interior vacuum
cb_imprimere_actio(constans Arbor2TabulaActio* actio, vacuum* ctx)
{
    (vacuum)ctx;

    printf("  %-16s -> %-7s %3d\n",
        arbor2_lexema_genus_nomen(actio->lexema),
        arbor2_actio_genus_nomen(actio->actio),
        actio->valor);
}

nomen structura {
    i32 numerus;
} ContextusNumerans;

interior vacuum
cb_imprimere_status_actio(i32 status, constans Arbor2TabulaActio* actio, vacuum* ctx)
{
    ContextusNumerans* numerans;

    numerans = (ContextusNumerans*)ctx;
    numerans->numerus++;

    printf("  State %3d: %-16s -> %-7s %3d\n",
        status,
        arbor2_lexema_genus_nomen(actio->lexema),
        arbor2_actio_genus_nomen(actio->actio),
        actio->valor);
}

interior vacuum
cb_imprimere_goto(constans Arbor2TabulaGoto* entry, vacuum* ctx)
{
    ContextusNumerans* numerans;

    numerans = (ContextusNumerans*)ctx;
    numerans->numerus++;

    printf("  State %3d -> State %3d\n",
        entry->status,
        entry->status_novus);
}

/* ==================================================
 * Commands
 * ================================================== */

interior integer
cmd_state(constans character* arg)
{
    i32 status;
    i32 num_states;
    character* endptr;

    status = (i32)strtol(arg, &endptr, 10);
    si (*endptr != '\0')
    {
        fprintf(stderr, "Error: '%s' non est numerus validus\n", arg);
        redde I;
    }

    num_states = arbor2_tabula_numerus_statuum();
    si (status < ZEPHYRUM || status >= num_states)
    {
        fprintf(stderr, "Error: Status %d extra limites [0, %d)\n",
            status, num_states);
        redde I;
    }

    printf("State %d:\n", status);
    arbor2_tabula_iterare_actiones_status(status, cb_imprimere_actio, NIHIL);

    redde ZEPHYRUM;
}

interior integer
cmd_token(constans character* arg)
{
    s32 lexema;
    ContextusNumerans numerans;

    lexema = parsere_lexema_titulus(arg);
    si (lexema < (s32)ZEPHYRUM)
    {
        fprintf(stderr, "Error: Lexema '%s' ignotum\n", arg);
        fprintf(stderr, "Exempla: DUAMPERSAND, DUPIPA, SEMICOLON, EOF\n");
        redde I;
    }

    numerans.numerus = ZEPHYRUM;

    printf("Token %s:\n", arg);
    arbor2_tabula_iterare_actiones_lexema(
        (Arbor2LexemaGenus)lexema,
        cb_imprimere_status_actio,
        &numerans);

    printf("  (%d occurrences)\n", numerans.numerus);

    redde ZEPHYRUM;
}

interior integer
cmd_goto(constans character* arg)
{
    s32 nt;
    ContextusNumerans numerans;

    nt = parsere_nt_titulus(arg);
    si (nt < (s32)ZEPHYRUM)
    {
        fprintf(stderr, "Error: Non-terminalis '%s' ignotum\n", arg);
        fprintf(stderr, "Exempla: CONIUNCTIO, DISIUNCTIO, AEQUALITAS, EXPR\n");
        redde I;
    }

    numerans.numerus = ZEPHYRUM;

    printf("GOTO for %s:\n", arg);
    arbor2_tabula_iterare_goto_nt(nt, cb_imprimere_goto, &numerans);

    printf("  (%d entries)\n", numerans.numerus);

    redde ZEPHYRUM;
}

interior integer
cmd_rule(constans character* arg)
{
    i32 index;
    i32 num_regulae;
    character* endptr;
    Arbor2Regula* regula;

    index = (i32)strtol(arg, &endptr, 10);
    si (*endptr != '\0')
    {
        fprintf(stderr, "Error: '%s' non est numerus validus\n", arg);
        redde I;
    }

    num_regulae = arbor2_tabula_numerus_regularum();
    si (index < ZEPHYRUM || index >= num_regulae)
    {
        fprintf(stderr, "Error: Regula %d extra limites [0, %d)\n",
            index, num_regulae);
        redde I;
    }

    regula = arbor2_tabula_obtinere_regula(index);
    si (regula == NIHIL)
    {
        fprintf(stderr, "Error: Non potest obtinere regulam %d\n", index);
        redde I;
    }

    printf("Rule P%d:\n", index);
    printf("  LHS: %s\n", arbor2_nt_nomen(regula->sinister));
    printf("  RHS length: %d symbol(s)\n", regula->longitudo);
    printf("  Node type: %s\n", arbor2_nodus_genus_nomen(regula->nodus_genus));

    redde ZEPHYRUM;
}

interior integer
cmd_stats(vacuum)
{
    printf("GLR Table Statistics:\n");
    printf("  States:   %d\n", arbor2_tabula_numerus_statuum());
    printf("  Rules:    %d\n", arbor2_tabula_numerus_regularum());
    printf("  Actions:  %d\n", arbor2_tabula_numerus_actionum());
    printf("  GOTOs:    %d\n", arbor2_tabula_numerus_goto());

    redde ZEPHYRUM;
}

interior vacuum
imprimere_auxilium(vacuum)
{
    printf("glr_quaestio - GLR Table Query Tool\n\n");
    printf("Usus:\n");
    printf("  glr_quaestio state <N>      Show actions for state N\n");
    printf("  glr_quaestio token <NAME>   Find states with token\n");
    printf("  glr_quaestio goto <NT>      Find goto entries for non-terminal\n");
    printf("  glr_quaestio rule <P>       Show rule P details\n");
    printf("  glr_quaestio stats          Show table statistics\n");
    printf("  glr_quaestio --help         Show this help\n\n");
    printf("Token examples: DUAMPERSAND, DUPIPA, SEMICOLON, EOF\n");
    printf("NT examples: CONIUNCTIO, DISIUNCTIO, AEQUALITAS, EXPR\n");
}

/* ==================================================
 * Main
 * ================================================== */

integer principale(integer argc, constans character* constans* argv)
{
    si (argc < II)
    {
        imprimere_auxilium();
        redde I;
    }

    si (strcmp(argv[I], "--help") == ZEPHYRUM ||
        strcmp(argv[I], "-h") == ZEPHYRUM)
    {
        imprimere_auxilium();
        redde ZEPHYRUM;
    }

    si (strcmp(argv[I], "stats") == ZEPHYRUM)
    {
        redde cmd_stats();
    }

    si (argc < III)
    {
        fprintf(stderr, "Error: Argumentum deest\n");
        fprintf(stderr, "Usa: glr_quaestio %s <value>\n", argv[I]);
        redde I;
    }

    si (strcmp(argv[I], "state") == ZEPHYRUM)
    {
        redde cmd_state(argv[II]);
    }

    si (strcmp(argv[I], "token") == ZEPHYRUM)
    {
        redde cmd_token(argv[II]);
    }

    si (strcmp(argv[I], "goto") == ZEPHYRUM)
    {
        redde cmd_goto(argv[II]);
    }

    si (strcmp(argv[I], "rule") == ZEPHYRUM)
    {
        redde cmd_rule(argv[II]);
    }

    fprintf(stderr, "Error: Mandatum '%s' ignotum\n", argv[I]);
    imprimere_auxilium();
    redde I;
}
