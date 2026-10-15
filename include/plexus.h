/* plexus.h - Greges braidarum (Artin) B_n, EXACTE: verba, actio in
 * gregem liberum F_n, aequalitas
 *
 * VERBUM: litterae s32, +i = sigma_i, -i = sigma_i^-1, 1 <= i < fila;
 * verbum vacuum = identitas. Plexus valor est (fila, longitudo,
 * litterae); litterae in piscina vocantis, numquam mutantur. Structura
 * publica est: plexus manu factus per plexus_validus probatur, et omnis
 * actio (permutatio, imagines, aequalitas, ansae, in_maiore) plexum
 * invalidum refutat (FALSUM), numquam extra alveum legit.
 *
 * CAUTIO TYPORUM: fila et longitudo i32 sunt (SINE signo), litterae
 * s32. Gyrus retrogradus 'k = longitudo - 1; k >= 0; k--' cum k i32
 * numquam finit: indicem s32 adhibe.
 *
 * FILA: 2 <= fila <= PLEXUS_FILA_MAXIMA (2^20); maius refutatur
 * (FALSUM), numquam truncatur.
 *
 * PERMUTATIO: sigma_i positiones i et i+1 (ab 1 numeratas) commutat,
 * litteris a sinistra ad dextram actis. plexus_permutatio ponit
 * imago[k] = filum (positione initiali, ab 0) quod in positione k (ab
 * 0) finit. Ergo imago_{pq}[k] = imago_p[imago_q[k]].
 *
 * ACTIO IN F_n (Artin): x_1 ... x_n generatores liberi;
 *   sigma_i:    x_i -> x_i x_{i+1} x_i^-1,  x_{i+1} -> x_i
 *   sigma_i^-1: x_i -> x_{i+1},             x_{i+1} -> x_{i+1}^-1 x_i
 *                                                      x_{i+1}
 * ceteri fixi. Plexui a_1 ... a_m automorphismus phi = phi_{a_1} o ...
 * o phi_{a_m} (compositio substitutionum), ergo phi_{pq} = phi_p o
 * phi_q (actio SINISTRA). Actio FIDELIS est (Artin 1925): p = q in B_n
 * si et solum si phi_p = phi_q, ergo plexus_aequales problema verborum
 * EXACTE solvit. plexus_aequales est REFERENTIA (Artin); methodus
 * celerior per ansas: plexus_aequales_ansis.
 *
 * LIMES: imagines verborum exponentialiter crescere possunt (|phi(x)|
 * usque ad 3^m). Omnis functio actionis limitem longitudinis summae
 * accipit; limite superato FALSUM redditur, numquam responsum
 * truncatum.
 *
 * MEMORIA: effecta in piscina vocantis; temporaria in piscinis
 * propriis, destructis ante reditum. Memoria propria deficiens ->
 * FALSUM in actione in F_n; in piscina vocantis (et intra Magnus)
 * allocatio deficiens fatalis est, ut in omni piscina. Purgamenta
 * verborum contrahuntur cum piscina propria quater crevit post
 * contractionem ultimam; plexus_contractiones numerat.
 * FALSUM -> exitus non tangitur.
 *
 * USUS:
 *   Plexus p;
 *   b32    aequales;
 *   (vacuum)plexus_ex_chorda(III, chorda_ex_literis("1 2 1", piscina),
 *       piscina, &p);
 *   si (plexus_aequales(p, q, M, piscina, &aequales)) ...
 *
 * Consilium: project-specs/plexus-plan.md. Vide lib/plexus.worklog.md.
 */
/* <aedilis corpus="lib/plexus.c"/> */
#ifndef PLEXUS_H
#define PLEXUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"

#define PLEXUS_FILA_MAXIMA              1048576
/* Delta longitudinem n(n-1)/2 habet: n <= 65536 ut in 2^31 capiat */
#define PLEXUS_FILA_DIMIDIAE_TORSIONIS  65536

nomen structura {
     i32  fila;         /* 2 <= n <= PLEXUS_FILA_MAXIMA */
     i32  longitudo;
     s32* litterae;     /* +i sigma_i, -i sigma_i^-1, 1 <= i < fila */
} Plexus;

/* verbum gregis liberi F_n: +j = x_j, -j = x_j^-1, j >= 1; semper
 * libere reductum (nulla littera iuxta inversam suam) */
nomen structura {
     i32  longitudo;
     s32* litterae;
} VerbumLiberum;


/* ==================================================
 * Verba
 * ================================================== */

/* litterae copiantur; FALSUM si fila extra 2..PLEXUS_FILA_MAXIMA aut
 * littera 0 vel |littera| >= fila */
b32
plexus_creare (
                i32  fila,
       constans s32* litterae,
                i32  numerus,
            Piscina* piscina,
             Plexus* exitus);

/* textus: integri signati, character ' ' (solum) separati ("1 -2 1");
 * spatia multiplicia, initialia, finalia et cifrae '0' initiales
 * admittuntur ("-0002" = -2); '+', tabula, linea nova non. Vacuus aut
 * spatia sola = identitas. FALSUM si malformatus aut littera extra
 * 1..fila-1 (exitus non tangitur, piscina refecta). Plexus in alveo
 * analysis ipso: nihil aliud in piscina relinquitur. */
b32
plexus_ex_chorda (
         i32  fila,
      chorda  textus,
     Piscina* piscina,
      Plexus* exitus);

/* "1 -2 1"; identitas "" */
chorda
plexus_ad_chordam (
     Plexus  p,
    Piscina* piscina);

/* a_m^-1 ... a_1^-1 */
Plexus
plexus_inversum (
     Plexus  p,
    Piscina* piscina);

/* p deinde q; FALSUM si fila diversa */
b32
plexus_iungere (
     Plexus  p,
     Plexus  q,
    Piscina* piscina,
     Plexus* exitus);

/* reductio libera: sigma_i sigma_i^-1 et sigma_i^-1 sigma_i delentur
 * (solum - relationes braidarum non adhibentur) */
Plexus
plexus_reducere (
     Plexus  p,
    Piscina* piscina);

/* summa signorum litterarum (homomorphismus B_n -> Z) */
s64
plexus_summa_exponentium (
    Plexus p);

/* VERUM si plexus validus: fila 2..PLEXUS_FILA_MAXIMA, litterae non
 * NIHIL si longitudo > 0, omnis littera 1 <= |littera| < fila. O(m). */
b32
plexus_validus (
    Plexus p);

/* imago[0 .. fila-1]: vide PERMUTATIO supra. FALSUM si plexus
 * invalidus (imago non tangitur) */
b32
plexus_permutatio (
    Plexus  p,
       i32* imago);

/* Delta = (sigma_1 ... sigma_{n-1}) (sigma_1 ... sigma_{n-2}) ...
 * sigma_1, dimidia torsio Garside; Delta^2 centrum B_n generat.
 * FALSUM si fila extra 2..PLEXUS_FILA_DIMIDIAE_TORSIONIS */
b32
plexus_dimidia_torsio (
         i32  fila,
     Piscina* piscina,
      Plexus* exitus);


/* ==================================================
 * Actio in gregem liberum (Artin) et aequalitas
 * ================================================== */

/* phi_p(x_j), 1 <= j <= fila. limes: longitudo maxima, post quamque
 * litteram, summae imaginum omnium generatorum, et cuiusque producti
 * ante reductionem (supra 2^31 - 1 non repraesentabilis); limes etiam
 * memoriam propriam terminat (~ 4 * limes octeti per productum).
 * FALSUM si plexus invalidus, j extra, limes superatus, aut memoria
 * propria deficit. Temporaria in piscinis propriis. */
b32
plexus_imago_libera (
           Plexus  p,
              i32  j,
              i64  limes,
          Piscina* piscina,
    VerbumLiberum* exitus);

/* imagines omnium generatorum: (*exitus)[j - 1] = phi_p(x_j), j = 1 ..
 * fila; idem limes et eadem refutatio quae plexus_imago_libera */
b32
plexus_imagines_liberae (
            Plexus   p,
               i64   limes,
           Piscina*  piscina,
     VerbumLiberum** exitus);

/* *aequales = (p == q in B_n): r = reductio(p q^-1); r vacuum -> VERUM
 * statim (sine limite); summa exponentium et permutatio r prius
 * (necessaria), deinde phi_r = identitas. FALSUM si plexus invalidus,
 * fila diversa, aut limes superatus (tunc *aequales non tangitur).
 * piscina ut alveus temporarius: usus ad statum initii reficitur. */
b32
plexus_aequales (
     Plexus  p,
     Plexus  q,
        i64  limes,
    Piscina* piscina,
        b32* aequales);

/* "1 2 -1"; identitas "" */
chorda
verbum_liberum_ad_chordam (
    VerbumLiberum  w,
          Piscina* piscina);

/* plexus p (n fila) in B_fila: littera +-i -> +-(i + translatio).
 * FALSUM si p invalidus, fila extra 2..PLEXUS_FILA_MAXIMA aut p.fila +
 * translatio > fila */
b32
plexus_in_maiore (
     Plexus  p,
        i32  fila,
        i32  translatio,
    Piscina* piscina,
     Plexus* exitus);


/* ==================================================
 * Actio Dynnikov in ansas (laminae integrae)
 * ==================================================
 *
 * ANSA: lamina integra (multicurva) in disco cum fila punctis, per
 * coordinatas Dynnikov u = (a_1 .. a_{n-2}, b_1 .. b_{n-2}), n = fila
 * >= 3. Omnis vector integer laminam describit (Dynnikov 2002;
 * citatum, non hic probatum).
 * Valores Magnus: crescentia numquam superfluit.
 *
 * FONS: J.-L. Thiffeault, "Braids of entangled particle trajectories",
 * Chaos 20, 017516 (2010), arXiv:0906.3647: coordinatae aeq. (3)-(4),
 * L(u) aeq. (5)-(6), regulae aeq. (7)-(15), eaedem in appendice A.2
 * (loopsigma, loopinter) - ambae transcriptiones congruunt.
 * CONVENTIO fontis: sigma_i puncta i et i+1 HOROLOGII SENSU commutat
 * (Birman contra); verbum ansae a sinistra ad dextram adhibetur, ergo
 * actio DEXTRA: u . (pq) = (u . p) . q (probatum), contra actionem in
 * F_n quae sinistra est.
 *
 * DISCUS: in D_n torsio plena Delta^2 (torsio circa marginem) omnem
 * ansam clausam fixam relinquit, ergo actio in D_n Delta^2 non videt.
 * plexus_aequales_ansis plexum in D_{n+2} (punctis mediis) agit, ubi
 * Delta^2 punctorum mediorum non centralis est.
 *
 * Regulae per partes lineares sunt: f+ = max(f, 0), f- = min(f, 0);
 * omnis generator stratum ReLU integrum est. */

nomen structura {
        i32  fila;      /* puncta, >= 3 */
     Magnus* a;         /* fila - 2 */
     Magnus* b;         /* fila - 2 */
} Ansa;

/* coordinatae copiantur; FALSUM si fila extra 3..PLEXUS_FILA_MAXIMA */
b32
ansa_ex_coordinatis (
                i32  fila,
    constans Magnus* a,
    constans Magnus* b,
            Piscina* piscina,
               Ansa* exitus);

/* ansa simplex circa puncta k et k+1 (1 <= k < fila): a = 0, b_k = 1
 * (si k <= n-2), b_{k-1} = -1 (si k >= 2), ceterae 0 */
b32
ansa_circa (
         i32  fila,
         i32  k,
     Piscina* piscina,
        Ansa* exitus);

/* ansa post plexum (litterae a sinistra; actio dextra). FALSUM si p
 * invalidus, ansa invalida (fila < 3, coordinatae NIHIL) aut p.fila !=
 * u.fila. Temporaria in piscina propria; effectum in piscina vocantis.
 */
b32
ansa_agere (
        Ansa  u,
      Plexus  p,
     Piscina* piscina,
        Ansa* exitus);

/* L(u): sectiones minimae ansae cum linea horizontali per puncta (aeq.
 * 5-6) */
Magnus
ansa_sectiones (
        Ansa  u,
     Piscina* piscina);

b32
ansae_aequales (
    Ansa u,
    Ansa v);

/* "(a_1 ... a_{n-2}; b_1 ... b_{n-2})" */
chorda
ansa_ad_chordam (
        Ansa  u,
     Piscina* piscina);

/* *aequales = (p == q in B_n) per actionem in ansas circa puncta
 * vicina D_{n+2}: r = p q^-1 in punctis mediis, aequales si r omnes
 * ansas E_1 .. E_{n+1} fixas relinquit. Congruentia cum Artin
 * (plexus_aequales) exhaustive probata pro verbis brevibus (B_3 <= 6,
 * B_4 <= 5, B_5 <= 4 litterae; probatio_plexus); fidelitas generalis
 * hic non probatur - plexus_aequales manet referentia.
 * FALSUM si plexus invalidus aut fila diversa. piscina ad statum
 * initii reficitur. */
b32
plexus_aequales_ansis (
     Plexus  p,
     Plexus  q,
    Piscina* piscina,
        b32* aequales);

/* contractiones piscinarum propriarum (imagines in F_n, ansae) ab
 * initio processus: instrumentum probationis et mensurae, non status
 * logicus (nullum effectum mutat) */
i64
plexus_contractiones (vacuum);

#endif /* PLEXUS_H */
