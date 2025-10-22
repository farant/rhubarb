#ifndef ARENA_H
#define ARENA_H

#include "latina.h"

nomen structura Arena Arena;

Arena*
arena_generare_dynamicum (
				 character* arenae_titulum,
		memoriae_index  mensura_initialis);

Arena*
arena_generare_certae_magnitudinis (
				 character* arenae_titulum,
		memoriae_index  mensura_buffer);

vacuum
arena_destruere (
		Arena* arena);


/* ===============================================
 * Allocatio - fatalis si fallit
 * =============================================== */

vacuum*
arena_allocare (
						 Arena* arena,
		memoriae_index  mensura);

vacuum*
arena_allocare_ordinatum (
						 Arena* arena,
		memoriae_index  mensura,
		memoriae_index  ordinatio);


/* ===============================================
 * Allocatio - redde NIHIL si defectu
 * =============================================== */

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

memoriae_index
arena_summa_apex_usus (
		constans Arena* arena);

#endif
