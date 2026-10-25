/* probatio_nundinae_apple.c - partes purae clientis App Store Connect
 * (testa-ios C2 T1): tessera ES256 (clave P-256 in probatione
 * facta, signatura verificata), renovatio, iteratio, causae (errors[]
 * JSON:API, corpus non-JSON, nulla tessera), paginae, configuratio,
 * clavis (modus 0600). Nulla vocatio retis. */
#include "postulata_posix.h"
/* ANTE latina.h: numeri Romani nomina BearSSL tangerent */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include "json.h"
#include "filum.h"
#include "xar.h"
#include "credo.h"
#include "nundinae_apple.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FIXA "probationes/fixa/nundinae_apple/"

/* clavis P-256 probationis: HMAC-DRBG semine fixo (determinata) ->
 * PKCS#8 DER in piscina; clavis_publica in capsa_publica */
interior i8*
_clavem_facere (
    br_ec_public_key* clavis_publica,
       unsigned char* capsa_publica,
                 i32* mensura,
             Piscina* p)
{
    br_hmac_drbg_context  fors;
       br_ec_private_key  clavis_privata;
           unsigned char  sk_capsa[BR_EC_KBUF_PRIV_MAX_SIZE];
      constans character* semen = "nundinae_apple probatio semen";
                  size_t  n;
                      i8* der;

    br_hmac_drbg_init(&fors, &br_sha256_vtable, semen, strlen(semen));
    si (br_ec_keygen(&fors.vtable, br_ec_get_default(), &clavis_privata,
        sk_capsa,
            BR_EC_secp256r1) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    si (br_ec_compute_pub(br_ec_get_default(), clavis_publica,
        capsa_publica, &clavis_privata)
        == ZEPHYRUM)
    {
        redde NIHIL;
    }
    n    = br_encode_ec_pkcs8_der(NIHIL, &clavis_privata,
        clavis_publica);
    der  = (i8*)piscina_allocare(p, (memoriae_index)n);
    si (   !der || br_encode_ec_pkcs8_der(der, &clavis_privata,
        clavis_publica) != n)
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
       i32 numerus;

    initium    = ZEPHYRUM;
    numerus    = ZEPHYRUM;
    r.datum    = NIHIL;
    r.mensura  = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= t.mensura; i++)
    {
        si (i == t.mensura || t.datum[i] == '.')
        {
            si (numerus == k)
            {
                r.datum    = t.datum + initium;
                r.mensura  = i - initium;
                redde r;
            }
            numerus += I;
            initium = i + I;
        }
    }
    redde r;
}

interior JsonValor*
_fixum (
    constans character* via,
               Piscina* p)
{
          chorda textus;
    JsonResultus r;

    textus  = filum_legere_totum(via, p);
    r       = json_legere(textus, p);
    redde r.successus ? r.radix : NIHIL;
}

interior vacuum
_sectio_tessera (
    CredoContextus* c)
{
            Piscina* p;
   br_ec_public_key  clavis_publica;
      unsigned char  capsa_publica[BR_EC_KBUF_PUB_MAX_SIZE];
                 i8* der;
                i32  mensura;
             chorda  t;
             chorda  textus;
      Base64Fructus  caput;
      Base64Fructus  asserta;
      Base64Fructus  signatura;
       JsonResultus  j;
  br_sha256_context  h;
      unsigned char  digestum[XXXII];
                i32  i;
                b32  aequalia;

    p    = c->piscina;
    der  = _clavem_facere(&clavis_publica, capsa_publica, &mensura, p);
    CREDO_NON_NIHIL(der);
    si (!der)
    {
        redde;
    }
    t = nundinae_apple_tesseram_facere(der, mensura,
        chorda_ex_literis("TEST123", p),
        chorda_ex_literis("emissor-test", p), 1792800000, MCC, p);
    CREDO_GRADUS("tres partes base64url sine '='");
    CREDO_VERUM(t.mensura > ZEPHYRUM);
    aequalia = VERUM;
    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        si (t.datum[i] == '=')
        {
            aequalia = FALSUM;
        }
    }
    CREDO_VERUM(aequalia);
    caput      = base64url_decodificare(_pars(t, ZEPHYRUM), p);
    asserta    = base64url_decodificare(_pars(t, I), p);
    signatura  = base64url_decodificare(_pars(t, II), p);
    CREDO_NON_NIHIL(caput.datum);
    CREDO_NON_NIHIL(asserta.datum);
    CREDO_NON_NIHIL(signatura.datum);
    si (!caput.datum || !asserta.datum || !signatura.datum)
    {
        redde;
    }
    CREDO_GRADUS("caput et asserta");
    CREDO_VERUM(caput.mensura == (i32)strlen(
        "{\"alg\":\"ES256\",\"kid\":\"TEST123\",\"typ\":\"JWT\"}")
        && memcmp(caput.datum,
        "{\"alg\":\"ES256\",\"kid\":\"TEST123\",\"typ\":\"JWT\"}",
        (size_t)caput.mensura) == ZEPHYRUM);
    textus.datum    = asserta.datum;
    textus.mensura  = asserta.mensura;
    j               = json_legere(textus, p);
    CREDO_VERUM(j.successus);
    si (j.successus)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(j.radix, "iss",
            chorda_ex_literis("", p)), "emissor-test");
        CREDO_CHORDA_AEQUALIS_LITERIS(json_capere_chorda(j.radix, "aud",
            chorda_ex_literis("", p)), "appstoreconnect-v1");
        CREDO_VERUM(json_ad_integer(json_objectum_capere(j.radix,
            "iat"))
            == 1792800000);
        CREDO_VERUM(json_ad_integer(json_objectum_capere(j.radix,
            "exp"))
            == 1792801200);
    }
    CREDO_GRADUS("signatura r || s verificatur");
    CREDO_AEQUALIS_I32(signatura.mensura, LXIV);
    br_sha256_init(&h);
    br_sha256_update(&h, t.datum, (size_t)(_pars(t, ZEPHYRUM).mensura
        + I + _pars(t, I).mensura));
    br_sha256_out(&h, digestum);
    CREDO_VERUM(br_ecdsa_i31_vrfy_raw(br_ec_get_default(), digestum,
        XXXII, &clavis_publica, signatura.datum,
        (size_t)signatura.mensura) == I);
    CREDO_GRADUS("clavis truncata: tessera vacua");
    CREDO_AEQUALIS_I32(nundinae_apple_tesseram_facere(der, mensura / II,
        chorda_ex_literis("TEST123", p),
        chorda_ex_literis("emissor-test", p), 1792800000, MCC,
        p).mensura, ZEPHYRUM);
}

interior vacuum
_sectio_renovatio (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_FALSUM(nundinae_apple_renovandum(M, DCCCLXX));
    CREDO_VERUM(nundinae_apple_renovandum(M, DCCCLXXXI));
    CREDO_VERUM(nundinae_apple_renovandum(M, M + X));
}

interior vacuum
_sectio_iteratio (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(nundinae_apple_iterandum(HTTP_GET, CDXXIX, FALSUM));
    CREDO_VERUM(nundinae_apple_iterandum(HTTP_GET, D, FALSUM));
    CREDO_VERUM(nundinae_apple_iterandum(HTTP_GET, DIII, FALSUM));
    CREDO_VERUM(nundinae_apple_iterandum(HTTP_GET, ZEPHYRUM, VERUM));
    CREDO_FALSUM(nundinae_apple_iterandum(HTTP_GET, CDIV, FALSUM));
    CREDO_FALSUM(nundinae_apple_iterandum(HTTP_POST, D, FALSUM));
    CREDO_FALSUM(nundinae_apple_iterandum(HTTP_PATCH, CDXXIX, FALSUM));
    CREDO_FALSUM(nundinae_apple_iterandum(HTTP_DELETE, ZEPHYRUM,
        VERUM));
}

interior vacuum
_sectio_causa (
    CredoContextus* c)
{
     Piscina* p;
      chorda  corpus;
      chorda  causa;
   character  html[DCCC];
         i32  i;

    p = c->piscina;
    CREDO_GRADUS("errors[0] JSON:API");
    corpus  = filum_legere_totum(FIXA "errores.json", p);
    causa   = nundinae_apple_causa_ex(CDIX, corpus, p);
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("409", p));
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis(
        "ENTITY_ERROR.ATTRIBUTE.INVALID", p));
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis(
        "An attribute value is invalid.", p));
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis(
        "previously used", p));
    CREDO_GRADUS("corpus non-JSON: status + octeti primi, non plus");
    strcpy(html, "<html><body>Bad Gateway");
    per (i = (i32)strlen(html); i < DCCLXXX; i++)
    {
        html[i] = 'x';
    }
    html[DCCLXXX] = '\0';
    strcpy(html + CD, "Bearer eyJSECRETUM");
    html[CD + XVIII] = 'x';
    causa = nundinae_apple_causa_ex(DII, chorda_ex_literis(html, p), p);
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("502", p));
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("Bad Gateway", p));
    CREDO_FALSUM(chorda_continet(causa, chorda_ex_literis("SECRETUM",
        p)));
    CREDO_VERUM(causa.mensura <= NUNDINAE_APPLE_CAUSA_CORPUS + XXXII);
    CREDO_GRADUS("causa composita: methodus via, nulla capita");
    causa = nundinae_apple_causam_componere(HTTP_GET,
        chorda_ex_literis("/v1/apps", p),
        nundinae_apple_causa_ex(CDIX, corpus, p), p);
    CREDO_VERUM(causa.mensura > XIII
        && memcmp(causa.datum, "GET /v1/apps: 409", XVII) == ZEPHYRUM);
    CREDO_FALSUM(chorda_continet(causa, chorda_ex_literis("Bearer",
        p)));
}

/* pactum non acceptum: codex Apple extractus (instrumentum -status) */
interior vacuum
_sectio_codex (
    CredoContextus* c)
{
     Piscina* p;

    p = c->piscina;
    CREDO_CHORDA_AEQUALIS_LITERIS(nundinae_apple_codex_ex(
        filum_legere_totum(FIXA "pactum.json", p), p),
        NUNDINAE_APPLE_CODEX_PACTUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(nundinae_apple_codex_ex(
        filum_legere_totum(FIXA "errores.json", p), p),
        "ENTITY_ERROR.ATTRIBUTE.INVALID");
    CREDO_AEQUALIS_I32(nundinae_apple_codex_ex(chorda_ex_literis(
        "<html>502</html>", p), p).mensura, ZEPHYRUM);
}

interior vacuum
_sectio_paginae (
    CredoContextus* c)
{
     Piscina* p;
   JsonValor* r;

    p = c->piscina;
    r = _fixum(FIXA "paginae.json", p);
    CREDO_NON_NIHIL(r);
    CREDO_CHORDA_AEQUALIS_LITERIS(nundinae_apple_proxima(r),
        "https://api.appstoreconnect.apple.com/v1/apps"
        "?cursor=AQ&limit=50");
    r = _fixum(FIXA "ultima.json", p);
    CREDO_NON_NIHIL(r);
    CREDO_AEQUALIS_I32(nundinae_apple_proxima(r).mensura, ZEPHYRUM);
}

interior vacuum
_sectio_configuratio (
    CredoContextus* c)
{
                    Piscina* p;
  NundinaeAppleConfiguratio  cfg;
                     chorda  causa;
         constans character* domus;

    p      = c->piscina;
    domus  = getenv("HOME");
    CREDO_VERUM(nundinae_apple_configurationem_legere(
        FIXA "configuratio.stml", &cfg, &causa, p));
    CREDO_CHORDA_AEQUALIS_LITERIS(cfg.clavis_id, "TEST123");
    CREDO_CHORDA_AEQUALIS_LITERIS(cfg.emissor, "emissor-test");
    CREDO_CHORDA_AEQUALIS_LITERIS(cfg.turma, "Q5WC9FQ3J2");
    CREDO_GRADUS("'~/' = $HOME");
    si (domus)
    {
        CREDO_VERUM(chorda_incipit(cfg.via_clavis,
            chorda_ex_literis(domus, p)));
        CREDO_VERUM(chorda_continet(cfg.via_clavis,
            chorda_ex_literis("/.rhubarb/ios/AuthKey_TEST123.p8", p)));
    }
    CREDO_GRADUS("attributum absens nominatur");
    CREDO_FALSUM(nundinae_apple_configurationem_legere(
        FIXA "manca.stml", &cfg, &causa, p));
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("emissor", p));
}

interior vacuum
_sectio_clavis (
    CredoContextus* c)
{
                     Piscina* p;
            br_ec_public_key  clavis_publica;
               unsigned char  capsa_publica[BR_EC_KBUF_PUB_MAX_SIZE];
                          i8* der;
                         i32  mensura;
                      chorda  textus_clavis;
   NundinaeAppleConfiguratio  cfg;
               NundinaeApple  n;
                      chorda  causa;
                   character  via[DXII];
          constans character* tmp;

    p    = c->piscina;
    der  = _clavem_facere(&clavis_publica, capsa_publica, &mensura, p);
    tmp  = getenv("TMPDIR");
    CREDO_NON_NIHIL(der);
    CREDO_NON_NIHIL(tmp);
    si (!der || !tmp || strlen(tmp) > CD)
    {
        redde;
    }
    sprintf(via, "%s/AuthKey_TEST123.p8", tmp);
    textus_clavis = chorda_concatenare(chorda_ex_literis(
        "-----BEGIN PRIVATE KEY-----\n", p), chorda_concatenare(
        base64_codificare(der, mensura, p), chorda_ex_literis(
        "\n-----END PRIVATE KEY-----\n", p), p), p);
    CREDO_VERUM(filum_scribere(via, textus_clavis));
    CREDO_VERUM(filum_modum_ponere(via, 0644));
    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.clavis_id   = chorda_ex_literis("TEST123", p);
    cfg.emissor     = chorda_ex_literis("emissor-test", p);
    cfg.turma       = chorda_ex_literis("Q5WC9FQ3J2", p);
    cfg.via_clavis  = chorda_ex_literis(via, p);
    CREDO_GRADUS("modus 0644: recusatio nominans 0600");
    CREDO_FALSUM(nundinae_apple_aperire(&n, &cfg, &causa, p));
    CREDO_CHORDA_CONTINET(causa, chorda_ex_literis("0600", p));
    CREDO_GRADUS("modus 0600: clavis DER in memoria");
    CREDO_VERUM(filum_modum_ponere(via, 0600));
    CREDO_VERUM(nundinae_apple_aperire(&n, &cfg, &causa, p));
    CREDO_AEQUALIS_I32(n.clavis_mensura, mensura);
    CREDO_VERUM(n.clavis && memcmp(n.clavis, der, (size_t)mensura)
        == ZEPHYRUM);
    nundinae_apple_claudere(&n);
    CREDO_AEQUALIS_I32(n.clavis_mensura, ZEPHYRUM);
    CREDO_VERUM(n.clavis == NIHIL);
    (vacuum)filum_delere(via);
}

/* TLV DER ad 'd': longitudo capitis (tag + longitudo) et contenti */
interior b32
_elementum_der (
    constans i8* d,
            i32  n,
            i32* caput,
            i32* longitudo)
{
    i32 k;
    i32 i;

    si (n < II)
    {
        redde FALSUM;
    }
    si ((d[I] & 0x80) == ZEPHYRUM)
    {
        *caput      = II;
        *longitudo  = (i32)d[I];
        redde *caput + *longitudo <= n;
    }
    k = (i32)(d[I] & 0x7F);
    si (k < I || k > III || II + k > n)
    {
        redde FALSUM;
    }
    *longitudo = ZEPHYRUM;
    per (i = ZEPHYRUM; i < k; i++)
    {
        *longitudo = (*longitudo << VIII) | (i32)d[II + i];
    }
    *caput = II + k;
    redde *caput + *longitudo <= n;
}

interior b32
_continet_octetos (
    constans i8* d,
            i32  n,
    constans i8* acus,
            i32  m)
{
    i32 i;

    per (i = ZEPHYRUM; i + m <= n; i++)
    {
        si (memcmp(d + i, acus, (size_t)m) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* C2 T2: clavis RSA, petitio PKCS#10 verificata, PEM, deletio */
interior vacuum
_sectio_petitio (
    CredoContextus* c)
{
                   Piscina* p;
    NundinaeAppleClavisRsa  k;
                    chorda  petitio_der;
                    chorda  textus_clavis;
         br_rsa_public_key  publica;
         br_sha256_context  h;
             unsigned char  digestum[XXXII];
             unsigned char  receptum[XXXII];
                       i32  caput;
                       i32  longitudo;
                       i32  caput_informationis;
                       i32  longitudo_informationis;
                       i32  locus;
    hic_manens constans i8  algorithmus_signi[] = { 0x2A, 0x86, 0x48,
        0x86, 0xF7,
        0x0D, 0x01, 0x01, 0x0B };

    p = c->piscina;
    CREDO_GRADUS("clavis RSA MMXLVIII");
    CREDO_VERUM(nundinae_apple_clavem_rsa_generare(&k, p));
    CREDO_AEQUALIS_I32(k.bits, MMXLVIII);
    CREDO_AEQUALIS_I32(k.modulus_mensura, CCLVI);
    CREDO_GRADUS("petitio: SEQUENCE { CRI, algorithmus, signatura }");
    petitio_der = nundinae_apple_petitionem_facere(&k,
        chorda_ex_literis(
        "rhubarb probatio", p), p);
    CREDO_VERUM(petitio_der.mensura > CCC && petitio_der.datum[ZEPHYRUM]
        == 0x30);
    CREDO_VERUM(_elementum_der(petitio_der.datum, petitio_der.mensura,
        &caput, &longitudo)
        && caput + longitudo == petitio_der.mensura);
    locus = caput;
    CREDO_VERUM(_elementum_der(petitio_der.datum + locus,
        petitio_der.mensura - locus, &caput_informationis,
        &longitudo_informationis) && petitio_der.datum[locus] == 0x30);
    CREDO_GRADUS("CRI: versio 0, CN, modulus clavis");
    CREDO_VERUM(petitio_der.datum[locus + caput_informationis] == 0x02
        && petitio_der.datum[locus + caput_informationis + I] == 0x01
        && petitio_der.datum[locus + caput_informationis + II] == 0x00);
    CREDO_VERUM(_continet_octetos(petitio_der.datum + locus,
        caput_informationis + longitudo_informationis,
            (constans i8*)"rhubarb probatio",
        XVI));
    CREDO_VERUM(_continet_octetos(petitio_der.datum + locus,
        caput_informationis + longitudo_informationis, k.modulus,
            k.modulus_mensura));
    CREDO_VERUM(_continet_octetos(petitio_der.datum,
        petitio_der.mensura, algorithmus_signi,
        IX));
    CREDO_GRADUS("signatura PKCS#1 SHA-256 super CRI verificatur");
    br_sha256_init(&h);
    br_sha256_update(&h, petitio_der.datum + locus,
        (size_t)(caput_informationis + longitudo_informationis));
    br_sha256_out(&h, digestum);
    publica.n     = k.modulus;
    publica.nlen  = (size_t)k.modulus_mensura;
    publica.e     = k.exponens;
    publica.elen  = (size_t)k.exponens_mensura;
    CREDO_VERUM(br_rsa_pkcs1_vrfy_get_default()(petitio_der.datum
        + petitio_der.mensura
        - CCLVI, CCLVI, BR_HASH_OID_SHA256, XXXII, &publica, receptum)
        == I);
    CREDO_VERUM(memcmp(digestum, receptum, XXXII) == ZEPHYRUM);
    CREDO_GRADUS("PEM PKCS#1");
    textus_clavis = nundinae_apple_clavem_rsa_textum(&k, p);
    CREDO_VERUM(chorda_incipit(textus_clavis, chorda_ex_literis(
        "-----BEGIN RSA PRIVATE KEY-----\n", p)));
    CREDO_CHORDA_CONTINET(textus_clavis, chorda_ex_literis(
        "\n-----END RSA PRIVATE KEY-----\n", p));
    CREDO_GRADUS("deletio zephyrat");
    nundinae_apple_clavem_rsa_delere(&k);
    CREDO_VERUM(k.primus_p == NIHIL && k.primus_p_mensura == ZEPHYRUM);
    CREDO_VERUM(k.modulus == NIHIL);
}

interior vacuum
_sectio_responsa (
    CredoContextus* c)
{
                    Piscina* p;
                        Xar* certificata;
  NundinaeAppleCertificatum* cert;

    p = c->piscina;
    CREDO_GRADUS("certificata: DISTRIBUTION sola");
    certificata = xar_creare(p,
        (i32)magnitudo(NundinaeAppleCertificatum));
    CREDO_VERUM(nundinae_apple_certificata_legere(_fixum(
        FIXA "certificata.json", p), certificata, p));
    CREDO_AEQUALIS_I32(xar_numerus(certificata), II);
    cert = (NundinaeAppleCertificatum*)xar_obtinere(certificata,
        ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(cert->id, "CERT1");
    CREDO_CHORDA_AEQUALIS_LITERIS(cert->contentum, "MIIBAA==");
    cert = (NundinaeAppleCertificatum*)xar_obtinere(certificata, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(cert->id, "CERT3");
    CREDO_CHORDA_AEQUALIS_LITERIS(cert->titulus, "Xcode Cloud");
    CREDO_GRADUS("signum fasciculi: congruentia exacta");
    CREDO_CHORDA_AEQUALIS_LITERIS(nundinae_apple_signum_invenire(_fixum(
        FIXA "signa.json", p), chorda_ex_literis("earth.golden.pictor",
        p), p), "BID7");
    CREDO_AEQUALIS_I32(nundinae_apple_signum_invenire(_fixum(
        FIXA "signa.json", p), chorda_ex_literis("earth.golden.scriba",
        p), p).mensura, ZEPHYRUM);
    CREDO_GRADUS("SHA-1 hex maiusculae");
    CREDO_CHORDA_AEQUALIS_LITERIS(nundinae_apple_digestum_sha1(
        chorda_ex_literis("abc", p), p),
        "A9993E364706816ABA3E25717850C26C9CD0D89D");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tessera", _sectio_tessera, NIHIL, NIHIL, NIHIL, NIHIL },
    { "renovatio", _sectio_renovatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "iteratio", _sectio_iteratio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "causa", _sectio_causa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "codex", _sectio_codex, NIHIL, NIHIL, NIHIL, NIHIL },
    { "paginae", _sectio_paginae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "configuratio", _sectio_configuratio, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "clavis", _sectio_clavis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "petitio", _sectio_petitio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "responsa", _sectio_responsa, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("nundinae_apple", SECTIONES);
}
