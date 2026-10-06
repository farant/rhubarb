/* probatio_ludus_tessera_demissio.c - mandata quadri in cellulas
 * operis, politica una (A2)
 *
 * I.  Politica runarum ex opere: GRAPHEMATUM -> GRAPHEMATUM, SIMPLEX
 *     -> SIMPLEX.
 * II. Via tota per ludus_tessera_demittere: textus "familia ZWJ + x"
 *     (mandata_textus) sub utraque politica operis. GRAPHEMATUM:
 *     unitas una lata internata (GRAPHEMA + LATUM), continuatio, 'x'
 *     in columna II. SIMPLEX (ut Terminal.app): tria emoji lata
 *     (columnae 0, II, IV), 'x' in columna VI. Politica a vocante non
 *     datur - si demissio politicam suam haberet, columnae labuntur.
 * III. Fundus: cellula vacua fundum datum accipit; argumenta NIHIL ->
 *     FALSUM.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "color.h"
#include "mandatum.h"
#include "modulus.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "ludus_tessera_demissio.h"
#include "credo.h"
#include <stdio.h>

/* 👨 ZWJ 👩 ZWJ 👧, deinde x */
#define FAMILIA_ET_X \
    "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x91\xA9\xE2\x80\x8D" \
    "\xF0\x9F\x91\xA7x"

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

/* Quadrum unum: textus FAMILIA_ET_X sub politica operis data, in
 * opus X x I cellularum; reddit opus */
interior TesseraOpus*
_quadrum (
                Piscina* piscina,
    InternamentumChorda* intern,
        TesseraPolitica  politica,
                   b32* bona)
{
    TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(piscina, X,
        I);
             TesseraOpus* opus = tessera_aperire(piscina, &pm->pons);
                 Mandata* m = mandata_creare(piscina, intern);
                 Modulus  modulus = modulus_creare(VI, VIII, LX, VIII);

    tessera_politicam_ponere(opus, politica);
    mandata_textus(m, ZEPHYRUM, ZEPHYRUM,
        chorda_ex_literis(FAMILIA_ET_X, piscina), ZEPHYRUM,
        _rgba(0xFFFFFF));
    *bona = ludus_tessera_demittere(opus, m, &modulus, 0x000000, NIHIL,
        NIHIL, piscina);
    redde opus;
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum(
        "probatio_ludus_tessera_demissio", 16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- I. politica runarum ex opere ---\n");
    {
        TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(piscina,
            IV, I);
                TesseraOpus* opus = tessera_aperire(piscina, &pm->pons);

        tessera_politicam_ponere(opus, TESSERA_POLITICA_GRAPHEMATUM);
        CREDO_AEQUALIS_I32 ((i32)ludus_tessera_politica_runarum(opus),
            (i32)RUNAE_POLITICA_GRAPHEMATUM);
        tessera_politicam_ponere(opus, TESSERA_POLITICA_SIMPLEX);
        CREDO_AEQUALIS_I32 ((i32)ludus_tessera_politica_runarum(opus),
            (i32)RUNAE_POLITICA_SIMPLEX);
    }

    imprimere("\n--- II. familia ZWJ sub utraque politica ---\n");
    {
                b32  bona = FALSUM;
        TesseraOpus* opus = _quadrum(piscina, intern,
            TESSERA_POLITICA_GRAPHEMATUM, &bona);

        CREDO_VERUM (bona);
        CREDO_VERUM (tessera_cellulam_legere(opus, ZEPHYRUM,
            ZEPHYRUM).ornamenta & TESSERA_ORNAMENTUM_GRAPHEMA);
        CREDO_VERUM (tessera_cellulam_legere(opus, ZEPHYRUM,
            ZEPHYRUM).ornamenta & TESSERA_ORNAMENTUM_LATUM);
        CREDO_VERUM (tessera_cellulam_legere(opus, I,
            ZEPHYRUM).ornamenta & TESSERA_ORNAMENTUM_CONTINUATIO);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, II,
            ZEPHYRUM).signum, (i32)'x');
    }
    {
                b32  bona = FALSUM;
        TesseraOpus* opus = _quadrum(piscina, intern,
            TESSERA_POLITICA_SIMPLEX, &bona);
        s32 x;

        CREDO_VERUM (bona);
        per (x = ZEPHYRUM; x < VI; x += II)
        {
            CREDO_VERUM (tessera_cellulam_legere(opus, x,
                ZEPHYRUM).ornamenta & TESSERA_ORNAMENTUM_LATUM);
            CREDO_VERUM (tessera_cellulam_legere(opus, x + I,
                ZEPHYRUM).ornamenta & TESSERA_ORNAMENTUM_CONTINUATIO);
        }
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, VI,
            ZEPHYRUM).signum, (i32)'x');
    }

    imprimere("\n--- III. fundus et argumenta ---\n");
    {
                b32  bona = FALSUM;
        TesseraOpus* opus = _quadrum(piscina, intern,
            TESSERA_POLITICA_GRAPHEMATUM, &bona);
        Modulus modulus = modulus_creare(VI, VIII, LX, VIII);

        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, IX,
            ZEPHYRUM).color_fundi, 0x000000);
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, ZEPHYRUM,
            ZEPHYRUM).color_litterae, 0xFFFFFF);
        CREDO_FALSUM (ludus_tessera_demittere(NIHIL,
            mandata_creare(piscina, intern), &modulus, ZEPHYRUM, NIHIL,
            NIHIL, piscina));
        CREDO_FALSUM (ludus_tessera_demittere(opus, NIHIL, &modulus,
            ZEPHYRUM, NIHIL, NIHIL, piscina));
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
