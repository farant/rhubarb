/* http_vocatio.h - vocatio HTTP(S) NON-OBSTRUENS super reactorem
 * (http-vocatio-spec II.4)
 *
 * Machina status vocationis unius: NOMINA (nomina.h) -> CONNEXIO
 * (tcp_incipere) -> PACTIO (tls_gradus, solum https) -> MITTENS ->
 * LEGENS (http_lector) -> FINITA | IRRITA | ABRUPTA. Reactor a VOCANTE
 * datur et ab eo poscitur (reactor_poll); vocatio fossas et timers suos
 * ibi inscribit. Nihil umquam obstat.
 *
 * Contractus:
 * - octeti corporis ad rivum SOLUM pro 2xx, dum adveniunt, numquam post
 *   finem (abrumpere intra rivum ceteros eiusdem capsae tacet);
 * - vocatio finitur cum lector finitum nuntiat (sine clausura
 *   exspectata - keep-alive), in clausura munda si corpus clausura
 *   terminatur, aut errore / tempore / abrumpere;
 * - finis SEMEL, numquam intra http_vocare; post finem nihil vocationis
 *   in reactore (fossae e reactore PRIUS, deinde clausae);
 * - tempora: connexionis (NOMINA + CONNEXIO + PACTIO simul, R8) et
 *   otii (sine octeto in MITTENS/LEGENS, renovatur in omni progressu);
 *   nullum tempus totius durationis;
 * - petitio serializata post missionem et in fine zephyratur (clavis
 *   API in capite esse potest, R2);
 * - piscinam vocationis finis NE destruat: codex post reditum finis
 *   adhuc currit - dimitte in pulsu proximo. */
#ifndef HTTP_VOCATIO_H
#define HTTP_VOCATIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "http.h"
#include "nomina.h"

nomen enumeratio {
    HTTP_VOCATIO_NOMINA = 0,
    HTTP_VOCATIO_CONNEXIO,
    HTTP_VOCATIO_PACTIO,        /* solum https */
    HTTP_VOCATIO_MITTENS,
    HTTP_VOCATIO_LEGENS,
    HTTP_VOCATIO_FINITA,
    HTTP_VOCATIO_IRRITA,
    HTTP_VOCATIO_ABRUPTA
} HttpVocatioStatus;

nomen structura HttpVocatio HttpVocatio;

/* semel per vocationem; numquam intra http_vocare */
nomen vacuum (*HttpVocatioFinis)(vacuum* datum, HttpVocatio* vocatio);

/* optiones NIHIL aut memset ZEPHYRUM = ordinariae */
nomen structura {
    i32 mora_connexionis_ms;  /* 0 = tempus petitionis, aliter XXX s */
    i32 mora_otii_ms;         /* 0 = LX s sine octeto */
    /* conatus gradati (RFC 8305 par. 5): post mora_gradus_ms sine
     * successu conatus proximus PARALLELE incipit; 0 = CCL ms */
    i32 mora_gradus_ms;
    /* addressus iam resoluti (probationes: sine DNS); numerus 0 =
     * hospes URL per nomina resolvitur */
    constans NominaAddressus* addressus;
                         i32  addressus_numerus;
} HttpVocatioOptiones;

/* vocatio incipit; optiones NIHIL = ordinariae. NIHIL solum si
 * argumenta NIHIL - ceteri defectus per finem */
HttpVocatio*
http_vocare (
                         Reactor* reactor,
                     HttpPetitio* petitio,
                       HttpRivus  rivus,
                          vacuum* rivi_datum,
                HttpVocatioFinis  finis,
                          vacuum* finis_datum,
    constans HttpVocatioOptiones* optiones,
                         Piscina* piscina);

HttpVocatioStatus
http_vocatio_status (
    constans HttpVocatio* vocatio);

b32
http_vocatio_finita (
    constans HttpVocatio* vocatio);

/* post finem: FINITA -> successus + responsum (status, capita, corpus
 * totum); aliter error + descriptio, responsum NIHIL (R7) */
HttpResultus
http_vocatio_resultus (
    constans HttpVocatio* vocatio);

/* idempotens, synchronus: finis (ABRUPTA) ANTE reditum vocatur */
vacuum
http_vocationem_abrumpere (
    HttpVocatio* vocatio);

#endif /* HTTP_VOCATIO_H */
