/* probatio_lapifex_lr1.c - Probationes FOLLOW et LR(1) Collectionis */

#include "latina.h"
#include "lapifex_generare.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "credo.h"
#include <stdio.h>

/* Grammatica arithmetica probationis (eadem ac in probatio_lapifex_generare) */
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

/* Auxiliaris: verificare si FOLLOW continet terminalem per titulum */
hic_manens b32
follow_habet_terminalem(
    LapifexGrammatica*  grammatica,
    s32                 symbolum_index,
    constans character* terminalis_titulus)
{
    Xar* follow;
    i32  i;

    si (symbolum_index < ZEPHYRUM) redde FALSUM;

    follow = lapifex_follow_obtinere(grammatica, (i32)symbolum_index);
    si (!follow) redde FALSUM;

    per (i = ZEPHYRUM; i < (i32)xar_numerus(follow); i++)
    {
        s32* idx = (s32*)xar_obtinere(follow, (i32)i);
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

s32 principale(vacuum)
{
    Piscina*              piscina;
    InternamentumChorda*  intern;

    piscina = piscina_generare_dynamicum("probatio_lapifex_lr1", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ========================================================
     * PROBARE: FOLLOW coniuncta - expr
     * ======================================================== */
    {
        LapifexGrammatica* g;
        s32 expr_idx;
        s32 term_idx;
        s32 factor_idx;
        Xar* follow_expr;
        Xar* follow_term;
        Xar* follow_factor;

        imprimere("\n--- Probans FOLLOW coniuncta ---\n");

        g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_STML);
        CREDO_NON_NIHIL(g);

        CREDO_VERUM(lapifex_first_computare(g));
        CREDO_VERUM(lapifex_follow_computare(g));

        expr_idx = invenire_symbolum(g, "expr");
        term_idx = invenire_symbolum(g, "term");
        factor_idx = invenire_symbolum(g, "factor");

        CREDO_VERUM(expr_idx >= ZEPHYRUM);
        CREDO_VERUM(term_idx >= ZEPHYRUM);
        CREDO_VERUM(factor_idx >= ZEPHYRUM);

        /* FOLLOW(expr) = {PLUS, RPAREN, EOF, $}
         * Nota: EOF est terminale in grammatica, $ est sentinella -1
         * In productione augmentata: __initium__ -> expr EOF
         * Ergo FOLLOW(expr) continet EOF (terminalem) ex FIRST(EOF)
         * Et $ ex FOLLOW(__initium__)
         */
        follow_expr = lapifex_follow_obtinere(g, (i32)expr_idx);
        CREDO_NON_NIHIL(follow_expr);
        CREDO_VERUM(follow_habet_terminalem(g, expr_idx, "PLUS"));
        CREDO_VERUM(follow_habet_terminalem(g, expr_idx, "RPAREN"));
        CREDO_VERUM(follow_habet_terminalem(g, expr_idx, "EOF"));

        /* FOLLOW(term) = {PLUS, STAR, RPAREN, EOF, $} */
        follow_term = lapifex_follow_obtinere(g, (i32)term_idx);
        CREDO_NON_NIHIL(follow_term);
        CREDO_VERUM(follow_habet_terminalem(g, term_idx, "PLUS"));
        CREDO_VERUM(follow_habet_terminalem(g, term_idx, "STAR"));
        CREDO_VERUM(follow_habet_terminalem(g, term_idx, "RPAREN"));
        CREDO_VERUM(follow_habet_terminalem(g, term_idx, "EOF"));

        /* FOLLOW(factor) = {PLUS, STAR, RPAREN, EOF, $} */
        follow_factor = lapifex_follow_obtinere(g, (i32)factor_idx);
        CREDO_NON_NIHIL(follow_factor);
        CREDO_VERUM(follow_habet_terminalem(g, factor_idx, "PLUS"));
        CREDO_VERUM(follow_habet_terminalem(g, factor_idx, "STAR"));
        CREDO_VERUM(follow_habet_terminalem(g, factor_idx, "RPAREN"));
        CREDO_VERUM(follow_habet_terminalem(g, factor_idx, "EOF"));

        lapifex_follow_imprimere(g);
    }

    /* ========================================================
     * PROBARE: Collectio canonica LR(1)
     * ======================================================== */
    {
        LapifexGrammatica* g;
        LapifexCollectio*  collectio;
        i32 num_status;
        i32 num_transitiones;

        imprimere("\n--- Probans collectionem canonicam LR(1) ---\n");

        g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_STML);
        CREDO_NON_NIHIL(g);

        CREDO_VERUM(lapifex_first_computare(g));

        collectio = lapifex_collectio_construere(g);
        CREDO_NON_NIHIL(collectio);

        num_status = (i32)xar_numerus(collectio->status_omnes);
        num_transitiones = (i32)xar_numerus(collectio->transitiones);

        imprimere("  Status: %d, Transitiones: %d\n",
            (int)num_status, (int)num_transitiones);

        /* Status 0 existit et continet res initium */
        {
            LapifexStatus* status_0 = (LapifexStatus*)xar_obtinere(
                collectio->status_omnes, ZEPHYRUM);
            CREDO_NON_NIHIL(status_0);
            CREDO_AEQUALIS_S32(status_0->index, ZEPHYRUM);
            CREDO_VERUM((i32)xar_numerus(status_0->res) > ZEPHYRUM);
        }

        /* Numerus statuum rationabilis: LR(1) arithmetica ~20-40 */
        CREDO_VERUM(num_status > X);
        CREDO_VERUM(num_status < C);

        /* Transitiones existunt */
        CREDO_VERUM(num_transitiones > ZEPHYRUM);

        /* Nulla status duplicata: omnes indices unici */
        {
            i32 i;
            b32 duplicata_inventa = FALSUM;
            per (i = ZEPHYRUM; i < num_status; i++)
            {
                LapifexStatus* s = (LapifexStatus*)xar_obtinere(
                    collectio->status_omnes, i);
                si (s)
                {
                    CREDO_AEQUALIS_S32(s->index, (s32)i);
                }
            }
            CREDO_FALSUM(duplicata_inventa);
        }

        /* Verificare: existit configuratio accepta
         * Aliquis status continet [__initium__ -> expr EOF ., $]
         */
        {
            i32 i;
            b32 acceptum_inventum = FALSUM;
            s32 initium_idx = invenire_symbolum(g, "__initium__");

            per (i = ZEPHYRUM; i < num_status && !acceptum_inventum; i++)
            {
                LapifexStatus* s = (LapifexStatus*)xar_obtinere(
                    collectio->status_omnes, i);
                i32 j;

                si (!s) perge;

                per (j = ZEPHYRUM; j < (i32)xar_numerus(s->res); j++)
                {
                    LapifexRes* res = (LapifexRes*)xar_obtinere(s->res, j);
                    LapifexProductio* prod;

                    si (!res) perge;

                    prod = (LapifexProductio*)xar_obtinere(
                        g->productiones, (i32)res->productio);
                    si (!prod) perge;

                    /* Productio augmentata cum punctum ad finem et $ prospectus */
                    si (prod->sinistrum == initium_idx &&
                        (i32)res->punctum == (i32)xar_numerus(prod->dextrum) &&
                        res->prospectus == -I)
                    {
                        acceptum_inventum = VERUM;
                        imprimere("  Acceptum in statu %d\n", (int)i);
                        frange;
                    }
                }
            }
            CREDO_VERUM(acceptum_inventum);
        }

        /* Imprimere collectionem pro inspectione */
        lapifex_collectio_imprimere(collectio);
    }

    /* ========================================================
     * PROBARE: Status 0 continet res correctas
     * ======================================================== */
    {
        LapifexGrammatica* g;
        LapifexCollectio*  collectio;
        LapifexStatus*     status_0;
        s32 initium_idx;
        b32 habet_initium_res = FALSUM;
        i32 j;

        imprimere("\n--- Probans statum 0 ---\n");

        g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_STML);
        CREDO_NON_NIHIL(g);
        CREDO_VERUM(lapifex_first_computare(g));

        collectio = lapifex_collectio_construere(g);
        CREDO_NON_NIHIL(collectio);

        status_0 = (LapifexStatus*)xar_obtinere(
            collectio->status_omnes, ZEPHYRUM);
        CREDO_NON_NIHIL(status_0);

        initium_idx = invenire_symbolum(g, "__initium__");

        /* Status 0 debet habere res cum productione augmentata */
        per (j = ZEPHYRUM; j < (i32)xar_numerus(status_0->res); j++)
        {
            LapifexRes* res = (LapifexRes*)xar_obtinere(status_0->res, j);
            LapifexProductio* prod;

            si (!res) perge;

            prod = (LapifexProductio*)xar_obtinere(
                g->productiones, (i32)res->productio);
            si (!prod) perge;

            si (prod->sinistrum == initium_idx &&
                res->punctum == ZEPHYRUM &&
                res->prospectus == -I)
            {
                habet_initium_res = VERUM;
            }
        }
        CREDO_VERUM(habet_initium_res);

        /* Status 0 debet habere plures res (clausura expandit) */
        CREDO_VERUM((i32)xar_numerus(status_0->res) > I);
        imprimere("  Status 0: %d res\n",
            (int)xar_numerus(status_0->res));
    }

    /* ========================================================
     * PROBARE: Transitiones ex statu 0
     * ======================================================== */
    {
        LapifexGrammatica* g;
        LapifexCollectio*  collectio;
        i32 num_trans_ex_0 = ZEPHYRUM;
        i32 t;

        imprimere("\n--- Probans transitiones ex statu 0 ---\n");

        g = lapifex_grammaticam_legere(piscina, intern, GRAMMATICA_STML);
        CREDO_NON_NIHIL(g);
        CREDO_VERUM(lapifex_first_computare(g));

        collectio = lapifex_collectio_construere(g);
        CREDO_NON_NIHIL(collectio);

        /* Numerare transitiones ex statu 0 */
        per (t = ZEPHYRUM; t < (i32)xar_numerus(collectio->transitiones); t++)
        {
            LapifexTransitio* trans = (LapifexTransitio*)xar_obtinere(
                collectio->transitiones, t);
            si (trans && trans->status == ZEPHYRUM)
            {
                num_trans_ex_0++;
            }
        }

        /* Status 0 debet habere transitiones pro:
         * expr, term, factor, NUMBER, LPAREN (minimum 5)
         */
        imprimere("  Transitiones ex statu 0: %d\n", (int)num_trans_ex_0);
        CREDO_VERUM(num_trans_ex_0 >= V);
    }

    /* ========================================================
     * Compendium
     * ======================================================== */
    imprimere("\n");
    credo_imprimere_compendium();

    piscina_destruere(piscina);

    redde (credo_omnia_praeterierunt() ? ZEPHYRUM : I);
}
