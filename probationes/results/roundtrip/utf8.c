/*
 * utf8.c - Implementatio bibliothecae UTF-8
 *
 * UTF-8 structura:
 *   0xxxxxxx                            = U+0000-007F   (1 byte, ASCII)
 *   110xxxxx 10xxxxxx                   = U+0080-07FF   (2 bytes)
 *   1110xxxx 10xxxxxx 10xxxxxx          = U+0800-FFFF   (3 bytes)
 *   11110xxx 10xxxxxx 10xxxxxx 10xxxxxx = U+10000-10FFFF (4 bytes)
 */

#include "../include/utf8.h"

/* Mascherae pro decodendo */
#define MASCA_ASCII       0x80u  /* 10000000 */
#define MASCA_CONT        0xC0u  /* 11000000 */
#define MASCA_2BYTE       0xE0u  /* 11100000 */
#define MASCA_3BYTE       0xF0u  /* 11110000 */
#define MASCA_4BYTE       0xF8u  /* 11111000 */

#define VALOR_CONT        0x80u  /* 10xxxxxx */
#define VALOR_2BYTE       0xC0u  /* 110xxxxx */
#define VALOR_3BYTE       0xE0u  /* 1110xxxx */
#define VALOR_4BYTE       0xF0u  /* 11110xxx */

/* Codepoint maximus validus */
#define CODEPOINT_MAXIMUS 0x10FFFF

/* Surrogates (invalidi in UTF-8) */
#define SURROGATUM_INITIUM 0xD800
#define SURROGATUM_FINIS   0xDFFF

s32
utf8_longitudo_byte(i8 byte)
{
    i8 b = byte;

    /* ASCII: 0xxxxxxx */
    si ((b & MASCA_ASCII) == 0)
    {
        redde 1;
    }

    /* Continuatio: 10xxxxxx - non est principalis */
    si ((b & MASCA_CONT) == VALOR_CONT)
    {
        redde 0;
    }

    /* 2 bytes: 110xxxxx */
    si ((b & MASCA_2BYTE) == VALOR_2BYTE)
    {
        redde 2;
    }

    /* 3 bytes: 1110xxxx */
    si ((b & MASCA_3BYTE) == VALOR_3BYTE)
    {
        redde 3;
    }

    /* 4 bytes: 11110xxx */
    si ((b & MASCA_4BYTE) == VALOR_4BYTE)
    {
        redde 4;
    }

    /* Invalidum: 11111xxx vel aliud */
    redde 0;
}

b32
utf8_est_continuatio(i8 byte)
{
    redde ((byte & MASCA_CONT) == VALOR_CONT);
}
