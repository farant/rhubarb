/* dithering_tabulae_generare.c - tabulas exemplariorum GENERARE
 *
 * Usus: dithering_tabulae_generare <exitus.c>
 *
 * Ex fontibus communibus (dithering_exemplaria_fontes.h: matrix Bayer,
 * pictogrammata) ordines, densitates et pondera computat EODEM modo ac
 * classicus (_exemplar_finire in lib/dithering_exemplaria.c: Gaussiana
 * XIII x XIII in toro, f64, quantizata ad 0..GRADUS_MIXTI), et lucem
 * linearem octeti sRGB per color_srgb_ad_linearem (ut
 * color_octetus_ad_linearem). Scribit
 * lib/dithering_exemplaria_tabulae.c (capite
 * include/dithering_exemplaria_tabulae.h). f64 per %.17g: valor
 * scriptus legitur idem octetim.
 *
 * Instrumentum aetatis aedificationis;
 * tools/dithering_tabulae_generare.sh id struit (aedilis) et currit
 * (-probare: tabula commissa recens?).
 */
#include "latina.h"
#include "color.h"
#include "lectiones.h"
#include "dithering_exemplaria_fontes.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

nomen structura {
     i8 ordo[DITHERING_TEGULA];
    s32 densitas;
     i8 pondus[LXIV];
     i8 pondus_latum[LXIV];
} Exemplar;

interior s32
_punctum (
    constans i8* ordo,
            s32  x,
            s32  y)
{
    redde (s32)((ordo[y & VII] >> (VII - (x & VII))) & I);
}

/* idem ac classici _exemplar_finire, ordine operationum eodem */
interior vacuum
_exemplar_finire (
       Exemplar* e,
    constans i8* ordo)
{
    s32 x, y, dx, dy;

    memcpy(e->ordo, ordo, DITHERING_TEGULA);
    e->densitas = 0;
    per (y = 0; y < DITHERING_TEGULA; y++)
    {
        per (x = 0; x < DITHERING_TEGULA; x++)
        {
            f64 summa = 0.0, pondera = 0.0, summa_lata = 0.0,
                pondera_lata = 0.0;
            e->densitas += _punctum(ordo, x, y);
            per (dy = -VI; dy <= VI; dy++)
            {
                per (dx = -VI; dx <= VI; dx++)
                {
                    f64 g = exp(-(f64)(dx * dx + dy * dy)
                        / (2.0 * DITHERING_SIGMA_OCULI
                        * DITHERING_SIGMA_OCULI));
                    f64 gl = exp(-(f64)(dx * dx + dy * dy)
                        / (2.0 * DITHERING_SIGMA_OCULI_LATI
                        * DITHERING_SIGMA_OCULI_LATI));
                    f64 valor = (f64)_punctum(ordo, x + dx + LXIV, y
                        + dy + LXIV);
                    summa         += g * valor;
                    pondera       += g;
                    summa_lata    += gl * valor;
                    pondera_lata  += gl;
                }
            }
            e->pondus[y * DITHERING_TEGULA + x] = (i8)(s32)(summa
                / pondera * DITHERING_GRADUS_MIXTI + 0.5);
            e->pondus_latum[y * DITHERING_TEGULA + x] = (i8)(s32)(
                summa_lata / pondera_lata * DITHERING_GRADUS_MIXTI
                + 0.5);
        }
    }
}

interior vacuum
_octeti_scribere (
                  FILE* f,
    constans character* titulus,
           constans i8* valores,
                   s32  numerus,
                   s32  latus)
{
    s32 i, j;

    fprintf(f, "constans i8 %s[%d][%d] = {\n", titulus,
        (integer)numerus, (integer)latus);
    per (i = 0; i < numerus; i++)
    {
        fprintf(f, "    {");
        per (j = 0; j < latus; j++)
        {
            si (j > 0 && j % XVI == 0) fprintf(f, "\n     ");
            fprintf(f, "%s%u", j > 0 ? ", " : "",
                (insignatus)valores[i * latus + j]);
        }
        fprintf(f, "}%s\n", i + 1 < numerus ? "," : "");
    }
    fprintf(f, "};\n\n");
}

interior vacuum
_genus_scribere (
                  FILE* f,
    constans character* praefixum,
     constans Exemplar* e,
                   s32  numerus)
{
    i8 ordines[XVI * VIII], pondera[XVI * LXIV], lata[XVI * LXIV];
    character titulus[LXIV];
          s32 i;

    per (i = 0; i < numerus; i++)
    {
        memcpy(ordines + i * VIII, e[i].ordo, VIII);
        memcpy(pondera + i * LXIV, e[i].pondus, LXIV);
        memcpy(lata + i * LXIV, e[i].pondus_latum, LXIV);
    }
    sprintf(titulus, "%s_ORDINES", praefixum);
    _octeti_scribere(f, titulus, ordines, numerus, VIII);
    fprintf(f, "constans s32 %s_DENSITATES[%d] = {\n    ", praefixum,
        (integer)numerus);
    per (i = 0; i < numerus; i++) fprintf(f, "%s%d", i > 0 ? ", " : "",
                                      (integer)e[i].densitas);
    fprintf(f, "\n};\n\n");
    sprintf(titulus, "%s_PONDERA", praefixum);
    _octeti_scribere(f, titulus, pondera, numerus, LXIV);
    sprintf(titulus, "%s_PONDERA_LATA", praefixum);
    _octeti_scribere(f, titulus, lata, numerus, LXIV);
}

s32
principale (
      integer   argc,
    character** argv)
{
    Exemplar bayer[DITHERING_BAYER_NUMERUS];
    Exemplar figurae[DITHERING_PICTOGRAMMATA_FONTIUM];
          i8 ordo[DITHERING_TEGULA];
    FILE* f;
    s32 k, x, y;

    si (argc != II)
    {
        fprintf(stderr,
            "usus: dithering_tabulae_generare <exitus.c>\n");
        redde II;
    }
    /* Bayer: densitas k/64, k = 0 (solidum) .. 60 */
    per (k = 0; k < DITHERING_BAYER_NUMERUS; k++)
    {
        memset(ordo, 0, DITHERING_TEGULA);
        per (y = 0; y < DITHERING_TEGULA; y++)
        {
            per (x = 0; x < DITHERING_TEGULA; x++)
            {
                si (DITHERING_BAYER[y][x] < k * IV) ordo[y] = (i8)(
                                                        ordo[y] | (0x80
                                                            >> x));
            }
        }
        _exemplar_finire(&bayer[k], ordo);
    }
    per (k = 0; k < DITHERING_PICTOGRAMMATA_FONTIUM; k++)
    {
        memset(ordo, 0, DITHERING_TEGULA);
        per (y = 0; y < DITHERING_TEGULA; y++)
        {
            per (x = 0; x < DITHERING_TEGULA; x++)
            {
                si (DITHERING_PICTOGRAMMATA[k].lineae[y][x]
                    == '#') ordo[y] = (i8)(ordo[y] | (0x80 >> x));
            }
        }
        _exemplar_finire(&figurae[k], ordo);
    }

    f = lectiones_fopen(argv[I], "w");
    si (f == NIHIL)
    {
        fprintf(stderr, "dithering_tabulae_generare: scribi non potest:"
            " %s\n", argv[I]);
        redde I;
    }
    fprintf(f,
        "/* dithering_exemplaria_tabulae.c - GENERATUM: "
        "tools/dithering_tabulae_generare.sh (NE EDITA MANU)\n"
        " *\n"
        " * Ex lib/dithering_exemplaria_fontes.c (matrix Bayer,\n"
        " * pictogrammata); caput\n"
        " * include/dithering_exemplaria_tabulae.h.\n"
        " */\n"
        "#include \"dithering_exemplaria_tabulae.h\"\n\n");
    _genus_scribere(f, "DITHERING_BAYER", bayer,
        DITHERING_BAYER_NUMERUS);
    _genus_scribere(f, "DITHERING_PICTOGRAMMATUM", figurae,
        DITHERING_PICTOGRAMMATA_FONTIUM);
    fprintf(f, "constans f64 DITHERING_SRGB_LINEARIS[CCLVI] = {\n");
    per (k = 0; k < CCLVI; k++)
    {
        fprintf(f, "%s%.17g", k == 0 ? "    " : k % IV == 0 ? ",\n    "
            : ", ", color_srgb_ad_linearem((f64)k / 255.0));
    }
    fprintf(f, "\n};\n");
    si (fclose(f) != 0)
    {
        fprintf(stderr,
            "dithering_tabulae_generare: clausura fracta\n");
        redde I;
    }
    redde 0;
}
