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
 * SPECULUM: positura imago alterius.
 *
 *   <positura titulus="contactus_dex" speculum="contactus_sin">
 *     <art titulus="caput" rotatio="0 5 0"/>
 *   </positura>
 *
 * Imago trans planum x = 0 (sinistrum et dextrum): articuli "X_sin"
 * et "X_dex" valores permutant; rotatio (x y z) fit (x -y -z), motus
 * (x y z) fit (-x y z), scala manet. Articulus sine "_sin" / "_dex"
 * in loco suo reflectitur. <art> liberi post imaginem valores
 * substituunt, ut in 'ex'. 'speculum' et 'ex' simul non licent.
 *
 * OSSA: plagula ossium, a figuris pluribus communis.
 *
 *   <ossa titulus="bipes">
 *     <articulus titulus="pelvis" locus="0 1 0">
 *       <articulus titulus="femur_sin" locus="0.2 0 0"/>
 *     </articulus>
 *     <positura .../> <cyclus .../>
 *   </ossa>
 *
 * Arborem articulorum (solida non tenet), posituras et cyclos fert.
 *
 *   <figura titulus="goblin" ossa="bipes">
 *     <articulus titulus="femur_sin" locus="0.15 0 0">
 *       <capsa .../>
 *     </articulus>
 *   </figura>
 *
 * Figura cum 'ossa' articulos PLANOS enumerat (non nidificatos):
 * quisque solida in articulum ossium eius tituli suspendit et 'locus'
 * novum dare potest (proportiones); gyrus et rotatio ossium manent.
 * Articulus ossium non nominatus nodus vacuus manet. Ordo nodorum:
 * arbor ossium; in quoque articulo solida figurae ante articulos
 * liberos. Positura aut cyclus figurae eiusdem tituli ossium
 * substituit; cyclus ossium posituram substitutam accipit. ('ex' et
 * 'speculum' valores TUNC transcribunt cum leguntur: positura ossium
 * ex alia derivata substitutionem posteriorem illius non sequitur.)
 *
 * MOTUS RADICIS: motus in positura ossium multiplicatur per
 * (altitudo articuli radicalis in figura / in ossibus); ossium
 * altitudo 0: factor I. Rotationes et scalae non mutantur. Motus in
 * positura figurae ipsius non multiplicatur.
 *
 * RECUSAT praeterea: speculum ignotum, posterius aut cum 'ex';
 * "X_sin" sine "X_dex" (aut contra) in positura speculata; par cuius
 * quies imago non est (locus, rotatio aut parens); ossa ignota aut
 * fontem absentem; titulum ossium extra a-z 0-9 _; <ossa> cum solidis
 * aut cum 'ossa'; in figura cum ossibus articulum ignotum, iteratum,
 * nidificatum, aut cum gyro / rotatione. Error in plagula ossium:
 * 'plagula' titulum ossium fert et 'linea' lineam ILLIUS plagulae.
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
              chorda  plagula;     /* vacua: error in figura ipsa (aut
                                    * successus); aliter titulus
                                    * ossium ubi error stat */
} PlasticaFiguraFructus;

/*
 * PlasticaFons - plagulam ossium titulo dare
 *
 * @titulus:   valor attributi 'ossa' (a-z 0-9 _ solum)
 * @contextus: ut plastica_figura_legere_cum_fonte datus
 *
 * Redde: textus plagulae totus; chorda vacua si deest
 */
nomen chorda (*PlasticaFons) (
    chorda  titulus,
    vacuum* contextus);

/*
 * plastica_figura_legere - figuram cum motu in statuam vertere
 *
 * @textus:  plagula tota, radix <figura>
 * @semen:   ut plastica_legere_seminis (0 = nullum)
 * @piscina: statua, tabulae et nuntius
 *
 * Figura sine positura et sine cyclo licet: statua sine cyclis.
 * Figura quae 'ossa' nominat recusatur ("skeleton sheets need a
 * source"): vide plastica_figura_legere_cum_fonte.
 */
PlasticaFiguraFructus
plastica_figura_legere (
     chorda  textus,
        i32  semen,
    Piscina* piscina);

/*
 * plastica_figura_legere_cum_fonte - ut plastica_figura_legere, cum
 * plagulis ossium
 *
 * @fons:      plagulam ossium titulo dat; NIHIL: figura cum 'ossa'
 *             recusatur
 * @contextus: fonti traditur
 *
 * Fons semel vocatur, et solum si figura 'ossa' nominat.
 */
PlasticaFiguraFructus
plastica_figura_legere_cum_fonte (
          chorda  textus,
             i32  semen,
    PlasticaFons  fons,
          vacuum* contextus,
         Piscina* piscina);

#endif /* PLASTICA_MOTUS_H */
