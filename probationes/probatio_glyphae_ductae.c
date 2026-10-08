/* probatio_glyphae_ductae.c - characteres capsarum, quadrorum et
 * braille ut larvae (aemulator-plan D7c)
 *
 * Larvae exspectatae ut picturae: '.' vacuum, '#' plenum, '1' '2' '3'
 * umbrae (0x40 0x80 0xC0, Ghostty Shade). Valores manu derivati ex
 * algorithmo Ghostty (font/sprite/draw/box.zig linesChar, dash*,
 * block.zig, braille.zig) ad cellulam VI x VIII: linea levis ordine III
 * columna II, gravis ordinibus III-IV columnis II-III, duplex ordinibus
 * II et IV columnis I et III. Leges per tabulam totam: margo cellulae
 * unam ex quattuor formis lineae habet (continuitas inter cellulas);
 * nihil extra larvam scribitur, quaevis magnitudo. */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "glyphae_ductae.h"
#include <stdio.h>
#include <string.h>

#define LAT       VI
#define ALT       VIII
#define MAXIMA    MXXIV
#define CUSTOS    0xA5

hic_manens i8 larva[MAXIMA + XVI];

interior i8
valor_signi (
    character s)
{
    commutatio (s)
    {
        casus '#': redde (i8)0xFF;
        casus '1': redde (i8)0x40;
        casus '2': redde (i8)0x80;
        casus '3': redde (i8)0xC0;
        ordinarius: redde ZEPHYRUM;
    }
}

interior character
signum_valoris (
    i8 v)
{
    commutatio (v)
    {
        casus 0x00: redde '.';
        casus 0xFF: redde '#';
        casus 0x40: redde '1';
        casus 0x80: redde '2';
        casus 0xC0: redde '3';
        ordinarius: redde '?';
    }
}

/* larva runae VI x VIII == pictura; discrepantia imprimitur */
interior b32
pictura_est (
                     s32   runa,
      constans character** pictura)
{
    i32 x;
    i32 y;
    b32 bonum;

    memset(larva, CUSTOS, magnitudo(larva));
    si (!glyphae_ductae_pingere(runa, LAT, ALT, larva))
    {
        imprimere("  U+%04X: non pingitur\n", (unsigned)runa);
        redde FALSUM;
    }
    bonum = VERUM;
    per (y = ZEPHYRUM; y < ALT; y++)
    {
        per (x = ZEPHYRUM; x < LAT; x++)
        {
            si (larva[y * LAT + x] != valor_signi(pictura[y][x]))
            {
                bonum = FALSUM;
            }
        }
    }
    si (!bonum)
    {
        imprimere("  U+%04X: exspectatum | receptum\n", (unsigned)runa);
        per (y = ZEPHYRUM; y < ALT; y++)
        {
            imprimere("    %s | ", pictura[y]);
            per (x = ZEPHYRUM; x < LAT; x++)
            {
                imprimere("%c", signum_valoris(larva[y * LAT + x]));
            }
            imprimere("\n");
        }
    }
    redde bonum;
}

interior vacuum
lineas_probare (vacuum)
{
    constans character* horizontalis[] = {
        "......", "......", "......", "######",
        "......", "......", "......", "......" };
    constans character* verticalis[] = {
        "..#...", "..#...", "..#...", "..#...",
        "..#...", "..#...", "..#...", "..#..." };
    constans character* angulus[] = {
        "......", "......", "......", "..####",
        "..#...", "..#...", "..#...", "..#..." };
    constans character* gravis[] = {
        "......", "......", "......", "######",
        "######", "......", "......", "......" };
    constans character* gravis_verticalis[] = {
        "..##..", "..##..", "..##..", "..##..",
        "..##..", "..##..", "..##..", "..##.." };
    constans character* crux[] = {
        "..#...", "..#...", "..#...", "######",
        "..#...", "..#...", "..#...", "..#..." };
    constans character* duplicata[] = {
        "......", "......", "######", "......",
        "######", "......", "......", "......" };
    constans character* duplex_verticalis[] = {
        ".#.#..", ".#.#..", ".#.#..", ".#.#..",
        ".#.#..", ".#.#..", ".#.#..", ".#.#.." };
    constans character* duplex_angulus[] = {
        "......", "......", ".#####", ".#....",
        ".#.###", ".#.#..", ".#.#..", ".#.#.." };
    constans character* dimidia_sinistra[] = {
        "......", "......", "......", "###...",
        "......", "......", "......", "......" };

    imprimere("\n--- I: lineae (Ghostty linesChar) ---\n");
    CREDO_VERUM(pictura_est(0x2500, horizontalis));
    CREDO_VERUM(pictura_est(0x2502, verticalis));
    CREDO_VERUM(pictura_est(0x250C, angulus));
    CREDO_VERUM(pictura_est(0x2501, gravis));
    CREDO_VERUM(pictura_est(0x2503, gravis_verticalis));
    CREDO_VERUM(pictura_est(0x253C, crux));
    CREDO_VERUM(pictura_est(0x2550, duplicata));
    CREDO_VERUM(pictura_est(0x2551, duplex_verticalis));
    CREDO_VERUM(pictura_est(0x2554, duplex_angulus));
    CREDO_VERUM(pictura_est(0x2574, dimidia_sinistra));
}

interior vacuum
interruptas_probare (vacuum)
{
    constans character* tres[] = {
        "......", "......", "......", "#.#.#.",
        "......", "......", "......", "......" };
    constans character* duae[] = {
        "......", "......", "......", "##.##.",
        "......", "......", "......", "......" };
    constans character* tres_verticales[] = {
        "..#...", "..#...", "......", "..#...",
        "..#...", "......", "..#...", "......" };
    constans character* quattuor_verticales[] = {
        "..#...", "......", "..#...", "......",
        "..#...", "......", "..#...", "......" };
    /* IV lineolae in VI pixelis non capiunt: Ghostty lineam levem
     * plenam ducit (hlineMiddle) */
    constans character* plena[] = {
        "......", "......", "......", "######",
        "......", "......", "......", "......" };

    imprimere("\n--- II: lineolae (Ghostty dash*) ---\n");
    CREDO_VERUM(pictura_est(0x2504, tres));
    CREDO_VERUM(pictura_est(0x254C, duae));
    CREDO_VERUM(pictura_est(0x2506, tres_verticales));
    CREDO_VERUM(pictura_est(0x250A, quattuor_verticales));
    CREDO_VERUM(pictura_est(0x2508, plena));
}

/* arcus et diagonales: Ghostty cum levigatione pingit, nos sine -
 * picturae oculis inspectae (6x8 et 12x16, D7c) deinde pinnatae; arcus
 * lineas puras in ordine III et columna II tangunt */
interior vacuum
arcus_probare (vacuum)
{
    constans character* arcus_dextrorsum_deorsum[] = {
        "......", "......", "......", "...###",
        "..##..", "..#...", "..#...", "..#..." };
    constans character* arcus_sinistrorsum_sursum[] = {
        "..#...", "..#...", ".##...", "##....",
        "......", "......", "......", "......" };
    constans character* ascendens[] = {
        ".....#", "....#.", "....#.", "...#..",
        "..#...", ".#....", "##....", "#....." };
    constans character* crux_obliqua[] = {
        "#....#", "##..#.", ".#..#.", "..##..",
        "..##..", ".#..#.", "##..#.", "#....#" };

    imprimere("\n--- II b: arcus et diagonales (pinnata oculis) ---\n");
    CREDO_VERUM(pictura_est(0x256D, arcus_dextrorsum_deorsum));
    CREDO_VERUM(pictura_est(0x256F, arcus_sinistrorsum_sursum));
    CREDO_VERUM(pictura_est(0x2571, ascendens));
    CREDO_VERUM(pictura_est(0x2573, crux_obliqua));
    /* limen strictum: ╳ 12x16 ordo IX "....#..#....", non "####" */
    CREDO_VERUM(glyphae_ductae_pingere(0x2573, XII, XVI, larva));
    CREDO_AEQUALIS_I32((i32)larva[IX * XII + IV], 0xFF);
    CREDO_AEQUALIS_I32((i32)larva[IX * XII + V], ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)larva[IX * XII + VI], ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)larva[IX * XII + VII], 0xFF);
}

interior vacuum
quadra_probare (vacuum)
{
    constans character* superum[] = {
        "######", "######", "######", "######",
        "......", "......", "......", "......" };
    constans character* inferum[] = {
        "......", "......", "......", "......",
        "######", "######", "######", "######" };
    constans character* sinistrum[] = {
        "###...", "###...", "###...", "###...",
        "###...", "###...", "###...", "###..." };
    constans character* dextrum[] = {
        "...###", "...###", "...###", "...###",
        "...###", "...###", "...###", "...###" };
    constans character* octava_infima[] = {
        "......", "......", "......", "......",
        "......", "......", "......", "######" };
    constans character* octava_sinistra[] = {
        "#.....", "#.....", "#.....", "#.....",
        "#.....", "#.....", "#.....", "#....." };
    constans character* quadrantes[] = {
        "######", "######", "######", "######",
        "###...", "###...", "###...", "###..." };
    constans character* plenum[] = {
        "######", "######", "######", "######",
        "######", "######", "######", "######" };
    constans character* umbra_levis[] = {
        "111111", "111111", "111111", "111111",
        "111111", "111111", "111111", "111111" };
    constans character* umbra_media[] = {
        "222222", "222222", "222222", "222222",
        "222222", "222222", "222222", "222222" };
    constans character* umbra_densa[] = {
        "333333", "333333", "333333", "333333",
        "333333", "333333", "333333", "333333" };

    imprimere("\n--- III: elementa quadrata (Ghostty block.zig) ---\n");
    CREDO_VERUM(pictura_est(0x2580, superum));
    CREDO_VERUM(pictura_est(0x2584, inferum));
    CREDO_VERUM(pictura_est(0x258C, sinistrum));
    CREDO_VERUM(pictura_est(0x2590, dextrum));
    CREDO_VERUM(pictura_est(0x2581, octava_infima));
    CREDO_VERUM(pictura_est(0x258F, octava_sinistra));
    CREDO_VERUM(pictura_est(0x259B, quadrantes));
    CREDO_VERUM(pictura_est(0x2588, plenum));
    CREDO_VERUM(pictura_est(0x2591, umbra_levis));
    CREDO_VERUM(pictura_est(0x2592, umbra_media));
    CREDO_VERUM(pictura_est(0x2593, umbra_densa));
}

interior vacuum
puncta_probare (vacuum)
{
    constans character* omnia[] = {
        ".#..#.", "......", ".#..#.", "......",
        ".#..#.", "......", ".#..#.", "......" };
    constans character* primum[] = {
        ".#....", "......", "......", "......",
        "......", "......", "......", "......" };
    constans character* ultimum[] = {
        "......", "......", "......", "......",
        "......", "......", "....#.", "......" };
    constans character* vacuum_[] = {
        "......", "......", "......", "......",
        "......", "......", "......", "......" };

    imprimere("\n--- IV: braille (Ghostty braille.zig) ---\n");
    CREDO_VERUM(pictura_est(0x28FF, omnia));
    CREDO_VERUM(pictura_est(0x2801, primum));
    CREDO_VERUM(pictura_est(0x2880, ultimum));
    CREDO_VERUM(pictura_est(0x2800, vacuum_));
}

/* ambitus: solum U+2500-259F et U+2800-28FF; aliena larvam non
 * tangunt */
interior vacuum
ambitum_probare (vacuum)
{
    i32 k;
    b32 intacta;

    imprimere("\n--- V: ambitus ---\n");
    CREDO_FALSUM(glyphae_ductae_est(0x24FF));
    CREDO_VERUM(glyphae_ductae_est(0x2500));
    CREDO_VERUM(glyphae_ductae_est(0x257F));
    CREDO_VERUM(glyphae_ductae_est(0x2580));
    CREDO_VERUM(glyphae_ductae_est(0x259F));
    /* 0x25A0 ■ symbolum v2 est; finis quadrorum per 0x25A2 */
    CREDO_FALSUM(glyphae_ductae_est(0x25A2));
    CREDO_FALSUM(glyphae_ductae_est(0x27FF));
    CREDO_VERUM(glyphae_ductae_est(0x2800));
    CREDO_VERUM(glyphae_ductae_est(0x28FF));
    CREDO_FALSUM(glyphae_ductae_est(0x2900));
    CREDO_FALSUM(glyphae_ductae_est('A'));
    memset(larva, CUSTOS, magnitudo(larva));
    CREDO_FALSUM(glyphae_ductae_pingere('A', LAT, ALT, larva));
    CREDO_FALSUM(glyphae_ductae_pingere(0x2500, ZEPHYRUM, ALT, larva));
    CREDO_FALSUM(glyphae_ductae_pingere(0x2500, LAT, ZEPHYRUM, larva));
    intacta = VERUM;
    per (k = ZEPHYRUM; k < (i32)magnitudo(larva); k++)
    {
        si (larva[k] != CUSTOS)
        {
            intacta = FALSUM;
        }
    }
    CREDO_VERUM(intacta);
}

/* margo cellulae: columna (aut ordo) ut vector */
interior vacuum
marginem_legere (
    i32  lat,
    i32  alt,
    b32  verticalis,
    i32  index,
     i8* exitus)
{
    i32 k;
    i32 n;

    n = verticalis ? alt : lat;
    per (k = ZEPHYRUM; k < n; k++)
    {
        exitus[k] = verticalis ? larva[k * lat + index]
                               : larva[index * lat + k];
    }
}

/* lex continuitatis: omnis character linearum (non lineolae, arcus,
 * diagonales) in quoque margine habet formam vacuam aut formam
 * marginis lineae purae (─ ━ ═ dextrorsum/sinistrorsum,
 * │ ┃ ║ sursum/deorsum) - aliter linea inter cellulas
 * frangitur */
interior b32
continuitas (
    i32 lat,
    i32 alt)
{
     i8 formae[IV][IV][LXIV];   /* margo (s d i l), stilus, vector */
     i8 margo[LXIV];
    s32 purae_h[III];
    s32 purae_v[III];
    s32 runa;
    i32 m;
    i32 s;
    i32 n;
    b32 inventa;
    b32 bonum;

    purae_h[ZEPHYRUM]  = 0x2500;
    purae_h[I]         = 0x2501;
    purae_h[II]        = 0x2550;
    purae_v[ZEPHYRUM]  = 0x2502;
    purae_v[I]         = 0x2503;
    purae_v[II]        = 0x2551;
    memset(formae, ZEPHYRUM, magnitudo(formae));
    per (s = ZEPHYRUM; s < III; s++)
    {
        (vacuum)glyphae_ductae_pingere(purae_v[s], lat, alt, larva);
        marginem_legere(lat, alt, FALSUM, ZEPHYRUM, formae[ZEPHYRUM][s
            + I]);
        marginem_legere(lat, alt, FALSUM, alt - I, formae[II][s + I]);
        (vacuum)glyphae_ductae_pingere(purae_h[s], lat, alt, larva);
        marginem_legere(lat, alt, VERUM, lat - I, formae[I][s + I]);
        marginem_legere(lat, alt, VERUM, ZEPHYRUM, formae[III][s + I]);
    }
    bonum = VERUM;
    per (runa = 0x2500; runa < 0x2580; runa++)
    {
        si (   (runa >= 0x2504 && runa <= 0x250B)
            || (runa >= 0x254C && runa <= 0x254F)
            || (runa >= 0x256D && runa <= 0x2573))
        {
            perge;
        }
        (vacuum)glyphae_ductae_pingere(runa, lat, alt, larva);
        per (m = ZEPHYRUM; m < IV; m++)
        {
            n = (m == I || m == III) ? alt : lat;
            marginem_legere(lat, alt, m == I || m == III,
                m == ZEPHYRUM ? ZEPHYRUM
                : m == I ? lat - I
                : m == II ? alt - I : ZEPHYRUM, margo);
            inventa = FALSUM;
            per (s = ZEPHYRUM; s < IV; s++)
            {
                si (memcmp(margo, formae[m][s], (memoriae_index)n)
                    == ZEPHYRUM)
                {
                    inventa = VERUM;
                }
            }
            si (!inventa)
            {
                imprimere("  U+%04X %ux%u: margo %u fractus\n",
                    (unsigned)runa, (unsigned)lat, (unsigned)alt,
                    (unsigned)m);
                bonum = FALSUM;
            }
        }
    }
    redde bonum;
}

/* nihil extra larvam, quaevis magnitudo (etiam I x I) */
interior b32
intra_larvam (
    i32 lat,
    i32 alt)
{
    s32 runa;
    i32 k;

    per (runa = 0x2500; runa <= 0x28FF; runa++)
    {
        si (!glyphae_ductae_est(runa))
        {
            perge;
        }
        memset(larva, CUSTOS, magnitudo(larva));
        (vacuum)glyphae_ductae_pingere(runa, lat, alt, larva);
        per (k = lat * alt; k < (i32)magnitudo(larva); k++)
        {
            si (larva[k] != CUSTOS)
            {
                imprimere("  U+%04X %ux%u: extra larvam\n",
                    (unsigned)runa, (unsigned)lat, (unsigned)alt);
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

interior vacuum
leges_probare (vacuum)
{
    imprimere("\n--- VI: leges per tabulam ---\n");
    CREDO_VERUM(continuitas(VI, VIII));
    CREDO_VERUM(continuitas(VII, IX));
    CREDO_VERUM(continuitas(XII, XVI));
    CREDO_VERUM(continuitas(IX, XX));
    CREDO_VERUM(continuitas(XX, XL));
    CREDO_VERUM(intra_larvam(I, I));
    CREDO_VERUM(intra_larvam(II, III));
    CREDO_VERUM(intra_larvam(VI, VIII));
    CREDO_VERUM(intra_larvam(XVII, XXXI));
}

/* VII: symbola (v2, census Claude Code): picturae VI x VIII manu
 * ductae, scalatae per proximum; lex: quodque symbolum agnoscitur et
 * aliquid pingit */
interior vacuum
symbola_probare (vacuum)
{
    hic_manens constans s32 symbola[] = {
        0x2013, 0x2014, 0x2022, 0x2026, 0x203A, 0x2190, 0x2191,
        0x2192, 0x2193, 0x21B5, 0x21E7, 0x2219, 0x2261, 0x2264,
        0x2265, 0x22EE, 0x2318, 0x2325, 0x23BF, 0x23CE, 0x23F5,
        0x23F8, 0x23FA, 0x25A0, 0x25A1, 0x25AA, 0x25B2, 0x25B6,
        0x25B8, 0x25BC, 0x25C9, 0x25CB, 0x25CF, 0x25D0, 0x25D1,
        0x25D2, 0x25D3, 0x25EF, 0x2605, 0x2610, 0x2612, 0x26A0,
        0x2713, 0x2714, 0x2715, 0x2716, 0x2717, 0x2718, 0x2722,
        0x2726, 0x2733, 0x2736, 0x273B, 0x273D, 0x276F, 0x29C9,
        0x2B24, ZEPHYRUM };
    constans character* nota[] = {
        "......", "......", ".....#", "....#.",
        ".#.#..", "..#...", "......", "......" };
    constans character* circulus[] = {
        "......", "..###.", ".#####", ".#####",
        ".#####", "..###.", "......", "......" };
    i32 k;
    i32 j;
    b32 omnia;
    b32 pictum;

    imprimere("\n--- VII: symbola (v2) ---\n");
    CREDO_VERUM(pictura_est(0x2713, nota));
    CREDO_VERUM(pictura_est(0x23FA, circulus));
    /* XII x XVI = II x: pixelum (x, y) = pictura[y / II][x / II] */
    CREDO_VERUM(glyphae_ductae_pingere(0x25CF, XII, XVI, larva));
    CREDO_AEQUALIS_I32((i32)larva[II * XII + IV], 0xFF);
    CREDO_AEQUALIS_I32((i32)larva[II * XII + III], ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)larva[III * XII + V], 0xFF);
    CREDO_AEQUALIS_I32((i32)larva[ZEPHYRUM * XII + V], ZEPHYRUM);
    omnia = VERUM;
    per (k = ZEPHYRUM; symbola[k]; k++)
    {
        si (   !glyphae_ductae_est(symbola[k])
            || !glyphae_ductae_pingere(symbola[k], LAT, ALT, larva))
        {
            imprimere("  U+%04X: non agnitum\n", (unsigned)symbola[k]);
            omnia = FALSUM;
            perge;
        }
        pictum = FALSUM;
        per (j = ZEPHYRUM; j < LAT * ALT; j++)
        {
            si (larva[j] != ZEPHYRUM)
            {
                pictum = VERUM;
            }
        }
        si (!pictum)
        {
            imprimere("  U+%04X: vacuum\n", (unsigned)symbola[k]);
            omnia = FALSUM;
        }
    }
    CREDO_VERUM(omnia);
    CREDO_AEQUALIS_I32(k, LVII);
    CREDO_FALSUM(glyphae_ductae_est(0x2012));
    CREDO_FALSUM(glyphae_ductae_est(0x2600));
    CREDO_FALSUM(glyphae_ductae_est(0x2B23));
    CREDO_FALSUM(glyphae_ductae_est(0x2B25));
}

/* VIII: symbola v3 (Franus 2026-10-08, tofu in terminale): Graeca
 * Latinis dissimilia, numeri supra et infra scripti, radices, >> et
 * <<. Similia (Alpha, omicron) per fons, non hic. */
interior vacuum
graeca_probare (vacuum)
{
    hic_manens constans s32 nova[] = {
        0x00B2, 0x00B3, 0x00B9, 0x0393, 0x0394, 0x0398, 0x039B,
        0x039E, 0x03A0, 0x03A3, 0x03A6, 0x03A8, 0x03A9, 0x03B1,
        0x03B2, 0x03B3, 0x03B4, 0x03B5, 0x03B6, 0x03B7, 0x03B8,
        0x03B9, 0x03BA, 0x03BB, 0x03BC, 0x03BD, 0x03BE, 0x03C0,
        0x03C1, 0x03C2, 0x03C3, 0x03C4, 0x03C5, 0x03C6, 0x03C7,
        0x03C8, 0x03C9, 0x2070, 0x2074, 0x2075, 0x2076, 0x2077,
        0x2078, 0x2079, 0x207A, 0x207B, 0x207F, 0x2080, 0x2081,
        0x2082, 0x2083, 0x2084, 0x2085, 0x2086, 0x2087, 0x2088,
        0x2089, 0x221A, 0x221B, 0x226A, 0x226B, ZEPHYRUM };
    constans character* pi[] = {
        "......", "......", ".#####", "..#.#.",
        "..#.#.", "..#.#.", "..#..#", "......" };
    constans character* duo_supra[] = {
        ".###..", "...#..", ".###..", ".#....",
        ".###..", "......", "......", "......" };
    i32 k;
    i32 j;
    b32 omnia;
    b32 situs;

    imprimere("\n--- VIII: symbola v3 (Graeca, mathematica) ---\n");
    CREDO_VERUM(pictura_est(0x03C0, pi));
    CREDO_VERUM(pictura_est(0x00B2, duo_supra));
    omnia = VERUM;
    situs = VERUM;
    per (k = ZEPHYRUM; nova[k]; k++)
    {
        si (   !glyphae_ductae_est(nova[k])
            || !glyphae_ductae_pingere(nova[k], LAT, ALT, larva))
        {
            imprimere("  U+%04X: non agnitum\n", (unsigned)nova[k]);
            omnia = FALSUM;
            perge;
        }
        /* supra scripti: ordines V-VII vacui; infra: 0-II vacui */
        per (j = ZEPHYRUM; j < LAT * ALT; j++)
        {
            si (larva[j] == ZEPHYRUM)
            {
                perge;
            }
            si (   (   nova[k] == 0x00B2 || nova[k] == 0x00B3
                    || nova[k] == 0x00B9
                    || (nova[k] >= 0x2070 && nova[k] <= 0x207F))
                && j / LAT >= V)
            {
                situs = FALSUM;
            }
            si (   nova[k] >= 0x2080 && nova[k] <= 0x2089
                && j / LAT < III)
            {
                situs = FALSUM;
            }
        }
    }
    CREDO_VERUM(omnia);
    CREDO_VERUM(situs);
    CREDO_AEQUALIS_I32(k, LXI);
    /* similia Latinis per fons; vicini non assignati */
    CREDO_FALSUM(glyphae_ductae_est(0x0391));
    CREDO_FALSUM(glyphae_ductae_est(0x03BF));
    CREDO_FALSUM(glyphae_ductae_est(0x03A2));
    CREDO_FALSUM(glyphae_ductae_est(0x03CA));
    CREDO_FALSUM(glyphae_ductae_est(0x208A));
    CREDO_FALSUM(glyphae_ductae_est(0x221C));
}

s32
principale (vacuum)
{
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_glyphae_ductae",
        MXXIV * LXIV);
    credo_aperire(piscina);

    lineas_probare();
    interruptas_probare();
    arcus_probare();
    quadra_probare();
    puncta_probare();
    ambitum_probare();
    leges_probare();
    symbola_probare();
    graeca_probare();

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
