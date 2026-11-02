/* pulsus.h - notificationes domus in servo communionis (project-specs/
 * pulsus-spec.md III.1; pulsus-plan T2)
 *
 * Facultas domus, non pictoris: apparatus tesseram APNs inscribit;
 * applicatio eventum sub clave nuntiat; servus per (personam
 * mittentis, spatium, clavem) quietem exspectat - eventus quisque
 * horam renovat, nullus finis maximus - deinde notificationem UNAM
 * reliquis spatii mittit (personae mittentis numquam). statim = nunc.
 *
 * Tabulae in volumine servi (iuxta communio_*):
 *   pulsus_inscriptiones (apparatus, thema) -> tessera, ambitus,
 *                         creatus_ms, usus_ultimus_ms
 *   pulsus_pendentes     (civis, spatium, clavis) -> thema, res (JSON),
 *                         omissa, unus, plures, primus_ms, ultimus_ms,
 *                         mora_ms
 * Greges pendentes in volumine servantur: initium novum (dimissio)
 * eos resumit (pulsus_proximum_ms), nihil amittitur.
 *
 * Tempus TRAHITUR (plan C4): pulsus_maturos_mittere omnes greges
 * maturos mittit; hora reactoris (T3) eam solum vocat. Horologium
 * (nunc_ms) et mittens injiciuntur.
 *
 * Titulus notificationis = segmentum ultimum thematis
 * ("earth.golden.pictor" -> "pictor"). Signa in nuntiis stderr solum
 * VIII signis praefixis.
 */

#ifndef PULSUS_H
#define PULSUS_H

/* <aedilis corpus="lib/pulsus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "cives.h"
#include "communio_servus.h"
#include "pulsus_forma.h"

/* res per gregem (ultra: omissae, numeratae) */
#define PULSUS_RES_PER_GREGEM M

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

/* mittens copiam suam missionis reddit (pulsus scit quae finita) */
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

/* ordo inscriptionis (chordae in piscina vocantis; tessera VIII
 * signis praefixis) */
nomen structura {
    chorda apparatus;
    chorda thema;
    chorda tessera;
    chorda ambitus;
       s64 creatus_ms;
       s64 usus_ultimus_ms;
} PulsusInscriptio;

nomen structura Pulsus Pulsus;

/* tabulas creat si desunt; nunc_ms NIHIL = horologium verum. NIHIL si
 * argumenta mala aut schema deficit. */
Pulsus*
pulsus_creare (
           Piscina* piscina,
           Volumen* volumen,
    CommunioServus* servus,
             Cives* cives,
               s64  (*nunc_ms)(vacuum));

/* mittens (NIHIL = nihil mittitur: "pulsus: apns absens - non
 * mittitur" in stderr, greges tamen consumuntur) */
vacuum
pulsus_mittentem_ponere (
           Pulsus* pulsus,
    PulsusMittere  mittere,
           vacuum* sui);

/* inscriptio (apparatus, thema) ponitur aut substituitur; FALSUM si
 * argumenta vacua aut scriptura deficit */
b32
pulsus_inscribere (
     Pulsus* pulsus,
     chorda  apparatus,
     chorda  tessera,
     chorda  ambitus,
     chorda  thema);

/* eventus apparatus (mittens = persona et spatium eius). FALSUM si
 * apparatus ignotus aut revocatus. Res nullae sine statim: nihil.
 * statim: grex (si quis, cum rebus novis) nunc mittitur. */
b32
pulsus_eventum_addere (
                    Pulsus* pulsus,
                    chorda  apparatus,
    constans PulsusEventus* eventus);

/* greges maturi (ultimus_ms + mora_ms <= nunc) mittuntur et
 * delentur; numerus missorum */
i32
pulsus_maturos_mittere (
    Pulsus* pulsus);

/* hora proxima maturitatis (ms), -1 si nullus grex */
s64
pulsus_proximum_ms (
    Pulsus* pulsus);

/* Xar de PulsusInscriptio (apparatus revocati non purgati quoque) */
Xar*
pulsus_inscriptiones_enumerare (
     Pulsus* pulsus,
    Piscina* piscina);

/* notificatio probationis ad omnes apparatus (non revocatos) personae;
 * FALSUM si persona ignota */
b32
pulsus_probare (
     Pulsus* pulsus,
     chorda  civis);

/* arenam propriam reddit; volumen manet */
vacuum
pulsus_destruere (
    Pulsus* pulsus);

#endif /* PULSUS_H */
