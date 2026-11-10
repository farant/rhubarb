/*
 * dithering_exemplaria_fontes.h - fontes exemplariorum communes
 *
 * Matrix Bayer 8x8 et pictogrammata XI (delineationes 8x8, '#' =
 * punctum) cum constantibus quas pondera eorum postulant. Sedes UNA:
 * dithering_exemplaria.c (classicus, pondera ipse computat),
 * dithering_exemplaria_velox.c et generator tabularum
 * (tools/dithering_tabulae_generare.c) hinc legunt - exemplar mutatum
 * omnes mutat. Ordo bitorum ut exemplaria.h (0x80 = x 0).
 */

#ifndef DITHERING_EXEMPLARIA_FONTES_H
#define DITHERING_EXEMPLARIA_FONTES_H

#include "latina.h"
#include "dithering_exemplaria.h"

/* latus exemplaris; pondus 0..GRADUS_MIXTI; Bayer densitate k/64,
 * k = 0..60 (gradu IV); oculus basis et oculus latus (regiones,
 * hachurae); pictogrammata XI (undae bis: densitates duae) */
#define DITHERING_TEGULA                VIII
#define DITHERING_GRADUS_MIXTI          XXXII
#define DITHERING_BAYER_NUMERUS         XVI
#define DITHERING_SIGMA_OCULI           1.0
#define DITHERING_SIGMA_OCULI_LATI      2.0
#define DITHERING_PICTOGRAMMATA_FONTIUM XI

nomen structura {
    DitheringPictogramma  familia;
      constans character* lineae[DITHERING_TEGULA];
} DitheringPictogrammaFons;

extern constans s32 DITHERING_BAYER[DITHERING_TEGULA][DITHERING_TEGULA];

extern constans DitheringPictogrammaFons
    DITHERING_PICTOGRAMMATA[DITHERING_PICTOGRAMMATA_FONTIUM];

#endif /* DITHERING_EXEMPLARIA_FONTES_H */
