/* exemplaria.h - exemplaria classica Mac 8x8 (1 bit), communia
 *
 * Tabula olim intra lib/delineare.c (delineare legatum eam per hoc
 * caput nunc legit); vicus-latera (indicium foci) rasterizatoribus
 * mandatorum eam dat. Octetus linea est, bitus summus (0x80) = x 0;
 * bitus positus = pingitur. Tegendo: (x, y) quaelibet, modulo VIII. */

#ifndef EXEMPLARIA_H
#define EXEMPLARIA_H

/* <aedilis corpus="lib/exemplaria.c"/> */

#include "latina.h"


/* ==================================================
 * Constantae - Exemplaria Interna
 * ================================================== */

/* Identificationes exemplarium internorum (exemplaria classica Mac) */
nomen enumeratio {
    EXEMPLAR_SOLIDUS = ZEPHYRUM,      /* Exemplar 00 */
    EXEMPLAR_PUNCTA_DISPERSA,          /* Exemplar 01 */
    EXEMPLAR_PUNCTA_REGULARIA,         /* Exemplar 02 */
    EXEMPLAR_TESSELLATUM,              /* Exemplar 03 - checkerboard */
    EXEMPLAR_LINEOLAE_HORIZONTALES,    /* Exemplar 04 */
    EXEMPLAR_LINEAE_VERTICALES,        /* Exemplar 05 */
    EXEMPLAR_DIAGONALIS,               /* Exemplar 06 */
    EXEMPLAR_VERTICALIS_DISPERSUS,     /* Exemplar 07 */
    EXEMPLAR_DISPERSUS,                /* Exemplar 08 */
    EXEMPLAR_PUNCTA_DIAGONALIA,        /* Exemplar 09 */
    EXEMPLAR_LATERICIUM,               /* Exemplar 10 - brick */
    EXEMPLAR_LATERICIUM_OFFSET,        /* Exemplar 11 */
    EXEMPLAR_CINEREUM_LEVE,            /* Exemplar 12 */
    EXEMPLAR_DIAGONALIS_DISPERSUS,     /* Exemplar 13 */
    EXEMPLAR_RHOMBUS_DISPERSUS,        /* Exemplar 14 */
    EXEMPLAR_TEXTUM,                   /* Exemplar 15 - weave */
    EXEMPLAR_MIXTUS,                   /* Exemplar 16 */
    EXEMPLAR_ANGULUS,                  /* Exemplar 17 */
    EXEMPLAR_LATERICIUM_COMPLEXUS,     /* Exemplar 18 */
    EXEMPLAR_ALBUS,                    /* Exemplar 19 */
    EXEMPLAR_RARISSIMUS,               /* Exemplar 20 */
    EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA, /* Exemplar 21 */
    EXEMPLAR_PUNCTA_DUPLICIA,          /* Exemplar 22 */
    EXEMPLAR_HORIZONTALIS_DISPERSUS,   /* Exemplar 23 */
    EXEMPLAR_LINEAE_HORIZONTALES,      /* Exemplar 24 */
    EXEMPLAR_DIAGONALIS_ANTERIOR,      /* Exemplar 25 */
    EXEMPLAR_HORIZONTALIS_LATUS,       /* Exemplar 26 */
    EXEMPLAR_DIAGONALIS_POSTERIOR,     /* Exemplar 27 */
    EXEMPLAR_DISPERSUS_MIXTUS,         /* Exemplar 28 */
    EXEMPLAR_MARGO_SINISTER,           /* Exemplar 29 */
    EXEMPLAR_ARCUS,                    /* Exemplar 30 */
    EXEMPLAR_LINEA_RHOMBUS,            /* Exemplar 31 */
    EXEMPLAR_ANGULI_PARVI,             /* Exemplar 32 */
    EXEMPLAR_DIAGONALIS_UNDULATUS,     /* Exemplar 33 */
    EXEMPLAR_CRUX,                     /* Exemplar 34 */
    EXEMPLAR_MIXTUS_GRAVIS,            /* Exemplar 35 */
    EXEMPLAR_CAPSAE_COMPLEXAE,         /* Exemplar 36 */
    EXEMPLAR_RHOMBUS_LINEATUS,         /* Exemplar 37 */
    EXEMPLAR_NUMERUS
} exemplar_internum_t;

/* octo octeti exemplaris; NIHIL si id extra */
constans i8*
exemplar_obtinere (
    i32 id);

/* bitus exemplaris ad (x, y) coordinatis quibuslibet (tegendo,
 * modulo VIII, negativa quoque); FALSUM si id extra */
b32
exemplar_punctum (
    i32 id,
    s32 x,
    s32 y);

#endif /* EXEMPLARIA_H */
