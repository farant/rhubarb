#ifndef PISCINA_H
#define PISCINA_H

#include "latina.h"


/* ===============================================
 * Creatio
 * =============================================== */

nomen structura Piscina Piscina;

Piscina*
piscina_generare_dynamicum (
				 character* piscinae_titulum,
		memoriae_index  mensura_blocki_initia);

Piscina*
piscina_generare_certae_magnitudinis (
				 character* piscinae_titulum,
		memoriae_index  mensura_buffer);


/* ===============================================
 * Destructio
 * =============================================== */

vacuum
piscina_destruere (
		Piscina* piscina);


/* ===============================================
 * Allocatio - fatalis si fallit
 * =============================================== */

vacuum*
piscina_allocare (
						 Piscina* piscina,
		memoriae_index  mensura);

vacuum*
piscina_allocare_ordinatum (
						 Piscina* piscina,
		memoriae_index  mensura,
		memoriae_index  ordinatio);


/* ===============================================
 * Allocatio - redde NIHIL si defectu
 * =============================================== */

vacuum*
piscina_conari_allocare (
						 Piscina* piscina,
		memoriae_index  mensura);

vacuum*
piscina_conari_allocare_ordinatum (
						 Piscina* piscina,
		memoriae_index  mensura,
		memoriae_index  ordinatio);


/* ===============================================
 * Cyclus vitae
 * =============================================== */

vacuum 
piscina_vacare (
		Piscina* piscina);


/* ===============================================
 * Quaestio
 * =============================================== */

memoriae_index 
piscina_summa_usus (
		constans Piscina* piscina);

memoriae_index
piscina_summa_inutilis_allocatus (
		constans Piscina* piscina);

memoriae_index
piscina_reliqua_antequam_cresca_blocki (
		constans Piscina* piscina);

memoriae_index
piscina_summa_apex_usus (
		constans Piscina* piscina);

#endif
