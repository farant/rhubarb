/*
 * dithering_exemplaria_velox.h - dithering per exemplaria, velox
 *
 * Idem modus quam dithering_exemplaria_colorum (strata quinque,
 * optiones, materia, palette eaedem), aedificatus ut quadra animata
 * (latus 3D) singula dithering accipere possint: plana f32 per canalem,
 * bitsets pro larvis, tabulae constantes ex generatore, nihil inter
 * vocationes servatum. Output a classico differre licet (pixelatim);
 * aspectus idem oculo iudicatur (tools/exemplaria_mensura.c imagines
 * paris scribit). Classicus intactus manet.
 *
 * Determinatum: idem input + optiones -> eadem indices.
 */

#ifndef DITHERING_EXEMPLARIA_VELOX_H
#define DITHERING_EXEMPLARIA_VELOX_H

#include "latina.h"
#include "piscina.h"
#include "dithering.h"
#include "dithering_exemplaria.h"

/*
 * dithering_exemplaria_velox_colorum - signatura classici ipsa
 * (vide dithering_exemplaria.h); optiones NIHIL = ordinariae.
 */
DitheringFructus
dithering_exemplaria_velox_colorum (
                             constans i8* rgba,
                                     i32  latitudo,
                                     i32  altitudo,
                            constans b32  colores_activi[XVI],
    constans DitheringExemplariaOptiones* optiones,
               DitheringExemplariaCensus* census,
                                 Piscina* piscina);

#endif /* DITHERING_EXEMPLARIA_VELOX_H */
