/* probatio_silva_conditio.c - Probationes evaluatoris #if/#elif (Chunk D) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_expandere.h"
#include "silva_conditio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* Evaluare expressionem cum praefixo optionali directivarum
 * (#define...). EOF lexatoris remotum - evaluator lineam
 * directivae accipit, quae EOF numquam continet. */
interior s64
_evaluare (
               Piscina* piscina,
    constans character* praefixum,
    constans character* expressio,
                   b32* successus)
{
    SilvaExpansio* exp;
              Xar* lexemata;
              Xar* sine_eof;
              i32  i;
              i32  n;

    exp = silva_expansio_creare(piscina);
    silva_fons_addere(exp, "probatio.c", FALSUM);
    si (praefixum != NIHIL)
    {
        Xar* lex_praefixi;

        lex_praefixi = silva_lexare(piscina, praefixum,
            (i32)strlen(praefixum), ZEPHYRUM);
        silva_expansio_directivas_processare(exp, lex_praefixi, NIHIL);
    }

    lexemata = silva_lexare(piscina, expressio, (i32)strlen(expressio),
        ZEPHYRUM);
    sine_eof  = xar_creare(piscina, magnitudo(SilvaToken*));
    n         = xar_numerus(lexemata);
    per (i = ZEPHYRUM; i < n; i++)
    {
        SilvaToken*  t;
        SilvaToken** locus;

        t = *(SilvaToken**)xar_obtinere(lexemata, i);
        si (t->genus != SILVA_LEX_EOF)
        {
            locus = (SilvaToken**)xar_addere(sine_eof);
            si (locus != NIHIL)
            {
                *locus = t;
            }
        }
    }

    redde silva_conditio_evaluare(exp, sine_eof, successus);
}

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;
        b32  ok;

    piscina = piscina_generare_dynamicum("probatio_silva_conditio",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: litterae et arithmetica
     * ======================================================== */

    {
        imprimere("\n--- Probans arithmeticam ---\n");

        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1", &ok), I);
        CREDO_VERUM (ok);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "0", &ok),
            ZEPHYRUM);
        CREDO_VERUM (ok);

        /* praecedentia */
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 + 2 * 3", &ok),
            VII);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "(1 + 2) * 3",
            &ok), IX);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "10 / 3", &ok),
            III);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "10 % 3", &ok),
            I);

        /* basis: hex, oct, suffixa */
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "0x10", &ok),
            XVI);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "010", &ok),
            VIII);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "16L", &ok), XVI);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1U", &ok), I);
    }


    /* ========================================================
     * PROBARE: comparatio SIGNATA (regressio defectus arbor2:
     * i64 latinae INSIGNATUS est - #if -1 < 0 erat FALSUM!)
     * ======================================================== */

    {
        imprimere("\n--- Probans comparationem signatam ---\n");

        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "-1 < 0", &ok),
            I);
        CREDO_VERUM (ok);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "-5 / 2", &ok),
            -II);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "0 - 3", &ok),
            -III);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "~0", &ok), -I);
    }


    /* ========================================================
     * PROBARE: bitalis, translatio, logica, ternarius
     * ======================================================== */

    {
        imprimere("\n--- Probans operatores ceteros ---\n");

        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "0xFF & 0x0F",
            &ok), XV);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 | 2", &ok),
            III);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "5 ^ 3", &ok),
            VI);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 << 4", &ok),
            XVI);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "256 >> 4", &ok),
            XVI);

        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "3 > 2", &ok), I);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "2 >= 3", &ok),
            ZEPHYRUM);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 == 1", &ok),
            I);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 != 1", &ok),
            ZEPHYRUM);

        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 && 0", &ok),
            ZEPHYRUM);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 || 0", &ok),
            I);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "!0", &ok), I);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "!5", &ok),
            ZEPHYRUM);

        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 ? 5 : 10",
            &ok), V);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "0 ? 5 : 10",
            &ok), X);

        /* littera characteris */
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "'A'", &ok), LXV);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "'\\n'", &ok), X);
    }


    /* ========================================================
     * PROBARE: defined (ambae formae; operandum NON expansum)
     * ======================================================== */

    {
        imprimere("\n--- Probans defined ---\n");

        CREDO_AEQUALIS_S64 (
            _evaluare(piscina, "#define FOO 1\n", "defined FOO", &ok),
            I);
        CREDO_AEQUALIS_S64 (
            _evaluare(piscina, "#define FOO 1\n", "defined(FOO)", &ok),
            I);
        CREDO_AEQUALIS_S64 (
            _evaluare(piscina, NIHIL, "defined BAR", &ok), ZEPHYRUM);
        CREDO_VERUM (ok);
        CREDO_AEQUALIS_S64 (
            _evaluare(piscina, NIHIL, "defined(BAR)", &ok), ZEPHYRUM);

        /* operandum protectum: FOO ad 0 expanderetur, sed defined
         * ANTE expansionem substituitur */
        CREDO_AEQUALIS_S64 (
            _evaluare(piscina, "#define FOO 0\n", "defined(FOO)", &ok),
            I);

        /* compositum */
        CREDO_AEQUALIS_S64 (
            _evaluare(piscina, "#define FOO 1\n",
                "defined(FOO) && !defined(BAR)", &ok), I);
    }


    /* ========================================================
     * PROBARE: expansio macro plena (arbor2 solum unum lexema
     * obiecti-simile poterat - silva expansorem verum adhibet)
     * ======================================================== */

    {
        imprimere("\n--- Probans expansionem in conditione ---\n");

        CREDO_AEQUALIS_S64 (
            _evaluare(piscina, "#define N 4\n", "N > 2", &ok), I);

        /* corpus multi-lexematum */
        CREDO_AEQUALIS_S64 (
            _evaluare(piscina, "#define N (2 + 3)\n", "N * 2", &ok), X);

        /* functio-similis in conditione */
        CREDO_AEQUALIS_S64 (
            _evaluare(piscina,
                "#define MAX(a, b) ((a) > (b) ? (a) : (b))\n",
                "MAX(3, 5) == 5", &ok), I);
        CREDO_VERUM (ok);

        /* identificator indefinitus = 0 (C89) */
        CREDO_AEQUALIS_S64 (
            _evaluare(piscina, NIHIL, "IGNOTUS + 1", &ok), I);
        CREDO_VERUM (ok);
    }


    /* ========================================================
     * PROBARE: errores - 0 et successus FALSUM, nullus fragor
     * ======================================================== */

    {
        imprimere("\n--- Probans errores ---\n");

        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 / 0", &ok),
            ZEPHYRUM);
        CREDO_FALSUM (ok);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 % 0", &ok),
            ZEPHYRUM);
        CREDO_FALSUM (ok);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 +", &ok),
            ZEPHYRUM);
        CREDO_FALSUM (ok);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, ")", &ok),
            ZEPHYRUM);
        CREDO_FALSUM (ok);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "", &ok),
            ZEPHYRUM);
        CREDO_FALSUM (ok);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 << 65", &ok),
            ZEPHYRUM);
        CREDO_FALSUM (ok);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "defined", &ok),
            ZEPHYRUM);
        CREDO_FALSUM (ok);
        CREDO_AEQUALIS_S64 (_evaluare(piscina, NIHIL, "1 ? 2", &ok),
            ZEPHYRUM);
        CREDO_FALSUM (ok);
    }


    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

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
