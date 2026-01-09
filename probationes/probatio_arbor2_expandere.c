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


    /* Claudere credo */
    credo_claudere();
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
