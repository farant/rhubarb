/* mimen.c - Tabula MIME communis
 *
 * Tabula XIV ordinum verbatim ex vitrea_macos.m portata (capsula
 * metadata nulla fert - verdictum exploratoris; consumptoris officium).
 */

#include "mimen.h"

#include <string.h>

nomen structura {
    constans character* extensio;
    constans character* mimen;
} MimenOrdo;

interior constans MimenOrdo MIMINA[] = {
    { "html",  "text/html" },
    { "css",   "text/css" },
    { "js",    "text/javascript" },
    { "mjs",   "text/javascript" },
    { "json",  "application/json" },
    { "svg",   "image/svg+xml" },
    { "png",   "image/png" },
    { "jpg",   "image/jpeg" },
    { "jpeg",  "image/jpeg" },
    { "gif",   "image/gif" },
    { "webp",  "image/webp" },
    { "woff2", "font/woff2" },
    { "wasm",  "application/wasm" },
    { "txt",   "text/plain" }
};

#define MIMINA_NUMERUS ((i32)(magnitudo(MIMINA) / magnitudo(MIMINA[0])))

constans character*
mimen_pro_via (
    constans character* via)
{
    constans character* punctum = NIHIL;
    constans character* c;
                   i32  i;

    si (via == NIHIL)
    {
        redde "application/octet-stream";
    }

    per (c = via; *c != '\0'; c++)
    {
        si (*c == '.')
        {
            punctum = c + I;
        }
        alioquin si (*c == '/')
        {
            punctum = NIHIL;
        }
    }
    si (punctum != NIHIL)
    {
        per (i = ZEPHYRUM; i < MIMINA_NUMERUS; i++)
        {
            si (strcmp(punctum, MIMINA[i].extensio) == ZEPHYRUM)
            {
                redde MIMINA[i].mimen;
            }
        }
    }
    redde "application/octet-stream";
}

constans character*
mimen_pro_via_chorda (
    chorda via)
{
    i32 punctum        = 0;
    b32 habet_punctum  = FALSUM;
    i32 ext_len;
    i32 i;

    per (i = 0; i < via.mensura; i++)
    {
        si (via.datum[i] == '.')
        {
            punctum        = i + I;
            habet_punctum  = VERUM;
        }
        alioquin si (via.datum[i] == '/')
        {
            habet_punctum = FALSUM;
        }
    }

    si (habet_punctum)
    {
        /* punctum <= mensura semper - punctum caudale dat ext_len 0,
         * nullam extensionem aequat, octet-stream sequitur */
        ext_len = via.mensura - punctum;
        per (i = ZEPHYRUM; i < MIMINA_NUMERUS; i++)
        {
            si (   ext_len == (i32)strlen(MIMINA[i].extensio)
                && memcmp(&via.datum[punctum], MIMINA[i].extensio,
                       (size_t)ext_len) == ZEPHYRUM)
            {
                redde MIMINA[i].mimen;
            }
        }
    }

    redde "application/octet-stream";
}
