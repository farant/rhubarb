/* probatio_silva_expansio.c - Probationes expansoris: Chunk A
 * (acta, directivae, definitiones) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_expandere.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior SilvaExpansio*
_praeparare (Piscina* piscina, constans character* fons, Xar** reliqua_out,
             Xar** directivae_out)
{
    SilvaExpansio* exp;
    Xar* lexemata;

    exp = silva_expansio_creare(piscina);
    silva_fons_addere(exp, "probatio.c", FALSUM);
    lexemata = silva_lexare(piscina, fons, (i32)strlen(fons), ZEPHYRUM);
    *reliqua_out = silva_expansio_directivas_processare(exp, lexemata,
        directivae_out);
    redde exp;
}

interior chorda
_ch (constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = literis;
    c.datum = u.m;
    c.mensura = (i32)strlen(literis);
    redde c;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_expansio", 262144);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: definitio simplex
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaMacroDef* def;
        Xar* reliqua;
        Xar* directivae;

        imprimere("\n--- Probans definitionem simplicem ---\n");

        exp = _praeparare(piscina, "#define X 42\nint y;", &reliqua, &directivae);

        def = silva_expansio_quaerere(exp, _ch("X"));
        CREDO_NON_NIHIL (def);
        CREDO_FALSUM (def->est_functio);
        CREDO_AEQUALIS_I32 (xar_numerus(def->corpus), I);
        CREDO_AEQUALIS_I32 (def->linea_def, I);
        CREDO_AEQUALIS_S32 (def->fons_index, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(exp->acta), I);

        /* reliqua: int y ; EOF (directiva consumpta) */
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), IV);
        CREDO_AEQUALIS_I32 (xar_numerus(directivae), I);
    }


    /* ========================================================
     * PROBARE: functio-similis et regula adiacentiae
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaMacroDef* def;
        Xar* reliqua;

        imprimere("\n--- Probans functionem-similem ---\n");

        exp = _praeparare(piscina, "#define F(a, b) a + b", &reliqua, NIHIL);
        def = silva_expansio_quaerere(exp, _ch("F"));
        CREDO_NON_NIHIL (def);
        CREDO_VERUM (def->est_functio);
        CREDO_AEQUALIS_I32 (xar_numerus(def->parametra), II);
        CREDO_AEQUALIS_I32 (xar_numerus(def->corpus), III);  /* a + b */

        /* spatium ante ( : NON functio-similis */
        exp = _praeparare(piscina, "#define G (x)", &reliqua, NIHIL);
        def = silva_expansio_quaerere(exp, _ch("G"));
        CREDO_NON_NIHIL (def);
        CREDO_FALSUM (def->est_functio);
        CREDO_AEQUALIS_I32 (xar_numerus(def->corpus), III);  /* ( x ) */

        /* variadica (C99) signata; "..." parametrum __VA_ARGS__ appendit */
        exp = _praeparare(piscina, "#define V(a, ...) a", &reliqua, NIHIL);
        def = silva_expansio_quaerere(exp, _ch("V"));
        CREDO_NON_NIHIL (def);
        CREDO_VERUM (def->est_functio);
        CREDO_VERUM (def->est_variadica);
        CREDO_AEQUALIS_I32 (xar_numerus(def->parametra), II);
    }


    /* ========================================================
     * PROBARE: undef et replay actorum
     * ======================================================== */

    {
        SilvaExpansio* exp;
        Xar* reliqua;
        TabulaDispersa* tunc;
        vacuum* valor;

        imprimere("\n--- Probans undef et acta ---\n");

        exp = _praeparare(piscina,
            "#define A 1\n#define B 2\n#undef A\nint x;",
            &reliqua, NIHIL);

        /* tabula viva: A deleta, B manet */
        CREDO_NIHIL (silva_expansio_quaerere(exp, _ch("A")));
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp, _ch("B")));
        CREDO_AEQUALIS_I32 (xar_numerus(exp->acta), III);

        /* replay: ad lineam II solum A definita erat */
        tunc = silva_expansio_macros_ad_lineam(exp, piscina, ZEPHYRUM, II);
        CREDO_VERUM (tabula_dispersa_invenire(tunc, _ch("A"), &valor));
        CREDO_FALSUM (tabula_dispersa_invenire(tunc, _ch("B"), &valor));

        /* ad lineam III: A et B */
        tunc = silva_expansio_macros_ad_lineam(exp, piscina, ZEPHYRUM, III);
        CREDO_VERUM (tabula_dispersa_invenire(tunc, _ch("A"), &valor));
        CREDO_VERUM (tabula_dispersa_invenire(tunc, _ch("B"), &valor));

        /* ad lineam IV: A deleta */
        tunc = silva_expansio_macros_ad_lineam(exp, piscina, ZEPHYRUM, IV);
        CREDO_FALSUM (tabula_dispersa_invenire(tunc, _ch("A"), &valor));
        CREDO_VERUM (tabula_dispersa_invenire(tunc, _ch("B"), &valor));
    }


    /* ========================================================
     * PROBARE: regressio defectus arbor2 - directivae columnae I
     * consecutivae (classis defectus hic irrepraesentabilis)
     * ======================================================== */

    {
        SilvaExpansio* exp;
        Xar* reliqua;

        imprimere("\n--- Probans regressionem defectus directivarum ---\n");

        exp = _praeparare(piscina, "#define A 1\n#define B 2", &reliqua, NIHIL);
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp, _ch("A")));
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp, _ch("B")));

        /* reliqua = solum EOF */
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), I);
    }


    /* ========================================================
     * PROBARE: directiva laminata (linea logica trans continuationem)
     * ======================================================== */

    {
        SilvaExpansio* exp;
        SilvaMacroDef* def;
        Xar* reliqua;

        imprimere("\n--- Probans directivam laminatam ---\n");

        exp = _praeparare(piscina, "#define X \\\n 42\nint y;", &reliqua, NIHIL);
        def = silva_expansio_quaerere(exp, _ch("X"));
        CREDO_NON_NIHIL (def);
        CREDO_AEQUALIS_I32 (xar_numerus(def->corpus), I);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            (*(SilvaToken**)xar_obtinere(def->corpus, 0))->valor, "42");
    }


    /* ========================================================
     * PROBARE: cancellum extra initium lineae NON directiva
     * ======================================================== */

    {
        SilvaExpansio* exp;
        Xar* reliqua;

        imprimere("\n--- Probans cancellum medium ---\n");

        exp = _praeparare(piscina, "x # define Y 1", &reliqua, NIHIL);
        CREDO_NIHIL (silva_expansio_quaerere(exp, _ch("Y")));
        CREDO_AEQUALIS_I32 (xar_numerus(exp->acta), ZEPHYRUM);
        /* omnia lexemata manent: x # define Y 1 EOF */
        CREDO_AEQUALIS_I32 (xar_numerus(reliqua), VI);
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
