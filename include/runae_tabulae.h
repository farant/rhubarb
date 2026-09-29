/* runae_tabulae.h - Tabulae GENERATAE runarum (runae)
 *
 * lib/runae_tabulae.c a tools/runae_generare.sh scribitur ex datis
 * Unicode fixis; hoc caput manu scriptum et stabile est. Tabula duorum
 * graduum: GRADUS_PRIMUS[runa >> VIII] = index truncii; truncus
 * CCLVI octetorum in GRADUS_SECUNDUS (trunci identici semel servati).
 * Valor octeti: bits 0-1 latitudo (0-II); bits 2-6 classis rupturae
 * graphematum (RunaeClassis); bit 7 basis variationis emoji
 * (emoji-variation-sequences.txt). Caput hoc et generatori et
 * bibliothecae commune est (una sedes dispositionis).
 */

#ifndef RUNAE_TABULAE_H
#define RUNAE_TABULAE_H

#include "latina.h"

#define RUNAE_TRUNCUS 256
#define RUNAE_TRUNCI_PRIMI 4352          /* 0x110000 / RUNAE_TRUNCUS */
#define RUNAE_LATITUDO_MASCULA 0x03
#define RUNAE_CLASSIS_POSITIO  2
#define RUNAE_CLASSIS_MASCULA  0x7C
#define RUNAE_BASIS_VARIATIONIS 0x80

/* Classis rupturae: Grapheme_Cluster_Break cum InCB, Emoji_Modifier et
 * Extended_Pictographic in unum alphabetum conflatis (Unicode 15.1:
 * Extended_Pictographic et InCB Consonant semper GCB Other; InCB
 * Extend in GCB Extend aut ZWJ; InCB Linker et Emoji_Modifier in GCB
 * Extend - generator haec ASSERIT). Valores numquam interponendi. */
nomen enumeratio {
    RUNAE_CLASSIS_ALIA = 0,         /* Other */
    RUNAE_CLASSIS_CR,
    RUNAE_CLASSIS_LF,
    RUNAE_CLASSIS_REGIMEN,          /* Control */
    RUNAE_CLASSIS_EXTENSIO,         /* Extend, InCB None */
    RUNAE_CLASSIS_EXTENSIO_INCB,    /* Extend, InCB Extend */
    RUNAE_CLASSIS_CONIUNCTOR,       /* Extend, InCB Linker */
    RUNAE_CLASSIS_MODIFICATOR,      /* Extend, Emoji_Modifier */
    RUNAE_CLASSIS_IUNCTOR,          /* ZWJ (InCB Extend) */
    RUNAE_CLASSIS_REGIONIS,         /* Regional_Indicator */
    RUNAE_CLASSIS_PRAEPOSITUM,      /* Prepend */
    RUNAE_CLASSIS_SPATIANS,         /* SpacingMark */
    RUNAE_CLASSIS_SYLLABA_INITIALIS,    /* L */
    RUNAE_CLASSIS_SYLLABA_MEDIA,        /* V */
    RUNAE_CLASSIS_SYLLABA_FINALIS,      /* T */
    RUNAE_CLASSIS_SYLLABA_APERTA,       /* LV */
    RUNAE_CLASSIS_SYLLABA_CLAUSA,       /* LVT */
    RUNAE_CLASSIS_PICTOGRAPHUM,     /* Extended_Pictographic */
    RUNAE_CLASSIS_CONSONANS         /* InCB Consonant */
} RunaeClassis;

externus constans i16 RUNAE_GRADUS_PRIMUS[RUNAE_TRUNCI_PRIMI];
externus constans i8  RUNAE_GRADUS_SECUNDUS[];

#endif /* RUNAE_TABULAE_H */
