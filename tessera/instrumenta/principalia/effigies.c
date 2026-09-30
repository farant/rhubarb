/* effigies.c - Spectator imaginum in cellulis terminalis (quadrans Q5;
 * terminal verum!)
 *
 * Imaginem (PNG, JPEG ... per imago) ad scrinium aptat sine distortione
 * (effigies_mensurare, aspectus cellulae 1:2), per IMAGO_SCALA_AREA
 * scalat, in cellulas quadrantum vertit (quadrans) et in medio pingit.
 * Linea status: titulus, mensurae, modus, profunditas colorum, error
 * reconstructionis (quadrans_error).
 *
 * Claves: m = modus (QUADRANTES / DIMIDIUM); c = colores (MEDIA /
 * EXTREMA, planum D2 - oculo iudicandum); p = paletta Aquinas (Q6:
 * XVI colores per Atkinson; error contra imaginem NON diffusam);
 * ] / dextra = imago proxima; [ / sinistra = prior; q = exire.
 * Amplitudo mutata: iterum aptatur.
 *
 * Curre per: ./tessera/effigies.sh [imagines...] (ordinarie: imagines
 * probationis in probationes/fixa/quadrans)
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "imago.h"
#include "imago_opus.h"
#include "quadrans.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_posix.h"
#include "tessera_eventum.h"
#include "tessera_opus.h"
#include "effigies_pictura.h"
#include <stdio.h>
#include <string.h>

#define IMAGINES_MAXIMAE 64

nomen structura {
     constans character* titulus;
                  Imago  imago;
} EffigiesImago;

/* Titulus ex via: basis sine suffixo */
interior constans character*
_titulus (
    constans character* via,
               Piscina* piscina)
{
     constans character* basis = strrchr(via, '/');
     constans character* punctum;
                    i32  mensura;
              character* titulus;

    basis    = (basis != NIHIL) ? basis + I : via;
    punctum  = strrchr(basis, '.');
    mensura  = (punctum != NIHIL) ? (i32)(punctum - basis)
                                  : (i32)strlen(basis);
    titulus  = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura + I);
    si (titulus == NIHIL)
    {
        redde via;
    }
    memcpy(titulus, basis, (memoriae_index)mensura);
    titulus[mensura] = '\0';
    redde titulus;
}

/* Imaginem currentem aptare, computare, pingere; lineam status
 * scribere */
interior vacuum
_pingere (
               TesseraOpus* opus,
    constans EffigiesImago* e,
                       i32  index,
                       i32  numerus,
             QuadransModus  modus,
           QuadransColores  colores,
                       b32  paletta)
{
    TesseraStilus nativus = tessera_stilus_nativus();
    TesseraStilus status  = tessera_stilus(TESSERA_COLOR_NATIVUS,
        TESSERA_COLOR_NATIVUS, TESSERA_ORNAMENTUM_INVERSUM);
             i32 lat     = tessera_latitudo(opus);
             i32 alt     = tessera_altitudo(opus) > I
                 ? tessera_altitudo(opus) - I : I;
         Piscina* arena   =
             piscina_generare_dynamicum("effigies_quadrum",
             4194304);
        character linea[CCLVI];
              i32 sub_lat  = ZEPHYRUM;
              i32 sub_alt  = ZEPHYRUM;
              i32 c_lat    = ZEPHYRUM;
              i32 c_alt    = ZEPHYRUM;
              i32 error    = ZEPHYRUM;

    tessera_purgare(opus, nativus);
    si (arena != NIHIL)
    {
        QuadransOptiones o = quadrans_optiones_ordinariae();

        o.modus    = modus;
        o.colores  = colores;
        effigies_mensurare(e->imago.latitudo, e->imago.altitudo, lat,
            alt, modus, EFFIGIES_ASPECTUS_ORDINARIUS, &sub_lat,
            &sub_alt);
        si (sub_lat > ZEPHYRUM && sub_alt > ZEPHYRUM)
        {
            Imago scalata = imago_scalare(&e->imago, sub_lat, sub_alt,
                IMAGO_SCALA_AREA, arena);
            QuadransCellula* cellulae;

            quadrans_mensurare(&o, sub_lat, sub_alt, &c_lat, &c_alt);
            cellulae = (QuadransCellula*)piscina_allocare(arena,
                (memoriae_index)(c_lat * c_alt)
                    * magnitudo(QuadransCellula));
            si (scalata.pixela != NIHIL && cellulae != NIHIL)
            {
                /* paletta: cellulae ex imagine diffusa, error tamen
                 * contra scalatam (pretium aspectus verum) */
                Imago fons = paletta
                    ? effigies_palettam_applicare(&scalata, arena)
                    : scalata;

                si (fons.pixela == NIHIL)
                {
                    fons = scalata;
                }
                quadrans_computare(&fons, ZEPHYRUM, ZEPHYRUM, sub_lat,
                    sub_alt, &o, cellulae);
                effigies_pingere(opus, (s32)((lat - c_lat) / II),
                    (s32)((alt - c_alt) / II), cellulae, c_lat, c_alt);
                error = quadrans_error(&scalata, ZEPHYRUM, ZEPHYRUM,
                    sub_lat, sub_alt, &o, cellulae);
            }
        }
        piscina_destruere(arena);
    }
    tessera_replere(opus, ZEPHYRUM, (s32)alt, (s32)lat, I, (i32)' ',
        status);
    sprintf(linea, " %s %u/%u | %ux%u -> %ux%u cellulae | %s %s%s | %s "
        "| error %u.%02u | m c p ] [ q", e->titulus,
        (insignatus integer)(index + I), (insignatus integer)numerus,
        (insignatus integer)e->imago.latitudo,
        (insignatus integer)e->imago.altitudo,
        (insignatus integer)c_lat, (insignatus integer)c_alt,
        (modus == QUADRANS_DIMIDIUM) ? "DIMIDIUM" : "QUADRANTES",
        (colores == QUADRANS_EXTREMA) ? "EXTREMA" : "MEDIA",
        paletta ? " PALETTA" : "",
        (opus->colores == TESSERA_COLORES_CCLVI) ? "CCLVI" : "PLENI",
        (insignatus integer)(error / C), (insignatus integer)(error
            % C));
    tessera_scribere_literis(opus, ZEPHYRUM, (s32)alt, linea, status);
    tessera_cursorem_ponere(opus, -I, -I);
    (vacuum)tessera_praesentare(opus);
}

s32
principale (
                    s32  numerus_argumentorum,
     constans character* argumenta[])
{
         Piscina* piscina;
     TesseraPons* pons;
     TesseraOpus* opus;
   TesseraLector* initus;
   EffigiesImago* imagines;
             i32  numerus = ZEPHYRUM;
             i32  index   = ZEPHYRUM;
    QuadransModus modus   = QUADRANS_QUADRANTES;
  QuadransColores colores = QUADRANS_MEDIA;
             b32  paletta = FALSUM;
             b32  currens = VERUM;
             s32  k;

    si (numerus_argumentorum < II)
    {
        fprintf(stderr, "usus: effigies imago [...]\n");
        redde I;
    }
    piscina = piscina_generare_dynamicum("effigies", 67108864);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "effigies: piscina deest\n");
        redde I;
    }
    imagines = (EffigiesImago*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)IMAGINES_MAXIMAE
            * (memoriae_index)magnitudo(EffigiesImago), VIII);
    per (k = I; k < numerus_argumentorum && numerus < IMAGINES_MAXIMAE;
         k++)
    {
        ImagoFructus f = imago_caricare_ex_file(argumenta[k], piscina);

        si (!f.successus)
        {
            fprintf(stderr, "effigies: %s legi non potest\n",
                argumenta[k]);
            perge;
        }
        imagines[numerus].titulus  = _titulus(argumenta[k], piscina);
        imagines[numerus].imago    = f.imago;
        numerus++;
    }
    si (numerus == ZEPHYRUM)
    {
        fprintf(stderr, "effigies: nulla imago\n");
        redde I;
    }
    pons = tessera_pons_posix_creare(piscina);
    si (pons == NIHIL)
    {
        fprintf(stderr,
            "effigies: terminal verum requiritur (isatty)\n");
        redde I;
    }
    opus    = tessera_aperire(piscina, pons);
    initus  = tessera_lector_creare(piscina, pons);
    si (opus == NIHIL || initus == NIHIL)
    {
        fprintf(stderr, "effigies: apertura fracta\n");
        redde I;
    }
    tessera_politicam_ponere(opus, tessera_politica_ambitus());
    tessera_colores_ponere(opus, tessera_colores_ambitus());

    dum (currens)
    {
        TesseraEventum ev;

        _pingere(opus, &imagines[index], index, numerus, modus,
            colores, paletta);
        commutatio (tessera_eventum_expectare(initus, &ev, -I))
        {
            casus TESSERA_EVENTUM_CLAVIS:
                si (ev.runa == (s32)'q')
                {
                    currens = FALSUM;
                }
                alioquin si (ev.runa == (s32)'m')
                {
                    modus = (modus == QUADRANS_QUADRANTES)
                        ? QUADRANS_DIMIDIUM : QUADRANS_QUADRANTES;
                }
                alioquin si (ev.runa == (s32)'p')
                {
                    paletta = (b32)!paletta;
                }
                alioquin si (ev.runa == (s32)'c')
                {
                    colores = (colores == QUADRANS_MEDIA)
                        ? QUADRANS_EXTREMA : QUADRANS_MEDIA;
                }
                alioquin si (   ev.runa   == (s32)']'
                             || ev.clavis == TESSERA_CLAVIS_DEXTRA)
                {
                    index = (index + I) % numerus;
                }
                alioquin si (   ev.runa   == (s32)'['
                             || ev.clavis == TESSERA_CLAVIS_SINISTRA)
                {
                    index = (index + numerus - I) % numerus;
                }
                frange;
            casus TESSERA_EVENTUM_AMPLITUDO:
            casus TESSERA_EVENTUM_RESUMPTUM:
                tessera_magnitudinem_renovare(opus);
                frange;
            ordinarius:
                frange;
        }
    }

    tessera_claudere(opus);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
