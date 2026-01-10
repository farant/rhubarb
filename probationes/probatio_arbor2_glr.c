/* probatio_arbor2_glr.c - Probationes Arbor2 GLR Parser */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "arbor2_expandere.h"
#include "arbor2_glr.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Helper: Lex source to Arbor2Token array
 * ================================================== */

hic_manens Xar*
_lexare_ad_tokens(
    Piscina*                piscina,
    InternamentumChorda*    intern,
    constans character*     fons)
{
    Xar* tokens;
    Arbor2Lexator* lex;
    Arbor2Lexema* lexema;
    chorda* via;
    unio { constans character* c; i8* m; } u;

    tokens = xar_creare(piscina, magnitudo(Arbor2Token*));

    u.c = "test.c";
    via = piscina_allocare(piscina, magnitudo(chorda));
    via->datum = u.m;
    via->mensura = VI;

    lex = arbor2_lexator_creare(piscina, intern, fons, (i32)strlen(fons));

    dum (VERUM)
    {
        Arbor2Token* tok;
        Arbor2Token** slot;

        lexema = arbor2_lexema_proximum(lex);
        si (lexema == NIHIL)
        {
            frange;
        }

        /* Skip newlines for expression parsing */
        si (lexema->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            perge;
        }

        tok = arbor2_token_ex_lexema(piscina, lexema, via,
            lexema->linea, lexema->columna);
        slot = xar_addere(tokens);
        *slot = tok;

        si (lexema->genus == ARBOR2_LEXEMA_EOF)
        {
            frange;
        }
    }

    redde tokens;
}

/* ==================================================
 * Helper: Print AST for debugging
 * ================================================== */

hic_manens vacuum
_imprimere_arborem(Arbor2Nodus* nodus, i32 profunditas)
{
    i32 i;

    si (nodus == NIHIL)
    {
        redde;
    }

    per (i = ZEPHYRUM; i < profunditas; i++)
    {
        imprimere("  ");
    }

    commutatio (nodus->genus)
    {
        casus ARBOR2_NODUS_IDENTIFICATOR:
            imprimere("IDENTIFICATOR: %.*s\n",
                (integer)nodus->datum.folium.valor.mensura,
                nodus->datum.folium.valor.datum);
            frange;

        casus ARBOR2_NODUS_INTEGER:
            imprimere("INTEGER: %.*s\n",
                (integer)nodus->datum.folium.valor.mensura,
                nodus->datum.folium.valor.datum);
            frange;

        casus ARBOR2_NODUS_BINARIUM:
            imprimere("BINARIUM: op=%s\n",
                arbor2_lexema_genus_nomen(nodus->datum.binarium.operator));
            _imprimere_arborem(nodus->datum.binarium.sinister, profunditas + I);
            _imprimere_arborem(nodus->datum.binarium.dexter, profunditas + I);
            frange;

        casus ARBOR2_NODUS_UNARIUM:
            imprimere("UNARIUM: op=%s\n",
                arbor2_lexema_genus_nomen(nodus->datum.unarium.operator));
            _imprimere_arborem(nodus->datum.unarium.operandum, profunditas + I);
            frange;

        ordinarius:
            imprimere("NODUS: %s\n", arbor2_nodus_genus_nomen(nodus->genus));
            frange;
    }
}

/* ==================================================
 * Tests
 * ================================================== */

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    /* Aperire credo et piscina */
    piscina = piscina_generare_dynamicum("probatio_arbor2_glr", 131072);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* Create expansion context (for macro lookahead) */
    expansion = arbor2_expansion_creare(piscina, intern);

    /* Create GLR parser */
    glr = arbor2_glr_creare(piscina, intern, expansion);


    /* ========================================================
     * PROBARE: Simple identifier
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans simple identifier ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "foo");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        imprimere("  radix: %p\n", (vacuum*)res.radix);

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_IDENTIFICATOR);
        }
    }


    /* ========================================================
     * PROBARE: Simple integer
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans simple integer ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "42");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_INTEGER);
        }
    }


    /* ========================================================
     * PROBARE: Binary addition
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans binary addition: 1 + 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 + 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_PLUS);
        }
    }


    /* ========================================================
     * PROBARE: Precedence - 1 + 2 * 3
     * Should parse as 1 + (2 * 3), not (1 + 2) * 3
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;

        imprimere("\n--- Probans precedence: 1 + 2 * 3 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 + 2 * 3");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be + */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_PLUS);

            /* Left should be 1 */
            CREDO_NON_NIHIL(radix->datum.binarium.sinister);

            /* Right should be 2 * 3 */
            dexter = radix->datum.binarium.dexter;
            CREDO_NON_NIHIL(dexter);
            si (dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)dexter->genus, (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_ASTERISCUS);
            }
        }
    }


    /* ========================================================
     * PROBARE: Parenthesized expression
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans parenthesized: (1 + 2) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(1 + 2)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        /* Should unwrap to 1 + 2 */
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
        }
    }


    /* ========================================================
     * PROBARE: Unary dereference
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans unary dereference: *ptr ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "*ptr");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.unarium.operator,
                               (i32)ARBOR2_LEXEMA_ASTERISCUS);
        }
    }


    /* ========================================================
     * PROBARE: Unary address-of
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans unary address-of: &x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "&x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.unarium.operator,
                               (i32)ARBOR2_LEXEMA_AMPERSAND);
        }
    }


    /* ========================================================
     * PROBARE: Typedef detection API
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2Token* tok;

        imprimere("\n--- Probans typedef detection API ---\n");

        /* Register a typedef */
        arbor2_expansion_addere_typedef(expansion, "MyType");

        /* Test with known typedef */
        tokens = _lexare_ad_tokens(piscina, intern, "MyType");
        tok = *(Arbor2Token**)xar_obtinere(tokens, ZEPHYRUM);

        imprimere("  MyType est typus: %s\n",
            arbor2_glr_est_probabiliter_typus(glr, tok) ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)arbor2_glr_est_probabiliter_typus(glr, tok), VERUM);

        /* Test with unknown identifier */
        tokens = _lexare_ad_tokens(piscina, intern, "unknown");
        tok = *(Arbor2Token**)xar_obtinere(tokens, ZEPHYRUM);

        imprimere("  unknown est typus: %s\n",
            arbor2_glr_est_probabiliter_typus(glr, tok) ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)arbor2_glr_est_probabiliter_typus(glr, tok), FALSUM);
    }


    /* ========================================================
     * PROBARE: foo * bar (currently unambiguous - expression only)
     * When we add ambiguous states, this will fork and potentially merge
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans foo * bar (expression grammar) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "foo * bar");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        /* With current expression-only grammar, foo * bar is multiplication */
        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_ASTERISCUS);
        }

        /* No ambiguities with current grammar */
        imprimere("  ambigui: %d\n", xar_numerus(res.ambigui));
    }


    /* ========================================================
     * PROBARE: Parser statistics
     * ======================================================== */

    {
        imprimere("\n--- Parser statistics ---\n");
        imprimere("  Forks:     %d\n", glr->num_furcae);
        imprimere("  Merges:    %d\n", glr->num_mergae);
        imprimere("  Max front: %d\n", glr->max_frons);
    }


    /* Claudere credo */
    credo_claudere();
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
