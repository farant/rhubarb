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
    redde formator_lint(piscina, NIHIL, fons,
        (i32)strlen(fons));
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
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "\tredde;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "indentatio-quaterna") == ZEPHYRUM);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->linea, (i32)4);
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
            "\n"
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
            "\n"
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

    imprimere("\n--- Probans titulum in linea sua (R1+R8) ---\n");
    {
        /* titulus in linea specificatorum + parametrum in
         * linea parenthesis - ambae regulae flagrant */
        Xar* d = _lint(piscina,
            "interior i32 _f (i32 a)\n"
            "{\n"
            "    redde a;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)2);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "typus-in-linea-sua") == ZEPHYRUM);
        CREDO_VERUM(strcmp(_divergentia(d, 1)->regula,
            "parametra-singula") == ZEPHYRUM);
    }

    imprimere("\n--- Probans spatium definitionis (R2) ---\n");
    {
        /* nullum spatium ante parenthesim definitionis */
        Xar* d = _lint(piscina,
            "interior i32\n"
            "_f(\n"
            "    i32 a)\n"
            "{\n"
            "    redde a;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "spatium-definitionis") == ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)0);
    }
    {
        /* spatium ante parenthesim VOCATIONIS - inversum */
        Xar* d = _lint(piscina,
            "interior i32\n"
            "_f (\n"
            "    i32 a)\n"
            "{\n"
            "    redde _f (a);\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "spatium-definitionis") == ZEPHYRUM);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->linea, (i32)5);
    }

    imprimere("\n--- Probans bracchia (R3) ---\n");
    {
        /* brachium apertum in linea conditionis */
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_f (\n"
            "    i32 a)\n"
            "{\n"
            "    si (a) {\n"
            "        a = I;\n"
            "    }\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "bracchia-allman") == ZEPHYRUM);
        CREDO_AEQUALIS_I32(_divergentia(d, 0)->linea, (i32)5);
    }
    {
        /* columna bracchiorum non columna possessoris */
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_f (\n"
            "    i32 a)\n"
            "{\n"
            "    si (a)\n"
            "      {\n"
            "        a = I;\n"
            "      }\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)2);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)7);
        CREDO_AEQUALIS_I32(
            (i32)_divergentia(d, 0)->exspectatum, (i32)5);
    }

    imprimere("\n--- Probans custodem una linea (R4) ---\n");
    {
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_f (\n"
            "    i32 a)\n"
            "{\n"
            "    si (a)\n"
            "        a = I;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "custos-una-linea") == ZEPHYRUM);
    }

    imprimere("\n--- Probans radicem originis + vacuum (R1/R8) ---\n");
    {
        /* principale->main expansum: positiones per radicem
         * originis resolvendae; '(vacuum)' in linea manet */
        Xar* d = _lint(piscina,
            "integer\n"
            "principale (vacuum)\n"
            "{\n"
            "    redde ZEPHYRUM;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }

    imprimere("\n--- Probans columnas binas (R7) ---\n");
    {
        /* ordo structurae conformis: typi dextre ordinati,
         * stella in hiatu (G = II), tituli ad columnam unam */
        Xar* d = _lint(piscina,
            "nomen structura Probandum {\n"
            "     vacuum* buffer;\n"
            "    integer  numerus;\n"
            "} Probandum;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }
    {
        /* fractus: ora typorum inaequalis + titulus male situs */
        Xar* d = _lint(piscina,
            "nomen structura Probandum {\n"
            "    vacuum* buffer;\n"
            "    integer numerus;\n"
            "} Probandum;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)2);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "columnae-binae") == ZEPHYRUM);
        CREDO_VERUM(strcmp(_divergentia(d, 1)->regula,
            "columnae-binae") == ZEPHYRUM);
        CREDO_AEQUALIS_I32(
            (i32)_divergentia(d, 1)->exspectatum, (i32)14);
    }

    imprimere("\n--- Probans aequationem (R9 + locales R7) ---\n");
    {
        /* locales ordinatae + glomus '=' ordinatum */
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_f (\n"
            "    integer a)\n"
            "{\n"
            "    character* nota;\n"
            "      integer  b;\n"
            "\n"
            "    nota  = NIHIL;\n"
            "    b     = a;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }
    {
        /* glomus fractum: '=' secundum non ordinatum */
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_f (\n"
            "    integer a)\n"
            "{\n"
            "    integer alpha;\n"
            "    integer beta;\n"
            "\n"
            "    alpha  = a;\n"
            "    beta = a;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "aequatio-assignationum") == ZEPHYRUM);
        CREDO_AEQUALIS_I32(
            (i32)_divergentia(d, 0)->exspectatum, (i32)12);
    }
    {
        /* exceptio LXXII: membrum cuius forma ordinata limitem
         * transgrederetur tacet (brevis maneret > LXXII) */
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_f (\n"
            "    integer a)\n"
            "{\n"
            "    integer brevis;\n"
            "    integer longissimum_titulum_habens;\n"
            "\n"
            "    brevis = a + a + a + a + a + a + a + a + a"
            " + a + a + a + a;\n"
            "    longissimum_titulum_habens  = a;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }

    imprimere("\n--- Probans intervalla (R13) ---\n");
    {
        /* una vacua ante vexillum (duae exspectatae) */
        Xar* d = _lint(piscina,
            "integer a;\n"
            "\n"
            "/* ========================="
            "========================= */\n"
            "\n"
            "integer b;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "intervalla") == ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)1);
    }
    {
        /* cumulus vacuarum (IV) nimius */
        Xar* d = _lint(piscina,
            "integer a;\n"
            "\n\n\n\n"
            "integer b;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "intervalla") == ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)4);
    }

    imprimere("\n--- Probans operatores (R10) ---\n");
    {
        /* binarium arte scriptum: ante + post ambo flagrant */
        Xar* d = _lint(piscina,
            "interior i32\n"
            "_f (\n"
            "    i32 a)\n"
            "{\n"
            "    redde a+a;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)2);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "operatores") == ZEPHYRUM);
    }
    {
        /* virgula: spatium ante + nullum post */
        Xar* d = _lint(piscina,
            "interior i32\n"
            "_g (\n"
            "    i32 a)\n"
            "{\n"
            "    redde _g(a ,a);\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)2);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "operatores") == ZEPHYRUM);
        CREDO_VERUM(strcmp(_divergentia(d, 1)->regula,
            "operatores") == ZEPHYRUM);
    }

    imprimere("\n--- Probans continuationem (R11) ---\n");
    {
        /* continuatio parum indentata (< ca + IV) */
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_f (\n"
            "    i32 a)\n"
            "{\n"
            "    a = a\n"
            "  + a;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "continuatio") == ZEPHYRUM);
        CREDO_AEQUALIS_I32(
            (i32)_divergentia(d, 0)->exspectatum, (i32)9);
    }
    {
        /* operator claudens lineam (initio continuationis
         * ponendus) */
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_f (\n"
            "    i32 a)\n"
            "{\n"
            "    a = a +\n"
            "        a;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "continuatio") == ZEPHYRUM);
    }

    imprimere("\n--- Probans ordinem inclusionum (R15) ---\n");
    {
        /* domestica post systemicam */
        Xar* d = _lint(piscina,
            "#include <string.h>\n"
            "#include \"chorda.h\"\n"
            "i32 a;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "ordo-inclusionum") == ZEPHYRUM);
    }
    {
        /* postulata_posix.h non prima */
        Xar* d = _lint(piscina,
            "#include \"chorda.h\"\n"
            "#include \"postulata_posix.h\"\n"
            "i32 a;\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "ordo-inclusionum") == ZEPHYRUM);
    }

    imprimere("\n--- Probans intervalla inter functiones ---\n");
    {
        /* functiones conglutinatae (0 vacuae pro I) */
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_a (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"
            "interior vacuum\n"
            "_b (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "intervalla") == ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_divergentia(d, 0)->inventum,
            (i32)0);
    }

    imprimere("\n--- Probans capita praebita (R7 resoluta) ---\n");
    {
        /* DIFFERENTIALIS: typedef in capite praebito resolvitur
         * -> ordo fractus FLAGRAT (sine praebitione membrum
         * ambiguum exemptum esset et lint taceret) */
        SilvaContextus* ctx;
        constans character* caput;
        constans character* fons_probandus;
        Xar* d;

        caput = "nomen insignatus integer mensura_probanda;\n";
        ctx = silva_contextus_creare(piscina);
        CREDO_NON_NIHIL(ctx);
        CREDO_VERUM(silva_contextus_latinam_addere(ctx));
        CREDO_VERUM(silva_contextus_praebere(ctx,
            "probandum.h", caput, (i32)strlen(caput)));

        fons_probandus =
            "#include \"probandum.h\"\n"
            "interior vacuum\n"
            "_f (\n"
            "    vacuum* a)\n"
            "{\n"
            "    vacuum* datum;\n"
            "    mensura_probanda n;\n"
            "\n"
            "    datum = a;\n"
            "}\n";
        d = formator_lint(piscina, ctx, fons_probandus,
            (i32)strlen(fons_probandus));

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)2);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "columnae-binae") == ZEPHYRUM);
        CREDO_VERUM(strcmp(_divergentia(d, 1)->regula,
            "columnae-binae") == ZEPHYRUM);
    }

    imprimere("\n--- Probans catenam alioquin-si (caput) ---\n");
    {
        /* bracchia catenae e capite mensurata - conformis */
        Xar* d = _lint(piscina,
            "interior vacuum\n"
            "_f (\n"
            "    i32 a)\n"
            "{\n"
            "    si (a)\n"
            "    {\n"
            "        a = I;\n"
            "    }\n"
            "    alioquin si (a)\n"
            "    {\n"
            "        a = II;\n"
            "    }\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
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
