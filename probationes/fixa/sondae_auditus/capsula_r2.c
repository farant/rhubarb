/* sonda capsula_r2: capsula_aperire TOC contra mensuram blobi non
 * verificat? Blob XII byteorum (magica + versio + numerus=I), et blob
 * cum path_offset ultra finem. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "capsula.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens vacuum
_scribere_i32 (i8* p, i32 v)
{
    p[0] = (i8)(v & 0xFF);
    p[1] = (i8)((v >> 8) & 0xFF);
    p[2] = (i8)((v >> 16) & 0xFF);
    p[3] = (i8)((v >> 24) & 0xFF);
}

integer
principale (integer argc, character** argv)
{
    Piscina*     piscina = piscina_generare_dynamicum("sonda", 65536);
    CapsulaEmbed embed;
    Capsula*     cap;
    i8*          blob;

    si (argc > 1 && strcmp(argv[1], "offset") == 0)
    {
        /* 32 bytes: header + una TOC; path_offset = 1000000 */
        blob = (i8*)malloc(32);
        memset(blob, 0, 32);
        _scribere_i32(blob, (i32)CAPSULA_MAGICA);
        _scribere_i32(blob + 4, (i32)CAPSULA_VERSIO);
        _scribere_i32(blob + 8, 1);
        _scribere_i32(blob + 12, 1000000);  /* path_offset */
        _scribere_i32(blob + 16, 4);        /* path_len */
        embed.datum   = blob;
        embed.mensura = 32;
        imprimere("blob 32 byte, path_offset 1000000\n");
    }
    alioquin
    {
        /* 12 bytes: header solum, numerus = 1 -> TOC[0] ultra finem */
        blob = (i8*)malloc(12);
        _scribere_i32(blob, (i32)CAPSULA_MAGICA);
        _scribere_i32(blob + 4, (i32)CAPSULA_VERSIO);
        _scribere_i32(blob + 8, 1);
        embed.datum   = blob;
        embed.mensura = 12;
        imprimere("blob 12 byte, numerus 1\n");
    }

    cap = capsula_aperire(&embed, piscina);
    imprimere("capsula_aperire -> %p\n", (vacuum*)cap);
    free(blob);
    piscina_destruere(piscina);
    redde 0;
}
