/* surdus.h - Surdi: summae radicum quadratarum, signum EXACTUM, celeriter
 *
 * "Surdus": radix irrationalis (interpretes Latini al-Khwarizmi, 'asamm
 * = mutus; Anglice surd). Hic x = sum_S c_S sqrt(prod_{i in S} p_i),
 * S selectio primorum p_1..p_k distinctorum (k <= 3), c_S integri s64.
 * Typus valoris: nulla piscina, nulla allocatio.
 *
 * SIGNUM per tres gradus interiores, omnes EXACTI:
 *   1. nullum: x == 0 sse omnes coefficientes 0 (basis super Q
 *      linearis independens) - gratis;
 *   2. filtrum duplex certificatum: |fl(x)| > limes erroris -> signum
 *      eius (limes demonstratus in lib/surdus.c; contractio FMA limitem
 *      non frangit, rotundationes tantum minuit);
 *   3. quadratio recursiva in s64 custodito: x = a + b sqrt(p),
 *      signum(x) = signum(a) signum(a^2 - p b^2) ubi a, b signis
 *      oppositis.
 * RECUSAT (FALSUM), numquam coniectat: arithmetica quae s64 excederet,
 * et gradus 3 super x minimo non nullo cuius quadrata excederent. Tum
 * vocans ad extensio (magni) recurrit. Rationales: vocans ad integros
 * scalat (D119: comparatio per VIII multiplicata).
 *
 * USUS:
 *   s32 primi[III] = { II, III, V };
 *   SurdiSpatium sp;
 *   Surdus x, y;
 *   s32 s;
 *   si (surdi_spatium(primi, III, &sp)
 *       && surdus_basis(&sp, I, LXX, &x)            70 sqrt2
 *       && surdus_subtrahe(surdus_ex_s64(XCIX), x, &y)
 *       && surdus_signum(&sp, y, &s))                99 - 70 sqrt2 > 0
 *       ...
 *
 * Vide lib/surdus.worklog.md.
 */
/* <aedilis corpus="lib/surdus.c"/> */
#ifndef SURDUS_H
#define SURDUS_H

#include "latina.h"

#define SURDUS_RADICES_MAXIMAE  III


/* ==================================================
 * Spatium (corpus Q(sqrt p_1, ..., sqrt p_k))
 * ================================================== */

/* Membra a surdi_spatium ponuntur, a vocante non mutanda (producta et
 * radices praecomputatae: ansa calida signi et multiplicationis). */
nomen structura {
    s32 primi[III];       /* primi distincti, ordine crescente */
    s32 numerus;          /* k = 0..3 */
    s64 producta[VIII];   /* producta[S] = prod_{i in S} p_i (< 2^45) */
    f64 radices[VIII];    /* fl(sqrt(producta[S])) */
} SurdiSpatium;

/* FALSUM (exitus non tangitur): numerus > 3 aut < 0, primus non primus
 * aut >= 2^15, ordo non stricte crescens */
b32
surdi_spatium (
    constans          s32* primi,
                      s32  numerus,
             SurdiSpatium* exitus);


/* ==================================================
 * Surdus
 * ================================================== */

/* c[S], S mascula bitorum super primos (bitum i <-> primi[i]) */
nomen structura {
    s64 c[VIII];
} Surdus;

Surdus
surdus_ex_s64 (
    s64 x);

/* c sqrt(prod primorum in 'selectio'); FALSUM si selectio >= 2^k */
b32
surdus_basis (
    constans SurdiSpatium* sp,
                      i32  selectio,
                      s64  c,
                   Surdus* exitus);

b32
surdus_est_nullum (
    Surdus x);


/* ==================================================
 * Arithmetica custodita: FALSUM si s64 excederetur (exitus non
 * tangitur). Coefficientes extra basin spatii (S >= 2^k) nulli esse
 * debent: multiplica, signum et compara eos RECUSANT (FALSUM);
 * adde/subtrahe/scala spatium non vident et eos tantum transferunt.
 * ================================================== */

b32
surdus_adde (
    Surdus  a,
    Surdus  b,
    Surdus* exitus);

b32
surdus_subtrahe (
    Surdus  a,
    Surdus  b,
    Surdus* exitus);

b32
surdus_scala (
    Surdus  a,
       s64  c,
    Surdus* exitus);

b32
surdus_multiplica (
    constans SurdiSpatium* sp,
                   Surdus  a,
                   Surdus  b,
                   Surdus* exitus);


/* ==================================================
 * Signum
 * ================================================== */

/* -1, 0, +1 EXACTE; FALSUM (exitus non tangitur) solum si gradus 3
 * excederet aut coefficiens extra basin non nullus */
b32
surdus_signum (
    constans SurdiSpatium* sp,
                   Surdus  x,
                      s32* exitus);

/* signum(a - b); FALSUM si differentia aut signum excederet */
b32
surdus_compara (
    constans SurdiSpatium* sp,
                   Surdus  a,
                   Surdus  b,
                      s32* exitus);

#endif /* SURDUS_H */
