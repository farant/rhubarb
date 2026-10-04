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

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
       PictorDocumentum* doc;
     InsulaRepositorium* repo;
         ActioRegistrum* actiones;
         PictorActiones  actiones_ctx;
        FiguraRegistrum* figurae;
          PictorFigurae  figurae_ctx;
       PictorCompositio  compositio;
            Dispensator* d;
} PictorApplicatio;

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
