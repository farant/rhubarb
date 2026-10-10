/* radices.h - Radices reales polynomiorum integrorum, EXACTE
 *
 * Numerus algebraicus realis = RADIX polynomii f in Z[t] intra
 * intervallum (infra, supra) quod radicem f UNAM continet; infra ==
 * supra si radix est hic numerus rationalis exacte. f liber quadratis,
 * primitivus, coefficiente summo > 0; termini intervalli dyadici (aut
 * fractio data) - radicem suam numquam, radices ALIAS f fortasse (sqrt 3
 * in (1, 2) ubi 1, 2 quoque radices). Radices diversorum polynomiorum
 * comparantur (extensio corpora mixta refutat): aequalitas per
 * divisorem communem decernitur, numquam per latitudinem.
 *
 * ISOLATIO: regula signorum Cartesii cum bisectione (Vincent - Collins
 * - Akritas) super partem liberam quadratis, a limite Cauchy; radix
 * rationalis in medio puncto inventa exacte redditur.
 *
 * Valores in piscina, sicut polynomium; nihil liberatur.
 *
 * USUS:
 *   RadixRealis* r;
 *   i32          n;
 *   si (radices_reales(f, piscina, &r, &n))  (* r[0] < ... < r[n-1] *)
 *       (vacuum)radix_ad_chordam(r[0], XX, piscina, &textus);
 *
 * Vide lib/radices.worklog.md.
 */
/* <aedilis corpus="lib/radices.c"/> */
#ifndef RADICES_H
#define RADICES_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include "polynomium.h"

nomen structura {
    Polynomium f;                  /* liber quadratis, primitivus, lc > 0 */
       Fractio infra;
       Fractio supra;              /* infra == supra -> radix rationalis
                                    * (non vice versa: 1/5 ex 5t - 1
                                    * intervallum est); aliter radix in
                                    * (infra, supra) APERTO, termini radices
                                    * f aliae esse possunt */
} RadixRealis;

/* omnes radices reales DISTINCTAE f, ordine crescente (multiplicitas
 * abiecta); *exitus in piscina, *numerus = 0 si nullae. FALSUM si f
 * nullum aut exponens negativus adest. */
b32
radices_reales (
     Polynomium   f,
        Piscina*  piscina,
    RadixRealis** exitus,
            i32*  numerus);

/* q exacte, radix (den t - num) */
RadixRealis
radix_ex_fractione (
    Fractio  q,
    Piscina* piscina);

/* signum(a - b) EXACTE; FALSUM solum si limes bisectionum superatus
 * (exitus non tangitur) */
b32
radix_compara (
    RadixRealis  a,
    RadixRealis  b,
        Piscina* piscina,
            s32* exitus);

/* signum h(a), h in Z[t] (exponentes >= 0): 0 sse a radix gcd(f, h);
 * FALSUM sicut radix_compara aut exponens negativus in h */
b32
radix_signum_polynomii (
     Polynomium  h,
    RadixRealis  a,
        Piscina* piscina,
            s32* exitus);

/* intervallum angustatum infra latitudinem (> 0); radix rationalis
 * manet punctum */
b32
radix_angusta (
    RadixRealis  a,
        Fractio  latitudo,
        Piscina* piscina,
    RadixRealis* exitus);

/* decimalis CERTA: n digiti post punctum, versus nullum truncata
 * ("-1.4142" pro -sqrt 2, n = 4; "-0.00" pro -0.003, n = 2 - signum
 * servatur; n = 0 sine puncto: "1"). FALSUM si plus quam ~600 digiti
 * (limes bisectionum, MMXLVIII) */
b32
radix_ad_chordam (
    RadixRealis  a,
            i32  digiti,
        Piscina* piscina,
         chorda* exitus);

#endif /* RADICES_H */
