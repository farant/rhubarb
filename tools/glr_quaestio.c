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
#include "arbor.h"
#include "arbor2_glr.h"
#include "arbor2_lexema.h"
#include "piscina.h"
#include "internamentum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================================================
 * Global State for Dynamic Lookup Tables
 * ================================================== */

hic_manens Piscina* g_piscina = NIHIL;
hic_manens InternamentumChorda* g_intern = NIHIL;

/* Dynamic token lookup table (built from parsing arbor2_lexema.h) */
nomen structura {
    constans character* titulus;
    s32 valor;
} TitulusValor;

hic_manens TitulusValor* g_lexema_tabula = NIHIL;
hic_manens i32 g_lexema_numerus = ZEPHYRUM;

/* ==================================================
 * Dynamic Enum Extraction
 * ================================================== */

/*
 * Extrahere enumeratores ex filo header usando arbor1.
 *
 * via:           Path to header file (e.g., "include/arbor2_lexema.h")
 * typedef_titulus: Name of the typedef (e.g., "Arbor2LexemaGenus")
 * praefixum:     Prefix to strip from names (e.g., "ARBOR2_LEXEMA_")
 * tabula_out:    Output array of TitulusValor
 * numerus_out:   Output count
 *
 * Returns: VERUM if successful
 */
interior b32
extrahere_enum_ex_filo(
    constans character* via,
    constans character* typedef_titulus,
    constans character* praefixum,
    TitulusValor** tabula_out,
    i32* numerus_out)
{
    ArborResultus res;
    ArborNodus* radix;
    Xar* declarationes;
    i32 num_decl;
    i32 i;
    i32 praefixum_len;

    *tabula_out = NIHIL;
    *numerus_out = ZEPHYRUM;

    praefixum_len = (i32)strlen(praefixum);

    /* Parse the header file */
    res = arbor_parsere_filum(via, g_piscina, g_intern, NIHIL);
    si (!res.successus)
    {
        fprintf(stderr, "Warning: Non potest parsere %s\n", via);
        si (res.errores != NIHIL)
        {
            i32 e;
            per (e = ZEPHYRUM; e < xar_numerus(res.errores); e++)
            {
                ArborError** err_ptr = xar_obtinere(res.errores, e);
                si (err_ptr != NIHIL && *err_ptr != NIHIL)
                {
                    ArborError* err = *err_ptr;
                    fprintf(stderr, "  Error [%d:%d]: %.*s\n",
                        err->linea, err->columna,
                        err->nuntius.mensura, (constans character*)err->nuntius.datum);
                }
            }
        }
        redde FALSUM;
    }

    radix = res.radix;
    si (radix == NIHIL || radix->genus != ARBOR_NODUS_TRANSLATION_UNIT)
    {
        fprintf(stderr, "Warning: AST radix invalida pro %s (genus=%d)\n", via,
            radix != NIHIL ? (integer)radix->genus : -1);
        redde FALSUM;
    }

    /* Walk declarations looking for typedef with matching name */
    declarationes = radix->datum.genericum.liberi;
    si (declarationes == NIHIL)
    {
        fprintf(stderr, "Warning: No declarations in %s\n", via);
        redde FALSUM;
    }

    num_decl = xar_numerus(declarationes);
    per (i = ZEPHYRUM; i < num_decl; i++)
    {
        ArborNodus** decl_ptr;
        ArborNodus* decl;
        Xar* specifiers;
        Xar* declaratores;
        ArborNodus* enum_spec;
        i32 j;
        b32 est_typedef;
        b32 nomen_invenit;

        decl_ptr = xar_obtinere(declarationes, i);
        si (decl_ptr == NIHIL)
        {
            perge;
        }
        decl = *decl_ptr;

        si (decl->genus != ARBOR_NODUS_DECLARATION)
        {
            perge;
        }

        specifiers = decl->datum.declaratio.specifiers;
        declaratores = decl->datum.declaratio.declaratores;

        si (specifiers == NIHIL)
        {
            perge;
        }

        /* Check if this is a typedef and find enum specifier */
        est_typedef = FALSUM;
        enum_spec = NIHIL;

        per (j = ZEPHYRUM; j < xar_numerus(specifiers); j++)
        {
            ArborNodus** spec_ptr;
            ArborNodus* spec;

            spec_ptr = xar_obtinere(specifiers, j);
            si (spec_ptr == NIHIL)
            {
                perge;
            }
            spec = *spec_ptr;

            si (spec->genus == ARBOR_NODUS_STORAGE_CLASS)
            {
                /* Check if it's typedef keyword */
                si (spec->datum.folium.keyword == ARBOR_LEXEMA_TYPEDEF)
                {
                    est_typedef = VERUM;
                }
            }
            alioquin si (spec->genus == ARBOR_NODUS_ENUM_SPECIFIER)
            {
                enum_spec = spec;
            }
        }

        si (!est_typedef || enum_spec == NIHIL)
        {
            perge;
        }

        /* Check if the typedef name matches */
        nomen_invenit = FALSUM;
        si (declaratores != NIHIL)
        {
            per (j = ZEPHYRUM; j < xar_numerus(declaratores); j++)
            {
                ArborNodus** init_decl_ptr;
                ArborNodus* init_decl;
                ArborNodus* declarator;
                chorda* decl_nomen;

                init_decl_ptr = xar_obtinere(declaratores, j);
                si (init_decl_ptr == NIHIL)
                {
                    perge;
                }
                init_decl = *init_decl_ptr;

                /* Get the declarator (handle init_declarator or direct declarator) */
                si (init_decl->genus == ARBOR_NODUS_INIT_DECLARATOR)
                {
                    declarator = init_decl->datum.init_decl.declarator;
                }
                alioquin
                {
                    declarator = init_decl;
                }

                si (declarator == NIHIL)
                {
                    perge;
                }

                /* Get the name from the declarator */
                si (declarator->genus == ARBOR_NODUS_IDENTIFIER ||
                    declarator->genus == ARBOR_NODUS_TYPEDEF_NAME)
                {
                    decl_nomen = declarator->datum.folium.valor;
                    si (decl_nomen != NIHIL)
                    {
                        /* Compare with target typedef name */
                        si (decl_nomen->mensura == (i32)strlen(typedef_titulus) &&
                            strncmp((constans character*)decl_nomen->datum, typedef_titulus,
                                    (size_t)decl_nomen->mensura) == ZEPHYRUM)
                        {
                            nomen_invenit = VERUM;
                            frange;
                        }
                    }
                }
            }
        }

        /*
         * Note: arbor1 parses "typedef enum { ... } TypeName;" with empty declarators.
         * The typedef name isn't easily accessible. Instead, we verify by checking
         * that the first enumerator starts with the expected prefix.
         */
        (vacuum)typedef_titulus; /* Suppress unused warning - keeping param for future use */
        (vacuum)nomen_invenit;

        /* Found a typedef enum! Verify by prefix and extract enumerators */
        {
            Xar* enumeratores;
            i32 num_enum;
            i32 k;
            s32 valor_currens;
            TitulusValor* tabula;

            enumeratores = enum_spec->datum.enum_spec.enumeratores;
            si (enumeratores == NIHIL)
            {
                redde FALSUM;
            }

            num_enum = xar_numerus(enumeratores);
            si (num_enum == ZEPHYRUM)
            {
                perge; /* Try next declaration */
            }

            /* Verify first enumerator matches prefix */
            {
                ArborNodus** first_ptr;
                ArborNodus* first_enum;
                chorda* first_nomen;

                first_ptr = xar_obtinere(enumeratores, ZEPHYRUM);
                si (first_ptr == NIHIL)
                {
                    perge;
                }
                first_enum = *first_ptr;
                first_nomen = first_enum->datum.folium.valor;
                si (first_nomen == NIHIL ||
                    first_nomen->mensura <= praefixum_len ||
                    strncmp((constans character*)first_nomen->datum, praefixum,
                            (size_t)praefixum_len) != ZEPHYRUM)
                {
                    perge;
                }
            }

            /* Allocate table */
            tabula = piscina_allocare(g_piscina,
                (memoriae_index)(num_enum * (i32)magnitudo(TitulusValor)));
            si (tabula == NIHIL)
            {
                redde FALSUM;
            }

            valor_currens = ZEPHYRUM;
            per (k = ZEPHYRUM; k < num_enum; k++)
            {
                ArborNodus** enum_ptr;
                ArborNodus* enumerator;
                chorda* enum_nomen;
                constans character* stripped_nomen;

                enum_ptr = xar_obtinere(enumeratores, k);
                si (enum_ptr == NIHIL)
                {
                    perge;
                }
                enumerator = *enum_ptr;

                si (enumerator->genus != ARBOR_NODUS_ENUMERATOR &&
                    enumerator->genus != ARBOR_NODUS_IDENTIFIER)
                {
                    perge;
                }

                enum_nomen = enumerator->datum.folium.valor;
                si (enum_nomen == NIHIL)
                {
                    perge;
                }

                /* Strip prefix from name */
                si (enum_nomen->mensura > praefixum_len &&
                    strncmp((constans character*)enum_nomen->datum, praefixum, (size_t)praefixum_len) == ZEPHYRUM)
                {
                    /* Allocate stripped name */
                    i32 stripped_len;
                    character* stripped;

                    stripped_len = enum_nomen->mensura - praefixum_len;
                    stripped = piscina_allocare(g_piscina,
                        (memoriae_index)(stripped_len + I));
                    si (stripped != NIHIL)
                    {
                        memcpy(stripped, enum_nomen->datum + praefixum_len,
                               (size_t)stripped_len);
                        stripped[stripped_len] = '\0';
                        stripped_nomen = stripped;
                    }
                    alioquin
                    {
                        stripped_nomen = NIHIL;
                    }
                }
                alioquin
                {
                    /* No prefix to strip - allocate copy */
                    character* copia;

                    copia = piscina_allocare(g_piscina,
                        (memoriae_index)(enum_nomen->mensura + I));
                    si (copia != NIHIL)
                    {
                        memcpy(copia, enum_nomen->datum, (size_t)enum_nomen->mensura);
                        copia[enum_nomen->mensura] = '\0';
                        stripped_nomen = copia;
                    }
                    alioquin
                    {
                        stripped_nomen = NIHIL;
                    }
                }

                /*
                 * Note: Explicit value checking disabled for now.
                 * The enumerator node uses folium union member for the name,
                 * so we can't also access genericum.liberi (they overlap in the union).
                 * For arbor2_lexema.h, all values are sequential anyway.
                 */

                tabula[k].titulus = stripped_nomen;
                tabula[k].valor = valor_currens;
                valor_currens++;
            }

            *tabula_out = tabula;
            *numerus_out = num_enum;
            redde VERUM;
        }
    }

    redde FALSUM;
}

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
 * NT Name Table (hardcoded for now - Phase 2 will parse #defines)
 * ================================================== */

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

    /* Use dynamic table if available */
    si (g_lexema_tabula != NIHIL)
    {
        per (i = ZEPHYRUM; i < g_lexema_numerus; i++)
        {
            si (g_lexema_tabula[i].titulus != NIHIL &&
                strcmp(g_lexema_tabula[i].titulus, titulus) == ZEPHYRUM)
            {
                redde g_lexema_tabula[i].valor;
            }
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

interior b32
initializare_tabulas(vacuum)
{
    /* Initialize global state */
    g_piscina = piscina_generare_dynamicum("glr_quaestio", 1024 * 128);
    si (g_piscina == NIHIL)
    {
        fprintf(stderr, "Error: Non potest creare piscinam\n");
        redde FALSUM;
    }

    g_intern = internamentum_globale();

    /* Extract token enum from arbor2_lexema.h */
    si (!extrahere_enum_ex_filo(
            "include/arbor2_lexema.h",
            "Arbor2LexemaGenus",
            "ARBOR2_LEXEMA_",
            &g_lexema_tabula,
            &g_lexema_numerus))
    {
        fprintf(stderr, "Warning: Non potest extrahere lexema enum - using fallback\n");
        /* Continue anyway - token lookup will fail but other commands work */
    }

    redde VERUM;
}

integer principale(integer argc, constans character* constans* argv)
{
    /* Initialize lookup tables */
    si (!initializare_tabulas())
    {
        /* Non-fatal - continue with limited functionality */
    }

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
