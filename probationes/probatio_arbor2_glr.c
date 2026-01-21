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
                Arbor2Nodus* param_lista = nodus->datum.declarator_functi.parametri;
                si (param_lista != NIHIL && param_lista->genus == ARBOR2_NODUS_LISTA_SEPARATA)
                {
                    num_params = xar_numerus(param_lista->datum.lista_separata.elementa);
                }
                imprimere("DECLARATOR_FUNCTI: habet_void=%s, params=%d\n",
                         nodus->datum.declarator_functi.habet_void ? "VERUM" : "FALSUM",
                         num_params);
                si (nodus->datum.declarator_functi.declarator_interior != NIHIL)
                {
                    _imprimere_arborem(nodus->datum.declarator_functi.declarator_interior, profunditas + I);
                }
                /* Print parameters if present */
                si (param_lista != NIHIL && param_lista->genus == ARBOR2_NODUS_LISTA_SEPARATA)
                {
                    i32 j;
                    per (j = ZEPHYRUM; j < num_params; j++)
                    {
                        Arbor2Nodus* param;
                        param = *(Arbor2Nodus**)xar_obtinere(param_lista->datum.lista_separata.elementa, j);
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
            Arbor2Nodus* enumeratores = nodus->datum.enum_specifier.enumeratores;

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

            si (enumeratores != NIHIL && enumeratores->genus == ARBOR2_NODUS_LISTA_SEPARATA)
            {
                i32 num = xar_numerus(enumeratores->datum.lista_separata.elementa);
                i32 j;
                per (j = ZEPHYRUM; j < num; j++)
                {
                    Arbor2Nodus** slot = xar_obtinere(enumeratores->datum.lista_separata.elementa, j);
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

            si (nodus->datum.declarator.pointer_levels != NIHIL)
            {
                imprimere(" (ptr=%d)", xar_numerus(nodus->datum.declarator.pointer_levels));
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

        casus ARBOR2_NODUS_TERNARIUS:
            imprimere("TERNARIUS: ?:\n");
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("conditio:\n");
            _imprimere_arborem(nodus->datum.ternarius.conditio, profunditas + II);
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("verum:\n");
            _imprimere_arborem(nodus->datum.ternarius.verum, profunditas + II);
            per (i = ZEPHYRUM; i < profunditas + I; i++) imprimere("  ");
            imprimere("falsum:\n");
            _imprimere_arborem(nodus->datum.ternarius.falsum, profunditas + II);
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
    b32 praeteritus;

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
     * PROBARE: Table validation
     * ======================================================== */

    {
        b32 tabula_valida;
        b32 tags_valida;
        b32 reachability_valida;
        b32 goto_valida;

        imprimere("\n--- Probans table validation ---\n");

        tabula_valida = arbor2_glr_validare_tabulas();
        CREDO_AEQUALIS_I32((i32)tabula_valida, VERUM);

        imprimere("\n--- Probans tag validation ---\n");

        tags_valida = arbor2_glr_validare_tags();
        CREDO_AEQUALIS_I32((i32)tags_valida, VERUM);

        imprimere("\n--- Probans reachability ---\n");

        reachability_valida = arbor2_glr_validare_reachability();
        /* NOTE: Not asserting - dead states are informational, not fatal */
        imprimere("  Reachability: %s\n", reachability_valida ? "VERUM" : "FALSUM (dead states above)");

        imprimere("\n--- Probans GOTO completeness ---\n");

        goto_valida = arbor2_glr_validare_goto_completeness();
        /* NOTE: Not asserting - just reporting missing GOTOs for diagnostic purposes */
        imprimere("  GOTO validation: %s\n", goto_valida ? "VERUM" : "FALSUM (issues above)");
    }


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
        /* Should be PARENTHESIZED containing BINARIUM */
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PARENTHESIZED);
            CREDO_NON_NIHIL(res.radix->datum.parenthesized.expressio);
            si (res.radix->datum.parenthesized.expressio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.parenthesized.expressio->genus, (i32)ARBOR2_NODUS_BINARIUM);
            }
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
            /* Should be PARENTHESIZED containing BINARIUM */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PARENTHESIZED);
            si (res.radix->datum.parenthesized.expressio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.parenthesized.expressio->genus, (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.parenthesized.expressio->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_MINOR);
            }
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
            /* Should be PARENTHESIZED containing PARENTHESIZED containing BINARIUM */
            Arbor2Nodus* inner;
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_PARENTHESIZED);
            inner = res.radix->datum.parenthesized.expressio;
            si (inner != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)inner->genus, (i32)ARBOR2_NODUS_PARENTHESIZED);
                si (inner->datum.parenthesized.expressio != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)inner->datum.parenthesized.expressio->genus, (i32)ARBOR2_NODUS_BINARIUM);
                    CREDO_AEQUALIS_I32((i32)inner->datum.parenthesized.expressio->datum.binarium.operator,
                                       (i32)ARBOR2_LEXEMA_MINOR);
                }
            }
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
     * PROBARE: Left shift operator << (E9.3 - translatio)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sinistrum shift: a << 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a << 2");
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
                               (i32)ARBOR2_LEXEMA_SINISTRUM);
        }
    }

    /* ========================================================
     * PROBARE: Right shift operator >> (E9.3 - translatio)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans dextrum shift: x >> 3 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "x >> 3");
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
                               (i32)ARBOR2_LEXEMA_DEXTRUM);
        }
    }

    /* ========================================================
     * PROBARE: Chained shifts: a << b >> c (E9.3)
     * Should parse as (a << b) >> c (left associative)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans shift catena: a << b >> c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a << b >> c");
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
            /* Root should be >> */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_DEXTRUM);
            /* Left child should be << */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_SINISTRUM);
            }
        }
    }

    /* ========================================================
     * PROBARE: Shift with addition: (x + y) << 2 (E9.3)
     * Addition binds tighter, so x + y groups first
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans shift cum additione: x + y << 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "x + y << 2");
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
            /* Root should be << (shift binds less tightly than +) */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_SINISTRUM);
            /* Left child should be + */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_PLUS);
            }
        }
    }

    /* ========================================================
     * PROBARE: Comparison with shift: a < b << c (E9.3)
     * Shift binds tighter than comparison
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparatio cum shift: a < b << c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a < b << c");
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
            /* Root should be < (comparison binds less tightly than shift) */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINOR);
            /* Right child should be << */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_SINISTRUM);
            }
        }
    }

    /* ========================================================
     * PROBARE: Logical AND operator && (E10)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans coniunctio: a && b ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a && b");
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
                               (i32)ARBOR2_LEXEMA_DUAMPERSAND);
        }
    }

    /* ========================================================
     * PROBARE: Logical OR operator || (E10)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans disiunctio: a || b ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a || b");
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
                               (i32)ARBOR2_LEXEMA_DUPIPA);
        }
    }

    /* ========================================================
     * PROBARE: && has higher precedence than || (E10)
     * a || b && c should parse as a || (b && c)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans praecedentia: a || b && c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a || b && c");
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
            /* Root should be || */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_DUPIPA);
            /* Right child should be && */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_DUAMPERSAND);
            }
        }
    }

    /* ========================================================
     * PROBARE: Chained && is left-associative (E10)
     * a && b && c should parse as (a && b) && c
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans coniunctio catena: a && b && c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a && b && c");
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
            /* Root should be && */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_DUAMPERSAND);
            /* Left child should also be && (left-associative) */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_DUAMPERSAND);
            }
        }
    }

    /* ========================================================
     * PROBARE: Logical operators with comparisons (E10)
     * a == b && c < d should parse as (a == b) && (c < d)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans logica cum comparatiis: a == b && c < d ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a == b && c < d");
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
            /* Root should be && */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_DUAMPERSAND);
            /* Left child should be == */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_AEQUALIS);
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
                    Arbor2Nodus* pl = decl->datum.declarator_functi.parametri;
                    CREDO_AEQUALIS_I32((i32)pl->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32(xar_numerus(pl->datum.lista_separata.elementa), I);
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
                    Arbor2Nodus* pl = decl->datum.declarator_functi.parametri;
                    CREDO_AEQUALIS_I32((i32)pl->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32(xar_numerus(pl->datum.lista_separata.elementa), II);
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
                    Arbor2Nodus* pl = decl->datum.declarator_functi.parametri;
                    Arbor2Nodus* param;
                    CREDO_AEQUALIS_I32((i32)pl->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32(xar_numerus(pl->datum.lista_separata.elementa), I);
                    param = *(Arbor2Nodus**)xar_obtinere(pl->datum.lista_separata.elementa, ZEPHYRUM);
                    CREDO_AEQUALIS_I32((i32)param->genus, (i32)ARBOR2_NODUS_PARAMETER_DECL);
                    /* Check param declarator has pointer */
                    si (param->datum.parameter_decl.declarator != NIHIL)
                    {
                        CREDO_VERUM(param->datum.parameter_decl.declarator->datum.declarator.pointer_levels != NIHIL);
                        CREDO_AEQUALIS_I32((i32)xar_numerus(param->datum.parameter_decl.declarator->datum.declarator.pointer_levels), I);
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
     * PROBARE: Subscript in if condition: if (arr[0]) x;
     * Tests state 31 GOTO(POSTFIXUM)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans subscript in if condition: if (arr[0]) x; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "if (arr[0]) x;");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SI);
        }
    }

    /* ========================================================
     * PROBARE: Assignment inside if condition: if (x = 1) y;
     * Common C idiom - assignment in condition
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans assignment in if condition: if (x = 1) y; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "if (x = 1) y;");
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
            /* Condition should be an assignment */
            si (res.radix->datum.conditionale.conditio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.conditionale.conditio->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.conditionale.conditio->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_ASSIGNATIO);
            }
        }
    }


    /* ========================================================
     * PROBARE: Assignment inside while condition: while (x = 1) y;
     * Common C idiom - assignment in condition
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans assignment in while condition: while (x = 1) y; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "while (x = 1) y;");
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
            /* Condition should be an assignment */
            si (res.radix->datum.iteratio.conditio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.iteratio.conditio->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)res.radix->datum.iteratio.conditio->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_ASSIGNATIO);
            }
        }
    }


    /* ========================================================
     * PROBARE: Assignment in for-init: for (a = 1; b; c) d;
     * Common C idiom - assignment in for loop init
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans assignment in for-init: for (a = 1; b; c) d; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "for (a = 1; b; c) d;");
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
        }
    }


    /* ========================================================
     * PROBARE: Assignment in do-while: do x; while (a = 1);
     * Common C idiom - assignment in do-while condition
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans assignment in do-while: do x; while (a = 1); ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "do x; while (a = 1);");
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
        }
    }


    /* ========================================================
     * PROBARE: Assignment in switch: switch (a = 1) { }
     * Less common but valid C
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans assignment in switch: switch (a = 1) { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "switch (a = 1) { }");
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
                    Arbor2Nodus* pl = decl->datum.declarator_functi.parametri;
                    CREDO_AEQUALIS_I32((i32)pl->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32(xar_numerus(pl->datum.lista_separata.elementa), II);
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
                CREDO_VERUM(decl->datum.declarator_functi.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator_functi.pointer_levels), I);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Qualified return type: const int f() { } (P505)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans qualified return: const int f() { } (P505) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "const int f() { }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            /* Check tok_const is set */
            CREDO_NON_NIHIL(res.radix->datum.definitio_functi.tok_const);
            /* Check tok_volatile is NOT set */
            CREDO_NIHIL(res.radix->datum.definitio_functi.tok_volatile);
            /* Check lexema is the type token (int) */
            CREDO_NON_NIHIL(res.radix->lexema);
            si (res.radix->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_INT);
            }
            /* Check declarator exists */
            CREDO_NON_NIHIL(res.radix->datum.definitio_functi.declarator);
            /* Check corpus exists */
            CREDO_NON_NIHIL(res.radix->datum.definitio_functi.corpus);
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Qualified return type: volatile int g() { } (P506)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans qualified return: volatile int g() { } (P506) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "volatile int g() { }");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            /* Check tok_volatile is set */
            CREDO_NON_NIHIL(res.radix->datum.definitio_functi.tok_volatile);
            /* Check tok_const is NOT set */
            CREDO_NIHIL(res.radix->datum.definitio_functi.tok_const);
            /* Check lexema is the type token (int) */
            CREDO_NON_NIHIL(res.radix->lexema);
            si (res.radix->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_INT);
            }
            /* Check declarator exists */
            CREDO_NON_NIHIL(res.radix->datum.definitio_functi.declarator);
            /* Check corpus exists */
            CREDO_NON_NIHIL(res.radix->datum.definitio_functi.corpus);
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Basic function: void f() { } - verify tok_const/tok_volatile are NIHIL
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans basic return: void f() { } (no qualifier) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "void f() { }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            /* Check tok_const is NOT set (P44, not P505) */
            CREDO_NIHIL(res.radix->datum.definitio_functi.tok_const);
            /* Check tok_volatile is NOT set */
            CREDO_NIHIL(res.radix->datum.definitio_functi.tok_volatile);
            /* Check lexema is the type token (void) */
            CREDO_NON_NIHIL(res.radix->lexema);
            si (res.radix->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_VOID);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Qualified return with char: const char h() { }
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans qualified return: const char h() { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "const char h() { }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            /* Check tok_const is set */
            CREDO_NON_NIHIL(res.radix->datum.definitio_functi.tok_const);
            /* Check lexema is char token */
            CREDO_NON_NIHIL(res.radix->lexema);
            si (res.radix->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_CHAR);
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ================================================================
     * COMPOUND RETURN TYPE TESTS
     * These test function definitions with compound type specifiers.
     * ================================================================ */

    /* Test function with unsigned int return type */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans compound return: unsigned int f() { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "unsigned int f() { }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        }
    }

    /* Test function with long int return type */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans compound return: long int g() { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "long int g() { }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        }
    }

    /* Test function with short int return type */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans compound return: short int h() { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "short int h() { }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        }
    }

    /* Test function with signed int return type */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans compound return: signed int s() { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "signed int s() { }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        }
    }

    /* Test function with unsigned long return type */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans compound return: unsigned long l() { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "unsigned long l() { }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        }
    }

    /* Test function with unsigned return type (implicit int) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans compound return: unsigned u() { } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "unsigned u() { }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
        }
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

    /* ================================================================
     * PROBARE: Struct members with various type specifiers
     * These test that all C89 type specifiers work in struct members.
     * ================================================================ */

    /* Test struct with float member: struct foo { float f; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct float member: struct foo { float f; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { float f; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), I);
            }
        }
    }

    /* Test struct with double member: struct foo { double d; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct double member: struct foo { double d; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { double d; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Test struct with short member: struct foo { short s; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct short member: struct foo { short s; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { short s; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Test struct with long member: struct foo { long l; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct long member: struct foo { long l; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { long l; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Test struct with unsigned member: struct foo { unsigned u; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct unsigned member: struct foo { unsigned u; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { unsigned u; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Test struct with signed member: struct foo { signed s; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct signed member: struct foo { signed s; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { signed s; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Test struct with mixed type members: struct foo { int x; float f; } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct mixed members: struct foo { int x; float f; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { int x; float f; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            si (res.radix->datum.struct_specifier.membra != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.struct_specifier.membra), II);
            }
        }
    }

    /* ================================================================
     * COMPOUND TYPE SPECIFIER TESTS
     * These test multi-token type specifiers in struct members.
     * ================================================================ */

    /* Test struct with unsigned int member */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct unsigned int: struct foo { unsigned int x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { unsigned int x; }");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Test struct with long int member */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct long int: struct foo { long int x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { long int x; }");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Test struct with const int member */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct const int: struct foo { const int x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { const int x; }");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Test struct with unsigned long member */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct unsigned long: struct foo { unsigned long x; } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { unsigned long x; }");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
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
                Arbor2Nodus* el = res.radix->datum.enum_specifier.enumeratores;
                CREDO_AEQUALIS_I32((i32)el->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                CREDO_AEQUALIS_I32(xar_numerus(el->datum.lista_separata.elementa), III);  /* 3 enumerators */
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
                Arbor2Nodus* el = res.radix->datum.enum_specifier.enumeratores;
                CREDO_AEQUALIS_I32((i32)el->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                CREDO_AEQUALIS_I32(xar_numerus(el->datum.lista_separata.elementa), III);  /* 3 enumerators */
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
                Arbor2Nodus* enums_lista = res.radix->datum.enum_specifier.enumeratores;
                Arbor2Nodus** e0;
                Arbor2Nodus** e1;
                Arbor2Nodus** e2;

                CREDO_AEQUALIS_I32((i32)enums_lista->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                CREDO_AEQUALIS_I32(xar_numerus(enums_lista->datum.lista_separata.elementa), III);  /* 3 enumerators */

                /* Check first enumerator has value */
                e0 = xar_obtinere(enums_lista->datum.lista_separata.elementa, ZEPHYRUM);
                si (e0 != NIHIL && *e0 != NIHIL)
                {
                    CREDO_NON_NIHIL((*e0)->datum.enumerator.valor);  /* A = 1 */
                }

                /* Check second enumerator has no value */
                e1 = xar_obtinere(enums_lista->datum.lista_separata.elementa, I);
                si (e1 != NIHIL && *e1 != NIHIL)
                {
                    CREDO_NIHIL((*e1)->datum.enumerator.valor);  /* B (no value) */
                }

                /* Check third enumerator has value */
                e2 = xar_obtinere(enums_lista->datum.lista_separata.elementa, II);
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
                Arbor2Nodus* enums_lista = res.radix->datum.enum_specifier.enumeratores;
                Xar* enums;
                Arbor2Nodus** e0;

                CREDO_AEQUALIS_I32((i32)enums_lista->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                enums = enums_lista->datum.lista_separata.elementa;
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

    /* Test enum with unary minus: enum { A = -1 } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum unary minus: enum { A = -1 } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum { A = -1 }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
        }
    }

    /* Test enum with unary plus: enum { A = +1 } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum unary plus: enum { A = +1 } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum { A = +1 }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
        }
    }

    /* Test enum with bitwise NOT: enum { A = ~0 } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum bitwise NOT: enum { A = ~0 } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum { A = ~0 }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
        }
    }

    /* Test enum with logical NOT: enum { A = !0 } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum logical NOT: enum { A = !0 } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum { A = !0 }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
        }
    }

    /* Test enum with sizeof: enum { A = sizeof(int) } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum sizeof: enum { A = sizeof(int) } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum { A = sizeof(int) }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
        }
    }

    /* Test enum with char literal: enum { A = 'X' } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum char literal: enum { A = 'X' } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum { A = 'X' }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
        }
    }

    /* Test enum with subsequent enumerator using new expressions: enum { A, B = ~1, C = -2 } */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans enum subsequent expr: enum { A, B = ~1, C = -2 } ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "enum { A, B = ~1, C = -2 }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  resultus: %s\n", res.successus ? "verum" : "falsum");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, ZEPHYRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_ENUM_SPECIFIER);
        }
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
                    /* Declarator should have one pointer level */
                    CREDO_NON_NIHIL(member->datum.declaratio.declarator);
                    si (member->datum.declaratio.declarator != NIHIL)
                    {
                        CREDO_VERUM(member->datum.declaratio.declarator->datum.declarator.pointer_levels != NIHIL);
                        CREDO_AEQUALIS_I32((i32)xar_numerus(member->datum.declaratio.declarator->datum.declarator.pointer_levels), I);
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
                CREDO_VERUM(res.radix->datum.declaratio.declarator->datum.declarator.pointer_levels == NIHIL);
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
                CREDO_VERUM(res.radix->datum.declaratio.declarator->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.declaratio.declarator->datum.declarator.pointer_levels), I);
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
                CREDO_VERUM(res.radix->datum.declaratio.declarator->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.declaratio.declarator->datum.declarator.pointer_levels), I);
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
                CREDO_VERUM(res.radix->datum.declaratio.declarator->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.declaratio.declarator->datum.declarator.pointer_levels), I);
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
                    Arbor2ArrayDimension** dim_ptr;
                    Arbor2ArrayDimension* dim;
                    CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.dimensiones), 1);
                    dim_ptr = xar_obtinere(decl->datum.declarator.dimensiones, ZEPHYRUM);
                    dim = *dim_ptr;
                    CREDO_NON_NIHIL(dim);
                    CREDO_NON_NIHIL(dim->tok_bracket_ap);
                    CREDO_NIHIL(dim->dimensio);  /* Unsized array has NIHIL dimension */
                    CREDO_NON_NIHIL(dim->tok_bracket_cl);
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

    /* Test array of pointers: int *arr[10]
     * NOTE: This may produce AMBIGUUS because the grammar doesn't distinguish
     * keywords from identifiers in expression contexts. The declaration
     * interpretation should be present in the ambiguity. */
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
            /* Accept DECLARATIO or AMBIGUUS (with declaration as one interpretation) */
            si (res.radix->genus == ARBOR2_NODUS_DECLARATIO)
            {
                si (res.radix->datum.declaratio.declarator != NIHIL)
                {
                    Arbor2Nodus* decl = res.radix->datum.declaratio.declarator;
                    CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR);
                    CREDO_VERUM(decl->datum.declarator.pointer_levels != NIHIL);
                    CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.pointer_levels), I);
                    CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                }
            }
            alioquin si (res.radix->genus == ARBOR2_NODUS_AMBIGUUS)
            {
                /* GLR found both decl and expr paths - acceptable for now */
                imprimere("  (ambiguus: decl vs expr - grammar limitation)\n");
            }
            alioquin
            {
                /* Unexpected - force failure */
                CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
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

    /* Test typedef float: typedef float MyFloat; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans typedef float: typedef float MyFloat; ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "typedef float MyFloat;");
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
                Arbor2ArrayDimension** dim_ptr = xar_obtinere(decl->datum.declarator.dimensiones, ZEPHYRUM);
                Arbor2ArrayDimension* dim = *dim_ptr;
                CREDO_NON_NIHIL(dim);
                CREDO_NON_NIHIL(dim->tok_bracket_ap);
                CREDO_NON_NIHIL(dim->dimensio);  /* Should have expression node */
                si (dim->dimensio != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)dim->dimensio->genus, (i32)ARBOR2_NODUS_BINARIUM);
                }
                CREDO_NON_NIHIL(dim->tok_bracket_cl);
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
                    CREDO_VERUM(decl->datum.declarator.pointer_levels != NIHIL);
                    CREDO_AEQUALIS_I32((i32)xar_numerus(decl->datum.declarator.pointer_levels), (i32)I);
                    /* Should have array dimension */
                    CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                }
            }
        }

        imprimere("  furcae: %d\n", glr->num_furcae);
    }


    /* ========================================================
     * PROBARE: Assignment operator - simple a = 1
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans assignment: a = 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a = 1");
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
                               (i32)ARBOR2_LEXEMA_ASSIGNATIO);
        }
    }

    /* ========================================================
     * PROBARE: Assignment right-associativity - a = b = c
     * Should parse as a = (b = c), NOT (a = b) = c
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;

        imprimere("\n--- Probans assignment right-associativity: a = b = c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a = b = c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be = with a on left, (b = c) on right */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_ASSIGNATIO);

            /* Right should be b = c (another assignment) */
            dexter = radix->datum.binarium.dexter;
            CREDO_NON_NIHIL(dexter);
            si (dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)dexter->genus, (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_ASSIGNATIO);
            }
        }
    }

    /* ========================================================
     * PROBARE: Compound assignment operators
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans compound assignments: += -= *= /= ---\n");

        /* Test += */
        tokens = _lexare_ad_tokens(piscina, intern, "a += 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_PLUS_ASSIGN);
        }

        /* Test -= */
        tokens = _lexare_ad_tokens(piscina, intern, "a -= 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MINUS_ASSIGN);
        }

        /* Test *= */
        tokens = _lexare_ad_tokens(piscina, intern, "a *= 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_MULT_ASSIGN);
        }

        /* Test /= */
        tokens = _lexare_ad_tokens(piscina, intern, "a /= 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_DIV_ASSIGN);
        }
    }

    /* ========================================================
     * PROBARE: Bitwise assignment operators
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans bitwise assignments: &= |= ^= <<= >>= ---\n");

        /* Test &= */
        tokens = _lexare_ad_tokens(piscina, intern, "a &= 0xFF");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_AND_ASSIGN);
        }

        /* Test |= */
        tokens = _lexare_ad_tokens(piscina, intern, "a |= 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_OR_ASSIGN);
        }

        /* Test ^= */
        tokens = _lexare_ad_tokens(piscina, intern, "a ^= mask");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_XOR_ASSIGN);
        }

        /* Test <<= */
        tokens = _lexare_ad_tokens(piscina, intern, "a <<= 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_SHL_ASSIGN);
        }

        /* Test >>= */
        tokens = _lexare_ad_tokens(piscina, intern, "a >>= 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_SHR_ASSIGN);
        }
    }

    /* ========================================================
     * PROBARE: Comma operator - a, b
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comma operator: a, b ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a, b");
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
                               (i32)ARBOR2_LEXEMA_COMMA);
        }
    }

    /* ========================================================
     * PROBARE: Comma left-associativity - a, b, c
     * Should parse as ((a, b), c)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* sinister;

        imprimere("\n--- Probans comma left-associativity: a, b, c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a, b, c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be , with (a, b) on left, c on right */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_COMMA);

            /* Left should be (a, b) another comma */
            sinister = radix->datum.binarium.sinister;
            CREDO_NON_NIHIL(sinister);
            si (sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)sinister->genus, (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_COMMA);
            }
        }
    }

    /* ========================================================
     * PROBARE: Assignment / comma precedence - a = 1, b = 2
     * Should parse as (a = 1), (b = 2) - comma binds looser
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* sinister;
        Arbor2Nodus* dexter;

        imprimere("\n--- Probans assignment/comma precedence: a = 1, b = 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a = 1, b = 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be comma */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_COMMA);

            /* Left should be a = 1 */
            sinister = radix->datum.binarium.sinister;
            CREDO_NON_NIHIL(sinister);
            si (sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)sinister->genus, (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)sinister->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_ASSIGNATIO);
            }

            /* Right should be b = 2 */
            dexter = radix->datum.binarium.dexter;
            CREDO_NON_NIHIL(dexter);
            si (dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)dexter->genus, (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_ASSIGNATIO);
            }
        }
    }

    /* ========================================================
     * PROBARE: Assignment below logical OR - a = b || c
     * Should parse as a = (b || c)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;

        imprimere("\n--- Probans assignment precedence: a = b || c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a = b || c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be = */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_ASSIGNATIO);

            /* Right should be || */
            dexter = radix->datum.binarium.dexter;
            CREDO_NON_NIHIL(dexter);
            si (dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)dexter->genus, (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)dexter->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_DUPIPA);
            }
        }
    }


    /* ========================================================
     * PROBARE: Ternary operator - simple
     * a ? 1 : c -> TERNARIUS(a, 1, c)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans ternary operator: a ? 1 : c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a ? 1 : c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be ternary */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_TERNARIUS);

            /* Condition should be identifier 'a' */
            CREDO_NON_NIHIL(radix->datum.ternarius.conditio);
            si (radix->datum.ternarius.conditio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.conditio->genus,
                                   (i32)ARBOR2_NODUS_IDENTIFICATOR);
            }

            /* True branch should be integer '1' */
            CREDO_NON_NIHIL(radix->datum.ternarius.verum);
            si (radix->datum.ternarius.verum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.verum->genus,
                                   (i32)ARBOR2_NODUS_INTEGER);
            }

            /* False branch should be identifier 'c' */
            CREDO_NON_NIHIL(radix->datum.ternarius.falsum);
            si (radix->datum.ternarius.falsum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.falsum->genus,
                                   (i32)ARBOR2_NODUS_IDENTIFICATOR);
            }
        }
    }


    /* ========================================================
     * PROBARE: Ternary operator - right associativity
     * a ? 1 : b ? 2 : 3 -> a ? 1 : (b ? 2 : 3)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* falsum;

        imprimere("\n--- Probans ternary right-associativity: a ? 1 : b ? 2 : 3 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a ? 1 : b ? 2 : 3");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be ternary: a ? 1 : (nested) */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_TERNARIUS);

            /* Condition = a */
            CREDO_NON_NIHIL(radix->datum.ternarius.conditio);
            si (radix->datum.ternarius.conditio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.conditio->genus,
                                   (i32)ARBOR2_NODUS_IDENTIFICATOR);
            }

            /* True = 1 */
            CREDO_NON_NIHIL(radix->datum.ternarius.verum);
            si (radix->datum.ternarius.verum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.verum->genus,
                                   (i32)ARBOR2_NODUS_INTEGER);
            }

            /* False = nested ternary (b ? 2 : 3) - proves right-associativity */
            falsum = radix->datum.ternarius.falsum;
            CREDO_NON_NIHIL(falsum);
            si (falsum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)falsum->genus, (i32)ARBOR2_NODUS_TERNARIUS);

                /* Nested condition = b */
                si (falsum->datum.ternarius.conditio != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)falsum->datum.ternarius.conditio->genus,
                                       (i32)ARBOR2_NODUS_IDENTIFICATOR);
                }

                /* Nested true = 2 */
                si (falsum->datum.ternarius.verum != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)falsum->datum.ternarius.verum->genus,
                                       (i32)ARBOR2_NODUS_INTEGER);
                }

                /* Nested false = 3 */
                si (falsum->datum.ternarius.falsum != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)falsum->datum.ternarius.falsum->genus,
                                       (i32)ARBOR2_NODUS_INTEGER);
                }
            }
        }
    }


    /* ========================================================
     * PROBARE: Ternary with assignment
     * a = x ? 1 : 2 -> a = (x ? 1 : 2)
     * Ternary binds tighter than assignment
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;

        imprimere("\n--- Probans ternary/assignment precedence: a = x ? 1 : 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a = x ? 1 : 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be assignment (=) */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)radix->datum.binarium.operator,
                               (i32)ARBOR2_LEXEMA_ASSIGNATIO);

            /* Left = a */
            CREDO_NON_NIHIL(radix->datum.binarium.sinister);
            si (radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_IDENTIFICATOR);
            }

            /* Right = ternary (x ? 1 : 2) */
            dexter = radix->datum.binarium.dexter;
            CREDO_NON_NIHIL(dexter);
            si (dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)dexter->genus, (i32)ARBOR2_NODUS_TERNARIUS);
            }
        }
    }


    /* ========================================================
     * PROBARE: Ternary with logical OR
     * a || b ? 1 : 2 -> (a || b) ? 1 : 2
     * Logical OR binds tighter than ternary
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* conditio;

        imprimere("\n--- Probans ternary/logical-or precedence: a || b ? 1 : 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a || b ? 1 : 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be ternary */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_TERNARIUS);

            /* Condition = (a || b) - || binds tighter */
            conditio = radix->datum.ternarius.conditio;
            CREDO_NON_NIHIL(conditio);
            si (conditio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)conditio->genus, (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)conditio->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_DUPIPA);
            }

            /* True = 1 */
            CREDO_NON_NIHIL(radix->datum.ternarius.verum);
            si (radix->datum.ternarius.verum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.verum->genus,
                                   (i32)ARBOR2_NODUS_INTEGER);
            }

            /* False = 2 */
            CREDO_NON_NIHIL(radix->datum.ternarius.falsum);
            si (radix->datum.ternarius.falsum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.falsum->genus,
                                   (i32)ARBOR2_NODUS_INTEGER);
            }
        }
    }


    /* ========================================================
     * PROBARE: Ternary with expressions in branches
     * a ? 1 + 2 : 3 * 4 (use integers to avoid label ambiguity)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans ternary with expressions: a ? 1 + 2 : 3 * 4 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a ? 1 + 2 : 3 * 4");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        si (res.radix != NIHIL)
        {
            _imprimere_arborem(res.radix, II);
        }

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Root should be ternary */
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_TERNARIUS);

            /* True branch = 1 + 2 */
            si (radix->datum.ternarius.verum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.verum->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.verum->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_PLUS);
            }

            /* False branch = 3 * 4 */
            si (radix->datum.ternarius.falsum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.falsum->genus,
                                   (i32)ARBOR2_NODUS_BINARIUM);
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.falsum->datum.binarium.operator,
                                   (i32)ARBOR2_LEXEMA_ASTERISCUS);
            }
        }
    }


    /* ========================================================
     * PROBARE: Unary operators - more tests
     * ======================================================== */

    /* Bitwise NOT: ~x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans bitwise NOT: ~x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "~x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_TILDE);
            }
        }
    }

    /* Logical NOT: !x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans logical NOT: !x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "!x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_EXCLAMATIO);
            }
        }
    }

    /* Double NOT: !!x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans double NOT: !!x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "!!x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->datum.unarium.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.unarium.operandum->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }

    /* Mixed unary: ~!x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans mixed unary: ~!x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "~!x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_TILDE);
            }
            si (radix->datum.unarium.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.unarium.operandum->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }

    /* Unary with binary: ~x + y */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans unary with binary: ~x + y ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "~x + y");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: (~x) + y */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.sinister->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }

    /* Triple unary: ***ptr */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans triple deref: ***ptr ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "***ptr");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->datum.unarium.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.unarium.operandum->genus, (i32)ARBOR2_NODUS_UNARIUM);
                si (radix->datum.unarium.operandum->datum.unarium.operandum != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)radix->datum.unarium.operandum->datum.unarium.operandum->genus, (i32)ARBOR2_NODUS_UNARIUM);
                }
            }
        }
    }

    /* Address of address: & &x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans double address: & &x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "& &x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->datum.unarium.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.unarium.operandum->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }

    /* Deref then address: *&x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans deref-address: *&x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "*&x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_ASTERISCUS);
            }
            si (radix->datum.unarium.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.unarium.operandum->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }

    /* ========================================================
     * PROBARE: Unary minus and plus operators
     * ======================================================== */

    /* Unary minus: -x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans unary minus: -x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "-x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_MINUS);
            }
        }
    }

    /* Unary plus: +x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans unary plus: +x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "+x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_PLUS);
            }
        }
    }

    /* Unary minus with integer literal: -5 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans unary minus integer: -5 ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "-5");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->datum.unarium.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.unarium.operandum->genus, (i32)ARBOR2_NODUS_INTEGER);
            }
        }
    }

    /* Double negation: - -x (space between) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans double negation: - -x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "- -x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->datum.unarium.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.unarium.operandum->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }

    /* Unary minus with binary plus: -a + b */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans unary with binary: -a + b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "-a + b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: (-a) + b */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.sinister->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }

    /* Unary minus after binary: a + -b */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans binary then unary: a + -b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a + -b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: a + (-b) */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.dexter->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }

    /* Mixed unary: -!x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans mixed: -!x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "-!x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_MINUS);
            }
            si (radix->datum.unarium.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.unarium.operandum->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }

    /* Unary minus with multiply: a * -b */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans multiply unary: a * -b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a * -b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: a * (-b) */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.dexter->genus, (i32)ARBOR2_NODUS_UNARIUM);
            }
        }
    }


    /* ========================================================
     * PROBARE: Bitwise binary operators
     * ======================================================== */

    /* Bitwise AND: a & b */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans bitwise AND: a & b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a & b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_AMPERSAND);
            }
        }
    }

    /* Bitwise OR: a | b */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans bitwise OR: a | b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a | b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_PIPA);
            }
        }
    }

    /* Bitwise XOR: a ^ b */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans bitwise XOR: a ^ b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a ^ b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_CARET);
            }
        }
    }

    /* Bitwise precedence: a | b ^ c */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans bitwise precedence: a | b ^ c (| lower than ^) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a | b ^ c");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: a | (b ^ c) - | has lower precedence than ^ */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_PIPA);
            }
            si (radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.dexter->genus, (i32)ARBOR2_NODUS_BINARIUM);
            }
        }
    }

    /* Bitwise precedence: a ^ b & c */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans bitwise precedence: a ^ b & c (^ lower than &) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a ^ b & c");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: a ^ (b & c) - ^ has lower precedence than & */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_CARET);
            }
            si (radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.dexter->genus, (i32)ARBOR2_NODUS_BINARIUM);
            }
        }
    }

    /* Bitwise chain: a | b | c */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans bitwise chain: a | b | c (left-associative) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a | b | c");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: (a | b) | c */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.sinister->genus, (i32)ARBOR2_NODUS_BINARIUM);
            }
            si (radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.dexter->genus, (i32)ARBOR2_NODUS_IDENTIFICATOR);
            }
        }
    }

    /* Bitwise with comparison: a & b == c */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans bitwise/comparison: a & b == c ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a & b == c");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* In C: & is lower precedence than ==, so (a & b) == c would be wrong
             * but a & (b == c) is what most parsers do (including ours) */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
        }
    }

    /* All bitwise: a | b ^ c & d */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans all bitwise: a | b ^ c & d ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a | b ^ c & d");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: a | (b ^ (c & d)) */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_PIPA);
            }
        }
    }


    /* ========================================================
     * PROBARE: Complex nested expressions
     * ======================================================== */

    /* Deeply nested parens: (((x))) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans deeply nested parens: (((x))) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "(((x)))");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Complex arithmetic: (a + b) * (c - d) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans complex arithmetic: (a + b) * (c - d) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "(a + b) * (c - d)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->lexema != NIHIL && radix->lexema->lexema != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->lexema->lexema->genus, (i32)ARBOR2_LEXEMA_ASTERISCUS);
            }
        }
    }

    /* Mixed precedence: a + b * c - d / e */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans mixed precedence: a + b * c - d / e ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a + b * c - d / e");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: ((a + (b * c)) - (d / e)) */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
        }
    }

    /* Logical with ternary: a && b ? 1 : 2 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans logical with ternary: a && b ? 1 : 2 ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a && b ? 1 : 2");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: ((a && b) ? 1 : 2) */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_TERNARIUS);
        }
    }

    /* Multiple ternary: a ? b ? 1 : 2 : 3 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans nested ternary in true branch: a ? b ? 1 : 2 : 3 ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a ? b ? 1 : 2 : 3");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: a ? (b ? 1 : 2) : 3 */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_TERNARIUS);
            si (radix->datum.ternarius.verum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.ternarius.verum->genus, (i32)ARBOR2_NODUS_TERNARIUS);
            }
        }
    }

    /* Unary in expressions: ~a & !b | *c */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans unary in expressions: ~a & !b | *c ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "~a & !b | *c");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
        }
    }

    /* Assignment chain: a = b = c = 1 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans assignment chain: a = b = c = 1 ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a = b = c = 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: a = (b = (c = 1)) - right associative */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.dexter->genus, (i32)ARBOR2_NODUS_BINARIUM);
            }
        }
    }

    /* Comma chain: a, b, c, d */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans comma chain: a, b, c, d ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a, b, c, d");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: ((a, b), c), d) - left associative */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)radix->datum.binarium.sinister->genus, (i32)ARBOR2_NODUS_BINARIUM);
            }
        }
    }

    /* Shift with arithmetic: a << 2 + 1 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans shift with arithmetic: a << 2 + 1 ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a << 2 + 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            /* Should be: a << (2 + 1) - shift has lower precedence than + */
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
        }
    }


    /* ========================================================
     * PROBARE: Control flow edge cases
     * ======================================================== */

    /* if-else-if chain */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans if-else-if chain ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "if (a) x; else if (b) y; else z;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Deeply nested if: if (a) if (b) if (c) x; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans deeply nested if ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "if (a) if (b) if (c) x;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Nested loops: while (a) for (;;) if (b) break; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested loops with break ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "while (a) for (;;) if (b) break;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Complex for loop: for (a = 0; a < 10; a = a + 1) x; */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans complex for loop ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "for (a = 0; a < 10; a = a + 1) x;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Switch with fallthrough cases */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans switch with multiple cases ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "switch (x) { case 1: case 2: case 3: y; break; default: z; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* do-while with simple condition */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans do-while with simple condition ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "do { x; y; } while (a < b);");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Multiple statements in compound */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans multiple statements in compound ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "{ a; b; c; d; e; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Nested compound statements */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans nested compound statements ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "{ { { a; } } }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* ========================================================
     * PROBARE: Declarations in compound statements (C99-style)
     * ======================================================== */

    /* Simple declaration in block */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans declaration in block: { int x; } ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "{ int x; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CORPUS);
            si (res.radix->datum.corpus.sententiae != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.corpus.sententiae), I);
            }
        }
    }

    /* Declaration with initializer in block */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans declaration with init: { int x = 5; } ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "{ int x = 5; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Multiple declarations in block */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans multiple declarations: { int x; int y; } ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "{ int x; int y; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CORPUS);
            si (res.radix->datum.corpus.sententiae != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.corpus.sententiae), II);
            }
        }
    }

    /* Mixed declarations and statements (C99-style anywhere) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans mixed decl/stmt: { int x; x = 1; int y; return y; } ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "{ int x; x = 1; int y; return y; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CORPUS);
            si (res.radix->datum.corpus.sententiae != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.corpus.sententiae), IV);
            }
        }
    }

    /* Static declaration in block */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static in block: { static int x = 0; } ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "{ static int x = 0; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Const declaration in block */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const in block: { const int x = 42; } ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "{ const int x = 42; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Static const declaration in block */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static const in block: { static const int x = 1; } ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "{ static const int x = 1; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Return with expression */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans return with expression ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "return a + b * c;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }


    /* ========================================================
     * PROBARE: Combined stress tests
     * ======================================================== */

    /* Function with complex body */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans function with complex body ---\n");
        /* MyType already registered earlier in typedef tests */
        tokens = _lexare_ad_tokens(piscina, intern,
            "MyType foo(int x, int y) { "
            "if (x < y) return x; "
            "while (x > 0) x = x - 1; "
            "return y; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Struct with complex members */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans struct with complex members ---\n");
        tokens = _lexare_ad_tokens(piscina, intern,
            "struct Point { "
            "int x; "
            "int y; "
            "int z; "
            "int flags; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Expression in for condition */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans expression in for ---\n");
        tokens = _lexare_ad_tokens(piscina, intern,
            "for (a = 0; a < 10; a = a + 1) x;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Comparison in if condition */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans comparison in if condition ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "if (a < b) x;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* Assignment in while condition */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans assignment in while ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "while (x = x + 1) y;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }


    /* ========================================================
     * PROBARE: Unary ~ and ! in expression contexts
     * (TAG VALIDATION: tests for missing TILDE/EXCLAMATIO)
     * ======================================================== */

    /* State 6: after '(' - tilde in parentheses */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans (~a) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "(~a)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* State 7: after unary '*' */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans *~a ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "*~a");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* State 9: after '+' */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans a + ~b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a + ~b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* State 10: after '*' (multiplication) */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans a * !b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a * !b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* State 31: if condition with tilde */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans if (~a) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "if (~a) b;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* State 241: after comparison - tilde after < */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans a < ~b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a < ~b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* State 254: after && - tilde */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans a && ~b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a && ~b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* State 256: after || - exclamatio */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans a || !b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a || !b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* State 265: after << - tilde */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans a << ~b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a << ~b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }

    /* ========================================================
     * PROBARE: Array subscript expressions
     * ======================================================== */

    /* Simple subscript: arr[0] */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans arr[0] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "arr[0]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }

    /* Subscript with identifier index: arr[i] */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans arr[i] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "arr[i]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }

    /* Chained subscripts: arr[i][j] */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans arr[i][j] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "arr[i][j]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Outer subscript, inner should also be subscript */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
            si (res.radix->datum.subscriptio.basis != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.subscriptio.basis->genus,
                                   (i32)ARBOR2_NODUS_SUBSCRIPTIO);
            }
        }
    }

    /* Subscript in expression: a + b[0] */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans a + b[0] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a + b[0]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Should be BINARIUM with right child as SUBSCRIPTIO */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_SUBSCRIPTIO);
            }
        }
    }

    /* Multiply with subscript: a * b[0] - tests state 10 GOTO(POSTFIXUM)
     * Note: This is genuinely ambiguous (could be multiplication or pointer-to-array decl)
     * so we accept either BINARIUM or AMBIGUUS with BINARIUM as one interpretation */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans a * b[0] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "a * b[0]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Could be BINARIUM directly or AMBIGUUS (if decl path also succeeded) */
            si (res.radix->genus == ARBOR2_NODUS_BINARIUM)
            {
                /* Direct binary interpretation - check right child is subscript */
                si (res.radix->datum.binarium.dexter != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                       (i32)ARBOR2_NODUS_SUBSCRIPTIO);
                }
            }
            alioquin si (res.radix->genus == ARBOR2_NODUS_AMBIGUUS)
            {
                /* GLR found both expr and decl paths - acceptable */
                imprimere("  (ambiguus: mult vs decl - expected)\n");
            }
            alioquin
            {
                /* Unexpected result */
                CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            }
        }
    }

    /* Unary with subscript: *arr[i] - should be *(arr[i]) */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans *arr[i] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "*arr[i]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Should be UNARIUM with operand as SUBSCRIPTIO */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            si (res.radix->datum.unarium.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.unarium.operandum->genus,
                                   (i32)ARBOR2_NODUS_SUBSCRIPTIO);
            }
        }
    }

    /* Complex expression in subscript index */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans arr[i + 1] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "arr[i + 1]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }

    /* ========================================================
     * PROBARE: Function call expressions
     * ======================================================== */

    /* Empty function call: foo() */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans foo() ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "foo()");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }

    /* Function call with one arg: foo(a) */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans foo(a) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "foo(a)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }

    /* Function call with multiple args: foo(a, b, c) */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans foo(a, b, c) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "foo(a, b, c)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }

    /* Chained: foo()[0] - function call then subscript */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans foo()[0] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "foo()[0]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }

    /* Chained: arr[0]() - subscript then function call */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans arr[0]() ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "arr[0]()");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }

    /* Function call with expression args: foo(a + b) */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans foo(a + b) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "foo(a + b)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }

    /* ========================================================
     * PROBARE: Member access expressions
     * ======================================================== */

    /* Simple member access: s.x */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans s.x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "s.x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_MEMBRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.membrum.est_sagitta, FALSUM);
        }
    }

    /* Pointer member access: p->x */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans p->x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "p->x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_MEMBRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.membrum.est_sagitta, VERUM);
        }
    }

    /* Chained member access: s.x.y */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans s.x.y ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "s.x.y");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_MEMBRUM);
            /* y is the outer member, s.x is the basis */
            CREDO_AEQUALIS_I32((i32)res.radix->datum.membrum.basis->genus, (i32)ARBOR2_NODUS_MEMBRUM);
        }
    }

    /* Chained pointer member: p->x->y */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans p->x->y ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "p->x->y");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_MEMBRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.membrum.basis->genus, (i32)ARBOR2_NODUS_MEMBRUM);
        }
    }

    /* Mixed: subscript then member: arr[0].x */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans arr[0].x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "arr[0].x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_MEMBRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.membrum.basis->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }

    /* Mixed: call then member: foo().x */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans foo().x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "foo().x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_MEMBRUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.membrum.basis->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }

    /* Mixed: member then subscript: s.x[0] */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans s.x[0] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "s.x[0]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.subscriptio.basis->genus, (i32)ARBOR2_NODUS_MEMBRUM);
        }
    }

    /* Mixed: member then call: s.x() */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans s.x() ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "s.x()");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.vocatio.basis->genus, (i32)ARBOR2_NODUS_MEMBRUM);
        }
    }

    /* ========================================================
     * PROBARE: Increment/Decrement operators
     * ======================================================== */

    /* Post-increment: x++ */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans x++ ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "x++");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_POST_UNARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.post_unarium.operator,
                              (i32)ARBOR2_LEXEMA_DUPLUS);
        }
    }

    /* Post-decrement: x-- */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans x-- ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "x--");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_POST_UNARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.post_unarium.operator,
                              (i32)ARBOR2_LEXEMA_DUMINUS);
        }
    }

    /* Pre-increment: ++x */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans ++x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "++x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
        }
    }

    /* Pre-decrement: --x */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans --x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "--x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
        }
    }

    /* Post-increment on subscript: arr[0]++ */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans arr[0]++ ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "arr[0]++");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_POST_UNARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.post_unarium.operandum->genus,
                              (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }

    /* Pre-increment on subscript: ++arr[0] */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans ++arr[0] ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "++arr[0]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.unarium.operandum->genus,
                              (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }

    /* Post-increment on member: s.x++ */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans s.x++ ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "s.x++");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_POST_UNARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.post_unarium.operandum->genus,
                              (i32)ARBOR2_NODUS_MEMBRUM);
        }
    }

    /* Post-decrement on ptr member: p->x-- */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans p->x-- ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "p->x--");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_POST_UNARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.post_unarium.operandum->genus,
                              (i32)ARBOR2_NODUS_MEMBRUM);
        }
    }

    /* Mixed: ++x + y-- */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans ++x + y-- ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "++x + y--");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                              (i32)ARBOR2_NODUS_UNARIUM);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                              (i32)ARBOR2_NODUS_POST_UNARIUM);
        }
    }

    /* Chained post: x++++ should fail or parse as (x++)++ */
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans x++++ ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "x++++");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        /* This may succeed or fail - just testing it doesn't crash */
        CREDO_VERUM(VERUM);  /* Parser handles it */
        (vacuum)res;  /* Suppress unused warning */
    }


    /* ========================================================
     * PROBARE: Float literal
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans float literal: 3.14 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "3.14");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_FLOAT);
        }
    }


    /* ========================================================
     * PROBARE: Char literal
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans char literal: 'a' ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "'a'");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CHAR);
        }
    }


    /* ========================================================
     * PROBARE: String literal
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans string literal: \"hello\" ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "\"hello\"");
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
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_STRING);
        }
    }


    /* ========================================================
     * PROBARE: Float in binary expression
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans float in expression: 3.14 + 2.0 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "3.14 + 2.0");
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
            /* sinister should be FLOAT */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_FLOAT);
            }
            /* dexter should be FLOAT */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_FLOAT);
            }
        }
    }


    /* ========================================================
     * PROBARE: Char in binary expression
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans char in expression: 'a' + 'b' ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "'a' + 'b'");
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
            /* sinister should be CHAR */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_CHAR);
            }
            /* dexter should be CHAR */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_CHAR);
            }
        }
    }


    /* ========================================================
     * PROBARE: Mixed literals in expression
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans mixed literals: 42 + 3.14 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "42 + 3.14");
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
            /* sinister should be INTEGER */
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_INTEGER);
            }
            /* dexter should be FLOAT */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus,
                                   (i32)ARBOR2_NODUS_FLOAT);
            }
        }
    }


    /* ========================================================
     * PROBARE: sizeof operator
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_NON_NIHIL(res.radix->datum.sizeof_expr.operandum);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(x) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(x)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(x) + 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(x) + 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            /* Should be BINARIUM with sizeof on left */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_SIZEOF);
            }
        }
    }


    /* ========================================================
     * PROBARE: type casts
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (int)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            CREDO_NON_NIHIL(res.radix->datum.conversio.expressio);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (char)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(char)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (int)x + 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(int)x + 1");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            /* Should be BINARIUM with cast on left */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            si (res.radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.sinister->genus,
                                   (i32)ARBOR2_NODUS_CONVERSIO);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (int)(char)x (chained casts) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(int)(char)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            /* Outer should be CONVERSIO, inner should be CONVERSIO too */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            si (res.radix->datum.conversio.expressio != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.conversio.expressio->genus,
                                   (i32)ARBOR2_NODUS_CONVERSIO);
            }
        }
    }


    /* ========================================================
     * PROBARE: Pointer casts (Phase 1.1)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (int*)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(int*)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            /* typus should be DECLARATOR with 1 pointer */
            si (res.radix->datum.conversio.typus != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.conversio.typus->genus,
                                   (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_VERUM(res.radix->datum.conversio.typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.conversio.typus->datum.declarator.pointer_levels), (i32)I);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (char**)argv ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(char**)argv");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            /* typus should be DECLARATOR with 2 pointers */
            si (res.radix->datum.conversio.typus != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.conversio.typus->genus,
                                   (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_VERUM(res.radix->datum.conversio.typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.conversio.typus->datum.declarator.pointer_levels), (i32)II);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (void*)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(void*)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            si (res.radix->datum.conversio.typus != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.conversio.typus->genus,
                                   (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_VERUM(res.radix->datum.conversio.typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.conversio.typus->datum.declarator.pointer_levels), (i32)I);
            }
        }
    }


    /* ========================================================
     * PROBARE: sizeof(type) (Phase 1.1)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(int) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
            si (res.radix->datum.sizeof_expr.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.operandum->genus,
                                   (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_VERUM(res.radix->datum.sizeof_expr.operandum->datum.declarator.pointer_levels == NIHIL);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(int*) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(int*)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
            si (res.radix->datum.sizeof_expr.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.operandum->genus,
                                   (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_VERUM(res.radix->datum.sizeof_expr.operandum->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.sizeof_expr.operandum->datum.declarator.pointer_levels), (i32)I);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(char**) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(char**)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
            si (res.radix->datum.sizeof_expr.operandum != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.operandum->genus,
                                   (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_VERUM(res.radix->datum.sizeof_expr.operandum->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.sizeof_expr.operandum->datum.declarator.pointer_levels), (i32)II);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(void*) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(void*)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }


    /* ========================================================
     * PROBARE: Identifier (typedef) casts - Phase 1.1b
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (TypeName)x - ID cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(TypeName)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            /* GLR fork produces AMBIGUUS (both cast and paren-expr interpretations)
             * OR CONVERSIO if cast path wins */
            CREDO_VERUM((i32)res.radix->genus == (i32)ARBOR2_NODUS_CONVERSIO ||
                        (i32)res.radix->genus == (i32)ARBOR2_NODUS_AMBIGUUS);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (TypeName*)ptr - ID pointer cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(TypeName*)ptr");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            /* Unambiguous - pointer type can only be a cast */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            si (res.radix->datum.conversio.typus != NIHIL)
            {
                /* Check pointer depth = 1 */
                CREDO_VERUM(res.radix->datum.conversio.typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.conversio.typus->datum.declarator.pointer_levels), (i32)I);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (TypeName**)pp - ID double pointer cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(TypeName**)pp");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            /* Unambiguous - double pointer type can only be a cast */
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            si (res.radix->datum.conversio.typus != NIHIL)
            {
                /* Check pointer depth = 2 */
                CREDO_VERUM(res.radix->datum.conversio.typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.conversio.typus->datum.declarator.pointer_levels), (i32)II);
            }
        }
    }


    /* ========================================================
     * PROBARE: struct/union/enum casts (Phase 1.1b Part 2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (struct foo)x - struct cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(struct foo)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (struct foo*)ptr - struct pointer cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(struct foo*)ptr");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            si (res.radix->datum.conversio.typus != NIHIL)
            {
                CREDO_VERUM(res.radix->datum.conversio.typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.conversio.typus->datum.declarator.pointer_levels), (i32)I);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (struct foo**)pp - struct double pointer cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(struct foo**)pp");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            si (res.radix->datum.conversio.typus != NIHIL)
            {
                CREDO_VERUM(res.radix->datum.conversio.typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.conversio.typus->datum.declarator.pointer_levels), (i32)II);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (union bar)u - union cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(union bar)u");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (union bar*)ptr - union pointer cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(union bar*)ptr");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            si (res.radix->datum.conversio.typus != NIHIL)
            {
                CREDO_VERUM(res.radix->datum.conversio.typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.conversio.typus->datum.declarator.pointer_levels), (i32)I);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (enum color)c - enum cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(enum color)c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (enum color*)ptr - enum pointer cast ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(enum color*)ptr");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            si (res.radix->datum.conversio.typus != NIHIL)
            {
                CREDO_VERUM(res.radix->datum.conversio.typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(res.radix->datum.conversio.typus->datum.declarator.pointer_levels), (i32)I);
            }
        }
    }

    /* ========================================================
     * PROBARE: sizeof(struct/union/enum) (Phase 1.1b Part 2)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(struct foo) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(struct foo)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(struct foo*) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(struct foo*)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(struct foo**) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(struct foo**)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(union bar) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(union bar)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(union bar*) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(union bar*)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(enum color) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(enum color)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(enum color*) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(enum color*)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    /* ========================================================
     * PROBARE: sizeof(type[N]) - array types in sizeof
     * Phase 1.1c
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(int[10]) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(int[10])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
            /* Operandum should be a declarator with dimensions */
            si (res.radix->datum.sizeof_expr.operandum != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.sizeof_expr.operandum;
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                CREDO_AEQUALIS_I32(xar_numerus(decl->datum.declarator.dimensiones), I);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(char[256]) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(char[256])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(void[1]) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(void[1])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(int[10+5]) with expression ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(int[10+5])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            si (res.radix->datum.sizeof_expr.operandum != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.sizeof_expr.operandum;
                /* The dimension should be a BINARIUM expression */
                si (decl->datum.declarator.dimensiones != NIHIL)
                {
                    Arbor2Nodus** dim = xar_obtinere(decl->datum.declarator.dimensiones, ZEPHYRUM);
                    CREDO_NON_NIHIL(dim);
                    CREDO_NON_NIHIL(*dim);
                    CREDO_AEQUALIS_I32((i32)(*dim)->genus, (i32)ARBOR2_NODUS_BINARIUM);
                }
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans x = sizeof(int[10]) in assignment ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "x = sizeof(int[10])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            /* RHS should be sizeof */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus, (i32)ARBOR2_NODUS_SIZEOF);
            }
        }
    }

    /* ========================================================
     * PROBARE: sizeof(type[N][M]) - multi-dimensional array types
     * Phase 1.1d
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(int[10][20]) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(int[10][20])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
            /* Operandum should be a declarator with 2 dimensions */
            si (res.radix->datum.sizeof_expr.operandum != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.sizeof_expr.operandum;
                CREDO_AEQUALIS_I32((i32)decl->genus, (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_NON_NIHIL(decl->datum.declarator.dimensiones);
                CREDO_AEQUALIS_I32(xar_numerus(decl->datum.declarator.dimensiones), II);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(char[256][4]) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(char[256][4])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(void[1][1]) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(void[1][1])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(MyType[5][10]) with ID ---\n");

        /* Note: For ID types, the parser can't distinguish between:
         * - sizeof(variable[index1][index2]) - array subscript expression
         * - sizeof(Type[dim1][dim2]) - array type size
         * Without type information, it defaults to parsing as expression.
         * So est_typus will be FALSUM (sizeof expr, not sizeof type). */
        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(MyType[5][10])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            /* Not checking est_typus - parser treats ID as expression */
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(int[N][M]) with expressions ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(int[N][M])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            /* Both dimensions should be identifier expressions */
            si (res.radix->datum.sizeof_expr.operandum != NIHIL)
            {
                Arbor2Nodus* decl = res.radix->datum.sizeof_expr.operandum;
                si (decl->datum.declarator.dimensiones != NIHIL)
                {
                    Arbor2Nodus** dim1 = xar_obtinere(decl->datum.declarator.dimensiones, ZEPHYRUM);
                    Arbor2Nodus** dim2 = xar_obtinere(decl->datum.declarator.dimensiones, I);
                    CREDO_NON_NIHIL(dim1);
                    CREDO_NON_NIHIL(dim2);
                    CREDO_NON_NIHIL(*dim1);
                    CREDO_NON_NIHIL(*dim2);
                    CREDO_AEQUALIS_I32((i32)(*dim1)->genus, (i32)ARBOR2_NODUS_IDENTIFICATOR);
                    CREDO_AEQUALIS_I32((i32)(*dim2)->genus, (i32)ARBOR2_NODUS_IDENTIFICATOR);
                }
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans x = sizeof(int[10][20]) in assignment ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "x = sizeof(int[10][20])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            /* RHS should be sizeof */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus, (i32)ARBOR2_NODUS_SIZEOF);
            }
        }
    }


    /* ========================================================
     * PROBARE: sizeof(type*[N]) - pointer array types
     * Phase 1.1e
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(int*[10]) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(int*[10])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
            si (res.radix->datum.sizeof_expr.operandum != NIHIL)
            {
                Arbor2Nodus* typus = res.radix->datum.sizeof_expr.operandum;
                CREDO_AEQUALIS_I32((i32)typus->genus, (i32)ARBOR2_NODUS_DECLARATOR);
                CREDO_VERUM(typus->datum.declarator.pointer_levels != NIHIL);
                CREDO_AEQUALIS_I32((i32)xar_numerus(typus->datum.declarator.pointer_levels), (i32)I);  /* one pointer */
                CREDO_NON_NIHIL(typus->datum.declarator.dimensiones);
                si (typus->datum.declarator.dimensiones != NIHIL)
                {
                    CREDO_AEQUALIS_I32(xar_numerus(typus->datum.declarator.dimensiones), I);  /* one dimension */
                }
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(char*[256]) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(char*[256])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(void*[5]) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(void*[5])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(MyType*[20]) with ID ---\n");

        /* Note: sizeof(ID*[N]) doesn't work because the ID path in state 388
         * is disabled to avoid ambiguity. The parser can't distinguish between
         * sizeof(type) and sizeof(expr) for identifier types without type info.
         * This test verifies the parser fails gracefully (known limitation). */
        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(MyType*[20])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        /* Expected to fail - ID path disabled */
        CREDO_AEQUALIS_I32((i32)res.successus, FALSUM);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans x = sizeof(int*[10]) in assignment ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "x = sizeof(int*[10])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
            /* RHS should be sizeof */
            si (res.radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.binarium.dexter->genus, (i32)ARBOR2_NODUS_SIZEOF);
            }
        }
    }

    /* ========================================================
     * PROBARE: sizeof with type modifiers
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(unsigned int) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(signed int) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(long int) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(long int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(short int) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(short int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(unsigned char) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned char)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(signed char) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed char)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(unsigned long int) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned long int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(unsigned short int) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned short int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(signed long int) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed long int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(signed short int) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed short int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    /* sizeof implicit int variants */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(unsigned) implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(long) implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(long)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(short) implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(short)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(unsigned long) implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned long)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(unsigned short) implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned short)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(signed long) implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed long)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(signed short) implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed short)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }

    /* ========================================================
     * PROBARE: Cast with type modifiers
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned int)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed int)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (long int)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(long int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (short int)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(short int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned char)c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned char)c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed char)c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed char)c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned long int)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned short int)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned short int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed long int)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed long int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed short int)x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed short int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    /* Implicit int casts */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned)x implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (long)x implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(long)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (short)x implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(short)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned long)x implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned short)x implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned short)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed long)x implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed long)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed short)x implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed short)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    /* ========================================================
     * PROBARE: Pointer casts with type modifiers
     * ======================================================== */

    /* Explicit type single pointer casts */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned int *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed int *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned char *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned char *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed char *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed char *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (long int *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(long int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (short int *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(short int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned long int *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned short int *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned short int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed long int *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed long int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed short int *)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed short int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    /* Explicit type double pointer casts */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned int **)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned int **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (long int **)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(long int **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned char **)p ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned char **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    /* Implicit int single pointer casts */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned *)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (long *)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(long *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (short *)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(short *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned long *)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned short *)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned short *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed long *)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed long *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (signed short *)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(signed short *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    /* Implicit int double pointer casts */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned **)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (long **)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(long **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (unsigned long **)p implicit int ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }


    /* ========================================================
     * PROBARE: Storage class specifiers
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static int x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "static int x");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_NONE);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static float f ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "static float f");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern int y ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "extern int y");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_EXTERN);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const int z ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "const int z");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_NONE);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans volatile int w ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "volatile int w");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_VOLATILE);
        }
    }

    /* ========================================================
     * PROBARE: Specifier combinations (Phase 1.4)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static const int x ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "static const int x");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern const int y ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "extern const int y");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_EXTERN);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const volatile int z ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "const volatile int z");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_NONE);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST | ARBOR2_QUAL_VOLATILE);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans volatile const int w ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "volatile const int w");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_NONE);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST | ARBOR2_QUAL_VOLATILE);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static volatile int sv ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "static volatile int sv");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_VOLATILE);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static const int x = 5 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "static const int x = 5");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern const int y = 10 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "extern const int y = 10");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_EXTERN);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static const int *ptr ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "static const int *ptr");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST);
        }
    }

    /* ========================================================
     * PROBARE: Initializer declarations (Phase 1.2a)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int x = 5 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = 5");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.declarator);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INTEGER);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int y = a + b ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int y = a + b");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                /* Initializer should be binary + operation */
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_BINARIUM);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int z = x * 2 + 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int z = x * 2 + 1");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
        }
    }

    /* Storage class/qualifier initializer tests */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static int s = 10 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "static int s = 10");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern int e = 20 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "extern int e = 20");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_EXTERN);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans register int r = 30 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "register int r = 30");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_REGISTER);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans auto int a = 40 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "auto int a = 40");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_AUTO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const int c = 50 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "const int c = 50");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans volatile int v = 60 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "volatile int v = 60");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_VOLATILE);
        }
    }

    /* Test with expression initializer and storage class */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static int sum = a + b ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "static int sum = a + b");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_BINARIUM);
            }
        }
    }

    /* ========================================================
     * PROBARE: Brace initializers (Phase 1.2b)
     * ======================================================== */

    /* Basic brace initializer with single value */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int a[] = {1} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int a[] = {1}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor->datum.initializor_lista.items);
                si (res.radix->datum.declaratio.initializor->datum.initializor_lista.items != NIHIL)
                {
                    Arbor2Nodus* items = res.radix->datum.declaratio.initializor->datum.initializor_lista.items;
                    CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 1);
                }
            }
        }
    }

    /* Brace initializer with multiple values */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int b[] = {1, 2, 3} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int b[] = {1, 2, 3}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor->datum.initializor_lista.items);
                si (res.radix->datum.declaratio.initializor->datum.initializor_lista.items != NIHIL)
                {
                    Arbor2Nodus* items = res.radix->datum.declaratio.initializor->datum.initializor_lista.items;
                    CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 3);
                }
            }
        }
    }

    /* Empty brace initializer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int c[] = {} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int c[] = {}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor->datum.initializor_lista.items);
                si (res.radix->datum.declaratio.initializor->datum.initializor_lista.items != NIHIL)
                {
                    Arbor2Nodus* items = res.radix->datum.declaratio.initializor->datum.initializor_lista.items;
                    CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 0);
                }
            }
        }
    }

    /* Trailing comma */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int d[] = {1, 2,} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int d[] = {1, 2,}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                si (res.radix->datum.declaratio.initializor->datum.initializor_lista.items != NIHIL)
                {
                    Arbor2Nodus* items = res.radix->datum.declaratio.initializor->datum.initializor_lista.items;
                    CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 2);
                }
            }
        }
    }

    /* Nested braces */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int m[2][2] = {{1, 2}, {3, 4}} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int m[2][2] = {{1, 2}, {3, 4}}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                si (res.radix->datum.declaratio.initializor->datum.initializor_lista.items != NIHIL)
                {
                    Arbor2Nodus* items = res.radix->datum.declaratio.initializor->datum.initializor_lista.items;
                    CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    /* Outer list has 2 items (nested lists) */
                    CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 2);
                    /* First nested list */
                    {
                        Arbor2Nodus** first_item = xar_obtinere(items->datum.lista_separata.elementa, 0);
                        CREDO_NON_NIHIL(first_item);
                        si (first_item != NIHIL)
                        {
                            Arbor2Nodus* nested = *first_item;
                            Arbor2Nodus* nested_items;
                            CREDO_AEQUALIS_I32((i32)nested->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                            nested_items = nested->datum.initializor_lista.items;
                            CREDO_AEQUALIS_I32((i32)nested_items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                            CREDO_AEQUALIS_I32((i32)xar_numerus(nested_items->datum.lista_separata.elementa), 2);
                        }
                    }
                }
            }
        }
    }

    /* Static with brace initializer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static int s[] = {10, 20} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "static int s[] = {10, 20}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            }
        }
    }

    /* Const with brace initializer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const int c[] = {30, 40} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "const int c[] = {30, 40}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_CONST);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            }
        }
    }

    /* Single value in braces (not array) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int x = {5} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {5}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                si (res.radix->datum.declaratio.initializor->datum.initializor_lista.items != NIHIL)
                {
                    Arbor2Nodus* items = res.radix->datum.declaratio.initializor->datum.initializor_lista.items;
                    CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 1);
                }
            }
        }
    }

    /* Expression initializers still work */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int y = 42 (still works) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int y = 42");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                /* Expression initializer, not brace initializer */
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INTEGER);
            }
        }
    }


    /* ========================================================
     * PROBARE: Designated initializers (Phase 1.2c)
     * ======================================================== */

    /* Field designator: .x = 1 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus** item;

        imprimere("\n--- Probans int x = {.a = 5} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {.a = 5}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                si (res.radix->datum.declaratio.initializor->datum.initializor_lista.items != NIHIL)
                {
                    Arbor2Nodus* items = res.radix->datum.declaratio.initializor->datum.initializor_lista.items;
                    CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
                    CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 1);
                    /* First item should be a DESIGNATOR_ITEM */
                    item = xar_obtinere(items->datum.lista_separata.elementa, ZEPHYRUM);
                    si (item != NIHIL && *item != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)(*item)->genus, (i32)ARBOR2_NODUS_DESIGNATOR_ITEM);
                    }
                }
            }
        }
    }

    /* Array designator: [5] = 100 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int x = {[5] = 100} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {[5] = 100}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
            si (res.radix->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)res.radix->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            }
        }
    }

    /* Multiple field designators */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;

        imprimere("\n--- Probans int x = {.a = 1, .b = 2} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {.a = 1, .b = 2}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL && res.radix->datum.declaratio.initializor != NIHIL)
        {
            Arbor2Nodus* items;
            init = res.radix->datum.declaratio.initializor;
            CREDO_AEQUALIS_I32((i32)init->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            items = init->datum.initializor_lista.items;
            CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
            CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 2);
        }
    }

    /* Chained designators: [0].x = 5 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;
        Arbor2Nodus** item;

        imprimere("\n--- Probans int x = {[0].y = 5} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {[0].y = 5}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL && res.radix->datum.declaratio.initializor != NIHIL)
        {
            Arbor2Nodus* items;
            init = res.radix->datum.declaratio.initializor;
            CREDO_AEQUALIS_I32((i32)init->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            items = init->datum.initializor_lista.items;
            CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
            /* Should have 1 item with 2 designators chained */
            item = xar_obtinere(items->datum.lista_separata.elementa, ZEPHYRUM);
            si (item != NIHIL && *item != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)(*item)->genus, (i32)ARBOR2_NODUS_DESIGNATOR_ITEM);
                CREDO_AEQUALIS_I32((i32)xar_numerus((*item)->datum.designator_item.designatores), 2);
            }
        }
    }

    /* Mixed positional and designated */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;

        imprimere("\n--- Probans int x = {1, .b = 2, 3} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {1, .b = 2, 3}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL && res.radix->datum.declaratio.initializor != NIHIL)
        {
            Arbor2Nodus* items;
            init = res.radix->datum.declaratio.initializor;
            CREDO_AEQUALIS_I32((i32)init->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            items = init->datum.initializor_lista.items;
            CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
            CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 3);
        }
    }

    /* Designator with nested brace value */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;
        Arbor2Nodus** item;

        imprimere("\n--- Probans int x = {.a = {1, 2}} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {.a = {1, 2}}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL && res.radix->datum.declaratio.initializor != NIHIL)
        {
            Arbor2Nodus* items;
            init = res.radix->datum.declaratio.initializor;
            CREDO_AEQUALIS_I32((i32)init->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            items = init->datum.initializor_lista.items;
            CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
            item = xar_obtinere(items->datum.lista_separata.elementa, ZEPHYRUM);
            si (item != NIHIL && *item != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)(*item)->genus, (i32)ARBOR2_NODUS_DESIGNATOR_ITEM);
                /* Value should be an INITIALIZOR_LISTA */
                CREDO_NON_NIHIL((*item)->datum.designator_item.valor);
                si ((*item)->datum.designator_item.valor != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)(*item)->datum.designator_item.valor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                }
            }
        }
    }

    /* Multiple array designators */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;

        imprimere("\n--- Probans int x = {[0] = 1, [5] = 6} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {[0] = 1, [5] = 6}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL && res.radix->datum.declaratio.initializor != NIHIL)
        {
            Arbor2Nodus* items;
            init = res.radix->datum.declaratio.initializor;
            CREDO_AEQUALIS_I32((i32)init->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            items = init->datum.initializor_lista.items;
            CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
            CREDO_AEQUALIS_I32((i32)xar_numerus(items->datum.lista_separata.elementa), 2);
        }
    }

    /* Multi-dimensional array designator: [0][1] = 5 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;
        Arbor2Nodus** item;

        imprimere("\n--- Probans int x = {[0][1] = 5} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {[0][1] = 5}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL && res.radix->datum.declaratio.initializor != NIHIL)
        {
            Arbor2Nodus* items;
            init = res.radix->datum.declaratio.initializor;
            CREDO_AEQUALIS_I32((i32)init->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            items = init->datum.initializor_lista.items;
            CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
            item = xar_obtinere(items->datum.lista_separata.elementa, ZEPHYRUM);
            si (item != NIHIL && *item != NIHIL)
            {
                /* Should have 2 designators chained */
                CREDO_AEQUALIS_I32((i32)xar_numerus((*item)->datum.designator_item.designatores), 2);
            }
        }
    }

    /* Field chain: .a.b = 5 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;
        Arbor2Nodus** item;

        imprimere("\n--- Probans int x = {.a.b = 5} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {.a.b = 5}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL && res.radix->datum.declaratio.initializor != NIHIL)
        {
            Arbor2Nodus* items;
            init = res.radix->datum.declaratio.initializor;
            CREDO_AEQUALIS_I32((i32)init->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            items = init->datum.initializor_lista.items;
            CREDO_AEQUALIS_I32((i32)items->genus, (i32)ARBOR2_NODUS_LISTA_SEPARATA);
            item = xar_obtinere(items->datum.lista_separata.elementa, ZEPHYRUM);
            si (item != NIHIL && *item != NIHIL)
            {
                /* Should have 2 designators chained */
                CREDO_AEQUALIS_I32((i32)xar_numerus((*item)->datum.designator_item.designatores), 2);
            }
        }
    }

    /* Designator with trailing comma */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int x = {.a = 1,} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {.a = 1,}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
    }


    /* ========================================================
     * PROBARE: Multi-declarator (comma-separated)
     * ======================================================== */

    /* Simple multi-declarator: int x, y */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

        imprimere("\n--- Probans int x, y ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x, y");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            first = res.radix;
            CREDO_AEQUALIS_I32((i32)first->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(first->datum.declaratio.declarator);
            CREDO_NIHIL(first->datum.declaratio.initializor);

            /* Check second declaration via proxima */
            second = first->datum.declaratio.proxima;
            CREDO_NON_NIHIL(second);
            si (second != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)second->genus, (i32)ARBOR2_NODUS_DECLARATIO);
                CREDO_NON_NIHIL(second->datum.declaratio.declarator);
                CREDO_NIHIL(second->datum.declaratio.initializor);
                CREDO_NIHIL(second->datum.declaratio.proxima);  /* No third */
            }
        }
    }

    /* Multi-declarator with initializers: int x = 1, y = 2 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

        imprimere("\n--- Probans int x = 1, y = 2 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = 1, y = 2");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            first = res.radix;
            CREDO_AEQUALIS_I32((i32)first->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(first->datum.declaratio.initializor);
            si (first->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)first->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INTEGER);
            }

            second = first->datum.declaratio.proxima;
            CREDO_NON_NIHIL(second);
            si (second != NIHIL)
            {
                CREDO_NON_NIHIL(second->datum.declaratio.initializor);
                si (second->datum.declaratio.initializor != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)second->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INTEGER);
                }
                CREDO_NIHIL(second->datum.declaratio.proxima);
            }
        }
    }

    /* Three declarators: int x, y, z */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;
        Arbor2Nodus* third;

        imprimere("\n--- Probans int x, y, z ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x, y, z");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            first = res.radix;
            CREDO_AEQUALIS_I32((i32)first->genus, (i32)ARBOR2_NODUS_DECLARATIO);

            second = first->datum.declaratio.proxima;
            CREDO_NON_NIHIL(second);
            si (second != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)second->genus, (i32)ARBOR2_NODUS_DECLARATIO);

                third = second->datum.declaratio.proxima;
                CREDO_NON_NIHIL(third);
                si (third != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)third->genus, (i32)ARBOR2_NODUS_DECLARATIO);
                    CREDO_NIHIL(third->datum.declaratio.proxima);  /* No fourth */
                }
            }
        }
    }

    /* Mixed: int x = 1, y (some with init, some without) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

        imprimere("\n--- Probans int x = 1, y ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int x = 1, y");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            first = res.radix;
            CREDO_NON_NIHIL(first->datum.declaratio.initializor);  /* x has init */

            second = first->datum.declaratio.proxima;
            CREDO_NON_NIHIL(second);
            si (second != NIHIL)
            {
                CREDO_NIHIL(second->datum.declaratio.initializor);  /* y has no init */
            }
        }
    }

    /* Struct type multi-declarator: struct foo x, y */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

        imprimere("\n--- Probans struct foo x, y ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo x, y");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            first = res.radix;
            CREDO_AEQUALIS_I32((i32)first->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(first->datum.declaratio.specifier);
            si (first->datum.declaratio.specifier != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)first->datum.declaratio.specifier->genus, (i32)ARBOR2_NODUS_STRUCT_SPECIFIER);
            }

            second = first->datum.declaratio.proxima;
            CREDO_NON_NIHIL(second);
            si (second != NIHIL)
            {
                /* Both should share the same type specifier */
                CREDO_AEQUALIS_PTR(second->datum.declaratio.specifier, first->datum.declaratio.specifier);
            }
        }
    }

    /* Multi-declarator with brace initializers */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

        imprimere("\n--- Probans int a[] = {1}, b[] = {2} ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "int a[] = {1}, b[] = {2}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            first = res.radix;
            CREDO_NON_NIHIL(first->datum.declaratio.initializor);
            si (first->datum.declaratio.initializor != NIHIL)
            {
                CREDO_AEQUALIS_I32((i32)first->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
            }

            second = first->datum.declaratio.proxima;
            CREDO_NON_NIHIL(second);
            si (second != NIHIL)
            {
                CREDO_NON_NIHIL(second->datum.declaratio.initializor);
                si (second->datum.declaratio.initializor != NIHIL)
                {
                    CREDO_AEQUALIS_I32((i32)second->datum.declaratio.initializor->genus, (i32)ARBOR2_NODUS_INITIALIZOR_LISTA);
                }
            }
        }
    }


    /* ========================================================
     * PROBARE: Translation unit - multiple declarations
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Translation unit: two declarations ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int x; int y;");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
            CREDO_AEQUALIS_I32(
                xar_numerus(res.radix->datum.translation_unit.declarationes), II);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Translation unit: three declarations ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int x; int y; int z;");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
            CREDO_AEQUALIS_I32(
                xar_numerus(res.radix->datum.translation_unit.declarationes), III);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Translation unit: declaration with init ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int x = 5; int y = 10;");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
            CREDO_AEQUALIS_I32(
                xar_numerus(res.radix->datum.translation_unit.declarationes), II);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Translation unit: function definition ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int foo() { return 1; }");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
            CREDO_AEQUALIS_I32(
                xar_numerus(res.radix->datum.translation_unit.declarationes), I);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Translation unit: declaration + function ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int x; int foo() { return x; }");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
            CREDO_AEQUALIS_I32(
                xar_numerus(res.radix->datum.translation_unit.declarationes), II);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Translation unit: multiple functions ---\n");
        tokens = _lexare_ad_tokens(piscina, intern,
            "int foo() { return 1; } int bar() { return 2; }");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
            CREDO_AEQUALIS_I32(
                xar_numerus(res.radix->datum.translation_unit.declarationes), II);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Translation unit: empty input ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
            CREDO_AEQUALIS_I32(
                xar_numerus(res.radix->datum.translation_unit.declarationes), ZEPHYRUM);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Translation unit: function with params ---\n");
        tokens = _lexare_ad_tokens(piscina, intern,
            "int add(int a, int b) { return a + b; }");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
            CREDO_AEQUALIS_I32(
                xar_numerus(res.radix->datum.translation_unit.declarationes), I);
        }
    }

    /* Function with mixed type params: int and float */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Translation unit: function with int and float params ---\n");
        tokens = _lexare_ad_tokens(piscina, intern,
            "float scale(int x, float factor) { return x * factor; }");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }


    /* ========================================================
     * PROBARE: Typedef registration post-parse
     *
     * After parsing a translation unit with typedef declarations,
     * the typedef names should be registered in the expansion context.
     * This enables subsequent declarations using the typedef'd names.
     * ======================================================== */

    /* Test: typedef registration - simple typedef */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        Arbor2Nodus* tu;
        Arbor2Nodus* first_decl;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Typedef registration: typedef int Integer; ---\n");

        /* Create fresh GLR with clean expansion context for this test */
        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern, "typedef int Integer;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                tu = res.radix;
                CREDO_AEQUALIS_I32((i32)tu->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
                CREDO_AEQUALIS_I32(xar_numerus(tu->datum.translation_unit.declarationes), I);

                /* Verify first declaration is a typedef */
                si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
                {
                    Arbor2Nodus** ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                    first_decl = *ptr;
                    CREDO_NON_NIHIL(first_decl);
                    si (first_decl != NIHIL)
                    {
                        CREDO_AEQUALIS_I32((i32)first_decl->genus, (i32)ARBOR2_NODUS_DECLARATIO);
                        CREDO_VERUM(first_decl->datum.declaratio.est_typedef);
                    }
                }

                /* Verify Integer is registered as typedef */
                u.c = "Integer";
                titulus_ch.datum = u.m;
                titulus_ch.mensura = VII;
                CREDO_VERUM(arbor2_expansion_est_typedef(exp_test, titulus_ch));
            }
        }
    }

    /* Test: pointer typedef registration */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Typedef registration: typedef int *IntPtr; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern, "typedef int *IntPtr;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);

            /* Verify IntPtr is registered as typedef */
            u.c = "IntPtr";
            titulus_ch.datum = u.m;
            titulus_ch.mensura = VI;
            CREDO_VERUM(arbor2_expansion_est_typedef(exp_test, titulus_ch));
        }
    }

    /* Test: struct typedef registration */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Typedef registration: typedef struct { int x; } Point; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef struct { int x; } Point;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.translation_unit.declarationes), I);
            }

            /* Verify Point is registered as typedef */
            u.c = "Point";
            titulus_ch.datum = u.m;
            titulus_ch.mensura = V;
            CREDO_VERUM(arbor2_expansion_est_typedef(exp_test, titulus_ch));
        }
    }

    /* Test: enum typedef registration */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Typedef registration: typedef enum { RED, GREEN, BLUE } Color; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef enum { RED, GREEN, BLUE } Color;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                CREDO_AEQUALIS_I32(xar_numerus(res.radix->datum.translation_unit.declarationes), I);
            }

            /* Verify Color is registered as typedef */
            u.c = "Color";
            titulus_ch.datum = u.m;
            titulus_ch.mensura = V;
            CREDO_VERUM(arbor2_expansion_est_typedef(exp_test, titulus_ch));
        }
    }


    /* ========================================================
     * PROBARE: Type modifiers (Phase 1.4)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans unsigned int x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "unsigned int x");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans long int y ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "long int y");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans unsigned long z (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "unsigned long z");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans short s (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "short s");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans signed char c ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "signed char c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans unsigned short int u ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "unsigned short int u");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans signed long int w ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "signed long int w");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans unsigned char uc ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "unsigned char uc");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }

    /* ========================================================
     * PROBARE: Phase 1.4b - Storage class + type modifiers
     * ======================================================== */

    /* static + type modifiers */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static unsigned int x ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned int x");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static long int y ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static long int y");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static unsigned long int z ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned long int z");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static short int s ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static short int s");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static signed char c ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static signed char c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static unsigned int u ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned int u");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* extern + type modifiers */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern unsigned int a ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "extern unsigned int a");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern long int b ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "extern long int b");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern unsigned short int c ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "extern unsigned short int c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* register + type modifiers */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans register short int d ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "register short int d");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans register unsigned long int e ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "register unsigned long int e");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans register signed int f ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "register signed int f");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* auto + type modifiers */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans auto signed char g ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "auto signed char g");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans auto long int h ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "auto long int h");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans auto unsigned short int i ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "auto unsigned short int i");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* Complex combinations */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static unsigned long int j ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned long int j");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern signed short int k ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "extern signed short int k");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans register unsigned char l ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "register unsigned char l");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans auto signed long int m ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "auto signed long int m");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* ========================================================
     * PROBARE: Phase 1.4c - Implicit int with storage class + type modifiers
     * ======================================================== */

    /* static + implicit int */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static unsigned x (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned x");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static long y (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static long y");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static short z (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static short z");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static signed a (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static signed a");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static unsigned long b (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned long b");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* extern + implicit int */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern short c (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "extern short c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern unsigned long d (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "extern unsigned long d");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* register + implicit int */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans register signed e (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "register signed e");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans register long f (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "register long f");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* auto + implicit int */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans auto unsigned short g (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "auto unsigned short g");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans auto signed long h (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "auto signed long h");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* with initializer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static unsigned i = 5 (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned i = 5");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern long j = 10 (implicit int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "extern long j = 10");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }


    /* ========================================================
     * PROBARE: Phase 1.4d - Pointer qualifiers
     * ======================================================== */

    /* int * const p - const pointer to int */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int * const p (pointer qualifier) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int * const p");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int * volatile v - volatile pointer to int */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int * volatile v (pointer qualifier) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int * volatile v");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int * const volatile cv - const volatile pointer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int * const volatile cv (pointer qualifier) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int * const volatile cv");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int * volatile const vc - volatile const pointer (order variant) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int * volatile const vc (pointer qualifier) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int * volatile const vc");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* char ** const pp - const pointer to pointer to char */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans char ** const pp (pointer qualifier) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "char ** const pp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int * const * volatile pp - multiple qualified pointers */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int * const * volatile pp (multi qualifier) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int * const * volatile pp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* const int * const cp - const pointer to const int */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const int * const cp (type + pointer qualifier) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "const int * const cp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }


    /* ========================================================
     * PROBARE: Qualifier + type modifier combinations
     * ======================================================== */

    /* const unsigned int x */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const unsigned int x  ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "const unsigned int x");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* const long int y */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const long int y  ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "const long int y");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* const short int z */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const short int z  ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "const short int z");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* volatile unsigned int a */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans volatile unsigned int a  ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "volatile unsigned int a");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* volatile long int b */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans volatile long int b  ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "volatile long int b");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* const signed char c */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans const signed char c  ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "const signed char c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* static const unsigned int d (storage + qualifier + type modifier) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans static const unsigned int d  ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "static const unsigned int d");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* extern volatile long int e (storage + qualifier + type modifier) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans extern volatile long int e  ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "extern volatile long int e");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }


    /* ========================================================
     * PROBARE: void pointer declarations
     * ======================================================== */

    /* void *p - pointer to void */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans void *p (void pointer) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "void *p");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* void **pp - pointer to pointer to void */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans void **pp (void double pointer) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "void **pp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* void * const vp - const pointer to void */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans void * const vp (const void pointer) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "void * const vp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }


    /* ========================================================
     * PROBARE: Floating point types
     * ======================================================== */

    /* float f - basic float declaration */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans float f (float declaration) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "float f");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* double d - basic double declaration */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans double d (double declaration) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "double d");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* long double ld - long double declaration */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans long double ld (long double declaration) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "long double ld");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* float *fp - pointer to float */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans float *fp (float pointer) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "float *fp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* double *dp - pointer to double */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans double *dp (double pointer) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "double *dp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* float arr[10] - float array */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans float arr[10] (float array) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "float arr[10]");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* double arr[10] - double array */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans double arr[10] (double array) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "double arr[10]");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }


    /* ========================================================
     * PROBARE: Float/Double casts
     * ======================================================== */

    /* (float)x - cast to float */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (float)x (cast to float) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "(float)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* (double)x - cast to double */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (double)x (cast to double) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "(double)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* (long double)x - cast to long double */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (long double)x (cast to long double) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "(long double)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* (float *)p - pointer cast to float */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (float *)p (pointer cast to float) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "(float *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* (double *)p - pointer cast to double */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans (double *)p (pointer cast to double) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "(double *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }


    /* ========================================================
     * PROBARE: Float/Double sizeof
     * ======================================================== */

    /* sizeof(float) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(float) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(float)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* sizeof(double) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(double) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(double)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* sizeof(long double) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(long double) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(long double)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* sizeof(float *) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(float *) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(float *)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* sizeof(double *) */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans sizeof(double *) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(double *)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }


    /* ========================================================
     * PROBARE: Function pointers
     * ======================================================== */

    /* int (*fp)(void) - function pointer returning int, no params */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int (*fp)(void) (function pointer) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int (*fp)(void)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int (*fp)(int) - function pointer returning int, int param */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int (*fp)(int) (function pointer with param) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int (*fp)(int)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int (*fp)(int, int) - function pointer with multiple params */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int (*fp)(int, int) (function pointer multi params) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int (*fp)(int, int)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* void (*fp)(void) - void function pointer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans void (*fp)(void) (void function pointer) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "void (*fp)(void)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int (**fpp)(int) - pointer to function pointer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int (**fpp)(int) (pointer to function pointer) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int (**fpp)(int)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }


    /* ========================================================
     * PROBARE: Array parameters
     * ======================================================== */

    /* int fn(int arr[]) - function with array parameter */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int fn(int arr[]) (array parameter) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int fn(int arr[])");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int fn(int arr[10]) - function with sized array parameter */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int fn(int arr[10]) (sized array parameter) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int fn(int arr[10])");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* void fn(char *argv[]) - function with pointer array parameter */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans void fn(char *argv[]) (pointer array parameter) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "void fn(char *argv[])");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* ========================================================
     * PROBARE: Variadic functions (ellipsis)
     * ======================================================== */

    /* int printf(char *fmt, ...) - variadic function */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int printf(char *fmt, ...) (variadic function) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int printf(char *fmt, ...)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int foo(int x, ...) - variadic with int param */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int foo(int x, ...) (variadic with int) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int foo(int x, ...)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* ========================================================
     * PROBARE: Const qualifier in parameters
     * ======================================================== */

    /* int foo(const int x) - const value parameter */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int foo(const int x) (const value param) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int foo(const int x)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int foo(const char *s) - const pointer parameter */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int foo(const char *s) (const pointer param) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int foo(const char *s)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* void bar(int x, const int y) - mixed params with const */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans void bar(int x, const int y) (mixed const param) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "void bar(int x, const int y)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }

    /* int fn(volatile int v) - volatile parameter */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans int fn(volatile int v) (volatile param) ---\n");
        tokens = _lexare_ad_tokens(piscina, intern, "int fn(volatile int v)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }


    /* ========================================================
     * PROBARE: Table validation
     * ======================================================== */

    {
        b32 valida;

        imprimere("\n--- Table validation ---\n");
        valida = arbor2_glr_validare_tabulas();
        CREDO_VERUM(valida);

        imprimere("\n--- Tag validation ---\n");
        valida = arbor2_glr_validare_tags();
        CREDO_VERUM(valida);
    }


    /* ========================================================
     * PROBARE: Location propagation (Phase 2.1)
     * ======================================================== */

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans location on identifier ---\n");

        /* "foo" at column 1 */
        tokens = _lexare_ad_tokens(piscina, intern, "foo");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32(res.radix->linea_initium, I);
            CREDO_AEQUALIS_I32(res.radix->columna_initium, I);
            CREDO_AEQUALIS_I32(res.radix->linea_finis, I);
            /* foo = 3 characters, columna_finis = 1 + 3 = 4 */
            CREDO_AEQUALIS_I32(res.radix->columna_finis, IV);
            CREDO_AEQUALIS_I32(res.radix->layer_index, ZEPHYRUM);
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans location on binary expression ---\n");

        /* "a + b" - should span columns 1-5 */
        tokens = _lexare_ad_tokens(piscina, intern, "a + b");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL && radix->genus == ARBOR2_NODUS_BINARIUM)
        {
            CREDO_AEQUALIS_I32(radix->linea_initium, I);
            CREDO_AEQUALIS_I32(radix->columna_initium, I);  /* start at 'a' */
            CREDO_AEQUALIS_I32(radix->linea_finis, I);
            CREDO_AEQUALIS_I32(radix->columna_finis, VI);   /* end at 'b' = 5 + 1 = 6 */
            CREDO_AEQUALIS_I32(radix->layer_index, ZEPHYRUM);

            /* Check left operand (a) location */
            si (radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_I32(radix->datum.binarium.sinister->columna_initium, I);
                CREDO_AEQUALIS_I32(radix->datum.binarium.sinister->columna_finis, II);
            }
            /* Check right operand (b) location */
            si (radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_I32(radix->datum.binarium.dexter->columna_initium, V);
                CREDO_AEQUALIS_I32(radix->datum.binarium.dexter->columna_finis, VI);
            }
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans location on integer literal ---\n");

        /* "12345" at column 1, 5 digits */
        tokens = _lexare_ad_tokens(piscina, intern, "12345");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32(res.radix->genus, ARBOR2_NODUS_INTEGER);
            CREDO_AEQUALIS_I32(res.radix->linea_initium, I);
            CREDO_AEQUALIS_I32(res.radix->columna_initium, I);
            CREDO_AEQUALIS_I32(res.radix->columna_finis, VI);  /* 1 + 5 = 6 */
        }
    }

    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        imprimere("\n--- Probans location on function call ---\n");

        /* "foo()" - should span columns 1-5 */
        tokens = _lexare_ad_tokens(piscina, intern, "foo()");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL && radix->genus == ARBOR2_NODUS_VOCATIO)
        {
            CREDO_AEQUALIS_I32(radix->linea_initium, I);
            CREDO_AEQUALIS_I32(radix->columna_initium, I);  /* start at 'f' */
            CREDO_AEQUALIS_I32(radix->linea_finis, I);
            CREDO_AEQUALIS_I32(radix->columna_finis, VI);   /* end at ')' = 5 + 1 = 6 */
        }
    }


    /* ========================================================
     * PROBARE: Parent pointer (pater) on binary expression
     * ======================================================== */

    {
        Arbor2Nodus* radix;
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans pater on binary expression: a + b ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a + b");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL && radix->genus == ARBOR2_NODUS_BINARIUM)
        {
            /* Radix has no parent */
            CREDO_AEQUALIS_PTR(radix->pater, NIHIL);

            /* Left and right children point back to parent */
            si (radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.binarium.sinister->pater, radix);
            }
            si (radix->datum.binarium.dexter != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.binarium.dexter->pater, radix);
            }
        }
    }

    /* ========================================================
     * PROBARE: Parent pointer (pater) on nested expression
     * ======================================================== */

    {
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans pater on nested: a + b * c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a + b * c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL && radix->genus == ARBOR2_NODUS_BINARIUM)
        {
            /* Root is +, right child is * due to precedence */
            CREDO_AEQUALIS_PTR(radix->pater, NIHIL);

            /* 'a' points to root (+) */
            si (radix->datum.binarium.sinister != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.binarium.sinister->pater, radix);
            }

            /* The * node points to root (+) */
            dexter = radix->datum.binarium.dexter;
            si (dexter != NIHIL)
            {
                CREDO_AEQUALIS_PTR(dexter->pater, radix);

                /* b and c point to * node */
                si (dexter->genus == ARBOR2_NODUS_BINARIUM)
                {
                    si (dexter->datum.binarium.sinister != NIHIL)
                    {
                        CREDO_AEQUALIS_PTR(dexter->datum.binarium.sinister->pater, dexter);
                    }
                    si (dexter->datum.binarium.dexter != NIHIL)
                    {
                        CREDO_AEQUALIS_PTR(dexter->datum.binarium.dexter->pater, dexter);
                    }
                }
            }
        }
    }

    /* ========================================================
     * PROBARE: Parent pointer (pater) on ternary
     * ======================================================== */

    {
        Arbor2Nodus* radix;
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans pater on ternary: a ? b : c ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "a ? b : c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL && radix->genus == ARBOR2_NODUS_TERNARIUS)
        {
            CREDO_AEQUALIS_PTR(radix->pater, NIHIL);

            si (radix->datum.ternarius.conditio != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.ternarius.conditio->pater, radix);
            }
            si (radix->datum.ternarius.verum != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.ternarius.verum->pater, radix);
            }
            si (radix->datum.ternarius.falsum != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.ternarius.falsum->pater, radix);
            }
        }
    }

    /* ========================================================
     * PROBARE: Parent pointer (pater) on function call
     * ======================================================== */

    {
        Arbor2Nodus* radix;
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans pater on function call: foo(a) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "foo(a)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL && radix->genus == ARBOR2_NODUS_VOCATIO)
        {
            CREDO_AEQUALIS_PTR(radix->pater, NIHIL);

            /* basis (foo) points back to call node */
            si (radix->datum.vocatio.basis != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.vocatio.basis->pater, radix);
            }

            /* argumenta (LISTA_SEPARATA containing 'a') points back to call node */
            si (radix->datum.vocatio.argumenta != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.vocatio.argumenta->pater, radix);
            }
        }
    }

    /* ========================================================
     * PROBARE: Parent pointer (pater) on subscript
     * ======================================================== */

    {
        Arbor2Nodus* radix;
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans pater on subscript: arr[i] ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "arr[i]");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL && radix->genus == ARBOR2_NODUS_SUBSCRIPTIO)
        {
            CREDO_AEQUALIS_PTR(radix->pater, NIHIL);

            si (radix->datum.subscriptio.basis != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.subscriptio.basis->pater, radix);
            }
            si (radix->datum.subscriptio.index != NIHIL)
            {
                CREDO_AEQUALIS_PTR(radix->datum.subscriptio.index->pater, radix);
            }
        }
    }

    /* ========================================================
     * PROBARE: Latin type macro disambiguation (Phase 3.2)
     * ======================================================== */

    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans Latin type macro disambiguation ---\n");

        /* Register latina.h macros */
        arbor2_includere_latina(expansion);
        imprimere("  Latina macros registered.\n");

        /* Test: "integer x" should use type hint from macro */
        imprimere("  Testing 'integer x' with latina.h macros...\n");
        tokens = _lexare_ad_tokens(piscina, intern, "integer x");
        res = arbor2_glr_parsere(glr, tokens);
        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE: Nested macro resolution (Phase 3.2)
     * ======================================================== */

    {
        Arbor2GLRResultus res;
        Xar* tokens;

        imprimere("\n--- Probans nested macro resolution ---\n");

        /* Add i32 -> integer (which -> int via latina.h) */
        arbor2_expansion_addere_macro(expansion, "i32", "integer", NIHIL);

        /* Test: "i32 x" should follow two levels to int */
        imprimere("  Testing 'i32 x' with nested macros...\n");
        tokens = _lexare_ad_tokens(piscina, intern, "i32 x");
        res = arbor2_glr_parsere(glr, tokens);
        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
    }

    /* ========================================================
     * PROBARE: AMBIGUUS identifier tracking (Phase 3.3)
     * ======================================================== */

    {
        Arbor2GLRResultus res;
        Xar* tokens;
        Arbor2Expansion* fresh_exp;
        Arbor2GLR* fresh_glr;

        imprimere("\n--- Probans AMBIGUUS identifier tracking ---\n");

        /* Create fresh context without latina.h to get ambiguity */
        fresh_exp = arbor2_expansion_creare(piscina, intern);
        fresh_glr = arbor2_glr_creare(piscina, intern, fresh_exp);

        /* Test: "foo * bar" without type info is ambiguous */
        imprimere("  Testing 'foo * bar' (ambiguous)...\n");
        tokens = _lexare_ad_tokens(piscina, intern, "foo * bar");
        res = arbor2_glr_parsere_expressio(fresh_glr, tokens);
        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        /* Check if ambiguous and verify identificator tracking */
        si (res.radix != NIHIL && res.radix->genus == ARBOR2_NODUS_AMBIGUUS)
        {
            imprimere("  AMBIGUUS node created with %d interpretations\n",
                xar_numerus(res.radix->datum.ambiguus.interpretationes));
            CREDO_VERUM(xar_numerus(res.radix->datum.ambiguus.interpretationes) >= II);
        }
    }

    /* ========================================================
     * PROBARE: Error Recovery
     * ======================================================== */

    /* Test 1: Single invalid declaration, followed by valid one */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

        imprimere("\n--- Probans error recovery: invalid decl + valid decl ---\n");

        /* "int @ x;" is invalid, "int y;" is valid */
        tokens = _lexare_ad_tokens(piscina, intern, "int @ x; int y;");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);  /* Should succeed (with error recovery) */
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            CREDO_AEQUALIS_I32((i32)tu->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
            num_decls = xar_numerus(tu->datum.translation_unit.declarationes);
            imprimere("  num_decls: %d\n", num_decls);
            CREDO_AEQUALIS_I32(num_decls, II);  /* ERROR node + valid decl */

            si (num_decls >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* first;
                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                first = *first_ptr;
                imprimere("  first node genus: %s\n", arbor2_nodus_genus_nomen(first->genus));
                CREDO_AEQUALIS_I32((i32)first->genus, (i32)ARBOR2_NODUS_ERROR);

                /* Check ERROR node has skipped tokens */
                si (first->genus == ARBOR2_NODUS_ERROR)
                {
                    i32 num_skipped;
                    num_skipped = first->datum.error.lexemata_saltata != NIHIL ?
                        xar_numerus(first->datum.error.lexemata_saltata) : ZEPHYRUM;
                    imprimere("  skipped tokens: %d\n", num_skipped);
                    CREDO_VERUM(num_skipped > ZEPHYRUM);
                }
            }

            si (num_decls >= II)
            {
                Arbor2Nodus** second_ptr;
                Arbor2Nodus* second;
                second_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, I);
                second = *second_ptr;
                imprimere("  second node genus: %s\n", arbor2_nodus_genus_nomen(second->genus));
                CREDO_AEQUALIS_I32((i32)second->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            }
        }
    }

    /* Test 2: Multiple consecutive errors */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

        imprimere("\n--- Probans error recovery: multiple errors ---\n");

        /* All three are invalid: "@ x;", "@ y;", "@ z;" */
        tokens = _lexare_ad_tokens(piscina, intern, "@ x; @ y; @ z;");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);  /* Should succeed (with ERROR nodes) */
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            num_decls = xar_numerus(tu->datum.translation_unit.declarationes);
            imprimere("  num_decls: %d\n", num_decls);
            CREDO_AEQUALIS_I32(num_decls, III);  /* Three ERROR nodes */

            /* Verify all are ERROR nodes */
            si (num_decls >= III)
            {
                Arbor2Nodus** ptr1;
                Arbor2Nodus** ptr2;
                Arbor2Nodus** ptr3;
                ptr1 = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                ptr2 = xar_obtinere(tu->datum.translation_unit.declarationes, I);
                ptr3 = xar_obtinere(tu->datum.translation_unit.declarationes, II);
                CREDO_AEQUALIS_I32((i32)(*ptr1)->genus, (i32)ARBOR2_NODUS_ERROR);
                CREDO_AEQUALIS_I32((i32)(*ptr2)->genus, (i32)ARBOR2_NODUS_ERROR);
                CREDO_AEQUALIS_I32((i32)(*ptr3)->genus, (i32)ARBOR2_NODUS_ERROR);
            }
        }
    }

    /* Test 3: Error followed by valid function */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

        imprimere("\n--- Probans error recovery: error + valid function ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "@ badstuff; int foo() { return 0; }");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            num_decls = xar_numerus(tu->datum.translation_unit.declarationes);
            imprimere("  num_decls: %d\n", num_decls);
            CREDO_AEQUALIS_I32(num_decls, II);  /* ERROR + function */

            si (num_decls >= II)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus** second_ptr;
                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                second_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, I);
                CREDO_AEQUALIS_I32((i32)(*first_ptr)->genus, (i32)ARBOR2_NODUS_ERROR);
                CREDO_AEQUALIS_I32((i32)(*second_ptr)->genus, (i32)ARBOR2_NODUS_DEFINITIO_FUNCTI);
            }
        }
    }

    /* Test 4: Errors collection in result */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Probans error collection in result ---\n");

        tokens = _lexare_ad_tokens(piscina, intern, "@ bad;");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        imprimere("  errores: %p\n", (vacuum*)res.errores);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.errores);  /* Should have error messages */

        si (res.errores != NIHIL)
        {
            i32 num_err;
            num_err = xar_numerus(res.errores);
            imprimere("  num errors: %d\n", num_err);
            CREDO_VERUM(num_err > ZEPHYRUM);
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


    /* ========================================================
     * PROBARE: Conditional Compilation (Phase 5)
     * ======================================================== */

    /* Test 1: Simple #ifdef/#endif */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

        imprimere("\n--- Probans simple #ifdef/#endif ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef DEBUG\n"
            "int debug_level = 1;\n"
            "#endif\n"
            "int x = 5;");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            num_decls = xar_numerus(tu->datum.translation_unit.declarationes);
            imprimere("  num_decls: %d\n", num_decls);
            CREDO_AEQUALIS_I32(num_decls, II);  /* CONDITIONALIS + int x */

            si (num_decls >= II)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;
                imprimere("  first node: %s\n", arbor2_nodus_genus_nomen(cond->genus));
                CREDO_AEQUALIS_I32((i32)cond->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS)
                {
                    i32 num_rami;
                    num_rami = xar_numerus(cond->datum.conditionalis.rami);
                    imprimere("  num_rami: %d\n", num_rami);
                    CREDO_AEQUALIS_I32(num_rami, I);  /* One branch (ifdef) */
                }
            }
        }
    }

    /* Test 2: #ifdef/#else/#endif */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

        imprimere("\n--- Probans #ifdef/#else/#endif ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef DEBUG\n"
            "int level = 1;\n"
            "#else\n"
            "int level = 0;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            num_decls = xar_numerus(tu->datum.translation_unit.declarationes);
            imprimere("  num_decls: %d\n", num_decls);
            CREDO_AEQUALIS_I32(num_decls, I);  /* One CONDITIONALIS */

            si (num_decls >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;
                imprimere("  first node: %s\n", arbor2_nodus_genus_nomen(cond->genus));
                CREDO_AEQUALIS_I32((i32)cond->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS)
                {
                    i32 num_rami;
                    num_rami = xar_numerus(cond->datum.conditionalis.rami);
                    imprimere("  num_rami: %d\n", num_rami);
                    CREDO_AEQUALIS_I32(num_rami, II);  /* Two branches (ifdef + else) */
                }
            }
        }
    }

    /* Test 3: #ifndef */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

        imprimere("\n--- Probans #ifndef ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifndef HEADER_H\n"
            "int header_guard;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            num_decls = xar_numerus(tu->datum.translation_unit.declarationes);
            imprimere("  num_decls: %d\n", num_decls);
            CREDO_AEQUALIS_I32(num_decls, I);

            si (num_decls >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;
                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;
                CREDO_AEQUALIS_I32((i32)cond->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    imprimere("  ramus genus: %d (IFNDEF=%d)\n", ramus->genus, ARBOR2_DIRECTIVUM_IFNDEF);
                    CREDO_AEQUALIS_I32((i32)ramus->genus, (i32)ARBOR2_DIRECTIVUM_IFNDEF);

                    /* Check condition is captured */
                    si (ramus->conditio != NIHIL)
                    {
                        imprimere("  conditio: %.*s\n", ramus->conditio->mensura, ramus->conditio->datum);
                        CREDO_AEQUALIS_I32(ramus->conditio->mensura, VIII);  /* "HEADER_H" = 8 */
                    }
                }
            }
        }
    }

    /* Test 4: Multiple independent conditionals */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

        imprimere("\n--- Probans multiple conditionals ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef X\n"
            "int x;\n"
            "#endif\n"
            "#ifdef Y\n"
            "int y;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            num_decls = xar_numerus(tu->datum.translation_unit.declarationes);
            imprimere("  num_decls: %d\n", num_decls);
            CREDO_AEQUALIS_I32(num_decls, II);  /* Two CONDITIONALIS nodes */

            si (num_decls >= II)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus** second_ptr;
                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                second_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, I);
                CREDO_AEQUALIS_I32((i32)(*first_ptr)->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);
                CREDO_AEQUALIS_I32((i32)(*second_ptr)->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);
            }
        }
    }

    /* Test 5: Conditional mixed with regular declarations */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

        imprimere("\n--- Probans conditional mixed with declarations ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "int before;\n"
            "#ifdef FEATURE\n"
            "int feature;\n"
            "#endif\n"
            "int after;");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            num_decls = xar_numerus(tu->datum.translation_unit.declarationes);
            imprimere("  num_decls: %d\n", num_decls);
            CREDO_AEQUALIS_I32(num_decls, III);  /* decl + CONDITIONALIS + decl */

            si (num_decls >= III)
            {
                Arbor2Nodus** ptr1;
                Arbor2Nodus** ptr2;
                Arbor2Nodus** ptr3;
                ptr1 = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                ptr2 = xar_obtinere(tu->datum.translation_unit.declarationes, I);
                ptr3 = xar_obtinere(tu->datum.translation_unit.declarationes, II);

                imprimere("  node 0: %s\n", arbor2_nodus_genus_nomen((*ptr1)->genus));
                imprimere("  node 1: %s\n", arbor2_nodus_genus_nomen((*ptr2)->genus));
                imprimere("  node 2: %s\n", arbor2_nodus_genus_nomen((*ptr3)->genus));

                CREDO_AEQUALIS_I32((i32)(*ptr2)->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);
            }
        }
    }

    /* Test 6: Nested conditionals */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

        imprimere("\n--- Probans nested conditionals ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef A\n"
            "int a;\n"
            "#ifdef B\n"
            "int b;\n"
            "#endif\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            num_decls = xar_numerus(tu->datum.translation_unit.declarationes);
            imprimere("  num_decls: %d\n", num_decls);
            CREDO_AEQUALIS_I32(num_decls, I);  /* One outer CONDITIONALIS */

            si (num_decls >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;
                i32 num_tokens;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;
                CREDO_AEQUALIS_I32((i32)cond->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);

                /* Check that nested conditional tokens are included in branch */
                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    num_tokens = xar_numerus(ramus->lexemata);
                    imprimere("  branch tokens: %d\n", num_tokens);
                    /* Should have tokens for: int a; # ifdef B int b; # endif */
                    CREDO_MAIOR_I32(num_tokens, V);
                }
            }
        }
    }


    /* ========================================================
     * PROBARE: Conditional Expression Evaluation (Phase 5b)
     * ======================================================== */

    /* Test: #if with simple constant 1 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if 1\n"
            "int yes;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;
                CREDO_AEQUALIS_I32((i32)cond->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_AEQUALIS_I32((i32)ramus->genus, (i32)ARBOR2_DIRECTIVUM_IF);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_VERUM(ramus->valor_evaluatus != ZEPHYRUM);  /* 1 = true */
                }
            }
        }
    }

    /* Test: #if with simple constant 0 */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if 0 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if 0\n"
            "int no;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_AEQUALIS_S64((s64)ramus->valor_evaluatus, ZEPHYRUM);  /* 0 = false */
                }
            }
        }
    }

    /* Test: #if with arithmetic expression */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if 1 + 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if 1 + 1\n"
            "int two;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_AEQUALIS_S64((s64)ramus->valor_evaluatus, II);  /* 1+1 = 2 */
                }
            }
        }
    }

    /* Test: #if with comparison */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if 5 > 3 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if 5 > 3\n"
            "int greater;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_VERUM(ramus->valor_evaluatus != ZEPHYRUM);  /* 5 > 3 = true */
                }
            }
        }
    }

    /* Test: #if with logical AND */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if 1 && 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if 1 && 1\n"
            "int both;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_VERUM(ramus->valor_evaluatus != ZEPHYRUM);  /* 1 && 1 = true */
                }
            }
        }
    }

    /* Test: #if with logical NOT */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if !0 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if !0\n"
            "int negated;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_VERUM(ramus->valor_evaluatus != ZEPHYRUM);  /* !0 = true */
                }
            }
        }
    }

    /* Test: #if with defined() - undefined macro */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if defined(UNDEFINED) ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if defined(UNDEFINED)\n"
            "int undefined;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_AEQUALIS_S64((s64)ramus->valor_evaluatus, ZEPHYRUM);  /* undefined = 0 */
                }
            }
        }
    }

    /* Test: #if with ternary operator */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if 1 ? 5 : 10 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if 1 ? 5 : 10\n"
            "int ternary;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_AEQUALIS_S64((s64)ramus->valor_evaluatus, V);  /* 1 ? 5 : 10 = 5 */
                }
            }
        }
    }

    /* Test: #elif evaluation */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if 0 / #elif 1 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if 0\n"
            "int first;\n"
            "#elif 1\n"
            "int second;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) >= II)
                {
                    Arbor2CondRamus* ramus0;
                    Arbor2CondRamus* ramus1;

                    ramus0 = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    ramus1 = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, I);

                    /* First branch (#if 0) should be false */
                    CREDO_AEQUALIS_I32((i32)ramus0->genus, (i32)ARBOR2_DIRECTIVUM_IF);
                    CREDO_VERUM(ramus0->est_evaluatum);
                    imprimere("  ramus0 valor: %lld\n", (longus longus)ramus0->valor_evaluatus);
                    CREDO_AEQUALIS_S64((s64)ramus0->valor_evaluatus, ZEPHYRUM);

                    /* Second branch (#elif 1) should be true */
                    CREDO_AEQUALIS_I32((i32)ramus1->genus, (i32)ARBOR2_DIRECTIVUM_ELIF);
                    CREDO_VERUM(ramus1->est_evaluatum);
                    imprimere("  ramus1 valor: %lld\n", (longus longus)ramus1->valor_evaluatus);
                    CREDO_VERUM(ramus1->valor_evaluatus != ZEPHYRUM);
                }
            }
        }
    }

    /* Test: #ifdef evaluation with defined macro */
    {
        Arbor2Expansion* exp_test;
        Arbor2GLR* glr_test;
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #ifdef with defined macro ---\n");

        /* Create expansion context with macro defined */
        exp_test = arbor2_expansion_creare(piscina, intern);
        arbor2_expansion_addere_macro(exp_test, "FEATURE_X", "1", NIHIL);
        glr_test = arbor2_glr_creare(piscina, intern, exp_test);

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef FEATURE_X\n"
            "int feature;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_AEQUALIS_I32((i32)ramus->genus, (i32)ARBOR2_DIRECTIVUM_IFDEF);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_VERUM(ramus->valor_evaluatus != ZEPHYRUM);  /* defined = true */
                }
            }
        }
    }

    /* Test: #ifndef evaluation with undefined macro */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #ifndef with undefined macro ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifndef UNDEFINED_MACRO\n"
            "int not_defined;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_AEQUALIS_I32((i32)ramus->genus, (i32)ARBOR2_DIRECTIVUM_IFNDEF);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_VERUM(ramus->valor_evaluatus != ZEPHYRUM);  /* !defined = true */
                }
            }
        }
    }

    /* Test: Complex expression with parentheses */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if (1 + 2) * 3 == 9 ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if (1 + 2) * 3 == 9\n"
            "int complex;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_VERUM(ramus->valor_evaluatus != ZEPHYRUM);  /* (1+2)*3==9 = true */
                }
            }
        }
    }

    /* Test: Bitwise operators */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans #if 0xFF & 0x0F ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#if 0xFF & 0x0F\n"
            "int bitwise;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_VERUM(ramus->est_evaluatum);
                    imprimere("  valor_evaluatus: %lld (expect 15)\n", (longus longus)ramus->valor_evaluatus);
                    CREDO_AEQUALIS_S64((s64)ramus->valor_evaluatus, XV);  /* 0xFF & 0x0F = 15 */
                }
            }
        }
    }


    /* ========================================================
     * PROBARE: Branch Parsing (Phase 5.4)
     * ======================================================== */

    /* Test: Basic parsed branch - one declaration */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans branch parsing: basic ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef DEBUG\n"
            "int x;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;
                CREDO_AEQUALIS_I32((i32)cond->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);

                    /* Verificare ramus->parsed */
                    imprimere("  ramus->parsed: %s\n", ramus->parsed != NIHIL ? "non-NIHIL" : "NIHIL");
                    CREDO_NON_NIHIL(ramus->parsed);

                    si (ramus->parsed != NIHIL)
                    {
                        Arbor2Nodus* parsed_tu = ramus->parsed;
                        i32 num_parsed;

                        CREDO_AEQUALIS_I32((i32)parsed_tu->genus, (i32)ARBOR2_NODUS_TRANSLATION_UNIT);
                        num_parsed = xar_numerus(parsed_tu->datum.translation_unit.declarationes);
                        imprimere("  parsed declarations: %d\n", num_parsed);
                        CREDO_AEQUALIS_I32(num_parsed, I);  /* int x; */
                    }
                }
            }
        }
    }

    /* Test: Multiple declarations in branch */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans branch parsing: multiple decls ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef DEBUG\n"
            "int x;\n"
            "int y;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_NON_NIHIL(ramus->parsed);

                    si (ramus->parsed != NIHIL)
                    {
                        Arbor2Nodus* parsed_tu = ramus->parsed;
                        i32 num_parsed;

                        num_parsed = xar_numerus(parsed_tu->datum.translation_unit.declarationes);
                        imprimere("  parsed declarations: %d\n", num_parsed);
                        CREDO_AEQUALIS_I32(num_parsed, II);  /* int x; int y; */
                    }
                }
            }
        }
    }

    /* Test: Nested conditional in branch */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans branch parsing: nested conditional ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef OUTER\n"
            "#ifdef INNER\n"
            "int nested;\n"
            "#endif\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                Arbor2CondRamus* ramus;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS &&
                    xar_numerus(cond->datum.conditionalis.rami) > ZEPHYRUM)
                {
                    ramus = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                    CREDO_NON_NIHIL(ramus->parsed);

                    si (ramus->parsed != NIHIL)
                    {
                        Arbor2Nodus* parsed_tu = ramus->parsed;
                        i32 num_parsed;

                        num_parsed = xar_numerus(parsed_tu->datum.translation_unit.declarationes);
                        imprimere("  parsed declarations: %d\n", num_parsed);
                        CREDO_AEQUALIS_I32(num_parsed, I);  /* One nested CONDITIONALIS */

                        si (num_parsed >= I)
                        {
                            Arbor2Nodus** inner_ptr;
                            Arbor2Nodus* inner_cond;

                            inner_ptr = xar_obtinere(parsed_tu->datum.translation_unit.declarationes, ZEPHYRUM);
                            inner_cond = *inner_ptr;
                            imprimere("  inner node genus: %d (CONDITIONALIS=%d)\n",
                                (i32)inner_cond->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);
                            CREDO_AEQUALIS_I32((i32)inner_cond->genus, (i32)ARBOR2_NODUS_CONDITIONALIS);
                        }
                    }
                }
            }
        }
    }

    /* Test: Empty branch */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans branch parsing: empty branch ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef DEBUG\n"
            "#else\n"
            "int fallback;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                i32 num_rami;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS)
                {
                    num_rami = xar_numerus(cond->datum.conditionalis.rami);
                    imprimere("  num_rami: %d\n", num_rami);
                    CREDO_AEQUALIS_I32(num_rami, II);  /* ifdef branch + else branch */

                    si (num_rami >= II)
                    {
                        Arbor2CondRamus* ramus_if;
                        Arbor2CondRamus* ramus_else;

                        ramus_if = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                        ramus_else = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, I);

                        /* Empty branch should have NIHIL parsed */
                        imprimere("  ifdef branch parsed: %s\n",
                            ramus_if->parsed != NIHIL ? "non-NIHIL" : "NIHIL");
                        CREDO_NIHIL(ramus_if->parsed);

                        /* Else branch should have one declaration */
                        imprimere("  else branch parsed: %s\n",
                            ramus_else->parsed != NIHIL ? "non-NIHIL" : "NIHIL");
                        CREDO_NON_NIHIL(ramus_else->parsed);

                        si (ramus_else->parsed != NIHIL)
                        {
                            i32 num_else_decls;
                            num_else_decls = xar_numerus(ramus_else->parsed->datum.translation_unit.declarationes);
                            imprimere("  else declarations: %d\n", num_else_decls);
                            CREDO_AEQUALIS_I32(num_else_decls, I);
                        }
                    }
                }
            }
        }
    }

    /* Test: Declaration after ifdef/else */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

        imprimere("\n--- Probans branch parsing: ifdef with else ---\n");

        tokens = _lexare_ad_tokens(piscina, intern,
            "#ifdef DEBUG\n"
            "int debug_val;\n"
            "#else\n"
            "int release_val;\n"
            "#endif\n");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);

        CREDO_VERUM(res.successus);

        si (res.radix != NIHIL)
        {
            tu = res.radix;
            si (xar_numerus(tu->datum.translation_unit.declarationes) >= I)
            {
                Arbor2Nodus** first_ptr;
                Arbor2Nodus* cond;
                i32 num_rami;

                first_ptr = xar_obtinere(tu->datum.translation_unit.declarationes, ZEPHYRUM);
                cond = *first_ptr;

                si (cond->genus == ARBOR2_NODUS_CONDITIONALIS)
                {
                    num_rami = xar_numerus(cond->datum.conditionalis.rami);
                    CREDO_AEQUALIS_I32(num_rami, II);

                    si (num_rami >= II)
                    {
                        Arbor2CondRamus* ramus_if;
                        Arbor2CondRamus* ramus_else;

                        ramus_if = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, ZEPHYRUM);
                        ramus_else = *(Arbor2CondRamus**)xar_obtinere(cond->datum.conditionalis.rami, I);

                        /* Both branches should have parsed content */
                        CREDO_NON_NIHIL(ramus_if->parsed);
                        CREDO_NON_NIHIL(ramus_else->parsed);

                        si (ramus_if->parsed != NIHIL)
                        {
                            i32 num_if;
                            num_if = xar_numerus(ramus_if->parsed->datum.translation_unit.declarationes);
                            imprimere("  ifdef decls: %d\n", num_if);
                            CREDO_AEQUALIS_I32(num_if, I);
                        }

                        si (ramus_else->parsed != NIHIL)
                        {
                            i32 num_else;
                            num_else = xar_numerus(ramus_else->parsed->datum.translation_unit.declarationes);
                            imprimere("  else decls: %d\n", num_else);
                            CREDO_AEQUALIS_I32(num_else, I);
                        }
                    }
                }
            }
        }
    }


    /* ========================================================
     * PROBARE: Typedef Edge Cases
     *
     * Comprehensive tests for typedef handling in various contexts:
     * nested typedefs, pointer typedefs, casts, sizeof, etc.
     * ======================================================== */

    /* Test: Nested typedef - typedef of typedef */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Typedef edge: typedef int A; typedef A B; B x; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int A;\n"
                "typedef A B;\n"
                "B x;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 3 declarations */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), III);

                /* Verify A is registered */
                u.c = "A";
                titulus_ch.datum = u.m;
                titulus_ch.mensura = I;
                CREDO_VERUM(arbor2_expansion_est_typedef(exp_test, titulus_ch));

                /* Verify B is registered */
                u.c = "B";
                titulus_ch.datum = u.m;
                titulus_ch.mensura = I;
                CREDO_VERUM(arbor2_expansion_est_typedef(exp_test, titulus_ch));
            }
        }
    }

    /* Test: Pointer typedef usage */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Typedef edge: typedef int *IntPtr; IntPtr p; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int *IntPtr;\n"
                "IntPtr p;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 2 declarations */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), II);
            }
        }
    }

    /* Test: Double pointer from typedef */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Typedef edge: typedef int *IntPtr; IntPtr *pp; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int *IntPtr;\n"
                "IntPtr *pp;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 2 declarations */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), II);
            }
        }
    }

    /* Test: Typedef in cast expression */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Typedef edge: typedef int MyInt; int x = (MyInt)42; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int MyInt;\n"
                "int x = (MyInt)42;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 2 declarations */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), II);
            }
        }
    }

    /* Test: Typedef in sizeof */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Typedef edge: typedef int MyInt; int x = sizeof(MyInt); ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int MyInt;\n"
                "int x = sizeof(MyInt);\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 2 declarations */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), II);
            }
        }
    }

    /* Test: Typedef as function parameter */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Typedef edge: typedef int A; void f(A x) {} ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int A;\n"
                "void f(A x) {}\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 2 declarations (typedef + function) */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), II);
            }
        }
    }

    /* Test: Typedef in struct member */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Typedef edge: typedef int A; struct S { A x; }; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int A;\n"
                "struct S { A x; };\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 2 declarations */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), II);
            }
        }
    }

    /* Test: Chained usage - multiple variables */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Typedef edge: typedef int A; A x, y, z; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int A;\n"
                "A x, y, z;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 2 declarations */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), II);
            }
        }
    }

    /* Test: Array typedef */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Typedef edge: typedef int Arr[10]; Arr a; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int Arr[10];\n"
                "Arr a;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 2 declarations */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), II);

                /* Verify Arr is registered */
                u.c = "Arr";
                titulus_ch.datum = u.m;
                titulus_ch.mensura = III;
                CREDO_VERUM(arbor2_expansion_est_typedef(exp_test, titulus_ch));
            }
        }
    }

    /* Test: Function pointer typedef with (void) params */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Typedef edge: typedef int (*FP)(void); ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int (*FP)(void);\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 1 declaration */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), I);

                /* Verify FP is registered as typedef */
                u.c = "FP";
                titulus_ch.datum = u.m;
                titulus_ch.mensura = II;
                CREDO_VERUM(arbor2_expansion_est_typedef(exp_test, titulus_ch));
            }
        }
    }

    /* Test: Function pointer typedef with empty params () */
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

        imprimere("\n--- Typedef edge: typedef int (*Callback)(); ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int (*Callback)();\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 1 declaration */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), I);

                /* Verify Callback is registered as typedef */
                u.c = "Callback";
                titulus_ch.datum = u.m;
                titulus_ch.mensura = VIII;
                CREDO_VERUM(arbor2_expansion_est_typedef(exp_test, titulus_ch));
            }
        }
    }

    /* Test: Function pointer typedef used in subsequent declaration */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- Typedef edge: typedef int (*FP)(void); FP f; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "typedef int (*FP)(void);\n"
                "FP f;\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);

            si (res.radix != NIHIL)
            {
                /* Should have 2 declarations */
                CREDO_AEQUALIS_I32(
                    xar_numerus(res.radix->datum.translation_unit.declarationes), II);
            }
        }
    }

    /* ================================================================
     * DECLARATOR_CONTEXTUS tests - Grouped declarators (function pointers)
     * These test that PAREN_APERTA is handled after type specifiers
     * ================================================================ */

    /* Test: static int (*fp)(void) - storage class + function pointer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- DECLARATOR_CONTEXTUS: static int (*fp)(void); ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "static int (*fp)(void);\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);
        }
    }

    /* Test: extern int (*callback)() - extern + function pointer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- DECLARATOR_CONTEXTUS: extern int (*callback)(); ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "extern int (*callback)();\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);
        }
    }

    /* Test: void func(int (*param)(void)) - function pointer parameter */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- DECLARATOR_CONTEXTUS: void func(int (*param)(void)) ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "void func(int (*param)(void)) {}\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);
        }
    }

    /* Test: struct S { int (*fp)(); }; - struct member function pointer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- DECLARATOR_CONTEXTUS: struct S { int (*fp)(); }; ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "struct S { int (*fp)(); };\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);
        }
    }

    /* Test: int x, (*fp)(); - multi-declarator with function pointer */
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        imprimere("\n--- DECLARATOR_CONTEXTUS: int x, (*fp)(); ---\n");

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern,
                "int x, (*fp)();\n");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            CREDO_NON_NIHIL(res.radix);
        }
    }


    /* Compendium */
    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    credo_claudere();
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
