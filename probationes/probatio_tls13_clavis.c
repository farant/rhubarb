/* probatio_tls13_clavis.c - tabula clavium TLS 1.3 (tls13-spec II.1):
 * RFC 5869 A.1 (HKDF), RFC 8448 §3 (omnes derivationes 'tls13 ...'),
 * lector RFC 8448 ipse. Oraculum: textus RFC immutatus. */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "tls13_clavis.h"
#include "rfc8448.h"
#include <stdio.h>
#include <string.h>

#define VIA_RFC "probationes/fixa/tls13/rfc8448.txt"

interior vacuum
_hex (
    constans character* h,
                    i8* o,
                   i32  n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        unsigned int v = ZEPHYRUM;

        (vacuum)sscanf(h + k * II, "%2x", &v);
        o[k] = (i8)v;
    }
}

interior vacuum
_suitam_parare (CredoContextus* c)
{
    Rfc8448* r = (Rfc8448*)piscina_allocare(c->piscina,
        magnitudo(Rfc8448));

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(rfc8448_legere(r, VIA_RFC, c->piscina));
    c->datum = r;
}

/* lector: campi noti adsunt cum mensura declarata */
interior vacuum
_sectio_lector (CredoContextus* c)
{
          constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* f;

    f = rfc8448_quaerere(r, III, 'c', "construct a ClientHello",
        ZEPHYRUM,
        "ClientHello");
    CREDO_NECESSE_NON_NIHIL(f);
    CREDO_AEQUALIS_I32(f->octeti.mensura, CXCVI);
    f = rfc8448_quaerere(r, III, 's',
        "derive secret \"tls13 c hs traffic\"",
        ZEPHYRUM, "expanded");
    CREDO_NECESSE_NON_NIHIL(f);
    CREDO_AEQUALIS_I32(f->octeti.mensura, XXXII);
    CREDO_AEQUALIS_I32((i32)(unsigned char)f->octeti.datum[ZEPHYRUM],
        0xb3);
    f = rfc8448_quaerere(r, V, 's', "construct a ServerHello", ZEPHYRUM,
        "ServerHello");      /* HRR */
    CREDO_NECESSE_NON_NIHIL(f);
}

/* RFC 5869 A.1 */
interior vacuum
_sectio_rfc5869 (CredoContextus* c)
{
    i8 ikm[XXII], sal[XIII], info[X], prk[XXXII], okm[XLII];
    i8 prk_e[XXXII], okm_e[XLII];

    (vacuum)c;
    memset(ikm, 0x0b, XXII);
    _hex("000102030405060708090a0b0c", sal, XIII);
    _hex("f0f1f2f3f4f5f6f7f8f9", info, X);
    _hex("077709362c2e32df0ddc3f0dc47bba63"
         "90b6c73bb50f9c3122ec844ad7c2b3e5", prk_e, XXXII);
    _hex("3cb25f25faacd57a90434f64d0362f2a"
         "2d2d0a90cf1a5a4c5db02d56ecc4c5bf"
         "34007208d5b887185865", okm_e, XLII);
    tls13_hkdf_extrahere(TLS13_SHA256, sal, XIII, ikm, XXII, prk);
    CREDO_VERUM(memcmp(prk, prk_e, XXXII) == ZEPHYRUM);
    CREDO_NECESSE(tls13_hkdf_expandere(TLS13_SHA256, prk, info, X, okm,
        XLII));
    CREDO_VERUM(memcmp(okm, okm_e, XLII) == ZEPHYRUM);
}

/* omnis 'derive secret ...' §3 (cliens et servus): PRK + hash -> info
 * (titulus encodatus) et expanded */
interior vacuum
_derivatio (CredoContextus* c, character latus,
    constans character* actio,
    constans character* titulus, i32 ordo)
{
          constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* prk = rfc8448_quaerere(r, III, latus, actio,
        ordo, "PRK");
    constans Rfc8448Campus* hash = rfc8448_quaerere(r, III, latus,
        actio,
        ordo, "hash");
    constans Rfc8448Campus* exp = rfc8448_quaerere(r, III, latus, actio,
        ordo, "expanded");
    i8 o[XXXII];

    CREDO_NECESSE(prk && hash && exp);
    CREDO_NECESSE(tls13_expandere_titulo(TLS13_SHA256,
        prk->octeti.datum,
        titulus, hash->octeti.datum, hash->octeti.mensura, o, XXXII));
    CREDO_VERUM(memcmp(o, exp->octeti.datum, XXXII) == ZEPHYRUM);
}

interior vacuum
_sectio_derivationes (
    CredoContextus* c)
{
    _derivatio(c, 's', "derive secret for handshake \"tls13 derived\"",
        "derived", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 c hs traffic\"",
        "c hs traffic", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 s hs traffic\"",
        "s hs traffic", ZEPHYRUM);
    _derivatio(c, 's', "derive secret for master \"tls13 derived\"",
        "derived", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 c ap traffic\"",
        "c ap traffic", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 s ap traffic\"",
        "s ap traffic", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 exp master\"",
        "exp master", ZEPHYRUM);
}

/* extractiones: early (sal nulli), handshake, master */
interior vacuum
_sectio_extractiones (CredoContextus* c)
{
          constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* sec;
    constans Rfc8448Campus* sal;
    constans Rfc8448Campus* ikm;
    i8 o[XXXII], nulli[XXXII];

    memset(nulli, ZEPHYRUM, XXXII);
    sec = rfc8448_quaerere(r, III, 's', "extract secret \"early\"",
        ZEPHYRUM, "secret");
    CREDO_NECESSE_NON_NIHIL(sec);
    tls13_hkdf_extrahere(TLS13_SHA256, NIHIL, ZEPHYRUM, nulli, XXXII,
        o);
    CREDO_VERUM(memcmp(o, sec->octeti.datum, XXXII) == ZEPHYRUM);
    sal = rfc8448_quaerere(r, III, 's', "extract secret \"handshake\"",
        ZEPHYRUM, "salt");
    ikm = rfc8448_quaerere(r, III, 's', "extract secret \"handshake\"",
        ZEPHYRUM, "IKM");
    sec = rfc8448_quaerere(r, III, 's', "extract secret \"handshake\"",
        ZEPHYRUM, "secret");
    CREDO_NECESSE(sal && ikm && sec);
    tls13_hkdf_extrahere(TLS13_SHA256, sal->octeti.datum, XXXII,
        ikm->octeti.datum, XXXII, o);
    CREDO_VERUM(memcmp(o, sec->octeti.datum, XXXII) == ZEPHYRUM);
}

/* claves traffici: 'derive write traffic keys for handshake data' */
interior vacuum
_sectio_claves (CredoContextus* c)
{
          constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* prk = rfc8448_quaerere(r, III, 's',
        "derive write traffic keys for handshake data", ZEPHYRUM,
        "PRK");
    constans Rfc8448Campus* k = rfc8448_quaerere(r, III, 's',
        "derive write traffic keys for handshake data", ZEPHYRUM,
        "key expanded");
    constans Rfc8448Campus* iv = rfc8448_quaerere(r, III, 's',
        "derive write traffic keys for handshake data", ZEPHYRUM,
        "iv expanded");
    i8 clavis[XVI], vector[XII];

    CREDO_NECESSE(prk && k && iv);
    tls13_claves_traffici(TLS13_SHA256, prk->octeti.datum, XVI, clavis,
        vector);
    CREDO_VERUM(memcmp(clavis, k->octeti.datum, XVI) == ZEPHYRUM);
    CREDO_VERUM(memcmp(vector, iv->octeti.datum, XII) == ZEPHYRUM);
}

/* finished: 'calculate finished "tls13 finished"' (servus) dat PRK
 * (secretum s hs), 'hash (0 octets)' (contextus Expand vacuus),
 * 'expanded' (finished_key) et 'finished' (verify_data) - sed NON hash
 * transcripti: eum ex nuntiis CH SH EE Certificate CV RFC ipsius
 * computamus (sic transcriptum quoque per quinque nuntios probatur) */
interior vacuum
_sectio_finis (CredoContextus* c)
{
      constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans character* actio =
        "calculate finished \"tls13 finished\"";
    constans Rfc8448Campus* prk = rfc8448_quaerere(r, III, 's', actio,
        ZEPHYRUM, "PRK");
    constans Rfc8448Campus* exp = rfc8448_quaerere(r, III, 's', actio,
        ZEPHYRUM, "expanded");
    constans Rfc8448Campus* fin = rfc8448_quaerere(r, III, 's', actio,
        ZEPHYRUM, "finished");
    constans character* nuntii[V][III] = {
        { "c", "construct a ClientHello", "ClientHello" },
        { "s", "construct a ServerHello", "ServerHello" },
        { "s", "construct an EncryptedExtensions",
            "EncryptedExtensions" },
        { "s", "construct a Certificate", "Certificate" },
        { "s", "construct a CertificateVerify", "CertificateVerify" } };
    Tls13Transcriptum* t = tls13_transcriptum_creare(TLS13_SHA256,
        c->piscina);
    i8  o[XXXII], hash_transcripti[XXXII];
    i32 k;

    CREDO_NECESSE(prk && exp && fin && t);
    /* finished_key = Expand-Label(base, "finished", "", Hash.length) */
    CREDO_NECESSE(tls13_expandere_titulo(TLS13_SHA256,
        prk->octeti.datum,
        "finished", NIHIL, ZEPHYRUM, o, XXXII));
    CREDO_VERUM(memcmp(o, exp->octeti.datum, XXXII) == ZEPHYRUM);
    per (k = ZEPHYRUM; k < V; k++)
    {
        constans Rfc8448Campus* n = rfc8448_quaerere(r, III,
            nuntii[k][ZEPHYRUM][ZEPHYRUM], nuntii[k][I], ZEPHYRUM,
            nuntii[k][II]);

        CREDO_NECESSE_NON_NIHIL(n);
        tls13_transcriptum_addere(t, n->octeti.datum,
            n->octeti.mensura);
    }
    tls13_transcriptum_digerere(t, hash_transcripti);
    tls13_finis_calculare(TLS13_SHA256, prk->octeti.datum,
        hash_transcripti, o);
    CREDO_VERUM(memcmp(o, fin->octeti.datum, XXXII) == ZEPHYRUM);
}

/* transcriptum: hash ClientHello + ServerHello == hash
 * 'c hs traffic' */
interior vacuum
_sectio_transcriptum (CredoContextus* c)
{
          constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* ch = rfc8448_quaerere(r, III, 'c',
        "construct a ClientHello", ZEPHYRUM, "ClientHello");
    constans Rfc8448Campus* sh = rfc8448_quaerere(r, III, 's',
        "construct a ServerHello", ZEPHYRUM, "ServerHello");
    constans Rfc8448Campus* h = rfc8448_quaerere(r, III, 's',
        "derive secret \"tls13 c hs traffic\"", ZEPHYRUM, "hash");
    Tls13Transcriptum* t = tls13_transcriptum_creare(TLS13_SHA256,
        c->piscina);
    i8 o[XXXII];

    CREDO_NECESSE(ch && sh && h && t);
    tls13_transcriptum_addere(t, ch->octeti.datum, ch->octeti.mensura);
    tls13_transcriptum_addere(t, sh->octeti.datum, sh->octeti.mensura);
    tls13_transcriptum_digerere(t, o);
    CREDO_VERUM(memcmp(o, h->octeti.datum, XXXII) == ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "lector", _sectio_lector, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rfc5869", _sectio_rfc5869, NIHIL, NIHIL, NIHIL, NIHIL },
    { "derivationes", _sectio_derivationes, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "extractiones", _sectio_extractiones, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "claves", _sectio_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "finis", _sectio_finis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "transcriptum", _sectio_transcriptum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls13_clavis", SECTIONES,
        _suitam_parare, NIHIL);
}
