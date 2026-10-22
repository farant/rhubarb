/* horreum.h - cliens S3-compatibilis, NUCLEUS PURUS (horreum-spec II)
 *
 * Provisores (data, non rami codicis), opera (descriptio petitionis
 * validata: capacitas deest -> HORREUM_INCAPAX, nihil aedificatur),
 * signatio opus -> HttpPetitio, lectio responsorum in facta et errores
 * typatos, politica iterationis. Nihil I/O nisi horreum_ex_tabella
 * (lectio CRUDA clavis, horreum-plan P5). Executores: horreum_cliens.h
 * (obstans), horreum_vocatio.h (reactor).
 *
 * Secretum in capsa fixa intra Horreum; horreum_claudere zephyrat.
 * Nullus error, nullum caput, nulla URL secretum continet (praesignata
 * signaturam continet, non secretum).
 *
 *   Horreum* h = horreum_ex_tabella(via_tabellae, &e, p);
 *   HorreumOpus o;
 *   si (horreum_opus_ponere(h, situla, clavis, corpus, NIHIL, &o,
 *           &e, p))
 *       ... horreum_exsequi(h, &o, ...)  (horreum_cliens.h) */
#ifndef HORREUM_H
#define HORREUM_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "fasti.h"
#include "http.h"
#include "signatura_aws.h"

/* <aedilis corpus="lib/horreum.c"/> */

#define HORREUM_CAP_ACL             0x01
#define HORREUM_CAP_SOLUM_SI_ABEST  0x02
#define HORREUM_CAP_SOLUM_SI_ETAG   0x04
#define HORREUM_CAP_RETENTIO        0x08   /* reservatum v1 */
#define HORREUM_CAP_CHECKSUM_CRC32  0x10
#define HORREUM_CAP_CHECKSUM_SHA256 0x20
#define HORREUM_CAP_DELERE_PLURA    0x40

#define HORREUM_MIB                 (MXXIV * MXXIV)
#define HORREUM_PARS_ORDINARIA      (XVI * HORREUM_MIB)
#define HORREUM_PARS_MINIMA         (V * HORREUM_MIB)
#define HORREUM_PARTES_MAXIMAE      (X * M)
#define HORREUM_FRUSTUM_LECTIONIS   (VIII * HORREUM_MIB)   /* P1 */
/* DeleteObjects: claves plurimae per petitionem */
#define HORREUM_CLAVES_PLURIMAE     M
#define HORREUM_CAPSA_CLAVIS        CCLVI

nomen enumeratio {
    HORREUM_STILUS_HOSPES = 0,    /* situla.hospes/clavis */
    HORREUM_STILUS_VIA            /* hospes/situla/clavis */
} HorreumStilus;

nomen enumeratio {
    HORREUM_ACL_NULLA = 0,        /* non mittitur */
    HORREUM_ACL_PRIVATA,
    HORREUM_ACL_PUBLICA_LECTIO
} HorreumAcl;

nomen enumeratio {
    HORREUM_CHECKSUM_NULLUM = 0,
    HORREUM_CHECKSUM_CRC32,
    HORREUM_CHECKSUM_SHA256
} HorreumChecksum;

nomen enumeratio {
    HORREUM_BENE = 0,
    HORREUM_ABEST,         /* 404 NoSuchKey/NoSuchBucket/NoSuchUpload */
    HORREUM_CONDITIO,      /* 412 (aut 304): condicio non tenuit */
    HORREUM_VETITUM,       /* 403 AccessDenied, SignatureDoesNotMatch */
    HORREUM_INCAPAX,       /* provisor caret - ante missionem */
    HORREUM_TRANSITORIUM,  /* 5xx, SlowDown, rete, tempus, horologium */
    HORREUM_MALUM          /* 400 ceterae, argumenta mala */
} HorreumErrorGenus;

/* provisor: data. probatae = capacitates vivae probatae (horreum
 * probare); in fonte "probatum YYYY-MM-DD" aut "creditum URL" */
nomen structura {
    constans character* titulus;
    constans character* hospes_forma;  /* {regio}, {rationarium} */
         HorreumStilus  stilus;
    constans character* regio;         /* NIHIL = debet dari */
                   i32  capacitates;
                   i32  probatae;
                   b32  http_licet;    /* solum generic */
} HorreumProvisor;

nomen structura {
    constans character* provisor;      /* "digitalocean", "r2", "b2",
                                        * "generic" */
                chorda terminus;      /* "https://hospes[:portus]";
                                        * vacuus = ex forma */
                chorda regio;
                chorda rationarium;   /* R2 account id */
                chorda situla;        /* ordinaria */
                chorda clavis_id;
                chorda secretum;      /* copiatur; vocans zephyrat */
} HorreumConfiguratio;

nomen structura Horreum Horreum;

nomen structura {
    chorda titulus;          /* sine "x-amz-meta-", minusculus */
    chorda valor;
} HorreumMeta;

nomen structura {
         chorda  clavis;
            i64  mensura;
         chorda  etag;                 /* sine virgulis duplicibus */
    MomentumSec  mutatum;              /* 0 = ignotum */
         chorda  genus_contenti;
         chorda  versio;
         chorda  checksum_crc32;       /* base64 ut missum */
         chorda  checksum_sha256;
    HorreumMeta* meta;
            i32  meta_numerus;
            b32  iam_nostrum;          /* II.5: 412 post iterationem */
} HorreumObjectum;

nomen structura {
    HorreumErrorGenus genus;
                  i32 status;         /* HTTP; 0 = ante missionem */
               chorda codex;          /* "NoSuchKey" */
               chorda nuntius;        /* numquam secretum */
               chorda petitio_id;     /* x-amz-request-id, RequestId,
                                        * cf-ray */
                  s64 deviatio_s;     /* RequestTimeTooSkewed: servus -
                                        * nos */
} HorreumError;

/* meta vocantis per opus (ultra: MALUM nominatum, numquam caput
 * tacite omissum). Titulus HORREUM_META_SIGILLUM horrei est (P10:
 * x-amz-meta-horreum-sha256 = SHA-256 corporis in ponere condicionali)
 * - vocanti vetitus */
#define HORREUM_META_MAXIMA    XVI
#define HORREUM_META_SIGILLUM  "horreum-sha256"

nomen structura {
                  chorda  genus_contenti;
                  chorda  cache_control;
    constans HorreumMeta* meta;
                     i32  meta_numerus;
                     b32  solum_si_abest;      /* If-None-Match: * */
                  chorda  solum_si_etag;       /* If-Match */
              HorreumAcl  acl;
         HorreumChecksum  checksum;
                     b32  periculum_accipere;  /* abest non probatum */
} HorreumPonendi;

nomen structura {
            i64 ab;
            i64 ad;                   /* inclusive */
            b32 frustum;              /* Range: bytes=ab-ad */
         chorda si_etag;              /* If-Match */
         chorda nisi_etag;            /* If-None-Match */
    MomentumSec si_mutatum_post;      /* If-Modified-Since; 0 nullum */
} HorreumCapiendi;

nomen structura {
    chorda praefixum;
    chorda delimitator;
       i32 magnitudo_paginae;          /* 0 = M */
    chorda signum_continuationis;
} HorreumEnumerandi;

nomen structura {
    HorreumObjectum* obiecta;
                i32  obiecta_numerus;
             chorda* praefixa_communia;
                i32  praefixa_numerus;
                b32  truncata;
             chorda  signum_proximum;
} HorreumPagina;

nomen structura {
          chorda clavis;
             b32 deletum;
    HorreumError error;
} HorreumDeletio;

nomen structura {
       i32 numerus;
    chorda etag;
       i64 mensura;
} HorreumPars;

nomen structura {
         chorda clavis;
         chorda id_oneris;
    MomentumSec initium;
} HorreumOnusPendens;

nomen enumeratio {
    HORREUM_OPUS_PONERE = 0,
    HORREUM_OPUS_CAPERE,
    HORREUM_OPUS_INSPICERE,
    HORREUM_OPUS_DELERE,
    HORREUM_OPUS_DELERE_PLURA,
    HORREUM_OPUS_COPIARE,
    HORREUM_OPUS_ENUMERARE,
    HORREUM_OPUS_MP_INCIPERE,
    HORREUM_OPUS_MP_PARS,
    HORREUM_OPUS_MP_PERFICERE,
    HORREUM_OPUS_MP_ABICERE,
    HORREUM_OPUS_MP_PARTES,
    HORREUM_OPUS_MP_PENDENTES
} HorreumOpusGenus;

/* descriptio petitionis ante signaturam (P6) */
nomen structura {
    HorreumOpusGenus  genus;
        HttpMethodus  methodus;
              chorda  situla;
              chorda  clavis;                   /* decodificata */
        SignaturaPar* quaestio;
                 i32  quaestio_numerus;
        SignaturaPar* capita;
                 i32  capita_numerus;
              chorda  corpus;
              chorda  corpus_sha256_hex;
                 b32  condicionale;             /* P10 */
                 b32  idempotens;
              chorda  perficere_etag_expectatum;
} HorreumOpus;

/* observatio: executores (horreum_cliens, horreum_vocatio) observatorem
 * post QUEMQUE conatum vocant (iterationes et HEAD iam_nostrum
 * comprehensa), antequam responsum legitur. Omnia valent solum intra
 * vocationem. horreum ipsum nihil scribit: vocans decernit quo eat
 * (horreum_observationem_describere -> filum, stderr). */
nomen structura {
      constans HorreumOpus* opus;
      constans HttpPetitio* petitio;            /* signata */
    constans HttpResponsum* responsum;          /* NIHIL: transportus
                                                  * defecit */
                    chorda error_transportus;
                       i32 conatus;            /* operis, 1.. */
} HorreumObservatio;

nomen vacuum (*HorreumObservator)(vacuum* datum,
    constans HorreumObservatio* o);

constans HorreumProvisor*
horreum_provisor (
    constans character* titulus);

Horreum*
horreum_creare (
    constans HorreumConfiguratio* c,
                    HorreumError* error,
                         Piscina* piscina);

/* via ad filum tabellae (P5); modus latior quam 0600 recusatur */
Horreum*
horreum_ex_tabella (
    constans character* via,
          HorreumError* error,
               Piscina* piscina);

chorda
horreum_situla_ordinaria (
    Horreum* h);

constans HorreumProvisor*
horreum_provisor_horrei (
    constans Horreum* h);

/* zephyrat (secretum inclusum); NIHIL tutum */
vacuum
horreum_claudere (
    Horreum* h);

/* sedes executoris: horreum_cliens statum suum (optiones, sors,
 * deviatio horologii) ibi servat; ceteri non tangunt. NIHIL initio */
vacuum
horreum_sedem_ponere (
    Horreum* h,
     vacuum* sedes);

vacuum*
horreum_sedes (
    Horreum* h);

/* SOLUM horreum probare (horreum_probatio.h): capacitates omnes creditae
 * et probatae - aedificatores nihil recusant, ut probatio quaerat quod
 * provisor revera facit. NIHIL tutum */
vacuum
horreum_omnia_permittere (
    Horreum* h);

b32
horreum_opus_ponere (
                    Horreum* h,
                     chorda  situla,
                     chorda  clavis,
                     chorda  corpus,
    constans HorreumPonendi* optiones,
                HorreumOpus* opus,
               HorreumError* error,
                    Piscina* piscina);

b32
horreum_opus_capere (
                     Horreum* h,
                      chorda  situla,
                      chorda  clavis,
    constans HorreumCapiendi* optiones,
                 HorreumOpus* opus,
                HorreumError* error,
                     Piscina* piscina);

b32
horreum_opus_inspicere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
     HorreumOpus* opus,
    HorreumError* error,
         Piscina* piscina);

b32
horreum_opus_delere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
     HorreumOpus* opus,
    HorreumError* error,
         Piscina* piscina);

b32
horreum_opus_delere_plura (
            Horreum* h,
             chorda  situla,
    constans chorda* claves,
                i32  numerus,
        HorreumOpus* opus,
       HorreumError* error,
            Piscina* piscina);

/* optiones NIHIL = meta fontis servatur (COPY), aliter REPLACE */
b32
horreum_opus_copiare (
                    Horreum* h,
                     chorda  situla_fons,
                     chorda  clavis_fons,
                     chorda  situla,
                     chorda  clavis,
    constans HorreumPonendi* optiones,
                HorreumOpus* opus,
               HorreumError* error,
                    Piscina* piscina);

b32
horreum_opus_enumerare (
                       Horreum* h,
                        chorda  situla,
    constans HorreumEnumerandi* optiones,
                   HorreumOpus* opus,
                  HorreumError* error,
                       Piscina* piscina);

b32
horreum_opus_mp_incipere (
                    Horreum* h,
                     chorda  situla,
                     chorda  clavis,
    constans HorreumPonendi* optiones,
                HorreumOpus* opus,
               HorreumError* error,
                    Piscina* piscina);

b32
horreum_opus_mp_pars (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id_oneris,
             i32  numerus,
          chorda  corpus,
     HorreumOpus* opus,
    HorreumError* error,
         Piscina* piscina);

b32
horreum_opus_mp_perficere (
                 Horreum* h,
                  chorda  situla,
                  chorda  clavis,
                  chorda  id_oneris,
    constans HorreumPars* partes,
                     i32  numerus,
             HorreumOpus* opus,
            HorreumError* error,
                 Piscina* piscina);

b32
horreum_opus_mp_abicere (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id_oneris,
     HorreumOpus* opus,
    HorreumError* error,
         Piscina* piscina);

b32
horreum_opus_mp_partes (
         Horreum* h,
          chorda  situla,
          chorda  clavis,
          chorda  id_oneris,
     HorreumOpus* opus,
    HorreumError* error,
         Piscina* piscina);

b32
horreum_opus_mp_pendentes (
         Horreum* h,
          chorda  situla,
          chorda  praefixum,
     HorreumOpus* opus,
    HorreumError* error,
         Piscina* piscina);

/* opus -> HttpPetitio signata: Host, x-amz-date, x-amz-content-sha256,
 * Authorization, Content-Length 0 si corpus vacuum (P3). tempus =
 * horologium + deviatio (vocans). NIHIL + error si opus malum. */
HttpPetitio*
horreum_petitionem_signare (
                 Horreum* h,
    constans HorreumOpus* opus,
             MomentumSec  tempus,
            HorreumError* error,
                 Piscina* piscina);

/* URL praesignata GET aut PUT (secundae 1..604800); genus_contenti
 * (PUT) signatur si non vacuum. Chorda vacua + error si malum. */
chorda
horreum_praesignare (
         Horreum* h,
    HttpMethodus  methodus,
          chorda  situla,
          chorda  clavis,
             i32  secundae,
          chorda  genus_contenti,
     MomentumSec  tempus,
    HorreumError* error,
         Piscina* piscina);

/* resultus -> VERUM (facta in obiectum) aut FALSUM (error typatus).
 * tempus_signandi = tempus quo petitio signata est (deviatio
 * horologii ex capite Date servi). obiectum NIHIL licet. CopyObject
 * et CompleteMultipartUpload: '200 + <Error>' error est (status CC);
 * ETag (et tempus copiae) ex corpore. */
b32
horreum_responsum_legere (
    constans HorreumOpus* opus,
            HttpResultus  resultus,
             MomentumSec  tempus_signandi,
         HorreumObjectum* obiectum,
            HorreumError* error,
                 Piscina* piscina);

b32
horreum_paginam_legere (
           chorda  corpus,
    HorreumPagina* pagina,
     HorreumError* error,
          Piscina* piscina);

b32
horreum_deletiones_legere (
             chorda   corpus,
     HorreumDeletio** exitus,
                i32*  numerus,
       HorreumError*  error,
            Piscina*  piscina);

b32
horreum_id_oneris_legere (
          chorda  corpus,
          chorda* id,
    HorreumError* error,
         Piscina* piscina);

b32
horreum_partes_legere (
          chorda   corpus,
     HorreumPars** exitus,
             i32*  numerus,
    HorreumError*  error,
         Piscina*  piscina);

b32
horreum_pendentes_legere (
                 chorda   corpus,
     HorreumOnusPendens** exitus,
                    i32*  numerus,
           HorreumError*  error,
                Piscina*  piscina);

/* CompleteMultipartUpload: S3 '200 + <Error>' quoque reddere potest */
b32
horreum_etag_perfecti_legere (
          chorda  corpus,
          chorda* etag,
    HorreumError* error,
         Piscina* piscina);

/* politica (pura): iterandumne post conatum 'conatus' (1..)? mora in
 * ms in [basis * 2^(c-1), basis * 2^c), Retry-After (secundae) si
 * maior; fortuna = valor aleatorius (vocans ex sors) */
b32
horreum_iterandum (
     constans HorreumOpus* opus,
    constans HorreumError* error,
                   chorda  mora_servi,
                      i32  conatus,
                      i32  conatus_maximi,
                      i32  mora_basis_ms,
                      i32  fortuna,
                      i32* mora_ms);

/* P12: pars aequalis, >= HORREUM_PARS_MINIMA, partes <= MAXIMAE,
 * multiplum MiB; petita 0 = HORREUM_PARS_ORDINARIA */
i64
horreum_partis_mensura (
    i64 mensura_tota,
    i64 petita);

/* transcriptio legibilis conatus unius: "> " linea petitionis, capita
 * (Authorization et X-Amz-Security-Token TECTA), corpus; "< " status,
 * capita, corpus responsi (aut defectus transportus). Corpus binarium
 * solum mensura nominatur; textus ultra IV KiB abbreviatur. */
chorda
horreum_observationem_describere (
    constans HorreumObservatio* o,
                       Piscina* piscina);

/* tempora (P7): ISO 8601 cum fractione; IMF-fixdate HTTP. 0 = malum */
MomentumSec
horreum_tempus_iso (
    chorda s);

MomentumSec
horreum_tempus_http (
    chorda s);

#endif /* HORREUM_H */
