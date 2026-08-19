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
#include "silva_differre.h"
#include "silva_lexema.h"
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

interior FormatorScriptum
_scribere (
              Piscina* piscina,
    constans character* fons)
{
    redde formator_scribere(piscina, NIHIL, fons,
        (i32)strlen(fons));
}

interior b32
_textus_aequalis (
               Piscina* piscina,
                chorda  textus,
    constans character* literae)
{
    redde chorda_aequalis(textus,
        chorda_ex_literis(literae, piscina));
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

    imprimere("\n--- Probans scribere: conformis intactus ---\n");
    {
        constans character* fons =
            "interior i32\n"
            "_adiuvare (\n"
            "    i32 valor)\n"
            "{\n"
            "    si (valor > I) redde valor;\n"
            "    redde ZEPHYRUM;\n"
            "}\n";
        FormatorScriptum s = _scribere(piscina, fons);

        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
        CREDO_AEQUALIS_I32(s.iterationes, (i32)1);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus, fons));
    }

    imprimere("\n--- Probans scribere: tabulae -> IV spatia ---\n");
    {
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "\tredde;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: caudae et finis ---\n");
    {
        /* spatia caudae + linea nova finalis deest */
        FormatorScriptum s = _scribere(piscina,
            "i32 a; \ni32 b;");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "i32 a;\ni32 b;\n"));
    }
    {
        /* lineae vacuae in fine -> una nova */
        FormatorScriptum s = _scribere(piscina,
            "i32 a;\n\n\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "i32 a;\n"));
    }

    imprimere("\n--- Probans scribere: virgula ---\n");
    {
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    f(a ,b);\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    f(a, b);\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: intervalla collapsa ---\n");
    {
        FormatorScriptum s = _scribere(piscina,
            "i32 a;\n"
            "\n"
            "\n"
            "\n"
            "i32 b;\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "i32 a;\n"
            "\n"
            "\n"
            "i32 b;\n"));
    }

    imprimere("\n--- Probans scribere: portae (differre/fidelitas/"
        "idempotentia) ---\n");
    {
        /* violationes multae in functione una: tabula + R6 +
         * caudae + virgula - una iteratione sanatae, altera
         * conformitas confirmata */
        constans character* fons =
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "\tsi(I) redde;   \n"
            "    f(a ,b);\n"
            "}\n";
        constans character* exspectatum =
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    si (I) redde;\n"
            "    f(a, b);\n"
            "}\n";
        FormatorScriptum s = _scribere(piscina, fons);

        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_AEQUALIS_I32(s.iterationes, (i32)2);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            exspectatum));

        /* PORTA DIFFERRE (canis cibus): classificatio =
         * cosmetica sola */
        CREDO_VERUM(strcmp(silva_differre_classificare_textus(
            piscina, chorda_ex_literis(fons, piscina),
            s.textus), "cosmetica") == ZEPHYRUM);

        /* PORTA FIDELITATIS: lexare(fructus) emissum ==
         * fructus, octetim */
        {
            Xar* lexemata;

            lexemata = silva_lexare(piscina,
                (constans character*)s.textus.datum,
                s.textus.mensura, ZEPHYRUM);
            CREDO_NON_NIHIL(lexemata);
            CREDO_VERUM(chorda_aequalis(
                silva_lexemata_emittere(piscina, lexemata),
                s.textus));
        }

        /* PORTA IDEMPOTENTIAE: scribere(fructus) intactus */
        {
            FormatorScriptum s2;

            s2 = formator_scribere(piscina, NIHIL,
                (constans character*)s.textus.datum,
                s.textus.mensura);
            CREDO_VERUM(s2.successus);
            CREDO_FALSUM(s2.mutatum);
            CREDO_VERUM(chorda_aequalis(s2.textus, s.textus));
        }
    }

    imprimere("\n--- Probans scribere: arbor composita (R8+R3+R4)"
        " ---\n");
    {
        /* regulae per iterationem componuntur: R8 parametrum
         * findit, R3 brachium movet, R4 custodem iungit; spatia
         * caudae orta iteratione sequente sanantur */
        constans character* fons =
            "interior i32\n"
            "adiuvare (i32 valor) {\n"
            "    si (valor)\n"
            "        redde I;\n"
            "    redde ZEPHYRUM;\n"
            "}\n";
        constans character* exspectatum =
            "interior i32\n"
            "adiuvare (\n"
            "    i32 valor)\n"
            "{\n"
            "    si (valor) redde I;\n"
            "    redde ZEPHYRUM;\n"
            "}\n";
        FormatorScriptum s = _scribere(piscina, fons);

        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_AEQUALIS_I32(s.iterationes, (i32)3);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            exspectatum));
        CREDO_VERUM(strcmp(silva_differre_classificare_textus(
            piscina, chorda_ex_literis(fons, piscina),
            s.textus), "cosmetica") == ZEPHYRUM);
        {
            FormatorScriptum s2;

            s2 = formator_scribere(piscina, NIHIL,
                (constans character*)s.textus.datum,
                s.textus.mensura);
            CREDO_VERUM(s2.successus);
            CREDO_FALSUM(s2.mutatum);
        }
    }

    imprimere("\n--- Probans scribere: radix sedes-usus (macro)"
        " ---\n");
    {
        /* silva_token_radix catenam INVOCATIONIS sequitur -
         * sententia expansa tota in lineam invocationis
         * collabitur, ergo R4 in corpore macro numquam flagrat
         * (definitio multi-linearis CONFORMIS est) */
        constans character* fons =
            "#define CUSTODI(x) \\\n"
            "    si (x) \\\n"
            "        redde ZEPHYRUM;\n"
            "\n"
            "i32\n"
            "probare (\n"
            "    i32 a)\n"
            "{\n"
            "    CUSTODI(a);\n"
            "    redde I;\n"
            "}\n";
        FormatorScriptum s = _scribere(piscina, fons);
        Xar* d = _lint(piscina, fons);

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus, fons));
    }

    imprimere("\n--- Probans scribere: custodia directivarum ---\n");
    {
        /* vexillum-post editio '\n' in linea '#include'
         * inserere vellet - custodia directivarum dilatat
         * (conservativa: etiam initium lineae), plagula
         * INTACTA, divergentia residua superest */
        constans character* fons =
            "/* ================================================== */\n"
            "#include \"probandum.h\"\n"
            "i32 a;\n";
        FormatorScriptum s = _scribere(piscina, fons);
        Xar* d = _lint(piscina, fons);

        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus, fons));
        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)1);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "intervalla") == ZEPHYRUM);
    }

    imprimere("\n--- Probans scribere: titulus in lineam suam"
        " (R1) ---\n");
    {
        FormatorScriptum s = _scribere(piscina,
            "interior i32 adiuvare (\n"
            "    i32 valor)\n"
            "{\n"
            "    redde valor;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "interior i32\n"
            "adiuvare (\n"
            "    i32 valor)\n"
            "{\n"
            "    redde valor;\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: spatium vocationis"
        " (R2) ---\n");
    {
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    facere (I);\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    facere(I);\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: intervalla vexilli ---\n");
    {
        /* una vacua ante (II debitae), nulla post (I debita) */
        FormatorScriptum s = _scribere(piscina,
            "i32 a;\n"
            "\n"
            "/* ==================================================\n"
            " * Titulus\n"
            " * ================================================== */\n"
            "i32 b;\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "i32 a;\n"
            "\n"
            "\n"
            "/* ==================================================\n"
            " * Titulus\n"
            " * ================================================== */\n"
            "\n"
            "i32 b;\n"));
    }

    imprimere("\n--- Probans scribere: functiones conglutinatae"
        " ---\n");
    {
        FormatorScriptum s = _scribere(piscina,
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

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "interior vacuum\n"
            "_a (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"
            "\n"
            "interior vacuum\n"
            "_b (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: vacuae inter functiones"
        " nimis ---\n");
    {
        /* III vacuae -> I per collapsam (III->II) tum
         * inter-functiones (II->I) - convergentia trans
         * iterationes, plagae imbricatae dilatae */
        FormatorScriptum s = _scribere(piscina,
            "interior vacuum\n"
            "_a (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"
            "\n"
            "\n"
            "\n"
            "interior vacuum\n"
            "_b (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_AEQUALIS_I32(s.iterationes, (i32)3);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "interior vacuum\n"
            "_a (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"
            "\n"
            "interior vacuum\n"
            "_b (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: columnae binae (R7) ---\n");
    {
        /* typi dextre ordinati, stella in hiatu, tituli
         * sinistre - forma exemplaris Alvei */
        SilvaContextus* ctx;
        constans character* caput;
        constans character* fons;
        FormatorScriptum s;

        caput = "nomen insignatus integer mensura_probanda;\n";
        ctx = silva_contextus_creare(piscina);
        CREDO_NON_NIHIL(ctx);
        CREDO_VERUM(silva_contextus_latinam_addere(ctx));
        CREDO_VERUM(silva_contextus_praebere(ctx,
            "probandum.h", caput, (i32)strlen(caput)));

        fons =
            "#include \"probandum.h\"\n"
            "nomen structura {\n"
            "    vacuum* memoria;\n"
            "    mensura_probanda cursor;\n"
            "} Probandum;\n";
        s = formator_scribere(piscina, ctx, fons,
            (i32)strlen(fons));

        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "#include \"probandum.h\"\n"
            "nomen structura {\n"
            "              vacuum* memoria;\n"
            "    mensura_probanda  cursor;\n"
            "} Probandum;\n"));
    }

    imprimere("\n--- Probans scribere: aequatio glomeris (R9)"
        " ---\n");
    {
        /* operator ad max(cb) + II: LHS longissimum spatia
         * DUO accipit (decretum "xyz  = valor") */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i32 a;\n"
            "    i32 b_longa;\n"
            "\n"
            "    a = I;\n"
            "    b_longa = II;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i32 a;\n"
            "    i32 b_longa;\n"
            "\n"
            "    a        = I;\n"
            "    b_longa  = II;\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: operatores arboris (R10)"
        " ---\n");
    {
        /* binarium spatiatur, accessus stringitur - et R9
         * glomus eadem iteratione co-ordinat */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i32 a;\n"
            "    i32 d;\n"
            "\n"
            "    a = b+c;\n"
            "    d = e -> f;\n"
            "}\n");

        /* a/d aequilata -> spatium UNUM (decretum novum) */
        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i32 a;\n"
            "    i32 d;\n"
            "\n"
            "    a = b + c;\n"
            "    d = e->f;\n"
            "}\n"));
    }

    imprimere("\n--- Probans R9: sinistra aequilata ---\n");
    {
        /* sinistris aequilatis spatium UNUM (forma nuntii:
         * valor = / valor |= manu scripta) - duplex ad unum
         * stringitur */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    valor  = I;\n"
            "    valor |= II;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    valor = I;\n"
            "    valor |= II;\n"
            "}\n"));
    }

    imprimere("\n--- Probans R9: glomus cadens (LXXII) ---\n");
    {
        /* ordinatio membrum longum trans LXXII truderet ->
         * glomus TOTUM ad minimum cadit (nulla raggedness
         * mixta - membrum breve duplex ad unum stringitur,
         * longum iam minimale manet) */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    a  = I;\n"
            "    b_longum_nomen_valde_extensum = functio_vocata(argumentum_primum_x);\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    a = I;\n"
            "    b_longum_nomen_valde_extensum = functio_vocata(argumentum_primum_x);\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: continuatio indentata"
        " (R11) ---\n");
    {
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    a = b\n"
            "      + c;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    a = b\n"
            "        + c;\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: motus operatoris (R11"
        " bi-span) ---\n");
    {
        /* operator finem lineae claudens ad initium
         * continuationis movetur - editiones duae, una
         * transactione (series lexematum immutata) */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    a = b &&\n"
            "        c;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_AEQUALIS_I32(s.iterationes, (i32)3);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    a = b\n"
            "        && c;\n"
            "}\n"));
    }

    imprimere("\n--- Probans scribere: parametra composita"
        " (R8 -> R7) ---\n");
    {
        /* compositio plena: R8 findit, R7 typos dextre
         * ordinat, stellam in hiatu ponit, titulos sinistre -
         * forma manuali exemplaris _allocare_interna */
        SilvaContextus* ctx;
        constans character* caput;
        constans character* fons;
        FormatorScriptum s;

        caput = "nomen insignatus integer i32;\n";
        ctx = silva_contextus_creare(piscina);
        CREDO_NON_NIHIL(ctx);
        CREDO_VERUM(silva_contextus_latinam_addere(ctx));
        CREDO_VERUM(silva_contextus_praebere(ctx,
            "probandum.h", caput, (i32)strlen(caput)));

        fons =
            "#include \"probandum.h\"\n"
            "vacuum\n"
            "probare (i32 breve_nomen, constans character* longum)\n"
            "{\n"
            "    redde;\n"
            "}\n";
        s = formator_scribere(piscina, ctx, fons,
            (i32)strlen(fons));

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "#include \"probandum.h\"\n"
            "vacuum\n"
            "probare (\n"
            "                   i32  breve_nomen,\n"
            "    constans character* longum)\n"
            "{\n"
            "    redde;\n"
            "}\n"));
        CREDO_VERUM(strcmp(silva_differre_classificare_textus(
            piscina, chorda_ex_literis(fons, piscina),
            s.textus), "cosmetica") == ZEPHYRUM);
        {
            FormatorScriptum s2;

            s2 = formator_scribere(piscina, ctx,
                (constans character*)s.textus.datum,
                s.textus.mensura);
            CREDO_VERUM(s2.successus);
            CREDO_FALSUM(s2.mutatum);
        }
    }

    imprimere("\n--- Probans scribere: catena logica (R17) ---\n");
    {
        /* exemplar Frani: operandum primum ad parenthesim + IV,
         * operatores ad parenthesim + I, comparationes ad
         * max(cb) + I (spatium UNUM post sinistrum longissimum) */
        constans character* fons =
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    si (_lex_aspicere(lex, I) == 'a'\n"
            "        && _lex_aspicere(lex, II) == 'l'\n"
            "        && _lex_aspicere(lex, III) == 's'\n"
            "        && _lex_aspicere(lex, IV) == 'e')\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n";
        constans character* exspectatum =
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    si (   _lex_aspicere(lex, I)   == 'a'\n"
            "        && _lex_aspicere(lex, II)  == 'l'\n"
            "        && _lex_aspicere(lex, III) == 's'\n"
            "        && _lex_aspicere(lex, IV)  == 'e')\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n";
        FormatorScriptum s = _scribere(piscina, fons);

        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_AEQUALIS_I32(s.iterationes, (i32)3);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            exspectatum));

        /* forma perfecta se ipsa conformis (R10-ante cedit
         * vindicationi R17 - sine cessione spatia ordinationis
         * flagrarent) */
        {
            Xar* d = _lint(piscina, exspectatum);

            CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
        }
    }

    imprimere("\n--- Probans scribere: catena in dum ---\n");
    {
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    dum (a == b\n"
            "        && c == d)\n"
            "    {\n"
            "        a = b;\n"
            "    }\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    dum (   a == b\n"
            "         && c == d)\n"
            "    {\n"
            "        a = b;\n"
            "    }\n"
            "}\n"));
    }

    imprimere("\n--- Probans catenam: negativa ---\n");
    {
        /* uni-linearis: catena arta manet (numquam involvimus) */
        Xar* d = _lint(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    si (a == b && c == d) redde;\n"
            "}\n");

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)0);
    }
    {
        /* multi-linearis NON catena (vocatio): R17 tacet, sine
         * suffarcinatione parenthesis */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    si (functio(a,\n"
            "        b))\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
    }

    imprimere("\n--- Probans latinam sine custode ---\n");
    {
        /* DIFFERENTIALIS: lexicon custodem LATINA_H definit,
         * ergo corpus latina.h praebiti sub inclusione
         * supprimitur et typi eius oraculo invisibiles manent
         * ('typus * x;' ambiguum, R7 caecum). Custode
         * neutralizato typus registratur et stella prava
         * FLAGRAT. */
        SilvaContextus* ctx;
        constans character* caput;
        constans character* fons;
        Xar* d;

        caput =
            "#ifndef LATINA_H\n"
            "#define LATINA_H\n"
            "nomen signatus character i8_probandum;\n"
            "#endif\n";
        ctx = silva_contextus_creare(piscina);
        CREDO_NON_NIHIL(ctx);
        CREDO_VERUM(silva_contextus_latinam_addere(ctx));
        CREDO_VERUM(formator_latinam_praebere(ctx, piscina,
            caput, (i32)strlen(caput)));

        fons =
            "#include \"latina.h\"\n"
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i8_probandum * a;\n"
            "}\n";
        d = formator_lint(piscina, ctx, fons,
            (i32)strlen(fons));

        /* stella prava + titulus pravus - ambo flagrant (sine
         * custode neutralizato: ZEPHYRUM, regio ambigua) */
        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)2);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "columnae-binae") == ZEPHYRUM);
        CREDO_VERUM(strcmp(_divergentia(d, 1)->regula,
            "columnae-binae") == ZEPHYRUM);
    }

    imprimere("\n--- Probans ambiguum praetermissum ---\n");
    {
        /* '(typus_ignotus)-I' = AMBIGUUS (conversio aut
         * binarium - oraculum non decernit). Ambulare ambas
         * lectiones spatia contradictoria imponeret
         * (oscillatio evolutionis II); regio ambigua non
         * iudicatur - stabilis et intacta */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (\n"
            "    i32 positus)\n"
            "{\n"
            "    si (positus != (ignotum_genus)-I) redde;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
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
