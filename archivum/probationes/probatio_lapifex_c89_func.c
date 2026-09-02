/* probatio_lapifex_c89_func.c - Probationes Definitionum Functi, Translationis, Typedef
 *
 * Probat M2.6: function definitions, translation units, naive typedef.
 */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "lapifex_c89.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ================================================
 * Auxiliares
 * ================================================ */

hic_manens Arbor2Nodus*
_parsare_decl(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons)
{
    redde lapifex_c89_declarationem_parsare(
        piscina, intern, fons, (i32)strlen(fons));
}

hic_manens Arbor2Nodus*
_parsare_trans(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons)
{
    redde lapifex_c89_translationem_parsare(
        piscina, intern, fons, (i32)strlen(fons));
}

/* ================================================
 * Probationes
 * ================================================ */

s32 principale(vacuum)
{
    b32                  praeteritus;
    Piscina*             piscina;
    InternamentumChorda* intern;
    Arbor2Nodus*         nodus;

    piscina = piscina_generare_dynamicum("probatio_lapifex_c89_func", MMMMXCVI * VIII);
    si (!piscina)
    {
        imprimere("FRACTA: piscina non creata\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE I: Simplex definitio functi - int f(void) { return 0; }
     * ======================================================== */
    {
        fprintf(stderr, "[TEST] I\n");
        imprimere("\n--- Probans I: int f(void) { return 0; } ---\n");

        nodus = _parsare_decl(piscina, intern, "int f(void) { return 0; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        CREDO_NON_NIHIL(nodus->datum.definitio_functi.specifier);
        CREDO_NON_NIHIL(nodus->datum.definitio_functi.declarator);
        CREDO_NON_NIHIL(nodus->datum.definitio_functi.corpus);
    }

    /* ========================================================
     * PROBARE II: Functi cum parametris - int add(int a, int b) { return a + b; }
     * ======================================================== */
    {
        imprimere("\n--- Probans II: int add(int a, int b) { return a + b; } ---\n");

        nodus = _parsare_decl(piscina, intern,
            "int add(int a, int b) { return a + b; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        /* Declarator debet esse DECLARATOR_FUNCTI */
        {
            Arbor2Nodus* decl = nodus->datum.definitio_functi.declarator;
            CREDO_NON_NIHIL(decl);
        }
        /* Corpus debet esse CORPUS */
        CREDO_AEQUALIS_S32(
            (s32)nodus->datum.definitio_functi.corpus->genus,
            (s32)ARBOR2_NODUS_CORPUS);
    }

    /* ========================================================
     * PROBARE III: void noop(void) { }
     * ======================================================== */
    {
        imprimere("\n--- Probans III: void noop(void) { } ---\n");

        nodus = _parsare_decl(piscina, intern, "void noop(void) { }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
    }

    /* ========================================================
     * PROBARE IV: Functi cum pointer - int *get_ptr(void) { return 0; }
     * ======================================================== */
    {
        imprimere("\n--- Probans IV: int *get_ptr(void) { return 0; } ---\n");

        nodus = _parsare_decl(piscina, intern,
            "int *get_ptr(void) { return 0; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
    }

    /* ========================================================
     * PROBARE V: Functi cum corpore complexo
     * ======================================================== */
    {
        imprimere("\n--- Probans V: functi cum corpore complexo ---\n");

        nodus = _parsare_decl(piscina, intern,
            "int factorial(int n) { if (n <= 1) return 1; return n; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        /* Corpus debet habere sententias */
        {
            Arbor2Nodus* corpus = nodus->datum.definitio_functi.corpus;
            CREDO_NON_NIHIL(corpus);
            CREDO_NON_NIHIL(corpus->datum.corpus.sententiae);
        }
    }

    /* ========================================================
     * PROBARE VI: Translatio - int x; int y;
     * ======================================================== */
    {
        fprintf(stderr, "[TEST] VI\n");
        imprimere("\n--- Probans VI: int x; int y; ---\n");

        nodus = _parsare_trans(piscina, intern, "int x; int y;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_NON_NIHIL(nodus->datum.translation_unit.declarationes);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
    }

    /* ========================================================
     * PROBARE VII: Translatio - functi + declaratio
     * ======================================================== */
    {
        imprimere("\n--- Probans VII: int f(void) { return 0; } int main(void) { return f(); } ---\n");

        nodus = _parsare_trans(piscina, intern,
            "int f(void) { return 0; } int main(void) { return f(); }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
        /* Utraque debet esse DEFINITIO_FUNCTI */
        {
            Arbor2Nodus** elem0 = (Arbor2Nodus**)xar_obtinere(
                nodus->datum.translation_unit.declarationes, ZEPHYRUM);
            Arbor2Nodus** elem1 = (Arbor2Nodus**)xar_obtinere(
                nodus->datum.translation_unit.declarationes, I);
            CREDO_NON_NIHIL(elem0);
            CREDO_NON_NIHIL(elem1);
            CREDO_AEQUALIS_S32((s32)(*elem0)->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            CREDO_AEQUALIS_S32((s32)(*elem1)->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        }
    }

    /* ========================================================
     * PROBARE VIII: Translatio - tres declarationes
     * ======================================================== */
    {
        imprimere("\n--- Probans VIII: int a; int b; int c; ---\n");

        nodus = _parsare_trans(piscina, intern, "int a; int b; int c;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), III);
    }

    /* ========================================================
     * PROBARE IX: Translatio - mixta (decl + functi)
     * ======================================================== */
    {
        imprimere("\n--- Probans IX: int x; void f(void) { } ---\n");

        nodus = _parsare_trans(piscina, intern,
            "int x; void f(void) { }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        {
            Arbor2Nodus** elem0 = (Arbor2Nodus**)xar_obtinere(
                nodus->datum.translation_unit.declarationes, ZEPHYRUM);
            Arbor2Nodus** elem1 = (Arbor2Nodus**)xar_obtinere(
                nodus->datum.translation_unit.declarationes, I);
            CREDO_NON_NIHIL(elem0);
            CREDO_NON_NIHIL(elem1);
            CREDO_AEQUALIS_S32((s32)(*elem0)->genus, (s32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_S32((s32)(*elem1)->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        }
    }

    /* ========================================================
     * PROBARE X: Typedef simplex - typedef int my_int; my_int x;
     * ======================================================== */
    {
        imprimere("\n--- Probans X: typedef int my_int; my_int x; ---\n");

        nodus = _parsare_trans(piscina, intern,
            "typedef int my_int; my_int x;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
        /* Secundum debet esse declaratio cum my_int ut specifier */
        {
            Arbor2Nodus** elem1 = (Arbor2Nodus**)xar_obtinere(
                nodus->datum.translation_unit.declarationes, I);
            CREDO_NON_NIHIL(elem1);
            CREDO_AEQUALIS_S32((s32)(*elem1)->genus, (s32)ARBOR2_NODUS_DECLARATIO);
        }
    }

    /* ========================================================
     * PROBARE XI: Typedef unsigned long
     * ======================================================== */
    {
        imprimere("\n--- Probans XI: typedef unsigned long size_t; size_t n; ---\n");

        nodus = _parsare_trans(piscina, intern,
            "typedef unsigned long size_t; size_t n;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
    }

    /* ========================================================
     * PROBARE XII: Typedef struct
     * ======================================================== */
    {
        imprimere("\n--- Probans XII: typedef struct Foo { int x; } Foo_t; Foo_t f; ---\n");

        nodus = _parsare_trans(piscina, intern,
            "typedef struct Foo { int x; } Foo_t; Foo_t f;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
    }

    /* ========================================================
     * PROBARE XIII: Typedef in functione
     * ======================================================== */
    {
        imprimere("\n--- Probans XIII: typedef int i32; i32 add(i32 a, i32 b) { return a + b; } ---\n");

        nodus = _parsare_trans(piscina, intern,
            "typedef int i32; i32 add(i32 a, i32 b) { return a + b; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), II);
        /* Secundum debet esse DEFINITIO_FUNCTI */
        {
            Arbor2Nodus** elem1 = (Arbor2Nodus**)xar_obtinere(
                nodus->datum.translation_unit.declarationes, I);
            CREDO_NON_NIHIL(elem1);
            CREDO_AEQUALIS_S32((s32)(*elem1)->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        }
    }

    /* ========================================================
     * PROBARE XIV: Programma completum
     * ======================================================== */
    {
        imprimere("\n--- Probans XIV: programma completum ---\n");

        nodus = _parsare_trans(piscina, intern,
            "typedef int i32;"
            " i32 add(i32 a, i32 b) { return a + b; }"
            " i32 main(void) { i32 x; x = add(1, 2); return x; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), III);
    }

    /* ========================================================
     * PROBARE XV: Typedef cum typedef (typedef size_t my_size_t;)
     * ======================================================== */
    {
        imprimere("\n--- Probans XV: typedef chaining ---\n");

        nodus = _parsare_trans(piscina, intern,
            "typedef int base_t; typedef base_t derived_t; derived_t v;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.translation_unit.declarationes), III);
    }

    /* ========================================================
     * PROBARE XVI: Functi cum static
     * ======================================================== */
    {
        imprimere("\n--- Probans XVI: static int helper(void) { return 0; } ---\n");

        nodus = _parsare_decl(piscina, intern,
            "static int helper(void) { return 0; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
    }

    /* ========================================================
     * PROBARE XVII: Functi void sine parametris
     * ======================================================== */
    {
        imprimere("\n--- Probans XVII: void setup(void) { int x; x = 0; } ---\n");

        nodus = _parsare_decl(piscina, intern,
            "void setup(void) { int x; x = 0; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        /* Corpus debet habere 2 sententias */
        {
            Arbor2Nodus* corpus = nodus->datum.definitio_functi.corpus;
            CREDO_NON_NIHIL(corpus->datum.corpus.sententiae);
            CREDO_AEQUALIS_I32(
                (i32)xar_numerus(corpus->datum.corpus.sententiae), II);
        }
    }

    /* ========================================================
     * PROBARE XVIII: Translatio - singula declaratio (non translation_unit)
     * ======================================================== */
    {
        imprimere("\n--- Probans XVIII: singula declaratio per translationem ---\n");

        nodus = _parsare_trans(piscina, intern, "int x;");
        CREDO_NON_NIHIL(nodus);
        /* Singula declaratio potest esse DECLARATIO vel TRANSLATION_UNIT */
        CREDO_VERUM(
            (s32)nodus->genus == (s32)ARBOR2_NODUS_DECLARATIO ||
            (s32)nodus->genus == (s32)ARBOR2_NODUS_TRANSLATION_UNIT);
    }

    /* ========================================================
     * PROBARE XIX: Externa typedef nomina
     * ======================================================== */
    {
        constans character* ext[] = {"size_t", "ptrdiff_t"};
        Arbor2Lexator* lexator;
        Xar* lexemata;

        imprimere("\n--- Probans XIX: externa typedef nomina ---\n");

        /* Lexere et praescandere */
        lexator = arbor2_lexator_creare(piscina, intern,
            "size_t n; ptrdiff_t d;", XXII);
        CREDO_NON_NIHIL(lexator);

        lexemata = arbor2_lexema_omnia(lexator);
        CREDO_NON_NIHIL(lexemata);

        lapifex_c89_typedef_praescandere(lexemata, intern, ext, II);

        /* Verificare quod size_t est remappatum */
        {
            Arbor2Lexema** lex0 = (Arbor2Lexema**)xar_obtinere(lexemata, ZEPHYRUM);
            CREDO_NON_NIHIL(lex0);
            CREDO_AEQUALIS_S32((s32)(*lex0)->genus,
                (s32)ARBOR2_LEXEMA_NOMEN_TYPUS);
        }
    }

    /* ========================================================
     * PROBARE XX: Functi cum multis specifieribus
     * ======================================================== */
    {
        imprimere("\n--- Probans XX: unsigned long f(void) { return 0; } ---\n");

        nodus = _parsare_decl(piscina, intern,
            "unsigned long f(void) { return 0; }");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
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
