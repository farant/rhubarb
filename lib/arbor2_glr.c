/* arbor2_glr.c - GLR Parser Core */
#include "latina.h"
#include "arbor2_glr.h"
#include "piscina.h"
#include "xar.h"
#include <string.h>
#include <stdio.h>

/* Debug flag - set to 1 to enable tracing */
#define GLR_DEBUG 1

/* ==================================================
 * Internal Forward Declarations
 * ================================================== */

interior Arbor2GSSNodus* _creare_gss_nodum(
    Arbor2GLR* glr, s32 status, Arbor2Nodus* valor, Arbor2Token* lexema);

interior vacuum _addere_praedecessorem(
    Arbor2GLR* glr, Arbor2GSSNodus* nodus, Arbor2GSSNodus* praed);

interior Arbor2Token* _currens_lexema(Arbor2GLR* glr);
interior vacuum _progredi(Arbor2GLR* glr);

interior Arbor2GSSNodus* _processare_actiones(Arbor2GLR* glr, b32* acceptatum);
interior vacuum _exequi_shift(Arbor2GLR* glr, Arbor2GSSNodus* nodus, s32 status_novus);

interior Arbor2Nodus* _construere_nodum_binarium(
    Arbor2GLR* glr, Arbor2Nodus* sinister, Arbor2Token* operator, Arbor2Nodus* dexter);

interior Arbor2Nodus* _construere_nodum_unarium(
    Arbor2GLR* glr, Arbor2Token* operator, Arbor2Nodus* operandum);

/* ==================================================
 * GSS Path Enumeration for Reductions
 *
 * When reducing, we need to explore ALL paths through the GSS,
 * not just paths starting from multi-predecessor nodes at the top.
 * Multi-predecessor nodes can appear at ANY depth.
 * ================================================== */

#define MAX_GSS_PATHS 64
#define MAX_POP_DEPTH 16

nomen structura GSSPath GSSPath;
structura GSSPath {
    Arbor2GSSNodus* end_cursor;
    Arbor2Nodus*    valori[MAX_POP_DEPTH];
    Arbor2Token*    lexemata[MAX_POP_DEPTH];
};

/* Recursively enumerate all paths of given depth from start node.
 * Stores results in paths array, returns number of paths found. */
interior s32
_enumerare_gss_vias(
    Arbor2GSSNodus* cursor,
    s32             depth,
    s32             target_depth,
    GSSPath*        current_path,
    GSSPath*        paths,
    s32             max_paths,
    s32             num_paths)
{
    s32 i;

    si (cursor == NIHIL)
    {
        redde num_paths;  /* Path died */
    }

    si (depth >= MAX_POP_DEPTH)
    {
        redde num_paths;  /* Safety limit */
    }

    /* Handle epsilon reduction (pop 0) - no popping needed */
    si (target_depth == ZEPHYRUM)
    {
        si (num_paths < max_paths)
        {
            paths[num_paths].end_cursor = cursor;
            num_paths++;
        }
        redde num_paths;
    }

    /* Store current node's data in path */
    current_path->valori[depth] = cursor->valor;
    current_path->lexemata[depth] = cursor->lexema;

    si (depth + I >= target_depth)
    {
        /* Reached target depth - save path for each predecessor */
        si (cursor->num_praed > ZEPHYRUM)
        {
            per (i = ZEPHYRUM; i < cursor->num_praed && num_paths < max_paths; i++)
            {
                s32 j;
                paths[num_paths].end_cursor = cursor->praedecessores[i];
                per (j = ZEPHYRUM; j <= depth; j++)
                {
                    paths[num_paths].valori[j] = current_path->valori[j];
                    paths[num_paths].lexemata[j] = current_path->lexemata[j];
                }
                num_paths++;
            }
        }
        /* If no predecessors at required depth, path is invalid - don't add it */
    }
    alioquin
    {
        /* Need to go deeper - recurse for each predecessor */
        si (cursor->num_praed > ZEPHYRUM)
        {
            per (i = ZEPHYRUM; i < cursor->num_praed; i++)
            {
                num_paths = _enumerare_gss_vias(
                    cursor->praedecessores[i],
                    depth + I,
                    target_depth,
                    current_path,
                    paths,
                    max_paths,
                    num_paths);
            }
        }
        /* If no predecessors and we need to go deeper, path dies - don't add */
    }

    redde num_paths;
}

/* ==================================================
 * Creation
 * ================================================== */

Arbor2GLR*
arbor2_glr_creare(
    Piscina*                piscina,
    InternamentumChorda*    intern,
    Arbor2Expansion*        expansion)
{
    Arbor2GLR* glr;

    glr = piscina_allocare(piscina, magnitudo(Arbor2GLR));
    si (glr == NIHIL)
    {
        redde NIHIL;
    }

    glr->piscina = piscina;
    glr->intern = intern;
    glr->expansion = expansion;

    glr->lexemata = NIHIL;
    glr->positus = ZEPHYRUM;

    glr->frons_activa = xar_creare(piscina, magnitudo(Arbor2GSSNodus*));
    glr->frons_nova = xar_creare(piscina, magnitudo(Arbor2GSSNodus*));

    glr->errores = xar_creare(piscina, magnitudo(chorda*));
    glr->ambigui = xar_creare(piscina, magnitudo(Arbor2Nodus*));

    glr->num_furcae = ZEPHYRUM;
    glr->num_mergae = ZEPHYRUM;
    glr->max_frons = ZEPHYRUM;

    /* Initialize LR tables */
    arbor2_glr_initializare_tabulas(glr);

    redde glr;
}

/* ==================================================
 * GSS Node Management
 * ================================================== */

interior Arbor2GSSNodus*
_creare_gss_nodum(
    Arbor2GLR*      glr,
    s32             status,
    Arbor2Nodus*    valor,
    Arbor2Token*    lexema)
{
    Arbor2GSSNodus* nodus;

    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2GSSNodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->status = status;
    nodus->praedecessores = NIHIL;
    nodus->num_praed = ZEPHYRUM;
    nodus->capacitas_praed = ZEPHYRUM;
    nodus->valor = valor;
    nodus->lexema = lexema;
    nodus->punctum_salutis = piscina_notare(glr->piscina);
    nodus->furca_id = ZEPHYRUM;  /* No fork by default */

    redde nodus;
}

interior vacuum
_addere_praedecessorem(
    Arbor2GLR*      glr,
    Arbor2GSSNodus* nodus,
    Arbor2GSSNodus* praed)
{
    si (nodus->num_praed >= nodus->capacitas_praed)
    {
        s32 nova_capacitas;
        Arbor2GSSNodus** nova_praed;

        nova_capacitas = (nodus->capacitas_praed == ZEPHYRUM) ? IV : nodus->capacitas_praed * II;
        nova_praed = piscina_allocare(glr->piscina,
            (memoriae_index)nova_capacitas * magnitudo(Arbor2GSSNodus*));

        si (nodus->praedecessores != NIHIL && nodus->num_praed > ZEPHYRUM)
        {
            memcpy(nova_praed, nodus->praedecessores,
                (memoriae_index)nodus->num_praed * magnitudo(Arbor2GSSNodus*));
        }

        nodus->praedecessores = nova_praed;
        nodus->capacitas_praed = nova_capacitas;
    }

    nodus->praedecessores[nodus->num_praed] = praed;
    nodus->num_praed++;
}

/* ==================================================
 * Token Access
 * ================================================== */

interior Arbor2Token*
_currens_lexema(Arbor2GLR* glr)
{
    si (glr->positus >= xar_numerus(glr->lexemata))
    {
        redde NIHIL;
    }
    redde *(Arbor2Token**)xar_obtinere(glr->lexemata, glr->positus);
}

/* Lookahead - will be used for macro lookahead in Step 6 */
#if 0
interior Arbor2Token*
_prospicere_lexema(Arbor2GLR* glr, i32 offset)
{
    i32 index = glr->positus + offset;
    si (index >= xar_numerus(glr->lexemata) || index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde *(Arbor2Token**)xar_obtinere(glr->lexemata, index);
}
#endif

interior vacuum
_progredi(Arbor2GLR* glr)
{
    glr->positus++;
}

/* ==================================================
 * AST Node Creation
 * ================================================== */

Arbor2Nodus*
arbor2_nodus_creare_folium(
    Arbor2GLR*          glr,
    Arbor2NodusGenus    genus,
    Arbor2Token*        lexema)
{
    Arbor2Nodus* nodus;

    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->genus = genus;
    nodus->lexema = lexema;
    nodus->datum.folium.valor = lexema->lexema->valor;

    redde nodus;
}

Arbor2Nodus*
arbor2_nodus_creare_binarium(
    Arbor2GLR*          glr,
    Arbor2Nodus*        sinister,
    Arbor2LexemaGenus   operator,
    Arbor2Nodus*        dexter)
{
    Arbor2Nodus* nodus;

    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->genus = ARBOR2_NODUS_BINARIUM;
    nodus->lexema = NIHIL;
    nodus->datum.binarium.sinister = sinister;
    nodus->datum.binarium.operator = operator;
    nodus->datum.binarium.dexter = dexter;

    redde nodus;
}

Arbor2Nodus*
arbor2_nodus_creare_unarium(
    Arbor2GLR*          glr,
    Arbor2LexemaGenus   operator,
    Arbor2Nodus*        operandum)
{
    Arbor2Nodus* nodus;

    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->genus = ARBOR2_NODUS_UNARIUM;
    nodus->lexema = NIHIL;
    nodus->datum.unarium.operator = operator;
    nodus->datum.unarium.operandum = operandum;

    redde nodus;
}

interior Arbor2Nodus*
_construere_nodum_binarium(
    Arbor2GLR*      glr,
    Arbor2Nodus*    sinister,
    Arbor2Token*    operator,
    Arbor2Nodus*    dexter)
{
    Arbor2Nodus* nodus;

    nodus = arbor2_nodus_creare_binarium(glr, sinister, operator->lexema->genus, dexter);
    si (nodus != NIHIL)
    {
        nodus->lexema = operator;
    }

    redde nodus;
}

interior Arbor2Nodus*
_construere_nodum_unarium(
    Arbor2GLR*      glr,
    Arbor2Token*    operator,
    Arbor2Nodus*    operandum)
{
    Arbor2Nodus* nodus;

    nodus = arbor2_nodus_creare_unarium(glr, operator->lexema->genus, operandum);
    si (nodus != NIHIL)
    {
        nodus->lexema = operator;
    }

    redde nodus;
}

/* ==================================================
 * GLR Algorithm - Typedef Pruning
 * ================================================== */

/* Check if an identifier token might be a type name */
interior b32
_potest_esse_typus(Arbor2GLR* glr, Arbor2Token* tok)
{
    si (tok == NIHIL)
    {
        redde FALSUM;
    }

    si (tok->lexema->genus != ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        redde FALSUM;
    }

    /* Check if registered as typedef */
    si (glr->expansion != NIHIL)
    {
        redde arbor2_expansion_est_typedef(glr->expansion, tok->lexema->valor);
    }

    redde FALSUM;
}

/* Check if a macro might expand to a type */
interior b32
_macro_suggerit_typum(Arbor2GLR* glr, Arbor2Token* tok)
{
    Arbor2MacroDef* macro;

    si (tok == NIHIL || glr->expansion == NIHIL)
    {
        redde FALSUM;
    }

    si (tok->lexema->genus != ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        redde FALSUM;
    }

    macro = arbor2_expansion_quaerere_macro(glr->expansion, tok->lexema->valor);
    si (macro == NIHIL)
    {
        redde FALSUM;
    }

    /* Check first token of expansion for type keyword */
    si (xar_numerus(macro->corpus) > ZEPHYRUM)
    {
        Arbor2Lexema* primus;
        primus = *(Arbor2Lexema**)xar_obtinere(macro->corpus, ZEPHYRUM);

        /* Check for type keywords: int, char, void, etc. */
        commutatio (primus->genus)
        {
            casus ARBOR2_LEXEMA_INT:
            casus ARBOR2_LEXEMA_CHAR:
            casus ARBOR2_LEXEMA_VOID:
            casus ARBOR2_LEXEMA_FLOAT:
            casus ARBOR2_LEXEMA_DOUBLE:
            casus ARBOR2_LEXEMA_LONG:
            casus ARBOR2_LEXEMA_SHORT:
            casus ARBOR2_LEXEMA_SIGNED:
            casus ARBOR2_LEXEMA_UNSIGNED:
            casus ARBOR2_LEXEMA_STRUCT:
            casus ARBOR2_LEXEMA_UNION:
            casus ARBOR2_LEXEMA_ENUM:
                redde VERUM;
            ordinarius:
                frange;
        }
    }

    redde FALSUM;
}

/* Check if an identifier is likely a type (typedef or type-suggesting macro) */
b32
arbor2_glr_est_probabiliter_typus(Arbor2GLR* glr, Arbor2Token* tok)
{
    redde _potest_esse_typus(glr, tok) || _macro_suggerit_typum(glr, tok);
}

/* ==================================================
 * GLR Algorithm - Merge Support
 * ================================================== */

/* Compare two AST nodes for equality (shallow) */
interior b32
_nodi_aequales(Arbor2Nodus* a, Arbor2Nodus* b)
{
    si (a == NIHIL && b == NIHIL) redde VERUM;
    si (a == NIHIL || b == NIHIL) redde FALSUM;
    si (a->genus != b->genus) redde FALSUM;

    /* For now, just compare genus - could add deeper comparison */
    redde VERUM;
}

/* Create an ambiguous node holding multiple interpretations */
interior Arbor2Nodus*
_creare_nodum_ambiguum(
    Arbor2GLR*      glr,
    Arbor2Nodus*    a,
    Arbor2Nodus*    b,
    Arbor2Token*    lexema)
{
    Arbor2Nodus* nodus;
    Arbor2Nodus** slot;

    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->genus = ARBOR2_NODUS_AMBIGUUS;
    nodus->lexema = lexema;
    nodus->datum.ambiguus.genus = ARBOR2_AMBIG_TYPEDEF_IGNOTUM;
    nodus->datum.ambiguus.identificator = NIHIL;
    nodus->datum.ambiguus.interpretationes = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));

    slot = xar_addere(nodus->datum.ambiguus.interpretationes);
    *slot = a;
    slot = xar_addere(nodus->datum.ambiguus.interpretationes);
    *slot = b;

    /* Track ambiguous node for later resolution */
    slot = xar_addere(glr->ambigui);
    *slot = nodus;

    redde nodus;
}

/* Copy predecessors from one node to another */
interior vacuum
_copiare_praedecessores(
    Arbor2GLR*      glr,
    Arbor2GSSNodus* dest,
    Arbor2GSSNodus* src)
{
    s32 i;
    per (i = ZEPHYRUM; i < src->num_praed; i++)
    {
        _addere_praedecessorem(glr, dest, src->praedecessores[i]);
    }
}

/* Merge compatible nodes in frontier */
interior vacuum
_mergere_compatibiles(Arbor2GLR* glr, Xar* frontier)
{
    i32 i, j;
    i32 num;

    num = xar_numerus(frontier);

    per (i = ZEPHYRUM; i < num; i++)
    {
        Arbor2GSSNodus* a;
        Arbor2GSSNodus** slot_a;

        slot_a = xar_obtinere(frontier, i);
        a = *slot_a;

        si (a == NIHIL) perge;  /* Already merged away */

        per (j = i + I; j < num; j++)
        {
            Arbor2GSSNodus* b;
            Arbor2GSSNodus** slot_b;

            slot_b = xar_obtinere(frontier, j);
            b = *slot_b;

            si (b == NIHIL) perge;

            si (a->status == b->status)
            {
                /* Same state - merge! */
                si (_nodi_aequales(a->valor, b->valor))
                {
                    /* True merge - just combine predecessors */
                    _copiare_praedecessores(glr, a, b);
                    glr->num_mergae++;
                }
                alioquin
                {
                    /* Different values - create ambiguous node */
                    a->valor = _creare_nodum_ambiguum(glr, a->valor, b->valor, a->lexema);
                    _copiare_praedecessores(glr, a, b);
                }

                /* Mark b as merged away */
                *slot_b = NIHIL;
            }
        }
    }
}

/* ==================================================
 * GLR Algorithm - Action Execution
 * ================================================== */

interior vacuum
_exequi_shift(
    Arbor2GLR*      glr,
    Arbor2GSSNodus* nodus,
    s32             status_novus)
{
    Arbor2GSSNodus* nodus_novus;
    Arbor2Token* lexema;
    Arbor2Nodus* valor;
    Arbor2GSSNodus** slot;

    lexema = _currens_lexema(glr);

    /* Create leaf node for token */
    si (lexema->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        valor = arbor2_nodus_creare_folium(glr, ARBOR2_NODUS_IDENTIFICATOR, lexema);
    }
    alioquin si (lexema->lexema->genus == ARBOR2_LEXEMA_INTEGER)
    {
        valor = arbor2_nodus_creare_folium(glr, ARBOR2_NODUS_INTEGER, lexema);
    }
    alioquin si (lexema->lexema->genus == ARBOR2_LEXEMA_FLOAT_LIT)
    {
        valor = arbor2_nodus_creare_folium(glr, ARBOR2_NODUS_FLOAT, lexema);
    }
    alioquin si (lexema->lexema->genus == ARBOR2_LEXEMA_CHAR_LIT)
    {
        valor = arbor2_nodus_creare_folium(glr, ARBOR2_NODUS_CHAR, lexema);
    }
    alioquin si (lexema->lexema->genus == ARBOR2_LEXEMA_STRING_LIT)
    {
        valor = arbor2_nodus_creare_folium(glr, ARBOR2_NODUS_STRING, lexema);
    }
    alioquin
    {
        valor = NIHIL;  /* Operators don't create leaf nodes directly */
    }

    nodus_novus = _creare_gss_nodum(glr, status_novus, valor, lexema);
    _addere_praedecessorem(glr, nodus_novus, nodus);

    slot = xar_addere(glr->frons_nova);
    *slot = nodus_novus;
}

/* ==================================================
 * GLR Algorithm - Main Loop
 *
 * The key insight: reductions add to a pending list,
 * and we process until no more reductions are possible.
 * Then shifts move us to the next token.
 * ================================================== */

interior b32
_processare_unam_actionem(
    Arbor2GLR*          glr,
    Arbor2GSSNodus*     nodus,
    Arbor2LexemaGenus   genus,
    Xar*                reducenda,      /* Queue for nodes needing reduction processing */
    b32*                acceptatum,
    Arbor2GSSNodus**    nodus_acceptatus)  /* Output: the accepting node */
{
    Xar* actiones;
    i32 j;
    i32 num_actiones;
    b32 habuit_shift;

    actiones = arbor2_glr_quaerere_actiones(glr, (i32)nodus->status, genus);
    num_actiones = xar_numerus(actiones);

#if GLR_DEBUG
    printf("  [DEBUG] State %d, token %s: %d actions\n",
           nodus->status, arbor2_lexema_genus_nomen(genus), num_actiones);
#endif

    si (num_actiones == ZEPHYRUM)
    {
#if GLR_DEBUG
        printf("  [DEBUG] NO ACTIONS - path dies\n");
#endif
        redde FALSUM;  /* No valid action - this path dies */
    }

    /* Typedef pruning: if multiple actions and node has identifier that is typedef,
     * filter out REDUCE actions (expression path) and keep only SHIFT (declaration) */
    si (num_actiones > I && nodus->lexema != NIHIL)
    {
        si (arbor2_glr_est_probabiliter_typus(glr, nodus->lexema))
        {
            /* Prune expression path - keep only SHIFT actions */
            Xar* filtered = xar_creare(glr->piscina, magnitudo(Arbor2TabulaActio*));
            i32 k;
            per (k = ZEPHYRUM; k < num_actiones; k++)
            {
                Arbor2TabulaActio* act = *(Arbor2TabulaActio**)xar_obtinere(actiones, k);
                si (act->actio == ARBOR2_ACTIO_SHIFT)
                {
                    Arbor2TabulaActio** slot = xar_addere(filtered);
                    *slot = act;
                }
            }
            si (xar_numerus(filtered) > ZEPHYRUM)
            {
                actiones = filtered;
                num_actiones = xar_numerus(actiones);
            }
        }
    }

    si (num_actiones > I)
    {
        glr->num_furcae++;
    }

    habuit_shift = FALSUM;

    per (j = ZEPHYRUM; j < num_actiones; j++)
    {
        Arbor2TabulaActio* actio;

        actio = *(Arbor2TabulaActio**)xar_obtinere(actiones, j);

        commutatio (actio->actio)
        {
            casus ARBOR2_ACTIO_SHIFT:
#if GLR_DEBUG
                printf("  [DEBUG] -> SHIFT to state %d\n", actio->valor);
#endif
                _exequi_shift(glr, nodus, (s32)actio->valor);
                habuit_shift = VERUM;
                frange;

            casus ARBOR2_ACTIO_REDUCE:
            {
                /* Execute reduce and add result to pending queue */
                Arbor2Regula* regula;

                regula = arbor2_glr_obtinere_regula(glr, actio->valor);
                si (regula == NIHIL)
                {
                    frange;
                }

#if GLR_DEBUG
                printf("  [DEBUG] -> REDUCE P%d (pop %d, NT=%s)\n",
                       actio->valor, regula->longitudo, arbor2_nt_nomen(regula->sinister));
#endif

                /* For merged nodes, we need to explore ALL predecessor paths.
                 * This is critical for GLR parsing correctness.
                 * Multi-predecessors can occur at ANY depth of the GSS. */
                {
                    GSSPath gss_paths[MAX_GSS_PATHS];
                    GSSPath current_path;
                    s32 num_gss_paths;
                    s32 path_idx;

                    /* Enumerate all paths of the required length through the GSS */
                    num_gss_paths = _enumerare_gss_vias(
                        nodus,
                        ZEPHYRUM,
                        (s32)regula->longitudo,
                        &current_path,
                        gss_paths,
                        MAX_GSS_PATHS,
                        ZEPHYRUM);

                    /* Process each path */
                    per (path_idx = ZEPHYRUM; path_idx < num_gss_paths; path_idx++)
                    {
                        Arbor2GSSNodus* cursor;
                        Arbor2Nodus* valor_novus;
                        s32 goto_status;
                        Arbor2GSSNodus* nodus_novus;
                        Arbor2GSSNodus** slot;
                        Arbor2Nodus* valori[16];
                        Arbor2Token* lexemata[16];
                        s32 num_pop;
                        s32 k;

                        cursor = gss_paths[path_idx].end_cursor;
                        num_pop = (s32)regula->longitudo;

                        /* Copy values and lexemes from the path */
                        per (k = ZEPHYRUM; k < num_pop; k++)
                        {
                            valori[k] = gss_paths[path_idx].valori[k];
                            lexemata[k] = gss_paths[path_idx].lexemata[k];
                        }

                        si (cursor == NIHIL && regula->longitudo > ZEPHYRUM)
                        {
                            perge;  /* Path died - not enough on stack */
                        }

                /* Build AST node */
                commutatio (regula->nodus_genus)
                {
                    casus ARBOR2_NODUS_BINARIUM:
                        si (num_pop >= III)
                        {
                            valor_novus = _construere_nodum_binarium(glr,
                                valori[II], lexemata[I], valori[ZEPHYRUM]);
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_UNARIUM:
                        si (num_pop >= II)
                        {
                            valor_novus = _construere_nodum_unarium(glr,
                                lexemata[I], valori[ZEPHYRUM]);
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_SIZEOF:
                        /* P143: factor -> 'sizeof' factor (2 symbols)
                         * P162-P165: factor -> 'sizeof' '(' type ')' (4 symbols)
                         * P166-P169: factor -> 'sizeof' '(' type '*' ')' (5 symbols)
                         * P170-P173: factor -> 'sizeof' '(' type '*' '*' ')' (6 symbols)
                         * P183,P186,P189: factor -> 'sizeof' '(' STRUCT/UNION/ENUM ID ')' (5 symbols)
                         * P184,P187,P190: factor -> 'sizeof' '(' STRUCT/UNION/ENUM ID '*' ')' (6 symbols)
                         * P185,P188,P191: factor -> 'sizeof' '(' STRUCT/UNION/ENUM ID '*' '*' ')' (7 symbols) */
                        si (num_pop == II)
                        {
                            /* P143: sizeof expr */
                            Arbor2Nodus* nodus_sizeof;

                            nodus_sizeof = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_sizeof->genus = ARBOR2_NODUS_SIZEOF;
                            nodus_sizeof->lexema = lexemata[I];  /* sizeof token */
                            nodus_sizeof->datum.sizeof_expr.est_typus = FALSUM;
                            nodus_sizeof->datum.sizeof_expr.operandum = valori[ZEPHYRUM];

                            valor_novus = nodus_sizeof;
                        }
                        alioquin si (num_pop >= IV && num_pop <= VII)
                        {
                            /* sizeof(type) with various forms */
                            Arbor2Nodus* nodus_sizeof;
                            Arbor2Nodus* nodus_typus;
                            s32 num_stellae;
                            s32 prod_num;
                            s32 type_token_idx;

                            prod_num = (s32)actio->valor;

                            /* Determinare numerum stellarum (pointers)
                             * P183-P191 (struct/union/enum): extra symbol, so subtract V
                             * P162-P173 (basic types): subtract IV */
                            si (prod_num >= 183 && prod_num <= 191)
                            {
                                /* sizeof(struct/union/enum): 5=base, 6=*, 7=** */
                                num_stellae = num_pop - V;
                                type_token_idx = num_pop - III;  /* ID token (struct ID) */
                            }
                            alioquin
                            {
                                /* sizeof(basic type): 4=base, 5=*, 6=** */
                                num_stellae = num_pop - IV;
                                type_token_idx = num_pop - II;  /* type keyword */
                            }

                            /* Creare nodus typus */
                            nodus_typus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_typus->genus = ARBOR2_NODUS_DECLARATOR;
                            nodus_typus->lexema = lexemata[type_token_idx];  /* type token */
                            nodus_typus->datum.declarator.num_stellae = num_stellae;
                            nodus_typus->datum.declarator.titulus.datum = NIHIL;
                            nodus_typus->datum.declarator.titulus.mensura = ZEPHYRUM;

                            /* Creare nodus sizeof */
                            nodus_sizeof = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_sizeof->genus = ARBOR2_NODUS_SIZEOF;
                            nodus_sizeof->lexema = lexemata[num_pop - I];  /* sizeof token */
                            nodus_sizeof->datum.sizeof_expr.est_typus = VERUM;
                            nodus_sizeof->datum.sizeof_expr.operandum = nodus_typus;

                            valor_novus = nodus_sizeof;
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_CONVERSIO:
                        /* P144-P147: factor -> '(' type ')' factor (4 symbols)
                         * P154-P157: factor -> '(' type '*' ')' factor (5 symbols)
                         * P158-P161: factor -> '(' type '*' '*' ')' factor (6 symbols)
                         * P174,P177,P180: factor -> '(' STRUCT/UNION/ENUM ID ')' factor (5 symbols)
                         * P175,P178,P181: factor -> '(' STRUCT/UNION/ENUM ID '*' ')' factor (6 symbols)
                         * P176,P179,P182: factor -> '(' STRUCT/UNION/ENUM ID '*' '*' ')' factor (7 symbols)
                         * lexemata[n-1]='(', lexemata[n-2]=type, ..., valori[0]=operand */
                        si (num_pop >= IV && num_pop <= VII)
                        {
                            Arbor2Nodus* nodus_cast;
                            Arbor2Nodus* nodus_typus;
                            s32 num_stellae;
                            s32 prod_num;
                            s32 type_token_idx;

                            prod_num = (s32)actio->valor;

                            /* Determinare numerum stellarum (pointers)
                             * P174-P182 (struct/union/enum): extra symbol, so subtract V
                             * P144-P161 (basic types): subtract IV */
                            si (prod_num >= 174 && prod_num <= 182)
                            {
                                /* struct/union/enum casts: 5=base, 6=*, 7=** */
                                num_stellae = num_pop - V;
                                type_token_idx = num_pop - III;  /* ID token (struct ID) */
                            }
                            alioquin
                            {
                                /* basic type casts: 4=base, 5=*, 6=** */
                                num_stellae = num_pop - IV;
                                type_token_idx = num_pop - II;  /* type keyword */
                            }

                            /* Creare nodus typus */
                            nodus_typus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_typus->genus = ARBOR2_NODUS_DECLARATOR;
                            nodus_typus->lexema = lexemata[type_token_idx];  /* type token */
                            nodus_typus->datum.declarator.num_stellae = num_stellae;
                            nodus_typus->datum.declarator.titulus.datum = NIHIL;
                            nodus_typus->datum.declarator.titulus.mensura = ZEPHYRUM;

                            /* Creare nodus conversio */
                            nodus_cast = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_cast->genus = ARBOR2_NODUS_CONVERSIO;
                            nodus_cast->lexema = lexemata[type_token_idx];  /* type token for display */
                            nodus_cast->datum.conversio.typus = nodus_typus;
                            nodus_cast->datum.conversio.expressio = valori[ZEPHYRUM];

                            valor_novus = nodus_cast;
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_TERNARIUS:
                        /* P123: ternarius -> disiunctio '?' ternarius ':' ternarius
                         * 5 symbols: valori[4]=cond, [3]=?, [2]=verum, [1]=:, [0]=falsum */
                        si (num_pop >= V)
                        {
                            Arbor2Nodus* nodus_tern;
                            nodus_tern = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_tern->genus = ARBOR2_NODUS_TERNARIUS;
                            nodus_tern->lexema = lexemata[III];  /* ? token */
                            nodus_tern->datum.ternarius.conditio = valori[IV];
                            nodus_tern->datum.ternarius.verum = valori[II];
                            nodus_tern->datum.ternarius.falsum = valori[ZEPHYRUM];
                            valor_novus = nodus_tern;
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_SUBSCRIPTIO:
                        /* P128: postfixum -> postfixum '[' expressio ']'
                         * 4 symbols: valori[3]=base, [2]='[', [1]=index, [0]=']' */
                        si (num_pop >= IV)
                        {
                            Arbor2Nodus* nodus_sub;
                            nodus_sub = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_sub->genus = ARBOR2_NODUS_SUBSCRIPTIO;
                            nodus_sub->lexema = lexemata[II];  /* '[' token */
                            nodus_sub->datum.subscriptio.basis = valori[III];
                            nodus_sub->datum.subscriptio.index = valori[I];
                            valor_novus = nodus_sub;
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_VOCATIO:
                        /* P130: postfixum -> postfixum '(' ')'
                         * 3 symbols: valori[2]=base, [1]='(', [0]=')'
                         * P131: postfixum -> postfixum '(' argumenta ')'
                         * 4 symbols: valori[3]=base, [2]='(', [1]=args, [0]=')' */
                        si (actio->valor == 130)
                        {
                            /* Empty call: foo() */
                            Arbor2Nodus* nodus_call;
                            nodus_call = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_call->genus = ARBOR2_NODUS_VOCATIO;
                            nodus_call->lexema = lexemata[I];  /* '(' token */
                            nodus_call->datum.vocatio.basis = valori[II];
                            nodus_call->datum.vocatio.argumenta = NIHIL;
                            valor_novus = nodus_call;
                        }
                        alioquin si (actio->valor == 131 && num_pop >= IV)
                        {
                            /* Call with args: foo(a, b, c)
                             * valori[1] is an argument list node we need to extract */
                            Arbor2Nodus* nodus_call;
                            nodus_call = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_call->genus = ARBOR2_NODUS_VOCATIO;
                            nodus_call->lexema = lexemata[II];  /* '(' token */
                            nodus_call->datum.vocatio.basis = valori[III];
                            /* valori[I] is the argumenta - we need to collect the args */
                            nodus_call->datum.vocatio.argumenta = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            /* For now, just store the single arg expression (TODO: handle multiple args properly) */
                            {
                                Arbor2Nodus** slot = xar_addere(nodus_call->datum.vocatio.argumenta);
                                *slot = valori[I];  /* argumenta node contains the args */
                            }
                            valor_novus = nodus_call;
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_MEMBRUM:
                        /* P134: postfixum -> postfixum '.' ID (3 symbols)
                         * P135: postfixum -> postfixum '->' ID (3 symbols)
                         * valori[2]=base, [1]=operator, [0]=member ID token */
                        si (num_pop >= III)
                        {
                            Arbor2Nodus* nodus_mem;
                            nodus_mem = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_mem->genus = ARBOR2_NODUS_MEMBRUM;
                            nodus_mem->lexema = lexemata[I];  /* '.' or '->' token */
                            nodus_mem->datum.membrum.basis = valori[II];
                            nodus_mem->datum.membrum.membrum = lexemata[ZEPHYRUM]->lexema->valor;
                            nodus_mem->datum.membrum.est_sagitta = (actio->valor == 135);
                            valor_novus = nodus_mem;
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_POST_UNARIUM:
                        /* P136: postfixum -> postfixum '++' (2 symbols)
                         * P137: postfixum -> postfixum '--' (2 symbols)
                         * valori[1]=operand, lexemata[0]=operator */
                        si (num_pop >= II)
                        {
                            Arbor2Nodus* nodus_post;
                            nodus_post = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_post->genus = ARBOR2_NODUS_POST_UNARIUM;
                            nodus_post->lexema = lexemata[ZEPHYRUM];  /* '++' or '--' token */
                            nodus_post->datum.post_unarium.operandum = valori[I];
                            nodus_post->datum.post_unarium.operator = lexemata[ZEPHYRUM]->lexema->genus;
                            valor_novus = nodus_post;
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_IDENTIFICATOR:
                    casus ARBOR2_NODUS_INTEGER:
                    casus ARBOR2_NODUS_FLOAT:
                    casus ARBOR2_NODUS_CHAR:
                    casus ARBOR2_NODUS_STRING:
                        valor_novus = (num_pop > ZEPHYRUM) ? valori[ZEPHYRUM] : NIHIL;
                        frange;

                    casus ARBOR2_NODUS_DECLARATIO:
                        /* Check for struct member rules P48-P51 first */
                        si (actio->valor == 48)
                        {
                            /* P48: struct_member_list -> type_spec declarator ';' (3 symbols)
                             * lexemata: [2]=type, [1]=declarator_tok, [0]=;
                             * valori: [1]=declarator node (pre-built) */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node = valori[I];
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[II];
#if GLR_DEBUG
                            printf("  [DEBUG P48] Creating member list with declarator, type=%.*s\n",
                                   (integer)type_tok->lexema->valor.mensura, type_tok->lexema->valor.datum);
#endif
                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
#if GLR_DEBUG
                            printf("  [DEBUG P48] Created lista=%p with %d members\n",
                                   (vacuum*)lista, xar_numerus(lista));
#endif
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 49)
                        {
                            /* P49: member_list type_spec declarator ';' (4 symbols)
                             * valori: [3]=list, [1]=declarator node (pre-built)
                             * lexemata: [2]=type, [0]=; */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node = valori[I];
                            Xar* lista = (Xar*)valori[III];
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[II];

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 50)
                        {
                            /* P50: UNUSED - pointer members now handled by P48 via declarator
                             * Keeping code for backwards compatibility, but state machine won't trigger this.
                             * Original: type_spec '*' ID ';' (4 symbols, pointer first member) */
                            /* lexemata: [3]=type, [2]=*, [1]=name, [0]=; */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[III];
                            Arbor2Token* name_tok = lexemata[I];

                            decl_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            decl_node->genus = ARBOR2_NODUS_DECLARATOR;
                            decl_node->lexema = name_tok;
                            decl_node->datum.declarator.num_stellae = I;
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;
                            decl_node->datum.declarator.latitudo_biti = NIHIL;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 51)
                        {
                            /* P51: UNUSED - pointer members now handled by P49 via declarator
                             * Keeping code for backwards compatibility, but state machine won't trigger this.
                             * Original: member_list type_spec '*' ID ';' (5 symbols, append pointer) */
                            /* valori: [4]=list, [3]=type, [2]=*, [1]=name, [0]=; */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista = (Xar*)valori[IV];
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[III];
                            Arbor2Token* name_tok = lexemata[I];

                            decl_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            decl_node->genus = ARBOR2_NODUS_DECLARATOR;
                            decl_node->lexema = name_tok;
                            decl_node->datum.declarator.num_stellae = I;
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;
                            decl_node->datum.declarator.latitudo_biti = NIHIL;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        /* ========== BIT FIELD RULES P62-P65 ========== */
                        alioquin si (actio->valor == 62)
                        {
                            /* P62: type_spec ID ':' expr ';' (5 symbols, first named bit field) */
                            /* lexemata: [4]=type, [3]=name, [2]=':', [1]=expr_tok, [0]=';' */
                            /* valori: [1]=expr_node */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[IV];
                            Arbor2Token* name_tok = lexemata[III];
                            Arbor2Nodus* expr_node = valori[I];
#if GLR_DEBUG
                            printf("  [DEBUG P62] Creating bit field member, type=%.*s, name=%.*s\n",
                                   (integer)type_tok->lexema->valor.mensura, type_tok->lexema->valor.datum,
                                   (integer)name_tok->lexema->valor.mensura, name_tok->lexema->valor.datum);
#endif
                            decl_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            decl_node->genus = ARBOR2_NODUS_DECLARATOR;
                            decl_node->lexema = name_tok;
                            decl_node->datum.declarator.num_stellae = ZEPHYRUM;
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;
                            decl_node->datum.declarator.latitudo_biti = expr_node;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 63)
                        {
                            /* P63: member_list type_spec ID ':' expr ';' (6 symbols, append named bit field) */
                            /* valori: [5]=list, [1]=expr_node */
                            /* lexemata: [4]=type, [3]=name */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista = (Xar*)valori[V];
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[IV];
                            Arbor2Token* name_tok = lexemata[III];
                            Arbor2Nodus* expr_node = valori[I];

                            decl_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            decl_node->genus = ARBOR2_NODUS_DECLARATOR;
                            decl_node->lexema = name_tok;
                            decl_node->datum.declarator.num_stellae = ZEPHYRUM;
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;
                            decl_node->datum.declarator.latitudo_biti = expr_node;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 64)
                        {
                            /* P64: type_spec ':' expr ';' (4 symbols, first anonymous bit field) */
                            /* lexemata: [3]=type, [2]=':', [0]=';' */
                            /* valori: [1]=expr_node */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[III];
                            Arbor2Nodus* expr_node = valori[I];
#if GLR_DEBUG
                            printf("  [DEBUG P64] Creating anonymous bit field member, type=%.*s\n",
                                   (integer)type_tok->lexema->valor.mensura, type_tok->lexema->valor.datum);
#endif
                            decl_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            decl_node->genus = ARBOR2_NODUS_DECLARATOR;
                            decl_node->lexema = type_tok;  /* use type token since no name */
                            decl_node->datum.declarator.num_stellae = ZEPHYRUM;
                            decl_node->datum.declarator.titulus.datum = NIHIL;
                            decl_node->datum.declarator.titulus.mensura = ZEPHYRUM;  /* anonymous */
                            decl_node->datum.declarator.latitudo_biti = expr_node;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 65)
                        {
                            /* P65: member_list type_spec ':' expr ';' (5 symbols, append anonymous bit field) */
                            /* valori: [4]=list, [1]=expr_node */
                            /* lexemata: [3]=type */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista = (Xar*)valori[IV];
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[III];
                            Arbor2Nodus* expr_node = valori[I];

                            decl_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            decl_node->genus = ARBOR2_NODUS_DECLARATOR;
                            decl_node->lexema = type_tok;  /* use type token since no name */
                            decl_node->datum.declarator.num_stellae = ZEPHYRUM;
                            decl_node->datum.declarator.titulus.datum = NIHIL;
                            decl_node->datum.declarator.titulus.mensura = ZEPHYRUM;  /* anonymous */
                            decl_node->datum.declarator.latitudo_biti = expr_node;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        /* ========== NESTED TYPE AS MEMBER P66-P73 ========== */
                        alioquin si (actio->valor == 66)
                        {
                            /* P66: struct_specifier declarator ';' (3 symbols, first nested struct member)
                             * E8: Uses pre-built declarator from valori[I] */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node = valori[I];  /* Pre-built declarator! */
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Nodus* spec_node = valori[II];
#ifdef DEBUG_PARSER
                            printf("  [DEBUG P66] Nested struct member with declarator, name=%.*s\n",
                                   (integer)decl_node->datum.declarator.titulus.mensura,
                                   decl_node->datum.declarator.titulus.datum);
#endif
                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = spec_node->lexema;
                            member->datum.declaratio.specifier = spec_node;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 67)
                        {
                            /* P67: UNUSED - pointer variants now handled via declarator */
                            valor_novus = NIHIL;
                        }
                        alioquin si (actio->valor == 68)
                        {
                            /* P68: member_list struct_specifier declarator ';' (4 symbols, append nested)
                             * E8: Uses pre-built declarator from valori[I] */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node = valori[I];  /* Pre-built declarator! */
                            Xar* lista = (Xar*)valori[III];
                            Arbor2Nodus** slot;
                            Arbor2Nodus* spec_node = valori[II];

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = spec_node->lexema;
                            member->datum.declaratio.specifier = spec_node;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 69)
                        {
                            /* P69: UNUSED - pointer variants now handled via declarator */
                            valor_novus = NIHIL;
                        }
                        /* P70-P73: Same as P66-P69 but for enum_specifier */
                        alioquin si (actio->valor == 70)
                        {
                            /* P70: enum_specifier declarator ';' (3 symbols, first nested enum member)
                             * E8: Uses pre-built declarator from valori[I] */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node = valori[I];  /* Pre-built declarator! */
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Nodus* spec_node = valori[II];

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = spec_node->lexema;
                            member->datum.declaratio.specifier = spec_node;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 71)
                        {
                            /* P71: UNUSED - pointer variants now handled via declarator */
                            valor_novus = NIHIL;
                        }
                        alioquin si (actio->valor == 72)
                        {
                            /* P72: member_list enum_specifier declarator ';' (4 symbols, append nested enum)
                             * E8: Uses pre-built declarator from valori[I] */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node = valori[I];  /* Pre-built declarator! */
                            Xar* lista = (Xar*)valori[III];
                            Arbor2Nodus** slot;
                            Arbor2Nodus* spec_node = valori[II];

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = spec_node->lexema;
                            member->datum.declaratio.specifier = spec_node;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 73)
                        {
                            /* P73: UNUSED - pointer variants now handled via declarator */
                            valor_novus = NIHIL;
                        }
                        /* ========== TYPEDEF DECLARATIONS P74-P79 ========== */
                        alioquin si (actio->valor == 74)
                        {
                            /* P74: 'typedef' type_specifier declarator ';' (4 symbols)
                             * E8: Uses pre-built declarator from valori[1]
                             * lexemata: [3]=typedef, [2]=type, [1]=..., [0]=;
                             * valori: [1]=declarator node (pre-built) */
                            Arbor2Nodus* decl_node = valori[I];  /* Pre-built declarator! */
                            Arbor2Nodus* spec_node;
                            Arbor2Token* type_tok = lexemata[II];

                            spec_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            spec_node->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            spec_node->lexema = type_tok;
                            spec_node->datum.folium.valor = type_tok->lexema->valor;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = lexemata[III];  /* typedef token */
                            valor_novus->datum.declaratio.specifier = spec_node;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = VERUM;
                        }
                        alioquin si (actio->valor == 75)
                        {
                            /* P75: UNUSED - pointer typedefs now handled by P74 via declarator */
                            (vacuum)valori;
                            (vacuum)lexemata;
                            valor_novus = NIHIL;
                        }
                        alioquin si (actio->valor == 76)
                        {
                            /* P76: 'typedef' struct_specifier declarator ';' (4 symbols)
                             * E8: Uses pre-built declarator from valori[1]
                             * valori: [2]=struct_specifier_node, [1]=declarator
                             * lexemata: [3]=typedef, ..., [0]=; */
                            Arbor2Nodus* decl_node = valori[I];  /* Pre-built declarator! */
                            Arbor2Nodus* spec_node = valori[II];

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = lexemata[III];  /* typedef token */
                            valor_novus->datum.declaratio.specifier = spec_node;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = VERUM;
                        }
                        alioquin si (actio->valor == 77)
                        {
                            /* P77: UNUSED - struct pointer typedefs now handled by P76 via declarator */
                            (vacuum)valori;
                            (vacuum)lexemata;
                            valor_novus = NIHIL;
                        }
                        alioquin si (actio->valor == 78)
                        {
                            /* P78: 'typedef' enum_specifier declarator ';' (4 symbols)
                             * E8: Uses pre-built declarator from valori[1]
                             * valori: [2]=enum_specifier_node, [1]=declarator
                             * lexemata: [3]=typedef, ..., [0]=; */
                            Arbor2Nodus* decl_node = valori[I];  /* Pre-built declarator! */
                            Arbor2Nodus* spec_node = valori[II];

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = lexemata[III];  /* typedef token */
                            valor_novus->datum.declaratio.specifier = spec_node;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = VERUM;
                        }
                        alioquin si (actio->valor == 79)
                        {
                            /* P79: UNUSED - enum pointer typedefs now handled by P78 via declarator */
                            (vacuum)valori;
                            (vacuum)lexemata;
                            valor_novus = NIHIL;
                        }
                        /* P148-P153: Storage class and qualifier declarations */
                        alioquin si (actio->valor >= 148 && actio->valor <= 153)
                        {
                            /* 3 symbols: specifier type declarator
                             * lexemata[2] = storage/qualifier, lexemata[1] = type
                             * valori[0] = declarator */
                            Arbor2Nodus* type_spec;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = lexemata[I];  /* type token */

                            /* Creare specifier nodus ex typo */
                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = lexemata[I];

                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = valori[ZEPHYRUM];
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;

                            /* Ponere storage_class vel qualifiers */
                            commutatio (actio->valor)
                            {
                                casus 148:  /* static */
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_STATIC;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                                    frange;
                                casus 149:  /* extern */
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_EXTERN;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                                    frange;
                                casus 150:  /* register */
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_REGISTER;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                                    frange;
                                casus 151:  /* auto */
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_AUTO;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                                    frange;
                                casus 152:  /* const */
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_CONST;
                                    frange;
                                casus 153:  /* volatile */
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_VOLATILE;
                                    frange;
                                ordinarius:
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                                    frange;
                            }
                        }
                        /* P192: declaration with initializer */
                        alioquin si (actio->valor == 192)
                        {
                            /* P192: decl -> type declarator '=' assignatio (4 symbols)
                             * lexemata: [3]=type, [2]=declarator_name, [1]='=', [0]=init_end
                             * valori: [3]=type_spec?, [2]=declarator, [1]=nil, [0]=assignatio */
                            Arbor2Nodus* type_spec;
                            Arbor2Nodus* decl_node = valori[II];  /* Pre-built declarator */
                            Arbor2Nodus* init_expr = valori[ZEPHYRUM];  /* Initializer expression */
                            Arbor2Token* type_tok = lexemata[III];

                            /* Creare specifier nodus ex typo */
                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = type_tok;
                            type_spec->datum.folium.valor = type_tok->lexema->valor;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = type_tok;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = init_expr;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                            valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                        }
                        /* P193-P198: storage class/qualifier + type declarator = assignatio */
                        alioquin si (actio->valor >= 193 && actio->valor <= 198)
                        {
                            /* 5 symbols: storage/qual, type, declarator_name, '=', assignatio
                             * lexemata: [4]=storage/qual, [3]=type, [2]=name, [1]='=', [0]=end
                             * valori: [4]=nil, [3]=type?, [2]=declarator, [1]=nil, [0]=assignatio */
                            Arbor2Nodus* type_spec;
                            Arbor2Nodus* decl_node = valori[II];  /* Pre-built declarator */
                            Arbor2Nodus* init_expr = valori[ZEPHYRUM];  /* Initializer expression */
                            Arbor2Token* type_tok = lexemata[III];
                            i32 storage = ARBOR2_STORAGE_NONE;
                            i32 quals = ARBOR2_QUAL_NONE;

                            /* Determine storage class or qualifier based on production */
                            si (actio->valor == 193) storage = ARBOR2_STORAGE_STATIC;
                            alioquin si (actio->valor == 194) storage = ARBOR2_STORAGE_EXTERN;
                            alioquin si (actio->valor == 195) storage = ARBOR2_STORAGE_REGISTER;
                            alioquin si (actio->valor == 196) storage = ARBOR2_STORAGE_AUTO;
                            alioquin si (actio->valor == 197) quals = ARBOR2_QUAL_CONST;
                            alioquin si (actio->valor == 198) quals = ARBOR2_QUAL_VOLATILE;

                            /* Creare specifier nodus ex typo */
                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = type_tok;
                            type_spec->datum.folium.valor = type_tok->lexema->valor;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = type_tok;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = init_expr;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = storage;
                            valor_novus->datum.declaratio.qualifiers = quals;
                        }
                        /* P199: declaration with brace initializer */
                        alioquin si (actio->valor == 199)
                        {
                            /* P199: decl -> type declarator '=' init_lista (4 symbols)
                             * valori: [3]=type_spec?, [2]=declarator, [1]=nil, [0]=init_lista */
                            Arbor2Nodus* type_spec;
                            Arbor2Nodus* decl_node = valori[II];
                            Arbor2Nodus* init_lista = valori[ZEPHYRUM];
                            Arbor2Token* type_tok = lexemata[III];

                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = type_tok;
                            type_spec->datum.folium.valor = type_tok->lexema->valor;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = type_tok;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = init_lista;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                            valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                        }
                        /* P207-P212: storage class/qualifier + brace initializer */
                        alioquin si (actio->valor >= 207 && actio->valor <= 212)
                        {
                            /* 5 symbols: storage/qual, type, declarator, '=', init_lista
                             * valori: [4]=nil, [3]=type?, [2]=declarator, [1]=nil, [0]=init_lista */
                            Arbor2Nodus* type_spec;
                            Arbor2Nodus* decl_node = valori[II];
                            Arbor2Nodus* init_lista = valori[ZEPHYRUM];
                            Arbor2Token* type_tok = lexemata[III];
                            i32 storage = ARBOR2_STORAGE_NONE;
                            i32 quals = ARBOR2_QUAL_NONE;

                            si (actio->valor == 207) storage = ARBOR2_STORAGE_STATIC;
                            alioquin si (actio->valor == 208) storage = ARBOR2_STORAGE_EXTERN;
                            alioquin si (actio->valor == 209) storage = ARBOR2_STORAGE_REGISTER;
                            alioquin si (actio->valor == 210) storage = ARBOR2_STORAGE_AUTO;
                            alioquin si (actio->valor == 211) quals = ARBOR2_QUAL_CONST;
                            alioquin si (actio->valor == 212) quals = ARBOR2_QUAL_VOLATILE;

                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = type_tok;
                            type_spec->datum.folium.valor = type_tok->lexema->valor;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = type_tok;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = init_lista;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = storage;
                            valor_novus->datum.declaratio.qualifiers = quals;
                        }
                        /* P227-P234: Specifier combinations (4 symbols) */
                        alioquin si (actio->valor >= 227 && actio->valor <= 234)
                        {
                            /* 4 symbols: spec1, spec2, type, declarator
                             * lexemata[3]=spec1, lexemata[2]=spec2, lexemata[1]=type
                             * valori[0]=declarator */
                            Arbor2Nodus* type_spec;
                            Arbor2Nodus* decl_node = valori[ZEPHYRUM];
                            Arbor2Token* type_tok = lexemata[I];
                            i32 storage = ARBOR2_STORAGE_NONE;
                            i32 quals = ARBOR2_QUAL_NONE;

                            commutatio (actio->valor)
                            {
                                casus 227:  /* static const */
                                    storage = ARBOR2_STORAGE_STATIC;
                                    quals = ARBOR2_QUAL_CONST;
                                    frange;
                                casus 228:  /* extern const */
                                    storage = ARBOR2_STORAGE_EXTERN;
                                    quals = ARBOR2_QUAL_CONST;
                                    frange;
                                casus 229:  /* register const */
                                    storage = ARBOR2_STORAGE_REGISTER;
                                    quals = ARBOR2_QUAL_CONST;
                                    frange;
                                casus 230:  /* auto const */
                                    storage = ARBOR2_STORAGE_AUTO;
                                    quals = ARBOR2_QUAL_CONST;
                                    frange;
                                casus 231:  /* static volatile */
                                    storage = ARBOR2_STORAGE_STATIC;
                                    quals = ARBOR2_QUAL_VOLATILE;
                                    frange;
                                casus 232:  /* extern volatile */
                                    storage = ARBOR2_STORAGE_EXTERN;
                                    quals = ARBOR2_QUAL_VOLATILE;
                                    frange;
                                casus 233:  /* const volatile */
                                    storage = ARBOR2_STORAGE_NONE;
                                    quals = ARBOR2_QUAL_CONST | ARBOR2_QUAL_VOLATILE;
                                    frange;
                                casus 234:  /* volatile const */
                                    storage = ARBOR2_STORAGE_NONE;
                                    quals = ARBOR2_QUAL_CONST | ARBOR2_QUAL_VOLATILE;
                                    frange;
                                ordinarius:
                                    frange;
                            }

                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = type_tok;
                            type_spec->datum.folium.valor = type_tok->lexema->valor;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = type_tok;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = storage;
                            valor_novus->datum.declaratio.qualifiers = quals;
                        }
                        /* P235-P236: static/extern const with assignatio initializer (6 symbols) */
                        alioquin si (actio->valor >= 235 && actio->valor <= 236)
                        {
                            /* 6 symbols: spec1, const, type, declarator, '=', assignatio
                             * lexemata[5]=spec1, lexemata[4]=const, lexemata[3]=type
                             * valori[2]=declarator, valori[0]=assignatio */
                            Arbor2Nodus* type_spec;
                            Arbor2Nodus* decl_node = valori[II];
                            Arbor2Nodus* init_expr = valori[ZEPHYRUM];
                            Arbor2Token* type_tok = lexemata[III];
                            i32 storage = (actio->valor == 235) ? ARBOR2_STORAGE_STATIC : ARBOR2_STORAGE_EXTERN;

                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = type_tok;
                            type_spec->datum.folium.valor = type_tok->lexema->valor;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = type_tok;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = init_expr;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = storage;
                            valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_CONST;
                        }
                        /* P237-P238: static/extern const with brace initializer (6 symbols) */
                        alioquin si (actio->valor >= 237 && actio->valor <= 238)
                        {
                            /* 6 symbols: spec1, const, type, declarator, '=', init_lista
                             * lexemata[5]=spec1, lexemata[4]=const, lexemata[3]=type
                             * valori[2]=declarator, valori[0]=init_lista */
                            Arbor2Nodus* type_spec;
                            Arbor2Nodus* decl_node = valori[II];
                            Arbor2Nodus* init_lista = valori[ZEPHYRUM];
                            Arbor2Token* type_tok = lexemata[III];
                            i32 storage = (actio->valor == 237) ? ARBOR2_STORAGE_STATIC : ARBOR2_STORAGE_EXTERN;

                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = type_tok;
                            type_spec->datum.folium.valor = type_tok->lexema->valor;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = type_tok;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.initializor = init_lista;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = storage;
                            valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_CONST;
                        }
                        /* P239: stmt -> declaratio ';' (2 symbols, pass-through declaration) */
                        alioquin si (actio->valor == 239)
                        {
                            /* Declaration as statement - pass through the declaration node */
                            /* valori[1] = declaratio, valori[0] = ';' token (ignored) */
                            valor_novus = valori[I];
                        }
                        alioquin si (actio->valor == 226)
                        {
                            /* P226: declaratio -> type init_decl_list (2 symbols)
                             * lexemata: [1]=type, [0]=last_token
                             * valori: [1]=type_spec (if struct/enum/union) or nil (if keyword)
                             * valori: [0]=init_decl_list
                             * init_decl_list is Xar of pairs (each pair is [declarator, initializor]) */
                            Xar* lista = (Xar*)valori[ZEPHYRUM];
                            Arbor2Token* type_tok = lexemata[I];
                            Arbor2Nodus* type_spec = valori[I];
                            Arbor2Nodus* primus = NIHIL;
                            Arbor2Nodus* ultimus = NIHIL;
                            i32 num_items;
                            i32 i;

                            /* If no pre-built type_spec (simple type keyword), create one */
                            si (type_spec == NIHIL)
                            {
                                type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                                type_spec->lexema = type_tok;
                                type_spec->datum.folium.valor = type_tok->lexema->valor;
                            }

                            si (lista == NIHIL)
                            {
                                valor_novus = NIHIL;
                            }
                            alioquin
                            {
                                num_items = xar_numerus(lista);
                                per (i = ZEPHYRUM; i < num_items; i++)
                                {
                                    Xar** pair_ptr = xar_obtinere(lista, i);
                                    Xar* pair = *pair_ptr;
                                    Arbor2Nodus* decl_node = NIHIL;
                                    Arbor2Nodus* init_node = NIHIL;
                                    Arbor2Nodus* nodus;

                                    si (pair != NIHIL && xar_numerus(pair) >= II)
                                    {
                                        decl_node = *(Arbor2Nodus**)xar_obtinere(pair, ZEPHYRUM);
                                        init_node = *(Arbor2Nodus**)xar_obtinere(pair, I);
                                    }

                                    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                    nodus->genus = ARBOR2_NODUS_DECLARATIO;
                                    nodus->lexema = type_tok;
                                    nodus->datum.declaratio.specifier = type_spec;
                                    nodus->datum.declaratio.declarator = decl_node;
                                    nodus->datum.declaratio.initializor = init_node;
                                    nodus->datum.declaratio.proxima = NIHIL;
                                    nodus->datum.declaratio.est_typedef = FALSUM;
                                    nodus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                                    nodus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;

                                    /* Chain nodes together */
                                    si (primus == NIHIL)
                                    {
                                        primus = nodus;
                                    }
                                    alioquin
                                    {
                                        ultimus->datum.declaratio.proxima = nodus;
                                    }
                                    ultimus = nodus;
                                }
                                valor_novus = primus;
                            }
                        }
                        alioquin si (num_pop >= II)
                        {
                            /* Other declaration rules (P10 etc.) */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = lexemata[I];  /* type_specifier token */
                            valor_novus->datum.declaratio.specifier = valori[I];
                            valor_novus->datum.declaratio.declarator = valori[ZEPHYRUM];
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_DECLARATOR:
                        /* ========== ARRAY DECLARATOR P80-P81 ========== */
                        si (actio->valor == 80)
                        {
                            /* P80: declarator '[' expression ']' (4 symbols, sized array) */
                            /* valori: [3]=declarator, [2]='[', [1]=expr, [0]=']' */
                            Arbor2Nodus* inner = valori[III];
                            Arbor2Nodus* size_expr = valori[I];

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                            valor_novus->lexema = inner->lexema;

                            /* Copy base declarator fields */
                            valor_novus->datum.declarator.num_stellae = inner->datum.declarator.num_stellae;
                            valor_novus->datum.declarator.titulus = inner->datum.declarator.titulus;
                            valor_novus->datum.declarator.latitudo_biti = inner->datum.declarator.latitudo_biti;

                            /* Create or extend dimensions array */
                            si (inner->datum.declarator.dimensiones != NIHIL)
                            {
                                valor_novus->datum.declarator.dimensiones = inner->datum.declarator.dimensiones;
                            }
                            alioquin
                            {
                                valor_novus->datum.declarator.dimensiones = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            }

                            /* Add this dimension (expression for sized array) */
                            {
                                Arbor2Nodus** slot = xar_addere(valor_novus->datum.declarator.dimensiones);
                                *slot = size_expr;
                            }
                        }
                        alioquin si (actio->valor == 81)
                        {
                            /* P81: declarator '[' ']' (3 symbols, unsized array) */
                            /* valori: [2]=declarator, [1]='[', [0]=']' */
                            Arbor2Nodus* inner = valori[II];

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                            valor_novus->lexema = inner->lexema;

                            /* Copy base declarator fields */
                            valor_novus->datum.declarator.num_stellae = inner->datum.declarator.num_stellae;
                            valor_novus->datum.declarator.titulus = inner->datum.declarator.titulus;
                            valor_novus->datum.declarator.latitudo_biti = inner->datum.declarator.latitudo_biti;

                            /* Create or extend dimensions array */
                            si (inner->datum.declarator.dimensiones != NIHIL)
                            {
                                valor_novus->datum.declarator.dimensiones = inner->datum.declarator.dimensiones;
                            }
                            alioquin
                            {
                                valor_novus->datum.declarator.dimensiones = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            }

                            /* Add NIHIL dimension for unsized array */
                            {
                                Arbor2Nodus** slot = xar_addere(valor_novus->datum.declarator.dimensiones);
                                *slot = NIHIL;
                            }
                        }
                        alioquin si (num_pop == II)
                        {
                            /* P11: declarator -> '*' declarator */
                            /* Si interior est DECLARATOR_FUNCTI, preservare genus functi */
                            si (valori[ZEPHYRUM] != NIHIL &&
                                valori[ZEPHYRUM]->genus == ARBOR2_NODUS_DECLARATOR_FUNCTI)
                            {
                                /* Wrap function declarator with pointer - preservare FUNCTI */
                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_DECLARATOR_FUNCTI;
                                valor_novus->lexema = lexemata[I];  /* The '*' token */
                                valor_novus->datum.declarator_functi.declarator_interior =
                                    valori[ZEPHYRUM]->datum.declarator_functi.declarator_interior;
                                valor_novus->datum.declarator_functi.parametri =
                                    valori[ZEPHYRUM]->datum.declarator_functi.parametri;
                                valor_novus->datum.declarator_functi.habet_void =
                                    valori[ZEPHYRUM]->datum.declarator_functi.habet_void;
                                valor_novus->datum.declarator_functi.num_stellae =
                                    valori[ZEPHYRUM]->datum.declarator_functi.num_stellae + I;
                            }
                            alioquin
                            {
                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                                valor_novus->lexema = lexemata[I];  /* The '*' token */
                                /* Count stars: inner declarator's count + 1 */
                                si (valori[ZEPHYRUM] != NIHIL &&
                                    valori[ZEPHYRUM]->genus == ARBOR2_NODUS_DECLARATOR)
                                {
                                    valor_novus->datum.declarator.num_stellae =
                                        valori[ZEPHYRUM]->datum.declarator.num_stellae + I;
                                    valor_novus->datum.declarator.titulus =
                                        valori[ZEPHYRUM]->datum.declarator.titulus;
                                    valor_novus->datum.declarator.latitudo_biti =
                                        valori[ZEPHYRUM]->datum.declarator.latitudo_biti;
                                    valor_novus->datum.declarator.dimensiones =
                                        valori[ZEPHYRUM]->datum.declarator.dimensiones;
                                }
                                alioquin
                                {
                                    valor_novus->datum.declarator.num_stellae = I;
                                    valor_novus->datum.declarator.titulus.datum = NIHIL;
                                    valor_novus->datum.declarator.titulus.mensura = ZEPHYRUM;
                                    valor_novus->datum.declarator.latitudo_biti = NIHIL;
                                    valor_novus->datum.declarator.dimensiones = NIHIL;
                                }
                            }
                        }
                        alioquin si (num_pop == I)
                        {
                            /* P12: declarator -> IDENTIFIER */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                            valor_novus->lexema = lexemata[ZEPHYRUM];
                            valor_novus->datum.declarator.num_stellae = ZEPHYRUM;
                            valor_novus->datum.declarator.latitudo_biti = NIHIL;
                            valor_novus->datum.declarator.dimensiones = NIHIL;
                            si (valori[ZEPHYRUM] != NIHIL)
                            {
                                valor_novus->datum.declarator.titulus =
                                    valori[ZEPHYRUM]->datum.folium.valor;
                            }
                            alioquin
                            {
                                valor_novus->datum.declarator.titulus.datum = NIHIL;
                                valor_novus->datum.declarator.titulus.mensura = ZEPHYRUM;
                            }
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_DECLARATOR_FUNCTI:
                        /* P38: declarator -> declarator '(' ')' (3 symbols)
                         * P39: declarator -> declarator '(' VOID ')' (4 symbols)
                         * P40: declarator -> declarator '(' param_list ')' (4 symbols) */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_DECLARATOR_FUNCTI;
                        si (num_pop == III)
                        {
                            /* P38: declarator () */
                            /* valori[2]=declarator, [1]='(', [0]=')' */
                            valor_novus->lexema = lexemata[II];
                            valor_novus->datum.declarator_functi.declarator_interior = valori[II];
                            valor_novus->datum.declarator_functi.parametri = NIHIL;
                            valor_novus->datum.declarator_functi.habet_void = FALSUM;
                            valor_novus->datum.declarator_functi.num_stellae = ZEPHYRUM;
                        }
                        alioquin si (num_pop == IV)
                        {
                            /* P39 or P40: 4 symbols */
                            /* valori[3]=declarator, [2]='(', [1]=void/param_list, [0]=')' */
                            valor_novus->lexema = lexemata[III];
                            valor_novus->datum.declarator_functi.declarator_interior = valori[III];
                            valor_novus->datum.declarator_functi.num_stellae = ZEPHYRUM;
                            si (valori[I] == NIHIL)
                            {
                                /* P39: (void) - valori[1] is NULL from VOID token */
                                valor_novus->datum.declarator_functi.parametri = NIHIL;
                                valor_novus->datum.declarator_functi.habet_void = VERUM;
                            }
                            alioquin
                            {
                                /* P40: (param_list) - valori[1] is the Xar* */
                                valor_novus->datum.declarator_functi.parametri = (Xar*)valori[I];
                                valor_novus->datum.declarator_functi.habet_void = FALSUM;
                            }
                        }
                        alioquin
                        {
                            valor_novus->lexema = NIHIL;
                            valor_novus->datum.declarator_functi.declarator_interior = NIHIL;
                            valor_novus->datum.declarator_functi.parametri = NIHIL;
                            valor_novus->datum.declarator_functi.habet_void = FALSUM;
                            valor_novus->datum.declarator_functi.num_stellae = ZEPHYRUM;
                        }
                        frange;

                    casus ARBOR2_NODUS_SENTENTIA:
                        /* P13: statement -> expression ';' */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_SENTENTIA;
                        valor_novus->lexema = lexemata[ZEPHYRUM];  /* semicolon */
                        /* Expression is at index 1 (before semicolon) */
                        si (num_pop >= II && valori[I] != NIHIL)
                        {
                            valor_novus->datum.sententia.expressio = valori[I];
                        }
                        alioquin
                        {
                            valor_novus->datum.sententia.expressio = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_SENTENTIA_VACUA:
                        /* P14: statement -> ';' */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_SENTENTIA_VACUA;
                        valor_novus->lexema = lexemata[ZEPHYRUM];  /* semicolon */
                        valor_novus->datum.sententia.expressio = NIHIL;
                        frange;

                    casus ARBOR2_NODUS_CORPUS:
                        si (num_pop == ZEPHYRUM)
                        {
                            /* P18: statement_list -> ε (empty) */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_CORPUS;
                            valor_novus->lexema = NIHIL;
                            valor_novus->datum.corpus.sententiae =
                                xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                        }
                        alioquin si (num_pop == II)
                        {
                            /* P17: statement_list -> statement_list statement */
                            /* valori[1] = list, valori[0] = new statement */
                            Arbor2Nodus* lista = valori[I];
                            Arbor2Nodus* stmt = valori[ZEPHYRUM];
                            Arbor2Nodus** slot_stmt;

                            si (lista != NIHIL && lista->genus == ARBOR2_NODUS_CORPUS)
                            {
                                /* Append statement to existing list */
                                slot_stmt = xar_addere(lista->datum.corpus.sententiae);
                                *slot_stmt = stmt;
                                valor_novus = lista;
                            }
                            alioquin
                            {
                                /* Create new list with this statement */
                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_CORPUS;
                                valor_novus->lexema = NIHIL;
                                valor_novus->datum.corpus.sententiae =
                                    xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                                slot_stmt = xar_addere(valor_novus->datum.corpus.sententiae);
                                *slot_stmt = stmt;
                            }
                        }
                        alioquin si (num_pop == III)
                        {
                            /* P16: compound_statement -> '{' statement_list '}' */
                            /* valori[2] = '{', valori[1] = list, valori[0] = '}' */
                            Arbor2Nodus* lista = valori[I];

                            si (lista != NIHIL && lista->genus == ARBOR2_NODUS_CORPUS)
                            {
                                /* Already have CORPUS node from stmt_list, just use it */
                                valor_novus = lista;
                                valor_novus->lexema = lexemata[II];  /* '{' token */
                            }
                            alioquin
                            {
                                /* Create new CORPUS */
                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_CORPUS;
                                valor_novus->lexema = lexemata[II];
                                valor_novus->datum.corpus.sententiae =
                                    xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            }
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_SI:
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_SI;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'if' token */

                        si (num_pop == V)
                        {
                            /* P20: if_statement -> 'if' '(' expression ')' statement */
                            /* valori[4] = if, valori[3] = (, valori[2] = expr,
                               valori[1] = ), valori[0] = stmt */
                            valor_novus->datum.conditionale.conditio = valori[II];
                            valor_novus->datum.conditionale.consequens = valori[ZEPHYRUM];
                            valor_novus->datum.conditionale.alternans = NIHIL;
                        }
                        alioquin si (num_pop == VII)
                        {
                            /* P21: if_statement -> 'if' '(' expression ')' statement 'else' statement */
                            /* valori[6] = if, valori[5] = (, valori[4] = expr,
                               valori[3] = ), valori[2] = stmt, valori[1] = else,
                               valori[0] = stmt */
                            valor_novus->datum.conditionale.conditio = valori[IV];
                            valor_novus->datum.conditionale.consequens = valori[II];
                            valor_novus->datum.conditionale.alternans = valori[ZEPHYRUM];
                        }
                        alioquin
                        {
                            valor_novus->datum.conditionale.conditio = NIHIL;
                            valor_novus->datum.conditionale.consequens = NIHIL;
                            valor_novus->datum.conditionale.alternans = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_DUM:
                        /* P23: while_statement -> 'while' '(' expression ')' statement */
                        /* valori[4] = while, valori[3] = (, valori[2] = expr,
                           valori[1] = ), valori[0] = stmt */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_DUM;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'while' token */
                        si (num_pop == V)
                        {
                            valor_novus->datum.iteratio.conditio = valori[II];
                            valor_novus->datum.iteratio.corpus = valori[ZEPHYRUM];
                        }
                        alioquin
                        {
                            valor_novus->datum.iteratio.conditio = NIHIL;
                            valor_novus->datum.iteratio.corpus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_FAC:
                        /* P25: do_statement -> 'do' statement 'while' '(' expression ')' ';' */
                        /* valori[6] = do, valori[5] = stmt, valori[4] = while,
                           valori[3] = (, valori[2] = expr, valori[1] = ),
                           valori[0] = ; */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_FAC;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'do' token */
                        si (num_pop == VII)
                        {
                            valor_novus->datum.iteratio.corpus = valori[V];
                            valor_novus->datum.iteratio.conditio = valori[II];
                        }
                        alioquin
                        {
                            valor_novus->datum.iteratio.conditio = NIHIL;
                            valor_novus->datum.iteratio.corpus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_PER:
                        /* P27: for_statement -> 'for' '(' expr_opt ';' expr_opt ';' expr_opt ')' stmt */
                        /* valori[8] = for, valori[7] = (, valori[6] = init,
                           valori[5] = ;, valori[4] = cond, valori[3] = ;,
                           valori[2] = incr, valori[1] = ), valori[0] = stmt */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_PER;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'for' token */
                        si (num_pop == 9)
                        {
                            valor_novus->datum.circuitus.initium = valori[VI];
                            valor_novus->datum.circuitus.conditio = valori[IV];
                            valor_novus->datum.circuitus.incrementum = valori[II];
                            valor_novus->datum.circuitus.corpus = valori[ZEPHYRUM];
                        }
                        alioquin
                        {
                            valor_novus->datum.circuitus.initium = NIHIL;
                            valor_novus->datum.circuitus.conditio = NIHIL;
                            valor_novus->datum.circuitus.incrementum = NIHIL;
                            valor_novus->datum.circuitus.corpus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_FRANGE:
                        /* P30: statement -> 'break' ';' */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_FRANGE;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'break' token */
                        frange;

                    casus ARBOR2_NODUS_PERGE:
                        /* P31: statement -> 'continue' ';' */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_PERGE;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'continue' token */
                        frange;

                    casus ARBOR2_NODUS_REDDE:
                        /* P32: statement -> 'return' expr_opt ';' */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_REDDE;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'return' token */
                        /* valori[2] = return, valori[1] = expr_opt, valori[0] = ; */
                        valor_novus->datum.reditio.valor = valori[I];  /* expr_opt (may be NULL) */
                        frange;

                    casus ARBOR2_NODUS_SALTA:
                        /* P33: statement -> 'goto' IDENTIFIER ';' */
                        {
                            Arbor2Token* id_tok;
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_SALTA;
                            valor_novus->lexema = lexemata[num_pop - I];  /* 'goto' token */
                            /* valori[2] = goto, valori[1] = ID (as token), valori[0] = ; */
                            /* Get label from identifier token - lexemata[1] is the ID token */
                            id_tok = lexemata[I];
                            valor_novus->datum.saltus.destinatio = id_tok->lexema->valor;
                        }
                        frange;

                    casus ARBOR2_NODUS_TITULATUM:
                        /* P34: statement -> IDENTIFIER ':' statement */
                        /* valori[2] = ID node, valori[1] = ':', valori[0] = statement */
                        {
                            Arbor2Token* id_tok;
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_TITULATUM;
                            valor_novus->lexema = lexemata[II];  /* IDENTIFIER token */
                            id_tok = lexemata[II];
                            valor_novus->datum.titulatum.titulus = id_tok->lexema->valor;
                            valor_novus->datum.titulatum.sententia = valori[ZEPHYRUM];
                        }
                        frange;

                    casus ARBOR2_NODUS_COMMUTATIO:
                        /* P35: statement -> 'switch' '(' expression ')' statement */
                        /* valori[4]=switch, [3]='(', [2]=expr, [1]=')', [0]=stmt */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_COMMUTATIO;
                        valor_novus->lexema = lexemata[IV];
                        valor_novus->datum.selectivum.expressio = valori[II];
                        valor_novus->datum.selectivum.corpus = valori[ZEPHYRUM];
                        frange;

                    casus ARBOR2_NODUS_CASUS:
                        /* P36: statement -> 'case' expression ':' statement */
                        /* valori[3]=case, [2]=expr, [1]=':', [0]=stmt */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_CASUS;
                        valor_novus->lexema = lexemata[III];
                        valor_novus->datum.electio.valor = valori[II];
                        valor_novus->datum.electio.sententia = valori[ZEPHYRUM];
                        frange;

                    casus ARBOR2_NODUS_ORDINARIUS:
                        /* P37: statement -> 'default' ':' statement */
                        /* valori[2]=default, [1]=':', [0]=stmt */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_ORDINARIUS;
                        valor_novus->lexema = lexemata[II];
                        valor_novus->datum.defectus.sententia = valori[ZEPHYRUM];
                        frange;

                    casus ARBOR2_NODUS_PARAMETER_DECL:
                        /* P43: parameter_declaration -> type_specifier declarator */
                        /* valori[1]=type_spec, valori[0]=declarator */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_PARAMETER_DECL;
                        valor_novus->lexema = lexemata[I];  /* type_specifier token */
                        valor_novus->datum.parameter_decl.type_specifier = valori[I];
                        valor_novus->datum.parameter_decl.declarator = valori[ZEPHYRUM];
                        frange;

                    casus ARBOR2_NODUS_DEFINITIO_FUNCTI:
                        /* P44: function_definition -> type_specifier declarator compound_statement */
                        /* valori[2]=type_spec, valori[1]=declarator, valori[0]=corpus */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_DEFINITIO_FUNCTI;
                        valor_novus->lexema = lexemata[II];  /* type_specifier token */
                        valor_novus->datum.definitio_functi.specifier = valori[II];
                        valor_novus->datum.definitio_functi.declarator = valori[I];
                        valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                        frange;

                    casus ARBOR2_NODUS_STRUCT_SPECIFIER:
                        /* P45: struct ID { members } (5 symbols) */
                        /* P46: struct { members } (4 symbols, anonymous) */
                        /* P47: struct ID (2 symbols, forward ref) */
                        /* P52: union ID { members } (5 symbols) */
                        /* P53: union { members } (4 symbols, anonymous) */
                        /* P54: union ID (2 symbols, forward ref) */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_STRUCT_SPECIFIER;

                        si (num_pop == V)
                        {
                            /* P45: struct ID { member_list } or P52: union ID { member_list } */
                            /* valori: [4]=keyword, [3]=ID, [2]={, [1]=member_list, [0]=} */
                            /* lexemata: [4]=keyword, [3]=ID, [2]={, [1]=?, [0]=} */
                            Arbor2Nodus* tag_nodus;
                            Arbor2Token* id_tok = lexemata[III];
#if GLR_DEBUG
                            printf("  [DEBUG P45/P52] valori[0]=%p, valori[1]=%p, valori[2]=%p, valori[3]=%p, valori[4]=%p\n",
                                   (vacuum*)valori[0], (vacuum*)valori[1], (vacuum*)valori[2],
                                   (vacuum*)valori[3], (vacuum*)valori[4]);
                            si (valori[I] != NIHIL) {
                                Xar* test = (Xar*)valori[I];
                                printf("  [DEBUG P45/P52] valori[1] as Xar: numerus=%d\n", xar_numerus(test));
                            }
#endif
                            tag_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            tag_nodus->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            tag_nodus->lexema = id_tok;
                            tag_nodus->datum.folium.valor = id_tok->lexema->valor;

                            valor_novus->lexema = lexemata[IV];  /* struct/union token */
                            valor_novus->datum.struct_specifier.tag = tag_nodus;
                            valor_novus->datum.struct_specifier.membra = (Xar*)valori[I];
                            valor_novus->datum.struct_specifier.est_unio = (actio->valor == 52);
                        }
                        alioquin si (num_pop == IV)
                        {
                            /* P46: struct { member_list } or P53: union { member_list } (anonymous) */
                            /* valori: [3]=keyword, [2]={, [1]=member_list, [0]=} */
                            /* lexemata: [3]=keyword, [2]={, [1]=?, [0]=} */
                            valor_novus->lexema = lexemata[III];  /* struct/union token */
                            valor_novus->datum.struct_specifier.tag = NIHIL;
                            valor_novus->datum.struct_specifier.membra = (Xar*)valori[I];
                            valor_novus->datum.struct_specifier.est_unio = (actio->valor == 53);
                        }
                        alioquin si (num_pop == II)
                        {
                            /* P47: struct ID or P54: union ID (forward reference) */
                            /* valori: [1]=keyword, [0]=ID */
                            /* lexemata: [1]=keyword, [0]=ID */
                            Arbor2Nodus* tag_nodus;
                            Arbor2Token* id_tok = lexemata[ZEPHYRUM];

                            tag_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            tag_nodus->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            tag_nodus->lexema = id_tok;
                            tag_nodus->datum.folium.valor = id_tok->lexema->valor;

                            valor_novus->lexema = lexemata[I];  /* struct/union token */
                            valor_novus->datum.struct_specifier.tag = tag_nodus;
                            valor_novus->datum.struct_specifier.membra = NIHIL;
                            valor_novus->datum.struct_specifier.est_unio = (actio->valor == 54);
                        }
                        frange;

                    casus ARBOR2_NODUS_ENUM_SPECIFIER:
                        /* P55: enum ID { enumerator_list } (5 symbols) - named */
                        /* P56: enum { enumerator_list } (4 symbols) - anonymous */
                        /* P57: enum ID (2 symbols) - forward reference */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_ENUM_SPECIFIER;

                        si (num_pop == V)
                        {
                            /* P55: enum ID { enumerator_list } */
                            /* valori: [4]=enum, [3]=ID, [2]={, [1]=enumerator_list, [0]=} */
                            Arbor2Nodus* tag_nodus;
                            Arbor2Token* id_tok = lexemata[III];

                            tag_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            tag_nodus->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            tag_nodus->lexema = id_tok;
                            tag_nodus->datum.folium.valor = id_tok->lexema->valor;

                            valor_novus->lexema = lexemata[IV];  /* enum token */
                            valor_novus->datum.enum_specifier.tag = tag_nodus;
                            valor_novus->datum.enum_specifier.enumeratores = (Xar*)valori[I];
                        }
                        alioquin si (num_pop == IV)
                        {
                            /* P56: enum { enumerator_list } (anonymous) */
                            /* valori: [3]=enum, [2]={, [1]=enumerator_list, [0]=} */
                            valor_novus->lexema = lexemata[III];  /* enum token */
                            valor_novus->datum.enum_specifier.tag = NIHIL;
                            valor_novus->datum.enum_specifier.enumeratores = (Xar*)valori[I];
                        }
                        alioquin si (num_pop == II)
                        {
                            /* P57: enum ID (forward reference) */
                            /* valori: [1]=enum, [0]=ID */
                            Arbor2Nodus* tag_nodus;
                            Arbor2Token* id_tok = lexemata[ZEPHYRUM];

                            tag_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            tag_nodus->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            tag_nodus->lexema = id_tok;
                            tag_nodus->datum.folium.valor = id_tok->lexema->valor;

                            valor_novus->lexema = lexemata[I];  /* enum token */
                            valor_novus->datum.enum_specifier.tag = tag_nodus;
                            valor_novus->datum.enum_specifier.enumeratores = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_ENUMERATOR:
                        /* P60: enumerator -> IDENTIFIER (1 symbol) - plain */
                        /* P61: enumerator -> IDENTIFIER '=' expression (3 symbols) - with value */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_ENUMERATOR;

                        si (num_pop == I)
                        {
                            /* P60: enumerator -> IDENTIFIER */
                            /* valori: [0]=ID token (no AST node, just token) */
                            Arbor2Token* id_tok = lexemata[ZEPHYRUM];
                            valor_novus->lexema = id_tok;
                            valor_novus->datum.enumerator.titulus = id_tok->lexema->valor;
                            valor_novus->datum.enumerator.valor = NIHIL;
                        }
                        alioquin si (num_pop == III)
                        {
                            /* P61: enumerator -> IDENTIFIER '=' expression */
                            /* valori: [2]=ID, [1]='=', [0]=expression */
                            Arbor2Token* id_tok = lexemata[II];
                            valor_novus->lexema = id_tok;
                            valor_novus->datum.enumerator.titulus = id_tok->lexema->valor;
                            valor_novus->datum.enumerator.valor = valori[ZEPHYRUM];
                        }
                        frange;

                    casus ARBOR2_NODUS_INITIALIZOR_LISTA:
                        /* P200-P202: Initializer list { ... } */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_INITIALIZOR_LISTA;
                        valor_novus->lexema = lexemata[num_pop - I];  /* '{' token */

                        si (actio->valor == 200)
                        {
                            /* P200: init_lista -> '{' '}' (2 symbols) - empty list */
                            valor_novus->datum.initializor_lista.items = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                        }
                        alioquin si (actio->valor == 201)
                        {
                            /* P201: init_lista -> '{' init_items '}' (3 symbols)
                             * valori: [2]='{', [1]=init_items (Xar*), [0]='}' */
                            valor_novus->datum.initializor_lista.items = (Xar*)valori[I];
                        }
                        alioquin si (actio->valor == 202)
                        {
                            /* P202: init_lista -> '{' init_items ',' '}' (4 symbols)
                             * valori: [3]='{', [2]=init_items (Xar*), [1]=',', [0]='}' */
                            valor_novus->datum.initializor_lista.items = (Xar*)valori[II];
                        }
                        frange;

                    casus ARBOR2_NODUS_DESIGNATOR_ITEM:
                        /* P217, P220: Designated initializer .x = 1 or [5] = 100 */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_DESIGNATOR_ITEM;
                        valor_novus->lexema = NIHIL;  /* No single token for this */
                        /* valori: [2]=designator_list (Xar*), [1]='=', [0]=value */
                        valor_novus->datum.designator_item.designatores = (Xar*)valori[II];
                        valor_novus->datum.designator_item.valor = valori[ZEPHYRUM];
                        frange;

                    ordinarius:
                        /* Pass-through rules: take the inner value */
                        /* For 1-symbol rules, valori[0] is the value */
                        /* For 3-symbol rules like (expr), valori[1] is the inner expr */

                        /* Special handling for parameter list rules */
                        si (actio->valor == 41)
                        {
                            /* P41: parameter_list -> parameter_declaration */
                            /* Create Xar with single param */
                            Xar* lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;  /* Return as "Arbor2Nodus*" */
                        }
                        alioquin si (actio->valor == 42)
                        {
                            /* P42: parameter_list -> parameter_list ',' parameter_declaration */
                            /* valori[2]=existing list, valori[1]=',', valori[0]=new param */
                            Xar* lista = (Xar*)valori[II];
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 58)
                        {
                            /* P58: enumerator_list -> enumerator */
                            /* Create Xar with single enumerator */
                            Xar* lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 59)
                        {
                            /* P59: enumerator_list -> enumerator_list ',' enumerator */
                            /* valori[2]=existing list, valori[1]=',', valori[0]=new enumerator */
                            Xar* lista = (Xar*)valori[II];
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 203)
                        {
                            /* P203: init_items -> initializer (1 symbol)
                             * Create Xar with single initializer */
                            Xar* lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 204)
                        {
                            /* P204: init_items -> init_items ',' initializer (3 symbols)
                             * valori: [2]=existing list, [1]=',', [0]=new initializer */
                            Xar* lista = (Xar*)valori[II];
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        /* Phase 1.2c: Designated initializer productions */
                        alioquin si (actio->valor == 213)
                        {
                            /* P213: designator -> '[' expr ']' (3 symbols)
                             * valori: [2]='[', [1]=expr, [0]=']'
                             * Store the index expression directly */
                            valor_novus = valori[I];
                        }
                        alioquin si (actio->valor == 214)
                        {
                            /* P214: designator -> '.' IDENTIFIER (2 symbols)
                             * valori: [1]='.', [0]=IDENTIFIER
                             * Store the identifier token as a node */
                            valor_novus = valori[ZEPHYRUM];
                        }
                        alioquin si (actio->valor == 215)
                        {
                            /* P215: designator_list -> designator (1 symbol)
                             * Create Xar with single designator */
                            Xar* lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 216)
                        {
                            /* P216: designator_list -> designator_list designator (2 symbols)
                             * valori: [1]=existing list, [0]=new designator */
                            Xar* lista = (Xar*)valori[I];
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 218)
                        {
                            /* P218: init_items -> designator_item (1 symbol)
                             * Create Xar with single designator_item */
                            Xar* lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 219)
                        {
                            /* P219: init_items -> init_items ',' designator_item (3 symbols)
                             * valori: [2]=existing list, [1]=',', [0]=new designator_item */
                            Xar* lista = (Xar*)valori[II];
                            Arbor2Nodus** slot = xar_addere(lista);
                            *slot = valori[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        /* Phase 1.3: Init-declarator list productions */
                        alioquin si (actio->valor == 221)
                        {
                            /* P221: init_decl -> declarator (1 symbol)
                             * Create pair [declarator, NIHIL] for uniformity with P222/P223 */
                            Xar* pair = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            Arbor2Nodus** slot;
                            slot = xar_addere(pair);
                            *slot = valori[ZEPHYRUM];  /* declarator */
                            slot = xar_addere(pair);
                            *slot = NIHIL;  /* no initializor */
                            valor_novus = (Arbor2Nodus*)pair;
                        }
                        alioquin si (actio->valor == 222)
                        {
                            /* P222: init_decl -> declarator '=' assignatio (3 symbols)
                             * valori: [2]=declarator, [1]='=', [0]=assignatio
                             * Create a temporary pair structure using Xar */
                            Xar* pair = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            Arbor2Nodus** slot;
                            slot = xar_addere(pair);
                            *slot = valori[II];  /* declarator */
                            slot = xar_addere(pair);
                            *slot = valori[ZEPHYRUM];  /* initializor */
                            valor_novus = (Arbor2Nodus*)pair;
                        }
                        alioquin si (actio->valor == 223)
                        {
                            /* P223: init_decl -> declarator '=' init_lista (3 symbols)
                             * valori: [2]=declarator, [1]='=', [0]=init_lista
                             * Create a temporary pair structure using Xar */
                            Xar* pair = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            Arbor2Nodus** slot;
                            slot = xar_addere(pair);
                            *slot = valori[II];  /* declarator */
                            slot = xar_addere(pair);
                            *slot = valori[ZEPHYRUM];  /* init_lista */
                            valor_novus = (Arbor2Nodus*)pair;
                        }
                        alioquin si (actio->valor == 224)
                        {
                            /* P224: init_decl_list -> init_decl (1 symbol)
                             * Wrap the pair in a list (Xar of pairs) */
                            Xar* lista = xar_creare(glr->piscina, magnitudo(Xar*));
                            Xar** slot = xar_addere(lista);
                            *slot = (Xar*)valori[ZEPHYRUM];  /* The pair from P221/P222/P223 */
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 225)
                        {
                            /* P225: init_decl_list -> init_decl_list ',' init_decl (3 symbols)
                             * valori: [2]=init_decl_list (Xar of pairs), [1]=',', [0]=init_decl (pair) */
                            Xar* lista = (Xar*)valori[II];
                            Xar** slot = xar_addere(lista);
                            *slot = (Xar*)valori[ZEPHYRUM];  /* The new pair */
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (num_pop == III)
                        {
                            valor_novus = valori[I];  /* Middle element */
                        }
                        alioquin si (num_pop > ZEPHYRUM)
                        {
                            valor_novus = valori[ZEPHYRUM];
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;
                }

                si (cursor == NIHIL)
                {
                    goto_status = ZEPHYRUM;
                }
                alioquin
                {
                    goto_status = arbor2_glr_quaerere_goto(glr, cursor->status, regula->sinister);
#if GLR_DEBUG
                    printf("  [DEBUG]    GOTO(%d, %s) = %d\n",
                           cursor->status, arbor2_nt_nomen(regula->sinister), goto_status);
#endif
                }

                si (goto_status < ZEPHYRUM)
                {
#if GLR_DEBUG
                    printf("  [DEBUG]    GOTO failed! path dies\n");
#endif
                    perge;  /* Try next predecessor path */
                }

                nodus_novus = _creare_gss_nodum(glr, goto_status, valor_novus, nodus->lexema);
                si (nodus_novus == NIHIL)
                {
#if GLR_DEBUG
                    printf("  [DEBUG] ERROR: Failed to create GSS node for state %d\n", goto_status);
#endif
                    perge;  /* Try next predecessor path */
                }
                si (cursor != NIHIL)
                {
                    _addere_praedecessorem(glr, nodus_novus, cursor);
                }

                /* Add to pending queue for further processing */
                slot = xar_addere(reducenda);
                *slot = nodus_novus;
                    }  /* End of path loop */
                }  /* End of path enumeration block */
                frange;
            }

            casus ARBOR2_ACTIO_ACCEPT:
                *acceptatum = VERUM;
                *nodus_acceptatus = nodus;
                frange;

            casus ARBOR2_ACTIO_ERROR:
                frange;
        }
    }

    redde habuit_shift;
}

interior Arbor2GSSNodus*
_processare_actiones(Arbor2GLR* glr, b32* acceptatum_out)
{
    Arbor2Token* lexema;
    Arbor2LexemaGenus genus;
    b32 acceptatum;
    Xar* reducenda;     /* Queue of nodes pending reduction */
    Xar* acceptati;     /* Collect all accepting nodes */
    i32 i;
    Arbor2GSSNodus* nodus_acceptatus;

    lexema = _currens_lexema(glr);
    si (lexema == NIHIL)
    {
        genus = ARBOR2_LEXEMA_EOF;
    }
    alioquin
    {
        genus = lexema->lexema->genus;
    }

    acceptatum = FALSUM;
    nodus_acceptatus = NIHIL;

    /* Create pending reduction queue */
    reducenda = xar_creare(glr->piscina, magnitudo(Arbor2GSSNodus*));

    /* Create list for accepting nodes */
    acceptati = xar_creare(glr->piscina, magnitudo(Arbor2GSSNodus*));

    /* First pass: process original frontier, collect reductions */
    per (i = ZEPHYRUM; i < xar_numerus(glr->frons_activa); i++)
    {
        Arbor2GSSNodus* nodus;
        nodus = *(Arbor2GSSNodus**)xar_obtinere(glr->frons_activa, i);
        si (nodus == NIHIL)
        {
#if GLR_DEBUG
            printf("  [DEBUG] WARNING: NULL node in frons_activa at index %d\n", i);
#endif
            perge;
        }
        _processare_unam_actionem(glr, nodus, genus, reducenda, &acceptatum, &nodus_acceptatus);
        si (acceptatum && nodus_acceptatus != NIHIL)
        {
            Arbor2GSSNodus** slot = xar_addere(acceptati);
            *slot = nodus_acceptatus;
            acceptatum = FALSUM;  /* Reset to continue processing */
            nodus_acceptatus = NIHIL;
        }
    }

    /* Process reductions until queue is empty - don't stop on accept! */
    i = ZEPHYRUM;
    dum (i < xar_numerus(reducenda))
    {
        Arbor2GSSNodus* nodus;
        nodus = *(Arbor2GSSNodus**)xar_obtinere(reducenda, i);
        si (nodus == NIHIL)
        {
#if GLR_DEBUG
            printf("  [DEBUG] WARNING: NULL node in reducenda at index %d\n", i);
#endif
            i++;
            perge;
        }
        _processare_unam_actionem(glr, nodus, genus, reducenda, &acceptatum, &nodus_acceptatus);
        si (acceptatum && nodus_acceptatus != NIHIL)
        {
            Arbor2GSSNodus** slot = xar_addere(acceptati);
            *slot = nodus_acceptatus;
            acceptatum = FALSUM;
            nodus_acceptatus = NIHIL;
        }
        i++;
    }

    /* Handle multiple accepting paths */
    si (xar_numerus(acceptati) > I)
    {
        /* Multiple accepts - create ambiguous node */
        Arbor2GSSNodus* primus = *(Arbor2GSSNodus**)xar_obtinere(acceptati, ZEPHYRUM);
        i32 j;

        per (j = I; j < xar_numerus(acceptati); j++)
        {
            Arbor2GSSNodus* alius = *(Arbor2GSSNodus**)xar_obtinere(acceptati, j);
            si (primus->valor != NIHIL && alius->valor != NIHIL)
            {
                primus->valor = _creare_nodum_ambiguum(glr, primus->valor, alius->valor, primus->lexema);
            }
        }
        nodus_acceptatus = primus;
        acceptatum = VERUM;
    }
    alioquin si (xar_numerus(acceptati) == I)
    {
        nodus_acceptatus = *(Arbor2GSSNodus**)xar_obtinere(acceptati, ZEPHYRUM);
        acceptatum = VERUM;
    }

    *acceptatum_out = acceptatum;

    /* Merge compatible nodes in the new frontier */
    si (xar_numerus(glr->frons_nova) > I)
    {
        _mergere_compatibiles(glr, glr->frons_nova);
    }

    /* Track max frontier size */
    si (xar_numerus(glr->frons_nova) > (i32)glr->max_frons)
    {
        glr->max_frons = (i32)xar_numerus(glr->frons_nova);
    }

    redde nodus_acceptatus;
}

Arbor2GLRResultus
arbor2_glr_parsere_expressio(
    Arbor2GLR*      glr,
    Xar*            lexemata)
{
    Arbor2GLRResultus resultus;
    Arbor2GSSNodus* nodus_initialis;
    Arbor2GSSNodus* nodus_acceptatus;
    Arbor2GSSNodus** slot;
    b32 acceptatum;
    Xar* temp;

    /* Initialize */
    glr->lexemata = lexemata;
    glr->positus = ZEPHYRUM;
    nodus_acceptatus = NIHIL;

    /* Clear frontiers */
    xar_vacare(glr->frons_activa);
    xar_vacare(glr->frons_nova);

    /* Create initial GSS node at state 0 */
    nodus_initialis = _creare_gss_nodum(glr, ZEPHYRUM, NIHIL, NIHIL);
    slot = xar_addere(glr->frons_activa);
    *slot = nodus_initialis;

    acceptatum = FALSUM;

    /* Main parse loop */
    dum (xar_numerus(glr->frons_activa) > ZEPHYRUM && !acceptatum)
    {
        Arbor2GSSNodus* nodus_iter;

        /* Process all reductions, then shifts */
        nodus_iter = _processare_actiones(glr, &acceptatum);
        si (nodus_iter != NIHIL)
        {
            nodus_acceptatus = nodus_iter;
        }

        si (acceptatum)
        {
            frange;
        }

        /* Move to next token for shifts */
        si (xar_numerus(glr->frons_nova) > ZEPHYRUM)
        {
            _progredi(glr);

            /* Swap frontiers */
            temp = glr->frons_activa;
            glr->frons_activa = glr->frons_nova;
            glr->frons_nova = temp;
            xar_vacare(glr->frons_nova);
        }
        alioquin
        {
            /* No shifts possible - report error with expected tokens */
            si (xar_numerus(glr->frons_activa) > ZEPHYRUM)
            {
                Arbor2GSSNodus** nodus_ptr;
                Arbor2GSSNodus* nodus_err;
                Arbor2Token* lexema_err;
                chorda* exspectata;
                character* error_msg;
                i32 msg_len;
                chorda** slot;
                constans character* token_nomen;

                nodus_ptr = xar_obtinere(glr->frons_activa, ZEPHYRUM);
                nodus_err = *nodus_ptr;

                lexema_err = _currens_lexema(glr);
                token_nomen = lexema_err != NIHIL ?
                    arbor2_lexema_genus_nomen(lexema_err->lexema->genus) : "EOF";

                exspectata = arbor2_glr_exspectata_pro_statu(
                    glr->piscina, nodus_err->status);

                /* Build error message */
                si (exspectata != NIHIL && exspectata->mensura > ZEPHYRUM)
                {
                    /* "Unexpected TOKEN, expected: ..." */
                    msg_len = 32 + (i32)strlen(token_nomen) + exspectata->mensura;
                    error_msg = piscina_allocare(glr->piscina, (memoriae_index)msg_len);
                    si (error_msg != NIHIL)
                    {
                        chorda* err_chorda;
                        i32 written;

                        written = (i32)snprintf(error_msg, (size_t)msg_len,
                            "Inexpectatum %s, exspectata: %.*s",
                            token_nomen,
                            exspectata->mensura, (constans character*)exspectata->datum);

                        err_chorda = piscina_allocare(glr->piscina,
                            (memoriae_index)magnitudo(chorda));
                        si (err_chorda != NIHIL)
                        {
                            err_chorda->datum = (i8*)error_msg;
                            err_chorda->mensura = written;

                            slot = xar_addere(glr->errores);
                            *slot = err_chorda;
                        }
                    }
                }
            }

            xar_vacare(glr->frons_activa);
            frange;
        }
    }

    /* Build result */
    resultus.successus = acceptatum;
    resultus.errores = glr->errores;
    resultus.ambigui = glr->ambigui;

    /* Use the accepting node's value */
    si (acceptatum && nodus_acceptatus != NIHIL)
    {
        resultus.radix = nodus_acceptatus->valor;
    }
    alioquin
    {
        resultus.radix = NIHIL;
    }

    redde resultus;
}

Arbor2GLRResultus
arbor2_glr_parsere(
    Arbor2GLR*      glr,
    Xar*            lexemata)
{
    /* For now, just parse as expression */
    redde arbor2_glr_parsere_expressio(glr, lexemata);
}
