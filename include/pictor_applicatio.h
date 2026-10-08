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

/* pictorem montare: id NIHIL = radix repositorii (elementum radicis
 * 'pictor' iam exstat); aliter liberum <pictor id> creatur si abest.
 * Ordo: canones, elementum initiale, domini. radix = praefixum viarum
 * canonum. Tabula NOVA (vicus-latera S2c) magnitudine prospectus
 * superficiei: latitudo, altitudo minus linea status (LXIV minimum);
 * exstans suam servat. */
b32
pictor_montare (
         PictorMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo);

/* Argumenta communia: -fumus (volumen temporarium), -volumen <via>
 * (ordinarie "pictor.volumen"). Reddit volumen apertum aut NIHIL. */
Volumen*
pictor_volumen_aperire (
       Piscina*  piscina,
           s32   argc,
     character** argv,
           b32*  fumus);

/* Compositio tota super volumen apertum. radix = praefixum viarum
 * canonum (NIHIL = directorium currens); latitudo/altitudo superficiei
 * in pixelis nostris (compositio pictoris). FALSUM si pars deficit
 * (causa in stderr). */
b32
pictor_applicatio_aedificare (
       PictorApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo);

#endif /* PICTOR_APPLICATIO_H */
