#include "friatio.h"
#include <string.h>

/* ==================================================
 * FNV-1a (Fowler-Noll-Vo)
 * ================================================== */

/* FNV-1a constants (32-bit) */
#define FNV_OFFSET_BASIS 2166136261U
#define FNV_PRIME        16777619U

i32
friatio_fnv1a(
    chorda clavis)
{
    i32 friatum;
    i32 i;

    friatum = (i32)FNV_OFFSET_BASIS;

    per (i = ZEPHYRUM; i < clavis.mensura; i++)
    {
        friatum ^= (i8)clavis.datum[i];
        friatum *= (i32)FNV_PRIME;
    }

    redde friatum;
}

i32
friatio_fnv1a_literis(
    constans character* literae,
    i32                 mensura)
{
    i32 friatum;
    i32 i;

    friatum = (i32)FNV_OFFSET_BASIS;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        friatum ^= (i8)literae[i];
        friatum *= (i32)FNV_PRIME;
    }

    redde friatum;
}

/* ==================================================
 * DJB2 (Dan Bernstein)
 * ================================================== */

#define DJB2_INIT 5381U

i32
friatio_djb2(
    chorda clavis)
{
    i32 friatum;
    i32 i;

    friatum = (i32)DJB2_INIT;

    per (i = ZEPHYRUM; i < clavis.mensura; i++)
    {
        /* friatum = friatum * 33 + c */
        friatum = ((friatum << V) + friatum) + (i8)clavis.datum[i];
    }

    redde friatum;
}

i32
friatio_djb2_literis(
    constans character* literae,
    i32                 mensura)
{
    i32 friatum;
    i32 i;

    friatum = (i32)DJB2_INIT;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        friatum = ((friatum << V) + friatum) + (i8)literae[i];
    }

    redde friatum;
}

/* ==================================================
 * SHA-1 Implementation
 *
 * Based on RFC 3174
 * Produces 160-bit (20-byte) message digest
 * ================================================== */

/* Left rotate 32-bit value */
#define SHA1_ROL(value, bits) (((value) << (bits)) | ((value) >> (XXXII - (bits))))

/* SHA-1 round functions */
#define SHA1_F0(b,c,d) (((b) & (c)) | ((~(b)) & (d)))
#define SHA1_F1(b,c,d) ((b) ^ (c) ^ (d))
#define SHA1_F2(b,c,d) (((b) & (c)) | ((b) & (d)) | ((c) & (d)))
#define SHA1_F3(b,c,d) ((b) ^ (c) ^ (d))

/* SHA-1 constants */
#define SHA1_K0 0x5A827999UL
#define SHA1_K1 0x6ED9EBA1UL
#define SHA1_K2 0x8F1BBCDCUL
#define SHA1_K3 0xCA62C1D6UL

/* SHA-1 initial hash values */
#define SHA1_H0 0x67452301UL
#define SHA1_H1 0xEFCDAB89UL
#define SHA1_H2 0x98BADCFEUL
#define SHA1_H3 0x10325476UL
#define SHA1_H4 0xC3D2E1F0UL

/* Internal: Process a single 64-byte block */
interior vacuum
_sha1_transformare(
    i32       status[V],
    constans i8 buffer[LXIV])
{
    i32 a, b, c, d, e;
    i32 w[LXXX];
    i32 i;

    /* Copy buffer into 16 32-bit words (big-endian) */
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        w[i] = ((i32)((i8)buffer[i * IV] & 0xFF) << XXIV) |
               ((i32)((i8)buffer[i * IV + I] & 0xFF) << XVI) |
               ((i32)((i8)buffer[i * IV + II] & 0xFF) << VIII) |
               ((i32)((i8)buffer[i * IV + III] & 0xFF));
    }

    /* Extend the sixteen 32-bit words into eighty 32-bit words */
    per (i = XVI; i < LXXX; i++)
    {
        w[i] = SHA1_ROL(w[i - III] ^ w[i - VIII] ^ w[i - XIV] ^ w[i - XVI], I);
    }

    /* Initialize working variables */
    a = status[ZEPHYRUM];
    b = status[I];
    c = status[II];
    d = status[III];
    e = status[IV];

    /* Main loop - 80 rounds */
    per (i = ZEPHYRUM; i < XX; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F0(b, c, d) + e + w[i] + (i32)SHA1_K0;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    per (i = XX; i < XL; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F1(b, c, d) + e + w[i] + (i32)SHA1_K1;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    per (i = XL; i < LX; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F2(b, c, d) + e + w[i] + (i32)SHA1_K2;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    per (i = LX; i < LXXX; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F3(b, c, d) + e + w[i] + (i32)SHA1_K3;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    /* Add to state */
    status[ZEPHYRUM] += a;
    status[I] += b;
    status[II] += c;
    status[III] += d;
    status[IV] += e;
}

vacuum
sha1_initiare(
    SHA1Contextus* ctx)
{
    ctx->status[ZEPHYRUM] = (i32)SHA1_H0;
    ctx->status[I]        = (i32)SHA1_H1;
    ctx->status[II]       = (i32)SHA1_H2;
    ctx->status[III]      = (i32)SHA1_H3;
    ctx->status[IV]       = (i32)SHA1_H4;
    ctx->numerus[ZEPHYRUM] = ZEPHYRUM;
    ctx->numerus[I]        = ZEPHYRUM;
}

vacuum
sha1_addere(
    SHA1Contextus*  ctx,
    constans i8*    data,
    i32             mensura)
{
    i32 i;
    i32 j;

    j = (ctx->numerus[ZEPHYRUM] >> III) & 0x3F;

    /* Update bit count */
    ctx->numerus[ZEPHYRUM] += (mensura << III);
    si (ctx->numerus[ZEPHYRUM] < (mensura << III))
    {
        ctx->numerus[I]++;
    }
    ctx->numerus[I] += (mensura >> XXIX);

    /* Process data */
    si ((j + mensura) > LXIII)
    {
        i = LXIV - j;
        memcpy(&ctx->buffer[j], data, (memoriae_index)i);
        _sha1_transformare(ctx->status, ctx->buffer);

        per (; i + LXIII < mensura; i += LXIV)
        {
            _sha1_transformare(ctx->status, &data[i]);
        }
        j = ZEPHYRUM;
    }
    alioquin
    {
        i = ZEPHYRUM;
    }

    memcpy(&ctx->buffer[j], &data[i], (memoriae_index)(mensura - i));
}

vacuum
sha1_finire(
    SHA1Contextus* ctx,
    i8             digest[SHA1_DIGEST_MENSURA])
{
    i8  numerus_finalis[VIII];
    i8  c;
    i32 i;

    /* Store bit count (big-endian) */
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        numerus_finalis[i] = (i8)((ctx->numerus[(i >= IV ? ZEPHYRUM : I)] >>
                             ((III - (i & III)) * VIII)) & 0xFF);
    }

    /* Pad to 56 mod 64 */
    c = (i8)0x80;
    sha1_addere(ctx, &c, I);
    dum ((ctx->numerus[ZEPHYRUM] & 0x1F8) != 0x1C0)
    {
        c = (i8)0x00;
        sha1_addere(ctx, &c, I);
    }

    /* Append length */
    sha1_addere(ctx, numerus_finalis, VIII);

    /* Output hash (big-endian) */
    per (i = ZEPHYRUM; i < SHA1_DIGEST_MENSURA; i++)
    {
        digest[i] = (i8)((ctx->status[i >> II] >> ((III - (i & III)) * VIII)) & 0xFF);
    }
}

vacuum
sha1_friare(
    constans i8* data,
    i32          mensura,
    i8           digest[SHA1_DIGEST_MENSURA])
{
    SHA1Contextus ctx;

    sha1_initiare(&ctx);
    sha1_addere(&ctx, data, mensura);
    sha1_finire(&ctx, digest);
}

vacuum
sha1_friare_chorda(
    chorda clavis,
    i8     digest[SHA1_DIGEST_MENSURA])
{
    sha1_friare((constans i8*)clavis.datum, clavis.mensura, digest);
}
