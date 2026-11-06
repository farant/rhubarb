/* adumbratio.h - ADUMBRATIO: forma adumbrationum (mock-ups) laterum
 * vici (negotia-paginae T2; spec project-specs/negotia-paginae.md V).
 * Documentum STML -> Mandata per easdem functiones quas latera vera
 * vocant (dispositio in cellulis VI x VIII, figura.h), ergo quod
 * Franus videt est quod latus esset. Eadem Mandata in vico pinguntur
 * et in PNG scribuntur.
 *
 * MACRONES (stml_macros.h, spatium templorum '#@'): <#@ordo t="@t">
 * ...</#> semel, deinde <<#@ordo t="library">> per exemplum -
 * expansio ante omnia; vitium expansionis (fragmentum ignotum,
 * loculus non impletus, argumentum superfluum) nominatur cum linea.
 * Fragmentum aut vocatio SINE '@' (transclusio contenti, non
 * templum) vitium est - numquam tacite nihil pingitur.
 *
 * RADIX: <adumbratio latitudo="600" altitudo="700"> (pixela; absens =
 * ordinaria vocantis). Fundus COLOR_BACKGROUND.
 *
 * CAPSAE (nodi dispositionis):
 *   <columna> liberi deorsum      <linea> liberi dextrorsum
 *   latitudo / altitudo = "12" (cellulae, fixa) | "apta" (ordinaria:
 *     contento) | "crescens" (spatio reliquo) | "40%" (pars parentis)
 *   spatium="1" (margo interior, cellulae)  intervallum="1"
 *   allineatio_x / allineatio_y = initium | medium | finis
 *   fundus="<color>" (impletum)  margo="<color>" (linea 1px)
 * CAPSAE FIGURAE (capsae cum pictura figura.h, liberos habent):
 *   <ordo electus="verum">   ordo electus (figura_ordinem_electum)
 *   <ordo super="verum">     ordo sub mure (figura_ordinem_super)
 *   <mensa focata="verum">   mensa lateris (figura_mensam_pingere)
 *   <fascia>                 titulus gregis (figura_fasciam_pingere)
 *   <campus>                 campus (figura_campum_pingere)
 * FOLIA:
 *   <textus fons="parvus|titulus" color="<color>" audax="verum">..
 *     </textus>   (latitudo apta = longitudo textus)
 *   <globulum status="shipped|partial|planned"/> aut
 *     <globulum textus=".." electum="verum" obscurum="verum"/>
 *   <sagitta directio="sinistra|dextra" obscura="verum"/>
 *   <capsa facta="verum"/>
 *   <spatium/>   (spatium crescens)
 *   <mandata latitudo altitudo> sex primitiva in forma
 *     mandata_legere_stml (coetus rectangulum linea polygonum imago
 *     textus), coordinatae ab origine capsae suae - EFFUGIUM ad
 *     picturam liberam </mandata>
 * COLORES (nomina gradus thematis): fundus superficies textus
 *   obscurus selectio margo accentus accentus2 monitum vitium
 *   successus.
 *
 * VITIA: numquam latus vacuum - documentum malum, elementum aut
 * attributum ignotum, valor malus, vitium expansionis: Mandata =
 * pagina vitii ("Mock-up error" + nuntius), vitium = "linea N: ..." */

#ifndef ADUMBRATIO_H
#define ADUMBRATIO_H

/* <aedilis corpus="lib/adumbratio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "mandatum.h"

nomen structura {
     Mandata* mandata;     /* semper (pagina vitii si vitium) */
         s32  latitudo;    /* superficies (pixela) */
         s32  altitudo;
      chorda  vitium;      /* vacua = bene */
} AdumbratioResultus;

AdumbratioResultus
adumbrationem_reddere (
                 chorda  documentum,
                    s32  latitudo_ordinaria,
                    s32  altitudo_ordinaria,
                Piscina* piscina,
    InternamentumChorda* intern);

/* PNG latitudo x altitudo resultus; FALSUM si scriptura defecit */
b32
adumbrationem_png_scribere (
    constans AdumbratioResultus* r,
             constans character* via,
                        Piscina* piscina);

/* r in md intra fines: coetus cum sectione, origo = angulus sinister
 * superus finium; mandata r transcribuntur (chordae in piscina r
 * manent - r vivat dum md pingitur) */
vacuum
adumbrationem_inserere (
     constans AdumbratioResultus* r,
                         Mandata* md,
                           Fines  fines);

#endif /* ADUMBRATIO_H */
