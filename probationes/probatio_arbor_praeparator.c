/* ==================================================
 * PROBATIO ARBOR PRAEPARATOR - Unit Tests pro C89 Preprocessor
 * ================================================== */

#include "credo.h"
#include "arbor_praeparator.h"
#include "arbor_lexema.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Test: Creatio Praeparatoris
 * ================================================== */

interior vacuum
test_creare(Piscina* piscina, InternamentumChorda* intern)
{
    ArborPraeparator* pp;

    imprimere("\n--- Creatio ---\n");

    pp = arbor_praeparator_creare(piscina, intern);
    CREDO_NON_NIHIL(pp);

    imprimere("  Creare: OK\n");
}

/* ==================================================
 * Test: Definire Macros
 * ================================================== */

interior vacuum
test_definire(Piscina* piscina, InternamentumChorda* intern)
{
    ArborPraeparator* pp;
    chorda* titulus_macro;
    ArborMacroDefinitio* macro;

    imprimere("\n--- Definire Macros ---\n");

    pp = arbor_praeparator_creare(piscina, intern);
    CREDO_NON_NIHIL(pp);

    /* Definire macro simplex */
    arbor_praeparator_definire(pp, "DEBUG", "1");

    /* Verificare definitum */
    titulus_macro = chorda_internare_ex_literis(intern, "DEBUG");
    CREDO_NON_NIHIL(titulus_macro);
    CREDO_VERUM(arbor_praeparator_est_definitum(pp, titulus_macro));

    /* Obtinere macro */
    macro = arbor_praeparator_obtinere_macro(pp, titulus_macro);
    CREDO_NON_NIHIL(macro);
    CREDO_FALSUM(macro->est_functio);
    CREDO_FALSUM(macro->est_variadic);

    /* Verificare corpus habet lexema "1" */
    CREDO_NON_NIHIL(macro->corpus);
    CREDO_AEQUALIS_I32(xar_numerus(macro->corpus), I);

    imprimere("  Definire simplex: OK\n");

    /* Definire macro vacua */
    arbor_praeparator_definire(pp, "EMPTY", NIHIL);
    titulus_macro = chorda_internare_ex_literis(intern, "EMPTY");
    CREDO_VERUM(arbor_praeparator_est_definitum(pp, titulus_macro));
    macro = arbor_praeparator_obtinere_macro(pp, titulus_macro);
    CREDO_NON_NIHIL(macro);
    CREDO_AEQUALIS_I32(xar_numerus(macro->corpus), ZEPHYRUM);

    imprimere("  Definire vacua: OK\n");

    /* Definire macro cum valore multi-token */
    arbor_praeparator_definire(pp, "VERSION", "1 + 2");
    titulus_macro = chorda_internare_ex_literis(intern, "VERSION");
    macro = arbor_praeparator_obtinere_macro(pp, titulus_macro);
    CREDO_NON_NIHIL(macro);
    CREDO_AEQUALIS_I32(xar_numerus(macro->corpus), III);

    imprimere("  Definire multi-token: OK\n");
}

/* ==================================================
 * Test: Undefinire Macros
 * ================================================== */

interior vacuum
test_undefinire(Piscina* piscina, InternamentumChorda* intern)
{
    ArborPraeparator* pp;
    chorda* titulus_macro;

    imprimere("\n--- Undefinire Macros ---\n");

    pp = arbor_praeparator_creare(piscina, intern);
    CREDO_NON_NIHIL(pp);

    /* Definire et undefinire */
    arbor_praeparator_definire(pp, "TEMP", "42");
    titulus_macro = chorda_internare_ex_literis(intern, "TEMP");
    CREDO_VERUM(arbor_praeparator_est_definitum(pp, titulus_macro));

    arbor_praeparator_undefinire(pp, "TEMP");
    CREDO_FALSUM(arbor_praeparator_est_definitum(pp, titulus_macro));

    imprimere("  Undefinire: OK\n");
}

/* ==================================================
 * Test: Addere Viae Include
 * ================================================== */

interior vacuum
test_addere_via(Piscina* piscina, InternamentumChorda* intern)
{
    ArborPraeparator* pp;

    imprimere("\n--- Addere Via Include ---\n");

    pp = arbor_praeparator_creare(piscina, intern);
    CREDO_NON_NIHIL(pp);

    /* Addere vias - non debet collabi */
    arbor_praeparator_addere_via(pp, "/usr/include");
    arbor_praeparator_addere_via(pp, "/usr/local/include");
    arbor_praeparator_addere_via(pp, ".");

    imprimere("  Addere via: OK\n");
}

/* ==================================================
 * Test: Processare Simplex (sine directivis)
 * ================================================== */

interior vacuum
test_processare_simplex(Piscina* piscina, InternamentumChorda* intern)
{
    ArborPraeparator* pp;
    ArborLexator* lexator;
    Xar* lexemata;
    Xar* fructus;
    constans character* fons;
    i32 num_out;

    imprimere("\n--- Processare Simplex ---\n");

    pp = arbor_praeparator_creare(piscina, intern);
    CREDO_NON_NIHIL(pp);

    /* Fons sine directivis */
    fons = "int x = 42;";

    lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
    CREDO_NON_NIHIL(lexator);

    lexemata = arbor_lexema_omnia(lexator);
    CREDO_NON_NIHIL(lexemata);

    fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
    CREDO_NON_NIHIL(fructus);

    /* Verificare fructus */
    num_out = xar_numerus(fructus);
    CREDO_MAIOR_I32(num_out, ZEPHYRUM);

    /* Verificare primum lexema est "int" */
    {
        ArborLexemaOrigo* lo;
        lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, ZEPHYRUM);
        CREDO_NON_NIHIL(lo);
        CREDO_NON_NIHIL(lo->lexema);
        CREDO_AEQUALIS_I32(lo->lexema->genus, ARBOR_LEXEMA_INT);
    }

    imprimere("  Processare simplex: OK\n");
}

/* ==================================================
 * Test: Directivae Saltantur (pro nunc)
 * ================================================== */

interior vacuum
test_directivae_saltantur(Piscina* piscina, InternamentumChorda* intern)
{
    ArborPraeparator* pp;
    ArborLexator* lexator;
    Xar* lexemata;
    Xar* fructus;
    constans character* fons;
    i32 i;
    i32 num_out;
    b32 invenit_hash;

    imprimere("\n--- Directivae Saltantur ---\n");

    pp = arbor_praeparator_creare(piscina, intern);
    CREDO_NON_NIHIL(pp);

    /* Fons cum directiva - directiva debet saltari */
    fons = "#define FOO 1\nint x;";

    lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
    CREDO_NON_NIHIL(lexator);

    lexemata = arbor_lexema_omnia(lexator);
    CREDO_NON_NIHIL(lexemata);

    fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
    CREDO_NON_NIHIL(fructus);

    /* Verificare output non continet # token */
    num_out = xar_numerus(fructus);
    invenit_hash = FALSUM;
    per (i = ZEPHYRUM; i < num_out; i++)
    {
        ArborLexemaOrigo* lo;
        lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
        si (lo != NIHIL && lo->lexema != NIHIL)
        {
            si (lo->lexema->genus == ARBOR_LEXEMA_HASH)
            {
                invenit_hash = VERUM;
            }
        }
    }
    CREDO_FALSUM(invenit_hash);

    /* Verificare "int" est in output */
    {
        ArborLexemaOrigo* lo;
        lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, ZEPHYRUM);
        CREDO_NON_NIHIL(lo);
        CREDO_NON_NIHIL(lo->lexema);
        CREDO_AEQUALIS_I32(lo->lexema->genus, ARBOR_LEXEMA_INT);
    }

    imprimere("  Directivae saltantur: OK\n");
}

/* ==================================================
 * Test: Parsere #define via Processare
 * ================================================== */

interior vacuum
test_parsere_define(Piscina* piscina, InternamentumChorda* intern)
{
    ArborPraeparator* pp;
    ArborLexator* lexator;
    Xar* lexemata;
    Xar* fructus;
    ArborMacroDefinitio* macro;
    chorda* titulus;

    imprimere("\n--- Parsere #define ---\n");

    /* Test 1: Object-like macro simplex */
    {
        constans character* fons = "#define FOO 42\nint x;";

        pp = arbor_praeparator_creare(piscina, intern);
        CREDO_NON_NIHIL(pp);

        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* Verificare macro FOO est definita */
        titulus = chorda_internare_ex_literis(intern, "FOO");
        CREDO_VERUM(arbor_praeparator_est_definitum(pp, titulus));

        macro = arbor_praeparator_obtinere_macro(pp, titulus);
        CREDO_NON_NIHIL(macro);
        CREDO_FALSUM(macro->est_functio);
        CREDO_AEQUALIS_I32(xar_numerus(macro->corpus), I);

        imprimere("  Object-like simplex: OK\n");
    }

    /* Test 2: Object-like macro multi-token */
    {
        constans character* fons = "#define EXPR 1 + 2 * 3\nint y;";

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");

        titulus = chorda_internare_ex_literis(intern, "EXPR");
        macro = arbor_praeparator_obtinere_macro(pp, titulus);
        CREDO_NON_NIHIL(macro);
        CREDO_FALSUM(macro->est_functio);
        /* 1 + 2 * 3 = 5 tokens */
        CREDO_AEQUALIS_I32(xar_numerus(macro->corpus), V);

        imprimere("  Object-like multi-token: OK\n");
    }

    /* Test 3: Object-like macro vacua */
    {
        constans character* fons = "#define EMPTY\nint z;";

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");

        titulus = chorda_internare_ex_literis(intern, "EMPTY");
        macro = arbor_praeparator_obtinere_macro(pp, titulus);
        CREDO_NON_NIHIL(macro);
        CREDO_FALSUM(macro->est_functio);
        CREDO_AEQUALIS_I32(xar_numerus(macro->corpus), ZEPHYRUM);

        imprimere("  Object-like vacua: OK\n");
    }

    /* Test 4: Function-like macro cum uno parametro */
    {
        constans character* fons = "#define SQUARE(x) x * x\nint a;";

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");

        titulus = chorda_internare_ex_literis(intern, "SQUARE");
        macro = arbor_praeparator_obtinere_macro(pp, titulus);
        CREDO_NON_NIHIL(macro);
        CREDO_VERUM(macro->est_functio);
        CREDO_FALSUM(macro->est_variadic);
        CREDO_NON_NIHIL(macro->parametra);
        CREDO_AEQUALIS_I32(xar_numerus(macro->parametra), I);
        /* x * x = 3 tokens */
        CREDO_AEQUALIS_I32(xar_numerus(macro->corpus), III);

        imprimere("  Function-like unum param: OK\n");
    }

    /* Test 5: Function-like macro cum multis parametris */
    {
        constans character* fons = "#define ADD(a, b) a + b\nint c;";

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");

        titulus = chorda_internare_ex_literis(intern, "ADD");
        macro = arbor_praeparator_obtinere_macro(pp, titulus);
        CREDO_NON_NIHIL(macro);
        CREDO_VERUM(macro->est_functio);
        CREDO_AEQUALIS_I32(xar_numerus(macro->parametra), II);
        /* a + b = 3 tokens */
        CREDO_AEQUALIS_I32(xar_numerus(macro->corpus), III);

        imprimere("  Function-like multi param: OK\n");
    }

    /* Test 6: Function-like macro variadic */
    {
        constans character* fons = "#define PRINT(fmt, ...) printf(fmt)\nint d;";

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");

        titulus = chorda_internare_ex_literis(intern, "PRINT");
        macro = arbor_praeparator_obtinere_macro(pp, titulus);
        CREDO_NON_NIHIL(macro);
        CREDO_VERUM(macro->est_functio);
        CREDO_VERUM(macro->est_variadic);
        /* fmt param */
        CREDO_AEQUALIS_I32(xar_numerus(macro->parametra), I);

        imprimere("  Function-like variadic: OK\n");
    }

    /* Test 7: #undef via processare */
    {
        constans character* fons = "#define BAR 1\n#undef BAR\nint e;";

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");

        titulus = chorda_internare_ex_literis(intern, "BAR");
        CREDO_FALSUM(arbor_praeparator_est_definitum(pp, titulus));

        imprimere("  Undef via processare: OK\n");
    }
}

/* ==================================================
 * Test: Conditionales (#ifdef/#ifndef/#else/#endif)
 * ================================================== */

interior vacuum
test_conditionales(Piscina* piscina, InternamentumChorda* intern)
{
    ArborPraeparator* pp;
    ArborLexator* lexator;
    Xar* lexemata;
    Xar* fructus;
    i32 i;
    i32 num_out;

    imprimere("\n--- Conditionales ---\n");

    /* Test 1: #ifdef verum - emittit codicem */
    {
        constans character* fons = "#define FOO\n#ifdef FOO\nint x;\n#endif\n";
        b32 invenit_int;

        pp = arbor_praeparator_creare(piscina, intern);
        arbor_praeparator_definire(pp, "FOO", NIHIL);

        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* Verificare "int" est in output */
        num_out = xar_numerus(fructus);
        invenit_int = FALSUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_INT)
            {
                invenit_int = VERUM;
            }
        }
        CREDO_VERUM(invenit_int);

        imprimere("  ifdef verum: OK\n");
    }

    /* Test 2: #ifdef falsum - non emittit codicem */
    {
        constans character* fons = "#ifdef UNDEFINED\nint y;\n#endif\nint z;";
        b32 invenit_y;

        pp = arbor_praeparator_creare(piscina, intern);

        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* Verificare "y" NON est in output, sed "z" est */
        num_out = xar_numerus(fructus);
        invenit_y = FALSUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_IDENTIFICATOR)
            {
                si (lo->lexema->valor.mensura == I &&
                    lo->lexema->valor.datum[ZEPHYRUM] == 'y')
                {
                    invenit_y = VERUM;
                }
            }
        }
        CREDO_FALSUM(invenit_y);

        imprimere("  ifdef falsum: OK\n");
    }

    /* Test 3: #ifndef verum - emittit codicem */
    {
        constans character* fons = "#ifndef UNDEFINED\nint a;\n#endif\n";
        b32 invenit_a;

        pp = arbor_praeparator_creare(piscina, intern);

        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        num_out = xar_numerus(fructus);
        invenit_a = FALSUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_IDENTIFICATOR)
            {
                si (lo->lexema->valor.mensura == I &&
                    lo->lexema->valor.datum[ZEPHYRUM] == 'a')
                {
                    invenit_a = VERUM;
                }
            }
        }
        CREDO_VERUM(invenit_a);

        imprimere("  ifndef verum: OK\n");
    }

    /* Test 4: #ifdef cum #else */
    {
        constans character* fons = "#ifdef UNDEFINED\nint b;\n#else\nint c;\n#endif\n";
        b32 invenit_b;
        b32 invenit_c;

        pp = arbor_praeparator_creare(piscina, intern);

        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        num_out = xar_numerus(fructus);
        invenit_b = FALSUM;
        invenit_c = FALSUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_IDENTIFICATOR)
            {
                si (lo->lexema->valor.mensura == I)
                {
                    si (lo->lexema->valor.datum[ZEPHYRUM] == 'b')
                    {
                        invenit_b = VERUM;
                    }
                    si (lo->lexema->valor.datum[ZEPHYRUM] == 'c')
                    {
                        invenit_c = VERUM;
                    }
                }
            }
        }
        CREDO_FALSUM(invenit_b);
        CREDO_VERUM(invenit_c);

        imprimere("  ifdef cum else: OK\n");
    }

    /* Test 5: Conditionales nidificatae */
    {
        constans character* fons =
            "#define OUTER\n"
            "#ifdef OUTER\n"
            "  #ifdef INNER\n"
            "    int d;\n"
            "  #else\n"
            "    int e;\n"
            "  #endif\n"
            "#endif\n";
        b32 invenit_d;
        b32 invenit_e;

        pp = arbor_praeparator_creare(piscina, intern);

        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* OUTER definita, INNER non definita */
        /* Ergo: d non emittitur, e emittitur */
        num_out = xar_numerus(fructus);
        invenit_d = FALSUM;
        invenit_e = FALSUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_IDENTIFICATOR)
            {
                si (lo->lexema->valor.mensura == I)
                {
                    si (lo->lexema->valor.datum[ZEPHYRUM] == 'd')
                    {
                        invenit_d = VERUM;
                    }
                    si (lo->lexema->valor.datum[ZEPHYRUM] == 'e')
                    {
                        invenit_e = VERUM;
                    }
                }
            }
        }
        CREDO_FALSUM(invenit_d);
        CREDO_VERUM(invenit_e);

        imprimere("  conditionales nidificatae: OK\n");
    }

    /* Test 6: Include guard pattern */
    {
        constans character* fons =
            "#ifndef HEADER_H\n"
            "#define HEADER_H\n"
            "int f;\n"
            "#endif\n";
        b32 invenit_f;
        chorda* titulus;

        pp = arbor_praeparator_creare(piscina, intern);

        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* Verificare HEADER_H est definita */
        titulus = chorda_internare_ex_literis(intern, "HEADER_H");
        CREDO_VERUM(arbor_praeparator_est_definitum(pp, titulus));

        /* Verificare f est in output */
        num_out = xar_numerus(fructus);
        invenit_f = FALSUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_IDENTIFICATOR)
            {
                si (lo->lexema->valor.mensura == I &&
                    lo->lexema->valor.datum[ZEPHYRUM] == 'f')
                {
                    invenit_f = VERUM;
                }
            }
        }
        CREDO_VERUM(invenit_f);

        imprimere("  include guard pattern: OK\n");
    }
}

/* ==================================================
 * Test: Macro Expansion
 * ================================================== */

interior vacuum
test_macro_expansion(Piscina* piscina, InternamentumChorda* intern)
{
    ArborPraeparator* pp;
    ArborLexator* lexator;
    Xar* lexemata;
    Xar* fructus;
    i32 i;
    i32 num_out;

    imprimere("\n--- Macro Expansion ---\n");

    /* Test 1: Simple object-like macro expansion */
    {
        constans character* fons = "#define VALUE 42\nint x = VALUE;";
        b32 invenit_42;

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* Verificare 42 est in output (VALUE expanditur) */
        num_out = xar_numerus(fructus);
        invenit_42 = FALSUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_INTEGER)
            {
                si (lo->lexema->valor.mensura == II &&
                    lo->lexema->valor.datum[ZEPHYRUM] == '4' &&
                    lo->lexema->valor.datum[I] == '2')
                {
                    invenit_42 = VERUM;
                }
            }
        }
        CREDO_VERUM(invenit_42);

        imprimere("  Simplex expansion: OK\n");
    }

    /* Test 2: Multi-token macro expansion */
    {
        constans character* fons = "#define EXPR 1 + 2\nint x = EXPR;";
        i32 num_operators;

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* Verificare + est in output */
        num_out = xar_numerus(fructus);
        num_operators = ZEPHYRUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_PLUS)
            {
                num_operators++;
            }
        }
        CREDO_AEQUALIS_I32(num_operators, I);

        imprimere("  Multi-token expansion: OK\n");
    }

    /* Test 3: Nested macro expansion */
    {
        constans character* fons =
            "#define A 1\n"
            "#define B A\n"
            "int x = B;";
        b32 invenit_1;

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* B expands to A, A expands to 1 */
        num_out = xar_numerus(fructus);
        invenit_1 = FALSUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_INTEGER &&
                lo->lexema->valor.mensura == I &&
                lo->lexema->valor.datum[ZEPHYRUM] == '1')
            {
                invenit_1 = VERUM;
            }
        }
        CREDO_VERUM(invenit_1);

        imprimere("  Nested expansion: OK\n");
    }

    /* Test 4: Self-referential macro (recursion protection) */
    {
        constans character* fons =
            "#define FOO FOO\n"
            "int x = FOO;";
        b32 invenit_foo;

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* FOO should expand to FOO (identifier), not infinite loop */
        num_out = xar_numerus(fructus);
        invenit_foo = FALSUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_IDENTIFICATOR &&
                lo->lexema->valor.mensura == III)
            {
                si (lo->lexema->valor.datum[ZEPHYRUM] == 'F' &&
                    lo->lexema->valor.datum[I] == 'O' &&
                    lo->lexema->valor.datum[II] == 'O')
                {
                    invenit_foo = VERUM;
                }
            }
        }
        CREDO_VERUM(invenit_foo);

        imprimere("  Recursion protection: OK\n");
    }

    /* Test 5: Empty macro expansion */
    {
        constans character* fons =
            "#define EMPTY\n"
            "int EMPTY x;";
        b32 invenit_x;
        i32 count_int;

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* EMPTY expands to nothing, so we should have: int x ; */
        num_out = xar_numerus(fructus);
        invenit_x = FALSUM;
        count_int = ZEPHYRUM;
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL)
            {
                si (lo->lexema->genus == ARBOR_LEXEMA_INT)
                {
                    count_int++;
                }
                si (lo->lexema->genus == ARBOR_LEXEMA_IDENTIFICATOR &&
                    lo->lexema->valor.mensura == I &&
                    lo->lexema->valor.datum[ZEPHYRUM] == 'x')
                {
                    invenit_x = VERUM;
                }
            }
        }
        CREDO_AEQUALIS_I32(count_int, I);
        CREDO_VERUM(invenit_x);

        imprimere("  Empty expansion: OK\n");
    }

    /* Test 6: Origin tracking */
    {
        constans character* fons =
            "#define VALUE 42\n"
            "int x = VALUE;";

        pp = arbor_praeparator_creare(piscina, intern);
        lexator = arbor_lexator_creare(piscina, intern, fons, (i32)strlen(fons));
        lexemata = arbor_lexema_omnia(lexator);
        fructus = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");
        CREDO_NON_NIHIL(fructus);

        /* Find expanded token and check origo */
        num_out = xar_numerus(fructus);
        per (i = ZEPHYRUM; i < num_out; i++)
        {
            ArborLexemaOrigo* lo;
            lo = *(ArborLexemaOrigo**)xar_obtinere(fructus, i);
            si (lo != NIHIL && lo->lexema != NIHIL &&
                lo->lexema->genus == ARBOR_LEXEMA_INTEGER)
            {
                /* Expanded token should have origo */
                CREDO_NON_NIHIL(lo->origo);
                si (lo->origo != NIHIL)
                {
                    /* origo should have macro name */
                    CREDO_NON_NIHIL(lo->origo->nomen_macro);
                }
            }
        }

        imprimere("  Origin tracking: OK\n");
    }
}

/* ==================================================
 * Test: Nomen Directiva
 * ================================================== */

interior vacuum
test_directiva_nomina(vacuum)
{
    imprimere("\n--- Nomina Directivarum ---\n");

    CREDO_AEQUALIS_S32(
        strcmp(arbor_directiva_genus_nomen(ARBOR_DIRECTIVA_INCLUDE), "include"),
        ZEPHYRUM);
    CREDO_AEQUALIS_S32(
        strcmp(arbor_directiva_genus_nomen(ARBOR_DIRECTIVA_DEFINE), "define"),
        ZEPHYRUM);
    CREDO_AEQUALIS_S32(
        strcmp(arbor_directiva_genus_nomen(ARBOR_DIRECTIVA_IFDEF), "ifdef"),
        ZEPHYRUM);
    CREDO_AEQUALIS_S32(
        strcmp(arbor_directiva_genus_nomen(ARBOR_DIRECTIVA_IFNDEF), "ifndef"),
        ZEPHYRUM);
    CREDO_AEQUALIS_S32(
        strcmp(arbor_directiva_genus_nomen(ARBOR_DIRECTIVA_ENDIF), "endif"),
        ZEPHYRUM);

    imprimere("  Nomina directivarum: OK\n");
}

/* ==================================================
 * Main
 * ================================================== */

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("test_arbor_praeparator", M);
    intern = internamentum_creare(piscina);

    credo_aperire(piscina);

    imprimere("\n========================================\n");
    imprimere("PROBATIO ARBOR PRAEPARATOR\n");
    imprimere("========================================\n");

    test_creare(piscina, intern);
    test_definire(piscina, intern);
    test_undefinire(piscina, intern);
    test_addere_via(piscina, intern);
    test_processare_simplex(piscina, intern);
    test_directivae_saltantur(piscina, intern);
    test_parsere_define(piscina, intern);
    test_conditionales(piscina, intern);
    test_macro_expansion(piscina, intern);
    test_directiva_nomina();

    imprimere("\n========================================\n");
    credo_imprimere_compendium();
    imprimere("========================================\n");

    praeteritus = credo_omnia_praeterierunt();

    credo_claudere();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
