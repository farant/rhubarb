/*
 * dithering_ordinatum.h - dithering sine diffusione erroris: color
 * proximus et Bayer ordinatum
 *
 * Duo modi qui nihil inter pixela ferunt (contra Atkinson,
 * dithering.h): pars imaginis immota exitum immotum dat - ergo
 * imaginibus motis (latus statuarum, project-specs/statuae-spec.md)
 * apti. Paletta, colores activi et fructus ut in dithering.h.
 *
 * Modulus proprius, non pars dithering.h: craticulam DITHERING_BAYER
 * ex fontibus exemplariorum sumit (dithering_exemplaria_fontes.h,
 * sedes una), et dithering.c a fontibus illis liber manere debet
 * (clientes eum sine exemplariis nectunt: tessera).
 */

#ifndef DITHERING_ORDINATUM_H
#define DITHERING_ORDINATUM_H

/* <aedilis corpus="lib/dithering_ordinatum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "dithering.h"

/*
 * dithering_proximus_colorum - sine dithering: color activus proximus
 *
 * Quodque pixelum colorem activum proximum in spatio RGB accipit
 * (eadem distantia ac dithering_atkinson_colorum, sine diffusione
 * erroris). Paria: index minor vincit.
 *
 * Argumenta et fructus ut dithering_atkinson_colorum (successus
 * FALSUM si nullus color activus, imago NIHIL aut mensura 0).
 */
DitheringFructus
dithering_proximus_colorum (
     constans i8* rgba,
             i32  latitudo,
             i32  altitudo,
    constans b32  colores_activi[XVI],
         Piscina* piscina);

/*
 * dithering_bayer_colorum - dithering ordinatum (Bayer VIII x VIII)
 *
 * Pro colore cuiusque pixeli XVI candidati eliguntur quorum medium
 * eum approximat (color proximus, deinde proximus colori plus errore
 * cumulato - diffusio intra pixelum, non ad vicinos); per claritatem
 * ordinati, craticula DITHERING_BAYER unum eligit. Colores ergo ut
 * Atkinson misti, sed exemplar ordinatum.
 *
 * Craticula ad originem imaginis ancorata est et nihil inter pixela
 * fertur: pars imaginis immota exitum immotum dat (in motu
 * quietissimum), et imago VIII pixelis mota exitum eodem modo motum
 * dat.
 *
 * Imago coloris activi unius eum solum reddit. Paria: index minor
 * vincit.
 *
 * Argumenta et fructus ut dithering_atkinson_colorum (successus
 * FALSUM si nullus color activus, imago NIHIL aut mensura 0).
 */
DitheringFructus
dithering_bayer_colorum (
     constans i8* rgba,
             i32  latitudo,
             i32  altitudo,
    constans b32  colores_activi[XVI],
         Piscina* piscina);

#endif /* DITHERING_ORDINATUM_H */
