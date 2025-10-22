#include "arena.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define ARENA_DEBUG FALSUM /* Muta ad VERUM pro imprimere debugging */

nomen structura {
    vacuum* buffer;
    memoriae_index capacitas;
    memoriae_index offset;
    memoriae_index maximus_usus;
    character* titulus;
    b32 est_dynamicum;
} Arena;


/* ===========================================================
 * ADIUTORES INTERNI
 * =========================================================== */

interior memoriae_index
_proxima_ordinatio (
        memoriae_index ptr, 
        memoriae_index ordinatio)
{
    memoriae_index ordinatus = ptr + (ordinatio - I);
    redde ordinatus - (ordinatus % ordinatio);
}

interior vacuum
_debug_imprimere (
        constans character* arenae_titulum,
        constans character* operatio,
            memoriae_index  mensura)
{
    si (ARENA_DEBUG) 
    {
        imprimere("[ARENA %s] %s: %zu bytes\n", arenae_titulum, operatio, mensura);
    }
}


/* ===========================================================
 * GENERATIO
 * =========================================================== */

Arena*
arena_generare_dynamicum (
             character* arenae_titulum,
        memoriae_index  mensura_initialis)
{
    Arena* arena = (Arena*)memoriae_allocare(magnitudo(Arena));
    si (!arena) redde NIHIL;

    arena->buffer = memoriae_allocare(mensura_initialis);
    si (!arena->buffer) 
    {
        liberare(arena);
        redde NIHIL;
    }

    arena->capacitas     = mensura_initialis;
    arena->offset        = ZEPHYRUM;
    arena->maximus_usus  = ZEPHYRUM;
    arena->est_dynamicum = VERUM;

    si (arenae_titulum)
    {
        memoriae_index mensura_tituli = strlen(arenae_titulum);
        arena->titulus = (character*)memoriae_allocare(mensura_tituli + I);

        si (arena->titulus)
        {
            strcpy(arena->titulus, arenae_titulum);
        }
        alioquin
        {
            arena->titulus = NIHIL;
        }
    }
    alioquin
    {
        arena->titulus = NIHIL;
    }

    redde arena;
}

Arena*
arena_generare_certae_magnitudinis (
             character* arenae_titulum,
        memoriae_index  mensura_buffer)
{
    Arena* arena = (Arena*)memoriae_allocare(magnitudo(Arena));
    si (!arena) redde NIHIL;

    arena->buffer = memoriae_allocare(mensura_buffer);
    si (!arena->buffer)
    {
        liberare(arena);
        redde NIHIL;
    }

    arena->capacitas     = mensura_buffer;
    arena->offset        = ZEPHYRUM;
    arena->maximus_usus  = ZEPHYRUM;
    arena->est_dynamicum = FALSUM;

    si (arenae_titulum)
    {
        memoriae_index mensura_tituli = strlen(arenae_titulum);
        arena->titulus = (character*)memoriae_allocare(mensura_tituli + I);
        si (arena->titulus)
        {
            strcpy(arena->titulus, arenae_titulum);
        }
        alioquin
        {
            arena->titulus = NIHIL;
        }
    }
    alioquin
    {
        arena->titulus = NIHIL;
    }

    redde arena;
}


/* ===========================================================
 * DESTRUCTIO
 * =========================================================== */

vacuum 
arena_destruere (
        Arena* arena)
{
    si (!arena) redde;

    si (arena->buffer) liberare(arena->buffer);
    si (arena->titulus) liberare(arena->titulus);

    liberare(arena);
}

/* ===========================================================
 * ALLOCATIONIS ADIUTORES
 * =========================================================== */

interior vacuum*
_allocare_interna (
                 Arena* arena,
        memoriae_index  mensura,
        memoriae_index  ordinatio,
                   b32  fatalis)
{
    si (!arena || mensura == ZEPHYRUM) redde NIHIL;

    memoriae_index ordinatus_offset = _proxima_ordinatio(arena->offset, ordinatio);
    memoriae_index necessaria = ordinatus_offset + mensura;

    si (necessaria > arena->capacitas)
    {
        si (fatalis)
        {
            /* TODO: imprimere ad stderr; volo definitionem pro fprintf, 
             * non solum printf */
            imprimere("ALLOCATIO ARENAE FRACTA: %s (indigentia %zu, habeo %zu)\n",
                      arena->titulus ? arena->titulus : "nemo",
                      necessaria, arena->capacitas);
            exire(I);
        }
        redde NIHIL;
    }

    vacuum* ptr = (character*)arena->buffer + ordinatus_offset;
    arena->offset = necessaria;

    si (necessaria > arena->maximus_usus) 
    {
        arena->maximus_usus = necessaria;
    }

    _debug_imprimere(arena->titulus ? arena->titulus : "nemo", "allocare", mensura);

    redde ptr;
}

/* ===========================================================
 * ALLOCATIO - EXITIUM SI DEFECIT
 * =========================================================== */


vacuum*
arena_allocare (
                 Arena* arena,
        memoriae_index  mensura)
{
    redde _allocare_interna(arena, mensura, I, VERUM);
}

vacuum*
arena_allocare_ordinatum (
                 Arena* arena,
        memoriae_index  mensura,
        memoriae_index  ordinatio)
{
    redde _allocare_interna(arena, mensura, ordinatio, VERUM);
}

/* ===========================================================
 * ALLOCATIO - REDDE NIHIL SI DEFECIT
 * =========================================================== */

vacuum*
arena_conari_allocare (
                 Arena* arena,
        memoriae_index  mensura)
{
    redde _allocare_interna(arena, mensura, I, FALSUM);
}

vacuum*
arena_conari_allocare_ordinatum (
                 Arena* arena, 
        memoriae_index  mensura,
        memoriae_index  ordinatio)
{
    redde _allocare_interna(arena, mensura, ordinatio, FALSUM);
}

/* ===========================================================
 * CYCLUS VITAE
 * =========================================================== */

vacuum
arena_vacare (
        Arena* arena)
{
    si (!arena) redde;
    memset(arena->buffer, 0, arena->capacitas);
    arena->offset = ZEPHYRUM;
    _debug_imprimere(arena->titulus ? arena->titulus : "nemo", "vacare", ZEPHYRUM);
}

/* ===========================================================
 * QUAESTIO
 * =========================================================== */

memoriae_index
arena_summa_usus (
        constans Arena* arena)
{
    redde arena ? arena->offset : ZEPHYRUM;
}

memoriae_index
arena_summa_reliqua (
        constans Arena* arena)
{
    redde arena ? (arena->capacitas - arena->offset) : ZEPHYRUM;
}

memoriae_index
arena_summa_apex_usus (
        constans Arena* arena)
{
    redde arena ? arena->maximus_usus : ZEPHYRUM;
}

