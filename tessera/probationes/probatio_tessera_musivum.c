/* probatio_tessera_musivum.c - musivum: cellulae tessellationis in
 * TesseraOpus (tessellatio T5)
 * I.  musivum_pingere: unitas, lata + continuatio, junctura, vacua.
 * II. Via tota: Mandata -> tessellatio_computare -> musivum_pingere ->
 *     octeti relecti per lineam.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "color.h"
#include "thema.h"
#include "mandatum.h"
#include "modulus.h"
#include "tessellatio.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "musivum_pictura.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior ColorMandati
_rgba (
    i32 rgb)
{
    ColorMandati c;

    c.genus = COLOR_MANDATI_RGBA;
    c.valor = color_ad_pixelum(color_ex_rgba((i8)((rgb >> XVI) & 0xFF),
        (i8)((rgb >> VIII) & 0xFF), (i8)(rgb & 0xFF), (i8)CCLV));
    redde c;
}

/* Lineam operis in UTF-8 relegere (continuatio nihil) */
interior vacuum
_linea_operis (
    TesseraOpus* opus,
            s32  linea,
            s32  columnae,
      character* exitus)
{
    s32 i;
    s32 n;
    i32 k;
    i32 j;
     i8 octeti[XVI];

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < columnae; i++)
    {
        k = tessera_cellulae_octeti(opus, i, linea, octeti, XVI);
        per (j = ZEPHYRUM; j < k; j++)
        {
            exitus[n++] = (character)octeti[j];
        }
    }
    exitus[n] = '\0';
}

s32
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
    TesseraPonsMemoriae* pm;
            TesseraOpus* opus;

    piscina = piscina_generare_dynamicum("probatio_tessera_musivum",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern  = internamentum_creare(piscina);
    thema_initiare();
    pm    = tessera_pons_memoriae_creare(piscina, VIII, II);
    opus  = tessera_aperire(piscina, &pm->pons);
    CREDO_NON_NIHIL (opus);

    imprimere("\n--- I. Cellulae ponuntur ---\n");
    {
        TessellatioCellula c[V];
                        i8 octeti[VIII];
                       i32 k;

        memset(c, ZEPHYRUM, magnitudo(c));
        per (k = ZEPHYRUM; k < V; k++)
        {
            c[k].latitudo = I;
        }
        c[0].unitas          = (constans i8*)"a";
        c[0].mensura         = I;
        c[0].color_litterae  = 0xFFFFFF;
        c[0].color_fundi     = 0x0000FF;
        c[1].unitas          = (constans i8*)"\xE4\xB8\xAD";
        c[1].mensura         = III;
        c[1].latitudo        = II;
        c[1].color_litterae  = 0xFFFFFF;
        c[1].color_fundi     = 0x0000FF;
        c[2].latitudo        = ZEPHYRUM;            /* continuatio */
        c[2].color_fundi     = 0x0000FF;
        c[3].juncturae       = XV;
        c[3].color_litterae  = 0xFFFF00;
        c[3].color_fundi     = 0x000000;
        c[4].color_litterae  = 0x00FF00;
        c[4].color_fundi     = 0x00FF00;
        musivum_pingere(opus, I, ZEPHYRUM, c, V, I);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, I,
            ZEPHYRUM).signum, 0x61);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, I,
            ZEPHYRUM).color_fundi, 0x0000FF);
        CREDO_AEQUALIS_I32 (tessera_cellulae_octeti(opus, II, ZEPHYRUM,
            octeti, VIII), III);
        CREDO_VERUM (memcmp(octeti, "\xE4\xB8\xAD", III) == ZEPHYRUM);
        CREDO_VERUM ((tessera_cellulam_legere(opus, II,
            ZEPHYRUM).ornamenta & TESSERA_ORNAMENTUM_LATUM)
                != ZEPHYRUM);
        CREDO_VERUM ((tessera_cellulam_legere(opus, III,
            ZEPHYRUM).ornamenta & TESSERA_ORNAMENTUM_CONTINUATIO)
            != ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tessera_cellulae_octeti(opus, IV, ZEPHYRUM,
            octeti, VIII), III);
        CREDO_VERUM (memcmp(octeti, "\xE2\x94\xBC", III) == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, IV,
            ZEPHYRUM).color_litterae, 0xFFFF00);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, V,
            ZEPHYRUM).signum, 0x20);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, V,
            ZEPHYRUM).color_fundi, 0x00FF00);
    }

    imprimere("\n--- II. Via tota ---\n");
    {
                   Mandata* m;
                   Modulus  modulus;
                     Fines  f;
                   Punctum  a;
                   Punctum  b;
        TessellatioCellula  c[XVI];
                 character  linea[CXXVIII];

        m           = mandata_creare(piscina, intern);
        f.x         = ZEPHYRUM;
        f.y         = ZEPHYRUM;
        f.latitudo  = XXIV;
        f.altitudo  = VIII;
        mandata_rectangulum(m, f, _rgba(0x0000FF), VERUM);
        mandata_textus(m, VI, ZEPHYRUM, chorda_ex_literis("ok",
            piscina),
            ZEPHYRUM, _rgba(0xFFFFFF));
        a.x = ZEPHYRUM; a.y = XII; b.x = XLVII; b.y = XII;
        mandata_linea(m, a, b, I, _rgba(0xFFFF00));
        modulus = modulus_creare(VI, VIII, XLVIII, XVI);   /* 8 x 2 */
        tessellatio_computare(m, &modulus, RUNAE_POLITICA_GRAPHEMATUM,
            0x000000, NIHIL, NIHIL, NIHIL, c);
        musivum_pingere(opus, ZEPHYRUM, ZEPHYRUM, c, VIII, II);
        _linea_operis(opus, ZEPHYRUM, VIII, linea);
        CREDO_VERUM (strcmp(linea, " ok     ") == ZEPHYRUM);
        _linea_operis(opus, I, VIII, linea);
        CREDO_VERUM (strcmp(linea,
            "\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80"
            "\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80\xE2\x94\x80")
                == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, I,
            ZEPHYRUM).color_fundi, 0x0000FF);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, IV,
            ZEPHYRUM).color_fundi, 0x000000);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
