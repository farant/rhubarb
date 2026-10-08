/* norma_gignere.h - valores JSON ficti ex schemate (norma-spec par. IV)
 *
 * Modi: TYPICA (verisimilis), FINES (omnis nodus finitus in margine),
 * INVALIDA (vitium UNUM, via et causa redditae). Semina PER VIAM
 * (sors_derivare): campus additus valores fratrum non mutat.
 * Valores per fictio.h; functio gignens campi ordinarium substituit.
 *
 * Oraculum mutuum (probatio_norma_gignere): TYPICA et FINES iudicium
 * transeunt; INVALIDA vitium unum ad via_fracta cum causa_fracta.
 *
 * STATUS: PROBATUM a Frano 2026-10-08 (norma-plan-1 N0); implementatio in norma-plan-2.
 */
#ifndef NORMA_GIGNERE_H
#define NORMA_GIGNERE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "norma.h"

nomen enumeratio {
    NORMA_TYPICA = 0,
    NORMA_FINES,
    NORMA_INVALIDA
} NormaModusGignendi;

/* INVALIDA: via_fracta + causa_fracta nominant vitium unum infixum.
 * valor NIHIL in INVALIDA = nihil violabile (schema totum apertum
 * aut liberum); via_fracta tunc vacua. TYPICA/FINES: via_fracta vacua. */
nomen structura {
     JsonValor* valor;
        chorda  via_fracta;
    NormaCausa  causa_fracta;
} NormaGenitum;

NormaGenitum
norma_gignere (
        constans Norma* n,
    NormaModusGignendi  modus,
                   s64  semen,
               Piscina* piscina);

#endif /* NORMA_GIGNERE_H */
