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

/* Opus probationis (VIII x II, pons memoriae): sectio quaeque suum
 * aperit (cellulae scribuntur - status nullus communis); creatio semel
 * ASSERITUR in parare_suitae. NIHIL = creatio fracta. */
interior TesseraOpus*
_opus_aperire (
    Piscina* piscina)
{
    TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(piscina,
        VIII, II);

    si (pm == NIHIL)
    {
        redde NIHIL;
    }
    redde tessera_aperire(piscina, &pm->pons);
}

/* thema_initiare (status bibliothecae globalis, idempotens) semel ante
 * omnes sectiones, quovis ordine; creatio operis semel asserta (ut olim
 * ante blocos) */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    TesseraPonsMemoriae* pm;
            TesseraOpus* opus;

    thema_initiare();
    pm    = tessera_pons_memoriae_creare(c->piscina, VIII, II);
    opus  = tessera_aperire(c->piscina, &pm->pons);
    CREDO_NON_NIHIL (opus);
}


/* ==================================================
 * PROBARE: I. musivum_pingere - unitas, lata + continuatio,
 * junctura, vacua
 * ================================================== */

interior vacuum
_cellulas_probare (
    CredoContextus* c)
{
    TesseraOpus* opus = _opus_aperire(c->piscina);

    si (opus == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        TessellatioCellula cellulae[V];
                        i8 octeti[VIII];
                       i32 k;

        memset(cellulae, ZEPHYRUM, magnitudo(cellulae));
        per (k = ZEPHYRUM; k < V; k++)
        {
            cellulae[k].latitudo = I;
        }
        cellulae[0].unitas          = (constans i8*)"a";
        cellulae[0].mensura         = I;
        cellulae[0].color_litterae  = 0xFFFFFF;
        cellulae[0].color_fundi     = 0x0000FF;
        cellulae[1].unitas          = (constans i8*)"\xE4\xB8\xAD";
        cellulae[1].mensura         = III;
        cellulae[1].latitudo        = II;
        cellulae[1].color_litterae  = 0xFFFFFF;
        cellulae[1].color_fundi     = 0x0000FF;
        cellulae[2].latitudo        = ZEPHYRUM;     /* continuatio */
        cellulae[2].color_fundi     = 0x0000FF;
        cellulae[3].juncturae       = XV;
        cellulae[3].color_litterae  = 0xFFFF00;
        cellulae[3].color_fundi     = 0x000000;
        cellulae[4].color_litterae  = 0x00FF00;
        cellulae[4].color_fundi     = 0x00FF00;
        musivum_pingere(opus, I, ZEPHYRUM, cellulae, V, I);
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
}


/* ==================================================
 * PROBARE: II. Via tota - Mandata -> tessellatio_computare ->
 * musivum_pingere -> octeti relecti per lineam (internamentum
 * proprium sectionis: mandata in eo internant)
 * ================================================== */

interior vacuum
_viam_probare (
    CredoContextus* c)
{
                Piscina* piscina  = c->piscina;
    InternamentumChorda* intern   = internamentum_creare(piscina);
            TesseraOpus* opus     = _opus_aperire(piscina);

    si (opus == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
                   Mandata* m;
                   Modulus  modulus;
                     Fines  f;
                   Punctum  a;
                   Punctum  b;
        TessellatioCellula  cellulae[XVI];
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
            0x000000, NIHIL, NIHIL, NIHIL, cellulae);
        musivum_pingere(opus, ZEPHYRUM, ZEPHYRUM, cellulae, VIII, II);
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
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. Cellulae ponuntur", _cellulas_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "II. Via tota", _viam_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tessera_musivum", SECTIONES,
        _suitam_parare, NIHIL);
}
