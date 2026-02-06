/* probatio_lapifex_parsere.c - Probationes Motoris Parsationis LR(1) */

#include "latina.h"
#include "lapifex_generare.h"
#include "lapifex_parsere.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "credo.h"
#include <stdio.h>

/* Grammatica arithmetica probationis (eadem ac in probatio_lapifex_tabula) */
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

/* Auxiliaris: invenire indicem symboli per titulum */
hic_manens s32
invenire_symbolum(
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

/* Auxiliaris: construere tabulam completam */
hic_manens LapifexTabula*
tabulam_praeparare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    LapifexGrammatica**   g_fructus)
{
    LapifexGrammatica* g;
    LapifexCollectio*  c;
    LapifexTabula*     t;

    g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_STML);
    si (!g) redde NIHIL;

    lapifex_first_computare(g);

    c = lapifex_collectio_construere(g);
    si (!c) redde NIHIL;

    t = lapifex_tabulam_construere(c);

    si (g_fructus) *g_fructus = g;

    redde t;
}

/* ================================================
 * Callback Evaluationis Arithmeticae
 * ================================================
 *
 * Productiones:
 *   P0: expr   -> expr PLUS term    : valori[0] + valori[2]
 *   P1: expr   -> term              : valori[0]
 *   P2: term   -> term STAR factor  : valori[0] * valori[2]
 *   P3: term   -> factor            : valori[0]
 *   P4: factor -> LPAREN expr RPAREN: valori[1]
 *   P5: factor -> NUMBER            : valori[0]
 *   P6: __initium__ -> expr EOF     : valori[0]
 */
hic_manens s64
arithmetica_reductio(
    s32 productio_index,
    constans s64* valori,
    s32 numerus_dextrum,
    vacuum* contextus)
{
    (vacuum)numerus_dextrum;
    (vacuum)contextus;

    commutatio (productio_index)
    {
        casus ZEPHYRUM: redde valori[0] + valori[2];  /* expr -> expr PLUS term */
        casus I:        redde valori[0];               /* expr -> term */
        casus II:       redde valori[0] * valori[2];   /* term -> term STAR factor */
        casus III:      redde valori[0];               /* term -> factor */
        casus IV:       redde valori[1];               /* factor -> LPAREN expr RPAREN */
        casus V:        redde valori[0];               /* factor -> NUMBER */
        casus VI:       redde valori[0];               /* __initium__ -> expr EOF */
        ordinarius:     redde 0;
    }
}

s32 principale(vacuum)
{
    Piscina*              piscina;
    InternamentumChorda*  intern;
    LapifexGrammatica*    g;
    LapifexTabula*        t;
    s32 NUMBER_IDX;
    s32 PLUS_IDX;
    s32 STAR_IDX;
    s32 LPAREN_IDX;
    s32 RPAREN_IDX;
    s32 EOF_IDX;

    piscina = piscina_generare_dynamicum("probatio_lapifex_parsere", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* Praeparare tabulam et indices terminales */
    t = tabulam_praeparare(piscina, intern, &g);
    si (!t)
    {
        imprimere("FRACTA: tabula non constructa\n");
        redde I;
    }

    NUMBER_IDX = invenire_symbolum(g, "NUMBER");
    PLUS_IDX   = invenire_symbolum(g, "PLUS");
    STAR_IDX   = invenire_symbolum(g, "STAR");
    LPAREN_IDX = invenire_symbolum(g, "LPAREN");
    RPAREN_IDX = invenire_symbolum(g, "RPAREN");
    EOF_IDX    = invenire_symbolum(g, "EOF");

    /* ========================================================
     * PROBARE I: Simplex numerus — 42
     * ======================================================== */
    {
        LapifexSignum signa[II];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans 42 ---\n");

        signa[0].terminalis = NUMBER_IDX;
        signa[0].valor = XLII;
        signa[1].terminalis = EOF_IDX;
        signa[1].valor = 0;

        f = lapifex_parsare(t, signa, II, arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)XLII);
        imprimere("  42 = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE II: Additio — 2+3
     * ======================================================== */
    {
        LapifexSignum signa[IV];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans 2+3 ---\n");

        signa[0].terminalis = NUMBER_IDX;
        signa[0].valor = II;
        signa[1].terminalis = PLUS_IDX;
        signa[1].valor = 0;
        signa[2].terminalis = NUMBER_IDX;
        signa[2].valor = III;
        signa[3].terminalis = EOF_IDX;
        signa[3].valor = 0;

        f = lapifex_parsare(t, signa, IV, arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)V);
        imprimere("  2+3 = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE III: Praecedentia — 2+3*4 = 14
     * ======================================================== */
    {
        LapifexSignum signa[VI];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans 2+3*4 ---\n");

        signa[0].terminalis = NUMBER_IDX;
        signa[0].valor = II;
        signa[1].terminalis = PLUS_IDX;
        signa[1].valor = 0;
        signa[2].terminalis = NUMBER_IDX;
        signa[2].valor = III;
        signa[3].terminalis = STAR_IDX;
        signa[3].valor = 0;
        signa[4].terminalis = NUMBER_IDX;
        signa[4].valor = IV;
        signa[5].terminalis = EOF_IDX;
        signa[5].valor = 0;

        f = lapifex_parsare(t, signa, VI, arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)XIV);
        imprimere("  2+3*4 = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE IV: Associativitas sinistra — 1+2+3 = 6
     * ======================================================== */
    {
        LapifexSignum signa[VI];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans 1+2+3 ---\n");

        signa[0].terminalis = NUMBER_IDX;
        signa[0].valor = I;
        signa[1].terminalis = PLUS_IDX;
        signa[1].valor = 0;
        signa[2].terminalis = NUMBER_IDX;
        signa[2].valor = II;
        signa[3].terminalis = PLUS_IDX;
        signa[3].valor = 0;
        signa[4].terminalis = NUMBER_IDX;
        signa[4].valor = III;
        signa[5].terminalis = EOF_IDX;
        signa[5].valor = 0;

        f = lapifex_parsare(t, signa, VI, arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)VI);
        imprimere("  1+2+3 = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE V: Parentheses — (2+3)*4 = 20
     * ======================================================== */
    {
        LapifexSignum signa[VIII];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans (2+3)*4 ---\n");

        signa[0].terminalis = LPAREN_IDX;
        signa[0].valor = 0;
        signa[1].terminalis = NUMBER_IDX;
        signa[1].valor = II;
        signa[2].terminalis = PLUS_IDX;
        signa[2].valor = 0;
        signa[3].terminalis = NUMBER_IDX;
        signa[3].valor = III;
        signa[4].terminalis = RPAREN_IDX;
        signa[4].valor = 0;
        signa[5].terminalis = STAR_IDX;
        signa[5].valor = 0;
        signa[6].terminalis = NUMBER_IDX;
        signa[6].valor = IV;
        signa[7].terminalis = EOF_IDX;
        signa[7].valor = 0;

        f = lapifex_parsare(t, signa, VIII, arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)XX);
        imprimere("  (2+3)*4 = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE VI: Parentheses imbricatae — ((5)) = 5
     * ======================================================== */
    {
        LapifexSignum signa[VI];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans ((5)) ---\n");

        signa[0].terminalis = LPAREN_IDX;
        signa[0].valor = 0;
        signa[1].terminalis = LPAREN_IDX;
        signa[1].valor = 0;
        signa[2].terminalis = NUMBER_IDX;
        signa[2].valor = V;
        signa[3].terminalis = RPAREN_IDX;
        signa[3].valor = 0;
        signa[4].terminalis = RPAREN_IDX;
        signa[4].valor = 0;
        signa[5].terminalis = EOF_IDX;
        signa[5].valor = 0;

        f = lapifex_parsare(t, signa, VI, arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)V);
        imprimere("  ((5)) = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE VII: Complexum — (1+2)*(3+4) = 21
     * ======================================================== */
    {
        LapifexSignum signa[XII];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans (1+2)*(3+4) ---\n");

        signa[0].terminalis = LPAREN_IDX;
        signa[0].valor = 0;
        signa[1].terminalis = NUMBER_IDX;
        signa[1].valor = I;
        signa[2].terminalis = PLUS_IDX;
        signa[2].valor = 0;
        signa[3].terminalis = NUMBER_IDX;
        signa[3].valor = II;
        signa[4].terminalis = RPAREN_IDX;
        signa[4].valor = 0;
        signa[5].terminalis = STAR_IDX;
        signa[5].valor = 0;
        signa[6].terminalis = LPAREN_IDX;
        signa[6].valor = 0;
        signa[7].terminalis = NUMBER_IDX;
        signa[7].valor = III;
        signa[8].terminalis = PLUS_IDX;
        signa[8].valor = 0;
        signa[9].terminalis = NUMBER_IDX;
        signa[9].valor = IV;
        signa[10].terminalis = RPAREN_IDX;
        signa[10].valor = 0;
        signa[11].terminalis = EOF_IDX;
        signa[11].valor = 0;

        f = lapifex_parsare(t, signa, XII, arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)XXI);
        imprimere("  (1+2)*(3+4) = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE VIII: Error — +2 (PLUS in initio)
     * ======================================================== */
    {
        LapifexSignum signa[III];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans error: +2 ---\n");

        signa[0].terminalis = PLUS_IDX;
        signa[0].valor = 0;
        signa[1].terminalis = NUMBER_IDX;
        signa[1].valor = II;
        signa[2].terminalis = EOF_IDX;
        signa[2].valor = 0;

        f = lapifex_parsare(t, signa, III, arithmetica_reductio, NIHIL);
        CREDO_FALSUM(f.successus);
        CREDO_VERUM(f.error.est_error);
        CREDO_AEQUALIS_S32(f.error.status, ZEPHYRUM);
        imprimere("  Error in statu %d, terminalis %d, positio %d\n",
            (int)f.error.status, (int)f.error.terminalis,
            (int)f.error.positio);
    }

    /* ========================================================
     * PROBARE IX: Error — 2 sine EOF
     * ======================================================== */
    {
        LapifexSignum signa[I];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans error: 2 sine EOF ---\n");

        signa[0].terminalis = NUMBER_IDX;
        signa[0].valor = II;

        f = lapifex_parsare(t, signa, I, arithmetica_reductio, NIHIL);
        CREDO_FALSUM(f.successus);
        CREDO_VERUM(f.error.est_error);
        imprimere("  Error in statu %d, terminalis %d, positio %d\n",
            (int)f.error.status, (int)f.error.terminalis,
            (int)f.error.positio);
    }

    /* ========================================================
     * PROBARE X: Callback NIHIL — valor = 0
     * ======================================================== */
    {
        LapifexSignum signa[II];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans callback NIHIL ---\n");

        signa[0].terminalis = NUMBER_IDX;
        signa[0].valor = XLII;
        signa[1].terminalis = EOF_IDX;
        signa[1].valor = 0;

        f = lapifex_parsare(t, signa, II, NIHIL, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)ZEPHYRUM);
        imprimere("  NIHIL callback: valor = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE XI: Multiplicatio catena — 2*3*4 = 24
     * ======================================================== */
    {
        LapifexSignum signa[VI];
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans 2*3*4 ---\n");

        signa[0].terminalis = NUMBER_IDX;
        signa[0].valor = II;
        signa[1].terminalis = STAR_IDX;
        signa[1].valor = 0;
        signa[2].terminalis = NUMBER_IDX;
        signa[2].valor = III;
        signa[3].terminalis = STAR_IDX;
        signa[3].valor = 0;
        signa[4].terminalis = NUMBER_IDX;
        signa[4].valor = IV;
        signa[5].terminalis = EOF_IDX;
        signa[5].valor = 0;

        f = lapifex_parsare(t, signa, VI, arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)XXIV);
        imprimere("  2*3*4 = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */
    imprimere("\n");
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde (credo_omnia_praeterierunt() ? ZEPHYRUM : I);
}
