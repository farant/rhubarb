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
    /* nuntii progressus (missio longa); NIHIL = tacitus */
                  vacuum  (*observator)(vacuum* datum,
                                constans character* linea);
                       vacuum* observatoris_datum;
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


/* ==================================================
 * Identitas distributionis (T2): clavis RSA, petitio, certificata
 * ================================================== */

/* clavis RSA in octetis (sine typis BearSSL in capite: caput eius ANTE
 * latina.h staret). SECRETUM: primi, exponentes, coefficiens */
nomen structura {
    i32  bits;
     i8* modulus;
    i32  modulus_mensura;
     i8* exponens;              /* publicus (65537) */
    i32  exponens_mensura;
     i8* primus_p;
    i32  primus_p_mensura;
     i8* primus_q;
    i32  primus_q_mensura;
     i8* exponens_p;            /* d mod (p - 1) */
    i32  exponens_p_mensura;
     i8* exponens_q;            /* d mod (q - 1) */
    i32  exponens_q_mensura;
     i8* coefficiens;           /* q^-1 mod p */
    i32  coefficiens_mensura;
} NundinaeAppleClavisRsa;

nomen structura {
    chorda id;
    chorda numerus_serialis;
    chorda titulus;             /* displayName */
    chorda exspirat;            /* expirationDate */
    chorda contentum;           /* certificateContent (base64 DER) */
} NundinaeAppleCertificatum;

/* RSA MMXLVIII bits, e = 65537, fortuita systematis. FALSUM si fors aut
 * memoria deficit */
b32
nundinae_apple_clavem_rsa_generare (
    NundinaeAppleClavisRsa* k,
                   Piscina* p);

/* PKCS#1 PEM ("-----BEGIN RSA PRIVATE KEY-----", lineae LXIV) - forma
 * quam 'security import -f openssl -t priv' legit; vacua si deficit */
chorda
nundinae_apple_clavem_rsa_textum (
    constans NundinaeAppleClavisRsa* k,
                            Piscina* p);

/* omnes partes zephyrat et NIHIL ponit */
vacuum
nundinae_apple_clavem_rsa_delere (
    NundinaeAppleClavisRsa* k);

/* PKCS#10 DER: subiectum CN=<nomen_commune>, clavis publica RSA,
 * signatura sha256WithRSAEncryption; vacua si deficit */
chorda
nundinae_apple_petitionem_facere (
    constans NundinaeAppleClavisRsa* k,
                             chorda  nomen_commune,
                            Piscina* p);

/* responsum GET /v1/certificates -> Xar de NundinaeAppleCertificatum
 * (solum certificateType DISTRIBUTION) */
b32
nundinae_apple_certificata_legere (
    JsonValor* responsum,
          Xar* exitus,
      Piscina* p);

/* responsum GET /v1/bundleIds -> id cuius identifier EXACTE congruit
 * (filtrum Apple praefixum quoque reddit), aut vacua */
chorda
nundinae_apple_signum_invenire (
     JsonValor* responsum,
        chorda  identificator,
       Piscina* p);

/* SHA-1 octetorum ut XL litterae hex MAIUSCULAE (security find-identity
 * eas imprimit) */
chorda
nundinae_apple_digestum_sha1 (
     chorda  datum,
    Piscina* p);


/* ==================================================
 * Res rationis (T2; rete): quaere, deinde crea si deest. NUMQUAM delet.
 * creatum (NIHIL licet) = VERUM si hic creatum est.
 * ================================================== */

/* signum fasciculi (bundle ID) IOS */
b32
nundinae_apple_signum_fasciculi (
    NundinaeApple* n,
           chorda  identificator,
           chorda  titulus,
           chorda* id,
              b32* creatum,
           chorda* causa,
          Piscina* p);

/* certificata DISTRIBUTION turmae -> Xar de
 * NundinaeAppleCertificatum */
b32
nundinae_apple_certificata_distributionis (
    NundinaeApple* n,
              Xar* exitus,
           chorda* causa,
          Piscina* p);

/* certificatum DISTRIBUTION ex petitione PKCS#10 DER */
b32
nundinae_apple_certificatum_creare (
                NundinaeApple* n,
                       chorda  petitio_der,
    NundinaeAppleCertificatum* exitus,
                       chorda* causa,
                      Piscina* p);

/* diploma IOS_APP_STORE (ACTIVE) signi cum certificato; contentum =
 * profileContent (base64 .mobileprovision) */
b32
nundinae_apple_diploma (
    NundinaeApple* n,
           chorda  id_signi,
           chorda  id_certificati,
           chorda  titulus,
           chorda* contentum,
              b32* creatum,
           chorda* causa,
          Piscina* p);

/* applicatio per signum (solum quaeritur - API eam creare nequit);
 * absens -> FALSUM, causa Franum ad paginam mittit */
b32
nundinae_apple_applicatio (
    NundinaeApple* n,
           chorda  signum,
           chorda* id,
           chorda* causa,
          Piscina* p);

/* grex internus TestFlight applicationis per titulum */
b32
nundinae_apple_grex (
    NundinaeApple* n,
           chorda  id_applicationis,
           chorda  titulus,
           chorda* id,
              b32* creatum,
           chorda* causa,
          Piscina* p);

/* tester per email in grege (usor turmae pro grege interno) */
b32
nundinae_apple_tester (
    NundinaeApple* n,
           chorda  id_gregis,
           chorda  email,
              b32* creatum,
           chorda* causa,
          Piscina* p);


/* ==================================================
 * Distributio et missio (T4)
 * ================================================== */

nomen structura {
    chorda titulus;             /* nomen capitis HTTP */
    chorda valor;
} NundinaeAppleCaput;

/* operatio missionis Apple (DeliveryFileUploadOperation): methodus,
 * url, offset/longitudo in plagula, capita (NundinaeAppleCaput) */
nomen structura {
    chorda  methodus;
    chorda  url;
       s64  offset;
       s64  longitudo;
       Xar* capita;
} NundinaeAppleOperatio;

/* D9: viae obiectorum clausurae (aedilis --partes 'O') quarum basis
 * cryptographiam fert (tls, bearssl, aes, chacha, crypt...) -> nominata
 * (Xar de chorda). VERUM si ulla adest (applicatio "non exempta"). */
b32
nundinae_apple_cryptographia (
    constans chorda* obiecta,
                i32  numerus,
                Xar* nominata,
            Piscina* p);

/* D10 / Review Focus 2: numerus maximus inter builds (version) et
 * buildUploads (cfBundleVersion, etiam fractae) - non numerici
 * praetermittuntur; nullus -> 0. Uterque responsum NIHIL licet. */
i32
nundinae_apple_versio_maxima (
    JsonValor* aedificationes,
    JsonValor* missiones);

/* responsum creationis buildUploadFiles -> Xar de
 * NundinaeAppleOperatio */
b32
nundinae_apple_operationes_legere (
    JsonValor* responsum,
          Xar* operationes,
      Piscina* p);

/* MD5 ut XXXII litterae hex minusculae */
chorda
nundinae_apple_md5_hex (
    constans i8* datum,
            i64  mensura,
        Piscina* p);

/* aedificatio proxima: maximum (builds + buildUploads applicationis)
 * + I */
b32
nundinae_apple_aedificationem_proximam (
    NundinaeApple* n,
           chorda  id_applicationis,
              i32* proxima,
           chorda* causa,
          Piscina* p);

/* .ipa mittere (Build Uploads): buildUpload (app, versio, aedificatio,
 * IOS) -> buildUploadFile (com.apple.ipa) -> operationes ut datae ->
 * commissio cum MD5 -> status missionis (COMPLETE | FAILED) -> status
 * processus aedificationis (VALID | INVALID | FAILED). Ceilings: XXX
 * min per gradum. id_aedificationis = build Apple. */
b32
nundinae_apple_mittere (
       NundinaeApple* n,
              chorda  id_applicationis,
  constans character* via_archivi,
              chorda  versio,
                 i32  aedificatio,
              chorda* id_aedificationis,
              chorda* causa,
             Piscina* p);

#endif /* NUNDINAE_APPLE_H */
