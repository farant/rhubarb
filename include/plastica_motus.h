/*
 * plastica_motus.h - figura cum motu: positurae et cycli in Statuam
 *
 * Plagula <figura> (plastica.h) praeter solida et articulos POSITURAS
 * et CYCLOS tenet; plastica_figura_legere eam in Statuam finitam cum
 * cyclis vertit, quam omnia quae statuam movent sine mutatione
 * accipiunt (project-specs/plastica-motus-spec.md IV.2, IV.3, IV.5).
 * Nihil de mundo scit.
 *
 * POSITURA: quid articuli a quiete differant.
 *
 *   <positura titulus="compressa" ex="alia">
 *     <art titulus="corpus" rotatio="0 0 10" scala="1.2 0.7 1.2"
 *          motus="0 -0.1 0"/>
 *   </positura>
 *
 *   rotatio="x y z"  gradus, ad quietem ADDITI in spatio articuli
 *                    (circa x, deinde y, deinde z, ut 'rotatio'
 *                    plasticae); 0 0 0 = quies
 *   scala="s" aut "x y z"   quietem multiplicat; maior quam 0
 *   motus="x y z"    loco quietis additur; SOLUM in articulo cuius
 *                    parens radix figurae est
 *   ex="alia"        a positura PRIUS scripta incipit; <art> eius
 *                    articuli valores substituit
 *
 * <art> unum saltem ex tribus fert. Articulus non nominatus in quiete
 * manet. Positura "quies" semper adest (nihil mutat) et scribi non
 * potest. Maxime LXIV positurae.
 *
 * CYCLUS: positurae per tempus.
 *
 *   <cyclus titulus="walk" duratio="0.8" circulus="verum">
 *     <clavis tempus="0" positura="quies"/>
 *     <clavis tempus="0.4" positura="compressa"/>
 *     <momentum titulus="ictus" tempus="0.4"/>
 *   </cyclus>
 *
 *   duratio    secundae, maior quam 0, maxime LX
 *   circulus   "verum": positura prima ad 'duratio' iteratur (sutura
 *              clausa); clavis ultima ante 'duratio' stet.
 *              Ordinarius "falsum": positura ultima usque ad
 *              'duratio' tenetur
 *   clavis     prima ad tempus 0; tempora crescentia, non ultra
 *              'duratio'. Maxime LXIV per cyclum
 *   momentum   instans nominatum (ictus impetus); 0 .. duratio;
 *              titulus in cyclo unicus; maxime VIII per cyclum.
 *              Statuam non mutat: in 'momenta' redditur
 *
 * Inter claves linearis (rotatio: via brevissima). Maxime XVI cycli.
 *
 * COCTIO: articulus quem positura cycli tangit canalem accipit pro
 * quaque proprietate tacta (rotatio, scala, motus), cum clave ad
 * quodque tempus cycli; ceteri canalem nullum habent. Tituli cyclorum
 * ut scripti ("walk", "idle": mundus eos titulo invenit).
 *
 * RECUSAT, causa Anglice et linea: radicem aliam quam <figura>;
 * omnia quae plastica_legere recusat; articulum ignotum aut bis in
 * positura; <art> sine valore; motum in articulo non radicali;
 * scalam non positivam; titulum positurae aut cycli absentem aut
 * iteratum, aut "quies" scriptum; 'ex' ignotum aut posterius;
 * cyclum sine clavibus, durationem absentem aut non positivam,
 * clavem primam non ad 0, tempora non crescentia aut ultra
 * durationem, posituram ignotam; rotationem quae inter claves
 * vicinas CLXXX gradus aut plus in axe uno mutatur ("add a key
 * between": via brevissima aliter iret quam scriptum est) - clavis
 * suturae circuli modulo CCCLX confertur, ut gyrus plenus liceat;
 * momentum extra cyclum aut iteratum; <membrum> ("limbs are not
 * built yet"); elementum aut attributum ignotum. Figura recusata
 * NIHIL est.
 *
 * DETERMINATA: textus idem statuam eandem dat.
 */

#ifndef PLASTICA_MOTUS_H
#define PLASTICA_MOTUS_H

/* <aedilis corpus="lib/plastica_motus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "statua.h"
#include "plastica.h"

#define PLASTICA_MOTUS_POSITURAE_MAXIMAE  LXIV
#define PLASTICA_MOTUS_CYCLI_MAXIMI       XVI
#define PLASTICA_MOTUS_CLAVES_MAXIMAE     LXIV
#define PLASTICA_MOTUS_MOMENTA_MAXIMA     VIII

nomen structura {
       i32 cyclus;     /* index in statua->cycli */
    chorda titulus;
       f32 tempus;     /* secundae */
} PlasticaMomentum;

nomen structura {
                 b32  successus;
              Statua* statua;      /* finita, cum cyclis; NIHIL si
                                    * recusata */
              chorda* partes;      /* ut PlasticaFructus */
                 i32* articuli;
                 i32  articulorum;
    PlasticaMomentum* momenta;     /* ordine plagulae; NIHIL si nulla */
                 i32  momentorum;
              chorda  error;
                 i32  linea;
} PlasticaFiguraFructus;

/*
 * plastica_figura_legere - figuram cum motu in statuam vertere
 *
 * @textus:  plagula tota, radix <figura>
 * @semen:   ut plastica_legere_seminis (0 = nullum)
 * @piscina: statua, tabulae et nuntius
 *
 * Figura sine positura et sine cyclo licet: statua sine cyclis.
 */
PlasticaFiguraFructus
plastica_figura_legere (
     chorda  textus,
        i32  semen,
    Piscina* piscina);

#endif /* PLASTICA_MOTUS_H */
