/* http2_nexus.h - connexio HTTP/2 diuturna super reactorem
 * (http2-spec III.4; http2-plan T8)
 *
 * Glutinum: NOMINA -> CONNEXIO -> PACTIO (TLS cum ALPN "h2"
 * NECESSARIO - aliud electum = error, nullus descensus ad HTTP/1.1
 * intra hoc obiectum) -> VIVUS -> CLAUSUS. Octeti inter tls_* et
 * connexionem sine IO (http2.h) pulsantur; petitiones multae in
 * connexione una (APNs connexionem apertam servari petit).
 *
 * Contractus:
 * - finis petitionis SEMEL, numquam intra http2_nexus_petere (si
 *   connexio iam clausa: per timer 0);
 * - status: semel VERUM cum vivus, semel FALSUM cum perit aut GOAWAY
 *   accipit (petitiones pendentes tamen perficiuntur si possunt);
 * - tempora: connexionis (NOMINA + CONNEXIO + PACTIO) et otii (nullus
 *   octetus dum petitiones pendent) - modus http_vocatio. */
#ifndef HTTP2_NEXUS_H
#define HTTP2_NEXUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "reactor.h"
#include "nomina.h"
#include "hpack.h"

/* responsum (chordae et capita in piscina petitionis) */
nomen structura {
       b32  successus;
       i32  status;          /* :status, 0 si nullum */
       Xar* capita;          /* HpackCaput (sine :status), appendices
                             * in fine */
    chorda corpus;
    chorda causa;           /* defectus: quid accidit */
       b32 retentabilis;    /* servus petitionem certe non tractavit
                             * (GOAWAY, REFUSED_STREAM) */
} Http2Responsum;

nomen vacuum (*Http2NexusFinis)(vacuum* ctx,
    constans Http2Responsum* r);

nomen vacuum (*Http2NexusStatus)(vacuum* ctx, b32 vivus, chorda causa);

/* optiones (NIHIL aut memset 0 = ordinariae): mora_connexionis_ms XXX s,
 * mora_otii_ms LX s; addressus iam resoluti (sine DNS); tabula_via:
 * PROBATIONES SOLUM - octeti decrypti in <via>.servus.bin et
 * <via>.cliens.bin (http2-plan C5) */
nomen structura {
                         i32  mora_connexionis_ms;
                         i32  mora_otii_ms;
          constans character* tls_ancora_via;
    constans NominaAddressus* addressus;
                         i32  addressus_numerus;
          constans character* tabula_via;
} Http2NexusOptiones;

nomen structura Http2Nexus Http2Nexus;

/* connexio incipit (nihil obstat); status vocatur cum vivus aut perit.
 * NIHIL solum si argumenta mala aut memoria deest. */
Http2Nexus*
http2_nexum_aperire (
                        Reactor* reactor,
             constans character* hospes,
                            i32  portus,
    constans Http2NexusOptiones* optiones,
               Http2NexusStatus  status,
                         vacuum* ctx,
                        Piscina* piscina);

/* petitio: methodus ("GET"), via ("/x?y"), capita addita, corpus
 * (vacuum licet). Ante pactionem petitiones exspectant. FALSUM solum
 * si argumenta mala; ceteri defectus per finem. */
b32
http2_nexus_petere (
               Http2Nexus* x,
       constans character* methodus,
       constans character* via,
      constans HpackCaput* capita,
                      i32  numerus,
                   chorda  corpus,
          Http2NexusFinis  finis,
                   vacuum* ctx,
                  Piscina* piscina);

/* GOAWAY NO_ERROR (si vivus), connexio clausa; petitiones pendentes
 * finiuntur cum defectu ANTE reditum */
vacuum
http2_nexum_claudere (
    Http2Nexus* x);

#endif /* HTTP2_NEXUS_H */
