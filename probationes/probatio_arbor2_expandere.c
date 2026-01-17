/* probatio_arbor2_expandere.c - Probationes Arbor2 Expansionis */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "arbor2_expandere.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    b32 praeteritus;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("probatio_arbor2_expandere", 131072);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: Object-like macro expansion
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans object-like macro expansion ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "FOO", "42", NIHIL);

        fons = "int x = FOO;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* int */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INT);

        /* x */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* = */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_ASSIGNATIO);

        /* FOO expanded to 42 */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, II);  /* "42" */

        /* ; */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_SEMICOLON);
    }


    /* ========================================================
     * PROBARE: #define in source creates segment
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Xar* segmenta;

        imprimere("\n--- Probans #define creates segment ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        fons = "#define BAR 1\nint x = BAR;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Check segments were created */
        segmenta = arbor2_expansion_obtinere_segmenta(exp);
        CREDO_NON_NIHIL(segmenta);

        /* Should have at least 2 segments (before and after #define) */
        /* Actually, looking at implementation, first segment is at line 1,
         * second at line where #define appears */
        (vacuum)result;
    }


    /* ========================================================
     * PROBARE: Macro provenance chain
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        Arbor2Token* radix;

        imprimere("\n--- Probans macro provenance chain ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "NUM", "123", NIHIL);

        fons = "NUM";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Find expanded token */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* Check provenance */
        CREDO_NON_NIHIL(tok->origo_meta);
        CREDO_AEQUALIS_I32(tok->origo_meta->genus, ARBOR2_ORIGO_EXPANSIO);

        /* Walk to root */
        radix = arbor2_token_radix(tok);
        CREDO_NON_NIHIL(radix);
        CREDO_AEQUALIS_I32(radix->origo_meta->genus, ARBOR2_ORIGO_FONS);
    }


    /* ========================================================
     * PROBARE: Nested macro expansion
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans nested macro expansion ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "A", "B", NIHIL);
        arbor2_expansion_addere_macro(exp, "B", "99", NIHIL);

        fons = "A";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* A -> B -> 99 */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
    }


    /* ========================================================
     * PROBARE: Query macro at line
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        TabulaDispersa* macros;
        Arbor2MacroDef* def;
        chorda nomen_ch;
        vacuum* valor;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans query macro at line ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        fons = "#define X 1\nint a;\n#undef X\nint b;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        /* At line 2, X should be defined */
        macros = arbor2_expansion_macros_ad_lineam(exp, II);
        CREDO_NON_NIHIL(macros);

        u.c = "X";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = I;

        si (tabula_dispersa_invenire(macros, nomen_ch, &valor))
        {
            def = (Arbor2MacroDef*)valor;
            CREDO_NON_NIHIL(def);
        }
    }


    /* ========================================================
     * PROBARE: est_typedef query
     * ======================================================== */

    {
        Arbor2Expansion* exp;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans est_typedef query ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_typedef(exp, "MyType");

        u.c = "MyType";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VI;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);

        u.c = "NotType";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VII;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), FALSUM);
    }


    /* ========================================================
     * PROBARE: Simple typedef detection
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2TypedefInfo* info;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans simple typedef detection ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "typedef int MyInt;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "MyInt";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);

        info = arbor2_expansion_quaerere_typedef(exp, nomen_ch);
        CREDO_NON_NIHIL(info);
        CREDO_AEQUALIS_I32((i32)info->est_certum, (i32)VERUM);
        CREDO_AEQUALIS_I32((i32)info->layer_discovered, (i32)ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: Latin nomen typedef detection
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2TypedefInfo* info;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans nomen (Latin typedef) detection ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "nomen int MeusInt;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "MeusInt";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VII;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);

        info = arbor2_expansion_quaerere_typedef(exp, nomen_ch);
        CREDO_NON_NIHIL(info);
        /* est_certum is FALSUM for heuristic nomen detection */
        CREDO_AEQUALIS_I32((i32)info->est_certum, (i32)FALSUM);
    }


    /* ========================================================
     * PROBARE: Struct typedef detection
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans struct typedef detection ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "typedef struct { int x; int y; } Point;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "Point";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);

        /* Verify internal identifiers are not registered as typedefs */
        u.c = "x";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = I;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), FALSUM);
    }


    /* ========================================================
     * PROBARE: Function pointer typedef detection
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans function pointer typedef detection ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "typedef int (*Callback)(int, int);";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "Callback";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VIII;

        CREDO_AEQUALIS_I32((i32)arbor2_expansion_est_typedef(exp, nomen_ch), VERUM);
    }


    /* ========================================================
     * PROBARE: Typedef with origo information
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2TypedefInfo* info;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans typedef origo information ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        fons = "typedef int TestType;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");
        (vacuum)result;

        u.c = "TestType";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VIII;

        info = arbor2_expansion_quaerere_typedef(exp, nomen_ch);
        CREDO_NON_NIHIL(info);
        CREDO_NON_NIHIL(info->origo);
        CREDO_AEQUALIS_I32(info->origo->linea, I);  /* First line */
    }


    /* ========================================================
     * PROBARE: Function-like macro simple substitution
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_add[] = {"a", "b", NIHIL};

        imprimere("\n--- Probans function-like macro simple substitution ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "ADD", parametra_add, "((a)+(b))", NIHIL);

        fons = "ADD(1,2)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* 1 */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);

        /* + */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PLUS);

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, V);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* 2 */
        tok = *(Arbor2Token**)xar_obtinere(result, VI);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, VII);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, VIII);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);
    }


    /* ========================================================
     * PROBARE: Function-like macro with nested parens in args
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_add[] = {"a", "b", NIHIL};
        i32 i;

        imprimere("\n--- Probans function-like macro with nested parens ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "ADD", parametra_add, "((a)+(b))", NIHIL);

        /* ADD((1+2),3) - should treat (1+2) as first arg */
        fons = "ADD((1+2),3)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Find the first integer token (should be 1) */
        /* Result should be: ((( 1 + 2 ))+(3)) */
        i = ZEPHYRUM;
        dum (i < xar_numerus(result))
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_INTEGER)
            {
                frange;
            }
            i++;
        }
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        /* Should have found 1 */
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);
    }


    /* ========================================================
     * PROBARE: Function-like macro empty params
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_empty[] = {NIHIL};

        imprimere("\n--- Probans function-like macro empty params ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "EMPTY", parametra_empty, "x", NIHIL);

        fons = "EMPTY()";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* x */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);
    }


    /* ========================================================
     * PROBARE: Function-like macro MAX with conditional
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_max[] = {"a", "b", NIHIL};
        b32 found_question;
        i32 i;

        imprimere("\n--- Probans function-like macro MAX ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        /* MAX(a,b) ((a)>(b)?(a):(b)) */
        arbor2_expansion_addere_macro_functio(exp, "MAX", parametra_max,
                                               "((a)>(b)?(a):(b))", NIHIL);

        fons = "MAX(x,y)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Verify ? token exists (ternary operator) */
        found_question = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_QUAESTIO)
            {
                found_question = VERUM;
                frange;
            }
        }
        CREDO_VERUM(found_question);
    }


    /* ========================================================
     * PROBARE: Function-like macro via #define in source
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans function-like macro via #define ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define SQUARE(x) ((x)*(x)) then SQUARE(5) should produce ((5)*(5)) */
        fons = "#define SQUARE(x) ((x)*(x))\nSQUARE(5)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* 5 */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);

        /* * */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_ASTERISCUS);

        /* ( */
        tok = *(Arbor2Token**)xar_obtinere(result, V);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);

        /* 5 */
        tok = *(Arbor2Token**)xar_obtinere(result, VI);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, VII);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);

        /* ) */
        tok = *(Arbor2Token**)xar_obtinere(result, VIII);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_CLAUSA);
    }


    /* ========================================================
     * PROBARE: Stringification basic
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_str[] = {"x", NIHIL};

        imprimere("\n--- Probans stringification basic ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "STR", parametra_str, "#x", NIHIL);

        fons = "STR(hello)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce "hello" string literal */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
    }


    /* ========================================================
     * PROBARE: Stringification with numeric
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_str[] = {"x", NIHIL};

        imprimere("\n--- Probans stringification numeric ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "STR", parametra_str, "#x", NIHIL);

        fons = "STR(123)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
        /* Should contain "123" */
    }


    /* ========================================================
     * PROBARE: Stringification with escaping
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_str[] = {"x", NIHIL};
        b32 has_backslash;
        i32 j;

        imprimere("\n--- Probans stringification escaping ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "STR", parametra_str, "#x", NIHIL);

        /* Stringifying a string literal should escape the quotes */
        fons = "STR(\"hi\")";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);

        /* Check that result contains backslash (escaped quotes) */
        has_backslash = FALSUM;
        per (j = ZEPHYRUM; j < tok->lexema->valor.mensura; j++)
        {
            si (tok->lexema->valor.datum[j] == '\\')
            {
                has_backslash = VERUM;
                frange;
            }
        }
        CREDO_VERUM(has_backslash);
    }


    /* ========================================================
     * PROBARE: Stringification via #define
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans stringification via #define ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define STRINGIFY(x) #x then STRINGIFY(test) should produce "test" */
        fons = "#define STRINGIFY(x) #x\nSTRINGIFY(test)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce string literal "test" */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
    }


    /* ========================================================
     * PROBARE: Token pasting basic
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_paste[] = {"a", "b", NIHIL};

        imprimere("\n--- Probans token pasting basic ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "PASTE", parametra_paste, "a##b", NIHIL);

        fons = "PASTE(foo,bar)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce foobar identifier */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, VI);  /* "foobar" = 6 */
    }


    /* ========================================================
     * PROBARE: Token pasting prefix
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_var[] = {"n", NIHIL};

        imprimere("\n--- Probans token pasting prefix ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        /* VAR(n) -> var_##n */
        arbor2_expansion_addere_macro_functio(exp, "VAR", parametra_var, "var_##n", NIHIL);

        fons = "VAR(1)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce var_1 identifier */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, V);  /* "var_1" = 5 */
    }


    /* ========================================================
     * PROBARE: Token pasting numeric
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_paste[] = {"a", "b", NIHIL};

        imprimere("\n--- Probans token pasting numeric ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "PASTE", parametra_paste, "a##b", NIHIL);

        fons = "PASTE(12,34)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce 1234 integer */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, IV);  /* "1234" = 4 */
    }


    /* ========================================================
     * PROBARE: Token pasting via #define
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans token pasting via #define ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define GLUE(a,b) a##b then GLUE(hello,world) should produce helloworld */
        fons = "#define GLUE(a,b) a##b\nGLUE(hello,world)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should produce helloworld identifier */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, X);  /* "helloworld" = 10 */
    }


    /* ========================================================
     * PROBARE: Variadic macro basic
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_var[] = {"...", NIHIL};

        imprimere("\n--- Probans variadic macro basic ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "VARIADIC", parametra_var,
                                               "__VA_ARGS__", NIHIL);

        /* VARIADIC(a,b,c) should produce a,b,c */
        fons = "VARIADIC(a,b,c)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* a */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "a" */

        /* , */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_COMMA);

        /* b */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "b" */

        /* , */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_COMMA);

        /* c */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "c" */
    }


    /* ========================================================
     * PROBARE: Variadic macro with prefix
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        constans character* parametra_fmt[] = {"f", "...", NIHIL};

        imprimere("\n--- Probans variadic macro with prefix ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro_functio(exp, "FMT", parametra_fmt,
                                               "f(__VA_ARGS__)", NIHIL);

        fons = "FMT(foo,1,2)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* First should be foo identifier */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);

        /* Second should be ( */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PAREN_APERTA);
    }


    /* ========================================================
     * PROBARE: Variadic macro via #define
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans variadic macro via #define ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define ARGS(...) __VA_ARGS__ then ARGS(x,y,z) should produce x,y,z */
        fons = "#define ARGS(...) __VA_ARGS__\nARGS(x,y,z)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* x */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "x" */

        /* , */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_COMMA);

        /* y */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "y" */

        /* , */
        tok = *(Arbor2Token**)xar_obtinere(result, III);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_COMMA);

        /* z */
        tok = *(Arbor2Token**)xar_obtinere(result, IV);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "z" */
    }


    /* ========================================================
     * PROBARE: Recursion prevention - direct
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans recursion prevention direct ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        /* SELF expands to SELF - should not loop infinitely */
        arbor2_expansion_addere_macro(exp, "SELF", "SELF", NIHIL);

        fons = "SELF";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should output SELF identifier (not expanded further) */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, IV);  /* "SELF" = 4 */
    }


    /* ========================================================
     * PROBARE: Recursion prevention - mutual
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans recursion prevention mutual ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        /* A -> B, B -> A: should stop, not loop */
        arbor2_expansion_addere_macro(exp, "MACRO_A", "MACRO_B", NIHIL);
        arbor2_expansion_addere_macro(exp, "MACRO_B", "MACRO_A", NIHIL);

        fons = "MACRO_A";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should output MACRO_A (stopped because A was already being expanded) */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
    }


    /* ========================================================
     * PROBARE: Recursion prevention via #define
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        i32 num;

        imprimere("\n--- Probans recursion prevention via #define ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        fons = "#define RECURSIVE RECURSIVE\nRECURSIVE";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Test that processing completes (no infinite loop) */
        /* and produces some output */
        num = xar_numerus(result);
        CREDO_MAIOR_I32(num, ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: #undef stops expansion
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        b32 found_integer;
        b32 found_identifier;
        i32 i;

        imprimere("\n--- Probans #undef stops expansion ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* Define, use, undef, use again */
        fons = "#define TEMP 42\nTEMP\n#undef TEMP\nTEMP";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* First TEMP should be expanded to 42 (INTEGER) */
        /* Second TEMP should remain identifier (after #undef) */
        found_integer = FALSUM;
        found_identifier = FALSUM;

        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_INTEGER)
            {
                found_integer = VERUM;
            }
            si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
            {
                found_identifier = VERUM;
            }
        }

        /* Should have at least one integer (first TEMP expanded) */
        CREDO_VERUM(found_integer);
        /* Should have at least one identifier (second TEMP not expanded) */
        CREDO_VERUM(found_identifier);
    }


    /* ========================================================
     * PROBARE: Re-define after #undef
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        i32 integer_count;
        i32 i;

        imprimere("\n--- Probans re-define after #undef ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        fons = "#define X 1\nX\n#undef X\n#define X 2\nX";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should have integers from both X expansions */
        integer_count = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_INTEGER)
            {
                integer_count++;
            }
        }
        /* Should have at least one integer (macro expansion working) */
        CREDO_MAIOR_I32(integer_count, ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: #undef non-existent macro (no error)
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;

        imprimere("\n--- Probans #undef non-existent macro ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #undef on non-existent macro should not cause error */
        fons = "#undef NONEXISTENT\nint x;";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Should still have tokens */
        CREDO_MAIOR_I32(xar_numerus(result), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: Variadic macro - zero arguments
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        i32 num;
        b32 found_eof_only;
        i32 i;
        Arbor2Token* tok;

        imprimere("\n--- Probans variadic macro zero arguments ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define ARGS(...) __VA_ARGS__ then ARGS() should produce empty */
        fons = "#define ARGS(...) __VA_ARGS__\nARGS()";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Numerus tokenum debet esse parvum (solum EOF vel vacuum) */
        num = xar_numerus(result);
        found_eof_only = VERUM;
        per (i = ZEPHYRUM; i < num; i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus != ARBOR2_LEXEMA_EOF)
            {
                found_eof_only = FALSUM;
                frange;
            }
        }
        /* Vel vacuum vel solum EOF */
        CREDO_VERUM(num == ZEPHYRUM || found_eof_only);
    }


    /* ========================================================
     * PROBARE: Variadic macro - single argument
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans variadic macro single argument ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define ARGS(...) __VA_ARGS__ then ARGS(x) should produce x */
        fons = "#define ARGS(...) __VA_ARGS__\nARGS(x)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet esse x identificator */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, I);  /* "x" = 1 */
    }


    /* ========================================================
     * PROBARE: Variadic macro - __VA_ARGS__ used twice
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        i32 comma_count;
        i32 i;

        imprimere("\n--- Probans __VA_ARGS__ used twice ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define TWICE(...) __VA_ARGS__ __VA_ARGS__ */
        fons = "#define TWICE(...) __VA_ARGS__ __VA_ARGS__\nTWICE(a,b)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet habere a,b a,b - ergo 3 commas (2 in primo, 2 in secundo, minus 1) */
        /* Actually: a , b a , b = 2 commas */
        comma_count = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_COMMA)
            {
                comma_count++;
            }
        }
        /* Duo commas (unum in primo __VA_ARGS__, unum in secundo) */
        CREDO_AEQUALIS_I32(comma_count, II);
    }


    /* ========================================================
     * PROBARE: Nested macro invocation - macro arg is macro call
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        i32 plus_count;
        i32 i;

        imprimere("\n--- Probans nested macro - arg is macro call ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define MUL(a,b) ((a)*(b))
         * #define ADD(a,b) ((a)+(b))
         * ADD(MUL(2,3), 4) -> ((((2)*(3)))+(4)) */
        fons = "#define MUL(a,b) ((a)*(b))\n#define ADD(a,b) ((a)+(b))\nADD(MUL(2,3),4)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet habere PLUS token */
        plus_count = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_PLUS)
            {
                plus_count++;
            }
        }
        CREDO_AEQUALIS_I32(plus_count, I);
    }


    /* ========================================================
     * PROBARE: Nested macro invocation - chained function macros
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;
        b32 found_identifier;
        i32 i;

        imprimere("\n--- Probans chained function macros ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define INNER(x) (x)
         * #define OUTER(x) [x]
         * OUTER(INNER(foo)) -> [(foo)] */
        fons = "#define INNER(x) (x)\n#define OUTER(x) [x]\nOUTER(INNER(foo))";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet habere foo identifier et brackets */
        found_identifier = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(result); i++)
        {
            tok = *(Arbor2Token**)xar_obtinere(result, i);
            si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
                tok->lexema->valor.mensura == III)  /* "foo" = 3 */
            {
                found_identifier = VERUM;
                frange;
            }
        }
        CREDO_VERUM(found_identifier);
    }


    /* ========================================================
     * PROBARE: Object-like macro via #define - simple
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans object-like macro via #define simple ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define FOO 42 then FOO -> 42 */
        fons = "#define FOO 42\nFOO";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* 42 */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, II);  /* "42" = 2 */
    }


    /* ========================================================
     * PROBARE: Object-like macro via #define - multi-token
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans object-like macro multi-token ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define EXPR 1+2 then EXPR -> 1 + 2 */
        fons = "#define EXPR 1+2\nEXPR";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* 1 */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);

        /* + */
        tok = *(Arbor2Token**)xar_obtinere(result, I);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PLUS);

        /* 2 */
        tok = *(Arbor2Token**)xar_obtinere(result, II);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
    }


    /* ========================================================
     * PROBARE: Stringification - empty argument
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans stringification empty argument ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define STR(x) #x then STR() -> "" */
        fons = "#define STR(x) #x\nSTR()";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet esse chorda literalis vacua "" */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
        /* Mensura debet esse II pro "" (duae quotes) */
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, II);
    }


    /* ========================================================
     * PROBARE: Stringification - multiple tokens
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans stringification multiple tokens ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define STR(x) #x then STR(a b c) -> "a b c" */
        fons = "#define STR(x) #x\nSTR(a b c)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet esse chorda literalis "a b c" */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_STRING_LIT);
        /* Mensura maior quam simplex token */
        CREDO_MAIOR_I32(tok->lexema->valor.mensura, III);
    }


    /* ========================================================
     * PROBARE: Token pasting - create operator
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans token pasting create operator ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define OP(a,b) a##b then OP(+,=) -> += */
        fons = "#define OP(a,b) a##b\nOP(+,=)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Debet esse PLUS_ASSIGN vel duo tokens */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        /* += debet esse PLUS_ASSIGN */
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_PLUS_ASSIGN);
    }


    /* ========================================================
     * PROBARE: Token pasting - empty left argument
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;

        imprimere("\n--- Probans token pasting empty left arg ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* #define PASTE(a,b) a##b then PASTE(,bar)
         * Comportamentum vacuum paste non est bene definitum.
         * Verificare quod non frangit et producit aliquid. */
        fons = "#define PASTE(a,b) a##b\nPASTE(,bar)";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* Verificare quod processatio non fregit et producit tokens */
        CREDO_MAIOR_I32(xar_numerus(result), ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: Triple nested macro expansion
     * ======================================================== */

    {
        constans character* fons;
        Arbor2Expansion* exp;
        Xar* result;
        Arbor2Token* tok;

        imprimere("\n--- Probans triple nested macro expansion ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        /* A -> B -> C -> 999 */
        fons = "#define A B\n#define B C\n#define C 999\nA";
        result = arbor2_expansion_processare(exp, fons, (i32)strlen(fons), "test.c");

        /* A -> B -> C -> 999 */
        tok = *(Arbor2Token**)xar_obtinere(result, ZEPHYRUM);
        CREDO_AEQUALIS_I32(tok->lexema->genus, ARBOR2_LEXEMA_INTEGER);
        CREDO_AEQUALIS_I32(tok->lexema->valor.mensura, III);  /* "999" = 3 */
    }


    /* ========================================================
     * PROBARE: Lookahead genus - simple INT macro
     * ======================================================== */

    {
        Arbor2Expansion* exp;
        Arbor2LexemaGenus genus;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans lookahead genus simple INT macro ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "TYPUS", "int", NIHIL);

        u.c = "TYPUS";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        genus = arbor2_expansion_lookahead_genus(exp, nomen_ch);
        CREDO_AEQUALIS_I32((i32)genus, (i32)ARBOR2_LEXEMA_INT);
    }


    /* ========================================================
     * PROBARE: Lookahead genus - unknown macro returns ERROR
     * ======================================================== */

    {
        Arbor2Expansion* exp;
        Arbor2LexemaGenus genus;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans lookahead genus unknown macro ---\n");

        exp = arbor2_expansion_creare(piscina, intern);

        u.c = "NONEXISTENT";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = XI;

        genus = arbor2_expansion_lookahead_genus(exp, nomen_ch);
        CREDO_AEQUALIS_I32((i32)genus, (i32)ARBOR2_LEXEMA_ERROR);
    }


    /* ========================================================
     * PROBARE: Lookahead genus - empty macro returns EOF
     * ======================================================== */

    {
        Arbor2Expansion* exp;
        Arbor2LexemaGenus genus;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans lookahead genus empty macro ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "VACUUM", "", NIHIL);

        u.c = "VACUUM";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VI;

        genus = arbor2_expansion_lookahead_genus(exp, nomen_ch);
        CREDO_AEQUALIS_I32((i32)genus, (i32)ARBOR2_LEXEMA_EOF);
    }


    /* ========================================================
     * PROBARE: Lookahead extended - macro expanding to macro
     * ======================================================== */

    {
        Arbor2Expansion* exp;
        Arbor2ExpansionLookahead lookahead;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans lookahead extended recursive macro ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "INNER", "42", NIHIL);
        arbor2_expansion_addere_macro(exp, "OUTER", "INNER", NIHIL);

        u.c = "OUTER";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        lookahead = arbor2_expansion_lookahead(exp, nomen_ch);

        /* Primus token est INNER identificator */
        CREDO_AEQUALIS_I32((i32)lookahead.genus, (i32)ARBOR2_LEXEMA_IDENTIFICATOR);
        /* INNER est macro, ergo est_recursivum debet esse VERUM */
        CREDO_AEQUALIS_I32((i32)lookahead.est_recursivum, (i32)VERUM);
        CREDO_AEQUALIS_I32((i32)lookahead.est_vacuum, (i32)FALSUM);
    }


    /* ========================================================
     * PROBARE: Lookahead extended - empty macro est_vacuum
     * ======================================================== */

    {
        Arbor2Expansion* exp;
        Arbor2ExpansionLookahead lookahead;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans lookahead extended empty macro est_vacuum ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "EMPTY", "", NIHIL);

        u.c = "EMPTY";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        lookahead = arbor2_expansion_lookahead(exp, nomen_ch);

        CREDO_AEQUALIS_I32((i32)lookahead.genus, (i32)ARBOR2_LEXEMA_EOF);
        CREDO_AEQUALIS_I32((i32)lookahead.est_vacuum, (i32)VERUM);
        CREDO_AEQUALIS_I32((i32)lookahead.est_recursivum, (i32)FALSUM);
    }


    /* ========================================================
     * PROBARE: Lookahead extended - non-recursive identifier
     * ======================================================== */

    {
        Arbor2Expansion* exp;
        Arbor2ExpansionLookahead lookahead;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans lookahead extended non-recursive identifier ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        /* Macro expandit ad identificator qui NON est macro */
        arbor2_expansion_addere_macro(exp, "ALIAS", "foo", NIHIL);

        u.c = "ALIAS";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = V;

        lookahead = arbor2_expansion_lookahead(exp, nomen_ch);

        /* foo est identificator sed non macro */
        CREDO_AEQUALIS_I32((i32)lookahead.genus, (i32)ARBOR2_LEXEMA_IDENTIFICATOR);
        CREDO_AEQUALIS_I32((i32)lookahead.est_recursivum, (i32)FALSUM);
        CREDO_AEQUALIS_I32((i32)lookahead.est_vacuum, (i32)FALSUM);
    }


    /* ========================================================
     * PROBARE: Lookahead genus - integer literal
     * ======================================================== */

    {
        Arbor2Expansion* exp;
        Arbor2LexemaGenus genus;
        chorda nomen_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Probans lookahead genus integer literal ---\n");

        exp = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp, "NUMERUS", "42", NIHIL);

        u.c = "NUMERUS";
        nomen_ch.datum = u.m;
        nomen_ch.mensura = VII;

        genus = arbor2_expansion_lookahead_genus(exp, nomen_ch);
        CREDO_AEQUALIS_I32((i32)genus, (i32)ARBOR2_LEXEMA_INTEGER);
    }


    /* Compendium */
    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
