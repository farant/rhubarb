/* GENERATUM a bin/norma c ex probationes/fixa/norma/normae_fixae.norma - noli manu mutare */
#ifndef NORMAE_FIXAE_H
#define NORMAE_FIXAE_H

#include "latina.h"
#include "piscina.h"
#include "norma.h"

Norma*
fixa_norma_usus (
    Piscina* piscina);

Norma*
fixa_norma_omnia (
    Piscina* piscina);

/* vocans definit (norma-spec-2 §III.3) */
JsonValor*
fixa_norma_gignens_ex_functione (
       Sors* sors,
    Piscina* piscina,
     vacuum* datum);

#endif
