/* sonda flatura r1: stored block post dynamic block cuius EOB codex
 * brevis (1 bit) et maxima_bits XV - _flatura_saltare_ad_byte totum
 * bit_buffer abicit, octeti integri perduntur. */
#include "latina.h"
#include "piscina.h"
#include "flatura.h"
#include <stdio.h>
#include <string.h>

hic_manens i8  rivus[256];
hic_manens i32 rivus_pos = 0;
hic_manens i32 acc = 0;
hic_manens i32 acc_n = 0;

/* bits LSB primum */
interior vacuum
scr (i32 valor, i32 n)
{
    i32 k;
    per (k = 0; k < n; k++)
    {
        acc |= ((valor >> k) & 1) << acc_n;
        acc_n++;
        si (acc_n == 8)
        {
            rivus[rivus_pos++] = (i8)acc;
            acc = 0;
            acc_n = 0;
        }
    }
}

/* codex Huffman: MSB primum */
interior vacuum
scr_codex (i32 codex, i32 n)
{
    s32 k;
    per (k = (s32)n - 1; k >= 0; k--)
    {
        scr((codex >> k) & 1, 1);
    }
}

interior vacuum
align (vacuum)
{
    si (acc_n > 0)
    {
        rivus[rivus_pos++] = (i8)acc;
        acc = 0;
        acc_n = 0;
    }
}

/* code-length alphabet: symbola 1..15 et 18, omnes 4 bits:
 * canonicum: 1->0, 2->1, ..., 15->14, 18->15 */
interior vacuum
cl (i32 sym)
{
    scr_codex(sym == 18 ? 15 : sym - 1, 4);
}

integer
principale (vacuum)
{
    Piscina*       piscina;
    FlaturaFructus fr;
    hic_manens constans i8 ordo[19] = {
        16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
    i32 k;

    piscina = piscina_generare_dynamicum("sonda", 65536);

    /* block 1: BFINAL=0, BTYPE=2 (dynamic) */
    scr(0, 1);
    scr(2, 2);
    scr(0, 5);   /* HLIT = 257 */
    scr(1, 5);   /* HDIST = 2 */
    scr(15, 4);  /* HCLEN = 19 */
    per (k = 0; k < 19; k++)
    {
        i32 s = ordo[k];
        scr((s == 0 || s == 16 || s == 17) ? 0 : 4, 3);
    }
    /* literales 0..13 longitudines 2..15, 14 -> 15 */
    per (k = 0; k < 14; k++)
    {
        cl(k + 2);
    }
    cl(15);
    /* 15..255: 241 zeros = 138 + 103 */
    cl(18); scr(127, 7);
    cl(18); scr(92, 7);
    /* 256 (EOB) -> 1 */
    cl(1);
    /* distantiae 0,1 -> 1,1 */
    cl(1);
    cl(1);
    /* datum block 1: solum EOB = codex '0' (1 bit) */
    scr_codex(0, 1);

    /* block 2: BFINAL=1, BTYPE=0 stored "hi" */
    scr(1, 1);
    scr(0, 2);
    align();
    rivus[rivus_pos++] = 2;
    rivus[rivus_pos++] = 0;
    rivus[rivus_pos++] = (i8)0xFD;
    rivus[rivus_pos++] = (i8)0xFF;
    rivus[rivus_pos++] = 'h';
    rivus[rivus_pos++] = 'i';

    imprimere("rivus (%d): ", (int)rivus_pos);
    per (k = 0; k < rivus_pos; k++)
    {
        imprimere("%02x", (unsigned)(rivus[k] & 0xFF));
    }
    imprimere("\n");

    fr = flatura_inflare(rivus, rivus_pos, piscina);
    imprimere("status=%d (%s) mensura=%d\n", (int)fr.status,
        flatura_status_nuntium(fr.status), (int)fr.mensura);
    si (fr.status == FLATURA_STATUS_OK)
    {
        imprimere("datum='%.*s'\n", (int)fr.mensura, (character*)fr.datum);
    }
    piscina_destruere(piscina);
    redde 0;
}
