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

s32
principale (vacuum)
{
        b32  praeteritus;
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

        caput  = "nomen insignatus integer mensura_probanda;\n";
        ctx    = silva_contextus_creare(piscina);
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
        FormatorScriptum  s = _scribere(piscina, fons);
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
        FormatorScriptum  s = _scribere(piscina, fons);
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
          FormatorScriptum  s;

        caput  = "nomen insignatus integer mensura_probanda;\n";
        ctx    = silva_contextus_creare(piscina);
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

    imprimere("\n--- Probans R9: declarationes initiatae ---\n");
    {
        /* porta G1 clausa (nuntium 770): '=' initiatorum
         * glomeri participat - sinistrum = declarator (nomen),
         * latitudo mixta -> max + II */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    s64 shifted = I;\n"
            "    s64 mask = II;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    s64 shifted  = I;\n"
            "    s64 mask     = II;\n"
            "}\n"));
    }
    {
        /* declaratio initiata + assignatio contigua = glomus
         * UNUM (glomera avida trans limitem) */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i32 a = I;\n"
            "    b = II;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i32 a  = I;\n"
            "    b      = II;\n"
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
          FormatorScriptum  s;

        caput  = "nomen insignatus integer i32;\n";
        ctx    = silva_contextus_creare(piscina);
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

    imprimere("\n--- Probans typum lexici (radix sanata) ---\n");
    {
        /* Olim DIFFERENTIALIS custodis: formator_latinam_praebere
         * custodem LATINA_H vacuabat ut typi plagulae praebitae
         * registrarentur (involucrum). Radix sanata (94899b2):
         * typedefs LEXICI IPSIUS oraculo praeonerantur -
         * involucrum retiratum. Typus per lexicon planum visibilis:
         * stella prava + titulus pravus flagrant (ante sanationem:
         * ZEPHYRUM ordines, regio ambigua R7 caeca). */
            SilvaContextus* ctx;
        constans character* fons;
                       Xar* d;

        ctx = silva_contextus_creare(piscina);
        CREDO_NON_NIHIL(ctx);
        CREDO_VERUM(silva_contextus_latinam_addere(ctx));
        CREDO_VERUM(silva_contextus_lexicon_addere(ctx,
            "probandum.h",
            "typedef signed char i8_probandum;\n",
            (i32)strlen("typedef signed char i8_probandum;\n")));

        fons =
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i8_probandum * a;\n"
            "}\n";
        d = formator_lint(piscina, ctx, fons,
            (i32)strlen(fons));

        CREDO_AEQUALIS_I32((i32)xar_numerus(d), (i32)2);
        CREDO_VERUM(strcmp(_divergentia(d, 0)->regula,
            "columnae-binae") == ZEPHYRUM);
        CREDO_VERUM(strcmp(_divergentia(d, 1)->regula,
            "columnae-binae") == ZEPHYRUM);
    }

    imprimere("\n--- Probans unionem in situ (stabilis) ---\n");
    {
        /* venatio js_lexema: unio uni-linearis in bloco localium
         * - specificator bloccalis ordinem non participat, membra
         * interna eadem linea non co-ordinantur; stabilis */
        FormatorScriptum s = _scribere(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i32 solum;\n"
            "    unio { i32 a; i32 b; } u;\n"
            "}\n");

        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
    }

    imprimere("\n--- Probans sinistrum macro-expansum ---\n");
    {
        /* venatio coloratio 999: extensio sinistri expansi
         * MENTITUR (radix ad invocationem collabitur) - tractio
         * tolerans dilatatur, plagula numquam recusatur */
        FormatorScriptum s = _scribere(piscina,
            "#define LOCUS(t) ((t)->datum)\n"
            "vacuum\n"
            "probare (\n"
            "    vacuum* t)\n"
            "{\n"
            "    quies = I;\n"
            "    LOCUS(t) = II;\n"
            "}\n");

        CREDO_VERUM(s.successus);
    }

    imprimere("\n--- Probans prototypa (porta 2a clausa) ---\n");
    {
        /* casus arx_caeli Frani: typi parametrorum prototypi
         * dextre ordinandi (R7), spatium definitionis (R2) */
            SilvaContextus* ctx;
        constans character* caput;
        constans character* fons;
          FormatorScriptum  s;

        caput =
            "nomen structura ArcCaeli ArcCaeli;\n"
            "nomen vacuum (*FunctioLigaminis)(vacuum*);\n"
            "nomen insignatus integer i32;\n";
        ctx = silva_contextus_creare(piscina);
        CREDO_NON_NIHIL(ctx);
        CREDO_VERUM(silva_contextus_latinam_addere(ctx));
        CREDO_VERUM(silva_contextus_praebere(ctx,
            "probandum.h", caput, (i32)strlen(caput)));

        fons =
            "#include \"probandum.h\"\n"
            "vacuum\n"
            "arx_ponere_ligamen(\n"
            "    ArcCaeli*           arc,\n"
            "    FunctioLigaminis ligamen,\n"
            "    vacuum*             datum);\n";
        s = formator_scribere(piscina, ctx, fons,
            (i32)strlen(fons));

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "#include \"probandum.h\"\n"
            "vacuum\n"
            "arx_ponere_ligamen (\n"
            "            ArcCaeli* arc,\n"
            "    FunctioLigaminis  ligamen,\n"
            "              vacuum* datum);\n"));

        /* prototypum uni-lineare: R8 findit, R7 ordinat */
        fons =
            "#include \"probandum.h\"\n"
            "vacuum\n"
            "facere(i32 a, i32 b);\n";
        s = formator_scribere(piscina, ctx, fons,
            (i32)strlen(fons));

        CREDO_VERUM(s.successus);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "#include \"probandum.h\"\n"
            "vacuum\n"
            "facere (\n"
            "    i32 a,\n"
            "    i32 b);\n"));

        /* monstrator functionis: NON prototypum (internum
         * PARENTHESIS) - intactus */
        fons =
            "#include \"probandum.h\"\n"
            "nomen vacuum (*FunctioX)(i32 a, i32 b);\n";
        s = formator_scribere(piscina, ctx, fons,
            (i32)strlen(fons));

        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
    }

    imprimere("\n--- Probans vexillum post vexillum ---\n");
    {
        /* vexilla consecutiva: regula ANTE hiatum possidet (II
         * vacuae) - post I volebat = bellum aeternum unius
         * octeti (venatio persistentiae). II vacuae stabiles. */
        FormatorScriptum s = _scribere(piscina,
            "/* ==================================================\n"
            " * Caput\n"
            " * ================================================== */\n"
            "\n"
            "\n"
            "/* ==================================================\n"
            " * Sectio\n"
            " * ================================================== */\n"
            "\n"
            "i32 a;\n");

        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
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

    imprimere("\n--- Probans vocationem in corpore macri "
        "(geometria fida) ---\n");
    {
        /* vocatio INTRA corpus macri: lexemata omnia ad sedem
         * invocationis collabuntur - hiatus mensuratus mendax
         * (negativus; venatio probatio_silva_puritas: XLVI ordines
         * falsi e macris CREDO). Vocatio corporis NON iudicatur;
         * vocatio honesta eadem plagula iudicatur adhuc (contra
         * sectionem nimiam). */
        constans character* fons =
            "#define ADFIRMA(x) probare((x), 1)\n"
            "\n"
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    ADFIRMA(2);\n"
            "    probare (3);\n"
            "}\n";
        Xar* d                   = _lint(piscina, fons);
        i32  numerus_vocationis  = ZEPHYRUM;
        i32  linea_inventa       = ZEPHYRUM;
        i32  k;

        CREDO_NON_NIHIL(d);
        per (k = ZEPHYRUM; k < xar_numerus(d); k++)
        {
            si (strcmp(_divergentia(d, k)->regula,
                    "spatium-definitionis") == ZEPHYRUM)
            {
                numerus_vocationis  = numerus_vocationis + I;
                linea_inventa       = _divergentia(d, k)->linea;
            }
        }
        CREDO_AEQUALIS_I32(numerus_vocationis, (i32)1);
        CREDO_AEQUALIS_I32(linea_inventa, (i32)7);
    }

    imprimere("\n--- Probans ambitum nominatum (-intra) ---\n");
    {
        /* duae functiones male formatae cum prototypis; ambitus
         * 'b' b SOLAM tangit (prototypum, commentarium ducens,
         * intervalla supra - ad functionem infra pertinent), 'a'
         * octetim intacta manet. Nomina, non lineae: lineae inter
         * iterationes labuntur (prototypum b scissum lineam
         * definitionis movet - titulus eam denuo invenit). */
        constans character* fons =
            "vacuum a(vacuum);\n"
            "vacuum b(vacuum);\n"
            "\n"
            "/* a */\n"
            "vacuum\n"
            "a(vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"
            "\n"
            "\n"
            "\n"
            "/* b */\n"
            "vacuum\n"
            "b(vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n";
                  chorda  nomina[II];
                     b32  inventae[II];
           FormatorIntra  intra;
        FormatorScriptum  s;
        FormatorScriptum  s_tota;
                     Xar* d;
                     i32  k;

        intra.functiones  = nomina;
        intra.inventae    = inventae;

        /* ambitus b */
        nomina[ZEPHYRUM]  = chorda_ex_literis("b", piscina);
        intra.numerus     = I;
        s = formator_scribere_intra(piscina, NIHIL, fons,
            (i32)strlen(fons), &intra);
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(inventae[ZEPHYRUM]);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum a(vacuum);\n"
            "vacuum\n"
            "b (vacuum);\n"
            "\n"
            "/* a */\n"
            "vacuum\n"
            "a(vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"
            "\n"
            "/* b */\n"
            "vacuum\n"
            "b (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"));

        /* ambitus a: b et intervalla supra b intacta */
        nomina[ZEPHYRUM] = chorda_ex_literis("a", piscina);
        s = formator_scribere_intra(piscina, NIHIL, fons,
            (i32)strlen(fons), &intra);
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "a (vacuum);\n"
            "vacuum b(vacuum);\n"
            "\n"
            "/* a */\n"
            "vacuum\n"
            "a (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"
            "\n"
            "\n"
            "\n"
            "/* b */\n"
            "vacuum\n"
            "b(vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"));

        /* ambo = scriptura tota */
        nomina[I]      = chorda_ex_literis("b", piscina);
        intra.numerus  = II;
        s = formator_scribere_intra(piscina, NIHIL, fons,
            (i32)strlen(fons), &intra);
        s_tota = _scribere(piscina, fons);
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s_tota.successus);
        CREDO_VERUM(chorda_aequalis(s.textus, s_tota.textus));

        /* lint intra b: ordines omnes intra extenta b (prototypum
         * linea 2; definitio 10-18 - a linea post '}' functionis
         * a) */
        nomina[ZEPHYRUM]  = chorda_ex_literis("b", piscina);
        intra.numerus     = I;
        d = formator_lint_intra(piscina, NIHIL, fons,
            (i32)strlen(fons), &intra);
        CREDO_NON_NIHIL(d);
        CREDO_VERUM(xar_numerus(d) > (i32)ZEPHYRUM);
        per (k = ZEPHYRUM; k < xar_numerus(d); k += I)
        {
            i32 lin;

            lin = _divergentia(d, k)->linea;
            CREDO_VERUM(lin == (i32)II
                || (lin >= (i32)10 && lin <= (i32)18));
        }

        /* ignota: recusatio clamosa, textus originalis */
        nomina[ZEPHYRUM] = chorda_ex_literis("nemo", piscina);
        s = formator_scribere_intra(piscina, NIHIL, fons,
            (i32)strlen(fons), &intra);
        CREDO_FALSUM(s.successus);
        CREDO_FALSUM(s.mutatum);
        CREDO_FALSUM(inventae[ZEPHYRUM]);
        CREDO_NON_NIHIL(s.querela);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus, fons));

        /* inventae NIHIL: machina suas facit - recusat aeque */
        intra.inventae = NIHIL;
        s = formator_scribere_intra(piscina, NIHIL, fons,
            (i32)strlen(fons), &intra);
        CREDO_FALSUM(s.successus);
        CREDO_NON_NIHIL(s.querela);
    }

    imprimere("\n--- Probans custodiam LXXII glomerum (R7/R17) ---\n");
    {
        /* ordinatio numquam lineam ultra LXXII trudit; linea TOTA
         * metitur (commentarium caudae inclusum), non extensio nodi.
         * Venatio 2026-09-01: -scribere lineas longas CREABAT
         * (legatus.c membra LXXIV-LXXXV post R7, semantica.c catenae
         * LXXV-LXXXV post R17 A/C). */
        FormatorScriptum  s;
                     Xar* d;
                     i32  n_glomus;
                     i32  n_alia;
                     i32  k;

        /* R7: glomus cadit - nihil ordinatur, ordo LINT unus causam
         * nominat */
        s = _scribere(piscina,
            "nomen structura {\n"
            "    i32 genus;                 /* \"functio\"/\"vari"
            "abile\"/\"typus\" etc. */\n"
            "    vacuum* parsura;\n"
            "    b32 mortuus;               /* superpositio: plag"
            "ula re-iudicat */\n"
            "} Res;\n");
        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "nomen structura {\n"
            "    i32 genus;                 /* \"functio\"/\"vari"
            "abile\"/\"typus\" etc. */\n"
            "    vacuum* parsura;\n"
            "    b32 mortuus;               /* superpositio: plag"
            "ula re-iudicat */\n"
            "} Res;\n"));

        d = _lint(piscina,
            "nomen structura {\n"
            "    i32 genus;                 /* \"functio\"/\"vari"
            "abile\"/\"typus\" etc. */\n"
            "    vacuum* parsura;\n"
            "    b32 mortuus;               /* superpositio: plag"
            "ula re-iudicat */\n"
            "} Res;\n");
        CREDO_NON_NIHIL(d);
        n_glomus  = ZEPHYRUM;
        n_alia    = ZEPHYRUM;
        per (k = ZEPHYRUM; k < xar_numerus(d); k += I)
        {
            si (strcmp(_divergentia(d, k)->regula,
                    "columnae-binae") != ZEPHYRUM)
            {
                perge;
            }
            si (strncmp(_divergentia(d, k)->nuntius,
                    "glomus non ordinatum", 20) == ZEPHYRUM)
            {
                n_glomus += I;
                CREDO_AEQUALIS_I32(
                    _divergentia(d, k)->numerus_emendationum,
                    (i32)0);
            }
            alioquin
            {
                n_alia += I;
            }
        }
        CREDO_AEQUALIS_I32(n_glomus, (i32)1);
        CREDO_AEQUALIS_I32(n_alia, (i32)0);

        /* R17 A: linea prima LXXII plena - spatia tria non inseruntur;
         * B operatorem movet */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    dum (nodus != NIHIL && nodus->genus == (s32)SILV"
            "A_C89_GENUS_AMBIGUUS\n"
            "        && nodus->pater != NIHIL)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    dum (nodus != NIHIL && nodus->genus == (s32)SILV"
            "A_C89_GENUS_AMBIGUUS\n"
            "         && nodus->pater != NIHIL)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n"));

        /* R17 C: ordinatio lineam primam ad LXXXIII truderet - glomus
         * cadit ad cb + I (linea I intacta, linea II tracta) */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (   a == operandum_longissimum_valde_xxxxxxxx"
            "xxxxxxxxxxxxxxxxxx\n"
            "        && ccccccccccccccc  == d)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (   a == operandum_longissimum_valde_xxxxxxxx"
            "xxxxxxxxxxxxxxxxxx\n"
            "        && ccccccccccccccc == d)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n"));

        /* R17 C: eadem forma, linea prima LV - ordinatur (custodia non
         * cadit) */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (   a == operandum_valde_xxxxxxxxxxxxxxxxxxxx"
            "xxx\n"
            "        && ccccccccccccccc  == d)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (   a               == operandum_valde_xxxxxx"
            "xxxxxxxxxxxxxxxxx\n"
            "        && ccccccccccccccc == d)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n"));
    }

    imprimere("\n--- Probans fracturam linearum (R16 FIX) ---\n");
    {
        /* ordo longitudo-lxxii emendationem suam fert ubi candidatus
         * exstat: post virgulam / post '=' / ante operatorem binarium,
         * lexematis crudis (macro-honestum). Logici profunditate minima
         * vincunt; aliter dextimus cadens; nullo cadente sinistimus.
         * Indentatio: linea prima ca + IV; continuata gradus suus, nisi
         * candidatus artius ligat (+ IV). R11/R17 iteratione sequenti
         * perficiunt. */
        FormatorScriptum  s;
                     Xar* d;
                     i32  k;
                     b32  frangibilis_visa;

        /* argumenta vocationis: virgula dextima cadens; continuatio ca
         * + IV */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde functio_longa(argumentum_primum, argumentu"
            "m_secundum, tertium);\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde functio_longa(argumentum_primum, argumentu"
            "m_secundum,\n"
            "        tertium);\n"
            "}\n"));

        /* catena in si: ante && (logicus vincit); R17 A/B iteratione
         * sequenti */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (alpha_longissima == beta_longissima && gamma"
            "_longissima == delta_l)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (   alpha_longissima == beta_longissima\n"
            "        && gamma_longissima == delta_l)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n"));

        /* declaratio initiata: virgula (dextima cadens) prae =
         * (profundior sed dextior) */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    constans character* titulus_longus = functio(arg"
            "umentum_unum, alterum);\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    constans character* titulus_longus = functio(arg"
            "umentum_unum,\n"
            "        alterum);\n"
            "}\n"));

        /* logicus profunditate minima prae virgula interiore */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (alpha && functio(argumentum_primum, argument"
            "um_secundum, tertium_x))\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (   alpha\n"
            "        && functio(argumentum_primum, argumentum_sec"
            "undum, tertium_x))\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n"));

        /* fractura una sufficit si cauda cadit */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde functio_longa(argumentum_primum, argumentu"
            "m_secundum, tertium_longum, quartum_longum, quintum_"
            "longum);\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde functio_longa(argumentum_primum, argumentu"
            "m_secundum,\n"
            "        tertium_longum, quartum_longum, quintum_long"
            "um);\n"
            "}\n"));

        /* fracturae binae: cauda argumentorum eodem gradu manet */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde functio_longa(argumentum_primum, argumentu"
            "m_secundum, tertium_longum, quartum_longum, quintum_"
            "longum, sextum_longum, septimum_longum);\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde functio_longa(argumentum_primum, argumentu"
            "m_secundum,\n"
            "        tertium_longum, quartum_longum, quintum_long"
            "um, sextum_longum,\n"
            "        septimum_longum);\n"
            "}\n"));

        /* nullus candidatus cadit: sinistimus - residuum honestum
         * manet */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde functio_cum_titulo_valde_valde_valde_valde"
            "_valde_valde_longissimo(a, b);\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde functio_cum_titulo_valde_valde_valde_valde"
            "_valde_valde_longissimo(a,\n"
            "        b);\n"
            "}\n"));

        /* comparatio intra ramum catenae ||: gradus interior (+ IV) */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (   sem == NIHIL\n"
            "        || titulus_valde_longissimus_identificator_a"
            "lter == NIHIL_ALIUD_X\n"
            "        || datum->titulus_longissimus->mensura_valde"
            "_longa_ipsa == ZEPHYRUM)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (   sem == NIHIL\n"
            "        || titulus_valde_longissimus_identificator_a"
            "lter\n"
            "            == NIHIL_ALIUD_X\n"
            "        || datum->titulus_longissimus->mensura_valde"
            "_longa_ipsa\n"
            "            == ZEPHYRUM)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n"));

        /* catena + eodem gradu */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    summa = primus_valor_longus + secundus_valor_lon"
            "gus + tertius_valor_longus + quartus;\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    summa = primus_valor_longus + secundus_valor_lon"
            "gus\n"
            "        + tertius_valor_longus + quartus;\n"
            "}\n"));

        /* cauda commentarii sola transgreditur: nulla fractura (codex
         * cadit) */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    x = g(a, b);  /* commentarium longissimum quod l"
            "imitem excedit certe */\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    x = g(a, b);  /* commentarium longissimum quod l"
            "imitem excedit certe */\n"
            "}\n"));

        /* directiva: nulla fractura */
        s = _scribere(piscina,
            "#define MACRO_LONGA(a, b) ((a) + (b) + (a) * (b) - ("
            "a) / (b) + (a) % (b) + 1)\n"
            "\n"
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "#define MACRO_LONGA(a, b) ((a) + (b) + (a) * (b) - ("
            "a) / (b) + (a) % (b) + 1)\n"
            "\n"
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"));

        /* littera chordae sola: nullus candidatus */
        s = _scribere(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    imprimere(\"littera chordae longissima quae null"
            "o modo frangi potest hic\");\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    imprimere(\"littera chordae longissima quae null"
            "o modo frangi potest hic\");\n"
            "}\n"));

        /* corpus macri continuatum: nulla fractura */
        s = _scribere(piscina,
            "#define MACRO(a, b) \\\n"
            "    ((a) + (b) + (a) * (b) - (a) / (b) + (a) % (b) +"
            " (a) - (b) + 1)\n"
            "\n"
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n");
        CREDO_VERUM(s.successus);
        CREDO_FALSUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "#define MACRO(a, b) \\\n"
            "    ((a) + (b) + (a) * (b) - (a) / (b) + (a) % (b) +"
            " (a) - (b) + 1)\n"
            "\n"
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"));

        /* prototypum longum: R8 findit, fractura cedit regulae lineam
         * novam inserenti - nulla linea vacua inter parametra (venatio
         * semantica 4861) */
        s = _scribere(piscina,
            "vacuum _fluxus_canonicum_ligamen_longum(vacuum* cont"
            "extus, constans vacuum* nodus);\n");
        CREDO_VERUM(s.successus);
        CREDO_VERUM(s.mutatum);
        CREDO_VERUM(_textus_aequalis(piscina, s.textus,
            "vacuum\n"
            "_fluxus_canonicum_ligamen_longum (\n"
            "             vacuum* contextus,\n"
            "    constans vacuum* nodus);\n"));

        /* ordo longitudinis: frangibilis armatus (I emendatio, nuntius
         * proprius) contra residuum (0, nuntius planus) */
        d = _lint(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    redde functio_longa(argumentum_primum, argumentu"
            "m_secundum, tertium);\n"
            "}\n");
        CREDO_NON_NIHIL(d);
        frangibilis_visa = FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(d); k += I)
        {
            si (strcmp(_divergentia(d, k)->regula,
                    "longitudo-lxxii") != ZEPHYRUM)
            {
                perge;
            }
            CREDO_AEQUALIS_I32(
                _divergentia(d, k)->numerus_emendationum, (i32)1);
            CREDO_VERUM(strcmp(_divergentia(d, k)->nuntius,
                "linea nimis longa - frangibilis") == ZEPHYRUM);
            frangibilis_visa = VERUM;
        }
        CREDO_VERUM(frangibilis_visa);
        d = _lint(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    imprimere(\"littera chordae longissima quae null"
            "o modo frangi potest hic\");\n"
            "}\n");
        CREDO_NON_NIHIL(d);
        per (k = ZEPHYRUM; k < xar_numerus(d); k += I)
        {
            si (strcmp(_divergentia(d, k)->regula,
                    "longitudo-lxxii") != ZEPHYRUM)
            {
                perge;
            }
            CREDO_AEQUALIS_I32(
                _divergentia(d, k)->numerus_emendationum, (i32)0);
            CREDO_VERUM(strcmp(_divergentia(d, k)->nuntius,
                "linea nimis longa") == ZEPHYRUM);
        }
    }

    imprimere("\n--- Probans ancoras in nuntiis ordinationis ---\n");
    {
        /* nuntius ancoram nominat (membrum ad quod ceterae
         * ordinantur) et regulam ipsam - lector eam non iterum
         * derivet (desideratum 01M1FMEKZG III). */
        Xar* d;
        i32  k;
        b32  visa;

        /* R7: typus latissimus */
        d = _lint(piscina,
            "nomen structura {\n"
            "    vacuum* memoria;\n"
            "    i32 cursor;\n"
            "} Probandum;\n");
        CREDO_NON_NIHIL(d);
        visa = FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(d); k += I)
        {
            si (   strcmp(_divergentia(d, k)->regula, "columnae-binae")
                    == ZEPHYRUM
                && strstr(_divergentia(d, k)->nuntius,
                    "(ancora 'vacuum' l.2: typus latissimus)")
                    != NIHIL)
            {
                visa = VERUM;
            }
        }
        CREDO_VERUM(visa);

        /* R9: sinistrum longissimum + II */
        d = _lint(piscina,
            "vacuum\n"
            "probare (vacuum)\n"
            "{\n"
            "    i32 a;\n"
            "    i32 b_longa;\n"
            "\n"
            "    a = I;\n"
            "    b_longa = II;\n"
            "}\n");
        CREDO_NON_NIHIL(d);
        visa = FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(d); k += I)
        {
            si (   strcmp(_divergentia(d, k)->regula,
                    "aequatio-assignationum") == ZEPHYRUM
                && strstr(_divergentia(d, k)->nuntius,
                    "(ancora 'b_longa' l.8 + II)") != NIHIL)
            {
                visa = VERUM;
            }
        }
        CREDO_VERUM(visa);

        /* R17 C: sinistrum longissimum catenae + I */
        d = _lint(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    si (   alpha_longissima == beta\n"
            "        && g  == d)\n"
            "    {\n"
            "        redde;\n"
            "    }\n"
            "}\n");
        CREDO_NON_NIHIL(d);
        visa = FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(d); k += I)
        {
            si (   strcmp(_divergentia(d, k)->regula, "catena-logica")
                    == ZEPHYRUM
                && strstr(_divergentia(d, k)->nuntius,
                    "(ancora 'alpha_longissima' l.4 + I)") != NIHIL)
            {
                visa = VERUM;
            }
        }
        CREDO_VERUM(visa);

        /* R11: sententia ancorans + IV */
        d = _lint(piscina,
            "vacuum\n"
            "f (vacuum)\n"
            "{\n"
            "    fructus = functio(a,\n"
            "    b);\n"
            "}\n");
        CREDO_NON_NIHIL(d);
        visa = FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(d); k += I)
        {
            si (   strcmp(_divergentia(d, k)->regula, "continuatio")
                    == ZEPHYRUM
                && strstr(_divergentia(d, k)->nuntius,
                    "(ancora 'fructus' l.4 + IV)") != NIHIL)
            {
                visa = VERUM;
            }
        }
        CREDO_VERUM(visa);
    }

    imprimere("\n--- Probans extenta functionum (-lineae) ---\n");
    {
        /* idem ambitus quem -intra iudicat: prototypum a [1,1], b
         * [2,2]; definitio a [3,9] (linea post prototypum b), b [10,18]
         * (intervalla supra ad b pertinent) */
        constans character* fons =
            "vacuum a(vacuum);\n"
            "vacuum b(vacuum);\n"
            "\n"
            "/* a */\n"
            "vacuum\n"
            "a(vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n"
            "\n"
            "\n"
            "\n"
            "/* b */\n"
            "vacuum\n"
            "b(vacuum)\n"
            "{\n"
            "    redde;\n"
            "}\n";
                     Xar* x;
        FormatorExtentum* e;

        x = formator_extenta(piscina, NIHIL, fons, (i32)strlen(fons));
        CREDO_NON_NIHIL(x);
        CREDO_AEQUALIS_I32(xar_numerus(x), (i32)4);
        e = (FormatorExtentum*)xar_obtinere(x, ZEPHYRUM);
        CREDO_VERUM(chorda_aequalis_literis(e->titulus, "a"));
        CREDO_AEQUALIS_I32(e->linea_a, (i32)1);
        CREDO_AEQUALIS_I32(e->linea_b, (i32)1);
        CREDO_FALSUM(e->definitio);
        e = (FormatorExtentum*)xar_obtinere(x, I);
        CREDO_VERUM(chorda_aequalis_literis(e->titulus, "b"));
        CREDO_AEQUALIS_I32(e->linea_a, (i32)2);
        CREDO_AEQUALIS_I32(e->linea_b, (i32)2);
        e = (FormatorExtentum*)xar_obtinere(x, II);
        CREDO_VERUM(chorda_aequalis_literis(e->titulus, "a"));
        CREDO_AEQUALIS_I32(e->linea_a, (i32)3);
        CREDO_AEQUALIS_I32(e->linea_b, (i32)9);
        CREDO_VERUM(e->definitio);
        e = (FormatorExtentum*)xar_obtinere(x, III);
        CREDO_VERUM(chorda_aequalis_literis(e->titulus, "b"));
        CREDO_AEQUALIS_I32(e->linea_a, (i32)10);
        CREDO_AEQUALIS_I32(e->linea_b, (i32)18);

        /* parsura fracta: vacuum, non NIHIL */
        x = formator_extenta(piscina, NIHIL, "vacuum f(", (i32)9);
        CREDO_NON_NIHIL(x);
        CREDO_AEQUALIS_I32(xar_numerus(x), (i32)0);
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
