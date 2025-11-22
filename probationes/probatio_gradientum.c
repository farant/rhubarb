#include "piscina.h"
#include "fenestra.h"
#include "delineare.h"
#include "thema.h"
#include "tempus.h"
#include "chorda.h"
#include <stdio.h>

#define LATITUDO_FENESTRA  DCCCLIII  /* 853 */
#define ALTITUDO_FENESTRA  CDLXXX    /* 480 */

/* Numerus paginarum */
#define NUMERUS_PAGINARUM  XI

/* Helper: obtinere colorem RGB directe ex palette */
interior i32
palette_ad_rgb(i32 palette_index)
{
    i32 r, g, b;
    constans i8* pal = thema_palette_aquinas();
    i32 offset = palette_index * III;

    /* Palette uses 6-bit values (0x00-0x3F), scale to 8-bit (0x00-0xFF) */
    r = ((i32)pal[offset + ZEPHYRUM] * CCLV) / LXIII;
    g = ((i32)pal[offset + I] * CCLV) / LXIII;
    b = ((i32)pal[offset + II] * CCLV) / LXIII;

    /* Pack as ABGR for macOS pixel format */
    redde ((i32)CCLV << XXIV) | ((i32)b << XVI) | ((i32)g << VIII) | (i32)r;
}

/* Construere palette integram (omnes XVI colores Aquinas) */
interior vacuum
construere_palette_integram(i32* palette)
{
    i32 i;
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        palette[i] = palette_ad_rgb(i);
    }
}

/* Pingere titulum paginae */
interior vacuum
pingere_titulum(TabulaPixelorum* tabula, Piscina* piscina, constans character* text)
{
    chorda titulus = chorda_ex_literis(text, piscina);
    tabula_pixelorum_pingere_chordam_scalatam(tabula, X, V, titulus, palette_ad_rgb(V), I);
}

interior vacuum
reddere_paginam(
    ContextusDelineandi* ctx,
    TabulaPixelorum*     tabula,
    Piscina*             piscina,
    i32                  pagina,
    constans i32*        palette)
{
    commutatio (pagina)
    {
        casus ZEPHYRUM:  /* Page 0: Grayscale spectrum - Floyd-Steinberg */
        {
            pingere_titulum(tabula, piscina, "Page 0: Grayscale - Floyd-Steinberg (16 colors)");

            /* Horizontal gradients through all grays */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, L,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(I), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, LXX, CCC, L,
                palette_ad_rgb(I), palette_ad_rgb(II), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CXL, CCC, L,
                palette_ad_rgb(II), palette_ad_rgb(III), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCX, CCC, L,
                palette_ad_rgb(III), palette_ad_rgb(IV), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCLXXX, CCC, L,
                palette_ad_rgb(IV), palette_ad_rgb(V), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Vertical: full spectrum */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V), FALSUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Radial gradients */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(V), palette_ad_rgb(ZEPHYRUM),
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V),
                DITHERING_FLOYD_STEINBERG, palette, XVI);
            frange;
        }

        casus I:  /* Page 1: Red spectrum - Floyd-Steinberg */
        {
            pingere_titulum(tabula, piscina, "Page 1: Red Spectrum - Floyd-Steinberg (16 colors)");

            /* Dark red -> Medium red */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, C,
                palette_ad_rgb(VI), palette_ad_rgb(VII), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Medium red -> Bright red */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CLX, CCC, C,
                palette_ad_rgb(VII), palette_ad_rgb(VIII), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Black -> Bright red */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCCXX, CCC, C,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(VIII), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Vertical: full red range */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(VI), palette_ad_rgb(VIII), FALSUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Radial: red variations */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(VIII), palette_ad_rgb(ZEPHYRUM),
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(VI),
                DITHERING_FLOYD_STEINBERG, palette, XVI);
            frange;
        }

        casus II:  /* Page 2: Gold spectrum - Floyd-Steinberg */
        {
            pingere_titulum(tabula, piscina, "Page 2: Gold Spectrum - Floyd-Steinberg (16 colors)");

            /* Dark gold -> Medium gold */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, C,
                palette_ad_rgb(IX), palette_ad_rgb(X), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Medium gold -> Bright gold */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CLX, CCC, C,
                palette_ad_rgb(X), palette_ad_rgb(XI), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Black -> Bright gold */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCCXX, CCC, C,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(XI), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Vertical: full gold range */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(IX), palette_ad_rgb(XI), FALSUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Radial: gold variations */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(XI), palette_ad_rgb(ZEPHYRUM),
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(IX), palette_ad_rgb(V),
                DITHERING_FLOYD_STEINBERG, palette, XVI);
            frange;
        }

        casus III:  /* Page 3: Cyan spectrum - Floyd-Steinberg */
        {
            pingere_titulum(tabula, piscina, "Page 3: Cyan Spectrum - Floyd-Steinberg (16 colors)");

            /* Dark cyan -> Medium cyan */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, C,
                palette_ad_rgb(XII), palette_ad_rgb(XIII), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Medium cyan -> Bright cyan */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CLX, CCC, C,
                palette_ad_rgb(XIII), palette_ad_rgb(XIV), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Black -> Bright cyan */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCCXX, CCC, C,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(XIV), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Vertical: full cyan range */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(XII), palette_ad_rgb(XIV), FALSUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Radial: cyan variations */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(XIV), palette_ad_rgb(ZEPHYRUM),
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(XII),
                DITHERING_FLOYD_STEINBERG, palette, XVI);
            frange;
        }

        casus IV:  /* Page 4: Mixed color combinations - Floyd-Steinberg */
        {
            pingere_titulum(tabula, piscina, "Page 4: Mixed Colors - Floyd-Steinberg (16 colors)");

            /* Red -> Gold */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, L,
                palette_ad_rgb(VIII), palette_ad_rgb(XI), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Gold -> Cyan */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, LXX, CCC, L,
                palette_ad_rgb(XI), palette_ad_rgb(XIV), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Cyan -> Red */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CXL, CCC, L,
                palette_ad_rgb(XIV), palette_ad_rgb(VIII), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* White -> Warm gray */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCX, CCC, L,
                palette_ad_rgb(V), palette_ad_rgb(XV), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* All colors vertical */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(VIII), palette_ad_rgb(XIV), FALSUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Radial: complementary colors */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(XI), palette_ad_rgb(XIV),
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(VIII), palette_ad_rgb(XII),
                DITHERING_FLOYD_STEINBERG, palette, XVI);
            frange;
        }

        casus V:  /* Page 5: Grayscale spectrum - Bayer 4x4 */
        {
            pingere_titulum(tabula, piscina, "Page 5: Grayscale - Bayer 4x4 (16 colors)");

            /* Horizontal gradients through all grays */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, L,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(I), VERUM,
                DITHERING_BAYER_4X4, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, LXX, CCC, L,
                palette_ad_rgb(I), palette_ad_rgb(II), VERUM,
                DITHERING_BAYER_4X4, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CXL, CCC, L,
                palette_ad_rgb(II), palette_ad_rgb(III), VERUM,
                DITHERING_BAYER_4X4, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCX, CCC, L,
                palette_ad_rgb(III), palette_ad_rgb(IV), VERUM,
                DITHERING_BAYER_4X4, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCLXXX, CCC, L,
                palette_ad_rgb(IV), palette_ad_rgb(V), VERUM,
                DITHERING_BAYER_4X4, palette, XVI);

            /* Vertical: full spectrum */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V), FALSUM,
                DITHERING_BAYER_4X4, palette, XVI);

            /* Radial gradients */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(V), palette_ad_rgb(ZEPHYRUM),
                DITHERING_BAYER_4X4, palette, XVI);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V),
                DITHERING_BAYER_4X4, palette, XVI);
            frange;
        }

        casus VI:  /* Page 6: Grayscale spectrum - Bayer 8x8 */
        {
            pingere_titulum(tabula, piscina, "Page 6: Grayscale - Bayer 8x8 (16 colors)");

            /* Horizontal gradients through all grays */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, L,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(I), VERUM,
                DITHERING_BAYER_8X8, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, LXX, CCC, L,
                palette_ad_rgb(I), palette_ad_rgb(II), VERUM,
                DITHERING_BAYER_8X8, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CXL, CCC, L,
                palette_ad_rgb(II), palette_ad_rgb(III), VERUM,
                DITHERING_BAYER_8X8, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCX, CCC, L,
                palette_ad_rgb(III), palette_ad_rgb(IV), VERUM,
                DITHERING_BAYER_8X8, palette, XVI);

            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCLXXX, CCC, L,
                palette_ad_rgb(IV), palette_ad_rgb(V), VERUM,
                DITHERING_BAYER_8X8, palette, XVI);

            /* Vertical: full spectrum */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V), FALSUM,
                DITHERING_BAYER_8X8, palette, XVI);

            /* Radial gradients */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(V), palette_ad_rgb(ZEPHYRUM),
                DITHERING_BAYER_8X8, palette, XVI);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V),
                DITHERING_BAYER_8X8, palette, XVI);
            frange;
        }

        casus VII:  /* Page 7: All algorithms comparison - 4 columns */
        {
            pingere_titulum(tabula, piscina, "Page 7: Algorithm Comparison - FS/Atkinson/Bayer4x4/Bayer8x8");

            /* Floyd-Steinberg: Black -> White (column 1) */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCXIII, CDLXXX,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V), VERUM,
                DITHERING_FLOYD_STEINBERG, palette, XVI);

            /* Atkinson: Black -> White (column 2) */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCXIII, L, CCXIII, CDLXXX,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V), VERUM,
                DITHERING_ATKINSON, palette, XVI);

            /* Bayer 4x4: Black -> White (column 3) */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CDXXVI, L, CCXIII, CDLXXX,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V), VERUM,
                DITHERING_BAYER_4X4, palette, XVI);

            /* Bayer 8x8: Black -> White (column 4) */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, DCXXXIX, L, CCXIV, CDLXXX,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V), VERUM,
                DITHERING_BAYER_8X8, palette, XVI);
            frange;
        }

        casus VIII:  /* Page 8: Atkinson with ONLY golds/yellows (9-11) */
        {
            i32 golds[III];

            golds[ZEPHYRUM] = palette_ad_rgb(IX);   /* Dark gold */
            golds[I] = palette_ad_rgb(X);            /* Medium gold */
            golds[II] = palette_ad_rgb(XI);          /* Bright gold */

            pingere_titulum(tabula, piscina, "Page 8: Atkinson - Gold Colors Only (3 colors)");

            /* Black -> Bright gold, constrained to gold palette */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, C,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(XI), VERUM,
                DITHERING_ATKINSON, golds, III);

            /* Dark gold -> Bright gold */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CLX, CCC, C,
                palette_ad_rgb(IX), palette_ad_rgb(XI), VERUM,
                DITHERING_ATKINSON, golds, III);

            /* Vertical: full gold range */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(IX), palette_ad_rgb(XI), FALSUM,
                DITHERING_ATKINSON, golds, III);

            /* Radial: gold variations */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(XI), palette_ad_rgb(IX),
                DITHERING_ATKINSON, golds, III);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(IX), palette_ad_rgb(XI),
                DITHERING_ATKINSON, golds, III);
            frange;
        }

        casus IX:  /* Page 9: Atkinson with ONLY reds (6-8) */
        {
            i32 reds[III];

            reds[ZEPHYRUM] = palette_ad_rgb(VI);    /* Dark red */
            reds[I] = palette_ad_rgb(VII);           /* Medium red */
            reds[II] = palette_ad_rgb(VIII);         /* Bright red */

            pingere_titulum(tabula, piscina, "Page 9: Atkinson - Red Colors Only (3 colors)");

            /* Black -> Bright red, constrained to red palette */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, C,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(VIII), VERUM,
                DITHERING_ATKINSON, reds, III);

            /* Dark red -> Bright red */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CLX, CCC, C,
                palette_ad_rgb(VI), palette_ad_rgb(VIII), VERUM,
                DITHERING_ATKINSON, reds, III);

            /* Vertical: full red range */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(VI), palette_ad_rgb(VIII), FALSUM,
                DITHERING_ATKINSON, reds, III);

            /* Radial: red variations */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(VIII), palette_ad_rgb(VI),
                DITHERING_ATKINSON, reds, III);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(VI), palette_ad_rgb(VIII),
                DITHERING_ATKINSON, reds, III);
            frange;
        }

        casus X:  /* Page 10: Atkinson with ONLY grays + one color accent */
        {
            i32 grays_plus_red[VII];

            grays_plus_red[ZEPHYRUM] = palette_ad_rgb(ZEPHYRUM);  /* Black */
            grays_plus_red[I] = palette_ad_rgb(I);                 /* Dark gray */
            grays_plus_red[II] = palette_ad_rgb(II);               /* Gray */
            grays_plus_red[III] = palette_ad_rgb(III);             /* Light gray */
            grays_plus_red[IV] = palette_ad_rgb(IV);               /* Lighter gray */
            grays_plus_red[V] = palette_ad_rgb(V);                 /* White */
            grays_plus_red[VI] = palette_ad_rgb(VIII);             /* Bright red accent */

            pingere_titulum(tabula, piscina, "Page 10: Atkinson - Grays + Red Accent (7 colors)");

            /* Black -> White with red accent available */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, L, CCC, C,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(V), VERUM,
                DITHERING_ATKINSON, grays_plus_red, VII);

            /* Black -> Red */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CLX, CCC, C,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(VIII), VERUM,
                DITHERING_ATKINSON, grays_plus_red, VII);

            /* Red -> White */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, L, CCCXX, CCC, C,
                palette_ad_rgb(VIII), palette_ad_rgb(V), VERUM,
                DITHERING_ATKINSON, grays_plus_red, VII);

            /* Vertical: black -> red */
            delineare_gradientum_linearem_dithered_cum_palette(ctx, CCCLX, L, C, CCC,
                palette_ad_rgb(ZEPHYRUM), palette_ad_rgb(VIII), FALSUM,
                DITHERING_ATKINSON, grays_plus_red, VII);

            /* Radial variations */
            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CXX, XC,
                palette_ad_rgb(VIII), palette_ad_rgb(ZEPHYRUM),
                DITHERING_ATKINSON, grays_plus_red, VII);

            delineare_gradientum_radialem_dithered_cum_palette(ctx, DLXXV, CCCLX, XC,
                palette_ad_rgb(V), palette_ad_rgb(ZEPHYRUM),
                DITHERING_ATKINSON, grays_plus_red, VII);
            frange;
        }
    }
}

int
main(void)
{
    Piscina*             piscina;
    Fenestra*            fenestra;
    TabulaPixelorum*     tabula;
    ContextusDelineandi* ctx;
    FenestraConfiguratio configuratio;
    Eventus              eventus;
    b32                  currens;
    i32                  pagina_currens;
    i32                  palette_integra[XVI];

    /* Initiare thema */
    thema_initiare();

    /* Initiare tempus */
    tempus_initiare();

    /* Construere palette integram */
    construere_palette_integram(palette_integra);

    /* Creare piscinam */
    piscina = piscina_generare_dynamicum("gradientum", M * M * IV);  /* 4MB */
    si (!piscina)
    {
        imprimere("Fractura: non potest creare piscinam\n");
        redde I;
    }

    /* Configurare fenestram */
    configuratio.titulus = "Probatio Gradientum Dithered - Multi-Algorithmus";
    configuratio.x = C;
    configuratio.y = C;
    configuratio.latitudo = LATITUDO_FENESTRA;
    configuratio.altitudo = ALTITUDO_FENESTRA;
    configuratio.vexilla = FENESTRA_ORDINARIA;

    /* Creare fenestram */
    fenestra = fenestra_creare(piscina, &configuratio);
    si (!fenestra)
    {
        imprimere("Fractura: non potest creare fenestram\n");
        redde I;
    }

    /* Creare tabulam pixelorum */
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra, ALTITUDO_FENESTRA);
    si (!tabula)
    {
        imprimere("Fractura: non potest creare tabulam\n");
        fenestra_destruere(fenestra);
        redde I;
    }

    /* Creare contextum delineandi */
    ctx = delineare_creare_contextum(piscina, tabula);
    si (!ctx)
    {
        imprimere("Fractura: non potest creare contextum\n");
        fenestra_destruere(fenestra);
        redde I;
    }

    /* Monstrare fenestram */
    fenestra_monstrare(fenestra);

    /* Cyclus principalis */
    currens = VERUM;
    pagina_currens = ZEPHYRUM;

    dum (currens && !fenestra_debet_claudere(fenestra))
    {
        /* Actualizare tempus */
        tempus_quadrum();

        /* Perscrutari eventus */
        fenestra_perscrutari_eventus(fenestra);

        /* Tractare eventus */
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            si (eventus.genus == EVENTUS_CLAUDERE)
            {
                currens = FALSUM;
            }
            alioquin si (eventus.genus == EVENTUS_CLAVIS_DEPRESSUS)
            {
                si (eventus.datum.clavis.clavis == CLAVIS_EFFUGIUM)
                {
                    currens = FALSUM;
                }
            }
            alioquin si (eventus.genus == EVENTUS_MUS_DEPRESSUS)
            {
                /* Click to advance to next page */
                pagina_currens = (pagina_currens + I) % NUMERUS_PAGINARUM;
            }
        }

        /* Vacare fondum */
        delineare_vacare(ctx, palette_ad_rgb(ZEPHYRUM));

        /* Reddere paginam currentem */
        reddere_paginam(ctx, tabula, piscina, pagina_currens, palette_integra);

        /* Praesentare pixela */
        fenestra_praesentare_pixela(fenestra, tabula);
    }

    /* Purgare */
    fenestra_destruere(fenestra);
    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
