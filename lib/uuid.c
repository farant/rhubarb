#include "uuid.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>  /* arc4random_buf on macOS */

/* ==================================================
 * SHA-1 Implementation (for UUIDv5)
 * ================================================== */

/* SHA-1 produces 20 bytes (160 bits) hash */
#define SHA1_DIGEST_SIZE XX

/* SHA-1 context structure */
nomen structura {
    i32 state[V];       /* A, B, C, D, E */
    i32 count[II];      /* Bit count (low, high) */
    i8  buffer[LXIV];   /* Input buffer */
} SHA1Context;

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

interior vacuum
_sha1_transform(i32 state[V], constans i8 buffer[LXIV])
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
    a = state[ZEPHYRUM];
    b = state[I];
    c = state[II];
    d = state[III];
    e = state[IV];

    /* Main loop */
    per (i = ZEPHYRUM; i < XX; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F0(b, c, d) + e + w[i] + SHA1_K0;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    per (i = XX; i < XL; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F1(b, c, d) + e + w[i] + SHA1_K1;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    per (i = XL; i < LX; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F2(b, c, d) + e + w[i] + SHA1_K2;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    per (i = LX; i < LXXX; i++)
    {
        i32 temp;
        temp = SHA1_ROL(a, V) + SHA1_F3(b, c, d) + e + w[i] + SHA1_K3;
        e = d;
        d = c;
        c = SHA1_ROL(b, XXX);
        b = a;
        a = temp;
    }

    /* Add to state */
    state[ZEPHYRUM] += a;
    state[I] += b;
    state[II] += c;
    state[III] += d;
    state[IV] += e;
}

interior vacuum
_sha1_init(SHA1Context* ctx)
{
    /* SHA-1 initial hash values */
    ctx->state[ZEPHYRUM] = (i32)0x67452301UL;
    ctx->state[I]        = (i32)0xEFCDAB89UL;
    ctx->state[II]       = (i32)0x98BADCFEUL;
    ctx->state[III]      = (i32)0x10325476UL;
    ctx->state[IV]       = (i32)0xC3D2E1F0UL;
    ctx->count[ZEPHYRUM] = ZEPHYRUM;
    ctx->count[I]        = ZEPHYRUM;
}

interior vacuum
_sha1_update(SHA1Context* ctx, constans i8* data, i32 len)
{
    i32 i;
    i32 j;

    j = (ctx->count[ZEPHYRUM] >> III) & 0x3F;

    /* Update bit count */
    ctx->count[ZEPHYRUM] += (len << III);
    si (ctx->count[ZEPHYRUM] < (len << III))
    {
        ctx->count[I]++;
    }
    ctx->count[I] += (len >> XXIX);

    /* Process data */
    si ((j + len) > LXIII)
    {
        i = LXIV - j;
        memcpy(&ctx->buffer[j], data, (memoriae_index)i);
        _sha1_transform(ctx->state, ctx->buffer);

        per (; i + LXIII < len; i += LXIV)
        {
            _sha1_transform(ctx->state, &data[i]);
        }
        j = ZEPHYRUM;
    }
    alioquin
    {
        i = ZEPHYRUM;
    }

    memcpy(&ctx->buffer[j], &data[i], (memoriae_index)(len - i));
}

interior vacuum
_sha1_final(SHA1Context* ctx, i8 digest[SHA1_DIGEST_SIZE])
{
    i8  finalcount[VIII];
    i8  c;
    i32 i;

    /* Store bit count (big-endian) */
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        finalcount[i] = (i8)((ctx->count[(i >= IV ? ZEPHYRUM : I)] >>
                             ((III - (i & III)) * VIII)) & 0xFF);
    }

    /* Pad to 56 mod 64 */
    c = (i8)0x80;
    _sha1_update(ctx, &c, I);
    dum ((ctx->count[ZEPHYRUM] & 0x1F8) != 0x1C0)
    {
        c = (i8)0x00;
        _sha1_update(ctx, &c, I);
    }

    /* Append length */
    _sha1_update(ctx, finalcount, VIII);

    /* Output hash (big-endian) */
    per (i = ZEPHYRUM; i < SHA1_DIGEST_SIZE; i++)
    {
        digest[i] = (i8)((ctx->state[i >> II] >> ((III - (i & III)) * VIII)) & 0xFF);
    }
}


/* ==================================================
 * Auxiliares Internae
 * ================================================== */

/* Capere tempus currens in milliseconds */
interior i64
_tempus_milliseconds(vacuum)
{
    structura timeval tv;
    gettimeofday(&tv, NIHIL);
    redde ((i64)tv.tv_sec * M) + ((i64)tv.tv_usec / M);
}

/* Generare bytes randomos */
interior vacuum
_generare_randomos(i8* bytes, i32 mensura)
{
#ifdef __APPLE__
    /* macOS habet arc4random_buf */
    arc4random_buf(bytes, (memoriae_index)mensura);
#else
    /* Linux/Unix: usus /dev/urandom */
    FILE* urandom;
    i32   i;

    urandom = fopen("/dev/urandom", "rb");
    si (urandom)
    {
        fread(bytes, I, (memoriae_index)mensura, urandom);
        fclose(urandom);
    }
    alioquin
    {
        /* Fallback: usus rand() (non cryptographice securus!) */
        per (i = ZEPHYRUM; i < mensura; i++)
        {
            bytes[i] = (insignatus i8)(rand() & 0xFF);
        }
    }
#endif
}


/* ==================================================
 * Creatio
 * ================================================== */

chorda
uuidv7_creare(
    Piscina* piscina)
{
    i8         bytes[XVI];
    i64        timestamp_ms;
    chorda     resultus;
    character* str;

    si (!piscina)
    {
        resultus.datum   = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Capere tempus in milliseconds */
    timestamp_ms = _tempus_milliseconds();

    /* Generare omnes bytes randomos primo */
    _generare_randomos(bytes, XVI);

    /* Ponere timestamp (48 bits, big-endian) in bytes 0-5 */
    bytes[ZEPHYRUM] = (i8)((timestamp_ms >> XL) & 0xFF);
    bytes[I]        = (i8)((timestamp_ms >> XXXII) & 0xFF);
    bytes[II]       = (i8)((timestamp_ms >> XXIV) & 0xFF);
    bytes[III]      = (i8)((timestamp_ms >> XVI) & 0xFF);
    bytes[IV]       = (i8)((timestamp_ms >> VIII) & 0xFF);
    bytes[V]        = (i8)(timestamp_ms & 0xFF);

    /* Ponere version bits (4 bits = 0111 = 7) in byte 6 */
    bytes[VI] = (i8)((bytes[VI] & 0x0F) | 0x70);

    /* Ponere variant bits (2 bits = 10) in byte 8 */
    bytes[VIII] = (i8)((bytes[VIII] & 0x3F) | 0x80);

    /* Allocare spatium pro chorda (36 characteres + nullus terminator) */
    str = (character*)piscina_allocare(piscina, UUID_MENSURA_CHORDAE + I);
    si (!str)
    {
        resultus.datum   = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Formare UUID chordam: xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx */
    sprintf(str,
            "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            bytes[ZEPHYRUM], bytes[I], bytes[II], bytes[III],
            bytes[IV], bytes[V],
            bytes[VI], bytes[VII],
            bytes[VIII], bytes[IX],
            bytes[X], bytes[XI], bytes[XII], bytes[XIII], bytes[XIV], bytes[XV]);

    /* Creare chordam */
    resultus.datum   = (i8*)str;
    resultus.mensura = UUID_MENSURA_CHORDAE;

    redde resultus;
}

chorda
uuidv5_creare(
    Piscina*            piscina,
    chorda              namespace_uuid,
    constans character* appellatio)
{
    SHA1Context ctx;
    i8          namespace_bytes[XVI];
    i8          hash[SHA1_DIGEST_SIZE];
    i8          uuid_bytes[XVI];
    chorda      resultus;
    character*  str;
    i32         appellatio_len;

    si (!piscina || !appellatio)
    {
        resultus.datum   = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Convert namespace UUID to bytes */
    si (!uuid_ex_chorda(namespace_uuid, namespace_bytes))
    {
        resultus.datum   = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Calculate name length */
    appellatio_len = ZEPHYRUM;
    dum (appellatio[appellatio_len] != '\0')
    {
        appellatio_len++;
    }

    /* Hash namespace + name */
    _sha1_init(&ctx);
    _sha1_update(&ctx, namespace_bytes, XVI);
    _sha1_update(&ctx, (constans i8*)appellatio, appellatio_len);
    _sha1_final(&ctx, hash);

    /* Copy first 16 bytes of hash to UUID */
    memcpy(uuid_bytes, hash, XVI);

    /* Set version bits (4 bits = 0101 = 5) in byte 6 */
    uuid_bytes[VI] = (i8)((uuid_bytes[VI] & 0x0F) | 0x50);

    /* Set variant bits (2 bits = 10) in byte 8 */
    uuid_bytes[VIII] = (i8)((uuid_bytes[VIII] & 0x3F) | 0x80);

    /* Allocate space for string */
    str = (character*)piscina_allocare(piscina, UUID_MENSURA_CHORDAE + I);
    si (!str)
    {
        resultus.datum   = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Format UUID string */
    sprintf(str,
            "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            (i8)uuid_bytes[ZEPHYRUM] & 0xFF, (i8)uuid_bytes[I] & 0xFF,
            (i8)uuid_bytes[II] & 0xFF, (i8)uuid_bytes[III] & 0xFF,
            (i8)uuid_bytes[IV] & 0xFF, (i8)uuid_bytes[V] & 0xFF,
            (i8)uuid_bytes[VI] & 0xFF, (i8)uuid_bytes[VII] & 0xFF,
            (i8)uuid_bytes[VIII] & 0xFF, (i8)uuid_bytes[IX] & 0xFF,
            (i8)uuid_bytes[X] & 0xFF, (i8)uuid_bytes[XI] & 0xFF,
            (i8)uuid_bytes[XII] & 0xFF, (i8)uuid_bytes[XIII] & 0xFF,
            (i8)uuid_bytes[XIV] & 0xFF, (i8)uuid_bytes[XV] & 0xFF);

    resultus.datum   = (i8*)str;
    resultus.mensura = UUID_MENSURA_CHORDAE;

    redde resultus;
}

/* Internal namespace UUID for entity scaffolding
 * This is a randomly generated UUID used as the namespace
 * for all entity-based UUIDv5 generation
 */
hic_manens constans character UUID_NAMESPACE_ENTITAS_BYTES[] =
    "f47ac10b-58cc-4372-a567-0e02b2c3d479";

chorda
uuidv5_ex_genere_et_titulo(
    Piscina*            piscina,
    constans character* genus,
    constans character* titulus)
{
    character  appellatio_buffer[CCLVI];
    i32        genus_len;
    i32        titulus_len;
    i32        i;
    chorda     namespace_chorda;

    si (!piscina || !genus || !titulus)
    {
        chorda resultus;
        resultus.datum   = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Calculate lengths */
    genus_len = ZEPHYRUM;
    dum (genus[genus_len] != '\0')
    {
        genus_len++;
    }

    titulus_len = ZEPHYRUM;
    dum (titulus[titulus_len] != '\0')
    {
        titulus_len++;
    }

    /* Build combined name: "genus:titulus" */
    si (genus_len + I + titulus_len >= CCLVI)
    {
        chorda resultus;
        resultus.datum   = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    per (i = ZEPHYRUM; i < genus_len; i++)
    {
        appellatio_buffer[i] = genus[i];
    }
    appellatio_buffer[genus_len] = ':';
    per (i = ZEPHYRUM; i < titulus_len; i++)
    {
        appellatio_buffer[genus_len + I + i] = titulus[i];
    }
    appellatio_buffer[genus_len + I + titulus_len] = '\0';

    /* Create namespace chorda from static bytes */
    namespace_chorda = chorda_ex_literis(UUID_NAMESPACE_ENTITAS_BYTES, piscina);

    redde uuidv5_creare(piscina, namespace_chorda, appellatio_buffer);
}


/* ==================================================
 * Conversio
 * ================================================== */

b32
uuid_ex_chorda(
    chorda  uuid_chorda,
    i8      bytes[XVI])
{
    i32       i;
    i32       byte_index;
    i32       char_index;
    character hi;
    character lo;

    si (!bytes || uuid_chorda.mensura != UUID_MENSURA_CHORDAE)
    {
        redde FALSUM;
    }

    byte_index = ZEPHYRUM;
    char_index = ZEPHYRUM;

    /* Parse 16 bytes ex formato UUID */
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        /* Saltare dashes */
        si (char_index == VIII || char_index == XIII ||
            char_index == XVIII || char_index == XXIII)
        {
            char_index++;
        }

        hi = (character)uuid_chorda.datum[char_index];
        lo = (character)uuid_chorda.datum[char_index + I];

        /* Convertere hex characteres ad byte */
        si (hi >= '0' && hi <= '9')
        {
            bytes[byte_index] = (i8)((hi - '0') << IV);
        }
        alioquin si (hi >= 'a' && hi <= 'f')
        {
            bytes[byte_index] = (i8)((hi - 'a' + X) << IV);
        }
        alioquin si (hi >= 'A' && hi <= 'F')
        {
            bytes[byte_index] = (i8)((hi - 'A' + X) << IV);
        }
        alioquin
        {
            redde FALSUM;  /* Character invalidus */
        }

        si (lo >= '0' && lo <= '9')
        {
            bytes[byte_index] |= (i8)(lo - '0');
        }
        alioquin si (lo >= 'a' && lo <= 'f')
        {
            bytes[byte_index] |= (i8)(lo - 'a' + X);
        }
        alioquin si (lo >= 'A' && lo <= 'F')
        {
            bytes[byte_index] |= (i8)(lo - 'A' + X);
        }
        alioquin
        {
            redde FALSUM;  /* Character invalidus */
        }

        byte_index++;
        char_index += II;
    }

    redde VERUM;
}

chorda
uuid_ad_chordam(
    i8       bytes[XVI],
    Piscina* piscina)
{
    chorda     resultus;
    character* str;

    si (!bytes || !piscina)
    {
        resultus.datum   = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Allocare spatium */
    str = (character*)piscina_allocare(piscina, UUID_MENSURA_CHORDAE + I);
    si (!str)
    {
        resultus.datum   = NIHIL;
        resultus.mensura = ZEPHYRUM;
        redde resultus;
    }

    /* Formare UUID chordam */
    sprintf(str,
            "%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            bytes[ZEPHYRUM], bytes[I], bytes[II], bytes[III],
            bytes[IV], bytes[V],
            bytes[VI], bytes[VII],
            bytes[VIII], bytes[IX],
            bytes[X], bytes[XI], bytes[XII], bytes[XIII], bytes[XIV], bytes[XV]);

    resultus.datum   = (i8*)str;
    resultus.mensura = UUID_MENSURA_CHORDAE;

    redde resultus;
}


/* ==================================================
 * Validatio
 * ================================================== */

b32
uuid_est_validus(
    chorda uuid_chorda)
{
    i32 i;

    /* Verificare mensuram */
    si (uuid_chorda.mensura != UUID_MENSURA_CHORDAE)
    {
        redde FALSUM;
    }

    /* Verificare dashes in locis correctis */
    si (uuid_chorda.datum[VIII] != '-' ||
        uuid_chorda.datum[XIII] != '-' ||
        uuid_chorda.datum[XVIII] != '-' ||
        uuid_chorda.datum[XXIII] != '-')
    {
        redde FALSUM;
    }

    /* Verificare omnes alii characteres sunt hex */
    per (i = ZEPHYRUM; i < UUID_MENSURA_CHORDAE; i++)
    {
        character c;

        /* Saltare dashes */
        si (i == VIII || i == XIII || i == XVIII || i == XXIII)
        {
            perge;
        }

        c = (character)uuid_chorda.datum[i];

        si (!((c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'f') ||
              (c >= 'A' && c <= 'F')))
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

b32
uuid_est_v7(
    chorda uuid_chorda)
{
    character version_char;

    /* Verificare si UUID validus primo */
    si (!uuid_est_validus(uuid_chorda))
    {
        redde FALSUM;
    }

    /* Version digit est in positu 14 (post "xxxxxxxx-xxxx-") */
    version_char = (character)uuid_chorda.datum[XIV];

    /* Debet esse '7' */
    redde version_char == '7';
}

b32
uuid_est_v5(
    chorda uuid_chorda)
{
    character version_char;

    /* Verificare si UUID validus primo */
    si (!uuid_est_validus(uuid_chorda))
    {
        redde FALSUM;
    }

    /* Version digit est in positu 14 (post "xxxxxxxx-xxxx-") */
    version_char = (character)uuid_chorda.datum[XIV];

    /* Debet esse '5' */
    redde version_char == '5';
}
