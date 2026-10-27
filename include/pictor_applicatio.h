/* pictor_applicatio.h - pictor sine scopo: volumen, documentum,
 * insulae (canones + domini e disco), registra, dispensator
 * (modulus 013 A4; Franus: compositio una, principalia duo)
 *
 * Principale fenestrae (pictor.c) et terminalis (pictor_terminalis.c)
 * hanc compositionem communem vocant et solum glutinum suum addunt
 * (ludus_fenestra aut ludus_tessera) - "applicatio in utroque scopo
 * sine #ifdef". Probatio iterationis (ludus_tessera) eandem vocat: app
 * vera, non copia. thema_initiare vocantis est (glutinum).
 */

#ifndef PICTOR_APPLICATIO_H
#define PICTOR_APPLICATIO_H

/* <aedilis corpus="lib/pictor_applicatio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_actiones.h"
#include "pictor_figurae.h"
#include "organum_montatio.h"
#include "tabula_dispersa.h"

/* MONTATIO (insula-rami-plan T1a): pictor in repositorio dato, ramo
 * <pictor id> (aut radice): documentum in spatio id, canones et
 * domini in ramo, contextus (ramus intus), registra propria. Contextus
 * HIC habitant - registra in eos monstrant: structuram ne copia. */
nomen structura {
       PictorDocumentum* doc;
            InsulaRamus  ramus;
         ActioRegistrum* actiones;
         PictorActiones  actiones_ctx;
        FiguraRegistrum* figurae;
          PictorFigurae  figurae_ctx;
       PictorCompositio  compositio;
         TabulaDispersa* documenta;   /* S3e: spatium -> documentum
                                       * (picturae iam ostensae) */
        StatusSystemata systemata;   /* S5b: relatio ad ligationem
                                       * novam (picturam_ponere) */
      PictorPinacotheca pinacotheca; /* T5: compositio, actiones,
                                       * figurae in eam monstrant */
} PictorMontatio;

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
     InsulaRepositorium* repo;
         PictorMontatio  montatio;
       PictorDocumentum* doc;        /* = montatio.doc */
         ActioRegistrum* actiones;   /* = montatio.actiones */
        FiguraRegistrum* figurae;    /* = montatio.figurae */
            Dispensator* d;
} PictorApplicatio;

/* S3e: picturam spatii (exsistentem) in montatione ostendere -
 * documentum ex memoria aut apertum; contextus actionum et figurarum
 * et dimensiones in insula (latitudo, altitudo; semitae R4c: pictura)
 * aptantur. FALSUM si nullum documentum ibi. */
b32
pictor_picturam_ponere (
         PictorMontatio* m,
     constans character* spatium);

/* S3f: picturam NOVAM (vacuam) in spatio condere et ostendere -
 * dimensiones ut tabula nova huius montationis (superficies, ut
 * pictor_montare); in memoriam (communem) ponitur. FALSUM si spatium
 * iam in bibliotheca est. */
b32
pictor_picturam_condere (
         PictorMontatio* m,
     constans character* spatium);

/* semitae R4c: argumentum lateris - URI ("pictor/flores") picturam
 * EXSTANTEM ostendit (semita numquam condit); cognomen "<spatium>" ut
 * ante: exstans ostenditur, ignota conditur si condere (S3f,
 * $pictor-new). FALSUM si semita mala aut pictura ignota. */
b32
pictor_argumentum_ponere (
          PictorMontatio* m,
      constans character* argumentum,
                     b32  condere);

/* semitae R4c (facies.semitam_secuta): post semitam a vico applicatam
 * (historia) documentum campum pictura sequitur; FALSUM si latus
 * nullum aut pictura ignota */
b32
pictor_semitam_secuta (
    vacuum* ctx);

/* pictorem montare: id NIHIL = radix repositorii (elementum radicis
 * 'pictor' iam exstat); aliter liberum <pictor id> creatur si abest.
 * Ordo: canones (manifestum infixum, genus pictor), elementum
 * initiale, domini. Tabula NOVA (vicus-latera S2c) magnitudine
 * prospectus superficiei minus margo cellulae utrimque (altitudo
 * etiam minus linea status; LXIV minimum); exstans suam servat.
 * systemata (status-plan S3-S5): conservatio - ramus registratur et
 * status servatus restituitur post elementum initiale, ante dominos;
 * relatio - post dominos ramus registratur et documentum ligatur
 * (verificatio statim). NIHIL = nulla (probationes). */
b32
pictor_montare (
         PictorMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
                    i32  latitudo,
                    i32  altitudo,
        StatusSystemata* systemata);

/* pinacotheca D8: picturam ultimam ex statu restituto (campus
 * 'pictura', conservatio) ostendere - pictura bibliothecae ponitur;
 * absens -> recentissima (pictor_documenta_recentia); vacua -> radix
 * manet. app->doc (et actiones, figurae) post hoc renovantur. Solum
 * applicatio fenestrae (apps/pictor/pictor.c) post aedificare vocat;
 * pictor terminalis numquam (spec D11). FALSUM si app NIHIL. */
b32
pictor_picturam_restituere (
    PictorApplicatio* app);

/* Argumenta communia: -fumus (volumen temporarium), -volumen <via>
 * (ordinarie "pictor.volumen"). Reddit volumen apertum aut NIHIL. */
Volumen*
pictor_volumen_aperire (
       Piscina*  piscina,
           s32   argc,
     character** argv,
           b32*  fumus);

/* Compositio tota super volumen apertum; latitudo/altitudo superficiei
 * in pixelis nostris (compositio pictoris). FALSUM si pars deficit
 * (causa in stderr). */
b32
pictor_applicatio_aedificare (
       PictorApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo);

#endif /* PICTOR_APPLICATIO_H */
