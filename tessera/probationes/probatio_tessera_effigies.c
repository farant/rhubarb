/* probatio_tessera_effigies.c - effigies: aptatio et pictura (quadrans
 * Q5)
 *
 * I.   effigies_mensurare: casus manu computati (quadratum, latum,
 *      altum, minimum sursum scalatum, vacuum; DIMIDIUM; aspectus
 *      cellulae quadratus).
 * II.  effigies_pingere: runa -> signum compactum (U+2580 = E2 96 80 ->
 *      0x8096E2), colores; spatium fundum servat.
 * III. Via tota: imago minima -> quadrans_computare -> effigies_pingere
 *      -> cellulae relectae.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "quadrans.h"
#include "effigies_pictura.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_mensura (
              i32  img_lat,
              i32  img_alt,
    QuadransModus  modus,
              i32  aspectus,
              i32  sub_lat,
              i32  sub_alt,
    constans character* causa)
{
    i32 lat = XCIX;
    i32 alt = XCIX;

    effigies_mensurare(img_lat, img_alt, XL, X, modus, aspectus, &lat,
        &alt);
    si (lat != sub_lat || alt != sub_alt)
    {
        imprimere("  FRACTA: %s: %ux%u, exspectata %ux%u\n", causa,
            (insignatus integer)lat, (insignatus integer)alt,
            (insignatus integer)sub_lat, (insignatus integer)sub_alt);
    }
    CREDO_AEQUALIS_I32 (lat, sub_lat);
    CREDO_AEQUALIS_I32 (alt, sub_alt);
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    TesseraPonsMemoriae* pm;
            TesseraOpus* opus;

    piscina = piscina_generare_dynamicum("probatio_tessera_effigies",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    pm    = tessera_pons_memoriae_creare(piscina, VIII, II);
    opus  = tessera_aperire(piscina, &pm->pons);
    CREDO_NON_NIHIL (opus);

    imprimere("\n--- I. Aptatio (intra XL x X cellulas) ---\n");
    /* quadratum: sub-pixelum quadrantis 1:2 altum -> 40x20 (20x10
     * cellulae, in scrinio quadratae) */
    _mensura(C, C, QUADRANS_QUADRANTES, L, XL, XX, "quadratum");
    _mensura(C, C, QUADRANS_DIMIDIUM, L, XX, XX, "quadratum dimidium");
    /* latum: latitudo limitat -> 80x10 */
    _mensura(CD, C, QUADRANS_QUADRANTES, L, LXXX, X, "latum");
    /* altum: 10x20 */
    _mensura(C, CD, QUADRANS_QUADRANTES, L, X, XX, "altum");
    /* minimum 1x1 sursum scalatum ad altitudinem: 40x20 */
    _mensura(I, I, QUADRANS_QUADRANTES, L, XL, XX, "minimum");
    /* cellula quadrata (aspectus C): sub-pixelum quadratum -> 20x20 */
    _mensura(C, C, QUADRANS_QUADRANTES, C, XX, XX, "cellula quadrata");
    _mensura(ZEPHYRUM, C, QUADRANS_QUADRANTES, L, ZEPHYRUM, ZEPHYRUM,
        "vacuum");

    imprimere("\n--- II. Pictura cellularum ---\n");
    {
        QuadransCellula c[II];
                     i8 octeti[VIII];

        c[0].runa            = 0x2580;
        c[0].color_litterae  = 0xFF0000;
        c[0].color_fundi     = 0x0000FF;
        c[1].runa            = 0x20;
        c[1].color_litterae  = 0x00FF00;
        c[1].color_fundi     = 0x00FF00;
        effigies_pingere(opus, I, ZEPHYRUM, c, II, I);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, I,
            ZEPHYRUM).signum, 0x8096E2);
        CREDO_AEQUALIS_I32 (tessera_cellulae_octeti(opus, I, ZEPHYRUM,
            octeti, VIII), III);
        CREDO_VERUM (memcmp(octeti, "\xE2\x96\x80", III) == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, I,
            ZEPHYRUM).color_litterae, 0xFF0000);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, I,
            ZEPHYRUM).color_fundi, 0x0000FF);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, II,
            ZEPHYRUM).signum, 0x20);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, II,
            ZEPHYRUM).color_fundi, 0x00FF00);
    }

    imprimere("\n--- III. Via tota ---\n");
    {
        /* 4x2: bloccus sinister TL niger (ceteri albi) -> U+2598;
         * dexter ruber uniformis -> spatium */
                      i8 pixela[IV * II * IV];
                   Imago im;
        QuadransOptiones o = quadrans_optiones_ordinariae();
         QuadransCellula c[II];
                      i8 octeti[VIII];
                     i32 k;

        per (k = ZEPHYRUM; k < IV * II; k++)
        {
            i32 px = k % IV;
            i32 color = (px >= II) ? 0xFF0000
                : (k == ZEPHYRUM) ? ZEPHYRUM : 0xFFFFFF;

            pixela[k * IV]        = (i8)((color >> XVI) & 0xFF);
            pixela[k * IV + I]    = (i8)((color >> VIII) & 0xFF);
            pixela[k * IV + II]   = (i8)(color & 0xFF);
            pixela[k * IV + III]  = 0xFF;
        }
        im.pixela    = pixela;
        im.latitudo  = IV;
        im.altitudo  = II;
        quadrans_computare(&im, ZEPHYRUM, ZEPHYRUM, IV, II, &o, c);
        effigies_pingere(opus, ZEPHYRUM, I, c, II, I);
        CREDO_AEQUALIS_I32 (tessera_cellulae_octeti(opus, ZEPHYRUM, I,
            octeti, VIII), III);
        CREDO_VERUM (memcmp(octeti, "\xE2\x96\x98", III) == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, ZEPHYRUM,
            I).color_litterae, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, ZEPHYRUM,
            I).color_fundi, 0xFFFFFF);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, I, I).signum,
            0x20);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, I,
            I).color_fundi, 0xFF0000);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
