/* recursus.h - SCHEDULA NATURALIA (pensa K1; project-specs/pensa-spec.md
 * III): phrasis Anglica ("every other monday from oct 26 2026") ->
 * regula opaca -> dies. Purus: nec volumen nec horologium - vocans dies
 * dat. Parsator determinatus: phrasis ignota = vitium nominatum,
 * numquam coniectura. Corpus lib/recursus.corpus.stml = probationes et
 * documentatio; forma canonica iterum lecta eandem regulam dat. */

#ifndef RECURSUS_H
#define RECURSUS_H

/* <aedilis corpus="lib/recursus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "fasti.h"

/* opaca: interior crescere potest (uniones, exceptiones, finis) */
nomen structura Recursus Recursus;

/* phrasis -> regula in piscina; NIHIL et *vitium (Anglice; vitium NIHIL
 * licet) si non intellegitur */
Recursus*
recursus_legere (
       chorda  phrasis,
      Piscina* piscina,
       chorda* vitium);

/* dies in regula? (NIHIL aut dies invalidus: FALSUM) */
b32
recursus_continet (
    constans Recursus* r,
                 Dies  d);

/* dies [prima, ultima] (inclusive) ordine in exitus (Xar de Dies);
 * numerus additorum (fenestra inversa: 0) */
i32
recursus_dies (
    constans Recursus* r,
                 Dies  prima,
                 Dies  ultima,
                  Xar* exitus);

/* dies primus >= ab in *exitus; FALSUM si nullus intra annos VIII */
b32
recursus_proximus (
    constans Recursus* r,
                 Dies  ab,
                 Dies* exitus);

/* forma canonica Anglica ("Every other Monday from Mon 26 Oct 2026") */
chorda
recursus_scribere (
    constans Recursus* r,
              Piscina* piscina);

#endif /* RECURSUS_H */
