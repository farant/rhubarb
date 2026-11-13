/*
 * mundus_mappa.h - mappa tabulae gressus: locus desuper in imagine
 *
 * Oculi eius qui locum instruit: ubi solum gressibile est, quam
 * altum, quo corpus pervenit, ubi res stant - cum numeris axium, ut
 * punctum in imagine visum in dispositionem scribi possit
 * (apps/mundus/loci). Nihil de plastica aut de volumine scit.
 *
 * IMAGO: MUNDUS_MAPPA_PIXELA_PER_CELLAM pixela per cellam; margo
 * MUNDUS_MAPPA_MARGO pixelorum supra et ad sinistram numeros fert.
 * Latitudo = margo + cellae x IV; x dextrorsum, z deorsum (ut
 * tabula).
 *
 * COLORES:
 *   gressibile   viride, altitudine clarescens (imum obscurum,
 *                summum flavum)
 *   gressibile non attactum (si 'attingibiles' datur)   roseum
 *   aqua         caeruleum
 *   solidum      griseum obscurum
 *   nihil        album (charta)
 *
 * RETICULUM: linea tenuis omni IV unitate, crassior omni XVI;
 * numeri (coordinata mundi) omni VIII unitate in margine supero
 * (x) et sinistro (z).
 *
 * NOTAE: punctum nigrum cum numero iuxta (res dispositionis);
 * numerus in tabella alba ut super quovis colore legatur.
 */

#ifndef MUNDUS_MAPPA_H
#define MUNDUS_MAPPA_H

/* <aedilis corpus="lib/mundus_mappa.c"/> */

#include "latina.h"
#include "piscina.h"
#include "imago_typus.h"
#include "mundus_solum.h"

#define MUNDUS_MAPPA_PIXELA_PER_CELLAM  IV
#define MUNDUS_MAPPA_MARGO              XVI

/* nota in mappa: punctum cum numero (res dispositionis) */
nomen structura {
    f32 x;
    f32 z;
    i32 numerus;    /* 0 .. 9999 */
} MundusMappaNota;

/*
 * mundus_mappa_pingere - tabulam gressus in imaginem pingere
 *
 * @attingibiles: per cellam 0 / non 0 (mundus_solum_attingere);
 *                NIHIL = nulla cella notatur
 * @notae:        notarum notae; NIHIL licet si notarum 0. Nota
 *                extra tabulam non pingitur
 * @exitus:       imago RGBA opaca; pixela in piscina
 *
 * Redde: FALSUM (exitus non tactus) si s, exitus aut piscina NIHIL,
 * tabula vacua (latitudo aut altitudo 0), notae NIHIL cum numero
 * non 0, aut memoria deficit
 */
b32
mundus_mappa_pingere (
        constans MundusSolum* s,
                 constans i8* attingibiles,
    constans MundusMappaNota* notae,
                         i32  notarum,
                       Imago* exitus,
                     Piscina* piscina);

#endif /* MUNDUS_MAPPA_H */
