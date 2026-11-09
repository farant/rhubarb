/* pulsus_forma.h - formae notificationum domus (pulsus-spec III.1;
 * pulsus-plan T1)
 *
 * Locus UNUS ubi eventus, inscriptio, corpus APNs et tactus JSON fiunt
 * et ex JSON redeunt: servus (pulsus) et clientes (communio_cliens,
 * pictor) hunc solum adhibent. Purum: nec rete nec horologium.
 *
 * EVENTUS (POST /communio/pulsus/eventus):
 *   {"clavis","thema","res":[id...],"mora":s,"unus","plures",
 *    "statim":b}
 * clavis/thema: I..CXXVIII signa [A-Za-z0-9._-]; res: C ad plurimum,
 * quaeque I..CCLVI signa sine signis moderantibus (servo opaca);
 * mora 0..MMMDC secundae (absens = CCC); unus/plures: formae textus,
 * I..CCLVI octeti, sine signis moderantibus, locatores {persona} et
 * {numerus} soli; statim absens = FALSUM.
 *
 * INSCRIPTIO (PUT /communio/pulsus/inscriptio):
 *   {"tessera":hex,"ambitus":"productio"|"sandbox","thema"}
 * tessera: II..CCLVI signa hex, numerus par.
 *
 * CORPUS (APNs, spec III.1):
 *   {"aps":{"alert":{"title","body"},"sound":"default","thread-id":
 *    clavis},"pulsus":{"clavis","res":[...],"a":civis[,"plures":true]}}
 * res a fine truncantur (plures VERUM) donec corpus <=
 * PULSUS_CORPUS_MAXIMUM octeti (limes Apple).
 */

#ifndef PULSUS_FORMA_H
#define PULSUS_FORMA_H

/* <aedilis corpus="lib/pulsus_forma.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

#define PULSUS_RES_PER_PETITIONEM C
#define PULSUS_ID_MAXIMUM         CCLVI
#define PULSUS_FORMA_MAXIMA       CCLVI
#define PULSUS_TITULUS_MAXIMUS    CXXVIII
#define PULSUS_TESSERA_MAXIMA     CCLVI
#define PULSUS_MORA_MAXIMA        MMMDC
#define PULSUS_MORA_ORDINARIA     CCC
#define PULSUS_CORPUS_MAXIMUM     ((i32)IV * (i32)M + (i32)XCVI)

/* chordae in piscina vocantis (aut in memoria JSON lecti); res = Xar
 * de chorda */
nomen structura {
    chorda  clavis;
    chorda  thema;
       Xar* res;
       i32  mora_s;
    chorda  unus;
    chorda  plures;
       b32  statim;
} PulsusEventus;

/* FALSUM et causa (campum nominat) si corpus non JSON aut lex C10
 * fracta */
b32
pulsus_eventum_legere (
           chorda  json,
          Piscina* piscina,
    PulsusEventus* exitus,
           chorda* causa);

/* eventus in JSON (cliens); nulla validatio - servus iudicat */
chorda
pulsus_eventum_scribere (
    constans PulsusEventus* eventus,
                   Piscina* piscina);

/* FALSUM et causa (campum nominat) si inscriptio mala */
b32
pulsus_inscriptionem_legere (
     chorda  json,
    Piscina* piscina,
     chorda* tessera,
     chorda* ambitus,
     chorda* thema,
     chorda* causa);

/* forma -> textus: {persona} et {numerus} (decimalis) substituuntur,
 * nihil aliud */
chorda
pulsus_textum_facere (
     chorda  forma,
     chorda  persona,
        i32  numerus,
    Piscina* piscina);

/* corpus APNs (supra); a = civis mittentis */
chorda
pulsus_corpus_facere (
          chorda  titulus,
          chorda  textus,
          chorda  clavis,
          chorda  a,
    constans Xar* res,
         Piscina* piscina);

/* tactus: obiectum "pulsus" corporis tradito */
nomen structura {
    chorda  clavis;
    chorda  a;
       Xar* res;       /* de chorda */
       b32  plures;
} PulsusTactus;

/* FALSUM si non JSON aut "pulsus" (cum clavi) deest */
b32
pulsus_tactum_legere (
          chorda  json,
         Piscina* piscina,
    PulsusTactus* exitus);

#endif /* PULSUS_FORMA_H */
