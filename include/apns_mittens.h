/* apns_mittens.h - mittens APNs non obstruens super reactorem
 * (pulsus-spec III.2; pulsus-plan T4, C5-C7)
 *
 * PulsusMittere pro servo: missio copiatur (piscina propria per
 * missionem, reddita post finem), petitio POST /3/device/<signum> in
 * connexione HTTP/2 ambitus sui (productio -> hospes, sandbox ->
 * hospes_sandbox), connexio una per ambitum, pigre aperta, servata,
 * post mortem renata. Finis SEMEL per missionem:
 *
 *   200                                   -> PULSUS_TRADITUM
 *   410, aut 400 BadDeviceToken           -> PULSUS_SIGNUM_MORTUUM
 *   403 Expired/InvalidProviderToken      -> tessera renovatur SEMEL,
 *                                            statim iteratur
 *   429, 5xx, transportus fractus,
 *   retentabilis                          -> iteratur post morae
 *                                            (I s, V s, XXX s), deinde
 *                                            PULSUS_DEFECTUS
 *   aliud                                 -> PULSUS_DEFECTUS
 *
 * Ratio finis: "reason" Apple (verbum nudum, e.g. "BadDeviceToken");
 * sine eo "status N"; defectus localis aut transportus semper "apns: "
 * praefixum ("apns: transportus - ...", "apns: mittens clauditur").
 *
 * Capita (spec III.1): apns-push-type alert, apns-topic <thema>,
 * apns-priority 10, apns-expiration nunc + dies, authorization bearer
 * <tessera> (numquam indicatum). Tessera provisoris renovatur cum
 * aetas >= XL minuta (C6); signatur in mittente ex clave copiata.
 *
 * Secreta: clavis in memoria propria copiatur et in claudere
 * zephyratur; tessera numquam scribitur; signa in nuntiis VIII signis
 * praefixis solum.
 *
 * Ordo: claudere ANTE eos quorum fines vocat (pulsus_destruere) et
 * ante reactorem.
 */

#ifndef APNS_MITTENS_H
#define APNS_MITTENS_H

/* <aedilis corpus="lib/apns_mittens.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "reactor.h"
#include "hpack.h"
#include "http2_nexus.h"
#include "apns.h"
#include "pulsus_missio.h"

/* morae iterationum ordinariae (C5) et maximus earum numerus */
#define APNS_MORAE_MAXIMAE VIII

/* aetas tessera provisoris ante renovationem (C6) */
#define APNS_TESSERA_AETAS_MS ((s64)XL * LX * M)

nomen structura ApnsMittens ApnsMittens;

/* vectura (sutura probationum): pro http2_nexus_petere in connexione
 * ambitus; contractus idem (finis semel, numquam intra petere) */
nomen b32 (*ApnsVectura)(
                  vacuum* sui,
                  chorda  ambitus,
     constans character* via,
    constans HpackCaput* capita,
                     i32  numerus,
                  chorda  corpus,
         Http2NexusFinis  finis,
                  vacuum* ctx,
                 Piscina* piscina);

/* clavis (PKCS#8 DER P-256) copiatur; tessera prima statim signatur -
 * NIHIL si clavis mala, clavis_id/turma non tuta, aut argumenta mala.
 * nunc_ms NIHIL = horologium verum (aetas tesserae, apns-expiration).
 * Connexiones nondum apertae. */
ApnsMittens*
apns_mittentem_aperire (
                    Reactor* reactor,
    constans ApnsConfiguratio* configuratio,
                constans i8* clavis_der,
                        i32  mensura,
                        s64  (*nunc_ms)(vacuum),
                    Piscina* piscina);

/* PulsusMittere. FALSUM (finis numquam) si ambitus ignotus aut
 * sandbox sine hospes_sandbox, signum vacuum aut non hex, aut mittens
 * clauditur. */
b32
apns_mittens_mittere (
                   vacuum* sui,
    constans PulsusMissio* missio,
              PulsusFinis  finis,
                   vacuum* ctx);

/* morae iterationum (ms), n <= APNS_MORAE_MAXIMAE; n 0 = nulla
 * iteratio (instrumentum apns). Ordinariae: M, V * M, XXX * M. */
vacuum
apns_mittentis_moras_ponere (
      ApnsMittens* mittens,
     constans i32* moras_ms,
              i32  n);

/* PROBATIONES: vectura pro connexionibus veris (NIHIL = verae) */
vacuum
apns_mittentis_vecturam_ponere (
    ApnsMittens* mittens,
    ApnsVectura  petere,
         vacuum* sui);

/* missiones acceptae nondum finitae */
i32
apns_mittentis_pendentes (
    constans ApnsMittens* mittens);

/* connexiones clauduntur; missiones pendentes PULSUS_DEFECTUS ANTE
 * reditum; clavis et tessera zephyrantur */
vacuum
apns_mittentem_claudere (
    ApnsMittens* mittens);

#endif /* APNS_MITTENS_H */
