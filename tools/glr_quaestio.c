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

/* Dynamic NT lookup table (from Arbor2NonTerminalis enum - for rules command) */
hic_manens TitulusValor* g_nt_tabula = NIHIL;
hic_manens i32 g_nt_numerus = ZEPHYRUM;

/* Dynamic INT_NT lookup table (from #define INT_NT_* - for goto command) */
hic_manens TitulusValor* g_int_nt_tabula = NIHIL;
hic_manens i32 g_int_nt_numerus = ZEPHYRUM;

/* ==================================================
 * Parsed Table Structures (from source introspection)
 * ================================================== */

/* Parsed GOTO entry */
nomen structura {
    i32 status;
    s32 nt;           /* INT_NT_* value */
    i32 status_novus;
} ParsedGoto;

hic_manens ParsedGoto* g_goto_tabula = NIHIL;
hic_manens i32 g_goto_numerus = ZEPHYRUM;

/* Parsed ACTION entry */
nomen structura {
    s32 status;       /* can be -1 before first state comment */
    s32 lexema;       /* ARBOR2_LEXEMA_* value */
    i32 actio;        /* 0=ERROR, 1=SHIFT, 2=REDUCE, 3=ACCEPT */
    i32 valor;
    b32 conflictus_intentus;  /* VERUM if intentional GLR conflict */
} ParsedActio;

hic_manens ParsedActio* g_actio_tabula = NIHIL;
hic_manens i32 g_actio_numerus = ZEPHYRUM;

/* Parsed REGULA entry */
nomen structura {
    s32 sinister;     /* ARBOR2_NT_* value */
    i32 longitudo;
    i32 nodus_genus;  /* ARBOR2_NODUS_* value */
    constans character* descriptio;  /* Rule description (Phase 2) */
} ParsedRegula;

/* Parsed STATE description entry (Phase 6) */
nomen structura {
    i32 status;
    constans character* descriptio;
} ParsedStatusDesc;

hic_manens ParsedRegula* g_regula_tabula = NIHIL;
hic_manens i32 g_regula_numerus = ZEPHYRUM;

/* State descriptions (from STATUS_TABULA_PARTIAL) */
hic_manens ParsedStatusDesc* g_status_desc_tabula = NIHIL;
hic_manens i32 g_status_desc_numerus = ZEPHYRUM;

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

/*
 * Extrahere INT_NT_* defines ex arbor2_glr_tabula.c via text parsing.
 * These are the actual values used in the GOTO table.
 */
interior b32
extrahere_int_nt_ex_filo(
    constans character* via,
    TitulusValor** tabula_out,
    i32* numerus_out)
{
    FILE* f;
    character linea[512];
    i32 numerus;
    i32 capacitas;
    TitulusValor* tabula;

    *tabula_out = NIHIL;
    *numerus_out = ZEPHYRUM;

    f = fopen(via, "r");
    si (f == NIHIL)
    {
        fprintf(stderr, "Warning: Non potest aperire %s\n", via);
        redde FALSUM;
    }

    /* First pass: count matching lines */
    numerus = ZEPHYRUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        si (strncmp(linea, "#define INT_NT_", 15) == ZEPHYRUM)
        {
            numerus++;
        }
    }

    si (numerus == ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Allocate table */
    capacitas = numerus;
    tabula = piscina_allocare(g_piscina,
        (memoriae_index)(capacitas * (i32)magnitudo(TitulusValor)));
    si (tabula == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Second pass: extract names and values */
    rewind(f);
    numerus = ZEPHYRUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        si (strncmp(linea, "#define INT_NT_", 15) == ZEPHYRUM)
        {
            character* nomen_initium;
            character* nomen_finis;
            character* valor_initium;
            character* nomen_copia;
            i32 nomen_len;
            s32 valor;

            /* Extract name: starts after "#define INT_NT_" */
            nomen_initium = linea + 15;
            nomen_finis = nomen_initium;
            dum (*nomen_finis != '\0' && *nomen_finis != ' ' &&
                 *nomen_finis != '\t' && *nomen_finis != '\n')
            {
                nomen_finis++;
            }
            nomen_len = (i32)(nomen_finis - nomen_initium);

            /* Extract value: skip whitespace after name */
            valor_initium = nomen_finis;
            dum (*valor_initium == ' ' || *valor_initium == '\t')
            {
                valor_initium++;
            }
            valor = (s32)strtol(valor_initium, NIHIL, 10);

            /* Allocate and copy name */
            nomen_copia = piscina_allocare(g_piscina,
                (memoriae_index)(nomen_len + I));
            si (nomen_copia != NIHIL)
            {
                memcpy(nomen_copia, nomen_initium, (size_t)nomen_len);
                nomen_copia[nomen_len] = '\0';

                tabula[numerus].titulus = nomen_copia;
                tabula[numerus].valor = valor;
                numerus++;
            }
        }
    }

    fclose(f);

    *tabula_out = tabula;
    *numerus_out = numerus;
    redde VERUM;
}

/* ==================================================
 * Source Table Parsing
 * ================================================== */

/*
 * Resolve INT_NT_<name> to its value using pre-extracted table.
 */
interior s32
resolvere_int_nt(constans character* titulus_nt, i32 titulus_len)
{
    i32 i;

    si (g_int_nt_tabula == NIHIL)
    {
        redde -I;
    }

    per (i = ZEPHYRUM; i < g_int_nt_numerus; i++)
    {
        si (g_int_nt_tabula[i].titulus != NIHIL &&
            (i32)strlen(g_int_nt_tabula[i].titulus) == titulus_len &&
            strncmp(g_int_nt_tabula[i].titulus, titulus_nt, (size_t)titulus_len) == ZEPHYRUM)
        {
            redde g_int_nt_tabula[i].valor;
        }
    }

    redde -I;
}

/*
 * Resolve ARBOR2_LEXEMA_<name> to its value.
 */
interior s32
resolvere_lexema(constans character* titulus_nt, i32 titulus_len)
{
    i32 i;

    si (g_lexema_tabula == NIHIL)
    {
        redde -I;
    }

    per (i = ZEPHYRUM; i < g_lexema_numerus; i++)
    {
        si (g_lexema_tabula[i].titulus != NIHIL &&
            (i32)strlen(g_lexema_tabula[i].titulus) == titulus_len &&
            strncmp(g_lexema_tabula[i].titulus, titulus_nt, (size_t)titulus_len) == ZEPHYRUM)
        {
            redde g_lexema_tabula[i].valor;
        }
    }

    redde -I;
}

/*
 * Resolve ARBOR2_NT_<name> to its value.
 */
interior s32
resolvere_nt(constans character* titulus_nt, i32 titulus_len)
{
    i32 i;

    si (g_nt_tabula == NIHIL)
    {
        redde -I;
    }

    per (i = ZEPHYRUM; i < g_nt_numerus; i++)
    {
        si (g_nt_tabula[i].titulus != NIHIL &&
            (i32)strlen(g_nt_tabula[i].titulus) == titulus_len &&
            strncmp(g_nt_tabula[i].titulus, titulus_nt, (size_t)titulus_len) == ZEPHYRUM)
        {
            redde g_nt_tabula[i].valor;
        }
    }

    redde -I;
}

/*
 * Parse GOTO_TABULA[] from arbor2_glr_tabula.c source.
 * Format: { <state>, INT_NT_<name>, <new_state> },
 */
interior b32
parsere_goto_tabula(constans character* via)
{
    FILE* f;
    character linea[512];
    b32 in_tabula;
    i32 numerus;
    i32 capacitas;
    ParsedGoto* tabula;

    f = fopen(via, "r");
    si (f == NIHIL)
    {
        fprintf(stderr, "Warning: Non potest aperire %s\n", via);
        redde FALSUM;
    }

    /* First pass: count entries */
    in_tabula = FALSUM;
    numerus = ZEPHYRUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        si (strstr(linea, "GOTO_TABULA[]") != NIHIL)
        {
            in_tabula = VERUM;
            perge;
        }

        si (in_tabula)
        {
            /* End of array */
            si (strstr(linea, "};") != NIHIL)
            {
                frange;
            }

            /* Count lines with { number, INT_NT_ */
            si (strstr(linea, "{ ") != NIHIL && strstr(linea, "INT_NT_") != NIHIL)
            {
                numerus++;
            }
        }
    }

    si (numerus == ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Allocate table */
    capacitas = numerus;
    tabula = piscina_allocare(g_piscina,
        (memoriae_index)(capacitas * (i32)magnitudo(ParsedGoto)));
    si (tabula == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Second pass: parse entries */
    rewind(f);
    in_tabula = FALSUM;
    numerus = ZEPHYRUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        si (strstr(linea, "GOTO_TABULA[]") != NIHIL)
        {
            in_tabula = VERUM;
            perge;
        }

        si (in_tabula)
        {
            character* p;
            character* nt_start;
            character* nt_end;
            i32 status;
            s32 nt_val;
            i32 status_novus;
            i32 nt_len;

            si (strstr(linea, "};") != NIHIL)
            {
                frange;
            }

            p = strstr(linea, "{ ");
            si (p == NIHIL || strstr(linea, "INT_NT_") == NIHIL)
            {
                perge;
            }

            /* Parse: { <status>, INT_NT_<name>, <status_novus> } */
            p += II; /* skip "{ " */
            status = (i32)strtol(p, &p, 10);

            /* Skip to INT_NT_ */
            nt_start = strstr(p, "INT_NT_");
            si (nt_start == NIHIL)
            {
                perge;
            }
            nt_start += VII; /* skip "INT_NT_" */

            /* Find end of NT name */
            nt_end = nt_start;
            dum (*nt_end != '\0' && *nt_end != ',' && *nt_end != ' ' && *nt_end != '\t')
            {
                nt_end++;
            }
            nt_len = (i32)(nt_end - nt_start);

            /* Resolve NT name to value */
            nt_val = resolvere_int_nt(nt_start, nt_len);
            si (nt_val < ZEPHYRUM)
            {
                /* Unknown NT - skip */
                perge;
            }

            /* Parse status_novus */
            p = nt_end;
            dum (*p == ',' || *p == ' ' || *p == '\t')
            {
                p++;
            }
            status_novus = (i32)strtol(p, NIHIL, 10);

            /* Store entry */
            tabula[numerus].status = status;
            tabula[numerus].nt = nt_val;
            tabula[numerus].status_novus = status_novus;
            numerus++;
        }
    }

    fclose(f);

    g_goto_tabula = tabula;
    g_goto_numerus = numerus;
    redde VERUM;
}

/*
 * Parse per-state action arrays from arbor2_glr_tabula.c source.
 * Format: STATUS_N_ACTIONES[] = { { LEXEMA, ACTIO, value, conflict_flag }, ... }
 */
interior b32
parsere_actiones_tabula(constans character* via)
{
    FILE* f;
    character linea[512];
    i32 numerus;
    i32 capacitas;
    s32 status_currens;
    b32 in_array;
    ParsedActio* tabula;

    f = fopen(via, "r");
    si (f == NIHIL)
    {
        redde FALSUM;
    }

    /* First pass: count action entries across all STATUS_N_ACTIONES arrays */
    numerus = ZEPHYRUM;
    in_array = FALSUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        /* Track when entering/exiting STATUS_N_ACTIONES arrays */
        si (strstr(linea, "STATUS_") != NIHIL && strstr(linea, "_ACTIONES[]") != NIHIL)
        {
            in_array = VERUM;
            perge;
        }
        si (in_array && strstr(linea, "};") != NIHIL)
        {
            in_array = FALSUM;
            perge;
        }

        /* Only count entries within per-state arrays */
        si (in_array &&
            strstr(linea, "{ ARBOR2_LEXEMA_") != NIHIL &&
            strstr(linea, "ARBOR2_ACTIO_") != NIHIL)
        {
            numerus++;
        }
    }

    si (numerus == ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Allocate table */
    capacitas = numerus;
    tabula = piscina_allocare(g_piscina,
        (memoriae_index)(capacitas * (i32)magnitudo(ParsedActio)));
    si (tabula == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Second pass: parse entries */
    rewind(f);
    numerus = ZEPHYRUM;
    status_currens = -I;
    in_array = FALSUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        character* p;
        character* lex_start;
        character* lex_end;
        character* actio_start;
        character* actio_end;
        s32 lex_val;
        i32 actio_val;
        i32 valor;
        b32 conflict_flag;

        /* Check for STATUS_N_ACTIONES[] array start */
        p = strstr(linea, "STATUS_");
        si (p != NIHIL && strstr(linea, "_ACTIONES[]") != NIHIL)
        {
            /* Extract state number from STATUS_N_ACTIONES */
            status_currens = (s32)strtol(p + VII, NIHIL, 10);
            in_array = VERUM;
            perge;
        }

        /* Check for array end */
        si (in_array && strstr(linea, "};") != NIHIL)
        {
            in_array = FALSUM;
            perge;
        }

        /* Parse action entry - only if inside a STATUS_N_ACTIONES array */
        si (!in_array)
        {
            perge;
        }

        p = strstr(linea, "{ ARBOR2_LEXEMA_");
        si (p == NIHIL)
        {
            perge;
        }

        /* Parse lexema */
        lex_start = p + XVI; /* skip "{ ARBOR2_LEXEMA_" */
        lex_end = lex_start;
        dum (*lex_end != '\0' && *lex_end != ',' && *lex_end != ' ')
        {
            lex_end++;
        }
        lex_val = resolvere_lexema(lex_start, (i32)(lex_end - lex_start));
        si (lex_val < ZEPHYRUM)
        {
            perge;
        }

        /* Parse actio type */
        actio_start = strstr(lex_end, "ARBOR2_ACTIO_");
        si (actio_start == NIHIL)
        {
            perge;
        }
        actio_start += XIII; /* skip "ARBOR2_ACTIO_" */
        actio_end = actio_start;
        dum (*actio_end != '\0' && *actio_end != ',' && *actio_end != ' ')
        {
            actio_end++;
        }

        /* Map actio name to value */
        si (strncmp(actio_start, "ERROR", V) == ZEPHYRUM)
        {
            actio_val = ZEPHYRUM;
        }
        alioquin si (strncmp(actio_start, "SHIFT", V) == ZEPHYRUM)
        {
            actio_val = I;
        }
        alioquin si (strncmp(actio_start, "REDUCE", VI) == ZEPHYRUM)
        {
            actio_val = II;
        }
        alioquin si (strncmp(actio_start, "ACCEPT", VI) == ZEPHYRUM)
        {
            actio_val = III;
        }
        alioquin
        {
            perge;
        }

        /* Parse value (3rd field) */
        p = actio_end;
        dum (*p == ',' || *p == ' ' || *p == '\t')
        {
            p++;
        }
        valor = (i32)strtol(p, &p, 10);

        /* Parse conflict flag (4th field): VERUM or FALSUM */
        conflict_flag = FALSUM;
        dum (*p == ',' || *p == ' ' || *p == '\t')
        {
            p++;
        }
        si (strncmp(p, "VERUM", V) == ZEPHYRUM)
        {
            conflict_flag = VERUM;
        }

        /* Store entry */
        tabula[numerus].status = status_currens;
        tabula[numerus].lexema = lex_val;
        tabula[numerus].actio = actio_val;
        tabula[numerus].valor = valor;
        tabula[numerus].conflictus_intentus = conflict_flag;
        numerus++;
    }

    fclose(f);

    g_actio_tabula = tabula;
    g_actio_numerus = numerus;
    redde VERUM;
}

/*
 * Parse REGULAE[] from arbor2_glr_tabula.c source.
 * Format: { ARBOR2_NT_<name>, <length>, ARBOR2_NODUS_<type>, "description" },
 */
interior b32
parsere_regulae_tabula(constans character* via)
{
    FILE* f;
    character linea[512];
    b32 in_tabula;
    i32 numerus;
    i32 capacitas;
    ParsedRegula* tabula;

    f = fopen(via, "r");
    si (f == NIHIL)
    {
        redde FALSUM;
    }

    /* First pass: count entries */
    in_tabula = FALSUM;
    numerus = ZEPHYRUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        si (strstr(linea, "REGULAE[]") != NIHIL && strstr(linea, "Arbor2Regula") != NIHIL)
        {
            in_tabula = VERUM;
            perge;
        }

        si (in_tabula)
        {
            si (strstr(linea, "};") != NIHIL && linea[ZEPHYRUM] != ' ')
            {
                frange;
            }

            si (strstr(linea, "{ ARBOR2_NT_") != NIHIL)
            {
                numerus++;
            }
        }
    }

    si (numerus == ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Allocate table */
    capacitas = numerus;
    tabula = piscina_allocare(g_piscina,
        (memoriae_index)(capacitas * (i32)magnitudo(ParsedRegula)));
    si (tabula == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Second pass: parse entries */
    rewind(f);
    in_tabula = FALSUM;
    numerus = ZEPHYRUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        si (strstr(linea, "REGULAE[]") != NIHIL && strstr(linea, "Arbor2Regula") != NIHIL)
        {
            in_tabula = VERUM;
            perge;
        }

        si (in_tabula)
        {
            character* p;
            character* nt_start;
            character* nt_end;
            character* desc_start;
            character* desc_end;
            s32 nt_val;
            i32 rhs_len;
            constans character* descriptio;

            si (strstr(linea, "};") != NIHIL && linea[ZEPHYRUM] != ' ')
            {
                frange;
            }

            p = strstr(linea, "{ ARBOR2_NT_");
            si (p == NIHIL)
            {
                perge;
            }

            /* Parse NT */
            nt_start = p + XII; /* skip "{ ARBOR2_NT_" */
            nt_end = nt_start;
            dum (*nt_end != '\0' && *nt_end != ',' && *nt_end != ' ')
            {
                nt_end++;
            }
            nt_val = resolvere_nt(nt_start, (i32)(nt_end - nt_start));
            si (nt_val < ZEPHYRUM)
            {
                perge;
            }

            /* Parse length */
            p = nt_end;
            dum (*p == ',' || *p == ' ' || *p == '\t')
            {
                p++;
            }
            rhs_len = (i32)strtol(p, &p, 10);

            /* Parse description (4th field): "..." */
            descriptio = NIHIL;
            desc_start = strchr(p, '"');
            si (desc_start != NIHIL)
            {
                desc_start++; /* skip opening quote */
                desc_end = strchr(desc_start, '"');
                si (desc_end != NIHIL)
                {
                    i32 desc_len;
                    character* desc_copia;

                    desc_len = (i32)(desc_end - desc_start);
                    desc_copia = piscina_allocare(g_piscina,
                        (memoriae_index)(desc_len + I));
                    si (desc_copia != NIHIL)
                    {
                        memcpy(desc_copia, desc_start, (size_t)desc_len);
                        desc_copia[desc_len] = '\0';
                        descriptio = desc_copia;
                    }
                }
            }

            /* Store entry */
            tabula[numerus].sinister = nt_val;
            tabula[numerus].longitudo = rhs_len;
            tabula[numerus].nodus_genus = ZEPHYRUM;
            tabula[numerus].descriptio = descriptio;
            numerus++;
        }
    }

    fclose(f);

    g_regula_tabula = tabula;
    g_regula_numerus = numerus;
    redde VERUM;
}

/*
 * Parse STATUS_TABULA_PARTIAL from arbor2_glr_tabula.c source.
 * Format: STATUS_INFO(N, "description"),
 */
interior b32
parsere_status_descriptiones(constans character* via)
{
    FILE* f;
    character linea[512];
    b32 in_tabula;
    i32 numerus;
    i32 capacitas;
    ParsedStatusDesc* tabula;

    f = fopen(via, "r");
    si (f == NIHIL)
    {
        redde FALSUM;
    }

    /* First pass: count STATUS_INFO entries */
    in_tabula = FALSUM;
    numerus = ZEPHYRUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        si (strstr(linea, "STATUS_TABULA_PARTIAL[]") != NIHIL)
        {
            in_tabula = VERUM;
            perge;
        }

        si (in_tabula)
        {
            si (strstr(linea, "};") != NIHIL)
            {
                frange;
            }

            si (strstr(linea, "STATUS_INFO(") != NIHIL)
            {
                numerus++;
            }
        }
    }

    si (numerus == ZEPHYRUM)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Allocate table */
    capacitas = numerus;
    tabula = piscina_allocare(g_piscina,
        (memoriae_index)(capacitas * (i32)magnitudo(ParsedStatusDesc)));
    si (tabula == NIHIL)
    {
        fclose(f);
        redde FALSUM;
    }

    /* Second pass: parse entries */
    rewind(f);
    in_tabula = FALSUM;
    numerus = ZEPHYRUM;
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        si (strstr(linea, "STATUS_TABULA_PARTIAL[]") != NIHIL)
        {
            in_tabula = VERUM;
            perge;
        }

        si (in_tabula)
        {
            character* p;
            character* desc_start;
            character* desc_end;
            i32 status_num;
            constans character* descriptio;

            si (strstr(linea, "};") != NIHIL)
            {
                frange;
            }

            p = strstr(linea, "STATUS_INFO(");
            si (p == NIHIL)
            {
                perge;
            }

            /* Parse state number: STATUS_INFO(N, ... */
            p += XII; /* skip "STATUS_INFO(" */
            status_num = (i32)strtol(p, &p, 10);

            /* Parse description: "..." */
            descriptio = NIHIL;
            desc_start = strchr(p, '"');
            si (desc_start != NIHIL)
            {
                desc_start++; /* skip opening quote */
                desc_end = strchr(desc_start, '"');
                si (desc_end != NIHIL)
                {
                    i32 desc_len;
                    character* desc_copia;

                    desc_len = (i32)(desc_end - desc_start);
                    desc_copia = piscina_allocare(g_piscina,
                        (memoriae_index)(desc_len + I));
                    si (desc_copia != NIHIL)
                    {
                        memcpy(desc_copia, desc_start, (size_t)desc_len);
                        desc_copia[desc_len] = '\0';
                        descriptio = desc_copia;
                    }
                }
            }

            /* Store entry */
            tabula[numerus].status = status_num;
            tabula[numerus].descriptio = descriptio;
            numerus++;
        }
    }

    fclose(f);

    g_status_desc_tabula = tabula;
    g_status_desc_numerus = numerus;
    redde VERUM;
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

    /* Use dynamic table if available */
    si (g_nt_tabula != NIHIL)
    {
        per (i = ZEPHYRUM; i < g_nt_numerus; i++)
        {
            si (g_nt_tabula[i].titulus != NIHIL &&
                strcmp(g_nt_tabula[i].titulus, titulus) == ZEPHYRUM)
            {
                redde g_nt_tabula[i].valor;
            }
        }
    }

    /* Fall back to hardcoded table (includes aliases) */
    per (i = ZEPHYRUM; NT_NOMINA[i].titulus != NIHIL; i++)
    {
        si (strcmp(NT_NOMINA[i].titulus, titulus) == ZEPHYRUM)
        {
            redde NT_NOMINA[i].valor;
        }
    }

    redde -I;
}

/*
 * Parse INT_NT_* value for goto table lookups.
 * Uses dynamically extracted values, falls back to hardcoded NT_NOMINA.
 */
interior s32
parsere_int_nt_titulus(constans character* titulus)
{
    i32 i;

    /* Use dynamic INT_NT table if available */
    si (g_int_nt_tabula != NIHIL)
    {
        per (i = ZEPHYRUM; i < g_int_nt_numerus; i++)
        {
            si (g_int_nt_tabula[i].titulus != NIHIL &&
                strcmp(g_int_nt_tabula[i].titulus, titulus) == ZEPHYRUM)
            {
                redde g_int_nt_tabula[i].valor;
            }
        }
    }

    /* Fall back to hardcoded table (includes aliases) */
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
 * Commands
 * ================================================== */

interior constans character*
obtinere_actio_nomen(i32 actio)
{
    commutatio (actio)
    {
        casus ZEPHYRUM: redde "ERROR";
        casus I:        redde "SHIFT";
        casus II:       redde "REDUCE";
        casus III:      redde "ACCEPT";
        ordinarius:     redde "???";
    }
}

interior constans character*
obtinere_lexema_nomen(s32 lexema_val)
{
    i32 i;

    si (g_lexema_tabula != NIHIL)
    {
        per (i = ZEPHYRUM; i < g_lexema_numerus; i++)
        {
            si (g_lexema_tabula[i].valor == lexema_val)
            {
                redde g_lexema_tabula[i].titulus;
            }
        }
    }

    redde "???";
}

interior constans character*
obtinere_nt_nomen(s32 nt_val)
{
    i32 i;

    si (g_nt_tabula != NIHIL)
    {
        per (i = ZEPHYRUM; i < g_nt_numerus; i++)
        {
            si (g_nt_tabula[i].valor == nt_val)
            {
                redde g_nt_tabula[i].titulus;
            }
        }
    }

    redde "???";
}

interior constans character*
obtinere_status_descriptio(i32 status)
{
    i32 i;

    si (g_status_desc_tabula != NIHIL)
    {
        per (i = ZEPHYRUM; i < g_status_desc_numerus; i++)
        {
            si (g_status_desc_tabula[i].status == status)
            {
                redde g_status_desc_tabula[i].descriptio;
            }
        }
    }

    redde NIHIL;
}

interior integer
cmd_state(constans character* arg)
{
    i32 status;
    i32 i;
    i32 inventa;
    character* endptr;
    constans character* desc;

    status = (i32)strtol(arg, &endptr, 10);
    si (*endptr != '\0')
    {
        fprintf(stderr, "Error: '%s' non est numerus validus\n", arg);
        redde I;
    }

    si (g_actio_tabula == NIHIL)
    {
        fprintf(stderr, "Error: ACTIONES tabula non parsita\n");
        redde I;
    }

    /* Show state header with description if available */
    desc = obtinere_status_descriptio(status);
    si (desc != NIHIL)
    {
        printf("State %d: %s\n", status, desc);
    }
    alioquin
    {
        printf("State %d:\n", status);
    }

    inventa = ZEPHYRUM;
    per (i = ZEPHYRUM; i < g_actio_numerus; i++)
    {
        si (g_actio_tabula[i].status == (s32)status)
        {
            printf("  %-16s -> %-7s %3d\n",
                obtinere_lexema_nomen(g_actio_tabula[i].lexema),
                obtinere_actio_nomen(g_actio_tabula[i].actio),
                g_actio_tabula[i].valor);
            inventa++;
        }
    }

    si (inventa == ZEPHYRUM)
    {
        printf("  (no actions found)\n");
    }

    redde ZEPHYRUM;
}

interior integer
cmd_token(constans character* arg)
{
    s32 lexema;
    i32 i;
    i32 inventa;

    lexema = parsere_lexema_titulus(arg);
    si (lexema < (s32)ZEPHYRUM)
    {
        fprintf(stderr, "Error: Lexema '%s' ignotum\n", arg);
        fprintf(stderr, "Exempla: DUAMPERSAND, DUPIPA, SEMICOLON, EOF\n");
        redde I;
    }

    si (g_actio_tabula == NIHIL)
    {
        fprintf(stderr, "Error: ACTIONES tabula non parsita\n");
        redde I;
    }

    inventa = ZEPHYRUM;
    printf("Token %s:\n", arg);

    per (i = ZEPHYRUM; i < g_actio_numerus; i++)
    {
        si (g_actio_tabula[i].lexema == lexema)
        {
            printf("  State %3d: %-16s -> %-7s %3d\n",
                g_actio_tabula[i].status,
                arg,
                obtinere_actio_nomen(g_actio_tabula[i].actio),
                g_actio_tabula[i].valor);
            inventa++;
        }
    }

    printf("  (%d occurrences)\n", inventa);

    redde ZEPHYRUM;
}

interior integer
cmd_goto(constans character* arg)
{
    s32 nt;
    i32 i;
    i32 inventa;

    /* Use INT_NT values for goto table lookups */
    nt = parsere_int_nt_titulus(arg);
    si (nt < (s32)ZEPHYRUM)
    {
        fprintf(stderr, "Error: Non-terminalis '%s' ignotum\n", arg);
        fprintf(stderr, "Exempla: FACTOR, EXPR, TERM, CONIUNCTIO, DISIUNCTIO\n");
        redde I;
    }

    si (g_goto_tabula == NIHIL)
    {
        fprintf(stderr, "Error: GOTO tabula non parsita\n");
        redde I;
    }

    inventa = ZEPHYRUM;
    printf("GOTO for %s:\n", arg);

    per (i = ZEPHYRUM; i < g_goto_numerus; i++)
    {
        si (g_goto_tabula[i].nt == nt)
        {
            printf("  State %3d -> State %3d\n",
                g_goto_tabula[i].status,
                g_goto_tabula[i].status_novus);
            inventa++;
        }
    }

    printf("  (%d entries)\n", inventa);

    redde ZEPHYRUM;
}

interior integer
cmd_rule(constans character* arg)
{
    i32 index;
    character* endptr;

    index = (i32)strtol(arg, &endptr, 10);
    si (*endptr != '\0')
    {
        fprintf(stderr, "Error: '%s' non est numerus validus\n", arg);
        redde I;
    }

    si (g_regula_tabula == NIHIL)
    {
        fprintf(stderr, "Error: REGULAE tabula non parsita\n");
        redde I;
    }

    si (index < ZEPHYRUM || index >= g_regula_numerus)
    {
        fprintf(stderr, "Error: Regula %d extra limites [0, %d)\n",
            index, g_regula_numerus);
        redde I;
    }

    printf("Rule P%d:\n", index);
    printf("  LHS: %s\n", obtinere_nt_nomen(g_regula_tabula[index].sinister));
    printf("  RHS length: %d symbol(s)\n", g_regula_tabula[index].longitudo);
    si (g_regula_tabula[index].descriptio != NIHIL)
    {
        printf("  Production: %s\n", g_regula_tabula[index].descriptio);
    }

    redde ZEPHYRUM;
}

interior i32
computare_max_status(vacuum)
{
    i32 i;
    i32 max_status;

    max_status = ZEPHYRUM;
    si (g_actio_tabula != NIHIL)
    {
        per (i = ZEPHYRUM; i < g_actio_numerus; i++)
        {
            si (g_actio_tabula[i].status > (s32)max_status)
            {
                max_status = (i32)g_actio_tabula[i].status;
            }
        }
    }

    redde max_status + I;
}

interior integer
cmd_stats(vacuum)
{
    printf("GLR Table Statistics (from source):\n");
    printf("  States:   %d\n", computare_max_status());
    printf("  Rules:    %d\n", g_regula_numerus);
    printf("  Actions:  %d\n", g_actio_numerus);
    printf("  GOTOs:    %d\n", g_goto_numerus);

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
    printf("  glr_quaestio rules <NT>     Find all rules producing NT\n");
    printf("  glr_quaestio conflicts      Find shift/reduce conflicts\n");
    printf("  glr_quaestio stats          Show table statistics\n");
    printf("  glr_quaestio tokens         List all known tokens\n");
    printf("  glr_quaestio nts            List all known non-terminals\n");
    printf("  glr_quaestio --help         Show this help\n\n");
    printf("Token examples: DUAMPERSAND, DUPIPA, SEMICOLON, EOF\n");
    printf("NT examples: CONIUNCTIO, DISIUNCTIO, AEQUALITAS, EXPR\n");
}

interior integer
cmd_tokens(vacuum)
{
    i32 i;

    si (g_lexema_tabula == NIHIL || g_lexema_numerus == ZEPHYRUM)
    {
        printf("No tokens loaded (extraction failed)\n");
        redde I;
    }

    printf("Known tokens (%d):\n", g_lexema_numerus);
    per (i = ZEPHYRUM; i < g_lexema_numerus; i++)
    {
        printf("  %3d: %s\n", g_lexema_tabula[i].valor, g_lexema_tabula[i].titulus);
    }
    redde ZEPHYRUM;
}

interior integer
cmd_nts(vacuum)
{
    i32 i;

    si (g_nt_tabula == NIHIL || g_nt_numerus == ZEPHYRUM)
    {
        printf("No NTs loaded (extraction failed), using hardcoded:\n");
        per (i = ZEPHYRUM; NT_NOMINA[i].titulus != NIHIL; i++)
        {
            printf("  %3d: %s\n", NT_NOMINA[i].valor, NT_NOMINA[i].titulus);
        }
        redde ZEPHYRUM;
    }

    printf("Known non-terminals (%d):\n", g_nt_numerus);
    per (i = ZEPHYRUM; i < g_nt_numerus; i++)
    {
        printf("  %3d: %s\n", g_nt_tabula[i].valor, g_nt_tabula[i].titulus);
    }
    redde ZEPHYRUM;
}

interior integer
cmd_rules(constans character* nt_titulus)
{
    s32 nt;
    i32 inventa;
    i32 i;

    nt = parsere_nt_titulus(nt_titulus);
    si (nt < (s32)ZEPHYRUM)
    {
        fprintf(stderr, "Error: Non-terminalis '%s' ignotum\n", nt_titulus);
        fprintf(stderr, "Exempla: CONIUNCTIO, DISIUNCTIO, AEQUALITAS, EXPR\n");
        redde I;
    }

    si (g_regula_tabula == NIHIL)
    {
        fprintf(stderr, "Error: REGULAE tabula non parsita\n");
        redde I;
    }

    inventa = ZEPHYRUM;
    printf("Rules with LHS = %s:\n", nt_titulus);

    per (i = ZEPHYRUM; i < g_regula_numerus; i++)
    {
        si (g_regula_tabula[i].sinister == nt)
        {
            si (g_regula_tabula[i].descriptio != NIHIL)
            {
                printf("  P%d: %s\n", i, g_regula_tabula[i].descriptio);
            }
            alioquin
            {
                printf("  P%d: %s -> (longitudo %d)\n",
                       i,
                       obtinere_nt_nomen(g_regula_tabula[i].sinister),
                       g_regula_tabula[i].longitudo);
            }
            inventa++;
        }
    }

    printf("\nInventa: %d regulae\n", inventa);
    redde ZEPHYRUM;
}

interior integer
cmd_conflicts(vacuum)
{
    i32 num_status;
    i32 status;
    i32 total_conflicts;
    i32 intentional_conflicts;
    i32 i;
    i32 j;
    b32 invenit_conflictum;

    /* Buffers for tracking seen actions per state */
    s32 lexemata_visa[256];
    i32 actiones_visae[256];
    i32 valores_visi[256];
    b32 intentus_visi[256];
    i32 numerus_visorum;

    si (g_actio_tabula == NIHIL)
    {
        fprintf(stderr, "Error: ACTIONES tabula non parsita\n");
        redde I;
    }

    num_status = computare_max_status();
    total_conflicts = ZEPHYRUM;
    intentional_conflicts = ZEPHYRUM;

    printf("Quaerendo conflictus in %d statibus...\n\n", num_status);

    per (status = ZEPHYRUM; status < num_status; status++)
    {
        numerus_visorum = ZEPHYRUM;

        /* Collect all actions for this state */
        per (i = ZEPHYRUM; i < g_actio_numerus; i++)
        {
            si (g_actio_tabula[i].status != (s32)status)
            {
                perge;
            }

            invenit_conflictum = FALSUM;

            /* Check if we've seen this lexema before in this state */
            per (j = ZEPHYRUM; j < numerus_visorum; j++)
            {
                si (lexemata_visa[j] == g_actio_tabula[i].lexema)
                {
                    /* Same lexema - check if actually different action */
                    si (actiones_visae[j] == g_actio_tabula[i].actio &&
                        valores_visi[j] == g_actio_tabula[i].valor)
                    {
                        /* Identical entry (duplicate) - not a conflict */
                        invenit_conflictum = VERUM; /* Skip adding again */
                        frange;
                    }

                    /* Conflictus! Duo actiones diversae pro eodem lexema */
                    {
                        b32 est_intentus;

                        est_intentus = intentus_visi[j] ||
                                       g_actio_tabula[i].conflictus_intentus;

                        printf("  State %d: CONFLICTUS pro %s%s\n",
                               status,
                               obtinere_lexema_nomen(g_actio_tabula[i].lexema),
                               est_intentus ? " (INTENTUS)" : "");
                        printf("    Prior: %-7s %3d\n",
                               obtinere_actio_nomen(actiones_visae[j]),
                               valores_visi[j]);
                        printf("    Nova:  %-7s %3d\n",
                               obtinere_actio_nomen(g_actio_tabula[i].actio),
                               g_actio_tabula[i].valor);
                        total_conflicts++;
                        si (est_intentus)
                        {
                            intentional_conflicts++;
                        }
                    }
                    invenit_conflictum = VERUM;
                    frange;
                }
            }

            /* Only add to seen list if not a duplicate */
            si (!invenit_conflictum && numerus_visorum < 256)
            {
                lexemata_visa[numerus_visorum] = g_actio_tabula[i].lexema;
                actiones_visae[numerus_visorum] = g_actio_tabula[i].actio;
                valores_visi[numerus_visorum] = g_actio_tabula[i].valor;
                intentus_visi[numerus_visorum] = g_actio_tabula[i].conflictus_intentus;
                numerus_visorum++;
            }
        }
    }

    printf("\nConflictus totales: %d (%d intentus, %d inexpectatus)\n",
           total_conflicts,
           intentional_conflicts,
           total_conflicts - intentional_conflicts);
    redde ZEPHYRUM;
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

    /* Extract NT enum from arbor2_glr.h (Arbor2NonTerminalis enum) - for rules command */
    si (!extrahere_enum_ex_filo(
            "include/arbor2_glr.h",
            "Arbor2NonTerminalis",
            "ARBOR2_NT_",
            &g_nt_tabula,
            &g_nt_numerus))
    {
        fprintf(stderr, "Warning: Non potest extrahere NT enum - using fallback\n");
        /* Continue anyway - NT lookup will use hardcoded table */
    }

    /* Extract INT_NT_* defines from arbor2_glr_tabula.c - for goto command */
    si (!extrahere_int_nt_ex_filo(
            "lib/arbor2_glr_tabula.c",
            &g_int_nt_tabula,
            &g_int_nt_numerus))
    {
        fprintf(stderr, "Warning: Non potest extrahere INT_NT defines - using fallback\n");
        /* Continue anyway - will use hardcoded NT_NOMINA table */
    }

    /* Parse tables directly from source - no library dependency */
    si (!parsere_goto_tabula("lib/arbor2_glr_tabula.c"))
    {
        fprintf(stderr, "Warning: Non potest parsere GOTO_TABULA\n");
    }

    si (!parsere_actiones_tabula("lib/arbor2_glr_tabula.c"))
    {
        fprintf(stderr, "Warning: Non potest parsere ACTIONES\n");
    }

    si (!parsere_regulae_tabula("lib/arbor2_glr_tabula.c"))
    {
        fprintf(stderr, "Warning: Non potest parsere REGULAE\n");
    }

    si (!parsere_status_descriptiones("lib/arbor2_glr_tabula.c"))
    {
        fprintf(stderr, "Warning: Non potest parsere STATUS_TABULA descriptiones\n");
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

    si (strcmp(argv[I], "tokens") == ZEPHYRUM)
    {
        redde cmd_tokens();
    }

    si (strcmp(argv[I], "nts") == ZEPHYRUM)
    {
        redde cmd_nts();
    }

    si (strcmp(argv[I], "conflicts") == ZEPHYRUM)
    {
        redde cmd_conflicts();
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

    si (strcmp(argv[I], "rules") == ZEPHYRUM)
    {
        redde cmd_rules(argv[II]);
    }

    fprintf(stderr, "Error: Mandatum '%s' ignotum\n", argv[I]);
    imprimere_auxilium();
    redde I;
}
