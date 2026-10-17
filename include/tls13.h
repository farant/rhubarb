/* tls13.h - MACHINA clientis TLS 1.3 (RFC 8446; tls13-spec II.2):
 * pactio, recorda, data - sine fossis, sine platforma, sine horologio,
 * sine globalibus. Vectura (tls_tls13.c) octetos inter machinam et
 * TcpConnexio movet, ut tls_bearssl.c cum BearSSL facit.
 *
 * Fluxus: creare -> ClientHello in exitu (RECORDA_MITTENDA); vectura
 * recorda exitus mittit et recorda accepta introitui dat; post
 * Finished nostrum DATA_MITTI_POSSUNT. Data scripta in recorda
 * obsignantur cum effundere vocatur; data accepta in capsa manent donec
 * lecta (contractus 'exhauri usque ad TLS_ITERUM').
 *
 * Fiducia per REDITUM (fiducia_iudicare sine ancora - vectura eam in
 * datum ligat); fors per reditum (NIHIL = HMAC-DRBG semine systematis).
 *
 * ORDO FORTIS (probationes fortem fixam ordine vocationum praebent):
 *   (1) clavis privata x25519, XXXII octeti - semper;
 *   (2) temere clientis, XXXII - solum sine salutatione fixa;
 *   (3) legacy_session_id, XXXII - idem;
 *   (4) post HelloRetryRequest quae P-256 petit: clavis privata P-256,
 *       XXXII (iterata si invalida). HRR cum crustulo solo (sine
 *       key_share) clavem x25519 primam iterum adhibet - nulla vocatio.
 *
 * Omnia in piscina vocantis; capsae fixae (recordum maximum utrimque);
 * post creationem sola DER catenae servatur in piscina. */
#ifndef TLS13_H
#define TLS13_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fiducia.h"

nomen structura Tls13Machina Tls13Machina;

/* fiducia: forma fiducia_iudicare sine ancora (in datum ligata) */
nomen b32 (*Tls13Fiducia)(
                        vacuum* datum,
    constans FiduciaCertificatum* catena,
                             i32  numerus,
              constans character* hospes,
                          chorda* causa,
                         Piscina* piscina);

/* fors: mensura octeti in exitus */
nomen vacuum (*Tls13Fors)(
    vacuum* datum,
        i8* exitus,
       i32  mensura);

nomen structura {
    constans character* hospes;          /* SNI + nomen fiduciae */
                   b32  verificare;      /* FALSUM: fiducia omittitur */
          Tls13Fiducia  fiducia;
                vacuum* fiducia_datum;
             Tls13Fors  fors;            /* NIHIL = DRBG */
                vacuum* fors_datum;
    /* PROBATIONES SOLUM (tls13-plan P2): ClientHello fixa (TOTA, cum
     * capite IV octetorum) loco encodatoris - [0] CH1, [1] CH2 post
     * HRR; NIHIL = encodator */
           constans i8* salutationes_fixae[II];
                   i32  salutationes_fixae_n[II];
} Tls13Optiones;

/* status (tls13_machina_status), bits */
#define TLS13_RECORDA_MITTENDA    I
#define TLS13_RECORDA_EXSPECTAT   II
#define TLS13_DATA_MITTI_POSSUNT  IV
#define TLS13_DATA_ADSUNT         VIII
#define TLS13_CLAUSA              XVI

nomen enumeratio {
    TLS13_ERROR_NULLUS = ZEPHYRUM,
    TLS13_ERROR_SOLUM_1_2,         /* hospes ut TLS 1.2 respondit */
    TLS13_ERROR_FIDUCIA,           /* fiducia negavit (alerta 42) */
    TLS13_ERROR_SUBSCRIPTIO,       /* CertificateVerify falsa (51) */
    TLS13_ERROR_FINIS,             /* Finished servi falsum (51) */
    TLS13_ERROR_FORMA,             /* nuntius male formatus (50) */
    TLS13_ERROR_PARAMETRUM,        /* non oblatum / non acceptum (47) */
    TLS13_ERROR_MAC,               /* AEAD fracta (20) */
    TLS13_ERROR_MOLES,             /* recordum nimis magnum (22) */
    TLS13_ERROR_ALERTA_RECEPTA,    /* alerta fatalis a pare */
    TLS13_ERROR_ENTROPIA           /* semen systematis defecit */
} Tls13ErrorCodex;

nomen structura {
    Tls13ErrorCodex codex;
             chorda causa;
                i32 alerta;      /* missa (aut recepta); 0 nulla */
} Tls13Error;

/* NIHIL si piscina exhausta, salutatio fixa nimis longa; error
 * ENTROPIA in machina reddita si semen defecit (status 0) */
Tls13Machina*
tls13_machina_creare (
    constans Tls13Optiones* optiones,
                   Piscina* piscina);

/* TLS13_RECORDA_MITTENDA | _RECORDA_EXSPECTAT | _DATA_MITTI_POSSUNT |
 * _DATA_ADSUNT | _CLAUSA; 0 post errorem cum nihil mittendum */
i32
tls13_machina_status (
    Tls13Machina* machina);

/* recorda mittenda: index et *n; ack(k) k octetos missos consumit */
i8*
tls13_recorda_exitus (
    Tls13Machina* machina,
             i32* n);

vacuum
tls13_recorda_exitus_ack (
    Tls13Machina* machina,
             i32  n);

/* spatium pro octetis acceptis: index et *n capacitas (0 = nunc
 * nihil accipit); ack(k) k octetos scriptos tractat */
i8*
tls13_recorda_introitus (
    Tls13Machina* machina,
             i32* n);

vacuum
tls13_recorda_introitus_ack (
    Tls13Machina* machina,
             i32  n);

/* spatium pro datis scribendis (solum post pactionem) */
i8*
tls13_data_exitus (
    Tls13Machina* machina,
             i32* n);

vacuum
tls13_data_exitus_ack (
    Tls13Machina* machina,
             i32  n);

/* data scripta in recordum obsignat */
vacuum
tls13_data_effundere (
    Tls13Machina* machina);

/* data accepta legenda */
i8*
tls13_data_introitus (
    Tls13Machina* machina,
             i32* n);

vacuum
tls13_data_introitus_ack (
    Tls13Machina* machina,
             i32  n);

/* close_notify obsignatum in exitum */
vacuum
tls13_machina_claudere (
    Tls13Machina* machina);

Tls13Error
tls13_machina_error (
    Tls13Machina* machina);

/* 0x0304 post pactionem; 0 ante */
i32
tls13_machina_versio (
    Tls13Machina* machina);

/* catena servi (folium primum), DER in piscina; NIHIL ante */
constans FiduciaCertificatum*
tls13_machina_catena (
    Tls13Machina* machina,
             i32* numerus);

#endif /* TLS13_H */
