/* crusta_diagnostica.h - Diagnostica crustae
 *
 * Derivata ex CRUSTA_DIAGNOSTICA (declaratio registri, B1) per
 * ambulatorem materiae unum, et EMISSA a parsatore
 * (CrustaParsura.diagnostica: classes sine vestigio declarabili).
 * Concordia cum numeris parsatoris est ORACULUM INDEPENDENS: duae
 * implementationes super codicem disiunctum se mutuo tegunt
 * (spec S9). Porta praesentiam per classem poscit (machina
 * arithmetica vestigium unum pluries numerat, delta D7) et 'sana'
 * exacte: sana si et solum si diagnosticum nullum.
 */
#ifndef CRUSTA_DIAGNOSTICA_H
#define CRUSTA_DIAGNOSTICA_H

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "materia_diagnostica.h"
#include "crusta_arbor.h"

/* Xar de MateriaDiagnosticum (materia_diagnostica_derivare); NIHIL =
 * memoria. relatio NIHIL licet (emissa nulla). */
Xar*
crusta_diagnostica (
                   Piscina* piscina,
     constans MateriaNodus* radix,
    constans CrustaParsura* relatio);

nomen structura {
                    i32  mala;
                    i32  clausurae;
                    i32  vacuae;
                    i32  transpositae;
                    i32  errata;
                    i32  monita;
                    b32  concordat;
     constans character* causa;   /* classis prima discors; NIHIL */
} CrustaConcordia;

/* Numeri derivati contra numeros parsatoris, classis per classem. */
b32
crusta_diagnostica_concordia (
                   Piscina* piscina,
     constans MateriaNodus* radix,
    constans CrustaParsura* relatio,
           CrustaConcordia* concordia);

#endif /* CRUSTA_DIAGNOSTICA_H */
