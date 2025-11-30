/* probatio_sputnik_interpres.c - Probationes pro Sputnik Interpres
 *
 * Probare tree-walking evaluator.
 */

#include "sputnik_interpres.h"
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
    _probare_functiones(piscina, intern);
    _probare_arrays(piscina, intern);
    _probare_objecta(piscina, intern);
    _probare_builtins(piscina, intern);
    _probare_errores(piscina, intern);
    _probare_complex(piscina, intern);

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
