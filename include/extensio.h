/* extensio.h - Corpora numerorum algebraicorum EXACTA: Q(alpha)
 *
 * Corpus K = Q(alpha) = Q[t]/f(t), f monicus in Z[t], irreducibilis,
 * gradus d. Elementum = numerator(alpha) / denominator: numerator
 * polynomium in alpha coefficientibus magnus, gradus < d; denominator
 * magnus > 0; gcd(contentum numeratoris, denominator) = 1; nullum =
 * 0/1. Forma canonica unica, ergo aequalitas = aequalitas partium.
 * Exactum, sine exundatione: coefficientes magnus.
 *
 * Elementa SIGNATA corpore suo (sicut cyclotomia): operationes corpus
 * ex elementis legunt, corpora mixta refutantur; corpus NIHIL =
 * INVALIDUM, ex corporibus mixtis ortum, sicut NaN propagatur.
 * IDENTITAS CORPORIS PER INDICEM (sicut anulus_residuorum): duo
 * vocamina extensio_quadratica(5) corpora DIVERSA reddunt, quorum
 * elementa non miscentur - corpus semel creatum communica.
 *
 * CORPORA: extensio_quadratica (Q(sqrt d)) et extensio_cosinus
 * (Q(cos 2 pi/n): Q(sqrt 2) n = 8, Q(sqrt 3) n = 12, Q(sqrt 5) n = 5).
 * Uterque irreducibilis per constructionem. Gradus 2 viam celerem
 * habet (inversa forma clausa); aliter inversa per regulam Crameri
 * super matricem multiplicationis (determinans = norma).
 *
 * ORDO: corpus radicem realem f electam portare potest (extensio_
 * ordinata), indice ordine crescente electam (radix 0 = minima). Radix
 * per catenam Sturm isolatur (intervallum rationale unam radicem
 * continens); algebraicus_signum numeratorem super intervallum per
 * arithmeticam intervallorum EXACTAM aestimat et bisecat donec nullum
 * excludatur - terminatur quia numerator gradus < d in alpha non
 * evanescit (f irreducibilis). Testimonium nullius: |numerator(alpha)|
 * >= 1/M^(d-1) nisi nullus (norma integra), ergo intervallum angustius
 * nullum continens nullum PROBAT - f reducibilis (a vocante asserta
 * irreducibilis) refutationem dat, non ansam aeternam.
 *
 * USUS:
 *   Extensio*   k = extensio_quadratica(V, piscina);
 *   Algebraicus a = algebraicus_generator(k, piscina);    (* sqrt 5 *)
 *   Algebraicus phi;
 *   (vacuum)algebraicus_ex_chorda(k, chorda_ex_literis("(a + 1)/2",
 *       piscina), piscina, &phi);                          (* aureus *)
 *
 * Vide lib/extensio.worklog.md.
 */
/* <aedilis corpus="lib/extensio.c"/> */
#ifndef EXTENSIO_H
#define EXTENSIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "magnus.h"
#include "fractio.h"
#include "polynomium.h"
#include "anulus.h"

/* corpus Q(alpha): opacum */
nomen structura Extensio Extensio;

/* Elementum SIGNATUM corpore suo. PRIVATUM - per functiones legendum.
 * corpus NIHIL = invalidum (algebraicus_est_validum). */
nomen structura {
     constans Extensio* corpus;
            Polynomium  numerator;     /* in alpha, 0 <= gradus < d */
                Magnus  denominator;   /* > 0 */
} Algebraicus;


/* ==================================================
 * Corpora
 * ================================================== */

/* Q(sqrt d): f = t^2 - d. NIHIL si d = 0, d = 1, d non liber quadratis,
 * aut |d| >= 2^31. d > 0: alpha = +sqrt d (radix maior), corpus
 * ordinatum; d < 0: sine ordine (Q(i) pro d = -1). */
Extensio*
extensio_quadratica (
         s64  d,
     Piscina* piscina);

/* Q(cos 2 pi/n): alpha = 2 cos(2 pi/n), radix maxima Psi_n, polynomii
 * minimi (Phi_n(t) = t^(phi(n)/2) Psi_n(t + 1/t), n >= 3; Psi_1 = t -
 * 2, Psi_2 = t + 2). Gradus phi(n)/2 (n >= 3), omnes radices reales,
 * corpus ordinatum. n = 3, 4, 6: gradus 1 (alpha -1, 0, 1). NIHIL si n
 * nullus aut n > CYCLOTOMIA_ORDO_MAXIMUS (M). */
Extensio*
extensio_cosinus (
         i32  n,
     Piscina* piscina);

/* d = gradus f */
i32
extensio_gradus (
    constans Extensio* k);

/* f, monicus */
Polynomium
extensio_polynomium (
    constans Extensio* k);

/* VERUM si radix realis electa (signum, compara licent) */
b32
extensio_ordinata (
    constans Extensio* k);

/* index radicis electae (0 = minima realis), -1 si sine ordine */
s32
extensio_radix (
    constans Extensio* k);

/* Q(alpha), f(alpha) = 0: f monicus in Z[t] (exponentes >= 0), gradus
 * >= 1; radix = index radicis realis ordine crescente (0 = minima) ->
 * corpus ordinatum, aut -1 = sine ordine. NIHIL si f non monicus, non
 * liber quadratis, radicem rationalem habet (gradu > 1), aut radix
 * extra [-1, radices reales). Gradu 2-3 sine radice rationali
 * irreducibilis est; gradu >= 4 VOCANS irreducibilitatem asserit
 * (divisor nullius in inversa refutatur, signum testimonio nullius
 * refutat). SUMPTUS: catena Sturm crescit ut d^3 (gradu ~200 centena
 * MB); familiae nominatae (quadratica, cosinus) eam vitant. */
Extensio*
extensio_ex_polynomio (
    Polynomium  f,
           s32  radix,
       Piscina* piscina);

/* numerus radicum realium DISTINCTARUM f (Sturm); FALSUM si f nullum,
 * constans, aut exponentes negativos habet */
b32
extensio_radices_reales (
    Polynomium  f,
       Piscina* piscina,
           i32* exitus);

/* corpus pro matrix (elementa Algebraicus, titulus "Q(...)"): corpus et
 * integrum; divide_exacte = divisio (FALSUM solum si divisor nullus).
 * Una descriptio per corpus; elementa alterius corporis refutantur. */
constans Anulus*
extensio_anulus (
    constans Extensio* k);


/* ==================================================
 * Elementa
 * ================================================== */

/* q in K */
Algebraicus
algebraicus_ex_fractione (
     constans Extensio* k,
               Fractio  q,
               Piscina* piscina);

/* alpha (reductum: gradus 1 -> rationalis) */
Algebraicus
algebraicus_generator (
    constans Extensio* k,
              Piscina* piscina);

/* p(alpha) / denominator, p Laurent quilibet (exponentes negativi per
 * alpha^-1; exponentes magni per potentias, non per Hornerum densum).
 * FALSUM si denominator nullus, k NIHIL, aut exponens negativus cum
 * f(0) = 0 (alpha = 0 non invertibilis). */
b32
algebraicus_ex_polynomio (
     constans Extensio* k,
            Polynomium  p,
                Magnus  denominator,
               Piscina* piscina,
           Algebraicus* exitus);

/* coefficiens alpha^j (0 <= j < d) ut fractio; 0 si invalidum aut j
 * extra */
Fractio
algebraicus_coefficiens (
    Algebraicus  a,
            i32  j,
        Piscina* piscina);

/* VERUM nisi ex corporibus mixtis ortum */
b32
algebraicus_est_validum (
    Algebraicus a);

/* corpus elementi; NIHIL si invalidum */
constans Extensio*
algebraicus_corpus (
    Algebraicus a);


/* ==================================================
 * Arithmetica (corpus ex elementis; mixta -> invalidum)
 * ================================================== */

Algebraicus
algebraicus_adde (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina);

Algebraicus
algebraicus_subtrahe (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina);

Algebraicus
algebraicus_nega (
    Algebraicus  a,
        Piscina* piscina);

Algebraicus
algebraicus_multiplica (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina);

/* 1/a; FALSUM si a nullum, invalidum, aut divisor nullius (f
 * reducibilis) */
b32
algebraicus_inversum (
    Algebraicus  a,
        Piscina* piscina,
    Algebraicus* exitus);

/* a/b; FALSUM si b nullum aut corpora mixta */
b32
algebraicus_divide (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina,
    Algebraicus* exitus);

/* a^e, e quilibet (negativus per inversam); FALSUM si a nullum et e <
 * 0, aut invalidum */
b32
algebraicus_potentia (
    Algebraicus  a,
            s32  e,
        Piscina* piscina,
    Algebraicus* exitus);


/* ==================================================
 * Exacta
 * ================================================== */

/* FALSUM si corpora diversa aut invalidum */
b32
algebraicus_aequalis (
    Algebraicus a,
    Algebraicus b);

b32
algebraicus_est_nullum (
    Algebraicus a);

/* a in Q? valor optionalis (NIHIL licet) */
b32
algebraicus_est_rationalis (
    Algebraicus  a,
        Piscina* piscina,
        Fractio* valor);

/* N_{K/Q}(a) = det matricis multiplicationis; FALSUM si invalidum */
b32
algebraicus_norma (
    Algebraicus  a,
        Piscina* piscina,
        Fractio* exitus);

/* Tr_{K/Q}(a) = vestigium matricis multiplicationis */
b32
algebraicus_vestigium (
    Algebraicus  a,
        Piscina* piscina,
        Fractio* exitus);


/* ==================================================
 * Textus (exactus, relegibilis)
 * ================================================== */

/* "a^2 - 3", "(a + 1)/2", "3a/2", "1/2": numerator in littera 'a'
 * (polynomium_ad_chordam), "/denominator" si != 1; "invalidum" */
chorda
algebraicus_ad_chordam (
    Algebraicus  a,
        Piscina* piscina);

/* inversa ad_chordam: "P", "P/D" aut "(P)/D", P polynomium in 'a'
 * (Laurent licet), D integer non nullus. FALSUM si malformatum. */
b32
algebraicus_ex_chorda (
     constans Extensio* k,
                chorda  textus,
               Piscina* piscina,
           Algebraicus* exitus);


/* ==================================================
 * Ordo (solum si extensio_ordinata)
 * ================================================== */

/* signum a ad radicem electam: -1, 0, +1 EXACTUM. FALSUM si
 * invalidum, corpus sine ordine, aut numerator(alpha) = 0 cum
 * numerator non nullus PROBATUM (testimonio |N| >= 1: f
 * reducibilis). */
b32
algebraicus_signum (
    Algebraicus  a,
        Piscina* piscina,
            s32* exitus);

/* signum(a - b); FALSUM sicut algebraicus_signum aut corpora mixta */
b32
algebraicus_compara (
    Algebraicus  a,
    Algebraicus  b,
        Piscina* piscina,
            s32* exitus);

#endif /* EXTENSIO_H */
