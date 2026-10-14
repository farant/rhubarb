/*
 * mensura_contextus.h - contextus cursus (analytica T3)
 *
 * Contextus sessionis (commissum, sordidum, ramus, arbor, machina,
 * sessio) SEMEL per sessionem summam computatur et per AMBITUM
 * (MENSOR_*) prolibus traditur - cursus interiores eum hereditant,
 * git non iterum vocatur. bin/mensor (involucrum, incipere) et
 * bin/fabrica (actiones) eundem contextum adhibent.
 */

#ifndef MENSURA_CONTEXTUS_H
#define MENSURA_CONTEXTUS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "mensura.h"
#include "diurna.h"

/* Contextum in 'initium' ponit (sessio, parens, commissum, sordidum,
 * ramus, arbor, machina, vocator). MENSOR_COMMISSUM absens = contextus
 * computatur (git in arbore cwd) et in ambitum processus ponitur;
 * praesens = hereditatur. Vocator: MENSOR_VOCATOR, aliter 'agens' sub
 * Claude Code (CLAUDECODE), aliter 'homo'. Extra repositorium campi
 * vacui, nulla ruina. */
vacuum
mensura_contextum_parare (
    CursusInitium* initium,
          Piscina* piscina);

/* Contextus ARBORIS datae (analytica T4), non sessionis: arbor =
 * via_arboris, commissum HEAD, ramus, sordidum computati si via
 * repositorium est (VERUM redditur), aliter vacui (FALSUM). Ambitum
 * non tangit. Pro producente qui radicem ALIAM iudicat quam arbor
 * sessionis (bin/fabrica in radice fixturae). */
b32
mensura_contextum_arboris (
         CursusInitium* initium,
    constans character* via_arboris,
               Piscina* piscina);

/* diarium cursuum: MENSOR_DIARIUM aut ~/.rhubarb/mensurae.diarium */
constans character*
mensura_diarium_cursuum (
    Piscina* piscina);

/* taberna actorum cursuum (analytica T4): MENSOR_DIURNA aut
 * ~/.rhubarb/diurna. Clavis = id cursus. NIHIL si aperiri nequit. */
Diurna*
mensura_diurna_aperire (
    Piscina* piscina);

#endif /* MENSURA_CONTEXTUS_H */
