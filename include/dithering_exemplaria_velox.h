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
 *
 * Differentia a classico (project-specs/exemplaria-velox.md, VIII):
 * plana f32 et radix cubica approximata (T3) - lapis 2 px, pratum 14
 * px ex ~450 000 diversa. Omnia post T3 exacta (digesta in probatione
 * fixa). Velocitas (2026-11-07, N XXX, ms per quadrum, Apple silicon):
 *
 *   imago   magnitudo  classica  velox  arena c/v MB
 *   lapis   360x451      226      76    89/29
 *   lapis   600x751      581     185    247/80
 *   lapis   720x901      840     258    356/115
 *   pratum  600x800      663     193    260/82
 *
 * Ubi tempus abit (velox, sine inlining): basis XXXVII %, residuum
 * XVII %, campus striarum X %, pictographica VII %, structura VII %,
 * aspersio VI %.
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

/*
 * dithering_exemplaria_velox_cum_larva - idem, pixela quae vocans non
 * eget omittens (statua in quadro: charta circa eam tempus non sumat)
 *
 * @larva: I octetus per pixelum, ordo imaginis; non 0 = necessarium.
 *         NIHIL = omnia necessaria (idem ac ..._colorum)
 *
 * Pixela necessaria imago sunt: praesupputationes (hachurarum,
 * pictographica) et census fractiones NUMERI NECESSARIORUM sunt, non
 * rectanguli; hachurae, campus striarum et pictographica extra
 * necessaria non fiunt. Stratum basis areas sine pixelo necessario
 * omittit; residuum pixela non necessaria omittit (errorem nec
 * accipiunt nec diffundunt). Oculi et structura imaginem totam legunt
 * ut ante.
 *
 * Pixela necessaria: larva tota non 0 -> indices iidem ac sine larva,
 * octetus pro octeto. Larva partialis -> differre licet (error
 * residui a pixelis omissis non venit; regiones aliae eliguntur).
 * Pixela non necessaria: color activus aliquis, non definitus quis.
 */
DitheringFructus
dithering_exemplaria_velox_cum_larva (
                             constans i8* rgba,
                                     i32  latitudo,
                                     i32  altitudo,
                            constans b32  colores_activi[XVI],
    constans DitheringExemplariaOptiones* optiones,
               DitheringExemplariaCensus* census,
                             constans i8* larva,
                                 Piscina* piscina);

#endif /* DITHERING_EXEMPLARIA_VELOX_H */
