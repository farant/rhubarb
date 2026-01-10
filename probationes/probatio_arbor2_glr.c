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

        casus ARBOR2_NODUS_SENTENTIA:
            imprimere("SENTENTIA:\n");
            si (nodus->datum.sententia.expressio != NIHIL)
            {
                _imprimere_arborem(nodus->datum.sententia.expressio, profunditas + I);
            }
            frange;

        casus ARBOR2_NODUS_SENTENTIA_VACUA:
            imprimere("SENTENTIA_VACUA\n");
            frange;

        casus ARBOR2_NODUS_CORPUS:
            {
                i32 j;
                imprimere("CORPUS: %d sententiae\n",
                    nodus->datum.corpus.sententiae != NIHIL ?
                    xar_numerus(nodus->datum.corpus.sententiae) : ZEPHYRUM);
                si (nodus->datum.corpus.sententiae != NIHIL)
                {
                    per (j = ZEPHYRUM; j < xar_numerus(nodus->datum.corpus.sententiae); j++)
                    {
                        Arbor2Nodus* stmt;
                        stmt = *(Arbor2Nodus**)xar_obtinere(nodus->datum.corpus.sententiae, j);
                        _imprimere_arborem(stmt, profunditas + I);
                    }
                }
            }
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
     * PROBARE: foo * bar (ambiguous - could be expr or decl)
     * With no typedef info, should produce AMBIGUUS node
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans foo * bar (ambiguus) ---\n");

        /* Reset statistics */
        glr->num_furcae = ZEPHYRUM;
        glr->num_mergae = ZEPHYRUM;

        tokens = _lexare_ad_tokens(piscina, intern, "foo * bar");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        /* Without typedef info, foo * bar is ambiguous */
        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_AMBIGUUS);
            /* Should have 2 interpretations */
            si (res.radix->datum.ambiguus.interpretationes != NIHIL)
            {
                imprimere("  interpretationes: %d\n",
                    xar_numerus(res.radix->datum.ambiguus.interpretationes));
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.ambiguus.interpretationes), II);
            }
        }

        /* Should have forked */
        imprimere("  furcae: %d\n", glr->num_furcae);
        CREDO_AEQUALIS_I32((i32)(glr->num_furcae > ZEPHYRUM), VERUM);

        imprimere("  ambigui: %d\n", xar_numerus(res.ambigui));
    }


    /* ========================================================
     * PROBARE: MyType * ptr (typedef -> declaration only)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans MyType * ptr (typedef pruning) ---\n");

        /* MyType was already registered as typedef in earlier test */
        /* Reset statistics */
        glr->num_furcae = ZEPHYRUM;
        glr->num_mergae = ZEPHYRUM;

        tokens = _lexare_ad_tokens(piscina, intern, "MyType * ptr");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        /* With MyType as typedef, should be DECLARATIO not AMBIGUUS */
        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }

        /* Should NOT have forked (pruned expression path) */
        imprimere("  furcae: %d\n", glr->num_furcae);
        CREDO_AEQUALIS_I32((i32)glr->num_furcae, ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: Expression statement: x + 1;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans expression statement: x + 1; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "x + 1;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SENTENTIA);
            /* Check that expression is preserved */
            CREDO_NON_NIHIL(res.radix->datum.sententia.expressio);
            si (res.radix->datum.sententia.expressio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.sententia.expressio->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
            }
        }
    }


    /* ========================================================
     * PROBARE: Empty statement: ;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans empty statement: ; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, ";");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SENTENTIA_VACUA);
        }
    }


    /* ========================================================
     * PROBARE: Simple statement: 42;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans simple statement: 42; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "42;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SENTENTIA);
            si (res.radix->datum.sententia.expressio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.sententia.expressio->genus,
                                   (i32)ARBOR2_NODUS_INTEGER);
            }
        }
    }


    /* ========================================================
     * PROBARE: Empty compound statement: { }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans empty compound: { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "{ }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CORPUS);
            /* Empty compound should have 0 statements */
            CREDO_NON_NIHIL(res.radix->datum.corpus.sententiae);
            si (res.radix->datum.corpus.sententiae != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.corpus.sententiae), ZEPHYRUM);
            }
        }
    }


    /* ========================================================
     * PROBARE: Compound with one statement: { x; }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans compound with one stmt: { x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "{ x; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CORPUS);
            /* Should have 1 statement */
            si (res.radix->datum.corpus.sententiae != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.corpus.sententiae), I);
            }
        }
    }


    /* ========================================================
     * PROBARE: Compound with multiple statements: { x; y + 1; }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans compound multi stmt: { x; y + 1; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "{ x; y + 1; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CORPUS);
            /* Should have 2 statements */
            si (res.radix->datum.corpus.sententiae != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.corpus.sententiae), II);
            }
        }
    }


    /* ========================================================
     * PROBARE: Nested compound: { { x; } }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* inner;

        imprimere("\n--- Probans nested compound: { { x; } } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "{ { x; } }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CORPUS);
            /* Outer should have 1 statement (the inner compound) */
            si (res.radix->datum.corpus.sententiae != NIHIL &&
                xar_numerus(res.radix->datum.corpus.sententiae) > ZEPHYRUM)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.corpus.sententiae), I);

                /* Get first statement - should be CORPUS (compound became stmt) */
                inner = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.corpus.sententiae, ZEPHYRUM);
                CREDO_NON_NIHIL(inner);
                /* Inner compound is wrapped as CORPUS (via P15 stmt->compound passthrough) */
                si (inner != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)inner->genus, (i32)ARBOR2_NODUS_CORPUS);
                }
            }
        }
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
