/*
 * mundus_locus.h - locus instructus: dispositio rerum in loco nudo
 *
 * Locus mundi (latus $world) ex DISPOSITIONE fit: plagula STML quae
 * locum nudum (GLB) nominat et res plasticas cum locis suis. Hic
 * dispositio legitur et locus instructus struitur: res quaeque in
 * altitudine soli sub ea posita, omnes cum loco nudo in statuam unam
 * iunctae (statua_iunctio.h). Statua illa locus ordinarius est:
 * tabula gressus et pictura humi signa eius legunt ut semper
 * (project-specs/plastica-spec.md 11, IV.7).
 *
 * Plasticam non novit: vocans res componit et statuas praebet.
 *
 * FORMA:
 *
 *   <locus titulus="clearing"
 *          basis="probationes/fixa/statuae/planum.glb">
 *     <res fons="plastica/res/puteus.stml" locus="30 34" gyrus="15"/>
 *   </locus>
 *
 * titulus   titulus loci in volumine
 * basis     via plagulae GLB loci nudi
 * fons      via descriptionis plasticae rei
 * locus     "x z" in unitatibus mundi; altitudo ex tabula gressus
 *           loci NUDI sub puncto sumitur
 * gyrus     gradus circa axem erectum; ordinarius 0
 *
 * LEGERE RECUSAT, causa Anglice et linea: textum qui STML non est;
 * radicem aliam quam <locus>; elementum aut attributum ignotum;
 * titulum aut basim absentem aut vacuam; rem sine fonte; locum qui
 * duo numeri non est; gyrum qui numerus non est; rem cum liberis;
 * plus quam MMMMXCVI res. Dispositio sine rebus licet.
 *
 * STRUERE RECUSAT, lineam rei culpatae nominans (prima vincit; nihil
 * struitur):
 *   - punctum extra locum, in aqua, aut in solo non gressibili;
 *   - rem cum signo solido quae NULLAM cellam gressibilem claudit
 *     (tabula centra cellarum 0.25 unitatum inspicit: pars solida
 *     minus quam 0.5 lata nihil tegere potest - obex invisus addatur;
 *     spec 14a);
 *   - statuam rei NIHIL.
 * Res in loco NUDO stat: res super rem aliam non ponitur. Res se
 * invicem tangere licet.
 */

#ifndef MUNDUS_LOCUS_H
#define MUNDUS_LOCUS_H

/* <aedilis corpus="lib/mundus_locus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "statua.h"

#define MUNDUS_LOCUS_RES_MAXIMAE  (IV * MXXIV)

nomen structura {
    chorda fons;     /* via descriptionis plasticae */
       f32 x;
       f32 z;
       f32 gyrus;    /* gradus; ordinarius 0 */
       i32 linea;    /* linea elementi in plagula */
} MundusLocusRes;

nomen structura {
               b32  successus;
            chorda  titulus;
            chorda  basis;     /* via GLB loci nudi */
    MundusLocusRes* res;
               i32  numerus;
            chorda  error;     /* Anglice; vacua si successus */
               i32  linea;     /* ab I; 0 si ignota aut successus */
} MundusLocusFructus;

/*
 * mundus_locus_legere - dispositionem legere
 *
 * @textus:  plagula tota (non mutatur; chordae in piscinam copiantur)
 *
 * Redde: successus VERUM cum titulo, basi et rebus, aut FALSUM cum
 * errore et linea
 */
MundusLocusFructus
mundus_locus_legere (
     chorda  textus,
    Piscina* piscina);

nomen structura {
       b32  successus;
    Statua* statua;    /* locus instructus, finitus; NIHIL si
                        * recusatus */
    chorda error;     /* Anglice; vacua si successus */
       i32 linea;     /* linea rei culpatae; 0 si nulla */
} MundusLocusInstructus;

/*
 * mundus_locus_struere - locum nudum et res in locum instructum
 *
 * @basis:   locus nudus, finitus
 * @res:     numerus res cum locis
 * @statuae: statuae[k] = res[k] composita, finita (index idem pro
 *           fonte eodem: tabulae eius semel tenentur)
 * @piscina: locus instructus et labor
 *
 * Locus instructus pixela texturarum et claves cyclorum ex basi et
 * statuis mutuatur: illae diutius vivant.
 *
 * Redde: successus VERUM cum statua, aut FALSUM cum errore et linea
 * rei (0 si basis aut piscina NIHIL, aut memoria deficit)
 */
MundusLocusInstructus
mundus_locus_struere (
              constans Statua* basis,
      constans MundusLocusRes* res,
    constans Statua* constans* statuae,
                          i32  numerus,
                      Piscina* piscina);

#endif /* MUNDUS_LOCUS_H */
