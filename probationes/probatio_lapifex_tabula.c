/* probatio_lapifex_tabula.c - Probationes Tabulae ACTION/GOTO LR(1) */

#include "latina.h"
#include "lapifex_generare.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "credo.h"
#include <stdio.h>

/* Grammatica arithmetica probationis (eadem ac in probatio_lapifex_lr1) */
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

/* Auxiliaris: construere grammaticam, FIRST, collectio, tabula */
hic_manens LapifexTabula*
tabulam_praeparare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    LapifexGrammatica**   g_fructus,
    LapifexCollectio**    c_fructus)
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
    si (c_fructus) *c_fructus = c;

    redde t;
}

s32 principale(vacuum)
{
    Piscina*              piscina;
    InternamentumChorda*  intern;

    piscina = piscina_generare_dynamicum("probatio_lapifex_tabula", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE I: Constructio tabulae successit
     * ======================================================== */
    {
        LapifexTabula* t;

        imprimere("\n--- Probans constructionem tabulae ---\n");

        t = tabulam_praeparare(piscina, intern, NIHIL, NIHIL);
        CREDO_NON_NIHIL(t);
        CREDO_NON_NIHIL(t->status_tabulae);
        CREDO_NON_NIHIL(t->grammatica);
        CREDO_NON_NIHIL(t->collectio);

        imprimere("  Tabula constructa: %d status\n",
            (int)xar_numerus(t->status_tabulae));
    }

    /* ========================================================
     * PROBARE II: Omnis status habet actiones vel goto
     * ======================================================== */
    {
        LapifexTabula* t;
        i32 num_status;
        i32 s;
        b32 omnes_habent = VERUM;

        imprimere("\n--- Probans omnes status habent introitus ---\n");

        t = tabulam_praeparare(piscina, intern, NIHIL, NIHIL);
        CREDO_NON_NIHIL(t);

        num_status = (i32)xar_numerus(t->status_tabulae);

        per (s = ZEPHYRUM; s < num_status; s++)
        {
            LapifexStatusTabula* st =
                (LapifexStatusTabula*)xar_obtinere(t->status_tabulae, s);
            si (st)
            {
                i32 num_a = (i32)xar_numerus(st->actiones);
                i32 num_g = (i32)xar_numerus(st->goto_introitus);
                si (num_a == ZEPHYRUM && num_g == ZEPHYRUM)
                {
                    imprimere("  Status %d: nullae actiones/goto!\n", (int)s);
                    omnes_habent = FALSUM;
                }
            }
        }
        CREDO_VERUM(omnes_habent);
    }

    /* ========================================================
     * PROBARE III: Status 0 habet SHIFT pro NUMBER et LPAREN
     * ======================================================== */
    {
        LapifexGrammatica* g;
        LapifexTabula*     t;
        Xar*               actiones_number;
        Xar*               actiones_lparen;
        s32 number_idx;
        s32 lparen_idx;

        imprimere("\n--- Probans SHIFT in statu 0 ---\n");

        t = tabulam_praeparare(piscina, intern, &g, NIHIL);
        CREDO_NON_NIHIL(t);

        number_idx = invenire_symbolum(g, "NUMBER");
        lparen_idx = invenire_symbolum(g, "LPAREN");
        CREDO_VERUM(number_idx >= ZEPHYRUM);
        CREDO_VERUM(lparen_idx >= ZEPHYRUM);

        actiones_number = lapifex_actiones_quaerere(t, ZEPHYRUM, number_idx);
        CREDO_NON_NIHIL(actiones_number);
        CREDO_VERUM((i32)xar_numerus(actiones_number) > ZEPHYRUM);

        {
            LapifexActioIntroitus* ai =
                (LapifexActioIntroitus*)xar_obtinere(actiones_number, ZEPHYRUM);
            CREDO_NON_NIHIL(ai);
            CREDO_AEQUALIS_S32((s32)ai->actio, (s32)LAPIFEX_ACTIO_TRANSPONERE);
            imprimere("  NUMBER -> SHIFT %d\n", (int)ai->valor);
        }

        actiones_lparen = lapifex_actiones_quaerere(t, ZEPHYRUM, lparen_idx);
        CREDO_NON_NIHIL(actiones_lparen);
        CREDO_VERUM((i32)xar_numerus(actiones_lparen) > ZEPHYRUM);

        {
            LapifexActioIntroitus* ai =
                (LapifexActioIntroitus*)xar_obtinere(actiones_lparen, ZEPHYRUM);
            CREDO_NON_NIHIL(ai);
            CREDO_AEQUALIS_S32((s32)ai->actio, (s32)LAPIFEX_ACTIO_TRANSPONERE);
            imprimere("  LPAREN -> SHIFT %d\n", (int)ai->valor);
        }
    }

    /* ========================================================
     * PROBARE IV: ACCEPT in aliquo statu pro $ (-1)
     * ======================================================== */
    {
        LapifexTabula* t;
        i32 num_status;
        i32 s;
        b32 acceptum_inventum = FALSUM;
        s32 status_accepti = -I;

        imprimere("\n--- Probans ACCEPT ---\n");

        t = tabulam_praeparare(piscina, intern, NIHIL, NIHIL);
        CREDO_NON_NIHIL(t);

        num_status = (i32)xar_numerus(t->status_tabulae);

        per (s = ZEPHYRUM; s < num_status && !acceptum_inventum; s++)
        {
            Xar* actiones = lapifex_actiones_quaerere(t, (s32)s, -I);
            si (actiones)
            {
                i32 i;
                per (i = ZEPHYRUM; i < (i32)xar_numerus(actiones); i++)
                {
                    LapifexActioIntroitus* ai =
                        (LapifexActioIntroitus*)xar_obtinere(actiones, i);
                    si (ai && ai->actio == LAPIFEX_ACTIO_ACCIPERE)
                    {
                        acceptum_inventum = VERUM;
                        status_accepti = (s32)s;
                        frange;
                    }
                }
            }
        }

        CREDO_VERUM(acceptum_inventum);
        imprimere("  ACCEPT in statu %d\n", (int)status_accepti);
    }

    /* ========================================================
     * PROBARE V: Status 0 habet GOTO pro expr, term, factor
     * ======================================================== */
    {
        LapifexGrammatica* g;
        LapifexTabula*     t;
        s32 expr_idx;
        s32 term_idx;
        s32 factor_idx;
        s32 goto_expr;
        s32 goto_term;
        s32 goto_factor;

        imprimere("\n--- Probans GOTO in statu 0 ---\n");

        t = tabulam_praeparare(piscina, intern, &g, NIHIL);
        CREDO_NON_NIHIL(t);

        expr_idx = invenire_symbolum(g, "expr");
        term_idx = invenire_symbolum(g, "term");
        factor_idx = invenire_symbolum(g, "factor");

        goto_expr = lapifex_goto_quaerere(t, ZEPHYRUM, expr_idx);
        goto_term = lapifex_goto_quaerere(t, ZEPHYRUM, term_idx);
        goto_factor = lapifex_goto_quaerere(t, ZEPHYRUM, factor_idx);

        CREDO_VERUM(goto_expr >= ZEPHYRUM);
        CREDO_VERUM(goto_term >= ZEPHYRUM);
        CREDO_VERUM(goto_factor >= ZEPHYRUM);

        imprimere("  GOTO(expr) = %d\n", (int)goto_expr);
        imprimere("  GOTO(term) = %d\n", (int)goto_term);
        imprimere("  GOTO(factor) = %d\n", (int)goto_factor);
    }

    /* ========================================================
     * PROBARE VI: REDUCE in statu cum factor -> NUMBER .
     * ======================================================== */
    {
        LapifexGrammatica* g;
        LapifexCollectio*  c;
        LapifexTabula*     t;
        b32 reduce_inventum = FALSUM;
        i32 s;
        s32 number_idx;

        imprimere("\n--- Probans REDUCE ---\n");

        t = tabulam_praeparare(piscina, intern, &g, &c);
        CREDO_NON_NIHIL(t);

        number_idx = invenire_symbolum(g, "NUMBER");

        /* Invenire statum ubi factor -> NUMBER . existit */
        per (s = ZEPHYRUM; s < (i32)xar_numerus(c->status_omnes); s++)
        {
            LapifexStatus* status_lr =
                (LapifexStatus*)xar_obtinere(c->status_omnes, s);
            i32 r;

            si (!status_lr) perge;

            per (r = ZEPHYRUM; r < (i32)xar_numerus(status_lr->res); r++)
            {
                LapifexRes* res =
                    (LapifexRes*)xar_obtinere(status_lr->res, r);
                LapifexProductio* prod;

                si (!res) perge;

                prod = (LapifexProductio*)xar_obtinere(
                    g->productiones, (i32)res->productio);
                si (!prod) perge;

                /* factor -> NUMBER . (1 symbolum in dextro, punctum = 1) */
                si ((i32)xar_numerus(prod->dextrum) == I &&
                    (i32)res->punctum == I)
                {
                    s32* rhs_sym = (s32*)xar_obtinere(prod->dextrum, ZEPHYRUM);
                    si (rhs_sym && *rhs_sym == number_idx)
                    {
                        /* Verificare REDUCE actionem */
                        LapifexStatusTabula* st =
                            (LapifexStatusTabula*)xar_obtinere(
                                t->status_tabulae, s);
                        i32 a;
                        si (!st) perge;

                        per (a = ZEPHYRUM; a < (i32)xar_numerus(st->actiones); a++)
                        {
                            LapifexActioIntroitus* ai =
                                (LapifexActioIntroitus*)xar_obtinere(
                                    st->actiones, a);
                            si (ai && ai->actio == LAPIFEX_ACTIO_REDUCERE &&
                                ai->valor == prod->index)
                            {
                                reduce_inventum = VERUM;
                                imprimere("  Status %d: REDUCE P%d (factor -> NUMBER)\n",
                                    (int)s, (int)ai->valor);
                                frange;
                            }
                        }
                        si (reduce_inventum) frange;
                    }
                }
            }
            si (reduce_inventum) frange;
        }

        CREDO_VERUM(reduce_inventum);
    }

    /* ========================================================
     * PROBARE VII: Nulli conflictus pro grammatica non-ambigua
     * ======================================================== */
    {
        LapifexTabula* t;

        imprimere("\n--- Probans nullos conflictus ---\n");

        t = tabulam_praeparare(piscina, intern, NIHIL, NIHIL);
        CREDO_NON_NIHIL(t);

        CREDO_AEQUALIS_S32(t->numerus_conflictuum, ZEPHYRUM);
        imprimere("  Conflictus: %d\n", (int)t->numerus_conflictuum);
    }

    /* ========================================================
     * PROBARE VIII: Numerus actionum et goto rationabilis
     * ======================================================== */
    {
        LapifexTabula* t;
        i32 s;
        i32 num_status;
        s32 totalis_actiones = ZEPHYRUM;
        s32 totalis_goto = ZEPHYRUM;

        imprimere("\n--- Probans numeros actionum/goto ---\n");

        t = tabulam_praeparare(piscina, intern, NIHIL, NIHIL);
        CREDO_NON_NIHIL(t);

        num_status = (i32)xar_numerus(t->status_tabulae);

        per (s = ZEPHYRUM; s < num_status; s++)
        {
            LapifexStatusTabula* st =
                (LapifexStatusTabula*)xar_obtinere(t->status_tabulae, s);
            si (st)
            {
                totalis_actiones += (s32)xar_numerus(st->actiones);
                totalis_goto += (s32)xar_numerus(st->goto_introitus);
            }
        }

        imprimere("  Totalis actiones: %d\n", (int)totalis_actiones);
        imprimere("  Totalis goto: %d\n", (int)totalis_goto);

        /* Rationabilis: non-vacuum, non nimis magnum */
        CREDO_VERUM(totalis_actiones > X);
        CREDO_VERUM(totalis_goto > V);
        CREDO_VERUM(totalis_actiones < D);
        CREDO_VERUM(totalis_goto < CC);
    }

    /* ========================================================
     * PROBARE IX: Imprimere tabulam pro inspectione
     * ======================================================== */
    {
        LapifexTabula* t;

        imprimere("\n--- Imprimere tabulam ---\n");

        t = tabulam_praeparare(piscina, intern, NIHIL, NIHIL);
        CREDO_NON_NIHIL(t);

        lapifex_tabulam_imprimere(t);
        lapifex_conflictus_imprimere(t);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */
    imprimere("\n");
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde (credo_omnia_praeterierunt() ? ZEPHYRUM : I);
}
