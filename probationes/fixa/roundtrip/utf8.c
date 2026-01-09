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

s32
utf8_decodere(constans i8** ptr, constans i8* finis)
{
    constans i8* p;
    i8 primus;
    s32 longitudo;
    s32 codepoint;
    s32 i;

    si (ptr == NIHIL || *ptr == NIHIL || *ptr >= finis)
    {
        redde -1;
    }

    p = *ptr;
    primus = *p;
    longitudo = utf8_longitudo_byte(primus);

    /* Byte invalidus vel continuatio orphana */
    si (longitudo == 0)
    {
        (*ptr)++;
        redde -1;
    }

    /* Verifica satis bytes */
    si (p + longitudo > finis)
    {
        (*ptr)++;
        redde -1;
    }

    /* Decodere secundum longitudinem */
    commutatio (longitudo)
    {
        casus 1:
            /* ASCII directum */
            codepoint = (s32)primus;
            frange;

        casus 2:
            /* 110xxxxx 10xxxxxx */
            si (!utf8_est_continuatio(p[1]))
            {
                (*ptr)++;
                redde -1;
            }
            codepoint = ((s32)(primus & 0x1F) << 6) |
                        ((s32)(p[1] & 0x3F));
            /* Verifica non-overlong (minimum 0x80) */
            si (codepoint < 0x80)
            {
                (*ptr) += 2;
                redde -1;
            }
            frange;

        casus 3:
            /* 1110xxxx 10xxxxxx 10xxxxxx */
            per (i = 1; i < 3; i++)
            {
                si (!utf8_est_continuatio(p[i]))
                {
                    (*ptr)++;
                    redde -1;
                }
            }
            codepoint = ((s32)(primus & 0x0F) << 12) |
                        ((s32)(p[1] & 0x3F) << 6) |
                        ((s32)(p[2] & 0x3F));
            /* Verifica non-overlong (minimum 0x800) */
            si (codepoint < 0x800)
            {
                (*ptr) += 3;
                redde -1;
            }
            /* Verifica non-surrogatum */
            si (codepoint >= SURROGATUM_INITIUM && codepoint <= SURROGATUM_FINIS)
            {
                (*ptr) += 3;
                redde -1;
            }
            frange;

        casus 4:
            /* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
            per (i = 1; i < 4; i++)
            {
                si (!utf8_est_continuatio(p[i]))
                {
                    (*ptr)++;
                    redde -1;
                }
            }
            codepoint = ((s32)(primus & 0x07) << 18) |
                        ((s32)(p[1] & 0x3F) << 12) |
                        ((s32)(p[2] & 0x3F) << 6) |
                        ((s32)(p[3] & 0x3F));
            /* Verifica non-overlong (minimum 0x10000) */
            si (codepoint < 0x10000)
            {
                (*ptr) += 4;
                redde -1;
            }
            /* Verifica intra limites Unicode */
            si (codepoint > CODEPOINT_MAXIMUS)
            {
                (*ptr) += 4;
                redde -1;
            }
            frange;

        ordinarius:
            (*ptr)++;
            redde -1;
    }

    /* Promove indicator */
    (*ptr) += longitudo;
    redde codepoint;
}

s32
utf8_numerare_runas(constans i8* datum, s32 mensura)
{
    constans i8* ptr;
    constans i8* finis;
    s32 numerus;
    s32 runa;

    si (datum == NIHIL || mensura <= 0)
    {
        redde 0;
    }

    ptr = datum;
    finis = datum + mensura;
    numerus = 0;

    dum (ptr < finis)
    {
        runa = utf8_decodere(&ptr, finis);
        (vacuum)runa;  /* Non opus valore, solum numerare */
        numerus++;
    }

    redde numerus;
}

constans i8*
utf8_proxima_runa(constans i8* ptr, constans i8* finis)
{
    s32 longitudo;

    si (ptr == NIHIL || ptr >= finis)
    {
        redde ptr;
    }

    longitudo = utf8_longitudo_byte(*ptr);

    si (longitudo == 0)
    {
        /* Continuatio orphana, salire unum byte */
        redde ptr + 1;
    }

    /* Salire longitudinem, sed non ultra finem */
    si (ptr + longitudo > finis)
    {
        redde finis;
    }

    redde ptr + longitudo;
}

constans i8*
utf8_prior_runa(constans i8* ptr, constans i8* initium)
{
    constans i8* p;
    s32 retro;

    si (ptr == NIHIL || ptr <= initium)
    {
        redde initium;
    }

    p = ptr - 1;

    /* Retrocedere per continuationes (maximum 3) */
    retro = 0;
    dum (p > initium && utf8_est_continuatio(*p) && retro < 3)
    {
        p--;
        retro++;
    }

    redde p;
}
