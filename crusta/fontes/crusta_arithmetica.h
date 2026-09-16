/* crusta_arithmetica.h - Machina praecedentiae arithmeticae (( ))
 *
 * Spec par. VI (C11): grammatica clausa una linguae - expressio C
 * sine conversionibus et vocationibus, cum '**', comma,
 * assignationibus.
 * Machina shunting-yard super acervos duos (operanda, signa) quam
 * AEDIFICATOR agit: lexemata in modis arithmeticis petit, folia
 * (numerus, variabilis, partes $x ${ } $( )) ut operanda dat, signa ut
 * signa; parenthesis = gradus 'inclusa' cum machina sua. Nulla
 * recursio C in parsatione; aestimator (constans) recursivus est,
 * profunditate portae totalitatis mensurata (P9).
 *
 * TABULA (CRUSTA_BASH.arithmetici, XVI gradus, bash 5.2 mensuratus):
 * praeposita ARTIUS quam '**' ligant (-2**2 = 4); '**', ternaria et
 * assignationes dextrae. Species decernit qualem nodum reductio
 * gignat: praeposita, postposita, binaria, ternaria.
 *
 * TOTALITAS: operandum absens = locus absens (mala++), '?' sine ':' =
 * ternaria sine colon, ':' sine '?' = binaria ':' (mala++).
 */

#ifndef CRUSTA_ARITHMETICA_H
#define CRUSTA_ARITHMETICA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "crusta_registrum.h"

nomen structura {
                     Piscina* piscina;
    constans CrustaDialectus* dialectus;
                         Xar* operanda;      /* MateriaNodus* */
    /* Signum per valorem (privatum) */
                        Xar* signa;
                        b32  operandum_exspectatur;
                        i32  mala;
} CrustaArithmetica;

vacuum
crusta_arithmetica_incipere (
           CrustaArithmetica* machina,
                     Piscina* piscina,
    constans CrustaDialectus* dialectus);

/* FALSUM = memoria deficit (semper). */
b32
crusta_arithmetica_operandum (
    CrustaArithmetica* machina,
         MateriaNodus* nodus);

b32
crusta_arithmetica_operator (
    CrustaArithmetica* machina,
         MateriaToken* signum);

/* Reductio finalis: expressio (NIHIL = vacua). */
MateriaNodus*
crusta_arithmetica_finire (
    CrustaArithmetica* machina);

/* Aestimatio CONSTANS: arbor sine variabili et sine parte; LXIV bits
 * circumvolventes ut bash; divisio per zephyrum, exponens negativus,
 * assignationes, ++/-- RECUSANTUR (FALSUM). */
b32
crusta_arithmetica_aestimare (
    constans MateriaNodus* expressio,
                      s64* valor);

/* Operator tabulae per titulum et speciem; NIHIL si nullus. */
constans CrustaOperator*
crusta_arithmetica_quaerere (
    constans CrustaDialectus* dialectus,
                      chorda  titulus,
       CrustaOperatorSpecies  species);

#endif /* CRUSTA_ARITHMETICA_H */
