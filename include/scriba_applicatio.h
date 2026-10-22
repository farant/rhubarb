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
#include "organum_montatio.h"

/* MONTATIO (insula-rami-plan T1a): scriba in repositorio dato, ramo
 * <scriba id> (aut radice): documentum in spatio id, canones et
 * domini in ramo, contextus (ramus intus), registra propria. Contextus
 * HIC habitant - registra in eos monstrant: structuram ne copia. */
nomen structura {
       ScribaDocumentum* doc;
            InsulaRamus  ramus;
         ActioRegistrum* actiones;
         ScribaActiones  actiones_ctx;
        FiguraRegistrum* figurae;
          ScribaFigurae  figurae_ctx;
       ScribaCompositio  compositio;
    /* S2b: liber paginarum communis (NIHIL = documentum proprium);
     * doc = pagina CURRENS - scriba_reficere eum cum actionibus
     * concordat post navigationem */
            ScribaLiber* liber;
} ScribaMontatio;

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
     InsulaRepositorium* repo;
         ScribaMontatio  montatio;
       ScribaDocumentum* doc;        /* = montatio.doc */
         ActioRegistrum* actiones;   /* = montatio.actiones */
        FiguraRegistrum* figurae;    /* = montatio.figurae */
            Dispensator* d;
} ScribaApplicatio;

/* scribam montare: id NIHIL = radix repositorii (elementum radicis
 * 'scriba' iam exstat); aliter liberum <scriba id> creatur si abest.
 * Ordo: canones, elementum initiale, domini (scriptura initialis
 * anonyma ne recusetur). Folium NOVUM (vicus-latera S2c) magnitudine
 * superficiei: cellulae minus margines et linea status (XX x X
 * minimum); exstans suum servat. liber (S2b): visus paginae libri -
 * pagina ex plagula 'scriba/visus/<id>' (absens: prima), documentum
 * libri (commune); NIHIL = documentum proprium in spatio id (mos
 * prior). systemata (status-plan S6b): canon, initium, domini ex
 * manifesto infixo (genus scriba) per organum_ramum_parare;
 * conservatio cursorem servat; NIHIL = nulla (probationes). */
b32
scriba_montare (
         ScribaMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     InsulaRepositorium* repo,
     constans character* id,
                    i32  latitudo,
                    i32  altitudo,
            ScribaLiber* liber,
        StatusSystemata* systemata);

/* A6c: scribam montare SUB ramo parentis (nidificata, status-plan S8):
 * <scriba id> liberum parentis (creatur si abest); cetera ut
 * scriba_montare (liber, systemata, visus paginae 'scriba/visus/' +
 * id, clavis status id) - id unicum in volumine esse debet (canon:
 * nomen - "c1_notae", non "c1-notae") */
b32
scriba_montare_in_ramo (
          ScribaMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
                 Volumen* volumen,
    constans InsulaRamus* parens,
      constans character* id,
                     i32  latitudo,
                     i32  altitudo,
             ScribaLiber* liber,
         StatusSystemata* systemata);

/* A6d: visum montatum ad librum alium vertere (calendarium: liber
 * diei): pagina ex plagula visus si in libro novo exstat, aliter
 * prima; registra et contextus manent. FALSUM si liber NIHIL. */
b32
scriba_librum_ponere (
    ScribaMontatio* m,
       ScribaLiber* liber);

/* D5: visum montatum ad paginam nominatam libri sui vertere (plagula
 * visus scribitur, ut navigatio). FALSUM si liber NIHIL aut nomen
 * ignotum. */
b32
scriba_paginam_ostendere (
    ScribaMontatio* m,
            chorda  titulus);

/* Visus stalus reficitur (vicus-latera S2b): si alius visus paginam
 * commisit (cursor documenti != cursor_laboris), folium laboris ex
 * proiectione - visus sine gestu pendente solum (commissio in omni
 * commutatione foci et tabulae: solus visus focatus scribit). m->doc
 * cum actionibus concordat (post navigationem). VERUM si mutatum
 * (quadrum pingendum). Vicus in pulsu et ante focum vocat. */
b32
scriba_reficere (
    ScribaMontatio* m);

/* Argumenta communia: -fumus (volumen temporarium), -volumen <via>
 * (ordinarie "scriba.volumen"). Reddit volumen apertum aut NIHIL. */
Volumen*
scriba_volumen_aperire (
       Piscina*  piscina,
           s32   argc,
     character** argv,
           b32*  fumus);

/* Compositio tota super volumen apertum; latitudo/altitudo superficiei
 * in pixelis nostris. FALSUM si pars deficit (causa in stderr). */
b32
scriba_applicatio_aedificare (
       ScribaApplicatio* app,
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo);

#endif /* SCRIBA_APPLICATIO_H */
