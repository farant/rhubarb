/* rubrica.h - RUBRICAE (tags): nomen [a-z0-9-]+ (kebab, ut hashtag);
 * copia ordinata (ordine octetorum) sine iteratis, textu "a b c"
 * scripta. Communis agendae (res), calendario (filtrum, #verbum in
 * titulo) et personis (nomenclator-plan P1a; olim bis in agenda.c et
 * kalendarium.c). */
#ifndef RUBRICA_H
#define RUBRICA_H

/* <aedilis corpus="lib/rubrica.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* nomen validum: [a-z0-9-]+, non vacuum */
b32
rubrica_valida (
    chorda c);

/* ordo canonicus: octeti, deinde longitudo */
s32
rubricas_comparare (
    chorda a,
    chorda b);

/* "a b  c" -> Xar de chorda, ordinata, sine iteratis (frusta in
 * textu ipso, non copiata); NIHIL si rubrica mala */
Xar*
rubricas_legere (
     chorda  textus,
    Piscina* piscina);

/* copia -> "a b" (vacua: datum NIHIL) */
chorda
rubricas_iungere (
    constans Xar* copia,
         Piscina* piscina);

/* copia (Xar de chorda) rubricam continet? */
b32
rubricam_habet (
     constans Xar* copia,
           chorda  rubrica);

#endif /* RUBRICA_H */
