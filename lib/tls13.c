/* tls13.c - MACHINA clientis TLS 1.3 (RFC 8446; tls13-spec II.2,
 * tls13-plan T4): capsae recordorum et datorum, fors et fiducia per
 * reditus, nihil globale, nulla fossa.
 *
 * Stadia: SALUTATIO_MISSA -> EE -> CERT -> CV -> FINIS -> APERTA ->
 * CLAUSA, aut ERROR (alerta, si qua, in exitu iam posita).
 *
 * Recorda: introitus recorda integra (caput V + longitudo) tractat;
 * nuntii pactionis per caput IV octetorum reassemblantur, quomodocumque
 * recorda eos secant (Review focus 1). Recordum datorum non aperitur
 * dum data priora non lecta sunt - capsa una, ordo servatus.
 *
 * Claves: post ServerHello legere = s hs, scribere = c hs (alerta in
 * volatu servi sub c hs obsignatur); post Finished servi c ap / s ap.
 * CCS fictum (RFC 8446 D.4) ante recordum nostrum primum obsignatum, si
 * legacy_session_id non vacuum (tls13-plan P3). */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fiducia.h"
#include "tls13_clavis.h"
#include "tls13_recordum.h"
#include "tls13_nuntii.h"
#include "tls13.h"
#include <stdio.h>
#include <string.h>

#define PACTIO_MAXIMA     (LXIV * MXXIV)   /* nuntius coniunctus */
#define SALUTATIO_MAXIMA  MXXIV
#define DATA_MAXIMA       (XVI * MXXIV)   /* 2^14: recordum unum */
#define EXITUS_MAXIMUM    (II * TLS13_RECORDUM_MAXIMUM)
#define CATENA_MAXIMA     XVI
#define CAPUT             V                /* caput recordi */

/* AlertDescription (RFC 8446 6) */
#define ALERTA_CLAUSURA     ZEPHYRUM
#define ALERTA_INOPINATUM   X
#define ALERTA_MAC          XX
#define ALERTA_MOLES        XXII
#define ALERTA_CERTIFICATUM XLII
#define ALERTA_PARAMETRUM   XLVII
#define ALERTA_FORMA        L
#define ALERTA_VERIFICATIO    LI
#define ALERTA_VERSIO       LXX
#define ALERTA_INTERNA      LXXX
#define ALERTA_REVOCATA     XC                /* user_canceled */

nomen enumeratio {
    STADIUM_SALUTATIO_MISSA = ZEPHYRUM,
    STADIUM_EE,
    STADIUM_CERT,
    STADIUM_CV,
    STADIUM_FINIS,
    STADIUM_APERTA,
    STADIUM_CLAUSA,
    STADIUM_ERROR
} Stadium;

structura Tls13Machina {
                    Piscina* piscina;
              Tls13Optiones  opt;
                    Stadium  stadium;
                        i32  cifra;
                        i32  genus;             /* hash cifrae */
          Tls13Transcriptum* transcriptum;
                         i8  salutatio[SALUTATIO_MAXIMA];
                        i32  salutatio_n;       /* CH in transcripto
                                                  * post ServerHello */
                         i8 sessio[XXXII];
                        i32 sessio_n;
                        b32 ccs_missa;
                        b32 clausura_missa;
                        b32 claves_mutatae;    /* nuntius ultimus
                                                  * claves mutavit */
                         i8  privata_x25519[XXXII];
                         i8  secretum_pactionis[TLS13_HASH_MAXIMUS];
                         i8  c_hs[TLS13_HASH_MAXIMUS];
                         i8  s_hs[TLS13_HASH_MAXIMUS];
             Tls13Protectio* legere;            /* NIHIL = planum */
             Tls13Protectio* scribere;
                         i8  in[TLS13_RECORDUM_MAXIMUM];
                        i32  in_n;
                         i8  ex[EXITUS_MAXIMUM];
                        i32  ex_n;
                         i8  planum[TLS13_RECORDUM_MAXIMUM];
                         i8  pactio[PACTIO_MAXIMA];
                        i32  pactio_n;
                         i8  data_in[DATA_MAXIMA];
                        i32  data_in_locus;
                        i32  data_in_n;
                         i8  data_ex[DATA_MAXIMA];
                        i32  data_ex_n;
        FiduciaCertificatum  catena[CATENA_MAXIMA];
                        i32  numerus;
    br_x509_decoder_context  decoder;
                        i32  versio;
                 Tls13Error  error;
       br_hmac_drbg_context  drbg;              /* si fors NIHIL */
};

hic_manens constans i8 NULLI[TLS13_HASH_MAXIMUS] = { ZEPHYRUM };

hic_manens constans character CONTEXTUS_CV[] =
    "TLS 1.3, server CertificateVerify";

interior i32
_u24 (
    constans i8* o)
{
    redde ((i32)o[ZEPHYRUM] << XVI) | ((i32)o[I] << VIII) | (i32)o[II];
}


/* ================================================== *
 * Exitus: recorda plana et obsignata, alertae, errores
 * ================================================== */

interior b32
_recordum_planum (
    Tls13Machina* m,
             i32  genus,
             i32  versio_minor,
     constans i8* data,
             i32  n)
{
    i8* e = m->ex + m->ex_n;

    si (m->ex_n + CAPUT + n > EXITUS_MAXIMUM)
    {
        redde FALSUM;
    }
    e[ZEPHYRUM]  = (i8)genus;
    e[I]         = III;
    e[II]        = (i8)versio_minor;
    e[III]       = (i8)(n >> VIII);
    e[IV]        = (i8)n;
    memcpy(e + CAPUT, data, n);
    m->ex_n += CAPUT + n;
    redde VERUM;
}

/* planum ante claves; postea obsignatum (CCS fictum primum, P3) */
interior b32
_recordum_mittere (
    Tls13Machina* m,
             i32  genus,
     constans i8* data,
             i32  n)
{
    i32 k;

    si (m->scribere == NIHIL)
    {
        redde _recordum_planum(m, genus, III, data, n);
    }
    si (m->sessio_n > ZEPHYRUM && !m->ccs_missa)
    {
        hic_manens constans i8 unum[I] = { I };

        si (!_recordum_planum(m, TLS13_CONTENTUM_CCS, III, unum, I))
        {
            redde FALSUM;
        }
        m->ccs_missa = VERUM;
    }
    k = tls13_obsignare(m->scribere, genus, data, n, m->ex + m->ex_n,
        EXITUS_MAXIMUM - m->ex_n);
    m->ex_n += k;
    redde k > ZEPHYRUM;
}

interior vacuum
_alertam_mittere (
    Tls13Machina* m,
             i32  gradus,
             i32  descriptio)
{
    i8 a[II];

    a[ZEPHYRUM]  = (i8)gradus;
    a[I]         = (i8)descriptio;
    (vacuum)_recordum_mittere(m, TLS13_CONTENTUM_ALERTA, a, II);
}

/* error semel ponitur; alerta fatalis (si non 0) in exitum */
interior vacuum
_errorem_chorda (
       Tls13Machina* m,
    Tls13ErrorCodex  codex,
                i32  alerta,
             chorda  causa)
{
    si (m->stadium == STADIUM_ERROR)
    {
        redde;
    }
    m->error.codex   = codex;
    m->error.causa   = causa;
    m->error.alerta  = alerta;
    si (alerta != ZEPHYRUM)
    {
        _alertam_mittere(m, II, alerta);
    }
    m->stadium = STADIUM_ERROR;
}

interior vacuum
_errorem (
           Tls13Machina* m,
        Tls13ErrorCodex  codex,
                    i32  alerta,
     constans character* causa)
{
    _errorem_chorda(m, codex, alerta, chorda_ex_literis(causa,
        m->piscina));
}


/* ================================================== *
 * Fors et curvae
 * ================================================== */

interior vacuum
_fors (
    Tls13Machina* m,
              i8* exitus,
             i32  n)
{
    si (m->opt.fors != NIHIL)
    {
        m->opt.fors(m->opt.fors_datum, exitus, n);
    }
    alioquin
    {
        br_hmac_drbg_generate(&m->drbg, exitus, (size_t)n);
    }
}

/* X25519: BearSSL scalarem big-endian accipit, RFC 7748 little-endian
 * dat (tls13-plan P6); coordinata u manet */
interior vacuum
_invertere (
    constans i8* a,
             i8* b)
{
    i32 k;

    per (k = ZEPHYRUM; k < XXXII; k++)
    {
        b[k] = a[XXXI - k];
    }
}

interior b32
_x25519_publica (
    constans i8* privata,
             i8* publica)
{
    i8 inversa[XXXII];

    _invertere(privata, inversa);
    redde br_ec_get_default()->mulgen(publica, inversa, XXXII,
        BR_EC_curve25519) == XXXII;
}

/* secretum communis; FALSUM si mul fallit aut exitus totus nullus
 * (RFC 8446 7.4.2) */
interior b32
_x25519_communis (
    constans i8* privata,
    constans i8* alienum,
             i8* exitus)
{
     i8 inversa[XXXII];
     i8 summa = ZEPHYRUM;
    i32 k;

    _invertere(privata, inversa);
    memcpy(exitus, alienum, XXXII);
    si (br_ec_get_default()->mul(exitus, XXXII, inversa, XXXII,
        BR_EC_curve25519) != I)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < XXXII; k++)
    {
        summa |= exitus[k];
    }
    redde summa != ZEPHYRUM;
}

interior constans br_hash_class*
_classis (
    i32 genus)
{
    redde genus == TLS13_SHA384 ? &br_sha384_vtable : &br_sha256_vtable;
}

/* Hash("") hash cifrae */
interior vacuum
_hash_vacuum (
    i32  genus,
     i8* exitus)
{
    br_hash_compat_context h;

    _classis(genus)->init(&h.vtable);
    _classis(genus)->out(&h.vtable, exitus);
}

interior b32
_aequalia_tempore_constanti (
    constans i8* a,
    constans i8* b,
            i32  n)
{
     i8 differentia = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        differentia |= (i8)(a[k] ^ b[k]);
    }
    redde differentia == ZEPHYRUM;
}


/* ================================================== *
 * Nuntii pactionis
 * ================================================== */

interior vacuum
_responsum (
    Tls13Machina* m,
     constans i8* corpus,
             i32  n,
     constans i8* crudum,
             i32  crudi_n)
{
    Tls13ResponsumServi r;
                     i8 communis[XXXII];
                     i8 primum[TLS13_HASH_MAXIMUS];
                     i8 sal[TLS13_HASH_MAXIMUS];
                     i8 vacuum_hash[TLS13_HASH_MAXIMUS];
                     i8 hash_transcripti[TLS13_HASH_MAXIMUS];
                    i32 h;

    si (!tls13_responsum_legere(corpus, n, &r))
    {
        _errorem(m, TLS13_ERROR_FORMA, ALERTA_FORMA,
            "ServerHello male formatum");
        redde;
    }
    si (r.versio_1_2)
    {
        _errorem(m, TLS13_ERROR_SOLUM_1_2, ZEPHYRUM,
            "hospes ut TLS 1.2 respondit");
        redde;
    }
    si (r.hrr)
    {
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_PARAMETRUM,
            "HelloRetryRequest nondum sustinetur");
        redde;
    }
    si (r.versio != 0x0304)
    {
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_PARAMETRUM,
            "supported_versions non 0x0304");
        redde;
    }
    si (tls13_cifra_genus(r.cifra) == ZEPHYRUM)
    {
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_PARAMETRUM,
            "cifra non oblata");
        redde;
    }
    si (r.grex != TLS13_GREX_X25519 || r.clavis_n != XXXII)
    {
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_PARAMETRUM,
            "key_share servi non x25519");
        redde;
    }
    si (   r.sessio_n                               != m->sessio_n
        || memcmp(r.sessio, m->sessio, m->sessio_n) != ZEPHYRUM)
    {
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_PARAMETRUM,
            "legacy_session_id_echo alienum");
        redde;
    }
    si (!_x25519_communis(m->privata_x25519, r.clavis, communis))
    {
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_PARAMETRUM,
            "clavis x25519 servi invalida");
        redde;
    }
    m->cifra         = r.cifra;
    m->genus         = tls13_cifra_genus(r.cifra);
    h                = tls13_hash_mensura(m->genus);
    m->transcriptum  = tls13_transcriptum_creare(m->genus, m->piscina);
    si (m->transcriptum == NIHIL)
    {
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_INTERNA,
            "piscina exhausta");
        redde;
    }
    tls13_transcriptum_addere(m->transcriptum, m->salutatio,
        m->salutatio_n);
    tls13_transcriptum_addere(m->transcriptum, crudum, crudi_n);

    /* RFC 8446 7.1: early -> handshake */
    _hash_vacuum(m->genus, vacuum_hash);
    tls13_hkdf_extrahere(m->genus, NIHIL, ZEPHYRUM, NULLI, h, primum);
    tls13_secretum_derivare(m->genus, primum, "derived", vacuum_hash,
        sal);
    tls13_hkdf_extrahere(m->genus, sal, h, communis, XXXII,
        m->secretum_pactionis);
    tls13_transcriptum_digerere(m->transcriptum, hash_transcripti);
    tls13_secretum_derivare(m->genus, m->secretum_pactionis,
        "c hs traffic", hash_transcripti, m->c_hs);
    tls13_secretum_derivare(m->genus, m->secretum_pactionis,
        "s hs traffic", hash_transcripti, m->s_hs);
    m->legere = tls13_protectio_creare(m->cifra, m->s_hs, m->piscina);
    m->scribere = tls13_protectio_creare(m->cifra, m->c_hs, m->piscina);
    si (m->legere == NIHIL || m->scribere == NIHIL)
    {
        m->scribere = NIHIL;
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_INTERNA,
            "piscina exhausta");
        redde;
    }
    m->claves_mutatae  = VERUM;
    m->stadium         = STADIUM_EE;
}

interior vacuum
_certificata (
    Tls13Machina* m,
     constans i8* corpus,
             i32  n)
{
    FiduciaCertificatum lecta[CATENA_MAXIMA];
                    i32 numerus = ZEPHYRUM;
                    i32 k;

    si (!tls13_certificata_legere(corpus, n, lecta, CATENA_MAXIMA,
        &numerus))
    {
        _errorem(m, TLS13_ERROR_FORMA, ALERTA_FORMA,
            "Certificate male formatum");
        redde;
    }
    /* DER in piscinam: pactio transit, catena manet */
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        i8* der = (i8*)piscina_allocare(m->piscina,
            (memoriae_index)lecta[k].mensura);

        si (der == NIHIL)
        {
            _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_INTERNA,
                "piscina exhausta");
            redde;
        }
        memcpy(der, lecta[k].octeti, lecta[k].mensura);
        m->catena[k].octeti   = der;
        m->catena[k].mensura  = lecta[k].mensura;
    }
    m->numerus = numerus;
    br_x509_decoder_init(&m->decoder, NIHIL, NIHIL);
    br_x509_decoder_push(&m->decoder, m->catena[ZEPHYRUM].octeti,
        (size_t)m->catena[ZEPHYRUM].mensura);
    si (br_x509_decoder_get_pkey(&m->decoder) == NIHIL)
    {
        _errorem(m, TLS13_ERROR_FIDUCIA, ALERTA_CERTIFICATUM,
            "folium non est certificatum X.509 legibile");
        redde;
    }
    m->stadium = STADIUM_CV;
}

/* hash schematis CertificateVerify; NIHIL si non oblatum */
interior constans br_hash_class*
_classis_schematis (
    i32 schema)
{
    commutatio (schema)
    {
        casus 0x0403:
        casus 0x0804:
            redde &br_sha256_vtable;
        casus 0x0503:
        casus 0x0805:
            redde &br_sha384_vtable;
        casus 0x0806:
            redde &br_sha512_vtable;
        ordinarius:
            redde NIHIL;
    }
}

/* fiducia primum, deinde subscriptio super transcriptum usque ad
 * Certificate (RFC 8446 4.4.3) */
interior vacuum
_subscriptio (
    Tls13Machina* m,
     constans i8* corpus,
             i32  n)
{
                       i32  schema         = ZEPHYRUM;
               constans i8* subscriptio    = NIHIL;
                       i32  subscriptio_n  = ZEPHYRUM;
                        i8  contentum[LXIV + XXXIV
                            + TLS13_HASH_MAXIMUS];
                        i8  digestum[LXIV];
    constans br_hash_class* classis;
    br_hash_compat_context  status_hash;
                       i32  mensura_digesti;
                       i32  h = tls13_hash_mensura(m->genus);
             br_x509_pkey*  clavis;
                       i32  bona;

    si (!tls13_cv_legere(corpus, n, &schema, &subscriptio,
        &subscriptio_n))
    {
        _errorem(m, TLS13_ERROR_FORMA, ALERTA_FORMA,
            "CertificateVerify male formatum");
        redde;
    }
    si (m->opt.verificare)
    {
        chorda causa;

        causa.mensura  = ZEPHYRUM;
        causa.datum    = NIHIL;
        si (   m->opt.fiducia == NIHIL
            || !m->opt.fiducia(m->opt.fiducia_datum, m->catena,
                m->numerus, m->opt.hospes, &causa, m->piscina))
        {
            si (causa.mensura == ZEPHYRUM)
            {
                causa = chorda_ex_literis("fiducia negata", m->piscina);
            }
            _errorem_chorda(m, TLS13_ERROR_FIDUCIA, ALERTA_CERTIFICATUM,
                causa);
            redde;
        }
    }
    classis = _classis_schematis(schema);
    si (classis == NIHIL)
    {
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_PARAMETRUM,
            "schema CertificateVerify non oblatum");
        redde;
    }
    memset(contentum, 0x20, LXIV);
    memcpy(contentum + LXIV, CONTEXTUS_CV, XXXIV);   /* cum 0x00 */
    tls13_transcriptum_digerere(m->transcriptum,
        contentum + LXIV + XXXIV);
    classis->init(&status_hash.vtable);
    classis->update(&status_hash.vtable, contentum,
        (size_t)(LXIV + XXXIV + h));
    classis->out(&status_hash.vtable, digestum);
    mensura_digesti = (i32)((classis->desc >> BR_HASHDESC_OUT_OFF)
        & BR_HASHDESC_OUT_MASK);
    clavis = br_x509_decoder_get_pkey(&m->decoder);
    si (schema == 0x0403 || schema == 0x0503)
    {
        i32 curva = schema
            == 0x0403 ? BR_EC_secp256r1 : BR_EC_secp384r1;

        si (   clavis->key_type     != BR_KEYTYPE_EC
            || clavis->key.ec.curve != (s32)curva)
        {
            _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_PARAMETRUM,
                "clavis folii schemati non congruit");
            redde;
        }
        bona = br_ecdsa_vrfy_asn1_get_default()(br_ec_get_default(),
            digestum, (size_t)mensura_digesti, &clavis->key.ec,
            subscriptio,
            (size_t)subscriptio_n);
    }
    alioquin
    {
        si (clavis->key_type != BR_KEYTYPE_RSA)
        {
            _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_PARAMETRUM,
                "clavis folii schemati non congruit");
            redde;
        }
        /* RSA-PSS: sal longitudine hash (RFC 8446 4.2.3) */
        bona = br_rsa_pss_vrfy_get_default()(subscriptio,
            (size_t)subscriptio_n, classis, classis, digestum,
            (size_t)mensura_digesti, &clavis->key.rsa);
    }
    si (!bona)
    {
        _errorem(m, TLS13_ERROR_SUBSCRIPTIO, ALERTA_VERIFICATIO,
            "subscriptio CertificateVerify falsa");
        redde;
    }
    m->stadium = STADIUM_FINIS;
}

/* Finished servi verificatum; secreta applicationis; volatus noster */
interior vacuum
_finis (
    Tls13Machina* m,
     constans i8* corpus,
             i32  n,
     constans i8* crudum,
             i32  crudi_n)
{
     i8 hash_transcripti[TLS13_HASH_MAXIMUS];
     i8 exspectata[TLS13_HASH_MAXIMUS];
     i8 vacuum_hash[TLS13_HASH_MAXIMUS];
     i8 sal[TLS13_HASH_MAXIMUS];
     i8 magister[TLS13_HASH_MAXIMUS];
     i8 c_ap[TLS13_HASH_MAXIMUS];
     i8 s_ap[TLS13_HASH_MAXIMUS];
     i8 finis[IV + TLS13_HASH_MAXIMUS];
    i32 h = tls13_hash_mensura(m->genus);

    si (n != h)
    {
        _errorem(m, TLS13_ERROR_FORMA, ALERTA_FORMA,
            "Finished longitudine aliena");
        redde;
    }
    tls13_transcriptum_digerere(m->transcriptum, hash_transcripti);
    tls13_finis_calculare(m->genus, m->s_hs, hash_transcripti,
        exspectata);
    si (!_aequalia_tempore_constanti(corpus, exspectata, h))
    {
        _errorem(m, TLS13_ERROR_FINIS, ALERTA_VERIFICATIO,
            "Finished servi falsum");
        redde;
    }
    tls13_transcriptum_addere(m->transcriptum, crudum, crudi_n);

    /* RFC 8446 7.1: handshake -> master; transcriptum per Finished
     * servi */
    _hash_vacuum(m->genus, vacuum_hash);
    tls13_secretum_derivare(m->genus, m->secretum_pactionis, "derived",
        vacuum_hash, sal);
    tls13_hkdf_extrahere(m->genus, sal, h, NULLI, h, magister);
    tls13_transcriptum_digerere(m->transcriptum, hash_transcripti);
    tls13_secretum_derivare(m->genus, magister, "c ap traffic",
        hash_transcripti, c_ap);
    tls13_secretum_derivare(m->genus, magister, "s ap traffic",
        hash_transcripti, s_ap);

    /* volatus noster sub c hs: (CCS) + Finished */
    finis[ZEPHYRUM]  = TLS13_NUNTIUS_FINIS;
    finis[I]         = ZEPHYRUM;
    finis[II]        = ZEPHYRUM;
    finis[III]       = (i8)h;
    tls13_finis_calculare(m->genus, m->c_hs, hash_transcripti,
        finis + IV);
    si (!_recordum_mittere(m, TLS13_CONTENTUM_PACTIO, finis, IV + h))
    {
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_INTERNA,
            "exitus plenus");
        redde;
    }
    tls13_transcriptum_addere(m->transcriptum, finis, IV + h);
    m->legere    = tls13_protectio_creare(m->cifra, s_ap, m->piscina);
    m->scribere  = tls13_protectio_creare(m->cifra, c_ap, m->piscina);
    si (m->legere == NIHIL || m->scribere == NIHIL)
    {
        m->scribere = NIHIL;
        _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_INTERNA,
            "piscina exhausta");
        redde;
    }
    m->versio          = 0x0304;
    m->claves_mutatae  = VERUM;
    m->stadium         = STADIUM_APERTA;
}

interior vacuum
_inopinatum (
    Tls13Machina* m)
{
    _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_INOPINATUM,
        "nuntius pactionis inopinatus");
}

interior vacuum
_nuntius (
    Tls13Machina* m,
             i32  typus,
     constans i8* corpus,
             i32  n,
     constans i8* crudum,
             i32  crudi_n)
{
    commutatio (m->stadium)
    {
        casus STADIUM_SALUTATIO_MISSA:
            si (typus != TLS13_NUNTIUS_SERVERHELLO)
            {
                _inopinatum(m);
                redde;
            }
            _responsum(m, corpus, n, crudum, crudi_n);
            redde;
        casus STADIUM_EE:
            si (typus != TLS13_NUNTIUS_EE)
            {
                _inopinatum(m);
                redde;
            }
            si (!tls13_extensiones_legere(corpus, n))
            {
                _errorem(m, TLS13_ERROR_FORMA, ALERTA_FORMA,
                    "EncryptedExtensions male formatum");
                redde;
            }
            tls13_transcriptum_addere(m->transcriptum, crudum, crudi_n);
            m->stadium = STADIUM_CERT;
            redde;
        casus STADIUM_CERT:
            si (typus != TLS13_NUNTIUS_CERTIFICATUM)
            {
                _inopinatum(m);
                redde;
            }
            _certificata(m, corpus, n);
            tls13_transcriptum_addere(m->transcriptum, crudum, crudi_n);
            redde;
        casus STADIUM_CV:
            si (typus != TLS13_NUNTIUS_CV)
            {
                _inopinatum(m);
                redde;
            }
            _subscriptio(m, corpus, n);
            tls13_transcriptum_addere(m->transcriptum, crudum, crudi_n);
            redde;
        casus STADIUM_FINIS:
            si (typus != TLS13_NUNTIUS_FINIS)
            {
                _inopinatum(m);
                redde;
            }
            _finis(m, corpus, n, crudum, crudi_n);
            redde;
        casus STADIUM_APERTA:
            /* NewSessionTicket: sine resumptione omittitur */
            si (typus != TLS13_NUNTIUS_NST)
            {
                _inopinatum(m);
            }
            redde;
        ordinarius:
            redde;
    }
}

/* nuntii integri ex fragmentis; nuntius claves mutans fragmentum
 * sequens non admittit (RFC 8446 5.1) */
interior vacuum
_pactio_addere (
    Tls13Machina* m,
     constans i8* o,
             i32  n)
{
    si (m->pactio_n + n > PACTIO_MAXIMA)
    {
        _errorem(m, TLS13_ERROR_MOLES, ALERTA_MOLES,
            "nuntius pactionis nimis magnus");
        redde;
    }
    memcpy(m->pactio + m->pactio_n, o, n);
    m->pactio_n += n;
    dum (m->stadium != STADIUM_ERROR && m->pactio_n >= IV)
    {
        i32 longitudo  = _u24(m->pactio + I);
        i32 totum      = IV + longitudo;

        si (totum > PACTIO_MAXIMA)
        {
            _errorem(m, TLS13_ERROR_MOLES, ALERTA_MOLES,
                "nuntius pactionis nimis magnus");
            redde;
        }
        si (m->pactio_n < totum)
        {
            frange;
        }
        m->claves_mutatae = FALSUM;
        _nuntius(m, (i32)m->pactio[ZEPHYRUM], m->pactio + IV, longitudo,
            m->pactio, totum);
        memmove(m->pactio, m->pactio + totum, m->pactio_n - totum);
        m->pactio_n -= totum;
        si (m->claves_mutatae && m->pactio_n > ZEPHYRUM)
        {
            _errorem(m, TLS13_ERROR_PARAMETRUM, ALERTA_INOPINATUM,
                "nuntius trans mutationem clavium");
        }
    }
}

interior vacuum
_alerta (
    Tls13Machina* m,
     constans i8* o,
             i32  n)
{
    character causa[CXXVIII];
          i32 descriptio;

    si (n != II)
    {
        _errorem(m, TLS13_ERROR_FORMA, ALERTA_FORMA,
            "alerta male formata");
        redde;
    }
    descriptio = (i32)o[I];
    si (descriptio == ALERTA_CLAUSURA)
    {
        m->stadium = STADIUM_CLAUSA;
        redde;
    }
    si (descriptio == ALERTA_REVOCATA && o[ZEPHYRUM] == I)
    {
        redde;
    }
    si (   descriptio == ALERTA_VERSIO
        && m->stadium == STADIUM_SALUTATIO_MISSA)
    {
        _errorem(m, TLS13_ERROR_SOLUM_1_2, ZEPHYRUM,
            "protocol_version ad ClientHello");
        redde;
    }
    sprintf(causa, "nuntius fatalis a pare: %u",
        (insignatus)descriptio);
    _errorem(m, TLS13_ERROR_ALERTA_RECEPTA, ZEPHYRUM, causa);
    m->error.alerta = descriptio;
}

/* unum recordum integrum (caput + corpus) */
interior vacuum
_recordum (
    Tls13Machina* m,
     constans i8* r,
             i32  n)
{
    i32 genus   = (i32)r[ZEPHYRUM];
    i32 corpus  = n - CAPUT;
    s32 k;
    i32 interius = ZEPHYRUM;

    si (genus == TLS13_CONTENTUM_CCS)
    {
        /* CCS servi in pactione omittitur (RFC 8446 D.4) */
        si (m->stadium == STADIUM_APERTA)
        {
            _inopinatum(m);
        }
        alioquin si (corpus != I || r[CAPUT] != I)
        {
            _errorem(m, TLS13_ERROR_FORMA, ALERTA_FORMA,
                "ChangeCipherSpec male formatum");
        }
        redde;
    }
    si (m->legere == NIHIL)
    {
        si (genus == TLS13_CONTENTUM_ALERTA)
        {
            _alerta(m, r + CAPUT, corpus);
        }
        alioquin si (genus != TLS13_CONTENTUM_PACTIO)
        {
            _inopinatum(m);
        }
        alioquin si (corpus == ZEPHYRUM || corpus > DATA_MAXIMA)
        {
            _errorem(m, TLS13_ERROR_MOLES, ALERTA_MOLES,
                "recordum planum longitudine aliena");
        }
        alioquin
        {
            _pactio_addere(m, r + CAPUT, corpus);
        }
        redde;
    }
    si (genus != TLS13_CONTENTUM_DATA)
    {
        _inopinatum(m);
        redde;
    }
    k = tls13_aperire(m->legere, r, n, m->planum,
        TLS13_RECORDUM_MAXIMUM, &interius);
    si (k == TLS13_RECORDUM_MAC)
    {
        _errorem(m, TLS13_ERROR_MAC, ALERTA_MAC, "bad_record_mac");
        redde;
    }
    si (k == TLS13_RECORDUM_MOLES)
    {
        _errorem(m, TLS13_ERROR_MOLES, ALERTA_MOLES, "record_overflow");
        redde;
    }
    si (k < ZEPHYRUM)
    {
        _errorem(m, TLS13_ERROR_FORMA, ALERTA_INOPINATUM,
            "TLSInnerPlaintext sine genere");
        redde;
    }
    si (interius == TLS13_CONTENTUM_PACTIO && k > ZEPHYRUM)
    {
        _pactio_addere(m, m->planum, (i32)k);
    }
    alioquin si (interius == TLS13_CONTENTUM_ALERTA)
    {
        _alerta(m, m->planum, (i32)k);
    }
    alioquin si (   interius   == TLS13_CONTENTUM_DATA
                 && m->stadium == STADIUM_APERTA)
    {
        memcpy(m->data_in, m->planum, (memoriae_index)k);
        m->data_in_locus  = ZEPHYRUM;
        m->data_in_n      = (i32)k;
    }
    alioquin
    {
        _inopinatum(m);
    }
}

/* recorda integra in introitu tractat, donec data priora legantur */
interior vacuum
_procedere (
    Tls13Machina* m)
{
    dum ((m->stadium < STADIUM_CLAUSA) && m->in_n >= CAPUT)
    {
        i32 longitudo  = ((i32)m->in[III] << VIII) | (i32)m->in[IV];
        i32 totum      = CAPUT + longitudo;

        si (totum > TLS13_RECORDUM_MAXIMUM)
        {
            _errorem(m, TLS13_ERROR_MOLES, ALERTA_MOLES,
                "recordum nimis magnum");
            redde;
        }
        si (m->in_n < totum || m->data_in_n > m->data_in_locus)
        {
            redde;
        }
        _recordum(m, m->in, totum);
        memmove(m->in, m->in + totum, m->in_n - totum);
        m->in_n -= totum;
    }
}


/* ================================================== *
 * API
 * ================================================== */

Tls13Machina*
tls13_machina_creare (
    constans Tls13Optiones* optiones,
                   Piscina* piscina)
{
    Tls13Machina* m = (Tls13Machina*)piscina_allocare(piscina,
        magnitudo(Tls13Machina));
    constans i8* fixa = optiones->salutationes_fixae[ZEPHYRUM];

    si (m == NIHIL)
    {
        redde NIHIL;
    }
    memset(m, ZEPHYRUM, magnitudo(*m));
    m->piscina  = piscina;
    m->opt      = *optiones;
    si (optiones->hospes != NIHIL)
    {
        memoriae_index  l = strlen(optiones->hospes);
             character* h;

        h = (character*)piscina_allocare(piscina, l + I);

        si (h == NIHIL)
        {
            redde NIHIL;
        }
        memcpy(h, optiones->hospes, l + I);
        m->opt.hospes = h;
    }
    si (optiones->fors == NIHIL)
    {
        br_prng_seeder semen = br_prng_seeder_system(NIHIL);

        br_hmac_drbg_init(&m->drbg, &br_sha256_vtable, NIHIL, ZEPHYRUM);
        si (semen == NIHIL || !semen(&m->drbg.vtable))
        {
            _errorem(m, TLS13_ERROR_ENTROPIA, ZEPHYRUM,
                "semen systematis defecit");
            redde m;
        }
    }
    _fors(m, m->privata_x25519, XXXII);
    si (fixa != NIHIL)
    {
        /* caput IV, versio II, temere XXXII, longitudo sessionis I */
        i32 n = optiones->salutationes_fixae_n[ZEPHYRUM];

        si (   n > SALUTATIO_MAXIMA || n < XXXIX
            || (i32)fixa[XXXVIII] > XXXII
            || XXXIX + (i32)fixa[XXXVIII] > n)
        {
            redde NIHIL;
        }
        memcpy(m->salutatio, fixa, n);
        m->salutatio_n  = n;
        m->sessio_n     = (i32)fixa[XXXVIII];
        memcpy(m->sessio, fixa + XXXIX, m->sessio_n);
    }
    alioquin
    {
        Tls13SalutatioParametri p;
                             i8 temere[XXXII];
                             i8 publica[XXXII];

        _fors(m, temere, XXXII);
        _fors(m, m->sessio, XXXII);
        m->sessio_n = XXXII;
        si (!_x25519_publica(m->privata_x25519, publica))
        {
            redde NIHIL;
        }
        memset(&p, ZEPHYRUM, magnitudo(p));
        p.hospes          = m->opt.hospes;
        p.temere          = temere;
        p.sessio          = m->sessio;
        p.sessio_n        = XXXII;
        p.grex            = TLS13_GREX_X25519;
        p.clavis_publica  = publica;
        p.clavis_n        = XXXII;
        m->salutatio_n = tls13_salutatio_scribere(&p, m->salutatio,
            SALUTATIO_MAXIMA);
        si (m->salutatio_n == ZEPHYRUM)
        {
            redde NIHIL;
        }
    }
    /* ClientHello primum: legacy_record_version 0x0301 (RFC 8446
     * 5.1) */
    (vacuum)_recordum_planum(m, TLS13_CONTENTUM_PACTIO, I, m->salutatio,
        m->salutatio_n);
    m->stadium = STADIUM_SALUTATIO_MISSA;
    redde m;
}

i32
tls13_machina_status (
    Tls13Machina* m)
{
    i32 status = ZEPHYRUM;

    si (m->ex_n > ZEPHYRUM)
    {
        status |= TLS13_RECORDA_MITTENDA;
    }
    si (m->stadium < STADIUM_CLAUSA)
    {
        status |= TLS13_RECORDA_EXSPECTAT;
    }
    si (m->stadium == STADIUM_APERTA && m->ex_n == ZEPHYRUM)
    {
        status |= TLS13_DATA_MITTI_POSSUNT;
    }
    si (m->data_in_n > m->data_in_locus)
    {
        status |= TLS13_DATA_ADSUNT;
    }
    si (m->stadium == STADIUM_CLAUSA)
    {
        status |= TLS13_CLAUSA;
    }
    redde status;
}

i8*
tls13_recorda_exitus (
    Tls13Machina* m,
             i32* n)
{
    *n = m->ex_n;
    redde m->ex_n > ZEPHYRUM ? m->ex : NIHIL;
}

vacuum
tls13_recorda_exitus_ack (
    Tls13Machina* m,
             i32  n)
{
    si (n > m->ex_n)
    {
        n = m->ex_n;
    }
    memmove(m->ex, m->ex + n, m->ex_n - n);
    m->ex_n -= n;
}

i8*
tls13_recorda_introitus (
    Tls13Machina* m,
             i32* n)
{
    si (m->stadium >= STADIUM_CLAUSA)
    {
        *n = ZEPHYRUM;
        redde NIHIL;
    }
    *n = TLS13_RECORDUM_MAXIMUM - m->in_n;
    redde m->in + m->in_n;
}

vacuum
tls13_recorda_introitus_ack (
    Tls13Machina* m,
             i32  n)
{
    si (n > TLS13_RECORDUM_MAXIMUM - m->in_n)
    {
        n = TLS13_RECORDUM_MAXIMUM - m->in_n;
    }
    m->in_n += n;
    _procedere(m);
}

i8*
tls13_data_exitus (
    Tls13Machina* m,
             i32* n)
{
    si (m->stadium != STADIUM_APERTA)
    {
        *n = ZEPHYRUM;
        redde NIHIL;
    }
    *n = DATA_MAXIMA - m->data_ex_n;
    redde m->data_ex + m->data_ex_n;
}

vacuum
tls13_data_exitus_ack (
    Tls13Machina* m,
             i32  n)
{
    si (n > DATA_MAXIMA - m->data_ex_n)
    {
        n = DATA_MAXIMA - m->data_ex_n;
    }
    m->data_ex_n += n;
}

/* data scripta in unum recordum, si exitus spatium habet (aliter
 * manent, DATA_MITTI_POSSUNT cadit donec exitus exhauriatur) */
vacuum
tls13_data_effundere (
    Tls13Machina* m)
{
    i32 k;

    si (   m->stadium != STADIUM_APERTA || m->data_ex_n == ZEPHYRUM
        || EXITUS_MAXIMUM - m->ex_n < TLS13_RECORDUM_MAXIMUM)
    {
        redde;
    }
    k = tls13_obsignare(m->scribere, TLS13_CONTENTUM_DATA, m->data_ex,
        m->data_ex_n, m->ex + m->ex_n, EXITUS_MAXIMUM - m->ex_n);
    si (k > ZEPHYRUM)
    {
        m->ex_n       += k;
        m->data_ex_n  = ZEPHYRUM;
    }
}

i8*
tls13_data_introitus (
    Tls13Machina* m,
             i32* n)
{
    *n = m->data_in_n - m->data_in_locus;
    redde *n > ZEPHYRUM ? m->data_in + m->data_in_locus : NIHIL;
}

vacuum
tls13_data_introitus_ack (
    Tls13Machina* m,
             i32  n)
{
    m->data_in_locus += n;
    si (m->data_in_locus >= m->data_in_n)
    {
        m->data_in_locus  = ZEPHYRUM;
        m->data_in_n      = ZEPHYRUM;
        _procedere(m);
    }
}

vacuum
tls13_machina_claudere (
    Tls13Machina* m)
{
    si (m->stadium == STADIUM_ERROR || m->clausura_missa)
    {
        redde;
    }
    m->clausura_missa = VERUM;
    _alertam_mittere(m, I, ALERTA_CLAUSURA);
    m->stadium = STADIUM_CLAUSA;
}

Tls13Error
tls13_machina_error (
    Tls13Machina* m)
{
    redde m->error;
}

i32
tls13_machina_versio (
    Tls13Machina* m)
{
    redde m->versio;
}

constans FiduciaCertificatum*
tls13_machina_catena (
    Tls13Machina* m,
             i32* numerus)
{
    *numerus = m->numerus;
    redde m->numerus > ZEPHYRUM ? m->catena : NIHIL;
}
