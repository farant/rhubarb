#ifndef NUNDINAE_APPLE_H
#define NUNDINAE_APPLE_H

/* nundinae_apple.h - cliens App Store Connect API (testa-ios C2, spec
 * project-specs/testa-ios-divulgatio-spec.md II.1). Familia 'nundinae'
 * (Latine "mercatus"): bibliothecae tabernarum applicationum - haec
 * Apple; Amazon / Google sequi possunt (Franus 2026-10-24). Bibliotheca
 * GENERALIS (D12): tessera, petitiones JSON:API, paginae, errores,
 * auxilia rerum - divulgatio TestFlight consumptor primus.
 *
 * SECRETUM: clavis API (.p8, PKCS#8 P-256) in memoria solum ad
 * signandum; numquam scribitur, imprimitur aut per lectiones legitur;
 * modus plagulae 0600 aliter recusatio. Causae errorum nec tesseram nec
 * capita continent - methodus, via, status, error Apple solum. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "http.h"

#define NUNDINAE_APPLE_DURATIO        MCC   /* s: vita tesserae (Apple
                                             * <= XX min) */
#define NUNDINAE_APPLE_RENOVATIO      CXX   /* s: renovare ante finem */
#define NUNDINAE_APPLE_CAUSA_CORPUS   CC    /* octeti corporis non-JSON
                                             * in causa */
#define NUNDINAE_APPLE_CONATUS        III   /* GET: conatus summi */

/* codex Apple: pactum (Program License Agreement aut pacta in App
 * Store Connect > Business) non acceptum - OMNIS petitio refutatur */
#define NUNDINAE_APPLE_CODEX_PACTUM \
    "FORBIDDEN.REQUIRED_AGREEMENTS_MISSING_OR_EXPIRED"

/* ~/.rhubarb/ios/nundinae_apple.stml (non secreta) */
nomen structura {
    chorda clavis_id;     /* "Key ID" clavis API */
    chorda emissor;       /* "Issuer ID" */
    chorda turma;         /* Team ID, e.g. Q5WC9FQ3J2 */
    chorda via_clavis;    /* ~/.rhubarb/ios/AuthKey_<id>.p8 */
} NundinaeAppleConfiguratio;

/* clavis = PKCS#8 DER (SECRETUM); radix_url sine '/' finali */
nomen structura {
    NundinaeAppleConfiguratio  configuratio;
                           i8* clavis;
                          i32  clavis_mensura;
                       chorda  tessera;
                          s64  exspirat_s;
           constans character* radix_url;
                      Piscina* piscina;
} NundinaeApple;

nomen structura {
           b32  successus;      /* 2xx */
           i32  status;         /* 0 = transportus fractus */
     JsonValor* corpus;         /* NIHIL si non JSON aut vacuum */
        chorda  causa;          /* si !successus */
        chorda  codex;          /* errors[0].code Apple, aut vacua */
} NundinaeAppleResponsum;

nomen structura {
    chorda id;
    chorda signum_fasciculi;    /* bundleId */
    chorda titulus;
} NundinaeAppleApplicatio;


/* ==================================================
 * Partes PURAE (probatae in hospite)
 * ================================================== */

/* configuratio ex STML (radix <nundinae_apple clavis_id= emissor=
 * turma= via_clavis=/>); '~/' in via_clavis = $HOME. FALSUM + causa
 * attributum absens nominans */
b32
nundinae_apple_configurationem_legere (
           constans character* via,
    NundinaeAppleConfiguratio* c,
                       chorda* causa,
                      Piscina* p);

/* JWT ES256: caput {"alg":"ES256","kid":<id>,"typ":"JWT"}, claims
 * {"iss":<emissor>,"iat":nunc,"exp":nunc+duratio,"aud":
 * "appstoreconnect-v1"}, base64url sine '=', signatura r || s (LXIV
 * octeti). Clavis PKCS#8 DER P-256; vacua si clavis mala */
chorda
nundinae_apple_tesseram_facere (
    constans i8* clavis_der,
            i32  mensura,
         chorda  clavis_id,
         chorda  emissor,
            s64  nunc_s,
            i32  duratio_s,
        Piscina* p);

/* VERUM si tessera renovanda (restant <= NUNDINAE_APPLE_RENOVATIO s) */
b32
nundinae_apple_renovandum (
    s64 exspirat_s,
    s64 nunc_s);

/* GET solum, et solum 429, 500-504 aut transportus fractus: scripturae
 * numquam caece iterantur */
b32
nundinae_apple_iterandum (
    HttpMethodus methodus,
             i32 status,
             b32 rete_fractum);

/* causa ex responso: errors[0] JSON:API "status code: title - detail";
 * corpus non-JSON -> "status: " + octeti primi (<= CAUSA_CORPUS) */
chorda
nundinae_apple_causa_ex (
        i32  status,
     chorda  corpus,
    Piscina* p);

/* errors[0].code responsi JSON:API, aut vacua (corpus non-JSON aut
 * sine errors) */
chorda
nundinae_apple_codex_ex (
     chorda  corpus,
    Piscina* p);

/* "METHODUS /via: causa" - nulla capita, nulla tessera */
chorda
nundinae_apple_causam_componere (
    HttpMethodus  methodus,
          chorda  via,
          chorda  causa,
         Piscina* p);

/* links.next responsi JSON:API aut vacua */
chorda
nundinae_apple_proxima (
    JsonValor* responsum);


/* ==================================================
 * Cliens (rete; clavis secreta)
 * ================================================== */

/* clavem legit (modus 0600 aliter FALSUM "0600"; lectio cruda, non
 * lectiones), PEM -> DER in memoria */
b32
nundinae_apple_aperire (
                         NundinaeApple* n,
    constans NundinaeAppleConfiguratio* c,
                                chorda* causa,
                               Piscina* p);

/* clavem in memoria zephyrat */
vacuum
nundinae_apple_claudere (
    NundinaeApple* n);

/* petitio una (via "/v1/apps?..." aut URL plena ex links.next);
 * corpus_json vacuum = sine corpore. Tessera renovatur si opus;
 * GET iteratur per nundinae_apple_iterandum (mora I, IV, XVI s,
 * Retry-After honoratur <= LX s) */
NundinaeAppleResponsum
nundinae_apple_vocare (
         NundinaeApple* n,
          HttpMethodus  methodus,
    constans character* via,
                chorda  corpus_json,
               Piscina* p);

/* status rationis (Franus 2026-10-24: "control plane"): petitio
 * levissima (GET /v1/apps?limit=1). VERUM = ratio bona; FALSUM +
 * responsum (codex NUNDINAE_APPLE_CODEX_PACTUM = pactum deest) */
b32
nundinae_apple_statum_probare (
             NundinaeApple* n,
    NundinaeAppleResponsum* r,
                   Piscina* p);

/* applicationes turmae (omnes paginae) -> Xar de
 * NundinaeAppleApplicatio */
b32
nundinae_apple_applicationes (
    NundinaeApple* n,
              Xar* exitus,
           chorda* causa,
          Piscina* p);

#endif /* NUNDINAE_APPLE_H */
