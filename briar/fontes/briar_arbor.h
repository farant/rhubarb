/* briar_arbor.h - Documentum thistle parsare: sequentia partium
 * (prosa | regio | elementum) cum interprete optionali et FINIS.
 *
 * SEMPER ARBOR: fons quilibet parsatur (regio non clausa usque ad
 * finem cum vitio; clausura vaga prosa est; saeptum apertum vitium
 * documenti). Emissio arboris == fons octetim (briar_emittere).
 * Extensio elementi STML: linea aperiens usque ad '</nomen>' in
 * columna 0, aut linea una se-claudens (spec par. II, emendatio
 * plani I).
 */

#ifndef BRIAR_ARBOR_H
#define BRIAR_ARBOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "materia_nodus.h"
#include "materia_scribere.h"

/* NIHIL solum si memoria deficit */
MateriaNodus*
briar_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);

i32
briar_partes_numerus (
    constans MateriaNodus* documentum);

/* NIHIL si index extra */
MateriaNodus*
briar_pars (
    constans MateriaNodus* documentum,
                      i32  index);

/* lexema loci dati aut NIHIL (locus absens aut non TOKEN) */
MateriaToken*
briar_lexema (
    constans MateriaNodus* nodus,
                      i32  locus);

/* regio/elementum: nomen tagi (lexema derivatum); alii: vacua */
chorda
briar_titulus (
    constans MateriaNodus* pars);

/* documentum/regio/elementum: BriarVitium; alii: NULLUM */
s32
briar_vitium (
    constans MateriaNodus* nodus);

/* octeti fontis ex arbore (fons ZEPHYRUM: derivata omissa) */
MateriaScriptura
briar_emittere (
                  Piscina* piscina,
    constans MateriaNodus* radix);

#endif /* BRIAR_ARBOR_H */
