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

/* diarium cursuum: MENSOR_DIARIUM aut ~/.rhubarb/mensurae.diarium */
constans character*
mensura_diarium_cursuum (
    Piscina* piscina);

#endif /* MENSURA_CONTEXTUS_H */
