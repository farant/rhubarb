/* cyclotomia.h - Integri cyclotomici EXACTI: Z[zeta_n] = Z[t]/Phi_n(t)
 *
 * Elementum = polynomium in zeta reductum ad gradum < phi(n) (basis
 * potentiarum 1, zeta, ..., zeta^(phi(n)-1)), coefficientibus magnus:
 * exactum, sine exundatione (knotapel Cyc8 'long' habebat). Forma
 * canonica unica, ergo aequalitas = aequalitas polynomiorum.
 *
 * Contextus per n (Cyclotomia): n, phi(n), Phi_n et tabula zeta^k ->
 * basis (k < n), ergo reductio = lectio tabulae. cyclotomia_anulus
 * anulum pro matrix reddit (integrum, non corpus), sicut
 * anulus_residuorum.
 *
 * DECISIONES: elementa ab omnibus decisionibus EXACTE tractantur
 * (aequalis, est_radix, norma, vestigium, modulus_quadratus);
 * cyclotomicus_ad_ostendendum SOLUM ad ostendendum (f64 interius) -
 * textum reddit, non numeros, ne in comparationes intret.
 *
 * USUS:
 *   Cyclotomia* r8 = cyclotomia_creare(VIII, piscina);
 *   Cyclotomicus z = cyclotomicus_radix(r8, I, piscina);
 *   Cyclotomicus v;
 *   (vacuum)cyclotomicus_ex_polynomio(r8, jones, II, piscina, &v);
 *       (* V(zeta_8^2) = V(i) *)
 *
 * Vide lib/cyclotomia.worklog.md.
 */
/* <aedilis corpus="lib/cyclotomia.c"/> */
#ifndef CYCLOTOMIA_H
#define CYCLOTOMIA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "polynomium.h"
#include "anulus.h"

#define CYCLOTOMIA_ORDO_MAXIMUS M

/* contextus Z[zeta_n]: opacus */
nomen structura Cyclotomia Cyclotomia;

/* forma canonica: polynomium in zeta, gradus < phi(n), imus >= 0;
 * PRIVATUM - per functiones legendum */
nomen structura {
    Polynomium p;
} Cyclotomicus;

/* Phi_n(t), n >= 1, per Phi_n = (t^n - 1) / prod_{d | n, d < n} Phi_d.
 * FALSUM si n nullus aut n > CYCLOTOMIA_ORDO_MAXIMUS. */
b32
polynomium_cyclotomicum (
           i32  n,
       Piscina* piscina,
    Polynomium* exitus);

/* contextus Z[zeta_n]; NIHIL si n < 1 aut n > CYCLOTOMIA_ORDO_MAXIMUS.
 * Memoria ~ n * phi(n) coefficientes (tabula potentiarum). */
Cyclotomia*
cyclotomia_creare (
         i32  n,
     Piscina* piscina);

/* n */
i32
cyclotomia_ordo (
    constans Cyclotomia* r);

/* phi(n) = dimensio basis */
i32
cyclotomia_gradus (
    constans Cyclotomia* r);

/* Phi_n contextus */
Polynomium
cyclotomia_polynomium (
    constans Cyclotomia* r);

/* anulus Z[zeta_n] (elementa Cyclotomicus, titulus "Z[zeta_n]") pro
 * matrix: integrum, non corpus, non Euclideus; divide_exacte per
 * normam. Una descriptio per contextum. */
constans Anulus*
cyclotomia_anulus (
    constans Cyclotomia* r);


/* ==================================================
 * Constructio
 * ================================================== */

Cyclotomicus
cyclotomicus_nullum (
    vacuum);

Cyclotomicus
cyclotomicus_integer (
    constans Cyclotomia* r,
                 Magnus  c,
                Piscina* piscina);

/* zeta^k, k quilibet (modulo n, negativus licet) */
Cyclotomicus
cyclotomicus_radix (
    constans Cyclotomia* r,
                    s32  k,
                Piscina* piscina);

/* p(zeta^k): variabilis polynomii (exponentes Laurent quilibet)
 * substituitur et reducitur - e.g. Jones V(t) ad t = zeta^k, uncinus
 * ad A = zeta^k. k = 1: reductio simplex. FALSUM nunquam nisi memoria;
 * b32 pro consensu. */
b32
cyclotomicus_ex_polynomio (
    constans Cyclotomia* r,
             Polynomium  p,
                    s32  k,
                Piscina* piscina,
           Cyclotomicus* exitus);

/* coefficiens zeta^j in basi (0 <= j < phi(n)) */
Magnus
cyclotomicus_coefficiens (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    i32  j);


/* ==================================================
 * Arithmetica
 * ================================================== */

b32
cyclotomicus_est_nullum (
    Cyclotomicus a);

b32
cyclotomicus_aequalis (
    Cyclotomicus a,
    Cyclotomicus b);

/* VERUM si a in Z (gradus 0); *valor scribitur si non NIHIL */
b32
cyclotomicus_est_integer (
    Cyclotomicus  a,
          Magnus* valor);

Cyclotomicus
cyclotomicus_adde (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina);

Cyclotomicus
cyclotomicus_subtrahe (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina);

Cyclotomicus
cyclotomicus_nega (
    Cyclotomicus  a,
         Piscina* piscina);

Cyclotomicus
cyclotomicus_multiplica (
    constans Cyclotomia* r,
           Cyclotomicus  a,
           Cyclotomicus  b,
                Piscina* piscina);

/* a^e, e >= 0 (quadrando) */
Cyclotomicus
cyclotomicus_potentia (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    i32  e,
                Piscina* piscina);

/* q = a / b in Z[zeta_n]: q = a * prod_{sigma != 1} sigma(b) / N(b).
 * FALSUM si b nullum aut b non dividit a (exitus non tangitur). */
b32
cyclotomicus_divide_exacte (
    constans Cyclotomia* r,
           Cyclotomicus  a,
           Cyclotomicus  b,
                Piscina* piscina,
           Cyclotomicus* exitus);


/* ==================================================
 * Galois et invariantes EXACTAE
 * ================================================== */

/* sigma_j: zeta -> zeta^j; FALSUM si gcd(j, n) != 1 */
b32
cyclotomicus_automorphismus (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    s32  j,
                Piscina* piscina,
           Cyclotomicus* exitus);

/* conjugatum complexum = sigma_{-1} */
Cyclotomicus
cyclotomicus_conjugatum (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina);

/* N(a) = prod_{gcd(j, n) = 1} sigma_j(a), in Z */
Magnus
cyclotomicus_norma (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina);

/* Tr(a) = summa_{gcd(j, n) = 1} sigma_j(a), in Z */
Magnus
cyclotomicus_vestigium (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina);

/* |a|^2 = a * conjugatum(a): realis (in Z[zeta + zeta^-1]), EXACTUM */
Cyclotomicus
cyclotomicus_modulus_quadratus (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina);

/* a = signum * zeta^k? (radix unitatis, angulus EXACTUS). VERUM et
 * *signum (+1/-1), *k (0 <= k < n) - signum +1 praefertur (n par:
 * -zeta^k = zeta^(k + n/2)). */
b32
cyclotomicus_est_radix (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    s32* signum,
                    i32* k);


/* ==================================================
 * Textus
 * ================================================== */

/* "z^3 - 2z + 1": littera 'z' = zeta_n; nullum = "0" */
chorda
cyclotomicus_ad_chordam (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                Piscina* piscina);

/* polynomium in 'z' (Laurent licet), reductum */
b32
cyclotomicus_ex_chorda (
    constans Cyclotomia* r,
                 chorda  textus,
                Piscina* piscina,
           Cyclotomicus* exitus);

/* AD OSTENDENDUM SOLUM: "0.7071 + 0.7071i" cum digitis decimalibus
 * datis (f64 interius, cos/sin). Numquam ad decisiones: aequalis,
 * est_radix, modulus_quadratus exacta sunt. */
chorda
cyclotomicus_ad_ostendendum (
    constans Cyclotomia* r,
           Cyclotomicus  a,
                    i32  digiti,
                Piscina* piscina);

#endif /* CYCLOTOMIA_H */
