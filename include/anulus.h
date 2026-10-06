/* anulus.h - Descriptio anuli: operationes elementorum per tabulam
 * functionum
 *
 * Bibliothecae algebraicae (matrix, et futurae) super quemlibet anulum
 * domus operantur per hanc tabulam: elementa opaca magnitudinis datae,
 * operationes per indices functionum. Tres anuli parati: Z (magnus), Q
 * (fractio, corpus), Z[t, t^-1] (polynomium, littera 't').
 *
 * Operationes b32 reddunt: FALSUM si operatio elementi refutat
 * (polynomium: exponens extra fines; divisio exacta non exacta) -
 * exitus tunc non tangitur. Effectus in piscina data vivunt;
 * transcribe copiam PROFUNDAM facit (pro piscinis temporariis
 * reficiendis).
 *
 * USUS:
 *   constans Anulus* z = &ANULUS_INTEGRORUM;
 *   Magnus a = magnus_ex_s64(VI);
 *   Magnus b = magnus_ex_s64(VII);
 *   Magnus c;
 *   (vacuum)z->multiplica(&a, &b, piscina, &c);
 *
 * Vide lib/anulus.worklog.md.
 */
/* <aedilis corpus="lib/anulus.c"/> */
#ifndef ANULUS_H
#define ANULUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"

nomen structura {
    constans character* titulus;      /* "Z", "Q", "Z[t,t^-1]" */
        memoriae_index  mensura;      /* octeti elementi */
                   b32  corpus;       /* non nulli invertibiles */

    vacuum (*nullum) (
        vacuum* exitus);
    vacuum (*unum) (
        Piscina* piscina,
         vacuum* exitus);
    b32 (*est_nullum) (
        constans vacuum* a);
    /* elementum sine memoria externa (totum in structura): Z in s64, Q
     * numerator et denominator in s64, Z[t] nullum solum. Algorithmi
     * hoc ad viam parvam (sine piscinis temporariis) eligendam
     * utuntur. */
    b32 (*parvum) (
        constans vacuum* a);
    b32 (*aequalis) (
        constans vacuum* a,
        constans vacuum* b);
    b32 (*adde) (
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    b32 (*subtrahe) (
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    b32 (*multiplica) (
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    /* a / b in anulo: FALSUM si b nullus aut b non dividit a */
    b32 (*divide_exacte) (
        constans vacuum* a,
        constans vacuum* b,
                Piscina* piscina,
                 vacuum* exitus);
    vacuum (*transcribe) (
        constans vacuum* a,
                Piscina* piscina,
                 vacuum* exitus);
    chorda (*ad_chordam) (
        constans vacuum* a,
                Piscina* piscina);
    b32 (*ex_chorda) (
          chorda  textus,
        Piscina*  piscina,
         vacuum*  exitus);
} Anulus;

/* Z: elementa Magnus */
extern constans Anulus ANULUS_INTEGRORUM;

/* Q: elementa Fractio (corpus) */
extern constans Anulus ANULUS_RATIONALIUM;

/* Z[t, t^-1]: elementa Polynomium, textus littera 't' */
extern constans Anulus ANULUS_POLYNOMIORUM;

#endif /* ANULUS_H */
