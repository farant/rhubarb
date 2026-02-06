/* probatio_lapifex_generare.c - Probationes Generatoris Grammaticae */

#include "latina.h"
#include "lapifex_generare.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "credo.h"
#include <stdio.h>

/* Grammatica arithmetica probationis */
hic_manens constans character* GRAMMATICA_STML =
    "<grammatica>"
    "  <terminalia>"
    "    <terminalis titulus=\"NUMBER\" genus=\"LAPIFEX_LEX_NUMBER\"/>"
    "    <terminalis titulus=\"PLUS\" genus=\"LAPIFEX_LEX_PLUS\"/>"
    "    <terminalis titulus=\"STAR\" genus=\"LAPIFEX_LEX_STAR\"/>"
    "    <terminalis titulus=\"LPAREN\" genus=\"LAPIFEX_LEX_LPAREN\"/>"
    "    <terminalis titulus=\"RPAREN\" genus=\"LAPIFEX_LEX_RPAREN\"/>"
    "    <terminalis titulus=\"EOF\" genus=\"LAPIFEX_LEX_EOF\"/>"
    "  </terminalia>"
    "  <regulae>"
    "    <regula titulus=\"expr\">"
    "      <productio>expr PLUS term</productio>"
    "      <productio>term</productio>"
    "    </regula>"
    "    <regula titulus=\"term\">"
    "      <productio>term STAR factor</productio>"
    "      <productio>factor</productio>"
    "    </regula>"
    "    <regula titulus=\"factor\">"
    "      <productio>LPAREN expr RPAREN</productio>"
    "      <productio>NUMBER</productio>"
    "    </regula>"
    "  </regulae>"
    "  <initium>expr</initium>"
    "</grammatica>";

/* Auxiliaris: verificare si FIRST coniunctum continet terminalem per titulum */
hic_manens b32
first_habet_terminalem(
    LapifexGrammatica*  grammatica,
    s32                 symbolum_index,
    constans character* terminalis_titulus)
{
    Xar* first;
    i32  i;

    si (symbolum_index < ZEPHYRUM) redde FALSUM;

    first = lapifex_first_obtinere(grammatica, (i32)symbolum_index);
    si (!first) redde FALSUM;

    per (i = ZEPHYRUM; i < (i32)xar_numerus(first); i++)
    {
        s32* idx = (s32*)xar_obtinere(first, (i32)i);
        si (idx && *idx >= ZEPHYRUM)
        {
            LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                grammatica->symbola, (i32)*idx);
            si (sym && chorda_aequalis_literis(*sym->titulus, terminalis_titulus))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

/* Auxiliaris: invenire indicem symboli per titulum */
hic_manens s32
invenire_symbolum_per_titulum(
    LapifexGrammatica*  grammatica,
    constans character*  titulus)
{
    i32 i;
    per (i = ZEPHYRUM; i < (i32)xar_numerus(grammatica->symbola); i++)
    {
        LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
            grammatica->symbola, (i32)i);
        si (sym && chorda_aequalis_literis(*sym->titulus, titulus))
        {
            redde sym->index;
        }
    }
    redde -I;
}

s32 principale(vacuum)
{
    Piscina*              piscina;
    InternamentumChorda*  intern;

    piscina = piscina_generare_dynamicum("probatio_lapifex", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: Parsatio grammaticae - structura basica
     * ======================================================== */
    {
        LapifexGrammatica* g;

        imprimere("\n--- Probans parsationem grammaticae ---\n");

        g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_STML);
        CREDO_NON_NIHIL(g);

        /* 6 terminalia */
        CREDO_AEQUALIS_I32(g->numerus_terminalium, VI);

        /* 3 regulae + 1 augmentata = 4 non-terminalia */
        CREDO_AEQUALIS_I32(g->numerus_non_terminalium, IV);

        /* 6 productiones + 1 augmentata = 7 */
        CREDO_AEQUALIS_I32((i32)xar_numerus(g->productiones), VII);

        /* Totalia symbola: 6 + 4 = 10 */
        CREDO_AEQUALIS_I32((i32)xar_numerus(g->symbola), X);

        imprimere("  Terminalia: %d, Non-terminalia: %d, Productiones: %d\n",
            (int)g->numerus_terminalium,
            (int)g->numerus_non_terminalium,
            (int)xar_numerus(g->productiones));
    }

    /* ========================================================
     * PROBARE: Resolutio symbolorum in productionibus
     * ======================================================== */
    {
        LapifexGrammatica* g;
        s32 expr_idx;
        s32 term_idx;
        s32 factor_idx;

        imprimere("\n--- Probans resolutionem symbolorum ---\n");

        g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_STML);
        CREDO_NON_NIHIL(g);

        /* Verificare symbola existunt */
        expr_idx = invenire_symbolum_per_titulum(g, "expr");
        term_idx = invenire_symbolum_per_titulum(g, "term");
        factor_idx = invenire_symbolum_per_titulum(g, "factor");

        CREDO_VERUM(expr_idx >= ZEPHYRUM);
        CREDO_VERUM(term_idx >= ZEPHYRUM);
        CREDO_VERUM(factor_idx >= ZEPHYRUM);

        /* expr non est terminale */
        {
            LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                g->symbola, (i32)expr_idx);
            CREDO_NON_NIHIL(sym);
            CREDO_FALSUM(sym->est_terminale);
        }

        /* NUMBER est terminale */
        {
            s32 number_idx = invenire_symbolum_per_titulum(g, "NUMBER");
            LapifexSymbolum* sym;
            CREDO_VERUM(number_idx >= ZEPHYRUM);
            sym = (LapifexSymbolum*)xar_obtinere(g->symbola, (i32)number_idx);
            CREDO_NON_NIHIL(sym);
            CREDO_VERUM(sym->est_terminale);
        }

        /* Verificare productio prima: expr -> expr PLUS term (3 symbola) */
        {
            LapifexProductio* prod = (LapifexProductio*)xar_obtinere(
                g->productiones, ZEPHYRUM);
            CREDO_NON_NIHIL(prod);
            CREDO_AEQUALIS_S32(prod->sinistrum, expr_idx);
            CREDO_AEQUALIS_I32((i32)xar_numerus(prod->dextrum), III);
        }

        /* Verificare productio secunda: expr -> term (1 symbolum) */
        {
            LapifexProductio* prod = (LapifexProductio*)xar_obtinere(
                g->productiones, I);
            CREDO_NON_NIHIL(prod);
            CREDO_AEQUALIS_S32(prod->sinistrum, expr_idx);
            CREDO_AEQUALIS_I32((i32)xar_numerus(prod->dextrum), I);
        }

        imprimere("  Symbola resoluta recte\n");
        (vacuum)term_idx;
        (vacuum)factor_idx;
    }

    /* ========================================================
     * PROBARE: FIRST coniuncta
     * ======================================================== */
    {
        LapifexGrammatica* g;
        s32 expr_idx;
        s32 term_idx;
        s32 factor_idx;
        Xar* first_expr;
        Xar* first_term;
        Xar* first_factor;

        imprimere("\n--- Probans FIRST coniuncta ---\n");

        g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_STML);
        CREDO_NON_NIHIL(g);

        CREDO_VERUM(lapifex_first_computare(g));

        expr_idx = invenire_symbolum_per_titulum(g, "expr");
        term_idx = invenire_symbolum_per_titulum(g, "term");
        factor_idx = invenire_symbolum_per_titulum(g, "factor");

        /* FIRST(factor) = {LPAREN, NUMBER} */
        first_factor = lapifex_first_obtinere(g, (i32)factor_idx);
        CREDO_NON_NIHIL(first_factor);
        CREDO_AEQUALIS_I32((i32)xar_numerus(first_factor), II);
        CREDO_VERUM(first_habet_terminalem(g, factor_idx, "LPAREN"));
        CREDO_VERUM(first_habet_terminalem(g, factor_idx, "NUMBER"));

        /* FIRST(term) = {LPAREN, NUMBER} */
        first_term = lapifex_first_obtinere(g, (i32)term_idx);
        CREDO_NON_NIHIL(first_term);
        CREDO_AEQUALIS_I32((i32)xar_numerus(first_term), II);
        CREDO_VERUM(first_habet_terminalem(g, term_idx, "LPAREN"));
        CREDO_VERUM(first_habet_terminalem(g, term_idx, "NUMBER"));

        /* FIRST(expr) = {LPAREN, NUMBER} */
        first_expr = lapifex_first_obtinere(g, (i32)expr_idx);
        CREDO_NON_NIHIL(first_expr);
        CREDO_AEQUALIS_I32((i32)xar_numerus(first_expr), II);
        CREDO_VERUM(first_habet_terminalem(g, expr_idx, "LPAREN"));
        CREDO_VERUM(first_habet_terminalem(g, expr_idx, "NUMBER"));

        /* Imprimere pro depuratione */
        lapifex_grammaticam_imprimere(g);
        lapifex_first_imprimere(g);
    }

    /* ========================================================
     * PROBARE: Error handling - STML malformatum
     * ======================================================== */
    {
        LapifexGrammatica* g;

        imprimere("\n--- Probans error handling ---\n");

        /* NIHIL input */
        g = lapifex_grammaticam_legere(piscina, intern, NIHIL);
        CREDO_NIHIL(g);

        /* STML sine <terminalia> */
        g = lapifex_grammaticam_legere(piscina, intern,
            "<grammatica><regulae></regulae><initium>x</initium></grammatica>");
        CREDO_NIHIL(g);

        /* STML sine <regulae> */
        g = lapifex_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia></terminalia><initium>x</initium></grammatica>");
        CREDO_NIHIL(g);

        /* STML sine <initium> */
        g = lapifex_grammaticam_legere(piscina, intern,
            "<grammatica><terminalia></terminalia><regulae></regulae></grammatica>");
        CREDO_NIHIL(g);

        imprimere("  Error handling recte\n");
    }

    /* ========================================================
     * PROBARE: Terminalia FIRST coniuncta
     * ======================================================== */
    {
        LapifexGrammatica* g;
        s32 number_idx;
        Xar* first_number;

        imprimere("\n--- Probans terminalia FIRST ---\n");

        g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_STML);
        CREDO_NON_NIHIL(g);
        CREDO_VERUM(lapifex_first_computare(g));

        /* FIRST(NUMBER) = {NUMBER} */
        number_idx = invenire_symbolum_per_titulum(g, "NUMBER");
        CREDO_VERUM(number_idx >= ZEPHYRUM);

        first_number = lapifex_first_obtinere(g, (i32)number_idx);
        CREDO_NON_NIHIL(first_number);
        CREDO_AEQUALIS_I32((i32)xar_numerus(first_number), I);
        CREDO_VERUM(first_habet_terminalem(g, number_idx, "NUMBER"));

        imprimere("  Terminalia FIRST recte\n");
    }

    /* ========================================================
     * Compendium
     * ======================================================== */
    imprimere("\n");
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde (credo_omnia_praeterierunt() ? ZEPHYRUM : I);
}
