/* surdus_interna.h - INTERNA surdi: signum cum gradu decernente. NON
 * API - solum lib/surdus.c et probationes id includunt; consumptores
 * surdus.h solum. Praefixum 'surdi_' (genetivus) ab API 'surdus_'
 * distinguit (surdi_spatium publicum excipitur: genetivus naturalis).
 *
 * Probationes gradum 3 (quadratio) directe contra oraculum agere debent,
 * non sperare ut inputa casu eo perveniant: filtrum = FALSUM gradum 2
 * omittit. */

#ifndef SURDUS_INTERNA_H
#define SURDUS_INTERNA_H

#include "surdus.h"

/* gradus decernens: I nullum aut rationale, II filtrum duplex, III
 * quadratio (maximus per recursionem). Contractus signi idem ac
 * surdus_signum; gradus NIHIL licet. */
b32
surdi_signum_gradu (
    constans SurdiSpatium* sp,
                   Surdus  x,
                      b32  filtrum,
                      s32* exitus,
                      s32* gradus);

#endif /* SURDUS_INTERNA_H */
