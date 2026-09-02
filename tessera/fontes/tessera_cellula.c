/* tessera_cellula.c - Implementatio cellulae (Phase A) */

#include "tessera_cellula.h"

TesseraStilus
tessera_stilus (
    i32 color_litterae,
    i32 color_fundi,
    i32 ornamenta)
{
    TesseraStilus stilus;

    stilus.color_litterae  = color_litterae;
    stilus.color_fundi     = color_fundi;
    stilus.ornamenta       = ornamenta;
    redde stilus;
}

TesseraStilus
tessera_stilus_nativus (vacuum)
{
    redde tessera_stilus(TESSERA_COLOR_NATIVUS, TESSERA_COLOR_NATIVUS,
        ZEPHYRUM);
}

b32
tessera_stilus_aequalis (
    TesseraStilus a,
    TesseraStilus b)
{
    redde (a.color_litterae == b.color_litterae
        && a.color_fundi == b.color_fundi
        && a.ornamenta == b.ornamenta) ? VERUM : FALSUM;
}

i32
tessera_signum_ex_octetis (
    constans i8* octeti,
            i32  numerus)
{
    i32 signum = ZEPHYRUM;
    i32 k;

    si (octeti == NIHIL || numerus == ZEPHYRUM || numerus > IV)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        signum |= ((i32)octeti[k]) << (VIII * k);
    }
    redde signum;
}

i32
tessera_signum_mensura (
    i32 signum)
{
    si (signum == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si ((signum >> VIII) == ZEPHYRUM)
    {
        redde I;
    }
    si ((signum >> XVI) == ZEPHYRUM)
    {
        redde II;
    }
    si ((signum >> XXIV) == ZEPHYRUM)
    {
        redde III;
    }
    redde IV;
}

vacuum
tessera_signum_scribere (
    ChordaAedificator* aed,
                  i32  signum)
{
    i32 mensura;
    i32 k;

    mensura = tessera_signum_mensura(signum);
    si (mensura == ZEPHYRUM)
    {
        chorda_aedificator_appendere_character(aed, ' ');
        redde;
    }
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        chorda_aedificator_appendere_character(aed,
            (character)((signum >> (VIII * k)) & 0xFF));
    }
}
