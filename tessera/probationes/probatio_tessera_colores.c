/* probatio_tessera_colores.c - profunditas colorum (quadrans Q4)
 *
 * I.   PLENI (ordinaria): 38;2 / 48;2 immota.
 * II.  CCLVI: 38;5 / 48;5, puncta manu computata (cubus 0/95/135/
 *      175/215/255; grisei 8 + 10k): rubrum -> 196, 808080 -> 244
 *      (griseus exactus), album -> 231, nigrum -> 16, 336699 -> 60
 *      (cubus griseum vincit); nativus nullum colorem emittit; cellula
 *      RGB verum servat.
 * III. Ambitus: COLORTERM et TERM_PROGRAM (profunditas) et
 *      TERM_PROGRAM (politica latitudinis, runae U5c - antea non
 *      probata).
 * IV.  Ornamenta (modulus 004, emissio per stilus_terminalis): octeti
 *      singulorum sex et omnium cum coloribus (XVII parametra, maxima
 *      tesserae) - ANTE codicem nulla probatio ornamenta emissa
 *      figebat (planta "transfixum omissum" 15/15 transibat).
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "tessera_opus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Octeti emissi continentne literas? */
interior b32
_continet (
                chorda  captum,
    constans character* literae)
{
    i32 n = (i32)strlen(literae);
    i32 k;

    per (k = ZEPHYRUM; k + n <= captum.mensura; k++)
    {
        si (memcmp(captum.datum + k, literae, (memoriae_index)n)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Cellulam unam cum coloribus et ornamentis datis sub profunditate
 * data emittere; reddit captum */
interior chorda
_emittere_ornatam (
           Piscina* piscina,
    TesseraColores  colores,
               i32  litterae,
               i32  fundi,
               i32  ornamenta)
{
    TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(piscina, IV,
        I);
    TesseraOpus* opus = tessera_aperire(piscina, &pm->pons);

    tessera_colores_ponere(opus, colores);
    tessera_cellulam_ponere(opus, ZEPHYRUM, ZEPHYRUM, (i32)'a',
        tessera_stilus(litterae, fundi, ornamenta));
    (vacuum)tessera_praesentare(opus);
    redde tessera_pons_memoriae_captum(pm);
}

interior chorda
_emittere (
           Piscina* piscina,
    TesseraColores  colores,
               i32  litterae,
               i32  fundi)
{
    redde _emittere_ornatam(piscina, colores, litterae, fundi,
        ZEPHYRUM);
}

interior vacuum
_ornatam_probare (
          Piscina* piscina,
    TesseraColores  colores,
              i32  litterae,
              i32  fundi,
              i32  ornamenta,
    constans character* exspectatum)
{
    chorda captum  = _emittere_ornatam(piscina, colores, litterae,
        fundi, ornamenta);
       b32 bona    = _continet(captum, exspectatum);

    si (!bona)
    {
        imprimere("  FRACTA: ornamenta 0x%x: '%s' non emissum\n",
            (insignatus integer)ornamenta, exspectatum + I);
    }
    CREDO_VERUM (bona);
}

interior vacuum
_emissionem_probare (
          Piscina* piscina,
    TesseraColores  colores,
              i32  litterae,
              i32  fundi,
    constans character* exspectatum,
    constans character* causa)
{
    chorda captum  = _emittere(piscina, colores, litterae, fundi);
       b32 bona    = _continet(captum, exspectatum);

    si (!bona)
    {
        imprimere("  FRACTA: %s: '%s' non emissum\n", causa,
            exspectatum + I);
    }
    CREDO_VERUM (bona);
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_tessera_colores",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- I. PLENI ---\n");
    _emissionem_probare(piscina, TESSERA_COLORES_PLENI, 0xFF0000,
        0x336699,
        "\033[0;38;2;255;0;0;48;2;51;102;153m", "pleni");

    imprimere("\n--- II. CCLVI ---\n");
    _emissionem_probare(piscina, TESSERA_COLORES_CCLVI, 0xFF0000,
        0x808080,
        "\033[0;38;5;196;48;5;244m", "rubrum et griseus exactus");
    _emissionem_probare(piscina, TESSERA_COLORES_CCLVI, 0xFFFFFF,
        ZEPHYRUM,
        "\033[0;38;5;231;48;5;16m", "album et nigrum");
    _emissionem_probare(piscina, TESSERA_COLORES_CCLVI, 0x336699,
        0x336699,
        "\033[0;38;5;60;48;5;60m", "cubus griseum vincit");
    {
        chorda captum = _emittere(piscina, TESSERA_COLORES_CCLVI,
            (i32)TESSERA_COLOR_NATIVUS, 0xFF0000);

        /* nativus: nulla 38 */
        CREDO_VERUM (_continet(captum, "\033[0;48;5;196m"));
        CREDO_FALSUM (_continet(captum, "38;"));
        CREDO_FALSUM (_continet(captum, ";2;"));
    }
    {
        TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(piscina,
            IV, I);
        TesseraOpus* opus = tessera_aperire(piscina, &pm->pons);

        /* cellula RGB verum servat: quantizatio in emissione sola */
        tessera_colores_ponere(opus, TESSERA_COLORES_CCLVI);
        tessera_cellulam_ponere(opus, ZEPHYRUM, ZEPHYRUM, (i32)'a',
            tessera_stilus(0x336699, TESSERA_COLOR_NATIVUS, ZEPHYRUM));
        CREDO_AEQUALIS_I32 (tessera_cellulam_legere(opus, ZEPHYRUM,
            ZEPHYRUM).color_litterae, 0x336699);
    }

    imprimere("\n--- III. Ambitus ---\n");
    unsetenv("COLORTERM");
    unsetenv("TERM_PROGRAM");
    CREDO_AEQUALIS_I32 ((i32)tessera_colores_ambitus(),
        (i32)TESSERA_COLORES_PLENI);
    CREDO_AEQUALIS_I32 ((i32)tessera_politica_ambitus(),
        (i32)TESSERA_POLITICA_GRAPHEMATUM);
    setenv("TERM_PROGRAM", "Apple_Terminal", I);
    CREDO_AEQUALIS_I32 ((i32)tessera_colores_ambitus(),
        (i32)TESSERA_COLORES_CCLVI);
    CREDO_AEQUALIS_I32 ((i32)tessera_politica_ambitus(),
        (i32)TESSERA_POLITICA_SIMPLEX);
    /* COLORTERM truecolor vincit (terminal se XXIV bitorum dicit) */
    setenv("COLORTERM", "truecolor", I);
    CREDO_AEQUALIS_I32 ((i32)tessera_colores_ambitus(),
        (i32)TESSERA_COLORES_PLENI);
    setenv("COLORTERM", "24bit", I);
    CREDO_AEQUALIS_I32 ((i32)tessera_colores_ambitus(),
        (i32)TESSERA_COLORES_PLENI);
    setenv("COLORTERM", "aliud", I);
    CREDO_AEQUALIS_I32 ((i32)tessera_colores_ambitus(),
        (i32)TESSERA_COLORES_CCLVI);
    setenv("TERM_PROGRAM", "ghostty", I);
    unsetenv("COLORTERM");
    CREDO_AEQUALIS_I32 ((i32)tessera_colores_ambitus(),
        (i32)TESSERA_COLORES_PLENI);

    imprimere("\n--- IV. Ornamenta ---\n");
    {
        constans i32 nativus = (i32)TESSERA_COLOR_NATIVUS;
        constans i32 omnia = TESSERA_ORNAMENTUM_CRASSUM
            | TESSERA_ORNAMENTUM_OBSCURUM | TESSERA_ORNAMENTUM_CURSIVUM
            | TESSERA_ORNAMENTUM_SUBLINEATUM
            | TESSERA_ORNAMENTUM_INVERSUM
            | TESSERA_ORNAMENTUM_TRANSFIXUM;

        _ornatam_probare(piscina, TESSERA_COLORES_PLENI, nativus,
            nativus, TESSERA_ORNAMENTUM_CRASSUM, "\033[0;1m");
        _ornatam_probare(piscina, TESSERA_COLORES_PLENI, nativus,
            nativus, TESSERA_ORNAMENTUM_OBSCURUM, "\033[0;2m");
        _ornatam_probare(piscina, TESSERA_COLORES_PLENI, nativus,
            nativus, TESSERA_ORNAMENTUM_CURSIVUM, "\033[0;3m");
        _ornatam_probare(piscina, TESSERA_COLORES_PLENI, nativus,
            nativus, TESSERA_ORNAMENTUM_SUBLINEATUM, "\033[0;4m");
        _ornatam_probare(piscina, TESSERA_COLORES_PLENI, nativus,
            nativus, TESSERA_ORNAMENTUM_INVERSUM, "\033[0;7m");
        _ornatam_probare(piscina, TESSERA_COLORES_PLENI, nativus,
            nativus, TESSERA_ORNAMENTUM_TRANSFIXUM, "\033[0;9m");
        /* omnia + colores: XVII parametra (maxima tesserae < XXIV) */
        _ornatam_probare(piscina, TESSERA_COLORES_PLENI, 0xFF0000,
            0x336699, omnia,
            "\033[0;1;2;3;4;7;9;38;2;255;0;0;48;2;51;102;153m");
        _ornatam_probare(piscina, TESSERA_COLORES_CCLVI, 0xFF0000,
            0x808080, omnia, "\033[0;1;2;3;4;7;9;38;5;196;48;5;244m");
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
