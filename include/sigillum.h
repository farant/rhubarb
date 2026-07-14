/* sigillum.h - sigillum contenti (SHA-256, FIPS 180-4)
 *
 * NOMEN NONDUM SIGILLATUM (tabularium-gradus.md phase A) - "sigillum"
 * quia friatio tabularum (friatio.h) friat ad dispergendos claves,
 * hoc autem SIGILLAT: ubi friatio identitas EST, resistentia
 * collisionibus onus fert. Sigillum rei = identitas contenti per
 * tabularium/smaragda-C (entia codicis, versiones reductorum,
 * repositorium CAS).
 *
 * Usus:
 *   Sigillum s = sigillum_computare(datum, mensura);
 *   character hex[LXV];
 *   sigillum_hex(&s, hex);
 *
 *   SigillumContextus ctx;
 *   sigillum_incipere(&ctx);
 *   sigillum_addere(&ctx, pars1, m1);
 *   sigillum_addere(&ctx, pars2, m2);
 *   s = sigillum_finire(&ctx);
 */

#ifndef SIGILLUM_H
#define SIGILLUM_H

#include "latina.h"

#define SIGILLUM_OCTETI 32
#define SIGILLUM_HEX_MENSURA 65   /* 64 characteres + terminator */

nomen structura {
    i8 octeti[SIGILLUM_OCTETI];
} Sigillum;

nomen structura {
    i32 status[8];       /* H0..H7 */
    i8  massa[64];       /* bloccus partialis */
    i32 massa_mensura;   /* octeti in massa (0..63) */
    i64 totalis;         /* octeti toti visi */
} SigillumContextus;

/* uno ictu */
Sigillum
sigillum_computare (
    constans vacuum* datum,
    memoriae_index   mensura);

/* incrementaliter */
vacuum
sigillum_incipere (
    SigillumContextus* contextus);

vacuum
sigillum_addere (
    SigillumContextus* contextus,
    constans vacuum*   datum,
    memoriae_index     mensura);

Sigillum
sigillum_finire (
    SigillumContextus* contextus);

/* effusio >= SIGILLUM_HEX_MENSURA; minusculae, terminata */
vacuum
sigillum_hex (
    constans Sigillum* sigillum,
    character*         effusio);

b32
sigillum_aequale (
    constans Sigillum* a,
    constans Sigillum* b);

#endif /* SIGILLUM_H */
