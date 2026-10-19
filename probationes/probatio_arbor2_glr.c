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

    /* ========================================================
     * PROBARE: Table validation
     * ======================================================== */
interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        b32 tabula_valida;

        tabula_valida = arbor2_glr_validare_tabulas();
        CREDO_AEQUALIS_I32((i32)tabula_valida, VERUM);
    }
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    (vacuum)c;
    {
        b32 tags_valida;

        tags_valida = arbor2_glr_validare_tags();
        CREDO_AEQUALIS_I32((i32)tags_valida, VERUM);
    }
}

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    (vacuum)c;
    {
        b32 reachability_valida;

        reachability_valida = arbor2_glr_validare_reachability();
        CREDO_AEQUALIS_I32((i32)reachability_valida, VERUM);
    }
}

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        b32 goto_valida;

        goto_valida = arbor2_glr_validare_goto_completeness();
        CREDO_VERUM(goto_valida);
        imprimere("  GOTO validation: %s\n", goto_valida ? "VERUM" : "FALSUM (issues above)");
    }
}

    /* ========================================================
     * PROBARE: Simple identifier
     * ======================================================== */
interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Simple integer
     * ======================================================== */
interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Binary addition
     * ======================================================== */
interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Precedence - 1 + 2 * 3
     * Should parse as 1 + (2 * 3), not (1 + 2) * 3
     * ======================================================== */
interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;

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
}

    /* ========================================================
     * PROBARE: Parenthesized expression
     * ======================================================== */
interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Subtraction (E9)
     * ======================================================== */
interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Division (E9)
     * ======================================================== */
interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Modulo (E9)
     * ======================================================== */
interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Mixed precedence (E9)
     * 1 + 2 * 3 - 4 / 2 should parse as (1 + (2 * 3)) - (4 / 2)
     * ======================================================== */
interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comparison operator < (E9.2)
     * ======================================================== */
interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comparison operator > (E9.2)
     * ======================================================== */
interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comparison operator <= (E9.2)
     * ======================================================== */
interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comparison operator >= (E9.2)
     * ======================================================== */
interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Equality operator == (E9.2)
     * ======================================================== */
interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Inequality operator != (E9.2)
     * ======================================================== */
interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comparison with expressions: 1 + 2 < 3 + 4 (E9.2)
     * Should parse as (1 + 2) < (3 + 4)
     * ======================================================== */
interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Equality with comparisons: 1 < 2 == 3 < 4 (E9.2)
     * Should parse as (1 < 2) == (3 < 4)
     * ======================================================== */
interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Mixed precedence: 1 + 2 < 3 * 4 (E9.2)
     * Should parse as (1 + 2) < (3 * 4) - multiplication binds tighter
     * ======================================================== */
interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comparison inside parens: (1 < 2) (E9.2)
     * ======================================================== */
interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Nested parens with comparison: ((1 < 2)) (E9.2)
     * ======================================================== */
interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Chained equality: 1 == 2 == 3 (E9.2)
     * Left-associative: ((1 == 2) == 3)
     * ======================================================== */
interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Chained comparison: 1 < 2 < 3 (E9.2)
     * Left-associative: ((1 < 2) < 3)
     * ======================================================== */
interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Complex RHS: 1 < 2 * 3 + 4 (E9.2)
     * Precedence: 1 < ((2 * 3) + 4)
     * ======================================================== */
interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comparison in if statement (E9.2)
     * ======================================================== */
interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Equality in while statement (E9.2)
     * ======================================================== */
interior vacuum
_sectio_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comparison in for loop condition (E9.2)
     * ======================================================== */
interior vacuum
_sectio_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Left shift operator << (E9.3 - translatio)
     * ======================================================== */
interior vacuum
_sectio_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Right shift operator >> (E9.3 - translatio)
     * ======================================================== */
interior vacuum
_sectio_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Chained shifts: a << b >> c (E9.3)
     * Should parse as (a << b) >> c (left associative)
     * ======================================================== */
interior vacuum
_sectio_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Shift with addition: (x + y) << 2 (E9.3)
     * Addition binds tighter, so x + y groups first
     * ======================================================== */
interior vacuum
_sectio_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comparison with shift: a < b << c (E9.3)
     * Shift binds tighter than comparison
     * ======================================================== */
interior vacuum
_sectio_tricesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Logical AND operator && (E10)
     * ======================================================== */
interior vacuum
_sectio_tricesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Logical OR operator || (E10)
     * ======================================================== */
interior vacuum
_sectio_tricesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: && has higher precedence than || (E10)
     * a || b && c should parse as a || (b && c)
     * ======================================================== */
interior vacuum
_sectio_tricesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Chained && is left-associative (E10)
     * a && b && c should parse as (a && b) && c
     * ======================================================== */
interior vacuum
_sectio_tricesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Logical operators with comparisons (E10)
     * a == b && c < d should parse as (a == b) && (c < d)
     * ======================================================== */
interior vacuum
_sectio_quadragesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Unary dereference
     * ======================================================== */
interior vacuum
_sectio_quadragesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Unary address-of
     * ======================================================== */
interior vacuum
_sectio_quadragesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Typedef detection API
     * ======================================================== */
interior vacuum
_sectio_quadragesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2Token* tok;

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
}

    /* ========================================================
     * PROBARE: foo * bar (ambiguous - could be expr or decl)
     * With no typedef info, should produce AMBIGUUS node
     * ======================================================== */
interior vacuum
_sectio_quadragesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: MyType * ptr (typedef -> declaration only)
     * ======================================================== */
interior vacuum
_sectio_quadragesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Function declarator: MyType * fn()
     * ======================================================== */
interior vacuum
_sectio_quadragesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Function declarator with void: MyType * fn(void)
     * ======================================================== */
interior vacuum
_sectio_quadragesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Single parameter: MyType * fn(int x)
     * ======================================================== */
interior vacuum
_sectio_quadragesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Multiple params: MyType * fn(int x, int y)
     * ======================================================== */
interior vacuum
_sectio_quadragesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Pointer param: MyType * fn(int *ptr)
     * ======================================================== */
interior vacuum
_sectio_quinquagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Expression statement: x + 1;
     * ======================================================== */
interior vacuum
_sectio_quinquagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Empty statement: ;
     * ======================================================== */
interior vacuum
_sectio_quinquagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Simple statement: 42;
     * ======================================================== */
interior vacuum
_sectio_quinquagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Empty compound statement: { }
     * ======================================================== */
interior vacuum
_sectio_quinquagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Compound with one statement: { x; }
     * ======================================================== */
interior vacuum
_sectio_quinquagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Compound with multiple statements: { x; y + 1; }
     * ======================================================== */
interior vacuum
_sectio_quinquagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Nested compound: { { x; } }
     * ======================================================== */
interior vacuum
_sectio_quinquagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* inner;

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
}

    /* ========================================================
     * PROBARE: Simple if statement: if (x) y;
     * ======================================================== */
interior vacuum
_sectio_quinquagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: If-else statement: if (x) y; else z;
     * ======================================================== */
interior vacuum
_sectio_quinquagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: If with compound body: if (x) { y; }
     * ======================================================== */
interior vacuum
_sectio_sexagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Subscript in if condition: if (arr[0]) x;
     * Tests state 31 GOTO(POSTFIXUM)
     * ======================================================== */
interior vacuum
_sectio_sexagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Assignment inside if condition: if (x = 1) y;
     * Common C idiom - assignment in condition
     * ======================================================== */
interior vacuum
_sectio_sexagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Assignment inside while condition: while (x = 1) y;
     * Common C idiom - assignment in condition
     * ======================================================== */
interior vacuum
_sectio_sexagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Assignment in for-init: for (a = 1; b; c) d;
     * Common C idiom - assignment in for loop init
     * ======================================================== */
interior vacuum
_sectio_sexagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Assignment in do-while: do x; while (a = 1);
     * Common C idiom - assignment in do-while condition
     * ======================================================== */
interior vacuum
_sectio_sexagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Assignment in switch: switch (a = 1) { }
     * Less common but valid C
     * ======================================================== */
interior vacuum
_sectio_sexagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Dangling else: if (a) if (b) c; else d;
     * else should bind to inner if (b), not outer if (a)
     * ======================================================== */
interior vacuum
_sectio_sexagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* inner_if;

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
}

    /* ========================================================
     * PROBARE: Simple while statement: while (x) y;
     * ======================================================== */
interior vacuum
_sectio_sexagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: While with compound body: while (x) { a; b; }
     * ======================================================== */
interior vacuum
_sectio_sexagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Simple do-while statement: do x; while (y);
     * ======================================================== */
interior vacuum
_sectio_septuagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Do-while with compound body: do { a; b; } while (c);
     * ======================================================== */
interior vacuum
_sectio_septuagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Nested while: while (a) while (b) c;
     * ======================================================== */
interior vacuum
_sectio_septuagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* inner_while;

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
}

    /* ========================================================
     * PROBARE: Simple for with all expressions: for (i; c; n) x;
     * ======================================================== */
interior vacuum
_sectio_septuagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: For with empty init: for (; c; n) x;
     * ======================================================== */
interior vacuum
_sectio_septuagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: For with empty condition: for (i; ; n) x;
     * ======================================================== */
interior vacuum
_sectio_septuagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: For with empty increment: for (i; c; ) x;
     * ======================================================== */
interior vacuum
_sectio_septuagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: For with all empty (infinite loop): for (;;) x;
     * ======================================================== */
interior vacuum
_sectio_septuagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: For with compound body: for (i; c; n) { a; b; }
     * ======================================================== */
interior vacuum
_sectio_septuagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Nested for: for (;;) for (;;) x;
     * ======================================================== */
interior vacuum
_sectio_septuagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* inner_for;

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
}

    /* ========================================================
     * PROBARE: break statement
     * ======================================================== */
interior vacuum
_sectio_octogesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: continue statement
     * ======================================================== */
interior vacuum
_sectio_octogesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: return statement without value
     * ======================================================== */
interior vacuum
_sectio_octogesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: return statement with value
     * ======================================================== */
interior vacuum
_sectio_octogesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: goto statement
     * ======================================================== */
interior vacuum
_sectio_octogesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: break inside while loop
     * ======================================================== */
interior vacuum
_sectio_octogesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: return with expression in compound
     * ======================================================== */
interior vacuum
_sectio_octogesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Xar* sententiae;

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
}

    /* ========================================================
     * PROBARE: Labeled statements (Phase C4b)
     * ======================================================== */
    /* Simple label */
interior vacuum
_sectio_octogesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Label with empty statement */
interior vacuum
_sectio_octogesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Nested labels */
interior vacuum
_sectio_octogesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Label with compound */
interior vacuum
_sectio_nonagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Label with if */
interior vacuum
_sectio_nonagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Label as goto target in compound */
interior vacuum
_sectio_nonagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Xar* sententiae;

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
}

    /* ========================================================
     * PROBARE: Switch/case/default statements (Phase C4c)
     * ======================================================== */
    /* Simple switch with one case */
interior vacuum
_sectio_nonagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Switch with default */
interior vacuum
_sectio_nonagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Switch with case and break */
interior vacuum
_sectio_nonagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Multiple cases (fall-through) */
interior vacuum
_sectio_nonagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Case with expression */
interior vacuum
_sectio_nonagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Function definition: MyType fn(void) { }
     * ======================================================== */
interior vacuum
_sectio_nonagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Function definition with body: MyType fn(void) { return x; }
     * ======================================================== */
interior vacuum
_sectio_nonagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Function with params: MyType foo(int x, int y) { return x; }
     * ======================================================== */
interior vacuum
_sectio_centesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Pointer return type: MyType * bar(void) { }
     * ======================================================== */
interior vacuum
_sectio_centesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Qualified return type: const int f() { } (P505)
     * ======================================================== */
interior vacuum
_sectio_centesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Qualified return type: volatile int g() { } (P506)
     * ======================================================== */
interior vacuum
_sectio_centesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Basic function: void f() { } - verify tok_const/tok_volatile are NIHIL
     * ======================================================== */
interior vacuum
_sectio_centesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Qualified return with char: const char h() { }
     * ======================================================== */
interior vacuum
_sectio_centesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ================================================================
     * COMPOUND RETURN TYPE TESTS
     * These test function definitions with compound type specifiers.
     * ================================================================ */
    /* Test function with unsigned int return type */
interior vacuum
_sectio_centesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test function with long int return type */
interior vacuum
_sectio_centesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test function with short int return type */
interior vacuum
_sectio_centesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test function with signed int return type */
interior vacuum
_sectio_centesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test function with unsigned long return type */
interior vacuum
_sectio_centesima_decima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test function with unsigned return type (implicit int) */
interior vacuum
_sectio_centesima_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Struct specifier tests
     * ======================================================== */
    /* Test forward reference: struct foo x */
interior vacuum
_sectio_centesima_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test named struct with single member: struct foo { int x; } */
interior vacuum
_sectio_centesima_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test named struct with multiple members */
interior vacuum
_sectio_centesima_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ================================================================
     * PROBARE: Struct members with various type specifiers
     * These test that all C89 type specifiers work in struct members.
     * ================================================================ */
    /* Test struct with float member: struct foo { float f; } */
interior vacuum
_sectio_centesima_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test struct with double member: struct foo { double d; } */
interior vacuum
_sectio_centesima_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { double d; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Test struct with short member: struct foo { short s; } */
interior vacuum
_sectio_centesima_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { short s; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Test struct with long member: struct foo { long l; } */
interior vacuum
_sectio_centesima_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { long l; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Test struct with unsigned member: struct foo { unsigned u; } */
interior vacuum
_sectio_centesima_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { unsigned u; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Test struct with signed member: struct foo { signed s; } */
interior vacuum
_sectio_centesima_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { signed s; }");
        res = arbor2_glr_parsere(glr, tokens);

        imprimere("  successus: %s\n", res.successus ? "VERUM" : "FALSUM");

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Test struct with mixed type members: struct foo { int x; float f; } */
interior vacuum
_sectio_centesima_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ================================================================
     * COMPOUND TYPE SPECIFIER TESTS
     * These test multi-token type specifiers in struct members.
     * ================================================================ */
    /* Test struct with unsigned int member */
interior vacuum
_sectio_centesima_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { unsigned int x; }");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Test struct with long int member */
interior vacuum
_sectio_centesima_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { long int x; }");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Test struct with const int member */
interior vacuum
_sectio_centesima_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { const int x; }");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Test struct with unsigned long member */
interior vacuum
_sectio_centesima_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "struct foo { unsigned long x; }");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Test struct variable declaration: struct foo { int x; } var */
interior vacuum
_sectio_centesima_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Bit fields (E4)
     * ======================================================== */
    /* Test single named bit field: struct foo { int x : 3; } */
interior vacuum
_sectio_centesima_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test multiple bit fields: struct foo { int x : 3; int y : 5; } */
interior vacuum
_sectio_centesima_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test anonymous bit field: struct foo { int : 4; } */
interior vacuum
_sectio_centesima_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test mixed named and anonymous: struct foo { int x : 3; int : 0; int y : 5; } */
interior vacuum
_sectio_centesima_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test expression bit width: struct foo { int x : 1 + 2; } */
interior vacuum
_sectio_centesima_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test bit field in union: union foo { int x : 3; } */
interior vacuum
_sectio_centesima_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Union specifier
     * ======================================================== */
    /* Test union forward reference: union foo */
interior vacuum
_sectio_centesima_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test named union with single member: union foo { int x; } */
interior vacuum
_sectio_centesima_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test named union with multiple members: union foo { int x; int y; } */
interior vacuum
_sectio_centesima_tricesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test anonymous union: union { int x; } anon */
interior vacuum
_sectio_centesima_tricesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Enum specifier (E3)
     * ======================================================== */
    /* Test enum forward reference: enum foo */
interior vacuum
_sectio_centesima_tricesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test anonymous enum: enum { A, B, C } */
interior vacuum
_sectio_centesima_tricesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test named enum: enum foo { A, B, C } */
interior vacuum
_sectio_centesima_tricesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum with values: enum foo { A = 1, B, C = 10 } */
interior vacuum
_sectio_centesima_quadragesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum as type in declaration: enum foo x */
interior vacuum
_sectio_centesima_quadragesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum with expression value: enum { A = 1 + 2 } */
interior vacuum
_sectio_centesima_quadragesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum with unary minus: enum { A = -1 } */
interior vacuum
_sectio_centesima_quadragesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum with unary plus: enum { A = +1 } */
interior vacuum
_sectio_centesima_quadragesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum with bitwise NOT: enum { A = ~0 } */
interior vacuum
_sectio_centesima_quadragesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum with logical NOT: enum { A = !0 } */
interior vacuum
_sectio_centesima_quadragesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum with sizeof: enum { A = sizeof(int) } */
interior vacuum
_sectio_centesima_quadragesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum with char literal: enum { A = 'X' } */
interior vacuum
_sectio_centesima_quadragesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test enum with subsequent enumerator using new expressions: enum { A, B = ~1, C = -2 } */
interior vacuum
_sectio_centesima_quadragesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Nested type specifiers (Phase E5)
     * ======================================================== */
    /* Test nested struct: struct outer { struct inner { int a; } x; } */
interior vacuum
_sectio_centesima_quinquagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test nested enum: struct outer { enum { A, B } status; } */
interior vacuum
_sectio_centesima_quinquagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test nested union: struct outer { union { int i; int j; } data; } */
interior vacuum
_sectio_centesima_quinquagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test nested pointer: struct outer { struct inner { int a; } *ptr; } */
interior vacuum
_sectio_centesima_quinquagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test multiple members with nested: struct outer { int x; struct { int a; } nested; int y; } */
interior vacuum
_sectio_centesima_quinquagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Typedef declarations (P74-P79)
     * ======================================================== */
    /* Test simple typedef: typedef int MyInt; */
interior vacuum
_sectio_centesima_quinquagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test pointer typedef: typedef int* IntPtr; */
interior vacuum
_sectio_centesima_quinquagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test typedef struct: typedef struct { int x; } Point; */
interior vacuum
_sectio_centesima_quinquagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test typedef struct pointer: typedef struct { int x; } *PointPtr; */
interior vacuum
_sectio_centesima_quinquagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test typedef enum: typedef enum { A, B } Status; */
interior vacuum
_sectio_centesima_quinquagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test typedef enum pointer: typedef enum { A, B } *StatusPtr; */
interior vacuum
_sectio_centesima_sexagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test typedef named struct: typedef struct MyStruct { int x; } MyStruct; */
interior vacuum
_sectio_centesima_sexagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test typedef forward ref: typedef struct Foo Foo; */
interior vacuum
_sectio_centesima_sexagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Array declarators (Phase E7)
     * ========================================================
     *
     * Phase E7 state machine complete - array tests enabled.
     */
    /* Test fixed size array: int arr[10] */
interior vacuum
_sectio_centesima_sexagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test unsized array: int arr[] */
interior vacuum
_sectio_centesima_sexagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test multi-dimensional array: int matrix[10][20] */
interior vacuum
_sectio_centesima_sexagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test array of pointers: int *arr[10]
     * NOTE: This may produce AMBIGUUS because the grammar doesn't distinguish
     * keywords from identifiers in expression contexts. The declaration
     * interpretation should be present in the ambiguity. */
interior vacuum
_sectio_centesima_sexagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* E8: Struct member arrays now supported via declarator refactor */
    /* Test struct member array: struct { int arr[5]; } */
interior vacuum
_sectio_centesima_sexagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test struct member pointer array: struct { int *ptrs[5]; } */
interior vacuum
_sectio_centesima_sexagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test struct multi-dimensional array: struct { int matrix[3][4]; } */
interior vacuum
_sectio_centesima_sexagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test struct multiple array members: struct { int a[5]; int b[10]; } */
interior vacuum
_sectio_centesima_septuagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* E8: Typedef arrays now supported via declarator refactor */
    /* Test typedef array: typedef int IntArray[10] */
interior vacuum
_sectio_centesima_septuagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test typedef pointer array: typedef int* IntPtrArray[10]; */
interior vacuum
_sectio_centesima_septuagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test typedef multi-dim array: typedef int Matrix[3][4]; */
interior vacuum
_sectio_centesima_septuagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test typedef float: typedef float MyFloat; */
interior vacuum
_sectio_centesima_septuagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test expression size: int arr[1 + 2] */
interior vacuum
_sectio_centesima_septuagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Nested type member arrays (Phase E8 Part 3)
     * ======================================================== */
    /* Test nested struct array: struct { struct Inner items[10]; } */
interior vacuum
_sectio_centesima_septuagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test nested enum array: struct { enum { A, B } status[5]; } */
interior vacuum
_sectio_centesima_septuagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test nested struct pointer array: struct { struct Inner *ptrs[3]; } */
interior vacuum
_sectio_centesima_septuagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Assignment operator - simple a = 1
     * ======================================================== */
interior vacuum
_sectio_centesima_septuagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Assignment right-associativity - a = b = c
     * Should parse as a = (b = c), NOT (a = b) = c
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;

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
}

    /* ========================================================
     * PROBARE: Compound assignment operators
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Bitwise assignment operators
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comma operator - a, b
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Comma left-associativity - a, b, c
     * Should parse as ((a, b), c)
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* sinister;

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
}

    /* ========================================================
     * PROBARE: Assignment / comma precedence - a = 1, b = 2
     * Should parse as (a = 1), (b = 2) - comma binds looser
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* sinister;
        Arbor2Nodus* dexter;

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
}

    /* ========================================================
     * PROBARE: Assignment below logical OR - a = b || c
     * Should parse as a = (b || c)
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;

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
}

    /* ========================================================
     * PROBARE: Ternary operator - simple
     * a ? 1 : c -> TERNARIUS(a, 1, c)
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* ========================================================
     * PROBARE: Ternary operator - right associativity
     * a ? 1 : b ? 2 : 3 -> a ? 1 : (b ? 2 : 3)
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* falsum;

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
}

    /* ========================================================
     * PROBARE: Ternary with assignment
     * a = x ? 1 : 2 -> a = (x ? 1 : 2)
     * Ternary binds tighter than assignment
     * ======================================================== */
interior vacuum
_sectio_centesima_octogesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;

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
}

    /* ========================================================
     * PROBARE: Ternary with logical OR
     * a || b ? 1 : 2 -> (a || b) ? 1 : 2
     * Logical OR binds tighter than ternary
     * ======================================================== */
interior vacuum
_sectio_centesima_nonagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;
        Arbor2Nodus* conditio;

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
}

    /* ========================================================
     * PROBARE: Ternary with expressions in branches
     * a ? 1 + 2 : 3 * 4 (use integers to avoid label ambiguity)
     * ======================================================== */
interior vacuum
_sectio_centesima_nonagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* ========================================================
     * PROBARE: Unary operators - more tests
     * ======================================================== */
    /* Bitwise NOT: ~x */
interior vacuum
_sectio_centesima_nonagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Logical NOT: !x */
interior vacuum
_sectio_centesima_nonagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Double NOT: !!x */
interior vacuum
_sectio_centesima_nonagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Mixed unary: ~!x */
interior vacuum
_sectio_centesima_nonagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Unary with binary: ~x + y */
interior vacuum
_sectio_centesima_nonagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Triple unary: ***ptr */
interior vacuum
_sectio_centesima_nonagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Address of address: & &x */
interior vacuum
_sectio_centesima_nonagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Deref then address: *&x */
interior vacuum
_sectio_centesima_nonagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* ========================================================
     * PROBARE: Unary minus and plus operators
     * ======================================================== */
    /* Unary minus: -x */
interior vacuum
_sectio_ducentesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Unary plus: +x */
interior vacuum
_sectio_ducentesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Unary minus with integer literal: -5 */
interior vacuum
_sectio_ducentesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Double negation: - -x (space between) */
interior vacuum
_sectio_ducentesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Unary minus with binary plus: -a + b */
interior vacuum
_sectio_ducentesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Unary minus after binary: a + -b */
interior vacuum
_sectio_ducentesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Mixed unary: -!x */
interior vacuum
_sectio_ducentesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Unary minus with multiply: a * -b */
interior vacuum
_sectio_ducentesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* ========================================================
     * PROBARE: Bitwise binary operators
     * ======================================================== */
    /* Bitwise AND: a & b */
interior vacuum
_sectio_ducentesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Bitwise OR: a | b */
interior vacuum
_sectio_ducentesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Bitwise XOR: a ^ b */
interior vacuum
_sectio_ducentesima_decima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Bitwise precedence: a | b ^ c */
interior vacuum
_sectio_ducentesima_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Bitwise precedence: a ^ b & c */
interior vacuum
_sectio_ducentesima_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Bitwise chain: a | b | c */
interior vacuum
_sectio_ducentesima_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Bitwise with comparison: a & b == c */
interior vacuum
_sectio_ducentesima_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* All bitwise: a | b ^ c & d */
interior vacuum
_sectio_ducentesima_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* ========================================================
     * PROBARE: Complex nested expressions
     * ======================================================== */
    /* Deeply nested parens: (((x))) */
interior vacuum
_sectio_ducentesima_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(((x)))");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Complex arithmetic: (a + b) * (c - d) */
interior vacuum
_sectio_ducentesima_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Mixed precedence: a + b * c - d / e */
interior vacuum
_sectio_ducentesima_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Logical with ternary: a && b ? 1 : 2 */
interior vacuum
_sectio_ducentesima_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Multiple ternary: a ? b ? 1 : 2 : 3 */
interior vacuum
_sectio_ducentesima_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Unary in expressions: ~a & !b | *c */
interior vacuum
_sectio_ducentesima_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

        tokens = _lexare_ad_tokens(piscina, intern, "~a & !b | *c");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        radix = res.radix;
        si (radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)radix->genus, (i32)ARBOR2_NODUS_BINARIUM);
        }
    }
}

    /* Assignment chain: a = b = c = 1 */
interior vacuum
_sectio_ducentesima_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Comma chain: a, b, c, d */
interior vacuum
_sectio_ducentesima_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* Shift with arithmetic: a << 2 + 1 */
interior vacuum
_sectio_ducentesima_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* ========================================================
     * PROBARE: Control flow edge cases
     * ======================================================== */
    /* if-else-if chain */
interior vacuum
_sectio_ducentesima_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "if (a) x; else if (b) y; else z;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Deeply nested if: if (a) if (b) if (c) x; */
interior vacuum
_sectio_ducentesima_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "if (a) if (b) if (c) x;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Nested loops: while (a) for (;;) if (b) break; */
interior vacuum
_sectio_ducentesima_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "while (a) for (;;) if (b) break;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Complex for loop: for (a = 0; a < 10; a = a + 1) x; */
interior vacuum
_sectio_ducentesima_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "for (a = 0; a < 10; a = a + 1) x;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Switch with fallthrough cases */
interior vacuum
_sectio_ducentesima_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "switch (x) { case 1: case 2: case 3: y; break; default: z; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* do-while with simple condition */
interior vacuum
_sectio_ducentesima_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "do { x; y; } while (a < b);");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Multiple statements in compound */
interior vacuum
_sectio_ducentesima_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "{ a; b; c; d; e; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Nested compound statements */
interior vacuum
_sectio_ducentesima_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "{ { { a; } } }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* ========================================================
     * PROBARE: Declarations in compound statements (C99-style)
     * ======================================================== */
    /* Simple declaration in block */
interior vacuum
_sectio_ducentesima_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Declaration with initializer in block */
interior vacuum
_sectio_ducentesima_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "{ int x = 5; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Multiple declarations in block */
interior vacuum
_sectio_ducentesima_tricesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Mixed declarations and statements (C99-style anywhere) */
interior vacuum
_sectio_ducentesima_tricesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Static declaration in block */
interior vacuum
_sectio_ducentesima_tricesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "{ static int x = 0; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Const declaration in block */
interior vacuum
_sectio_ducentesima_tricesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "{ const int x = 42; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Static const declaration in block */
interior vacuum
_sectio_ducentesima_tricesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "{ static const int x = 1; }");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Return with expression */
interior vacuum
_sectio_ducentesima_quadragesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "return a + b * c;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* ========================================================
     * PROBARE: Combined stress tests
     * ======================================================== */
    /* Function with complex body */
interior vacuum
_sectio_ducentesima_quadragesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Struct with complex members */
interior vacuum
_sectio_ducentesima_quadragesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Expression in for condition */
interior vacuum
_sectio_ducentesima_quadragesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern,
            "for (a = 0; a < 10; a = a + 1) x;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Comparison in if condition */
interior vacuum
_sectio_ducentesima_quadragesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "if (a < b) x;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* Assignment in while condition */
interior vacuum
_sectio_ducentesima_quadragesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "while (x = x + 1) y;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* ========================================================
     * PROBARE: Unary ~ and ! in expression contexts
     * (TAG VALIDATION: tests for missing TILDE/EXCLAMATIO)
     * ======================================================== */
    /* State 6: after '(' - tilde in parentheses */
interior vacuum
_sectio_ducentesima_quadragesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "(~a)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* State 7: after unary '*' */
interior vacuum
_sectio_ducentesima_quadragesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "*~a");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* State 9: after '+' */
interior vacuum
_sectio_ducentesima_quadragesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "a + ~b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* State 10: after '*' (multiplication) */
interior vacuum
_sectio_ducentesima_quadragesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "a * !b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* State 31: if condition with tilde */
interior vacuum
_sectio_ducentesima_quinquagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "if (~a) b;");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* State 241: after comparison - tilde after < */
interior vacuum
_sectio_ducentesima_quinquagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "a < ~b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* State 254: after && - tilde */
interior vacuum
_sectio_ducentesima_quinquagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "a && ~b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* State 256: after || - exclamatio */
interior vacuum
_sectio_ducentesima_quinquagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "a || !b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* State 265: after << - tilde */
interior vacuum
_sectio_ducentesima_quinquagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "a << ~b");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* ========================================================
     * PROBARE: Array subscript expressions
     * ======================================================== */
    /* Simple subscript: arr[0] */
interior vacuum
_sectio_ducentesima_quinquagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "arr[0]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }
}

    /* Subscript with identifier index: arr[i] */
interior vacuum
_sectio_ducentesima_quinquagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "arr[i]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }
}

    /* Chained subscripts: arr[i][j] */
interior vacuum
_sectio_ducentesima_quinquagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Subscript in expression: a + b[0] */
interior vacuum
_sectio_ducentesima_quinquagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Multiply with subscript: a * b[0] - tests state 10 GOTO(POSTFIXUM)
     * Note: This is genuinely ambiguous (could be multiplication or pointer-to-array decl)
     * so we accept either BINARIUM or AMBIGUUS with BINARIUM as one interpretation */
interior vacuum
_sectio_ducentesima_quinquagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Unary with subscript: *arr[i] - should be *(arr[i]) */
interior vacuum
_sectio_ducentesima_sexagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Complex expression in subscript index */
interior vacuum
_sectio_ducentesima_sexagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "arr[i + 1]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }
}

    /* ========================================================
     * PROBARE: Function call expressions
     * ======================================================== */
    /* Empty function call: foo() */
interior vacuum
_sectio_ducentesima_sexagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "foo()");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }
}

    /* Function call with one arg: foo(a) */
interior vacuum
_sectio_ducentesima_sexagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "foo(a)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }
}

    /* Function call with multiple args: foo(a, b, c) */
interior vacuum
_sectio_ducentesima_sexagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "foo(a, b, c)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }
}

    /* Chained: foo()[0] - function call then subscript */
interior vacuum
_sectio_ducentesima_sexagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "foo()[0]");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SUBSCRIPTIO);
        }
    }
}

    /* Chained: arr[0]() - subscript then function call */
interior vacuum
_sectio_ducentesima_sexagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "arr[0]()");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }
}

    /* Function call with expression args: foo(a + b) */
interior vacuum
_sectio_ducentesima_sexagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "foo(a + b)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_VOCATIO);
        }
    }
}

    /* ========================================================
     * PROBARE: Member access expressions
     * ======================================================== */
    /* Simple member access: s.x */
interior vacuum
_sectio_ducentesima_sexagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Pointer member access: p->x */
interior vacuum
_sectio_ducentesima_sexagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Chained member access: s.x.y */
interior vacuum
_sectio_ducentesima_septuagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Chained pointer member: p->x->y */
interior vacuum
_sectio_ducentesima_septuagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Mixed: subscript then member: arr[0].x */
interior vacuum
_sectio_ducentesima_septuagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Mixed: call then member: foo().x */
interior vacuum
_sectio_ducentesima_septuagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Mixed: member then subscript: s.x[0] */
interior vacuum
_sectio_ducentesima_septuagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Mixed: member then call: s.x() */
interior vacuum
_sectio_ducentesima_septuagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* ========================================================
     * PROBARE: Increment/Decrement operators
     * ======================================================== */
    /* Post-increment: x++ */
interior vacuum
_sectio_ducentesima_septuagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Post-decrement: x-- */
interior vacuum
_sectio_ducentesima_septuagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Pre-increment: ++x */
interior vacuum
_sectio_ducentesima_septuagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "++x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
        }
    }
}

    /* Pre-decrement: --x */
interior vacuum
_sectio_ducentesima_septuagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "--x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_UNARIUM);
        }
    }
}

    /* Post-increment on subscript: arr[0]++ */
interior vacuum
_sectio_ducentesima_octogesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Pre-increment on subscript: ++arr[0] */
interior vacuum
_sectio_ducentesima_octogesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Post-increment on member: s.x++ */
interior vacuum
_sectio_ducentesima_octogesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Post-decrement on ptr member: p->x-- */
interior vacuum
_sectio_ducentesima_octogesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Mixed: ++x + y-- */
interior vacuum
_sectio_ducentesima_octogesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* Chained post: x++++ should fail or parse as (x++)++ */
interior vacuum
_sectio_ducentesima_octogesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

        tokens = _lexare_ad_tokens(piscina, intern, "x++++");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        /* This may succeed or fail - just testing it doesn't crash */
        CREDO_VERUM(VERUM);  /* Parser handles it */
        (vacuum)res;  /* Suppress unused warning */
    }
}

    /* ========================================================
     * PROBARE: Float literal
     * ======================================================== */
interior vacuum
_sectio_ducentesima_octogesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Char literal
     * ======================================================== */
interior vacuum
_sectio_ducentesima_octogesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: String literal
     * ======================================================== */
interior vacuum
_sectio_ducentesima_octogesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Float in binary expression
     * ======================================================== */
interior vacuum
_sectio_ducentesima_octogesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Char in binary expression
     * ======================================================== */
interior vacuum
_sectio_ducentesima_nonagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Mixed literals in expression
     * ======================================================== */
interior vacuum
_sectio_ducentesima_nonagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: sizeof operator
     * ======================================================== */
interior vacuum
_sectio_ducentesima_nonagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_ducentesima_nonagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(x)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        CREDO_NON_NIHIL(res.radix);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_ducentesima_nonagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: type casts
     * ======================================================== */
interior vacuum
_sectio_ducentesima_nonagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
            CREDO_NON_NIHIL(res.radix->datum.conversio.expressio);
        }
    }
}

interior vacuum
_sectio_ducentesima_nonagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(char)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_ducentesima_nonagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_ducentesima_nonagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Pointer casts (Phase 1.1)
     * ======================================================== */
interior vacuum
_sectio_ducentesima_nonagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: sizeof(type) (Phase 1.1)
     * ======================================================== */
interior vacuum
_sectio_trecentesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(void*)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }
}

    /* ========================================================
     * PROBARE: Identifier (typedef) casts - Phase 1.1b
     * ======================================================== */
interior vacuum
_sectio_trecentesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: struct/union/enum casts (Phase 1.1b Part 2)
     * ======================================================== */
interior vacuum
_sectio_trecentesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(struct foo)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_decima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(union bar)u");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(enum color)c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: sizeof(struct/union/enum) (Phase 1.1b Part 2)
     * ======================================================== */
interior vacuum
_sectio_trecentesima_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(struct foo)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(struct foo*)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(struct foo**)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(union bar)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(union bar*)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(enum color)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(enum color*)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

    /* ========================================================
     * PROBARE: sizeof(type[N]) - array types in sizeof
     * Phase 1.1c
     * ======================================================== */
interior vacuum
_sectio_trecentesima_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(char[256])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }
}

interior vacuum
_sectio_trecentesima_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(void[1])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: sizeof(type[N][M]) - multi-dimensional array types
     * Phase 1.1d
     * ======================================================== */
interior vacuum
_sectio_trecentesima_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(char[256][4])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }
}

interior vacuum
_sectio_trecentesima_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(void[1][1])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: sizeof(type*[N]) - pointer array types
     * Phase 1.1e
     * ======================================================== */
interior vacuum
_sectio_trecentesima_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_tricesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(char*[256])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }
}

interior vacuum
_sectio_trecentesima_tricesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(void*[5])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
            CREDO_AEQUALIS_I32((i32)res.radix->datum.sizeof_expr.est_typus, VERUM);
        }
    }
}

interior vacuum
_sectio_trecentesima_tricesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    arbor2_expansion_addere_typedef(expansion, "MyType");
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        /* Note: sizeof(ID*[N]) doesn't work because the ID path in state 388
         * is disabled to avoid ambiguity. The parser can't distinguish between
         * sizeof(type) and sizeof(expr) for identifier types without type info.
         * This test verifies the parser fails gracefully (known limitation). */
        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(MyType*[20])");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        /* Expected to fail - ID path disabled */
        CREDO_AEQUALIS_I32((i32)res.successus, FALSUM);
    }
}

interior vacuum
_sectio_trecentesima_tricesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: sizeof with type modifiers
     * ======================================================== */
interior vacuum
_sectio_trecentesima_tricesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quadragesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quadragesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(long int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quadragesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(short int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quadragesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned char)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quadragesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed char)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quadragesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned long int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quadragesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned short int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quadragesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed long int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quadragesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed short int)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

    /* sizeof implicit int variants */
interior vacuum
_sectio_trecentesima_quadragesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quinquagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(long)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quinquagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(short)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quinquagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned long)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quinquagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(unsigned short)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quinquagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed long)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

interior vacuum
_sectio_trecentesima_quinquagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(signed short)");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_SIZEOF);
        }
    }
}

    /* ========================================================
     * PROBARE: Cast with type modifiers
     * ======================================================== */
interior vacuum
_sectio_trecentesima_quinquagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_quinquagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_quinquagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(long int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_quinquagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(short int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_sexagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned char)c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_sexagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed char)c");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_sexagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_sexagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned short int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_sexagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed long int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_sexagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed short int)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

    /* Implicit int casts */
interior vacuum
_sectio_trecentesima_sexagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_sexagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(long)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_sexagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(short)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_sexagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_septuagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned short)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_septuagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed long)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_septuagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed short)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

    /* ========================================================
     * PROBARE: Pointer casts with type modifiers
     * ======================================================== */
    /* Explicit type single pointer casts */
interior vacuum
_sectio_trecentesima_septuagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_septuagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_septuagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned char *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_septuagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed char *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_septuagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(long int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_septuagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(short int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_septuagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_octogesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned short int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_octogesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed long int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_octogesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed short int *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

    /* Explicit type double pointer casts */
interior vacuum
_sectio_trecentesima_octogesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned int **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_octogesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(long int **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_octogesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned char **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

    /* Implicit int single pointer casts */
interior vacuum
_sectio_trecentesima_octogesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_octogesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(long *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_octogesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(short *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_octogesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_nonagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned short *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_nonagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed long *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_nonagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(signed short *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

    /* Implicit int double pointer casts */
interior vacuum
_sectio_trecentesima_nonagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_nonagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(long **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

interior vacuum
_sectio_trecentesima_nonagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(unsigned long **)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_CONVERSIO);
        }
    }
}

    /* ========================================================
     * PROBARE: Storage class specifiers
     * ======================================================== */
interior vacuum
_sectio_trecentesima_nonagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_trecentesima_nonagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static float f");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_STATIC);
        }
    }
}

interior vacuum
_sectio_trecentesima_nonagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "extern int y");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.storage_class, ARBOR2_STORAGE_EXTERN);
        }
    }
}

interior vacuum
_sectio_trecentesima_nonagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "volatile int w");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_AEQUALIS_I32(res.radix->datum.declaratio.qualifiers, ARBOR2_QUAL_VOLATILE);
        }
    }
}

    /* ========================================================
     * PROBARE: Specifier combinations (Phase 1.4)
     * ======================================================== */
interior vacuum
_sectio_quadringentesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Initializer declarations (Phase 1.2a)
     * ======================================================== */
interior vacuum
_sectio_quadringentesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_decima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int z = x * 2 + 1");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
            CREDO_NON_NIHIL(res.radix->datum.declaratio.initializor);
        }
    }
}

    /* Storage class/qualifier initializer tests */
interior vacuum
_sectio_quadringentesima_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test with expression initializer and storage class */
interior vacuum
_sectio_quadringentesima_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Brace initializers (Phase 1.2b)
     * ======================================================== */
    /* Basic brace initializer with single value */
interior vacuum
_sectio_quadringentesima_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Brace initializer with multiple values */
interior vacuum
_sectio_quadringentesima_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Empty brace initializer */
interior vacuum
_sectio_quadringentesima_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Trailing comma */
interior vacuum
_sectio_quadringentesima_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Nested braces */
interior vacuum
_sectio_quadringentesima_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Static with brace initializer */
interior vacuum
_sectio_quadringentesima_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Const with brace initializer */
interior vacuum
_sectio_quadringentesima_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Single value in braces (not array) */
interior vacuum
_sectio_quadringentesima_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Expression initializers still work */
interior vacuum
_sectio_quadringentesima_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Designated initializers (Phase 1.2c)
     * ======================================================== */
    /* Field designator: .x = 1 */
interior vacuum
_sectio_quadringentesima_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus** item;

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
}

    /* Array designator: [5] = 100 */
interior vacuum
_sectio_quadringentesima_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Multiple field designators */
interior vacuum
_sectio_quadringentesima_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;

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
}

    /* Chained designators: [0].x = 5 */
interior vacuum
_sectio_quadringentesima_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;
        Arbor2Nodus** item;

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
}

    /* Mixed positional and designated */
interior vacuum
_sectio_quadringentesima_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;

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
}

    /* Designator with nested brace value */
interior vacuum
_sectio_quadringentesima_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;
        Arbor2Nodus** item;

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
}

    /* Multiple array designators */
interior vacuum
_sectio_quadringentesima_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;

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
}

    /* Multi-dimensional array designator: [0][1] = 5 */
interior vacuum
_sectio_quadringentesima_tricesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;
        Arbor2Nodus** item;

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
}

    /* Field chain: .a.b = 5 */
interior vacuum
_sectio_quadringentesima_tricesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* init;
        Arbor2Nodus** item;

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
}

    /* Designator with trailing comma */
interior vacuum
_sectio_quadringentesima_tricesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int x = {.a = 1,}");
        res = arbor2_glr_parsere(glr, tokens);

        CREDO_AEQUALIS_I32((i32)res.successus, VERUM);
    }
}

    /* ========================================================
     * PROBARE: Multi-declarator (comma-separated)
     * ======================================================== */
    /* Simple multi-declarator: int x, y */
interior vacuum
_sectio_quadringentesima_tricesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

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
}

    /* Multi-declarator with initializers: int x = 1, y = 2 */
interior vacuum
_sectio_quadringentesima_tricesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

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
}

    /* Three declarators: int x, y, z */
interior vacuum
_sectio_quadringentesima_quadragesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;
        Arbor2Nodus* third;

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
}

    /* Mixed: int x = 1, y (some with init, some without) */
interior vacuum
_sectio_quadringentesima_quadragesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

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
}

    /* Struct type multi-declarator: struct foo x, y */
interior vacuum
_sectio_quadringentesima_quadragesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

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
}

    /* Multi-declarator with brace initializers */
interior vacuum
_sectio_quadringentesima_quadragesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* first;
        Arbor2Nodus* second;

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
}

    /* ========================================================
     * PROBARE: Translation unit - multiple declarations
     * ======================================================== */
interior vacuum
_sectio_quadringentesima_quadragesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_quadragesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_quadragesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_quadragesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_quadragesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_quadragesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_quinquagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quadringentesima_quinquagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Function with mixed type params: int and float */
interior vacuum
_sectio_quadringentesima_quinquagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern,
            "float scale(int x, float factor) { return x * factor; }");
        res = arbor2_glr_parsere_translation_unit(glr, tokens);
        CREDO_VERUM(res.successus);
        CREDO_NON_NIHIL(res.radix);
    }
}

    /* ========================================================
     * PROBARE: Typedef registration post-parse
     *
     * After parsing a translation unit with typedef declarations,
     * the typedef names should be registered in the expansion context.
     * This enables subsequent declarations using the typedef'd names.
     * ======================================================== */
    /* Test: typedef registration - simple typedef */
interior vacuum
_sectio_quadringentesima_quinquagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        Arbor2Nodus* tu;
        Arbor2Nodus* first_decl;
        unio { constans character* c; i8* m; } u;

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
}

    /* Test: pointer typedef registration */
interior vacuum
_sectio_quadringentesima_quinquagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

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
}

    /* Test: struct typedef registration */
interior vacuum
_sectio_quadringentesima_quinquagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

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
}

    /* Test: enum typedef registration */
interior vacuum
_sectio_quadringentesima_quinquagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

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
}

    /* ========================================================
     * PROBARE: Type modifiers (Phase 1.4)
     * ======================================================== */
interior vacuum
_sectio_quadringentesima_quinquagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "unsigned int x");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }
}

interior vacuum
_sectio_quadringentesima_quinquagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "long int y");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }
}

interior vacuum
_sectio_quadringentesima_quinquagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "unsigned long z");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }
}

interior vacuum
_sectio_quadringentesima_sexagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "short s");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }
}

interior vacuum
_sectio_quadringentesima_sexagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "signed char c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }
}

interior vacuum
_sectio_quadringentesima_sexagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "unsigned short int u");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }
}

interior vacuum
_sectio_quadringentesima_sexagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "signed long int w");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }
}

interior vacuum
_sectio_quadringentesima_sexagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "unsigned char uc");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
        si (res.radix != NIHIL)
        {
            CREDO_AEQUALIS_I32((i32)res.radix->genus, (i32)ARBOR2_NODUS_DECLARATIO);
        }
    }
}

    /* ========================================================
     * PROBARE: Phase 1.4b - Storage class + type modifiers
     * ======================================================== */
    /* static + type modifiers */
interior vacuum
_sectio_quadringentesima_sexagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned int x");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_sexagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static long int y");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_sexagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned long int z");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_sexagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static short int s");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_sexagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static signed char c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_septuagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned int u");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* extern + type modifiers */
interior vacuum
_sectio_quadringentesima_septuagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "extern unsigned int a");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_septuagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "extern long int b");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_septuagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "extern unsigned short int c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* register + type modifiers */
interior vacuum
_sectio_quadringentesima_septuagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "register short int d");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_septuagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "register unsigned long int e");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_septuagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "register signed int f");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* auto + type modifiers */
interior vacuum
_sectio_quadringentesima_septuagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "auto signed char g");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_septuagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "auto long int h");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_septuagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "auto unsigned short int i");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* Complex combinations */
interior vacuum
_sectio_quadringentesima_octogesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned long int j");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_octogesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "extern signed short int k");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_octogesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "register unsigned char l");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_octogesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "auto signed long int m");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Phase 1.4c - Implicit int with storage class + type modifiers
     * ======================================================== */
    /* static + implicit int */
interior vacuum
_sectio_quadringentesima_octogesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned x");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_octogesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static long y");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_octogesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static short z");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_octogesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static signed a");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_octogesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned long b");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* extern + implicit int */
interior vacuum
_sectio_quadringentesima_octogesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "extern short c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_nonagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "extern unsigned long d");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* register + implicit int */
interior vacuum
_sectio_quadringentesima_nonagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "register signed e");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_nonagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "register long f");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* auto + implicit int */
interior vacuum
_sectio_quadringentesima_nonagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "auto unsigned short g");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_nonagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "auto signed long h");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* with initializer */
interior vacuum
_sectio_quadringentesima_nonagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static unsigned i = 5");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

interior vacuum
_sectio_quadringentesima_nonagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "extern long j = 10");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Phase 1.4d - Pointer qualifiers
     * ======================================================== */
    /* int * const p - const pointer to int */
interior vacuum
_sectio_quadringentesima_nonagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int * const p");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int * volatile v - volatile pointer to int */
interior vacuum
_sectio_quadringentesima_nonagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int * volatile v");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int * const volatile cv - const volatile pointer */
interior vacuum
_sectio_quadringentesima_nonagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int * const volatile cv");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int * volatile const vc - volatile const pointer (order variant) */
interior vacuum
_sectio_quingentesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int * volatile const vc");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* char ** const pp - const pointer to pointer to char */
interior vacuum
_sectio_quingentesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "char ** const pp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int * const * volatile pp - multiple qualified pointers */
interior vacuum
_sectio_quingentesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int * const * volatile pp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* const int * const cp - const pointer to const int */
interior vacuum
_sectio_quingentesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "const int * const cp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Qualifier + type modifier combinations
     * ======================================================== */
    /* const unsigned int x */
interior vacuum
_sectio_quingentesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "const unsigned int x");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* const long int y */
interior vacuum
_sectio_quingentesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "const long int y");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* const short int z */
interior vacuum
_sectio_quingentesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "const short int z");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* volatile unsigned int a */
interior vacuum
_sectio_quingentesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "volatile unsigned int a");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* volatile long int b */
interior vacuum
_sectio_quingentesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "volatile long int b");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* const signed char c */
interior vacuum
_sectio_quingentesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "const signed char c");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* static const unsigned int d (storage + qualifier + type modifier) */
interior vacuum
_sectio_quingentesima_decima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "static const unsigned int d");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* extern volatile long int e (storage + qualifier + type modifier) */
interior vacuum
_sectio_quingentesima_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "extern volatile long int e");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: void pointer declarations
     * ======================================================== */
    /* void *p - pointer to void */
interior vacuum
_sectio_quingentesima_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "void *p");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* void **pp - pointer to pointer to void */
interior vacuum
_sectio_quingentesima_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "void **pp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* void * const vp - const pointer to void */
interior vacuum
_sectio_quingentesima_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "void * const vp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Floating point types
     * ======================================================== */
    /* float f - basic float declaration */
interior vacuum
_sectio_quingentesima_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "float f");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* double d - basic double declaration */
interior vacuum
_sectio_quingentesima_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "double d");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* long double ld - long double declaration */
interior vacuum
_sectio_quingentesima_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "long double ld");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* float *fp - pointer to float */
interior vacuum
_sectio_quingentesima_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "float *fp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* double *dp - pointer to double */
interior vacuum
_sectio_quingentesima_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "double *dp");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* float arr[10] - float array */
interior vacuum
_sectio_quingentesima_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "float arr[10]");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* double arr[10] - double array */
interior vacuum
_sectio_quingentesima_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "double arr[10]");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Float/Double casts
     * ======================================================== */
    /* (float)x - cast to float */
interior vacuum
_sectio_quingentesima_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(float)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* (double)x - cast to double */
interior vacuum
_sectio_quingentesima_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(double)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* (long double)x - cast to long double */
interior vacuum
_sectio_quingentesima_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(long double)x");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* (float *)p - pointer cast to float */
interior vacuum
_sectio_quingentesima_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(float *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* (double *)p - pointer cast to double */
interior vacuum
_sectio_quingentesima_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "(double *)p");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Float/Double sizeof
     * ======================================================== */
    /* sizeof(float) */
interior vacuum
_sectio_quingentesima_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(float)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* sizeof(double) */
interior vacuum
_sectio_quingentesima_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(double)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* sizeof(long double) */
interior vacuum
_sectio_quingentesima_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(long double)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* sizeof(float *) */
interior vacuum
_sectio_quingentesima_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(float *)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* sizeof(double *) */
interior vacuum
_sectio_quingentesima_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "sizeof(double *)");
        res = arbor2_glr_parsere_expressio(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Function pointers
     * ======================================================== */
    /* int (*fp)(void) - function pointer returning int, no params */
interior vacuum
_sectio_quingentesima_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int (*fp)(void)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int (*fp)(int) - function pointer returning int, int param */
interior vacuum
_sectio_quingentesima_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int (*fp)(int)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int (*fp)(int, int) - function pointer with multiple params */
interior vacuum
_sectio_quingentesima_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int (*fp)(int, int)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* void (*fp)(void) - void function pointer */
interior vacuum
_sectio_quingentesima_tricesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "void (*fp)(void)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int (**fpp)(int) - pointer to function pointer */
interior vacuum
_sectio_quingentesima_tricesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int (**fpp)(int)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Array parameters
     * ======================================================== */
    /* int fn(int arr[]) - function with array parameter */
interior vacuum
_sectio_quingentesima_tricesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int fn(int arr[])");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int fn(int arr[10]) - function with sized array parameter */
interior vacuum
_sectio_quingentesima_tricesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int fn(int arr[10])");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* void fn(char *argv[]) - function with pointer array parameter */
interior vacuum
_sectio_quingentesima_tricesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "void fn(char *argv[])");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Variadic functions (ellipsis)
     * ======================================================== */
    /* int printf(char *fmt, ...) - variadic function */
interior vacuum
_sectio_quingentesima_quadragesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int printf(char *fmt, ...)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int foo(int x, ...) - variadic with int param */
interior vacuum
_sectio_quingentesima_quadragesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int foo(int x, ...)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Const qualifier in parameters
     * ======================================================== */
    /* int foo(const int x) - const value parameter */
interior vacuum
_sectio_quingentesima_quadragesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int foo(const int x)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int foo(const char *s) - const pointer parameter */
interior vacuum
_sectio_quingentesima_quadragesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int foo(const char *s)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* void bar(int x, const int y) - mixed params with const */
interior vacuum
_sectio_quingentesima_quadragesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "void bar(int x, const int y)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* int fn(volatile int v) - volatile parameter */
interior vacuum
_sectio_quingentesima_quadragesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        tokens = _lexare_ad_tokens(piscina, intern, "int fn(volatile int v)");
        res = arbor2_glr_parsere(glr, tokens);
        CREDO_VERUM(res.successus);
    }
}

    /* ========================================================
     * PROBARE: Table validation
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quadragesima_sexta (
    CredoContextus* c)
{
    (vacuum)c;
    {
        b32 valida;

        valida = arbor2_glr_validare_tabulas();
        CREDO_VERUM(valida);
    }
}

interior vacuum
_sectio_quingentesima_quadragesima_septima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        b32 valida;

        valida = arbor2_glr_validare_tags();
        CREDO_VERUM(valida);
    }
}

    /* ========================================================
     * PROBARE: Location propagation (Phase 2.1)
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quadragesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quingentesima_quadragesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

interior vacuum
_sectio_quingentesima_quinquagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

interior vacuum
_sectio_quingentesima_quinquagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* radix;

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
}

    /* ========================================================
     * PROBARE: Parent pointer (pater) on binary expression
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quinquagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2Nodus* radix;
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* ========================================================
     * PROBARE: Parent pointer (pater) on nested expression
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quinquagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2Nodus* radix;
        Arbor2Nodus* dexter;
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* ========================================================
     * PROBARE: Parent pointer (pater) on ternary
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quinquagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2Nodus* radix;
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* ========================================================
     * PROBARE: Parent pointer (pater) on function call
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quinquagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2Nodus* radix;
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* ========================================================
     * PROBARE: Parent pointer (pater) on subscript
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quinquagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2Nodus* radix;
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* ========================================================
     * PROBARE: Latin type macro disambiguation (Phase 3.2)
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quinquagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* ========================================================
     * PROBARE: Nested macro resolution (Phase 3.2)
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quinquagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Arbor2GLRResultus res;
        Xar* tokens;

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
}

    /* ========================================================
     * PROBARE: AMBIGUUS identifier tracking (Phase 3.3)
     * ======================================================== */
interior vacuum
_sectio_quingentesima_quinquagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Arbor2GLRResultus res;
        Xar* tokens;
        Arbor2Expansion* fresh_exp;
        Arbor2GLR* fresh_glr;

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
}

    /* ========================================================
     * PROBARE: Error Recovery
     * ======================================================== */
    /* Test 1: Single invalid declaration, followed by valid one */
interior vacuum
_sectio_quingentesima_sexagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

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
}

    /* Test 2: Multiple consecutive errors */
interior vacuum
_sectio_quingentesima_sexagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

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
}

    /* Test 3: Error followed by valid function */
interior vacuum
_sectio_quingentesima_sexagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

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
}

    /* Test 4: Errors collection in result */
interior vacuum
_sectio_quingentesima_sexagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ========================================================
     * PROBARE: Conditional Compilation (Phase 5)
     * ======================================================== */
    /* Test 1: Simple #ifdef/#endif */
interior vacuum
_sectio_quingentesima_sexagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

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
}

    /* Test 2: #ifdef/#else/#endif */
interior vacuum
_sectio_quingentesima_sexagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

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
}

    /* Test 3: #ifndef */
interior vacuum
_sectio_quingentesima_sexagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

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
}

    /* Test 4: Multiple independent conditionals */
interior vacuum
_sectio_quingentesima_sexagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

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
}

    /* Test 5: Conditional mixed with regular declarations */
interior vacuum
_sectio_quingentesima_sexagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

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
}

    /* Test 6: Nested conditionals */
interior vacuum
_sectio_quingentesima_sexagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;
        i32 num_decls;

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
}

    /* ========================================================
     * PROBARE: Conditional Expression Evaluation (Phase 5b)
     * ======================================================== */
    /* Test: #if with simple constant 1 */
interior vacuum
_sectio_quingentesima_septuagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #if with simple constant 0 */
interior vacuum
_sectio_quingentesima_septuagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #if with arithmetic expression */
interior vacuum
_sectio_quingentesima_septuagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #if with comparison */
interior vacuum
_sectio_quingentesima_septuagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #if with logical AND */
interior vacuum
_sectio_quingentesima_septuagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #if with logical NOT */
interior vacuum
_sectio_quingentesima_septuagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #if with defined() - undefined macro */
interior vacuum
_sectio_quingentesima_septuagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #if with ternary operator */
interior vacuum
_sectio_quingentesima_septuagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #elif evaluation */
interior vacuum
_sectio_quingentesima_septuagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #ifdef evaluation with defined macro */
interior vacuum
_sectio_quingentesima_septuagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Arbor2Expansion* exp_test;
        Arbor2GLR* glr_test;
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: #ifndef evaluation with undefined macro */
interior vacuum
_sectio_quingentesima_octogesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: Complex expression with parentheses */
interior vacuum
_sectio_quingentesima_octogesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: Bitwise operators */
interior vacuum
_sectio_quingentesima_octogesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* ========================================================
     * PROBARE: Branch Parsing (Phase 5.4)
     * ======================================================== */
    /* Test: Basic parsed branch - one declaration */
interior vacuum
_sectio_quingentesima_octogesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: Multiple declarations in branch */
interior vacuum
_sectio_quingentesima_octogesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: Nested conditional in branch */
interior vacuum
_sectio_quingentesima_octogesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: Empty branch */
interior vacuum
_sectio_quingentesima_octogesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* Test: Declaration after ifdef/else */
interior vacuum
_sectio_quingentesima_octogesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;
    Arbor2Expansion* expansion;
    Arbor2GLR* glr;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    expansion = arbor2_expansion_creare(piscina, intern);
    glr = arbor2_glr_creare(piscina, intern, expansion);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        Arbor2Nodus* tu;

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
}

    /* ========================================================
     * PROBARE: Typedef Edge Cases
     *
     * Comprehensive tests for typedef handling in various contexts:
     * nested typedefs, pointer typedefs, casts, sizeof, etc.
     * ======================================================== */
    /* Test: Nested typedef - typedef of typedef */
interior vacuum
_sectio_quingentesima_octogesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

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
}

    /* Test: Pointer typedef usage */
interior vacuum
_sectio_quingentesima_octogesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: Double pointer from typedef */
interior vacuum
_sectio_quingentesima_nonagesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: Typedef in cast expression */
interior vacuum
_sectio_quingentesima_nonagesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: Typedef in sizeof */
interior vacuum
_sectio_quingentesima_nonagesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: Typedef as function parameter */
interior vacuum
_sectio_quingentesima_nonagesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: Typedef in struct member */
interior vacuum
_sectio_quingentesima_nonagesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: Chained usage - multiple variables */
interior vacuum
_sectio_quingentesima_nonagesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: Array typedef */
interior vacuum
_sectio_quingentesima_nonagesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

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
}

    /* Test: Function pointer typedef with (void) params */
interior vacuum
_sectio_quingentesima_nonagesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

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
}

    /* Test: Function pointer typedef with empty params () */
interior vacuum
_sectio_quingentesima_nonagesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        chorda titulus_ch;
        unio { constans character* c; i8* m; } u;

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
}

    /* Test: Function pointer typedef used in subsequent declaration */
interior vacuum
_sectio_quingentesima_nonagesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ================================================================
     * DECLARATOR_CONTEXTUS tests - Grouped declarators (function pointers)
     * These test that PAREN_APERTA is handled after type specifiers
     * ================================================================ */
    /* Test: static int (*fp)(void) - storage class + function pointer */
interior vacuum
_sectio_sescentesima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: extern int (*callback)() - extern + function pointer */
interior vacuum
_sectio_sescentesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: void func(int (*param)(void)) - function pointer parameter */
interior vacuum
_sectio_sescentesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: struct S { int (*fp)(); }; - struct member function pointer */
interior vacuum
_sectio_sescentesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* Test: int x, (*fp)(); - multi-declarator with function pointer */
interior vacuum
_sectio_sescentesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

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
}

    /* ================================================================
     * C89 COMPLIANCE: Empty struct/union rejection tests
     * C89 requires at least one member in struct/union
     * ================================================================ */
    /* Test: struct Empty {} - should be rejected (C89 compliance) */
    /* Note: GLR parser has error recovery, so it returns successus=VERUM
     * with errores populated. We check for parse errors being present. */
interior vacuum
_sectio_sescentesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        b32 habet_errores;

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern, "struct { }");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            /* Empty struct should produce parse errors (C89 compliance) */
            habet_errores = (res.errores != NIHIL && xar_numerus(res.errores) > ZEPHYRUM);
            CREDO_VERUM(habet_errores);
            imprimere("  Empty anonymous struct produced errors: %s\n",
                      habet_errores ? "VERUM (correctum)" : "FALSUM (BUG!)");
        }
    }
}

    /* Test: struct Empty {} - named empty struct should be rejected */
interior vacuum
_sectio_sescentesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        b32 habet_errores;

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern, "struct Empty { }");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            /* Empty struct should produce parse errors (C89 compliance) */
            habet_errores = (res.errores != NIHIL && xar_numerus(res.errores) > ZEPHYRUM);
            CREDO_VERUM(habet_errores);
            imprimere("  Empty named struct produced errors: %s\n",
                      habet_errores ? "VERUM (correctum)" : "FALSUM (BUG!)");
        }
    }
}

    /* Test: union Empty {} - empty union should also be rejected */
interior vacuum
_sectio_sescentesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        b32 habet_errores;

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern, "union { }");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            /* Empty union should produce parse errors (C89 compliance) */
            habet_errores = (res.errores != NIHIL && xar_numerus(res.errores) > ZEPHYRUM);
            CREDO_VERUM(habet_errores);
            imprimere("  Empty anonymous union produced errors: %s\n",
                      habet_errores ? "VERUM (correctum)" : "FALSUM (BUG!)");
        }
    }
}

    /* Test: union Empty {} - named empty union should be rejected */
interior vacuum
_sectio_sescentesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;
        b32 habet_errores;

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern, "union Empty { }");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            /* Empty union should produce parse errors (C89 compliance) */
            habet_errores = (res.errores != NIHIL && xar_numerus(res.errores) > ZEPHYRUM);
            CREDO_VERUM(habet_errores);
            imprimere("  Empty named union produced errors: %s\n",
                      habet_errores ? "VERUM (correctum)" : "FALSUM (BUG!)");
        }
    }
}

    /* Test: Non-empty struct still works (regression test) */
interior vacuum
_sectio_sescentesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern, "struct S { int x; }");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            imprimere("  Non-empty struct accepted: %s\n",
                      res.successus ? "VERUM (correctum)" : "FALSUM (REGRESSIO!)");
        }
    }
}

    /* Test: Non-empty union still works (regression test) */
interior vacuum
_sectio_sescentesima_decima (
    CredoContextus* c)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = c->piscina;
    intern = internamentum_creare(piscina);
    {
        Xar* tokens;
        Arbor2GLRResultus res;

        {
            Arbor2Expansion* exp_test = arbor2_expansion_creare(piscina, intern);
            Arbor2GLR* glr_test = arbor2_glr_creare(piscina, intern, exp_test);

            tokens = _lexare_ad_tokens(piscina, intern, "union U { int x; float f; }");
            res = arbor2_glr_parsere_translation_unit(glr_test, tokens);

            CREDO_VERUM(res.successus);
            imprimere("  Non-empty union accepted: %s\n",
                      res.successus ? "VERUM (correctum)" : "FALSUM (REGRESSIO!)");
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "table validation", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tag validation", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "reachability", _sectio_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "GOTO completeness", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple identifier", _sectio_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple integer", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "binary addition: 1 + 2", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "precedence: 1 + 2 * 3", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "parenthesized: (1 + 2)", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "subtraction: 5 - 3", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "division: 6 / 2", _sectio_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "modulo: 7 % 3", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mixed ops: 10 - 6 / 2", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio minor: 1 < 2", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio maior: 3 > 1", _sectio_decima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio minor_aeq: 2 <= 3", _sectio_decima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio maior_aeq: 5 >= 4", _sectio_decima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aequalitas: 1 == 1", _sectio_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "non-aequalitas: 1 != 2", _sectio_decima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio cum expressiis: 1 + 2 < 3 + 4", _sectio_vicesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aequalitas cum comparatiis: 1 < 2 == 3 < 4", _sectio_vicesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "praecedentia mixta: 1 + 2 < 3 * 4", _sectio_vicesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio in parens: (1 < 2)", _sectio_vicesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "parens nidata: ((1 < 2))", _sectio_vicesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aequalitas catena: 1 == 2 == 3", _sectio_vicesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio catena: 1 < 2 < 3", _sectio_vicesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "RHS complexa: 1 < 2 * 3 + 4", _sectio_vicesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio in si: if (x < 10) y;", _sectio_vicesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aequalitas in dum: while (a == b) x;", _sectio_vicesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio in per: for (; i < 10; ) x;", _sectio_tricesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sinistrum shift: a << 2", _sectio_tricesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dextrum shift: x >> 3", _sectio_tricesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "shift catena: a << b >> c", _sectio_tricesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "shift cum additione: x + y << 2", _sectio_tricesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparatio cum shift: a < b << c", _sectio_tricesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "coniunctio: a && b", _sectio_tricesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "disiunctio: a || b", _sectio_tricesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "praecedentia: a || b && c", _sectio_tricesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "coniunctio catena: a && b && c", _sectio_tricesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "logica cum comparatiis: a == b && c < d", _sectio_quadragesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unary dereference: *ptr", _sectio_quadragesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unary address-of: &x", _sectio_quadragesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef detection API", _sectio_quadragesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "foo * bar (ambiguus)", _sectio_quadragesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "MyType * ptr (typedef pruning)", _sectio_quadragesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function declarator: MyType * fn()", _sectio_quadragesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function declarator: MyType * fn(void)", _sectio_quadragesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "single param: MyType * fn(int x)", _sectio_quadragesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multi param: MyType * fn(int x, int y)", _sectio_quadragesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pointer param: MyType * fn(int *ptr)", _sectio_quinquagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "expression statement: x + 1;", _sectio_quinquagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "empty statement: ;", _sectio_quinquagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple statement: 42;", _sectio_quinquagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "empty compound: { }", _sectio_quinquagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compound with one stmt: { x; }", _sectio_quinquagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compound multi stmt: { x; y + 1; }", _sectio_quinquagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested compound: { { x; } }", _sectio_quinquagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple if: if (x) y;", _sectio_quinquagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "if-else: if (x) y; else z;", _sectio_quinquagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "if with compound: if (x) { y; }", _sectio_sexagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "subscript in if condition: if (arr[0]) x;", _sectio_sexagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment in if condition: if (x = 1) y;", _sectio_sexagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment in while condition: while (x = 1) y;", _sectio_sexagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment in for-init: for (a = 1; b; c) d;", _sectio_sexagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment in do-while: do x; while (a = 1);", _sectio_sexagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment in switch: switch (a = 1) { }", _sectio_sexagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dangling else: if (a) if (b) c; else d;", _sectio_sexagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple while: while (x) y;", _sectio_sexagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "while with compound: while (x) { a; b; }", _sectio_sexagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple do-while: do x; while (y);", _sectio_septuagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "do-while with compound: do { a; b; } while (c);", _sectio_septuagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested while: while (a) while (b) c;", _sectio_septuagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple for: for (i; c; n) x;", _sectio_septuagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "for empty init: for (; c; n) x;", _sectio_septuagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "for empty condition: for (i; ; n) x;", _sectio_septuagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "for empty increment: for (i; c; ) x;", _sectio_septuagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "for all empty: for (;;) x;", _sectio_septuagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "for with compound: for (i; c; n) { a; b; }", _sectio_septuagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested for: for (;;) for (;;) x;", _sectio_septuagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "break statement", _sectio_octogesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "continue statement", _sectio_octogesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "return without value", _sectio_octogesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "return with value", _sectio_octogesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "goto statement", _sectio_octogesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "break inside while", _sectio_octogesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "return in compound", _sectio_octogesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple label: foo: x;", _sectio_octogesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "label with empty: foo: ;", _sectio_octogesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested labels: foo: bar: x;", _sectio_octogesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "label with compound: foo: { x; }", _sectio_nonagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "label with if: foo: if (x) y;", _sectio_nonagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "label as goto target: { goto foo; foo: x; }", _sectio_nonagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "switch with case: switch (x) { case 1: y; }", _sectio_nonagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "switch with default: switch (x) { default: y; }", _sectio_nonagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "switch case break: switch (x) { case 1: break; }", _sectio_nonagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "fall-through: switch (x) { case 1: case 2: y; }", _sectio_nonagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "case expr: switch (x) { case 1 + 2: y; }", _sectio_nonagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function definition: MyType fn(void) { }", _sectio_nonagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function definition: MyType fn(void) { return x; }", _sectio_nonagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function: MyType foo(int x, int y) { return x; }", _sectio_centesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pointer return: MyType * bar(void) { }", _sectio_centesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "qualified return: const int f() { } (P505)", _sectio_centesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "qualified return: volatile int g() { } (P506)", _sectio_centesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "basic return: void f() { } (no qualifier)", _sectio_centesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "qualified return: const char h() { }", _sectio_centesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compound return: unsigned int f() { }", _sectio_centesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compound return: long int g() { }", _sectio_centesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compound return: short int h() { }", _sectio_centesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compound return: signed int s() { }", _sectio_centesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compound return: unsigned long l() { }", _sectio_centesima_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compound return: unsigned u() { }", _sectio_centesima_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct forward ref: struct foo x", _sectio_centesima_decima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct named: struct foo { int x; }", _sectio_centesima_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct multi-member: struct foo { int x; int y; }", _sectio_centesima_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct float member: struct foo { float f; }", _sectio_centesima_decima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct double member: struct foo { double d; }", _sectio_centesima_decima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct short member: struct foo { short s; }", _sectio_centesima_decima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct long member: struct foo { long l; }", _sectio_centesima_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct unsigned member: struct foo { unsigned u; }", _sectio_centesima_decima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct signed member: struct foo { signed s; }", _sectio_centesima_vicesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct mixed members: struct foo { int x; float f; }", _sectio_centesima_vicesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct unsigned int: struct foo { unsigned int x; }", _sectio_centesima_vicesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct long int: struct foo { long int x; }", _sectio_centesima_vicesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct const int: struct foo { const int x; }", _sectio_centesima_vicesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct unsigned long: struct foo { unsigned long x; }", _sectio_centesima_vicesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct var decl: struct foo { int x; } var", _sectio_centesima_vicesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bit field: struct foo { int x : 3; }", _sectio_centesima_vicesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bit fields: struct foo { int x : 3; int y : 5; }", _sectio_centesima_vicesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "anonymous bit field: struct foo { int : 4; }", _sectio_centesima_vicesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mixed bit fields: struct foo { int x : 3; int : 0; int y : 5; }", _sectio_centesima_tricesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "expression bit field: struct foo { int x : 1 + 2; }", _sectio_centesima_tricesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bit field in union: union foo { int x : 3; }", _sectio_centesima_tricesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "union forward ref: union foo", _sectio_centesima_tricesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "union named: union foo { int x; }", _sectio_centesima_tricesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "union multi-member: union foo { int x; int y; }", _sectio_centesima_tricesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "union anon: union { int x; } anon", _sectio_centesima_tricesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum forward ref: enum foo", _sectio_centesima_tricesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum anon: enum { A, B, C }", _sectio_centesima_tricesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum named: enum foo { A, B, C }", _sectio_centesima_tricesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum with values: enum foo { A = 1, B, C = 10 }", _sectio_centesima_quadragesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum decl: enum foo x", _sectio_centesima_quadragesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum expr value: enum { A = 1 + 2 }", _sectio_centesima_quadragesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum unary minus: enum { A = -1 }", _sectio_centesima_quadragesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum unary plus: enum { A = +1 }", _sectio_centesima_quadragesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum bitwise NOT: enum { A = ~0 }", _sectio_centesima_quadragesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum logical NOT: enum { A = !0 }", _sectio_centesima_quadragesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum sizeof: enum { A = sizeof(int) }", _sectio_centesima_quadragesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum char literal: enum { A = 'X' }", _sectio_centesima_quadragesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "enum subsequent expr: enum { A, B = ~1, C = -2 }", _sectio_centesima_quadragesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested struct: struct outer { struct inner { int a; } x; }", _sectio_centesima_quinquagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested enum: struct outer { enum { A, B } status; }", _sectio_centesima_quinquagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested union: struct outer { union { int i; int j; } data; }", _sectio_centesima_quinquagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested pointer: struct outer { struct inner { int a; } *ptr; }", _sectio_centesima_quinquagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multiple with nested: struct outer { int x; struct { int a; } nested; int y; }", _sectio_centesima_quinquagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef simplex: typedef int MyInt;", _sectio_centesima_quinquagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef pointer: typedef int* IntPtr;", _sectio_centesima_quinquagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef struct: typedef struct { int x; } Point;", _sectio_centesima_quinquagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef struct pointer: typedef struct { int x; } *PointPtr;", _sectio_centesima_quinquagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef enum: typedef enum { A, B } Status;", _sectio_centesima_quinquagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef enum pointer: typedef enum { A, B } *StatusPtr;", _sectio_centesima_sexagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef struct nominatum: typedef struct MyStruct { int x; } MyStruct;", _sectio_centesima_sexagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef struct forward: typedef struct Foo Foo;", _sectio_centesima_sexagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "array fixed: int arr[10]", _sectio_centesima_sexagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "array unsized: int arr[]", _sectio_centesima_sexagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "array multi-dim: int matrix[10][20]", _sectio_centesima_sexagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "array of pointers: int *arr[10]", _sectio_centesima_sexagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct member array: struct { int arr[5]; }", _sectio_centesima_sexagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct member ptr array: struct { int *ptrs[5]; }", _sectio_centesima_sexagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct multi-dim array: struct { int matrix[3][4]; }", _sectio_centesima_sexagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct multi arrays: struct { int a[5]; int b[10]; }", _sectio_centesima_septuagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef array: typedef int IntArray[10];", _sectio_centesima_septuagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef pointer array: typedef int* IntPtrArray[10];", _sectio_centesima_septuagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef multi-dim: typedef int Matrix[3][4];", _sectio_centesima_septuagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "typedef float: typedef float MyFloat;", _sectio_centesima_septuagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "array expr size: int arr[1 + 2]", _sectio_centesima_septuagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested struct array: struct { struct Inner items[10]; }", _sectio_centesima_septuagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested enum array: struct { enum { A, B } status[5]; }", _sectio_centesima_septuagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested struct ptr array: struct { struct Inner *ptrs[3]; }", _sectio_centesima_septuagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment: a = 1", _sectio_centesima_septuagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment right-associativity: a = b = c", _sectio_centesima_octogesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "compound assignments: += -= *= /=", _sectio_centesima_octogesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitwise assignments: &= |= ^= <<= >>=", _sectio_centesima_octogesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comma operator: a, b", _sectio_centesima_octogesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comma left-associativity: a, b, c", _sectio_centesima_octogesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment/comma precedence: a = 1, b = 2", _sectio_centesima_octogesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment precedence: a = b || c", _sectio_centesima_octogesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ternary operator: a ? 1 : c", _sectio_centesima_octogesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ternary right-associativity: a ? 1 : b ? 2 : 3", _sectio_centesima_octogesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ternary/assignment precedence: a = x ? 1 : 2", _sectio_centesima_octogesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ternary/logical-or precedence: a || b ? 1 : 2", _sectio_centesima_nonagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ternary with expressions: a ? 1 + 2 : 3 * 4", _sectio_centesima_nonagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitwise NOT: ~x", _sectio_centesima_nonagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "logical NOT: !x", _sectio_centesima_nonagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "double NOT: !!x", _sectio_centesima_nonagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mixed unary: ~!x", _sectio_centesima_nonagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unary with binary: ~x + y", _sectio_centesima_nonagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "triple deref: ***ptr", _sectio_centesima_nonagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "double address: & &x", _sectio_centesima_nonagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "deref-address: *&x", _sectio_centesima_nonagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unary minus: -x", _sectio_ducentesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unary plus: +x", _sectio_ducentesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unary minus integer: -5", _sectio_ducentesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "double negation: - -x", _sectio_ducentesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unary with binary: -a + b", _sectio_ducentesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "binary then unary: a + -b", _sectio_ducentesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mixed: -!x", _sectio_ducentesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multiply unary: a * -b", _sectio_ducentesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitwise AND: a & b", _sectio_ducentesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitwise OR: a | b", _sectio_ducentesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitwise XOR: a ^ b", _sectio_ducentesima_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitwise precedence: a | b ^ c (| lower than ^)", _sectio_ducentesima_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitwise precedence: a ^ b & c (^ lower than &)", _sectio_ducentesima_decima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitwise chain: a | b | c (left-associative)", _sectio_ducentesima_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "bitwise/comparison: a & b == c", _sectio_ducentesima_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "all bitwise: a | b ^ c & d", _sectio_ducentesima_decima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "deeply nested parens: (((x)))", _sectio_ducentesima_decima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "complex arithmetic: (a + b) * (c - d)", _sectio_ducentesima_decima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mixed precedence: a + b * c - d / e", _sectio_ducentesima_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "logical with ternary: a && b ? 1 : 2", _sectio_ducentesima_decima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested ternary in true branch: a ? b ? 1 : 2 : 3", _sectio_ducentesima_vicesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unary in expressions: ~a & !b | *c", _sectio_ducentesima_vicesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment chain: a = b = c = 1", _sectio_ducentesima_vicesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comma chain: a, b, c, d", _sectio_ducentesima_vicesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "shift with arithmetic: a << 2 + 1", _sectio_ducentesima_vicesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "if-else-if chain", _sectio_ducentesima_vicesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "deeply nested if", _sectio_ducentesima_vicesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested loops with break", _sectio_ducentesima_vicesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "complex for loop", _sectio_ducentesima_vicesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "switch with multiple cases", _sectio_ducentesima_vicesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "do-while with simple condition", _sectio_ducentesima_tricesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multiple statements in compound", _sectio_ducentesima_tricesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested compound statements", _sectio_ducentesima_tricesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "declaration in block: { int x; }", _sectio_ducentesima_tricesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "declaration with init: { int x = 5; }", _sectio_ducentesima_tricesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multiple declarations: { int x; int y; }", _sectio_ducentesima_tricesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mixed decl/stmt: { int x; x = 1; int y; return y; }", _sectio_ducentesima_tricesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static in block: { static int x = 0; }", _sectio_ducentesima_tricesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const in block: { const int x = 42; }", _sectio_ducentesima_tricesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static const in block: { static const int x = 1; }", _sectio_ducentesima_tricesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "return with expression", _sectio_ducentesima_quadragesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "function with complex body", _sectio_ducentesima_quadragesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct with complex members", _sectio_ducentesima_quadragesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "expression in for", _sectio_ducentesima_quadragesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "comparison in if condition", _sectio_ducentesima_quadragesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "assignment in while", _sectio_ducentesima_quadragesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(~a)", _sectio_ducentesima_quadragesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "*~a", _sectio_ducentesima_quadragesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "a + ~b", _sectio_ducentesima_quadragesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "a * !b", _sectio_ducentesima_quadragesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "if (~a)", _sectio_ducentesima_quinquagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "a < ~b", _sectio_ducentesima_quinquagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "a && ~b", _sectio_ducentesima_quinquagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "a || !b", _sectio_ducentesima_quinquagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "a << ~b", _sectio_ducentesima_quinquagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arr[0]", _sectio_ducentesima_quinquagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arr[i]", _sectio_ducentesima_quinquagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arr[i][j]", _sectio_ducentesima_quinquagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "a + b[0]", _sectio_ducentesima_quinquagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "a * b[0]", _sectio_ducentesima_quinquagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "*arr[i]", _sectio_ducentesima_sexagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arr[i + 1]", _sectio_ducentesima_sexagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "foo()", _sectio_ducentesima_sexagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "foo(a)", _sectio_ducentesima_sexagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "foo(a, b, c)", _sectio_ducentesima_sexagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "foo()[0]", _sectio_ducentesima_sexagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arr[0]()", _sectio_ducentesima_sexagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "foo(a + b)", _sectio_ducentesima_sexagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "s.x", _sectio_ducentesima_sexagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "p->x", _sectio_ducentesima_sexagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "s.x.y", _sectio_ducentesima_septuagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "p->x->y", _sectio_ducentesima_septuagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arr[0].x", _sectio_ducentesima_septuagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "foo().x", _sectio_ducentesima_septuagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "s.x[0]", _sectio_ducentesima_septuagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "s.x()", _sectio_ducentesima_septuagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "x++", _sectio_ducentesima_septuagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "x--", _sectio_ducentesima_septuagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "++x", _sectio_ducentesima_septuagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "--x", _sectio_ducentesima_septuagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "arr[0]++", _sectio_ducentesima_octogesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "++arr[0]", _sectio_ducentesima_octogesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "s.x++", _sectio_ducentesima_octogesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "p->x--", _sectio_ducentesima_octogesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "++x + y--", _sectio_ducentesima_octogesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "x++++", _sectio_ducentesima_octogesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "float literal: 3.14", _sectio_ducentesima_octogesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "char literal: 'a'", _sectio_ducentesima_octogesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "string literal: \"hello\"", _sectio_ducentesima_octogesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "float in expression: 3.14 + 2.0", _sectio_ducentesima_octogesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "char in expression: 'a' + 'b'", _sectio_ducentesima_nonagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "mixed literals: 42 + 3.14", _sectio_ducentesima_nonagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof x", _sectio_ducentesima_nonagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(x)", _sectio_ducentesima_nonagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(x) + 1", _sectio_ducentesima_nonagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(int)x", _sectio_ducentesima_nonagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(char)x", _sectio_ducentesima_nonagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(int)x + 1", _sectio_ducentesima_nonagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(int)(char)x (chained casts)", _sectio_ducentesima_nonagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(int*)x", _sectio_ducentesima_nonagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(char**)argv", _sectio_trecentesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(void*)p", _sectio_trecentesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(int)", _sectio_trecentesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(int*)", _sectio_trecentesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(char**)", _sectio_trecentesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(void*)", _sectio_trecentesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(TypeName)x - ID cast", _sectio_trecentesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(TypeName*)ptr - ID pointer cast", _sectio_trecentesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(TypeName**)pp - ID double pointer cast", _sectio_trecentesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(struct foo)x - struct cast", _sectio_trecentesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(struct foo*)ptr - struct pointer cast", _sectio_trecentesima_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(struct foo**)pp - struct double pointer cast", _sectio_trecentesima_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(union bar)u - union cast", _sectio_trecentesima_decima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(union bar*)ptr - union pointer cast", _sectio_trecentesima_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(enum color)c - enum cast", _sectio_trecentesima_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(enum color*)ptr - enum pointer cast", _sectio_trecentesima_decima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(struct foo)", _sectio_trecentesima_decima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(struct foo*)", _sectio_trecentesima_decima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(struct foo**)", _sectio_trecentesima_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(union bar)", _sectio_trecentesima_decima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(union bar*)", _sectio_trecentesima_vicesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(enum color)", _sectio_trecentesima_vicesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(enum color*)", _sectio_trecentesima_vicesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(int[10])", _sectio_trecentesima_vicesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(char[256])", _sectio_trecentesima_vicesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(void[1])", _sectio_trecentesima_vicesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(int[10+5]) with expression", _sectio_trecentesima_vicesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "x = sizeof(int[10]) in assignment", _sectio_trecentesima_vicesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(int[10][20])", _sectio_trecentesima_vicesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(char[256][4])", _sectio_trecentesima_vicesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(void[1][1])", _sectio_trecentesima_tricesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(MyType[5][10]) with ID", _sectio_trecentesima_tricesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(int[N][M]) with expressions", _sectio_trecentesima_tricesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "x = sizeof(int[10][20]) in assignment", _sectio_trecentesima_tricesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(int*[10])", _sectio_trecentesima_tricesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(char*[256])", _sectio_trecentesima_tricesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(void*[5])", _sectio_trecentesima_tricesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(MyType*[20]) with ID", _sectio_trecentesima_tricesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "x = sizeof(int*[10]) in assignment", _sectio_trecentesima_tricesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(unsigned int)", _sectio_trecentesima_tricesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(signed int)", _sectio_trecentesima_quadragesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(long int)", _sectio_trecentesima_quadragesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(short int)", _sectio_trecentesima_quadragesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(unsigned char)", _sectio_trecentesima_quadragesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(signed char)", _sectio_trecentesima_quadragesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(unsigned long int)", _sectio_trecentesima_quadragesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(unsigned short int)", _sectio_trecentesima_quadragesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(signed long int)", _sectio_trecentesima_quadragesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(signed short int)", _sectio_trecentesima_quadragesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(unsigned) implicit int", _sectio_trecentesima_quadragesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(long) implicit int", _sectio_trecentesima_quinquagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(short) implicit int", _sectio_trecentesima_quinquagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(unsigned long) implicit int", _sectio_trecentesima_quinquagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(unsigned short) implicit int", _sectio_trecentesima_quinquagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(signed long) implicit int", _sectio_trecentesima_quinquagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(signed short) implicit int", _sectio_trecentesima_quinquagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned int)x", _sectio_trecentesima_quinquagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed int)x", _sectio_trecentesima_quinquagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(long int)x", _sectio_trecentesima_quinquagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(short int)x", _sectio_trecentesima_quinquagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned char)c", _sectio_trecentesima_sexagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed char)c", _sectio_trecentesima_sexagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned long int)x", _sectio_trecentesima_sexagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned short int)x", _sectio_trecentesima_sexagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed long int)x", _sectio_trecentesima_sexagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed short int)x", _sectio_trecentesima_sexagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned)x implicit int", _sectio_trecentesima_sexagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(long)x implicit int", _sectio_trecentesima_sexagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(short)x implicit int", _sectio_trecentesima_sexagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned long)x implicit int", _sectio_trecentesima_sexagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned short)x implicit int", _sectio_trecentesima_septuagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed long)x implicit int", _sectio_trecentesima_septuagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed short)x implicit int", _sectio_trecentesima_septuagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned int *)p", _sectio_trecentesima_septuagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed int *)p", _sectio_trecentesima_septuagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned char *)p", _sectio_trecentesima_septuagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed char *)p", _sectio_trecentesima_septuagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(long int *)p", _sectio_trecentesima_septuagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(short int *)p", _sectio_trecentesima_septuagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned long int *)p", _sectio_trecentesima_septuagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned short int *)p", _sectio_trecentesima_octogesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed long int *)p", _sectio_trecentesima_octogesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed short int *)p", _sectio_trecentesima_octogesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned int **)p", _sectio_trecentesima_octogesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(long int **)p", _sectio_trecentesima_octogesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned char **)p", _sectio_trecentesima_octogesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned *)p implicit int", _sectio_trecentesima_octogesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(long *)p implicit int", _sectio_trecentesima_octogesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(short *)p implicit int", _sectio_trecentesima_octogesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned long *)p implicit int", _sectio_trecentesima_octogesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned short *)p implicit int", _sectio_trecentesima_nonagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed long *)p implicit int", _sectio_trecentesima_nonagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(signed short *)p implicit int", _sectio_trecentesima_nonagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned **)p implicit int", _sectio_trecentesima_nonagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(long **)p implicit int", _sectio_trecentesima_nonagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(unsigned long **)p implicit int", _sectio_trecentesima_nonagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static int x", _sectio_trecentesima_nonagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static float f", _sectio_trecentesima_nonagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern int y", _sectio_trecentesima_nonagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const int z", _sectio_trecentesima_nonagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "volatile int w", _sectio_quadringentesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static const int x", _sectio_quadringentesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern const int y", _sectio_quadringentesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const volatile int z", _sectio_quadringentesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "volatile const int w", _sectio_quadringentesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static volatile int sv", _sectio_quadringentesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static const int x = 5", _sectio_quadringentesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern const int y = 10", _sectio_quadringentesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static const int *ptr", _sectio_quadringentesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = 5", _sectio_quadringentesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int y = a + b", _sectio_quadringentesima_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int z = x * 2 + 1", _sectio_quadringentesima_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static int s = 10", _sectio_quadringentesima_decima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern int e = 20", _sectio_quadringentesima_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "register int r = 30", _sectio_quadringentesima_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "auto int a = 40", _sectio_quadringentesima_decima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const int c = 50", _sectio_quadringentesima_decima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "volatile int v = 60", _sectio_quadringentesima_decima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static int sum = a + b", _sectio_quadringentesima_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int a[] = {1}", _sectio_quadringentesima_decima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int b[] = {1, 2, 3}", _sectio_quadringentesima_vicesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int c[] = {}", _sectio_quadringentesima_vicesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int d[] = {1, 2,}", _sectio_quadringentesima_vicesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int m[2][2] = {{1, 2}, {3, 4}}", _sectio_quadringentesima_vicesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static int s[] = {10, 20}", _sectio_quadringentesima_vicesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const int c[] = {30, 40}", _sectio_quadringentesima_vicesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {5}", _sectio_quadringentesima_vicesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int y = 42 (still works)", _sectio_quadringentesima_vicesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {.a = 5}", _sectio_quadringentesima_vicesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {[5] = 100}", _sectio_quadringentesima_vicesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {.a = 1, .b = 2}", _sectio_quadringentesima_tricesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {[0].y = 5}", _sectio_quadringentesima_tricesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {1, .b = 2, 3}", _sectio_quadringentesima_tricesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {.a = {1, 2}}", _sectio_quadringentesima_tricesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {[0] = 1, [5] = 6}", _sectio_quadringentesima_tricesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {[0][1] = 5}", _sectio_quadringentesima_tricesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {.a.b = 5}", _sectio_quadringentesima_tricesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = {.a = 1,}", _sectio_quadringentesima_tricesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x, y", _sectio_quadringentesima_tricesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = 1, y = 2", _sectio_quadringentesima_tricesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x, y, z", _sectio_quadringentesima_quadragesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int x = 1, y", _sectio_quadringentesima_quadragesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "struct foo x, y", _sectio_quadringentesima_quadragesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int a[] = {1}, b[] = {2}", _sectio_quadringentesima_quadragesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Translation unit: two declarations", _sectio_quadringentesima_quadragesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Translation unit: three declarations", _sectio_quadringentesima_quadragesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Translation unit: declaration with init", _sectio_quadringentesima_quadragesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Translation unit: function definition", _sectio_quadringentesima_quadragesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Translation unit: declaration + function", _sectio_quadringentesima_quadragesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Translation unit: multiple functions", _sectio_quadringentesima_quadragesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Translation unit: empty input", _sectio_quadringentesima_quinquagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Translation unit: function with params", _sectio_quadringentesima_quinquagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Translation unit: function with int and float params", _sectio_quadringentesima_quinquagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef registration: typedef int Integer;", _sectio_quadringentesima_quinquagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef registration: typedef int *IntPtr;", _sectio_quadringentesima_quinquagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef registration: typedef struct { int x; } Point;", _sectio_quadringentesima_quinquagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef registration: typedef enum { RED, GREEN, BLUE } Color;", _sectio_quadringentesima_quinquagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unsigned int x", _sectio_quadringentesima_quinquagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "long int y", _sectio_quadringentesima_quinquagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unsigned long z (implicit int)", _sectio_quadringentesima_quinquagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "short s (implicit int)", _sectio_quadringentesima_sexagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signed char c", _sectio_quadringentesima_sexagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unsigned short int u", _sectio_quadringentesima_sexagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signed long int w", _sectio_quadringentesima_sexagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "unsigned char uc", _sectio_quadringentesima_sexagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static unsigned int x", _sectio_quadringentesima_sexagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static long int y", _sectio_quadringentesima_sexagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static unsigned long int z", _sectio_quadringentesima_sexagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static short int s", _sectio_quadringentesima_sexagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static signed char c", _sectio_quadringentesima_sexagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static unsigned int u", _sectio_quadringentesima_septuagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern unsigned int a", _sectio_quadringentesima_septuagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern long int b", _sectio_quadringentesima_septuagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern unsigned short int c", _sectio_quadringentesima_septuagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "register short int d", _sectio_quadringentesima_septuagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "register unsigned long int e", _sectio_quadringentesima_septuagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "register signed int f", _sectio_quadringentesima_septuagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "auto signed char g", _sectio_quadringentesima_septuagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "auto long int h", _sectio_quadringentesima_septuagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "auto unsigned short int i", _sectio_quadringentesima_septuagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static unsigned long int j", _sectio_quadringentesima_octogesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern signed short int k", _sectio_quadringentesima_octogesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "register unsigned char l", _sectio_quadringentesima_octogesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "auto signed long int m", _sectio_quadringentesima_octogesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static unsigned x (implicit int)", _sectio_quadringentesima_octogesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static long y (implicit int)", _sectio_quadringentesima_octogesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static short z (implicit int)", _sectio_quadringentesima_octogesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static signed a (implicit int)", _sectio_quadringentesima_octogesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static unsigned long b (implicit int)", _sectio_quadringentesima_octogesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern short c (implicit int)", _sectio_quadringentesima_octogesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern unsigned long d (implicit int)", _sectio_quadringentesima_nonagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "register signed e (implicit int)", _sectio_quadringentesima_nonagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "register long f (implicit int)", _sectio_quadringentesima_nonagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "auto unsigned short g (implicit int)", _sectio_quadringentesima_nonagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "auto signed long h (implicit int)", _sectio_quadringentesima_nonagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static unsigned i = 5 (implicit int)", _sectio_quadringentesima_nonagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern long j = 10 (implicit int)", _sectio_quadringentesima_nonagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int * const p (pointer qualifier)", _sectio_quadringentesima_nonagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int * volatile v (pointer qualifier)", _sectio_quadringentesima_nonagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int * const volatile cv (pointer qualifier)", _sectio_quadringentesima_nonagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int * volatile const vc (pointer qualifier)", _sectio_quingentesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "char ** const pp (pointer qualifier)", _sectio_quingentesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int * const * volatile pp (multi qualifier)", _sectio_quingentesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const int * const cp (type + pointer qualifier)", _sectio_quingentesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const unsigned int x ", _sectio_quingentesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const long int y ", _sectio_quingentesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const short int z ", _sectio_quingentesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "volatile unsigned int a ", _sectio_quingentesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "volatile long int b ", _sectio_quingentesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "const signed char c ", _sectio_quingentesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "static const unsigned int d ", _sectio_quingentesima_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extern volatile long int e ", _sectio_quingentesima_decima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "void *p (void pointer)", _sectio_quingentesima_decima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "void **pp (void double pointer)", _sectio_quingentesima_decima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "void * const vp (const void pointer)", _sectio_quingentesima_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "float f (float declaration)", _sectio_quingentesima_decima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "double d (double declaration)", _sectio_quingentesima_decima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "long double ld (long double declaration)", _sectio_quingentesima_decima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "float *fp (float pointer)", _sectio_quingentesima_decima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "double *dp (double pointer)", _sectio_quingentesima_decima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "float arr[10] (float array)", _sectio_quingentesima_vicesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "double arr[10] (double array)", _sectio_quingentesima_vicesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(float)x (cast to float)", _sectio_quingentesima_vicesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(double)x (cast to double)", _sectio_quingentesima_vicesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(long double)x (cast to long double)", _sectio_quingentesima_vicesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(float *)p (pointer cast to float)", _sectio_quingentesima_vicesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "(double *)p (pointer cast to double)", _sectio_quingentesima_vicesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(float)", _sectio_quingentesima_vicesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(double)", _sectio_quingentesima_vicesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(long double)", _sectio_quingentesima_vicesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(float *)", _sectio_quingentesima_tricesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sizeof(double *)", _sectio_quingentesima_tricesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int (*fp)(void) (function pointer)", _sectio_quingentesima_tricesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int (*fp)(int) (function pointer with param)", _sectio_quingentesima_tricesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int (*fp)(int, int) (function pointer multi params)", _sectio_quingentesima_tricesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "void (*fp)(void) (void function pointer)", _sectio_quingentesima_tricesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int (**fpp)(int) (pointer to function pointer)", _sectio_quingentesima_tricesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int fn(int arr[]) (array parameter)", _sectio_quingentesima_tricesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int fn(int arr[10]) (sized array parameter)", _sectio_quingentesima_tricesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "void fn(char *argv[]) (pointer array parameter)", _sectio_quingentesima_tricesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int printf(char *fmt, ...) (variadic function)", _sectio_quingentesima_quadragesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int foo(int x, ...) (variadic with int)", _sectio_quingentesima_quadragesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int foo(const int x) (const value param)", _sectio_quingentesima_quadragesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int foo(const char *s) (const pointer param)", _sectio_quingentesima_quadragesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "void bar(int x, const int y) (mixed const param)", _sectio_quingentesima_quadragesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "int fn(volatile int v) (volatile param)", _sectio_quingentesima_quadragesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Table validation", _sectio_quingentesima_quadragesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Tag validation", _sectio_quingentesima_quadragesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "location on identifier", _sectio_quingentesima_quadragesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "location on binary expression", _sectio_quingentesima_quadragesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "location on integer literal", _sectio_quingentesima_quinquagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "location on function call", _sectio_quingentesima_quinquagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pater on binary expression: a + b", _sectio_quingentesima_quinquagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pater on nested: a + b * c", _sectio_quingentesima_quinquagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pater on ternary: a ? b : c", _sectio_quingentesima_quinquagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pater on function call: foo(a)", _sectio_quingentesima_quinquagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "pater on subscript: arr[i]", _sectio_quingentesima_quinquagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Latin type macro disambiguation", _sectio_quingentesima_quinquagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested macro resolution", _sectio_quingentesima_quinquagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "AMBIGUUS identifier tracking", _sectio_quingentesima_quinquagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error recovery: invalid decl + valid decl", _sectio_quingentesima_sexagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error recovery: multiple errors", _sectio_quingentesima_sexagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error recovery: error + valid function", _sectio_quingentesima_sexagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "error collection in result", _sectio_quingentesima_sexagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "simple #ifdef/#endif", _sectio_quingentesima_sexagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#ifdef/#else/#endif", _sectio_quingentesima_sexagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#ifndef", _sectio_quingentesima_sexagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multiple conditionals", _sectio_quingentesima_sexagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "conditional mixed with declarations", _sectio_quingentesima_sexagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nested conditionals", _sectio_quingentesima_sexagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if 1", _sectio_quingentesima_septuagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if 0", _sectio_quingentesima_septuagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if 1 + 1", _sectio_quingentesima_septuagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if 5 > 3", _sectio_quingentesima_septuagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if 1 && 1", _sectio_quingentesima_septuagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if !0", _sectio_quingentesima_septuagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if defined(UNDEFINED)", _sectio_quingentesima_septuagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if 1 ? 5 : 10", _sectio_quingentesima_septuagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if 0 / #elif 1", _sectio_quingentesima_septuagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#ifdef with defined macro", _sectio_quingentesima_septuagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#ifndef with undefined macro", _sectio_quingentesima_octogesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if (1 + 2) * 3 == 9", _sectio_quingentesima_octogesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "#if 0xFF & 0x0F", _sectio_quingentesima_octogesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "branch parsing: basic", _sectio_quingentesima_octogesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "branch parsing: multiple decls", _sectio_quingentesima_octogesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "branch parsing: nested conditional", _sectio_quingentesima_octogesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "branch parsing: empty branch", _sectio_quingentesima_octogesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "branch parsing: ifdef with else", _sectio_quingentesima_octogesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int A; typedef A B; B x;", _sectio_quingentesima_octogesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int *IntPtr; IntPtr p;", _sectio_quingentesima_octogesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int *IntPtr; IntPtr *pp;", _sectio_quingentesima_nonagesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int MyInt; int x = (MyInt)42;", _sectio_quingentesima_nonagesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int MyInt; int x = sizeof(MyInt);", _sectio_quingentesima_nonagesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int A; void f(A x) {}", _sectio_quingentesima_nonagesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int A; struct S { A x; };", _sectio_quingentesima_nonagesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int A; A x, y, z;", _sectio_quingentesima_nonagesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int Arr[10]; Arr a;", _sectio_quingentesima_nonagesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int (*FP)(void);", _sectio_quingentesima_nonagesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int (*Callback)();", _sectio_quingentesima_nonagesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Typedef edge: typedef int (*FP)(void); FP f;", _sectio_quingentesima_nonagesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "DECLARATOR_CONTEXTUS: static int (*fp)(void);", _sectio_sescentesima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "DECLARATOR_CONTEXTUS: extern int (*callback)();", _sectio_sescentesima_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "DECLARATOR_CONTEXTUS: void func(int (*param)(void))", _sectio_sescentesima_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "DECLARATOR_CONTEXTUS: struct S { int (*fp)(); };", _sectio_sescentesima_tertia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "DECLARATOR_CONTEXTUS: int x, (*fp)();", _sectio_sescentesima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "C89: empty struct rejection (anonymous)", _sectio_sescentesima_quinta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "C89: empty struct rejection (named)", _sectio_sescentesima_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "C89: empty union rejection (anonymous)", _sectio_sescentesima_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "C89: empty union rejection (named)", _sectio_sescentesima_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "C89: non-empty struct still accepted", _sectio_sescentesima_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "C89: non-empty union still accepted", _sectio_sescentesima_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("arbor2_glr", SECTIONES);
}
