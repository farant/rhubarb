/* gabc_visus.h - VISUS GABC: cantus unus ex textu gabc in memoria
 * monstratus, paginatus, sonans (spec project-specs/psalterium-spec.md
 * D4, II; planum project-specs/psalterium-plan.md T1-T2).
 *
 * UNDE: ex lib/antiphonale.c extractum (gabc pars II P6-P7, spec
 * project-specs/gabc-spec.md V, G11, G18). Latus antiphonale plagulam
 * legit et speculatur; psalterium textum ab aptatore accipit. Utrique
 * idem visus: versio, compositio, paginae, spatium, syllaba electa,
 * sonus cum syllaba illuminata, pictura, linea ima, vexillum. Unde
 * textus venit visus nescit - involucrum (latus) ei dicit "ecce textus
 * novus".
 *
 * VERSIO (G18): textus bonus (gabc_successus) = versio nova, unitas
 * una cum piscina sua (textus copiatus, cantus, carmen, nexus,
 * compositio); vetus solum destruitur postquam nova aedificata est.
 * Textus malus versionem bonam ultimam servat et VEXILLUM "linea:col:
 * causa" ponit; textus bonus quilibet (etiam idem) vexillum tollit.
 * Textus idem ac versio currens: versio manet, nihil sonans mutatur.
 *
 * CONTINUARE: versio nova dum sonat -
 *   VERUM  (antiphonale, permutatio viva): sonus ad pulsum eundem in
 *          versionem novam transit; brevior aut non sonabilis sistit
 *          (vexillum "not playable: causa"); syllaba electa indicem
 *          servat, ad ultimam coercita.
 *   FALSUM (psalterium, tonus aut psalmus mutatus): melodia alia,
 *          ergo sonus sistit; electa -1 (Spatium ab initio canit).
 *
 * STATUS IN RAMO: campi "pagina" (ab 0; ultra finem = ultima) et
 * "spatium" (par VI..XLVIII, absens VIII) in ramo involucri, scriptore
 * actione involucri - manifestum involucri eos declarat. Superficies
 * ex "superficies_latitudo/_altitudo" rami, aliter latitudo/altitudo
 * creationis, aliter CDLXXX x CCCXX.
 *
 * DISPOSITIO: fundus intra limbum (VI x VIII); margo_supra pixellis
 * sub limbo superiore involucro relinquitur (lineae suae, ut caput
 * psalterii); pagina compositionis infra; linea ima "< ▶ titulus  2/5
 * >" (titulus ab involucro datus). Vexillum supra paginam.
 * NODI (gabc_visus_componere sub radice involucri): "sagitta_prior",
 * "sagitta_proxima", "globulus_canere"; radix ipsa (radix_id) ictu
 * syllabam eligit. Omnes actionem involucri ferunt.
 *
 * CLAVES: involucrum tabulam suam Praeceptorum tenet (tituli et claves
 * sui) et actiones infra vocat - visus nullam clavem novit.
 *
 * SONUS per portam (GabcVisusPorta): visus sonum non novit - hospes
 * portam ponit (vicus_sonus.h carmen_*). Porta nulla = mutum.
 *
 * USUS (antiphonale):
 *   visus = gabc_visus_creare(p, &m->ramus, "antiphonale.agere",
 *       "antiphonale", porta, latitudo, altitudo, 0);
 *   pulsus speculae: gabc_visus_textum_ponere(visus, textus, VERUM);
 *   plagula absens: gabc_visus_vexillum_ponere(visus, "missing: ...");
 * USUS (psalterium):
 *   tonus mutatus: gabc_visus_textum_ponere(visus, gabc, FALSUM);
 *   admonitio aptatoris: gabc_visus_vexillum_ponere(visus, causa);
 */
#ifndef GABC_VISUS_H
#define GABC_VISUS_H

/* <aedilis corpus="lib/gabc_visus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "componens.h"
#include "mandatum.h"
#include "eventus.h"
#include "fasti.h"

/* carmen = textus libri <carmina> (gabc_carmen.h + involucrum): porta
 * eum legit (visus carmen.h nescit - stratum soni). Ex
 * AntiphonalePorta translata (antiphonale.h nomen vetus servat). */
nomen structura {
    b32    (*incipere)(vacuum* ctx, chorda carmen, s64 pulsus);
    vacuum (*quaerere)(vacuum* ctx, s64 pulsus);
    vacuum (*sistere)(vacuum* ctx);
    s64    (*pulsus)(vacuum* ctx);       /* -1 si non sonat */
    vacuum* ctx;
} GabcVisusPorta;

nomen structura GabcVisus GabcVisus;


/* ==================================================
 * Creatio et textus
 * ================================================== */

/* visus vacuus (sine versione). ramus = ramus involucri (manet in
 * vita visus); actio = scriptor campi "pagina"/"spatium" et actio
 * nodorum; radix_id = titulus radicis involucri (ictus syllabam
 * eligit); porta NIHIL = muta (copiatur); latitudo/altitudo 0 =
 * ordinariae; margo_supra = pixelli supra paginam involucri. NIHIL
 * solum si memoria deficit. */
GabcVisus*
gabc_visus_creare (
                     Piscina* piscina,
                 InsulaRamus* ramus,
          constans character* actio,
          constans character* radix_id,
     constans GabcVisusPorta* porta,
                         i32  latitudo,
                         i32  altitudo,
                         i32  margo_supra);

/* textus gabc (copiatur) -> versio (VERSIO, CONTINUARE supra).
 * VERUM si quid pingendum mutatum (versio aut vexillum). */
b32
gabc_visus_textum_ponere (
     GabcVisus* visus,
        chorda  textus,
           b32  continuare);

/* versionem tollit: sonus sistit, electa -1, vexillum vacuum */
vacuum
gabc_visus_vacuare (
    GabcVisus* visus);

/* vexillum involucri ("missing: via", admonitio aptatoris); "" aut
 * NIHIL tollit. Textus bonus sequens id quoque tollit. */
vacuum
gabc_visus_vexillum_ponere (
             GabcVisus* visus,
    constans character* vexillum);

/* "" si nullum */
constans character*
gabc_visus_vexillum (
    constans GabcVisus* visus);

/* caput name: versionis; vacua si nulla aut sine nomine */
chorda
gabc_visus_titulus (
    constans GabcVisus* visus);

b32
gabc_visus_habet_versionem (
    constans GabcVisus* visus);


/* ==================================================
 * Actiones (involucrum ex tabula clavium vocat; VERUM = consumptum)
 * ================================================== */

/* Spatium: sonans sistit (electa = illuminata); aliter ab electa (aut
 * nota ultima si electa post eam, aut initio) canit. Non sonabile:
 * vexillum "not playable: causa"; porta recusat: "can't play: sound
 * muted or unavailable". */
b32
gabc_visus_canere (
    GabcVisus* visus);

/* sonans: sistit sine electione; aliter nihil */
b32
gabc_visus_sistere (
    GabcVisus* visus);

/* syllaba prior (-I) aut proxima (+I): sonans quaerit, aliter eligit */
b32
gabc_visus_syllabam_movere (
     GabcVisus* visus,
           s32  directio);

/* pagina +/- gradus; extra fines nihil */
b32
gabc_visus_paginam_movere (
     GabcVisus* visus,
           s32  gradus);

/* pagina absoluta (ab 0; ultra finem in pictura coercetur) */
b32
gabc_visus_paginam_ponere (
     GabcVisus* visus,
           s32  pagina);

/* spatium +/- gradus (VI..XLVIII); syllaba prima visa in conspectu
 * manet */
b32
gabc_visus_spatium_movere (
     GabcVisus* visus,
           s32  gradus);

/* ictus muris sinister in nodo visus (sagittae, globulus, radix:
 * syllaba sub puncto). FALSUM si nodus non visus aut nihil sub eo. */
b32
gabc_visus_premere (
               GabcVisus* visus,
               Componens* nodus,
        constans Eventus* eventus);


/* ==================================================
 * Componere, pingere, pulsus
 * ================================================== */

/* fines radicis = superficies currens; compositio pigra ad eam;
 * nodi lineae imae sub radice involucri additi */
vacuum
gabc_visus_componere (
              GabcVisus* visus,
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern);

/* figura tota (fundus, pagina, linea ima, vexillum) in fines
 * radicis. focatum = textura foci in margine. titulus = lineae imae.
 * nuntius_vacuus (si non NIHIL) pingitur cum versio nulla est
 * ("Open a chant with $antiphonale(path)"). */
vacuum
gabc_visus_pingere (
              GabcVisus* visus,
                Mandata* mandata,
                  Fines  totum,
                    b32  focatum,
                 chorda  titulus,
     constans character* nuntius_vacuus);

/* sonus: syllaba nexus currentis illuminatur, pagina eam sequitur;
 * porta silens = sonus finitus. VERUM si pingendum. */
b32
gabc_visus_pulsare (
    GabcVisus* visus);

b32
gabc_visus_sonat (
    constans GabcVisus* visus);

/* MORA_SONANS (XXXIII ms) dum sonat, aliter -1 (involucrum suam
 * moram addit: specula, relectio) */
Mora
gabc_visus_mora (
    constans GabcVisus* visus);

#endif /* GABC_VISUS_H */
