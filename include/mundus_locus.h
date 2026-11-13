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
 *     <res fons="plastica/res/pinus.stml" locus="6 15"
 *          magnitudo="1.2" semen="3"/>
 *     <sparsio fons="plastica/res/frutex.stml" numerus="12" semen="5"
 *              area="20 28 44 36" distantia="2" magnitudo_varia="0.2"
 *              gyrus_varius="verum" varietates="3"/>
 *   </locus>
 *
 * titulus   titulus loci in volumine
 * basis     via plagulae GLB loci nudi
 * fons      via descriptionis plasticae rei
 * locus     "x z" in unitatibus mundi; altitudo ex tabula gressus
 *           loci NUDI sub puncto sumitur
  * gyrus     gradus circa axem erectum; ordinarius 0
 * magnitudo scala rei totius; ordinaria 1 (0.25 .. 4)
 * semen     omni semini rei additur (plastica_legere_seminis): res
 *           eiusdem fontis alia forma; ordinarium 0 (0 .. 1000000)
 *
 * SPARSIO: 'numerus' exemplaria rei unius intra rectangulum area="x0
 * z0 x1 z1" sparsa. Loca ex semine sparsionis (dispositio eadem,
 * locus idem). Punctum praeteritur si extra locum est, in aqua, in
 * solo non gressibili, aut propius quam 'distantia' (ordinaria 1.5)
 * alii rei - scriptae CUIVIS (ubicumque in plagula) aut prius
 * sparsae. magnitudo_varia (0 .. < 1) scalam inter 1 - v et 1 + v
 * variat; gyrus_varius="verum" quodque fortuito vertit; varietates
 * (I .. XVI, ordinarium I) quot semina rei diversa in orbem eant
 * (exemplar k semen k mod varietates habet). Sparsio quae numerum
 * suum ponere non potest RECUSATUR, positis nominatis.
 *
 * LEGERE RECUSAT, causa Anglice et linea: textum qui STML non est;
 * radicem aliam quam <locus>; elementum aut attributum ignotum;
 * titulum aut basim absentem aut vacuam; rem sine fonte; locum qui
 * duo numeri non est; gyrum qui numerus non est; rem cum liberis;
  * magnitudinem aut semen extra fines; sparsionem sine fonte, numero
 * aut area, cum area inversa, distantia negativa, magnitudo_varia
 * extra [0, 1), gyrus_varius alio quam "verum", varietatibus extra
 * I .. XVI; plus quam MMMMXCVI res. Dispositio sine rebus licet.
 *
 * STRUERE RECUSAT, lineam rei culpatae nominans (prima vincit; nihil
 * struitur):
 *   - punctum extra locum, in aqua, aut in solo non gressibili;
 *   - rem cum signo solido quae NULLAM cellam gressibilem claudit
 *     (tabula centra cellarum 0.25 unitatum inspicit: pars solida
 *     minus quam 0.5 lata nihil tegere potest - obex invisus addatur;
 *     spec 14a);
  *   - statuam rei NIHIL; scalam non positivam.
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
       f32 scala;    /* magnitudo; ordinaria 1. In structura semper
                      * positiva sit (0 non est "ordinaria") */
       i32 semen;    /* semini rei additum; ordinarium 0 */
       i32 linea;    /* linea elementi in plagula (rei sparsae:
                      * linea sparsionis) */
} MundusLocusRes;

nomen structura {
    chorda fons;
       i32 numerus;          /* I .. CCLVI */
       i32 semen;
       f32 area[IV];         /* x0 z0 x1 z1 */
       f32 distantia;        /* ordinaria 1.5 */
       f32 magnitudo_varia;  /* 0 .. < 1 */
       b32 gyrus_varius;
       i32 varietates;       /* I .. XVI; ordinarium I */
       i32 linea;
} MundusLocusSparsio;

nomen structura {
                   b32  successus;
                chorda  titulus;
                chorda  basis;     /* via GLB loci nudi */
        MundusLocusRes* res;       /* res scriptae */
                   i32  numerus;
    MundusLocusSparsio* sparsiones;
                   i32  sparsionum;
                chorda  error;     /* Anglice; vacua si successus */
                   i32  linea;     /* ab I; 0 si ignota aut
                                    * successus */
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
    MundusLocusRes* res;       /* res scriptae, deinde sparsae ordine
                                * sparsionum; NIHIL si recusata */
               i32 numerus;
            chorda error;     /* Anglice; vacua si successus */
               i32 linea;     /* linea sparsionis culpatae; 0 si
                                * nulla */
} MundusLocusExplicatus;

/*
 * mundus_locus_explicare - sparsiones in res singulas explicare
 *
 * @d:     dispositio lecta (successus)
 * @basis: locus nudus, finitus: tabula gressus eius loca probat.
 *         NIHIL licet si dispositio sparsiones non habet
 *
 * Redde: res omnes (scriptae primae), aut FALSUM cum errore et
 * linea sparsionis quae numerum suum ponere non potuit; linea 0 si
 * argumenta NIHIL, dispositio non lecta aut memoria deficit
 */
MundusLocusExplicatus
mundus_locus_explicare (
    constans MundusLocusFructus* d,
                constans Statua* basis,
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
  * @res:     numerus res cum locis (mundus_locus_explicare)
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
