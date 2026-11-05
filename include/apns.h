/* apns.h - Apple Push Notification service, provisor per tesseram
 * (http2-plan T11; spec III.4)
 *
 * Gradus I: finis gradus - petitio una ad servum sandbox cum signo
 * apparatus ficto; Apple "BadDeviceToken" reddens nostram struem
 * HTTP/2 + TLS + tesseram probat. Servitium push (cauda, signa
 * apparatuum) proiectum sequens est.
 *
 * Clavis numquam hic legitur: vocans tesseram (tessera_apns_facere)
 * tradit; authorization numquam indicatur (hpack secreta). */
#ifndef APNS_H
#define APNS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

#define APNS_SIGNUM_NULLUM \
    "0000000000000000000000000000000000000000000000000000000000000000"

/* ~/.rhubarb/ios/apns.stml: <apns clavis_via= clavis_id= turma= thema=
 * hospes= [hospes_sandbox=] [portus=] [ancora=]/> - ancora et portus
 * probationibus (servus localis); clavis_via "~/" expanditur. Nullum
 * secretum in plagula. hospes = ambitus "productio", hospes_sandbox =
 * "sandbox" (vacuum = nullae missiones sandbox; pulsus-plan C7). */
nomen structura {
    chorda clavis_via;
    chorda clavis_id;
    chorda turma;
    chorda thema;          /* apns-topic: bundle ID */
    chorda hospes;
    chorda hospes_sandbox; /* optionale */
       i32 portus;         /* ordinarie CDXLIII */
    chorda ancora;         /* via ad CA DER; vacua = fiducia systematis */
} ApnsConfiguratio;

/* FALSUM si plagula abest, non STML, aut campus necessarius deest
 * (causa eum nominat) */
b32
apns_configurationem_legere (
     constans character* via,
       ApnsConfiguratio* c,
                 chorda* causa,
                Piscina* p);

nomen structura {
       b32  transita;    /* responsum HTTP/2 totum receptum */
       i32  status;      /* :status */
    chorda  ratio;       /* JSON "reason" (vacua si nulla) */
    chorda  apns_id;     /* caput apns-id */
       Xar* capita;      /* HpackCaput responsi */
    chorda  causa;       /* si non transita */
} ApnsResponsum;

/* POST /3/device/<signum> cum apns-topic, apns-push-type: alert,
 * authorization: bearer <tessera>, corpus {"aps":{"alert":"probatio"}}.
 * OBSTRUIT (reactor proprius, XXX s ad summum). VERUM si responsum aut
 * defectus in r positus. */
b32
apns_mittere (
    constans ApnsConfiguratio* c,
                       chorda  tessera,
           constans character* signum,
                ApnsResponsum* r,
                      Piscina* p);

nomen enumeratio {
    APNS_TRANSITUS = ZEPHYRUM,  /* tessera accepta (BadDeviceToken) */
    APNS_DEFECTUS,              /* transportus, tessera aut thema */
    APNS_ITERANDUM              /* signo fortuito iterum (spec III.4) */
} ApnsIudicium;

/* regula spec III.4: Invalid/Expired/MissingProviderToken, Topic-
 * Disallowed, BadTopic -> DEFECTUS; BadDeviceToken aut 200 ->
 * TRANSITUS; aliud: post signum nullum ITERANDUM, aliter TRANSITUS */
ApnsIudicium
apns_iudicare (
    constans ApnsResponsum* r,
                       b32  signum_nullum);

/* LXIV signa hex minuscula (et NUL) ex semine - signum apparatus
 * bene formatum, Apple ignotum */
vacuum
apns_signum_fortuitum (
    character* exitus,
          s64  semen);

#endif /* APNS_H */
