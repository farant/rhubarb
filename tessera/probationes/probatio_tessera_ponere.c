/* probatio_tessera_ponere.c - tessera_graphema_ponere (runae U6c)
 *
 * I.  Casus: unitas pingenda prima ponitur (regula runae.h: C0/DEL et
 *     octetus invalidus -> '?' I columna; graphema sub politica operis;
 *     latitudo 0 nihil pingit); reddit finem unitatis et latitudinem.
 * II. Congruentia cum runae: per corpus Lapidis (XXXV linguae,
 *     politicae ambae) summa latitudinum a tessera reddita ==
 *     runae_latitudo_textus - regula in tessera SCRIPTA contra regulam
 *     in runae scriptam, et ambulatio ad finem ipsum pervenit.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "runae.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LITTERA_LATA      "\xE5\xBA\x83"
#define E_ACUTUM   "e\xCC\x81"
#define FAMILIA    "\xF0\x9F\x91\xA8\xE2\x80\x8D" \
                   "\xF0\x9F\x91\xA9\xE2\x80\x8D" \
                   "\xF0\x9F\x91\xA7"
#define VEXILLA    (TESSERA_ORNAMENTUM_LATUM \
                    | TESSERA_ORNAMENTUM_CONTINUATIO \
                    | TESSERA_ORNAMENTUM_GRAPHEMA)
#define CORPUS_MAXIMUM 65536

/* Cellula: octeti exspectati et vexilla */
interior b32
_cella_est (
           TesseraOpus* opus,
                   s32  x,
                   s32  y,
    constans character* octeti,
                   i32  vexilla)
{
     i8 lecti[TESSERA_GRAPHEMA_OCTETI_MAXIMI];
    i32 n = tessera_cellulae_octeti(opus, x, y, lecti,
        TESSERA_GRAPHEMA_OCTETI_MAXIMI);
    i32 v = tessera_cellulam_legere(opus, x, y).ornamenta & VEXILLA;

    si (   v != vexilla || n != (i32)strlen(octeti)
        || memcmp(lecti, octeti, (memoriae_index)n) != ZEPHYRUM)
    {
        imprimere("  FRACTA: cellula (%d,%d): %u octeti, vexilla %03x; "
            "exspectati %u, vexilla %03x\n", (integer)x, (integer)y,
            (insignatus integer)n, (insignatus integer)v,
            (insignatus integer)strlen(octeti),
            (insignatus integer)vexilla);
        redde FALSUM;
    }
    redde VERUM;
}

/* Unitatem ponere et reditum iudicare: octeti consumpti et latitudo */
interior vacuum
_ponere (
           TesseraOpus* opus,
                   s32  x,
                   s32  y,
    constans character* octeti,
                   i32  consumpti,
                   i32  latitudo,
    constans character* causa)
{
    constans i8* initium  = (constans i8*)octeti;
    constans i8* finis    = initium + strlen(octeti);
            i32  lat      = XCIX;
    constans i8* post     = tessera_graphema_ponere(opus, x, y, initium,
        finis, tessera_stilus_nativus(), &lat);

    si ((i32)(post - initium) != consumpti || lat != latitudo)
    {
        imprimere("  FRACTA: %s: consumpti %d latitudo %u, exspectati "
            "%u et %u\n", causa, (integer)(post - initium),
            (insignatus integer)lat, (insignatus integer)consumpti,
            (insignatus integer)latitudo);
    }
    CREDO_AEQUALIS_I32 ((i32)(post - initium), consumpti);
    CREDO_AEQUALIS_I32 (lat, latitudo);
}

/* Fasciculum corporis legere (RHUBARB_RADIX a cursore datur) */
interior i32
_corpus_legere (
    constans character* lingua,
                    i8* alveus)
{
             character  via[DXII];
    constans character* radix = getenv("RHUBARB_RADIX");
         FILE* f;
         i32  lecti;

    sprintf(via, "%s/probationes/fixa/runae/corpus/%s.txt",
        radix != NIHIL ? radix : "..", lingua);
    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        imprimere("  FRACTA: %s legi non potest\n", via);
        redde ZEPHYRUM;
    }
    lecti = (i32)fread(alveus, I, CORPUS_MAXIMUM, f);
    fclose(f);
    redde lecti;
}

/* Textum per tessera_graphema_ponere ambulare: summa latitudinum
 * (ordo II, praecisio nulla refert - summa reditorum iudicatur) */
interior b32
_congruit (
           TesseraOpus* opus,
           constans i8* textus,
                   i32  mensura,
         RunaePolitica  politica,
    constans character* lingua)
{
    constans i8* cursor  = textus;
    constans i8* finis   = textus + mensura;
            i32  summa   = ZEPHYRUM;
            i32  runae_summa = runae_latitudo_textus(textus, finis,
                politica);

    dum (cursor < finis)
    {
                i32  lat;
        constans i8* post = tessera_graphema_ponere(opus, ZEPHYRUM, II,
            cursor, finis, tessera_stilus_nativus(), &lat);

        si (post <= cursor)
        {
            imprimere("  FRACTA: %s: ponere non progreditur ad %d\n",
                lingua, (integer)(cursor - textus));
            redde FALSUM;
        }
        summa   += lat;
        cursor  = post;
    }
    si (cursor != finis || summa != runae_summa)
    {
        imprimere("  FRACTA: %s: tessera %u columnae, runae %u\n",
            lingua,
            (insignatus integer)summa, (insignatus integer)runae_summa);
        redde FALSUM;
    }
    redde VERUM;
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    TesseraPonsMemoriae* pm;
            TesseraOpus* opus;
            TesseraOpus* simplex;
          TesseraStilus  nat;

    piscina = piscina_generare_dynamicum("probatio_tessera_ponere",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    nat   = tessera_stilus_nativus();
    pm    = tessera_pons_memoriae_creare(piscina, XVI, IV);
    opus  = tessera_aperire(piscina, &pm->pons);
    simplex  = tessera_aperire(piscina,
        &tessera_pons_memoriae_creare(piscina, XVI, IV)->pons);
    CREDO_NON_NIHIL (opus);
    CREDO_NON_NIHIL (simplex);
    tessera_politicam_ponere(simplex, TESSERA_POLITICA_SIMPLEX);

    imprimere("\n--- Casus ---\n");
    _ponere(opus, ZEPHYRUM, ZEPHYRUM, "ab", I, I, "a");
    CREDO_VERUM (_cella_est(opus, ZEPHYRUM, ZEPHYRUM, "a", ZEPHYRUM));
    _ponere(opus, II, ZEPHYRUM, LITTERA_LATA "b", III, II, "guang");
    CREDO_VERUM (_cella_est(opus, II, ZEPHYRUM, LITTERA_LATA,
        TESSERA_ORNAMENTUM_LATUM));
    CREDO_VERUM (_cella_est(opus, III, ZEPHYRUM, "",
        TESSERA_ORNAMENTUM_CONTINUATIO));
    _ponere(opus, V, ZEPHYRUM, E_ACUTUM "x", III, I, "e + acutum");
    CREDO_VERUM (_cella_est(opus, V, ZEPHYRUM, E_ACUTUM,
        TESSERA_ORNAMENTUM_GRAPHEMA));
    _ponere(opus, VI, ZEPHYRUM, "\x01" "b", I, I, "regimen C0");
    CREDO_VERUM (_cella_est(opus, VI, ZEPHYRUM, "?", ZEPHYRUM));
    _ponere(opus, VII, ZEPHYRUM, "\xE0\x80\x80", I, I,
        "series nimis longa: octetus unus");
    CREDO_VERUM (_cella_est(opus, VII, ZEPHYRUM, "?", ZEPHYRUM));
    tessera_cellulam_ponere(opus, VIII, ZEPHYRUM, (i32)'k', nat);
    _ponere(opus, VIII, ZEPHYRUM, "\xCC\x81" "q", II, ZEPHYRUM,
        "signum solum: nihil pingitur");
    CREDO_VERUM (_cella_est(opus, VIII, ZEPHYRUM, "k", ZEPHYRUM));
    _ponere(opus, ZEPHYRUM, I, FAMILIA "z", XVIII, II, "familia");
    CREDO_VERUM (_cella_est(opus, ZEPHYRUM, I, FAMILIA,
        TESSERA_ORNAMENTUM_GRAPHEMA | TESSERA_ORNAMENTUM_LATUM));
    _ponere(simplex, ZEPHYRUM, I, FAMILIA "z", VII, II,
        "familia SIMPLEX: vir + ZWJ");
    CREDO_VERUM (_cella_est(simplex, ZEPHYRUM, I,
        "\xF0\x9F\x91\xA8\xE2\x80\x8D",
        TESSERA_ORNAMENTUM_GRAPHEMA | TESSERA_ORNAMENTUM_LATUM));
    /* columna ultima: latitudo UNITATIS redditur (mensura runae), sed
     * cellula spatium fit (regula U5) */
    _ponere(opus, XV, I, LITTERA_LATA, III, II,
        "guang in columna ultima");
    CREDO_VERUM (_cella_est(opus, XV, I, "", ZEPHYRUM));
    _ponere(opus, ZEPHYRUM, II, "", ZEPHYRUM, ZEPHYRUM, "vacuum");
    /* limes octetorum (graphema > LXIV octeti): ka + i (II columnae) +
     * XXX acuta = LXVI octeti -> runa prima sola (I columna) + spatium,
     * ne columna altera unitatis vetus maneat */
    {
        character longum[LXXX];
              i32 k;

        memcpy(longum, "\xE0\xA4\x95\xE0\xA4\xBF", VI);
        per (k = ZEPHYRUM; k < XXX; k++)
        {
            longum[VI + k * II]      = (character)0xCC;
            longum[VI + k * II + I]  = (character)0x81;
        }
        longum[LXVI] = '\0';
        tessera_cellulam_ponere(opus, XI, III, (i32)'k', nat);
        _ponere(opus, X, III, longum, LXVI, II,
            "limes: runa prima + spatium");
        CREDO_VERUM (_cella_est(opus, X, III, "\xE0\xA4\x95",
            ZEPHYRUM));
        CREDO_VERUM (_cella_est(opus, XI, III, " ", ZEPHYRUM));
    }

    imprimere("\n--- Congruentia cum runae (corpus) ---\n");
    {
        constans character* LINGUAE[] = {
            "ar", "bn", "ceb", "de", "el", "en", "es", "fa", "fr",
            "gu", "he", "hi", "hu", "id", "ig", "it", "ja", "ko",
            "la", "ml", "nl", "pl", "pt", "ro", "ru", "rw", "sv",
            "sw", "ta", "th", "tl", "tr", "vi", "yo", "zh"
        };
         i8* alveus = (i8*)piscina_allocare(piscina, CORPUS_MAXIMUM);
        i32  k;
        i32  congruae = ZEPHYRUM;

        CREDO_NON_NIHIL (alveus);
        per (k = ZEPHYRUM; k < XXXV; k++)
        {
            i32 mensura = _corpus_legere(LINGUAE[k], alveus);

            CREDO_VERUM (mensura > ZEPHYRUM);
            si (   _congruit(opus, alveus, mensura,
                       RUNAE_POLITICA_GRAPHEMATUM, LINGUAE[k])
                && _congruit(simplex, alveus, mensura,
                       RUNAE_POLITICA_SIMPLEX, LINGUAE[k]))
            {
                congruae++;
            }
        }
        imprimere("  linguae congruae: %u\n",
            (insignatus integer)congruae);
        CREDO_AEQUALIS_I32 (congruae, XXXV);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
