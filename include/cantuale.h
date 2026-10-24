/* cantuale.h - Liber cantilenarum nominatarum: titulus, genera, textus
 *
 * Cantuale (liber cantuum medii aevi) = collectio cantilenarum (cantus.h)
 * nominatarum et per GENERA (occasiones, affectus) signatarum, ut hospes
 * (vicus, auditio) 'aliquam cantilenam finis' sorte eligat.
 *
 * FORMA (STML; liber ordinarius cantuale.stml, in binarium infixus -
 * cantuale_infixum.h):
 *
 *   <cantuale>
 *     <cantilena titulus="arpeggium" genera="finis laetus">
 *       <textus!>t180 o6 l16 e g# b >e8</textus>
 *     </cantilena>
 *     <cantilena titulus="pulsatio" genera="permissio" intactus>
 *       <textus!>@punctum t160 o6 l16 c r c r8 <g4</textus>
 *     </cantilena>
 *   </cantuale>
 *
 *   textus    forma CRUDA ('<textus!>') - MML '<' et '>' fert
 *   titulus   [a-z0-9_]+, unicus in libro
 *   genera    tituli spatio separati, [a-z0-9_]+ (fortasse nulli: tum
 *             per titulum solum invenitur). Occasiones: permissio
 *             (Claudius te exspectat), finis (opus perfectum), vitium
 *             (aliquid fractum); affectus: laetus, gravis ...
 *   intactus  tenere FALSUM (cantus.h): color praeparati intactus
 *             (campana, ictus); sine eo notae per longitudinem tenentur
 *
 * TOTUS LIBER IN LECTIONE IUDICATUR: textus quisque per cantus_legere,
 * '@titulus' quisque per cantus_praeparatum_sonitus, notae <=
 * CANTUALE_NOTAE_MAXIMAE (hospes mixturam fixam habet: cantus sedem per
 * notam capit). Cantilena fracta = liber fractus, vitium cum titulo et
 * offset - numquam notificatio quae demum sonans deficit.
 *
 * Purus: nihil sonat. Sors a vocante (probationes deterministicae).
 *
 * USUS:
 *   Cantuale       liber;
 *   CantualeVitium v;
 *   constans Cantio* c = NIHIL;
 *   si (!cantuale_ordinarium(piscina, &liber, &v)) ... v.titulus, v.causa
 *   c = cantuale_eligere(&liber, chorda_ex_literis("finis", p), &sors, c);
 *   forma.tenere = !c->intactus;
 *   cantus_iussa_parare(&c->cantilena, &forma, rector, iussa, &vitium);
 */
/* <aedilis corpus="lib/cantuale.c"/> */
#ifndef CANTUALE_H
#define CANTUALE_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "sors.h"
#include "cantus.h"

/* limes notarum per cantilenam (sedes vocum hospitis) */
#define CANTUALE_NOTAE_MAXIMAE XXXII

nomen structura {
       chorda  titulus;
       chorda  textus;          /* MML, ut in libro (offset vitii hic) */
       chorda* genera;
          i32  genera_numerus;
          b32  intactus;
    Cantilena  cantilena;       /* iam lecta - notae in textum spectant */
} Cantio;

nomen structura {
    Cantio* cantiones;   /* ordine libri */
       i32  numerus;
} Cantuale;

nomen structura {
                 chorda titulus;  /* cantio vitiosa; vacua = liber ipse
                                   * (STML fractum, radix, elementum) */
                    i32  offset;   /* in textu cantionis (0 aliter) */
     constans character* causa;    /* Latina, statica */
} CantualeVitium;

/* librum STML legit et TOTUM iudicat. FALSUM: vitium impletum, liber
 * vacuus. Chordae libri (tituli, textus, genera) in piscina
 * internatae - vivunt dum piscina */
b32
cantuale_legere (
            chorda  stml,
           Piscina* piscina,
          Cantuale* liber,
    CantualeVitium* vitium);

/* cantilena UNA extra librum (praeco cantus, tintinnabulum_canere):
 * iudicia libri eadem - textus per cantus_legere, '@titulus' notus,
 * 1..CANTUALE_NOTAE_MAXIMAE notae; cantuale_legere eam per cantilenam
 * vocat (regula una). titulus vacuus, genera nulla; textus NON copiatur
 * (notae in eum spectant - vivat dum cantio). FALSUM: vitium (titulus
 * vacuus, offset in textu) */
b32
cantionem_legere (
            chorda  textus,
               b32  intactus,
           Piscina* piscina,
            Cantio* cantio,
    CantualeVitium* vitium);

/* liber ordinarius (cantuale.stml infixus) - FALSUM solum si liber
 * commissus fractus est (probatio id custodit) aut memoria deest */
b32
cantuale_ordinarium (
           Piscina* piscina,
          Cantuale* liber,
    CantualeVitium* vitium);

/* NIHIL si ignota */
constans Cantio*
cantuale_invenire (
    constans Cantuale* liber,
               chorda  titulus);

/* sorte aequa inter cantiones generis; 'vitanda' (electa priore,
 * NIHIL licet) omittitur si alia exstat - notificationes duae non
 * eandem canunt. NIHIL si nulla cantio generis */
constans Cantio*
cantuale_eligere (
    constans Cantuale* liber,
               chorda  genus,
                 Sors* sors,
      constans Cantio* vitanda);

b32
cantio_generis_est (
    constans Cantio* cantio,
             chorda  genus);

#endif /* CANTUALE_H */
