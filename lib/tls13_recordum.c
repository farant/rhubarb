/* tls13_recordum.c - stratum recordorum TLS 1.3 (RFC 8446 §5.2-5.3):
 * vector recordi (nonce) = iv XOR numerus sequentiae (64 bit,
 * dextrorsum); TLSInnerPlaintext = contentum || genus || zeri; AES-GCM
 * (br_aes_ct64_ctr + ghash_ctmul64, tempore constanti - P8) aut
 * ChaCha20-Poly1305 (br_poly1305_ctmul_run + br_chacha20_ct_run). */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "tls13_clavis.h"
#include "tls13_recordum.h"
#include <string.h>

structura Tls13Protectio {
                     i32 cifra;
                     i32 genus;           /* hash */
                      i8 secretum[TLS13_HASH_MAXIMUS];
                      i8 clavis[XXXII];
                     i32 clavis_n;
                      i8 iv[XII];
                     i64 numerus;         /* sequentia */
    br_aes_ct64_ctr_keys aes;
          br_gcm_context gcm;
};

i32
tls13_cifra_genus (
    i32 cifra)
{
    redde cifra == TLS13_AES_256_GCM_SHA384 ? TLS13_SHA384
        : (cifra == TLS13_AES_128_GCM_SHA256
           || cifra == TLS13_CHACHA20_POLY1305_SHA256) ? TLS13_SHA256
        : ZEPHYRUM;
}

interior vacuum
_claves (
    Tls13Protectio* pr)
{
    tls13_claves_traffici(pr->genus, pr->secretum, pr->clavis_n,
        pr->clavis, pr->iv);
    pr->numerus = ZEPHYRUM;
    si (pr->cifra != TLS13_CHACHA20_POLY1305_SHA256)
    {
        br_aes_ct64_ctr_init(&pr->aes, pr->clavis,
            (size_t)pr->clavis_n);
        br_gcm_init(&pr->gcm, &pr->aes.vtable, &br_ghash_ctmul64);
    }
}

Tls13Protectio*
tls13_protectio_creare (
            i32  cifra,
    constans i8* secretum,
        Piscina* p)
{
    Tls13Protectio* pr;
               i32  genus = tls13_cifra_genus(cifra);

    si (genus == ZEPHYRUM)
    {
        redde NIHIL;
    }
    pr = (Tls13Protectio*)piscina_allocare(p, magnitudo(*pr));
    si (pr == NIHIL)
    {
        redde NIHIL;
    }
    memset(pr, ZEPHYRUM, magnitudo(*pr));
    pr->cifra     = cifra;
    pr->genus     = genus;
    pr->clavis_n  = cifra == TLS13_AES_128_GCM_SHA256 ? XVI : XXXII;
    memcpy(pr->secretum, secretum, tls13_hash_mensura(genus));
    _claves(pr);
    redde pr;
}

interior vacuum
_vectorem_formare (
    constans Tls13Protectio* pr,
                         i8* vector_recordi)
{
    i32 k;

    memcpy(vector_recordi, pr->iv, XII);
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        vector_recordi[XI - k] = (i8)(vector_recordi[XI - k]
            ^ (i8)((pr->numerus >> (VIII * k)) & 0xff));
    }
}

/* AEAD in loco: data (n) cum aad (V) -> tag (XVI); cifrare VERUM aut
 * FALSUM; FALSUM reddit si tag non congruit (solum aperiendo) */
interior b32
_aead (
    Tls13Protectio* pr,
               b32  cifrare,
       constans i8* aad,
                i8* data,
               i32  n,
                i8* tag)
{
    i8 vector_recordi[XII];

    _vectorem_formare(pr, vector_recordi);
    si (pr->cifra == TLS13_CHACHA20_POLY1305_SHA256)
    {
         i8 computatus[XVI];
        i32 k;
         i8 differentia = ZEPHYRUM;

        br_poly1305_ctmul_run(pr->clavis, vector_recordi, data,
            (size_t)n, aad,
            V,
            cifrare ? tag : computatus, &br_chacha20_ct_run,
            cifrare ? I : ZEPHYRUM);
        si (cifrare)
        {
            redde VERUM;
        }
        per (k = ZEPHYRUM; k < XVI; k++)
        {
            differentia = (i8)(differentia | (computatus[k] ^ tag[k]));
        }
        redde differentia == ZEPHYRUM;
    }
    br_gcm_reset(&pr->gcm, vector_recordi, XII);
    br_gcm_aad_inject(&pr->gcm, aad, V);
    br_gcm_flip(&pr->gcm);
    br_gcm_run(&pr->gcm, cifrare ? I : ZEPHYRUM, data, (size_t)n);
    si (cifrare)
    {
        br_gcm_get_tag(&pr->gcm, tag);
        redde VERUM;
    }
    redde br_gcm_check_tag(&pr->gcm, tag) != ZEPHYRUM;
}

i32
tls13_obsignare (
    Tls13Protectio* pr,
               i32  genus_contenti,
       constans i8* data,
               i32  n,
                i8* exitus,
               i32  capacitas)
{
    i32 corpus = n + I + XVI;

    si (n > XVI * MXXIV || V + corpus > capacitas)
    {
        redde ZEPHYRUM;
    }
    exitus[ZEPHYRUM]  = (i8)TLS13_CONTENTUM_DATA;
    exitus[I]         = (i8)0x03;
    exitus[II]        = (i8)0x03;
    exitus[III]       = (i8)(corpus >> VIII);
    exitus[IV]        = (i8)(corpus & 0xff);
    memcpy(exitus + V, data, n);
    exitus[V + n] = (i8)genus_contenti;
    (vacuum)_aead(pr, VERUM, exitus, exitus + V, n + I, exitus + V + n
        + I);
    pr->numerus++;
    redde V + corpus;
}

s32
tls13_aperire (Tls13Protectio* pr, constans i8* recordum, i32 n,
    i8* exitus, i32 capacitas, i32* genus_contenti)
{
    i32 corpus;
    i32 k;
     i8 tag[XVI];

    si (n < V)
    {
        redde TLS13_RECORDUM_FORMA;
    }
    corpus = ((i32)(unsigned char)recordum[III] << VIII)
        | (i32)(unsigned char)recordum[IV];
    si (corpus > XVI * MXXIV + CCLVI)
    {
        redde TLS13_RECORDUM_MOLES;
    }
    si (   corpus != n - V || corpus < XVII || corpus - XVI > capacitas
        || (unsigned char)recordum[ZEPHYRUM] != TLS13_CONTENTUM_DATA)
    {
        redde TLS13_RECORDUM_FORMA;
    }
    memcpy(exitus, recordum + V, corpus - XVI);
    memcpy(tag, recordum + V + corpus - XVI, XVI);
    si (!_aead(pr, FALSUM, recordum, exitus, corpus - XVI, tag))
    {
        redde TLS13_RECORDUM_MAC;
    }
    pr->numerus++;
    /* genus = ultimus octetus non-zero; zeros = padding */
    k = corpus - XVI;
    dum (k > ZEPHYRUM && exitus[k - I] == ZEPHYRUM)
    {
        k--;
    }
    si (k == ZEPHYRUM)
    {
        redde TLS13_RECORDUM_FORMA;
    }
    *genus_contenti = (i32)(unsigned char)exitus[k - I];
    si (k - I > XVI * MXXIV)
    {
        redde TLS13_RECORDUM_MOLES;
    }
    redde (s32)(k - I);
}

vacuum
tls13_protectio_renovare (
    Tls13Protectio* pr)
{
    i8 novum[TLS13_HASH_MAXIMUS];

    tls13_secretum_proximum(pr->genus, pr->secretum, novum);
    memcpy(pr->secretum, novum, tls13_hash_mensura(pr->genus));
    _claves(pr);
}
