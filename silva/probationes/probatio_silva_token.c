/* probatio_silva_token.c - Probationes contractus lexematis unificati */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior chorda*
_titulus_novus (Piscina* piscina, constans character* literis)
{
    chorda* titulus;

    titulus = (chorda*)piscina_allocare(piscina, (memoriae_index)magnitudo(chorda));
    *titulus = chorda_ex_literis(literis, piscina);
    redde titulus;
}

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_token", 65536);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: silva_token_ex_fonte
     * ======================================================== */

    {
        SilvaToken* token;
        chorda      valor;

        imprimere("\n--- Probans silva_token_ex_fonte ---\n");

        valor = chorda_ex_literis("piscina", piscina);
        token = silva_token_ex_fonte(piscina, SILVA_LEX_IDENTIFICATOR,
            valor, XLII, III, VII, ZEPHYRUM);

        CREDO_NON_NIHIL (token);
        CREDO_AEQUALIS_I32 ((i32)token->genus, (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_S32 (token->byte_offset, XLII);
        CREDO_AEQUALIS_I32 (token->longitudo, VII);
        CREDO_AEQUALIS_I32 (token->linea, III);
        CREDO_AEQUALIS_I32 (token->columna, VII);
        CREDO_AEQUALIS_S32 (token->fons_index, ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (token->valor, "piscina");

        /* Origo inserta: FONS, sine praedecessoribus */
        CREDO_AEQUALIS_I32 ((i32)token->origo.genus, (i32)SILVA_ORIGO_FONS);
        CREDO_VERUM (silva_token_est_fons(token));
        CREDO_AEQUALIS_PTR (silva_token_radix(token), token);
        CREDO_AEQUALIS_I32 (silva_token_profunditas(token), ZEPHYRUM);

        /* Trivia et scissurae: NIHIL per defaltam */
        CREDO_NIHIL (token->spatia_ante);
        CREDO_NIHIL (token->spatia_post);
        CREDO_NIHIL (token->scissurae);
        CREDO_FALSUM (token->initium_lineae);
    }


    /* ========================================================
     * PROBARE: catena expansionis (duae generationes)
     * ======================================================== */

    {
        SilvaToken* corpus_m;    /* lexema in corpore macro M (def-site) */
        SilvaToken* invocatio_m; /* lexema "M" in codice utentis (use-site) */
        SilvaToken* corpus_n;    /* lexema in corpore macro N */
        SilvaToken* gen1;
        SilvaToken* gen2;
        chorda*     nomen_m;
        chorda*     nomen_n;

        imprimere("\n--- Probans catenam expansionis ---\n");

        nomen_m = _titulus_novus(piscina, "M");
        nomen_n = _titulus_novus(piscina, "N");

        corpus_m = silva_token_ex_fonte(piscina, SILVA_LEX_INTEGER,
            chorda_ex_literis("42", piscina), X, I, IX, I);
        invocatio_m = silva_token_ex_fonte(piscina, SILVA_LEX_IDENTIFICATOR,
            chorda_ex_literis("M", piscina), C, V, I, ZEPHYRUM);

        gen1 = silva_token_ex_expansione(piscina, corpus_m, invocatio_m,
            nomen_m, NIHIL);

        /* Identitas lexicalis a corpore (def-site) */
        CREDO_AEQUALIS_I32 ((i32)gen1->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (gen1->valor, "42");
        CREDO_AEQUALIS_S32 (gen1->fons_index, I);

        /* Catena use-site */
        CREDO_AEQUALIS_I32 ((i32)gen1->origo.genus, (i32)SILVA_ORIGO_EXPANSIO);
        CREDO_AEQUALIS_PTR (gen1->origo.datum.expansio.corpus, corpus_m);
        CREDO_AEQUALIS_PTR (gen1->origo.datum.expansio.invocatio, invocatio_m);
        CREDO_AEQUALIS_PTR (silva_token_radix(gen1), invocatio_m);
        CREDO_AEQUALIS_I32 (silva_token_profunditas(gen1), I);
        CREDO_FALSUM (silva_token_est_fons(gen1));

        /* Generatio secunda: N cuius invocatio est gen1 */
        corpus_n = silva_token_ex_fonte(piscina, SILVA_LEX_PLUS,
            chorda_ex_literis("+", piscina), XX, II, IV, I);
        gen2 = silva_token_ex_expansione(piscina, corpus_n, gen1,
            nomen_n, NIHIL);

        CREDO_AEQUALIS_PTR (silva_token_radix(gen2), invocatio_m);
        CREDO_AEQUALIS_I32 (silva_token_profunditas(gen2), II);
    }


    /* ========================================================
     * PROBARE: pasta (##) - parentes ambo servati
     * ======================================================== */

    {
        SilvaToken* sinister;
        SilvaToken* dexter;
        SilvaToken* pasta;
        chorda*     titulus;

        imprimere("\n--- Probans pastam ---\n");

        titulus = _titulus_novus(piscina, "GLUE");
        sinister = silva_token_ex_fonte(piscina, SILVA_LEX_IDENTIFICATOR,
            chorda_ex_literis("silva_", piscina), ZEPHYRUM, I, I, ZEPHYRUM);
        dexter = silva_token_ex_fonte(piscina, SILVA_LEX_IDENTIFICATOR,
            chorda_ex_literis("crescat", piscina), X, I, XI, ZEPHYRUM);

        pasta = silva_token_ex_pasta(piscina, SILVA_LEX_IDENTIFICATOR,
            chorda_ex_literis("silva_crescat", piscina),
            sinister, dexter, titulus, NIHIL);

        CREDO_CHORDA_AEQUALIS_LITERIS (pasta->valor, "silva_crescat");
        CREDO_AEQUALIS_I32 ((i32)pasta->origo.genus, (i32)SILVA_ORIGO_PASTA);
        CREDO_AEQUALIS_PTR (pasta->origo.datum.pasta.sinister, sinister);
        CREDO_AEQUALIS_PTR (pasta->origo.datum.pasta.dexter, dexter);

        /* Radix sequitur sinistrum (catena primaria) */
        CREDO_AEQUALIS_PTR (silva_token_radix(pasta), sinister);
    }


    /* ========================================================
     * PROBARE: caecatio - extensio O(1), cauda communis
     * ======================================================== */

    {
        SilvaCaecatio* c_m;
        SilvaCaecatio* c_mn;
        SilvaCaecatio* c_mx;
        chorda*        nomen_m;
        chorda*        nomen_n;
        chorda*        nomen_x;

        imprimere("\n--- Probans caecationem ---\n");

        nomen_m = _titulus_novus(piscina, "M");
        nomen_n = _titulus_novus(piscina, "N");
        nomen_x = _titulus_novus(piscina, "X");

        c_m = silva_caecatio_extendere(piscina, NIHIL, nomen_m);
        CREDO_NON_NIHIL (c_m);
        CREDO_VERUM  (silva_caecatio_continet(c_m, chorda_ex_literis("M", piscina)));
        CREDO_FALSUM (silva_caecatio_continet(c_m, chorda_ex_literis("N", piscina)));
        CREDO_FALSUM (silva_caecatio_continet(NIHIL, chorda_ex_literis("M", piscina)));

        /* Fratres caudam communicant - structura, non exemplaria */
        c_mn = silva_caecatio_extendere(piscina, c_m, nomen_n);
        c_mx = silva_caecatio_extendere(piscina, c_m, nomen_x);
        CREDO_AEQUALIS_PTR (c_mn->cauda, c_m);
        CREDO_AEQUALIS_PTR (c_mx->cauda, c_m);

        /* Continentia per caudam */
        CREDO_VERUM  (silva_caecatio_continet(c_mn, chorda_ex_literis("M", piscina)));
        CREDO_VERUM  (silva_caecatio_continet(c_mn, chorda_ex_literis("N", piscina)));
        CREDO_FALSUM (silva_caecatio_continet(c_mn, chorda_ex_literis("X", piscina)));
        CREDO_VERUM  (silva_caecatio_continet(c_mx, chorda_ex_literis("X", piscina)));
        CREDO_FALSUM (silva_caecatio_continet(c_mx, chorda_ex_literis("N", piscina)));
    }


    /* ========================================================
     * PROBARE: nomina generum - ordo enumerationis et tabulae
     * congruunt (primus, medius, ultimus)
     * ======================================================== */

    {
        imprimere("\n--- Probans nomina generum ---\n");

        CREDO_VERUM (strcmp(silva_lexema_genus_nomen(SILVA_LEX_EOF), "EOF") == ZEPHYRUM);
        CREDO_VERUM (strcmp(silva_lexema_genus_nomen(SILVA_LEX_IDENTIFICATOR), "IDENTIFICATOR") == ZEPHYRUM);
        CREDO_VERUM (strcmp(silva_lexema_genus_nomen(SILVA_LEX_WHILE), "WHILE") == ZEPHYRUM);
        CREDO_VERUM (strcmp(silva_lexema_genus_nomen(SILVA_LEX_ASSIGNATIO), "ASSIGNATIO") == ZEPHYRUM);
        CREDO_VERUM (strcmp(silva_lexema_genus_nomen(SILVA_LEX_CANCELLUM_CANCELLUM), "CANCELLUM_CANCELLUM") == ZEPHYRUM);
        CREDO_VERUM (strcmp(silva_lexema_genus_nomen(SILVA_LEX_COMMENTUM_LINEA), "COMMENTUM_LINEA") == ZEPHYRUM);
        CREDO_VERUM (strcmp(silva_origo_genus_nomen(SILVA_ORIGO_PASTA), "PASTA") == ZEPHYRUM);
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
