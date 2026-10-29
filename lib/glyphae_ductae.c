/* glyphae_ductae.c - characteres capsarum, quadrorum et braille ut
 * larvae (aemulator-plan D7c). Algorithmi Ghostty
 * (src/font/sprite/draw: box.zig, block.zig, braille.zig, common.zig)
 * translati; tabula brachiorum ex box.zig generata (scriptum in
 * lib/glyphae_ductae.worklog.md). */
#include "glyphae_ductae.h"
#include "fons_6x8.h"
#include <string.h>
#include <math.h>

/* stilus brachii (Ghostty Lines.Style) */
#define LINEA_NULLA      ZEPHYRUM
#define LINEA_LEVIS      I
#define LINEA_GRAVIS     II
#define LINEA_DUPLICATA  III

/* opacitates umbrarum (Ghostty Shade) */
#define UMBRA_LEVIS      0x40
#define UMBRA_MEDIA      0x80
#define UMBRA_DENSA      0xC0
#define PLENUM           0xFF

/* puncta viae arcus aut diagonalis (lineae + curva in XVI) */
#define VIA_PUNCTA_MAXIMA  XXIV
#define CURVA_GRADUS       XVI

/* U+2500-257F: brachia (superum | dextrum << 2 | inferum << 4 | laevum
 * << 6), stilus LINEA_*; 0 = lineolae, arcus, diagonales (a parte).
 * Commentarium: s d i l = superum dextrum inferum laevum. */
interior constans i8 brachia_capsarum[CXXVIII] = {
    0x44,   /* 2500 ─ s0 d1 i0 l1 */
    0x88,   /* 2501 ━ s0 d2 i0 l2 */
    0x11,   /* 2502 │ s1 d0 i1 l0 */
    0x22,   /* 2503 ┃ s2 d0 i2 l0 */
    0x00,   /* 2504 ┄ lineolae */
    0x00,   /* 2505 ┅ lineolae */
    0x00,   /* 2506 ┆ lineolae */
    0x00,   /* 2507 ┇ lineolae */
    0x00,   /* 2508 ┈ lineolae */
    0x00,   /* 2509 ┉ lineolae */
    0x00,   /* 250A ┊ lineolae */
    0x00,   /* 250B ┋ lineolae */
    0x14,   /* 250C ┌ s0 d1 i1 l0 */
    0x18,   /* 250D ┍ s0 d2 i1 l0 */
    0x24,   /* 250E ┎ s0 d1 i2 l0 */
    0x28,   /* 250F ┏ s0 d2 i2 l0 */
    0x50,   /* 2510 ┐ s0 d0 i1 l1 */
    0x90,   /* 2511 ┑ s0 d0 i1 l2 */
    0x60,   /* 2512 ┒ s0 d0 i2 l1 */
    0xA0,   /* 2513 ┓ s0 d0 i2 l2 */
    0x05,   /* 2514 └ s1 d1 i0 l0 */
    0x09,   /* 2515 ┕ s1 d2 i0 l0 */
    0x06,   /* 2516 ┖ s2 d1 i0 l0 */
    0x0A,   /* 2517 ┗ s2 d2 i0 l0 */
    0x41,   /* 2518 ┘ s1 d0 i0 l1 */
    0x81,   /* 2519 ┙ s1 d0 i0 l2 */
    0x42,   /* 251A ┚ s2 d0 i0 l1 */
    0x82,   /* 251B ┛ s2 d0 i0 l2 */
    0x15,   /* 251C ├ s1 d1 i1 l0 */
    0x19,   /* 251D ┝ s1 d2 i1 l0 */
    0x16,   /* 251E ┞ s2 d1 i1 l0 */
    0x25,   /* 251F ┟ s1 d1 i2 l0 */
    0x26,   /* 2520 ┠ s2 d1 i2 l0 */
    0x1A,   /* 2521 ┡ s2 d2 i1 l0 */
    0x29,   /* 2522 ┢ s1 d2 i2 l0 */
    0x2A,   /* 2523 ┣ s2 d2 i2 l0 */
    0x51,   /* 2524 ┤ s1 d0 i1 l1 */
    0x91,   /* 2525 ┥ s1 d0 i1 l2 */
    0x52,   /* 2526 ┦ s2 d0 i1 l1 */
    0x61,   /* 2527 ┧ s1 d0 i2 l1 */
    0x62,   /* 2528 ┨ s2 d0 i2 l1 */
    0x92,   /* 2529 ┩ s2 d0 i1 l2 */
    0xA1,   /* 252A ┪ s1 d0 i2 l2 */
    0xA2,   /* 252B ┫ s2 d0 i2 l2 */
    0x54,   /* 252C ┬ s0 d1 i1 l1 */
    0x94,   /* 252D ┭ s0 d1 i1 l2 */
    0x58,   /* 252E ┮ s0 d2 i1 l1 */
    0x98,   /* 252F ┯ s0 d2 i1 l2 */
    0x64,   /* 2530 ┰ s0 d1 i2 l1 */
    0xA4,   /* 2531 ┱ s0 d1 i2 l2 */
    0x68,   /* 2532 ┲ s0 d2 i2 l1 */
    0xA8,   /* 2533 ┳ s0 d2 i2 l2 */
    0x45,   /* 2534 ┴ s1 d1 i0 l1 */
    0x85,   /* 2535 ┵ s1 d1 i0 l2 */
    0x49,   /* 2536 ┶ s1 d2 i0 l1 */
    0x89,   /* 2537 ┷ s1 d2 i0 l2 */
    0x46,   /* 2538 ┸ s2 d1 i0 l1 */
    0x86,   /* 2539 ┹ s2 d1 i0 l2 */
    0x4A,   /* 253A ┺ s2 d2 i0 l1 */
    0x8A,   /* 253B ┻ s2 d2 i0 l2 */
    0x55,   /* 253C ┼ s1 d1 i1 l1 */
    0x95,   /* 253D ┽ s1 d1 i1 l2 */
    0x59,   /* 253E ┾ s1 d2 i1 l1 */
    0x99,   /* 253F ┿ s1 d2 i1 l2 */
    0x56,   /* 2540 ╀ s2 d1 i1 l1 */
    0x65,   /* 2541 ╁ s1 d1 i2 l1 */
    0x66,   /* 2542 ╂ s2 d1 i2 l1 */
    0x96,   /* 2543 ╃ s2 d1 i1 l2 */
    0x5A,   /* 2544 ╄ s2 d2 i1 l1 */
    0xA5,   /* 2545 ╅ s1 d1 i2 l2 */
    0x69,   /* 2546 ╆ s1 d2 i2 l1 */
    0x9A,   /* 2547 ╇ s2 d2 i1 l2 */
    0xA9,   /* 2548 ╈ s1 d2 i2 l2 */
    0xA6,   /* 2549 ╉ s2 d1 i2 l2 */
    0x6A,   /* 254A ╊ s2 d2 i2 l1 */
    0xAA,   /* 254B ╋ s2 d2 i2 l2 */
    0x00,   /* 254C ╌ lineolae */
    0x00,   /* 254D ╍ lineolae */
    0x00,   /* 254E ╎ lineolae */
    0x00,   /* 254F ╏ lineolae */
    0xCC,   /* 2550 ═ s0 d3 i0 l3 */
    0x33,   /* 2551 ║ s3 d0 i3 l0 */
    0x1C,   /* 2552 ╒ s0 d3 i1 l0 */
    0x34,   /* 2553 ╓ s0 d1 i3 l0 */
    0x3C,   /* 2554 ╔ s0 d3 i3 l0 */
    0xD0,   /* 2555 ╕ s0 d0 i1 l3 */
    0x70,   /* 2556 ╖ s0 d0 i3 l1 */
    0xF0,   /* 2557 ╗ s0 d0 i3 l3 */
    0x0D,   /* 2558 ╘ s1 d3 i0 l0 */
    0x07,   /* 2559 ╙ s3 d1 i0 l0 */
    0x0F,   /* 255A ╚ s3 d3 i0 l0 */
    0xC1,   /* 255B ╛ s1 d0 i0 l3 */
    0x43,   /* 255C ╜ s3 d0 i0 l1 */
    0xC3,   /* 255D ╝ s3 d0 i0 l3 */
    0x1D,   /* 255E ╞ s1 d3 i1 l0 */
    0x37,   /* 255F ╟ s3 d1 i3 l0 */
    0x3F,   /* 2560 ╠ s3 d3 i3 l0 */
    0xD1,   /* 2561 ╡ s1 d0 i1 l3 */
    0x73,   /* 2562 ╢ s3 d0 i3 l1 */
    0xF3,   /* 2563 ╣ s3 d0 i3 l3 */
    0xDC,   /* 2564 ╤ s0 d3 i1 l3 */
    0x74,   /* 2565 ╥ s0 d1 i3 l1 */
    0xFC,   /* 2566 ╦ s0 d3 i3 l3 */
    0xCD,   /* 2567 ╧ s1 d3 i0 l3 */
    0x47,   /* 2568 ╨ s3 d1 i0 l1 */
    0xCF,   /* 2569 ╩ s3 d3 i0 l3 */
    0xDD,   /* 256A ╪ s1 d3 i1 l3 */
    0x77,   /* 256B ╫ s3 d1 i3 l1 */
    0xFF,   /* 256C ╬ s3 d3 i3 l3 */
    0x00,   /* 256D ╭ arcus */
    0x00,   /* 256E ╮ arcus */
    0x00,   /* 256F ╯ arcus */
    0x00,   /* 2570 ╰ arcus */
    0x00,   /* 2571 ╱ diagonalis */
    0x00,   /* 2572 ╲ diagonalis */
    0x00,   /* 2573 ╳ diagonalis */
    0x40,   /* 2574 ╴ s0 d0 i0 l1 */
    0x01,   /* 2575 ╵ s1 d0 i0 l0 */
    0x04,   /* 2576 ╶ s0 d1 i0 l0 */
    0x10,   /* 2577 ╷ s0 d0 i1 l0 */
    0x80,   /* 2578 ╸ s0 d0 i0 l2 */
    0x02,   /* 2579 ╹ s2 d0 i0 l0 */
    0x08,   /* 257A ╺ s0 d2 i0 l0 */
    0x20,   /* 257B ╻ s0 d0 i2 l0 */
    0x48,   /* 257C ╼ s0 d2 i0 l1 */
    0x21,   /* 257D ╽ s1 d0 i2 l0 */
    0x84,   /* 257E ╾ s0 d1 i0 l2 */
    0x12,   /* 257F ╿ s2 d0 i1 l0 */
};

/* v2: symbola VI x VIII (ordines VIII, columnae VI; '#' plenum),
 * ORDINE RUNARUM (quaestio bipartita). Corpus in columnis I-V, ordines
 * 0-VI ut fons_6x8. Generata ex scripto symbola.py (worklog) - manu
 * ibi corrigenda, deinde regeneranda. v3 (2026-10-08): Graeca Latinis
 * dissimilia (similia per fons), numeri supra/infra scripti, radices,
 * << >>. */
nomen structura {
                    s32  runa;
     constans character* ordines[VIII];
} Symbolum;

interior constans Symbolum symbola[] = {
    { 0x00B2,   /* ² superscript two */
        { ".###..", "...#..", ".###..", ".#....",
          ".###..", "......", "......", "......" } },
    { 0x00B3,   /* ³ superscript three */
        { ".###..", "...#..", "..##..", "...#..",
          ".###..", "......", "......", "......" } },
    { 0x00B9,   /* ¹ superscript one */
        { "..#...", ".##...", "..#...", "..#...",
          ".###..", "......", "......", "......" } },
    { 0x014B,   /* ŋ latin small letter eng (n cauda infra) */
        { "......", "......", ".#.##.", ".##..#",
          ".#...#", ".#...#", ".#...#", "...##." } },
    { 0x0250,   /* ɐ latin small letter turned a (a versum) */
        { "......", "......", ".####.", ".#...#",
          ".####.", ".#....", "..###.", "......" } },
    { 0x0251,   /* ɑ latin small letter alpha */
        { "......", "......", "..####", ".#...#",
          ".#...#", ".#..##", "..##.#", "......" } },
    { 0x0252,   /* ɒ latin small letter turned alpha */
        { "......", "......", ".#.##.", ".##..#",
          ".#...#", ".#...#", ".####.", "......" } },
    { 0x0254,   /* ɔ latin small letter open o (c inversum) */
        { "......", "......", "..###.", ".#...#",
          ".....#", ".#...#", "..###.", "......" } },
    { 0x0259,   /* ə latin small letter schwa (e versum) */
        { "......", "......", "..###.", ".....#",
          ".#####", ".#...#", "..###.", "......" } },
    { 0x025B,   /* ɛ latin small letter open e */
        { "......", "......", "..####", ".#....",
          "..###.", ".#....", "..####", "......" } },
    { 0x025C,   /* ɜ latin small letter reversed open e */
        { "......", "......", ".####.", ".....#",
          "..###.", ".....#", ".####.", "......" } },
    { 0x0261,   /* ɡ latin small letter script g (ut g fontis) */
        { "......", "......", "..##.#", ".#..##",
          ".#...#", "..####", ".....#", "..###." } },
    { 0x0263,   /* ɣ latin small letter gamma (IPA) */
        { "......", "......", ".#...#", ".#...#",
          "..#.#.", "...#..", "..#.#.", "...#.." } },
    { 0x026A,   /* ɪ latin letter small capital i */
        { "......", "......", "..###.", "...#..",
          "...#..", "...#..", "..###.", "......" } },
    { 0x0279,   /* ɹ latin small letter turned r */
        { "......", "......", "....#.", "....#.",
          "....#.", ".#..#.", "..##.#", "......" } },
    { 0x027E,   /* ɾ latin small letter r with fishhook */
        { "......", "......", "...##.", "..#...",
          "..#...", "..#...", "..#...", "......" } },
    { 0x0283,   /* ʃ latin small letter esh */
        { "....##", "...#..", "...#..", "...#..",
          "...#..", "...#..", "...#..", ".##..." } },
    { 0x028A,   /* ʊ latin small letter upsilon */
        { "......", "......", ".##.##", ".#...#",
          ".#...#", ".#...#", "..###.", "......" } },
    { 0x028C,   /* ʌ latin small letter turned v */
        { "......", "......", "...#..", "..#.#.",
          ".#...#", ".#...#", ".#...#", "......" } },
    { 0x0292,   /* ʒ latin small letter ezh */
        { "......", "......", ".#####", "....#.",
          "...##.", ".....#", ".#...#", "..###." } },
    { 0x0294,   /* ʔ latin letter glottal stop */
        { "..###.", ".#...#", ".....#", "....#.",
          "...#..", "...#..", "...#..", "......" } },
    { 0x02C8,   /* ˈ modifier letter vertical line (accentus primus) */
        { "...#..", "...#..", "...#..", "......",
          "......", "......", "......", "......" } },
    { 0x02CC,   /* ˌ low vertical line (accentus secundus) */
        { "......", "......", "......", "......",
          "......", "...#..", "...#..", "...#.." } },
    { 0x02D0,   /* ː modifier letter triangular colon (longa) */
        { "......", "......", "..###.", "...#..",
          "......", "...#..", "..###.", "......" } },
    { 0x02D1,   /* ˑ half triangular colon (semilonga) */
        { "......", "......", "..###.", "...#..",
          "......", "......", "......", "......" } },
    { 0x0393,   /* Γ greek capital letter gamma */
        { ".#####", ".#....", ".#....", ".#....",
          ".#....", ".#....", ".#....", "......" } },
    { 0x0394,   /* Δ greek capital letter delta */
        { "...#..", "...#..", "..#.#.", "..#.#.",
          ".#...#", ".#...#", ".#####", "......" } },
    { 0x0398,   /* Θ greek capital letter theta */
        { "..###.", ".#...#", ".#...#", ".#####",
          ".#...#", ".#...#", "..###.", "......" } },
    { 0x039B,   /* Λ greek capital letter lamda */
        { "...#..", "...#..", "..#.#.", "..#.#.",
          ".#...#", ".#...#", ".#...#", "......" } },
    { 0x039E,   /* Ξ greek capital letter xi */
        { ".#####", "......", "......", "..###.",
          "......", "......", ".#####", "......" } },
    { 0x03A0,   /* Π greek capital letter pi */
        { ".#####", ".#...#", ".#...#", ".#...#",
          ".#...#", ".#...#", ".#...#", "......" } },
    { 0x03A3,   /* Σ greek capital letter sigma */
        { ".#####", ".#....", "..#...", "...#..",
          "..#...", ".#....", ".#####", "......" } },
    { 0x03A6,   /* Φ greek capital letter phi */
        { "...#..", "..###.", ".#.#.#", ".#.#.#",
          ".#.#.#", "..###.", "...#..", "......" } },
    { 0x03A8,   /* Ψ greek capital letter psi */
        { ".#.#.#", ".#.#.#", ".#.#.#", "..###.",
          "...#..", "...#..", "...#..", "......" } },
    { 0x03A9,   /* Ω greek capital letter omega */
        { "..###.", ".#...#", ".#...#", ".#...#",
          "..#.#.", "..#.#.", ".##.##", "......" } },
    { 0x03B1,   /* α greek small letter alpha */
        { "......", "......", "..##.#", ".#..#.",
          ".#..#.", ".#..#.", "..##.#", "......" } },
    { 0x03B2,   /* β greek small letter beta */
        { "..##..", ".#..#.", ".#.#..", ".#..#.",
          ".#...#", ".#...#", ".####.", ".#...." } },
    { 0x03B3,   /* γ greek small letter gamma */
        { "......", "......", ".#...#", ".#...#",
          "..#.#.", "..#.#.", "...#..", "...#.." } },
    { 0x03B4,   /* δ greek small letter delta */
        { "..##..", ".#....", "..#...", "..##..",
          ".#..#.", ".#..#.", "..##..", "......" } },
    { 0x03B5,   /* ε greek small letter epsilon */
        { "......", "......", "..###.", ".#....",
          "..##..", ".#....", "..###.", "......" } },
    { 0x03B6,   /* ζ greek small letter zeta */
        { ".####.", "...#..", "..#...", ".#....",
          ".#....", "..###.", ".....#", "....#." } },
    { 0x03B7,   /* η greek small letter eta */
        { "......", "......", ".#.##.", ".##..#",
          ".#...#", ".#...#", ".#...#", ".....#" } },
    { 0x03B8,   /* θ greek small letter theta */
        { "..##..", ".#..#.", ".#..#.", ".####.",
          ".#..#.", ".#..#.", "..##..", "......" } },
    { 0x03B9,   /* ι greek small letter iota */
        { "......", "......", "..#...", "..#...",
          "..#...", "..#..#", "...##.", "......" } },
    { 0x03BA,   /* κ greek small letter kappa */
        { "......", "......", ".#..#.", ".#.#..",
          ".##...", ".#.#..", ".#..#.", "......" } },
    { 0x03BB,   /* λ greek small letter lamda */
        { ".#....", "..#...", "..#...", "...#..",
          "..#.#.", ".#...#", ".#...#", "......" } },
    { 0x03BC,   /* μ greek small letter mu */
        { "......", "......", ".#...#", ".#...#",
          ".#...#", ".#..##", ".###.#", ".#...." } },
    { 0x03BD,   /* ν greek small letter nu */
        { "......", "......", ".#...#", ".#...#",
          "..#..#", "..#.#.", "...#..", "......" } },
    { 0x03BE,   /* ξ greek small letter xi */
        { ".####.", "..#...", "...##.", "..#...",
          ".#....", "..###.", ".....#", "...##." } },
    { 0x03C0,   /* π greek small letter pi */
        { "......", "......", ".#####", "..#.#.",
          "..#.#.", "..#.#.", "..#..#", "......" } },
    { 0x03C1,   /* ρ greek small letter rho */
        { "......", "......", "..###.", ".#...#",
          ".#...#", ".##..#", ".#.##.", ".#...." } },
    { 0x03C2,   /* ς greek small letter final sigma */
        { "......", "......", "..###.", ".#....",
          ".#....", "..##..", "....#.", "...#.." } },
    { 0x03C3,   /* σ greek small letter sigma */
        { "......", "......", "..####", ".#..#.",
          ".#...#", ".#...#", "..###.", "......" } },
    { 0x03C4,   /* τ greek small letter tau */
        { "......", "......", ".#####", "...#..",
          "...#..", "...#..", "....##", "......" } },
    { 0x03C5,   /* υ greek small letter upsilon */
        { "......", "......", ".#..#.", ".#...#",
          ".#...#", ".#...#", "..###.", "......" } },
    { 0x03C6,   /* φ greek small letter phi */
        { "......", "...#..", ".#.##.", ".#.#.#",
          ".#.#.#", "..###.", "...#..", "...#.." } },
    { 0x03C7,   /* χ greek small letter chi */
        { "......", "......", ".#...#", "..#.#.",
          "...#..", "..#.#.", ".#...#", "......" } },
    { 0x03C8,   /* ψ greek small letter psi */
        { "......", "...#..", ".#.#.#", ".#.#.#",
          ".#.#.#", "..###.", "...#..", "...#.." } },
    { 0x03C9,   /* ω greek small letter omega */
        { "......", "......", "..#.#.", ".#...#",
          ".#.#.#", ".#.#.#", "..#.#.", "......" } },
    { 0x2013,   /* – en dash */
        { "......", "......", "......", ".####.",
          "......", "......", "......", "......" } },
    { 0x2014,   /* — em dash */
        { "......", "......", "......", "######",
          "......", "......", "......", "......" } },
    { 0x2022,   /* • bullet */
        { "......", "......", "...#..", "..###.",
          "...#..", "......", "......", "......" } },
    { 0x2026,   /* … horizontal ellipsis */
        { "......", "......", "......", "......",
          "......", "......", ".#.#.#", "......" } },
    { 0x203A,   /* › single right-pointing angle quotation mark */
        { "......", "......", "..#...", "...#..",
          "..#...", "......", "......", "......" } },
    { 0x203B,   /* ※ reference mark */
        { "...#..", "......", "..#.#.", ".#.#.#",
          "..#.#.", "......", "...#..", "......" } },
    { 0x2070,   /* ⁰ superscript zero */
        { ".###..", ".#.#..", ".#.#..", ".#.#..",
          ".###..", "......", "......", "......" } },
    { 0x2074,   /* ⁴ superscript four */
        { ".#.#..", ".#.#..", ".###..", "...#..",
          "...#..", "......", "......", "......" } },
    { 0x2075,   /* ⁵ superscript five */
        { ".###..", ".#....", ".###..", "...#..",
          ".###..", "......", "......", "......" } },
    { 0x2076,   /* ⁶ superscript six */
        { ".###..", ".#....", ".###..", ".#.#..",
          ".###..", "......", "......", "......" } },
    { 0x2077,   /* ⁷ superscript seven */
        { ".###..", "...#..", "..#...", "..#...",
          "..#...", "......", "......", "......" } },
    { 0x2078,   /* ⁸ superscript eight */
        { ".###..", ".#.#..", ".###..", ".#.#..",
          ".###..", "......", "......", "......" } },
    { 0x2079,   /* ⁹ superscript nine */
        { ".###..", ".#.#..", ".###..", "...#..",
          ".###..", "......", "......", "......" } },
    { 0x207A,   /* ⁺ superscript plus sign */
        { "......", "..#...", ".###..", "..#...",
          "......", "......", "......", "......" } },
    { 0x207B,   /* ⁻ superscript minus */
        { "......", "......", ".###..", "......",
          "......", "......", "......", "......" } },
    { 0x207F,   /* ⁿ superscript latin small letter n */
        { "......", ".##...", ".#.#..", ".#.#..",
          ".#.#..", "......", "......", "......" } },
    { 0x2080,   /* ₀ subscript zero */
        { "......", "......", "......", ".###..",
          ".#.#..", ".#.#..", ".#.#..", ".###.." } },
    { 0x2081,   /* ₁ subscript one */
        { "......", "......", "......", "..#...",
          ".##...", "..#...", "..#...", ".###.." } },
    { 0x2082,   /* ₂ subscript two */
        { "......", "......", "......", ".###..",
          "...#..", ".###..", ".#....", ".###.." } },
    { 0x2083,   /* ₃ subscript three */
        { "......", "......", "......", ".###..",
          "...#..", "..##..", "...#..", ".###.." } },
    { 0x2084,   /* ₄ subscript four */
        { "......", "......", "......", ".#.#..",
          ".#.#..", ".###..", "...#..", "...#.." } },
    { 0x2085,   /* ₅ subscript five */
        { "......", "......", "......", ".###..",
          ".#....", ".###..", "...#..", ".###.." } },
    { 0x2086,   /* ₆ subscript six */
        { "......", "......", "......", ".###..",
          ".#....", ".###..", ".#.#..", ".###.." } },
    { 0x2087,   /* ₇ subscript seven */
        { "......", "......", "......", ".###..",
          "...#..", "..#...", "..#...", "..#..." } },
    { 0x2088,   /* ₈ subscript eight */
        { "......", "......", "......", ".###..",
          ".#.#..", ".###..", ".#.#..", ".###.." } },
    { 0x2089,   /* ₉ subscript nine */
        { "......", "......", "......", ".###..",
          ".#.#..", ".###..", "...#..", ".###.." } },
    { 0x2190,   /* ← leftwards arrow */
        { "......", "......", "..#...", ".#####",
          "..#...", "......", "......", "......" } },
    { 0x2191,   /* ↑ upwards arrow */
        { "...#..", "..###.", ".#.#.#", "...#..",
          "...#..", "...#..", "...#..", "......" } },
    { 0x2192,   /* → rightwards arrow */
        { "......", "......", "....#.", ".#####",
          "....#.", "......", "......", "......" } },
    { 0x2193,   /* ↓ downwards arrow */
        { "...#..", "...#..", "...#..", "...#..",
          ".#.#.#", "..###.", "...#..", "......" } },
    { 0x2194,   /* ↔ left right arrow */
        { "......", "......", ".#..#.", "######",
          ".#..#.", "......", "......", "......" } },
    { 0x21B5,   /* ↵ downwards arrow with corner leftwards */
        { "......", ".....#", ".....#", "..#..#",
          ".#####", "..#...", "......", "......" } },
    { 0x21E7,   /* ⇧ upwards white arrow */
        { "...#..", "..#.#.", ".#...#", ".##.##",
          "..#.#.", "..#.#.", "..###.", "......" } },
    { 0x2219,   /* ∙ bullet operator */
        { "......", "......", "......", "..##..",
          "..##..", "......", "......", "......" } },
    { 0x221A,   /* √ square root */
        { ".....#", ".....#", "....#.", "....#.",
          ".#.#..", "..##..", "...#..", "......" } },
    { 0x221B,   /* ∛ cube root */
        { ".##..#", "..#..#", ".##.#.", "....#.",
          ".#.#..", "..##..", "...#..", "......" } },
    { 0x2248,   /* ≈ almost equal to */
        { "......", "..##.#", ".#..#.", "......",
          "..##.#", ".#..#.", "......", "......" } },
    { 0x2261,   /* ≡ identical to */
        { "......", ".#####", "......", ".#####",
          "......", ".#####", "......", "......" } },
    { 0x2264,   /* ≤ less-than or equal to */
        { "....#.", "...#..", "..#...", "...#..",
          "....#.", "......", "..###.", "......" } },
    { 0x2265,   /* ≥ greater-than or equal to */
        { "..#...", "...#..", "....#.", "...#..",
          "..#...", "......", "..###.", "......" } },
    { 0x226A,   /* ≪ much less-than */
        { "......", "...#.#", "..#.#.", ".#.#..",
          "..#.#.", "...#.#", "......", "......" } },
    { 0x226B,   /* ≫ much greater-than */
        { "......", ".#.#..", "..#.#.", "...#.#",
          "..#.#.", ".#.#..", "......", "......" } },
    { 0x2282,   /* ⊂ subset of */
        { "......", "..####", ".#....", ".#....",
          ".#....", "..####", "......", "......" } },
    { 0x2283,   /* ⊃ superset of */
        { "......", ".####.", ".....#", ".....#",
          ".....#", ".####.", "......", "......" } },
    { 0x2286,   /* ⊆ subset of or equal to */
        { "..####", ".#....", ".#....", ".#....",
          "..####", "......", ".#####", "......" } },
    { 0x2287,   /* ⊇ superset of or equal to */
        { ".####.", ".....#", ".....#", ".....#",
          ".####.", "......", ".#####", "......" } },
    { 0x2295,   /* ⊕ circled plus */
        { "..###.", ".#.#.#", ".#.#.#", ".#####",
          ".#.#.#", ".#.#.#", "..###.", "......" } },
    { 0x2296,   /* ⊖ circled minus */
        { "..###.", ".#...#", ".#...#", ".#####",
          ".#...#", ".#...#", "..###.", "......" } },
    { 0x2297,   /* ⊗ circled times */
        { "..###.", ".#...#", ".##.##", ".#.#.#",
          ".##.##", ".#...#", "..###.", "......" } },
    { 0x2298,   /* ⊘ circled division slash */
        { "..###.", ".#...#", ".#..##", ".#.#.#",
          ".##..#", ".#...#", "..###.", "......" } },
    { 0x2299,   /* ⊙ circled dot operator */
        { "..###.", ".#...#", ".#...#", ".#.#.#",
          ".#...#", ".#...#", "..###.", "......" } },
    { 0x22EE,   /* ⋮ vertical ellipsis */
        { "......", "...#..", "......", "...#..",
          "......", "...#..", "......", "......" } },
    { 0x22EF,   /* ⋯ midline horizontal ellipsis */
        { "......", "......", "......", ".#.#.#",
          "......", "......", "......", "......" } },
    { 0x2308,   /* ⌈ left ceiling */
        { ".###..", ".#....", ".#....", ".#....",
          ".#....", ".#....", ".#....", "......" } },
    { 0x2309,   /* ⌉ right ceiling */
        { ".###..", "...#..", "...#..", "...#..",
          "...#..", "...#..", "...#..", "......" } },
    { 0x2318,   /* ⌘ place of interest sign */
        { "......", ".##.##", ".#####", "..#.#.",
          ".#####", ".##.##", "......", "......" } },
    { 0x2325,   /* ⌥ option key */
        { "......", "......", ".##.##", "...#..",
          "....##", "......", "......", "......" } },
    { 0x23BF,   /* ⎿ dentistry symbol light vertical and bottom */
        { "..#...", "..#...", "..#...", "..####",
          "......", "......", "......", "......" } },
    { 0x23CE,   /* ⏎ return symbol */
        { "......", ".....#", ".....#", "..#..#",
          ".#####", "..#...", "......", "......" } },
    { 0x23F5,   /* ⏵ black medium right-pointing triangle */
        { "......", "..#...", "..##..", "..###.",
          "..##..", "..#...", "......", "......" } },
    { 0x23F8,   /* ⏸ double vertical bar */
        { "......", ".##.##", ".##.##", ".##.##",
          ".##.##", ".##.##", "......", "......" } },
    { 0x23FA,   /* ⏺ black circle for record */
        { "......", "..###.", ".#####", ".#####",
          ".#####", "..###.", "......", "......" } },
    { 0x24D8,   /* ⓘ circled latin small letter i */
        { "..###.", ".#.#.#", ".#...#", ".#.#.#",
          ".#.#.#", ".#.#.#", "..###.", "......" } },
    { 0x25A0,   /* ■ black square */
        { "......", ".#####", ".#####", ".#####",
          ".#####", ".#####", "......", "......" } },
    { 0x25A1,   /* □ white square */
        { "......", ".#####", ".#...#", ".#...#",
          ".#...#", ".#####", "......", "......" } },
    { 0x25AA,   /* ▪ black small square */
        { "......", "......", "..###.", "..###.",
          "..###.", "......", "......", "......" } },
    { 0x25B0,   /* ▰ black parallelogram */
        { "......", "......", "..####", "..####",
          ".####.", ".####.", "......", "......" } },
    { 0x25B1,   /* ▱ white parallelogram */
        { "......", "......", "..####", "..#..#",
          ".#..#.", ".####.", "......", "......" } },
    { 0x25B2,   /* ▲ black up-pointing triangle */
        { "......", "...#..", "..###.", ".#####",
          "......", "......", "......", "......" } },
    { 0x25B6,   /* ▶ black right-pointing triangle */
        { ".#....", ".##...", ".###..", ".####.",
          ".###..", ".##...", ".#....", "......" } },
    { 0x25B8,   /* ▸ black right-pointing small triangle */
        { "......", "......", "..#...", "..##..",
          "..#...", "......", "......", "......" } },
    { 0x25BC,   /* ▼ black down-pointing triangle */
        { "......", "......", ".#####", "..###.",
          "...#..", "......", "......", "......" } },
    { 0x25C9,   /* ◉ fisheye */
        { "......", "..###.", ".#...#", ".#.#.#",
          ".#...#", "..###.", "......", "......" } },
    { 0x25CB,   /* ○ white circle */
        { "......", "..###.", ".#...#", ".#...#",
          ".#...#", "..###.", "......", "......" } },
    { 0x25CF,   /* ● black circle */
        { "......", "..###.", ".#####", ".#####",
          ".#####", "..###.", "......", "......" } },
    { 0x25D0,   /* ◐ circle with left half black */
        { "......", "..###.", ".##..#", ".##..#",
          ".##..#", "..###.", "......", "......" } },
    { 0x25D1,   /* ◑ circle with right half black */
        { "......", "..###.", ".#..##", ".#..##",
          ".#..##", "..###.", "......", "......" } },
    { 0x25D2,   /* ◒ circle with lower half black */
        { "......", "..###.", ".#...#", ".#####",
          ".#####", "..###.", "......", "......" } },
    { 0x25D3,   /* ◓ circle with upper half black */
        { "......", "..###.", ".#####", ".#####",
          ".#...#", "..###.", "......", "......" } },
    { 0x25EF,   /* ◯ large circle */
        { "..###.", ".#...#", ".#...#", ".#...#",
          ".#...#", ".#...#", "..###.", "......" } },
    { 0x2605,   /* ★ black star */
        { "...#..", "...#..", ".#####", "..###.",
          "..#.#.", ".#...#", "......", "......" } },
    { 0x2610,   /* ☐ ballot box */
        { ".#####", ".#...#", ".#...#", ".#...#",
          ".#...#", ".#...#", ".#####", "......" } },
    { 0x2612,   /* ☒ ballot box with x */
        { "......", ".#####", ".##.##", ".#.#.#",
          ".##.##", ".#####", "......", "......" } },
    { 0x26A0,   /* ⚠ warning sign */
        { "...#..", "..###.", "..#.#.", ".##.##",
          ".#####", ".##.##", ".#####", "......" } },
    { 0x2705,   /* ✅ white heavy check mark (ut ✔, sine quadrato) */
        { "......", ".....#", "....##", ".#.##.",
          ".###..", "..#...", "......", "......" } },
    { 0x2713,   /* ✓ check mark */
        { "......", "......", ".....#", "....#.",
          ".#.#..", "..#...", "......", "......" } },
    { 0x2714,   /* ✔ heavy check mark */
        { "......", ".....#", "....##", ".#.##.",
          ".###..", "..#...", "......", "......" } },
    { 0x2715,   /* ✕ multiplication x */
        { "......", ".#...#", "..#.#.", "...#..",
          "..#.#.", ".#...#", "......", "......" } },
    { 0x2716,   /* ✖ heavy multiplication x */
        { "......", ".#...#", ".##.##", "..###.",
          ".##.##", ".#...#", "......", "......" } },
    { 0x2717,   /* ✗ ballot x */
        { "......", ".#...#", "..#.#.", "...#..",
          "..#.#.", ".#...#", "......", "......" } },
    { 0x2718,   /* ✘ heavy ballot x */
        { "......", ".#...#", ".##.##", "..###.",
          ".##.##", ".#...#", "......", "......" } },
    { 0x2722,   /* ✢ four teardrop-spoked asterisk */
        { "......", "...#..", "...#..", ".##.##",
          "...#..", "...#..", "......", "......" } },
    { 0x2726,   /* ✦ black four pointed star */
        { "...#..", "...#..", "..###.", ".#####",
          "..###.", "...#..", "...#..", "......" } },
    { 0x2733,   /* ✳ eight spoked asterisk */
        { "......", ".#.#.#", "..###.", ".#####",
          "..###.", ".#.#.#", "......", "......" } },
    { 0x2736,   /* ✶ six pointed black star */
        { "......", "...#..", ".#####", "..###.",
          ".#####", "...#..", "......", "......" } },
    { 0x273B,   /* ✻ teardrop-spoked asterisk */
        { "......", ".#.#.#", "..#.#.", ".##.##",
          "..#.#.", ".#.#.#", "......", "......" } },
    { 0x273D,   /* ✽ heavy teardrop-spoked asterisk */
        { "......", "..#.#.", ".##.##", "...#..",
          ".##.##", "..#.#.", "......", "......" } },
    { 0x276F,   /* ❯ heavy right-pointing angle quotation mark */
        { "......", ".##...", "..##..", "...##.",
          "..##..", ".##...", "......", "......" } },
    { 0x27E8,   /* ⟨ mathematical left angle bracket */
        { "...#..", "..#...", "..#...", ".#....",
          "..#...", "..#...", "...#..", "......" } },
    { 0x27E9,   /* ⟩ mathematical right angle bracket */
        { "..#...", "...#..", "...#..", "....#.",
          "...#..", "...#..", "..#...", "......" } },
    { 0x29C9,   /* ⧉ two joined squares */
        { "......", ".###..", ".#.###", ".###.#",
          "...#.#", "...###", "......", "......" } },
    { 0x2B24,   /* ⬤ black large circle */
        { "..###.", ".#####", ".#####", ".#####",
          ".#####", ".#####", "..###.", "......" } },
};

#define SYMBOLA_NUMERUS  (magnitudo(symbola) / magnitudo(Symbolum))

nomen structura {
     i8* larva;
    s32  latitudo;
    s32  altitudo;
    s32  levis;      /* crassitudo levis (Ghostty box_thickness) */
    s32  gravis;
} Pictor;

interior s32
minuere (
    s32 a,
    s32 b)
{
    redde a > b ? a - b : ZEPHYRUM;
}

/* [x0, x1) x [y0, y1) valore impletur, intra larvam (Ghostty
 * canvas.box) */
interior vacuum
quadrum (
    constans Pictor* p,
                s32  x0,
                s32  y0,
                s32  x1,
                s32  y1,
                 i8  valor)
{
    s32 x;
    s32 y;

    x0 = x0 < ZEPHYRUM ? ZEPHYRUM : x0;
    y0 = y0 < ZEPHYRUM ? ZEPHYRUM : y0;
    x1 = x1 > p->latitudo ? p->latitudo : x1;
    y1 = y1 > p->altitudo ? p->altitudo : y1;
    per (y = y0; y < y1; y++)
    {
        per (x = x0; x < x1; x++)
        {
            p->larva[y * p->latitudo + x] = valor;
        }
    }
}


/* ==================================================
 * Lineae (Ghostty box.zig linesChar)
 * ================================================== */

interior vacuum
lineas_ducere (
    constans Pictor* p,
                 i8  brachia)
{
    s32 superum;
    s32 dextrum;
    s32 inferum;
    s32 laevum;
    s32 h_levis_summa;
    s32 h_levis_infima;
    s32 h_gravis_summa;
    s32 h_gravis_infima;
    s32 h_duplex_summa;
    s32 h_duplex_infima;
    s32 v_levis_sinistra;
    s32 v_levis_dextra;
    s32 v_gravis_sinistra;
    s32 v_gravis_dextra;
    s32 v_duplex_sinistra;
    s32 v_duplex_dextra;
    s32 superi_finis;
    s32 inferi_initium;
    s32 laevi_finis;
    s32 dextri_initium;

    superum  = (s32)(brachia & III);
    dextrum  = (s32)((brachia >> II) & III);
    inferum  = (s32)((brachia >> IV) & III);
    laevum   = (s32)((brachia >> VI) & III);

    h_levis_summa      = minuere(p->altitudo, p->levis) / II;
    h_levis_infima     = h_levis_summa + p->levis;
    h_gravis_summa     = minuere(p->altitudo, p->gravis) / II;
    h_gravis_infima    = h_gravis_summa + p->gravis;
    h_duplex_summa     = minuere(h_levis_summa, p->levis);
    h_duplex_infima    = h_levis_infima + p->levis;
    v_levis_sinistra   = minuere(p->latitudo, p->levis) / II;
    v_levis_dextra     = v_levis_sinistra + p->levis;
    v_gravis_sinistra  = minuere(p->latitudo, p->gravis) / II;
    v_gravis_dextra    = v_gravis_sinistra + p->gravis;
    v_duplex_sinistra  = minuere(v_levis_sinistra, p->levis);
    v_duplex_dextra    = v_levis_dextra + p->levis;

    /* fines brachiorum in centro (Ghostty up_bottom, down_top,
     * left_right, right_left) */
    si (laevum == LINEA_GRAVIS || dextrum == LINEA_GRAVIS)
    {
        superi_finis    = h_gravis_infima;
        inferi_initium  = h_gravis_summa;
    }
    alioquin
    {
        si (laevum != dextrum || inferum == superum)
        {
            b32 duplicata;

            duplicata = laevum == LINEA_DUPLICATA
                     || dextrum == LINEA_DUPLICATA;
            superi_finis = duplicata ? h_duplex_infima : h_levis_infima;
            inferi_initium =
                duplicata ? h_duplex_summa : h_levis_summa;
        }
        alioquin si (laevum == LINEA_NULLA && dextrum == LINEA_NULLA)
        {
            superi_finis    = h_levis_infima;
            inferi_initium  = h_levis_summa;
        }
        alioquin
        {
            superi_finis    = h_levis_summa;
            inferi_initium  = h_levis_infima;
        }
    }
    si (superum == LINEA_GRAVIS || inferum == LINEA_GRAVIS)
    {
        laevi_finis     = v_gravis_dextra;
        dextri_initium  = v_gravis_sinistra;
    }
    alioquin
    {
        si (superum != inferum || laevum == dextrum)
        {
            b32 duplicata;

            duplicata = superum == LINEA_DUPLICATA
                     || inferum == LINEA_DUPLICATA;
            laevi_finis     = duplicata ? v_duplex_dextra
                                        : v_levis_dextra;
            dextri_initium  = duplicata ? v_duplex_sinistra
                                        : v_levis_sinistra;
        }
        alioquin si (superum == LINEA_NULLA && inferum == LINEA_NULLA)
        {
            laevi_finis     = v_levis_dextra;
            dextri_initium  = v_levis_sinistra;
        }
        alioquin
        {
            laevi_finis     = v_levis_sinistra;
            dextri_initium  = v_levis_dextra;
        }
    }

    commutatio (superum)
    {
        casus LINEA_LEVIS:
            quadrum(p, v_levis_sinistra, ZEPHYRUM, v_levis_dextra,
                superi_finis, PLENUM);
            frange;
        casus LINEA_GRAVIS:
            quadrum(p, v_gravis_sinistra, ZEPHYRUM, v_gravis_dextra,
                superi_finis, PLENUM);
            frange;
        casus LINEA_DUPLICATA:
            quadrum(p, v_duplex_sinistra, ZEPHYRUM, v_levis_sinistra,
                laevum == LINEA_DUPLICATA ? h_levis_summa
                                          : superi_finis,
                PLENUM);
            quadrum(p, v_levis_dextra, ZEPHYRUM, v_duplex_dextra,
                dextrum == LINEA_DUPLICATA ? h_levis_summa
                                           : superi_finis,
                PLENUM);
            frange;
        ordinarius:
            frange;
    }
    commutatio (dextrum)
    {
        casus LINEA_LEVIS:
            quadrum(p, dextri_initium, h_levis_summa, p->latitudo,
                h_levis_infima, PLENUM);
            frange;
        casus LINEA_GRAVIS:
            quadrum(p, dextri_initium, h_gravis_summa, p->latitudo,
                h_gravis_infima, PLENUM);
            frange;
        casus LINEA_DUPLICATA:
            quadrum(p,
                superum == LINEA_DUPLICATA ? v_levis_dextra
                                           : dextri_initium,
                h_duplex_summa, p->latitudo, h_levis_summa, PLENUM);
            quadrum(p,
                inferum == LINEA_DUPLICATA ? v_levis_dextra
                                           : dextri_initium,
                h_levis_infima, p->latitudo, h_duplex_infima, PLENUM);
            frange;
        ordinarius:
            frange;
    }
    commutatio (inferum)
    {
        casus LINEA_LEVIS:
            quadrum(p, v_levis_sinistra, inferi_initium, v_levis_dextra,
                p->altitudo, PLENUM);
            frange;
        casus LINEA_GRAVIS:
            quadrum(p, v_gravis_sinistra, inferi_initium,
                v_gravis_dextra, p->altitudo, PLENUM);
            frange;
        casus LINEA_DUPLICATA:
            quadrum(p, v_duplex_sinistra,
                laevum == LINEA_DUPLICATA ? h_levis_infima
                                          : inferi_initium,
                v_levis_sinistra, p->altitudo, PLENUM);
            quadrum(p, v_levis_dextra,
                dextrum == LINEA_DUPLICATA ? h_levis_infima
                                           : inferi_initium,
                v_duplex_dextra, p->altitudo, PLENUM);
            frange;
        ordinarius:
            frange;
    }
    commutatio (laevum)
    {
        casus LINEA_LEVIS:
            quadrum(p, ZEPHYRUM, h_levis_summa, laevi_finis,
                h_levis_infima, PLENUM);
            frange;
        casus LINEA_GRAVIS:
            quadrum(p, ZEPHYRUM, h_gravis_summa, laevi_finis,
                h_gravis_infima, PLENUM);
            frange;
        casus LINEA_DUPLICATA:
            quadrum(p, ZEPHYRUM, h_duplex_summa,
                superum == LINEA_DUPLICATA ? v_levis_sinistra
                                           : laevi_finis,
                h_levis_summa, PLENUM);
            quadrum(p, ZEPHYRUM, h_levis_infima,
                inferum == LINEA_DUPLICATA ? v_levis_sinistra
                                           : laevi_finis,
                h_duplex_infima, PLENUM);
            frange;
        ordinarius:
            frange;
    }
}


/* ==================================================
 * Lineolae (Ghostty dashHorizontal / dashVertical)
 * ================================================== */

/* numerus lineolarum, crassitudo, intervallum optatum; dimidia
 * intervalla in marginibus (horizontaliter centratae) */
interior vacuum
interruptas_horizontales (
    constans Pictor* p,
                s32  numerus,
                s32  crassitudo,
                s32  intervallum_optatum)
{
    s32 intervallum;
    s32 longitudo;
    s32 reliquum;
    s32 x;
    s32 x1;
    s32 y;
    s32 k;

    si (p->latitudo < II * numerus)
    {
        y = minuere(p->altitudo, p->levis) / II;
        quadrum(p, ZEPHYRUM, y, p->latitudo, y + p->levis, PLENUM);
        redde;
    }
    intervallum = p->latitudo / (II * numerus);
    si (intervallum_optatum < intervallum)
    {
        intervallum = intervallum_optatum;
    }
    longitudo  = (p->latitudo - numerus * intervallum) / numerus;
    reliquum   = (p->latitudo - numerus * intervallum) % numerus;
    y          = minuere(p->altitudo, crassitudo) / II;
    x          = intervallum / II;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        x1 = x + longitudo;
        si (reliquum > ZEPHYRUM)
        {
            reliquum--;
            x1++;
        }
        quadrum(p, x, y, x1, y + crassitudo, PLENUM);
        x = x1 + intervallum;
    }
}

/* verticaliter: intervallum integrum infra (iunctura cum solidis) */
interior vacuum
interruptas_verticales (
    constans Pictor* p,
                s32  numerus,
                s32  crassitudo,
                s32  intervallum_optatum)
{
    s32 intervallum;
    s32 longitudo;
    s32 reliquum;
    s32 x;
    s32 y;
    s32 y1;
    s32 k;

    si (p->altitudo < II * numerus)
    {
        x = minuere(p->latitudo, p->levis) / II;
        quadrum(p, x, ZEPHYRUM, x + p->levis, p->altitudo, PLENUM);
        redde;
    }
    intervallum = p->altitudo / (II * numerus);
    si (intervallum_optatum < intervallum)
    {
        intervallum = intervallum_optatum;
    }
    longitudo  = (p->altitudo - numerus * intervallum) / numerus;
    reliquum   = (p->altitudo - numerus * intervallum) % numerus;
    x          = minuere(p->latitudo, crassitudo) / II;
    y          = ZEPHYRUM;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        y1 = y + longitudo;
        si (reliquum > ZEPHYRUM)
        {
            reliquum--;
            y1++;
        }
        quadrum(p, x, y, x + crassitudo, y1, PLENUM);
        y = y1 + intervallum;
    }
}

interior vacuum
interruptas_ducere (
    constans Pictor* p,
                s32  runa)
{
    s32 optatum;

    optatum = p->levis > IV ? p->levis : IV;
    commutatio (runa)
    {
        casus 0x2504: interruptas_horizontales(p, III, p->levis,
                          optatum);
            frange;
        casus 0x2505: interruptas_horizontales(p, III, p->gravis,
                          optatum);
            frange;
        casus 0x2506: interruptas_verticales(p, III, p->levis, optatum);
            frange;
        casus 0x2507: interruptas_verticales(p, III, p->gravis,
                          optatum);
            frange;
        casus 0x2508: interruptas_horizontales(p, IV, p->levis,
                          optatum);
            frange;
        casus 0x2509: interruptas_horizontales(p, IV, p->gravis,
                          optatum);
            frange;
        casus 0x250A: interruptas_verticales(p, IV, p->levis, optatum);
            frange;
        casus 0x250B: interruptas_verticales(p, IV, p->gravis, optatum);
            frange;
        casus 0x254C: interruptas_horizontales(p, II, p->levis,
                          p->levis);
            frange;
        casus 0x254D: interruptas_horizontales(p, II, p->gravis,
                          p->gravis);
            frange;
        casus 0x254E: interruptas_verticales(p, II, p->levis,
                          p->gravis);
            frange;
        casus 0x254F: interruptas_verticales(p, II, p->gravis,
                          p->gravis);
            frange;
        ordinarius:
            frange;
    }
}


/* ==================================================
 * Arcus et diagonales: via per puncta, pixelum plenum si centrum eius
 * intra dimidiam crassitudinem viae iacet (Ghostty pingit cum
 * levigatione; nos v1 sine)
 * ================================================== */

nomen structura {
    f64 x[VIA_PUNCTA_MAXIMA];
    f64 y[VIA_PUNCTA_MAXIMA];
    s32 numerus;
} Via;

interior vacuum
viam_addere (
    Via* via,
    f64  x,
    f64  y)
{
    si (via->numerus < VIA_PUNCTA_MAXIMA)
    {
        via->x[via->numerus] = x;
        via->y[via->numerus] = y;
        via->numerus++;
    }
}

/* curva cubica a puncto ultimo per (x1,y1) (x2,y2) ad (x3,y3) */
interior vacuum
curvam_addere (
    Via* via,
    f64  x1,
    f64  y1,
    f64  x2,
    f64  y2,
    f64  x3,
    f64  y3)
{
    f64 x0;
    f64 y0;
    f64 t;
    f64 u;
    s32 k;

    x0 = via->x[via->numerus - I];
    y0 = via->y[via->numerus - I];
    per (k = I; k <= CURVA_GRADUS; k++)
    {
        t = (f64)k / (f64)CURVA_GRADUS;
        u = 1.0 - t;
        viam_addere(via,
            u * u * u * x0 + 3.0 * u * u * t * x1 + 3.0 * u * t * t * x2
                + t * t * t * x3,
            u * u * u * y0 + 3.0 * u * u * t * y1 + 3.0 * u * t * t * y2
                + t * t * t * y3);
    }
}

interior f64
distantia_segmenti (
    f64 px,
    f64 py,
    f64 x0,
    f64 y0,
    f64 x1,
    f64 y1)
{
    f64 dx;
    f64 dy;
    f64 t;
    f64 longitudo;

    dx         = x1 - x0;
    dy         = y1 - y0;
    longitudo  = dx * dx + dy * dy;
    t          = longitudo > 0.0
               ? ((px - x0) * dx + (py - y0) * dy) / longitudo : 0.0;
    t   = t < 0.0 ? 0.0 : (t > 1.0 ? 1.0 : t);
    dx  = x0 + t * dx - px;
    dy  = y0 + t * dy - py;
    redde sqrt(dx * dx + dy * dy);
}

interior vacuum
viam_pingere (
    constans Pictor* p,
       constans Via* via,
                f64  crassitudo)
{
    s32 x;
    s32 y;
    s32 k;
    f64 d;
    f64 minima;

    per (y = ZEPHYRUM; y < p->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < p->latitudo; x++)
        {
            minima = 1.0e9;
            per (k = I; k < via->numerus; k++)
            {
                d = distantia_segmenti((f64)x + 0.5, (f64)y + 0.5,
                    via->x[k - I], via->y[k - I], via->x[k], via->y[k]);
                minima = d < minima ? d : minima;
            }
            /* stricte: centra in ipsa dimidia distantia (anguli
             * diagonalium) pixela non duplicant */
            si (minima < crassitudo / 2.0)
            {
                p->larva[y * p->latitudo + x] = PLENUM;
            }
        }
    }
}

/* ╭ ╮ ╯ ╰ (Ghostty arc): linea ex margine, curva cubica radii
 * min(lat, alt)/2 cum punctis regentibus ad quartam partem, linea ad
 * marginem alterum. dextrorsum/deorsum: directio arcus */
interior vacuum
arcum_ducere (
    constans Pictor* p,
                b32  dextrorsum,
                b32  deorsum)
{
    Via via;
    f64 crassitudo;
    f64 cx;
    f64 cy;
    f64 r;
    f64 sx;
    f64 sy;
    f64 s;

    crassitudo = (f64)p->levis;
    cx = (f64)(minuere(p->latitudo, p->levis) / II) + crassitudo / 2.0;
    cy = (f64)(minuere(p->altitudo, p->levis) / II) + crassitudo / 2.0;
    r  = (f64)(p->latitudo < p->altitudo ? p->latitudo : p->altitudo)
        / 2.0;
    s            = 0.25;
    sx           = dextrorsum ? 1.0 : -1.0;
    sy           = deorsum ? 1.0 : -1.0;
    via.numerus  = ZEPHYRUM;
    viam_addere(&via, cx, deorsum ? (f64)p->altitudo : 0.0);
    viam_addere(&via, cx, cy + sy * r);
    curvam_addere(&via, cx, cy + sy * s * r, cx + sx * s * r, cy,
        cx + sx * r, cy);
    viam_addere(&via, dextrorsum ? (f64)p->latitudo : 0.0, cy);
    viam_pingere(p, &via, crassitudo);
}

/* ╱ ╲ (Ghostty lightDiagonal*): ultra angulos dimidio pixelo, clivo
 * servato */
interior vacuum
diagonalem_ducere (
    constans Pictor* p,
                b32  descendens)
{
    Via via;
    f64 lat;
    f64 alt;
    f64 sx;
    f64 sy;

    lat          = (f64)p->latitudo;
    alt          = (f64)p->altitudo;
    sx           = lat / alt < 1.0 ? lat / alt : 1.0;
    sy           = alt / lat < 1.0 ? alt / lat : 1.0;
    via.numerus  = ZEPHYRUM;
    si (descendens)
    {
        viam_addere(&via, -0.5 * sx, -0.5 * sy);
        viam_addere(&via, lat + 0.5 * sx, alt + 0.5 * sy);
    }
    alioquin
    {
        viam_addere(&via, lat + 0.5 * sx, -0.5 * sy);
        viam_addere(&via, -0.5 * sx, alt + 0.5 * sy);
    }
    viam_pingere(p, &via, (f64)p->levis);
}


/* ==================================================
 * Elementa quadrata (Ghostty block.zig)
 * ================================================== */

/* quadrum alignatum: latitudo et altitudo in octavis cellulae,
 * rotundatae (Ghostty blockShade); positio: 0 sinistrum/summum, 1
 * dextrum/imum */
interior vacuum
quadrum_marginale (
    constans Pictor* p,
                s32  octavae_latitudinis,
                s32  octavae_altitudinis,
                b32  dextrum,
                b32  imum,
                 i8  valor)
{
    s32 w;
    s32 h;
    s32 x;
    s32 y;

    w = (II * p->latitudo * octavae_latitudinis + VIII) / XVI;
    h = (II * p->altitudo * octavae_altitudinis + VIII) / XVI;
    x = dextrum ? p->latitudo - w : ZEPHYRUM;
    y = imum ? p->altitudo - h : ZEPHYRUM;
    quadrum(p, x, y, x + w, y + h, valor);
}

/* quadrans (Ghostty fill cum Fraction.min/max ad dimidium) */
interior vacuum
quadrantem_implere (
    constans Pictor* p,
                b32  dextrum,
                b32  imum)
{
    s32 x_medius_min;
    s32 x_medius_max;
    s32 y_medius_min;
    s32 y_medius_max;

    x_medius_min = p->latitudo - (p->latitudo + I) / II;
    x_medius_max = (p->latitudo + I) / II;
    y_medius_min = p->altitudo - (p->altitudo + I) / II;
    y_medius_max = (p->altitudo + I) / II;
    quadrum(p,
        dextrum ? x_medius_min : ZEPHYRUM,
        imum ? y_medius_min : ZEPHYRUM,
        dextrum ? p->latitudo : x_medius_max,
        imum ? p->altitudo : y_medius_max,
        PLENUM);
}

/* quadrantes ut bita: I summus sinister, II summus dexter, IV imus
 * sinister, VIII imus dexter */
interior vacuum
quadrantes_implere (
    constans Pictor* p,
                s32  bita)
{
    si (bita & I)
    {
        quadrantem_implere(p, FALSUM, FALSUM);
    }
    si (bita & II)
    {
        quadrantem_implere(p, VERUM, FALSUM);
    }
    si (bita & IV)
    {
        quadrantem_implere(p, FALSUM, VERUM);
    }
    si (bita & VIII)
    {
        quadrantem_implere(p, VERUM, VERUM);
    }
}

interior vacuum
quadra_ducere (
    constans Pictor* p,
                s32  runa)
{
    commutatio (runa)
    {
        casus 0x2580: quadrum_marginale(p, VIII, IV, FALSUM, FALSUM,
                          PLENUM); frange;
        casus 0x2581: quadrum_marginale(p, VIII, I, FALSUM, VERUM,
                          PLENUM); frange;
        casus 0x2582: quadrum_marginale(p, VIII, II, FALSUM, VERUM,
                          PLENUM); frange;
        casus 0x2583: quadrum_marginale(p, VIII, III, FALSUM, VERUM,
                          PLENUM); frange;
        casus 0x2584: quadrum_marginale(p, VIII, IV, FALSUM, VERUM,
                          PLENUM); frange;
        casus 0x2585: quadrum_marginale(p, VIII, V, FALSUM, VERUM,
                          PLENUM); frange;
        casus 0x2586: quadrum_marginale(p, VIII, VI, FALSUM, VERUM,
                          PLENUM); frange;
        casus 0x2587: quadrum_marginale(p, VIII, VII, FALSUM, VERUM,
                          PLENUM); frange;
        casus 0x2588: quadrum(p, ZEPHYRUM, ZEPHYRUM, p->latitudo,
                          p->altitudo, PLENUM); frange;
        casus 0x2589: quadrum_marginale(p, VII, VIII, FALSUM, FALSUM,
                          PLENUM); frange;
        casus 0x258A: quadrum_marginale(p, VI, VIII, FALSUM, FALSUM,
                          PLENUM); frange;
        casus 0x258B: quadrum_marginale(p, V, VIII, FALSUM, FALSUM,
                          PLENUM); frange;
        casus 0x258C: quadrum_marginale(p, IV, VIII, FALSUM, FALSUM,
                          PLENUM); frange;
        casus 0x258D: quadrum_marginale(p, III, VIII, FALSUM, FALSUM,
                          PLENUM); frange;
        casus 0x258E: quadrum_marginale(p, II, VIII, FALSUM, FALSUM,
                          PLENUM); frange;
        casus 0x258F: quadrum_marginale(p, I, VIII, FALSUM, FALSUM,
                          PLENUM); frange;
        casus 0x2590: quadrum_marginale(p, IV, VIII, VERUM, FALSUM,
                          PLENUM); frange;
        casus 0x2591: quadrum(p, ZEPHYRUM, ZEPHYRUM, p->latitudo,
                          p->altitudo, UMBRA_LEVIS); frange;
        casus 0x2592: quadrum(p, ZEPHYRUM, ZEPHYRUM, p->latitudo,
                          p->altitudo, UMBRA_MEDIA); frange;
        casus 0x2593: quadrum(p, ZEPHYRUM, ZEPHYRUM, p->latitudo,
                          p->altitudo, UMBRA_DENSA); frange;
        casus 0x2594: quadrum_marginale(p, VIII, I, FALSUM, FALSUM,
                          PLENUM); frange;
        casus 0x2595: quadrum_marginale(p, I, VIII, VERUM, FALSUM,
                          PLENUM); frange;
        casus 0x2596: quadrantes_implere(p, IV); frange;
        casus 0x2597: quadrantes_implere(p, VIII); frange;
        casus 0x2598: quadrantes_implere(p, I); frange;
        casus 0x2599: quadrantes_implere(p, I | IV | VIII); frange;
        casus 0x259A: quadrantes_implere(p, I | VIII); frange;
        casus 0x259B: quadrantes_implere(p, I | II | IV); frange;
        casus 0x259C: quadrantes_implere(p, I | II | VIII); frange;
        casus 0x259D: quadrantes_implere(p, II); frange;
        casus 0x259E: quadrantes_implere(p, II | IV); frange;
        casus 0x259F: quadrantes_implere(p, II | IV | VIII); frange;
        ordinarius:
            frange;
    }
}


/* ==================================================
 * Braille (Ghostty braille.zig): puncta II x IV, mensurae distributae
 * ================================================== */

interior vacuum
puncta_ducere (
    constans Pictor* p,
                s32  runa)
{
    s32 w;
    s32 spatium_x;
    s32 spatium_y;
    s32 margo_x;
    s32 margo_y;
    s32 reliquum_x;
    s32 reliquum_y;
    s32 x[II];
    s32 y[IV];
    s32 bita;
    s32 k;

    spatium_x   = p->latitudo / IV;
    spatium_y   = p->altitudo / VIII;
    w           = spatium_x < spatium_y ? spatium_x : spatium_y;
    margo_x     = spatium_x / II;
    margo_y     = spatium_y / II;
    reliquum_x  = p->latitudo - II * margo_x - spatium_x - II * w;
    reliquum_y  = p->altitudo - II * margo_y - III * spatium_y - IV * w;
    si (reliquum_x >= II && reliquum_y >= IV && w == ZEPHYRUM)
    {
        w++;
        reliquum_x -= II;
        reliquum_y -= IV;
    }
    si (reliquum_x >= II && margo_x == ZEPHYRUM)
    {
        margo_x     = I;
        reliquum_x  -= II;
    }
    si (reliquum_y >= II && margo_y == ZEPHYRUM)
    {
        margo_y     = I;
        reliquum_y  -= II;
    }
    si (reliquum_x >= I)
    {
        spatium_x++;
        reliquum_x--;
    }
    si (reliquum_y >= III)
    {
        spatium_y++;
        reliquum_y -= III;
    }
    si (reliquum_x >= II)
    {
        margo_x++;
        reliquum_x -= II;
    }
    si (reliquum_y >= II)
    {
        margo_y++;
        reliquum_y -= II;
    }
    si (reliquum_x >= II && reliquum_y >= IV)
    {
        w++;
    }
    x[ZEPHYRUM]  = margo_x;
    x[I]         = margo_x + w + spatium_x;
    y[ZEPHYRUM]  = margo_y;
    per (k = I; k < IV; k++)
    {
        y[k] = y[k - I] + w + spatium_y;
    }
    /* bita Unicode: 0 tl, 1 ul, 2 ll, 3 tr, 4 ur, 5 lr, 6 bl, 7 br */
    bita = runa & 0xFF;
    per (k = ZEPHYRUM; k < III; k++)
    {
        si (bita & (I << k))
        {
            quadrum(p, x[ZEPHYRUM], y[k], x[ZEPHYRUM] + w, y[k] + w,
                PLENUM);
        }
        si (bita & (I << (k + III)))
        {
            quadrum(p, x[I], y[k], x[I] + w, y[k] + w, PLENUM);
        }
    }
    si (bita & 0x40)
    {
        quadrum(p, x[ZEPHYRUM], y[III], x[ZEPHYRUM] + w, y[III] + w,
            PLENUM);
    }
    si (bita & 0x80)
    {
        quadrum(p, x[I], y[III], x[I] + w, y[III] + w, PLENUM);
    }
}


/* ==================================================
 * Symbola (v2)
 * ================================================== */

interior constans Symbolum*
symbolum_quaerere (
    s32 runa)
{
    i32 imum;
    i32 summum;
    i32 medium;

    imum    = ZEPHYRUM;
    summum  = SYMBOLA_NUMERUS;
    dum (imum < summum)
    {
        medium = (imum + summum) / II;
        si (symbola[medium].runa == runa)
        {
            redde &symbola[medium];
        }
        si (symbola[medium].runa < runa)
        {
            imum = medium + I;
        }
        alioquin
        {
            summum = medium;
        }
    }
    redde NIHIL;
}

/* pictura VI x VIII ad latitudinem x altitudinem per proximum */
interior vacuum
symbolum_ducere (
      constans Pictor* p,
    constans Symbolum* s)
{
    s32 x;
    s32 y;

    per (y = ZEPHYRUM; y < p->altitudo; y++)
    {
        per (x = ZEPHYRUM; x < p->latitudo; x++)
        {
            si (s->ordines[y * VIII / p->altitudo]
                    [x * VI / p->latitudo] == '#')
            {
                p->larva[y * p->latitudo + x] = PLENUM;
            }
        }
    }
}


/* ==================================================
 * Litterae notatae (v3, Franus 2026-10-28)
 * ================================================== */

#define NOTA_MACRON       I
#define NOTA_BREVE        II
#define NOTA_ACUTUS       IV
#define NOTA_GRAVIS       VIII
#define NOTA_CIRCUMFLEXA  XVI
#define NOTA_DIAERESIS    XXXII
#define NOTA_LENIS        LXIV
#define NOTA_ASPER        CXXVIII
#define NOTA_IOTA         CCLVI

nomen structura {
    s32 runa;
    s32 basis;      /* littera sine notis */
    i32 notae;      /* NOTA_* */
} Composita;

/* ex decompositione Unicode (NFD, unicodedata 15.0) semel scripta,
 * ordine runarum (quaestio bipartita): Latina Ext-A (macron, breve),
 * Graeca cum tonis U+0386-03CE, Graeca extensa U+1F00-1FFF quorum
 * notae omnes nobis notae et basis littera (worklog) */
interior constans Composita compositae[] = {
    { 0x0100, 0x0041, NOTA_MACRON },   /* Ā */
    { 0x0101, 0x0061, NOTA_MACRON },   /* ā */
    { 0x0102, 0x0041, NOTA_BREVE },   /* Ă */
    { 0x0103, 0x0061, NOTA_BREVE },   /* ă */
    { 0x0112, 0x0045, NOTA_MACRON },   /* Ē */
    { 0x0113, 0x0065, NOTA_MACRON },   /* ē */
    { 0x0114, 0x0045, NOTA_BREVE },   /* Ĕ */
    { 0x0115, 0x0065, NOTA_BREVE },   /* ĕ */
    { 0x012A, 0x0049, NOTA_MACRON },   /* Ī */
    { 0x012B, 0x0069, NOTA_MACRON },   /* ī */
    { 0x012C, 0x0049, NOTA_BREVE },   /* Ĭ */
    { 0x012D, 0x0069, NOTA_BREVE },   /* ĭ */
    { 0x014C, 0x004F, NOTA_MACRON },   /* Ō */
    { 0x014D, 0x006F, NOTA_MACRON },   /* ō */
    { 0x014E, 0x004F, NOTA_BREVE },   /* Ŏ */
    { 0x014F, 0x006F, NOTA_BREVE },   /* ŏ */
    { 0x016A, 0x0055, NOTA_MACRON },   /* Ū */
    { 0x016B, 0x0075, NOTA_MACRON },   /* ū */
    { 0x016C, 0x0055, NOTA_BREVE },   /* Ŭ */
    { 0x016D, 0x0075, NOTA_BREVE },   /* ŭ */
    { 0x0232, 0x0059, NOTA_MACRON },   /* Ȳ */
    { 0x0233, 0x0079, NOTA_MACRON },   /* ȳ */
    { 0x0386, 0x0391, NOTA_ACUTUS },   /* Ά */
    { 0x0388, 0x0395, NOTA_ACUTUS },   /* Έ */
    { 0x0389, 0x0397, NOTA_ACUTUS },   /* Ή */
    { 0x038A, 0x0399, NOTA_ACUTUS },   /* Ί */
    { 0x038C, 0x039F, NOTA_ACUTUS },   /* Ό */
    { 0x038E, 0x03A5, NOTA_ACUTUS },   /* Ύ */
    { 0x038F, 0x03A9, NOTA_ACUTUS },   /* Ώ */
    { 0x0390, 0x03B9, NOTA_DIAERESIS | NOTA_ACUTUS },   /* ΐ */
    { 0x03AA, 0x0399, NOTA_DIAERESIS },   /* Ϊ */
    { 0x03AB, 0x03A5, NOTA_DIAERESIS },   /* Ϋ */
    { 0x03AC, 0x03B1, NOTA_ACUTUS },   /* ά */
    { 0x03AD, 0x03B5, NOTA_ACUTUS },   /* έ */
    { 0x03AE, 0x03B7, NOTA_ACUTUS },   /* ή */
    { 0x03AF, 0x03B9, NOTA_ACUTUS },   /* ί */
    { 0x03B0, 0x03C5, NOTA_DIAERESIS | NOTA_ACUTUS },   /* ΰ */
    { 0x03CA, 0x03B9, NOTA_DIAERESIS },   /* ϊ */
    { 0x03CB, 0x03C5, NOTA_DIAERESIS },   /* ϋ */
    { 0x03CC, 0x03BF, NOTA_ACUTUS },   /* ό */
    { 0x03CD, 0x03C5, NOTA_ACUTUS },   /* ύ */
    { 0x03CE, 0x03C9, NOTA_ACUTUS },   /* ώ */
    { 0x1F00, 0x03B1, NOTA_LENIS },   /* ἀ */
    { 0x1F01, 0x03B1, NOTA_ASPER },   /* ἁ */
    { 0x1F02, 0x03B1, NOTA_LENIS | NOTA_GRAVIS },   /* ἂ */
    { 0x1F03, 0x03B1, NOTA_ASPER | NOTA_GRAVIS },   /* ἃ */
    { 0x1F04, 0x03B1, NOTA_LENIS | NOTA_ACUTUS },   /* ἄ */
    { 0x1F05, 0x03B1, NOTA_ASPER | NOTA_ACUTUS },   /* ἅ */
    { 0x1F06, 0x03B1, NOTA_LENIS | NOTA_CIRCUMFLEXA },   /* ἆ */
    { 0x1F07, 0x03B1, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* ἇ */
    { 0x1F08, 0x0391, NOTA_LENIS },   /* Ἀ */
    { 0x1F09, 0x0391, NOTA_ASPER },   /* Ἁ */
    { 0x1F0A, 0x0391, NOTA_LENIS | NOTA_GRAVIS },   /* Ἂ */
    { 0x1F0B, 0x0391, NOTA_ASPER | NOTA_GRAVIS },   /* Ἃ */
    { 0x1F0C, 0x0391, NOTA_LENIS | NOTA_ACUTUS },   /* Ἄ */
    { 0x1F0D, 0x0391, NOTA_ASPER | NOTA_ACUTUS },   /* Ἅ */
    { 0x1F0E, 0x0391, NOTA_LENIS | NOTA_CIRCUMFLEXA },   /* Ἆ */
    { 0x1F0F, 0x0391, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* Ἇ */
    { 0x1F10, 0x03B5, NOTA_LENIS },   /* ἐ */
    { 0x1F11, 0x03B5, NOTA_ASPER },   /* ἑ */
    { 0x1F12, 0x03B5, NOTA_LENIS | NOTA_GRAVIS },   /* ἒ */
    { 0x1F13, 0x03B5, NOTA_ASPER | NOTA_GRAVIS },   /* ἓ */
    { 0x1F14, 0x03B5, NOTA_LENIS | NOTA_ACUTUS },   /* ἔ */
    { 0x1F15, 0x03B5, NOTA_ASPER | NOTA_ACUTUS },   /* ἕ */
    { 0x1F18, 0x0395, NOTA_LENIS },   /* Ἐ */
    { 0x1F19, 0x0395, NOTA_ASPER },   /* Ἑ */
    { 0x1F1A, 0x0395, NOTA_LENIS | NOTA_GRAVIS },   /* Ἒ */
    { 0x1F1B, 0x0395, NOTA_ASPER | NOTA_GRAVIS },   /* Ἓ */
    { 0x1F1C, 0x0395, NOTA_LENIS | NOTA_ACUTUS },   /* Ἔ */
    { 0x1F1D, 0x0395, NOTA_ASPER | NOTA_ACUTUS },   /* Ἕ */
    { 0x1F20, 0x03B7, NOTA_LENIS },   /* ἠ */
    { 0x1F21, 0x03B7, NOTA_ASPER },   /* ἡ */
    { 0x1F22, 0x03B7, NOTA_LENIS | NOTA_GRAVIS },   /* ἢ */
    { 0x1F23, 0x03B7, NOTA_ASPER | NOTA_GRAVIS },   /* ἣ */
    { 0x1F24, 0x03B7, NOTA_LENIS | NOTA_ACUTUS },   /* ἤ */
    { 0x1F25, 0x03B7, NOTA_ASPER | NOTA_ACUTUS },   /* ἥ */
    { 0x1F26, 0x03B7, NOTA_LENIS | NOTA_CIRCUMFLEXA },   /* ἦ */
    { 0x1F27, 0x03B7, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* ἧ */
    { 0x1F28, 0x0397, NOTA_LENIS },   /* Ἠ */
    { 0x1F29, 0x0397, NOTA_ASPER },   /* Ἡ */
    { 0x1F2A, 0x0397, NOTA_LENIS | NOTA_GRAVIS },   /* Ἢ */
    { 0x1F2B, 0x0397, NOTA_ASPER | NOTA_GRAVIS },   /* Ἣ */
    { 0x1F2C, 0x0397, NOTA_LENIS | NOTA_ACUTUS },   /* Ἤ */
    { 0x1F2D, 0x0397, NOTA_ASPER | NOTA_ACUTUS },   /* Ἥ */
    { 0x1F2E, 0x0397, NOTA_LENIS | NOTA_CIRCUMFLEXA },   /* Ἦ */
    { 0x1F2F, 0x0397, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* Ἧ */
    { 0x1F30, 0x03B9, NOTA_LENIS },   /* ἰ */
    { 0x1F31, 0x03B9, NOTA_ASPER },   /* ἱ */
    { 0x1F32, 0x03B9, NOTA_LENIS | NOTA_GRAVIS },   /* ἲ */
    { 0x1F33, 0x03B9, NOTA_ASPER | NOTA_GRAVIS },   /* ἳ */
    { 0x1F34, 0x03B9, NOTA_LENIS | NOTA_ACUTUS },   /* ἴ */
    { 0x1F35, 0x03B9, NOTA_ASPER | NOTA_ACUTUS },   /* ἵ */
    { 0x1F36, 0x03B9, NOTA_LENIS | NOTA_CIRCUMFLEXA },   /* ἶ */
    { 0x1F37, 0x03B9, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* ἷ */
    { 0x1F38, 0x0399, NOTA_LENIS },   /* Ἰ */
    { 0x1F39, 0x0399, NOTA_ASPER },   /* Ἱ */
    { 0x1F3A, 0x0399, NOTA_LENIS | NOTA_GRAVIS },   /* Ἲ */
    { 0x1F3B, 0x0399, NOTA_ASPER | NOTA_GRAVIS },   /* Ἳ */
    { 0x1F3C, 0x0399, NOTA_LENIS | NOTA_ACUTUS },   /* Ἴ */
    { 0x1F3D, 0x0399, NOTA_ASPER | NOTA_ACUTUS },   /* Ἵ */
    { 0x1F3E, 0x0399, NOTA_LENIS | NOTA_CIRCUMFLEXA },   /* Ἶ */
    { 0x1F3F, 0x0399, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* Ἷ */
    { 0x1F40, 0x03BF, NOTA_LENIS },   /* ὀ */
    { 0x1F41, 0x03BF, NOTA_ASPER },   /* ὁ */
    { 0x1F42, 0x03BF, NOTA_LENIS | NOTA_GRAVIS },   /* ὂ */
    { 0x1F43, 0x03BF, NOTA_ASPER | NOTA_GRAVIS },   /* ὃ */
    { 0x1F44, 0x03BF, NOTA_LENIS | NOTA_ACUTUS },   /* ὄ */
    { 0x1F45, 0x03BF, NOTA_ASPER | NOTA_ACUTUS },   /* ὅ */
    { 0x1F48, 0x039F, NOTA_LENIS },   /* Ὀ */
    { 0x1F49, 0x039F, NOTA_ASPER },   /* Ὁ */
    { 0x1F4A, 0x039F, NOTA_LENIS | NOTA_GRAVIS },   /* Ὂ */
    { 0x1F4B, 0x039F, NOTA_ASPER | NOTA_GRAVIS },   /* Ὃ */
    { 0x1F4C, 0x039F, NOTA_LENIS | NOTA_ACUTUS },   /* Ὄ */
    { 0x1F4D, 0x039F, NOTA_ASPER | NOTA_ACUTUS },   /* Ὅ */
    { 0x1F50, 0x03C5, NOTA_LENIS },   /* ὐ */
    { 0x1F51, 0x03C5, NOTA_ASPER },   /* ὑ */
    { 0x1F52, 0x03C5, NOTA_LENIS | NOTA_GRAVIS },   /* ὒ */
    { 0x1F53, 0x03C5, NOTA_ASPER | NOTA_GRAVIS },   /* ὓ */
    { 0x1F54, 0x03C5, NOTA_LENIS | NOTA_ACUTUS },   /* ὔ */
    { 0x1F55, 0x03C5, NOTA_ASPER | NOTA_ACUTUS },   /* ὕ */
    { 0x1F56, 0x03C5, NOTA_LENIS | NOTA_CIRCUMFLEXA },   /* ὖ */
    { 0x1F57, 0x03C5, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* ὗ */
    { 0x1F59, 0x03A5, NOTA_ASPER },   /* Ὑ */
    { 0x1F5B, 0x03A5, NOTA_ASPER | NOTA_GRAVIS },   /* Ὓ */
    { 0x1F5D, 0x03A5, NOTA_ASPER | NOTA_ACUTUS },   /* Ὕ */
    { 0x1F5F, 0x03A5, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* Ὗ */
    { 0x1F60, 0x03C9, NOTA_LENIS },   /* ὠ */
    { 0x1F61, 0x03C9, NOTA_ASPER },   /* ὡ */
    { 0x1F62, 0x03C9, NOTA_LENIS | NOTA_GRAVIS },   /* ὢ */
    { 0x1F63, 0x03C9, NOTA_ASPER | NOTA_GRAVIS },   /* ὣ */
    { 0x1F64, 0x03C9, NOTA_LENIS | NOTA_ACUTUS },   /* ὤ */
    { 0x1F65, 0x03C9, NOTA_ASPER | NOTA_ACUTUS },   /* ὥ */
    { 0x1F66, 0x03C9, NOTA_LENIS | NOTA_CIRCUMFLEXA },   /* ὦ */
    { 0x1F67, 0x03C9, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* ὧ */
    { 0x1F68, 0x03A9, NOTA_LENIS },   /* Ὠ */
    { 0x1F69, 0x03A9, NOTA_ASPER },   /* Ὡ */
    { 0x1F6A, 0x03A9, NOTA_LENIS | NOTA_GRAVIS },   /* Ὢ */
    { 0x1F6B, 0x03A9, NOTA_ASPER | NOTA_GRAVIS },   /* Ὣ */
    { 0x1F6C, 0x03A9, NOTA_LENIS | NOTA_ACUTUS },   /* Ὤ */
    { 0x1F6D, 0x03A9, NOTA_ASPER | NOTA_ACUTUS },   /* Ὥ */
    { 0x1F6E, 0x03A9, NOTA_LENIS | NOTA_CIRCUMFLEXA },   /* Ὦ */
    { 0x1F6F, 0x03A9, NOTA_ASPER | NOTA_CIRCUMFLEXA },   /* Ὧ */
    { 0x1F70, 0x03B1, NOTA_GRAVIS },   /* ὰ */
    { 0x1F71, 0x03B1, NOTA_ACUTUS },   /* ά */
    { 0x1F72, 0x03B5, NOTA_GRAVIS },   /* ὲ */
    { 0x1F73, 0x03B5, NOTA_ACUTUS },   /* έ */
    { 0x1F74, 0x03B7, NOTA_GRAVIS },   /* ὴ */
    { 0x1F75, 0x03B7, NOTA_ACUTUS },   /* ή */
    { 0x1F76, 0x03B9, NOTA_GRAVIS },   /* ὶ */
    { 0x1F77, 0x03B9, NOTA_ACUTUS },   /* ί */
    { 0x1F78, 0x03BF, NOTA_GRAVIS },   /* ὸ */
    { 0x1F79, 0x03BF, NOTA_ACUTUS },   /* ό */
    { 0x1F7A, 0x03C5, NOTA_GRAVIS },   /* ὺ */
    { 0x1F7B, 0x03C5, NOTA_ACUTUS },   /* ύ */
    { 0x1F7C, 0x03C9, NOTA_GRAVIS },   /* ὼ */
    { 0x1F7D, 0x03C9, NOTA_ACUTUS },   /* ώ */
    { 0x1F80, 0x03B1, NOTA_LENIS | NOTA_IOTA },   /* ᾀ */
    { 0x1F81, 0x03B1, NOTA_ASPER | NOTA_IOTA },   /* ᾁ */
    { 0x1F82, 0x03B1, NOTA_LENIS | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1F83, 0x03B1, NOTA_ASPER | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1F84, 0x03B1, NOTA_LENIS | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1F85, 0x03B1, NOTA_ASPER | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1F86, 0x03B1, NOTA_LENIS | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1F87, 0x03B1, NOTA_ASPER | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1F88, 0x0391, NOTA_LENIS | NOTA_IOTA },   /* ᾈ */
    { 0x1F89, 0x0391, NOTA_ASPER | NOTA_IOTA },   /* ᾉ */
    { 0x1F8A, 0x0391, NOTA_LENIS | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1F8B, 0x0391, NOTA_ASPER | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1F8C, 0x0391, NOTA_LENIS | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1F8D, 0x0391, NOTA_ASPER | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1F8E, 0x0391, NOTA_LENIS | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1F8F, 0x0391, NOTA_ASPER | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1F90, 0x03B7, NOTA_LENIS | NOTA_IOTA },   /* ᾐ */
    { 0x1F91, 0x03B7, NOTA_ASPER | NOTA_IOTA },   /* ᾑ */
    { 0x1F92, 0x03B7, NOTA_LENIS | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1F93, 0x03B7, NOTA_ASPER | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1F94, 0x03B7, NOTA_LENIS | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1F95, 0x03B7, NOTA_ASPER | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1F96, 0x03B7, NOTA_LENIS | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1F97, 0x03B7, NOTA_ASPER | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1F98, 0x0397, NOTA_LENIS | NOTA_IOTA },   /* ᾘ */
    { 0x1F99, 0x0397, NOTA_ASPER | NOTA_IOTA },   /* ᾙ */
    { 0x1F9A, 0x0397, NOTA_LENIS | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1F9B, 0x0397, NOTA_ASPER | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1F9C, 0x0397, NOTA_LENIS | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1F9D, 0x0397, NOTA_ASPER | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1F9E, 0x0397, NOTA_LENIS | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1F9F, 0x0397, NOTA_ASPER | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1FA0, 0x03C9, NOTA_LENIS | NOTA_IOTA },   /* ᾠ */
    { 0x1FA1, 0x03C9, NOTA_ASPER | NOTA_IOTA },   /* ᾡ */
    { 0x1FA2, 0x03C9, NOTA_LENIS | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1FA3, 0x03C9, NOTA_ASPER | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1FA4, 0x03C9, NOTA_LENIS | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1FA5, 0x03C9, NOTA_ASPER | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1FA6, 0x03C9, NOTA_LENIS | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1FA7, 0x03C9, NOTA_ASPER | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1FA8, 0x03A9, NOTA_LENIS | NOTA_IOTA },   /* ᾨ */
    { 0x1FA9, 0x03A9, NOTA_ASPER | NOTA_IOTA },   /* ᾩ */
    { 0x1FAA, 0x03A9, NOTA_LENIS | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1FAB, 0x03A9, NOTA_ASPER | NOTA_GRAVIS | NOTA_IOTA },
    { 0x1FAC, 0x03A9, NOTA_LENIS | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1FAD, 0x03A9, NOTA_ASPER | NOTA_ACUTUS | NOTA_IOTA },
    { 0x1FAE, 0x03A9, NOTA_LENIS | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1FAF, 0x03A9, NOTA_ASPER | NOTA_CIRCUMFLEXA | NOTA_IOTA },
    { 0x1FB0, 0x03B1, NOTA_BREVE },   /* ᾰ */
    { 0x1FB1, 0x03B1, NOTA_MACRON },   /* ᾱ */
    { 0x1FB2, 0x03B1, NOTA_GRAVIS | NOTA_IOTA },   /* ᾲ */
    { 0x1FB3, 0x03B1, NOTA_IOTA },   /* ᾳ */
    { 0x1FB4, 0x03B1, NOTA_ACUTUS | NOTA_IOTA },   /* ᾴ */
    { 0x1FB6, 0x03B1, NOTA_CIRCUMFLEXA },   /* ᾶ */
    { 0x1FB7, 0x03B1, NOTA_CIRCUMFLEXA | NOTA_IOTA },   /* ᾷ */
    { 0x1FB8, 0x0391, NOTA_BREVE },   /* Ᾰ */
    { 0x1FB9, 0x0391, NOTA_MACRON },   /* Ᾱ */
    { 0x1FBA, 0x0391, NOTA_GRAVIS },   /* Ὰ */
    { 0x1FBB, 0x0391, NOTA_ACUTUS },   /* Ά */
    { 0x1FBC, 0x0391, NOTA_IOTA },   /* ᾼ */
    { 0x1FC2, 0x03B7, NOTA_GRAVIS | NOTA_IOTA },   /* ῂ */
    { 0x1FC3, 0x03B7, NOTA_IOTA },   /* ῃ */
    { 0x1FC4, 0x03B7, NOTA_ACUTUS | NOTA_IOTA },   /* ῄ */
    { 0x1FC6, 0x03B7, NOTA_CIRCUMFLEXA },   /* ῆ */
    { 0x1FC7, 0x03B7, NOTA_CIRCUMFLEXA | NOTA_IOTA },   /* ῇ */
    { 0x1FC8, 0x0395, NOTA_GRAVIS },   /* Ὲ */
    { 0x1FC9, 0x0395, NOTA_ACUTUS },   /* Έ */
    { 0x1FCA, 0x0397, NOTA_GRAVIS },   /* Ὴ */
    { 0x1FCB, 0x0397, NOTA_ACUTUS },   /* Ή */
    { 0x1FCC, 0x0397, NOTA_IOTA },   /* ῌ */
    { 0x1FD0, 0x03B9, NOTA_BREVE },   /* ῐ */
    { 0x1FD1, 0x03B9, NOTA_MACRON },   /* ῑ */
    { 0x1FD2, 0x03B9, NOTA_DIAERESIS | NOTA_GRAVIS },   /* ῒ */
    { 0x1FD3, 0x03B9, NOTA_DIAERESIS | NOTA_ACUTUS },   /* ΐ */
    { 0x1FD6, 0x03B9, NOTA_CIRCUMFLEXA },   /* ῖ */
    { 0x1FD7, 0x03B9, NOTA_DIAERESIS | NOTA_CIRCUMFLEXA },   /* ῗ */
    { 0x1FD8, 0x0399, NOTA_BREVE },   /* Ῐ */
    { 0x1FD9, 0x0399, NOTA_MACRON },   /* Ῑ */
    { 0x1FDA, 0x0399, NOTA_GRAVIS },   /* Ὶ */
    { 0x1FDB, 0x0399, NOTA_ACUTUS },   /* Ί */
    { 0x1FE0, 0x03C5, NOTA_BREVE },   /* ῠ */
    { 0x1FE1, 0x03C5, NOTA_MACRON },   /* ῡ */
    { 0x1FE2, 0x03C5, NOTA_DIAERESIS | NOTA_GRAVIS },   /* ῢ */
    { 0x1FE3, 0x03C5, NOTA_DIAERESIS | NOTA_ACUTUS },   /* ΰ */
    { 0x1FE4, 0x03C1, NOTA_LENIS },   /* ῤ */
    { 0x1FE5, 0x03C1, NOTA_ASPER },   /* ῥ */
    { 0x1FE6, 0x03C5, NOTA_CIRCUMFLEXA },   /* ῦ */
    { 0x1FE7, 0x03C5, NOTA_DIAERESIS | NOTA_CIRCUMFLEXA },   /* ῧ */
    { 0x1FE8, 0x03A5, NOTA_BREVE },   /* Ῠ */
    { 0x1FE9, 0x03A5, NOTA_MACRON },   /* Ῡ */
    { 0x1FEA, 0x03A5, NOTA_GRAVIS },   /* Ὺ */
    { 0x1FEB, 0x03A5, NOTA_ACUTUS },   /* Ύ */
    { 0x1FEC, 0x03A1, NOTA_ASPER },   /* Ῥ */
    { 0x1FF2, 0x03C9, NOTA_GRAVIS | NOTA_IOTA },   /* ῲ */
    { 0x1FF3, 0x03C9, NOTA_IOTA },   /* ῳ */
    { 0x1FF4, 0x03C9, NOTA_ACUTUS | NOTA_IOTA },   /* ῴ */
    { 0x1FF6, 0x03C9, NOTA_CIRCUMFLEXA },   /* ῶ */
    { 0x1FF7, 0x03C9, NOTA_CIRCUMFLEXA | NOTA_IOTA },   /* ῷ */
    { 0x1FF8, 0x039F, NOTA_GRAVIS },   /* Ὸ */
    { 0x1FF9, 0x039F, NOTA_ACUTUS },   /* Ό */
    { 0x1FFA, 0x03A9, NOTA_GRAVIS },   /* Ὼ */
    { 0x1FFB, 0x03A9, NOTA_ACUTUS },   /* Ώ */
    { 0x1FFC, 0x03A9, NOTA_IOTA },   /* ῼ */
};

#define COMPOSITAE_NUMERUS  \
    (magnitudo(compositae) / magnitudo(Composita))

interior constans Composita*
compositam_quaerere (
    s32 runa)
{
    i32 imum;
    i32 summum;
    i32 medium;

    imum    = ZEPHYRUM;
    summum  = COMPOSITAE_NUMERUS;
    dum (imum < summum)
    {
        medium = (imum + summum) / II;
        si (compositae[medium].runa == runa)
        {
            redde &compositae[medium];
        }
        si (compositae[medium].runa < runa)
        {
            imum = medium + I;
        }
        alioquin
        {
            summum = medium;
        }
    }
    redde NIHIL;
}

/* ordines VIII x VI ('#' plenum) cum terminatore */
nomen character Ordines[VIII][VII];

/* basis: pictura nostra (Graeca), aliter fons ipse (Latina; Graeca
 * Latinis similes per litteras Latinas); FALSUM si ignota */
interior b32
basim_implere (
        s32 basis,
    Ordines ordines)
{
     constans Symbolum* s;
                   i32  y;
                   i32  x;
                   s32  c;

    s = symbolum_quaerere(basis);
    si (s)
    {
        per (y = ZEPHYRUM; y < VIII; y++)
        {
            memcpy(ordines[y], s->ordines[y], VII);
        }
        redde VERUM;
    }
    commutatio (basis)
    {
        casus 0x0391: c = 'A'; frange;
        casus 0x0395: c = 'E'; frange;
        casus 0x0397: c = 'H'; frange;
        casus 0x0399: c = 'I'; frange;
        casus 0x039F: c = 'O'; frange;
        casus 0x03A1: c = 'P'; frange;
        casus 0x03A5: c = 'Y'; frange;
        casus 0x03BF: c = 'o'; frange;
        ordinarius:   c = basis; frange;
    }
    si (c < 0x20 || c > 0x7E)
    {
        redde FALSUM;
    }
    per (y = ZEPHYRUM; y < VIII; y++)
    {
        per (x = ZEPHYRUM; x < VI; x++)
        {
            ordines[y][x] = ((insignatus character)fons_6x8[c][y]
                >> (VII - x)) & I ? '#' : '.';
        }
        ordines[y][VI] = '\0';
    }
    redde VERUM;
}

/* maiuscula (ordines 0-VI) ad corpus minusculae (II-VI) contrahitur:
 * ordo aequalis sequenti demitur (a medio), aliter medius; ordines
 * 0-I vacui */
interior vacuum
contrahere (
    Ordines ordines)
{
    character r[VII][VII];
          s32 n;
          s32 i;
          s32 d;
          s32 k;

    per (i = ZEPHYRUM; i < VII; i++)
    {
        memcpy(r[i], ordines[i], VII);
    }
    n = VII;
    dum (n > V)
    {
        k = -I;
        per (d = ZEPHYRUM; d < n && k < ZEPHYRUM; d++)
        {
            /* a medio: m, m+1, m-1, m+2, ... */
            i = (n - II) / II + ((d % II) ? (d + I) / II : -(d / II));
            si (   i                          >= I && i <= n - II
                && memcmp(r[i], r[i + I], VI) == ZEPHYRUM)
            {
                k = i;
            }
        }
        si (k < ZEPHYRUM)
        {
            k = n / II;
        }
        per (i = k; i < n - I; i++)
        {
            memcpy(r[i], r[i + I], VII);
        }
        n--;
    }
    per (i = ZEPHYRUM; i < II; i++)
    {
        memcpy(ordines[i], "......", VII);
    }
    per (i = ZEPHYRUM; i < V; i++)
    {
        memcpy(ordines[II + i], r[i], VII);
    }
}

/* nota in ordines 0-1 (OR) */
interior vacuum
notam_addere (
                Ordines  ordines,
     constans character* supra,
     constans character* infra)
{
    i32 x;

    per (x = ZEPHYRUM; x < VI; x++)
    {
        si (supra[x] == '#')
        {
            ordines[ZEPHYRUM][x] = '#';
        }
        si (infra[x] == '#')
        {
            ordines[I][x] = '#';
        }
    }
}

/* notae supra: sola centrata; spiritus (sinistra) cum accentu
 * (dextra); diaeresis (ordo I) cum accentu (ordo 0) */
interior vacuum
notas_ponere (
    Ordines ordines,
        i32 notae)
{
    i32 accentus;

    accentus = notae & (NOTA_ACUTUS | NOTA_GRAVIS | NOTA_CIRCUMFLEXA);
    si (notae & (NOTA_LENIS | NOTA_ASPER))
    {
        si (accentus == ZEPHYRUM)
        {
            notam_addere(ordines, "..##..", (notae & NOTA_LENIS)
                ? "...#.." : "..#...");
            redde;
        }
        notam_addere(ordines, "##....", (notae & NOTA_LENIS)
            ? ".#...." : "#.....");
        notam_addere(ordines, accentus == NOTA_ACUTUS ? ".....#"
            : accentus == NOTA_GRAVIS ? "...#.." : "....#.",
            accentus == NOTA_CIRCUMFLEXA ? "...#.#" : "....#.");
        redde;
    }
    si (notae & NOTA_DIAERESIS)
    {
        notam_addere(ordines, accentus == NOTA_ACUTUS ? "...#.."
            : accentus == NOTA_GRAVIS ? "..#..." : accentus
            == NOTA_CIRCUMFLEXA ? "..##.." : "......", ".#..#.");
        redde;
    }
    si (notae & NOTA_MACRON)
    {
        notam_addere(ordines, "......", ".####.");
    }
    alioquin si (notae & NOTA_BREVE)
    {
        notam_addere(ordines, ".#..#.", "..##..");
    }
    alioquin si (accentus == NOTA_ACUTUS)
    {
        notam_addere(ordines, "....#.", "...#..");
    }
    alioquin si (accentus == NOTA_GRAVIS)
    {
        notam_addere(ordines, "..#...", "...#..");
    }
    alioquin si (accentus == NOTA_CIRCUMFLEXA)
    {
        notam_addere(ordines, "..#...", ".#.#..");
    }
}

/* littera notata: basis, maiuscula contracta (si notae supra),
 * minuscula ordinibus 0-1 vacuatis (i sine puncto), notae, iota
 * subscriptum in ordine VII; per symbolum_ducere scalatur */
interior b32
compositam_ducere (
    constans Pictor* p,
                s32  runa)
{
    constans Composita* c;
               Ordines  ordines;
              Symbolum  s;
                   i32  y;

    c = compositam_quaerere(runa);
    si (!c || !basim_implere(c->basis, ordines))
    {
        redde FALSUM;
    }
    si (c->notae & ~(i32)NOTA_IOTA)
    {
        si (   (c->basis >= 'A' && c->basis <= 'Z')
            || (c->basis >= 0x0391 && c->basis <= 0x03A9))
        {
            contrahere(ordines);
        }
        alioquin
        {
            memcpy(ordines[ZEPHYRUM], "......", VII);
            memcpy(ordines[I], "......", VII);
        }
        notas_ponere(ordines, c->notae);
    }
    si (c->notae & NOTA_IOTA)
    {
        ordines[VII][II] = '#';
    }
    s.runa = runa;
    per (y = ZEPHYRUM; y < VIII; y++)
    {
        s.ordines[y] = ordines[y];
    }
    symbolum_ducere(p, &s);
    redde VERUM;
}


/* ==================================================
 * Interfacies
 * ================================================== */

b32
glyphae_ductae_est (
    s32 runa)
{
    redde (runa >= 0x2500 && runa <= 0x259F)
        || (runa >= 0x2800 && runa <= 0x28FF)
        || symbolum_quaerere(runa) != NIHIL
        || compositam_quaerere(runa) != NIHIL;
}

b32
glyphae_ductae_pingere (
    s32  runa,
    i32  latitudo,
    i32  altitudo,
     i8* larva)
{
    Pictor p;
        i8 brachia;

    si (   !glyphae_ductae_est(runa) || latitudo == ZEPHYRUM
        || altitudo == ZEPHYRUM || !larva)
    {
        redde FALSUM;
    }
    memset(larva, ZEPHYRUM, (memoriae_index)(latitudo * altitudo));
    p.larva     = larva;
    p.latitudo  = (s32)latitudo;
    p.altitudo  = (s32)altitudo;
    /* Ghostty box_thickness = crassitudo sublineae fontis; nos ex
     * altitudine: I usque ad XXIII pixela */
    p.levis   = ((s32)altitudo + VIII) / XVI;
    p.levis   = p.levis < I ? I : p.levis;
    p.gravis  = II * p.levis;
    si (symbolum_quaerere(runa))
    {
        symbolum_ducere(&p, symbolum_quaerere(runa));
        redde VERUM;
    }
    si (compositam_ducere(&p, runa))
    {
        redde VERUM;
    }
    si (runa >= 0x2800)
    {
        puncta_ducere(&p, runa);
        redde VERUM;
    }
    si (runa >= 0x2580)
    {
        quadra_ducere(&p, runa);
        redde VERUM;
    }
    brachia = brachia_capsarum[runa - 0x2500];
    si (brachia != ZEPHYRUM)
    {
        lineas_ducere(&p, brachia);
    }
    alioquin si (runa >= 0x256D && runa <= 0x2570)
    {
        /* ╭ deorsum dextrorsum, ╮ deorsum sinistrorsum, ╯ sursum
         * sinistrorsum, ╰ sursum dextrorsum */
        arcum_ducere(&p, runa == 0x256D || runa == 0x2570,
            runa == 0x256D || runa == 0x256E);
    }
    alioquin si (runa >= 0x2571 && runa <= 0x2573)
    {
        si (runa != 0x2572)
        {
            diagonalem_ducere(&p, FALSUM);
        }
        si (runa != 0x2571)
        {
            diagonalem_ducere(&p, VERUM);
        }
    }
    alioquin
    {
        interruptas_ducere(&p, runa);
    }
    redde VERUM;
}
