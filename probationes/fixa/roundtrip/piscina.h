#ifndef PISCINA_H
#define PISCINA_H

#include "latina.h"


/* ===============================================
 * Creatio
 * =============================================== */

nomen structura Piscina Piscina;

/* PiscinaNotatio - nota pro mark/reset pattern
 * Captat statum piscinam ut postea reficere possit
 */
nomen structura PiscinaNotatio {
    vacuum*        alveus_nunc;   /* Index ad alveum currentem */
    memoriae_index positus;       /* Offset in alveo */
} PiscinaNotatio;

Piscina*
piscina_generare_dynamicum (
		constans character* piscinae_titulum,
			  memoriae_index  mensura_alvei_initia);

Piscina*
piscina_generare_certae_magnitudinis (
		constans character* piscinae_titulum,
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
 * Cyclus Vitae
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
piscina_reliqua_antequam_cresca_alvei (
		constans Piscina* piscina);

memoriae_index
piscina_summa_apex_usus (
		constans Piscina* piscina);


/* ===============================================
 * Notatio - mark/reset pattern
 * =============================================== */

/* piscina_notare - Captat statum currentem
 * "Notare positionem currentem pro refectione postea"
 *
 * Usus: PiscinaNotatio nota = piscina_notare(piscina);
 *       ... allocare temporaria ...
 *       piscina_reficere(piscina, nota);
 */
PiscinaNotatio
piscina_notare (
		Piscina* piscina);

/* piscina_reficere - Reficit statum ad notationem
 * "Reficere piscinam ad statum notatum"
 *
 * Omnia allocata post notationem erunt invalida!
 */
vacuum
piscina_reficere (
		      Piscina* piscina,
		PiscinaNotatio notatio);

/* piscina_potesne_allocare - Verificat si allocatio possibilis
 * "Potesne allocare hanc mensuram sine crescentia?"
 *
 * Utile pro piscinis certae magnitudinis
 * Redde: VERUM si allocatio in alveo nunc capit
 */
b32
piscina_potesne_allocare (
		constans Piscina* piscina,
		  memoriae_index  mensura);

#endif
