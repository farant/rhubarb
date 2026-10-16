/* probatio_bearssl.c - BearSSL vendicatum (vendor/bearssl_amalgama.c)
 * nectitur et currit: SHA-256 FIPS 180-2 "abc", X25519 RFC 7748 §6.1
 * (clavis publica Alicae), entropia systematis (pactio sine ea
 * recusatur - tls-bearssl-spec II.6). KAT plenus upstream: porta
 * 'bearssl' (tools/bearssl_probare.sh). */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_hex (
    constans character* hex,
         unsigned char* exitus,
                   i32  mensura)
{
    i32 i;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        unsigned int octetus = ZEPHYRUM;

        (vacuum)sscanf(hex + i * II, "%2x", &octetus);
        exitus[i] = (unsigned char)octetus;
    }
}

interior vacuum
_sectio_sha256 (
    CredoContextus* c)
{
    br_sha256_context ctx;
        unsigned char h[XXXII];
        unsigned char exspectatum[XXXII];

    (vacuum)c;
    _hex("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad",
        exspectatum, XXXII);
    br_sha256_init(&ctx);
    br_sha256_update(&ctx, "abc", III);
    br_sha256_out(&ctx, h);
    CREDO_VERUM(memcmp(h, exspectatum, XXXII) == ZEPHYRUM);
}

interior vacuum
_sectio_x25519 (
    CredoContextus* c)
{
    constans br_ec_impl* ec = br_ec_get_default();
          unsigned char  privata[XXXII];
          unsigned char  publica[XXXII];
          unsigned char  exspectata[XXXII];
                 size_t  n;
                    i32  k;

    (vacuum)c;
    _hex("77076d0a7318a57d3c16c17251b26645df4c2f87ebc0992ab177fba51db92c2a",
        privata, XXXII);
    _hex("8520f0098930a754748b7ddcb43ef75a0dbf3a0d26381af4eba4a98eaa9b4e6a",
        exspectata, XXXII);
    CREDO_NECESSE_NON_NIHIL(ec);
    /* RFC 7748 scalarem little-endian dat; API BearSSL multiplicatores
     * big-endian accipit (bearssl_ec.h; test_crypto.c revbytes) -
     * coordinata u ipsa little-endian manet */
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        unsigned char t = privata[k];

        privata[k]         = privata[XXXI - k];
        privata[XXXI - k]  = t;
    }
    n = ec->mulgen(publica, privata, XXXII, BR_EC_curve25519);
    CREDO_AEQUALIS_I32((i32)n, XXXII);
    CREDO_VERUM(memcmp(publica, exspectata, XXXII) == ZEPHYRUM);
}

interior vacuum
_sectio_semen (
    CredoContextus* c)
{
       constans character* titulus  = NIHIL;
           br_prng_seeder  semen    = br_prng_seeder_system(&titulus);
     br_hmac_drbg_context  generator;

    (vacuum)c;
    CREDO_NECESSE(semen != NIHIL);
    CREDO_VERUM(titulus != NIHIL && strlen(titulus) > ZEPHYRUM);
    br_hmac_drbg_init(&generator, &br_sha256_vtable, "probatio", VIII);
    CREDO_VERUM(semen(&generator.vtable) != ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "sha256", _sectio_sha256, NIHIL, NIHIL, NIHIL, NIHIL },
    { "x25519", _sectio_x25519, NIHIL, NIHIL, NIHIL, NIHIL },
    { "entropia", _sectio_semen, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("bearssl", SECTIONES);
}
