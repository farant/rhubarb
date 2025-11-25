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

/* ==================================================
 * SHA-256 Implementation
 *
 * Based on FIPS 180-4
 * Produces 256-bit (32-byte) message digest
 * ================================================== */

/* Right rotate 32-bit value */
#define SHA256_ROR(value, bits) (((value) >> (bits)) | ((value) << (XXXII - (bits))))

/* SHA-256 logical functions */
#define SHA256_CH(x,y,z)  (((x) & (y)) ^ ((~(x)) & (z)))
#define SHA256_MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define SHA256_EP0(x)     (SHA256_ROR(x, II) ^ SHA256_ROR(x, XIII) ^ SHA256_ROR(x, XXII))
#define SHA256_EP1(x)     (SHA256_ROR(x, VI) ^ SHA256_ROR(x, XI) ^ SHA256_ROR(x, XXV))
#define SHA256_SIG0(x)    (SHA256_ROR(x, VII) ^ SHA256_ROR(x, XVIII) ^ ((x) >> III))
#define SHA256_SIG1(x)    (SHA256_ROR(x, XVII) ^ SHA256_ROR(x, XIX) ^ ((x) >> X))

/* SHA-256 initial hash values (first 32 bits of fractional parts of square roots of first 8 primes) */
#define SHA256_H0 0x6A09E667UL
#define SHA256_H1 0xBB67AE85UL
#define SHA256_H2 0x3C6EF372UL
#define SHA256_H3 0xA54FF53AUL
#define SHA256_H4 0x510E527FUL
#define SHA256_H5 0x9B05688CUL
#define SHA256_H6 0x1F83D9ABUL
#define SHA256_H7 0x5BE0CD19UL

/* SHA-256 round constants (first 32 bits of fractional parts of cube roots of first 64 primes) */
hic_manens constans i32 SHA256_K[LXIV] = {
    (i32)0x428A2F98UL, (i32)0x71374491UL, (i32)0xB5C0FBCFUL, (i32)0xE9B5DBA5UL,
    (i32)0x3956C25BUL, (i32)0x59F111F1UL, (i32)0x923F82A4UL, (i32)0xAB1C5ED5UL,
    (i32)0xD807AA98UL, (i32)0x12835B01UL, (i32)0x243185BEUL, (i32)0x550C7DC3UL,
    (i32)0x72BE5D74UL, (i32)0x80DEB1FEUL, (i32)0x9BDC06A7UL, (i32)0xC19BF174UL,
    (i32)0xE49B69C1UL, (i32)0xEFBE4786UL, (i32)0x0FC19DC6UL, (i32)0x240CA1CCUL,
    (i32)0x2DE92C6FUL, (i32)0x4A7484AAUL, (i32)0x5CB0A9DCUL, (i32)0x76F988DAUL,
    (i32)0x983E5152UL, (i32)0xA831C66DUL, (i32)0xB00327C8UL, (i32)0xBF597FC7UL,
    (i32)0xC6E00BF3UL, (i32)0xD5A79147UL, (i32)0x06CA6351UL, (i32)0x14292967UL,
    (i32)0x27B70A85UL, (i32)0x2E1B2138UL, (i32)0x4D2C6DFCUL, (i32)0x53380D13UL,
    (i32)0x650A7354UL, (i32)0x766A0ABBUL, (i32)0x81C2C92EUL, (i32)0x92722C85UL,
    (i32)0xA2BFE8A1UL, (i32)0xA81A664BUL, (i32)0xC24B8B70UL, (i32)0xC76C51A3UL,
    (i32)0xD192E819UL, (i32)0xD6990624UL, (i32)0xF40E3585UL, (i32)0x106AA070UL,
    (i32)0x19A4C116UL, (i32)0x1E376C08UL, (i32)0x2748774CUL, (i32)0x34B0BCB5UL,
    (i32)0x391C0CB3UL, (i32)0x4ED8AA4AUL, (i32)0x5B9CCA4FUL, (i32)0x682E6FF3UL,
    (i32)0x748F82EEUL, (i32)0x78A5636FUL, (i32)0x84C87814UL, (i32)0x8CC70208UL,
    (i32)0x90BEFFFAUL, (i32)0xA4506CEBUL, (i32)0xBEF9A3F7UL, (i32)0xC67178F2UL
};

/* Internal: Process a single 64-byte block */
interior vacuum
_sha256_transformare(
    i32         status[VIII],
    constans i8 buffer[LXIV])
{
    i32 a, b, c, d, e, f, g, h;
    i32 w[LXIV];
    i32 i;
    i32 t1, t2;

    /* Copy buffer into 16 32-bit words (big-endian) */
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        w[i] = ((i32)((i8)buffer[i * IV] & 0xFF) << XXIV) |
               ((i32)((i8)buffer[i * IV + I] & 0xFF) << XVI) |
               ((i32)((i8)buffer[i * IV + II] & 0xFF) << VIII) |
               ((i32)((i8)buffer[i * IV + III] & 0xFF));
    }

    /* Extend the sixteen 32-bit words into sixty-four 32-bit words */
    per (i = XVI; i < LXIV; i++)
    {
        w[i] = SHA256_SIG1(w[i - II]) + w[i - VII] + SHA256_SIG0(w[i - XV]) + w[i - XVI];
    }

    /* Initialize working variables */
    a = status[ZEPHYRUM];
    b = status[I];
    c = status[II];
    d = status[III];
    e = status[IV];
    f = status[V];
    g = status[VI];
    h = status[VII];

    /* Main loop - 64 rounds */
    per (i = ZEPHYRUM; i < LXIV; i++)
    {
        t1 = h + SHA256_EP1(e) + SHA256_CH(e, f, g) + SHA256_K[i] + w[i];
        t2 = SHA256_EP0(a) + SHA256_MAJ(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    /* Add to state */
    status[ZEPHYRUM] += a;
    status[I] += b;
    status[II] += c;
    status[III] += d;
    status[IV] += e;
    status[V] += f;
    status[VI] += g;
    status[VII] += h;
}

vacuum
sha256_initiare(
    SHA256Contextus* ctx)
{
    ctx->status[ZEPHYRUM] = (i32)SHA256_H0;
    ctx->status[I]        = (i32)SHA256_H1;
    ctx->status[II]       = (i32)SHA256_H2;
    ctx->status[III]      = (i32)SHA256_H3;
    ctx->status[IV]       = (i32)SHA256_H4;
    ctx->status[V]        = (i32)SHA256_H5;
    ctx->status[VI]       = (i32)SHA256_H6;
    ctx->status[VII]      = (i32)SHA256_H7;
    ctx->numerus[ZEPHYRUM] = ZEPHYRUM;
    ctx->numerus[I]        = ZEPHYRUM;
}

vacuum
sha256_addere(
    SHA256Contextus* ctx,
    constans i8*     data,
    i32              mensura)
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
        _sha256_transformare(ctx->status, ctx->buffer);

        per (; i + LXIII < mensura; i += LXIV)
        {
            _sha256_transformare(ctx->status, &data[i]);
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
sha256_finire(
    SHA256Contextus* ctx,
    i8               digest[SHA256_DIGEST_MENSURA])
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
    sha256_addere(ctx, &c, I);
    dum ((ctx->numerus[ZEPHYRUM] & 0x1F8) != 0x1C0)
    {
        c = (i8)0x00;
        sha256_addere(ctx, &c, I);
    }

    /* Append length */
    sha256_addere(ctx, numerus_finalis, VIII);

    /* Output hash (big-endian) */
    per (i = ZEPHYRUM; i < SHA256_DIGEST_MENSURA; i++)
    {
        digest[i] = (i8)((ctx->status[i >> II] >> ((III - (i & III)) * VIII)) & 0xFF);
    }
}

vacuum
sha256_friare(
    constans i8* data,
    i32          mensura,
    i8           digest[SHA256_DIGEST_MENSURA])
{
    SHA256Contextus ctx;

    sha256_initiare(&ctx);
    sha256_addere(&ctx, data, mensura);
    sha256_finire(&ctx, digest);
}

vacuum
sha256_friare_chorda(
    chorda clavis,
    i8     digest[SHA256_DIGEST_MENSURA])
{
    sha256_friare((constans i8*)clavis.datum, clavis.mensura, digest);
}

/* ==================================================
 * CRC32 Implementation
 *
 * IEEE 802.3 polynomial: 0xEDB88320 (reflected)
 * Used in: Ethernet, ZIP, PNG, etc.
 * ================================================== */

/* CRC32 lookup table (pre-computed for IEEE polynomial) */
hic_manens constans i32 CRC32_TABULA[CCLVI] = {
    (i32)0x00000000UL, (i32)0x77073096UL, (i32)0xEE0E612CUL, (i32)0x990951BAUL,
    (i32)0x076DC419UL, (i32)0x706AF48FUL, (i32)0xE963A535UL, (i32)0x9E6495A3UL,
    (i32)0x0EDB8832UL, (i32)0x79DCB8A4UL, (i32)0xE0D5E91EUL, (i32)0x97D2D988UL,
    (i32)0x09B64C2BUL, (i32)0x7EB17CBDUL, (i32)0xE7B82D07UL, (i32)0x90BF1D91UL,
    (i32)0x1DB71064UL, (i32)0x6AB020F2UL, (i32)0xF3B97148UL, (i32)0x84BE41DEUL,
    (i32)0x1ADAD47DUL, (i32)0x6DDDE4EBUL, (i32)0xF4D4B551UL, (i32)0x83D385C7UL,
    (i32)0x136C9856UL, (i32)0x646BA8C0UL, (i32)0xFD62F97AUL, (i32)0x8A65C9ECUL,
    (i32)0x14015C4FUL, (i32)0x63066CD9UL, (i32)0xFA0F3D63UL, (i32)0x8D080DF5UL,
    (i32)0x3B6E20C8UL, (i32)0x4C69105EUL, (i32)0xD56041E4UL, (i32)0xA2677172UL,
    (i32)0x3C03E4D1UL, (i32)0x4B04D447UL, (i32)0xD20D85FDUL, (i32)0xA50AB56BUL,
    (i32)0x35B5A8FAUL, (i32)0x42B2986CUL, (i32)0xDBBBC9D6UL, (i32)0xACBCF940UL,
    (i32)0x32D86CE3UL, (i32)0x45DF5C75UL, (i32)0xDCD60DCFUL, (i32)0xABD13D59UL,
    (i32)0x26D930ACUL, (i32)0x51DE003AUL, (i32)0xC8D75180UL, (i32)0xBFD06116UL,
    (i32)0x21B4F4B5UL, (i32)0x56B3C423UL, (i32)0xCFBA9599UL, (i32)0xB8BDA50FUL,
    (i32)0x2802B89EUL, (i32)0x5F058808UL, (i32)0xC60CD9B2UL, (i32)0xB10BE924UL,
    (i32)0x2F6F7C87UL, (i32)0x58684C11UL, (i32)0xC1611DABUL, (i32)0xB6662D3DUL,
    (i32)0x76DC4190UL, (i32)0x01DB7106UL, (i32)0x98D220BCUL, (i32)0xEFD5102AUL,
    (i32)0x71B18589UL, (i32)0x06B6B51FUL, (i32)0x9FBFE4A5UL, (i32)0xE8B8D433UL,
    (i32)0x7807C9A2UL, (i32)0x0F00F934UL, (i32)0x9609A88EUL, (i32)0xE10E9818UL,
    (i32)0x7F6A0DBBUL, (i32)0x086D3D2DUL, (i32)0x91646C97UL, (i32)0xE6635C01UL,
    (i32)0x6B6B51F4UL, (i32)0x1C6C6162UL, (i32)0x856530D8UL, (i32)0xF262004EUL,
    (i32)0x6C0695EDUL, (i32)0x1B01A57BUL, (i32)0x8208F4C1UL, (i32)0xF50FC457UL,
    (i32)0x65B0D9C6UL, (i32)0x12B7E950UL, (i32)0x8BBEB8EAUL, (i32)0xFCB9887CUL,
    (i32)0x62DD1DDFUL, (i32)0x15DA2D49UL, (i32)0x8CD37CF3UL, (i32)0xFBD44C65UL,
    (i32)0x4DB26158UL, (i32)0x3AB551CEUL, (i32)0xA3BC0074UL, (i32)0xD4BB30E2UL,
    (i32)0x4ADFA541UL, (i32)0x3DD895D7UL, (i32)0xA4D1C46DUL, (i32)0xD3D6F4FBUL,
    (i32)0x4369E96AUL, (i32)0x346ED9FCUL, (i32)0xAD678846UL, (i32)0xDA60B8D0UL,
    (i32)0x44042D73UL, (i32)0x33031DE5UL, (i32)0xAA0A4C5FUL, (i32)0xDD0D7CC9UL,
    (i32)0x5005713CUL, (i32)0x270241AAUL, (i32)0xBE0B1010UL, (i32)0xC90C2086UL,
    (i32)0x5768B525UL, (i32)0x206F85B3UL, (i32)0xB966D409UL, (i32)0xCE61E49FUL,
    (i32)0x5EDEF90EUL, (i32)0x29D9C998UL, (i32)0xB0D09822UL, (i32)0xC7D7A8B4UL,
    (i32)0x59B33D17UL, (i32)0x2EB40D81UL, (i32)0xB7BD5C3BUL, (i32)0xC0BA6CADUL,
    (i32)0xEDB88320UL, (i32)0x9ABFB3B6UL, (i32)0x03B6E20CUL, (i32)0x74B1D29AUL,
    (i32)0xEAD54739UL, (i32)0x9DD277AFUL, (i32)0x04DB2615UL, (i32)0x73DC1683UL,
    (i32)0xE3630B12UL, (i32)0x94643B84UL, (i32)0x0D6D6A3EUL, (i32)0x7A6A5AA8UL,
    (i32)0xE40ECF0BUL, (i32)0x9309FF9DUL, (i32)0x0A00AE27UL, (i32)0x7D079EB1UL,
    (i32)0xF00F9344UL, (i32)0x8708A3D2UL, (i32)0x1E01F268UL, (i32)0x6906C2FEUL,
    (i32)0xF762575DUL, (i32)0x806567CBUL, (i32)0x196C3671UL, (i32)0x6E6B06E7UL,
    (i32)0xFED41B76UL, (i32)0x89D32BE0UL, (i32)0x10DA7A5AUL, (i32)0x67DD4ACCUL,
    (i32)0xF9B9DF6FUL, (i32)0x8EBEEFF9UL, (i32)0x17B7BE43UL, (i32)0x60B08ED5UL,
    (i32)0xD6D6A3E8UL, (i32)0xA1D1937EUL, (i32)0x38D8C2C4UL, (i32)0x4FDFF252UL,
    (i32)0xD1BB67F1UL, (i32)0xA6BC5767UL, (i32)0x3FB506DDUL, (i32)0x48B2364BUL,
    (i32)0xD80D2BDAUL, (i32)0xAF0A1B4CUL, (i32)0x36034AF6UL, (i32)0x41047A60UL,
    (i32)0xDF60EFC3UL, (i32)0xA867DF55UL, (i32)0x316E8EEFUL, (i32)0x4669BE79UL,
    (i32)0xCB61B38CUL, (i32)0xBC66831AUL, (i32)0x256FD2A0UL, (i32)0x5268E236UL,
    (i32)0xCC0C7795UL, (i32)0xBB0B4703UL, (i32)0x220216B9UL, (i32)0x5505262FUL,
    (i32)0xC5BA3BBEUL, (i32)0xB2BD0B28UL, (i32)0x2BB45A92UL, (i32)0x5CB36A04UL,
    (i32)0xC2D7FFA7UL, (i32)0xB5D0CF31UL, (i32)0x2CD99E8BUL, (i32)0x5BDEAE1DUL,
    (i32)0x9B64C2B0UL, (i32)0xEC63F226UL, (i32)0x756AA39CUL, (i32)0x026D930AUL,
    (i32)0x9C0906A9UL, (i32)0xEB0E363FUL, (i32)0x72076785UL, (i32)0x05005713UL,
    (i32)0x95BF4A82UL, (i32)0xE2B87A14UL, (i32)0x7BB12BAEUL, (i32)0x0CB61B38UL,
    (i32)0x92D28E9BUL, (i32)0xE5D5BE0DUL, (i32)0x7CDCEFB7UL, (i32)0x0BDBDF21UL,
    (i32)0x86D3D2D4UL, (i32)0xF1D4E242UL, (i32)0x68DDB3F8UL, (i32)0x1FDA836EUL,
    (i32)0x81BE16CDUL, (i32)0xF6B9265BUL, (i32)0x6FB077E1UL, (i32)0x18B74777UL,
    (i32)0x88085AE6UL, (i32)0xFF0F6A70UL, (i32)0x66063BCAUL, (i32)0x11010B5CUL,
    (i32)0x8F659EFFUL, (i32)0xF862AE69UL, (i32)0x616BFFD3UL, (i32)0x166CCF45UL,
    (i32)0xA00AE278UL, (i32)0xD70DD2EEUL, (i32)0x4E048354UL, (i32)0x3903B3C2UL,
    (i32)0xA7672661UL, (i32)0xD06016F7UL, (i32)0x4969474DUL, (i32)0x3E6E77DBUL,
    (i32)0xAED16A4AUL, (i32)0xD9D65ADCUL, (i32)0x40DF0B66UL, (i32)0x37D83BF0UL,
    (i32)0xA9BCAE53UL, (i32)0xDEBB9EC5UL, (i32)0x47B2CF7FUL, (i32)0x30B5FFE9UL,
    (i32)0xBDBDF21CUL, (i32)0xCABAC28AUL, (i32)0x53B39330UL, (i32)0x24B4A3A6UL,
    (i32)0xBAD03605UL, (i32)0xCDD70693UL, (i32)0x54DE5729UL, (i32)0x23D967BFUL,
    (i32)0xB3667A2EUL, (i32)0xC4614AB8UL, (i32)0x5D681B02UL, (i32)0x2A6F2B94UL,
    (i32)0xB40BBE37UL, (i32)0xC30C8EA1UL, (i32)0x5A05DF1BUL, (i32)0x2D02EF8DUL
};

i32
crc32_initiare(vacuum)
{
    redde (i32)0xFFFFFFFFUL;
}

i32
crc32_addere(
    i32          crc_currens,
    constans i8* data,
    i32          mensura)
{
    i32 i;
    i32 crc;

    crc = crc_currens;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        crc = CRC32_TABULA[((crc ^ (i8)data[i]) & 0xFF)] ^ (crc >> VIII);
    }

    redde crc;
}

i32
crc32_finire(
    i32 crc_currens)
{
    redde crc_currens ^ (i32)0xFFFFFFFFUL;
}

i32
crc32_calculare(
    constans i8* data,
    i32          mensura)
{
    i32 crc;

    crc = crc32_initiare();
    crc = crc32_addere(crc, data, mensura);
    crc = crc32_finire(crc);

    redde crc;
}

i32
crc32_calculare_chorda(
    chorda clavis)
{
    redde crc32_calculare((constans i8*)clavis.datum, clavis.mensura);
}
