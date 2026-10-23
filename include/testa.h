#ifndef TESTA_H
#define TESTA_H

/* testa.h - "applicatio sum: da mihi testam" (testa-spec II.1).
 * Includere = principale a testa accipere: lib/testa.c (macOS, et
 * quaevis fenestra perscrutabilis) aut lib/testa_android.c (--varians
 * android,posix) - regula variantium aedilis, nullus casus in
 * scriptis. Applicatio SOLUM applicatio_initium definit. Probationes
 * testa_cursus.h includunt, numquam hoc (principale duplex). */

#include "testa_cursus.h"

/* Status applicationis in a->piscina aedificare et formam implere.
 * Semel vocatur, schirmo noto; FALSUM = exitus I (nuntium ipsa
 * scribit). Monstratores in formam dati vivere debent usque ad finem:
 * nulla variabilis localis (R3). */
b32
applicatio_initium (
    constans TestaAmbitus* a,
               TestaForma* forma);

#endif /* TESTA_H */
