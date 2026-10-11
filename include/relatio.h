/* relatio.h - SYSTEMA RELATIONUM (status-plan S5b): campi cum
 * referentia ('ligamen/genus') post omnem mutationem documenti ligati
 * (historia observata), post ligationem novam et post restitutionem
 * verificantur; orbati per 'orbatus' reparantur (summum = ultimum
 * enumerationis, nullum = attributum tollitur; orbatus absens =
 * intactus), scriptore 'relatio' (S5a). Undo reparationem non revocat
 * (Franus: a). */

#ifndef RELATIO_H
#define RELATIO_H

/* <aedilis corpus="lib/relatio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "historia.h"
#include "organum.h"

/* enumeratio entium generis unius in documento, ORDINE (imum primum) */
nomen structura {
       i32 (*numerus)(constans vacuum* documentum);
    chorda (*id)(constans vacuum* documentum, i32 index,
                 Piscina* piscina);
} RelatioEnumerator;

nomen structura Relatio Relatio;

Relatio*
relatio_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo);

/* genus documenti + genus entis -> enumerator ("pictor-documentum",
 * "stratum"); FALSUM si bis */
b32
relatio_genus_addere (
                 Relatio* relatio,
      constans character* genus_documenti,
      constans character* genus_entis,
       RelatioEnumerator  enumerator);

/* ramum registrare (campi cum referentia ex organo) */
b32
relatio_ramum_addere (
                 Relatio* relatio,
    constans InsulaRamus* ramus,
        constans Organum* organum);

/* ligamen rami ad documentum (montatio, ligatio nova): observatorem
 * in historia eius ponit (semel per historiam) et statim verificat.
 * FALSUM si ramus non registratus aut ligamen ignotum */
b32
relatio_ligare (
                 Relatio* relatio,
    constans InsulaRamus* ramus,
      constans character* ligamen,
                  vacuum* documentum,
                Historia* historia);

#endif /* RELATIO_H */
