/* agmen_interna.h - INTERNA agminis: viae scalares RELATAE. NON API -
 * solum lib/agmen.c et probationes id includunt; consumptores agmen.h
 * solum. Praefixum 'agminis_' (genetivus) ab API 'agmen_' distinguit.
 *
 * Viae scalares: idem calculus ac nuclei publici, sed ansae sub clang
 * NON vectorizatae (pragma) - probationibus oraculum (nuclei vectorizati
 * contra eas, elementum per elementum, BIT PRO BIT), mensurae basis.
 * Contractus idem ac in agmen.h. */

#ifndef AGMEN_INTERNA_H
#define AGMEN_INTERNA_H

#include "agmen.h"

vacuum
agminis_adde_modulo_scalaris (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus);

vacuum
agminis_subtrahe_modulo_scalaris (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus);

vacuum
agminis_multiplica_modulo_scalaris (
    constans          i32* a,
    constans          i32* b,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus);

vacuum
agminis_multiplica_adde_modulo_scalaris (
                      i32  c,
    constans          i32* a,
                      i32* exitus,
                      i32  numerus,
    constans AgmenModulus* modulus);

i32
agminis_productum_internum_modulo_scalaris (
    constans          i32* a,
    constans          i32* b,
                      i32  numerus,
    constans AgmenModulus* modulus);

#endif /* AGMEN_INTERNA_H */
