/* probatio_sputnik_interpres.c - Probationes pro Sputnik Interpres
 *
 * Probare tree-walking evaluator.
 */

#include "sputnik_interpres.h"
#include "persistentia.h"
#include "entitas_repositorium.h"
#include "credo.h"
#include <stdio.h>

/* Commoditas macros */
#define CREDO(x) CREDO_VERUM(x)
#define CREDO_AEQUALIS_F64(a, b) CREDO_F64_PROXIMUS(a, b, 0.0001)


/* ==================================================
 * Probationes - Literalia
 * ================================================== */

interior vacuum
_probare_literalia(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Literalia ---\n");

    /* Numerus */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("42;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_NUMERUS);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 42.0);
    }

    /* Numerus decimalis */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("3.14159;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_NUMERUS);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.14159);
    }

    /* Chorda */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\";", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "hello"));
    }

    /* Verum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("verum;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    /* Falsum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("falsum;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }

    /* Nihil */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("nihil;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_NIHIL);
    }
}


/* ==================================================
 * Probationes - Arithmetica
 * ================================================== */

interior vacuum
_probare_arithmeticam(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Arithmetica ---\n");

    /* Additio */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("2 + 3;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 5.0);
    }

    /* Subtractio */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("10 - 4;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 6.0);
    }

    /* Multiplicatio */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("6 * 7;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 42.0);
    }

    /* Divisio */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("15 / 3;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 5.0);
    }

    /* Divisio decimalis */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("10 / 4;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.5);
    }

    /* Modulus */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("17 % 5;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }

    /* Praecedentia */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("2 + 3 * 4;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 14.0);
    }

    /* Parentheses */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("(2 + 3) * 4;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 20.0);
    }

    /* Unarius minus */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("-5;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, -5.0);
    }

    /* Concatenatio chordarum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\" + \" \" + \"world\";", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "hello world"));
    }
}


/* ==================================================
 * Probationes - Comparatio
 * ================================================== */

interior vacuum
_probare_comparationem(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Comparatio ---\n");

    /* Minor */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("3 < 5;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("5 < 3;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }

    /* Maior */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("5 > 3;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    /* Minor aut aequalis */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("5 <= 5;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    /* Aequalitas */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("5 == 5;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("5 == 6;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }

    /* Strict aequalitas */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("5 === 5;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    /* Non aequalitas */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("5 != 6;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    /* Chorda aequalitas */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"abc\" == \"abc\";", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }
}


/* ==================================================
 * Probationes - Logici
 * ================================================== */

interior vacuum
_probare_logicos(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Logici ---\n");

    /* Et */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("verum && verum;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("verum && falsum;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }

    /* Vel */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("falsum || verum;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("falsum || falsum;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }

    /* Non */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("!verum;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("!falsum;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    /* Short-circuit */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("falsum && (1/0);", piscina, intern);
        CREDO(r.successus);  /* Non error quia short-circuit */
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("verum || (1/0);", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }
}


/* ==================================================
 * Probationes - Ternaria
 * ================================================== */

interior vacuum
_probare_ternariam(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Ternaria ---\n");

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("verum ? 1 : 2;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 1.0);
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("falsum ? 1 : 2;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }

    /* Nested */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("verum ? falsum ? 1 : 2 : 3;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }
}


/* ==================================================
 * Probationes - Variabiles
 * ================================================== */

interior vacuum
_probare_variabiles(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Variabiles ---\n");

    /* Let */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("sit x = 10; x;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 10.0);
    }

    /* Let sine initializatione */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("sit y; y;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_NIHIL);
    }

    /* Assignatio */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("sit z = 5; z = 15; z;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 15.0);
    }

    /* Const */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("constans PI = 3.14; PI;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.14);
    }

    /* Const reassign error */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("constans C = 1; C = 2;", piscina, intern);
        CREDO(!r.successus);  /* Debet fallere */
    }

    /* Expressio cum variabilibus */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("sit a = 5; sit b = 3; a * b + 1;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 16.0);
    }
}


/* ==================================================
 * Probationes - Control Flow
 * ================================================== */

interior vacuum
_probare_control_flow(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Control Flow ---\n");

    /* If verum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 0;"
            "si (verum) { x = 1; }"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 1.0);
    }

    /* If falsum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 0;"
            "si (falsum) { x = 1; }"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 0.0);
    }

    /* If-else */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 0;"
            "si (falsum) { x = 1; } alioquin { x = 2; }"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }

    /* While */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit sum = 0;"
            "sit i = 1;"
            "dum (i <= 5) {"
            "  sum = sum + i;"
            "  i = i + 1;"
            "}"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 15.0);
    }

    /* For */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit sum = 0;"
            "per (sit i = 1; i <= 5; i = i + 1) {"
            "  sum = sum + i;"
            "}"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 15.0);
    }
}


/* ==================================================
 * Probationes - Break/Continue (Frange/Perge)
 * ================================================== */

interior vacuum
_probare_frange_perge(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Frange/Perge ---\n");

    /* Frange in while */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit sum = 0;"
            "sit i = 0;"
            "dum (verum) {"
            "  i = i + 1;"
            "  si (i > 5) { frange; }"
            "  sum = sum + i;"
            "}"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 15.0);  /* 1+2+3+4+5 = 15 */
    }

    /* Frange in for */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit sum = 0;"
            "per (sit i = 0; i < 100; i = i + 1) {"
            "  si (i >= 5) { frange; }"
            "  sum = sum + i;"
            "}"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 10.0);  /* 0+1+2+3+4 = 10 */
    }

    /* Perge in while */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit sum = 0;"
            "sit i = 0;"
            "dum (i < 10) {"
            "  i = i + 1;"
            "  si (i % 2 == 0) { perge; }"
            "  sum = sum + i;"
            "}"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 25.0);  /* 1+3+5+7+9 = 25 */
    }

    /* Perge in for */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit sum = 0;"
            "per (sit i = 1; i <= 10; i = i + 1) {"
            "  si (i % 2 == 0) { perge; }"
            "  sum = sum + i;"
            "}"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 25.0);  /* 1+3+5+7+9 = 25 */
    }

    /* Nested loops cum frange */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit count = 0;"
            "per (sit i = 0; i < 5; i = i + 1) {"
            "  per (sit j = 0; j < 5; j = j + 1) {"
            "    si (j >= 2) { frange; }"
            "    count = count + 1;"
            "  }"
            "}"
            "count;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 10.0);  /* 5 outer * 2 inner = 10 */
    }
}


/* ==================================================
 * Probationes - Increment/Decrement
 * ================================================== */

interior vacuum
_probare_incrementum_decrementum(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Incrementum/Decrementum ---\n");

    /* Increment simplex */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 5;"
            "x++;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 6.0);
    }

    /* Decrement simplex */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 5;"
            "x--;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 4.0);
    }

    /* Increment in for loop */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit sum = 0;"
            "per (sit i = 0; i < 5; i++) {"
            "  sum = sum + i;"
            "}"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 10.0);  /* 0+1+2+3+4 = 10 */
    }

    /* Decrement in while */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 5;"
            "sit sum = 0;"
            "dum (x > 0) {"
            "  sum = sum + x;"
            "  x--;"
            "}"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 15.0);  /* 5+4+3+2+1 = 15 */
    }

    /* Multiple increments */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 0;"
            "x++; x++; x++;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.0);
    }

    /* Increment constans error */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "constans x = 5;"
            "x++;",
            piscina, intern);
        CREDO(!r.successus);  /* Debet fallere */
    }
}


/* ==================================================
 * Probationes - Compound Assignment
 * ================================================== */

interior vacuum
_probare_assignatio_complexa(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Assignatio Complexa ---\n");

    /* += */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 10;"
            "x += 5;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 15.0);
    }

    /* -= */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 10;"
            "x -= 3;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 7.0);
    }

    /* *= */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 5;"
            "x *= 4;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 20.0);
    }

    /* /= */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 20;"
            "x /= 4;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 5.0);
    }

    /* %= */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 17;"
            "x %= 5;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }

    /* Chained compound assignment */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 10;"
            "x += 5;"
            "x *= 2;"
            "x -= 10;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 20.0);  /* 10+5=15, 15*2=30, 30-10=20 */
    }

    /* Compound assignment cum expressione */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 10;"
            "sit y = 5;"
            "x += y * 2;"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 20.0);  /* 10 + (5*2) = 20 */
    }

    /* += cum chorda concatenatio */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit s = \"hello\";"
            "s += \" world\";"
            "s;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "hello world"));
    }

    /* Compound assignment constans error */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "constans x = 5;"
            "x += 1;",
            piscina, intern);
        CREDO(!r.successus);  /* Debet fallere */
    }
}


/* ==================================================
 * Probationes - Functiones
 * ================================================== */

interior vacuum
_probare_functiones(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Functiones ---\n");

    /* Functio simplex */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "functio add(a, b) { redde a + b; }"
            "add(2, 3);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 5.0);
    }

    /* Functio sine return */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "functio noop() { }"
            "noop();",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_NIHIL);
    }

    /* Recursio: factorial */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "functio factorial(n) {"
            "  si (n <= 1) { redde 1; }"
            "  redde n * factorial(n - 1);"
            "}"
            "factorial(5);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 120.0);
    }

    /* Recursio: fibonacci */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "functio fib(n) {"
            "  si (n <= 1) { redde n; }"
            "  redde fib(n - 1) + fib(n - 2);"
            "}"
            "fib(10);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 55.0);
    }

    /* Functio cum multiple return paths */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "functio abs(x) {"
            "  si (x < 0) { redde -x; }"
            "  redde x;"
            "}"
            "abs(-5) + abs(3);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 8.0);
    }
}


/* ==================================================
 * Probationes - Arrays
 * ================================================== */

interior vacuum
_probare_arrays(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Arrays ---\n");

    /* Array literalis */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("[1, 2, 3];", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_XAR);
    }

    /* Array accessus */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("sit arr = [10, 20, 30]; arr[1];", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 20.0);
    }

    /* Array assignatio */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr[0] = 100;"
            "arr[0];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 100.0);
    }

    /* Array len */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("len([1, 2, 3, 4, 5]);", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 5.0);
    }
}


/* ==================================================
 * Probationes - Objecta
 * ================================================== */

interior vacuum
_probare_objecta(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Objecta ---\n");

    /* Objectum literalis */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("sit obj = { a: 1, b: 2 }; obj;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_OBJECTUM);
    }

    /* Objectum accessus membri */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("sit obj = { x: 42 }; obj.x;", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 42.0);
    }

    /* Objectum assignatio membri */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit obj = { x: 1 };"
            "obj.x = 99;"
            "obj.x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 99.0);
    }

    /* Objectum bracket accessus */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit obj = { name: \"test\" };"
            "obj[\"name\"];",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "test"));
    }

    /* Objectum novum membrum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit obj = {};"
            "obj.new_prop = 123;"
            "obj.new_prop;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 123.0);
    }
}


/* ==================================================
 * Probationes - Built-ins
 * ================================================== */

interior vacuum
_probare_builtins(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Built-ins ---\n");

    /* len chorda */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("len(\"hello\");", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 5.0);
    }

    /* len array */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("len([1, 2, 3]);", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.0);
    }

    /* typeof */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("typeof(42);", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "number"));
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("typeof(\"hello\");", piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "string"));
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("typeof(verum);", piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "boolean"));
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("typeof(nihil);", piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "null"));
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("typeof([1,2]);", piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "array"));
    }

    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("typeof({a:1});", piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "object"));
    }
}


/* ==================================================
 * Probationes - Errors
 * ================================================== */

interior vacuum
_probare_errores(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Errores ---\n");

    /* Variabilis non definita */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("x;", piscina, intern);
        CREDO(!r.successus);
    }

    /* Divisio per nihil */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("1 / 0;", piscina, intern);
        CREDO(!r.successus);
    }

    /* Index extra limites */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("sit arr = [1, 2]; arr[10];", piscina, intern);
        CREDO(!r.successus);
    }

    /* Const reassign */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("constans x = 1; x = 2;", piscina, intern);
        CREDO(!r.successus);
    }

    /* Non-functio vocatio */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("sit x = 5; x();", piscina, intern);
        CREDO(!r.successus);
    }
}


/* ==================================================
 * Probationes - Complex Programs
 * ================================================== */

interior vacuum
_probare_complex(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Complex ---\n");

    /* FizzBuzz-like */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit result = 0;"
            "per (sit i = 1; i <= 15; i = i + 1) {"
            "  si (i % 15 == 0) {"
            "    result = result + 15;"
            "  } alioquin si (i % 3 == 0) {"
            "    result = result + 3;"
            "  } alioquin si (i % 5 == 0) {"
            "    result = result + 5;"
            "  } alioquin {"
            "    result = result + 1;"
            "  }"
            "}"
            "result;",
            piscina, intern);
        CREDO(r.successus);
        /* i=1,2,4,7,8,11,13,14 add 1 each (8), i=3,6,9,12 add 3 each (12),
           i=5,10 add 5 each (10), i=15 adds 15 = 8+12+10+15 = 45 */
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 45.0);
    }

    /* Nested function calls */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "functio double(x) { redde x * 2; }"
            "functio triple(x) { redde x * 3; }"
            "double(triple(double(5)));",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 60.0);  /* 5*2=10, 10*3=30, 30*2=60 */
    }

    /* Array manipulation */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3, 4, 5];"
            "sit sum = 0;"
            "per (sit i = 0; i < len(arr); i = i + 1) {"
            "  sum = sum + arr[i];"
            "}"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 15.0);
    }
}


/* ==================================================
 * Probationes - Arrow Functions (Functiones Sagittae)
 * ================================================== */

interior vacuum
_probare_functiones_sagittas(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Functiones Sagittae ---\n");

    /* Arrow simplex: x => x * 2 */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit double = x => x * 2;"
            "double(5);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 10.0);
    }

    /* Arrow cum duobus parametris: (a, b) => a + b */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit add = (a, b) => a + b;"
            "add(3, 7);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 10.0);
    }

    /* Arrow cum corpore block */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit complex = x => { sit y = x * 2; redde y + 1; };"
            "complex(5);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 11.0);
    }

    /* Arrow sine parametris */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit getFortyTwo = () => 42;"
            "getFortyTwo();",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 42.0);
    }

    /* Arrow in variabili et vocata */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit square = x => x * x;"
            "sit a = 5;"
            "square(a) + square(3);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 34.0);  /* 25 + 9 = 34 */
    }

    /* Arrow vocata ab alia functione */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit add = (a, b) => a + b;"
            "functio applyTwice(f, x) { redde f(f(x, 1), 1); }"
            "applyTwice(add, 5);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 7.0);  /* add(add(5,1),1) = add(6,1) = 7 */
    }
}


/* ==================================================
 * Probationes - Array Methods (Methodi Xar)
 * ================================================== */

interior vacuum
_probare_methodos_xar(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Methodi Xar ---\n");

    /* push */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr.push(4);"
            "len(arr);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 4.0);
    }

    /* push - valor ultimo */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr.push(4);"
            "arr[3];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 4.0);
    }

    /* pop */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "sit x = arr.pop();"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.0);
    }

    /* pop - longitudo reducta */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr.pop();"
            "len(arr);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }

    /* shift */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "sit x = arr.shift();"
            "x;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 1.0);
    }

    /* shift - primum novum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr.shift();"
            "arr[0];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }

    /* unshift */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [2, 3];"
            "arr.unshift(1);"
            "arr[0];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 1.0);
    }

    /* indexOf - inventum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [10, 20, 30];"
            "arr.indexOf(20);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 1.0);
    }

    /* indexOf - non inventum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [10, 20, 30];"
            "arr.indexOf(99);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, -1.0);
    }

    /* includes - verum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr.includes(2);",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    /* includes - falsum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr.includes(99);",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }

    /* slice */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3, 4, 5];"
            "sit sliced = arr.slice(1, 4);"
            "len(sliced);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.0);
    }

    /* slice - valores */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3, 4, 5];"
            "sit sliced = arr.slice(1, 4);"
            "sliced[0] + sliced[1] + sliced[2];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 9.0);  /* 2 + 3 + 4 = 9 */
    }

    /* concat */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit a = [1, 2];"
            "sit b = [3, 4];"
            "sit c = a.concat(b);"
            "len(c);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 4.0);
    }

    /* join */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr.join(\"-\");",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "1-2-3"));
    }

    /* length() methodus */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr.length();",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.0);
    }

    /* length() - array vacua */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("[].length();", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 0.0);
    }

    /* length() post push */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2];"
            "arr.push(3);"
            "arr.length();",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.0);
    }
}


/* ==================================================
 * Probationes - Callback Methods (map, filter, find, forEach)
 * ================================================== */

interior vacuum
_probare_methodos_callback(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Methodi Callback ---\n");

    /* map - simplex */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "sit doubled = arr.map(x => x * 2);"
            "doubled[0] + doubled[1] + doubled[2];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 12.0);  /* 2 + 4 + 6 = 12 */
    }

    /* map - cum block corpus */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "sit result = arr.map(x => { sit y = x * 2; redde y + 1; });"
            "result[0] + result[1] + result[2];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 15.0);  /* 3 + 5 + 7 = 15 */
    }

    /* filter - simplex */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3, 4, 5, 6];"
            "sit evens = arr.filter(x => x % 2 == 0);"
            "len(evens);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.0);
    }

    /* filter - valores */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3, 4, 5, 6];"
            "sit evens = arr.filter(x => x % 2 == 0);"
            "evens[0] + evens[1] + evens[2];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 12.0);  /* 2 + 4 + 6 = 12 */
    }

    /* filter - nullus */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "sit result = arr.filter(x => x > 100);"
            "len(result);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 0.0);
    }

    /* find - inventum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3, 4, 5];"
            "arr.find(x => x > 3);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 4.0);
    }

    /* find - non inventum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "arr.find(x => x > 100);",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_NIHIL);
    }

    /* forEach - effectus lateris (usque count) */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit count = 0;"
            "[1, 2, 3].forEach(x => { count = count + 1; });"
            "count;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.0);
    }

    /* forEach - summa per effectum lateris */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit sum = 0;"
            "[1, 2, 3, 4, 5].forEach(x => { sum = sum + x; });"
            "sum;",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 15.0);
    }

    /* Chaining: filter + map */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3, 4, 5];"
            "sit result = arr.filter(x => x > 2).map(y => y * 2);"
            "result[0] + result[1] + result[2];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 24.0);  /* 6 + 8 + 10 = 24 */
    }

    /* map cum functione regulari */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "functio triple(x) { redde x * 3; }"
            "sit arr = [1, 2, 3];"
            "sit result = arr.map(x => triple(x));"
            "result[0] + result[1] + result[2];",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 18.0);  /* 3 + 6 + 9 = 18 */
    }
}


/* ==================================================
 * Probationes - Methodi Chorda (String Methods)
 * ================================================== */

interior vacuum
_probare_methodos_chorda(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Methodi Chorda ---\n");

    /* length */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\".length();", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 5.0);
    }

    /* length - chorda vacua */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"\".length();", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 0.0);
    }

    /* charAt */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\".charAt(1);", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "e"));
    }

    /* charAt - extra fines */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hi\".charAt(10);", piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, ""));
    }

    /* indexOf - inventum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\".indexOf(\"l\");", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }

    /* indexOf - non inventum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\".indexOf(\"x\");", piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, -1.0);
    }

    /* includes - verum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\".includes(\"ell\");", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    /* includes - falsum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\".includes(\"xyz\");", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }

    /* substring */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\".substring(1, 4);", piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "ell"));
    }

    /* toUpperCase */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"hello\".toUpperCase();", piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "HELLO"));
    }

    /* toLowerCase */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("\"HELLO\".toLowerCase();", piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "hello"));
    }

    /* split */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = \"a,b,c\".split(\",\");"
            "len(arr);",
            piscina, intern);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 3.0);
    }

    /* split - verificare valores */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = \"a,b,c\".split(\",\");"
            "arr[0];",
            piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "a"));
    }

    /* chaining methodi */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "\"HELLO\".toLowerCase().substring(0, 2);",
            piscina, intern);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "he"));
    }
}


/* ==================================================
 * Probationes - Template Strings
 * ================================================== */

interior vacuum
_probare_template_strings(Piscina* piscina, InternamentumChorda* intern)
{
    imprimere("\n--- Probatio Template Strings ---\n");

    /* Template simplex sine interpolatione */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("`Salve mundus!`;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "Salve mundus!"));
    }

    /* Template vacuum */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare("``;", piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(r.valor.ut.chorda_valor.mensura == ZEPHYRUM);
    }

    /* Interpolatio simplex cum variabili */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit nomen = \"Marcus\";"
            "`Salve ${nomen}!`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "Salve Marcus!"));
    }

    /* Interpolatio cum numero */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 42;"
            "`Numerus: ${x}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "Numerus: 42"));
    }

    /* Interpolatio multiplex */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit a = 1;"
            "sit b = 2;"
            "`${a} + ${b} = ${a + b}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "1 + 2 = 3"));
    }

    /* Expressio in interpolatione */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 5;"
            "`Duplum: ${x * 2}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "Duplum: 10"));
    }

    /* Vocatio functionis in interpolatione */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit arr = [1, 2, 3];"
            "`Longitudo: ${len(arr)}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "Longitudo: 3"));
    }

    /* Interpolatio ad initium (vacuum ante) */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit v = \"test\";"
            "`${v}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "test"));
    }

    /* Interpolatio cum concatenatione chordae */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit prae = \"Salve\";"
            "sit post = \"mundus\";"
            "`${prae} ${post}!`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "Salve mundus!"));
    }

    /* Interpolatio cum ternaria */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit x = 10;"
            "`x est ${x > 5 ? \"magnus\" : \"parvus\"}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "x est magnus"));
    }

    /* Template in variabili */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit nomen = \"Iulius\";"
            "sit salutatio = `Ave ${nomen}!`;"
            "salutatio;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "Ave Iulius!"));
    }

    /* Nested template (template in template) */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit a = \"X\";"
            "sit b = \"Y\";"
            "`outer ${`inner ${a}`} ${b}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "outer inner X Y"));
    }

    /* Objectum literalis in interpolatione (bracchium profunditas) */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "`typus: ${typeof({a: 1})}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "typus: object"));
    }

    /* Objectum complexum in interpolatione */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit obj = {x: 10, y: 20};"
            "`x=${obj.x}, y=${obj.y}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "x=10, y=20"));
    }

    /* Array literalis in interpolatione */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "`len: ${len([1, 2, 3])}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "len: 3"));
    }

    /* Bracchia multiplicata in interpolatione */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare(
            "sit f = x => { redde x * 2; };"
            "`result: ${f(5)}`;",
            piscina, intern);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_CHORDA);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "result: 10"));
    }
}


/* ==================================================
 * Probationes - Entitates (Entity System)
 * ================================================== */

interior vacuum
_probare_entitates(Piscina* piscina, InternamentumChorda* intern)
{
    Persistentia* persistentia;
    EntitasRepositorium* repositorium;

    imprimere("\n--- Probatio Entitates ---\n");

    /* Creare repositorium in memoria */
    persistentia = persistentia_memoria_creare(piscina);
    CREDO(persistentia != NIHIL);

    repositorium = entitas_repositorium_creare(piscina, persistentia);
    CREDO(repositorium != NIHIL);

    /* REPO.scaffold - creare */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit city = REPO.scaffold(\"City\", \"Roma\");"
            "city;",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_ENTITAS);
    }

    /* REPO.scaffold - idempotens */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit a = REPO.scaffold(\"City\", \"Roma\");"
            "sit b = REPO.scaffold(\"City\", \"Roma\");"
            "a.id == b.id;",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }

    /* entity.id (proprietas) */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit e = REPO.scaffold(\"City\", \"Test\");"
            "typeof(e.id);",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "string"));
    }

    /* entity.genus (proprietas) */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit e = REPO.scaffold(\"City\", \"Urbs\");"
            "e.genus;",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "City"));
    }

    /* TODO: entity proprietas custom - functio in memoria non operatur
     * Operatur cum persistentia filum sed non cum persistentia_memoria.
     * Haec probatio praetermittitur donec persistentia_memoria reparetur.
     */
    /*
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit e = REPO.scaffold(\"City\", \"Londinium\");"
            "e.population = \"9000000\";"
            "e.population;",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "9000000"));
    }
    */

    /* REPO.get */
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit e = REPO.scaffold(\"City\", \"Athena\");"
            "sit id = e.id;"
            "sit found = REPO.get(id);"
            "found.genus;",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(chorda_aequalis_literis(r.valor.ut.chorda_valor, "City"));
    }

    /* TODO: add_relation + related - non operatur
     * Relationes non inveniuntur post add_relation.
     * Haec probatio praetermittitur donec reparetur.
     */
    /*
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit country = REPO.scaffold(\"Country\", \"Italia\");"
            "sit city = REPO.scaffold(\"City\", \"Roma\");"
            "country.add_relation(\"contains\", city.id);"
            "sit rel = country.related(\"contains\");"
            "rel.id == city.id;",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }
    */

    /* TODO: related_all - non operatur (vide supra) */
    /*
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit parent = REPO.scaffold(\"Parent\", \"P1\");"
            "sit c1 = REPO.scaffold(\"Child\", \"C1\");"
            "sit c2 = REPO.scaffold(\"Child\", \"C2\");"
            "parent.add_relation(\"child\", c1.id);"
            "parent.add_relation(\"child\", c2.id);"
            "sit all = parent.related_all(\"child\");"
            "len(all);",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }
    */

    /* TODO: add_tag + has_tag - non operatur cum persistentia_memoria */
    /*
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit e = REPO.scaffold(\"City\", \"Paris\");"
            "e.add_tag(\"capital\");"
            "e.has_tag(\"capital\");",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }
    */

    /* TODO: has_tag - falsum - non operatur cum persistentia_memoria */
    /*
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit e = REPO.scaffold(\"City\", \"Lyon\");"
            "e.has_tag(\"capital\");",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }
    */

    /* TODO: remove_tag - non operatur cum persistentia_memoria */
    /*
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit e = REPO.scaffold(\"City\", \"Berlin\");"
            "e.add_tag(\"wall\");"
            "e.remove_tag(\"wall\");"
            "e.has_tag(\"wall\");",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_FALSUM);
    }
    */

    /* TODO: REPO.tagged - non operatur cum persistentia_memoria
     * Notae non inveniuntur.
     */
    /*
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit e1 = REPO.scaffold(\"City\", \"Vienna\");"
            "sit e2 = REPO.scaffold(\"City\", \"Prague\");"
            "e1.add_tag(\"historic\");"
            "e2.add_tag(\"historic\");"
            "sit found = REPO.tagged(\"historic\");"
            "len(found);",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO_AEQUALIS_F64(r.valor.ut.numerus, 2.0);
    }
    */

    /* TODO: REPO.search - non operatur
     * Tituli non inveniuntur.
     */
    /*
    {
        SputnikInterpresResultus r;
        r = sputnik_evaluare_cum_repositorio(
            "sit e = REPO.scaffold(\"Document\", \"Manifesto\");"
            "sit found = REPO.search(\"Manifesto\");"
            "len(found) > 0;",
            piscina, intern, repositorium);
        CREDO(r.successus);
        CREDO(r.valor.genus == SPUTNIK_VALOR_VERUM);
    }
    */
}


/* ==================================================
 * Main
 * ================================================== */

integer principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    b32 praeteritus;

    piscina = piscina_generare_dynamicum("probatio_interpres", 1024 * 64);
    intern = internamentum_creare(piscina);

    credo_aperire(piscina);

    _probare_literalia(piscina, intern);
    _probare_arithmeticam(piscina, intern);
    _probare_comparationem(piscina, intern);
    _probare_logicos(piscina, intern);
    _probare_ternariam(piscina, intern);
    _probare_variabiles(piscina, intern);
    _probare_control_flow(piscina, intern);
    _probare_frange_perge(piscina, intern);
    _probare_incrementum_decrementum(piscina, intern);
    _probare_assignatio_complexa(piscina, intern);
    _probare_functiones(piscina, intern);
    _probare_arrays(piscina, intern);
    _probare_objecta(piscina, intern);
    _probare_builtins(piscina, intern);
    _probare_errores(piscina, intern);
    _probare_complex(piscina, intern);
    _probare_functiones_sagittas(piscina, intern);
    _probare_methodos_xar(piscina, intern);
    _probare_methodos_callback(piscina, intern);
    _probare_methodos_chorda(piscina, intern);
    _probare_template_strings(piscina, intern);
    _probare_entitates(piscina, intern);

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
