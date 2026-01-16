/* probatio_arbor2_scribere.c - Roundtrip tests for AST serializer */
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

/* ==================================================
 * Helper Functions
 * ================================================== */

/* Lexare input string to tokens */
hic_manens Xar*
_lexare(Piscina* p, InternamentumChorda* intern, constans character* input)
{
    Xar* tokens;
    Arbor2Lexator* lex;
    chorda* via;
    unio { constans character* c; i8* m; } u;

    tokens = xar_creare(p, magnitudo(Arbor2Token*));
    lex = arbor2_lexator_creare(p, intern, input, (i32)strlen(input));

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

        si (l->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            perge;
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

/* Test roundtrip for expression parsing */
hic_manens b32
_probare_roundtrip_expressio(Piscina* p, InternamentumChorda* intern,
                             Arbor2Expansion* expansion,
                             constans character* input,
                             constans character* expectatum)
{
    Arbor2GLR* glr;
    Xar* tokens;
    Arbor2GLRResultus result;
    chorda* output;
    b32 aequalis;

    glr = arbor2_glr_creare(p, intern, expansion);
    tokens = _lexare(p, intern, input);
    result = arbor2_glr_parsere_expressio(glr, tokens);

    si (!result.successus)
    {
        imprimere("  FRACTA: parsing failed for '%s'\n", input);
        redde FALSUM;
    }

    output = arbor2_scribere(p, result.radix);

    si (expectatum == NIHIL)
    {
        expectatum = input;
    }

    aequalis = (output->mensura == (i32)strlen(expectatum)) &&
               (memcmp(output->datum, expectatum, (size_t)output->mensura) == ZEPHYRUM);

    si (!aequalis)
    {
        imprimere("  FRACTA: roundtrip mismatch\n");
        imprimere("    Input:    '%s'\n", input);
        imprimere("    Expected: '%s'\n", expectatum);
        imprimere("    Got:      '%.*s'\n", output->mensura, (constans character*)output->datum);
    }

    redde aequalis;
}

/* Test roundtrip for full parsing */
hic_manens b32
_probare_roundtrip(Piscina* p, InternamentumChorda* intern,
                   Arbor2Expansion* expansion,
                   constans character* input,
                   constans character* expectatum)
{
    Arbor2GLR* glr;
    Xar* tokens;
    Arbor2GLRResultus result;
    chorda* output;
    b32 aequalis;

    glr = arbor2_glr_creare(p, intern, expansion);
    tokens = _lexare(p, intern, input);
    result = arbor2_glr_parsere(glr, tokens);

    si (!result.successus)
    {
        imprimere("  FRACTA: parsing failed for '%s'\n", input);
        redde FALSUM;
    }

    output = arbor2_scribere(p, result.radix);

    si (expectatum == NIHIL)
    {
        expectatum = input;
    }

    aequalis = (output->mensura == (i32)strlen(expectatum)) &&
               (memcmp(output->datum, expectatum, (size_t)output->mensura) == ZEPHYRUM);

    si (!aequalis)
    {
        imprimere("  FRACTA: roundtrip mismatch\n");
        imprimere("    Input:    '%s'\n", input);
        imprimere("    Expected: '%s'\n", expectatum);
        imprimere("    Got:      '%.*s'\n", output->mensura, (constans character*)output->datum);
    }

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

    piscina = piscina_generare_dynamicum("probatio_scribere", 1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: Trivia preservation test
     * ======================================================== */
    {
        imprimere("\n--- Probans trivia preservation ---\n");

        /* Test whitespace between tokens */
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a + b", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a  +  b", NIHIL));
    }

    /* ========================================================
     * PROBARE: Simple expressions
     * ======================================================== */
    {
        imprimere("\n--- Probans simple expressions ---\n");

        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "x", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "42", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "3.14", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "'a'", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "\"hello\"", NIHIL));
    }

    /* ========================================================
     * PROBARE: Binary expressions
     * Note: a*b, a/b etc are ambiguous (could be declaration)
     * Testing only non-ambiguous binary expressions here
     * ======================================================== */
    {
        imprimere("\n--- Probans binary expressions ---\n");

        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "1+2", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "1-2", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "1*2", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "1/2", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "1%2", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a==b", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a!=b", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a<b", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a>b", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a&&b", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a||b", NIHIL));
    }

    /* ========================================================
     * PROBARE: Unary expressions
     * ======================================================== */
    {
        imprimere("\n--- Probans unary expressions ---\n");

        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "!x", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "~x", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "++x", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "--x", NIHIL));

        /* Unary minus and plus */
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "-x", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "+x", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "-5", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "- -x", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "-a+b", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a+-b", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "-!x", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a*-b", NIHIL));
    }

    /* ========================================================
     * PROBARE: Postfix expressions
     * ======================================================== */
    {
        imprimere("\n--- Probans postfix expressions ---\n");

        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "x++", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "x--", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a[0]", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a.b", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "a->b", NIHIL));
    }

    /* ========================================================
     * PROBARE: Function calls
     * ======================================================== */
    {
        imprimere("\n--- Probans function calls ---\n");

        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "f()", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "f(x)", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "f(x,y)", NIHIL));
        CREDO_VERUM(_probare_roundtrip_expressio(piscina, intern, expansion, "f(x,y,z)", NIHIL));
    }

    /* ========================================================
     * PROBARE: Simple declarations (in block)
     * Note: Pointer declarations like `int *x` are ambiguous in GLR
     * ======================================================== */
    {
        imprimere("\n--- Probans simple declarations ---\n");

        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{int x;}", NIHIL));
        /* Pointer declarations are ambiguous - skip for now */
    }

    /* ========================================================
     * PROBARE: Declarations with initializers
     * Note: tok_assignatio not yet passed through parser - skip for now
     * ======================================================== */
    {
        imprimere("\n--- Probans declarations with initializers ---\n");

        /* TODO: Need to pass '=' token from P222 to P226 */
        /* For now, test without initializers */
    }

    /* ========================================================
     * PROBARE: Simple statements (in block)
     * ======================================================== */
    {
        imprimere("\n--- Probans simple statements ---\n");

        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{x;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{x=1;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{return;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{return 0;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{break;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{continue;}", NIHIL));
    }

    /* ========================================================
     * PROBARE: Control flow (in block)
     * ======================================================== */
    {
        imprimere("\n--- Probans control flow ---\n");

        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{if(x)y;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{if(x)y;else z;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{while(x)y;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{do x;while(y);}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{for(;;)x;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{for(i=0;i<n;i++)x;}", NIHIL));
    }

    /* ========================================================
     * PROBARE: Compound statements
     * ======================================================== */
    {
        imprimere("\n--- Probans compound statements ---\n");

        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{x;}", NIHIL));
        CREDO_VERUM(_probare_roundtrip(piscina, intern, expansion, "{x;y;}", NIHIL));
    }

    /* ========================================================
     * PROBARE: Initializer lists (in declaration)
     * Note: Requires tok_assignatio fix first - skip for now
     * ======================================================== */
    {
        imprimere("\n--- Probans initializer lists ---\n");

        /* TODO: Need to pass '=' token from P222/P223 to P226 first */
    }

    /* Print summary */
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
