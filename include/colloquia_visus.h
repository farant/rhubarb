/* colloquia_visus.h - visus sessionum Claude Code (genus "colloquia";
 * colloquia-plan T5) et fons communis eius.
 *
 * VISUS: greges status (album): "Needs you" (EXSPECTAT), "Working"
 * (LABORAT), "Done" (PERFECIT), "Idle" (OTIOSUM), "Closed" (CLAUSUM,
 * solum si campus clausa); intra gregem recentissima primum. Ordo:
 * signum coloris status, titulus, aetas; linea status infra. Claves:
 * j k, [ ], c (clausae), Enter aut ictus duplex (porta: saltus ad
 * sessionem). Manifestum apps/colloquia/organum.stml.
 *
 * FONS: cauda libri praeconii, deinde praeconium_legere_ab, plica
 * incrementalis (colloquia_status_*); tituli per sessionem in memoria
 * (transcriptum; nomen directorii non servatur); panelli tmux per V s
 * in SERVITORE cuiusque sessionis (vivi / clausi; sine servitore non
 * iudicatur). Pulsatur a lateribus visibilibus solum. */

#ifndef COLLOQUIA_VISUS_H
#define COLLOQUIA_VISUS_H

/* <aedilis corpus="lib/colloquia_visus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "organum_montatio.h"
#include "multiplexor.h"
#include "colloquia.h"

/* porta saltus (vicus: ut admonitiones T4c); VERUM si acta */
nomen b32 (*ColloquiiActio)(vacuum* ctx, constans Colloquium* c);

/* porta temporis: ms ab epocha */
nomen s64 (*ColloquiorumNunc)(vacuum* ctx);

nomen structura {
        ColloquiaStatus* status;
     constans character* via;
                    s64  positio;
   constans Multiplexor* multiplexor;   /* exemplar (binarium); socket
                                        * = servitor cuiusque sessionis;
                                        * NIHIL = sine tmux */
                    Xar* panelli;       /* per servitorem (privatum);
                                        * NIHIL = nondum lecti */
                    s64  panelli_ad;     /* ms lectionis ultimae */
                Piscina* panellorum;    /* per lectionem vacatur */
                    i64  signum_panellorum;  /* FNV idorum vivorum */
                    Xar* tituli;
                Piscina* piscina;       /* diuturna */
                Piscina* scheda;
} ColloquiorumFons;

b32
colloquia_visus_fontem_initiare (
      ColloquiorumFons* f,
               Piscina* piscina,
    constans character* via,
                   i32  octeti_initiales,
  constans Multiplexor* m);

/* pulsus: eventus novi; panelli si V s praeterierunt (nunc ms). VERUM
 * si mutatum (eventus novi, panelli mutati) */
b32
colloquia_visus_fontem_pulsare (
    ColloquiorumFons* f,
                 s64  nunc);

/* instantanea: colloquia cum titulis et panellis applicatis (Xar de
 * Colloquium in piscina data) */
Xar*
colloquia_visus_fontis (
    ColloquiorumFons* f,
             Piscina* piscina);

nomen structura {
             InsulaRamus  ramus;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
        ColloquiorumFons* fons;       /* ligamen (communis) */
          ColloquiiActio  agere;
                  vacuum* agere_ctx;
        ColloquiorumNunc  nunc;       /* post montationem; NIHIL =
                                       * sine aetate, fons sine
                                       * panellis */
                  vacuum* nunc_ctx;
                 Piscina* piscina;
     InternamentumChorda* intern;
                  vacuum* figurae_ctx; /* privatum */
} ColloquiorumMontatio;

/* montare in ramo <colloquia id> (manifestum infixum); superficies
 * latitudo x altitudo (0 = ordinaria); systemata NIHIL licet */
b32
colloquia_visus_montare (
    ColloquiorumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
        ColloquiorumFons* fons,
          ColloquiiActio  agere,
                  vacuum* agere_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx =
 * ColloquiorumMontatio* */
Componens*
colloquia_visus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* pulsus lateris visibilis: fons pulsatur; VERUM si mutatum */
b32
colloquia_visus_reficere (
    ColloquiorumMontatio* m);

#endif /* COLLOQUIA_VISUS_H */
