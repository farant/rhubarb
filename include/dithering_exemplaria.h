/*
 * dithering_exemplaria.h - dithering per exemplaria (MacPaint) super
 * palettam Aquinatis
 *
 * Modus tertius iuxta dithering_atkinson_colorum. Imago in XVI colores
 * redditur stratis quinque, quodque in sua parte imaginis:
 *
 *   I   basis: exemplaria Bayer 8x8 (ad cancellos globales ancorata) per
 *       areas 2x2 electa; mixtio in luce lineari, oculus Gaussianus, OKLab
 *   II  hachurae curvae: striae formam sequentes in tonis mediis
 *       cohaerentibus, regiones maximae primum, sub praesupputatione
 *   III pictographica: regiones quietae (color proximus + fascia luminis)
 *       situla implentur UNO pictogrammate ex materia coloris, quietissimae
 *       primum, sub praesupputatione
 *   IV  aspersio: umbra quam regio plana amisit punctis fortuitis variae
 *       pressionis redditur (intra regiones pictographicas solum)
 *   V   residuum Atkinson ubi error limen superat
 *
 * Imagines latitudine ~300-700 pixelorum optime: exemplaria pixela
 * visibilia postulant. Diminuere officium vocantis est.
 * Determinatum: idem input + optiones -> eadem indices.
 */

#ifndef DITHERING_EXEMPLARIA_H
#define DITHERING_EXEMPLARIA_H

#include "latina.h"
#include "piscina.h"
#include "dithering.h"


/* ============================================================
 * Pictogrammata
 * ============================================================ */

nomen enumeratio {
    DITHERING_PICTOGRAMMA_NULLUM = -1,  /* regio plana manet */
    DITHERING_PICTOGRAMMA_LATERES = 0,  /* murus */
    DITHERING_PICTOGRAMMA_SQUAMAE,      /* folia */
    DITHERING_PICTOGRAMMA_UNDAE,        /* aqua, caelum (duae densitates) */
    DITHERING_PICTOGRAMMA_RHOMBI,
    DITHERING_PICTOGRAMMA_TEXTUM,       /* textile */
    DITHERING_PICTOGRAMMA_CRUCES,
    DITHERING_PICTOGRAMMA_HERBA,
    DITHERING_PICTOGRAMMA_SAGITTAE,
    DITHERING_PICTOGRAMMA_ANULI,
    DITHERING_PICTOGRAMMA_PUNCTA,
    DITHERING_PICTOGRAMMATA_NUMERUS
} DitheringPictogramma;


/* ============================================================
 * Optiones et census
 * ============================================================ */

nomen structura {
                     f64 praesupputatio_pictographica;  /* fractio imaginis maxima (0.30) */
                     f64 praesupputatio_hachurarum;     /* (0.15); 0 = sine hachuris */
                     f64 pressio_aspersionis;           /* (2.0); 0 = sine aspersione */
                     f64 tectum_aspersionis;            /* densitas maxima punctorum (0.50) */
    DitheringPictogramma materia[XVI];                  /* per colorem regionis; NULLUM = plana */
                     i32 semen;                         /* strepitus aspersionis et striarum */
} DitheringExemplariaOptiones;

nomen structura {
    f64 tegmen_hachurarum;        /* fractio pixelorum imaginis */
    f64 tegmen_pictographicum;
    f64 tegmen_aspersionis;       /* fractio pixelorum regionum mutata */
    i32 regiones_hachuratae;
    i32 regiones_pictographicae;
} DitheringExemplariaCensus;


/* ============================================================
 * Functiones
 * ============================================================ */

/*
 * dithering_exemplaria_optiones_ordinariae - optiones decretae
 * (Fran 2026-10-13). Materia: virides -> squamae/herba, ruber ->
 * lateres, caeruleus -> undae, aurum -> rhombi/anuli, grisei ->
 * textum/puncta, malva -> sagittae, roseus -> cruces, niger/albus ->
 * plana.
 */
vacuum
dithering_exemplaria_optiones_ordinariae (
    DitheringExemplariaOptiones* optiones);

/*
 * dithering_exemplaria_colorum - modus per exemplaria
 *
 * @rgba:            RGBA input (IV octeti per pixelum)
 * @latitudo:        latitudo imaginis (quaevis, etiam 1)
 * @altitudo:        altitudo imaginis
 * @colores_activi:  XVI b32; colores inactivi numquam emittuntur
 * @optiones:        NIHIL = ordinariae
 * @census:          NIHIL licet; aliter impletur
 * @piscina:         pro output et memoria temporaria
 *
 * Redde: DitheringFructus cum indicibus (unus octetus per pixelum)
 */
DitheringFructus
dithering_exemplaria_colorum (
                             constans i8* rgba,
                                     i32  latitudo,
                                     i32  altitudo,
                            constans b32  colores_activi[XVI],
    constans DitheringExemplariaOptiones* optiones,
               DitheringExemplariaCensus* census,
                                 Piscina* piscina);

#endif /* DITHERING_EXEMPLARIA_H */
