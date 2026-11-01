/* probatio_tessera_es256.c - tessera ES256 communis (http2-plan T10):
 * nucleus (caput et asserta data), tessera APNs (kid; iss + iat), et
 * nundinae_apple_tesseram_facere byte pro byte idem ac nucleus cum JSON
 * eius (signatura RFC 6979 determinata). Clavis P-256 in probatione ex
 * semine facta - nulla clavis privata in arbore. */
#include "postulata_posix.h"
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include "tessera_es256.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include "nundinae_apple.h"
#include "credo.h"
#include <string.h>

hic_manens Piscina* piscina;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
}

/* PKCS#8 DER clavis P-256 ex semine fixo; publica in capsa */
interior i8*
_clavem_facere (
    br_ec_public_key* publica,
       unsigned char* capsa_publica,
                 i32* mensura)
{
    br_hmac_drbg_context  fors;
       br_ec_private_key  privata;
           unsigned char  capsa_privata[BR_EC_KBUF_PRIV_MAX_SIZE];
      constans character* semen = "tessera_es256 probatio semen";
                  size_t  n;
                      i8* der;

    br_hmac_drbg_init(&fors, &br_sha256_vtable, semen, strlen(semen));
    si (   br_ec_keygen(&fors.vtable, br_ec_get_default(), &privata,
            capsa_privata, BR_EC_secp256r1) == ZEPHYRUM
        || br_ec_compute_pub(br_ec_get_default(), publica,
        capsa_publica,
               &privata) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    n    = br_encode_ec_pkcs8_der(NIHIL, &privata, publica);
    der  = (i8*)piscina_allocare(piscina, (memoriae_index)n);
    si (!der || br_encode_ec_pkcs8_der(der, &privata, publica) != n)
    {
        redde NIHIL;
    }
    *mensura = (i32)n;
    redde der;
}

/* pars k (0..II) tesserae "a.b.c" */
interior chorda
_pars (
    chorda t,
       i32 k)
{
    chorda r;
       i32 i;
       i32 initium;
       i32 pars;

    initium  = ZEPHYRUM;
    pars     = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= t.mensura; i++)
    {
        si (i == t.mensura || t.datum[i] == '.')
        {
            si (pars == k)
            {
                r.datum    = t.datum + initium;
                r.mensura  = i - initium;
                redde r;
            }
            pars++;
            initium = i + I;
        }
    }
    r.datum    = NIHIL;
    r.mensura  = ZEPHYRUM;
    redde r;
}

interior b32
pars_est (
                 chorda  t,
                    i32  k,
     constans character* json)
{
    Base64Fructus d = base64url_decodificare(_pars(t, k), piscina);

    redde d.datum && d.mensura == (i32)strlen(json)
        && memcmp(d.datum, json, (size_t)d.mensura) == ZEPHYRUM;
}

interior b32
signatura_valet (
                       chorda  t,
    constans br_ec_public_key* publica)
{
    br_sha256_context h;
        unsigned char digestum[XXXII];
        Base64Fructus s;

    s = base64url_decodificare(_pars(t, II), piscina);
    si (!s.datum || s.mensura != LXIV)
    {
        redde FALSUM;
    }
    br_sha256_init(&h);
    br_sha256_update(&h, t.datum, (size_t)(_pars(t, ZEPHYRUM).mensura
        + I
        + _pars(t, I).mensura));
    br_sha256_out(&h, digestum);
    redde br_ecdsa_i31_vrfy_raw(br_ec_get_default(), digestum, XXXII,
        publica, s.datum, (size_t)s.mensura) == I;
}

interior vacuum
_sectio_nucleus (
    CredoContextus* c)
{
    br_ec_public_key  publica;
       unsigned char  capsa[BR_EC_KBUF_PUB_MAX_SIZE];
                  i8* der;
                 i32  n;
              chorda  t;

    (vacuum)c;
    der = _clavem_facere(&publica, capsa, &n);
    CREDO_NECESSE_NON_NIHIL(der);
    CREDO_GRADUS("caput et asserta ut data, signatura valet");
    t = tessera_es256_facere(der, n,
        chorda_ex_literis("{\"alg\":\"ES256\",\"kid\":\"K1\"}",
        piscina),
        chorda_ex_literis("{\"iss\":\"T1\",\"iat\":1}", piscina),
        piscina);
    CREDO_VERUM(t.mensura > ZEPHYRUM);
    CREDO_VERUM(pars_est(t, ZEPHYRUM,
        "{\"alg\":\"ES256\",\"kid\":\"K1\"}"));
    CREDO_VERUM(pars_est(t, I, "{\"iss\":\"T1\",\"iat\":1}"));
    CREDO_VERUM(signatura_valet(t, &publica));

    CREDO_GRADUS("clavis mala, truncata, nulla: vacua");
    {
        i8 strepitus[LXIV];

        memset(strepitus, 0x5a, LXIV);
        CREDO_AEQUALIS_I32(tessera_es256_facere(strepitus, LXIV,
            chorda_ex_literis("{}", piscina),
            chorda_ex_literis("{}", piscina), piscina).mensura,
            ZEPHYRUM);
    }
    CREDO_AEQUALIS_I32(tessera_es256_facere(der, n / II,
        chorda_ex_literis("{}", piscina), chorda_ex_literis("{}",
        piscina),
        piscina).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(tessera_es256_facere(NIHIL, ZEPHYRUM,
        chorda_ex_literis("{}", piscina), chorda_ex_literis("{}",
        piscina),
        piscina).mensura, ZEPHYRUM);
}

interior vacuum
_sectio_apns (
    CredoContextus* c)
{
    br_ec_public_key  publica;
       unsigned char  capsa[BR_EC_KBUF_PUB_MAX_SIZE];
                  i8* der;
                 i32  n;
              chorda  t;

    (vacuum)c;
    der = _clavem_facere(&publica, capsa, &n);
    CREDO_NECESSE_NON_NIHIL(der);
    CREDO_GRADUS("kid; iss et iat");
    t = tessera_apns_facere(der, n, chorda_ex_literis("PROBA12345",
        piscina), chorda_ex_literis("TURMA12345", piscina), 1792800000,
        piscina);
    CREDO_VERUM(t.mensura > ZEPHYRUM);
    CREDO_VERUM(pars_est(t, ZEPHYRUM,
        "{\"alg\":\"ES256\",\"kid\":\"PROBA12345\"}"));
    CREDO_VERUM(pars_est(t, I,
        "{\"iss\":\"TURMA12345\",\"iat\":1792800000}"));
    CREDO_VERUM(signatura_valet(t, &publica));

    CREDO_GRADUS("identificatores non tuti: vacua");
    CREDO_AEQUALIS_I32(tessera_apns_facere(der, n, chorda_ex_literis(
        "A\"B", piscina), chorda_ex_literis("TURMA12345", piscina),
        1792800000, piscina).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(tessera_apns_facere(der, n, chorda_ex_literis(
        "PROBA12345", piscina), chorda_ex_literis("", piscina),
        1792800000,
        piscina).mensura, ZEPHYRUM);
}

interior vacuum
_sectio_nundinae (
    CredoContextus* c)
{
    br_ec_public_key  publica;
       unsigned char  capsa[BR_EC_KBUF_PUB_MAX_SIZE];
                  i8* der;
                 i32  n;
              chorda  vetus;
              chorda  nova;

    (vacuum)c;
    der = _clavem_facere(&publica, capsa, &n);
    CREDO_NECESSE_NON_NIHIL(der);
    CREDO_GRADUS("nundinae_apple idem ac nucleus cum JSON eius");
    vetus = nundinae_apple_tesseram_facere(der, n,
        chorda_ex_literis("TEST123", piscina),
        chorda_ex_literis("emissor-test", piscina), 1792800000, MCC,
        piscina);
    nova = tessera_es256_facere(der, n,
        chorda_ex_literis("{\"alg\":\"ES256\",\"kid\":\"TEST123\","
            "\"typ\":\"JWT\"}", piscina),
        chorda_ex_literis("{\"iss\":\"emissor-test\",\"iat\":1792800000,"
            "\"exp\":1792801200,\"aud\":\"appstoreconnect-v1\"}",
            piscina),
        piscina);
    CREDO_VERUM(vetus.mensura > ZEPHYRUM);
    CREDO_VERUM(chorda_aequalis(vetus, nova));
    CREDO_VERUM(signatura_valet(vetus, &publica));
}

/* T11: clavis ex plagula PEM PKCS#8 (modus 0600 necessarius) */
interior vacuum
_sectio_clavis (
    CredoContextus* c)
{
    br_ec_public_key  publica;
       unsigned char  capsa[BR_EC_KBUF_PUB_MAX_SIZE];
                  i8* der;
                  i8* lecta;
                 i32  n;
                 i32  m;
              chorda  causa;
           character  via[DXII];
  constans character* tmp;

    (vacuum)c;
    der = _clavem_facere(&publica, capsa, &n);
    tmp = getenv("TMPDIR");
    CREDO_NECESSE(der && tmp && strlen(tmp) < CD);
    sprintf(via, "%s/AuthKey_PROBA_ES256.p8", tmp);
    CREDO_VERUM(filum_scribere(via,
        chorda_concatenare(chorda_ex_literis(
        "-----BEGIN PRIVATE KEY-----\n", piscina), chorda_concatenare(
        base64_codificare(der, n, piscina), chorda_ex_literis(
        "\n-----END PRIVATE KEY-----\n", piscina), piscina), piscina)));

    CREDO_GRADUS("modus 0644: recusatio nominans 0600");
    CREDO_VERUM(filum_modum_ponere(via, 0644));
    CREDO_FALSUM(tessera_clavem_legere(via, &lecta, &m, &causa,
        piscina));
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("0600", piscina));

    CREDO_GRADUS("modus 0600: DER idem");
    CREDO_VERUM(filum_modum_ponere(via, 0600));
    CREDO_VERUM(tessera_clavem_legere(via, &lecta, &m, &causa,
        piscina));
    CREDO_AEQUALIS_I32(m, n);
    CREDO_VERUM(memcmp(lecta, der, (size_t)n) == ZEPHYRUM);

    CREDO_GRADUS("non PEM, absens: recusatio");
    CREDO_VERUM(filum_scribere(via, chorda_ex_literis("nihil\n",
        piscina)));
    CREDO_VERUM(filum_modum_ponere(via, 0600));
    CREDO_FALSUM(tessera_clavem_legere(via, &lecta, &m, &causa,
        piscina));
    (vacuum)filum_delere(via);
    CREDO_FALSUM(tessera_clavem_legere(via, &lecta, &m, &causa,
        piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "nucleus", _sectio_nucleus, _parare, _purgare, NIHIL, NIHIL },
    { "APNs", _sectio_apns, _parare, _purgare, NIHIL, NIHIL },
    { "nundinae_apple idem", _sectio_nundinae, _parare, _purgare, NIHIL,
        NIHIL },
    { "clavis ex plagula (T11)", _sectio_clavis, _parare, _purgare,
        NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("tessera_es256", SECTIONES);
}
