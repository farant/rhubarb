/* tls13_nuntii.h - nuntii pactionis TLS 1.3 (RFC 8446 §4, tls13-spec
 * II.1): ClientHello nostrum scribere; ServerHello / HelloRetryRequest,
 * EncryptedExtensions, Certificate, CertificateVerify, KeyUpdate
 * legere.
 *
 * Lectores CORPORA accipiunt (sine capite IV octetorum typus +
 * longitudo - machina id iam legit) et numquam extra fines legunt:
 * omne vitium formae (vector longior quam reliquum, octeti superflui)
 * FALSUM reddit. Indices in exitu (clavis, crustulum, certificata,
 * subscriptio) in corpus ipsum monstrant - vocans corpus servat aut
 * transcribit. */
#ifndef TLS13_NUNTII_H
#define TLS13_NUNTII_H

#include "latina.h"
#include "fiducia.h"

/* NamedGroup (RFC 8446 4.2.7) */
#define TLS13_GREX_X25519     0x001d
#define TLS13_GREX_SECP256R1  0x0017

/* HandshakeType (RFC 8446 4) */
#define TLS13_NUNTIUS_CLIENTHELLO   I
#define TLS13_NUNTIUS_SERVERHELLO   II
#define TLS13_NUNTIUS_NST           IV
#define TLS13_NUNTIUS_EE            VIII
#define TLS13_NUNTIUS_CERTIFICATUM  XI
#define TLS13_NUNTIUS_CR            XIII  /* CertificateRequest */
#define TLS13_NUNTIUS_CV            XV
#define TLS13_NUNTIUS_FINIS         XX
#define TLS13_NUNTIUS_KEYUPDATE     XXIV

/* parametri ClientHello */
nomen structura {
    constans character* hospes;          /* server_name; NIHIL aut IP
                                          * litteralis -> omittitur */
           constans i8* temere;           /* XXXII octeti */
           constans i8* sessio;           /* legacy_session_id */
                   i32  sessio_n;        /* 0 aut XXXII */
                   i32  grex;            /* key_share unicus */
           constans i8* clavis_publica;
                   i32  clavis_n;        /* XXXII x25519, LXV P-256 */
           constans i8* crustulum;        /* cookie HRR; NIHIL nullum */
                   i32  crustulum_n;
} Tls13SalutatioParametri;

/* ClientHello TOTUM (cum capite pactionis IV octetorum) in exitus:
 * cifrae 1301 1302 1303 (et sex 1.2 pro descensu: c02b c02f c02c c030
 * cca9 cca8); extensiones server_name, supported_groups
 * (x25519, secp256r1), signature_algorithms (0403 0503 0804 0805 0806;
 * et 0401 0501 0601 pro servis 1.2 tantum),
 * supported_versions (0304), cookie (si datum), key_share. Reddit
 * longitudinem, aut 0 si capacitas deficit */
i32
tls13_salutatio_scribere (
    constans Tls13SalutatioParametri* parametri,
                                  i8* exitus,
                                 i32  capacitas);

/* ServerHello aut HelloRetryRequest lectum */
nomen structura {
             b32  hrr;               /* temere = constans HRR */
             b32  versio_1_2;        /* supported_versions abest */
             b32  signum_descensus;  /* temere finit DOWNGRD\0 / \1 */
             i32  versio;            /* supported_versions (0x0304) */
             i32  cifra;
     constans i8* sessio;            /* legacy_session_id_echo */
             i32  sessio_n;
             i32  grex;              /* key_share (SH) aut grex
                                      * selectus (HRR) */
     constans i8* clavis;            /* key_exchange servi (SH) */
             i32  clavis_n;
     constans i8* crustulum;         /* cookie (HRR) */
             i32  crustulum_n;
} Tls13ResponsumServi;

/* ServerHello / HRR (RFC 8446 4.1.3-4.1.4); FALSUM = forma mala */
b32
tls13_responsum_legere (
            constans i8* corpus,
                    i32  n,
    Tls13ResponsumServi* responsum);

/* EncryptedExtensions: vector extensionum bene formatus (ignotae
 * praetereuntur) */
b32
tls13_extensiones_legere (
     constans i8* corpus,
             i32  n);

/* Certificate (RFC 8446 4.4.2): contextus vacuus, catena DER (folium
 * primum) in exitus; FALSUM si catena vacua aut plura quam maximum */
b32
tls13_certificata_legere (
            constans i8* corpus,
                    i32  n,
    FiduciaCertificatum* exitus,
                    i32  maximum,
                    i32* numerus);

/* CertificateVerify (RFC 8446 4.4.3): schema et subscriptio */
b32
tls13_cv_legere (
      constans i8*  corpus,
              i32   n,
              i32*  schema,
      constans i8** subscriptio,
              i32*  subscriptio_n);

/* KeyUpdate (RFC 8446 4.6.3): octetus unus 0 aut 1; *rogatur =
 * update_requested */
b32
tls13_keyupdate_legere (
     constans i8* corpus,
             i32  n,
             b32* rogatur);

#endif /* TLS13_NUNTII_H */
