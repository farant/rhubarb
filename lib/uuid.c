#include "uuid.h"
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
