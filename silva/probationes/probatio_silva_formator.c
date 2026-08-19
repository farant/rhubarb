/* probatio_silva_formator.c - machina lint formatoris (tranche
 * fluminis crudi: R5/R6/R12/R14/R16). Casus negativi onus
 * probant: tabulae in litteris chordarum immunes (genus, non
 * octetus), cursus '=' extra commenta taciti - semantica quam
 * substratum gratis dat, hic mensurata.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_formator.h"
#include "credo.h"

#include <stdio.h>
#include <string.h>

interior Xar*
_lint (
              Piscina* piscina,
    constans character* fons)
{
    redde formator_lint(piscina, fons, (i32)strlen(fons));
}

interior FormatorDivergentia*
_divergentia (
    Xar* divergentiae,
    i32  index)
{
    redde (FormatorDivergentia*)xar_obtinere(divergentiae,
        index);
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_formatoris",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans fontem conformem ---\n");
    {
        Xar* d = _lint(piscina,
            "interior i32\n"
            "_adiuvare (\n"
            "    i32 valor)\n"
            "{\n"
            "    si (valor > I) redde valor;\n"
            "    redde ZEPHYRUM;\n"
            "}\n");

        CREDO_NON_NIHIL(d);
        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }

    imprimere("\n--- Probans tabulas (R5) ---\n");
    {
        Xar* d = _lint(piscina,
            "vacuum probare (vacuum)\n"
            "{\n"
            "\tredde;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "indentatio-quaterna") == ZEPHYRUM);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->linea, (i32)3);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->columna, (i32)1);
    }

    imprimere("\n--- Probans tabulam in littera (immunis) ---\n");
    {
        Xar* d = _lint(piscina,
            "constans character* s = \"a\tb\";\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }

    imprimere("\n--- Probans spatium post claves (R6) ---\n");
    {
        Xar* d = _lint(piscina, "si(x) redde;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "spatium-post-claves") == ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)0);
    }
    {
        Xar* d = _lint(piscina, "dum  (x) perge;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)2);
    }
    {
        Xar* d = _lint(piscina, "commutatio (x) frange;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }

    imprimere("\n--- Probans spatia caudae (R12) ---\n");
    {
        Xar* d = _lint(piscina, "i32 a; \ni32 b;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "spatia-caudae") == ZEPHYRUM);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->linea, (i32)1);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->columna, (i32)7);
    }
    {
        /* sine linea nova finali */
        Xar* d = _lint(piscina, "i32 a;");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->nuntius,
            "linea nova finalis deest") == ZEPHYRUM);
    }
    {
        /* lineae vacuae in fine */
        Xar* d = _lint(piscina, "i32 a;\n\n\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)3);
    }

    imprimere("\n--- Probans vexilla (R14) ---\n");
    {
        /* vexillum 59 (anomalia piscinae) - ambo cursus flagrant */
        Xar* d = _lint(piscina,
            "/* =============================="
            "=============================\n"
            " * Titulus\n"
            " * =============================="
            "============================= */\n"
            "i32 a;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)2);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "vexillum-quinquaginta") == ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)59);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->linea, (i32)1);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->columna, (i32)4);
        CREDO_AEQUALIS_I32(_divergentia(d, 1)->linea, (i32)3);
    }
    {
        /* vexillum rectum (50) tacet */
        Xar* d = _lint(piscina,
            "/* ========================="
            "=========================\n"
            " * Titulus\n"
            " * ========================="
            "========================= */\n"
            "i32 a;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }
    {
        /* cursus '=' in littera chordae tacet */
        Xar* d = _lint(piscina,
            "constans character* s =\n"
            "    \"====================\";\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }

    imprimere("\n--- Probans longitudinem (R16) ---\n");
    {
        /* linea 76 octetorum: "i32 " + 71 'a' + ';' */
        Xar* d = _lint(piscina,
            "i32 aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "longitudo-lxxii") == ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)76);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->columna,
            (i32)73);
    }

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
