/* probatio_arbor2_file_roundtrip.c - File-based roundtrip tests for AST serializer */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "arbor2_expandere.h"
#include "arbor2_glr.h"
#include "arbor2_scribere.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/* ==================================================
 * Helper Functions
 * ================================================== */

/* Read entire file into buffer */
hic_manens character*
_legere_fasciculum(constans character* via, i32* mensura_out)
{
    FILE* f;
    longus mensura;
    character* buffer;
    size_t lectum;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        *mensura_out = ZEPHYRUM;
        redde NIHIL;
    }

    fseek(f, 0, SEEK_END);
    mensura = ftell(f);
    fseek(f, 0, SEEK_SET);

    buffer = malloc((size_t)mensura + I);
    si (buffer == NIHIL)
    {
        fclose(f);
        *mensura_out = ZEPHYRUM;
        redde NIHIL;
    }

    lectum = fread(buffer, I, (size_t)mensura, f);
    buffer[lectum] = '\0';
    fclose(f);

    *mensura_out = (i32)lectum;
    redde buffer;
}

/* Lexare input string to tokens */
hic_manens Xar*
_lexare(Piscina* p, InternamentumChorda* intern, constans character* input, i32 mensura)
{
    Xar* tokens;
    Arbor2Lexator* lex;
    chorda* via;
    unio { constans character* c; i8* m; } u;

    tokens = xar_creare(p, magnitudo(Arbor2Token*));
    lex = arbor2_lexator_creare(p, intern, input, mensura);

    u.c = "<test>";
    via = piscina_allocare(p, magnitudo(chorda));
    via->datum = u.m;
    via->mensura = VI;

    dum (VERUM)
    {
        Arbor2Lexema* l = arbor2_lexema_proximum(lex);
        Arbor2Token* t;
        Arbor2Token** slot;

        si (l == NIHIL)
        {
            frange;
        }

        t = arbor2_token_ex_lexema(p, l, via, l->linea, l->columna);
        slot = xar_addere(tokens);
        *slot = t;

        si (l->genus == ARBOR2_LEXEMA_EOF)
        {
            frange;
        }
    }

    redde tokens;
}

/* Test roundtrip for a file */
hic_manens b32
_probare_roundtrip_fasciculum(Piscina* p, InternamentumChorda* intern,
                               Arbor2Expansion* expansion,
                               constans character* via)
{
    character* fons;
    i32 mensura;
    Arbor2GLR* glr;
    Xar* tokens;
    Arbor2GLRResultus result;
    chorda* output;
    b32 aequalis;

    /* Read file */
    fons = _legere_fasciculum(via, &mensura);
    si (fons == NIHIL)
    {
        imprimere("  FRACTA: non potuit legere '%s'\n", via);
        redde FALSUM;
    }

    imprimere("  Testing: %s (%d bytes)\n", via, mensura);

    /* Parse */
    {
        clock_t t0, t1, t2, t3;
        duplex lex_time, parse_time, scrib_time;

        t0 = clock();
        glr = arbor2_glr_creare(p, intern, expansion);
        tokens = _lexare(p, intern, fons, mensura);
        t1 = clock();
        result = arbor2_glr_parsere_translation_unit(glr, tokens);
        t2 = clock();
        output = arbor2_scribere(p, result.radix);
        t3 = clock();

        lex_time = (duplex)(t1 - t0) / CLOCKS_PER_SEC;
        parse_time = (duplex)(t2 - t1) / CLOCKS_PER_SEC;
        scrib_time = (duplex)(t3 - t2) / CLOCKS_PER_SEC;

        si (lex_time + parse_time + scrib_time > 0.1)
        {
            imprimere("    [TIME] lex=%.2fs parse=%.2fs scrib=%.2fs total=%.2fs\n",
                      lex_time, parse_time, scrib_time,
                      lex_time + parse_time + scrib_time);
        }
    }

    si (!result.successus)
    {
        imprimere("  FRACTA: parsing failed for '%s'\n", via);
        si (result.errores != NIHIL)
        {
            i32 i;
            i32 num = xar_numerus(result.errores);
            per (i = ZEPHYRUM; i < num && i < V; i++)
            {
                chorda** err = xar_obtinere(result.errores, i);
                imprimere("    Error: %.*s\n", (*err)->mensura, (*err)->datum);
            }
        }
        free(fons);
        redde FALSUM;
    }

    /* Serialize back */
    output = arbor2_scribere(p, result.radix);

    /* Compare */
    aequalis = (output->mensura == mensura) &&
               (memcmp(output->datum, fons, (size_t)mensura) == ZEPHYRUM);

    si (!aequalis)
    {
        imprimere("  FRACTA: roundtrip mismatch for '%s'\n", via);
        imprimere("    Input length:  %d\n", mensura);
        imprimere("    Output length: %d\n", output->mensura);

        /* Find first difference */
        {
            i32 i;
            i32 min_len = mensura < output->mensura ? mensura : output->mensura;
            per (i = ZEPHYRUM; i < min_len; i++)
            {
                si (fons[i] != output->datum[i])
                {
                    imprimere("    First diff at byte %d: expected 0x%02X '%c', got 0x%02X '%c'\n",
                              i,
                              (insignatus character)fons[i],
                              fons[i] >= 32 && fons[i] < 127 ? fons[i] : '?',
                              (insignatus character)output->datum[i],
                              output->datum[i] >= 32 && output->datum[i] < 127 ? output->datum[i] : '?');
                    frange;
                }
            }
        }

        /* Show context around difference */
        imprimere("\n    --- Expected (first 200 chars) ---\n");
        {
            i32 show = mensura < 200 ? mensura : 200;
            imprimere("    '%.*s'\n", show, fons);
        }
        imprimere("\n    --- Got (first 200 chars) ---\n");
        {
            i32 show = output->mensura < 200 ? output->mensura : 200;
            imprimere("    '%.*s'\n", show, output->datum);
        }
    }
    alioquin
    {
        imprimere("  OK: exact roundtrip\n");
    }

    free(fons);
    redde aequalis;
}

/* ==================================================
 * Main
 * ================================================== */

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;

    piscina = piscina_generare_dynamicum("probatio_file_roundtrip", 4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: Minimal files
     * ======================================================== */
    {
        imprimere("\n--- Probans minimal files ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/minimal3.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/minimal.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/minimal2.c"));
    }

    /* ========================================================
     * PROBARE: Simple file with control flow
     * ======================================================== */
    {
        imprimere("\n--- Probans simple file ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/simple.c"));
    }

    /* ========================================================
     * PROBARE: Multiple structs
     * ======================================================== */
    {
        imprimere("\n--- Probans two structs ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/two_structs.c"));
    }

    /* ========================================================
     * PROBARE: Enums
     * ======================================================== */
    {
        imprimere("\n--- Probans enum file ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/enum_test.c"));
    }

    /* ========================================================
     * PROBARE: Typedef declarations
     * ======================================================== */
    {
        imprimere("\n--- Probans typedef files ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/typedef_simple.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/typedef_struct.c"));
    }

    /* ========================================================
     * PROBARE: Struct definition + simple function
     * ======================================================== */
    {
        imprimere("\n--- Probans struct + function ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/test_debug.c"));
    }

    /* ========================================================
     * PROBARE: Struct/union/enum as function parameters
     *
     * States 1453-1455 reduce struct/union/enum specifiers to
     * non-terminals before parameter parsing continues. This makes
     * the stack arithmetic correct for P43 (param -> type declarator).
     * ======================================================== */
    {
        imprimere("\n--- Probans struct/union/enum parameters ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/struct_param.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/test_struct_func.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/test_slow.c"));
    }

    /* ========================================================
     * PROBARE: Local struct/union/enum variable declarations
     *
     * Tests: void f(void) { struct Point p; }
     * Fixed by adding INT_NT_STRUCT_SPEC/INT_NT_ENUM_SPEC
     * GOTO entries to STATUS_26_GOTO pointing to state 4.
     * ======================================================== */
    {
        imprimere("\n--- Probans local struct variable declarations ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/local_struct_var.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/local_decl_extended.c"));
    }

    /* ========================================================
     * PROBARE: Pointer-to-member (arrow) expressions
     * Tests: p->x access pattern
     * ======================================================== */
    {
        imprimere("\n--- Probans pointer-to-member (arrow) expressions ---\n");

        /* Debug 1: simple pointer deref */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/arrow_debug1.c"));

        /* Debug 2: struct pointer param, no arrow */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/arrow_debug2.c"));

        /* Debug 3: struct pointer param with arrow */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/arrow_debug3.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/arrow_member.c"));

        /* Arrow on LHS of assignment: s->x = 1; */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/arrow_lhs.c"));

        /* Dereference on LHS: *y = 1; */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/deref_lhs.c"));

        /* Arrow on RHS of assignment: *y = s->x; */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/arrow_rhs.c"));

        /* Original issue case: p->x = p->x + dx; */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/arrow_assign.c"));
    }

    /* ========================================================
     * PROBARE: Qualified parameters (const/volatile)
     * ======================================================== */
    {
        imprimere("\n--- Probans qualified parameters ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/const_param.c"));
    }

    /* ========================================================
     * PROBARE: Cast expressions in declaration initializers
     * ======================================================== */
    {
        imprimere("\n--- Probans cast in initializers ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/cast_init.c"));
    }

    /* ========================================================
     * NOTA: Known issues with structs.c and arrays.c
     *
     * 1. CONSECUTIVE COMMENTS - FIXED (was unsigned underflow in
     *    _habet_lineam_vacuam_ante, loop var needed s32 not i32)
     *
     * 2. LOCAL STRUCT VARIABLE DECLARATIONS - FIXED
     *    - Simple case works: { struct Point p; }
     *    - Pointer case works: { struct Point *ptr; }
     *    Fix: Created State 1500 for struct/enum specs in compound
     *    statements that only has SHIFT actions (no reduce conflict).
     *    STATUS_26_GOTO routes STRUCT_SPEC/ENUM_SPEC to State 1500.
     *
     * 3. POINTER-TO-MEMBER expressions - FIXED
     *    p->x = p->x + dx;  -- now works correctly
     *    Fix: Added INT_NT_POSTFIXUM GOTO entries to States 26, 70, 293
     *    so that postfix expressions (subscript, call, member access)
     *    can parse correctly inside compound statements, return, and
     *    assignment RHS.
     *
     * 4. DEREFERENCE AS LVALUE - FIXED
     *    *y = 1;  -- now works correctly
     *    Fix: State 15 (after '* factor' unary) was missing assignment
     *    operators (=, +=, -=, etc). Added all assignment ops to reduce P8.
     * ======================================================== */

    /* ========================================================
     * PROBARE: Files with preprocessor directives
     * Now supported with preprocessor roundtrip implementation!
     * ======================================================== */
    {
        imprimere("\n--- Probans files with preprocessor ---\n");

        /* Simple include + declaration */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/include_test.c"));

        /* Another simple include */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/include_cursor.c"));

        /* File with #define and #ifdef */
        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/preprocessor_test.c"));
    }

    /* ========================================================
     * PROBARE: Expression-level interior comments
     * Tests comments inside expressions for exact roundtrip.
     * ======================================================== */
    {
        imprimere("\n--- Probans expression interior comments ---\n");

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_basic.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_ops.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_unary.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_parens.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_call.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_array.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_member.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_assign.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_ternary.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_cast.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_expr_sizeof.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_stmt_basic.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_decl_basic.c"));

        CREDO_VERUM(_probare_roundtrip_fasciculum(piscina, intern, expansion,
            "probationes/fixa/roundtrip/comment_multiline.c"));
    }

    /* Print summary */
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
