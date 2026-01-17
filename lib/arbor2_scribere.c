/* arbor2_scribere.c - AST Serializer for Roundtrip
 *
 * Serializes an AST back to source code, preserving
 * all trivia (whitespace, comments) for exact roundtrip.
 */

#include "arbor2_scribere.h"
#include "arbor2_glr.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "piscina.h"
#include "xar.h"
#include "chorda.h"

/* ==================================================
 * Internal Helpers
 * ================================================== */

/* Append bytes to output buffer */
hic_manens vacuum
_appendere(Xar* output, constans i8* data, i32 mensura)
{
    i32 i;
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        i8* slot = xar_addere(output);
        *slot = data[i];
    }
}

/* Append chorda to output buffer */
hic_manens vacuum
_appendere_chordam(Xar* output, chorda* s)
{
    si (s != NIHIL && s->datum != NIHIL && s->mensura > ZEPHYRUM)
    {
        _appendere(output, s->datum, s->mensura);
    }
}

/* Emit spatia array (whitespace AND comments).
 * Phase 2.6: Iterates Arbor2Lexema* tokens instead of Arbor2Trivia*.
 * For roundtrip serialization, comments are emitted from token spatia.
 * Promoted comment nodes (commenta_ante/commenta_post) are for AST access,
 * not for serialization. */
hic_manens vacuum
_emittere_spatia(Xar* output, Xar* spatia)
{
    i32 i;
    i32 num;

    si (spatia == NIHIL)
    {
        redde;
    }

    num = xar_numerus(spatia);
    per (i = ZEPHYRUM; i < num; i++)
    {
        Arbor2Lexema** lex_ptr = xar_obtinere(spatia, i);
        si (lex_ptr != NIHIL && *lex_ptr != NIHIL)
        {
            _appendere_chordam(output, &(*lex_ptr)->valor);
        }
    }
}

/* Emit single token with spatia (whitespace/comments).
 * Phase 2.6: Uses spatia_ante/spatia_post instead of trivia. */
vacuum arbor2_scribere_lexema(Xar* output, Arbor2Token* token)
{
    si (token == NIHIL || token->lexema == NIHIL)
    {
        redde;
    }

    /* Emit leading spatia */
    _emittere_spatia(output, token->lexema->spatia_ante);

    /* Emit token text */
    _appendere_chordam(output, &token->lexema->valor);

    /* Emit trailing spatia */
    _emittere_spatia(output, token->lexema->spatia_post);
}

/* Forward declaration */
hic_manens vacuum _scribere_nodum(Xar* output, Arbor2Nodus* nodus);

/* Emit LISTA_SEPARATA (comma-separated list) */
hic_manens vacuum
_scribere_lista_separata(Xar* output, Arbor2Nodus* nodus)
{
    i32 i;
    i32 num_elem;
    i32 num_sep;

    si (nodus == NIHIL || nodus->genus != ARBOR2_NODUS_LISTA_SEPARATA)
    {
        redde;
    }

    si (nodus->datum.lista_separata.elementa == NIHIL)
    {
        redde;
    }

    num_elem = xar_numerus(nodus->datum.lista_separata.elementa);
    num_sep = nodus->datum.lista_separata.separatores != NIHIL
        ? xar_numerus(nodus->datum.lista_separata.separatores)
        : ZEPHYRUM;

    per (i = ZEPHYRUM; i < num_elem; i++)
    {
        Arbor2Nodus** elem = xar_obtinere(nodus->datum.lista_separata.elementa, i);
        _scribere_nodum(output, *elem);

        /* Emit separator if there's one after this element */
        si (i < num_sep)
        {
            Arbor2Token** sep = xar_obtinere(nodus->datum.lista_separata.separatores, i);
            arbor2_scribere_lexema(output, *sep);
        }
    }
}

/* Emit pointer levels (for declarators) */
hic_manens vacuum
_scribere_pointer_levels(Xar* output, Xar* pointer_levels)
{
    i32 i;
    i32 num;

    si (pointer_levels == NIHIL)
    {
        redde;
    }

    num = xar_numerus(pointer_levels);
    per (i = ZEPHYRUM; i < num; i++)
    {
        Arbor2PointerLevel** lvl = xar_obtinere(pointer_levels, i);
        si (*lvl != NIHIL)
        {
            arbor2_scribere_lexema(output, (*lvl)->tok_stella);
            si ((*lvl)->tok_const != NIHIL)
            {
                arbor2_scribere_lexema(output, (*lvl)->tok_const);
            }
            si ((*lvl)->tok_volatile != NIHIL)
            {
                arbor2_scribere_lexema(output, (*lvl)->tok_volatile);
            }
        }
    }
}

/* Forward declaration for mutual recursion */
hic_manens vacuum _scribere_nodum(Xar* output, Arbor2Nodus* nodus);

/* Emit attached comments (commenta_ante or commenta_post).
 * NOTE: Reserved for future reformatting scenarios where we need
 * to emit comments from their promoted locations.
 * For roundtrip, comments are emitted via token trivia.
 */
#if 0
hic_manens vacuum
_emittere_commenta(Xar* output, Xar* commenta)
{
    i32 i;
    i32 num;
    Arbor2Nodus** c_ptr;
    Arbor2Nodus* commentum;

    si (commenta == NIHIL)
    {
        redde;
    }

    num = xar_numerus(commenta);
    per (i = ZEPHYRUM; i < num; i++)
    {
        c_ptr = xar_obtinere(commenta, i);
        si (c_ptr != NIHIL && *c_ptr != NIHIL)
        {
            commentum = *c_ptr;
            si (commentum->genus == ARBOR2_NODUS_COMMENTUM)
            {
                /* Emit comment with its trivia */
                _scribere_nodum(output, commentum);
            }
        }
    }
}
#endif

/* Main node serializer */
hic_manens vacuum
_scribere_nodum(Xar* output, Arbor2Nodus* nodus)
{
    si (nodus == NIHIL)
    {
        redde;
    }

    /* NOTE: Do NOT emit commenta_ante here.
     * Promoted comments are for AST tooling access, not serialization.
     * Roundtrip serialization uses token trivia which preserves
     * exact positioning and whitespace. */

    commutatio (nodus->genus)
    {
        /* Literals - just emit the token */
        casus ARBOR2_NODUS_IDENTIFICATOR:
        casus ARBOR2_NODUS_INTEGER:
        casus ARBOR2_NODUS_FLOAT:
        casus ARBOR2_NODUS_CHAR:
        casus ARBOR2_NODUS_STRING:
            arbor2_scribere_lexema(output, nodus->lexema);
            frange;

        /* BINARIUM: sinister op dexter */
        casus ARBOR2_NODUS_BINARIUM:
            _scribere_nodum(output, nodus->datum.binarium.sinister);
            arbor2_scribere_lexema(output, nodus->datum.binarium.tok_operator);
            _scribere_nodum(output, nodus->datum.binarium.dexter);
            frange;

        /* UNARIUM: op operandum */
        casus ARBOR2_NODUS_UNARIUM:
            arbor2_scribere_lexema(output, nodus->datum.unarium.tok_operator);
            _scribere_nodum(output, nodus->datum.unarium.operandum);
            frange;

        /* POST_UNARIUM: operandum op */
        casus ARBOR2_NODUS_POST_UNARIUM:
            _scribere_nodum(output, nodus->datum.post_unarium.operandum);
            arbor2_scribere_lexema(output, nodus->datum.post_unarium.tok_operator);
            frange;

        /* PARENTHESIZED: (expr) */
        casus ARBOR2_NODUS_PARENTHESIZED:
            arbor2_scribere_lexema(output, nodus->datum.parenthesized.tok_paren_ap);
            _scribere_nodum(output, nodus->datum.parenthesized.expressio);
            arbor2_scribere_lexema(output, nodus->datum.parenthesized.tok_paren_cl);
            frange;

        /* CONVERSIO: (type) expr */
        casus ARBOR2_NODUS_CONVERSIO:
            arbor2_scribere_lexema(output, nodus->datum.conversio.tok_paren_ap);
            /* For cast types, emit type specifier then pointers (int* not *int) */
            si (nodus->datum.conversio.typus != NIHIL &&
                nodus->datum.conversio.typus->genus == ARBOR2_NODUS_DECLARATOR)
            {
                Arbor2Nodus* typus = nodus->datum.conversio.typus;
                /* Emit type specifier (lexema) first */
                arbor2_scribere_lexema(output, typus->lexema);
                /* Then emit pointer levels */
                _scribere_pointer_levels(output, typus->datum.declarator.pointer_levels);
            }
            alioquin
            {
                _scribere_nodum(output, nodus->datum.conversio.typus);
            }
            arbor2_scribere_lexema(output, nodus->datum.conversio.tok_paren_cl);
            _scribere_nodum(output, nodus->datum.conversio.expressio);
            frange;

        /* TERNARIUS: cond ? verum : falsum */
        casus ARBOR2_NODUS_TERNARIUS:
            _scribere_nodum(output, nodus->datum.ternarius.conditio);
            arbor2_scribere_lexema(output, nodus->datum.ternarius.tok_interrogatio);
            _scribere_nodum(output, nodus->datum.ternarius.verum);
            arbor2_scribere_lexema(output, nodus->datum.ternarius.tok_colon);
            _scribere_nodum(output, nodus->datum.ternarius.falsum);
            frange;

        /* SIZEOF: sizeof(type) or sizeof expr */
        casus ARBOR2_NODUS_SIZEOF:
            arbor2_scribere_lexema(output, nodus->datum.sizeof_expr.tok_sizeof);
            si (nodus->datum.sizeof_expr.tok_paren_ap != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.sizeof_expr.tok_paren_ap);
            }
            /* For sizeof(type), emit type specifier then pointers (int* not *int) */
            si (nodus->datum.sizeof_expr.est_typus &&
                nodus->datum.sizeof_expr.operandum != NIHIL &&
                nodus->datum.sizeof_expr.operandum->genus == ARBOR2_NODUS_DECLARATOR)
            {
                Arbor2Nodus* typus = nodus->datum.sizeof_expr.operandum;
                /* Emit type specifier (lexema) first */
                arbor2_scribere_lexema(output, typus->lexema);
                /* Then emit pointer levels */
                _scribere_pointer_levels(output, typus->datum.declarator.pointer_levels);
                /* Then array dimensions if any */
                si (typus->datum.declarator.dimensiones != NIHIL)
                {
                    i32 i;
                    i32 num = xar_numerus(typus->datum.declarator.dimensiones);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        Arbor2Nodus** dim = xar_obtinere(typus->datum.declarator.dimensiones, i);
                        _scribere_nodum(output, *dim);
                    }
                }
            }
            alioquin
            {
                _scribere_nodum(output, nodus->datum.sizeof_expr.operandum);
            }
            si (nodus->datum.sizeof_expr.tok_paren_cl != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.sizeof_expr.tok_paren_cl);
            }
            frange;

        /* VOCATIO: base(args) */
        casus ARBOR2_NODUS_VOCATIO:
            _scribere_nodum(output, nodus->datum.vocatio.basis);
            arbor2_scribere_lexema(output, nodus->datum.vocatio.tok_paren_ap);
            si (nodus->datum.vocatio.argumenta != NIHIL)
            {
                _scribere_lista_separata(output, nodus->datum.vocatio.argumenta);
            }
            arbor2_scribere_lexema(output, nodus->datum.vocatio.tok_paren_cl);
            frange;

        /* SUBSCRIPTIO: base[index] */
        casus ARBOR2_NODUS_SUBSCRIPTIO:
            _scribere_nodum(output, nodus->datum.subscriptio.basis);
            arbor2_scribere_lexema(output, nodus->datum.subscriptio.tok_bracket_ap);
            _scribere_nodum(output, nodus->datum.subscriptio.index);
            arbor2_scribere_lexema(output, nodus->datum.subscriptio.tok_bracket_cl);
            frange;

        /* MEMBRUM: base.member or base->member */
        casus ARBOR2_NODUS_MEMBRUM:
            _scribere_nodum(output, nodus->datum.membrum.basis);
            arbor2_scribere_lexema(output, nodus->datum.membrum.tok_accessor);
            arbor2_scribere_lexema(output, nodus->datum.membrum.tok_membrum);
            frange;

        /* DECLARATIO: [storage] [const] [volatile] type declarator [= init]; */
        casus ARBOR2_NODUS_DECLARATIO:
            si (nodus->datum.declaratio.tok_storage != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.declaratio.tok_storage);
            }
            si (nodus->datum.declaratio.tok_const != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.declaratio.tok_const);
            }
            si (nodus->datum.declaratio.tok_volatile != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.declaratio.tok_volatile);
            }
            _scribere_nodum(output, nodus->datum.declaratio.specifier);
            _scribere_nodum(output, nodus->datum.declaratio.declarator);
            si (nodus->datum.declaratio.tok_assignatio != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.declaratio.tok_assignatio);
                _scribere_nodum(output, nodus->datum.declaratio.initializor);
            }
            si (nodus->datum.declaratio.tok_semicolon != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.declaratio.tok_semicolon);
            }
            /* Handle chained declarations */
            si (nodus->datum.declaratio.proxima != NIHIL)
            {
                _scribere_nodum(output, nodus->datum.declaratio.proxima);
            }
            frange;

        /* DECLARATOR: *name or name */
        casus ARBOR2_NODUS_DECLARATOR:
            _scribere_pointer_levels(output, nodus->datum.declarator.pointer_levels);
            /* Emit identifier token */
            arbor2_scribere_lexema(output, nodus->lexema);
            /* Array dimensions */
            si (nodus->datum.declarator.dimensiones != NIHIL)
            {
                i32 i;
                i32 num = xar_numerus(nodus->datum.declarator.dimensiones);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    Arbor2ArrayDimension** dim_ptr = xar_obtinere(nodus->datum.declarator.dimensiones, i);
                    Arbor2ArrayDimension* dim = *dim_ptr;
                    arbor2_scribere_lexema(output, dim->tok_bracket_ap);
                    si (dim->dimensio != NIHIL)
                    {
                        _scribere_nodum(output, dim->dimensio);
                    }
                    arbor2_scribere_lexema(output, dim->tok_bracket_cl);
                }
            }
            /* Bit field */
            si (nodus->datum.declarator.latitudo_biti != NIHIL)
            {
                _scribere_nodum(output, nodus->datum.declarator.latitudo_biti);
            }
            frange;

        /* DECLARATOR_FUNCTI: name(params) */
        casus ARBOR2_NODUS_DECLARATOR_FUNCTI:
            _scribere_nodum(output, nodus->datum.declarator_functi.declarator_interior);
            arbor2_scribere_lexema(output, nodus->datum.declarator_functi.tok_paren_ap);
            si (nodus->datum.declarator_functi.parametri != NIHIL)
            {
                _scribere_lista_separata(output, nodus->datum.declarator_functi.parametri);
            }
            arbor2_scribere_lexema(output, nodus->datum.declarator_functi.tok_paren_cl);
            frange;

        /* PARAMETER_DECL: type name */
        casus ARBOR2_NODUS_PARAMETER_DECL:
            _scribere_nodum(output, nodus->datum.parameter_decl.type_specifier);
            _scribere_nodum(output, nodus->datum.parameter_decl.declarator);
            frange;

        /* DEFINITIO_FUNCTI: type name(params) { body } */
        casus ARBOR2_NODUS_DEFINITIO_FUNCTI:
            _scribere_nodum(output, nodus->datum.definitio_functi.specifier);
            _scribere_nodum(output, nodus->datum.definitio_functi.declarator);
            _scribere_nodum(output, nodus->datum.definitio_functi.corpus);
            frange;

        /* STRUCT_SPECIFIER: struct name { members } */
        casus ARBOR2_NODUS_STRUCT_SPECIFIER:
            arbor2_scribere_lexema(output, nodus->datum.struct_specifier.tok_struct_or_union);
            si (nodus->datum.struct_specifier.tag != NIHIL)
            {
                _scribere_nodum(output, nodus->datum.struct_specifier.tag);
            }
            si (nodus->datum.struct_specifier.tok_brace_ap != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.struct_specifier.tok_brace_ap);
                si (nodus->datum.struct_specifier.membra != NIHIL)
                {
                    i32 i;
                    i32 num = xar_numerus(nodus->datum.struct_specifier.membra);
                    per (i = ZEPHYRUM; i < num; i++)
                    {
                        Arbor2Nodus** member = xar_obtinere(nodus->datum.struct_specifier.membra, i);
                        _scribere_nodum(output, *member);
                    }
                }
                arbor2_scribere_lexema(output, nodus->datum.struct_specifier.tok_brace_cl);
            }
            frange;

        /* ENUM_SPECIFIER: enum name { enumerators } */
        casus ARBOR2_NODUS_ENUM_SPECIFIER:
            arbor2_scribere_lexema(output, nodus->datum.enum_specifier.tok_enum);
            si (nodus->datum.enum_specifier.tag != NIHIL)
            {
                _scribere_nodum(output, nodus->datum.enum_specifier.tag);
            }
            si (nodus->datum.enum_specifier.tok_brace_ap != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.enum_specifier.tok_brace_ap);
                si (nodus->datum.enum_specifier.enumeratores != NIHIL)
                {
                    _scribere_lista_separata(output, nodus->datum.enum_specifier.enumeratores);
                }
                arbor2_scribere_lexema(output, nodus->datum.enum_specifier.tok_brace_cl);
            }
            frange;

        /* ENUMERATOR: NAME or NAME = value */
        casus ARBOR2_NODUS_ENUMERATOR:
            arbor2_scribere_lexema(output, nodus->lexema);
            si (nodus->datum.enumerator.valor != NIHIL)
            {
                /* Note: We need the '=' token here but it's not stored in the AST.
                 * For now emit directly, but this is a limitation. */
                _scribere_nodum(output, nodus->datum.enumerator.valor);
            }
            frange;

        /* SENTENTIA: expr; */
        casus ARBOR2_NODUS_SENTENTIA:
            si (nodus->datum.sententia.expressio != NIHIL)
            {
                _scribere_nodum(output, nodus->datum.sententia.expressio);
            }
            arbor2_scribere_lexema(output, nodus->datum.sententia.tok_semicolon);
            frange;

        /* SENTENTIA_VACUA: ; */
        casus ARBOR2_NODUS_SENTENTIA_VACUA:
            arbor2_scribere_lexema(output, nodus->lexema);
            frange;

        /* CORPUS: { stmts } */
        casus ARBOR2_NODUS_CORPUS:
            arbor2_scribere_lexema(output, nodus->datum.corpus.tok_brace_ap);
            si (nodus->datum.corpus.sententiae != NIHIL)
            {
                i32 i;
                i32 num = xar_numerus(nodus->datum.corpus.sententiae);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    Arbor2Nodus** stmt = xar_obtinere(nodus->datum.corpus.sententiae, i);
                    _scribere_nodum(output, *stmt);
                }
            }
            arbor2_scribere_lexema(output, nodus->datum.corpus.tok_brace_cl);
            frange;

        /* SI: if (cond) consequens [else alternans] */
        casus ARBOR2_NODUS_SI:
            arbor2_scribere_lexema(output, nodus->datum.conditionale.tok_si);
            arbor2_scribere_lexema(output, nodus->datum.conditionale.tok_paren_ap);
            _scribere_nodum(output, nodus->datum.conditionale.conditio);
            arbor2_scribere_lexema(output, nodus->datum.conditionale.tok_paren_cl);
            _scribere_nodum(output, nodus->datum.conditionale.consequens);
            si (nodus->datum.conditionale.tok_alioquin != NIHIL)
            {
                arbor2_scribere_lexema(output, nodus->datum.conditionale.tok_alioquin);
                _scribere_nodum(output, nodus->datum.conditionale.alternans);
            }
            frange;

        /* DUM: while (cond) body */
        casus ARBOR2_NODUS_DUM:
            arbor2_scribere_lexema(output, nodus->datum.iteratio.tok_dum);
            arbor2_scribere_lexema(output, nodus->datum.iteratio.tok_paren_ap);
            _scribere_nodum(output, nodus->datum.iteratio.conditio);
            arbor2_scribere_lexema(output, nodus->datum.iteratio.tok_paren_cl);
            _scribere_nodum(output, nodus->datum.iteratio.corpus);
            frange;

        /* FAC: do body while (cond); */
        casus ARBOR2_NODUS_FAC:
            arbor2_scribere_lexema(output, nodus->datum.iteratio.tok_fac);
            _scribere_nodum(output, nodus->datum.iteratio.corpus);
            arbor2_scribere_lexema(output, nodus->datum.iteratio.tok_dum);
            arbor2_scribere_lexema(output, nodus->datum.iteratio.tok_paren_ap);
            _scribere_nodum(output, nodus->datum.iteratio.conditio);
            arbor2_scribere_lexema(output, nodus->datum.iteratio.tok_paren_cl);
            arbor2_scribere_lexema(output, nodus->datum.iteratio.tok_semicolon);
            frange;

        /* PER: for (init; cond; incr) body */
        casus ARBOR2_NODUS_PER:
            arbor2_scribere_lexema(output, nodus->datum.circuitus.tok_per);
            arbor2_scribere_lexema(output, nodus->datum.circuitus.tok_paren_ap);
            si (nodus->datum.circuitus.initium != NIHIL)
            {
                _scribere_nodum(output, nodus->datum.circuitus.initium);
            }
            arbor2_scribere_lexema(output, nodus->datum.circuitus.tok_semicolon1);
            si (nodus->datum.circuitus.conditio != NIHIL)
            {
                _scribere_nodum(output, nodus->datum.circuitus.conditio);
            }
            arbor2_scribere_lexema(output, nodus->datum.circuitus.tok_semicolon2);
            si (nodus->datum.circuitus.incrementum != NIHIL)
            {
                _scribere_nodum(output, nodus->datum.circuitus.incrementum);
            }
            arbor2_scribere_lexema(output, nodus->datum.circuitus.tok_paren_cl);
            _scribere_nodum(output, nodus->datum.circuitus.corpus);
            frange;

        /* REDDE: return [expr]; */
        casus ARBOR2_NODUS_REDDE:
            arbor2_scribere_lexema(output, nodus->datum.reditio.tok_redde);
            si (nodus->datum.reditio.valor != NIHIL)
            {
                _scribere_nodum(output, nodus->datum.reditio.valor);
            }
            arbor2_scribere_lexema(output, nodus->datum.reditio.tok_semicolon);
            frange;

        /* SALTA: goto label; */
        casus ARBOR2_NODUS_SALTA:
            arbor2_scribere_lexema(output, nodus->datum.saltus.tok_salta);
            arbor2_scribere_lexema(output, nodus->datum.saltus.tok_destinatio);
            arbor2_scribere_lexema(output, nodus->datum.saltus.tok_semicolon);
            frange;

        /* TITULATUM: label: stmt */
        casus ARBOR2_NODUS_TITULATUM:
            arbor2_scribere_lexema(output, nodus->datum.titulatum.tok_titulus);
            arbor2_scribere_lexema(output, nodus->datum.titulatum.tok_colon);
            _scribere_nodum(output, nodus->datum.titulatum.sententia);
            frange;

        /* COMMUTATIO: switch (expr) body */
        casus ARBOR2_NODUS_COMMUTATIO:
            arbor2_scribere_lexema(output, nodus->datum.selectivum.tok_commutatio);
            arbor2_scribere_lexema(output, nodus->datum.selectivum.tok_paren_ap);
            _scribere_nodum(output, nodus->datum.selectivum.expressio);
            arbor2_scribere_lexema(output, nodus->datum.selectivum.tok_paren_cl);
            _scribere_nodum(output, nodus->datum.selectivum.corpus);
            frange;

        /* CASUS: case expr: stmt */
        casus ARBOR2_NODUS_CASUS:
            arbor2_scribere_lexema(output, nodus->datum.electio.tok_casus);
            _scribere_nodum(output, nodus->datum.electio.valor);
            arbor2_scribere_lexema(output, nodus->datum.electio.tok_colon);
            _scribere_nodum(output, nodus->datum.electio.sententia);
            frange;

        /* ORDINARIUS: default: stmt */
        casus ARBOR2_NODUS_ORDINARIUS:
            arbor2_scribere_lexema(output, nodus->datum.defectus.tok_ordinarius);
            arbor2_scribere_lexema(output, nodus->datum.defectus.tok_colon);
            _scribere_nodum(output, nodus->datum.defectus.sententia);
            frange;

        /* FRANGE: break; */
        casus ARBOR2_NODUS_FRANGE:
            arbor2_scribere_lexema(output, nodus->datum.frangendum.tok_frange);
            arbor2_scribere_lexema(output, nodus->datum.frangendum.tok_semicolon);
            frange;

        /* PERGE: continue; */
        casus ARBOR2_NODUS_PERGE:
            arbor2_scribere_lexema(output, nodus->datum.pergendum.tok_perge);
            arbor2_scribere_lexema(output, nodus->datum.pergendum.tok_semicolon);
            frange;

        /* INITIALIZOR_LISTA: { items } */
        casus ARBOR2_NODUS_INITIALIZOR_LISTA:
            arbor2_scribere_lexema(output, nodus->datum.initializor_lista.tok_brace_ap);
            si (nodus->datum.initializor_lista.items != NIHIL)
            {
                _scribere_lista_separata(output, nodus->datum.initializor_lista.items);
            }
            arbor2_scribere_lexema(output, nodus->datum.initializor_lista.tok_brace_cl);
            frange;

        /* DESIGNATOR_ITEM: .x = value or [i] = value */
        casus ARBOR2_NODUS_DESIGNATOR_ITEM:
            si (nodus->datum.designator_item.designatores != NIHIL)
            {
                i32 i;
                i32 num = xar_numerus(nodus->datum.designator_item.designatores);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    Arbor2Nodus** des = xar_obtinere(nodus->datum.designator_item.designatores, i);
                    _scribere_nodum(output, *des);
                }
            }
            /* Note: '=' token not stored - limitation */
            _scribere_nodum(output, nodus->datum.designator_item.valor);
            frange;

        /* LISTA_SEPARATA: handled by _scribere_lista_separata */
        casus ARBOR2_NODUS_LISTA_SEPARATA:
            _scribere_lista_separata(output, nodus);
            frange;

        /* TRANSLATION_UNIT: list of declarations */
        casus ARBOR2_NODUS_TRANSLATION_UNIT:
            si (nodus->datum.translation_unit.declarationes != NIHIL)
            {
                i32 i;
                i32 num = xar_numerus(nodus->datum.translation_unit.declarationes);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    Arbor2Nodus** decl = xar_obtinere(nodus->datum.translation_unit.declarationes, i);
                    _scribere_nodum(output, *decl);
                }
            }
            frange;

        /* AMBIGUUS: pick first interpretation */
        casus ARBOR2_NODUS_AMBIGUUS:
            si (nodus->datum.ambiguus.interpretationes != NIHIL &&
                xar_numerus(nodus->datum.ambiguus.interpretationes) > ZEPHYRUM)
            {
                Arbor2Nodus** first = xar_obtinere(nodus->datum.ambiguus.interpretationes, ZEPHYRUM);
                _scribere_nodum(output, *first);
            }
            frange;

        /* ERROR: skip */
        casus ARBOR2_NODUS_ERROR:
            /* Error nodes can't be serialized - skip */
            frange;

        /* PARAMETER_LIST: intermediate node */
        casus ARBOR2_NODUS_PARAMETER_LIST:
            /* This should be converted to LISTA_SEPARATA in normal parsing.
             * Fallback: emit parameters directly */
            si (nodus->datum.parameter_list.parametra != NIHIL)
            {
                i32 i;
                i32 num = xar_numerus(nodus->datum.parameter_list.parametra);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    Arbor2Nodus** param = xar_obtinere(nodus->datum.parameter_list.parametra, i);
                    _scribere_nodum(output, *param);
                }
            }
            frange;

        /* COMMENTUM: emit spatia + raw comment text */
        casus ARBOR2_NODUS_COMMENTUM:
            /* Emit leading spatia (whitespace before comment) */
            _emittere_spatia(output, nodus->datum.commentum.spatia_ante);
            /* Emit the comment text */
            _appendere_chordam(output, &nodus->datum.commentum.textus_crudus);
            /* Emit trailing spatia (whitespace after comment) */
            _emittere_spatia(output, nodus->datum.commentum.spatia_post);
            frange;

        ordinarius:
            /* Unknown node type - skip */
            frange;
    }

    /* NOTE: Do NOT emit commenta_post here - see commenta_ante note above. */
}

/* ==================================================
 * Public API
 * ================================================== */

chorda* arbor2_scribere(Piscina* piscina, Arbor2Nodus* radix)
{
    Xar* output;
    chorda* resultus;
    i8* datum;
    i32 mensura;

    si (radix == NIHIL)
    {
        resultus = piscina_allocare(piscina, magnitudo(chorda));
        resultus->datum = NIHIL;
        resultus->mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Use Xar as growable byte buffer */
    output = xar_creare(piscina, magnitudo(i8));

    /* Serialize the tree */
    _scribere_nodum(output, radix);

    /* Convert to chorda */
    mensura = xar_numerus(output);
    datum = piscina_allocare(piscina, mensura);
    {
        i32 i;
        per (i = ZEPHYRUM; i < mensura; i++)
        {
            i8* byte = xar_obtinere(output, i);
            datum[i] = *byte;
        }
    }

    resultus = piscina_allocare(piscina, magnitudo(chorda));
    resultus->datum = datum;
    resultus->mensura = mensura;

    redde resultus;
}
