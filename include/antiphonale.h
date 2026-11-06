/* antiphonale.h - ANTIPHONALE: cantus gabc in latere vici (genus
 * "antiphonale"; gabc pars II P6-P7, spec project-specs/gabc-spec.md
 * §V, G11, G13, G17, G18). Manifestum apps/antiphonale/organum.stml;
 * status visus (pagina, spatium) in ramo.
 *
 * PLAGULA SEQUITUR (G13): argumentum = via plagulae .gabc; specula
 * (specula.h) eam speculatur - scriptura, deletio, renominatio
 * pulsum expergefaciunt (fossa), pulsus legit. VERSIO BONA =
 * gabc_successus; versio mala (vitium, plagula vacua, absens) versionem
 * bonam ultimam servat et VEXILLUM in summo pingit ("3:8: causa",
 * "empty file", "missing: via"); versio bona sequens vexillum tollit
 * sine clave. Versio = unitas una cum piscina sua (textus, cantus,
 * compositio); vetus solum destruitur postquam nova aedificata est.
 *
 * PICTURA: fundus planus intra limbum (VI x VIII; textura foci solum
 * in margine, ut navigator); pagina compositionis (gabc_compositio.h,
 * gabc_pictura.h) ad latitudinem intra limbum; linea ima "< titulus ·
 * 2/5 >" (sagittae ictu paginam vertunt). Titulus = caput name,
 * aliter nomen plagulae.
 * CLAVES: PageDown / PageUp (et j / k) paginae; + (aut =) et - spatium
 * (VI..XLVIII, gradu II) - syllaba prima visa in conspectu manet.
 * Compositio pigre in componere cum latitudo aut spatium mutatur.
 *
 * SONUS (P7) per portam (AntiphonalePorta): latus sonum non novit -
 * hospes portam ponit (vicus_sonus.h carmen_*). Porta nulla = mutum.
 */
#ifndef ANTIPHONALE_H
#define ANTIPHONALE_H

/* <aedilis corpus="lib/antiphonale.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "fasti.h"

nomen structura {
    /* carmen = textus libri <carmina> (gabc_carmen.h + involucrum):
     * porta eum legit (latus carmen.h nescit - stratum soni) */
    b32    (*incipere)(vacuum* ctx, chorda carmen, s64 pulsus);
    vacuum (*quaerere)(vacuum* ctx, s64 pulsus);
    vacuum (*sistere)(vacuum* ctx);
    s64    (*pulsus)(vacuum* ctx);       /* -1 si non sonat */
    vacuum* ctx;
} AntiphonalePorta;

nomen structura {
          InsulaRamus  ramus;
       ActioRegistrum* actiones;
      FiguraRegistrum* figurae;
     AntiphonalePorta  porta;       /* incipere NIHIL = mutum */
              Piscina* piscina;
  InternamentumChorda* intern;
                  i32  latitudo;    /* superficies montationis */
                  i32  altitudo;
               vacuum* figurae_ctx; /* privatum: versio, specula,
                                     * vexillum */
} AntiphonaleMontatio;

/* montare in ramo <antiphonale id> (manifestum infixum) super plagulam
 * via (NIHIL aut "" = nulla: nuntium "Open a chant with
 * $antiphonale(path)"); porta NIHIL = muta (copiatur); systemata NIHIL
 * licet. Plagula statim legitur. */
b32
antiphonale_montare (
          AntiphonaleMontatio* m,
                      Piscina* piscina,
          InternamentumChorda* intern,
           InsulaRepositorium* repo,
           constans character* id,
           constans character* via,
                          i32  latitudo,
                          i32  altitudo,
    constans AntiphonalePorta* porta,
              StatusSystemata* systemata);

/* compositor (dispensator; facies vici); ctx = AntiphonaleMontatio* */
Componens*
antiphonale_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* plagula alia (vicus_acervo_mutare): specula nova, pagina 0; via
 * vacua aut NIHIL: FALSUM, nihil mutatum */
b32
antiphonale_argumentum_ponere (
       AntiphonaleMontatio* m,
        constans character* via);

/* pulsus (vicus: facies.pulsare involvit): specula exhausta, plagula
 * mutata relegitur; VERUM = versio aut vexillum mutatum (pingendum) */
b32
antiphonale_pulsare (
    vacuum* ctx);

/* facies.mora: -1 dum otiosum (specula expergefacit); sine specula
 * (platforma, directorium absens) CCL - relegit et speculam iterum
 * temptat */
Mora
antiphonale_mora (
    vacuum* ctx);

/* facies.fossa: fossa speculae (REACTOR_LEGERE); -1 nulla */
s32
antiphonale_fossa (
    vacuum* ctx,
       i32* eventus);

/* facies.locus_titulus: caput name, aliter nomen plagulae */
chorda
antiphonale_titulus (
     vacuum* ctx,
    Piscina* piscina);

#endif /* ANTIPHONALE_H */
