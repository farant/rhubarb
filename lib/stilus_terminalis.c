/* stilus_terminalis.c - Vide stilus_terminalis.h */

#include "stilus_terminalis.h"
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
_colores_aequales (
    StilusColor a,
    StilusColor b)
{
    si (a.genus != b.genus)
    {
        redde FALSUM;
    }
    /* NATIVUS: valor nihil significat */
    redde (b32)(a.genus == STILUS_COLOR_NATIVUS || a.valor == b.valor);
}

/* Color SGR: praefixum (";38;", ";48;", ";58;") + "5;n" aut "2;r;g;b";
 * NATIVUS nihil (reditio nuda defaltam dat) */
interior vacuum
_colorem (
      ChordaAedificator* a,
     constans character* praefixum,
            StilusColor  c,
                    i32  codificatio)
{
    si (c.genus == STILUS_COLOR_NATIVUS)
    {
        redde;
    }
    chorda_aedificator_appendere_literis(a, praefixum);
    si (c.genus == STILUS_COLOR_TABULA)
    {
        chorda_aedificator_appendere_literis(a, "5;");
        chorda_aedificator_appendere_i32(a, c.valor & 0xFF);
        redde;
    }
    si (codificatio == STILUS_CODIFICATIO_CCLVI)
    {
        chorda_aedificator_appendere_literis(a, "5;");
        chorda_aedificator_appendere_i32(a, stilus_quantizare(c.valor));
        redde;
    }
    chorda_aedificator_appendere_literis(a, "2;");
    chorda_aedificator_appendere_i32(a, (c.valor >> XVI) & 0xFF);
    chorda_aedificator_appendere_character(a, ';');
    chorda_aedificator_appendere_i32(a, (c.valor >> VIII) & 0xFF);
    chorda_aedificator_appendere_character(a, ';');
    chorda_aedificator_appendere_i32(a, c.valor & 0xFF);
}


/* ==================================================
 * Publica
 * ================================================== */

vacuum
stilus_nativus (
    StilusTerminalis* st)
{
    memset(st, ZEPHYRUM, magnitudo(StilusTerminalis));
}

b32
stilus_aequalis (
    constans StilusTerminalis* a,
    constans StilusTerminalis* b)
{
    redde (b32)(   a->ornamenta == b->ornamenta
                && a->sublinea == b->sublinea
                && _colores_aequales(a->color_litterae,
                b->color_litterae)
                && _colores_aequales(a->color_fundi, b->color_fundi)
                && _colores_aequales(a->color_sublineae,
                       b->color_sublineae));
}

vacuum
stilus_codificare (
            ChordaAedificator* a,
    constans StilusTerminalis* prior,
    constans StilusTerminalis* st,
                          i32  codificatio)
{
    i32 o = st->ornamenta;

    si (prior && stilus_aequalis(prior, st))
    {
        redde;      /* v1: differentia nulla = nihil; aliter plena */
    }
    chorda_aedificator_appendere_literis(a, "\033[0");
    si (o & STILUS_CRASSUM)
    { chorda_aedificator_appendere_literis(a, ";1");
    }
    si (o & STILUS_OBSCURUM)
    { chorda_aedificator_appendere_literis(a, ";2");
    }
    si (o & STILUS_CURSIVUM)
    { chorda_aedificator_appendere_literis(a, ";3");
    }
    si (st->sublinea == STILUS_SUBLINEA_SIMPLEX)
    {
        chorda_aedificator_appendere_literis(a, ";4");
    }
    alioquin si (st->sublinea != STILUS_SUBLINEA_NULLA)
    {
        chorda_aedificator_appendere_literis(a, ";4:");
        chorda_aedificator_appendere_i32(a, (i32)st->sublinea);
    }
    si (o & STILUS_NICTANS)
    { chorda_aedificator_appendere_literis(a, ";5");
    }
    si (o & STILUS_INVERSUM)
    { chorda_aedificator_appendere_literis(a, ";7");
    }
    si (o & STILUS_INVISIBILE)
    { chorda_aedificator_appendere_literis(a, ";8");
    }
    si (o & STILUS_TRANSFIXUM)
    { chorda_aedificator_appendere_literis(a, ";9");
    }
    si (o & STILUS_SUPERLINEA)
    { chorda_aedificator_appendere_literis(a, ";53");
    }
    _colorem(a, ";38;", st->color_litterae, codificatio);
    _colorem(a, ";48;", st->color_fundi, codificatio);
    _colorem(a, ";58;", st->color_sublineae, codificatio);
    chorda_aedificator_appendere_character(a, 'm');
}

i32
stilus_tabulae_color (
    s32 index)
{
    /* XVI PRIMAE: valores xterm defalti */
    hic_manens constans i32 PRIMAE[XVI] = {
        0x000000, 0xCD0000, 0x00CD00, 0xCDCD00,
        0x0000EE, 0xCD00CD, 0x00CDCD, 0xE5E5E5,
        0x7F7F7F, 0xFF0000, 0x00FF00, 0xFFFF00,
        0x5C5CFF, 0xFF00FF, 0x00FFFF, 0xFFFFFF
    };
    hic_manens constans i32 GRADUS[VI] = { 0, 95, 135, 175, 215, 255 };

    si (index < ZEPHYRUM || index > CCLV)
    {
        redde ZEPHYRUM;
    }
    si (index < XVI)
    {
        redde PRIMAE[index];
    }
    si (index < CCXXXII)
    {
        s32 k = index - XVI;

        redde (GRADUS[k / XXXVI] << XVI) | (GRADUS[(k / VI) % VI]
            << VIII)
            | GRADUS[k % VI];
    }
    {
        i32 v = (i32)(VIII + X * (index - CCXXXII));

        redde (v << XVI) | (v << VIII) | v;
    }
}

i32
stilus_quantizare (
    i32 rgb)
{
    /* tessera _cclvi (quadrans Q4) translatus: cubus (in aequalitate
     * gradus inferior) aut griseus (8 + 10k, proximus mediae RGB);
     * aequalitas distantiarum: cubus */
    hic_manens constans i32 GRADUS[VI] = { 0, 95, 135, 175, 215, 255 };
                        i32 index[III];
                        i32 d_cubi    = ZEPHYRUM;
                        i32 d_grisei  = ZEPHYRUM;
                        i32 summa     = ZEPHYRUM;
                        i32 griseus;
                        i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 c = (s32)((rgb >> (XVI - VIII * k)) & 0xFF);
        s32 d;
        i32 j;

        index[k] = ZEPHYRUM;
        per (j = I; j < VI; j++)
        {
            s32 prior  = c - (s32)GRADUS[index[k]];
            s32 hic    = c - (s32)GRADUS[j];

            si (hic * hic < prior * prior)
            {
                index[k] = j;
            }
        }
        d       = c - (s32)GRADUS[index[k]];
        d_cubi  += (i32)(d * d);
        summa   += (i32)c;
    }
    griseus = (summa / III < VIII) ? ZEPHYRUM
                                   : (summa / III - VIII + V) / X;
    si (griseus > XXIII)
    {
        griseus = XXIII;
    }
    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 d = (s32)((rgb >> (XVI - VIII * k)) & 0xFF)
            - (s32)(VIII + X * griseus);

        d_grisei += (i32)(d * d);
    }
    si (d_grisei < d_cubi)
    {
        redde CCXXXII + griseus;
    }
    redde XVI + XXXVI * index[0] + VI * index[1] + index[2];
}
