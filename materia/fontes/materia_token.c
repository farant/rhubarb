/* materia_token.c - Lexema substrati
 *
 * Vide materia_token.h pro ratione et mensuris.
 */

#include "materia_token.h"

#include <string.h>


/* Ordinatio: silva piscina_allocare nudam adhibet (ordinatio I) et
 * lexemata eius VIII-ordinata manent tantum quia magnitudo CXXVIII
 * est et quidquid interponitur item octonarium. Fortuna, non
 * contractus. Cauda id ferre non potest - punctator eius ordinatus
 * esse DEBET - ergo hic poscitur quod illic sperabatur. */
#define _ORDINATIO ((memoriae_index)magnitudo(vacuum*))


MateriaToken*
materia_token_creare (
                        Piscina* piscina,
     constans MateriaTokenForma* forma,
                            s32  genus,
                         chorda  valor,
                            s32  byte_offset,
                            i32  linea,
                            i32  columna,
                            s32  fons_index)
{
      MateriaToken* token;
    memoriae_index  mensura;
               i32  cauda;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    cauda = (forma == NIHIL) ? ZEPHYRUM : forma->mensura_caudae;
    mensura = (memoriae_index)magnitudo(MateriaToken)
            + (memoriae_index)cauda;

    token = (MateriaToken*)piscina_allocare_ordinatum(piscina, mensura,
        _ORDINATIO);
    si (token == NIHIL)
    {
        redde NIHIL;
    }
    memset(token, ZEPHYRUM, (size_t)mensura);

    token->valor        = valor;
    token->genus        = genus;
    token->byte_offset  = byte_offset;
    token->linea        = linea;
    token->columna      = columna;
    token->fons_index   = fons_index;
    si (cauda > ZEPHYRUM)
    {
        token->vexilla |= MATERIA_TOKEN_CAUDA;
    }

    redde token;
}

vacuum*
materia_token_cauda (
    constans MateriaToken* token)
{
    si (   token                                  == NIHIL
        || (token->vexilla & MATERIA_TOKEN_CAUDA) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    /* Cauda ipsa MUTABILIS est quamquam lexema constans traditur:
     * datum frontis est, non lexematis. Cast-qual hic consultus. */
    redde (vacuum*)(size_t)((constans character*)token
                          + magnitudo(MateriaToken));
}


/* ==================================================
 * Trivia
 * ================================================== */

interior b32
_trivia_ponere (
     MateriaToken*** destinatio,
              i32*   numerus_destinatio,
          Piscina*   piscina,
     MateriaToken**  fonte,
              i32    numerus)
{
      MateriaToken** series;
               i32   i;

    si (numerus == ZEPHYRUM)
    {
        *destinatio          = NIHIL;
        *numerus_destinatio  = ZEPHYRUM;
        redde VERUM;
    }
    si (piscina == NIHIL || fonte == NIHIL)
    {
        redde FALSUM;
    }

    /* EXACTA - hoc est totum consilium (RP XV). Xar hic
     * DCCIV octetos posceret ut numerus*VIII servaret. */
    series = (MateriaToken**)piscina_allocare_ordinatum(piscina,
        (memoriae_index)numerus * (memoriae_index)magnitudo(MateriaToken*),
        _ORDINATIO);
    si (series == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        series[i] = fonte[i];
    }

    *destinatio          = series;
    *numerus_destinatio  = numerus;
    redde VERUM;
}

b32
materia_token_trivia_ante_ponere (
         MateriaToken*  token,
              Piscina*  piscina,
         MateriaToken** fonte,
                  i32   numerus)
{
    si (token == NIHIL)
    {
        redde FALSUM;
    }
    redde _trivia_ponere(&token->spatia_ante, &token->numerus_ante,
        piscina, fonte, numerus);
}

b32
materia_token_trivia_post_ponere (
         MateriaToken*  token,
              Piscina*  piscina,
         MateriaToken** fonte,
                  i32   numerus)
{
    si (token == NIHIL)
    {
        redde FALSUM;
    }
    redde _trivia_ponere(&token->spatia_post, &token->numerus_post,
        piscina, fonte, numerus);
}


/* ==================================================
 * Vexilla
 * ================================================== */

b32
materia_token_initium_lineae (
    constans MateriaToken* token)
{
    si (token == NIHIL)
    {
        redde FALSUM;
    }
    redde (b32)((token->vexilla & MATERIA_TOKEN_INITIUM_LINEAE)
                != ZEPHYRUM);
}

vacuum
materia_token_initium_lineae_ponere (
     MateriaToken* token,
              b32  valor)
{
    si (token == NIHIL)
    {
        redde;
    }
    si (valor)
    {
        token->vexilla |= MATERIA_TOKEN_INITIUM_LINEAE;
    }
    alioquin
    {
        token->vexilla &= ~MATERIA_TOKEN_INITIUM_LINEAE;
    }
}
