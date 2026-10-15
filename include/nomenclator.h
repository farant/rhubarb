/* nomenclator.h - NOMENCLATOR (contactus; nomenclator-plan P2): index
 * personarum in latere vici. Manifestum apps/nomenclator/organum.stml
 * (genus "nomenclator"); documentum personarum per ligamen (a
 * parente, commune omnibus lateribus); status visus in ramo insulae
 * (persona_electa, pagina). Index per album (membrum__album),
 * praecepta per membrum__praecepta. */
#ifndef NOMENCLATOR_H
#define NOMENCLATOR_H

/* <aedilis corpus="lib/nomenclator.c"/> */

#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "personae.h"
#include "organum_montatio.h"

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
      PersonaeDocumentum* documentum;   /* ligamen (a parente) */
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* figurae_ctx;  /* privatum */
} NomenclatorMontatio;

/* montare in ramo <nomenclator id="..."> repositorii: organum ex
 * manifesto infixo, actio nomenclator.agere, figurae; superficies
 * latitudo x altitudo (0 = ordinaria); systemata NIHIL licet */
b32
nomenclator_montare (
     NomenclatorMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
      PersonaeDocumentum* documentum,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = NomenclatorMontatio* */
Componens*
nomenclator_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* persona electa: campus persona_electa si viva, aliter prima indicis;
 * 0 si nulla */
s32
nomenclator_persona_electa (
    constans NomenclatorMontatio* m);

#endif /* NOMENCLATOR_H */
