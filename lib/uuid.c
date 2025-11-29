#include "uuid.h"
#include "friatio.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <stdlib.h>  /* arc4random_buf on macOS */

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
    SHA1Contextus ctx;
    i8            namespace_bytes[XVI];
    i8            hash[SHA1_DIGEST_MENSURA];
    i8            uuid_bytes[XVI];
    chorda        resultus;
    character*    str;
    i32           appellatio_len;

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

    /* Hash namespace + name using friatio SHA-1 */
    sha1_initiare(&ctx);
    sha1_addere(&ctx, namespace_bytes, XVI);
    sha1_addere(&ctx, (constans i8*)appellatio, appellatio_len);
    sha1_finire(&ctx, hash);

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
 * Tempus Extractio
 * ================================================== */

s64
uuidv7_tempus_extrahere(
    chorda uuid_chorda)
{
    i8  bytes[VI];
    i32 i;
    i32 char_index;
    s64 timestamp_ms;
    character hi;
    character lo;

    /* Verificare si UUIDv7 */
    si (!uuid_est_v7(uuid_chorda))
    {
        redde -I;
    }

    /* Extrahere primos 6 bytes (48 bits) - timestamp */
    /* UUID format: xxxxxxxx-xxxx-7xxx-...
     * Characters 0-7:   bytes 0-3
     * Characters 9-12:  bytes 4-5 (post primum dash)
     */
    char_index = ZEPHYRUM;

    per (i = ZEPHYRUM; i < VI; i++)
    {
        /* Saltare dash post 8 characteres */
        si (char_index == VIII)
        {
            char_index++;
        }

        hi = (character)uuid_chorda.datum[char_index];
        lo = (character)uuid_chorda.datum[char_index + I];

        /* Convertere hex ad byte */
        si (hi >= '0' && hi <= '9')
        {
            bytes[i] = (i8)((hi - '0') << IV);
        }
        alioquin si (hi >= 'a' && hi <= 'f')
        {
            bytes[i] = (i8)((hi - 'a' + X) << IV);
        }
        alioquin si (hi >= 'A' && hi <= 'F')
        {
            bytes[i] = (i8)((hi - 'A' + X) << IV);
        }
        alioquin
        {
            redde -I;
        }

        si (lo >= '0' && lo <= '9')
        {
            bytes[i] |= (i8)(lo - '0');
        }
        alioquin si (lo >= 'a' && lo <= 'f')
        {
            bytes[i] |= (i8)(lo - 'a' + X);
        }
        alioquin si (lo >= 'A' && lo <= 'F')
        {
            bytes[i] |= (i8)(lo - 'A' + X);
        }
        alioquin
        {
            redde -I;
        }

        char_index += II;
    }

    /* Reconstituere timestamp (big-endian 48 bits) */
    timestamp_ms = ((s64)(bytes[ZEPHYRUM] & 0xFF) << XL) |
                   ((s64)(bytes[I] & 0xFF) << XXXII) |
                   ((s64)(bytes[II] & 0xFF) << XXIV) |
                   ((s64)(bytes[III] & 0xFF) << XVI) |
                   ((s64)(bytes[IV] & 0xFF) << VIII) |
                   ((s64)(bytes[V] & 0xFF));

    redde timestamp_ms;
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
