/*
 * dithering.h - Atkinson dithering pro Aquinas palette
 *
 * Modulus pro convertere imagines ad 16-colorum Aquinas palette
 * cum Atkinson error diffusion algorithmo.
 */

#ifndef DITHERING_H
#define DITHERING_H

#include "latina.h"
#include "piscina.h"

/* ============================================================
 * Constantae
 * ============================================================ */

#define AQUINAS_COLORUM_NUMERUS XVI

/* ============================================================
 * Aquinas Palette (16 colores)
 * ============================================================ */

/*
 * Palette RGB valores (r, g, b pro singulis coloribus)
 * Declaratur in dithering.c
 */
externus constans i8 AQUINAS_PALETTE[XVI][III];

/* Nomina colorum pro debug */
externus constans character* AQUINAS_NOMINA[XVI];

/* ============================================================
 * Typi
 * ============================================================ */

/*
 * DitheringFructus - Fructus operationis dithering
 */
nomen structura {
    b32   successus;
    i8*   indices;    /* Palette indices 0-15, unus per pixel */
    i32   latitudo;
    i32   altitudo;
} DitheringFructus;

/* ============================================================
 * Functiones Principales
 * ============================================================ */

/*
 * dithering_atkinson_colorum - Modus coloris
 *
 * Invenit colorem proximum in spatio RGB.
 * Bonus pro imaginibus coloratis.
 *
 * @rgba:            RGBA input (4 bytes per pixel)
 * @latitudo:        Latitudo imaginis
 * @altitudo:        Altitudo imaginis
 * @colores_activi:  Array XVI b32, VERUM = color activus
 * @piscina:         Piscina pro allocare output
 *
 * Redde: DitheringFructus cum indices (unus byte per pixel)
 */
DitheringFructus
dithering_atkinson_colorum(
    constans i8*  rgba,
    i32           latitudo,
    i32           altitudo,
    constans b32  colores_activi[XVI],
    Piscina*      piscina);

/*
 * dithering_atkinson_griseum - Modus griseus
 *
 * Convertit ad luminantiam, utitur punctis ad partire in colores.
 * Bonus pro imaginibus nigro-albis vel cum paucis coloribus.
 *
 * @rgba:            RGBA input (4 bytes per pixel)
 * @latitudo:        Latitudo imaginis
 * @altitudo:        Altitudo imaginis
 * @colores_activi:  Array XVI b32, VERUM = color activus
 * @puncta:          Array III punctorum (default: {64, 128, 192})
 * @piscina:         Piscina pro allocare output
 *
 * Redde: DitheringFructus cum indices
 */
DitheringFructus
dithering_atkinson_griseum(
    constans i8*  rgba,
    i32           latitudo,
    i32           altitudo,
    constans b32  colores_activi[XVI],
    constans i32  puncta[III],
    Piscina*      piscina);

/* ============================================================
 * Functiones Auxiliares
 * ============================================================ */

/*
 * dithering_indices_ad_rgba - Convertere indices ad RGBA
 *
 * @indices:      Palette indices (0-15)
 * @rgba_output:  Buffer pro RGBA output (4 bytes per pixel)
 * @latitudo:     Latitudo
 * @altitudo:     Altitudo
 */
vacuum
dithering_indices_ad_rgba(
    constans i8*  indices,
    i8*           rgba_output,
    i32           latitudo,
    i32           altitudo);

/*
 * dithering_color_ex_indice - Obtinere colorem ex indice
 *
 * @index: Index coloris (0-15)
 * @r, @g, @b: Pointers ad valores output
 */
vacuum
dithering_color_ex_indice(s32 index, i8* r, i8* g, i8* b);

/* ============================================================
 * Praeparatores Colorum (Presets)
 * ============================================================ */

/* Omnes XVI colores */
vacuum dithering_praeparare_omnes(b32 colores[XVI]);

/* Colores grisei: 0, 1, 3, 4, 5, 15 */
vacuum dithering_praeparare_griseum(b32 colores[XVI]);

/* Colores calidi: 0, 5, 6, 9, 10, 11, 15 */
vacuum dithering_praeparare_calidus(b32 colores[XVI]);

/* Colores frigidi: 0, 1, 5, 7, 12, 13, 14 */
vacuum dithering_praeparare_frigidus(b32 colores[XVI]);

/* Monochromaticus (niger et albus): 0, 5 */
vacuum dithering_praeparare_monochromaticus(b32 colores[XVI]);

#endif /* DITHERING_H */
