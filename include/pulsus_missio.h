/* pulsus_missio.h - missio una notificationis inter pulsus et mittentem
 * (pulsus-plan T2/T4)
 *
 * Typi soli, sine corpore: pulsus (servus) missiones facit, mittens
 * (apns_mittens; probationes fictum) eas tradit et exitum semel
 * reddit. Hic seorsum ne mittens - et instrumentum apns super eum -
 * servum communionis totum (volumen, cives, hospitium) secum trahat.
 */

#ifndef PULSUS_MISSIO_H
#define PULSUS_MISSIO_H

#include "latina.h"
#include "chorda.h"

nomen enumeratio {
    PULSUS_TRADITUM = ZEPHYRUM,  /* Apple accepit (200) */
    PULSUS_SIGNUM_MORTUUM,       /* 410 / BadDeviceToken: inscriptio
                                  * deletur */
    PULSUS_DEFECTUS              /* mittens desiit (post iterationes) */
} PulsusExitus;

/* missio una: solum intra mittere legitur - mittens copiat quod
 * servat */
nomen structura {
    chorda signum;       /* tessera APNs apparatus (hex) */
    chorda ambitus;      /* "productio" | "sandbox" */
    chorda thema;        /* apns-topic */
    chorda corpus;       /* JSON (pulsus_corpus_facere) */
    chorda apparatus;    /* id apparatus destinatarii */
} PulsusMissio;

/* mittens copiam suam missionis reddit (pulsus scit quae finita);
 * ratio (APNs "reason" aut causa transportus) solum intra finem
 * valet */
nomen vacuum (*PulsusFinis)(
                  vacuum* ctx,
    constans PulsusMissio* missio,
            PulsusExitus  exitus,
                  chorda  ratio);

/* VERUM = accepta (finis postea semel, fortasse statim); FALSUM =
 * non accepta (finis numquam) */
nomen b32 (*PulsusMittere)(
                  vacuum* sui,
    constans PulsusMissio* missio,
             PulsusFinis  finis,
                  vacuum* ctx);

#endif /* PULSUS_MISSIO_H */
