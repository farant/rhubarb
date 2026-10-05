/* scriba_applicatio.h - scriba sine scopo: volumen, documentum,
 * insulae (canones + domini e disco), registra, dispensator, gestus
 * (scriba-plan S3; exemplar pictor_applicatio, modulus 013 A4)
 *
 * Principale fenestrae (scriba.c) et terminalis (scriba_terminalis.c)
 * hanc compositionem communem vocant et solum glutinum suum addunt.
 * Probatio iterationis (ludus_tessera) eandem vocat: app vera.
 * thema_initiare vocantis est (glutinum). Folium novum LXVIII x LVI
 * (TabulaCharacterum defalta); volumen exsistens folium suum fert. */

#ifndef SCRIBA_APPLICATIO_H
#define SCRIBA_APPLICATIO_H

/* <aedilis corpus="lib/scriba_applicatio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "scriba_documentum.h"
#include "scriba_actiones.h"
#include "scriba_componentia.h"
#include "scriba_figurae.h"

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
       ScribaDocumentum* doc;
     InsulaRepositorium* repo;
         ActioRegistrum* actiones;
         ScribaActiones  actiones_ctx;
        FiguraRegistrum* figurae;
          ScribaFigurae  figurae_ctx;
       ScribaCompositio  compositio;
            Dispensator* d;
} ScribaApplicatio;

/* Argumenta communia: -fumus (volumen temporarium), -volumen <via>
 * (ordinarie "scriba.volumen"). Reddit volumen apertum aut NIHIL. */
Volumen*
scriba_volumen_aperire (
       Piscina*  piscina,
           s32   argc,
     character** argv,
           b32*  fumus);

/* Compositio tota super volumen apertum. radix = praefixum viarum
 * canonum (NIHIL = directorium currens); latitudo/altitudo superficiei
 * in pixelis nostris. FALSUM si pars deficit (causa in stderr). */
b32
scriba_applicatio_aedificare (
       ScribaApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo);

#endif /* SCRIBA_APPLICATIO_H */
