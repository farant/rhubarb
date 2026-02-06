/* probatio_lapifex_lexere.c - Probationes Lexeris Lapifex (Phase 5) */

#include "latina.h"
#include "lapifex_generare.h"
#include "lapifex_parsere.h"
#include "lapifex_lexere.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "credo.h"
#include <stdio.h>

/* Grammatica arithmetica cum exemplis pro lexere */
hic_manens constans character* GRAMMATICA_CUM_EXEMPLIS =
    "<grammatica>"
    "  <terminalia>"
    "    <terminalis titulus=\"NUMBER\" exemplum=\"$numerus\"/>"
    "    <terminalis titulus=\"PLUS\" exemplum=\"+\"/>"
    "    <terminalis titulus=\"STAR\" exemplum=\"*\"/>"
    "    <terminalis titulus=\"LPAREN\" exemplum=\"(\"/>"
    "    <terminalis titulus=\"RPAREN\" exemplum=\")\"/>"
    "    <terminalis titulus=\"EOF\"/>"
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
    LapifexGrammatica*   grammatica,
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

/* Auxiliaris: praeparare grammaticam cum exemplis */
hic_manens LapifexGrammatica*
grammaticam_praeparare(
    Piscina*              piscina,
    InternamentumChorda*  intern)
{
    LapifexGrammatica* g;

    g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_CUM_EXEMPLIS);
    si (!g) redde NIHIL;

    lapifex_first_computare(g);

    redde g;
}

/* Auxiliaris: praeparare tabulam completam */
hic_manens LapifexTabula*
tabulam_praeparare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    LapifexGrammatica**   g_fructus)
{
    LapifexGrammatica* g;
    LapifexCollectio*  c;
    LapifexTabula*     t;

    g = grammaticam_praeparare(piscina, intern);
    si (!g) redde NIHIL;

    c = lapifex_collectio_construere(g);
    si (!c) redde NIHIL;

    t = lapifex_tabulam_construere(c);

    si (g_fructus) *g_fructus = g;

    redde t;
}

/* Callback evaluationis arithmeticae
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
        casus ZEPHYRUM: redde valori[0] + valori[2];
        casus I:        redde valori[0];
        casus II:       redde valori[0] * valori[2];
        casus III:      redde valori[0];
        casus IV:       redde valori[1];
        casus V:        redde valori[0];
        casus VI:       redde valori[0];
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
    s32 EOF_IDX;

    piscina = piscina_generare_dynamicum("probatio_lapifex_lexere", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* Praeparare tabulam et indices */
    t = tabulam_praeparare(piscina, intern, &g);
    si (!t)
    {
        imprimere("FRACTA: tabula non constructa\n");
        redde I;
    }

    NUMBER_IDX = invenire_symbolum(g, "NUMBER");
    EOF_IDX    = invenire_symbolum(g, "EOF");

    /* ========================================================
     * PROBARE I: Simplex numerus — "42"
     * ======================================================== */
    {
        LapifexLexFructus f;

        imprimere("\n--- Probans I: simplex numerus \"42\" ---\n");

        f = lapifex_lexere(g, "42", II, piscina);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S32(f.numerus, II);
        CREDO_AEQUALIS_S32(f.signa[0].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[0].valor, (s64)XLII);
        CREDO_AEQUALIS_S32(f.signa[1].terminalis, EOF_IDX);
        imprimere("  signa: NUMBER(%lld), EOF\n",
            (longus longus)f.signa[0].valor);
    }

    /* ========================================================
     * PROBARE II: Additio — "2 + 3"
     * ======================================================== */
    {
        LapifexLexFructus f;
        s32 PLUS_IDX;

        imprimere("\n--- Probans II: additio \"2 + 3\" ---\n");

        PLUS_IDX = invenire_symbolum(g, "PLUS");
        f = lapifex_lexere(g, "2 + 3", V, piscina);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S32(f.numerus, IV);
        CREDO_AEQUALIS_S32(f.signa[0].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[0].valor, (s64)II);
        CREDO_AEQUALIS_S32(f.signa[1].terminalis, PLUS_IDX);
        CREDO_AEQUALIS_S32(f.signa[2].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[2].valor, (s64)III);
        CREDO_AEQUALIS_S32(f.signa[3].terminalis, EOF_IDX);
        imprimere("  signa: NUMBER(2), PLUS, NUMBER(3), EOF\n");
    }

    /* ========================================================
     * PROBARE III: Sine spatiis — "2+3"
     * ======================================================== */
    {
        LapifexLexFructus f;
        s32 PLUS_IDX;

        imprimere("\n--- Probans III: sine spatiis \"2+3\" ---\n");

        PLUS_IDX = invenire_symbolum(g, "PLUS");
        f = lapifex_lexere(g, "2+3", III, piscina);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S32(f.numerus, IV);
        CREDO_AEQUALIS_S32(f.signa[0].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[0].valor, (s64)II);
        CREDO_AEQUALIS_S32(f.signa[1].terminalis, PLUS_IDX);
        CREDO_AEQUALIS_S32(f.signa[2].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[2].valor, (s64)III);
        CREDO_AEQUALIS_S32(f.signa[3].terminalis, EOF_IDX);
        imprimere("  signa: NUMBER(2), PLUS, NUMBER(3), EOF\n");
    }

    /* ========================================================
     * PROBARE IV: Multi-cifra — "123"
     * ======================================================== */
    {
        LapifexLexFructus f;

        imprimere("\n--- Probans IV: multi-cifra \"123\" ---\n");

        f = lapifex_lexere(g, "123", III, piscina);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S32(f.numerus, II);
        CREDO_AEQUALIS_S32(f.signa[0].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[0].valor, (s64)CXXIII);
        CREDO_AEQUALIS_S32(f.signa[1].terminalis, EOF_IDX);
        imprimere("  signa: NUMBER(%lld), EOF\n",
            (longus longus)f.signa[0].valor);
    }

    /* ========================================================
     * PROBARE V: Expressio plena — "2 + 3 * 4"
     * ======================================================== */
    {
        LapifexLexFructus f;
        s32 PLUS_IDX;
        s32 STAR_IDX;

        imprimere("\n--- Probans V: expressio plena \"2 + 3 * 4\" ---\n");

        PLUS_IDX = invenire_symbolum(g, "PLUS");
        STAR_IDX = invenire_symbolum(g, "STAR");
        f = lapifex_lexere(g, "2 + 3 * 4", IX, piscina);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S32(f.numerus, VI);
        CREDO_AEQUALIS_S32(f.signa[0].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[0].valor, (s64)II);
        CREDO_AEQUALIS_S32(f.signa[1].terminalis, PLUS_IDX);
        CREDO_AEQUALIS_S32(f.signa[2].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[2].valor, (s64)III);
        CREDO_AEQUALIS_S32(f.signa[3].terminalis, STAR_IDX);
        CREDO_AEQUALIS_S32(f.signa[4].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[4].valor, (s64)IV);
        CREDO_AEQUALIS_S32(f.signa[5].terminalis, EOF_IDX);
        imprimere("  signa: NUMBER(2), PLUS, NUMBER(3), STAR, NUMBER(4), EOF\n");
    }

    /* ========================================================
     * PROBARE VI: Parentheses — "(2+3)*4"
     * ======================================================== */
    {
        LapifexLexFructus f;
        s32 PLUS_IDX;
        s32 STAR_IDX;
        s32 LPAREN_IDX;
        s32 RPAREN_IDX;

        imprimere("\n--- Probans VI: parentheses \"(2+3)*4\" ---\n");

        PLUS_IDX   = invenire_symbolum(g, "PLUS");
        STAR_IDX   = invenire_symbolum(g, "STAR");
        LPAREN_IDX = invenire_symbolum(g, "LPAREN");
        RPAREN_IDX = invenire_symbolum(g, "RPAREN");
        f = lapifex_lexere(g, "(2+3)*4", VII, piscina);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S32(f.numerus, VIII);
        CREDO_AEQUALIS_S32(f.signa[0].terminalis, LPAREN_IDX);
        CREDO_AEQUALIS_S32(f.signa[1].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[1].valor, (s64)II);
        CREDO_AEQUALIS_S32(f.signa[2].terminalis, PLUS_IDX);
        CREDO_AEQUALIS_S32(f.signa[3].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[3].valor, (s64)III);
        CREDO_AEQUALIS_S32(f.signa[4].terminalis, RPAREN_IDX);
        CREDO_AEQUALIS_S32(f.signa[5].terminalis, STAR_IDX);
        CREDO_AEQUALIS_S32(f.signa[6].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[6].valor, (s64)IV);
        CREDO_AEQUALIS_S32(f.signa[7].terminalis, EOF_IDX);
        imprimere("  signa: LPAREN, NUMBER(2), PLUS, NUMBER(3), RPAREN, STAR, NUMBER(4), EOF\n");
    }

    /* ========================================================
     * PROBARE VII: Spatium copiosum — "  42  +  3  "
     * ======================================================== */
    {
        LapifexLexFructus f;
        s32 PLUS_IDX;

        imprimere("\n--- Probans VII: spatium copiosum \"  42  +  3  \" ---\n");

        PLUS_IDX = invenire_symbolum(g, "PLUS");
        f = lapifex_lexere(g, "  42  +  3  ", XII, piscina);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S32(f.numerus, IV);
        CREDO_AEQUALIS_S32(f.signa[0].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[0].valor, (s64)XLII);
        CREDO_AEQUALIS_S32(f.signa[1].terminalis, PLUS_IDX);
        CREDO_AEQUALIS_S32(f.signa[2].terminalis, NUMBER_IDX);
        CREDO_AEQUALIS_S64(f.signa[2].valor, (s64)III);
        CREDO_AEQUALIS_S32(f.signa[3].terminalis, EOF_IDX);
        imprimere("  signa: NUMBER(42), PLUS, NUMBER(3), EOF\n");
    }

    /* ========================================================
     * PROBARE VIII: Vacuum — ""
     * ======================================================== */
    {
        LapifexLexFructus f;

        imprimere("\n--- Probans VIII: vacuum \"\" ---\n");

        f = lapifex_lexere(g, "", ZEPHYRUM, piscina);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S32(f.numerus, I);
        CREDO_AEQUALIS_S32(f.signa[0].terminalis, EOF_IDX);
        imprimere("  signa: EOF\n");
    }

    /* ========================================================
     * PROBARE IX: Error — character ignotus "2 @ 3"
     * ======================================================== */
    {
        LapifexLexFructus f;

        imprimere("\n--- Probans IX: error character ignotus \"2 @ 3\" ---\n");

        f = lapifex_lexere(g, "2 @ 3", V, piscina);
        CREDO_FALSUM(f.successus);
        CREDO_AEQUALIS_S32(f.error_positio, II);
        CREDO_AEQUALIS_S32(f.error_linea, I);
        CREDO_AEQUALIS_S32(f.error_columna, III);
        imprimere("  error ad positio %d, linea %d, columna %d\n",
            (int)f.error_positio, (int)f.error_linea,
            (int)f.error_columna);
    }

    /* ========================================================
     * PROBARE X: End-to-end lex+parse "2+3*4" = 14
     * ======================================================== */
    {
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans X: lex+parse \"2+3*4\" = 14 ---\n");

        f = lapifex_lexere_et_parsare(t, "2+3*4", V,
            arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)XIV);
        imprimere("  2+3*4 = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE XI: End-to-end lex+parse "(1+2)*(3+4)" = 21
     * ======================================================== */
    {
        LapifexParsaturaFructus f;

        imprimere("\n--- Probans XI: lex+parse \"(1+2)*(3+4)\" = 21 ---\n");

        f = lapifex_lexere_et_parsare(t, "(1+2)*(3+4)", XI,
            arithmetica_reductio, NIHIL);
        CREDO_VERUM(f.successus);
        CREDO_AEQUALIS_S64(f.valor, (s64)XXI);
        imprimere("  (1+2)*(3+4) = %lld\n", (longus longus)f.valor);
    }

    /* ========================================================
     * PROBARE XII: Regressio — grammatica sine exemplis
     * ======================================================== */
    {
        constans character* GRAMMATICA_SINE_EXEMPLIS =
            "<grammatica>"
            "  <terminalia>"
            "    <terminalis titulus=\"NUMBER\" genus=\"LAPIFEX_LEX_NUMBER\"/>"
            "    <terminalis titulus=\"PLUS\" genus=\"LAPIFEX_LEX_PLUS\"/>"
            "    <terminalis titulus=\"EOF\" genus=\"LAPIFEX_LEX_EOF\"/>"
            "  </terminalia>"
            "  <regulae>"
            "    <regula titulus=\"expr\">"
            "      <productio>expr PLUS NUMBER</productio>"
            "      <productio>NUMBER</productio>"
            "    </regula>"
            "  </regulae>"
            "  <initium>expr</initium>"
            "</grammatica>";
        LapifexGrammatica* g2;

        imprimere("\n--- Probans XII: regressio grammatica sine exemplis ---\n");

        g2 = lapifex_grammaticam_legere(piscina, intern,
            GRAMMATICA_SINE_EXEMPLIS);
        CREDO_VERUM(g2 != NIHIL);

        si (g2)
        {
            LapifexSymbolum* sym;
            i32 k;

            /* Verificare quod exemplum est NIHIL pro omnibus symbolis */
            per (k = ZEPHYRUM; k < (i32)xar_numerus(g2->symbola); k++)
            {
                sym = (LapifexSymbolum*)xar_obtinere(g2->symbola, k);
                si (sym && sym->est_terminale)
                {
                    CREDO_VERUM(sym->exemplum == NIHIL);
                }
            }
            imprimere("  grammatica sine exemplis: omnia exempla NIHIL\n");
        }
    }

    /* ========================================================
     * Compendium
     * ======================================================== */
    imprimere("\n");
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde (credo_omnia_praeterierunt() ? ZEPHYRUM : I);
}
