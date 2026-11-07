/*
 * dithering_exemplaria_tabulae.h - tabulae constantes GENERATAE
 * (lib/dithering_exemplaria_tabulae.c,
 * tools/dithering_tabulae_generare.sh)
 *
 * Quod classicus in omni vocatione computat, hic semel in
 * aedificatione: ordines (bits per lineam), densitates, pondera oculo
 * SIGMA_OCULI et SIGMA_OCULI_LATI (Gaussiana in toro, quantizata
 * 0..GRADUS_MIXTI), index [exemplar][y * VIII + x]; et lux linearis
 * octeti sRGB. Valores IIDEM ac classici (_exemplar_finire,
 * color_octetus_ad_linearem): velox ex his == classicus octetim.
 */

#ifndef DITHERING_EXEMPLARIA_TABULAE_H
#define DITHERING_EXEMPLARIA_TABULAE_H

#include "latina.h"
#include "dithering_exemplaria_fontes.h"

extern constans  i8 DITHERING_BAYER_ORDINES[XVI][VIII];
extern constans s32 DITHERING_BAYER_DENSITATES[XVI];
extern constans  i8 DITHERING_BAYER_PONDERA[XVI][LXIV];
extern constans  i8 DITHERING_BAYER_PONDERA_LATA[XVI][LXIV];

extern constans  i8 DITHERING_PICTOGRAMMATUM_ORDINES[XI][VIII];
extern constans s32 DITHERING_PICTOGRAMMATUM_DENSITATES[XI];
extern constans  i8 DITHERING_PICTOGRAMMATUM_PONDERA[XI][LXIV];
extern constans  i8 DITHERING_PICTOGRAMMATUM_PONDERA_LATA[XI][LXIV];

extern constans f64 DITHERING_SRGB_LINEARIS[CCLVI];  /* f32 a T3 */

#endif /* DITHERING_EXEMPLARIA_TABULAE_H */
