/* tools/fabrica_litterae.h - INTERNA bin/fabrica portae (plan
 * portae-declaratae T3): litterae chordarum fontis C per lexatorem
 * silvae amalgamatae. Unitas translationis SEPARATA: silva/amalgama/
 * silva.h enumerationes STML suas definit quae stml.h domus (per
 * portae.h) iterum definit - una unitas utrumque capere nequit. */

/* <aedilis corpus="tools/fabrica_litterae.c"/> */

#ifndef FABRICA_LITTERAE_H
#define FABRICA_LITTERAE_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* contenta litterarum chordarum (sine '"', effugia cruda) in fructus
 * (Xar de chorda, in piscina copiata); commenta absunt, macrones NON
 * expanduntur. FALSUM = lexatio aut memoria fracta. */
b32
suturae_litteras_colligere (
               chorda  textus,
              Piscina* piscina,
                  Xar* fructus);

#endif /* FABRICA_LITTERAE_H */
