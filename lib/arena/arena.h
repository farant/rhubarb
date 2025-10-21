#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>
#include "latina.h"

nomen structura Arena Arena;

Arena*
arena_generare_dynamicum (
				 character* nomen_arenae,
		memoriae_index  mensura_initialis);

Arena*
arena_generare_certae_magnitudinis (
				 character* nomen_arenae,
		memoriae_index  mensura_buffer);

vacuum
arena_destruere (
		Arena* arena);

/* Allocation - fatal on failure */
vacuum*
arena_allocare (
						 Arena* arena,
		memoriae_index  mensura);

vacuum*
arena_allocare_ordinatum (
						 Arena* arena,
		memoriae_index  mensura,
		memoriae_index  ordinatio);


/* Allocation - return NULL on failure */
vacuum*
arena_conari_allocare (
		Arena* arena,
		memoriae_index mensura);

vacuum*
arena_conari_allocare_ordinatum (
		Arena* arena,
		memoriae_index mensura,
		memoriae_index ordinatio);


vacuum 
arena_vacare (
		Arena* arena);

memoriae_index 
arena_summa_usus (
		constans Arena* arena);

memoriae_index
arena_summa_reliqua (
		constans Arena* arena);

/* TODO: better name? */
memoriae_index
arena_summa_apex (
		constans Arena* arena);

#endif
