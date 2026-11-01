/* tessera_es256.c - tessera JWT ES256. Vide include/tessera_es256.h.
 * Nucleus ex nundinae_apple.c (C2) translatus, verbatim in signando. */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "base64.h"
#include "tessera_es256.h"
#include <stdio.h>
#include <string.h>

/* identificatores JSON sine effugio: litterae, numeri, - _ . */
interior b32
_tutum (
    chorda s)
{
    i32 i;

    si (s.mensura == ZEPHYRUM || s.mensura > CXXVIII)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        character k = (character)s.datum[i];

        si (!(   (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')
              || (k >= '0' && k <= '9') || k == '-' || k == '_'
              || k == '.'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

chorda
tessera_es256_facere (
    constans i8* clavis_der,
            i32  mensura,
         chorda  caput_json,
         chorda  asserta_json,
        Piscina* piscina)
{
    br_skey_decoder_context dc;
          br_sha256_context h;
              unsigned char digestum[XXXII];
              unsigned char signatura[LXIV];
                     chorda vacua;
                     chorda initium;
                     size_t n;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!clavis_der || mensura <= ZEPHYRUM || !piscina)
    {
        redde vacua;
    }
    br_skey_decoder_init(&dc);
    br_skey_decoder_push(&dc, clavis_der, (size_t)mensura);
    si (   br_skey_decoder_last_error(&dc)    != ZEPHYRUM
        || br_skey_decoder_key_type(&dc)      != BR_KEYTYPE_EC
        || br_skey_decoder_get_ec(&dc)->curve != BR_EC_secp256r1)
    {
        memset(&dc, ZEPHYRUM, magnitudo(dc));
        redde vacua;
    }
    initium = chorda_concatenare(chorda_concatenare(
        base64url_codificare(caput_json.datum, caput_json.mensura,
        piscina),
        chorda_ex_literis(".", piscina), piscina), base64url_codificare(
        asserta_json.datum, asserta_json.mensura, piscina), piscina);
    br_sha256_init(&h);
    br_sha256_update(&h, initium.datum, (size_t)initium.mensura);
    br_sha256_out(&h, digestum);
    n = br_ecdsa_i31_sign_raw(br_ec_get_default(), &br_sha256_vtable,
        digestum, br_skey_decoder_get_ec(&dc), signatura);
    memset(&dc, ZEPHYRUM, magnitudo(dc));
    si (n != (size_t)LXIV)
    {
        redde vacua;
    }
    redde chorda_concatenare(chorda_concatenare(initium,
        chorda_ex_literis(".", piscina), piscina), base64url_codificare(
        (constans i8*)signatura, LXIV, piscina), piscina);
}

chorda
tessera_apns_facere (
    constans i8* clavis_der,
            i32  mensura,
         chorda  clavis_id,
         chorda  turma,
            s64  nunc_s,
        Piscina* piscina)
{
    character caput[CCLVI];
    character asserta[CCLVI];
       chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (!_tutum(clavis_id) || !_tutum(turma))
    {
        redde vacua;
    }
    sprintf(caput, "{\"alg\":\"ES256\",\"kid\":\"%.*s\"}",
        (int)clavis_id.mensura, (constans character*)clavis_id.datum);
    sprintf(asserta, "{\"iss\":\"%.*s\",\"iat\":%ld}",
        (int)turma.mensura,
        (constans character*)turma.datum, (longus)nunc_s);
    redde tessera_es256_facere(clavis_der, mensura,
        chorda_ex_literis(caput, piscina),
        chorda_ex_literis(asserta, piscina), piscina);
}
