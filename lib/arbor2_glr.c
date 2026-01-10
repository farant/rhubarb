/* arbor2_glr.c - GLR Parser Core */
#include "latina.h"
#include "arbor2_glr.h"
#include "piscina.h"
#include "xar.h"
#include <string.h>

/* Debug flag - set to 1 to enable tracing */
#define GLR_DEBUG 0
#if GLR_DEBUG
#include <stdio.h>
#endif

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
                Arbor2GSSNodus* cursor;
                Arbor2Nodus* valor_novus;
                s32 goto_status;
                Arbor2GSSNodus* nodus_novus;
                Arbor2GSSNodus** slot;
                s32 k;
                Arbor2Nodus* valori[16];
                Arbor2Token* lexemata[16];
                s32 num_pop;

                regula = arbor2_glr_obtinere_regula(glr, actio->valor);
                si (regula == NIHIL)
                {
                    frange;
                }

#if GLR_DEBUG
                printf("  [DEBUG] -> REDUCE P%d (pop %d, NT=%s)\n",
                       actio->valor, regula->longitudo, arbor2_nt_nomen(regula->sinister));
#endif

                cursor = nodus;
                num_pop = ZEPHYRUM;

                per (k = ZEPHYRUM; k < (s32)regula->longitudo && cursor != NIHIL; k++)
                {
                    valori[num_pop] = cursor->valor;
                    lexemata[num_pop] = cursor->lexema;
                    num_pop++;

                    si (cursor->num_praed > ZEPHYRUM)
                    {
                        cursor = cursor->praedecessores[ZEPHYRUM];
                    }
                    alioquin
                    {
                        cursor = NIHIL;
                    }
                }

                si (cursor == NIHIL && k < (s32)regula->longitudo)
                {
                    frange;  /* Not enough on stack */
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

                    casus ARBOR2_NODUS_IDENTIFICATOR:
                    casus ARBOR2_NODUS_INTEGER:
                        valor_novus = (num_pop > ZEPHYRUM) ? valori[ZEPHYRUM] : NIHIL;
                        frange;

                    casus ARBOR2_NODUS_DECLARATIO:
                        /* Check for struct member rules P48-P51 first */
                        si (actio->valor == 48)
                        {
                            /* P48: struct_member_list -> type_spec ID ';' (3 symbols) */
                            /* lexemata: [2]=type, [1]=name, [0]=; */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[II];
                            Arbor2Token* name_tok = lexemata[I];
#if GLR_DEBUG
                            printf("  [DEBUG P48] Creating member list, type=%.*s, name=%.*s\n",
                                   (integer)type_tok->lexema->valor.mensura, type_tok->lexema->valor.datum,
                                   (integer)name_tok->lexema->valor.mensura, name_tok->lexema->valor.datum);
#endif
                            decl_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            decl_node->genus = ARBOR2_NODUS_DECLARATOR;
                            decl_node->lexema = name_tok;
                            decl_node->datum.declarator.num_stellae = ZEPHYRUM;
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;

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
                            /* P49: member_list type_spec ID ';' (4 symbols) */
                            /* valori: [3]=list, [2]=type, [1]=name, [0]=; */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista = (Xar*)valori[III];
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[II];
                            Arbor2Token* name_tok = lexemata[I];

                            decl_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            decl_node->genus = ARBOR2_NODUS_DECLARATOR;
                            decl_node->lexema = name_tok;
                            decl_node->datum.declarator.num_stellae = ZEPHYRUM;
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;

                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 50)
                        {
                            /* P50: type_spec '*' ID ';' (4 symbols, pointer first member) */
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

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;

                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (actio->valor == 51)
                        {
                            /* P51: member_list type_spec '*' ID ';' (5 symbols, append pointer) */
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

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;

                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        alioquin si (num_pop >= II)
                        {
                            /* P10: declaration -> type_specifier declarator */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = lexemata[I];  /* type_specifier token */
                            valor_novus->datum.declaratio.specifier = valori[I];
                            valor_novus->datum.declaratio.declarator = valori[ZEPHYRUM];
                        }
                        alioquin
                        {
                            valor_novus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_DECLARATOR:
                        si (num_pop == II)
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
                                }
                                alioquin
                                {
                                    valor_novus->datum.declarator.num_stellae = I;
                                    valor_novus->datum.declarator.titulus.datum = NIHIL;
                                    valor_novus->datum.declarator.titulus.mensura = ZEPHYRUM;
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
                    frange;
                }

                nodus_novus = _creare_gss_nodum(glr, goto_status, valor_novus, nodus->lexema);
                si (cursor != NIHIL)
                {
                    _addere_praedecessorem(glr, nodus_novus, cursor);
                }

                /* Add to pending queue for further processing */
                slot = xar_addere(reducenda);
                *slot = nodus_novus;
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
            /* No shifts possible - clear active frontier to avoid infinite loop */
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
