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

        casus ARBOR2_NODUS_SI:
            imprimere("SI:\n");
            /* Print condition */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("conditio:\n");
            _imprimere_arborem(nodus->datum.conditionale.conditio, profunditas + II);
            /* Print then-branch */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("consequens:\n");
            _imprimere_arborem(nodus->datum.conditionale.consequens, profunditas + II);
            /* Print else-branch if present */
            si (nodus->datum.conditionale.alternans != NIHIL)
            {
                per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
                imprimere("alternans:\n");
                _imprimere_arborem(nodus->datum.conditionale.alternans, profunditas + II);
            }
            frange;

        casus ARBOR2_NODUS_DUM:
            imprimere("DUM:\n");
            /* Print condition */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("conditio:\n");
            _imprimere_arborem(nodus->datum.iteratio.conditio, profunditas + II);
            /* Print body */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("corpus:\n");
            _imprimere_arborem(nodus->datum.iteratio.corpus, profunditas + II);
            frange;

        casus ARBOR2_NODUS_FAC:
            imprimere("FAC:\n");
            /* Print body */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("corpus:\n");
            _imprimere_arborem(nodus->datum.iteratio.corpus, profunditas + II);
            /* Print condition */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("conditio:\n");
            _imprimere_arborem(nodus->datum.iteratio.conditio, profunditas + II);
            frange;

        casus ARBOR2_NODUS_PER:
            imprimere("PER:\n");
            /* Print init */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("initium: ");
            si (nodus->datum.circuitus.initium != NIHIL)
            {
                imprimere("\n");
                _imprimere_arborem(nodus->datum.circuitus.initium, profunditas + II);
            }
            alioquin
            {
                imprimere("(nihil)\n");
            }
            /* Print condition */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("conditio: ");
            si (nodus->datum.circuitus.conditio != NIHIL)
            {
                imprimere("\n");
                _imprimere_arborem(nodus->datum.circuitus.conditio, profunditas + II);
            }
            alioquin
            {
                imprimere("(nihil)\n");
            }
            /* Print increment */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("incrementum: ");
            si (nodus->datum.circuitus.incrementum != NIHIL)
            {
                imprimere("\n");
                _imprimere_arborem(nodus->datum.circuitus.incrementum, profunditas + II);
            }
            alioquin
            {
                imprimere("(nihil)\n");
            }
            /* Print body */
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("corpus:\n");
            _imprimere_arborem(nodus->datum.circuitus.corpus, profunditas + II);
            frange;

        casus ARBOR2_NODUS_FRANGE:
            imprimere("FRANGE\n");
            frange;

        casus ARBOR2_NODUS_PERGE:
            imprimere("PERGE\n");
            frange;

        casus ARBOR2_NODUS_REDDE:
            imprimere("REDDE:\n");
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("valor: ");
            si (nodus->datum.reditio.valor != NIHIL)
            {
                imprimere("\n");
                _imprimere_arborem(nodus->datum.reditio.valor, profunditas + II);
            }
            alioquin
            {
                imprimere("(nihil)\n");
            }
            frange;

        casus ARBOR2_NODUS_SALTA:
            imprimere("SALTA: %.*s\n",
                     (integer)nodus->datum.saltus.destinatio.mensura,
                     nodus->datum.saltus.destinatio.datum);
            frange;

        casus ARBOR2_NODUS_TITULATUM:
            imprimere("TITULATUM: %.*s\n",
                     (integer)nodus->datum.titulatum.titulus.mensura,
                     nodus->datum.titulatum.titulus.datum);
            si (nodus->datum.titulatum.sententia != NIHIL)
            {
                _imprimere_arborem(nodus->datum.titulatum.sententia, profunditas + I);
            }
            frange;

        casus ARBOR2_NODUS_COMMUTATIO:
            imprimere("COMMUTATIO:\n");
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("expressio:\n");
            si (nodus->datum.selectivum.expressio != NIHIL)
            {
                _imprimere_arborem(nodus->datum.selectivum.expressio, profunditas + II);
            }
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("corpus:\n");
            si (nodus->datum.selectivum.corpus != NIHIL)
            {
                _imprimere_arborem(nodus->datum.selectivum.corpus, profunditas + II);
            }
            frange;

        casus ARBOR2_NODUS_CASUS:
            imprimere("CASUS:\n");
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("valor:\n");
            si (nodus->datum.electio.valor != NIHIL)
            {
                _imprimere_arborem(nodus->datum.electio.valor, profunditas + II);
            }
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("sententia:\n");
            si (nodus->datum.electio.sententia != NIHIL)
            {
                _imprimere_arborem(nodus->datum.electio.sententia, profunditas + II);
            }
            frange;

        casus ARBOR2_NODUS_ORDINARIUS:
            imprimere("ORDINARIUS:\n");
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("sententia:\n");
            si (nodus->datum.defectus.sententia != NIHIL)
            {
                _imprimere_arborem(nodus->datum.defectus.sententia, profunditas + II);
            }
            frange;

        casus ARBOR2_NODUS_DECLARATOR_FUNCTI:
            {
                i32 num_params = ZEPHYRUM;
                si (nodus->datum.declarator_functi.parametri != NIHIL)
                {
                    num_params = xar_numerus(nodus->datum.declarator_functi.parametri);
                }
                imprimere("DECLARATOR_FUNCTI: habet_void=%s, params=%d\n",
                         nodus->datum.declarator_functi.habet_void ? "VERUM" : "FALSUM",
                         num_params);
                si (nodus->datum.declarator_functi.declarator_interior != NIHIL)
                {
                    _imprimere_arborem(nodus->datum.declarator_functi.declarator_interior, profunditas + I);
                }
                /* Print parameters if present */
                si (nodus->datum.declarator_functi.parametri != NIHIL)
                {
                    i32 j;
                    per (j = ZEPHYRUM; j < num_params; j++)
                    {
                        Arbor2Nodus* param;
                        param = *(Arbor2Nodus**)xar_obtinere(nodus->datum.declarator_functi.parametri, j);
                        per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
                        imprimere("param[%d]:\n", j);
                        _imprimere_arborem(param, profunditas + II);
                    }
                }
            }
            frange;

        casus ARBOR2_NODUS_PARAMETER_DECL:
            imprimere("PARAMETER_DECL:\n");
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("type:\n");
            si (nodus->datum.parameter_decl.type_specifier != NIHIL)
            {
                _imprimere_arborem(nodus->datum.parameter_decl.type_specifier, profunditas + II);
            }
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("declarator:\n");
            si (nodus->datum.parameter_decl.declarator != NIHIL)
            {
                _imprimere_arborem(nodus->datum.parameter_decl.declarator, profunditas + II);
            }
            frange;

        casus ARBOR2_NODUS_DEFINITIO_FUNCTI:
            imprimere("DEFINITIO_FUNCTI:\n");
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("specifier:\n");
            si (nodus->datum.definitio_functi.specifier != NIHIL)
            {
                _imprimere_arborem(nodus->datum.definitio_functi.specifier, profunditas + II);
            }
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("declarator:\n");
            si (nodus->datum.definitio_functi.declarator != NIHIL)
            {
                _imprimere_arborem(nodus->datum.definitio_functi.declarator, profunditas + II);
            }
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("corpus:\n");
            si (nodus->datum.definitio_functi.corpus != NIHIL)
            {
                _imprimere_arborem(nodus->datum.definitio_functi.corpus, profunditas + II);
            }
            frange;

        casus ARBOR2_NODUS_STRUCT_SPECIFIER:
        {
            Arbor2Nodus* tag = nodus->datum.struct_specifier.tag;
            Xar* membra = nodus->datum.struct_specifier.membra;
            character constans* genus_nomen = nodus->datum.struct_specifier.est_unio ?
                "UNION_SPECIFIER" : "STRUCT_SPECIFIER";

            si (tag != NIHIL)
            {
                imprimere("%s: %.*s\n", genus_nomen,
                    (i32)tag->datum.folium.valor.mensura,
                    tag->datum.folium.valor.datum);
            }
            alioquin
            {
                imprimere("%s: (anonymous)\n", genus_nomen);
            }

            si (membra != NIHIL)
            {
                i32 num = xar_numerus(membra);
                i32 j;
                per (j = ZEPHYRUM; j < num; j++)
                {
                    Arbor2Nodus** slot = xar_obtinere(membra, j);
                    per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
                    imprimere("member[%d]:\n", j);
                    si (slot != NIHIL && *slot != NIHIL)
                    {
                        _imprimere_arborem(*slot, profunditas + II);
                    }
                }
            }
            frange;
        }

        casus ARBOR2_NODUS_ENUM_SPECIFIER:
        {
            Arbor2Nodus* tag = nodus->datum.enum_specifier.tag;
            Xar* enumeratores = nodus->datum.enum_specifier.enumeratores;

            si (tag != NIHIL)
            {
                imprimere("ENUM_SPECIFIER: %.*s\n",
                    (i32)tag->datum.folium.valor.mensura,
                    tag->datum.folium.valor.datum);
            }
            alioquin
            {
                imprimere("ENUM_SPECIFIER: (anonymous)\n");
            }

            si (enumeratores != NIHIL)
            {
                i32 num = xar_numerus(enumeratores);
                i32 j;
                per (j = ZEPHYRUM; j < num; j++)
                {
                    Arbor2Nodus** slot = xar_obtinere(enumeratores, j);
                    si (slot != NIHIL && *slot != NIHIL)
                    {
                        _imprimere_arborem(*slot, profunditas + I);
                    }
                }
            }
            frange;
        }

        casus ARBOR2_NODUS_ENUMERATOR:
        {
            chorda titulus = nodus->datum.enumerator.titulus;
            Arbor2Nodus* valor = nodus->datum.enumerator.valor;

            imprimere("ENUMERATOR: %.*s",
                (i32)titulus.mensura, titulus.datum);

            si (valor != NIHIL)
            {
                imprimere(" = ");
                /* Imprimere valor inline */
                si (valor->genus == ARBOR2_NODUS_INTEGER)
                {
                    imprimere("%.*s", (i32)valor->datum.folium.valor.mensura,
                        valor->datum.folium.valor.datum);
                }
                alioquin si (valor->genus == ARBOR2_NODUS_IDENTIFICATOR)
                {
                    imprimere("%.*s", (i32)valor->datum.folium.valor.mensura,
                        valor->datum.folium.valor.datum);
                }
                alioquin
                {
                    imprimere("(expr)");
                }
            }
            imprimere("\n");
            frange;
        }

        casus ARBOR2_NODUS_DECLARATOR:
        {
            chorda titulus = nodus->datum.declarator.titulus;
            Arbor2Nodus* latitudo = nodus->datum.declarator.latitudo_biti;

            si (titulus.mensura > ZEPHYRUM)
            {
                imprimere("DECLARATOR: %.*s", (i32)titulus.mensura, titulus.datum);
            }
            alioquin
            {
                imprimere("DECLARATOR: (anonymous)");
            }

            si (nodus->datum.declarator.num_stellae > ZEPHYRUM)
            {
                imprimere(" (ptr=%d)", nodus->datum.declarator.num_stellae);
            }

            si (latitudo != NIHIL)
            {
                imprimere(" : ");
                /* Print bit width inline */
                si (latitudo->genus == ARBOR2_NODUS_INTEGER)
                {
                    imprimere("%.*s", (i32)latitudo->datum.folium.valor.mensura,
                        latitudo->datum.folium.valor.datum);
                }
                alioquin si (latitudo->genus == ARBOR2_NODUS_IDENTIFICATOR)
                {
                    imprimere("%.*s", (i32)latitudo->datum.folium.valor.mensura,
                        latitudo->datum.folium.valor.datum);
                }
                alioquin
                {
                    imprimere("(expr)");
                }
            }
            imprimere("\n");
            frange;
        }

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
     * PROBARE: Subtraction (E9)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans subtraction: 5 - 3 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "5 - 3");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINUS);
        }
    }

    /* ========================================================
     * PROBARE: Division (E9)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans division: 6 / 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "6 / 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_SOLIDUS);
        }
    }

    /* ========================================================
     * PROBARE: Modulo (E9)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans modulo: 7 %% 3 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "7 % 3");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_PERCENTUM);
        }
    }

    /* ========================================================
     * PROBARE: Mixed precedence (E9)
     * 1 + 2 * 3 - 4 / 2 should parse as (1 + (2 * 3)) - (4 / 2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans mixed ops: 10 - 6 / 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "10 - 6 / 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        /* Root should be - (lower precedence than /) */
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINUS);
            /* Right child should be / */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_SOLIDUS);
            }
        }
    }


    /* ========================================================
     * PROBARE: Comparison operator < (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio minor: 1 < 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 < 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINOR);
        }
    }

    /* ========================================================
     * PROBARE: Comparison operator > (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio maior: 3 > 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "3 > 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MAIOR);
        }
    }

    /* ========================================================
     * PROBARE: Comparison operator <= (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio minor_aeq: 2 <= 3 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "2 <= 3");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINOR_AEQ);
        }
    }

    /* ========================================================
     * PROBARE: Comparison operator >= (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio maior_aeq: 5 >= 4 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "5 >= 4");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MAIOR_AEQ);
        }
    }

    /* ========================================================
     * PROBARE: Equality operator == (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans aequalitas: 1 == 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 == 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_AEQUALIS);
        }
    }

    /* ========================================================
     * PROBARE: Inequality operator != (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans non-aequalitas: 1 != 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 != 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_NON_AEQUALIS);
        }
    }

    /* ========================================================
     * PROBARE: Comparison with expressions: 1 + 2 < 3 + 4 (E9.2)
     * Should parse as (1 + 2) < (3 + 4)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio cum expressiis: 1 + 2 < 3 + 4 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 + 2 < 3 + 4");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Root should be < */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINOR);
            /* Left child should be + */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_PLUS);
            }
            /* Right child should be + */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_PLUS);
            }
        }
    }

    /* ========================================================
     * PROBARE: Equality with comparisons: 1 < 2 == 3 < 4 (E9.2)
     * Should parse as (1 < 2) == (3 < 4)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans aequalitas cum comparatiis: 1 < 2 == 3 < 4 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 < 2 == 3 < 4");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Root should be == */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_AEQUALIS);
            /* Left child should be < */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_MINOR);
            }
            /* Right child should be < */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_MINOR);
            }
        }
    }

    /* ========================================================
     * PROBARE: Mixed precedence: 1 + 2 < 3 * 4 (E9.2)
     * Should parse as (1 + 2) < (3 * 4) - multiplication binds tighter
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans praecedentia mixta: 1 + 2 < 3 * 4 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 + 2 < 3 * 4");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Root should be < */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINOR);
            /* Left child should be + (1 + 2) */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_PLUS);
            }
            /* Right child should be * (3 * 4) */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_ASTERISCUS);
            }
        }
    }

    /* ========================================================
     * PROBARE: Comparison inside parens: (1 < 2) (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio in parens: (1 < 2) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(1 < 2)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINOR);
        }
    }

    /* ========================================================
     * PROBARE: Nested parens with comparison: ((1 < 2)) (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans parens nidata: ((1 < 2)) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "((1 < 2))");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINOR);
        }
    }

    /* ========================================================
     * PROBARE: Chained equality: 1 == 2 == 3 (E9.2)
     * Left-associative: ((1 == 2) == 3)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans aequalitas catena: 1 == 2 == 3 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 == 2 == 3");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Root should be == (the second one) */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_AEQUALIS);
            /* Left child should be == (the first one) - left associative */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_AEQUALIS);
            }
            /* Right child should be 3 (integer) */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_INTEGER);
            }
        }
    }

    /* ========================================================
     * PROBARE: Chained comparison: 1 < 2 < 3 (E9.2)
     * Left-associative: ((1 < 2) < 3)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio catena: 1 < 2 < 3 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 < 2 < 3");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Root should be < (the second one) */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINOR);
            /* Left child should be < (the first one) - left associative */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_MINOR);
            }
        }
    }

    /* ========================================================
     * PROBARE: Complex RHS: 1 < 2 * 3 + 4 (E9.2)
     * Precedence: 1 < ((2 * 3) + 4)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans RHS complexa: 1 < 2 * 3 + 4 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "1 < 2 * 3 + 4");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Root should be < */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINOR);
            /* Left child should be 1 */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_INTEGER);
            }
            /* Right child should be + (2*3 + 4) */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_PLUS);
            }
        }
    }

    /* ========================================================
     * PROBARE: Comparison in if statement (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio in si: if (x < 10) y; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "if (x < 10) y;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SI);
            /* Condition should be < */
            si (res.radix->datum.conditionale.conditio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.conditionale.conditio->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.conditionale.conditio->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_MINOR);
            }
        }
    }

    /* ========================================================
     * PROBARE: Equality in while statement (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans aequalitas in dum: while (a == b) x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "while (a == b) x;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DUM);
            /* Condition should be == */
            si (res.radix->datum.iteratio.conditio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.iteratio.conditio->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.iteratio.conditio->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_AEQUALIS);
            }
        }
    }

    /* ========================================================
     * PROBARE: Comparison in for loop condition (E9.2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio in per: for (; i < 10; ) x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "for (; i < 10; ) x;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PER);
            /* Condition should be < */
            si (res.radix->datum.circuitus.conditio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.circuitus.conditio->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.circuitus.conditio->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_MINOR);
            }
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
     * PROBARE: Function declarator: MyType * fn()
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans function declarator: MyType * fn() ---\n");

        /* Reset statistics */
        glr->num_furcae = ZEPHYRUM;
        glr->num_mergae = ZEPHYRUM;

        tokens = _lexare_ad_tokens(piscina, intern, "MyType * fn()");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        /* Should be DECLARATIO with DECLARATOR_FUNCTI */
        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            /* Check declarator is function declarator */
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.declarator->genus,
                                   (i32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
                /* Check habet_void is false for () */
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.declarator->datum.declarator_functi.habet_void,
                                   FALSUM);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Function declarator with void: MyType * fn(void)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans function declarator: MyType * fn(void) ---\n");

        /* Reset statistics */
        glr->num_furcae = ZEPHYRUM;
        glr->num_mergae = ZEPHYRUM;

        tokens = _lexare_ad_tokens(piscina, intern, "MyType * fn(void)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        /* Should be DECLARATIO with DECLARATOR_FUNCTI */
        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            /* Check declarator is function declarator */
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.declarator->genus,
                                   (i32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
                /* Check habet_void is true for (void) */
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.declarator->datum.declarator_functi.habet_void,
                                   VERUM);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* ========================================================
     * PROBARE: Single parameter: MyType * fn(int x)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans single param: MyType * fn(int x) ---\n");

        /* Reset statistics */
        glr->num_furcae = ZEPHYRUM;
        glr->num_mergae = ZEPHYRUM;

        tokens = _lexare_ad_tokens(piscina, intern, "MyType * fn(int x)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        /* Should be DECLARATIO with DECLARATOR_FUNCTI with 1 param */
        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
                CREDO_AEQUALIS_I32((i32)decl->datum.declarator_functi.habet_void, FALSUM);
                CREDO_NON_NIHIL(decl->datum.declarator_functi.parametri);
                si (decl->datum.declarator_functi.parametri != NIHIL)
                {
                    CREDO_AEQUALIS_I32(xar_numerus(decl->datum.declarator_functi.parametri), I);
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* ========================================================
     * PROBARE: Multiple params: MyType * fn(int x, int y)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans multi param: MyType * fn(int x, int y) ---\n");

        /* Reset statistics */
        glr->num_furcae = ZEPHYRUM;
        glr->num_mergae = ZEPHYRUM;

        tokens = _lexare_ad_tokens(piscina, intern, "MyType * fn(int x, int y)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        /* Should be DECLARATIO with DECLARATOR_FUNCTI with 2 params */
        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
                CREDO_NON_NIHIL(decl->datum.declarator_functi.parametri);
                si (decl->datum.declarator_functi.parametri != NIHIL)
                {
                    CREDO_AEQUALIS_I32(xar_numerus(decl->datum.declarator_functi.parametri), II);
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* ========================================================
     * PROBARE: Pointer param: MyType * fn(int *ptr)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans pointer param: MyType * fn(int *ptr) ---\n");

        /* Reset statistics */
        glr->num_furcae = ZEPHYRUM;
        glr->num_mergae = ZEPHYRUM;

        tokens = _lexare_ad_tokens(piscina, intern, "MyType * fn(int *ptr)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        /* Should be DECLARATIO with DECLARATOR_FUNCTI with 1 pointer param */
        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
                CREDO_NON_NIHIL(decl->datum.declarator_functi.parametri);
                si (decl->datum.declarator_functi.parametri != NIHIL)
                {
                    Arbor2Nodus* param;
                    CREDO_AEQUALIS_I32(xar_numerus(decl->datum.declarator_functi.parametri), I);
                    param = *(Arbor2Nodus**)xar_obtinere(decl->datum.declarator_functi.parametri, ZEPHYRUM);
                    CREDO_AEQUALIS_I32((i32)param->genus, (i32)ARBOR2_NODUS_PARAMETER_DECL);
                    /* Check param declarator has pointer */
                    si (param->datum.parameter_decl.declarator != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)param->datum.parameter_decl.declarator->datum.declarator.num_stellae, I);
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
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
     * PROBARE: Simple if statement: if (x) y;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans simple if: if (x) y; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "if (x) y;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SI);
            /* Should have condition and consequens, no alternans */
            CREDO_NON_NIHIL(res.radix->datum.conditionale.conditio);
            CREDO_NON_NIHIL(res.radix->datum.conditionale.consequens);
            CREDO_NIHIL(res.radix->datum.conditionale.alternans);
        }
    }


    /* ========================================================
     * PROBARE: If-else statement: if (x) y; else z;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans if-else: if (x) y; else z; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "if (x) y; else z;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SI);
            /* Should have condition, consequens, AND alternans */
            CREDO_NON_NIHIL(res.radix->datum.conditionale.conditio);
            CREDO_NON_NIHIL(res.radix->datum.conditionale.consequens);
            CREDO_NON_NIHIL(res.radix->datum.conditionale.alternans);
        }
    }


    /* ========================================================
     * PROBARE: If with compound body: if (x) { y; }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans if with compound: if (x) { y; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "if (x) { y; }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SI);
            /* Consequens should be CORPUS */
            si (res.radix->datum.conditionale.consequens != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.conditionale.consequens->genus,
                                   (i32)ARBOR2_NODUS_CORPUS);
            }
        }
    }


    /* ========================================================
     * PROBARE: Dangling else: if (a) if (b) c; else d;
     * else should bind to inner if (b), not outer if (a)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* inner_if;

        imprimere("\n--- Probans dangling else: if (a) if (b) c; else d; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "if (a) if (b) c; else d;");
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
            /* Outer if should NOT have an else */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SI);
            CREDO_NIHIL(res.radix->datum.conditionale.alternans);

            /* Inner if (consequens of outer) SHOULD have an else */
            inner_if = res.radix->datum.conditionale.consequens;
            CREDO_NON_NIHIL(inner_if);
            si (inner_if != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)inner_if->genus, (i32)ARBOR2_NODUS_SI);
                CREDO_NON_NIHIL(inner_if->datum.conditionale.alternans);
            }
        }
    }


    /* ========================================================
     * PROBARE: Simple while statement: while (x) y;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans simple while: while (x) y; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "while (x) y;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DUM);
            /* Should have condition and body */
            CREDO_NON_NIHIL(res.radix->datum.iteratio.conditio);
            CREDO_NON_NIHIL(res.radix->datum.iteratio.corpus);
        }
    }


    /* ========================================================
     * PROBARE: While with compound body: while (x) { a; b; }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans while with compound: while (x) { a; b; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "while (x) { a; b; }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DUM);
            /* Body should be CORPUS */
            si (res.radix->datum.iteratio.corpus != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.iteratio.corpus->genus,
                                   (i32)ARBOR2_NODUS_CORPUS);
            }
        }
    }


    /* ========================================================
     * PROBARE: Simple do-while statement: do x; while (y);
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans simple do-while: do x; while (y); ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "do x; while (y);");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_FAC);
            /* Should have body and condition */
            CREDO_NON_NIHIL(res.radix->datum.iteratio.corpus);
            CREDO_NON_NIHIL(res.radix->datum.iteratio.conditio);
        }
    }


    /* ========================================================
     * PROBARE: Do-while with compound body: do { a; b; } while (c);
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans do-while with compound: do { a; b; } while (c); ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "do { a; b; } while (c);");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_FAC);
            /* Body should be CORPUS */
            si (res.radix->datum.iteratio.corpus != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.iteratio.corpus->genus,
                                   (i32)ARBOR2_NODUS_CORPUS);
            }
        }
    }


    /* ========================================================
     * PROBARE: Nested while: while (a) while (b) c;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* inner_while;

        imprimere("\n--- Probans nested while: while (a) while (b) c; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "while (a) while (b) c;");
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
            /* Outer while */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DUM);

            /* Inner while (body of outer) */
            inner_while = res.radix->datum.iteratio.corpus;
            CREDO_NON_NIHIL(inner_while);
            si (inner_while != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)inner_while->genus, (i32)ARBOR2_NODUS_DUM);
            }
        }
    }


    /* ========================================================
     * PROBARE: Simple for with all expressions: for (i; c; n) x;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans simple for: for (i; c; n) x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "for (i; c; n) x;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PER);
            /* Should have all four parts */
            CREDO_NON_NIHIL(res.radix->datum.circuitus.initium);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.conditio);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.incrementum);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.corpus);
        }
    }


    /* ========================================================
     * PROBARE: For with empty init: for (; c; n) x;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans for empty init: for (; c; n) x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "for (; c; n) x;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PER);
            /* Init should be NULL, others present */
            CREDO_NIHIL(res.radix->datum.circuitus.initium);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.conditio);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.incrementum);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.corpus);
        }
    }


    /* ========================================================
     * PROBARE: For with empty condition: for (i; ; n) x;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans for empty condition: for (i; ; n) x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "for (i; ; n) x;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PER);
            /* Condition should be NULL, others present */
            CREDO_NON_NIHIL(res.radix->datum.circuitus.initium);
            CREDO_NIHIL(res.radix->datum.circuitus.conditio);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.incrementum);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.corpus);
        }
    }


    /* ========================================================
     * PROBARE: For with empty increment: for (i; c; ) x;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans for empty increment: for (i; c; ) x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "for (i; c; ) x;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PER);
            /* Increment should be NULL, others present */
            CREDO_NON_NIHIL(res.radix->datum.circuitus.initium);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.conditio);
            CREDO_NIHIL(res.radix->datum.circuitus.incrementum);
            CREDO_NON_NIHIL(res.radix->datum.circuitus.corpus);
        }
    }


    /* ========================================================
     * PROBARE: For with all empty (infinite loop): for (;;) x;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans for all empty: for (;;) x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "for (;;) x;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PER);
            /* All optional parts should be NULL */
            CREDO_NIHIL(res.radix->datum.circuitus.initium);
            CREDO_NIHIL(res.radix->datum.circuitus.conditio);
            CREDO_NIHIL(res.radix->datum.circuitus.incrementum);
            /* Body should still be present */
            CREDO_NON_NIHIL(res.radix->datum.circuitus.corpus);
        }
    }


    /* ========================================================
     * PROBARE: For with compound body: for (i; c; n) { a; b; }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans for with compound: for (i; c; n) { a; b; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "for (i; c; n) { a; b; }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PER);
            /* Body should be CORPUS */
            si (res.radix->datum.circuitus.corpus != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.circuitus.corpus->genus,
                                   (i32)ARBOR2_NODUS_CORPUS);
            }
        }
    }


    /* ========================================================
     * PROBARE: Nested for: for (;;) for (;;) x;
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* inner_for;

        imprimere("\n--- Probans nested for: for (;;) for (;;) x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "for (;;) for (;;) x;");
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
            /* Outer for */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PER);

            /* Inner for (body of outer) */
            inner_for = res.radix->datum.circuitus.corpus;
            CREDO_NON_NIHIL(inner_for);
            si (inner_for != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)inner_for->genus, (i32)ARBOR2_NODUS_PER);
            }
        }
    }


    /* ========================================================
     * PROBARE: break statement
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans break statement ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "break;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_FRANGE);
        }
    }


    /* ========================================================
     * PROBARE: continue statement
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans continue statement ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "continue;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PERGE);
        }
    }


    /* ========================================================
     * PROBARE: return statement without value
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans return without value ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "return;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_REDDE);
            CREDO_NIHIL(res.radix->datum.reditio.valor);  /* No return value */
        }
    }


    /* ========================================================
     * PROBARE: return statement with value
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans return with value ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "return x;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_REDDE);
            CREDO_NON_NIHIL(res.radix->datum.reditio.valor);  /* Has return value */
        }
    }


    /* ========================================================
     * PROBARE: goto statement
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans goto statement ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "goto label;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SALTA);
        }
    }


    /* ========================================================
     * PROBARE: break inside while loop
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans break inside while ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "while (x) break;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DUM);
            CREDO_NON_NIHIL(res.radix->datum.iteratio.corpus);
            si (res.radix->datum.iteratio.corpus != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.iteratio.corpus->genus, (i32)ARBOR2_NODUS_FRANGE);
            }
        }
    }


    /* ========================================================
     * PROBARE: return with expression in compound
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Xar* sententiae;

        imprimere("\n--- Probans return in compound ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "{ return x + 1; }");
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
            sententiae = res.radix->datum.corpus.sententiae;
            CREDO_NON_NIHIL(sententiae);
            si (sententiae != NIHIL && xar_numerus(sententiae) > ZEPHYRUM)
            {
                Arbor2Nodus* ret = *(Arbor2Nodus**)xar_obtinere(sententiae, ZEPHYRUM);
                CREDO_AEQUALIS_I32((i32)ret->genus, (i32)ARBOR2_NODUS_REDDE);
                CREDO_NON_NIHIL(ret->datum.reditio.valor);  /* Has expression x + 1 */
            }
        }
    }

    /* ========================================================
     * PROBARE: Labeled statements (Phase C4b)
     * ======================================================== */

    /* Simple label */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans simple label: foo: x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "foo: x;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TITULATUM);
            CREDO_NON_NIHIL(res.radix->datum.titulatum.sententia);
        }
    }

    /* Label with empty statement */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans label with empty: foo: ; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "foo: ;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TITULATUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.titulatum.sententia->genus, (i32)ARBOR2_NODUS_SENTENTIA_VACUA);
        }
    }

    /* Nested labels */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested labels: foo: bar: x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "foo: bar: x;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TITULATUM);
            CREDO_NON_NIHIL(res.radix->datum.titulatum.sententia);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.titulatum.sententia->genus, (i32)ARBOR2_NODUS_TITULATUM);
        }
    }

    /* Label with compound */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans label with compound: foo: { x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "foo: { x; }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TITULATUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.titulatum.sententia->genus, (i32)ARBOR2_NODUS_CORPUS);
        }
    }

    /* Label with if */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans label with if: foo: if (x) y; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "foo: if (x) y;");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TITULATUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.titulatum.sententia->genus, (i32)ARBOR2_NODUS_SI);
        }
    }

    /* Label as goto target in compound */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Xar* sententiae;

        imprimere("\n--- Probans label as goto target: { goto foo; foo: x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "{ goto foo; foo: x; }");
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
            sententiae = res.radix->datum.corpus.sententiae;
            CREDO_NON_NIHIL(sententiae);
            si (sententiae != NIHIL && xar_numerus(sententiae) >= II)
            {
                Arbor2Nodus* salta_node = *(Arbor2Nodus**)xar_obtinere(sententiae, ZEPHYRUM);
                Arbor2Nodus* label_node = *(Arbor2Nodus**)xar_obtinere(sententiae, I);
                CREDO_AEQUALIS_I32((i32)salta_node->genus, (i32)ARBOR2_NODUS_SALTA);
                CREDO_AEQUALIS_I32((i32)label_node->genus, (i32)ARBOR2_NODUS_TITULATUM);
            }
        }
    }


    /* ========================================================
     * PROBARE: Switch/case/default statements (Phase C4c)
     * ======================================================== */

    /* Simple switch with one case */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans switch with case: switch (x) { case 1: y; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "switch (x) { case 1: y; }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_COMMUTATIO);
        }
    }

    /* Switch with default */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans switch with default: switch (x) { default: y; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "switch (x) { default: y; }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_COMMUTATIO);
        }
    }

    /* Switch with case and break */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans switch case break: switch (x) { case 1: break; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "switch (x) { case 1: break; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Multiple cases (fall-through) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans fall-through: switch (x) { case 1: case 2: y; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "switch (x) { case 1: case 2: y; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Case with expression */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans case expr: switch (x) { case 1 + 2: y; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "switch (x) { case 1 + 2: y; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }


    /* ========================================================
     * PROBARE: Function definition: MyType fn(void) { }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans function definition: MyType fn(void) { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "MyType fn(void) { }");
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
            Arbor2Nodus* decl;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            /* Check specifier */
            CREDO_NON_NIHIL(res.radix->datum.definitio_functi.specifier);
            /* Check declarator is function declarator */
            decl = res.radix->datum.definitio_functi.declarator;
            CREDO_NON_NIHIL(decl);
            si (decl != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
                CREDO_AEQUALIS_I32((i32)decl->datum.declarator_functi.habet_void, VERUM);
            }
            /* Check corpus exists */
            CREDO_NON_NIHIL(res.radix->datum.definitio_functi.corpus);
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Function definition with body: MyType fn(void) { return x; }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans function definition: MyType fn(void) { return x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "MyType fn(void) { return x; }");
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
            Arbor2Nodus* corpus;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            /* Check corpus has statement */
            corpus = res.radix->datum.definitio_functi.corpus;
            CREDO_NON_NIHIL(corpus);
            si (corpus != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)corpus->genus, (i32)ARBOR2_NODUS_CORPUS);
                CREDO_AEQUALIS_I32(xar_numerus(corpus->datum.corpus.sententiae), I);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Function with params: MyType foo(int x, int y) { return x; }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans function: MyType foo(int x, int y) { return x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "MyType foo(int x, int y) { return x; }");
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
            Arbor2Nodus* decl;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            decl = res.radix->datum.definitio_functi.declarator;
            CREDO_NON_NIHIL(decl);
            si (decl != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
                CREDO_NON_NIHIL(decl->datum.declarator_functi.parametri);
                si (decl->datum.declarator_functi.parametri != NIHIL)
                {
                    CREDO_AEQUALIS_I32(xar_numerus(decl->datum.declarator_functi.parametri), II);
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Pointer return type: MyType * bar(void) { }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans pointer return: MyType * bar(void) { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "MyType * bar(void) { }");
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
            Arbor2Nodus* decl;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            decl = res.radix->datum.definitio_functi.declarator;
            CREDO_NON_NIHIL(decl);
            si (decl != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR_FUNCTI);
                /* Check there's a pointer in the declarator chain */
                CREDO_AEQUALIS_I32((i32)decl->datum.declarator_functi.num_stellae, I);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Struct specifier tests
     * ======================================================== */

    /* Test forward reference: struct foo x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct forward ref: struct foo x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo x");
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
            Arbor2Nodus* spec;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            /* Check type specifier is a struct */
            spec = res.radix->datum.declaratio.specifier;
            CREDO_NON_NIHIL(spec);
            si (spec != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)spec->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
                CREDO_NON_NIHIL(spec->datum.struct_specifier.tag);  /* Has tag */
                CREDO_NIHIL(spec->datum.struct_specifier.membra);   /* No body */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test named struct with single member: struct foo { int x; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct named: struct foo { int x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { int x; }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.struct_specifier.tag);     /* Has tag */
            CREDO_NON_NIHIL(res.radix->datum.struct_specifier.membra);  /* Has body */
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);  /* 1 member */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test named struct with multiple members */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct multi-member: struct foo { int x; int y; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { int x; int y; }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), II);  /* 2 members */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test struct variable declaration: struct foo { int x; } var */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct var decl: struct foo { int x; } var ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { int x; } var");
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
            Arbor2Nodus* spec;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            /* Check specifier is struct */
            spec = res.radix->datum.declaratio.specifier;
            si (spec != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)spec->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Bit fields (E4)
     * ======================================================== */

    /* Test single named bit field: struct foo { int x : 3; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans bit field: struct foo { int x : 3; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { int x : 3; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.struct_specifier.membra);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                Arbor2Nodus* member;
                Arbor2Nodus* decl;
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                CREDO_NON_NIHIL(member);
                si (member != NIHIL)
                {
                    decl = member->datum.declaratio.declarator;
                    CREDO_NON_NIHIL(decl);
                    si (decl != NIHIL)
                    {
                        CREDO_NON_NIHIL(decl->datum.declarator.latitudo_biti);  /* Has bit width */
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test multiple bit fields: struct foo { int x : 3; int y : 5; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans bit fields: struct foo { int x : 3; int y : 5; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { int x : 3; int y : 5; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.struct_specifier.membra);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), II);  /* 2 members */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test anonymous bit field: struct foo { int : 4; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans anonymous bit field: struct foo { int : 4; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { int : 4; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                Arbor2Nodus* member;
                Arbor2Nodus* decl;
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                CREDO_NON_NIHIL(member);
                si (member != NIHIL)
                {
                    decl = member->datum.declaratio.declarator;
                    CREDO_NON_NIHIL(decl);
                    si (decl != NIHIL)
                    {
                        /* Anonymous: titulus.mensura == 0 */
                        CREDO_AEQUALIS_I32((i32)decl->datum.declarator.titulus.mensura, ZEPHYRUM);
                        CREDO_NON_NIHIL(decl->datum.declarator.latitudo_biti);  /* Has bit width */
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test mixed named and anonymous: struct foo { int x : 3; int : 0; int y : 5; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans mixed bit fields: struct foo { int x : 3; int : 0; int y : 5; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { int x : 3; int : 0; int y : 5; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), III);  /* 3 members */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test expression bit width: struct foo { int x : 1 + 2; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans expression bit field: struct foo { int x : 1 + 2; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { int x : 1 + 2; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                Arbor2Nodus* member;
                Arbor2Nodus* decl;
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                si (member != NIHIL)
                {
                    decl = member->datum.declaratio.declarator;
                    si (decl != NIHIL)
                    {
                        /* latitudo_biti should be a BINARIUM node (1 + 2) */
                        CREDO_NON_NIHIL(decl->datum.declarator.latitudo_biti);
                        si (decl->datum.declarator.latitudo_biti != NIHIL)
                        {
                            CREDO_AEQUALIS_I32((i32)decl->datum.declarator.latitudo_biti->genus,
                                               (i32)ARBOR2_NODUS_BINARIUM);
                        }
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test bit field in union: union foo { int x : 3; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans bit field in union: union foo { int x : 3; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "union foo { int x : 3; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            CREDO_VERUM(res.radix->datum.struct_specifier.est_unio);  /* Is union */
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Union specifier
     * ======================================================== */

    /* Test union forward reference: union foo */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans union forward ref: union foo ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "union foo");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            Arbor2Nodus* spec = res.radix;
            imprimere("  genus: %d\n", (i32)spec->genus);
            _imprimere_arborem(res.radix, ZEPHYRUM);

            si (spec != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)spec->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
                CREDO_NON_NIHIL(spec->datum.struct_specifier.tag);  /* Has tag */
                CREDO_NIHIL(spec->datum.struct_specifier.membra);   /* No body */
                CREDO_VERUM(spec->datum.struct_specifier.est_unio); /* Is union */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test named union with single member: union foo { int x; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans union named: union foo { int x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "union foo { int x; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.struct_specifier.tag);     /* Has tag */
            CREDO_NON_NIHIL(res.radix->datum.struct_specifier.membra);  /* Has body */
            CREDO_VERUM(res.radix->datum.struct_specifier.est_unio);    /* Is union */
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);  /* 1 member */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test named union with multiple members: union foo { int x; int y; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans union multi-member: union foo { int x; int y; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "union foo { int x; int y; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            CREDO_VERUM(res.radix->datum.struct_specifier.est_unio);  /* Is union */
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), II);  /* 2 members */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test anonymous union: union { int x; } anon */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans union anon: union { int x; } anon ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "union { int x; } anon");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            Arbor2Nodus* spec;
            _imprimere_arborem(res.radix, ZEPHYRUM);
            /* This should be a DECLARATIO with a union type */
            spec = res.radix->datum.declaratio.specifier;
            si (spec != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)spec->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
                CREDO_VERUM(spec->datum.struct_specifier.est_unio);  /* Is union */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Enum specifier (E3)
     * ======================================================== */

    /* Test enum forward reference: enum foo */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum forward ref: enum foo ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum foo");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.enum_specifier.tag);          /* Has tag */
            CREDO_NIHIL(res.radix->datum.enum_specifier.enumeratores);     /* No body */
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test anonymous enum: enum { A, B, C } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum anon: enum { A, B, C } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum { A, B, C }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
            CREDO_NIHIL(res.radix->datum.enum_specifier.tag);              /* No tag (anonymous) */
            CREDO_NON_NIHIL(res.radix->datum.enum_specifier.enumeratores); /* Has body */
            si (res.radix->datum.enum_specifier.enumeratores != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.enum_specifier.enumeratores), III);  /* 3 enumerators */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test named enum: enum foo { A, B, C } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum named: enum foo { A, B, C } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum foo { A, B, C }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.enum_specifier.tag);          /* Has tag */
            CREDO_NON_NIHIL(res.radix->datum.enum_specifier.enumeratores); /* Has body */
            si (res.radix->datum.enum_specifier.enumeratores != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.enum_specifier.enumeratores), III);  /* 3 enumerators */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test enum with values: enum foo { A = 1, B, C = 10 } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum with values: enum foo { A = 1, B, C = 10 } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum foo { A = 1, B, C = 10 }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.enum_specifier.tag);          /* Has tag */
            CREDO_NON_NIHIL(res.radix->datum.enum_specifier.enumeratores); /* Has body */
            si (res.radix->datum.enum_specifier.enumeratores != NIHIL)
            {
                Xar* enums = res.radix->datum.enum_specifier.enumeratores;
                Arbor2Nodus** e0;
                Arbor2Nodus** e1;
                Arbor2Nodus** e2;

                CREDO_AEQUALIS_I32(xar_numerus(enums), III);  /* 3 enumerators */

                /* Check first enumerator has value */
                e0 = xar_obtinere(enums, ZEPHYRUM);
                si (e0 != NIHIL && *e0 != NIHIL)
                {
                    CREDO_NON_NIHIL((*e0)->datum.enumerator.valor);  /* A = 1 */
                }

                /* Check second enumerator has no value */
                e1 = xar_obtinere(enums, I);
                si (e1 != NIHIL && *e1 != NIHIL)
                {
                    CREDO_NIHIL((*e1)->datum.enumerator.valor);  /* B (no value) */
                }

                /* Check third enumerator has value */
                e2 = xar_obtinere(enums, II);
                si (e2 != NIHIL && *e2 != NIHIL)
                {
                    CREDO_NON_NIHIL((*e2)->datum.enumerator.valor);  /* C = 10 */
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test enum as type in declaration: enum foo x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum decl: enum foo x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum foo x");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            Arbor2Nodus* spec;
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            spec = res.radix->datum.declaratio.specifier;
            si (spec != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)spec->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
                CREDO_NON_NIHIL(spec->datum.enum_specifier.tag);       /* Has tag */
                CREDO_NIHIL(spec->datum.enum_specifier.enumeratores);  /* No body */
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test enum with expression value: enum { A = 1 + 2 } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum expr value: enum { A = 1 + 2 } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum { A = 1 + 2 }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
            si (res.radix->datum.enum_specifier.enumeratores != NIHIL)
            {
                Xar* enums = res.radix->datum.enum_specifier.enumeratores;
                Arbor2Nodus** e0;

                CREDO_AEQUALIS_I32(xar_numerus(enums), I);  /* 1 enumerator */

                e0 = xar_obtinere(enums, ZEPHYRUM);
                si (e0 != NIHIL && *e0 != NIHIL)
                {
                    /* Check the value is a binary expression */
                    CREDO_NON_NIHIL((*e0)->datum.enumerator.valor);
                    si ((*e0)->datum.enumerator.valor != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)(*e0)->datum.enumerator.valor->genus,
                            (i32)ARBOR2_NODUS_BINARIUM);
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Nested type specifiers (Phase E5)
     * ======================================================== */

    /* Test nested struct: struct outer { struct inner { int a; } x; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested struct: struct outer { struct inner { int a; } x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct outer { struct inner { int a; } x; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                Arbor2Nodus* member;
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                CREDO_NON_NIHIL(member);
                si (member != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)member->genus, (i32)ARBOR2_NODUS_DECLARATIO);
                    /* Specifier should be nested struct_specifier */
                    CREDO_NON_NIHIL(member->datum.declaratio.specifier);
                    si (member->datum.declaratio.specifier != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)member->datum.declaratio.specifier->genus,
                                          (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test nested enum: struct outer { enum { A, B } status; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested enum: struct outer { enum { A, B } status; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct outer { enum { A, B } status; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                Arbor2Nodus* member;
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                CREDO_NON_NIHIL(member);
                si (member != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)member->genus, (i32)ARBOR2_NODUS_DECLARATIO);
                    /* Specifier should be enum_specifier */
                    CREDO_NON_NIHIL(member->datum.declaratio.specifier);
                    si (member->datum.declaratio.specifier != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)member->datum.declaratio.specifier->genus,
                                          (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test nested union: struct outer { union { int i; int j; } data; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested union: struct outer { union { int i; int j; } data; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct outer { union { int i; int j; } data; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                Arbor2Nodus* member;
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                CREDO_NON_NIHIL(member);
                si (member != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)member->genus, (i32)ARBOR2_NODUS_DECLARATIO);
                    /* Specifier should be union (struct_specifier with est_unio flag) */
                    CREDO_NON_NIHIL(member->datum.declaratio.specifier);
                    si (member->datum.declaratio.specifier != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)member->datum.declaratio.specifier->genus,
                                          (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
                        CREDO_VERUM(member->datum.declaratio.specifier->datum.struct_specifier.est_unio);
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test nested pointer: struct outer { struct inner { int a; } *ptr; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested pointer: struct outer { struct inner { int a; } *ptr; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct outer { struct inner { int a; } *ptr; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                Arbor2Nodus* member;
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                CREDO_NON_NIHIL(member);
                si (member != NIHIL)
                {
                    /* Declarator should have num_stellae = 1 */
                    CREDO_NON_NIHIL(member->datum.declaratio.declarator);
                    si (member->datum.declaratio.declarator != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)member->datum.declaratio.declarator->datum.declarator.num_stellae, I);
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test multiple members with nested: struct outer { int x; struct { int a; } nested; int y; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans multiple with nested: struct outer { int x; struct { int a; } nested; int y; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct outer { int x; struct { int a; } nested; int y; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                Arbor2Nodus* member1;
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), III);
                /* Second member (index 1) should be the nested struct */
                member1 = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, I);
                CREDO_NON_NIHIL(member1);
                si (member1 != NIHIL && member1->datum.declaratio.specifier != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)member1->datum.declaratio.specifier->genus,
                                      (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* ========================================================
     * PROBARE: Typedef declarations (P74-P79)
     * ======================================================== */

    /* Test simple typedef: typedef int MyInt; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef simplex: typedef int MyInt; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef int MyInt;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_VERUM(res.radix->datum.declaratio.est_typedef);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.declarator);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.declarator->datum.declarator.num_stellae, ZEPHYRUM);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test pointer typedef: typedef int* IntPtr; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef pointer: typedef int* IntPtr; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef int* IntPtr;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_VERUM(res.radix->datum.declaratio.est_typedef);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.declarator);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.declarator->datum.declarator.num_stellae, I);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test typedef struct: typedef struct { int x; } Point; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef struct: typedef struct { int x; } Point; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef struct { int x; } Point;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_VERUM(res.radix->datum.declaratio.est_typedef);
            /* Specifier should be a STRUCT_SPECIFIER */
            CREDO_NON_NIHIL(res.radix->datum.declaratio.specifier);
            si (res.radix->datum.declaratio.specifier != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.specifier->genus,
                                  (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test typedef struct pointer: typedef struct { int x; } *PointPtr; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef struct pointer: typedef struct { int x; } *PointPtr; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef struct { int x; } *PointPtr;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_VERUM(res.radix->datum.declaratio.est_typedef);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.declarator);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.declarator->datum.declarator.num_stellae, I);
            }
            CREDO_NON_NIHIL(res.radix->datum.declaratio.specifier);
            si (res.radix->datum.declaratio.specifier != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.specifier->genus,
                                  (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test typedef enum: typedef enum { A, B } Status; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef enum: typedef enum { A, B } Status; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef enum { A, B } Status;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_VERUM(res.radix->datum.declaratio.est_typedef);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.specifier);
            si (res.radix->datum.declaratio.specifier != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.specifier->genus,
                                  (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test typedef enum pointer: typedef enum { A, B } *StatusPtr; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef enum pointer: typedef enum { A, B } *StatusPtr; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef enum { A, B } *StatusPtr;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_VERUM(res.radix->datum.declaratio.est_typedef);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.declarator);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.declarator->datum.declarator.num_stellae, I);
            }
            CREDO_NON_NIHIL(res.radix->datum.declaratio.specifier);
            si (res.radix->datum.declaratio.specifier != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.specifier->genus,
                                  (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test typedef named struct: typedef struct MyStruct { int x; } MyStruct; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef struct nominatum: typedef struct MyStruct { int x; } MyStruct; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef struct MyStruct { int x; } MyStruct;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_VERUM(res.radix->datum.declaratio.est_typedef);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.specifier);
            si (res.radix->datum.declaratio.specifier != NIHIL)
            {
                /* Specifier should be STRUCT_SPECIFIER with a tag */
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.specifier->genus,
                                  (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
                CREDO_NON_NIHIL(res.radix->datum.declaratio.specifier->datum.struct_specifier.tag);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test typedef forward ref: typedef struct Foo Foo; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef struct forward: typedef struct Foo Foo; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef struct Foo Foo;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_VERUM(res.radix->datum.declaratio.est_typedef);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.specifier);
            si (res.radix->datum.declaratio.specifier != NIHIL)
            {
                /* Specifier should be STRUCT_SPECIFIER (forward ref) */
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.specifier->genus,
                                  (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Array declarators (Phase E7)
     * ========================================================
     *
     * Phase E7 state machine complete - array tests enabled.
     */

    /* Test fixed size array: int arr[10] */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans array fixed: int arr[10] ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int arr[10]");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                si (decl->datum.declarator.dimensiones != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.dimensiones), 1);
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test unsized array: int arr[] */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans array unsized: int arr[] ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int arr[]");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                si (decl->datum.declarator.dimensiones != NIHIL)
                {
                    Arbor2Nodus** dim;
                    CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.dimensiones), 1);
                    dim = xar_obtinere(decl->datum.declarator.dimensiones, ZEPHYRUM);
                    CREDO_NIHIL(*dim);  /* Unsized array has NIHIL dimension */
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test multi-dimensional array: int matrix[10][20] */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans array multi-dim: int matrix[10][20] ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int matrix[10][20]");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                si (decl->datum.declarator.dimensiones != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.dimensiones), 2);
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test array of pointers: int *arr[10] */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans array of pointers: int *arr[10] ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int *arr[10]");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_AEQUALIS_I32((i32)decl->datum.declarator.num_stellae, 1);
                CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* E8: Struct member arrays now supported via declarator refactor */
    /* Test struct member array: struct { int arr[5]; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct member array: struct { int arr[5]; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct { int arr[5]; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test struct member pointer array: struct { int *ptrs[5]; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct member ptr array: struct { int *ptrs[5]; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct { int *ptrs[5]; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test struct multi-dimensional array: struct { int matrix[3][4]; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct multi-dim array: struct { int matrix[3][4]; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct { int matrix[3][4]; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test struct multiple array members: struct { int a[5]; int b[10]; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct multi arrays: struct { int a[5]; int b[10]; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct { int a[5]; int b[10]; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* E8: Typedef arrays now supported via declarator refactor */
    /* Test typedef array: typedef int IntArray[10] */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef array: typedef int IntArray[10]; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef int IntArray[10];");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_VERUM(res.radix->datum.declaratio.est_typedef);
            si (res.radix->datum.declaratio.declarator != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
                CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test typedef pointer array: typedef int* IntPtrArray[10]; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef pointer array: typedef int* IntPtrArray[10]; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef int* IntPtrArray[10];");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test typedef multi-dim array: typedef int Matrix[3][4]; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef multi-dim: typedef int Matrix[3][4]; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef int Matrix[3][4];");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test expression size: int arr[1 + 2] */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans array expr size: int arr[1 + 2] ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int arr[1 + 2]");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL && res.radix->datum.declaratio.declarator != NIHIL)
        {
            Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
            CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
            si (decl->datum.declarator.dimensiones != NIHIL)
            {
                Arbor2Nodus** dim = xar_obtinere(decl->datum.declarator.dimensiones, ZEPHYRUM);
                CREDO_NON_NIHIL(*dim);  /* Should have expression node */
                si (*dim != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)(*dim)->genus, (i32)ARBOR2_NODUS_BINARIUM);
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* ========================================================
     * PROBARE: Nested type member arrays (Phase E8 Part 3)
     * ======================================================== */

    /* Test nested struct array: struct { struct Inner items[10]; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested struct array: struct { struct Inner items[10]; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct { struct Inner items[10]; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            Arbor2Nodus* member;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.struct_specifier.membra);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                CREDO_NON_NIHIL(member);
                si (member != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)member->genus, (i32)ARBOR2_NODUS_DECLARATIO);
                    /* Specifier should be nested struct_specifier */
                    CREDO_NON_NIHIL(member->datum.declaratio.specifier);
                    si (member->datum.declaratio.specifier != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)member->datum.declaratio.specifier->genus,
                                          (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
                    }
                    /* Declarator should have array dimension */
                    CREDO_NON_NIHIL(member->datum.declaratio.declarator);
                    si (member->datum.declaratio.declarator != NIHIL)
                    {
                        Arbor2Nodus* decl = member->datum.declaratio.declarator;
                        CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                        si (decl->datum.declarator.dimensiones != NIHIL)
                        {
                            CREDO_AEQUALIS_I32(xar_numerus(decl->datum.declarator.dimensiones), I);
                        }
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test nested enum array: struct { enum { A, B } status[5]; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested enum array: struct { enum { A, B } status[5]; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct { enum { A, B } status[5]; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            Arbor2Nodus* member;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.struct_specifier.membra);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                CREDO_NON_NIHIL(member);
                si (member != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)member->genus, (i32)ARBOR2_NODUS_DECLARATIO);
                    /* Specifier should be enum_specifier */
                    CREDO_NON_NIHIL(member->datum.declaratio.specifier);
                    si (member->datum.declaratio.specifier != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)member->datum.declaratio.specifier->genus,
                                          (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
                    }
                    /* Declarator should have array dimension */
                    CREDO_NON_NIHIL(member->datum.declaratio.declarator);
                    si (member->datum.declaratio.declarator != NIHIL)
                    {
                        Arbor2Nodus* decl = member->datum.declaratio.declarator;
                        CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                    }
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }

    /* Test nested struct pointer array: struct { struct Inner *ptrs[3]; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested struct ptr array: struct { struct Inner *ptrs[3]; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct { struct Inner *ptrs[3]; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            Arbor2Nodus* member;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            CREDO_NON_NIHIL(res.radix->datum.struct_specifier.membra);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                member = *(Arbor2Nodus**)xar_obtinere(res.radix->datum.struct_specifier.membra, ZEPHYRUM);
                CREDO_NON_NIHIL(member);
                si (member != NIHIL && member->datum.declaratio.declarator != NIHIL)
                {
                    Arbor2Nodus* decl = member->datum.declaratio.declarator;
                    /* Should be pointer */
                    CREDO_AEQUALIS_I32((i32)decl->datum.declarator.num_stellae, (i32)I);
                    /* Should have array dimension */
                    CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
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
