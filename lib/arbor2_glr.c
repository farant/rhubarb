/* arbor2_glr.c - GLR Parser Core */
#include "latina.h"
#include "arbor2_glr.h"
#include "arbor2_conditio_evaluare.h"
#include "piscina.h"
#include "xar.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

/* Debug flag - set to 1 to enable tracing */
#define GLR_DEBUG 1

/* Trace flag for slowdown diagnosis - prints per-token stats */
#define GLR_TRACE_SLOWDOWN 0

#if GLR_TRACE_SLOWDOWN
/* Global counters for profiling */
hic_manens i64 g_gss_nodes = 0;
hic_manens i64 g_shifts = 0;
hic_manens i64 g_reduces = 0;
#endif

/* ==================================================
 * Location Propagation Helper
 *
 * Sets node location from token range. Uses arbor2_token_radix()
 * to trace back to original source (invocation site).
 * lexemata[primus_idx] = first token (start of span)
 * lexemata[ultimus_idx] = last token (end of span)
 * ================================================== */

#define LOCUS_EX_LEXEMATIS(nodus, primus_idx, ultimus_idx) \
    fac { \
        Arbor2Token* _primus = arbor2_token_radix(lexemata[primus_idx]); \
        Arbor2Token* _ultimus = arbor2_token_radix(lexemata[ultimus_idx]); \
        (nodus)->linea_initium = _primus->origo_meta->linea; \
        (nodus)->columna_initium = _primus->origo_meta->columna; \
        (nodus)->linea_finis = _ultimus->origo_meta->linea; \
        (nodus)->columna_finis = _ultimus->origo_meta->columna + (i32)_ultimus->lexema->longitudo; \
        (nodus)->layer_index = lexemata[primus_idx]->origo_meta->layer_index; \
    } dum (FALSUM)

/* Single-token location helper (start and end are same token) */
#define LOCUS_EX_LEXEMA(nodus, idx) LOCUS_EX_LEXEMATIS(nodus, idx, idx)

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

/* Error recovery forward declarations */
interior Arbor2Nodus* _creare_nodum_error(
    Arbor2GLR* glr, chorda* nuntius, Xar* lexemata_saltata, Arbor2Token* lexema_initium);

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
 * Comment Node Helpers
 *
 * Functions for creating comment nodes from trivia
 * and detecting comment attachment.
 * ================================================== */

/* Extract comment text without delimiters */
interior chorda
_extrahere_textum_commenti(chorda textus_crudus, b32 est_c99)
{
    chorda textus;

    si (textus_crudus.datum == NIHIL || textus_crudus.mensura < II)
    {
        textus.datum = NIHIL;
        textus.mensura = ZEPHYRUM;
        redde textus;
    }

    si (est_c99)
    {
        /* C99 line comment: remove leading // */
        textus.datum = textus_crudus.datum + II;
        textus.mensura = textus_crudus.mensura - II;
    }
    alioquin
    {
        /* Block comment: remove leading and trailing delimiters */
        si (textus_crudus.mensura >= IV)
        {
            textus.datum = textus_crudus.datum + II;
            textus.mensura = textus_crudus.mensura - IV;
        }
        alioquin
        {
            textus.datum = NIHIL;
            textus.mensura = ZEPHYRUM;
        }
    }

    redde textus;
}

/* Create a COMMENTUM node from spatium (Arbor2Lexema* comment token).
 * Phase 2.6: Uses typed lexema tokens instead of Arbor2Trivia. */
interior Arbor2Nodus*
_creare_nodum_commentum(
    Piscina*             piscina,
    Arbor2Lexema*        spatium,
    b32                  est_fluitans,
    Xar*                 spatia_ante,
    Xar*                 spatia_post)
{
    Arbor2Nodus* nodus;
    b32 est_linea;

    nodus = piscina_allocare(piscina, magnitudo(Arbor2Nodus));
    nodus->genus = ARBOR2_NODUS_COMMENTUM;
    nodus->lexema = NIHIL;
    nodus->pater = NIHIL;
    nodus->commenta_ante = NIHIL;
    nodus->commenta_post = NIHIL;

    /* Location from spatium */
    nodus->linea_initium = spatium->linea;
    nodus->columna_initium = spatium->columna;
    /* Calculate end position from text length */
    nodus->linea_finis = spatium->linea;  /* May span lines but start with single */
    nodus->columna_finis = spatium->columna + spatium->valor.mensura;
    nodus->layer_index = ZEPHYRUM;

    /* Determine subgenus from genus */
    est_linea = (spatium->genus == ARBOR2_LEXEMA_COMMENTUM_LINEA);
    si (est_linea)
    {
        nodus->datum.commentum.subgenus = ARBOR2_COMMENTUM_LINEA;
    }
    alioquin
    {
        nodus->datum.commentum.subgenus = ARBOR2_COMMENTUM_CLAUSUM;
    }

    nodus->datum.commentum.textus_crudus = spatium->valor;
    nodus->datum.commentum.textus = _extrahere_textum_commenti(spatium->valor, est_linea);
    nodus->datum.commentum.est_fluitans = est_fluitans;
    nodus->datum.commentum.fragmenta = NIHIL;
    nodus->datum.commentum.spatia_ante = spatia_ante;
    nodus->datum.commentum.spatia_post = spatia_post;

    redde nodus;
}

/* Helper: Check if lexema is a comment token */
interior b32
_est_commentum_lexema(Arbor2Lexema* lex)
{
    redde lex->genus == ARBOR2_LEXEMA_COMMENTUM_CLAUSUM ||
           lex->genus == ARBOR2_LEXEMA_COMMENTUM_LINEA;
}

/* Check if spatia sequence has a blank line (2+ newlines) before given index.
 * Phase 2.6: Uses Arbor2Lexema* instead of Arbor2Trivia*.
 * Phase 2.7: Newlines come from CONTINUATIO or NOVA_LINEA tokens.
 * NOTE: Loop variable must be signed (s32) to avoid unsigned underflow at 0. */
interior b32
_habet_lineam_vacuam_ante(Xar* spatia, i32 index)
{
    s32 i;  /* Must be signed for i-- loop to terminate at 0 */
    i32 newline_count;

    si (spatia == NIHIL || index <= ZEPHYRUM)
    {
        redde FALSUM;
    }

    newline_count = ZEPHYRUM;

    /* Walk backwards from index looking for consecutive newlines */
    per (i = (s32)index - I; i >= ZEPHYRUM; i--)
    {
        Arbor2Lexema** s_ptr = xar_obtinere(spatia, (i32)i);
        si (s_ptr != NIHIL && *s_ptr != NIHIL)
        {
            Arbor2Lexema* s = *s_ptr;

            si (_est_commentum_lexema(s))
            {
                /* Hit another comment, reset count */
                newline_count = ZEPHYRUM;
            }
            alioquin si (s->genus == ARBOR2_LEXEMA_CONTINUATIO ||
                         s->genus == ARBOR2_LEXEMA_NOVA_LINEA)
            {
                /* CONTINUATIO and NOVA_LINEA each have exactly one newline */
                newline_count++;
                si (newline_count >= II)
                {
                    redde VERUM;
                }
            }
            /* SPATIA and TABULAE have no newlines */
        }
    }

    redde FALSUM;
}

/* Check if spatia sequence has a blank line (2+ newlines) after given index.
 * Phase 2.6: Uses Arbor2Lexema* instead of Arbor2Trivia*.
 * Phase 2.7: Newlines come from CONTINUATIO or NOVA_LINEA tokens. */
interior b32
_habet_lineam_vacuam_post(Xar* spatia, i32 index)
{
    i32 i;
    i32 num;
    i32 newline_count;

    si (spatia == NIHIL)
    {
        redde FALSUM;
    }

    num = xar_numerus(spatia);
    si (index >= num - I)
    {
        redde FALSUM;
    }

    newline_count = ZEPHYRUM;

    /* Walk forward from index looking for consecutive newlines */
    per (i = index + I; i < num; i++)
    {
        Arbor2Lexema** s_ptr = xar_obtinere(spatia, i);
        si (s_ptr != NIHIL && *s_ptr != NIHIL)
        {
            Arbor2Lexema* s = *s_ptr;

            si (_est_commentum_lexema(s))
            {
                /* Hit another comment, reset count */
                newline_count = ZEPHYRUM;
            }
            alioquin si (s->genus == ARBOR2_LEXEMA_CONTINUATIO ||
                         s->genus == ARBOR2_LEXEMA_NOVA_LINEA)
            {
                /* CONTINUATIO and NOVA_LINEA each have exactly one newline */
                newline_count++;
                si (newline_count >= II)
                {
                    redde VERUM;
                }
            }
            /* SPATIA and TABULAE have no newlines */
        }
    }

    redde FALSUM;
}

/* Check if a comment in spatia_post is at end of line (no newline before it).
 * Phase 2.6: Uses Arbor2Lexema* instead of Arbor2Trivia*.
 * Phase 2.7: Newlines come from CONTINUATIO or NOVA_LINEA tokens. */
interior b32
_est_commentum_finis_lineae(Xar* spatia_post, i32 comment_index)
{
    i32 i;

    si (spatia_post == NIHIL || comment_index < ZEPHYRUM)
    {
        redde FALSUM;
    }

    /* Check spatia before this comment - if there's any newline, it's not EOL */
    per (i = ZEPHYRUM; i < comment_index; i++)
    {
        Arbor2Lexema** s_ptr = xar_obtinere(spatia_post, i);
        si (s_ptr != NIHIL && *s_ptr != NIHIL)
        {
            Arbor2Lexema* s = *s_ptr;
            /* CONTINUATIO and NOVA_LINEA tokens contain newlines */
            si (s->genus == ARBOR2_LEXEMA_CONTINUATIO ||
                s->genus == ARBOR2_LEXEMA_NOVA_LINEA)
            {
                redde FALSUM;
            }
            /* SPATIA and TABULAE have no newlines, so skip them */
        }
    }

    redde VERUM;
}

/* Extract whitespace spatia around a comment.
 * NOTE: Reserved for future reformatting scenarios where we need to
 * move/modify comments and track their surrounding whitespace.
 * For roundtrip, whitespace stays in token spatia.
 * Phase 2.6: Uses Arbor2Lexema* instead of Arbor2Trivia*.
 */
#if 0
interior vacuum
_extrahere_spatia_circa_commentum(
    Piscina*    piscina,
    Xar*        spatia,
    i32         comment_index,
    i32         start_index,    /* Start of range to consider (inclusive) */
    Xar**       out_spatia_ante,
    Xar**       out_spatia_post)
{
    i32 i;
    i32 num;
    Xar* ante;
    Xar* post;
    Arbor2Lexema** s_ptr;
    Arbor2Lexema** slot;

    ante = NIHIL;
    post = NIHIL;

    *out_spatia_ante = NIHIL;
    *out_spatia_post = NIHIL;

    si (spatia == NIHIL)
    {
        redde;
    }

    num = xar_numerus(spatia);

    /* Collect whitespace before comment (from start_index to comment_index-1) */
    per (i = start_index; i < comment_index; i++)
    {
        s_ptr = xar_obtinere(spatia, i);
        si (s_ptr != NIHIL && *s_ptr != NIHIL && !_est_commentum_lexema(*s_ptr))
        {
            si (ante == NIHIL)
            {
                ante = xar_creare(piscina, magnitudo(Arbor2Lexema*));
            }
            slot = xar_addere(ante);
            *slot = *s_ptr;
        }
    }

    /* Collect whitespace after comment (until next comment or end) */
    per (i = comment_index + I; i < num; i++)
    {
        s_ptr = xar_obtinere(spatia, i);
        si (s_ptr != NIHIL && *s_ptr != NIHIL)
        {
            si (_est_commentum_lexema(*s_ptr))
            {
                /* Hit another comment, stop */
                frange;
            }
            si (post == NIHIL)
            {
                post = xar_creare(piscina, magnitudo(Arbor2Lexema*));
            }
            slot = xar_addere(post);
            *slot = *s_ptr;
        }
    }

    *out_spatia_ante = ante;
    *out_spatia_post = post;
}
#endif

/* Attach a comment node to commenta_ante of target node */
interior vacuum
_adhaerere_commentum_ante(Piscina* piscina, Arbor2Nodus* target, Arbor2Nodus* commentum)
{
    Arbor2Nodus** slot;

    si (target == NIHIL || commentum == NIHIL)
    {
        redde;
    }

    si (target->commenta_ante == NIHIL)
    {
        target->commenta_ante = xar_creare(piscina, magnitudo(Arbor2Nodus*));
    }

    slot = xar_addere(target->commenta_ante);
    *slot = commentum;
    commentum->pater = target;
}

/* Attach a comment node to commenta_post of target node */
interior vacuum
_adhaerere_commentum_post(Piscina* piscina, Arbor2Nodus* target, Arbor2Nodus* commentum)
{
    Arbor2Nodus** slot;

    si (target == NIHIL || commentum == NIHIL)
    {
        redde;
    }

    si (target->commenta_post == NIHIL)
    {
        target->commenta_post = xar_creare(piscina, magnitudo(Arbor2Nodus*));
    }

    slot = xar_addere(target->commenta_post);
    *slot = commentum;
    commentum->pater = target;
}

/* Process spatia on a token and promote comments to nodes.
 * Attaches leading comments (non-floating) to nodus_post as commenta_ante.
 * Attaches trailing comments to nodus_ante as commenta_post.
 * Adds floating comments to lista_parens as sibling nodes.
 *
 * Phase 2.6: Uses Arbor2Lexema* spatia instead of Arbor2Trivia.
 * Comments are promoted as first-class AST nodes with their own spatia.
 * Returns number of comment nodes created.
 */
interior i32
_promovere_commenta_ex_spatia(
    Piscina*        piscina,
    Arbor2Token*    token,
    Arbor2Nodus*    nodus_ante,     /* Node before spatia (for commenta_post) */
    Arbor2Nodus*    nodus_post,     /* Node after spatia (for commenta_ante) */
    Xar*            lista_parens)   /* Parent list for floating comments */
{
    i32 count;
    i32 i;
    i32 num;
    Xar* spatia;
    Arbor2Lexema** s_ptr;
    Arbor2Lexema* s;
    b32 blank_ante;
    b32 blank_post;
    b32 est_fluitans;
    b32 est_eol;
    Arbor2Nodus* commentum;
    Arbor2Nodus** slot;

    count = ZEPHYRUM;

    si (token == NIHIL || token->lexema == NIHIL)
    {
        redde count;
    }

    /* Process spatia_ante (leading spatia) */
    spatia = token->lexema->spatia_ante;
    si (spatia != NIHIL)
    {
        num = xar_numerus(spatia);
        per (i = ZEPHYRUM; i < num; i++)
        {
            s_ptr = xar_obtinere(spatia, i);
            si (s_ptr != NIHIL && *s_ptr != NIHIL)
            {
                s = *s_ptr;
                si (_est_commentum_lexema(s))
                {
                    /* Check if floating (blank lines both sides) */
                    blank_ante = _habet_lineam_vacuam_ante(spatia, i);
                    blank_post = _habet_lineam_vacuam_post(spatia, i);
                    est_fluitans = blank_ante && blank_post;

                    /* NOTE: For roundtrip, whitespace remains in token spatia.
                     * Comment spatia fields reserved for reformatting scenarios
                     * where we need to move/modify comments. */
                    commentum = _creare_nodum_commentum(piscina, s, est_fluitans, NIHIL, NIHIL);

                    si (est_fluitans && lista_parens != NIHIL)
                    {
                        /* Floating comment - add to parent list */
                        slot = xar_addere(lista_parens);
                        *slot = commentum;
                    }
                    alioquin si (nodus_post != NIHIL)
                    {
                        /* Leading comment - attach to next node */
                        _adhaerere_commentum_ante(piscina, nodus_post, commentum);
                    }

                    count++;
                }
            }
        }
    }

    /* Process spatia_post (trailing spatia) */
    spatia = token->lexema->spatia_post;
    si (spatia != NIHIL)
    {
        num = xar_numerus(spatia);
        per (i = ZEPHYRUM; i < num; i++)
        {
            s_ptr = xar_obtinere(spatia, i);
            si (s_ptr != NIHIL && *s_ptr != NIHIL)
            {
                s = *s_ptr;
                si (_est_commentum_lexema(s))
                {
                    /* Check if end-of-line comment */
                    est_eol = _est_commentum_finis_lineae(spatia, i);

                    si (est_eol && nodus_ante != NIHIL)
                    {
                        /* EOL comment - attach to previous node */
                        commentum = _creare_nodum_commentum(piscina, s, FALSUM, NIHIL, NIHIL);
                        _adhaerere_commentum_post(piscina, nodus_ante, commentum);
                        count++;
                    }
                }
            }
        }
    }

    redde count;
}

/* Promovere commenta ab tokeno interiore ad nodos filiorum.
 * Pro operatoribus inter sinister et dexter:
 *   - spatia_ante tok → nodus_post.commenta_ante (praecedentia)
 *   - spatia_post tok → nodus_post.commenta_ante (interiora)
 * Pro parenibus et braketis:
 *   - spatia_post tok_apertum → nodus_contentum.commenta_ante
 *   - spatia_ante tok_clausum → nodus_contentum.commenta_post
 *
 * Phase 2.6: Uses Arbor2Lexema* spatia instead of Arbor2Trivia.
 * Comments are promoted as first-class AST nodes.
 * Whitespace remains in token spatia for roundtrip; comment spatia fields
 * are reserved for future reformatting scenarios.
 * Nulla commenta fluitantia in expressionibus - omnia adhaerent.
 */
interior vacuum
_promovere_commenta_interior(
    Piscina*        piscina,
    Arbor2Token*    tok_interior,   /* Token interior (operator, paren, etc.) */
    Arbor2Nodus*    nodus_ante,     /* Filius ante tokenum */
    Arbor2Nodus*    nodus_post)     /* Filius post tokenum */
{
    i32 i;
    i32 num;
    Xar* spatia;
    Arbor2Lexema** s_ptr;
    Arbor2Lexema* s;
    Arbor2Nodus* commentum;
    b32 est_eol;

    si (tok_interior == NIHIL || tok_interior->lexema == NIHIL)
    {
        redde;
    }

    /* Processare spatia_ante (commenta praecedentia tokenum) */
    spatia = tok_interior->lexema->spatia_ante;
    si (spatia != NIHIL && nodus_post != NIHIL)
    {
        num = xar_numerus(spatia);
        per (i = ZEPHYRUM; i < num; i++)
        {
            s_ptr = xar_obtinere(spatia, i);
            si (s_ptr != NIHIL && *s_ptr != NIHIL)
            {
                s = *s_ptr;
                si (_est_commentum_lexema(s))
                {
                    /* In expressionibus, commenta praecedentia adhaerent ad nodum post */
                    commentum = _creare_nodum_commentum(piscina, s, FALSUM, NIHIL, NIHIL);
                    _adhaerere_commentum_ante(piscina, nodus_post, commentum);
                }
            }
        }
    }

    /* Processare spatia_post (commenta post tokenum) */
    spatia = tok_interior->lexema->spatia_post;
    si (spatia != NIHIL)
    {
        num = xar_numerus(spatia);
        per (i = ZEPHYRUM; i < num; i++)
        {
            s_ptr = xar_obtinere(spatia, i);
            si (s_ptr != NIHIL && *s_ptr != NIHIL)
            {
                s = *s_ptr;
                si (_est_commentum_lexema(s))
                {
                    /* Commenta post operatorem: ad nodum post si existit,
                     * alioquin ad nodum ante ut commenta_post */
                    si (nodus_post != NIHIL)
                    {
                        commentum = _creare_nodum_commentum(piscina, s, FALSUM, NIHIL, NIHIL);
                        _adhaerere_commentum_ante(piscina, nodus_post, commentum);
                    }
                    alioquin si (nodus_ante != NIHIL)
                    {
                        /* Pro tokenis clausis sine nodo sequenti */
                        est_eol = _est_commentum_finis_lineae(spatia, i);
                        si (est_eol)
                        {
                            commentum = _creare_nodum_commentum(piscina, s, FALSUM, NIHIL, NIHIL);
                            _adhaerere_commentum_post(piscina, nodus_ante, commentum);
                        }
                    }
                }
            }
        }
    }
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
    glr->num_errores = ZEPHYRUM;

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

#if GLR_TRACE_SLOWDOWN
    g_gss_nodes++;
#endif

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
    Arbor2Token* radix;

    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->genus = genus;
    nodus->lexema = lexema;

    /* Locus ex unico lexema */
    radix = arbor2_token_radix(lexema);
    nodus->linea_initium = radix->origo_meta->linea;
    nodus->columna_initium = radix->origo_meta->columna;
    nodus->linea_finis = radix->origo_meta->linea;
    nodus->columna_finis = radix->origo_meta->columna + (i32)radix->lexema->longitudo;
    nodus->layer_index = lexema->origo_meta->layer_index;
    nodus->pater = NIHIL;  /* Statuetur cum addetur ad parentem */

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
    nodus->pater = NIHIL;
    nodus->datum.binarium.sinister = sinister;
    nodus->datum.binarium.tok_operator = NIHIL;
    nodus->datum.binarium.operator = operator;
    nodus->datum.binarium.dexter = dexter;

    /* Statuere patrem pro filiis */
    si (sinister != NIHIL) sinister->pater = nodus;
    si (dexter != NIHIL) dexter->pater = nodus;

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
    nodus->pater = NIHIL;
    nodus->datum.unarium.tok_operator = NIHIL;
    nodus->datum.unarium.operator = operator;
    nodus->datum.unarium.operandum = operandum;

    /* Statuere patrem pro filio */
    si (operandum != NIHIL) operandum->pater = nodus;

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
        nodus->datum.binarium.tok_operator = operator;
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
        nodus->datum.unarium.tok_operator = operator;
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

/* Check if a macro might expand to a type.
 * Uses the public lookahead API and follows one level of macro recursion. */
interior b32
_macro_suggerit_typum(Arbor2GLR* glr, Arbor2Token* tok)
{
    Arbor2ExpansionLookahead lookahead;

    si (tok == NIHIL || glr->expansion == NIHIL)
    {
        redde FALSUM;
    }

    si (tok->lexema->genus != ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        redde FALSUM;
    }

    lookahead = arbor2_expansion_lookahead(glr->expansion, tok->lexema->valor);

    /* Unknown macro or empty body */
    si (lookahead.genus == ARBOR2_LEXEMA_ERROR || lookahead.est_vacuum)
    {
        redde FALSUM;
    }

    /* Recursive macro - follow one level (e.g., integer -> int) */
    si (lookahead.est_recursivum)
    {
        Arbor2MacroDef* def;
        def = arbor2_expansion_quaerere_macro(glr->expansion, tok->lexema->valor);
        si (def != NIHIL && xar_numerus(def->corpus) > ZEPHYRUM)
        {
            Arbor2Lexema* primus;
            primus = *(Arbor2Lexema**)xar_obtinere(def->corpus, ZEPHYRUM);
            lookahead = arbor2_expansion_lookahead(glr->expansion, primus->valor);
        }
    }

    /* Check if genus is a type keyword */
    commutatio (lookahead.genus)
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
        casus ARBOR2_LEXEMA_TYPEDEF:
        casus ARBOR2_LEXEMA_CONST:
        casus ARBOR2_LEXEMA_VOLATILE:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
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

    /* For TERNARIUS, compare structure recursively */
    si (a->genus == ARBOR2_NODUS_TERNARIUS)
    {
        si (!_nodi_aequales(a->datum.ternarius.conditio, b->datum.ternarius.conditio))
        {
            redde FALSUM;
        }
        si (!_nodi_aequales(a->datum.ternarius.verum, b->datum.ternarius.verum))
        {
            redde FALSUM;
        }
        si (!_nodi_aequales(a->datum.ternarius.falsum, b->datum.ternarius.falsum))
        {
            redde FALSUM;
        }
    }

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

    /* Set identificator if lexema is an identifier - enables later resolution */
    si (lexema != NIHIL && lexema->lexema != NIHIL &&
        lexema->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        nodus->datum.ambiguus.identificator = piscina_allocare(glr->piscina, magnitudo(chorda));
        *(nodus->datum.ambiguus.identificator) = lexema->lexema->valor;
    }
    alioquin
    {
        nodus->datum.ambiguus.identificator = NIHIL;
    }

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

#if GLR_TRACE_SLOWDOWN
    g_shifts++;
#endif
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

#if GLR_TRACE_SLOWDOWN
                g_reduces++;
#endif

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
                            si (valor_novus != NIHIL)
                            {
                                LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);

                                /* Initializare campos commentorum */
                                valor_novus->commenta_ante = NIHIL;
                                valor_novus->commenta_post = NIHIL;

                                /* Promovere commenta ab operatore */
                                _promovere_commenta_interior(
                                    glr->piscina,
                                    lexemata[I],  /* tok_operator */
                                    valori[II],   /* sinister */
                                    valori[ZEPHYRUM]);  /* dexter */
                            }
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
                            si (valor_novus != NIHIL)
                            {
                                LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);

                                /* Initializare campos commentorum */
                                valor_novus->commenta_ante = NIHIL;
                                valor_novus->commenta_post = NIHIL;

                                /* Promovere commenta ab operatore */
                                _promovere_commenta_interior(
                                    glr->piscina,
                                    lexemata[I],         /* tok_operator */
                                    NIHIL,               /* nulla ante */
                                    valori[ZEPHYRUM]);   /* operandum */
                            }
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
                         * P185,P188,P191: factor -> 'sizeof' '(' STRUCT/UNION/ENUM ID '*' '*' ')' (7 symbols)
                         * P240-P243: factor -> 'sizeof' '(' type '[' expr ']' ')' (7 symbols) */
                        si (num_pop == II)
                        {
                            /* P143: sizeof expr */
                            Arbor2Nodus* nodus_sizeof;

                            nodus_sizeof = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_sizeof->genus = ARBOR2_NODUS_SIZEOF;
                            nodus_sizeof->lexema = lexemata[I];  /* sizeof token */
                            nodus_sizeof->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_sizeof, I, ZEPHYRUM);
                            nodus_sizeof->datum.sizeof_expr.tok_sizeof = lexemata[I];
                            nodus_sizeof->datum.sizeof_expr.tok_paren_ap = NIHIL;
                            nodus_sizeof->datum.sizeof_expr.est_typus = FALSUM;
                            nodus_sizeof->datum.sizeof_expr.operandum = valori[ZEPHYRUM];
                            nodus_sizeof->datum.sizeof_expr.tok_paren_cl = NIHIL;

                            /* Initializare campos commentorum */
                            nodus_sizeof->commenta_ante = NIHIL;
                            nodus_sizeof->commenta_post = NIHIL;

                            si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = nodus_sizeof;

                            /* Promovere commenta ab sizeof token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],         /* tok_sizeof */
                                NIHIL,               /* nulla ante */
                                valori[ZEPHYRUM]);   /* operandum */

                            valor_novus = nodus_sizeof;
                        }
                        alioquin si (num_pop == VII && actio->valor >= 240 && actio->valor <= 243)
                        {
                            /* P240-P243: sizeof(type[N]) - 7 symbols
                             * sizeof ( type [ expr ] )
                             *    6    5   4   3   2  1  0
                             * valori[2] = dimension expression */
                            Arbor2Nodus* nodus_sizeof;
                            Arbor2Nodus* nodus_typus;
                            Arbor2Nodus** dim_slot;

                            /* Creare nodus typus cum dimensione */
                            nodus_typus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_typus->genus = ARBOR2_NODUS_DECLARATOR;
                            nodus_typus->lexema = lexemata[IV];  /* type token */
                            nodus_typus->pater = NIHIL;
                            nodus_typus->datum.declarator.pointer_levels = NIHIL;
                            nodus_typus->datum.declarator.titulus.datum = NIHIL;
                            nodus_typus->datum.declarator.titulus.mensura = ZEPHYRUM;
                            nodus_typus->datum.declarator.latitudo_biti = NIHIL;

                            /* Creare dimensiones array et addere dimensionem */
                            nodus_typus->datum.declarator.dimensiones = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            dim_slot = xar_addere(nodus_typus->datum.declarator.dimensiones);
                            *dim_slot = valori[II];  /* dimension expression at valori[2] */
                            si (valori[II] != NIHIL) valori[II]->pater = nodus_typus;

                            /* Creare nodus sizeof */
                            nodus_sizeof = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_sizeof->genus = ARBOR2_NODUS_SIZEOF;
                            nodus_sizeof->lexema = lexemata[VI];  /* sizeof token */
                            nodus_sizeof->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_sizeof, VI, ZEPHYRUM);
                            nodus_sizeof->datum.sizeof_expr.tok_sizeof = lexemata[VI];
                            nodus_sizeof->datum.sizeof_expr.tok_paren_ap = lexemata[V];
                            nodus_sizeof->datum.sizeof_expr.est_typus = VERUM;
                            nodus_sizeof->datum.sizeof_expr.operandum = nodus_typus;
                            nodus_sizeof->datum.sizeof_expr.tok_paren_cl = lexemata[ZEPHYRUM];
                            nodus_typus->pater = nodus_sizeof;

                            valor_novus = nodus_sizeof;
                        }
                        alioquin si (num_pop == X && actio->valor >= 244 && actio->valor <= 247)
                        {
                            /* P244-P247: sizeof(type[N][M]) - 10 symbols
                             * sizeof ( type [ expr ] [ expr ] )
                             *    9    8   7   6   5  4  3   2  1  0
                             * valori[5] = first dimension expression
                             * valori[2] = second dimension expression */
                            Arbor2Nodus* nodus_sizeof;
                            Arbor2Nodus* nodus_typus;
                            Arbor2Nodus** dim_slot;

                            /* Creare nodus typus cum duobus dimensionibus */
                            nodus_typus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_typus->genus = ARBOR2_NODUS_DECLARATOR;
                            nodus_typus->lexema = lexemata[VII];  /* type token */
                            nodus_typus->pater = NIHIL;
                            nodus_typus->datum.declarator.pointer_levels = NIHIL;
                            nodus_typus->datum.declarator.titulus.datum = NIHIL;
                            nodus_typus->datum.declarator.titulus.mensura = ZEPHYRUM;
                            nodus_typus->datum.declarator.latitudo_biti = NIHIL;

                            /* Creare dimensiones array et addere duas dimensiones */
                            nodus_typus->datum.declarator.dimensiones = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));

                            /* Prima dimensio */
                            dim_slot = xar_addere(nodus_typus->datum.declarator.dimensiones);
                            *dim_slot = valori[V];  /* first dimension at valori[5] */
                            si (valori[V] != NIHIL) valori[V]->pater = nodus_typus;

                            /* Secunda dimensio */
                            dim_slot = xar_addere(nodus_typus->datum.declarator.dimensiones);
                            *dim_slot = valori[II];  /* second dimension at valori[2] */
                            si (valori[II] != NIHIL) valori[II]->pater = nodus_typus;

                            /* Creare nodus sizeof */
                            nodus_sizeof = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_sizeof->genus = ARBOR2_NODUS_SIZEOF;
                            nodus_sizeof->lexema = lexemata[IX];  /* sizeof token */
                            nodus_sizeof->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_sizeof, IX, ZEPHYRUM);
                            nodus_sizeof->datum.sizeof_expr.tok_sizeof = lexemata[IX];
                            nodus_sizeof->datum.sizeof_expr.tok_paren_ap = lexemata[VIII];
                            nodus_sizeof->datum.sizeof_expr.est_typus = VERUM;
                            nodus_sizeof->datum.sizeof_expr.operandum = nodus_typus;
                            nodus_sizeof->datum.sizeof_expr.tok_paren_cl = lexemata[ZEPHYRUM];
                            nodus_typus->pater = nodus_sizeof;

                            valor_novus = nodus_sizeof;
                        }
                        alioquin si (num_pop == VIII && actio->valor >= 248 && actio->valor <= 251)
                        {
                            /* P248-P251: sizeof(type*[N]) - 8 symbols (pointer array)
                             * sizeof ( type * [ expr ] )
                             *    7    6   5   4  3   2  1  0
                             * valori[2] = dimension expression
                             * lexemata[5] = type token
                             * lexemata[7] = sizeof token */
                            Arbor2Nodus* nodus_sizeof;
                            Arbor2Nodus* nodus_typus;
                            Arbor2Nodus** dim_slot;

                            /* Creare nodus typus cum uno pointer et una dimensione */
                            nodus_typus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_typus->genus = ARBOR2_NODUS_DECLARATOR;
                            nodus_typus->lexema = lexemata[V];  /* type token */
                            nodus_typus->pater = NIHIL;
                            /* One pointer level */
                            {
                                Arbor2PointerLevel* lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                                Arbor2PointerLevel** lvl_slot;
                                lvl->tok_stella = lexemata[IV];
                                lvl->tok_const = NIHIL;
                                lvl->tok_volatile = NIHIL;
                                nodus_typus->datum.declarator.pointer_levels = xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                lvl_slot = xar_addere(nodus_typus->datum.declarator.pointer_levels);
                                *lvl_slot = lvl;
                            }
                            nodus_typus->datum.declarator.titulus.datum = NIHIL;
                            nodus_typus->datum.declarator.titulus.mensura = ZEPHYRUM;
                            nodus_typus->datum.declarator.latitudo_biti = NIHIL;

                            /* Creare dimensiones array et addere dimensionem */
                            nodus_typus->datum.declarator.dimensiones = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            dim_slot = xar_addere(nodus_typus->datum.declarator.dimensiones);
                            *dim_slot = valori[II];  /* dimension at valori[2] */
                            si (valori[II] != NIHIL) valori[II]->pater = nodus_typus;

                            /* Creare nodus sizeof */
                            nodus_sizeof = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_sizeof->genus = ARBOR2_NODUS_SIZEOF;
                            nodus_sizeof->lexema = lexemata[VII];  /* sizeof token */
                            nodus_sizeof->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_sizeof, VII, ZEPHYRUM);
                            nodus_sizeof->datum.sizeof_expr.tok_sizeof = lexemata[VII];
                            nodus_sizeof->datum.sizeof_expr.tok_paren_ap = lexemata[VI];
                            nodus_sizeof->datum.sizeof_expr.est_typus = VERUM;
                            nodus_sizeof->datum.sizeof_expr.operandum = nodus_typus;
                            nodus_sizeof->datum.sizeof_expr.tok_paren_cl = lexemata[ZEPHYRUM];
                            nodus_typus->pater = nodus_sizeof;

                            valor_novus = nodus_sizeof;
                        }
                        alioquin si (num_pop >= IV && num_pop <= VII)
                        {
                            /* sizeof(type) with various forms */
                            Arbor2Nodus* nodus_sizeof;
                            Arbor2Nodus* nodus_typus;
                            s32 ptr_count;
                            s32 prod_num;
                            s32 type_token_idx;

                            prod_num = (s32)actio->valor;

                            /* Determinare numerum stellarum (pointers)
                             * P183-P191 (struct/union/enum): extra symbol, so subtract V
                             * P162-P173 (basic types): subtract IV */
                            si (prod_num >= 183 && prod_num <= 191)
                            {
                                /* sizeof(struct/union/enum): 5=base, 6=*, 7=** */
                                ptr_count = num_pop - V;
                                type_token_idx = num_pop - III;  /* ID token (struct ID) */
                            }
                            alioquin
                            {
                                /* sizeof(basic type): 4=base, 5=*, 6=**
                                 * sizeof ( type )    -> type at index 1 = num_pop - 3
                                 * sizeof ( type * )  -> type at index 2 = num_pop - 3
                                 * sizeof ( type ** ) -> type at index 3 = num_pop - 3 */
                                ptr_count = num_pop - IV;
                                type_token_idx = num_pop - III;  /* type keyword */
                            }

                            /* Creare nodus typus */
                            nodus_typus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_typus->genus = ARBOR2_NODUS_DECLARATOR;
                            nodus_typus->lexema = lexemata[type_token_idx];  /* type token */
                            nodus_typus->pater = NIHIL;
                            /* Create pointer levels */
                            si (ptr_count > ZEPHYRUM)
                            {
                                s32 i;
                                nodus_typus->datum.declarator.pointer_levels = xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                per (i = ZEPHYRUM; i < ptr_count; i++)
                                {
                                    Arbor2PointerLevel* lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                                    Arbor2PointerLevel** lvl_slot;
                                    /* Pointer tokens are between type and ')' */
                                    lvl->tok_stella = lexemata[type_token_idx - I - i];
                                    lvl->tok_const = NIHIL;
                                    lvl->tok_volatile = NIHIL;
                                    lvl_slot = xar_addere(nodus_typus->datum.declarator.pointer_levels);
                                    *lvl_slot = lvl;
                                }
                            }
                            alioquin
                            {
                                nodus_typus->datum.declarator.pointer_levels = NIHIL;
                            }
                            nodus_typus->datum.declarator.titulus.datum = NIHIL;
                            nodus_typus->datum.declarator.titulus.mensura = ZEPHYRUM;

                            /* Creare nodus sizeof */
                            nodus_sizeof = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_sizeof->genus = ARBOR2_NODUS_SIZEOF;
                            nodus_sizeof->lexema = lexemata[num_pop - I];  /* sizeof token */
                            nodus_sizeof->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_sizeof, num_pop - I, ZEPHYRUM);
                            nodus_sizeof->datum.sizeof_expr.tok_sizeof = lexemata[num_pop - I];
                            nodus_sizeof->datum.sizeof_expr.tok_paren_ap = lexemata[num_pop - II];
                            nodus_sizeof->datum.sizeof_expr.est_typus = VERUM;
                            nodus_sizeof->datum.sizeof_expr.operandum = nodus_typus;
                            nodus_sizeof->datum.sizeof_expr.tok_paren_cl = lexemata[ZEPHYRUM];
                            nodus_typus->pater = nodus_sizeof;

                            /* Initializare campos commentorum */
                            nodus_sizeof->commenta_ante = NIHIL;
                            nodus_sizeof->commenta_post = NIHIL;
                            nodus_typus->commenta_ante = NIHIL;
                            nodus_typus->commenta_post = NIHIL;

                            /* Promovere commenta ab '(' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[num_pop - II],  /* tok_paren_ap */
                                NIHIL,                   /* nulla ante */
                                nodus_typus);            /* typus */

                            /* Promovere commenta ab ')' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[ZEPHYRUM],      /* tok_paren_cl */
                                nodus_typus,             /* typus */
                                NIHIL);                  /* nulla post */

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
                            nodus_typus->pater = NIHIL;
                            nodus_typus->datum.declarator.titulus.datum = NIHIL;
                            nodus_typus->datum.declarator.titulus.mensura = ZEPHYRUM;

                            /* Creare pointer_levels */
                            si (num_stellae > ZEPHYRUM)
                            {
                                s32 i;
                                nodus_typus->datum.declarator.pointer_levels = xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                per (i = ZEPHYRUM; i < num_stellae; i++)
                                {
                                    Arbor2PointerLevel* lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                                    Arbor2PointerLevel** lvl_slot;
                                    /* Pointer tokens are before ')' which is at index 1
                                     * Stars start at index 2 and go up:
                                     * (TYPE *)     -> star at index 2, num_stellae=1
                                     * (TYPE * *)   -> stars at indices 3,2, num_stellae=2
                                     * Formula: lexemata[I + num_stellae - i] */
                                    lvl->tok_stella = lexemata[I + num_stellae - i];
                                    lvl->tok_const = NIHIL;
                                    lvl->tok_volatile = NIHIL;
                                    lvl_slot = xar_addere(nodus_typus->datum.declarator.pointer_levels);
                                    *lvl_slot = lvl;
                                }
                            }
                            alioquin
                            {
                                nodus_typus->datum.declarator.pointer_levels = NIHIL;
                            }

                            /* Creare nodus conversio */
                            nodus_cast = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_cast->genus = ARBOR2_NODUS_CONVERSIO;
                            nodus_cast->lexema = lexemata[type_token_idx];  /* type token for display */
                            nodus_cast->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_cast, num_pop - I, ZEPHYRUM);
                            nodus_cast->datum.conversio.tok_paren_ap = lexemata[num_pop - I];
                            nodus_cast->datum.conversio.typus = nodus_typus;
                            nodus_cast->datum.conversio.tok_paren_cl = lexemata[I];  /* ) before operand */
                            nodus_cast->datum.conversio.expressio = valori[ZEPHYRUM];

                            /* Initializare campos commentorum */
                            nodus_cast->commenta_ante = NIHIL;
                            nodus_cast->commenta_post = NIHIL;
                            nodus_typus->commenta_ante = NIHIL;
                            nodus_typus->commenta_post = NIHIL;

                            /* Statuere patrem pro filiis */
                            nodus_typus->pater = nodus_cast;
                            si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = nodus_cast;

                            /* Promovere commenta ab '(' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[num_pop - I],  /* tok_paren_ap */
                                NIHIL,                  /* nulla ante */
                                nodus_typus);           /* typus */

                            /* Promovere commenta ab ')' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],            /* tok_paren_cl */
                                nodus_typus,            /* typus */
                                valori[ZEPHYRUM]);      /* expressio */

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
                            nodus_tern->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_tern, IV, ZEPHYRUM);
                            nodus_tern->datum.ternarius.conditio = valori[IV];
                            nodus_tern->datum.ternarius.tok_interrogatio = lexemata[III];
                            nodus_tern->datum.ternarius.verum = valori[II];
                            nodus_tern->datum.ternarius.tok_colon = lexemata[I];
                            nodus_tern->datum.ternarius.falsum = valori[ZEPHYRUM];

                            /* Initializare campos commentorum */
                            nodus_tern->commenta_ante = NIHIL;
                            nodus_tern->commenta_post = NIHIL;

                            /* Statuere patrem pro filiis */
                            si (valori[IV] != NIHIL) valori[IV]->pater = nodus_tern;
                            si (valori[II] != NIHIL) valori[II]->pater = nodus_tern;
                            si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = nodus_tern;

                            /* Promovere commenta ab ? token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[III],  /* tok_interrogatio */
                                valori[IV],     /* conditio */
                                valori[II]);    /* verum */

                            /* Promovere commenta ab : token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],    /* tok_colon */
                                valori[II],     /* verum */
                                valori[ZEPHYRUM]);  /* falsum */

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
                            nodus_sub->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_sub, III, ZEPHYRUM);
                            nodus_sub->datum.subscriptio.basis = valori[III];
                            nodus_sub->datum.subscriptio.tok_bracket_ap = lexemata[II];
                            nodus_sub->datum.subscriptio.index = valori[I];
                            nodus_sub->datum.subscriptio.tok_bracket_cl = lexemata[ZEPHYRUM];

                            /* Initializare campos commentorum */
                            nodus_sub->commenta_ante = NIHIL;
                            nodus_sub->commenta_post = NIHIL;

                            /* Statuere patrem pro filiis */
                            si (valori[III] != NIHIL) valori[III]->pater = nodus_sub;
                            si (valori[I] != NIHIL) valori[I]->pater = nodus_sub;

                            /* Promovere commenta ab '[' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[II],   /* tok_bracket_ap */
                                valori[III],    /* basis */
                                valori[I]);     /* index */

                            /* Promovere commenta ab ']' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[ZEPHYRUM],  /* tok_bracket_cl */
                                valori[I],      /* index */
                                NIHIL);         /* nulla post */

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
                            nodus_call->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_call, II, ZEPHYRUM);
                            nodus_call->datum.vocatio.basis = valori[II];
                            nodus_call->datum.vocatio.tok_paren_ap = lexemata[I];
                            nodus_call->datum.vocatio.argumenta = NIHIL;
                            nodus_call->datum.vocatio.tok_paren_cl = lexemata[ZEPHYRUM];

                            /* Initializare campos commentorum */
                            nodus_call->commenta_ante = NIHIL;
                            nodus_call->commenta_post = NIHIL;

                            /* Statuere patrem pro filio */
                            si (valori[II] != NIHIL) valori[II]->pater = nodus_call;

                            /* Promovere commenta ab '(' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],    /* tok_paren_ap */
                                valori[II],     /* basis */
                                NIHIL);         /* nulla argumenta */

                            /* Promovere commenta ab ')' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[ZEPHYRUM],  /* tok_paren_cl */
                                valori[II],     /* basis (no args, attach to base) */
                                NIHIL);

                            valor_novus = nodus_call;
                        }
                        alioquin si (actio->valor == 131 && num_pop >= IV)
                        {
                            /* Call with args: foo(a, b, c)
                             * valori: [3]=base, [2]='(', [1]=argumenta (LISTA_SEPARATA), [0]=')'
                             * argumenta is now a LISTA_SEPARATA node from P132/P133 */
                            Arbor2Nodus* nodus_call;
                            nodus_call = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            nodus_call->genus = ARBOR2_NODUS_VOCATIO;
                            nodus_call->lexema = lexemata[II];  /* '(' token */
                            nodus_call->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_call, III, ZEPHYRUM);
                            nodus_call->datum.vocatio.basis = valori[III];
                            nodus_call->datum.vocatio.tok_paren_ap = lexemata[II];
                            /* valori[I] is the LISTA_SEPARATA node with arguments and comma tokens */
                            nodus_call->datum.vocatio.argumenta = valori[I];
                            nodus_call->datum.vocatio.tok_paren_cl = lexemata[ZEPHYRUM];

                            /* Initializare campos commentorum */
                            nodus_call->commenta_ante = NIHIL;
                            nodus_call->commenta_post = NIHIL;

                            /* Statuere patrem pro filiis */
                            si (valori[III] != NIHIL) valori[III]->pater = nodus_call;
                            si (valori[I] != NIHIL) valori[I]->pater = nodus_call;

                            /* Promovere commenta ab '(' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[II],   /* tok_paren_ap */
                                valori[III],    /* basis */
                                valori[I]);     /* argumenta */

                            /* Promovere commenta ab ')' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[ZEPHYRUM],  /* tok_paren_cl */
                                valori[I],      /* argumenta */
                                NIHIL);

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
                            nodus_mem->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_mem, II, ZEPHYRUM);
                            nodus_mem->datum.membrum.basis = valori[II];
                            nodus_mem->datum.membrum.tok_accessor = lexemata[I];
                            nodus_mem->datum.membrum.tok_membrum = lexemata[ZEPHYRUM];
                            nodus_mem->datum.membrum.membrum = lexemata[ZEPHYRUM]->lexema->valor;
                            nodus_mem->datum.membrum.est_sagitta = (actio->valor == 135);

                            /* Initializare campos commentorum */
                            nodus_mem->commenta_ante = NIHIL;
                            nodus_mem->commenta_post = NIHIL;

                            /* Statuere patrem pro filio */
                            si (valori[II] != NIHIL) valori[II]->pater = nodus_mem;

                            /* Promovere commenta ab accessor token (. or ->) */
                            /* Commenta post accessor adhaerent ad basis.commenta_post */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],    /* tok_accessor */
                                valori[II],     /* basis */
                                NIHIL);         /* membrum est token, non nodus */

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
                            nodus_post->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(nodus_post, I, ZEPHYRUM);
                            nodus_post->datum.post_unarium.operandum = valori[I];
                            nodus_post->datum.post_unarium.tok_operator = lexemata[ZEPHYRUM];
                            nodus_post->datum.post_unarium.operator = lexemata[ZEPHYRUM]->lexema->genus;

                            /* Initializare campos commentorum */
                            nodus_post->commenta_ante = NIHIL;
                            nodus_post->commenta_post = NIHIL;

                            /* Statuere patrem pro filio */
                            si (valori[I] != NIHIL) valori[I]->pater = nodus_post;

                            /* Promovere commenta ab operator token (++ or --) */
                            /* Commenta post operandum adhaerent ad operandum.commenta_post */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[ZEPHYRUM],  /* tok_operator */
                                valori[I],           /* operandum */
                                NIHIL);              /* nulla post */

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
                            member->pater = NIHIL;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.tok_unsigned = NIHIL;
                            member->datum.declaratio.tok_signed = NIHIL;
                            member->datum.declaratio.tok_long = NIHIL;
                            member->datum.declaratio.tok_long2 = NIHIL;
                            member->datum.declaratio.tok_short = NIHIL;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->pater = member;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            /* Statuere patrem pro filiis */
                            si (decl_node != NIHIL) decl_node->pater = member;

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
                            member->pater = NIHIL;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.tok_unsigned = NIHIL;
                            member->datum.declaratio.tok_signed = NIHIL;
                            member->datum.declaratio.tok_long = NIHIL;
                            member->datum.declaratio.tok_long2 = NIHIL;
                            member->datum.declaratio.tok_short = NIHIL;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->pater = member;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            /* Statuere patrem pro filiis */
                            si (decl_node != NIHIL) decl_node->pater = member;

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
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;
                            decl_node->datum.declarator.latitudo_biti = NIHIL;
                            decl_node->datum.declarator.dimensiones = NIHIL;
                            /* One pointer level with * at lexemata[II] */
                            {
                                Arbor2PointerLevel* lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                                Arbor2PointerLevel** lvl_slot;
                                lvl->tok_stella = lexemata[II];
                                lvl->tok_const = NIHIL;
                                lvl->tok_volatile = NIHIL;
                                decl_node->datum.declarator.pointer_levels = xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                lvl_slot = xar_addere(decl_node->datum.declarator.pointer_levels);
                                *lvl_slot = lvl;
                            }

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;
                            decl_node->datum.declarator.latitudo_biti = NIHIL;
                            decl_node->datum.declarator.dimensiones = NIHIL;
                            /* One pointer level with * at lexemata[II] */
                            {
                                Arbor2PointerLevel* lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                                Arbor2PointerLevel** lvl_slot;
                                lvl->tok_stella = lexemata[II];
                                lvl->tok_const = NIHIL;
                                lvl->tok_volatile = NIHIL;
                                decl_node->datum.declarator.pointer_levels = xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                lvl_slot = xar_addere(decl_node->datum.declarator.pointer_levels);
                                *lvl_slot = lvl;
                            }

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                            decl_node->datum.declarator.pointer_levels = NIHIL;
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;
                            decl_node->datum.declarator.latitudo_biti = expr_node;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                            /* lexemata: [4]=type, [3]=name, [0]=; */
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
                            decl_node->datum.declarator.pointer_levels = NIHIL;
                            decl_node->datum.declarator.titulus = name_tok->lexema->valor;
                            decl_node->datum.declarator.latitudo_biti = expr_node;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                            decl_node->datum.declarator.pointer_levels = NIHIL;
                            decl_node->datum.declarator.titulus.datum = NIHIL;
                            decl_node->datum.declarator.titulus.mensura = ZEPHYRUM;  /* anonymous */
                            decl_node->datum.declarator.latitudo_biti = expr_node;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                            /* lexemata: [3]=type, [0]=; */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista = (Xar*)valori[IV];
                            Arbor2Nodus** slot;
                            Arbor2Token* type_tok = lexemata[III];
                            Arbor2Nodus* expr_node = valori[I];

                            decl_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            decl_node->genus = ARBOR2_NODUS_DECLARATOR;
                            decl_node->lexema = type_tok;  /* use type token since no name */
                            decl_node->datum.declarator.pointer_levels = NIHIL;
                            decl_node->datum.declarator.titulus.datum = NIHIL;
                            decl_node->datum.declarator.titulus.mensura = ZEPHYRUM;  /* anonymous */
                            decl_node->datum.declarator.latitudo_biti = expr_node;
                            decl_node->datum.declarator.dimensiones = NIHIL;

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->lexema = type_tok;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = spec_node;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = spec_node;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = spec_node;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.specifier = spec_node;
                            member->datum.declaratio.declarator = decl_node;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
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
                        /* ========== COMPOUND TYPE SPECIFIERS P348-P366 ========== */
                        alioquin si (actio->valor >= 348 && actio->valor <= 366)
                        {
                            /* P348-P366: Compound type specifiers in first struct member
                             * P348: unsigned int declarator ;     (4 symbols)
                             * P349: signed int declarator ;       (4 symbols)
                             * P350: unsigned char declarator ;    (4 symbols)
                             * P351: signed char declarator ;      (4 symbols)
                             * P352: long int declarator ;         (4 symbols)
                             * P353: short int declarator ;        (4 symbols)
                             * P354: unsigned long declarator ;    (4 symbols)
                             * P355: unsigned short declarator ;   (4 symbols)
                             * P356: signed long declarator ;      (4 symbols)
                             * P357: signed short declarator ;     (4 symbols)
                             * P358: unsigned long int declarator ; (5 symbols)
                             * P359: unsigned short int declarator ; (5 symbols)
                             * P360: signed long int declarator ;   (5 symbols)
                             * P361: signed short int declarator ;  (5 symbols)
                             * P362: const type declarator ;        (4 symbols)
                             * P363: volatile type declarator ;     (4 symbols)
                             * P364: long long declarator ;         (4 symbols)
                             * P365: unsigned long long declarator ; (5 symbols)
                             * P366: signed long long declarator ;  (5 symbols)
                             */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Token* base_type_tok = NIHIL;
                            Arbor2Token* modifier1_tok = NIHIL;
                            Arbor2Token* modifier2_tok = NIHIL;
                            i32 num_symbols = regula->longitudo;

                            /* Get declarator from valori[1] (pre-built by P12/P11) */
                            decl_node = valori[I];

                            /* Create member node */
                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->pater = NIHIL;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.tok_unsigned = NIHIL;
                            member->datum.declaratio.tok_signed = NIHIL;
                            member->datum.declaratio.tok_long = NIHIL;
                            member->datum.declaratio.tok_long2 = NIHIL;
                            member->datum.declaratio.tok_short = NIHIL;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            /* Parse tokens based on production number */
                            si (num_symbols == 4)
                            {
                                /* 4-symbol productions: modifier type decl ; */
                                modifier1_tok = lexemata[III];
                                base_type_tok = lexemata[II];
                            }
                            alioquin si (num_symbols == 5)
                            {
                                /* 5-symbol productions: modifier1 modifier2 type decl ; */
                                modifier1_tok = lexemata[IV];
                                modifier2_tok = lexemata[III];
                                base_type_tok = lexemata[II];
                            }

                            /* Set modifier tokens based on production */
                            commutatio (actio->valor)
                            {
                                casus 348: /* unsigned int */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    frange;
                                casus 349: /* signed int */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    frange;
                                casus 350: /* unsigned char */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    frange;
                                casus 351: /* signed char */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    frange;
                                casus 352: /* long int */
                                    member->datum.declaratio.tok_long = modifier1_tok;
                                    frange;
                                casus 353: /* short int */
                                    member->datum.declaratio.tok_short = modifier1_tok;
                                    frange;
                                casus 354: /* unsigned long (implicit int) */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_long = base_type_tok;
                                    base_type_tok = NIHIL; /* No explicit base type */
                                    frange;
                                casus 355: /* unsigned short (implicit int) */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_short = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 356: /* signed long (implicit int) */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_long = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 357: /* signed short (implicit int) */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_short = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 358: /* unsigned long int */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    frange;
                                casus 359: /* unsigned short int */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_short = modifier2_tok;
                                    frange;
                                casus 360: /* signed long int */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    frange;
                                casus 361: /* signed short int */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_short = modifier2_tok;
                                    frange;
                                casus 362: /* const type */
                                    member->datum.declaratio.tok_const = modifier1_tok;
                                    frange;
                                casus 363: /* volatile type */
                                    member->datum.declaratio.tok_volatile = modifier1_tok;
                                    frange;
                                casus 364: /* long long (implicit int) */
                                    member->datum.declaratio.tok_long = modifier1_tok;
                                    member->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 365: /* unsigned long long */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    member->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 366: /* signed long long */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    member->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                ordinarius:
                                    frange;
                            }

                            /* Set specifier (base type) */
                            si (base_type_tok != NIHIL)
                            {
                                member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                                member->datum.declaratio.specifier->lexema = base_type_tok;
                                member->datum.declaratio.specifier->pater = member;
                                member->datum.declaratio.specifier->datum.folium.valor = base_type_tok->lexema->valor;
                                member->lexema = base_type_tok;
                            }
                            alioquin
                            {
                                /* No explicit base type (e.g., "unsigned long" = implicit int) */
                                member->datum.declaratio.specifier = NIHIL;
                                member->lexema = modifier1_tok;
                            }

                            member->datum.declaratio.declarator = decl_node;
                            si (decl_node != NIHIL) decl_node->pater = member;

                            /* Create member list */
                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        /* ========== QUALIFIER + TYPE MODIFIER SPECIFIERS P386-P417 ========== */
                        alioquin si (actio->valor >= 386 && actio->valor <= 417)
                        {
                            /* P386-P417: Qualifier + type modifier combinations in first struct member
                             * P386-P402: const + type modifier combinations
                             * P403-P417: volatile + type modifier combinations
                             *
                             * 5-symbol: qualifier modifier type decl ;
                             * 6-symbol: qualifier modifier1 modifier2 type decl ;
                             *
                             * lexemata layout:
                             *   5-symbol: [4]=qual [3]=mod [2]=type [1]=... [0]=;
                             *   6-symbol: [5]=qual [4]=mod1 [3]=mod2 [2]=type [1]=... [0]=;
                             */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Token* qualifier_tok = NIHIL;
                            Arbor2Token* modifier1_tok = NIHIL;
                            Arbor2Token* modifier2_tok = NIHIL;
                            Arbor2Token* base_type_tok = NIHIL;
                            i32 num_symbols = regula->longitudo;

                            /* Get declarator from valori[1] (pre-built by P12/P11) */
                            decl_node = valori[I];

                            /* Create member node */
                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->pater = NIHIL;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_const = NIHIL;
                            member->datum.declaratio.tok_volatile = NIHIL;
                            member->datum.declaratio.tok_unsigned = NIHIL;
                            member->datum.declaratio.tok_signed = NIHIL;
                            member->datum.declaratio.tok_long = NIHIL;
                            member->datum.declaratio.tok_long2 = NIHIL;
                            member->datum.declaratio.tok_short = NIHIL;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            /* Parse tokens based on number of symbols */
                            si (num_symbols == 5)
                            {
                                /* 5-symbol: qualifier modifier type decl ; */
                                qualifier_tok = lexemata[IV];
                                modifier1_tok = lexemata[III];
                                base_type_tok = lexemata[II];
                            }
                            alioquin si (num_symbols == 6)
                            {
                                /* 6-symbol: qualifier modifier1 modifier2 type decl ; */
                                qualifier_tok = lexemata[V];
                                modifier1_tok = lexemata[IV];
                                modifier2_tok = lexemata[III];
                                base_type_tok = lexemata[II];
                            }

                            /* Set qualifier token (const or volatile) */
                            si (actio->valor >= 386 && actio->valor <= 402)
                            {
                                /* const + type modifiers (P386-P402) */
                                member->datum.declaratio.tok_const = qualifier_tok;
                            }
                            alioquin
                            {
                                /* volatile + type modifiers (P403-P417) */
                                member->datum.declaratio.tok_volatile = qualifier_tok;
                            }

                            /* Set modifier tokens based on production */
                            commutatio (actio->valor)
                            {
                                /* P386-P402: const + type modifiers */
                                casus 386: /* const unsigned int */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    frange;
                                casus 387: /* const unsigned char */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    frange;
                                casus 388: /* const unsigned long (implicit int) */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_long = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 389: /* const unsigned short (implicit int) */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_short = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 390: /* const unsigned long int */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    frange;
                                casus 391: /* const unsigned short int */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_short = modifier2_tok;
                                    frange;
                                casus 392: /* const unsigned long long */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    member->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 393: /* const signed int */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    frange;
                                casus 394: /* const signed char */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    frange;
                                casus 395: /* const signed long (implicit int) */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_long = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 396: /* const signed short (implicit int) */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_short = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 397: /* const signed long int */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    frange;
                                casus 398: /* const signed short int */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_short = modifier2_tok;
                                    frange;
                                casus 399: /* const signed long long */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    member->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 400: /* const long int */
                                    member->datum.declaratio.tok_long = modifier1_tok;
                                    frange;
                                casus 401: /* const long long (implicit int) */
                                    member->datum.declaratio.tok_long = modifier1_tok;
                                    member->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 402: /* const short int */
                                    member->datum.declaratio.tok_short = modifier1_tok;
                                    frange;

                                /* P403-P417: volatile + type modifiers */
                                casus 403: /* volatile unsigned int */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    frange;
                                casus 404: /* volatile unsigned char */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    frange;
                                casus 405: /* volatile unsigned long (implicit int) */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_long = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 406: /* volatile unsigned short (implicit int) */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_short = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 407: /* volatile unsigned long int */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    frange;
                                casus 408: /* volatile unsigned short int */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_short = modifier2_tok;
                                    frange;
                                casus 409: /* volatile unsigned long long */
                                    member->datum.declaratio.tok_unsigned = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    member->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 410: /* volatile signed int */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    frange;
                                casus 411: /* volatile signed char */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    frange;
                                casus 412: /* volatile signed long (implicit int) */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_long = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 413: /* volatile signed short (implicit int) */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_short = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 414: /* volatile signed long int */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    frange;
                                casus 415: /* volatile signed short int */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_short = modifier2_tok;
                                    frange;
                                casus 416: /* volatile signed long long */
                                    member->datum.declaratio.tok_signed = modifier1_tok;
                                    member->datum.declaratio.tok_long = modifier2_tok;
                                    member->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 417: /* volatile long int */
                                    member->datum.declaratio.tok_long = modifier1_tok;
                                    frange;

                                ordinarius:
                                    frange;
                            }

                            /* Set specifier (base type) */
                            si (base_type_tok != NIHIL)
                            {
                                member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                                member->datum.declaratio.specifier->lexema = base_type_tok;
                                member->datum.declaratio.specifier->pater = member;
                                member->datum.declaratio.specifier->datum.folium.valor = base_type_tok->lexema->valor;
                                member->lexema = qualifier_tok;  /* Use qualifier as primary lexema */
                            }
                            alioquin
                            {
                                /* No explicit base type (implicit int) */
                                member->datum.declaratio.specifier = NIHIL;
                                member->lexema = qualifier_tok;
                            }

                            member->datum.declaratio.declarator = decl_node;
                            si (decl_node != NIHIL) decl_node->pater = member;

                            /* Create member list */
                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        /* ========== DOUBLE QUALIFIER P418-P419 ========== */
                        alioquin si (actio->valor == 418 || actio->valor == 419)
                        {
                            /* P418: 'const' 'volatile' type declarator ';' (5 symbols)
                             * P419: 'volatile' 'const' type declarator ';' (5 symbols)
                             * lexemata: [4]=qual1 [3]=qual2 [2]=type [1]=... [0]=;
                             * valori: [1]=declarator node */
                            Arbor2Nodus* member;
                            Arbor2Nodus* decl_node;
                            Xar* lista;
                            Arbor2Nodus** slot;
                            Arbor2Token* qual1_tok = lexemata[IV];
                            Arbor2Token* qual2_tok = lexemata[III];
                            Arbor2Token* type_tok = lexemata[II];

                            decl_node = valori[I];

                            member = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->genus = ARBOR2_NODUS_DECLARATIO;
                            member->pater = NIHIL;
                            member->datum.declaratio.tok_storage = NIHIL;
                            member->datum.declaratio.tok_unsigned = NIHIL;
                            member->datum.declaratio.tok_signed = NIHIL;
                            member->datum.declaratio.tok_long = NIHIL;
                            member->datum.declaratio.tok_long2 = NIHIL;
                            member->datum.declaratio.tok_short = NIHIL;
                            member->datum.declaratio.tok_assignatio = NIHIL;
                            member->datum.declaratio.initializor = NIHIL;
                            member->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            member->datum.declaratio.tok_comma = NIHIL;
                            member->datum.declaratio.proxima = NIHIL;

                            /* Set qualifiers - both const and volatile */
                            si (actio->valor == 418)
                            {
                                /* P418: const volatile - qual1=const, qual2=volatile */
                                member->datum.declaratio.tok_const = qual1_tok;
                                member->datum.declaratio.tok_volatile = qual2_tok;
                            }
                            alioquin
                            {
                                /* P419: volatile const - qual1=volatile, qual2=const */
                                member->datum.declaratio.tok_volatile = qual1_tok;
                                member->datum.declaratio.tok_const = qual2_tok;
                            }

                            /* Set specifier (base type) */
                            member->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            member->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            member->datum.declaratio.specifier->lexema = type_tok;
                            member->datum.declaratio.specifier->pater = member;
                            member->datum.declaratio.specifier->datum.folium.valor = type_tok->lexema->valor;
                            member->lexema = qual1_tok;

                            member->datum.declaratio.declarator = decl_node;
                            si (decl_node != NIHIL) decl_node->pater = member;

                            lista = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            slot = xar_addere(lista);
                            *slot = member;
                            valor_novus = (Arbor2Nodus*)lista;
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
                            valor_novus->datum.declaratio.tok_storage = lexemata[III];  /* typedef */
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.specifier = spec_node;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                            valor_novus->datum.declaratio.tok_storage = lexemata[III];  /* typedef */
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.specifier = spec_node;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                            valor_novus->datum.declaratio.tok_storage = lexemata[III];  /* typedef */
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.specifier = spec_node;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                        /* ========== FUNCTION POINTER TYPEDEF P525-P526 ========== */
                        alioquin si (actio->valor == 525)
                        {
                            /* P525: 'typedef' type '(' '*' ID ')' '(' ')' ';' (9 symbols)
                             * lexemata: [8]=typedef, [7]=type, [6]=(, [5]=*, [4]=ID,
                             *           [3]=), [2]=(, [1]=), [0]=; */
                            Arbor2Nodus* spec_node;
                            Arbor2Nodus* inner_decl;
                            Arbor2Nodus* func_decl;
                            Arbor2PointerLevel* ptr_lvl;
                            Arbor2PointerLevel** ptr_slot;

                            /* Creare specifier (type) */
                            spec_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            spec_node->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            spec_node->lexema = lexemata[VII];
                            spec_node->datum.folium.valor = lexemata[VII]->lexema->valor;

                            /* Creare inner declarator (*name) */
                            inner_decl = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            inner_decl->genus = ARBOR2_NODUS_DECLARATOR;
                            inner_decl->lexema = lexemata[IV];  /* ID token */
                            inner_decl->datum.declarator.titulus = lexemata[IV]->lexema->valor;
                            inner_decl->datum.declarator.latitudo_biti = NIHIL;
                            inner_decl->datum.declarator.dimensiones = NIHIL;
                            inner_decl->datum.declarator.pointer_levels =
                                xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                            ptr_lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                            ptr_lvl->tok_stella = lexemata[V];
                            ptr_lvl->tok_const = NIHIL;
                            ptr_lvl->tok_volatile = NIHIL;
                            ptr_slot = xar_addere(inner_decl->datum.declarator.pointer_levels);
                            *ptr_slot = ptr_lvl;

                            /* Creare function declarator wrapping inner */
                            func_decl = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            func_decl->genus = ARBOR2_NODUS_DECLARATOR_FUNCTI;
                            func_decl->lexema = lexemata[IV];
                            func_decl->datum.declarator_functi.declarator_interior = inner_decl;
                            func_decl->datum.declarator_functi.tok_paren_ap = lexemata[II];
                            func_decl->datum.declarator_functi.parametri = NIHIL;
                            func_decl->datum.declarator_functi.tok_paren_cl = lexemata[I];
                            func_decl->datum.declarator_functi.tok_void = NIHIL;
                            func_decl->datum.declarator_functi.habet_void = FALSUM;
                            func_decl->datum.declarator_functi.est_variadicus = FALSUM;
                            func_decl->datum.declarator_functi.pointer_levels = NIHIL;

                            /* Creare declaration */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = lexemata[VIII];  /* typedef */
                            valor_novus->datum.declaratio.tok_storage = lexemata[VIII];
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.tok_unsigned = NIHIL;
                            valor_novus->datum.declaratio.tok_signed = NIHIL;
                            valor_novus->datum.declaratio.tok_long = NIHIL;
                            valor_novus->datum.declaratio.tok_long2 = NIHIL;
                            valor_novus->datum.declaratio.tok_short = NIHIL;
                            valor_novus->datum.declaratio.specifier = spec_node;
                            valor_novus->datum.declaratio.declarator = func_decl;
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = VERUM;
                            valor_novus->datum.declaratio.storage_class = ZEPHYRUM;
                            valor_novus->datum.declaratio.qualifiers = ZEPHYRUM;
                        }
                        alioquin si (actio->valor == 526)
                        {
                            /* P526: 'typedef' type '(' '*' ID ')' '(' 'void' ')' ';' (10 symbols)
                             * lexemata: [9]=typedef, [8]=type, [7]=(, [6]=*, [5]=ID,
                             *           [4]=), [3]=(, [2]=void, [1]=), [0]=; */
                            Arbor2Nodus* spec_node;
                            Arbor2Nodus* inner_decl;
                            Arbor2Nodus* func_decl;
                            Arbor2PointerLevel* ptr_lvl;
                            Arbor2PointerLevel** ptr_slot;

                            /* Creare specifier (type) */
                            spec_node = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            spec_node->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            spec_node->lexema = lexemata[VIII];
                            spec_node->datum.folium.valor = lexemata[VIII]->lexema->valor;

                            /* Creare inner declarator (*name) */
                            inner_decl = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            inner_decl->genus = ARBOR2_NODUS_DECLARATOR;
                            inner_decl->lexema = lexemata[V];  /* ID token */
                            inner_decl->datum.declarator.titulus = lexemata[V]->lexema->valor;
                            inner_decl->datum.declarator.latitudo_biti = NIHIL;
                            inner_decl->datum.declarator.dimensiones = NIHIL;
                            inner_decl->datum.declarator.pointer_levels =
                                xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                            ptr_lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                            ptr_lvl->tok_stella = lexemata[VI];
                            ptr_lvl->tok_const = NIHIL;
                            ptr_lvl->tok_volatile = NIHIL;
                            ptr_slot = xar_addere(inner_decl->datum.declarator.pointer_levels);
                            *ptr_slot = ptr_lvl;

                            /* Creare function declarator wrapping inner */
                            func_decl = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            func_decl->genus = ARBOR2_NODUS_DECLARATOR_FUNCTI;
                            func_decl->lexema = lexemata[V];
                            func_decl->datum.declarator_functi.declarator_interior = inner_decl;
                            func_decl->datum.declarator_functi.tok_paren_ap = lexemata[III];
                            func_decl->datum.declarator_functi.parametri = NIHIL;
                            func_decl->datum.declarator_functi.tok_paren_cl = lexemata[I];
                            func_decl->datum.declarator_functi.tok_void = lexemata[II];
                            func_decl->datum.declarator_functi.habet_void = VERUM;
                            func_decl->datum.declarator_functi.est_variadicus = FALSUM;
                            func_decl->datum.declarator_functi.pointer_levels = NIHIL;

                            /* Creare declaration */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->lexema = lexemata[IX];  /* typedef */
                            valor_novus->datum.declaratio.tok_storage = lexemata[IX];
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.tok_unsigned = NIHIL;
                            valor_novus->datum.declaratio.tok_signed = NIHIL;
                            valor_novus->datum.declaratio.tok_long = NIHIL;
                            valor_novus->datum.declaratio.tok_long2 = NIHIL;
                            valor_novus->datum.declaratio.tok_short = NIHIL;
                            valor_novus->datum.declaratio.specifier = spec_node;
                            valor_novus->datum.declaratio.declarator = func_decl;
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = VERUM;
                            valor_novus->datum.declaratio.storage_class = ZEPHYRUM;
                            valor_novus->datum.declaratio.qualifiers = ZEPHYRUM;
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

                            /* Initialize token slots based on production */
                            valor_novus->datum.declaratio.tok_storage = NIHIL;
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = valori[ZEPHYRUM];
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;  /* No semicolon in intermediate form */
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;

                            /* Ponere storage_class vel qualifiers */
                            commutatio (actio->valor)
                            {
                                casus 148:  /* static */
                                    valor_novus->datum.declaratio.tok_storage = lexemata[II];
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_STATIC;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                                    frange;
                                casus 149:  /* extern */
                                    valor_novus->datum.declaratio.tok_storage = lexemata[II];
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_EXTERN;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                                    frange;
                                casus 150:  /* register */
                                    valor_novus->datum.declaratio.tok_storage = lexemata[II];
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_REGISTER;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                                    frange;
                                casus 151:  /* auto */
                                    valor_novus->datum.declaratio.tok_storage = lexemata[II];
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_AUTO;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;
                                    frange;
                                casus 152:  /* const */
                                    valor_novus->datum.declaratio.tok_const = lexemata[II];
                                    valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                                    valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_CONST;
                                    frange;
                                casus 153:  /* volatile */
                                    valor_novus->datum.declaratio.tok_volatile = lexemata[II];
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
                            valor_novus->datum.declaratio.tok_storage = NIHIL;
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.tok_assignatio = lexemata[I];  /* '=' */
                            valor_novus->datum.declaratio.initializor = init_expr;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;  /* No semicolon in intermediate */
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                            valor_novus->datum.declaratio.tok_storage = (storage != ARBOR2_STORAGE_NONE) ? lexemata[IV] : NIHIL;
                            valor_novus->datum.declaratio.tok_const = (quals == ARBOR2_QUAL_CONST) ? lexemata[IV] : NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = (quals == ARBOR2_QUAL_VOLATILE) ? lexemata[IV] : NIHIL;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.tok_assignatio = lexemata[I];  /* '=' */
                            valor_novus->datum.declaratio.initializor = init_expr;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;  /* No semicolon in intermediate */
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                            valor_novus->datum.declaratio.tok_storage = NIHIL;
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.tok_assignatio = lexemata[I];  /* '=' */
                            valor_novus->datum.declaratio.initializor = init_lista;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;  /* No semicolon in intermediate */
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                            valor_novus->datum.declaratio.tok_storage = (storage != ARBOR2_STORAGE_NONE) ? lexemata[IV] : NIHIL;
                            valor_novus->datum.declaratio.tok_const = (quals == ARBOR2_QUAL_CONST) ? lexemata[IV] : NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = (quals == ARBOR2_QUAL_VOLATILE) ? lexemata[IV] : NIHIL;
                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.tok_assignatio = lexemata[I];  /* '=' */
                            valor_novus->datum.declaratio.initializor = init_lista;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;  /* No semicolon in intermediate */
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                            valor_novus->datum.declaratio.tok_storage = (storage != ARBOR2_STORAGE_NONE) ? lexemata[III] : NIHIL;

                            /* Handle token positions for const/volatile combinations
                             * P227-P232: storage + qualifier - lexemata[III]=storage, lexemata[II]=qualifier
                             * P233: const volatile - lexemata[III]=const, lexemata[II]=volatile
                             * P234: volatile const - lexemata[III]=volatile, lexemata[II]=const */
                            si (actio->valor == 233)
                            {
                                /* const volatile: [3]=const, [2]=volatile */
                                valor_novus->datum.declaratio.tok_const = lexemata[III];
                                valor_novus->datum.declaratio.tok_volatile = lexemata[II];
                            }
                            alioquin si (actio->valor == 234)
                            {
                                /* volatile const: [3]=volatile, [2]=const */
                                valor_novus->datum.declaratio.tok_volatile = lexemata[III];
                                valor_novus->datum.declaratio.tok_const = lexemata[II];
                            }
                            alioquin
                            {
                                /* P227-P232: storage + single qualifier */
                                valor_novus->datum.declaratio.tok_const = (quals & ARBOR2_QUAL_CONST) ? lexemata[II] : NIHIL;
                                valor_novus->datum.declaratio.tok_volatile = (quals & ARBOR2_QUAL_VOLATILE) ? lexemata[II] : NIHIL;
                            }

                            valor_novus->datum.declaratio.specifier = type_spec;
                            valor_novus->datum.declaratio.declarator = decl_node;
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;  /* No semicolon in intermediate */
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = storage;
                            valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_CONST;
                        }
                        /* P239: stmt -> declaratio ';' (2 symbols, pass-through declaration) */
                        alioquin si (actio->valor == 239)
                        {
                            /* Declaration as statement - pass through the declaration node */
                            /* valori[1] = declaratio, lexemata[0] = ';' token */
                            valor_novus = valori[I];
                            /* Set the semicolon token on the declaration */
                            si (valor_novus != NIHIL && valor_novus->genus == ARBOR2_NODUS_DECLARATIO)
                            {
                                valor_novus->datum.declaratio.tok_semicolon = lexemata[ZEPHYRUM];
                            }
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
                                    Arbor2Token* tok_assign = NIHIL;
                                    Arbor2Nodus* init_node = NIHIL;
                                    Arbor2Token* tok_comma = NIHIL;
                                    Arbor2Nodus* nodus;

                                    /* Quad: [0]=declarator, [1]=tok_assignatio, [2]=initializor, [3]=tok_comma */
                                    si (pair != NIHIL && xar_numerus(pair) >= III)
                                    {
                                        decl_node = *(Arbor2Nodus**)xar_obtinere(pair, ZEPHYRUM);
                                        tok_assign = *(Arbor2Token**)xar_obtinere(pair, I);
                                        init_node = *(Arbor2Nodus**)xar_obtinere(pair, II);
                                    }
                                    /* Read tok_comma from 4th slot if available */
                                    si (pair != NIHIL && xar_numerus(pair) >= IV)
                                    {
                                        tok_comma = *(Arbor2Token**)xar_obtinere(pair, III);
                                    }

                                    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                    nodus->genus = ARBOR2_NODUS_DECLARATIO;
                                    nodus->lexema = type_tok;
                                    nodus->datum.declaratio.tok_storage = NIHIL;
                                    nodus->datum.declaratio.tok_const = NIHIL;
                                    nodus->datum.declaratio.tok_volatile = NIHIL;
                                    nodus->datum.declaratio.specifier = type_spec;
                                    nodus->datum.declaratio.declarator = decl_node;
                                    nodus->datum.declaratio.tok_assignatio = tok_assign;
                                    nodus->datum.declaratio.initializor = init_node;
                                    nodus->datum.declaratio.tok_semicolon = NIHIL;  /* Set by P239 or post-process */
                                    nodus->datum.declaratio.tok_comma = tok_comma;
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
                        /* ========== TOP-LEVEL COMPOUND TYPE SPECIFIERS P420-P436 ========== */
                        alioquin si (actio->valor >= 420 && actio->valor <= 436)
                        {
                            /* P420-P436: Compound type specifiers at top-level
                             * P420: unsigned int declarator     (3 symbols)
                             * P421: signed int declarator       (3 symbols)
                             * P422: unsigned char declarator    (3 symbols)
                             * P423: signed char declarator      (3 symbols)
                             * P424: long int declarator         (3 symbols)
                             * P425: short int declarator        (3 symbols)
                             * P426: unsigned long declarator    (3 symbols)
                             * P427: unsigned short declarator   (3 symbols)
                             * P428: signed long declarator      (3 symbols)
                             * P429: signed short declarator     (3 symbols)
                             * P430: unsigned long int declarator  (4 symbols)
                             * P431: unsigned short int declarator (4 symbols)
                             * P432: signed long int declarator    (4 symbols)
                             * P433: signed short int declarator   (4 symbols)
                             * P434: long long declarator          (3 symbols)
                             * P435: unsigned long long declarator (4 symbols)
                             * P436: signed long long declarator   (4 symbols)
                             */
                            Arbor2Nodus* decl_node;
                            Arbor2Token* base_type_tok = NIHIL;
                            Arbor2Token* modifier1_tok = NIHIL;
                            Arbor2Token* modifier2_tok = NIHIL;
                            i32 num_symbols = regula->longitudo;

                            /* Get declarator from valori[0] (pre-built by P12/P11) */
                            decl_node = valori[ZEPHYRUM];

                            /* Create DECLARATIO node */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->pater = NIHIL;
                            valor_novus->datum.declaratio.tok_storage = NIHIL;
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.tok_unsigned = NIHIL;
                            valor_novus->datum.declaratio.tok_signed = NIHIL;
                            valor_novus->datum.declaratio.tok_long = NIHIL;
                            valor_novus->datum.declaratio.tok_long2 = NIHIL;
                            valor_novus->datum.declaratio.tok_short = NIHIL;
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                            valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;

                            /* Parse tokens based on production number */
                            si (num_symbols == 3)
                            {
                                /* 3-symbol productions: modifier type decl */
                                modifier1_tok = lexemata[II];
                                base_type_tok = lexemata[I];
                            }
                            alioquin si (num_symbols == 4)
                            {
                                /* 4-symbol productions: modifier1 modifier2 type decl */
                                modifier1_tok = lexemata[III];
                                modifier2_tok = lexemata[II];
                                base_type_tok = lexemata[I];
                            }

                            /* Set modifier tokens based on production */
                            commutatio (actio->valor)
                            {
                                casus 420: /* unsigned int */
                                    valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                    frange;
                                casus 421: /* signed int */
                                    valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                    frange;
                                casus 422: /* unsigned char */
                                    valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                    frange;
                                casus 423: /* signed char */
                                    valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                    frange;
                                casus 424: /* long int */
                                    valor_novus->datum.declaratio.tok_long = modifier1_tok;
                                    frange;
                                casus 425: /* short int */
                                    valor_novus->datum.declaratio.tok_short = modifier1_tok;
                                    frange;
                                casus 426: /* unsigned long (implicit int) */
                                    valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_long = base_type_tok;
                                    base_type_tok = NIHIL; /* No explicit base type */
                                    frange;
                                casus 427: /* unsigned short (implicit int) */
                                    valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_short = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 428: /* signed long (implicit int) */
                                    valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_long = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 429: /* signed short (implicit int) */
                                    valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_short = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 430: /* unsigned long int */
                                    valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_long = modifier2_tok;
                                    frange;
                                casus 431: /* unsigned short int */
                                    valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_short = modifier2_tok;
                                    frange;
                                casus 432: /* signed long int */
                                    valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_long = modifier2_tok;
                                    frange;
                                casus 433: /* signed short int */
                                    valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_short = modifier2_tok;
                                    frange;
                                casus 434: /* long long (implicit int) */
                                    valor_novus->datum.declaratio.tok_long = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 435: /* unsigned long long */
                                    valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_long = modifier2_tok;
                                    valor_novus->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                casus 436: /* signed long long */
                                    valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                    valor_novus->datum.declaratio.tok_long = modifier2_tok;
                                    valor_novus->datum.declaratio.tok_long2 = base_type_tok;
                                    base_type_tok = NIHIL;
                                    frange;
                                ordinarius:
                                    frange;
                            }

                            /* Set specifier (base type) */
                            si (base_type_tok != NIHIL)
                            {
                                valor_novus->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                                valor_novus->datum.declaratio.specifier->lexema = base_type_tok;
                                valor_novus->datum.declaratio.specifier->pater = valor_novus;
                                valor_novus->datum.declaratio.specifier->datum.folium.valor = base_type_tok->lexema->valor;
                                valor_novus->lexema = base_type_tok;
                            }
                            alioquin
                            {
                                /* No explicit base type (e.g., "unsigned long" = implicit int) */
                                valor_novus->datum.declaratio.specifier = NIHIL;
                                valor_novus->lexema = modifier1_tok;
                            }

                            valor_novus->datum.declaratio.declarator = decl_node;
                            si (decl_node != NIHIL) decl_node->pater = valor_novus;
                        }
                        /* ========== IMPLICIT INT TYPE MODIFIER DECLARATIONS P527-P530 ========== */
                        alioquin si (actio->valor >= 527 && actio->valor <= 530)
                        {
                            /* P527-P530: Implicit int declarations in local context
                             * P527: long declarator      (2 symbols)
                             * P528: short declarator     (2 symbols)
                             * P529: unsigned declarator  (2 symbols)
                             * P530: signed declarator    (2 symbols)
                             *
                             * These handle declarations like:
                             *   long a;      (implicit long int)
                             *   short b;     (implicit short int)
                             *   unsigned c;  (implicit unsigned int)
                             *   signed d;    (implicit signed int)
                             */
                            Arbor2Nodus* decl_node;
                            Arbor2Token* modifier_tok;

                            /* Get declarator from valori[0] (pre-built by P12/P11) */
                            decl_node = valori[ZEPHYRUM];

                            /* Get modifier token from lexemata[I] (position 1 in stack) */
                            modifier_tok = lexemata[I];

                            /* Create DECLARATIO node */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->pater = NIHIL;
                            valor_novus->datum.declaratio.tok_storage = NIHIL;
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.tok_unsigned = NIHIL;
                            valor_novus->datum.declaratio.tok_signed = NIHIL;
                            valor_novus->datum.declaratio.tok_long = NIHIL;
                            valor_novus->datum.declaratio.tok_long2 = NIHIL;
                            valor_novus->datum.declaratio.tok_short = NIHIL;
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                            valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;

                            /* Set modifier token based on production */
                            commutatio (actio->valor)
                            {
                                casus 527: /* long (implicit int) */
                                    valor_novus->datum.declaratio.tok_long = modifier_tok;
                                    frange;
                                casus 528: /* short (implicit int) */
                                    valor_novus->datum.declaratio.tok_short = modifier_tok;
                                    frange;
                                casus 529: /* unsigned (implicit int) */
                                    valor_novus->datum.declaratio.tok_unsigned = modifier_tok;
                                    frange;
                                casus 530: /* signed (implicit int) */
                                    valor_novus->datum.declaratio.tok_signed = modifier_tok;
                                    frange;
                                ordinarius:
                                    frange;
                            }

                            /* No explicit base type (implicit int) */
                            valor_novus->datum.declaratio.specifier = NIHIL;
                            valor_novus->lexema = modifier_tok;

                            valor_novus->datum.declaratio.declarator = decl_node;
                            si (decl_node != NIHIL) decl_node->pater = valor_novus;
                        }
                        /* ========== IMPLICIT INT TYPE MODIFIER DECLARATIONS WITH INITIALIZERS P531-P534 ========== */
                        alioquin si (actio->valor >= 531 && actio->valor <= 534)
                        {
                            /* P531-P534: Implicit int declarations with initializers
                             * P531: long declarator '=' assignatio   (4 symbols)
                             * P532: short declarator '=' assignatio  (4 symbols)
                             * P533: unsigned declarator '=' assignatio (4 symbols)
                             * P534: signed declarator '=' assignatio (4 symbols)
                             *
                             * Stack layout (index 0 = rightmost):
                             *   [3] = modifier token ('long'/'short'/etc)
                             *   [2] = declarator (AST node from P12)
                             *   [1] = '=' token (no AST)
                             *   [0] = assignatio (expression AST)
                             *
                             * valori[2] = declarator AST
                             * valori[0] = assignatio AST
                             * lexemata[3] = modifier token
                             * lexemata[1] = '=' token
                             */
                            Arbor2Nodus* decl_node;
                            Arbor2Nodus* init_expr;
                            Arbor2Token* modifier_tok;
                            Arbor2Token* assign_tok;

                            /* Get declarator from valori[2] (pre-built by P12/P11) */
                            decl_node = valori[II];

                            /* Get initializer expression from valori[0] */
                            init_expr = valori[ZEPHYRUM];

                            /* Get modifier token from lexemata[III] and '=' from lexemata[I] */
                            modifier_tok = lexemata[III];
                            assign_tok = lexemata[I];

                            /* Create DECLARATIO node */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->pater = NIHIL;
                            valor_novus->datum.declaratio.tok_storage = NIHIL;
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.tok_unsigned = NIHIL;
                            valor_novus->datum.declaratio.tok_signed = NIHIL;
                            valor_novus->datum.declaratio.tok_long = NIHIL;
                            valor_novus->datum.declaratio.tok_long2 = NIHIL;
                            valor_novus->datum.declaratio.tok_short = NIHIL;
                            valor_novus->datum.declaratio.tok_assignatio = assign_tok;
                            valor_novus->datum.declaratio.initializor = init_expr;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                            valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;

                            /* Set modifier token based on production */
                            commutatio (actio->valor)
                            {
                                casus 531: /* long (implicit int) */
                                    valor_novus->datum.declaratio.tok_long = modifier_tok;
                                    frange;
                                casus 532: /* short (implicit int) */
                                    valor_novus->datum.declaratio.tok_short = modifier_tok;
                                    frange;
                                casus 533: /* unsigned (implicit int) */
                                    valor_novus->datum.declaratio.tok_unsigned = modifier_tok;
                                    frange;
                                casus 534: /* signed (implicit int) */
                                    valor_novus->datum.declaratio.tok_signed = modifier_tok;
                                    frange;
                                ordinarius:
                                    frange;
                            }

                            /* No explicit base type (implicit int) */
                            valor_novus->datum.declaratio.specifier = NIHIL;
                            valor_novus->lexema = modifier_tok;

                            valor_novus->datum.declaratio.declarator = decl_node;
                            si (decl_node != NIHIL) decl_node->pater = valor_novus;
                            si (init_expr != NIHIL) init_expr->pater = valor_novus;
                        }
                        /* ========== MULTI-ID TYPE SPECIFIER CHAINS P535-P538 ========== */
                        alioquin si (actio->valor == 535)
                        {
                            /* P535: type_spec_list -> ID (1 symbol)
                             * Start a new type_spec_list with first identifier token.
                             * Value is a Xar of Arbor2Token* (not an AST node).
                             */
                            Xar* type_list = xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                            Arbor2Token** slot = xar_addere(type_list);
                            *slot = lexemata[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)type_list;
                        }
                        alioquin si (actio->valor == 536)
                        {
                            /* P536: type_spec_list -> type_spec_list ID (2 symbols)
                             * Append new identifier to existing list.
                             * valori[1] = existing type_list (Xar*)
                             * lexemata[0] = new ID token
                             */
                            Xar* type_list = (Xar*)valori[I];
                            si (type_list == NIHIL)
                            {
                                valor_novus = NIHIL;
                            }
                            alioquin
                            {
                                Arbor2Token** slot = xar_addere(type_list);
                                *slot = lexemata[ZEPHYRUM];
                                valor_novus = (Arbor2Nodus*)type_list;
                            }
                        }
                        alioquin si (actio->valor == 537)
                        {
                            /* P537: declaratio -> type_spec_list init_decl_list (2 symbols)
                             * Build declaration from accumulated type tokens and declarator list.
                             * valori[1] = type_spec_list (Xar of Arbor2Token*)
                             * valori[0] = init_decl_list (Xar of pairs)
                             *
                             * Last token in type_list is the base type specifier.
                             * First tokens become storage/qualifiers (stored in extra_specifiers).
                             */
                            Xar* type_list = (Xar*)valori[I];
                            Xar* init_list = (Xar*)valori[ZEPHYRUM];
                            i32 num_types;
                            Arbor2Token* base_type_tok;
                            Arbor2Nodus* type_spec;
                            Arbor2Nodus* primus = NIHIL;
                            Arbor2Nodus* ultimus = NIHIL;
                            i32 num_items;
                            i32 i;

                            si (type_list == NIHIL)
                            {
                                valor_novus = NIHIL;
                            }
                            alioquin
                            {
                                num_types = xar_numerus(type_list);

                                si (num_types <= ZEPHYRUM)
                                {
                                    valor_novus = NIHIL;
                                }
                                alioquin
                                {
                                /* Last token is the base type */
                                base_type_tok = *(Arbor2Token**)xar_obtinere(type_list, num_types - I);

                            /* Create type_spec node */
                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = base_type_tok;
                            type_spec->datum.folium.valor = base_type_tok->lexema->valor;

                            si (init_list != NIHIL)
                            {
                                num_items = xar_numerus(init_list);
                                per (i = ZEPHYRUM; i < num_items; i++)
                                {
                                    Xar** pair_ptr = xar_obtinere(init_list, i);
                                    Xar* pair = *pair_ptr;
                                    Arbor2Nodus* decl_node = NIHIL;
                                    Arbor2Token* tok_assign = NIHIL;
                                    Arbor2Nodus* init_node = NIHIL;
                                    Arbor2Token* tok_comma = NIHIL;
                                    Arbor2Nodus* nodus;

                                    si (pair != NIHIL && xar_numerus(pair) >= III)
                                    {
                                        decl_node = *(Arbor2Nodus**)xar_obtinere(pair, ZEPHYRUM);
                                        tok_assign = *(Arbor2Token**)xar_obtinere(pair, I);
                                        init_node = *(Arbor2Nodus**)xar_obtinere(pair, II);
                                    }
                                    si (pair != NIHIL && xar_numerus(pair) >= IV)
                                    {
                                        tok_comma = *(Arbor2Token**)xar_obtinere(pair, III);
                                    }

                                    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                    nodus->genus = ARBOR2_NODUS_DECLARATIO;
                                    nodus->lexema = base_type_tok;

                                    /* First token as tok_storage (if more than 1 type token) */
                                    si (num_types > I)
                                    {
                                        nodus->datum.declaratio.tok_storage = *(Arbor2Token**)xar_obtinere(type_list, ZEPHYRUM);
                                    }
                                    alioquin
                                    {
                                        nodus->datum.declaratio.tok_storage = NIHIL;
                                    }

                                    /* Middle tokens as extra_specifiers (if more than 2 type tokens) */
                                    si (num_types > II)
                                    {
                                        i32 j;
                                        nodus->datum.declaratio.extra_specifiers = xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                                        per (j = I; j < num_types - I; j++)
                                        {
                                            Arbor2Token** extra_slot = xar_addere(nodus->datum.declaratio.extra_specifiers);
                                            *extra_slot = *(Arbor2Token**)xar_obtinere(type_list, j);
                                        }
                                    }
                                    alioquin
                                    {
                                        nodus->datum.declaratio.extra_specifiers = NIHIL;
                                    }

                                    nodus->datum.declaratio.tok_const = NIHIL;
                                    nodus->datum.declaratio.tok_volatile = NIHIL;
                                    nodus->datum.declaratio.tok_unsigned = NIHIL;
                                    nodus->datum.declaratio.tok_signed = NIHIL;
                                    nodus->datum.declaratio.tok_long = NIHIL;
                                    nodus->datum.declaratio.tok_long2 = NIHIL;
                                    nodus->datum.declaratio.tok_short = NIHIL;
                                    nodus->datum.declaratio.specifier = type_spec;
                                    nodus->datum.declaratio.declarator = decl_node;
                                    nodus->datum.declaratio.tok_assignatio = tok_assign;
                                    nodus->datum.declaratio.initializor = init_node;
                                    nodus->datum.declaratio.tok_semicolon = NIHIL;
                                    nodus->datum.declaratio.tok_comma = tok_comma;
                                    nodus->datum.declaratio.proxima = NIHIL;
                                    nodus->datum.declaratio.est_typedef = FALSUM;
                                    nodus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                                    nodus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;

                                    si (decl_node != NIHIL) decl_node->pater = nodus;
                                    si (init_node != NIHIL) init_node->pater = nodus;

                                    si (primus == NIHIL)
                                    {
                                        primus = nodus;
                                        ultimus = nodus;
                                    }
                                    alioquin
                                    {
                                        ultimus->datum.declaratio.proxima = nodus;
                                        ultimus = nodus;
                                    }
                                }
                            }
                            valor_novus = primus;
                            }  /* end alioquin for num_types > 0 */
                            }  /* end alioquin for type_list != NIHIL */
                        }
                        alioquin si (actio->valor == 538)
                        {
                            /* P538: func_def -> type_spec_list declarator compound (3 symbols)
                             * valori[2] = type_spec_list (Xar of Arbor2Token*)
                             * valori[1] = declarator (DECLARATOR_FUNCTI node)
                             * valori[0] = compound (CORPUS node)
                             */
                            Xar* type_list = (Xar*)valori[II];
                            Arbor2Nodus* decl_node = valori[I];
                            Arbor2Nodus* corpus_node = valori[ZEPHYRUM];
                            i32 num_types = xar_numerus(type_list);
                            Arbor2Token* base_type_tok;
                            Arbor2Nodus* type_spec;

                            /* Last token is the base type */
                            base_type_tok = *(Arbor2Token**)xar_obtinere(type_list, num_types - I);

                            /* Create type_spec node */
                            type_spec = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            type_spec->genus = ARBOR2_NODUS_IDENTIFICATOR;
                            type_spec->lexema = base_type_tok;
                            type_spec->datum.folium.valor = base_type_tok->lexema->valor;

                            /* Create function definition node */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DEFINITIO_FUNCTI;
                            valor_novus->lexema = base_type_tok;
                            valor_novus->datum.definitio_functi.specifier = type_spec;
                            valor_novus->datum.definitio_functi.declarator = decl_node;
                            valor_novus->datum.definitio_functi.corpus = corpus_node;
                            valor_novus->datum.definitio_functi.tok_const = NIHIL;
                            valor_novus->datum.definitio_functi.tok_volatile = NIHIL;
                            valor_novus->datum.definitio_functi.tok_unsigned = NIHIL;
                            valor_novus->datum.definitio_functi.tok_signed = NIHIL;
                            valor_novus->datum.definitio_functi.tok_long = NIHIL;
                            valor_novus->datum.definitio_functi.tok_long2 = NIHIL;
                            valor_novus->datum.definitio_functi.tok_short = NIHIL;

                            si (decl_node != NIHIL) decl_node->pater = valor_novus;
                            si (corpus_node != NIHIL) corpus_node->pater = valor_novus;
                        }
                        /* ========== QUALIFIER + COMPOUND TYPE SPECIFIERS P437-P504 ========== */
                        alioquin si (actio->valor >= 437 && actio->valor <= 504)
                        {
                            /* P437-P504: Qualifier + Compound type specifiers at top-level
                             *
                             * P437-P453: const + compound types (4-5 symbols)
                             * P454-P470: volatile + compound types (4-5 symbols)
                             * P471-P487: const volatile + compound types (5-6 symbols)
                             * P488-P504: volatile const + compound types (5-6 symbols)
                             *
                             * Token layout depends on production number and symbol count.
                             */
                            Arbor2Nodus* decl_node;
                            Arbor2Token* base_type_tok = NIHIL;
                            Arbor2Token* modifier1_tok = NIHIL;
                            Arbor2Token* modifier2_tok = NIHIL;
                            Arbor2Token* const_tok = NIHIL;
                            Arbor2Token* volatile_tok = NIHIL;
                            i32 prod = actio->valor;
                            i32 num_symbols = regula->longitudo;

                            /* Get declarator from valori[0] (pre-built by P12/P11) */
                            decl_node = valori[ZEPHYRUM];

                            /* Create DECLARATIO node */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATIO;
                            valor_novus->pater = NIHIL;
                            valor_novus->datum.declaratio.tok_storage = NIHIL;
                            valor_novus->datum.declaratio.tok_const = NIHIL;
                            valor_novus->datum.declaratio.tok_volatile = NIHIL;
                            valor_novus->datum.declaratio.tok_unsigned = NIHIL;
                            valor_novus->datum.declaratio.tok_signed = NIHIL;
                            valor_novus->datum.declaratio.tok_long = NIHIL;
                            valor_novus->datum.declaratio.tok_long2 = NIHIL;
                            valor_novus->datum.declaratio.tok_short = NIHIL;
                            valor_novus->datum.declaratio.tok_assignatio = NIHIL;
                            valor_novus->datum.declaratio.initializor = NIHIL;
                            valor_novus->datum.declaratio.tok_semicolon = NIHIL;
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
                            valor_novus->datum.declaratio.proxima = NIHIL;
                            valor_novus->datum.declaratio.est_typedef = FALSUM;
                            valor_novus->datum.declaratio.storage_class = ARBOR2_STORAGE_NONE;
                            valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_NONE;

                            /* Determine qualifier group and extract tokens */
                            si (prod <= 453)
                            {
                                /* P437-P453: const + compound (4-5 symbols) */
                                /* const is at position num_symbols-1 (rightmost in stack) */
                                const_tok = lexemata[num_symbols - 1];
                                valor_novus->datum.declaratio.tok_const = const_tok;
                                valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_CONST;

                                si (num_symbols == 4)
                                {
                                    /* const modifier type decl */
                                    modifier1_tok = lexemata[II];
                                    base_type_tok = lexemata[I];
                                }
                                alioquin si (num_symbols == 5)
                                {
                                    /* const modifier1 modifier2 type decl */
                                    modifier1_tok = lexemata[III];
                                    modifier2_tok = lexemata[II];
                                    base_type_tok = lexemata[I];
                                }
                            }
                            alioquin si (prod <= 470)
                            {
                                /* P454-P470: volatile + compound (4-5 symbols) */
                                volatile_tok = lexemata[num_symbols - 1];
                                valor_novus->datum.declaratio.tok_volatile = volatile_tok;
                                valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_VOLATILE;

                                si (num_symbols == 4)
                                {
                                    modifier1_tok = lexemata[II];
                                    base_type_tok = lexemata[I];
                                }
                                alioquin si (num_symbols == 5)
                                {
                                    modifier1_tok = lexemata[III];
                                    modifier2_tok = lexemata[II];
                                    base_type_tok = lexemata[I];
                                }
                            }
                            alioquin si (prod <= 487)
                            {
                                /* P471-P487: const volatile + compound (5-6 symbols) */
                                /* const at position num_symbols-1, volatile at num_symbols-2 */
                                const_tok = lexemata[num_symbols - 1];
                                volatile_tok = lexemata[num_symbols - 2];
                                valor_novus->datum.declaratio.tok_const = const_tok;
                                valor_novus->datum.declaratio.tok_volatile = volatile_tok;
                                valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_CONST | ARBOR2_QUAL_VOLATILE;

                                si (num_symbols == 5)
                                {
                                    modifier1_tok = lexemata[II];
                                    base_type_tok = lexemata[I];
                                }
                                alioquin si (num_symbols == 6)
                                {
                                    modifier1_tok = lexemata[III];
                                    modifier2_tok = lexemata[II];
                                    base_type_tok = lexemata[I];
                                }
                            }
                            alioquin
                            {
                                /* P488-P504: volatile const + compound (5-6 symbols) */
                                /* volatile at position num_symbols-1, const at num_symbols-2 */
                                volatile_tok = lexemata[num_symbols - 1];
                                const_tok = lexemata[num_symbols - 2];
                                valor_novus->datum.declaratio.tok_volatile = volatile_tok;
                                valor_novus->datum.declaratio.tok_const = const_tok;
                                valor_novus->datum.declaratio.qualifiers = ARBOR2_QUAL_CONST | ARBOR2_QUAL_VOLATILE;

                                si (num_symbols == 5)
                                {
                                    modifier1_tok = lexemata[II];
                                    base_type_tok = lexemata[I];
                                }
                                alioquin si (num_symbols == 6)
                                {
                                    modifier1_tok = lexemata[III];
                                    modifier2_tok = lexemata[II];
                                    base_type_tok = lexemata[I];
                                }
                            }

                            /* Set modifier tokens based on production within each group */
                            /* Group offset: 437, 454, 471, 488 */
                            {
                                i32 group_offset;
                                si (prod <= 453) group_offset = prod - 437;
                                alioquin si (prod <= 470) group_offset = prod - 454;
                                alioquin si (prod <= 487) group_offset = prod - 471;
                                alioquin group_offset = prod - 488;

                                /* Each group follows same pattern as P420-P436:
                                 * 0: unsigned int (3 tokens after qual)
                                 * 1: signed int
                                 * 2: unsigned char
                                 * 3: signed char
                                 * 4: long int
                                 * 5: short int
                                 * 6: unsigned long (implicit int)
                                 * 7: unsigned short (implicit int)
                                 * 8: signed long (implicit int)
                                 * 9: signed short (implicit int)
                                 * 10: unsigned long int (4 tokens after qual)
                                 * 11: unsigned short int
                                 * 12: signed long int
                                 * 13: signed short int
                                 * 14: long long (implicit int)
                                 * 15: unsigned long long
                                 * 16: signed long long
                                 */
                                commutatio (group_offset)
                                {
                                    casus 0: /* unsigned int */
                                        valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                        frange;
                                    casus 1: /* signed int */
                                        valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                        frange;
                                    casus 2: /* unsigned char */
                                        valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                        frange;
                                    casus 3: /* signed char */
                                        valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                        frange;
                                    casus 4: /* long int */
                                        valor_novus->datum.declaratio.tok_long = modifier1_tok;
                                        frange;
                                    casus 5: /* short int */
                                        valor_novus->datum.declaratio.tok_short = modifier1_tok;
                                        frange;
                                    casus 6: /* unsigned long (implicit int) */
                                        valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_long = base_type_tok;
                                        base_type_tok = NIHIL;
                                        frange;
                                    casus 7: /* unsigned short (implicit int) */
                                        valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_short = base_type_tok;
                                        base_type_tok = NIHIL;
                                        frange;
                                    casus 8: /* signed long (implicit int) */
                                        valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_long = base_type_tok;
                                        base_type_tok = NIHIL;
                                        frange;
                                    casus 9: /* signed short (implicit int) */
                                        valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_short = base_type_tok;
                                        base_type_tok = NIHIL;
                                        frange;
                                    casus 10: /* unsigned long int */
                                        valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_long = modifier2_tok;
                                        frange;
                                    casus 11: /* unsigned short int */
                                        valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_short = modifier2_tok;
                                        frange;
                                    casus 12: /* signed long int */
                                        valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_long = modifier2_tok;
                                        frange;
                                    casus 13: /* signed short int */
                                        valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_short = modifier2_tok;
                                        frange;
                                    casus 14: /* long long (implicit int) */
                                        valor_novus->datum.declaratio.tok_long = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_long2 = base_type_tok;
                                        base_type_tok = NIHIL;
                                        frange;
                                    casus 15: /* unsigned long long */
                                        valor_novus->datum.declaratio.tok_unsigned = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_long = modifier2_tok;
                                        valor_novus->datum.declaratio.tok_long2 = base_type_tok;
                                        base_type_tok = NIHIL;
                                        frange;
                                    casus 16: /* signed long long */
                                        valor_novus->datum.declaratio.tok_signed = modifier1_tok;
                                        valor_novus->datum.declaratio.tok_long = modifier2_tok;
                                        valor_novus->datum.declaratio.tok_long2 = base_type_tok;
                                        base_type_tok = NIHIL;
                                        frange;
                                    ordinarius:
                                        frange;
                                }
                            }

                            /* Set specifier (base type) */
                            si (base_type_tok != NIHIL)
                            {
                                valor_novus->datum.declaratio.specifier = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->datum.declaratio.specifier->genus = ARBOR2_NODUS_IDENTIFICATOR;
                                valor_novus->datum.declaratio.specifier->lexema = base_type_tok;
                                valor_novus->datum.declaratio.specifier->pater = valor_novus;
                                valor_novus->datum.declaratio.specifier->datum.folium.valor = base_type_tok->lexema->valor;
                                valor_novus->lexema = base_type_tok;
                            }
                            alioquin
                            {
                                /* No explicit base type (e.g., "const unsigned long" = implicit int) */
                                valor_novus->datum.declaratio.specifier = NIHIL;
                                valor_novus->lexema = modifier1_tok;
                            }

                            valor_novus->datum.declaratio.declarator = decl_node;
                            si (decl_node != NIHIL) decl_node->pater = valor_novus;
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
                            valor_novus->datum.declaratio.tok_comma = NIHIL;
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
                            /* lexemata: [3]=last_decl, [2]='[', [1]=last_expr, [0]=']' */
                            Arbor2Nodus* inner = valori[III];
                            Arbor2Nodus* size_expr = valori[I];
                            Arbor2ArrayDimension* dim;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                            valor_novus->lexema = inner->lexema;

                            /* Copy base declarator fields */
                            valor_novus->datum.declarator.pointer_levels = inner->datum.declarator.pointer_levels;
                            valor_novus->datum.declarator.titulus = inner->datum.declarator.titulus;
                            valor_novus->datum.declarator.latitudo_biti = inner->datum.declarator.latitudo_biti;

                            /* Create or extend dimensions array */
                            si (inner->datum.declarator.dimensiones != NIHIL)
                            {
                                valor_novus->datum.declarator.dimensiones = inner->datum.declarator.dimensiones;
                            }
                            alioquin
                            {
                                valor_novus->datum.declarator.dimensiones = xar_creare(glr->piscina, magnitudo(Arbor2ArrayDimension*));
                            }

                            /* Create and add this dimension with bracket tokens */
                            dim = piscina_allocare(glr->piscina, magnitudo(Arbor2ArrayDimension));
                            dim->tok_bracket_ap = lexemata[II];
                            dim->dimensio = size_expr;
                            dim->tok_bracket_cl = lexemata[ZEPHYRUM];
                            {
                                Arbor2ArrayDimension** slot = xar_addere(valor_novus->datum.declarator.dimensiones);
                                *slot = dim;
                            }
                        }
                        alioquin si (actio->valor == 81)
                        {
                            /* P81: declarator '[' ']' (3 symbols, unsized array) */
                            /* valori: [2]=declarator, [1]='[', [0]=']' */
                            /* lexemata: [2]=last_decl, [1]='[', [0]=']' */
                            Arbor2Nodus* inner = valori[II];
                            Arbor2ArrayDimension* dim;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                            valor_novus->lexema = inner->lexema;

                            /* Copy base declarator fields */
                            valor_novus->datum.declarator.pointer_levels = inner->datum.declarator.pointer_levels;
                            valor_novus->datum.declarator.titulus = inner->datum.declarator.titulus;
                            valor_novus->datum.declarator.latitudo_biti = inner->datum.declarator.latitudo_biti;

                            /* Create or extend dimensions array */
                            si (inner->datum.declarator.dimensiones != NIHIL)
                            {
                                valor_novus->datum.declarator.dimensiones = inner->datum.declarator.dimensiones;
                            }
                            alioquin
                            {
                                valor_novus->datum.declarator.dimensiones = xar_creare(glr->piscina, magnitudo(Arbor2ArrayDimension*));
                            }

                            /* Create and add dimension with NIHIL size for unsized array */
                            dim = piscina_allocare(glr->piscina, magnitudo(Arbor2ArrayDimension));
                            dim->tok_bracket_ap = lexemata[I];
                            dim->dimensio = NIHIL;
                            dim->tok_bracket_cl = lexemata[ZEPHYRUM];
                            {
                                Arbor2ArrayDimension** slot = xar_addere(valor_novus->datum.declarator.dimensiones);
                                *slot = dim;
                            }
                        }
                        /* ========== GROUPED POINTER DECLARATOR P339 ========== */
                        alioquin si (actio->valor == 339)
                        {
                            /* P339: declarator -> '(' '*' declarator ')' (4 symbols)
                             * For function pointers: int (*fp)(void)
                             * valori: [3]='(', [2]='*', [1]=declarator, [0]=')'
                             * Increment pointer count and return the inner declarator */
                            Arbor2Nodus* inner = valori[I];  /* The inner declarator */

                            si (inner != NIHIL && inner->genus == ARBOR2_NODUS_DECLARATOR)
                            {
                                Arbor2PointerLevel* lvl;
                                Arbor2PointerLevel** lvl_slot;

                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                                valor_novus->lexema = lexemata[II];  /* The '*' token */
                                valor_novus->datum.declarator.titulus =
                                    inner->datum.declarator.titulus;
                                valor_novus->datum.declarator.latitudo_biti =
                                    inner->datum.declarator.latitudo_biti;
                                valor_novus->datum.declarator.dimensiones =
                                    inner->datum.declarator.dimensiones;

                                /* Copy existing pointer_levels and add new level */
                                valor_novus->datum.declarator.pointer_levels =
                                    xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                si (inner->datum.declarator.pointer_levels != NIHIL)
                                {
                                    i32 i;
                                    i32 n = xar_numerus(inner->datum.declarator.pointer_levels);
                                    per (i = ZEPHYRUM; i < n; i++)
                                    {
                                        Arbor2PointerLevel** src = xar_obtinere(inner->datum.declarator.pointer_levels, i);
                                        Arbor2PointerLevel** dst = xar_addere(valor_novus->datum.declarator.pointer_levels);
                                        *dst = *src;
                                    }
                                }
                                /* Add new pointer level */
                                lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                                lvl->tok_stella = lexemata[II];
                                lvl->tok_const = NIHIL;
                                lvl->tok_volatile = NIHIL;
                                lvl_slot = xar_addere(valor_novus->datum.declarator.pointer_levels);
                                *lvl_slot = lvl;
                            }
                            alioquin
                            {
                                /* Fallback: create basic declarator with one star */
                                Arbor2PointerLevel* lvl;
                                Arbor2PointerLevel** lvl_slot;

                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                                valor_novus->lexema = lexemata[II];
                                valor_novus->datum.declarator.titulus.datum = NIHIL;
                                valor_novus->datum.declarator.titulus.mensura = ZEPHYRUM;
                                valor_novus->datum.declarator.latitudo_biti = NIHIL;
                                valor_novus->datum.declarator.dimensiones = NIHIL;

                                /* Create pointer_levels with one level */
                                valor_novus->datum.declarator.pointer_levels =
                                    xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                                lvl->tok_stella = lexemata[II];
                                lvl->tok_const = NIHIL;
                                lvl->tok_volatile = NIHIL;
                                lvl_slot = xar_addere(valor_novus->datum.declarator.pointer_levels);
                                *lvl_slot = lvl;
                            }
                        }
                        /* ========== VARIADIC PARAMS P341 ========== */
                        alioquin si (actio->valor == 341)
                        {
                            /* P341: params -> params , ... (3 symbols)
                             * Variadic parameter list
                             * valori: [0]=param_list, [1]=comma, [2]=ellipsis */
                            Arbor2Nodus* params = valori[ZEPHYRUM];

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_PARAMETER_LIST;
                            valor_novus->lexema = NIHIL;

                            /* Copy parameters from existing list and mark variadic */
                            si (params->genus == ARBOR2_NODUS_PARAMETER_LIST)
                            {
                                valor_novus->datum.parameter_list.parametra = params->datum.parameter_list.parametra;
                            }
                            alioquin
                            {
                                /* Should be a PARAMETER_LIST at this point, but handle gracefully */
                                valor_novus->datum.parameter_list.parametra = xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                                *(Arbor2Nodus**)xar_addere(valor_novus->datum.parameter_list.parametra) = params;
                            }
                            valor_novus->datum.parameter_list.est_variadicus = VERUM;
                        }
                        /* ========== POINTER QUALIFIERS P252-P255 ========== */
                        alioquin si (actio->valor == 252 || actio->valor == 253)
                        {
                            /* P252: declarator -> '*' 'const' declarator (3 symbols)
                             * P253: declarator -> '*' 'volatile' declarator (3 symbols)
                             * valori: [2]='*', [1]=qualifier, [0]=declarator */
                            Arbor2Nodus* inner = valori[ZEPHYRUM];
                            Arbor2PointerLevel* lvl;
                            Arbor2PointerLevel** lvl_slot;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                            /* Copy inner declarator's lexema (identifier) for serialization */
                            valor_novus->lexema = (inner != NIHIL) ? inner->lexema : lexemata[II];

                            si (inner != NIHIL && inner->genus == ARBOR2_NODUS_DECLARATOR)
                            {
                                valor_novus->datum.declarator.titulus =
                                    inner->datum.declarator.titulus;
                                valor_novus->datum.declarator.latitudo_biti =
                                    inner->datum.declarator.latitudo_biti;
                                valor_novus->datum.declarator.dimensiones =
                                    inner->datum.declarator.dimensiones;

                                /* Copy inner pointer levels and add new level */
                                valor_novus->datum.declarator.pointer_levels =
                                    xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                si (inner->datum.declarator.pointer_levels != NIHIL)
                                {
                                    i32 i;
                                    i32 n = xar_numerus(inner->datum.declarator.pointer_levels);
                                    per (i = ZEPHYRUM; i < n; i++)
                                    {
                                        Arbor2PointerLevel** src = xar_obtinere(inner->datum.declarator.pointer_levels, i);
                                        Arbor2PointerLevel** dst = xar_addere(valor_novus->datum.declarator.pointer_levels);
                                        *dst = *src;
                                    }
                                }
                            }
                            alioquin
                            {
                                valor_novus->datum.declarator.titulus.datum = NIHIL;
                                valor_novus->datum.declarator.titulus.mensura = ZEPHYRUM;
                                valor_novus->datum.declarator.latitudo_biti = NIHIL;
                                valor_novus->datum.declarator.dimensiones = NIHIL;
                                valor_novus->datum.declarator.pointer_levels =
                                    xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                            }

                            /* Add new pointer level with qualifier */
                            lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                            lvl->tok_stella = lexemata[II];
                            lvl->tok_const = (actio->valor == 252) ? lexemata[I] : NIHIL;
                            lvl->tok_volatile = (actio->valor == 253) ? lexemata[I] : NIHIL;
                            lvl_slot = xar_addere(valor_novus->datum.declarator.pointer_levels);
                            *lvl_slot = lvl;
                        }
                        alioquin si (actio->valor == 254 || actio->valor == 255)
                        {
                            /* P254: declarator -> '*' 'const' 'volatile' declarator (4 symbols)
                             * P255: declarator -> '*' 'volatile' 'const' declarator (4 symbols)
                             * valori: [3]='*', [2]=qual1, [1]=qual2, [0]=declarator */
                            Arbor2Nodus* inner = valori[ZEPHYRUM];
                            Arbor2PointerLevel* lvl;
                            Arbor2PointerLevel** lvl_slot;

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                            /* Copy inner declarator's lexema (identifier) for serialization */
                            valor_novus->lexema = (inner != NIHIL) ? inner->lexema : lexemata[III];

                            si (inner != NIHIL && inner->genus == ARBOR2_NODUS_DECLARATOR)
                            {
                                valor_novus->datum.declarator.titulus =
                                    inner->datum.declarator.titulus;
                                valor_novus->datum.declarator.latitudo_biti =
                                    inner->datum.declarator.latitudo_biti;
                                valor_novus->datum.declarator.dimensiones =
                                    inner->datum.declarator.dimensiones;

                                /* Copy inner pointer levels and add new level */
                                valor_novus->datum.declarator.pointer_levels =
                                    xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                si (inner->datum.declarator.pointer_levels != NIHIL)
                                {
                                    i32 i;
                                    i32 n = xar_numerus(inner->datum.declarator.pointer_levels);
                                    per (i = ZEPHYRUM; i < n; i++)
                                    {
                                        Arbor2PointerLevel** src = xar_obtinere(inner->datum.declarator.pointer_levels, i);
                                        Arbor2PointerLevel** dst = xar_addere(valor_novus->datum.declarator.pointer_levels);
                                        *dst = *src;
                                    }
                                }
                            }
                            alioquin
                            {
                                valor_novus->datum.declarator.titulus.datum = NIHIL;
                                valor_novus->datum.declarator.titulus.mensura = ZEPHYRUM;
                                valor_novus->datum.declarator.latitudo_biti = NIHIL;
                                valor_novus->datum.declarator.dimensiones = NIHIL;
                                valor_novus->datum.declarator.pointer_levels =
                                    xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                            }

                            /* Add new pointer level with both qualifiers
                             * P254: const volatile -> const at [2], volatile at [1]
                             * P255: volatile const -> volatile at [2], const at [1] */
                            lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                            lvl->tok_stella = lexemata[III];
                            si (actio->valor == 254)
                            {
                                lvl->tok_const = lexemata[II];
                                lvl->tok_volatile = lexemata[I];
                            }
                            alioquin
                            {
                                lvl->tok_volatile = lexemata[II];
                                lvl->tok_const = lexemata[I];
                            }
                            lvl_slot = xar_addere(valor_novus->datum.declarator.pointer_levels);
                            *lvl_slot = lvl;
                        }
                        alioquin si (num_pop == II)
                        {
                            /* P11: declarator -> '*' declarator */
                            /* Si interior est DECLARATOR_FUNCTI, preservare genus functi */
                            si (valori[ZEPHYRUM] != NIHIL &&
                                valori[ZEPHYRUM]->genus == ARBOR2_NODUS_DECLARATOR_FUNCTI)
                            {
                                /* Wrap function declarator with pointer - preservare FUNCTI */
                                Arbor2PointerLevel* lvl;
                                Arbor2PointerLevel** lvl_slot;
                                Arbor2Nodus* inner_functi = valori[ZEPHYRUM];

                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_DECLARATOR_FUNCTI;
                                /* Preserve inner declarator's lexema (the identifier token) */
                                valor_novus->lexema = inner_functi->lexema;
                                valor_novus->datum.declarator_functi.declarator_interior =
                                    inner_functi->datum.declarator_functi.declarator_interior;
                                valor_novus->datum.declarator_functi.tok_paren_ap =
                                    inner_functi->datum.declarator_functi.tok_paren_ap;
                                valor_novus->datum.declarator_functi.parametri =
                                    inner_functi->datum.declarator_functi.parametri;
                                valor_novus->datum.declarator_functi.tok_paren_cl =
                                    inner_functi->datum.declarator_functi.tok_paren_cl;
                                valor_novus->datum.declarator_functi.habet_void =
                                    inner_functi->datum.declarator_functi.habet_void;
                                valor_novus->datum.declarator_functi.est_variadicus =
                                    inner_functi->datum.declarator_functi.est_variadicus;

                                /* Copy and extend pointer_levels */
                                valor_novus->datum.declarator_functi.pointer_levels =
                                    xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                si (inner_functi->datum.declarator_functi.pointer_levels != NIHIL)
                                {
                                    i32 i;
                                    i32 n = xar_numerus(inner_functi->datum.declarator_functi.pointer_levels);
                                    per (i = ZEPHYRUM; i < n; i++)
                                    {
                                        Arbor2PointerLevel** src = xar_obtinere(inner_functi->datum.declarator_functi.pointer_levels, i);
                                        Arbor2PointerLevel** dst = xar_addere(valor_novus->datum.declarator_functi.pointer_levels);
                                        *dst = *src;
                                    }
                                }
                                lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                                lvl->tok_stella = lexemata[I];
                                lvl->tok_const = NIHIL;
                                lvl->tok_volatile = NIHIL;
                                lvl_slot = xar_addere(valor_novus->datum.declarator_functi.pointer_levels);
                                *lvl_slot = lvl;
                            }
                            alioquin
                            {
                                Arbor2PointerLevel* lvl;
                                Arbor2PointerLevel** lvl_slot;
                                Arbor2Nodus* inner = valori[ZEPHYRUM];

                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                                /* Preserve inner declarator's lexema (the identifier token),
                                 * NOT the '*' token which goes in pointer_levels */
                                valor_novus->lexema = (inner != NIHIL) ? inner->lexema : lexemata[I];
                                /* Count stars: inner declarator's count + 1 */
                                si (inner != NIHIL &&
                                    inner->genus == ARBOR2_NODUS_DECLARATOR)
                                {
                                    valor_novus->datum.declarator.titulus =
                                        inner->datum.declarator.titulus;
                                    valor_novus->datum.declarator.latitudo_biti =
                                        inner->datum.declarator.latitudo_biti;
                                    valor_novus->datum.declarator.dimensiones =
                                        inner->datum.declarator.dimensiones;

                                    /* Copy and extend pointer_levels */
                                    valor_novus->datum.declarator.pointer_levels =
                                        xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                    si (inner->datum.declarator.pointer_levels != NIHIL)
                                    {
                                        i32 i;
                                        i32 n = xar_numerus(inner->datum.declarator.pointer_levels);
                                        per (i = ZEPHYRUM; i < n; i++)
                                        {
                                            Arbor2PointerLevel** src = xar_obtinere(inner->datum.declarator.pointer_levels, i);
                                            Arbor2PointerLevel** dst = xar_addere(valor_novus->datum.declarator.pointer_levels);
                                            *dst = *src;
                                        }
                                    }
                                }
                                alioquin
                                {
                                    valor_novus->datum.declarator.titulus.datum = NIHIL;
                                    valor_novus->datum.declarator.titulus.mensura = ZEPHYRUM;
                                    valor_novus->datum.declarator.latitudo_biti = NIHIL;
                                    valor_novus->datum.declarator.dimensiones = NIHIL;
                                    valor_novus->datum.declarator.pointer_levels =
                                        xar_creare(glr->piscina, magnitudo(Arbor2PointerLevel*));
                                }

                                /* Add new pointer level */
                                lvl = piscina_allocare(glr->piscina, magnitudo(Arbor2PointerLevel));
                                lvl->tok_stella = lexemata[I];
                                lvl->tok_const = NIHIL;
                                lvl->tok_volatile = NIHIL;
                                lvl_slot = xar_addere(valor_novus->datum.declarator.pointer_levels);
                                *lvl_slot = lvl;
                            }
                        }
                        alioquin si (num_pop == I)
                        {
                            /* P12: declarator -> IDENTIFIER */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_DECLARATOR;
                            valor_novus->lexema = lexemata[ZEPHYRUM];
                            valor_novus->datum.declarator.pointer_levels = NIHIL;
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
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);
                        si (num_pop == III)
                        {
                            /* P38: declarator () */
                            /* valori[2]=declarator, [1]='(', [0]=')' */
                            valor_novus->lexema = lexemata[II];
                            valor_novus->datum.declarator_functi.declarator_interior = valori[II];
                            valor_novus->datum.declarator_functi.tok_paren_ap = lexemata[I];
                            valor_novus->datum.declarator_functi.parametri = NIHIL;
                            valor_novus->datum.declarator_functi.tok_paren_cl = lexemata[ZEPHYRUM];
                            valor_novus->datum.declarator_functi.tok_void = NIHIL;
                            valor_novus->datum.declarator_functi.habet_void = FALSUM;
                            valor_novus->datum.declarator_functi.est_variadicus = FALSUM;
                            valor_novus->datum.declarator_functi.pointer_levels = NIHIL;
                        }
                        alioquin si (num_pop == IV)
                        {
                            /* P39 or P40: 4 symbols */
                            /* valori[3]=declarator, [2]='(', [1]=void/param_list, [0]=')' */
                            valor_novus->lexema = lexemata[III];
                            valor_novus->datum.declarator_functi.declarator_interior = valori[III];
                            valor_novus->datum.declarator_functi.tok_paren_ap = lexemata[II];
                            valor_novus->datum.declarator_functi.tok_paren_cl = lexemata[ZEPHYRUM];
                            valor_novus->datum.declarator_functi.pointer_levels = NIHIL;
                            valor_novus->datum.declarator_functi.est_variadicus = FALSUM;
                            si (valori[I] == NIHIL)
                            {
                                /* P39: (void) - valori[1] is NULL from VOID token */
                                valor_novus->datum.declarator_functi.parametri = NIHIL;
                                valor_novus->datum.declarator_functi.tok_void = lexemata[I];
                                valor_novus->datum.declarator_functi.habet_void = VERUM;
                            }
                            alioquin
                            {
                                /* P40: (param_list) - valori[1] is LISTA_SEPARATA node */
                                valor_novus->datum.declarator_functi.parametri = valori[I];
                                valor_novus->datum.declarator_functi.tok_void = NIHIL;
                                valor_novus->datum.declarator_functi.habet_void = FALSUM;
                            }
                        }
                        alioquin
                        {
                            valor_novus->lexema = NIHIL;
                            valor_novus->datum.declarator_functi.declarator_interior = NIHIL;
                            valor_novus->datum.declarator_functi.tok_paren_ap = NIHIL;
                            valor_novus->datum.declarator_functi.parametri = NIHIL;
                            valor_novus->datum.declarator_functi.tok_paren_cl = NIHIL;
                            valor_novus->datum.declarator_functi.tok_void = NIHIL;
                            valor_novus->datum.declarator_functi.habet_void = FALSUM;
                            valor_novus->datum.declarator_functi.est_variadicus = FALSUM;
                            valor_novus->datum.declarator_functi.pointer_levels = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_SENTENTIA:
                        /* P13: statement -> expression ';' */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_SENTENTIA;
                        valor_novus->lexema = lexemata[ZEPHYRUM];  /* semicolon */
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);
                        valor_novus->datum.sententia.tok_semicolon = lexemata[ZEPHYRUM];
                        /* Expression is at index 1 (before semicolon) */
                        si (num_pop >= II && valori[I] != NIHIL)
                        {
                            valor_novus->datum.sententia.expressio = valori[I];
                            /* Statuere patrem pro expressio */
                            valori[I]->pater = valor_novus;
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
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMA(valor_novus, ZEPHYRUM);
                        valor_novus->datum.sententia.expressio = NIHIL;
                        valor_novus->datum.sententia.tok_semicolon = lexemata[ZEPHYRUM];
                        frange;

                    casus ARBOR2_NODUS_CORPUS:
                        si (num_pop == ZEPHYRUM)
                        {
                            /* P18: statement_list -> ε (empty) */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_CORPUS;
                            valor_novus->lexema = NIHIL;
                            valor_novus->pater = NIHIL;
                            valor_novus->datum.corpus.tok_brace_ap = NIHIL;
                            valor_novus->datum.corpus.sententiae =
                                xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            valor_novus->datum.corpus.tok_brace_cl = NIHIL;
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
                                /* Statuere patrem pro stmt */
                                si (stmt != NIHIL) stmt->pater = valor_novus;
                            }
                            alioquin
                            {
                                /* Create new list with this statement */
                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_CORPUS;
                                valor_novus->lexema = NIHIL;
                                valor_novus->pater = NIHIL;
                                valor_novus->datum.corpus.tok_brace_ap = NIHIL;
                                valor_novus->datum.corpus.sententiae =
                                    xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                                valor_novus->datum.corpus.tok_brace_cl = NIHIL;
                                slot_stmt = xar_addere(valor_novus->datum.corpus.sententiae);
                                *slot_stmt = stmt;
                                /* Statuere patrem pro stmt */
                                si (stmt != NIHIL) stmt->pater = valor_novus;
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
                                valor_novus->datum.corpus.tok_brace_ap = lexemata[II];
                                valor_novus->datum.corpus.tok_brace_cl = lexemata[ZEPHYRUM];
                            }
                            alioquin
                            {
                                /* Create new CORPUS */
                                valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                                valor_novus->genus = ARBOR2_NODUS_CORPUS;
                                valor_novus->lexema = lexemata[II];
                                valor_novus->pater = NIHIL;
                                valor_novus->datum.corpus.tok_brace_ap = lexemata[II];
                                valor_novus->datum.corpus.sententiae =
                                    xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                                valor_novus->datum.corpus.tok_brace_cl = lexemata[ZEPHYRUM];
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
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);

                        /* Initializare campos commentorum */
                        valor_novus->commenta_ante = NIHIL;
                        valor_novus->commenta_post = NIHIL;

                        si (num_pop == V)
                        {
                            /* P20: if_statement -> 'if' '(' expression ')' statement */
                            /* lexemata[4] = if, [3] = (, [1] = ), [0] = stmt */
                            valor_novus->datum.conditionale.tok_si = lexemata[IV];
                            valor_novus->datum.conditionale.tok_paren_ap = lexemata[III];
                            valor_novus->datum.conditionale.conditio = valori[II];
                            valor_novus->datum.conditionale.tok_paren_cl = lexemata[I];
                            valor_novus->datum.conditionale.consequens = valori[ZEPHYRUM];
                            valor_novus->datum.conditionale.tok_alioquin = NIHIL;
                            valor_novus->datum.conditionale.alternans = NIHIL;

                            /* Statuere patrem pro filiis */
                            si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                            si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;

                            /* Promovere commenta ab '(' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[III],    /* tok_paren_ap */
                                NIHIL,            /* nulla ante */
                                valori[II]);      /* conditio */

                            /* Promovere commenta ab ')' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],      /* tok_paren_cl */
                                valori[II],       /* conditio */
                                valori[ZEPHYRUM]); /* consequens */
                        }
                        alioquin si (num_pop == VII)
                        {
                            /* P21: if_statement -> 'if' '(' expression ')' statement 'else' statement */
                            /* lexemata[6] = if, [5] = (, [3] = ), [1] = else */
                            valor_novus->datum.conditionale.tok_si = lexemata[VI];
                            valor_novus->datum.conditionale.tok_paren_ap = lexemata[V];
                            valor_novus->datum.conditionale.conditio = valori[IV];
                            valor_novus->datum.conditionale.tok_paren_cl = lexemata[III];
                            valor_novus->datum.conditionale.consequens = valori[II];
                            valor_novus->datum.conditionale.tok_alioquin = lexemata[I];
                            valor_novus->datum.conditionale.alternans = valori[ZEPHYRUM];

                            /* Statuere patrem pro filiis */
                            si (valori[IV] != NIHIL) valori[IV]->pater = valor_novus;
                            si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                            si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;

                            /* Promovere commenta ab '(' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[V],      /* tok_paren_ap */
                                NIHIL,            /* nulla ante */
                                valori[IV]);      /* conditio */

                            /* Promovere commenta ab ')' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[III],    /* tok_paren_cl */
                                valori[IV],       /* conditio */
                                valori[II]);      /* consequens */

                            /* Promovere commenta ab 'else' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],      /* tok_alioquin */
                                valori[II],       /* consequens */
                                valori[ZEPHYRUM]); /* alternans */
                        }
                        alioquin
                        {
                            valor_novus->datum.conditionale.tok_si = NIHIL;
                            valor_novus->datum.conditionale.tok_paren_ap = NIHIL;
                            valor_novus->datum.conditionale.conditio = NIHIL;
                            valor_novus->datum.conditionale.tok_paren_cl = NIHIL;
                            valor_novus->datum.conditionale.consequens = NIHIL;
                            valor_novus->datum.conditionale.tok_alioquin = NIHIL;
                            valor_novus->datum.conditionale.alternans = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_DUM:
                        /* P23: while_statement -> 'while' '(' expression ')' statement */
                        /* lexemata[4] = while, [3] = (, [1] = ) */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_DUM;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'while' token */
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);

                        /* Initializare campos commentorum */
                        valor_novus->commenta_ante = NIHIL;
                        valor_novus->commenta_post = NIHIL;

                        si (num_pop == V)
                        {
                            valor_novus->datum.iteratio.tok_fac = NIHIL;
                            valor_novus->datum.iteratio.tok_dum = lexemata[IV];
                            valor_novus->datum.iteratio.tok_paren_ap = lexemata[III];
                            valor_novus->datum.iteratio.conditio = valori[II];
                            valor_novus->datum.iteratio.tok_paren_cl = lexemata[I];
                            valor_novus->datum.iteratio.corpus = valori[ZEPHYRUM];
                            valor_novus->datum.iteratio.tok_semicolon = NIHIL;

                            /* Statuere patrem pro filiis */
                            si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                            si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;

                            /* Promovere commenta ab '(' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[III],    /* tok_paren_ap */
                                NIHIL,            /* nulla ante */
                                valori[II]);      /* conditio */

                            /* Promovere commenta ab ')' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],      /* tok_paren_cl */
                                valori[II],       /* conditio */
                                valori[ZEPHYRUM]); /* corpus */
                        }
                        alioquin
                        {
                            valor_novus->datum.iteratio.tok_fac = NIHIL;
                            valor_novus->datum.iteratio.tok_dum = NIHIL;
                            valor_novus->datum.iteratio.tok_paren_ap = NIHIL;
                            valor_novus->datum.iteratio.conditio = NIHIL;
                            valor_novus->datum.iteratio.tok_paren_cl = NIHIL;
                            valor_novus->datum.iteratio.corpus = NIHIL;
                            valor_novus->datum.iteratio.tok_semicolon = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_FAC:
                        /* P25: do_statement -> 'do' statement 'while' '(' expression ')' ';' */
                        /* lexemata[6] = do, [4] = while, [3] = (, [1] = ), [0] = ; */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_FAC;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'do' token */
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);

                        /* Initializare campos commentorum */
                        valor_novus->commenta_ante = NIHIL;
                        valor_novus->commenta_post = NIHIL;

                        si (num_pop == VII)
                        {
                            valor_novus->datum.iteratio.tok_fac = lexemata[VI];
                            valor_novus->datum.iteratio.corpus = valori[V];
                            valor_novus->datum.iteratio.tok_dum = lexemata[IV];
                            valor_novus->datum.iteratio.tok_paren_ap = lexemata[III];
                            valor_novus->datum.iteratio.conditio = valori[II];
                            valor_novus->datum.iteratio.tok_paren_cl = lexemata[I];
                            valor_novus->datum.iteratio.tok_semicolon = lexemata[ZEPHYRUM];

                            /* Statuere patrem pro filiis */
                            si (valori[V] != NIHIL) valori[V]->pater = valor_novus;
                            si (valori[II] != NIHIL) valori[II]->pater = valor_novus;

                            /* Promovere commenta ab 'while' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[IV],     /* tok_dum */
                                valori[V],        /* corpus */
                                NIHIL);           /* paren_ap sequitur */

                            /* Promovere commenta ab '(' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[III],    /* tok_paren_ap */
                                NIHIL,            /* nulla ante */
                                valori[II]);      /* conditio */

                            /* Promovere commenta ab ')' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],      /* tok_paren_cl */
                                valori[II],       /* conditio */
                                NIHIL);           /* semicolon sequitur */
                        }
                        alioquin
                        {
                            valor_novus->datum.iteratio.tok_fac = NIHIL;
                            valor_novus->datum.iteratio.tok_dum = NIHIL;
                            valor_novus->datum.iteratio.tok_paren_ap = NIHIL;
                            valor_novus->datum.iteratio.conditio = NIHIL;
                            valor_novus->datum.iteratio.tok_paren_cl = NIHIL;
                            valor_novus->datum.iteratio.corpus = NIHIL;
                            valor_novus->datum.iteratio.tok_semicolon = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_PER:
                        /* P27: for_statement -> 'for' '(' expr_opt ';' expr_opt ';' expr_opt ')' stmt */
                        /* lexemata[8] = for, [7] = (, [5] = first ;, [3] = second ;, [1] = ) */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_PER;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'for' token */
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);

                        /* Initializare campos commentorum */
                        valor_novus->commenta_ante = NIHIL;
                        valor_novus->commenta_post = NIHIL;

                        si (num_pop == IX)
                        {
                            valor_novus->datum.circuitus.tok_per = lexemata[VIII];
                            valor_novus->datum.circuitus.tok_paren_ap = lexemata[VII];
                            valor_novus->datum.circuitus.initium = valori[VI];
                            valor_novus->datum.circuitus.tok_semicolon1 = lexemata[V];
                            valor_novus->datum.circuitus.conditio = valori[IV];
                            valor_novus->datum.circuitus.tok_semicolon2 = lexemata[III];
                            valor_novus->datum.circuitus.incrementum = valori[II];
                            valor_novus->datum.circuitus.tok_paren_cl = lexemata[I];
                            valor_novus->datum.circuitus.corpus = valori[ZEPHYRUM];

                            /* Statuere patrem pro filiis */
                            si (valori[VI] != NIHIL) valori[VI]->pater = valor_novus;
                            si (valori[IV] != NIHIL) valori[IV]->pater = valor_novus;
                            si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                            si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;

                            /* Promovere commenta ab '(' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[VII],    /* tok_paren_ap */
                                NIHIL,            /* nulla ante */
                                valori[VI]);      /* initium */

                            /* Promovere commenta ab first ';' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[V],      /* tok_semicolon1 */
                                valori[VI],       /* initium */
                                valori[IV]);      /* conditio */

                            /* Promovere commenta ab second ';' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[III],    /* tok_semicolon2 */
                                valori[IV],       /* conditio */
                                valori[II]);      /* incrementum */

                            /* Promovere commenta ab ')' token */
                            _promovere_commenta_interior(
                                glr->piscina,
                                lexemata[I],      /* tok_paren_cl */
                                valori[II],       /* incrementum */
                                valori[ZEPHYRUM]); /* corpus */
                        }
                        alioquin
                        {
                            valor_novus->datum.circuitus.tok_per = NIHIL;
                            valor_novus->datum.circuitus.tok_paren_ap = NIHIL;
                            valor_novus->datum.circuitus.initium = NIHIL;
                            valor_novus->datum.circuitus.tok_semicolon1 = NIHIL;
                            valor_novus->datum.circuitus.conditio = NIHIL;
                            valor_novus->datum.circuitus.tok_semicolon2 = NIHIL;
                            valor_novus->datum.circuitus.incrementum = NIHIL;
                            valor_novus->datum.circuitus.tok_paren_cl = NIHIL;
                            valor_novus->datum.circuitus.corpus = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_FRANGE:
                        /* P30: statement -> 'break' ';' */
                        /* lexemata[1] = break, [0] = ; */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_FRANGE;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'break' token */
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, I, ZEPHYRUM);
                        valor_novus->datum.frangendum.tok_frange = lexemata[I];
                        valor_novus->datum.frangendum.tok_semicolon = lexemata[ZEPHYRUM];
                        frange;

                    casus ARBOR2_NODUS_PERGE:
                        /* P31: statement -> 'continue' ';' */
                        /* lexemata[1] = continue, [0] = ; */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_PERGE;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'continue' token */
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, I, ZEPHYRUM);
                        valor_novus->datum.pergendum.tok_perge = lexemata[I];
                        valor_novus->datum.pergendum.tok_semicolon = lexemata[ZEPHYRUM];
                        frange;

                    casus ARBOR2_NODUS_REDDE:
                        /* P32: statement -> 'return' expr_opt ';' */
                        /* lexemata[2] = return, [0] = ; */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_REDDE;
                        valor_novus->lexema = lexemata[num_pop - I];  /* 'return' token */
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);
                        valor_novus->datum.reditio.tok_redde = lexemata[II];
                        valor_novus->datum.reditio.valor = valori[I];  /* expr_opt (may be NULL) */
                        valor_novus->datum.reditio.tok_semicolon = lexemata[ZEPHYRUM];

                        /* Statuere patrem pro filio */
                        si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                        frange;

                    casus ARBOR2_NODUS_SALTA:
                        /* P33: statement -> 'goto' IDENTIFIER ';' */
                        /* lexemata[2] = goto, [1] = ID, [0] = ; */
                        {
                            Arbor2Token* id_tok;
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_SALTA;
                            valor_novus->lexema = lexemata[num_pop - I];  /* 'goto' token */
                            valor_novus->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(valor_novus, II, ZEPHYRUM);
                            id_tok = lexemata[I];
                            valor_novus->datum.saltus.tok_salta = lexemata[II];
                            valor_novus->datum.saltus.tok_destinatio = id_tok;
                            valor_novus->datum.saltus.destinatio = id_tok->lexema->valor;
                            valor_novus->datum.saltus.tok_semicolon = lexemata[ZEPHYRUM];
                        }
                        frange;

                    casus ARBOR2_NODUS_TITULATUM:
                        /* P34: statement -> IDENTIFIER ':' statement */
                        /* lexemata[2] = ID, [1] = : */
                        {
                            Arbor2Token* id_tok;
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_TITULATUM;
                            valor_novus->lexema = lexemata[II];  /* IDENTIFIER token */
                            valor_novus->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(valor_novus, II, ZEPHYRUM);
                            id_tok = lexemata[II];
                            valor_novus->datum.titulatum.tok_titulus = id_tok;
                            valor_novus->datum.titulatum.titulus = id_tok->lexema->valor;
                            valor_novus->datum.titulatum.tok_colon = lexemata[I];
                            valor_novus->datum.titulatum.sententia = valori[ZEPHYRUM];

                            /* Statuere patrem pro filio */
                            si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                        }
                        frange;

                    casus ARBOR2_NODUS_COMMUTATIO:
                        /* P35: statement -> 'switch' '(' expression ')' statement */
                        /* lexemata[4] = switch, [3] = (, [1] = ) */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_COMMUTATIO;
                        valor_novus->lexema = lexemata[IV];
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, IV, ZEPHYRUM);
                        valor_novus->datum.selectivum.tok_commutatio = lexemata[IV];
                        valor_novus->datum.selectivum.tok_paren_ap = lexemata[III];
                        valor_novus->datum.selectivum.expressio = valori[II];
                        valor_novus->datum.selectivum.tok_paren_cl = lexemata[I];
                        valor_novus->datum.selectivum.corpus = valori[ZEPHYRUM];

                        /* Statuere patrem pro filiis */
                        si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                        si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                        frange;

                    casus ARBOR2_NODUS_CASUS:
                        /* P36: statement -> 'case' expression ':' statement */
                        /* lexemata[3] = case, [1] = : */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_CASUS;
                        valor_novus->lexema = lexemata[III];
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                        valor_novus->datum.electio.tok_casus = lexemata[III];
                        valor_novus->datum.electio.valor = valori[II];
                        valor_novus->datum.electio.tok_colon = lexemata[I];
                        valor_novus->datum.electio.sententia = valori[ZEPHYRUM];

                        /* Statuere patrem pro filiis */
                        si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                        si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                        frange;

                    casus ARBOR2_NODUS_ORDINARIUS:
                        /* P37: statement -> 'default' ':' statement */
                        /* lexemata[2] = default, [1] = : */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_ORDINARIUS;
                        valor_novus->lexema = lexemata[II];
                        valor_novus->pater = NIHIL;
                        LOCUS_EX_LEXEMATIS(valor_novus, II, ZEPHYRUM);
                        valor_novus->datum.defectus.tok_ordinarius = lexemata[II];
                        valor_novus->datum.defectus.tok_colon = lexemata[I];
                        valor_novus->datum.defectus.sententia = valori[ZEPHYRUM];

                        /* Statuere patrem pro filio */
                        si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                        frange;

                    casus ARBOR2_NODUS_PARAMETER_DECL:
                        /* P342: param -> 'const' type declarator (3 symbols)
                         * P343: param -> 'volatile' type declarator (3 symbols) */
                        si (actio->valor == 342 || actio->valor == 343)
                        {
                            /* lexemata: [2]=qualifier, [1]=type, [0]=declarator_token
                             * valori: [1]=type_specifier, [0]=declarator */
                            Arbor2Nodus* type_spec = valori[I];
                            Arbor2Nodus* decl_node = valori[ZEPHYRUM];

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_PARAMETER_DECL;
                            valor_novus->lexema = lexemata[I];  /* type token (pro fallback serialization) */
                            valor_novus->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(valor_novus, II, ZEPHYRUM);

                            /* Statuere qualifier lexemata */
                            valor_novus->datum.parameter_decl.tok_const = NIHIL;
                            valor_novus->datum.parameter_decl.tok_volatile = NIHIL;
                            si (actio->valor == 342)
                                valor_novus->datum.parameter_decl.tok_const = lexemata[II];
                            alioquin
                                valor_novus->datum.parameter_decl.tok_volatile = lexemata[II];

                            valor_novus->datum.parameter_decl.type_specifier = type_spec;
                            valor_novus->datum.parameter_decl.declarator = decl_node;

                            /* Statuere patrem pro filiis */
                            si (type_spec != NIHIL) type_spec->pater = valor_novus;
                            si (decl_node != NIHIL) decl_node->pater = valor_novus;
                        }
                        /* P344: param -> 'const' type (2 symbols, abstract const param)
                         * P345: param -> 'volatile' type (2 symbols, abstract volatile param) */
                        alioquin si (actio->valor == 344 || actio->valor == 345)
                        {
                            /* lexemata: [1]=qualifier, [0]=type
                             * valori: [0]=type specifier */
                            Arbor2Nodus* type_spec = valori[ZEPHYRUM];

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_PARAMETER_DECL;
                            valor_novus->lexema = lexemata[ZEPHYRUM];  /* type token (pro fallback serialization) */
                            valor_novus->pater = NIHIL;

                            /* Statuere qualifier lexemata */
                            valor_novus->datum.parameter_decl.tok_const = NIHIL;
                            valor_novus->datum.parameter_decl.tok_volatile = NIHIL;
                            si (actio->valor == 344)
                                valor_novus->datum.parameter_decl.tok_const = lexemata[I];
                            alioquin
                                valor_novus->datum.parameter_decl.tok_volatile = lexemata[I];

                            valor_novus->datum.parameter_decl.type_specifier = type_spec;
                            valor_novus->datum.parameter_decl.declarator = NIHIL;  /* abstract - nullus declarator */

                            /* Statuere patrem pro filiis */
                            si (type_spec != NIHIL) type_spec->pater = valor_novus;
                        }
                        /* P340: param -> type (1 symbol, abstract unqualified param) */
                        alioquin si (actio->valor == 340)
                        {
                            Arbor2Nodus* type_spec = valori[ZEPHYRUM];

                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_PARAMETER_DECL;
                            valor_novus->lexema = NIHIL;
                            valor_novus->pater = NIHIL;
                            valor_novus->datum.parameter_decl.tok_const = NIHIL;
                            valor_novus->datum.parameter_decl.tok_volatile = NIHIL;
                            valor_novus->datum.parameter_decl.type_specifier = type_spec;
                            valor_novus->datum.parameter_decl.declarator = NIHIL;

                            si (type_spec != NIHIL) type_spec->pater = valor_novus;
                        }
                        /* P43: parameter_declaration -> type_specifier declarator (generic case) */
                        alioquin
                        {
                            /* valori[1]=type_spec, valori[0]=declarator */
                            valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            valor_novus->genus = ARBOR2_NODUS_PARAMETER_DECL;
                            valor_novus->lexema = lexemata[I];  /* type_specifier token */
                            valor_novus->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(valor_novus, I, ZEPHYRUM);
                            valor_novus->datum.parameter_decl.tok_const = NIHIL;
                            valor_novus->datum.parameter_decl.tok_volatile = NIHIL;
                            valor_novus->datum.parameter_decl.type_specifier = valori[I];
                            valor_novus->datum.parameter_decl.declarator = valori[ZEPHYRUM];

                            /* Statuere patrem pro filiis */
                            si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                            si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                        }
                        frange;

                    casus ARBOR2_NODUS_DEFINITIO_FUNCTI:
                        /* P44: func_def -> type declarator compound (3 sym) */
                        /* P505/P506: func_def -> qual type declarator compound (4 sym) */
                        /* P507-P510: func_def -> modifier type declarator compound (4 sym) */
                        /* P511-P517: func_def -> mod mod [type] declarator compound (5-6 sym) */
                        /* P518-P522: func_def -> mod mod declarator compound (4 sym, implicit int) */
                        /* P523-P524: func_def -> mod mod mod declarator compound (5 sym, implicit int) */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_DEFINITIO_FUNCTI;
                        valor_novus->pater = NIHIL;
                        valor_novus->datum.definitio_functi.tok_const = NIHIL;
                        valor_novus->datum.definitio_functi.tok_volatile = NIHIL;
                        valor_novus->datum.definitio_functi.tok_unsigned = NIHIL;
                        valor_novus->datum.definitio_functi.tok_signed = NIHIL;
                        valor_novus->datum.definitio_functi.tok_long = NIHIL;
                        valor_novus->datum.definitio_functi.tok_long2 = NIHIL;
                        valor_novus->datum.definitio_functi.tok_short = NIHIL;
                        valor_novus->datum.definitio_functi.specifier = NIHIL;

                        commutatio (actio->valor)
                        {
                            casus 44:
                                /* P44: type declarator compound (3 sym) */
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, II, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 505:
                                /* P505: const type declarator compound (4 sym) */
                                valor_novus->datum.definitio_functi.tok_const = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 506:
                                /* P506: volatile type declarator compound (4 sym) */
                                valor_novus->datum.definitio_functi.tok_volatile = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 507:
                                /* P507: unsigned type declarator compound (4 sym) */
                                valor_novus->datum.definitio_functi.tok_unsigned = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 508:
                                /* P508: signed type declarator compound (4 sym) */
                                valor_novus->datum.definitio_functi.tok_signed = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 509:
                                /* P509: long type declarator compound (4 sym) */
                                valor_novus->datum.definitio_functi.tok_long = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 510:
                                /* P510: short type declarator compound (4 sym) */
                                valor_novus->datum.definitio_functi.tok_short = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 511:
                                /* P511: unsigned long type declarator compound (5 sym) */
                                valor_novus->datum.definitio_functi.tok_unsigned = lexemata[IV];
                                valor_novus->datum.definitio_functi.tok_long = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, IV, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 512:
                                /* P512: signed long type declarator compound (5 sym) */
                                valor_novus->datum.definitio_functi.tok_signed = lexemata[IV];
                                valor_novus->datum.definitio_functi.tok_long = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, IV, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 513:
                                /* P513: unsigned short type declarator compound (5 sym) */
                                valor_novus->datum.definitio_functi.tok_unsigned = lexemata[IV];
                                valor_novus->datum.definitio_functi.tok_short = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, IV, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 514:
                                /* P514: signed short type declarator compound (5 sym) */
                                valor_novus->datum.definitio_functi.tok_signed = lexemata[IV];
                                valor_novus->datum.definitio_functi.tok_short = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, IV, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 515:
                                /* P515: long long type declarator compound (5 sym) */
                                valor_novus->datum.definitio_functi.tok_long = lexemata[IV];
                                valor_novus->datum.definitio_functi.tok_long2 = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, IV, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 516:
                                /* P516: unsigned long long type declarator compound (6 sym) */
                                valor_novus->datum.definitio_functi.tok_unsigned = lexemata[V];
                                valor_novus->datum.definitio_functi.tok_long = lexemata[IV];
                                valor_novus->datum.definitio_functi.tok_long2 = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, V, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 517:
                                /* P517: signed long long type declarator compound (6 sym) */
                                valor_novus->datum.definitio_functi.tok_signed = lexemata[V];
                                valor_novus->datum.definitio_functi.tok_long = lexemata[IV];
                                valor_novus->datum.definitio_functi.tok_long2 = lexemata[III];
                                valor_novus->lexema = lexemata[II];
                                LOCUS_EX_LEXEMATIS(valor_novus, V, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.specifier = valori[II];
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[II] != NIHIL) valori[II]->pater = valor_novus;
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            /* Implicit int cases (no explicit type specifier) */
                            casus 518:
                                /* P518: unsigned long declarator compound (4 sym, implicit int) */
                                valor_novus->datum.definitio_functi.tok_unsigned = lexemata[III];
                                valor_novus->datum.definitio_functi.tok_long = lexemata[II];
                                valor_novus->lexema = lexemata[III];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 519:
                                /* P519: signed long declarator compound (4 sym, implicit int) */
                                valor_novus->datum.definitio_functi.tok_signed = lexemata[III];
                                valor_novus->datum.definitio_functi.tok_long = lexemata[II];
                                valor_novus->lexema = lexemata[III];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 520:
                                /* P520: unsigned short declarator compound (4 sym, implicit int) */
                                valor_novus->datum.definitio_functi.tok_unsigned = lexemata[III];
                                valor_novus->datum.definitio_functi.tok_short = lexemata[II];
                                valor_novus->lexema = lexemata[III];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 521:
                                /* P521: signed short declarator compound (4 sym, implicit int) */
                                valor_novus->datum.definitio_functi.tok_signed = lexemata[III];
                                valor_novus->datum.definitio_functi.tok_short = lexemata[II];
                                valor_novus->lexema = lexemata[III];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 522:
                                /* P522: long long declarator compound (4 sym, implicit int) */
                                valor_novus->datum.definitio_functi.tok_long = lexemata[III];
                                valor_novus->datum.definitio_functi.tok_long2 = lexemata[II];
                                valor_novus->lexema = lexemata[III];
                                LOCUS_EX_LEXEMATIS(valor_novus, III, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 523:
                                /* P523: unsigned long long declarator compound (5 sym, implicit int) */
                                valor_novus->datum.definitio_functi.tok_unsigned = lexemata[IV];
                                valor_novus->datum.definitio_functi.tok_long = lexemata[III];
                                valor_novus->datum.definitio_functi.tok_long2 = lexemata[II];
                                valor_novus->lexema = lexemata[IV];
                                LOCUS_EX_LEXEMATIS(valor_novus, IV, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            casus 524:
                                /* P524: signed long long declarator compound (5 sym, implicit int) */
                                valor_novus->datum.definitio_functi.tok_signed = lexemata[IV];
                                valor_novus->datum.definitio_functi.tok_long = lexemata[III];
                                valor_novus->datum.definitio_functi.tok_long2 = lexemata[II];
                                valor_novus->lexema = lexemata[IV];
                                LOCUS_EX_LEXEMATIS(valor_novus, IV, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;

                            ordinarius:
                                /* Fallback for unhandled production */
                                valor_novus->lexema = lexemata[num_pop - I];
                                LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);
                                valor_novus->datum.definitio_functi.declarator = valori[I];
                                valor_novus->datum.definitio_functi.corpus = valori[ZEPHYRUM];
                                si (valori[I] != NIHIL) valori[I]->pater = valor_novus;
                                si (valori[ZEPHYRUM] != NIHIL) valori[ZEPHYRUM]->pater = valor_novus;
                                frange;
                        }
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
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);
                        valor_novus->datum.struct_specifier.tok_semicolon = NIHIL;

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
                            valor_novus->datum.struct_specifier.tok_struct_or_union = lexemata[IV];
                            valor_novus->datum.struct_specifier.tag = tag_nodus;
                            valor_novus->datum.struct_specifier.tok_brace_ap = lexemata[II];
                            valor_novus->datum.struct_specifier.membra = (Xar*)valori[I];
                            valor_novus->datum.struct_specifier.tok_brace_cl = lexemata[ZEPHYRUM];
                            valor_novus->datum.struct_specifier.est_unio = (actio->valor == 52);
                        }
                        alioquin si (num_pop == IV)
                        {
                            /* P46: struct { member_list } or P53: union { member_list } (anonymous) */
                            /* valori: [3]=keyword, [2]={, [1]=member_list, [0]=} */
                            /* lexemata: [3]=keyword, [2]={, [1]=?, [0]=} */
                            valor_novus->lexema = lexemata[III];  /* struct/union token */
                            valor_novus->datum.struct_specifier.tok_struct_or_union = lexemata[III];
                            valor_novus->datum.struct_specifier.tag = NIHIL;
                            valor_novus->datum.struct_specifier.tok_brace_ap = lexemata[II];
                            valor_novus->datum.struct_specifier.membra = (Xar*)valori[I];
                            valor_novus->datum.struct_specifier.tok_brace_cl = lexemata[ZEPHYRUM];
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
                            valor_novus->datum.struct_specifier.tok_struct_or_union = lexemata[I];
                            valor_novus->datum.struct_specifier.tag = tag_nodus;
                            valor_novus->datum.struct_specifier.tok_brace_ap = NIHIL;
                            valor_novus->datum.struct_specifier.membra = NIHIL;
                            valor_novus->datum.struct_specifier.tok_brace_cl = NIHIL;
                            valor_novus->datum.struct_specifier.est_unio = (actio->valor == 54);
                        }
                        frange;

                    casus ARBOR2_NODUS_ENUM_SPECIFIER:
                        /* P55: enum ID { enumerator_list } (5 symbols) - named */
                        /* P56: enum { enumerator_list } (4 symbols) - anonymous */
                        /* P57: enum ID (2 symbols) - forward reference */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_ENUM_SPECIFIER;
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);
                        valor_novus->datum.enum_specifier.tok_semicolon = NIHIL;

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
                            valor_novus->datum.enum_specifier.tok_enum = lexemata[IV];
                            valor_novus->datum.enum_specifier.tag = tag_nodus;
                            valor_novus->datum.enum_specifier.tok_brace_ap = lexemata[II];
                            /* valori[I] is now a LISTA_SEPARATA node */
                            valor_novus->datum.enum_specifier.enumeratores = valori[I];
                            valor_novus->datum.enum_specifier.tok_brace_cl = lexemata[ZEPHYRUM];
                        }
                        alioquin si (num_pop == IV)
                        {
                            /* P56: enum { enumerator_list } (anonymous) */
                            /* valori: [3]=enum, [2]={, [1]=enumerator_list, [0]=} */
                            valor_novus->lexema = lexemata[III];  /* enum token */
                            valor_novus->datum.enum_specifier.tok_enum = lexemata[III];
                            valor_novus->datum.enum_specifier.tag = NIHIL;
                            valor_novus->datum.enum_specifier.tok_brace_ap = lexemata[II];
                            /* valori[I] is now a LISTA_SEPARATA node */
                            valor_novus->datum.enum_specifier.enumeratores = valori[I];
                            valor_novus->datum.enum_specifier.tok_brace_cl = lexemata[ZEPHYRUM];
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
                            valor_novus->datum.enum_specifier.tok_enum = lexemata[I];
                            valor_novus->datum.enum_specifier.tag = tag_nodus;
                            valor_novus->datum.enum_specifier.tok_brace_ap = NIHIL;
                            valor_novus->datum.enum_specifier.enumeratores = NIHIL;
                            valor_novus->datum.enum_specifier.tok_brace_cl = NIHIL;
                        }
                        frange;

                    casus ARBOR2_NODUS_ENUMERATOR:
                        /* P60: enumerator -> IDENTIFIER (1 symbol) - plain */
                        /* P61: enumerator -> IDENTIFIER '=' expression (3 symbols) - with value */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_ENUMERATOR;
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);

                        si (num_pop == I)
                        {
                            /* P60: enumerator -> IDENTIFIER */
                            /* valori: [0]=ID token (no AST node, just token) */
                            Arbor2Token* id_tok = lexemata[ZEPHYRUM];
                            valor_novus->lexema = id_tok;
                            valor_novus->datum.enumerator.titulus = id_tok->lexema->valor;
                            valor_novus->datum.enumerator.tok_assignatio = NIHIL;
                            valor_novus->datum.enumerator.valor = NIHIL;
                        }
                        alioquin si (num_pop == III)
                        {
                            /* P61: enumerator -> IDENTIFIER '=' expression */
                            /* lexemata: [2]=ID, [1]='=', [0]=last_expr */
                            /* valori: [2]=nil, [1]=nil, [0]=expression */
                            Arbor2Token* id_tok = lexemata[II];
                            valor_novus->lexema = id_tok;
                            valor_novus->datum.enumerator.titulus = id_tok->lexema->valor;
                            valor_novus->datum.enumerator.tok_assignatio = lexemata[I];
                            valor_novus->datum.enumerator.valor = valori[ZEPHYRUM];
                        }
                        frange;

                    casus ARBOR2_NODUS_INITIALIZOR_LISTA:
                        /* P200-P202: Initializer list { ... } */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_INITIALIZOR_LISTA;
                        valor_novus->lexema = lexemata[num_pop - I];  /* '{' token */
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);

                        si (actio->valor == 200)
                        {
                            /* P200: init_lista -> '{' '}' (2 symbols) - empty list
                             * Create empty LISTA_SEPARATA */
                            Arbor2Nodus* empty_lista;
                            empty_lista = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            empty_lista->genus = ARBOR2_NODUS_LISTA_SEPARATA;
                            empty_lista->lexema = NIHIL;
                            empty_lista->datum.lista_separata.elementa =
                                xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            empty_lista->datum.lista_separata.separatores =
                                xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                            valor_novus->datum.initializor_lista.tok_brace_ap = lexemata[I];
                            valor_novus->datum.initializor_lista.items = empty_lista;
                            valor_novus->datum.initializor_lista.tok_brace_cl = lexemata[ZEPHYRUM];
                        }
                        alioquin si (actio->valor == 201)
                        {
                            /* P201: init_lista -> '{' init_items '}' (3 symbols)
                             * valori: [2]=NIHIL, [1]=init_items (LISTA_SEPARATA), [0]=NIHIL */
                            valor_novus->datum.initializor_lista.tok_brace_ap = lexemata[II];
                            valor_novus->datum.initializor_lista.items = valori[I];
                            valor_novus->datum.initializor_lista.tok_brace_cl = lexemata[ZEPHYRUM];
                        }
                        alioquin si (actio->valor == 202)
                        {
                            /* P202: init_lista -> '{' init_items ',' '}' (4 symbols)
                             * valori: [3]=NIHIL, [2]=init_items (LISTA_SEPARATA), [1]=NIHIL, [0]=NIHIL */
                            valor_novus->datum.initializor_lista.tok_brace_ap = lexemata[III];
                            valor_novus->datum.initializor_lista.items = valori[II];
                            valor_novus->datum.initializor_lista.tok_brace_cl = lexemata[ZEPHYRUM];
                        }
                        frange;

                    casus ARBOR2_NODUS_DESIGNATOR_ITEM:
                        /* P217, P220: Designated initializer .x = 1 or [5] = 100 */
                        valor_novus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                        valor_novus->genus = ARBOR2_NODUS_DESIGNATOR_ITEM;
                        valor_novus->lexema = NIHIL;  /* No single token for this */
                        LOCUS_EX_LEXEMATIS(valor_novus, num_pop - I, ZEPHYRUM);
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
                            /* P41: parameter_list -> parameter_declaration (1 symbol)
                             * Create LISTA_SEPARATA with first parameter */
                            Arbor2Nodus* lista_nodus;
                            lista_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            lista_nodus->genus = ARBOR2_NODUS_LISTA_SEPARATA;
                            lista_nodus->lexema = NIHIL;
                            lista_nodus->datum.lista_separata.elementa =
                                xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            lista_nodus->datum.lista_separata.separatores =
                                xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                            /* Add first parameter */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
                        }
                        alioquin si (actio->valor == 42)
                        {
                            /* P42: parameter_list -> parameter_list ',' parameter_declaration (3 symbols)
                             * valori: [2]=parameter_list (LISTA_SEPARATA), [1]=NIHIL, [0]=parameter
                             * lexemata: [2]=?, [1]=',', [0]=last_token_of_param */
                            Arbor2Nodus* lista_nodus = valori[II];
                            /* Add comma token to separatores */
                            {
                                Arbor2Token** sep_slot = xar_addere(lista_nodus->datum.lista_separata.separatores);
                                *sep_slot = lexemata[I];  /* The comma token */
                            }
                            /* Add new parameter */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
                        }
                        alioquin si (actio->valor == 58)
                        {
                            /* P58: enumerator_list -> enumerator (1 symbol)
                             * Create LISTA_SEPARATA with first enumerator */
                            Arbor2Nodus* lista_nodus;
                            lista_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            lista_nodus->genus = ARBOR2_NODUS_LISTA_SEPARATA;
                            lista_nodus->lexema = NIHIL;
                            lista_nodus->datum.lista_separata.elementa =
                                xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            lista_nodus->datum.lista_separata.separatores =
                                xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                            /* Add first enumerator */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
                        }
                        alioquin si (actio->valor == 59)
                        {
                            /* P59: enumerator_list -> enumerator_list ',' enumerator (3 symbols)
                             * valori: [2]=enumerator_list (LISTA_SEPARATA), [1]=NIHIL, [0]=enumerator
                             * lexemata: [2]=?, [1]=',', [0]=last_token_of_enumerator */
                            Arbor2Nodus* lista_nodus = valori[II];
                            /* Add comma token to separatores */
                            {
                                Arbor2Token** sep_slot = xar_addere(lista_nodus->datum.lista_separata.separatores);
                                *sep_slot = lexemata[I];  /* The comma token */
                            }
                            /* Add new enumerator */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
                        }
                        alioquin si (actio->valor == 203)
                        {
                            /* P203: init_items -> initializer (1 symbol)
                             * Create LISTA_SEPARATA with first initializer */
                            Arbor2Nodus* lista_nodus;
                            lista_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            lista_nodus->genus = ARBOR2_NODUS_LISTA_SEPARATA;
                            lista_nodus->lexema = NIHIL;
                            lista_nodus->datum.lista_separata.elementa =
                                xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            lista_nodus->datum.lista_separata.separatores =
                                xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                            /* Add first initializer */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
                        }
                        alioquin si (actio->valor == 204)
                        {
                            /* P204: init_items -> init_items ',' initializer (3 symbols)
                             * valori: [2]=LISTA_SEPARATA, [1]=NIHIL, [0]=new initializer
                             * lexemata: [1]=',' */
                            Arbor2Nodus* lista_nodus = valori[II];
                            /* Add comma token to separatores */
                            {
                                Arbor2Token** sep_slot = xar_addere(lista_nodus->datum.lista_separata.separatores);
                                *sep_slot = lexemata[I];
                            }
                            /* Add initializer to elementa */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
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
                             * Create LISTA_SEPARATA with first designator_item */
                            Arbor2Nodus* lista_nodus;
                            lista_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            lista_nodus->genus = ARBOR2_NODUS_LISTA_SEPARATA;
                            lista_nodus->lexema = NIHIL;
                            lista_nodus->datum.lista_separata.elementa =
                                xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            lista_nodus->datum.lista_separata.separatores =
                                xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                            /* Add first designator_item */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
                        }
                        alioquin si (actio->valor == 219)
                        {
                            /* P219: init_items -> init_items ',' designator_item (3 symbols)
                             * valori: [2]=LISTA_SEPARATA, [1]=NIHIL, [0]=new designator_item
                             * lexemata: [1]=',' */
                            Arbor2Nodus* lista_nodus = valori[II];
                            /* Add comma token to separatores */
                            {
                                Arbor2Token** sep_slot = xar_addere(lista_nodus->datum.lista_separata.separatores);
                                *sep_slot = lexemata[I];
                            }
                            /* Add designator_item to elementa */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
                        }
                        /* Phase 1.3: Init-declarator list productions */
                        alioquin si (actio->valor == 221)
                        {
                            /* P221: init_decl -> declarator (1 symbol)
                             * Create quad [declarator, tok_assignatio, initializor, tok_comma] */
                            Xar* pair = xar_creare(glr->piscina, magnitudo(vacuum*));
                            vacuum** slot;
                            slot = xar_addere(pair);
                            *slot = valori[ZEPHYRUM];  /* declarator */
                            slot = xar_addere(pair);
                            *slot = NIHIL;  /* no tok_assignatio */
                            slot = xar_addere(pair);
                            *slot = NIHIL;  /* no initializor */
                            slot = xar_addere(pair);
                            *slot = NIHIL;  /* no tok_comma (first in list) */
                            valor_novus = (Arbor2Nodus*)pair;
                        }
                        alioquin si (actio->valor == 222)
                        {
                            /* P222: init_decl -> declarator '=' assignatio (3 symbols)
                             * lexemata: [2]=last_decl, [1]='=', [0]=last_init
                             * valori: [2]=declarator, [1]=nil, [0]=assignatio
                             * Create quad [declarator, tok_assignatio, initializor, tok_comma] */
                            Xar* pair = xar_creare(glr->piscina, magnitudo(vacuum*));
                            vacuum** slot;
                            slot = xar_addere(pair);
                            *slot = valori[II];  /* declarator */
                            slot = xar_addere(pair);
                            *slot = lexemata[I];  /* tok_assignatio '=' */
                            slot = xar_addere(pair);
                            *slot = valori[ZEPHYRUM];  /* initializor */
                            slot = xar_addere(pair);
                            *slot = NIHIL;  /* no tok_comma (first in list) */
                            valor_novus = (Arbor2Nodus*)pair;
                        }
                        alioquin si (actio->valor == 223)
                        {
                            /* P223: init_decl -> declarator '=' init_lista (3 symbols)
                             * lexemata: [2]=last_decl, [1]='=', [0]='}'
                             * valori: [2]=declarator, [1]=nil, [0]=init_lista
                             * Create quad [declarator, tok_assignatio, initializor, tok_comma] */
                            Xar* pair = xar_creare(glr->piscina, magnitudo(vacuum*));
                            vacuum** slot;
                            slot = xar_addere(pair);
                            *slot = valori[II];  /* declarator */
                            slot = xar_addere(pair);
                            *slot = lexemata[I];  /* tok_assignatio '=' */
                            slot = xar_addere(pair);
                            *slot = valori[ZEPHYRUM];  /* init_lista */
                            slot = xar_addere(pair);
                            *slot = NIHIL;  /* no tok_comma (first in list) */
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
                             * valori: [2]=init_decl_list (Xar of pairs), [1]=NIHIL, [0]=init_decl (quad)
                             * lexemata: [2]=?, [1]=',', [0]=last_token */
                            Xar* lista = (Xar*)valori[II];
                            Xar* new_quad = (Xar*)valori[ZEPHYRUM];
                            Xar** slot_lista;
                            /* Store comma token in quad's 4th slot */
                            si (new_quad != NIHIL && xar_numerus(new_quad) >= IV)
                            {
                                vacuum** comma_slot = xar_obtinere(new_quad, III);
                                *comma_slot = lexemata[I];  /* tok_comma */
                            }
                            slot_lista = xar_addere(lista);
                            *slot_lista = new_quad;
                            valor_novus = (Arbor2Nodus*)lista;
                        }
                        /* Phase 2: Argument lists as LISTA_SEPARATA for roundtrip */
                        alioquin si (actio->valor == 132)
                        {
                            /* P132: argumenta -> assignatio (1 symbol)
                             * Create LISTA_SEPARATA with first argument */
                            Arbor2Nodus* lista_nodus;
                            lista_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            lista_nodus->genus = ARBOR2_NODUS_LISTA_SEPARATA;
                            lista_nodus->lexema = NIHIL;
                            lista_nodus->datum.lista_separata.elementa =
                                xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));
                            lista_nodus->datum.lista_separata.separatores =
                                xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                            /* Add first argument */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
                        }
                        alioquin si (actio->valor == 133)
                        {
                            /* P133: argumenta -> argumenta ',' assignatio (3 symbols)
                             * valori: [2]=argumenta (LISTA_SEPARATA), [1]=NIHIL, [0]=assignatio
                             * lexemata: [2]=?, [1]=',', [0]=last_token_of_assignatio */
                            Arbor2Nodus* lista_nodus = valori[II];
                            /* Add comma token to separatores */
                            {
                                Arbor2Token** sep_slot = xar_addere(lista_nodus->datum.lista_separata.separatores);
                                *sep_slot = lexemata[I];  /* The comma token */
                            }
                            /* Add new argument */
                            {
                                Arbor2Nodus** slot = xar_addere(lista_nodus->datum.lista_separata.elementa);
                                *slot = valori[ZEPHYRUM];
                            }
                            valor_novus = lista_nodus;
                        }
                        alioquin si (actio->valor == 7)
                        {
                            /* P7: factor -> '(' expression ')' (3 symbols)
                             * Create PARENTHESIZED node to preserve parens for roundtrip
                             * valori: [2]=NIHIL, [1]=expression, [0]=NIHIL
                             * lexemata: [2]='(', [1]=last_token_of_expr, [0]=')' */
                            Arbor2Nodus* paren_nodus;
                            paren_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
                            paren_nodus->genus = ARBOR2_NODUS_PARENTHESIZED;
                            paren_nodus->lexema = lexemata[II];  /* ( token */
                            paren_nodus->pater = NIHIL;
                            LOCUS_EX_LEXEMATIS(paren_nodus, II, ZEPHYRUM);
                            paren_nodus->datum.parenthesized.tok_paren_ap = lexemata[II];
                            paren_nodus->datum.parenthesized.expressio = valori[I];
                            paren_nodus->datum.parenthesized.tok_paren_cl = lexemata[ZEPHYRUM];
                            paren_nodus->commenta_ante = NIHIL;
                            paren_nodus->commenta_post = NIHIL;
                            si (valori[I] != NIHIL) valori[I]->pater = paren_nodus;
                            valor_novus = paren_nodus;
                        }
                        /* ========== MULTI-ID TYPE SPECIFIER CHAINS P535-P538 (Second Block) ========== */
                        alioquin si (actio->valor == 535)
                        {
                            /* P535: type_spec_list -> ID (1 symbol)
                             * Start a new type_spec_list with first identifier token. */
                            Xar* type_list = xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                            Arbor2Token** slot = xar_addere(type_list);
                            *slot = lexemata[ZEPHYRUM];
                            valor_novus = (Arbor2Nodus*)type_list;
                        }
                        alioquin si (actio->valor == 536)
                        {
                            /* P536: type_spec_list -> type_spec_list ID (2 symbols) */
                            Xar* type_list = (Xar*)valori[I];
                            si (type_list != NIHIL)
                            {
                                Arbor2Token** slot = xar_addere(type_list);
                                *slot = lexemata[ZEPHYRUM];
                                valor_novus = (Arbor2Nodus*)type_list;
                            }
                            alioquin
                            {
                                valor_novus = NIHIL;
                            }
                        }
                        alioquin si (actio->valor == 537)
                        {
                            /* P537: declaratio -> type_spec_list init_decl_list (2 symbols)
                             * This case should be handled by nodus_genus=DECLARATIO, not default. */
                            valor_novus = NIHIL;
                        }
                        alioquin si (actio->valor == 538)
                        {
                            /* P538: func_def -> type_spec_list declarator compound (3 symbols)
                             * This case should be handled by nodus_genus=DEFINITIO_FUNCTI, not default. */
                            valor_novus = NIHIL;
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

#if GLR_TRACE_SLOWDOWN
    /* Trace: report large reduction queues (sign of exponential blowup) */
    si (i > 50 || xar_numerus(glr->frons_activa) > 10)
    {
        constans character* tok_nomen = lexema != NIHIL ?
            arbor2_lexema_genus_nomen(lexema->lexema->genus) : "EOF";
        printf("  [TRACE] pos=%d tok=%s reductions=%d frontier=%d new_frontier=%d\n",
               glr->positus, tok_nomen, i,
               xar_numerus(glr->frons_activa), xar_numerus(glr->frons_nova));
        fflush(stdout);
    }
#endif

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
                si (!_nodi_aequales(primus->valor, alius->valor))
                {
                    /* IDENTIFICATOR solum est parse incompleta - non valida top-level.
                     * Si unus path IDENTIFICATOR habet et alter non, praeferre alterum. */
                    b32 primus_est_identificator = (primus->valor->genus == ARBOR2_NODUS_IDENTIFICATOR);
                    b32 alius_est_identificator = (alius->valor->genus == ARBOR2_NODUS_IDENTIFICATOR);

                    si (primus_est_identificator && !alius_est_identificator)
                    {
                        /* Primus est IDENTIFICATOR, alius melius - mutare */
                        primus->valor = alius->valor;
                    }
                    alioquin si (!primus_est_identificator && alius_est_identificator)
                    {
                        /* Alius est IDENTIFICATOR, primus melius - servare */
                    }
                    alioquin si (primus->valor->genus == ARBOR2_NODUS_TERNARIUS &&
                                 alius->valor->genus == ARBOR2_NODUS_TERNARIUS)
                    {
                        /* Both are TERNARIUS - prefer the one with nested ternary in falsum
                         * This handles `a ? b : c ? d : e` correctly as `a ? b : (c ? d : e)` */
                        b32 primus_habet_ternarium = (primus->valor->datum.ternarius.falsum != NIHIL &&
                            primus->valor->datum.ternarius.falsum->genus == ARBOR2_NODUS_TERNARIUS);
                        b32 alius_habet_ternarium = (alius->valor->datum.ternarius.falsum != NIHIL &&
                            alius->valor->datum.ternarius.falsum->genus == ARBOR2_NODUS_TERNARIUS);

                        si (!primus_habet_ternarium && alius_habet_ternarium)
                        {
                            /* Alius has nested ternary, prefer it */
                            primus->valor = alius->valor;
                        }
                        /* alioquin: either primus has it or neither does - keep primus */
                    }
                    alioquin
                    {
                        /* Neuter est IDENTIFICATOR solum - vere ambiguum */
                        primus->valor = _creare_nodum_ambiguum(glr, primus->valor, alius->valor, primus->lexema);
                    }
                }
                /* alioquin: eadem structura, eligere primum (commenta in trivia servantur) */
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

#if GLR_DEBUG
    /* Debug: print frontier size if large */
    si (xar_numerus(glr->frons_nova) > II)
    {
        printf("  [DEBUG] FRONTIER SIZE: %d (after merge)\n", xar_numerus(glr->frons_nova));
    }
#endif

    redde nodus_acceptatus;
}

/* ==================================================
 * Error Recovery Functions
 *
 * Create ERROR nodes with skipped token information.
 * Recovery is done at statement-level in the translation
 * unit parser using _invenire_finem_declarationis.
 * ================================================== */

/* _creare_nodum_error - Create ERROR node with skipped tokens
 *
 * Populates the error node fields:
 * - nuntius: error message
 * - lexemata_saltata: array of skipped tokens
 */
interior Arbor2Nodus*
_creare_nodum_error(
    Arbor2GLR*      glr,
    chorda*         nuntius,
    Xar*            lexemata_saltata,
    Arbor2Token*    lexema_initium)
{
    Arbor2Nodus* nodus;

    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->genus = ARBOR2_NODUS_ERROR;
    nodus->lexema = lexema_initium;
    nodus->pater = NIHIL;
    nodus->commenta_ante = NIHIL;
    nodus->commenta_post = NIHIL;

    /* Set location from first skipped token or initial token */
    si (lexema_initium != NIHIL && lexema_initium->origo_meta != NIHIL)
    {
        nodus->linea_initium = lexema_initium->origo_meta->linea;
        nodus->columna_initium = lexema_initium->origo_meta->columna;
        nodus->linea_finis = lexema_initium->origo_meta->linea;
        nodus->columna_finis = lexema_initium->origo_meta->columna;
        nodus->layer_index = lexema_initium->origo_meta->layer_index;
    }
    alioquin si (lexema_initium != NIHIL)
    {
        nodus->linea_initium = lexema_initium->lexema->linea;
        nodus->columna_initium = lexema_initium->lexema->columna;
        nodus->linea_finis = lexema_initium->lexema->linea;
        nodus->columna_finis = lexema_initium->lexema->columna;
        nodus->layer_index = ZEPHYRUM;
    }
    alioquin
    {
        nodus->linea_initium = ZEPHYRUM;
        nodus->columna_initium = ZEPHYRUM;
        nodus->linea_finis = ZEPHYRUM;
        nodus->columna_finis = ZEPHYRUM;
        nodus->layer_index = ZEPHYRUM;
    }

    nodus->datum.error.nuntius = nuntius;
    nodus->datum.error.lexemata_saltata = lexemata_saltata;

    redde nodus;
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
#if GLR_TRACE_SLOWDOWN
    i32 total_iterations = ZEPHYRUM;
    i32 max_frontier_seen = ZEPHYRUM;
#endif

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

#if GLR_TRACE_SLOWDOWN
        total_iterations++;
        si (xar_numerus(glr->frons_activa) > max_frontier_seen)
        {
            max_frontier_seen = xar_numerus(glr->frons_activa);
        }
#endif

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
    resultus.tokens_consumed = glr->positus;

#if GLR_TRACE_SLOWDOWN
    /* Report parse stats if significant */
    si (total_iterations > 50 || max_frontier_seen > 5)
    {
        printf("    [STATS] tokens=%d iters=%d max_frontier=%d\n",
               xar_numerus(lexemata), total_iterations, max_frontier_seen);
        fflush(stdout);
    }
#endif

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

/* ==================================================
 * Translation Unit Parser
 *
 * Parses a complete translation unit (multiple
 * external declarations). Uses wrapper approach -
 * scans for item boundaries, then parses each item.
 * ================================================== */

/* ==================================================
 * Conditional Directive Handling (Phase 5)
 *
 * Recognizes #ifdef/#ifndef/#else/#endif and creates
 * CONDITIONALIS nodes that contain all branches.
 * ================================================== */

/* Check if token at position is # followed by conditional keyword */
interior b32
_est_conditionale_directivum(Xar* lexemata, i32 positus, Arbor2DirectivumGenus* genus_out)
{
    Arbor2Token* tok;
    Arbor2Token* next_tok;
    i32 num;

    num = xar_numerus(lexemata);
    si (positus + I >= num)
    {
        redde FALSUM;
    }

    tok = *(Arbor2Token**)xar_obtinere(lexemata, positus);
    si (tok->lexema->genus != ARBOR2_LEXEMA_HASH)
    {
        redde FALSUM;
    }

    next_tok = *(Arbor2Token**)xar_obtinere(lexemata, positus + I);

    /* Check for conditional keywords - can be IDENTIFICATOR or keywords like ELSE/IF */

    /* #else - 'else' is tokenized as ARBOR2_LEXEMA_ELSE keyword */
    si (next_tok->lexema->genus == ARBOR2_LEXEMA_ELSE)
    {
        si (genus_out != NIHIL) *genus_out = ARBOR2_DIRECTIVUM_ELSE;
        redde VERUM;
    }

    /* #if - 'if' is tokenized as ARBOR2_LEXEMA_IF keyword */
    si (next_tok->lexema->genus == ARBOR2_LEXEMA_IF)
    {
        si (genus_out != NIHIL) *genus_out = ARBOR2_DIRECTIVUM_IF;
        redde VERUM;
    }

    /* For other directives, they must be identifiers */
    si (next_tok->lexema->genus != ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        redde FALSUM;
    }

    /* Check for conditional keywords that are identifiers (not reserved words) */
    si (next_tok->lexema->valor.mensura == V &&
        memcmp(next_tok->lexema->valor.datum, "ifdef", V) == ZEPHYRUM)
    {
        si (genus_out != NIHIL) *genus_out = ARBOR2_DIRECTIVUM_IFDEF;
        redde VERUM;
    }
    si (next_tok->lexema->valor.mensura == VI &&
        memcmp(next_tok->lexema->valor.datum, "ifndef", VI) == ZEPHYRUM)
    {
        si (genus_out != NIHIL) *genus_out = ARBOR2_DIRECTIVUM_IFNDEF;
        redde VERUM;
    }
    si (next_tok->lexema->valor.mensura == IV &&
        memcmp(next_tok->lexema->valor.datum, "elif", IV) == ZEPHYRUM)
    {
        si (genus_out != NIHIL) *genus_out = ARBOR2_DIRECTIVUM_ELIF;
        redde VERUM;
    }
    si (next_tok->lexema->valor.mensura == V &&
        memcmp(next_tok->lexema->valor.datum, "endif", V) == ZEPHYRUM)
    {
        si (genus_out != NIHIL) *genus_out = ARBOR2_DIRECTIVUM_ENDIF;
        redde VERUM;
    }

    redde FALSUM;
}

/* Check if token at position starts a conditional block (ifdef/ifndef/if) */
interior b32
_est_initium_conditionale(Xar* lexemata, i32 positus)
{
    Arbor2DirectivumGenus genus;

    si (!_est_conditionale_directivum(lexemata, positus, &genus))
    {
        redde FALSUM;
    }

    redde (genus == ARBOR2_DIRECTIVUM_IFDEF ||
           genus == ARBOR2_DIRECTIVUM_IFNDEF ||
           genus == ARBOR2_DIRECTIVUM_IF);
}

/* Skip to end of line (after directive) */
interior i32
_saltare_ad_finem_lineae(Xar* lexemata, i32 positus)
{
    i32 num;
    Arbor2Token* tok;

    num = xar_numerus(lexemata);
    dum (positus < num)
    {
        tok = *(Arbor2Token**)xar_obtinere(lexemata, positus);

        /* Check for newline in spatia_post */
        si (tok->lexema->spatia_post != NIHIL)
        {
            i32 j;
            i32 num_spatia;
            num_spatia = xar_numerus(tok->lexema->spatia_post);
            per (j = ZEPHYRUM; j < num_spatia; j++)
            {
                Arbor2Lexema* spatium;
                spatium = *(Arbor2Lexema**)xar_obtinere(tok->lexema->spatia_post, j);
                si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
                {
                    redde positus + I;
                }
            }
        }

        /* Check for newline in next token's spatia_ante */
        si (positus + I < num)
        {
            Arbor2Token* next_tok;
            next_tok = *(Arbor2Token**)xar_obtinere(lexemata, positus + I);
            si (next_tok->lexema->spatia_ante != NIHIL)
            {
                i32 j;
                i32 num_spatia;
                num_spatia = xar_numerus(next_tok->lexema->spatia_ante);
                per (j = ZEPHYRUM; j < num_spatia; j++)
                {
                    Arbor2Lexema* spatium;
                    spatium = *(Arbor2Lexema**)xar_obtinere(next_tok->lexema->spatia_ante, j);
                    si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
                    {
                        redde positus + I;
                    }
                }
            }
        }

        positus++;
    }

    redde positus;
}

/* Collect directive line tokens from # to newline (for roundtrip) */
interior Xar*
_colligere_directivum_lexemata(Arbor2GLR* glr, Xar* lexemata, i32 positus, i32* positus_novus)
{
    Xar* directivum;
    i32 num;

    directivum = xar_creare(glr->piscina, magnitudo(Arbor2Token*));
    num = xar_numerus(lexemata);

    dum (positus < num)
    {
        Arbor2Token* tok;
        Arbor2Token** tok_slot;

        tok = *(Arbor2Token**)xar_obtinere(lexemata, positus);
        tok_slot = xar_addere(directivum);
        *tok_slot = tok;

        /* Check for newline in spatia_post */
        si (tok->lexema->spatia_post != NIHIL)
        {
            i32 j;
            i32 num_spatia;
            num_spatia = xar_numerus(tok->lexema->spatia_post);
            per (j = ZEPHYRUM; j < num_spatia; j++)
            {
                Arbor2Lexema* spatium;
                spatium = *(Arbor2Lexema**)xar_obtinere(tok->lexema->spatia_post, j);
                si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
                {
                    *positus_novus = positus + I;
                    redde directivum;
                }
            }
        }

        /* Check for newline in next token's spatia_ante */
        si (positus + I < num)
        {
            Arbor2Token* next_tok;
            next_tok = *(Arbor2Token**)xar_obtinere(lexemata, positus + I);
            si (next_tok->lexema->spatia_ante != NIHIL)
            {
                i32 j;
                i32 num_spatia;
                num_spatia = xar_numerus(next_tok->lexema->spatia_ante);
                per (j = ZEPHYRUM; j < num_spatia; j++)
                {
                    Arbor2Lexema* spatium;
                    spatium = *(Arbor2Lexema**)xar_obtinere(next_tok->lexema->spatia_ante, j);
                    si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
                    {
                        *positus_novus = positus + I;
                        redde directivum;
                    }
                }
            }
        }

        positus++;
    }

    *positus_novus = positus;
    redde directivum;
}

/* Get condition string from #ifdef/#ifndef/#if directive */
interior chorda*
_obtinere_conditio(Arbor2GLR* glr, Xar* lexemata, i32 positus, Arbor2DirectivumGenus genus)
{
    i32 num;
    Arbor2Token* tok;
    chorda* conditio;

    num = xar_numerus(lexemata);

    /* Skip # and keyword (ifdef/ifndef/if) */
    positus += II;

    si (positus >= num)
    {
        redde NIHIL;
    }

    tok = *(Arbor2Token**)xar_obtinere(lexemata, positus);

    /* For ifdef/ifndef, the condition is just the macro name */
    si (genus == ARBOR2_DIRECTIVUM_IFDEF || genus == ARBOR2_DIRECTIVUM_IFNDEF)
    {
        si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
        {
            conditio = piscina_allocare(glr->piscina, magnitudo(chorda));
            *conditio = tok->lexema->valor;
            redde conditio;
        }
    }

    /* For #if/#elif, conditio is NIHIL (expression tokens are collected separately) */
    redde NIHIL;
}

/* Collect expression tokens for #if/#elif directives (Phase 5b) */
interior Xar*
_obtinere_expressio_lexemata(Arbor2GLR* glr, Xar* lexemata, i32 positus)
{
    Xar* expressio;
    i32 pos;
    i32 num;

    expressio = xar_creare(glr->piscina, magnitudo(Arbor2Token*));
    pos = positus + II;  /* Skip # and if/elif */
    num = xar_numerus(lexemata);

    /* Collect until end of line */
    dum (pos < num)
    {
        Arbor2Token* tok;
        Arbor2Token** slot;
        b32 found_newline;

        tok = *(Arbor2Token**)xar_obtinere(lexemata, pos);

        /* Check for newline in trailing trivia */
        found_newline = FALSUM;
        si (tok->lexema->spatia_post != NIHIL)
        {
            i32 j;
            i32 num_spatia;

            num_spatia = xar_numerus(tok->lexema->spatia_post);
            per (j = ZEPHYRUM; j < num_spatia; j++)
            {
                Arbor2Lexema* spatium;
                spatium = *(Arbor2Lexema**)xar_obtinere(tok->lexema->spatia_post, j);
                si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA ||
                    spatium->genus == ARBOR2_LEXEMA_CONTINUATIO)
                {
                    /* Still add this token, newline follows */
                    slot = xar_addere(expressio);
                    *slot = tok;
                    found_newline = VERUM;
                    frange;
                }
            }
        }

        si (found_newline)
        {
            frange;
        }

        /* Check for newline in leading trivia of next token */
        si (pos + I < num)
        {
            Arbor2Token* next_tok;
            next_tok = *(Arbor2Token**)xar_obtinere(lexemata, pos + I);
            si (next_tok->lexema->spatia_ante != NIHIL)
            {
                i32 j;
                i32 num_spatia;

                num_spatia = xar_numerus(next_tok->lexema->spatia_ante);
                per (j = ZEPHYRUM; j < num_spatia; j++)
                {
                    Arbor2Lexema* spatium;
                    spatium = *(Arbor2Lexema**)xar_obtinere(next_tok->lexema->spatia_ante, j);
                    si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
                    {
                        /* Add current token, newline is before next */
                        slot = xar_addere(expressio);
                        *slot = tok;
                        found_newline = VERUM;
                        frange;
                    }
                }
            }
        }

        si (found_newline)
        {
            frange;
        }

        /* Add token to expression */
        slot = xar_addere(expressio);
        *slot = tok;
        pos++;
    }

    redde expressio;
}

/* Parse branch tokens into AST (returns TRANSLATION_UNIT or NIHIL) */
interior Arbor2Nodus*
_parsere_ramus(
    Arbor2GLR*  glr,
    Xar*        lexemata)    /* Xar of Arbor2Token* */
{
    Xar*            lexemata_cum_eof;
    Arbor2Token*    eof_token;
    Arbor2Token**   slot;
    Arbor2GLRResultus res;
    i32             i;
    i32             num;

    /* Ramus vacuus -> NIHIL */
    si (lexemata == NIHIL || xar_numerus(lexemata) == ZEPHYRUM)
    {
        redde NIHIL;
    }

    /* Creare copiam lexematum cum EOF addito */
    lexemata_cum_eof = xar_creare(glr->piscina, magnitudo(Arbor2Token*));

    num = xar_numerus(lexemata);
    per (i = ZEPHYRUM; i < num; i++)
    {
        Arbor2Token** src = xar_obtinere(lexemata, i);
        slot = xar_addere(lexemata_cum_eof);
        *slot = *src;
    }

    /* Creare token EOF syntheticum */
    eof_token = piscina_allocare(glr->piscina, magnitudo(Arbor2Token));
    eof_token->lexema = piscina_allocare(glr->piscina, magnitudo(Arbor2Lexema));
    eof_token->lexema->genus = ARBOR2_LEXEMA_EOF;
    eof_token->lexema->valor.datum = NIHIL;
    eof_token->lexema->valor.mensura = ZEPHYRUM;
    eof_token->lexema->linea = I;
    eof_token->lexema->columna = I;
    eof_token->origo_token = NIHIL;
    eof_token->origo_meta = NIHIL;

    slot = xar_addere(lexemata_cum_eof);
    *slot = eof_token;

    /* Parsere ut fragmentum translation unit */
    res = arbor2_glr_parsere_translation_unit(glr, lexemata_cum_eof);

    redde res.radix;
}

/* Collect entire conditional block and create CONDITIONALIS node */
interior Arbor2Nodus*
_colligere_conditionale(Arbor2GLR* glr, Xar* lexemata, i32* positus)
{
    Arbor2Nodus* nodus;
    Xar* rami;
    Xar* endif_lexemata;
    Arbor2CondRamus* ramus_currens;
    Arbor2DirectivumGenus genus;
    i32 profunditas;
    i32 pos;
    i32 num;
    i32 linea_if;
    Arbor2Token* tok_initium;

    endif_lexemata = NIHIL;

    pos = *positus;
    num = xar_numerus(lexemata);

    /* Get initial directive info */
    si (!_est_conditionale_directivum(lexemata, pos, &genus))
    {
        redde NIHIL;
    }

    tok_initium = *(Arbor2Token**)xar_obtinere(lexemata, pos);
    linea_if = tok_initium->lexema->linea;

    /* Create branch array */
    rami = xar_creare(glr->piscina, magnitudo(Arbor2CondRamus*));

    /* Create first branch */
    {
        i32 pos_initium = pos;  /* Save for expression collection */
        ramus_currens = piscina_allocare(glr->piscina, magnitudo(Arbor2CondRamus));
        ramus_currens->genus = genus;
        ramus_currens->conditio = _obtinere_conditio(glr, lexemata, pos, genus);
        ramus_currens->expressio_lexemata = NIHIL;
        ramus_currens->valor_evaluatus = ZEPHYRUM;
        ramus_currens->est_evaluatum = FALSUM;
        ramus_currens->directivum_lexemata = _colligere_directivum_lexemata(glr, lexemata, pos, &pos);
        ramus_currens->lexemata = xar_creare(glr->piscina, magnitudo(Arbor2Token*));
        ramus_currens->parsed = NIHIL;
        ramus_currens->linea = tok_initium->lexema->linea;

        /* For #if, collect and evaluate expression */
        si (genus == ARBOR2_DIRECTIVUM_IF)
        {
            ramus_currens->expressio_lexemata = _obtinere_expressio_lexemata(glr, lexemata, pos_initium);
            ramus_currens->valor_evaluatus = arbor2_conditio_evaluare(
                glr->expansion,
                ramus_currens->expressio_lexemata,
                &ramus_currens->est_evaluatum);
        }
        /* For #ifdef/#ifndef, evaluate using conditio */
        alioquin si (genus == ARBOR2_DIRECTIVUM_IFDEF || genus == ARBOR2_DIRECTIVUM_IFNDEF)
        {
            si (ramus_currens->conditio != NIHIL)
            {
                b32 est_def;
                est_def = arbor2_conditio_est_definitum(glr->expansion, *ramus_currens->conditio);
                ramus_currens->valor_evaluatus = (genus == ARBOR2_DIRECTIVUM_IFDEF) ?
                    (est_def ? I : ZEPHYRUM) : (est_def ? ZEPHYRUM : I);
                ramus_currens->est_evaluatum = VERUM;
            }
        }
    }

    /* pos is already updated by _colligere_directivum_lexemata */

    profunditas = I;

    dum (pos < num && profunditas > ZEPHYRUM)
    {
        Arbor2DirectivumGenus genus_curr;

        si (_est_conditionale_directivum(lexemata, pos, &genus_curr))
        {
            /* Handle nested conditionals */
            si (genus_curr == ARBOR2_DIRECTIVUM_IFDEF ||
                genus_curr == ARBOR2_DIRECTIVUM_IFNDEF ||
                genus_curr == ARBOR2_DIRECTIVUM_IF)
            {
                /* Nested conditional - include tokens and increase depth */
                Arbor2Token** tok_slot;
                Arbor2Token* tok_curr;

                /* Add # token */
                tok_curr = *(Arbor2Token**)xar_obtinere(lexemata, pos);
                tok_slot = xar_addere(ramus_currens->lexemata);
                *tok_slot = tok_curr;
                pos++;

                /* Add remaining directive tokens */
                dum (pos < num)
                {
                    Arbor2Token* tok_next;
                    tok_next = *(Arbor2Token**)xar_obtinere(lexemata, pos);
                    tok_slot = xar_addere(ramus_currens->lexemata);
                    *tok_slot = tok_next;

                    /* Check if end of line */
                    si (tok_next->lexema->spatia_post != NIHIL)
                    {
                        i32 j;
                        i32 num_spatia;
                        b32 found_newline;

                        found_newline = FALSUM;
                        num_spatia = xar_numerus(tok_next->lexema->spatia_post);
                        per (j = ZEPHYRUM; j < num_spatia; j++)
                        {
                            Arbor2Lexema* spatium;
                            spatium = *(Arbor2Lexema**)xar_obtinere(tok_next->lexema->spatia_post, j);
                            si (spatium->genus == ARBOR2_LEXEMA_NOVA_LINEA)
                            {
                                found_newline = VERUM;
                                frange;
                            }
                        }
                        si (found_newline)
                        {
                            pos++;
                            frange;
                        }
                    }
                    pos++;
                }

                profunditas++;
                perge;
            }
            alioquin si (genus_curr == ARBOR2_DIRECTIVUM_ENDIF)
            {
                profunditas--;
                si (profunditas == ZEPHYRUM)
                {
                    /* End of our conditional - save current branch and finish */
                    Arbor2CondRamus** ramus_slot;
                    ramus_slot = xar_addere(rami);
                    *ramus_slot = ramus_currens;

                    /* Collect #endif tokens for roundtrip */
                    endif_lexemata = _colligere_directivum_lexemata(glr, lexemata, pos, &pos);
                    frange;
                }
                alioquin
                {
                    /* Nested #endif - include in current branch */
                    Arbor2Token** tok_slot;
                    Arbor2Token* tok_curr;

                    /* Add # and endif tokens */
                    tok_curr = *(Arbor2Token**)xar_obtinere(lexemata, pos);
                    tok_slot = xar_addere(ramus_currens->lexemata);
                    *tok_slot = tok_curr;
                    pos++;

                    si (pos < num)
                    {
                        tok_curr = *(Arbor2Token**)xar_obtinere(lexemata, pos);
                        tok_slot = xar_addere(ramus_currens->lexemata);
                        *tok_slot = tok_curr;
                    }

                    pos = _saltare_ad_finem_lineae(lexemata, pos);
                    perge;
                }
            }
            alioquin si ((genus_curr == ARBOR2_DIRECTIVUM_ELSE ||
                          genus_curr == ARBOR2_DIRECTIVUM_ELIF) &&
                         profunditas == I)
            {
                /* Top-level else/elif - start new branch */
                Arbor2CondRamus** ramus_slot;
                Arbor2Token* tok_curr;
                i32 pos_initium;

                /* Save current branch */
                ramus_slot = xar_addere(rami);
                *ramus_slot = ramus_currens;

                /* Create new branch */
                tok_curr = *(Arbor2Token**)xar_obtinere(lexemata, pos);
                pos_initium = pos;
                ramus_currens = piscina_allocare(glr->piscina, magnitudo(Arbor2CondRamus));
                ramus_currens->genus = genus_curr;
                ramus_currens->conditio = NIHIL;
                ramus_currens->expressio_lexemata = NIHIL;
                ramus_currens->valor_evaluatus = ZEPHYRUM;
                ramus_currens->est_evaluatum = FALSUM;
                ramus_currens->directivum_lexemata = _colligere_directivum_lexemata(glr, lexemata, pos, &pos);

                si (genus_curr == ARBOR2_DIRECTIVUM_ELIF)
                {
                    /* Collect and evaluate #elif expression */
                    ramus_currens->expressio_lexemata = _obtinere_expressio_lexemata(glr, lexemata, pos_initium);
                    ramus_currens->valor_evaluatus = arbor2_conditio_evaluare(
                        glr->expansion,
                        ramus_currens->expressio_lexemata,
                        &ramus_currens->est_evaluatum);
                }
                alioquin si (genus_curr == ARBOR2_DIRECTIVUM_ELSE)
                {
                    /* #else is always "true" if reached */
                    ramus_currens->valor_evaluatus = I;
                    ramus_currens->est_evaluatum = VERUM;
                }

                ramus_currens->lexemata = xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                ramus_currens->parsed = NIHIL;
                ramus_currens->linea = tok_curr->lexema->linea;

                /* pos is already updated by _colligere_directivum_lexemata */
                perge;
            }
            alioquin
            {
                /* Nested else/elif - include in current branch */
                Arbor2Token** tok_slot;
                Arbor2Token* tok_curr;

                tok_curr = *(Arbor2Token**)xar_obtinere(lexemata, pos);
                tok_slot = xar_addere(ramus_currens->lexemata);
                *tok_slot = tok_curr;
                pos++;

                si (pos < num)
                {
                    tok_curr = *(Arbor2Token**)xar_obtinere(lexemata, pos);
                    tok_slot = xar_addere(ramus_currens->lexemata);
                    *tok_slot = tok_curr;
                }

                pos = _saltare_ad_finem_lineae(lexemata, pos);
                perge;
            }
        }
        alioquin
        {
            /* Regular token - add to current branch */
            Arbor2Token** tok_slot;
            Arbor2Token* tok_curr;

            tok_curr = *(Arbor2Token**)xar_obtinere(lexemata, pos);
            tok_slot = xar_addere(ramus_currens->lexemata);
            *tok_slot = tok_curr;
            pos++;
        }
    }

    /* Parse each branch's tokens into AST */
    {
        i32 num_rami = xar_numerus(rami);
        i32 r;
        per (r = ZEPHYRUM; r < num_rami; r++)
        {
            Arbor2CondRamus** ramus_ptr = xar_obtinere(rami, r);
            Arbor2CondRamus*  ramus = *ramus_ptr;

            ramus->parsed = _parsere_ramus(glr, ramus->lexemata);
        }
    }

    /* Create CONDITIONALIS node */
    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    nodus->genus = ARBOR2_NODUS_CONDITIONALIS;
    nodus->lexema = tok_initium;
    nodus->pater = NIHIL;
    nodus->commenta_ante = NIHIL;
    nodus->commenta_post = NIHIL;
    nodus->linea_initium = linea_if;
    nodus->columna_initium = tok_initium->lexema->columna;
    nodus->linea_finis = (pos > ZEPHYRUM && pos <= num) ?
        (*(Arbor2Token**)xar_obtinere(lexemata, pos - I))->lexema->linea : linea_if;
    nodus->columna_finis = I;
    nodus->layer_index = ZEPHYRUM;

    nodus->datum.conditionalis.rami = rami;
    nodus->datum.conditionalis.linea_if = linea_if;
    nodus->datum.conditionalis.linea_endif = nodus->linea_finis;
    nodus->datum.conditionalis.endif_lexemata = endif_lexemata;

    *positus = pos;
    redde nodus;
}

/* ==================================================
 * Other Preprocessor Directive Handling
 *
 * Recognizes #define, #undef, #pragma, #include
 * ================================================== */

/* Directive type for non-conditional preprocessor directives */
nomen enumeratio {
    ARBOR2_PP_NONE,
    ARBOR2_PP_DEFINE,
    ARBOR2_PP_UNDEF,
    ARBOR2_PP_PRAGMA,
    ARBOR2_PP_INCLUDE
} Arbor2PPGenus;

/* Check what kind of preprocessor directive starts at position */
interior Arbor2PPGenus
_obtinere_pp_genus(Xar* lexemata, i32 positus)
{
    Arbor2Token* tok;
    Arbor2Token* next_tok;
    i32 num;
    constans i8* ident;

    num = xar_numerus(lexemata);
    si (positus + I >= num)
    {
        redde ARBOR2_PP_NONE;
    }

    tok = *(Arbor2Token**)xar_obtinere(lexemata, positus);
    si (tok->lexema->genus != ARBOR2_LEXEMA_HASH)
    {
        redde ARBOR2_PP_NONE;
    }

    next_tok = *(Arbor2Token**)xar_obtinere(lexemata, positus + I);
    si (next_tok->lexema->genus != ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        redde ARBOR2_PP_NONE;
    }

    ident = (constans i8*)next_tok->lexema->valor.datum;

    /* Check for define */
    si (next_tok->lexema->valor.mensura == VI &&
        ident[ZEPHYRUM] == 'd' && ident[I] == 'e' && ident[II] == 'f' &&
        ident[III] == 'i' && ident[IV] == 'n' && ident[V] == 'e')
    {
        redde ARBOR2_PP_DEFINE;
    }

    /* Check for undef */
    si (next_tok->lexema->valor.mensura == V &&
        ident[ZEPHYRUM] == 'u' && ident[I] == 'n' && ident[II] == 'd' &&
        ident[III] == 'e' && ident[IV] == 'f')
    {
        redde ARBOR2_PP_UNDEF;
    }

    /* Check for pragma */
    si (next_tok->lexema->valor.mensura == VI &&
        ident[ZEPHYRUM] == 'p' && ident[I] == 'r' && ident[II] == 'a' &&
        ident[III] == 'g' && ident[IV] == 'm' && ident[V] == 'a')
    {
        redde ARBOR2_PP_PRAGMA;
    }

    /* Check for include */
    si (next_tok->lexema->valor.mensura == VII &&
        ident[ZEPHYRUM] == 'i' && ident[I] == 'n' && ident[II] == 'c' &&
        ident[III] == 'l' && ident[IV] == 'u' && ident[V] == 'd' &&
        ident[VI] == 'e')
    {
        redde ARBOR2_PP_INCLUDE;
    }

    redde ARBOR2_PP_NONE;
}

/* Create a preprocessor directive node (DEFINE, UNDEF, PRAGMA, INCLUDE) */
interior Arbor2Nodus*
_creare_pp_nodus(Arbor2GLR* glr, Xar* lexemata, i32* positus, Arbor2PPGenus genus)
{
    Arbor2Nodus* nodus;
    Arbor2Token* tok_initium;
    Xar* tokens;
    i32 pos;

    pos = *positus;
    tok_initium = *(Arbor2Token**)xar_obtinere(lexemata, pos);

    /* Collect all tokens on this directive line */
    tokens = _colligere_directivum_lexemata(glr, lexemata, pos, &pos);

    /* Create node */
    nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    nodus->lexema = tok_initium;
    nodus->pater = NIHIL;
    nodus->commenta_ante = NIHIL;
    nodus->commenta_post = NIHIL;
    nodus->linea_initium = tok_initium->lexema->linea;
    nodus->columna_initium = tok_initium->lexema->columna;
    nodus->linea_finis = tok_initium->lexema->linea;
    nodus->columna_finis = I;
    nodus->layer_index = ZEPHYRUM;

    commutatio (genus)
    {
        casus ARBOR2_PP_DEFINE:
            nodus->genus = ARBOR2_NODUS_DEFINE;
            nodus->datum.define_directive.lexemata_originalia = tokens;
            nodus->datum.define_directive.nomen_macro = NIHIL;
            nodus->datum.define_directive.est_functio = FALSUM;

            /* Extract macro name (third token after # and define) */
            si (xar_numerus(tokens) >= III)
            {
                Arbor2Token* tok_name = *(Arbor2Token**)xar_obtinere(tokens, II);
                si (tok_name->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
                {
                    nodus->datum.define_directive.nomen_macro =
                        piscina_allocare(glr->piscina, magnitudo(chorda));
                    *nodus->datum.define_directive.nomen_macro = tok_name->lexema->valor;

                    /* Check if function-like (has lparen immediately after name) */
                    si (xar_numerus(tokens) >= IV)
                    {
                        Arbor2Token* tok_next = *(Arbor2Token**)xar_obtinere(tokens, III);
                        si (tok_next->lexema->genus == ARBOR2_LEXEMA_PAREN_APERTA)
                        {
                            nodus->datum.define_directive.est_functio = VERUM;
                        }
                    }
                }
            }
            frange;

        casus ARBOR2_PP_UNDEF:
            nodus->genus = ARBOR2_NODUS_UNDEF;
            nodus->datum.undef_directive.lexemata_originalia = tokens;
            nodus->datum.undef_directive.nomen_macro = NIHIL;

            /* Extract macro name */
            si (xar_numerus(tokens) >= III)
            {
                Arbor2Token* tok_name = *(Arbor2Token**)xar_obtinere(tokens, II);
                si (tok_name->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
                {
                    nodus->datum.undef_directive.nomen_macro =
                        piscina_allocare(glr->piscina, magnitudo(chorda));
                    *nodus->datum.undef_directive.nomen_macro = tok_name->lexema->valor;
                }
            }
            frange;

        casus ARBOR2_PP_PRAGMA:
            nodus->genus = ARBOR2_NODUS_PRAGMA;
            nodus->datum.pragma_directive.lexemata_originalia = tokens;
            frange;

        casus ARBOR2_PP_INCLUDE:
            nodus->genus = ARBOR2_NODUS_INCLUDE;
            nodus->datum.include_directive.lexemata_originalia = tokens;
            nodus->datum.include_directive.info = NIHIL;  /* No resolution attempted */
            frange;

        ordinarius:
            /* Should not happen */
            redde NIHIL;
    }

    *positus = pos;
    redde nodus;
}

/* Result from boundary finder */
nomen structura {
    i32 parse_finis;    /* Index up to (exclusive) for parsing */
    i32 proximus;       /* Index to continue from after this item */
} FinisInfo;

/* Find end of external declaration
 * Returns parse_finis (how many tokens to include in parse) and
 * proximus (where to start next declaration).
 * For functions (end with }): includes closing brace
 * For declarations (end with ;): excludes semicolon from parse
 */
hic_manens FinisInfo
_invenire_finem_declarationis(Xar* lexemata, i32 initium)
{
    FinisInfo info;
    i32 i;
    i32 num;
    i32 profunditas_bracei;
    i32 profunditas_paren;
    b32 in_functione;

    num = xar_numerus(lexemata);
    profunditas_bracei = ZEPHYRUM;
    profunditas_paren = ZEPHYRUM;
    in_functione = FALSUM;

    per (i = initium; i < num; i++)
    {
        Arbor2Token** tok_ptr;
        Arbor2Token* tok;

        tok_ptr = xar_obtinere(lexemata, i);
        tok = *tok_ptr;

        commutatio (tok->lexema->genus)
        {
            casus ARBOR2_LEXEMA_BRACE_APERTA:
                profunditas_bracei++;
                in_functione = VERUM;
                frange;

            casus ARBOR2_LEXEMA_BRACE_CLAUSA:
                profunditas_bracei--;
                /* Brace depth returned to 0 - check what follows */
                si (profunditas_bracei == ZEPHYRUM && in_functione)
                {
                    /* Check next token to distinguish function body from struct/enum def */
                    si (i + I < num)
                    {
                        Arbor2Token** next_ptr = xar_obtinere(lexemata, i + I);
                        Arbor2Token* next_tok = *next_ptr;
                        /* If followed by ; or identifier, this is struct/enum, not function */
                        /* Continue to find the semicolon as the real end */
                        si (next_tok->lexema->genus == ARBOR2_LEXEMA_SEMICOLON ||
                            next_tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR ||
                            next_tok->lexema->genus == ARBOR2_LEXEMA_ASTERISCUS)
                        {
                            in_functione = FALSUM;  /* Reset - continue to semicolon */
                            frange;
                        }
                    }
                    /* Function body ended - include closing brace */
                    info.parse_finis = i + I;
                    info.proximus = i + I;
                    redde info;
                }
                frange;

            casus ARBOR2_LEXEMA_PAREN_APERTA:
                profunditas_paren++;
                frange;

            casus ARBOR2_LEXEMA_PAREN_CLAUSA:
                profunditas_paren--;
                frange;

            casus ARBOR2_LEXEMA_SEMICOLON:
                /* Declaration ends at semicolon - exclude from parse, will be set after */
                si (profunditas_bracei == ZEPHYRUM && profunditas_paren == ZEPHYRUM)
                {
                    info.parse_finis = i;     /* Exclude semicolon */
                    info.proximus = i + I;    /* Skip past semicolon */
                    redde info;
                }
                frange;

            casus ARBOR2_LEXEMA_EOF:
                info.parse_finis = i;
                info.proximus = i;
                redde info;

            ordinarius:
                frange;
        }
    }

    info.parse_finis = num;
    info.proximus = num;
    redde info;
}

/* ==================================================
 * Typedef Collection
 *
 * Post-parse pass to register typedef names from AST.
 * This enables subsequent declarations to use typedef'd names.
 * ================================================== */

/* Extrahere nomen ex declarator (recursive helper)
 * Traverses nested declarator structures to find the name.
 * Handles: DECLARATOR, DECLARATOR_FUNCTI, PARENTHESIZED
 */
hic_manens chorda
_extrahere_nomen_ex_declarator(Arbor2Nodus* declarator)
{
    chorda vacua;
    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;

    si (declarator == NIHIL)
        redde vacua;

    /* Simple declarator - return the name */
    si (declarator->genus == ARBOR2_NODUS_DECLARATOR)
    {
        redde declarator->datum.declarator.titulus;
    }

    /* Function declarator - recurse into inner */
    si (declarator->genus == ARBOR2_NODUS_DECLARATOR_FUNCTI)
    {
        redde _extrahere_nomen_ex_declarator(
            declarator->datum.declarator_functi.declarator_interior);
    }

    /* Parenthesized - recurse into expression */
    si (declarator->genus == ARBOR2_NODUS_PARENTHESIZED)
    {
        redde _extrahere_nomen_ex_declarator(
            declarator->datum.parenthesized.expressio);
    }

    redde vacua;
}

/* Extrahere nomen typedef ex declaratione
 * Returns: chorda with typedef name, or empty chorda if not extractable
 */
hic_manens chorda
_extrahere_nomen_typedef(Arbor2Nodus* declaratio)
{
    chorda vacua;
    Arbor2Nodus* declarator;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;

    si (declaratio == NIHIL || declaratio->genus != ARBOR2_NODUS_DECLARATIO)
        redde vacua;

    si (!declaratio->datum.declaratio.est_typedef)
        redde vacua;

    declarator = declaratio->datum.declaratio.declarator;

    redde _extrahere_nomen_ex_declarator(declarator);
}

/* Registrare typedef ex singula declaratione
 * Call this immediately after parsing each declaration.
 * Handles comma-separated typedefs via proxima chain.
 */
hic_manens vacuum
_registrare_typedef_ex_declaratione(Arbor2GLR* glr, Arbor2Nodus* decl)
{
    Arbor2Nodus* currens;

    si (decl == NIHIL)
        redde;

    si (glr->expansion == NIHIL)
        redde;

    /* Only process typedef declarations */
    si (decl->genus != ARBOR2_NODUS_DECLARATIO ||
        !decl->datum.declaratio.est_typedef)
        redde;

    /* Walk the proxima chain for comma-separated typedefs */
    currens = decl;
    dum (currens != NIHIL)
    {
        chorda titulus = _extrahere_nomen_typedef(currens);
        si (titulus.datum != NIHIL && titulus.mensura > ZEPHYRUM)
        {
            /* Create null-terminated string for API */
            character* titulus_nt = piscina_allocare(glr->piscina,
                (size_t)titulus.mensura + I);
            memcpy(titulus_nt, titulus.datum, (size_t)titulus.mensura);
            titulus_nt[titulus.mensura] = '\0';

            arbor2_expansion_addere_typedef(glr->expansion, titulus_nt);
        }

        /* Move to next in comma chain */
        si (currens->genus == ARBOR2_NODUS_DECLARATIO)
            currens = currens->datum.declaratio.proxima;
        alioquin
            currens = NIHIL;
    }
}

Arbor2GLRResultus
arbor2_glr_parsere_translation_unit(
    Arbor2GLR*      glr,
    Xar*            lexemata)
{
    Arbor2GLRResultus resultus;
    Arbor2Nodus* tu_nodus;
    i32 positus;
    i32 num_tokens;

    /* Create translation unit node */
    tu_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
    tu_nodus->genus = ARBOR2_NODUS_TRANSLATION_UNIT;
    tu_nodus->lexema = NIHIL;
    tu_nodus->datum.translation_unit.declarationes =
        xar_creare(glr->piscina, magnitudo(Arbor2Nodus*));

    positus = ZEPHYRUM;
    num_tokens = xar_numerus(lexemata);

    /* Clear errors for fresh parse */
    xar_vacare(glr->errores);
    glr->num_errores = ZEPHYRUM;

#if GLR_TRACE_SLOWDOWN
    /* Reset profiling counters */
    g_gss_nodes = 0;
    g_shifts = 0;
    g_reduces = 0;
#endif

#if GLR_TRACE_SLOWDOWN
    {
        i32 loop_count = ZEPHYRUM;
        clock_t loop_start = clock();
#endif

    /* Parse external declarations until EOF */
    dum (positus < num_tokens)
    {
        Arbor2Token** tok_ptr;
        Arbor2Token* tok;
        Xar* sub_tokens;
        Arbor2GLRResultus sub_res;
        i32 i;
        FinisInfo finis_info;
        Arbor2Nodus** slot;
        Arbor2Token* eof_tok;
        Arbor2Token** eof_slot;

#if GLR_TRACE_SLOWDOWN
        loop_count++;
        si (loop_count % 100 == ZEPHYRUM)
        {
            clock_t now = clock();
            duplex elapsed = (duplex)(now - loop_start) / CLOCKS_PER_SEC;
            printf("  [LOOP] iter=%d pos=%d/%d elapsed=%.2fs\n",
                   loop_count, positus, num_tokens, elapsed);
            fflush(stdout);
        }
#endif

        /* Check if only EOF remains */
        tok_ptr = xar_obtinere(lexemata, positus);
        tok = *tok_ptr;
        si (tok->lexema->genus == ARBOR2_LEXEMA_EOF)
        {
            frange;
        }

        /* Check for conditional directive (#ifdef, #ifndef, #if) */
        si (_est_initium_conditionale(lexemata, positus))
        {
            Arbor2Nodus* conditionale;
            Arbor2Nodus** cond_slot;

            conditionale = _colligere_conditionale(glr, lexemata, &positus);
            si (conditionale != NIHIL)
            {
                cond_slot = xar_addere(tu_nodus->datum.translation_unit.declarationes);
                *cond_slot = conditionale;
            }
            perge;
        }

        /* Check for other preprocessor directives (#define, #undef, #pragma, #include) */
        {
            Arbor2PPGenus pp_genus = _obtinere_pp_genus(lexemata, positus);
            si (pp_genus != ARBOR2_PP_NONE)
            {
                Arbor2Nodus* pp_nodus;
                Arbor2Nodus** pp_slot;

                pp_nodus = _creare_pp_nodus(glr, lexemata, &positus, pp_genus);
                si (pp_nodus != NIHIL)
                {
                    pp_slot = xar_addere(tu_nodus->datum.translation_unit.declarationes);
                    *pp_slot = pp_nodus;
                }
                perge;
            }
        }

        /* Find end of this external declaration */
#if GLR_TRACE_SLOWDOWN
        {
            clock_t t_finis_init = clock();
#endif
        finis_info = _invenire_finem_declarationis(lexemata, positus);
#if GLR_TRACE_SLOWDOWN
            {
                clock_t t_finis_done = clock();
                duplex finis_time = (duplex)(t_finis_done - t_finis_init) / CLOCKS_PER_SEC;
                si (finis_time > 0.1)
                {
                    printf("  [SLOW] _invenire_finem at pos %d took %.2fs\n", positus, finis_time);
                    fflush(stdout);
                }
            }
        }
#endif

        /* Create sub-token array from positus to parse_finis */
        sub_tokens = xar_creare(glr->piscina, magnitudo(Arbor2Token*));
        per (i = positus; i < finis_info.parse_finis; i++)
        {
            Arbor2Token** src;
            Arbor2Token** dst;
            src = xar_obtinere(lexemata, i);
            dst = xar_addere(sub_tokens);
            *dst = *src;
        }

        /* Include semicolon for typedef declarations (grammar requires it) */
        si (finis_info.parse_finis < num_tokens &&
            tok->lexema->genus == ARBOR2_LEXEMA_TYPEDEF)
        {
            Arbor2Token** semi_ptr = xar_obtinere(lexemata, finis_info.parse_finis);
            Arbor2Token* semi_tok = *semi_ptr;
            si (semi_tok->lexema->genus == ARBOR2_LEXEMA_SEMICOLON)
            {
                Arbor2Token** semi_slot = xar_addere(sub_tokens);
                *semi_slot = semi_tok;
            }
        }

        /* Add EOF token at end */
        eof_tok = piscina_allocare(glr->piscina, magnitudo(Arbor2Token));
        eof_tok->lexema = piscina_allocare(glr->piscina, magnitudo(Arbor2Lexema));
        eof_tok->lexema->genus = ARBOR2_LEXEMA_EOF;
        eof_tok->lexema->valor.datum = NIHIL;
        eof_tok->lexema->valor.mensura = ZEPHYRUM;
        eof_tok->lexema->linea = I;
        eof_tok->lexema->columna = I;
        eof_tok->origo_token = NIHIL;
        eof_tok->origo_meta = NIHIL;
        eof_slot = xar_addere(sub_tokens);
        *eof_slot = eof_tok;

        /* Parse one external declaration */
#if GLR_TRACE_SLOWDOWN
        {
            clock_t t_initium = clock();
            hic_manens i32 decl_num = ZEPHYRUM;
            decl_num++;
#endif
        sub_res = arbor2_glr_parsere_expressio(glr, sub_tokens);
#if GLR_TRACE_SLOWDOWN
            {
                clock_t t_finis = clock();
                duplex tempus = (duplex)(t_finis - t_initium) / CLOCKS_PER_SEC;
                printf("  [TRACE] Decl #%d at pos %d: %.3fs (%d tokens)\n",
                       decl_num, positus, tempus, xar_numerus(sub_tokens));
                fflush(stdout);
            }
        }
#endif

        si (!sub_res.successus)
        {
            /* Error recovery - check if we've hit the error limit */
            glr->num_errores++;

            si (glr->num_errores > ARBOR2_GLR_MAX_ERRORES)
            {
                /* Too many errors - give up and return partial result */
                resultus.successus = FALSUM;
                resultus.errores = glr->errores;
                resultus.ambigui = sub_res.ambigui;
                resultus.radix = tu_nodus;
                resultus.tokens_consumed = positus;
                redde resultus;
            }

            /* Create ERROR node with skipped tokens */
            {
                Arbor2Nodus* error_nodus;
                Xar* lexemata_saltata;
                chorda* nuntius;
                Arbor2Nodus** error_slot;

                /* Collect tokens that were part of this failed parse */
                lexemata_saltata = xar_creare(glr->piscina, magnitudo(Arbor2Token*));
                per (i = positus; i < finis_info.proximus; i++)
                {
                    Arbor2Token** src_tok;
                    Arbor2Token** dst_tok;
                    src_tok = xar_obtinere(lexemata, i);
                    dst_tok = xar_addere(lexemata_saltata);
                    *dst_tok = *src_tok;
                }

                /* Get error message from sub_res.errores if available */
                nuntius = NIHIL;
                si (sub_res.errores != NIHIL && xar_numerus(sub_res.errores) > ZEPHYRUM)
                {
                    chorda** err_ptr;
                    err_ptr = xar_obtinere(sub_res.errores, ZEPHYRUM);
                    nuntius = *err_ptr;
                }

                /* Create ERROR node */
                error_nodus = _creare_nodum_error(glr, nuntius, lexemata_saltata, tok);

                si (error_nodus != NIHIL)
                {
                    error_slot = xar_addere(tu_nodus->datum.translation_unit.declarationes);
                    *error_slot = error_nodus;
                }
            }

            /* Advance past this erroneous item and continue */
            positus = finis_info.proximus;
            perge;
        }

        /* Initialize comment fields on the parsed node */
        si (sub_res.radix != NIHIL)
        {
            sub_res.radix->commenta_ante = NIHIL;
            sub_res.radix->commenta_post = NIHIL;
        }

        /* Set tok_semicolon on declaration (semicolon excluded from parse) */
        si (sub_res.radix != NIHIL &&
            sub_res.radix->genus == ARBOR2_NODUS_DECLARATIO &&
            finis_info.parse_finis < num_tokens)
        {
            Arbor2Token** semi_ptr = xar_obtinere(lexemata, finis_info.parse_finis);
            Arbor2Token* semi_tok = *semi_ptr;
            si (semi_tok->lexema->genus == ARBOR2_LEXEMA_SEMICOLON)
            {
                Arbor2Nodus* decl;
                /* Set on first declaration and all chained declarations */
                decl = sub_res.radix;
                dum (decl != NIHIL)
                {
                    decl->datum.declaratio.tok_semicolon = semi_tok;
                    decl = decl->datum.declaratio.proxima;
                }
            }
        }

        /* Set tok_semicolon on enum specifier (standalone type definition) */
        si (sub_res.radix != NIHIL &&
            sub_res.radix->genus == ARBOR2_NODUS_ENUM_SPECIFIER &&
            finis_info.parse_finis < num_tokens)
        {
            Arbor2Token** semi_ptr = xar_obtinere(lexemata, finis_info.parse_finis);
            Arbor2Token* semi_tok = *semi_ptr;
            si (semi_tok->lexema->genus == ARBOR2_LEXEMA_SEMICOLON)
            {
                sub_res.radix->datum.enum_specifier.tok_semicolon = semi_tok;
            }
        }

        /* Set tok_semicolon on struct/union specifier (standalone type definition) */
        si (sub_res.radix != NIHIL &&
            sub_res.radix->genus == ARBOR2_NODUS_STRUCT_SPECIFIER &&
            finis_info.parse_finis < num_tokens)
        {
            Arbor2Token** semi_ptr = xar_obtinere(lexemata, finis_info.parse_finis);
            Arbor2Token* semi_tok = *semi_ptr;
            si (semi_tok->lexema->genus == ARBOR2_LEXEMA_SEMICOLON)
            {
                sub_res.radix->datum.struct_specifier.tok_semicolon = semi_tok;
            }
        }

        /* Set tok_semicolon on DECLARATIO interpretations inside AMBIGUUS nodes */
        si (sub_res.radix != NIHIL &&
            sub_res.radix->genus == ARBOR2_NODUS_AMBIGUUS &&
            finis_info.parse_finis < num_tokens)
        {
            Arbor2Token** semi_ptr = xar_obtinere(lexemata, finis_info.parse_finis);
            Arbor2Token* semi_tok = *semi_ptr;
            si (semi_tok->lexema->genus == ARBOR2_LEXEMA_SEMICOLON &&
                sub_res.radix->datum.ambiguus.interpretationes != NIHIL)
            {
                i32 j;
                i32 num_interp = xar_numerus(sub_res.radix->datum.ambiguus.interpretationes);
                per (j = ZEPHYRUM; j < num_interp; j++)
                {
                    Arbor2Nodus** interp_ptr = xar_obtinere(sub_res.radix->datum.ambiguus.interpretationes, j);
                    Arbor2Nodus* interp = *interp_ptr;
                    si (interp != NIHIL && interp->genus == ARBOR2_NODUS_DECLARATIO)
                    {
                        /* Set on this declaration and all chained declarations */
                        Arbor2Nodus* decl = interp;
                        dum (decl != NIHIL)
                        {
                            decl->datum.declaratio.tok_semicolon = semi_tok;
                            decl = decl->datum.declaratio.proxima;
                        }
                    }
                }
            }
        }

        /* Add to translation unit */
        slot = xar_addere(tu_nodus->datum.translation_unit.declarationes);
        *slot = sub_res.radix;

        /* Register typedef names immediately so subsequent decls can use them */
#if GLR_TRACE_SLOWDOWN
        {
            clock_t t_reg_start = clock();
#endif
        _registrare_typedef_ex_declaratione(glr, sub_res.radix);
#if GLR_TRACE_SLOWDOWN
            {
                clock_t t_reg_end = clock();
                duplex reg_time = (duplex)(t_reg_end - t_reg_start) / CLOCKS_PER_SEC;
                si (reg_time > 0.1)
                {
                    printf("  [SLOW] _registrare_typedef took %.2fs\n", reg_time);
                    fflush(stdout);
                }
            }
        }
#endif

        /* Promote comments from the first token's trivia */
        {
            Arbor2Nodus* nodus_ante = NIHIL;
            i32 num_decls = xar_numerus(tu_nodus->datum.translation_unit.declarationes);

            /* Get previous declaration if any */
            si (num_decls > I)
            {
                Arbor2Nodus** prev_ptr = xar_obtinere(
                    tu_nodus->datum.translation_unit.declarationes,
                    num_decls - II);
                nodus_ante = *prev_ptr;
            }

#if GLR_TRACE_SLOWDOWN
            printf("  [TRACE] calling _promovere pos=%d\n", positus);
            fflush(stdout);
            {
                clock_t t_prom_start = clock();
#endif

            /* Promote comments from first token */
            _promovere_commenta_ex_spatia(
                glr->piscina,
                tok,
                nodus_ante,
                sub_res.radix,
                tu_nodus->datum.translation_unit.declarationes);

#if GLR_TRACE_SLOWDOWN
                {
                    clock_t t_prom_end = clock();
                    duplex prom_time = (duplex)(t_prom_end - t_prom_start) / CLOCKS_PER_SEC;
                    printf("  [TRACE] done _promovere pos=%d time=%.2fs\n", positus, prom_time);
                    fflush(stdout);
                }
            }
#endif
        }

        /* Advance position past this item (including semicolon if any) */
        positus = finis_info.proximus;
    }

#if GLR_TRACE_SLOWDOWN
        {
            clock_t loop_end = clock();
            duplex total_loop = (duplex)(loop_end - loop_start) / CLOCKS_PER_SEC;
            si (total_loop > 0.5)
            {
                printf("  [LOOP DONE] total_iters=%d total_time=%.2fs\n",
                       loop_count, total_loop);
                fflush(stdout);
            }
        }
    }
#endif

#if GLR_TRACE_SLOWDOWN
    /* Print profiling counters */
    printf("  [COUNTERS] gss_nodes=%lld shifts=%lld reduces=%lld\n",
           g_gss_nodes, g_shifts, g_reduces);
    fflush(stdout);
#endif

    /* Return translation unit with any collected errors */
    resultus.successus = VERUM;  /* We produced an AST (may contain ERROR nodes) */
    resultus.radix = tu_nodus;
    resultus.errores = (glr->num_errores > ZEPHYRUM) ? glr->errores : NIHIL;
    resultus.ambigui = NIHIL;
    resultus.tokens_consumed = positus;

    redde resultus;
}
