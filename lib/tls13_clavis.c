/* tls13_clavis.c - tabula clavium TLS 1.3 (RFC 8446 §7.1): HKDF super
 * br_hmac (Expand-Label nostrum - br_hkdf unum rivum Expand per
 * contextum habet; tls13-plan P7), transcriptum currens. */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "tls13_clavis.h"
#include <string.h>

structura Tls13Transcriptum {
                       i32 genus;
    br_hash_compat_context status;
};

interior constans br_hash_class*
_classis (
    i32 genus)
{
    redde genus == TLS13_SHA384 ? &br_sha384_vtable
        : genus == TLS13_SHA256 ? &br_sha256_vtable : NIHIL;
}

i32
tls13_hash_mensura (
    i32 genus)
{
    redde genus == TLS13_SHA384 ? XLVIII
        : genus == TLS13_SHA256 ? XXXII : ZEPHYRUM;
}

interior vacuum
_hmac (
            i32  genus,
    constans i8* clavis,
            i32  clavis_n,
    constans i8* a,
            i32  a_n,
    constans i8* b,
            i32  b_n,
    constans i8* c,
            i32  c_n,
             i8* exitus)
{
    br_hmac_key_context clavis_hmac;
        br_hmac_context status_hmac;

    br_hmac_key_init(&clavis_hmac, _classis(genus), clavis,
        (size_t)clavis_n);
    br_hmac_init(&status_hmac, &clavis_hmac, ZEPHYRUM);
    si (a_n)
    {
        br_hmac_update(&status_hmac, a, (size_t)a_n);
    }
    si (b_n)
    {
        br_hmac_update(&status_hmac, b, (size_t)b_n);
    }
    si (c_n)
    {
        br_hmac_update(&status_hmac, c, (size_t)c_n);
    }
    (vacuum)br_hmac_out(&status_hmac, exitus);
}

vacuum
tls13_hkdf_extrahere (
            i32  genus,
    constans i8* sal,
            i32  sal_n,
    constans i8* ikm,
            i32  ikm_n,
             i8* prk)
{
    i8 nulli[TLS13_HASH_MAXIMUS];

    si (sal == NIHIL)
    {
        memset(nulli, ZEPHYRUM, magnitudo(nulli));
        sal    = nulli;
        sal_n  = tls13_hash_mensura(genus);
    }
    _hmac(genus, sal, sal_n, ikm, ikm_n, NIHIL, ZEPHYRUM, NIHIL,
        ZEPHYRUM,
        prk);
}

b32
tls13_hkdf_expandere (
            i32  genus,
    constans i8* prk,
    constans i8* info,
            i32  info_n,
             i8* exitus,
            i32  mensura)
{
    i32 h = tls13_hash_mensura(genus);
     i8 t[TLS13_HASH_MAXIMUS];
    i32 t_n      = ZEPHYRUM;
    i32 scripta  = ZEPHYRUM;
     i8 numerus  = (i8)I;

    si (h == ZEPHYRUM || mensura > CCLV * h)
    {
        redde FALSUM;
    }
    dum (scripta < mensura)
    {
        i32 n;

        _hmac(genus, prk, h, t, t_n, info, info_n, &numerus, I, t);
        t_n  = h;
        n    = mensura - scripta < h ? mensura - scripta : h;
        memcpy(exitus + scripta, t, n);
        scripta += n;
        numerus = (i8)(numerus + I);
    }
    redde VERUM;
}

/* HkdfLabel = uint16 length || opaque label<7..255> ("tls13 " +
 * titulus) || opaque context<0..255> */
b32
tls13_expandere_titulo (
                   i32  genus,
           constans i8* secretum,
    constans character* titulus,
           constans i8* contextus,
                   i32  ctx_n,
                    i8* exitus,
                   i32  mensura)
{
     i8 info[II + I + CCLV + I + CCLV];
    i32 lt  = (i32)strlen(titulus);
    i32 n   = ZEPHYRUM;

    si (VI + lt > CCLV || ctx_n > CCLV)
    {
        redde FALSUM;
    }
    info[n++] = (i8)(mensura >> VIII);
    info[n++] = (i8)(mensura & 0xff);
    info[n++] = (i8)(VI + lt);
    memcpy(info + n, "tls13 ", VI);
    n += VI;
    memcpy(info + n, titulus, lt);
    n          += lt;
    info[n++]  = (i8)ctx_n;
    si (ctx_n)
    {
        memcpy(info + n, contextus, ctx_n);
        n += ctx_n;
    }
    redde tls13_hkdf_expandere(genus, secretum, info, n, exitus,
        mensura);
}

vacuum
tls13_secretum_derivare (
                   i32  genus,
           constans i8* secretum,
    constans character* titulus,
           constans i8* hash_transcripti,
                    i8* exitus)
{
    i32 h = tls13_hash_mensura(genus);

    (vacuum)tls13_expandere_titulo(genus, secretum, titulus,
        hash_transcripti, h, exitus, h);
}

vacuum
tls13_claves_traffici (
            i32  genus,
    constans i8* secretum,
            i32  clavis_n,
             i8* clavis,
             i8* iv)
{
    (vacuum)tls13_expandere_titulo(genus, secretum, "key", NIHIL,
        ZEPHYRUM, clavis, clavis_n);
    (vacuum)tls13_expandere_titulo(genus, secretum, "iv", NIHIL,
        ZEPHYRUM, iv, XII);
}

vacuum
tls13_finis_calculare (
            i32  genus,
    constans i8* secretum_base,
    constans i8* hash_transcripti,
             i8* verificatio)
{
    i32 h = tls13_hash_mensura(genus);
     i8 clavis[TLS13_HASH_MAXIMUS];

    (vacuum)tls13_expandere_titulo(genus, secretum_base, "finished",
        NIHIL,
        ZEPHYRUM, clavis, h);
    _hmac(genus, clavis, h, hash_transcripti, h, NIHIL, ZEPHYRUM, NIHIL,
        ZEPHYRUM, verificatio);
}

vacuum
tls13_secretum_proximum (
            i32  genus,
    constans i8* secretum,
             i8* exitus)
{
    i32 h = tls13_hash_mensura(genus);

    (vacuum)tls13_expandere_titulo(genus, secretum, "traffic upd",
        NIHIL,
        ZEPHYRUM, exitus, h);
}

Tls13Transcriptum*
tls13_transcriptum_creare (
        i32  genus,
    Piscina* p)
{
    Tls13Transcriptum* t;

    si (_classis(genus) == NIHIL)
    {
        redde NIHIL;
    }
    t = (Tls13Transcriptum*)piscina_allocare(p, magnitudo(*t));
    si (t != NIHIL)
    {
        memset(t, ZEPHYRUM, magnitudo(*t));
        t->genus = genus;
        _classis(genus)->init(&t->status.vtable);
    }
    redde t;
}

vacuum
tls13_transcriptum_addere (
    Tls13Transcriptum* t,
          constans i8* o,
                  i32  n)
{
    _classis(t->genus)->update(&t->status.vtable, o, (size_t)n);
}

vacuum
tls13_transcriptum_digerere (
    constans Tls13Transcriptum* t,
                            i8* hash)
{
    /* out() statum non mutat (bearssl_hash.h) */
    _classis(t->genus)->out(&t->status.vtable, hash);
}

/* RFC 8446 4.4.1: Transcript-Hash(CH1, HRR, ...) = Hash(message_hash ||
 * 00 00 Hash.length || Hash(CH1) || HRR || ...). Vocandum post CH1
 * additum, ANTE HRR additum. */
vacuum
tls13_transcriptum_hrr (
    Tls13Transcriptum* t)
{
     i8 h1[TLS13_HASH_MAXIMUS];
     i8 caput[IV];
    i32 h = tls13_hash_mensura(t->genus);

    tls13_transcriptum_digerere(t, h1);
    _classis(t->genus)->init(&t->status.vtable);
    caput[ZEPHYRUM]  = (i8)0xfe;
    caput[I]         = ZEPHYRUM;
    caput[II]        = ZEPHYRUM;
    caput[III]       = (i8)h;
    tls13_transcriptum_addere(t, caput, IV);
    tls13_transcriptum_addere(t, h1, h);
}
