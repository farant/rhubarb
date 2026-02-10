/* probatio_lapifex_c89_expandere.c - Probationes Integrationis Preprocessor-Parser
 *
 * Probat M3: Pons inter arbor2_expandere et lapifex C89 parser.
 * Pipeline: expand -> extract lexemata -> remap typedefs -> segment-split -> parse
 */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "lapifex_c89.h"
#include "arbor2_expandere.h"
#include "arbor2_glr.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ================================================
 * Auxiliaris: parsare fontem per expansionem
 * ================================================ */

hic_manens Arbor2Nodus*
_parsare_fontem(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    Arbor2Expansion*      exp,
    constans character*   fons)
{
    redde lapifex_c89_fontem_parsare(
        piscina, intern, exp, fons, (i32)strlen(fons), "probatio.c");
}

s32 principale(vacuum)
{
    b32                  praeteritus;
    Piscina*             piscina;
    InternamentumChorda* intern;
    Arbor2Nodus*         nodus;

    piscina = piscina_generare_dynamicum(
        "probatio_lapifex_c89_expandere", MMMMXCVI * XVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina non creata\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE I: Simplex pipeline - int x = 42;
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans I: int x = 42; ---\n");
        fprintf(stderr, "[TEST] I\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp, "int x = 42;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE II: Latina.h macros - si -> if, redde -> return
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans II: Latina.h macros ---\n");
        fprintf(stderr, "[TEST] II\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_includere_latina(exp);

        nodus = _parsare_fontem(piscina, intern, exp,
            "integer f(vacuum) { si (I) redde ZEPHYRUM; redde I; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
    }

    /* ========================================================
     * PROBARE III: #define object-like macro
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans III: #define MAX 100; int x = MAX; ---\n");
        fprintf(stderr, "[TEST] III\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp,
            "#define MAX 100\nint x = MAX;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE IV: typedef + typedef name usage
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans IV: typedef int i32t; i32t f(i32t a) { return a; } ---\n");
        fprintf(stderr, "[TEST] IV\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp,
            "typedef int i32t; i32t f(i32t a) { return a; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
    }

    /* ========================================================
     * PROBARE V: function-like macro
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans V: #define SQUARE(x) ((x)*(x)) ---\n");
        fprintf(stderr, "[TEST] V\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp,
            "#define SQUARE(x) ((x)*(x))\nint y = SQUARE(3);");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE VI: API-injected macro used in declaration
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans VI: API macro in declaration ---\n");
        fprintf(stderr, "[TEST] VI\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "VERSION", "42", NIHIL);
        nodus = _parsare_fontem(piscina, intern, exp,
            "int version = VERSION;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE VII: Multiple API-injected macros
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans VII: Multiple API macros ---\n");
        fprintf(stderr, "[TEST] VII\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "WIDTH", "640", NIHIL);
        arbor2_expansion_addere_macro(exp, "HEIGHT", "480", NIHIL);
        nodus = _parsare_fontem(piscina, intern, exp,
            "int w = WIDTH; int h = HEIGHT;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
    }

    /* ========================================================
     * PROBARE VIII: Pre-registered external typedef
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans VIII: External typedef (size_t) ---\n");
        fprintf(stderr, "[TEST] VIII\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_typedef(exp, "size_t");
        nodus = _parsare_fontem(piscina, intern, exp,
            "size_t n = 0;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE IX: Latina.h + typedef interaction
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans IX: nomen integer meus_typus; meus_typus x; ---\n");
        fprintf(stderr, "[TEST] IX\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_includere_latina(exp);
        nodus = _parsare_fontem(piscina, intern, exp,
            "nomen integer meus_typus; meus_typus x;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
    }

    /* ========================================================
     * PROBARE X: Multiple declarations through pipeline
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans X: int a; int b; int c; ---\n");
        fprintf(stderr, "[TEST] X\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp,
            "int a; int b; int c;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), III);
    }

    /* ========================================================
     * PROBARE XI: Function definition through pipeline
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans XI: int f(void) { return 0; } ---\n");
        fprintf(stderr, "[TEST] XI\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp,
            "int f(void) { return 0; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
    }

    /* ========================================================
     * PROBARE XII: #define + typedef interaction
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans XII: #define MYTYPE int; typedef MYTYPE my_int; my_int x; ---\n");
        fprintf(stderr, "[TEST] XII\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp,
            "#define MYTYPE int\ntypedef MYTYPE my_int; my_int x;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
    }

    /* ========================================================
     * PROBARE XIII: Latina.h function with compound body
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans XIII: Latina functi cum corpore complexo ---\n");
        fprintf(stderr, "[TEST] XIII\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_includere_latina(exp);

        nodus = _parsare_fontem(piscina, intern, exp,
            "integer addere(integer a, integer b) { redde a + b; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        CREDO_NON_NIHIL(nodus->datum.definitio_functi.corpus);
    }

    /* ========================================================
     * PROBARE XIV: Latina.h with struct
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans XIV: structura Punctum { integer x; integer y; }; ---\n");
        fprintf(stderr, "[TEST] XIV\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_includere_latina(exp);

        nodus = _parsare_fontem(piscina, intern, exp,
            "structura Punctum { integer x; integer y; };");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XV: Mixed decl + functi through pipeline
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans XV: int g; int f(void) { return g; } ---\n");
        fprintf(stderr, "[TEST] XV\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp,
            "int g; int f(void) { return g; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
    }

    /* ========================================================
     * PROBARE XVI: Multiple #defines
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans XVI: Multiple #defines ---\n");
        fprintf(stderr, "[TEST] XVI\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp,
            "#define A 1\n#define B 2\nint x = A + B;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XVII: Latina.h full pattern - nomen + structura
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans XVII: nomen structura { integer valor; } MeusTypus; ---\n");
        fprintf(stderr, "[TEST] XVII\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_includere_latina(exp);

        nodus = _parsare_fontem(piscina, intern, exp,
            "nomen structura { integer valor; } MeusTypus; MeusTypus m;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
    }

    /* ========================================================
     * PROBARE XVIII: API function-like macro
     * ======================================================== */
    {
        Arbor2Expansion* exp;
        constans character* params[] = {"a", "b", NIHIL};

        imprimere("\n--- Probans XVIII: API function-like macro ---\n");
        fprintf(stderr, "[TEST] XVIII\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(
            exp, "ADD", params, "((a)+(b))", NIHIL);
        nodus = _parsare_fontem(piscina, intern, exp,
            "int sum = ADD(3, 4);");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XIX: Latina.h with for loop
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans XIX: Latina per loop ---\n");
        fprintf(stderr, "[TEST] XIX\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_includere_latina(exp);

        nodus = _parsare_fontem(piscina, intern, exp,
            "vacuum f(vacuum) { integer i; per (i = ZEPHYRUM; i < X; i++) { } }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus,
            (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
    }

    /* ========================================================
     * PROBARE XX: Enum through pipeline
     * ======================================================== */
    {
        Arbor2Expansion* exp;

        imprimere("\n--- Probans XX: enum through pipeline ---\n");
        fprintf(stderr, "[TEST] XX\n");

        exp = arbor2_expansion_creare(piscina, intern);
        nodus = _parsare_fontem(piscina, intern, exp,
            "enum Color { RED, GREEN, BLUE };");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ================================================
     * Compendium
     * ================================================ */
    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        imprimere("\nConditio: OMNIA PRAETERIERUNT\n");
    }

    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
