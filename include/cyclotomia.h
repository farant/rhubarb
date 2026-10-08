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

/* Elementum SIGNATUM anulo suo: operationes anulum ex elementis legunt,
 * anuli mixti refutantur (aequalis(zeta_8, zeta_16) FALSUM; recensio
 * I). anulus NIHIL = INVALIDUM: ex anulis mixtis ortum, sicut NaN
 * propagatur (cyclotomicus_est_validum). Forma canonica: polynomium in
 * zeta, gradus < phi(n), imus >= 0. PRIVATUM - per functiones
 * legendum. */
nomen structura {
     constans Cyclotomia* anulus;
              Polynomium  p;
} Cyclotomicus;

/* Phi_n(t), n >= 1, per Phi_n = (t^n - 1) / prod_{d | n, d < n} Phi_d.
 * FALSUM si n nullus aut n > CYCLOTOMIA_ORDO_MAXIMUS. */
b32
polynomium_cyclotomicum (
           i32  n,
       Piscina* piscina,
    Polynomium* exitus);

/* contextus Z[zeta_n]; NIHIL si n < 1 aut n > CYCLOTOMIA_ORDO_MAXIMUS.
 * Memoria: tabulae potentiarum (n * phi(n) coefficientes, plerumque
 * parvi); n = 840: ~6 MB, creatio < 5 ms (recensio). */
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
 * normam. Una descriptio per contextum; elementa alterius anuli
 * refutantur (FALSUM). */
constans Anulus*
cyclotomia_anulus (
    constans Cyclotomia* r);


/* ==================================================
 * Constructio (contextum accipiunt)
 * ================================================== */

Cyclotomicus
cyclotomicus_nullum (
    constans Cyclotomia* r);

Cyclotomicus
cyclotomicus_integer (
    constans Cyclotomia* r,
                 Magnus  c,
                Piscina* piscina);

/* zeta^k, k quilibet (modulo n, negativus licet). Elementum tabulam
 * contextus PARTITUR (immutabile, sicut omnia polynomia): non copia. */
Cyclotomicus
cyclotomicus_radix (
    constans Cyclotomia* r,
                    s32  k,
                Piscina* piscina);

/* p(zeta^k): variabilis polynomii (exponentes Laurent quilibet)
 * substituitur et reducitur - e.g. Jones V(t) ad t = zeta^k, uncinus
 * ad A = zeta^k. k = 1: reductio simplex. Etiam immersio: a in
 * Z[zeta_8] -> Z[zeta_16] per cyclotomicus_ex_polynomio(r16, a.p, II)
 * (zeta_8 = zeta_16^2). b32 pro consensu (FALSUM si r NIHIL). */
b32
cyclotomicus_ex_polynomio (
    constans Cyclotomia* r,
             Polynomium  p,
                    s32  k,
                Piscina* piscina,
           Cyclotomicus* exitus);

/* summa c[k] zeta^k, k < numerus (quilibet: reducitur) - ex
 * arithmetica 's64' demonstrationum (Cyc8: numerus IV) */
b32
cyclotomicus_ex_s64 (
    constans Cyclotomia* r,
           constans s64* c,
                    i32  numerus,
                Piscina* piscina,
           Cyclotomicus* exitus);


/* ==================================================
 * Lectio
 * ================================================== */

/* VERUM nisi ex anulis mixtis ortum */
b32
cyclotomicus_est_validum (
    Cyclotomicus a);

/* contextus elementi; NIHIL si invalidum */
constans Cyclotomia*
cyclotomicus_anulus (
    Cyclotomicus a);

/* coefficiens zeta^j in basi (0 <= j < phi(n)); 0 si invalidum */
Magnus
cyclotomicus_coefficiens (
    Cyclotomicus a,
             i32 j);

/* phi(n) coefficientes in exitus (basis potentiarum); FALSUM si
 * invalidum aut coefficiens extra s64 (exitus tunc non fidus) */
b32
cyclotomicus_ad_s64 (
    Cyclotomicus  a,
             s64* exitus);


/* ==================================================
 * Arithmetica (anulus ex elementis; mixti -> invalidum)
 * ================================================== */

/* FALSUM si invalidum */
b32
cyclotomicus_est_nullum (
    Cyclotomicus a);

/* FALSUM si anuli diversi aut invalidum */
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
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina);

/* a^e, e >= 0 (quadrando) */
Cyclotomicus
cyclotomicus_potentia (
    Cyclotomicus  a,
             i32  e,
         Piscina* piscina);

/* q = a / b in Z[zeta_n]: q = a * prod_{sigma != 1} sigma(b) / N(b).
 * FALSUM si b nullum, b non dividit a, aut anuli mixti (exitus non
 * tangitur). */
b32
cyclotomicus_divide_exacte (
    Cyclotomicus  a,
    Cyclotomicus  b,
         Piscina* piscina,
    Cyclotomicus* exitus);


/* ==================================================
 * Galois et invariantes EXACTAE
 * ================================================== */

/* sigma_j: zeta -> zeta^j; FALSUM si gcd(j, n) != 1 aut invalidum */
b32
cyclotomicus_automorphismus (
    Cyclotomicus  a,
             s32  j,
         Piscina* piscina,
    Cyclotomicus* exitus);

/* conjugatum complexum = sigma_{-1} */
Cyclotomicus
cyclotomicus_conjugatum (
    Cyclotomicus  a,
         Piscina* piscina);

/* N(a) = prod_{gcd(j, n) = 1} sigma_j(a), in Z; FALSUM si invalidum */
b32
cyclotomicus_norma (
    Cyclotomicus  a,
         Piscina* piscina,
          Magnus* exitus);

/* Tr(a) = summa_{gcd(j, n) = 1} sigma_j(a), in Z; FALSUM si invalidum
 * (vestigium 0 legitimum est - ergo b32) */
b32
cyclotomicus_vestigium (
    Cyclotomicus  a,
         Piscina* piscina,
          Magnus* exitus);

/* |a|^2 = a * conjugatum(a): realis (in Z[zeta + zeta^-1]), EXACTUM */
Cyclotomicus
cyclotomicus_modulus_quadratus (
    Cyclotomicus  a,
         Piscina* piscina);

/* a = signum * zeta^k? (radix unitatis, angulus EXACTUS). VERUM et
 * *signum (+1/-1), *k (0 <= k < n) - signum +1 praefertur (n par:
 * -zeta^k = zeta^(k + n/2)). FALSUM si non radix aut invalidum. */
b32
cyclotomicus_est_radix (
    Cyclotomicus  a,
             s32* signum,
             i32* k);


/* ==================================================
 * Textus
 * ================================================== */

/* "z^3 - 2z + 1": littera 'z' = zeta_n; nullum = "0"; invalidum =
 * "invalidum" */
chorda
cyclotomicus_ad_chordam (
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
 * datis (maximum XV; |x| >= 10^15 per notationem e). f64 interius
 * (cos/sin): error absolutus ~ phi(n) * max|c| * 2^-52 - cancellatio
 * TACITA est ((sqrt2 - 1)^60 = 1e-23 ut milliones ostenditur). Numquam
 * ad decisiones: aequalis, est_radix, modulus_quadratus exacta sunt.
 * invalidum = "invalidum". */
chorda
cyclotomicus_ad_ostendendum (
    Cyclotomicus  a,
             i32  digiti,
         Piscina* piscina);

#endif /* CYCLOTOMIA_H */
