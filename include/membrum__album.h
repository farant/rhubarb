/* membrum__album.h - INDEX PAGINATUS cum titulis gregum
 * (nomenclator-plan U1; ex calendario extractum; genus membrum__ =
 * componens UI compositioni destinatum): ordines (res et tituli)
 * altitudinis datae in paginas capacitatis datae.
 *
 * Regulae paginarum:
 *   - ordo qui non capit paginam novam incipit; ordo capacitate maior
 *     solus in pagina sua;
 *   - titulus numquam ultimus paginae: titulus cum titulis sequentibus
 *     et re prima post eos manet ("Completed" supra diem et rem);
 *   - pagina in medio gregis incipiens titulum eius (ALBUM_TITULUS
 *     proximus prior) ITERAT; spatium eius in pagina servatur. Fascia
 *     (ALBUM_FASCIA) gregem non possidet, numquam iteratur.
 *
 * Purus: insulam, dispositionem, picturam non novit. Hospes ordines
 * addit, paginam currentem servat (campus suus), ordines paginae et
 * titulum iteratum disponit et pingit. Unitates altitudinis hospitis
 * sunt (calendarium: cellulae).
 */
#ifndef MEMBRUM__ALBUM_H
#define MEMBRUM__ALBUM_H

/* <aedilis corpus="lib/membrum__album.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

nomen enumeratio {
    ALBUM_RES = ZEPHYRUM,   /* res (eligi potest) */
    ALBUM_TITULUS,          /* titulus gregis: res sequentes
                                 * possidet usque ad titulum proximum;
                                 * iteratur */
    ALBUM_FASCIA            /* titulus sine grege: non iteratur */
} AlbumGenus;

nomen structura {
            chorda id;         /* copia in piscina albi */
        AlbumGenus genus;
               s32 altitudo;   /* unitates hospitis, > 0 */
               s32 res;        /* id rei hospitis (RES); 0 tituli */
} AlbumOrdo;

nomen structura Album Album;

/* album vacuum; capacitas = altitudo paginae (unitates hospitis) */
Album*
album_creare (
     Piscina* piscina,
         s32  capacitas);

/* ordinem addere (id copiatur); paginae recomputantur pigre */
b32
album_addere (
                  Album* c,
     constans character* id,
             AlbumGenus  genus,
                    s32  altitudo,
                    s32  res);

i32
album_numerus (
    constans Album* c);

constans AlbumOrdo*
album_ordo (
        constans Album* c,
                   i32  index);

/* numerus paginarum (>= I; vacuum = I) */
s32
album_paginae (
    Album* c);

/* k ad [0, paginae - 1] coactum */
s32
album_pagina_coacta (
        Album* c,
          s32  k);

/* pagina k (coacta): ordines [*a, *b); *titulus = index tituli
 * iterandi supra *a, aut -1 */
vacuum
album_pagina (
        Album* c,
          s32  k,
          s32* a,
          s32* b,
          s32* titulus);

/* pagina rei (id hospitis); res in gregibus pluribus: pagina
 * praeferenda si ibi est, aliter prima; -1 si nulla */
s32
album_pagina_rei (
        Album* c,
          s32  res,
          s32  praeferenda);

/* id rei primae paginae k; 0 si pagina sine re */
s32
album_res_prima (
        Album* c,
          s32  k);

#endif /* MEMBRUM__ALBUM_H */
