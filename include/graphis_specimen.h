/*
 * graphis_specimen.h - graphis: scheda ad inspiciendum
 *
 * Pictura compilata in schedam unam vertitur quam Claude legit:
 * pictura finita magna, et iuxta eam quattuor tabulae parvae ex
 * quibus facta est. Sic pictura inspiciendo emendatur
 * (project-specs/graphis-spec.md 2, III).
 *
 * SCHEDA (MCDXLVIII x CDLXXX):
 *
 *   +----------------------+-----------+-----------+
 *   |                      |   fons    |   larva   |
 *   |   pictura finita     +-----------+-----------+
 *   |        II x          | illustr.  | regiones  |
 *   +----------------------+-----------+-----------+
 *
 *   pictura     indices palettae, bis aucta
 *   fons        color ante lucem
 *   larva       vis exemplaris: nigrum 0, album 1; pixela lucentia
 *               caerulea
 *   illustr.    fons post lucem, ante Atkinson
 *   regiones    pictura pallida, regio quaeque colore suo plano
 *               (XII colores in orbem, ordine titulorum)
 *
 * Inter picturam et tabulas stria grisea VIII pixelorum.
 *
 * QUADRUM: pars picturae finitae quater aucta, ad angulum unum
 * prope inspiciendum.
 *
 * REGIONES: numeri cuiusque regionis (pixela possessa, capsa
 * circumscripta) - instrumentum eos imprimit, quia scheda litteras
 * non pingit.
 */

#ifndef GRAPHIS_SPECIMEN_H
#define GRAPHIS_SPECIMEN_H

/* <aedilis corpus="lib/graphis_specimen.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "imago.h"
#include "graphis.h"

#define GRAPHIS_SPECIMEN_STRIA     VIII
#define GRAPHIS_SPECIMEN_LATITUDO \
    (IV * GRAPHIS_LATITUDO + GRAPHIS_SPECIMEN_STRIA)
#define GRAPHIS_SPECIMEN_ALTITUDO  (II * GRAPHIS_ALTITUDO)
#define GRAPHIS_SPECIMEN_COLORES   XII
#define GRAPHIS_QUADRUM_AUCTIO     IV

nomen structura {
    chorda titulus;
       i32 pixela;       /* possessa post omnes formas */
       s32 x_primus;     /* capsa circumscripta, inclusiva */
       s32 y_primus;
       s32 x_ultimus;
       s32 y_ultimus;
} GraphisRegio;

/*
 * graphis_specimen_pingere - schedam pingere
 *
 * @fructus: pictura accepta (successus VERUM)
 * @piscina: pixela schedae
 *
 * Redde: imago RGBA GRAPHIS_SPECIMEN_LATITUDO x
 * GRAPHIS_SPECIMEN_ALTITUDO; imago vacua (pixela NIHIL) si fructus
 * NIHIL aut recusatus, aut memoria deficit
 */
Imago
graphis_specimen_pingere (
    constans GraphisFructus* fructus,
                    Piscina* piscina);

/*
 * graphis_quadrum_pingere - partem picturae finitae quater auctam
 *
 * @x, @y:      angulus sinister summus partis
 * @latitudo:   I .. GRAPHIS_LATITUDO
 * @altitudo:   I .. GRAPHIS_ALTITUDO
 *
 * Redde: imago RGBA (IV x latitudo) x (IV x altitudo); imago vacua
 * si pars non tota intra picturam iacet, fructus recusatus, aut
 * memoria deficit
 */
Imago
graphis_quadrum_pingere (
    constans GraphisFructus* fructus,
                        s32  x,
                        s32  y,
                        s32  latitudo,
                        s32  altitudo,
                    Piscina* piscina);

/*
 * graphis_regiones - numeri regionum
 *
 * @exitus: fructus->tabula->titulorum regiones, ordine titulorum
 *
 * Redde: numerus regionum scriptarum; 0 si fructus recusatus
 */
i32
graphis_regiones (
    constans GraphisFructus* fructus,
               GraphisRegio* exitus);

/*
 * graphis_specimen_color - color regionis in tabula regionum
 *
 * @dominus: I .. (numerus tabulae dominorum)
 *
 * Redde: rgb (III octeti, staticum); colores in orbem XII
 */
constans i8*
graphis_specimen_color (
    i32 dominus);

/* nomen coloris eiusdem, Anglice ("red", "blue" ...), ad indicem
 * regionum imprimendum */
constans character*
graphis_specimen_color_titulus (
    i32 dominus);

#endif /* GRAPHIS_SPECIMEN_H */
