/* probatio_lapifex_c89_decl.c - Probationes Declarationum C89 via Lapifex GLR
 *
 * Probat parsationem declarationum C89 per pipeline:
 *   lexer -> adaptator -> GLR parser -> AST
 *
 * Verificat structuram AST (genera nodorum, declaratores, specifiers).
 */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "lapifex_c89.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ================================================
 * Auxiliaris: parsare declarationem
 * ================================================ */

hic_manens Arbor2Nodus*
_parsare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons)
{
    redde lapifex_c89_declarationem_parsare(
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

    piscina = piscina_generare_dynamicum("probatio_lapifex_c89_decl", MMMMXCVI * IV);
    si (!piscina)
    {
        imprimere("FRACTA: piscina non creata\n");
        redde I;
    }

    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE I: Simplex declaratio - int x;
     * ======================================================== */
    {
        imprimere("\n--- Probans I: int x; ---\n");

        nodus = _parsare(piscina, intern, "int x;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
        CREDO_NON_NIHIL(nodus->datum.declaratio.specifier);
        CREDO_NON_NIHIL(nodus->datum.declaratio.declarator);
        CREDO_NON_NIHIL(nodus->datum.declaratio.tok_semicolon);
    }

    /* ========================================================
     * PROBARE II: char c;
     * ======================================================== */
    {
        imprimere("\n--- Probans II: char c; ---\n");

        nodus = _parsare(piscina, intern, "char c;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE III: void *p;
     * ======================================================== */
    {
        Arbor2Nodus* decl;

        imprimere("\n--- Probans III: void *p; ---\n");

        nodus = _parsare(piscina, intern, "void *p;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        decl = nodus->datum.declaratio.declarator;
        CREDO_NON_NIHIL(decl);
        CREDO_AEQUALIS_S32((s32)decl->genus, (s32)ARBOR2_NODUS_DECLARATOR);
        /* Debet habere pointer_levels */
        CREDO_NON_NIHIL(decl->datum.declarator.pointer_levels);
        CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.pointer_levels), I);
    }

    /* ========================================================
     * PROBARE IV: int x = 42;
     * ======================================================== */
    {
        imprimere("\n--- Probans IV: int x = 42; ---\n");

        nodus = _parsare(piscina, intern, "int x = 42;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
        /* Init declarator debet habere initializor */
        CREDO_NON_NIHIL(nodus->datum.declaratio.declarator);
    }

    /* ========================================================
     * PROBARE V: int a, b, c;
     * ======================================================== */
    {
        imprimere("\n--- Probans V: int a, b, c; ---\n");

        nodus = _parsare(piscina, intern, "int a, b, c;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
        /* Debet habere proxima (LISTA_SEPARATA) */
        CREDO_NON_NIHIL(nodus->datum.declaratio.proxima);
        CREDO_AEQUALIS_S32((s32)nodus->datum.declaratio.proxima->genus,
            (s32)ARBOR2_NODUS_LISTA_SEPARATA);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.declaratio.proxima->datum.lista_separata.elementa),
            III);
    }

    /* ========================================================
     * PROBARE VI: int *p;
     * ======================================================== */
    {
        Arbor2Nodus* decl;

        imprimere("\n--- Probans VI: int *p; ---\n");

        nodus = _parsare(piscina, intern, "int *p;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        decl = nodus->datum.declaratio.declarator;
        CREDO_NON_NIHIL(decl);
        CREDO_AEQUALIS_S32((s32)decl->genus, (s32)ARBOR2_NODUS_DECLARATOR);
        CREDO_NON_NIHIL(decl->datum.declarator.pointer_levels);
        CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.pointer_levels), I);
    }

    /* ========================================================
     * PROBARE VII: int **pp;
     * ======================================================== */
    {
        Arbor2Nodus* decl;

        imprimere("\n--- Probans VII: int **pp; ---\n");

        nodus = _parsare(piscina, intern, "int **pp;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        decl = nodus->datum.declaratio.declarator;
        CREDO_NON_NIHIL(decl);
        CREDO_NON_NIHIL(decl->datum.declarator.pointer_levels);
        CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.pointer_levels), II);
    }

    /* ========================================================
     * PROBARE VIII: const int *p;
     * ======================================================== */
    {
        imprimere("\n--- Probans VIII: const int *p; ---\n");

        nodus = _parsare(piscina, intern, "const int *p;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE IX: int arr[10];
     * ======================================================== */
    {
        Arbor2Nodus* decl;

        imprimere("\n--- Probans IX: int arr[10]; ---\n");

        nodus = _parsare(piscina, intern, "int arr[10];");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        decl = nodus->datum.declaratio.declarator;
        CREDO_NON_NIHIL(decl);
        CREDO_AEQUALIS_S32((s32)decl->genus, (s32)ARBOR2_NODUS_DECLARATOR);
        CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
        CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.dimensiones), I);
    }

    /* ========================================================
     * PROBARE X: int arr[];
     * ======================================================== */
    {
        Arbor2Nodus* decl;

        imprimere("\n--- Probans X: int arr[]; ---\n");

        nodus = _parsare(piscina, intern, "int arr[];");
        CREDO_NON_NIHIL(nodus);

        decl = nodus->datum.declaratio.declarator;
        CREDO_NON_NIHIL(decl);
        CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
        CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.dimensiones), I);
        {
            Arbor2ArrayDimension** dim = (Arbor2ArrayDimension**)xar_obtinere(
                decl->datum.declarator.dimensiones, ZEPHYRUM);
            CREDO_NIHIL((*dim)->dimensio); /* Unsized */
        }
    }

    /* ========================================================
     * PROBARE XI: int f(void);
     * ======================================================== */
    {
        Arbor2Nodus* decl;

        imprimere("\n--- Probans XI: int f(void); ---\n");

        nodus = _parsare(piscina, intern, "int f(void);");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        decl = nodus->datum.declaratio.declarator;
        CREDO_NON_NIHIL(decl);
        CREDO_AEQUALIS_S32((s32)decl->genus, (s32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
        CREDO_VERUM(decl->datum.declarator_functi.habet_void);
        CREDO_FALSUM(decl->datum.declarator_functi.est_variadicus);
    }

    /* ========================================================
     * PROBARE XII: int f(int a, char b);
     * ======================================================== */
    {
        Arbor2Nodus* decl;

        imprimere("\n--- Probans XII: int f(int a, char b); ---\n");

        nodus = _parsare(piscina, intern, "int f(int a, char b);");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        decl = nodus->datum.declaratio.declarator;
        CREDO_NON_NIHIL(decl);
        CREDO_AEQUALIS_S32((s32)decl->genus, (s32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
        CREDO_NON_NIHIL(decl->datum.declarator_functi.parametri);
        CREDO_FALSUM(decl->datum.declarator_functi.habet_void);
    }

    /* ========================================================
     * PROBARE XIII: int (*fp)(int);
     * ======================================================== */
    {
        imprimere("\n--- Probans XIII: int (*fp)(int); ---\n");

        nodus = _parsare(piscina, intern, "int (*fp)(int);");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XIV: void f(int, ...);
     * ======================================================== */
    {
        Arbor2Nodus* decl;

        imprimere("\n--- Probans XIV: void f(int, ...); ---\n");

        nodus = _parsare(piscina, intern, "void f(int, ...);");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        decl = nodus->datum.declaratio.declarator;
        CREDO_NON_NIHIL(decl);
        CREDO_AEQUALIS_S32((s32)decl->genus, (s32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
        CREDO_VERUM(decl->datum.declarator_functi.est_variadicus);
    }

    /* ========================================================
     * PROBARE XV: struct Foo { int x; char c; };
     * ======================================================== */
    {
        imprimere("\n--- Probans XV: struct Foo { int x; char c; }; ---\n");

        nodus = _parsare(piscina, intern, "struct Foo { int x; char c; };");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
        /* Specifier debet esse STRUCT_SPECIFIER */
        CREDO_NON_NIHIL(nodus->datum.declaratio.specifier);
        CREDO_AEQUALIS_S32((s32)nodus->datum.declaratio.specifier->genus,
            (s32)ARBOR2_NODUS_STRUCT_SPECIFIER);
        CREDO_NON_NIHIL(nodus->datum.declaratio.specifier->datum.struct_specifier.membra);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(nodus->datum.declaratio.specifier->datum.struct_specifier.membra),
            II);
    }

    /* ========================================================
     * PROBARE XVI: enum Color { RED, GREEN, BLUE };
     * ======================================================== */
    {
        Arbor2Nodus* spec;

        imprimere("\n--- Probans XVI: enum Color { RED, GREEN, BLUE }; ---\n");

        nodus = _parsare(piscina, intern, "enum Color { RED, GREEN, BLUE };");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        spec = nodus->datum.declaratio.specifier;
        CREDO_NON_NIHIL(spec);
        CREDO_AEQUALIS_S32((s32)spec->genus, (s32)ARBOR2_NODUS_ENUM_SPECIFIER);
        CREDO_NON_NIHIL(spec->datum.enum_specifier.enumeratores);
        /* LISTA_SEPARATA cum III elementis */
        CREDO_AEQUALIS_S32((s32)spec->datum.enum_specifier.enumeratores->genus,
            (s32)ARBOR2_NODUS_LISTA_SEPARATA);
        CREDO_AEQUALIS_I32(
            (i32)xar_numerus(spec->datum.enum_specifier.enumeratores->datum.lista_separata.elementa),
            III);
    }

    /* ========================================================
     * PROBARE XVII: int x = 2 + 3;
     * ======================================================== */
    {
        imprimere("\n--- Probans XVII: int x = 2 + 3; ---\n");

        nodus = _parsare(piscina, intern, "int x = 2 + 3;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XVIII: int arr[] = {1, 2, 3};
     * ======================================================== */
    {
        imprimere("\n--- Probans XVIII: int arr[] = {1, 2, 3}; ---\n");

        nodus = _parsare(piscina, intern, "int arr[] = {1, 2, 3};");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XIX: int a = 1, b = 2;
     * ======================================================== */
    {
        imprimere("\n--- Probans XIX: int a = 1, b = 2; ---\n");

        nodus = _parsare(piscina, intern, "int a = 1, b = 2;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XX: struct Point { int x; int y; } origin = {0, 0};
     * ======================================================== */
    {
        imprimere("\n--- Probans XX: struct + init ---\n");

        nodus = _parsare(piscina, intern,
            "struct Point { int x; int y; } origin = {0, 0};");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XXI: typedef unsigned long size_t;
     * ======================================================== */
    {
        imprimere("\n--- Probans XXI: typedef unsigned long size_t; ---\n");

        nodus = _parsare(piscina, intern, "typedef unsigned long size_t;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XXII: static const int MAX = 100;
     * ======================================================== */
    {
        imprimere("\n--- Probans XXII: static const int MAX = 100; ---\n");

        nodus = _parsare(piscina, intern, "static const int MAX = 100;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XXIII: Expressio regression - 2+3
     * ======================================================== */
    {
        imprimere("\n--- Probans XXIII: Expressio regressio 2+3 ---\n");

        nodus = _parsare(piscina, intern, "2+3");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_BINARIUM);
        CREDO_AEQUALIS_S32((s32)nodus->datum.binarium.operator,
            (s32)ARBOR2_LEXEMA_PLUS);
    }

    /* ========================================================
     * PROBARE XXIV: int f();
     * ======================================================== */
    {
        Arbor2Nodus* decl;

        imprimere("\n--- Probans XXIV: int f(); ---\n");

        nodus = _parsare(piscina, intern, "int f();");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        decl = nodus->datum.declaratio.declarator;
        CREDO_NON_NIHIL(decl);
        CREDO_AEQUALIS_S32((s32)decl->genus, (s32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
        CREDO_FALSUM(decl->datum.declarator_functi.habet_void);
    }

    /* ========================================================
     * PROBARE XXV: enum with values - enum E { A = 1, B = 2 };
     * ======================================================== */
    {
        imprimere("\n--- Probans XXV: enum E { A = 1, B = 2 }; ---\n");

        nodus = _parsare(piscina, intern, "enum E { A = 1, B = 2 };");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XXVI: Forward declaration - struct Foo;
     * ======================================================== */
    {
        imprimere("\n--- Probans XXVI: struct Foo; ---\n");

        nodus = _parsare(piscina, intern, "struct Foo;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XXVII: Trailing comma initializer - int a[] = {1, 2,};
     * ======================================================== */
    {
        imprimere("\n--- Probans XXVII: int a[] = {1, 2,}; ---\n");

        nodus = _parsare(piscina, intern, "int a[] = {1, 2,};");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XXVIII: extern int x;
     * ======================================================== */
    {
        imprimere("\n--- Probans XXVIII: extern int x; ---\n");

        nodus = _parsare(piscina, intern, "extern int x;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE XXIX: union U { int i; float f; };
     * ======================================================== */
    {
        Arbor2Nodus* spec;

        imprimere("\n--- Probans XXIX: union U { int i; float f; }; ---\n");

        nodus = _parsare(piscina, intern, "union U { int i; float f; };");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);

        spec = nodus->datum.declaratio.specifier;
        CREDO_NON_NIHIL(spec);
        CREDO_AEQUALIS_S32((s32)spec->genus, (s32)ARBOR2_NODUS_STRUCT_SPECIFIER);
        CREDO_VERUM(spec->datum.struct_specifier.est_unio);
    }

    /* ========================================================
     * PROBARE XXX: unsigned long long x;
     * ======================================================== */
    {
        imprimere("\n--- Probans XXX: unsigned long long x; ---\n");

        nodus = _parsare(piscina, intern, "unsigned long long x;");
        CREDO_NON_NIHIL(nodus);
        CREDO_AEQUALIS_S32((s32)nodus->genus, (s32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
