/* dithering_exemplaria_fontes.c - fontes exemplariorum communes
 * (vide caput)
 *
 * Ex lib/dithering_exemplaria.c moti (exemplaria-velox T2,
 * 2026-11-07), octetim iidem.
 */
#include "dithering_exemplaria_fontes.h"

constans s32 DITHERING_BAYER[DITHERING_TEGULA][DITHERING_TEGULA] = {
    { 0, 32,  8, 40,  2, 34, 10, 42},
    {48, 16, 56, 24, 50, 18, 58, 26},
    {12, 44,  4, 36, 14, 46,  6, 38},
    {60, 28, 52, 20, 62, 30, 54, 22},
    { 3, 35, 11, 43,  1, 33,  9, 41},
    {51, 19, 59, 27, 49, 17, 57, 25},
    {15, 47,  7, 39, 13, 45,  5, 37},
    {63, 31, 55, 23, 61, 29, 53, 21}
};

constans DitheringPictogrammaFons
    DITHERING_PICTOGRAMMATA[DITHERING_PICTOGRAMMATA_FONTIUM] =
    {
    {DITHERING_PICTOGRAMMA_LATERES,
     {"########", "#.......", "#.......", "#.......", "########",
         "....#...", "....#...", "....#..."}},
    {DITHERING_PICTOGRAMMA_SQUAMAE,
     {"#......#", ".#....#.", "..####..", "........", "...##...",
         "..#..#..", "##....##", "........"}},
    {DITHERING_PICTOGRAMMA_UNDAE,
     {"........", "##....##", "..#..#..", "...##...", "........",
         "........", "........", "........"}},
    {DITHERING_PICTOGRAMMA_UNDAE,
     {"........", "##....##", "..#..#..", "...##...", "........",
         "##....##", "..#..#..", "...##..."}},
    {DITHERING_PICTOGRAMMA_RHOMBI,
     {"#.......", ".#.....#", "..#...#.", "...#.#..", "....#...",
         "...#.#..", "..#...#.", ".#.....#"}},
    {DITHERING_PICTOGRAMMA_TEXTUM,
     {"#####.#.", "....#.#.", "#####.#.", "....#.#.", "#.#.####",
         "#.#.....", "#.#.####", "#.#....."}},
    {DITHERING_PICTOGRAMMA_CRUCES,
     {"........", "..#.....", ".###....", "..#.....", "........",
         "......#.", ".....###", "......#."}},
    {DITHERING_PICTOGRAMMA_HERBA,
     {"........", "#.#.#...", ".###....", "..#.....", "........",
         "#...#.#.", ".....###", "......#."}},
    {DITHERING_PICTOGRAMMA_SAGITTAE,
     {"#......#", ".#....#.", "..#..#..", "...##...", "#......#",
         ".#....#.", "..#..#..", "...##..."}},
    {DITHERING_PICTOGRAMMA_ANULI,
     {"........", "..###...", ".#...#..", ".#...#..", ".#...#..",
         "..###...", "........", "........"}},
    {DITHERING_PICTOGRAMMA_PUNCTA,
     {"........", "........", "..##....", "..##....", "........",
         "........", "......##", "......##"}}
};
