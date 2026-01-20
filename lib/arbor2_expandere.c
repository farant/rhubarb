/* ==================================================
 * ARBOR2 EXPANDERE - Macro Expansion Engine Implementation
 *
 * Segment-based layered expansion with full provenance.
 * ================================================== */

#include "arbor2_expandere.h"
#include "arbor2_glr.h"
#include "via.h"
#include "chorda_aedificator.h"
#include <string.h>
#include <stdio.h>

/* ==================================================
 * Internal Helper - Chorda Utilities
 * ================================================== */

interior chorda
_chorda_ex_cstring(constans character* s, InternamentumChorda* intern)
{
    chorda temp;
    chorda* internata;
    unio { constans character* c; i8* m; } u;

    si (s == NIHIL)
    {
        temp.datum = NIHIL;
        temp.mensura = ZEPHYRUM;
        redde temp;
    }

    u.c = s;
    temp.datum = u.m;
    temp.mensura = (i32)strlen(s);

    internata = chorda_internare(intern, temp);
    redde *internata;
}

interior b32
_chorda_aequalis(chorda a, chorda b)
{
    si (a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    si (a.mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde memcmp(a.datum, b.datum, (size_t)a.mensura) == ZEPHYRUM;
}

/* Phase 2.7: Check if token has NOVA_LINEA in spatia_post.
 * Used for directive boundary detection now that NOVA_LINEA
 * is collected as spatia instead of main stream tokens. */
interior b32
_habet_nova_linea_post(Arbor2Token* tok)
{
    Arbor2Lexema** spatium_ptr;
    Arbor2Lexema* spatium;
    i32 i;
    i32 num;

    si (tok == NIHIL || tok->lexema == NIHIL || tok->lexema->spatia_post == NIHIL)
    {
        redde FALSUM;
    }

    num = xar_numerus(tok->lexema->spatia_post);
    per (i = ZEPHYRUM; i < num; i++)
    {
        spatium_ptr = xar_obtinere(tok->lexema->spatia_post, i);
        si (spatium_ptr != NIHIL && *spatium_ptr != NIHIL)
        {
            spatium = *spatium_ptr;
            si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
            {
                redde VERUM;
            }
        }
    }

    redde FALSUM;
}

/* Phase 2.7: Check if token has NOVA_LINEA in spatia_ante.
 * Used to detect if a token is at start of a new line. */
interior b32
_habet_nova_linea_ante(Arbor2Token* tok)
{
    Arbor2Lexema** spatium_ptr;
    Arbor2Lexema* spatium;
    i32 i;
    i32 num;

    si (tok == NIHIL || tok->lexema == NIHIL || tok->lexema->spatia_ante == NIHIL)
    {
        redde FALSUM;
    }

    num = xar_numerus(tok->lexema->spatia_ante);
    per (i = ZEPHYRUM; i < num; i++)
    {
        spatium_ptr = xar_obtinere(tok->lexema->spatia_ante, i);
        si (spatium_ptr != NIHIL && *spatium_ptr != NIHIL)
        {
            spatium = *spatium_ptr;
            si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
            {
                redde VERUM;
            }
        }
    }

    redde FALSUM;
}

/* ==================================================
 * Context Creation
 * ================================================== */

Arbor2Expansion*
arbor2_expansion_creare(
    Piscina*                piscina,
    InternamentumChorda*    intern)
{
    Arbor2Expansion* exp;

    exp = piscina_allocare(piscina, magnitudo(Arbor2Expansion));
    exp->piscina = piscina;
    exp->intern = intern;

    exp->macros = tabula_dispersa_creare_chorda(piscina, 256);
    exp->typedefs = tabula_dispersa_creare_chorda(piscina, 64);

    exp->layers = xar_creare(piscina, magnitudo(Arbor2Layer*));
    exp->segmenta = xar_creare(piscina, magnitudo(Arbor2Segmentum*));
    exp->macro_stack = xar_creare(piscina, magnitudo(chorda*));

    exp->included_viae = tabula_dispersa_creare_chorda(piscina, LXIV);
    exp->include_modus = ARBOR2_INCLUDE_RESET_PER_PARSE;

    /* Include search paths */
    exp->system_viae = xar_creare(piscina, magnitudo(chorda*));
    exp->local_viae = xar_creare(piscina, magnitudo(chorda*));
    exp->system_learning_default = VERUM;   /* System includes are learning by default */
    exp->local_learning_default = FALSUM;   /* Local includes are full by default */

    exp->via_current = NIHIL;

    redde exp;
}

vacuum
arbor2_expansion_ponere_include_modum(
    Arbor2Expansion*        exp,
    Arbor2IncludeModus      modus)
{
    exp->include_modus = modus;
}

/* ==================================================
 * Macro Management
 * ================================================== */

vacuum
arbor2_expansion_addere_macro(
    Arbor2Expansion*        exp,
    constans character*     nomen_macro,
    constans character*     valor,
    constans character*     via_file)
{
    Arbor2MacroDef* def;
    chorda nomen_ch;
    chorda* nomen_ptr;
    Arbor2Lexator* lex;
    Xar* corpus;
    Arbor2Lexema* lexema;
    Arbor2Lexema** locus;

    def = piscina_allocare(exp->piscina, magnitudo(Arbor2MacroDef));

    nomen_ch = _chorda_ex_cstring(nomen_macro, exp->intern);
    nomen_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
    *nomen_ptr = nomen_ch;
    def->titulus = nomen_ptr;

    def->est_functio = FALSUM;
    def->est_variadic = FALSUM;
    def->parametra = NIHIL;

    /* Lex the value string */
    corpus = xar_creare(exp->piscina, magnitudo(Arbor2Lexema*));
    si (valor != NIHIL && strlen(valor) > ZEPHYRUM)
    {
        lex = arbor2_lexator_creare(exp->piscina, exp->intern, valor, (i32)strlen(valor));
        fac
        {
            lexema = arbor2_lexema_proximum(lex);
            si (lexema->genus != ARBOR2_LEXEMA_EOF &&
                lexema->genus != ARBOR2_LEXEMA_ERROR &&
                lexema->genus != ARBOR2_LEXEMA_NOVA_LINEA)
            {
                locus = xar_addere(corpus);
                si (locus != NIHIL)
                {
                    *locus = lexema;
                }
            }
        }
        dum (lexema->genus != ARBOR2_LEXEMA_EOF && lexema->genus != ARBOR2_LEXEMA_ERROR);
    }
    def->corpus = corpus;

    si (via_file != NIHIL)
    {
        chorda via_ch;
        chorda* via_ptr;
        via_ch = _chorda_ex_cstring(via_file, exp->intern);
        via_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
        *via_ptr = via_ch;
        def->via_file = via_ptr;
    }
    alioquin
    {
        def->via_file = NIHIL;
    }
    def->linea_def = -I;
    def->ex_api = VERUM;

    /* Add to table */
    tabula_dispersa_inserere(exp->macros, nomen_ch, def);
}

vacuum
arbor2_expansion_addere_macro_functio(
    Arbor2Expansion*        exp,
    constans character*     nomen_macro,
    constans character**    parametra,
    constans character*     corpus,
    constans character*     via_file)
{
    Arbor2MacroDef* def;
    chorda nomen_ch;
    chorda* nomen_ptr;
    Arbor2Lexator* lex;
    Xar* corpus_xar;
    Xar* param_xar;
    Arbor2Lexema* lexema;
    Arbor2Lexema** locus;
    i32 i;

    def = piscina_allocare(exp->piscina, magnitudo(Arbor2MacroDef));

    nomen_ch = _chorda_ex_cstring(nomen_macro, exp->intern);
    nomen_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
    *nomen_ptr = nomen_ch;
    def->titulus = nomen_ptr;

    def->est_functio = VERUM;
    def->est_variadic = FALSUM;

    /* Parse parameters */
    param_xar = xar_creare(exp->piscina, magnitudo(chorda*));
    si (parametra != NIHIL)
    {
        per (i = ZEPHYRUM; parametra[i] != NIHIL; i++)
        {
            chorda param_ch;
            chorda* param_ptr;
            chorda** param_locus;

            /* Check for variadic */
            si (strcmp(parametra[i], "...") == ZEPHYRUM)
            {
                def->est_variadic = VERUM;
                perge;
            }

            param_ch = _chorda_ex_cstring(parametra[i], exp->intern);
            param_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
            *param_ptr = param_ch;
            param_locus = xar_addere(param_xar);
            si (param_locus != NIHIL)
            {
                *param_locus = param_ptr;
            }
        }
    }
    def->parametra = param_xar;

    /* Lex the body string */
    corpus_xar = xar_creare(exp->piscina, magnitudo(Arbor2Lexema*));
    si (corpus != NIHIL && strlen(corpus) > ZEPHYRUM)
    {
        lex = arbor2_lexator_creare(exp->piscina, exp->intern, corpus, (i32)strlen(corpus));
        fac
        {
            lexema = arbor2_lexema_proximum(lex);
            si (lexema->genus != ARBOR2_LEXEMA_EOF &&
                lexema->genus != ARBOR2_LEXEMA_ERROR &&
                lexema->genus != ARBOR2_LEXEMA_NOVA_LINEA)
            {
                locus = xar_addere(corpus_xar);
                si (locus != NIHIL)
                {
                    *locus = lexema;
                }
            }
        }
        dum (lexema->genus != ARBOR2_LEXEMA_EOF && lexema->genus != ARBOR2_LEXEMA_ERROR);
    }
    def->corpus = corpus_xar;

    si (via_file != NIHIL)
    {
        chorda via_ch;
        chorda* via_ptr;
        via_ch = _chorda_ex_cstring(via_file, exp->intern);
        via_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
        *via_ptr = via_ch;
        def->via_file = via_ptr;
    }
    alioquin
    {
        def->via_file = NIHIL;
    }
    def->linea_def = -I;
    def->ex_api = VERUM;

    tabula_dispersa_inserere(exp->macros, nomen_ch, def);
}

vacuum
arbor2_expansion_addere_typedef(
    Arbor2Expansion*        exp,
    constans character*     nomen_typedef)
{
    chorda nomen_ch;
    Arbor2TypedefInfo* info;

    nomen_ch = _chorda_ex_cstring(nomen_typedef, exp->intern);

    info = piscina_allocare(exp->piscina, magnitudo(Arbor2TypedefInfo));
    info->titulus = piscina_allocare(exp->piscina, magnitudo(chorda));
    *(info->titulus) = nomen_ch;
    info->layer_discovered = -I;  /* API-injected marker */
    info->origo = NIHIL;
    info->est_certum = VERUM;

    tabula_dispersa_inserere(exp->typedefs, nomen_ch, info);
}

/* ==================================================
 * Include Path Management
 * ================================================== */

vacuum
arbor2_expansion_addere_system_via(
    Arbor2Expansion*        exp,
    constans character*     via)
{
    chorda via_ch;
    chorda* via_ptr;
    chorda** locus;

    si (via == NIHIL || exp == NIHIL)
    {
        redde;
    }

    via_ch = _chorda_ex_cstring(via, exp->intern);
    via_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
    *via_ptr = via_ch;

    locus = xar_addere(exp->system_viae);
    si (locus != NIHIL)
    {
        *locus = via_ptr;
    }
}

vacuum
arbor2_expansion_addere_local_via(
    Arbor2Expansion*        exp,
    constans character*     via)
{
    chorda via_ch;
    chorda* via_ptr;
    chorda** locus;

    si (via == NIHIL || exp == NIHIL)
    {
        redde;
    }

    via_ch = _chorda_ex_cstring(via, exp->intern);
    via_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
    *via_ptr = via_ch;

    locus = xar_addere(exp->local_viae);
    si (locus != NIHIL)
    {
        *locus = via_ptr;
    }
}

/* Try to resolve a path against a directory */
interior chorda*
_tentare_resolvere(Arbor2Expansion* exp, chorda* directorium, chorda via_specifier)
{
    chorda partes[II];
    chorda via_candidata;
    chorda* resultus;

    si (directorium == NIHIL || directorium->mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }

    partes[ZEPHYRUM] = *directorium;
    partes[I] = via_specifier;

    via_candidata = via_iungere(partes, II, exp->piscina);

    si (via_existit(via_candidata))
    {
        /* Normalize the path */
        via_candidata = via_normalizare(via_candidata, exp->piscina);

        resultus = piscina_allocare(exp->piscina, magnitudo(chorda));
        *resultus = via_candidata;
        redde resultus;
    }

    redde NIHIL;
}

chorda*
arbor2_expansion_resolvere_include(
    Arbor2Expansion*        exp,
    chorda                  via_specifier,
    i32                     genus,
    chorda*                 via_current)
{
    chorda* resultus;
    i32 i;
    i32 num;
    chorda** via_ptr;
    chorda dir_current;

    /* Local "" includes: first check relative to current file */
    si (genus == ARBOR2_INCLUDE_LOCAL && via_current != NIHIL)
    {
        dir_current = via_directorium(*via_current, exp->piscina);
        resultus = _tentare_resolvere(exp, &dir_current, via_specifier);
        si (resultus != NIHIL)
        {
            redde resultus;
        }
    }

    /* Local "" includes: search local_viae */
    si (genus == ARBOR2_INCLUDE_LOCAL)
    {
        num = xar_numerus(exp->local_viae);
        per (i = ZEPHYRUM; i < num; i++)
        {
            via_ptr = xar_obtinere(exp->local_viae, i);
            resultus = _tentare_resolvere(exp, *via_ptr, via_specifier);
            si (resultus != NIHIL)
            {
                redde resultus;
            }
        }
    }

    /* All includes: search system_viae */
    num = xar_numerus(exp->system_viae);
    per (i = ZEPHYRUM; i < num; i++)
    {
        via_ptr = xar_obtinere(exp->system_viae, i);
        resultus = _tentare_resolvere(exp, *via_ptr, via_specifier);
        si (resultus != NIHIL)
        {
            redde resultus;
        }
    }

    redde NIHIL;
}

/* ==================================================
 * Macro Queries
 * ================================================== */

Arbor2MacroDef*
arbor2_expansion_quaerere_macro(
    Arbor2Expansion*        exp,
    chorda                  titulus)
{
    vacuum* valor;
    si (tabula_dispersa_invenire(exp->macros, titulus, &valor))
    {
        redde (Arbor2MacroDef*)valor;
    }
    redde NIHIL;
}

b32
arbor2_expansion_est_typedef(
    Arbor2Expansion*        exp,
    chorda                  titulus)
{
    redde tabula_dispersa_continet(exp->typedefs, titulus);
}

Arbor2TypedefInfo*
arbor2_expansion_quaerere_typedef(
    Arbor2Expansion*        exp,
    chorda                  titulus)
{
    vacuum* valor;
    si (tabula_dispersa_invenire(exp->typedefs, titulus, &valor))
    {
        redde (Arbor2TypedefInfo*)valor;
    }
    redde NIHIL;
}

/* ==================================================
 * Macro Lookahead
 *
 * Inspect what a macro would expand to without full expansion.
 * ================================================== */

Arbor2LexemaGenus
arbor2_expansion_lookahead_genus(
    Arbor2Expansion*        exp,
    chorda                  nomen_macro)
{
    Arbor2MacroDef* def;
    Arbor2Lexema* primus;

    def = arbor2_expansion_quaerere_macro(exp, nomen_macro);
    si (def == NIHIL)
    {
        redde ARBOR2_LEXEMA_ERROR;
    }

    si (xar_numerus(def->corpus) == ZEPHYRUM)
    {
        redde ARBOR2_LEXEMA_EOF;
    }

    primus = *(Arbor2Lexema**)xar_obtinere(def->corpus, ZEPHYRUM);
    redde primus->genus;
}

Arbor2ExpansionLookahead
arbor2_expansion_lookahead(
    Arbor2Expansion*        exp,
    chorda                  nomen_macro)
{
    Arbor2ExpansionLookahead result;
    Arbor2MacroDef* def;
    Arbor2Lexema* primus;

    result.est_recursivum = FALSUM;
    result.est_vacuum = FALSUM;

    def = arbor2_expansion_quaerere_macro(exp, nomen_macro);
    si (def == NIHIL)
    {
        result.genus = ARBOR2_LEXEMA_ERROR;
        redde result;
    }

    si (xar_numerus(def->corpus) == ZEPHYRUM)
    {
        result.genus = ARBOR2_LEXEMA_EOF;
        result.est_vacuum = VERUM;
        redde result;
    }

    primus = *(Arbor2Lexema**)xar_obtinere(def->corpus, ZEPHYRUM);
    result.genus = primus->genus;

    /* Verificare si primus token est alius macro */
    si (primus->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        Arbor2MacroDef* proximus;
        proximus = arbor2_expansion_quaerere_macro(exp, primus->valor);
        si (proximus != NIHIL)
        {
            result.est_recursivum = VERUM;
        }
    }

    redde result;
}

/* ==================================================
 * Segment Management
 * ================================================== */

interior vacuum
_finire_segmentum(Arbor2Expansion* exp, i32 linea_finis)
{
    Arbor2Segmentum* seg;
    Arbor2Segmentum* ultimum;
    i32 num;

    num = xar_numerus(exp->segmenta);

    /* Update previous segment's end line */
    si (num > ZEPHYRUM)
    {
        ultimum = *(Arbor2Segmentum**)xar_obtinere(exp->segmenta, num - I);
        si (ultimum->linea_finis < ZEPHYRUM)
        {
            ultimum->linea_finis = (s32)linea_finis;
        }
    }

    /* Create new segment with current macro state */
    seg = piscina_allocare(exp->piscina, magnitudo(Arbor2Segmentum));
    seg->linea_initium = linea_finis;
    seg->linea_finis = -I;  /* To be filled later */

    /* Store reference to current macro table
     * Note: For true isolation, would need copy function.
     * Current approach: store current table (segments share table state) */
    seg->macros = exp->macros;

    {
        Arbor2Segmentum** locus;
        locus = xar_addere(exp->segmenta);
        si (locus != NIHIL)
        {
            *locus = seg;
        }
    }
}

interior vacuum
_initium_segmentum(Arbor2Expansion* exp, i32 linea_initium)
{
    i32 num;

    num = xar_numerus(exp->segmenta);
    si (num == ZEPHYRUM)
    {
        /* Create first segment */
        _finire_segmentum(exp, linea_initium);
    }
}

/* ==================================================
 * Typedef Detection
 * ================================================== */

interior vacuum
_detectare_typedef(
    Arbor2Expansion*    exp,
    Xar*                tokens,
    i32                 pos_initium,
    i32                 layer_index)
{
    i32 i;
    i32 num;
    i32 brace_depth;
    i32 paren_depth;
    b32 in_fn_ptr_name;
    b32 fn_ptr_found;
    Arbor2Token* fn_ptr_name;
    Arbor2Token* last_id;
    Arbor2Token* typedef_name;
    Arbor2Token* tok;
    Arbor2Token* prev;
    b32 est_certum;

    num = xar_numerus(tokens);
    brace_depth = ZEPHYRUM;
    paren_depth = ZEPHYRUM;
    in_fn_ptr_name = FALSUM;
    fn_ptr_found = FALSUM;
    fn_ptr_name = NIHIL;
    last_id = NIHIL;
    typedef_name = NIHIL;

    si (pos_initium >= num)
    {
        redde;
    }

    /* Check if starts with TYPEDEF keyword or "nomen" identifier */
    tok = *(Arbor2Token**)xar_obtinere(tokens, pos_initium);
    si (tok->lexema->genus == ARBOR2_LEXEMA_TYPEDEF)
    {
        est_certum = VERUM;
    }
    alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
                 tok->lexema->valor.mensura == V &&
                 memcmp(tok->lexema->valor.datum, "nomen", V) == ZEPHYRUM)
    {
        est_certum = FALSUM;  /* Heuristic - could be variable named "nomen" */
    }
    alioquin
    {
        redde;  /* Not a typedef */
    }

    /* Scan to semicolon, tracking depths */
    per (i = pos_initium + I; i < num; i++)
    {
        tok = *(Arbor2Token**)xar_obtinere(tokens, i);

        /* Skip newlines (legacy check - Phase 2.7 moves NOVA_LINEA to spatia) */
        si (tok->lexema->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            perge;
        }

        si (tok->lexema->genus == ARBOR2_LEXEMA_SEMICOLON &&
            brace_depth == ZEPHYRUM && paren_depth == ZEPHYRUM)
        {
            frange;
        }

        /* Track brace depth for struct bodies */
        si (tok->lexema->genus == ARBOR2_LEXEMA_BRACE_APERTA)
        {
            brace_depth++;
        }
        alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_BRACE_CLAUSA)
        {
            brace_depth--;
        }

        /* Track paren depth for function pointers */
        si (tok->lexema->genus == ARBOR2_LEXEMA_PAREN_APERTA)
        {
            si (paren_depth == ZEPHYRUM && brace_depth == ZEPHYRUM)
            {
                in_fn_ptr_name = VERUM;
            }
            paren_depth++;
        }
        alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_PAREN_CLAUSA)
        {
            paren_depth--;
            si (paren_depth == ZEPHYRUM)
            {
                in_fn_ptr_name = FALSUM;
            }
        }

        /* Detect function pointer: (* IDENTIFIER ) */
        si (in_fn_ptr_name && !fn_ptr_found && brace_depth == ZEPHYRUM)
        {
            prev = (i > pos_initium + I) ?
                   *(Arbor2Token**)xar_obtinere(tokens, i - I) : NIHIL;
            si (prev != NIHIL &&
                prev->lexema->genus == ARBOR2_LEXEMA_ASTERISCUS &&
                tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
            {
                fn_ptr_name = tok;
                fn_ptr_found = VERUM;
            }
        }

        /* Track last identifier at depth 0 */
        si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
            brace_depth == ZEPHYRUM && paren_depth == ZEPHYRUM)
        {
            last_id = tok;
        }
    }

    /* Determine typedef name */
    si (fn_ptr_found)
    {
        typedef_name = fn_ptr_name;
    }
    alioquin
    {
        typedef_name = last_id;
    }

    /* Register typedef */
    si (typedef_name != NIHIL)
    {
        chorda* interned;
        Arbor2TypedefInfo* info;

        interned = chorda_internare(exp->intern, typedef_name->lexema->valor);
        si (interned != NIHIL)
        {
            /* Don't overwrite existing typedef */
            si (tabula_dispersa_continet(exp->typedefs, *interned))
            {
                redde;
            }

            info = piscina_allocare(exp->piscina, magnitudo(Arbor2TypedefInfo));
            info->titulus = interned;
            info->layer_discovered = (s32)layer_index;
            info->est_certum = est_certum;

            /* Create origin metadata */
            info->origo = piscina_allocare(exp->piscina, magnitudo(Arbor2OrigoMeta));
            info->origo->genus = ARBOR2_ORIGO_FONS;
            info->origo->nomen_macro = NIHIL;
            info->origo->via_file = exp->via_current;
            info->origo->linea = typedef_name->lexema->linea;
            info->origo->columna = typedef_name->lexema->columna;
            info->origo->layer_index = layer_index;

            tabula_dispersa_inserere(exp->typedefs, *interned, info);
        }
    }
}

/* ==================================================
 * Directive Processing
 * ================================================== */

interior b32
_processare_define(Arbor2Expansion* exp, Xar* tokens, i32* positus)
{
    i32 pos;
    i32 num;
    Arbor2Token* tok;
    Arbor2MacroDef* def;
    chorda nomen_ch;
    chorda* nomen_ptr;
    Xar* param_xar;
    Xar* corpus_xar;

    pos = *positus;
    num = xar_numerus(tokens);

    /* Skip # and 'define' */
    pos += II;  /* Past # and define */

    si (pos >= num)
    {
        redde FALSUM;
    }

    /* Get macro name */
    tok = *(Arbor2Token**)xar_obtinere(tokens, pos);
    si (tok->lexema->genus != ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        redde FALSUM;
    }

    nomen_ch = tok->lexema->valor;
    nomen_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
    *nomen_ptr = nomen_ch;

    pos++;

    def = piscina_allocare(exp->piscina, magnitudo(Arbor2MacroDef));
    def->titulus = nomen_ptr;
    def->est_functio = FALSUM;
    def->est_variadic = FALSUM;
    def->parametra = NIHIL;
    def->via_file = exp->via_current;
    def->linea_def = (s32)tok->lexema->linea;
    def->ex_api = FALSUM;

    /* Check for function-like macro (paren immediately after name) */
    si (pos < num)
    {
        tok = *(Arbor2Token**)xar_obtinere(tokens, pos);

        /* Function-like if '(' immediately follows (no trivia) */
        si (tok->lexema->genus == ARBOR2_LEXEMA_PAREN_APERTA &&
            tok->lexema->spatia_ante == NIHIL)
        {
            def->est_functio = VERUM;
            pos++;  /* Skip ( */

            param_xar = xar_creare(exp->piscina, magnitudo(chorda*));

            /* Parse parameters */
            dum (pos < num)
            {
                tok = *(Arbor2Token**)xar_obtinere(tokens, pos);

                si (tok->lexema->genus == ARBOR2_LEXEMA_PAREN_CLAUSA)
                {
                    pos++;
                    frange;
                }
                alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_ELLIPSIS)
                {
                    def->est_variadic = VERUM;
                    pos++;
                }
                alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
                {
                    chorda* param_ptr;
                    chorda** param_locus;

                    param_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
                    *param_ptr = tok->lexema->valor;
                    param_locus = xar_addere(param_xar);
                    si (param_locus != NIHIL)
                    {
                        *param_locus = param_ptr;
                    }
                    pos++;
                }
                alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_COMMA)
                {
                    pos++;
                }
                alioquin si (_habet_nova_linea_ante(tok))
                {
                    /* Unterminated parameter list (Phase 2.7: check spatia_ante) */
                    frange;
                }
                alioquin
                {
                    pos++;
                }
            }

            def->parametra = param_xar;
        }
    }

    /* Collect body tokens until newline.
     * Phase 2.7: NOVA_LINEA is now in spatia_post, so we add token first
     * then check if it ends the line. */
    corpus_xar = xar_creare(exp->piscina, magnitudo(Arbor2Lexema*));
    dum (pos < num)
    {
        tok = *(Arbor2Token**)xar_obtinere(tokens, pos);

        /* Phase 2.7: Check if this token starts on a new line */
        si (_habet_nova_linea_ante(tok))
        {
            frange;
        }

        {
            Arbor2Lexema** locus;
            locus = xar_addere(corpus_xar);
            si (locus != NIHIL)
            {
                *locus = tok->lexema;
            }
        }
        pos++;

        /* Phase 2.7: Check if this token ends the line */
        si (_habet_nova_linea_post(tok))
        {
            frange;
        }
    }
    def->corpus = corpus_xar;

    /* Add to macro table and start new segment */
    tabula_dispersa_inserere(exp->macros, nomen_ch, def);
    _finire_segmentum(exp, tok->lexema->linea);

    *positus = pos;
    redde VERUM;
}

interior b32
_processare_undef(Arbor2Expansion* exp, Xar* tokens, i32* positus)
{
    i32 pos;
    i32 num;
    Arbor2Token* tok;
    chorda nomen_ch;

    pos = *positus;
    num = xar_numerus(tokens);

    /* Skip # and 'undef' */
    pos += II;

    si (pos >= num)
    {
        redde FALSUM;
    }

    /* Get macro name */
    tok = *(Arbor2Token**)xar_obtinere(tokens, pos);
    si (tok->lexema->genus != ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        redde FALSUM;
    }

    nomen_ch = tok->lexema->valor;

    /* Remove from macro table and start new segment */
    tabula_dispersa_delere(exp->macros, nomen_ch);
    _finire_segmentum(exp, tok->lexema->linea);

    /* Skip to newline (Phase 2.7: check spatia for NOVA_LINEA) */
    dum (pos < num)
    {
        tok = *(Arbor2Token**)xar_obtinere(tokens, pos);

        /* If this token starts on a new line, we're done */
        si (_habet_nova_linea_ante(tok))
        {
            frange;
        }

        pos++;

        /* If this token ends the line, we're done */
        si (_habet_nova_linea_post(tok))
        {
            frange;
        }
    }

    *positus = pos;
    redde VERUM;
}

/* ==================================================
 * Include Processing
 * ================================================== */

/* Read file contents into buffer */
interior chorda
_legere_filum(Arbor2Expansion* exp, chorda* via)
{
    FILE* filum;
    character buffer_via[MMMMXCVI];
    longus mensura;
    i8* contentum;
    chorda resultus;

    resultus.datum = NIHIL;
    resultus.mensura = ZEPHYRUM;

    si (via == NIHIL || via->mensura == ZEPHYRUM)
    {
        redde resultus;
    }

    /* Create null-terminated path */
    si (via->mensura >= MMMMXCVI)
    {
        redde resultus;
    }
    memcpy(buffer_via, via->datum, (size_t)via->mensura);
    buffer_via[via->mensura] = '\0';

    filum = fopen(buffer_via, "rb");
    si (filum == NIHIL)
    {
        redde resultus;
    }

    /* Get file size */
    fseek(filum, 0L, SEEK_END);
    mensura = ftell(filum);
    fseek(filum, 0L, SEEK_SET);

    si (mensura <= 0L)
    {
        fclose(filum);
        redde resultus;
    }

    /* Allocate and read */
    contentum = piscina_allocare(exp->piscina, (i32)mensura + I);
    si (fread(contentum, (size_t)1, (size_t)mensura, filum) != (size_t)mensura)
    {
        fclose(filum);
        redde resultus;
    }
    fclose(filum);

    contentum[mensura] = '\0';
    resultus.datum = contentum;
    resultus.mensura = (i32)mensura;

    redde resultus;
}

interior b32
_processare_include(Arbor2Expansion* exp, Xar* tokens, i32* positus, Xar* result)
{
    i32 pos;
    i32 num;
    Arbor2Token* tok;
    Arbor2Token* tok_hash;
    Arbor2IncludeGenus genus;
    Arbor2IncludeStatus status;
    chorda via_specifier;
    chorda* via_specifier_ptr;
    chorda* via_resoluta;
    b32 est_learning;
    Xar* lexemata_originalia;
    Arbor2Token** locus;
    i32 linea_directive;
    i32 bracket_depth;

    pos = *positus;
    num = xar_numerus(tokens);

    /* Store # token position and line */
    tok_hash = *(Arbor2Token**)xar_obtinere(tokens, pos);
    linea_directive = tok_hash->lexema->linea;

    /* Collect original tokens for roundtrip */
    lexemata_originalia = xar_creare(exp->piscina, magnitudo(Arbor2Token*));

    /* Add # token */
    locus = xar_addere(lexemata_originalia);
    si (locus != NIHIL) *locus = tok_hash;
    pos++;

    /* Skip 'include' */
    si (pos >= num)
    {
        *positus = pos;
        redde FALSUM;
    }
    tok = *(Arbor2Token**)xar_obtinere(tokens, pos);
    locus = xar_addere(lexemata_originalia);
    si (locus != NIHIL) *locus = tok;
    pos++;

    /* Determine include type and extract path */
    si (pos >= num)
    {
        *positus = pos;
        redde FALSUM;
    }
    tok = *(Arbor2Token**)xar_obtinere(tokens, pos);

    via_specifier.datum = NIHIL;
    via_specifier.mensura = ZEPHYRUM;

    si (tok->lexema->genus == ARBOR2_LEXEMA_STRING_LIT)
    {
        /* Local include: "path" */
        genus = ARBOR2_INCLUDE_LOCAL;

        /* Extract path (remove quotes) */
        si (tok->lexema->valor.mensura >= II)
        {
            via_specifier.datum = tok->lexema->valor.datum + I;
            via_specifier.mensura = tok->lexema->valor.mensura - II;
        }

        locus = xar_addere(lexemata_originalia);
        si (locus != NIHIL) *locus = tok;
        pos++;
    }
    alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_MINOR)
    {
        /* System include: <path> - collect tokens until > */
        ChordaAedificator* aedificator;
        chorda path_ch;
        chorda* path_interned;

        genus = ARBOR2_INCLUDE_SYSTEM;

        /* Add < token */
        locus = xar_addere(lexemata_originalia);
        si (locus != NIHIL) *locus = tok;
        pos++;

        /* Collect path tokens until > */
        aedificator = chorda_aedificator_creare(exp->piscina, CCLVI);
        bracket_depth = I;

        dum (pos < num && bracket_depth > ZEPHYRUM)
        {
            tok = *(Arbor2Token**)xar_obtinere(tokens, pos);

            /* Check for newline - unterminated include */
            si (_habet_nova_linea_ante(tok))
            {
                frange;
            }

            si (tok->lexema->genus == ARBOR2_LEXEMA_MAIOR)
            {
                bracket_depth--;
                si (bracket_depth == ZEPHYRUM)
                {
                    locus = xar_addere(lexemata_originalia);
                    si (locus != NIHIL) *locus = tok;
                    pos++;
                    frange;
                }
            }
            alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_MINOR)
            {
                bracket_depth++;
            }

            /* Add to path */
            chorda_aedificator_appendere_chorda(aedificator, tok->lexema->valor);

            locus = xar_addere(lexemata_originalia);
            si (locus != NIHIL) *locus = tok;
            pos++;
        }

        path_ch = chorda_aedificator_finire(aedificator);
        path_interned = chorda_internare(exp->intern, path_ch);
        via_specifier = *path_interned;
    }
    alioquin
    {
        /* Unknown include format - skip to newline */
        dum (pos < num)
        {
            tok = *(Arbor2Token**)xar_obtinere(tokens, pos);
            si (_habet_nova_linea_ante(tok))
            {
                frange;
            }
            locus = xar_addere(lexemata_originalia);
            si (locus != NIHIL) *locus = tok;
            pos++;
            si (_habet_nova_linea_post(tok))
            {
                frange;
            }
        }
        *positus = pos;
        redde FALSUM;
    }

    /* Skip to end of line */
    dum (pos < num)
    {
        tok = *(Arbor2Token**)xar_obtinere(tokens, pos);
        si (_habet_nova_linea_ante(tok))
        {
            frange;
        }
        locus = xar_addere(lexemata_originalia);
        si (locus != NIHIL) *locus = tok;
        pos++;
        si (_habet_nova_linea_post(tok))
        {
            frange;
        }
    }

    /* Store specifier */
    via_specifier_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
    *via_specifier_ptr = via_specifier;

    /* Determine mode */
    est_learning = (genus == ARBOR2_INCLUDE_SYSTEM)
                    ? exp->system_learning_default
                    : exp->local_learning_default;

    /* Resolve path */
    via_resoluta = arbor2_expansion_resolvere_include(
        exp, via_specifier, (i32)genus, exp->via_current);

    /* Determine status and process */
    si (via_resoluta == NIHIL)
    {
        status = ARBOR2_INCLUDE_UNRESOLVED;
    }
    alioquin si (tabula_dispersa_continet(exp->included_viae, *via_resoluta))
    {
        status = ARBOR2_INCLUDE_SKIPPED;
    }
    alioquin
    {
        chorda contentum;
        Xar* included_tokens;
        chorda* via_salvata;

        status = ARBOR2_INCLUDE_RESOLVED;

        /* Mark as included */
        tabula_dispersa_inserere(exp->included_viae, *via_resoluta, (vacuum*)I);

        /* Save current file context */
        via_salvata = exp->via_current;

        /* Read and process included file */
        contentum = _legere_filum(exp, via_resoluta);

        si (contentum.mensura > ZEPHYRUM)
        {
            included_tokens = arbor2_expansion_processare(
                exp,
                (constans character*)contentum.datum,
                contentum.mensura,
                (constans character*)via_resoluta->datum);

            /* Full mode: merge tokens into result */
            si (!est_learning && included_tokens != NIHIL)
            {
                i32 j;
                i32 inc_num;
                Arbor2Token* inc_tok;

                inc_num = xar_numerus(included_tokens);
                per (j = ZEPHYRUM; j < inc_num; j++)
                {
                    inc_tok = *(Arbor2Token**)xar_obtinere(included_tokens, j);

                    /* Skip EOF tokens */
                    si (inc_tok->lexema->genus == ARBOR2_LEXEMA_EOF)
                    {
                        perge;
                    }

                    locus = xar_addere(result);
                    si (locus != NIHIL)
                    {
                        *locus = inc_tok;
                    }
                }
            }
            /* Learning mode: macros/typedefs already extracted, tokens discarded */
        }

        /* Restore file context */
        exp->via_current = via_salvata;
    }

    /* Create INCLUDE node (stored in result for AST) */
    /* Note: For now we just pass through the tokens in learning mode
     * and merge in full mode. The INCLUDE node creation is for future
     * AST representation if needed. The roundtrip tokens are preserved
     * in lexemata_originalia for serialization. */

    /* Suppress unused variable warnings */
    (vacuum)status;
    (vacuum)via_specifier_ptr;
    (vacuum)est_learning;
    (vacuum)linea_directive;
    (vacuum)lexemata_originalia;

    *positus = pos;
    redde VERUM;
}

/* ==================================================
 * Argument Collection for Function-Like Macros
 * ================================================== */

interior Xar*
_colligere_argumenta(Arbor2Expansion* exp, Xar* tokens, i32* positus, i32 num_param)
{
    Xar* argumenta;
    Xar* arg_current;
    Arbor2Token* tok;
    Arbor2Token** locus;
    i32 pos;
    i32 num;
    i32 paren_depth;

    pos = *positus;
    num = xar_numerus(tokens);

    argumenta = xar_creare(exp->piscina, magnitudo(Xar*));
    arg_current = xar_creare(exp->piscina, magnitudo(Arbor2Token*));
    paren_depth = I;  /* Start after opening paren */

    dum (pos < num)
    {
        tok = *(Arbor2Token**)xar_obtinere(tokens, pos);

        si (tok->lexema->genus == ARBOR2_LEXEMA_PAREN_APERTA)
        {
            paren_depth++;
            locus = xar_addere(arg_current);
            si (locus != NIHIL)
            {
                *locus = tok;
            }
        }
        alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_PAREN_CLAUSA)
        {
            paren_depth--;
            si (paren_depth == ZEPHYRUM)
            {
                /* End of arguments */
                Xar** arg_locus;
                arg_locus = xar_addere(argumenta);
                si (arg_locus != NIHIL)
                {
                    *arg_locus = arg_current;
                }
                pos++;
                frange;
            }
            locus = xar_addere(arg_current);
            si (locus != NIHIL)
            {
                *locus = tok;
            }
        }
        alioquin si (tok->lexema->genus == ARBOR2_LEXEMA_COMMA && paren_depth == I)
        {
            /* Argument separator at depth 1 */
            Xar** arg_locus;
            arg_locus = xar_addere(argumenta);
            si (arg_locus != NIHIL)
            {
                *arg_locus = arg_current;
            }
            arg_current = xar_creare(exp->piscina, magnitudo(Arbor2Token*));
        }
        alioquin
        {
            /* Include token in current argument (including NOVA_LINEA) */
            locus = xar_addere(arg_current);
            si (locus != NIHIL)
            {
                *locus = tok;
            }
        }

        pos++;
    }

    /* Handle variadic: collect remaining args if we have more than expected */
    (vacuum)num_param;  /* May use for variadic handling later */

    *positus = pos;
    redde argumenta;
}

/* ==================================================
 * Stringification (# Operator)
 *
 * Standard-compliant:
 * - Each whitespace/newline sequence -> single space
 * - Leading/trailing whitespace removed
 * - Escape " -> \"
 * - Escape \ -> \\
 * ================================================== */

interior Arbor2Lexema*
_stringify_tokens(Arbor2Expansion* exp, Xar* arg_tokens)
{
    Arbor2Lexema* result;
    i32 i;
    i32 num;
    Arbor2Token* tok;
    b32 need_space;
    b32 in_content;
    i32 buf_size;
    i32 buf_pos;
    i8* buffer;
    chorda temp;
    chorda* internata;

    num = xar_numerus(arg_tokens);

    /* Allocare buffer (estimate: 2x token lengths + quotes + escapes) */
    buf_size = ZEPHYRUM;
    per (i = ZEPHYRUM; i < num; i++)
    {
        tok = *(Arbor2Token**)xar_obtinere(arg_tokens, i);
        buf_size += tok->lexema->valor.mensura * II + II;
    }
    buf_size += IV;  /* Quotes and null */

    buffer = piscina_allocare(exp->piscina, buf_size);
    buf_pos = ZEPHYRUM;

    /* Opening quote */
    buffer[buf_pos++] = '"';

    need_space = FALSUM;
    in_content = FALSUM;

    per (i = ZEPHYRUM; i < num; i++)
    {
        i32 j;
        tok = *(Arbor2Token**)xar_obtinere(arg_tokens, i);

        /* Skip NOVA_LINEA, collapse to space */
        si (tok->lexema->genus == ARBOR2_LEXEMA_NOVA_LINEA)
        {
            si (in_content)
            {
                need_space = VERUM;
            }
            perge;
        }

        /* Check for leading trivia (whitespace) */
        si (tok->lexema->spatia_ante != NIHIL && xar_numerus(tok->lexema->spatia_ante) > ZEPHYRUM)
        {
            si (in_content)
            {
                need_space = VERUM;
            }
        }

        /* Output space if needed (collapse whitespace) */
        si (need_space && in_content)
        {
            buffer[buf_pos++] = ' ';
            need_space = FALSUM;
        }

        /* Output token value with escaping */
        per (j = ZEPHYRUM; j < tok->lexema->valor.mensura; j++)
        {
            i8 c;
            c = tok->lexema->valor.datum[j];

            si (c == '"')
            {
                buffer[buf_pos++] = '\\';
                buffer[buf_pos++] = '"';
            }
            alioquin si (c == '\\')
            {
                buffer[buf_pos++] = '\\';
                buffer[buf_pos++] = '\\';
            }
            alioquin
            {
                buffer[buf_pos++] = c;
            }
        }

        in_content = VERUM;
    }

    /* Closing quote */
    buffer[buf_pos++] = '"';
    buffer[buf_pos] = '\0';

    /* Create string literal lexema */
    result = piscina_allocare(exp->piscina, magnitudo(Arbor2Lexema));
    result->genus = ARBOR2_LEXEMA_STRING_LIT;

    temp.datum = buffer;
    temp.mensura = buf_pos;
    internata = chorda_internare(exp->intern, temp);
    result->valor = *internata;

    result->byte_offset = ZEPHYRUM;
    result->longitudo = buf_pos;
    result->linea = ZEPHYRUM;
    result->columna = ZEPHYRUM;
    result->spatia_ante = NIHIL;
    result->spatia_post = NIHIL;
    result->standard = ARBOR2_STANDARD_C89;

    redde result;
}

/* ==================================================
 * Token Pasting (## Operator)
 *
 * Best-effort: re-lex concatenated text.
 * ================================================== */

interior Arbor2Lexema*
_relex_single_token(Arbor2Expansion* exp, chorda combined)
{
    Arbor2Lexator* lex;
    Arbor2Lexema* result;

    lex = arbor2_lexator_creare(exp->piscina, exp->intern,
                                 (constans character*)combined.datum, combined.mensura);
    result = arbor2_lexema_proximum(lex);

    /* If EOF or error, return error token with combined text */
    si (result->genus == ARBOR2_LEXEMA_EOF || result->genus == ARBOR2_LEXEMA_ERROR)
    {
        result->genus = ARBOR2_LEXEMA_ERROR;
        result->valor = combined;
    }

    redde result;
}

interior Arbor2Token*
_paste_tokens(Arbor2Expansion* exp, Arbor2Token* sinister, Arbor2Token* dexter,
              chorda* nomen_macro, i32 layer_index)
{
    chorda combined;
    i8* buffer;
    i32 len;
    Arbor2Lexema* lexema;
    chorda* internata;
    chorda temp;

    /* Concatenate text */
    len = sinister->lexema->valor.mensura + dexter->lexema->valor.mensura;
    buffer = piscina_allocare(exp->piscina, len + I);

    memcpy(buffer, sinister->lexema->valor.datum,
           (size_t)sinister->lexema->valor.mensura);
    memcpy(buffer + sinister->lexema->valor.mensura,
           dexter->lexema->valor.datum,
           (size_t)dexter->lexema->valor.mensura);
    buffer[len] = '\0';

    temp.datum = buffer;
    temp.mensura = len;
    internata = chorda_internare(exp->intern, temp);
    combined = *internata;

    /* Re-lex (single token, best-effort) */
    lexema = _relex_single_token(exp, combined);

    /* Create token with merged provenance */
    redde arbor2_token_ex_pasta(exp->piscina, lexema, sinister, dexter,
                                nomen_macro, layer_index);
}

/* ==================================================
 * Macro Expansion
 * ================================================== */

interior b32
_est_in_macro_stack(Arbor2Expansion* exp, chorda titulus)
{
    i32 i;
    i32 num;
    chorda* entry;

    num = xar_numerus(exp->macro_stack);
    per (i = ZEPHYRUM; i < num; i++)
    {
        entry = *(chorda**)xar_obtinere(exp->macro_stack, i);
        si (_chorda_aequalis(*entry, titulus))
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

interior Xar*
_expandere_macro(Arbor2Expansion* exp, Arbor2MacroDef* def, Xar* argumenta,
                 Arbor2Token* invocation_token, i32 layer_index)
{
    Xar* result;
    Xar* corpus;
    i32 i;
    i32 num;
    Arbor2Lexema* lex;
    Arbor2Token* tok;
    Arbor2Token** locus;
    chorda** stack_entry;

    result = xar_creare(exp->piscina, magnitudo(Arbor2Token*));
    corpus = def->corpus;
    num = xar_numerus(corpus);

    /* Push onto macro stack to prevent recursion */
    stack_entry = xar_addere(exp->macro_stack);
    si (stack_entry != NIHIL)
    {
        *stack_entry = def->titulus;
    }

    per (i = ZEPHYRUM; i < num; i++)
    {
        lex = *(Arbor2Lexema**)xar_obtinere(corpus, i);

        /* Check for # (stringify) operator */
        si (lex->genus == ARBOR2_LEXEMA_HASH && def->est_functio)
        {
            /* Next token should be parameter */
            si (i + I < num)
            {
                Arbor2Lexema* next_lex;
                next_lex = *(Arbor2Lexema**)xar_obtinere(corpus, i + I);

                si (next_lex->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
                {
                    /* Find parameter index */
                    s32 param_idx;
                    i32 num_params;
                    i32 p;

                    param_idx = -I;
                    num_params = xar_numerus(def->parametra);

                    per (p = ZEPHYRUM; p < num_params; p++)
                    {
                        chorda* param;
                        param = *(chorda**)xar_obtinere(def->parametra, p);
                        si (_chorda_aequalis(*param, next_lex->valor))
                        {
                            param_idx = (s32)p;
                            frange;
                        }
                    }

                    si (param_idx >= ZEPHYRUM && param_idx < (s32)xar_numerus(argumenta))
                    {
                        Xar* arg;
                        Arbor2Lexema* stringified;
                        Arbor2Token* str_tok;

                        arg = *(Xar**)xar_obtinere(argumenta, (i32)param_idx);
                        stringified = _stringify_tokens(exp, arg);
                        str_tok = arbor2_token_ex_chorda(exp->piscina, stringified,
                                                         invocation_token, def->titulus,
                                                         layer_index);
                        locus = xar_addere(result);
                        si (locus != NIHIL)
                        {
                            *locus = str_tok;
                        }

                        i++;  /* Skip parameter token */
                        perge;
                    }
                }
            }
        }

        /* Check for ## (paste) operator */
        si (lex->genus == ARBOR2_LEXEMA_HASH_HASH)
        {
            /* Paste previous and next tokens */
            si (xar_numerus(result) > ZEPHYRUM && i + I < num)
            {
                Arbor2Token* left;
                Arbor2Lexema* right_lex;
                Arbor2Token* right;
                Arbor2Token* pasted;

                left = *(Arbor2Token**)xar_obtinere(result, xar_numerus(result) - I);
                right_lex = *(Arbor2Lexema**)xar_obtinere(corpus, i + I);

                /* Handle parameter substitution for right token */
                si (right_lex->genus == ARBOR2_LEXEMA_IDENTIFICATOR && def->est_functio)
                {
                    s32 param_idx;
                    i32 num_params;
                    i32 p;

                    param_idx = -I;
                    num_params = xar_numerus(def->parametra);

                    per (p = ZEPHYRUM; p < num_params; p++)
                    {
                        chorda* param;
                        param = *(chorda**)xar_obtinere(def->parametra, p);
                        si (_chorda_aequalis(*param, right_lex->valor))
                        {
                            param_idx = (s32)p;
                            frange;
                        }
                    }

                    si (param_idx >= ZEPHYRUM && param_idx < (s32)xar_numerus(argumenta))
                    {
                        Xar* arg;
                        arg = *(Xar**)xar_obtinere(argumenta, (i32)param_idx);
                        si (xar_numerus(arg) > ZEPHYRUM)
                        {
                            right = *(Arbor2Token**)xar_obtinere(arg, ZEPHYRUM);
                        }
                        alioquin
                        {
                            right = arbor2_token_ex_expansione(exp->piscina, right_lex,
                                                               invocation_token, def->titulus,
                                                               def->via_file, layer_index);
                        }
                    }
                    alioquin
                    {
                        right = arbor2_token_ex_expansione(exp->piscina, right_lex,
                                                           invocation_token, def->titulus,
                                                           def->via_file, layer_index);
                    }
                }
                alioquin
                {
                    right = arbor2_token_ex_expansione(exp->piscina, right_lex,
                                                       invocation_token, def->titulus,
                                                       def->via_file, layer_index);
                }

                /* Remove left token from result and add pasted */
                xar_removere_ultimum(result);
                pasted = _paste_tokens(exp, left, right, def->titulus, layer_index);
                locus = xar_addere(result);
                si (locus != NIHIL)
                {
                    *locus = pasted;
                }

                i++;  /* Skip right token */
                perge;
            }
        }

        /* __VA_ARGS__ substitution for variadic macros */
        si (lex->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
            def->est_variadic &&
            lex->valor.mensura == XI &&
            memcmp(lex->valor.datum, "__VA_ARGS__", XI) == ZEPHYRUM)
        {
            i32 num_params;
            i32 num_args;
            i32 j;
            i32 k;
            b32 first_arg;

            num_params = def->parametra ? xar_numerus(def->parametra) : ZEPHYRUM;
            num_args = argumenta ? xar_numerus(argumenta) : ZEPHYRUM;
            first_arg = VERUM;

            /* Output all variadic arguments (those beyond named params) */
            per (j = num_params; j < num_args; j++)
            {
                Xar* arg;
                i32 arg_num;

                arg = *(Xar**)xar_obtinere(argumenta, j);
                arg_num = xar_numerus(arg);

                /* Add comma between variadic args (not before first) */
                si (!first_arg)
                {
                    Arbor2Lexema* comma_lex;
                    Arbor2Token* comma_tok;
                    chorda comma_ch;
                    chorda* comma_intern;
                    unio { constans character* c; i8* m; } u;

                    comma_lex = piscina_allocare(exp->piscina, magnitudo(Arbor2Lexema));
                    comma_lex->genus = ARBOR2_LEXEMA_COMMA;
                    u.c = ",";
                    comma_ch.datum = u.m;
                    comma_ch.mensura = I;
                    comma_intern = chorda_internare(exp->intern, comma_ch);
                    comma_lex->valor = *comma_intern;
                    comma_lex->byte_offset = ZEPHYRUM;
                    comma_lex->longitudo = I;
                    comma_lex->linea = ZEPHYRUM;
                    comma_lex->columna = ZEPHYRUM;
                    comma_lex->spatia_ante = NIHIL;
                    comma_lex->spatia_post = NIHIL;
                    comma_lex->standard = ARBOR2_STANDARD_C89;

                    comma_tok = arbor2_token_ex_expansione(exp->piscina, comma_lex,
                                                           invocation_token, def->titulus,
                                                           def->via_file, layer_index);
                    locus = xar_addere(result);
                    si (locus != NIHIL)
                    {
                        *locus = comma_tok;
                    }
                }

                /* Output tokens from this variadic arg */
                per (k = ZEPHYRUM; k < arg_num; k++)
                {
                    Arbor2Token* arg_tok;
                    arg_tok = *(Arbor2Token**)xar_obtinere(arg, k);

                    /* Skip NOVA_LINEA */
                    si (arg_tok->lexema->genus == ARBOR2_LEXEMA_NOVA_LINEA)
                    {
                        perge;
                    }

                    locus = xar_addere(result);
                    si (locus != NIHIL)
                    {
                        *locus = arg_tok;
                    }
                }

                first_arg = FALSUM;
            }
            perge;
        }

        /* Parameter substitution */
        si (lex->genus == ARBOR2_LEXEMA_IDENTIFICATOR && def->est_functio)
        {
            s32 param_idx;
            i32 num_params;
            i32 p;

            param_idx = -I;
            num_params = xar_numerus(def->parametra);

            per (p = ZEPHYRUM; p < num_params; p++)
            {
                chorda* param;
                param = *(chorda**)xar_obtinere(def->parametra, p);
                si (_chorda_aequalis(*param, lex->valor))
                {
                    param_idx = (s32)p;
                    frange;
                }
            }

            si (param_idx >= ZEPHYRUM && param_idx < (s32)xar_numerus(argumenta))
            {
                /* Substitute argument tokens */
                Xar* arg;
                i32 j;
                i32 arg_num;

                arg = *(Xar**)xar_obtinere(argumenta, (i32)param_idx);
                arg_num = xar_numerus(arg);

                per (j = ZEPHYRUM; j < arg_num; j++)
                {
                    Arbor2Token* arg_tok;
                    arg_tok = *(Arbor2Token**)xar_obtinere(arg, j);

                    /* Skip NOVA_LINEA in normal substitution */
                    si (arg_tok->lexema->genus == ARBOR2_LEXEMA_NOVA_LINEA)
                    {
                        perge;
                    }

                    locus = xar_addere(result);
                    si (locus != NIHIL)
                    {
                        *locus = arg_tok;
                    }
                }
                perge;
            }
        }

        /* Regular token - wrap with expansion provenance */
        tok = arbor2_token_ex_expansione(exp->piscina, lex, invocation_token,
                                          def->titulus, def->via_file, layer_index);
        locus = xar_addere(result);
        si (locus != NIHIL)
        {
            *locus = tok;
        }
    }

    /* Pop from macro stack */
    si (xar_numerus(exp->macro_stack) > ZEPHYRUM)
    {
        xar_removere_ultimum(exp->macro_stack);
    }

    redde result;
}

/* ==================================================
 * Single Layer Expansion
 * ================================================== */

interior Arbor2Layer*
_expand_layer(Arbor2Expansion* exp, Arbor2Layer* input)
{
    Arbor2Layer* output;
    Xar* result;
    i32 i;
    i32 num;
    Arbor2Token* tok;
    Arbor2Token* next_tok;
    Arbor2Token** locus;
    b32 changed;

    result = xar_creare(exp->piscina, magnitudo(Arbor2Token*));
    num = xar_numerus(input->lexemata);
    changed = FALSUM;

    per (i = ZEPHYRUM; i < num; i++)
    {
        tok = *(Arbor2Token**)xar_obtinere(input->lexemata, i);

        /* Check for preprocessor directive */
        si (tok->lexema->genus == ARBOR2_LEXEMA_HASH)
        {
            /* Check if at start of line (leading trivia or first token) */
            si (i == ZEPHYRUM || tok->lexema->spatia_ante != NIHIL)
            {
                si (i + I < num)
                {
                    next_tok = *(Arbor2Token**)xar_obtinere(input->lexemata, i + I);

                    /* #define */
                    si (next_tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
                        next_tok->lexema->valor.mensura == VI &&
                        memcmp(next_tok->lexema->valor.datum, "define", VI) == ZEPHYRUM)
                    {
                        _processare_define(exp, input->lexemata, &i);
                        changed = VERUM;
                        /* Adjust for loop increment - we want to process token at i next */
                        i--;
                        perge;
                    }

                    /* #undef */
                    si (next_tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
                        next_tok->lexema->valor.mensura == V &&
                        memcmp(next_tok->lexema->valor.datum, "undef", V) == ZEPHYRUM)
                    {
                        _processare_undef(exp, input->lexemata, &i);
                        changed = VERUM;
                        /* Adjust for loop increment */
                        i--;
                        perge;
                    }

                    /* #include */
                    si (next_tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR &&
                        next_tok->lexema->valor.mensura == VII &&
                        memcmp(next_tok->lexema->valor.datum, "include", VII) == ZEPHYRUM)
                    {
                        _processare_include(exp, input->lexemata, &i, result);
                        changed = VERUM;
                        /* Adjust for loop increment */
                        i--;
                        perge;
                    }
                }
            }

            /* Pass through non-processed # */
            locus = xar_addere(result);
            si (locus != NIHIL)
            {
                *locus = tok;
            }
            perge;
        }

        /* Check for typedef keyword */
        si (tok->lexema->genus == ARBOR2_LEXEMA_TYPEDEF)
        {
            _detectare_typedef(exp, input->lexemata, i, input->layer_index);
            /* Pass through typedef tokens to output */
            locus = xar_addere(result);
            si (locus != NIHIL)
            {
                *locus = tok;
            }
            perge;
        }

        /* Check for macro invocation */
        si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
        {
            Arbor2MacroDef* def;

            /* Check for "nomen" (Latin typedef) */
            si (tok->lexema->valor.mensura == V &&
                memcmp(tok->lexema->valor.datum, "nomen", V) == ZEPHYRUM)
            {
                _detectare_typedef(exp, input->lexemata, i, input->layer_index);
                /* Pass through - don't skip, let it be processed as identifier */
            }

            def = arbor2_expansion_quaerere_macro(exp, tok->lexema->valor);

            si (def != NIHIL && !_est_in_macro_stack(exp, tok->lexema->valor))
            {
                /* Function-like macro needs arguments */
                si (def->est_functio)
                {
                    si (i + I < num)
                    {
                        next_tok = *(Arbor2Token**)xar_obtinere(input->lexemata, i + I);
                        si (next_tok->lexema->genus == ARBOR2_LEXEMA_PAREN_APERTA)
                        {
                            Xar* argumenta;
                            Xar* expanded;
                            i32 j;
                            i32 exp_num;

                            i += II;  /* Skip name and ( */
                            argumenta = _colligere_argumenta(exp, input->lexemata, &i,
                                                             xar_numerus(def->parametra));
                            i--;  /* Adjust for loop increment */

                            expanded = _expandere_macro(exp, def, argumenta, tok,
                                                        input->layer_index + I);
                            exp_num = xar_numerus(expanded);

                            per (j = ZEPHYRUM; j < exp_num; j++)
                            {
                                Arbor2Token* exp_tok;
                                exp_tok = *(Arbor2Token**)xar_obtinere(expanded, j);
                                locus = xar_addere(result);
                                si (locus != NIHIL)
                                {
                                    *locus = exp_tok;
                                }
                            }

                            changed = VERUM;
                            perge;
                        }
                    }
                }
                alioquin
                {
                    /* Object-like macro */
                    Xar* expanded;
                    i32 j;
                    i32 exp_num;

                    expanded = _expandere_macro(exp, def, NIHIL, tok,
                                                input->layer_index + I);
                    exp_num = xar_numerus(expanded);

                    per (j = ZEPHYRUM; j < exp_num; j++)
                    {
                        Arbor2Token* exp_tok;
                        exp_tok = *(Arbor2Token**)xar_obtinere(expanded, j);
                        locus = xar_addere(result);
                        si (locus != NIHIL)
                        {
                            *locus = exp_tok;
                        }
                    }

                    changed = VERUM;
                    perge;
                }
            }
        }

        /* Pass through regular token */
        locus = xar_addere(result);
        si (locus != NIHIL)
        {
            *locus = tok;
        }
    }

    output = piscina_allocare(exp->piscina, magnitudo(Arbor2Layer));
    output->lexemata = result;
    output->layer_index = input->layer_index + I;
    output->est_fixpoint = !changed;

    redde output;
}

/* ==================================================
 * Main Processing Entry Point
 * ================================================== */

Xar*
arbor2_expansion_processare(
    Arbor2Expansion*        exp,
    constans character*     fons,
    i32                     mensura,
    constans character*     via_file)
{
    Arbor2Lexator* lex;
    Xar* lexemata;
    Arbor2Lexema* lexema;
    Arbor2Token* tok;
    Arbor2Token** locus;
    Arbor2Layer* layer;
    Arbor2Layer** layer_locus;
    chorda via_ch;
    chorda* via_ptr;
    i32 max_iterations;
    i32 iter;

    /* Set current file */
    si (via_file != NIHIL)
    {
        via_ch = _chorda_ex_cstring(via_file, exp->intern);
        via_ptr = piscina_allocare(exp->piscina, magnitudo(chorda));
        *via_ptr = via_ch;
        exp->via_current = via_ptr;
    }

    /* Reset include tracking if per-parse mode */
    si (exp->include_modus == ARBOR2_INCLUDE_RESET_PER_PARSE)
    {
        exp->included_viae = tabula_dispersa_creare_chorda(exp->piscina, LXIV);
    }

    /* Initialize first segment */
    _initium_segmentum(exp, I);

    /* Lex source into layer 0 */
    lex = arbor2_lexator_creare(exp->piscina, exp->intern, fons, mensura);
    lexemata = xar_creare(exp->piscina, magnitudo(Arbor2Token*));

    fac
    {
        lexema = arbor2_lexema_proximum(lex);
        tok = arbor2_token_ex_lexema(exp->piscina, lexema, exp->via_current,
                                      lexema->linea, lexema->columna);
        locus = xar_addere(lexemata);
        si (locus != NIHIL)
        {
            *locus = tok;
        }
    }
    dum (lexema->genus != ARBOR2_LEXEMA_EOF && lexema->genus != ARBOR2_LEXEMA_ERROR);

    /* Create layer 0 */
    layer = piscina_allocare(exp->piscina, magnitudo(Arbor2Layer));
    layer->lexemata = lexemata;
    layer->layer_index = ZEPHYRUM;
    layer->est_fixpoint = FALSUM;

    layer_locus = xar_addere(exp->layers);
    si (layer_locus != NIHIL)
    {
        *layer_locus = layer;
    }

    /* Expand to fixpoint (max 100 iterations to prevent infinite loops) */
    max_iterations = C;
    iter = ZEPHYRUM;

    dum (!layer->est_fixpoint && iter < max_iterations)
    {
        layer = _expand_layer(exp, layer);

        layer_locus = xar_addere(exp->layers);
        si (layer_locus != NIHIL)
        {
            *layer_locus = layer;
        }

        iter++;
    }

    /* Finalize last segment */
    {
        i32 num_seg;
        num_seg = xar_numerus(exp->segmenta);
        si (num_seg > ZEPHYRUM)
        {
            Arbor2Segmentum* last_seg;
            last_seg = *(Arbor2Segmentum**)xar_obtinere(exp->segmenta, num_seg - I);
            si (last_seg->linea_finis < ZEPHYRUM)
            {
                last_seg->linea_finis = M;  /* Large number = to end of file */
            }
        }
    }

    redde layer->lexemata;
}

/* ==================================================
 * Query Functions
 * ================================================== */

Xar*
arbor2_expansion_obtinere_segmenta(Arbor2Expansion* exp)
{
    redde exp->segmenta;
}

TabulaDispersa*
arbor2_expansion_macros_ad_lineam(Arbor2Expansion* exp, i32 linea)
{
    i32 i;
    i32 num;
    Arbor2Segmentum* seg;

    num = xar_numerus(exp->segmenta);

    per (i = ZEPHYRUM; i < num; i++)
    {
        seg = *(Arbor2Segmentum**)xar_obtinere(exp->segmenta, i);

        si (linea >= seg->linea_initium &&
            (seg->linea_finis < ZEPHYRUM || (s32)linea < seg->linea_finis))
        {
            redde seg->macros;
        }
    }

    /* Default to current macro table */
    redde exp->macros;
}

Xar*
arbor2_expansion_obtinere_layers(Arbor2Expansion* exp)
{
    redde exp->layers;
}

/* ==================================================
 * Built-in Latin C89 Macros (latina.h)
 *
 * Registers all macros from latina.h for Latin C89 parsing.
 * ================================================== */

hic_manens constans structura {
    constans character* titulus;
    constans character* valor;
} LATINA_MACROS[] = {
    /* Keywords */
    { "si", "if" },
    { "alioquin", "else" },
    { "per", "for" },
    { "dum", "while" },
    { "fac", "do" },
    { "commutatio", "switch" },
    { "casus", "case" },
    { "ordinarius", "default" },
    { "frange", "break" },
    { "perge", "continue" },
    { "redde", "return" },
    { "salta", "goto" },
    /* Type keywords - note: "nomen" itself is a macro so we write the string directly */
    { "structura", "struct" },
    { "unio", "union" },
    { "enumeratio", "enum" },
    { "nomen", "typedef" },
    /* Qualifiers */
    { "constans", "const" },
    { "volatilis", "volatile" },
    { "staticus", "static" },
    { "externus", "extern" },
    { "registrum", "register" },
    { "sponte", "auto" },
    /* Types */
    { "vacuum", "void" },
    { "character", "char" },
    { "integer", "int" },
    { "brevis", "short" },
    { "longus", "long" },
    { "signatus", "signed" },
    { "insignatus", "unsigned" },
    { "fluitans", "float" },
    { "duplex", "double" },
    /* Operators */
    { "magnitudo", "sizeof" },
    /* Constants */
    { "NIHIL", "NULL" },
    { "VERUM", "1" },
    { "FALSUM", "0" },
    /* Storage hints (all map to static) */
    { "hic_manens", "static" },
    { "interior", "static" },
    { "universalis", "static" },
    /* Sentinel */
    { NIHIL, NIHIL }
};

vacuum
arbor2_includere_latina(Arbor2Expansion* exp)
{
    i32 i;

    per (i = ZEPHYRUM; LATINA_MACROS[i].titulus != NIHIL; i++)
    {
        arbor2_expansion_addere_macro(exp,
            LATINA_MACROS[i].titulus,
            LATINA_MACROS[i].valor,
            "latina.h");
    }
}
