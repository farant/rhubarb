/* folium.c - Spectator corporis Lapidis (runae U7; terminal verum!)
 *
 * Textum ad latitudinem scrinii involvit (folium_pagina: involutio
 * NAIVA consulto), in paginas partitur, linguas commutat. Linea status:
 * lingua, pagina, statisticae latitudinis, politica.
 *
 * Claves: spatium / j / pag-deorsum / deorsum = pagina proxima;
 * k / pag-sursum / sursum = prior; ] / dextra = lingua proxima;
 * [ / sinistra = prior; g / domus = prima; G / finis = ultima;
 * q = exire. Amplitudo mutata: iterum involvitur; index lineae
 * primae servatur, ad initium paginae adstrictus.
 *
 * Curre per: ./tessera/folium.sh [fasciculi...] (ordinarie: omnes
 * fasciculi .txt in probationes/fixa/runae/corpus)
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "runae.h"
#include "tessera_cellula.h"
#include "tessera_pons.h"
#include "tessera_pons_posix.h"
#include "tessera_eventum.h"
#include "tessera_opus.h"
#include "folium_pagina.h"
#include <stdio.h>
#include <string.h>

#define DOCUMENTA_MAXIMA 256

nomen structura {
     constans character* via;
     constans character* titulus;   /* basis sine '.txt' */
                 chorda  textus;
} FoliumDocumentum;

nomen structura {
        TesseraOpus* opus;
      RunaePolitica  politica;
   FoliumDocumentum* documenta;
                i32  numerus;
                i32  currens;
            Piscina* arena;          /* involutionis currentis */
    FoliumInvolutio  involutio;
                i32  prima;          /* linea prima visibilis */
} Folium;

/* Altitudo textus: scrinium minus linea status */
interior i32
_altitudo (
    constans Folium* f)
{
    i32 alt = tessera_altitudo(f->opus);

    redde (alt > I) ? alt - I : I;
}

/* Documentum currens ad latitudinem scrinii involvere; lineam primam
 * ad initium paginae suae adstringere */
interior vacuum
_involvere (
    Folium* f)
{
    si (f->arena != NIHIL)
    {
        piscina_destruere(f->arena);
    }
    f->arena = piscina_generare_dynamicum("folium_involutio", 1048576);
    si (   f->arena == NIHIL
        || !folium_involvere(f->arena, f->documenta[f->currens].textus,
               tessera_latitudo(f->opus), f->politica, &f->involutio))
    {
        f->involutio.numerus = ZEPHYRUM;
    }
    si (f->prima >= f->involutio.numerus)
    {
        f->prima = (f->involutio.numerus > ZEPHYRUM)
            ? f->involutio.numerus - I : ZEPHYRUM;
    }
    f->prima -= f->prima % _altitudo(f);
}

interior i32
_paginae (
    constans Folium* f)
{
    i32 alt = _altitudo(f);

    redde (f->involutio.numerus + alt - I) / alt;
}

interior vacuum
_pingere (
    Folium* f)
{
       TesseraStilus nativus  = tessera_stilus_nativus();
       TesseraStilus status   = tessera_stilus(TESSERA_COLOR_NATIVUS,
           TESSERA_COLOR_NATIVUS, TESSERA_ORNAMENTUM_INVERSUM);
    constans FoliumDocumentum* d = &f->documenta[f->currens];
                    character  linea[CCLVI];
                          s32  x;

    tessera_purgare(f->opus, nativus);
    folium_paginam_pingere(f->opus, d->textus, &f->involutio, f->prima,
        _altitudo(f), nativus);
    tessera_replere(f->opus, ZEPHYRUM, (s32)_altitudo(f),
        (s32)tessera_latitudo(f->opus), I, (i32)' ', status);
    sprintf(linea,
        " %s %u/%u | pagina %u/%u | unitates %u, columnae %u, "
        "latae %u, nullae %u | %s | spatium k ] [ g G q",
        d->titulus, (insignatus integer)(f->currens + I),
        (insignatus integer)f->numerus,
        (insignatus integer)(f->prima / _altitudo(f) + I),
        (insignatus integer)_paginae(f),
        (insignatus integer)f->involutio.unitates,
        (insignatus integer)f->involutio.columnae,
        (insignatus integer)f->involutio.latae,
        (insignatus integer)f->involutio.nullae,
        (f->politica == RUNAE_POLITICA_SIMPLEX) ? "SIMPLEX"
                                                 : "GRAPHEMATUM");
    x = ZEPHYRUM;
    tessera_scribere_literis(f->opus, x, (s32)_altitudo(f), linea,
        status);
    tessera_cursorem_ponere(f->opus, -I, -I);
    (vacuum)tessera_praesentare(f->opus);
}

/* Titulus ex via: basis sine '.txt' */
interior constans character*
_titulus (
    constans character* via,
               Piscina* piscina)
{
    constans character* basis = strrchr(via, '/');
                   i32  mensura;
             character* titulus;

    basis    = (basis != NIHIL) ? basis + I : via;
    mensura  = (i32)strlen(basis);
    si (   mensura > IV
        && strcmp(basis + mensura - IV, ".txt") == ZEPHYRUM)
    {
        mensura -= IV;
    }
    titulus = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura + I);
    si (titulus == NIHIL)
    {
        redde via;
    }
    memcpy(titulus, basis, (memoriae_index)mensura);
    titulus[mensura] = '\0';
    redde titulus;
}

s32
principale (
                   s32  numerus_argumentorum,
    constans character* argumenta[])
{
             Piscina* piscina;
         TesseraPons* pons;
       TesseraLector* initus;
              Folium  f;
                 s32  k;
                 b32  currens = VERUM;

    si (numerus_argumentorum < II)
    {
        fprintf(stderr, "usus: folium fasciculus.txt [...]\n");
        redde I;
    }
    piscina = piscina_generare_dynamicum("folium", 16777216);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "folium: piscina deest\n");
        redde I;
    }
    f.documenta = (FoliumDocumentum*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)DOCUMENTA_MAXIMA
            * (memoriae_index)magnitudo(FoliumDocumentum), VIII);
    f.numerus = ZEPHYRUM;
    per (k = I; k < numerus_argumentorum
        && f.numerus < DOCUMENTA_MAXIMA; k++)
    {
        chorda textus = filum_legere_totum(argumenta[k], piscina);

        si (textus.datum == NIHIL)
        {
            fprintf(stderr, "folium: %s legi non potest\n",
                argumenta[k]);
            perge;
        }
        f.documenta[f.numerus].via = argumenta[k];
        f.documenta[f.numerus].titulus = _titulus(argumenta[k],
            piscina);
        f.documenta[f.numerus].textus = textus;
        f.numerus++;
    }
    si (f.numerus == ZEPHYRUM)
    {
        fprintf(stderr, "folium: nullum documentum\n");
        redde I;
    }
    pons = tessera_pons_posix_creare(piscina);
    si (pons == NIHIL)
    {
        fprintf(stderr, "folium: terminal verum requiritur (isatty)\n");
        redde I;
    }
    f.opus = tessera_aperire(piscina, pons);
    initus = tessera_lector_creare(piscina, pons);
    si (f.opus == NIHIL || initus == NIHIL)
    {
        fprintf(stderr, "folium: apertura fracta\n");
        redde I;
    }
    /* latitudo graphematum ut terminal hic eam metitur (ambitus) */
    tessera_politicam_ponere(f.opus, tessera_politica_ambitus());
    /* profunditas colorum ex ambitu (quadrans Q4) */
    tessera_colores_ponere(f.opus, tessera_colores_ambitus());
    f.politica = (f.opus->politica == TESSERA_POLITICA_SIMPLEX)
        ? RUNAE_POLITICA_SIMPLEX : RUNAE_POLITICA_GRAPHEMATUM;
    f.currens  = ZEPHYRUM;
    f.prima    = ZEPHYRUM;
    f.arena    = NIHIL;
    _involvere(&f);

    dum (currens)
    {
        TesseraEventum ev;
                   i32 alt;

        _pingere(&f);
        alt = _altitudo(&f);
        commutatio (tessera_eventum_expectare(initus, &ev, -I))
        {
            casus TESSERA_EVENTUM_CLAVIS:
                si (ev.runa == (s32)'q')
                {
                    currens = FALSUM;
                }
                alioquin si (   ev.runa   == (s32)' '
                             || ev.runa   == (s32)'j'
                             || ev.clavis
                                 == TESSERA_CLAVIS_PAGINA_DEORSUM
                             || ev.clavis == TESSERA_CLAVIS_DEORSUM)
                {
                    si (f.prima + alt < f.involutio.numerus)
                    {
                        f.prima += alt;
                    }
                }
                alioquin si (   ev.runa   == (s32)'k'
                             || ev.clavis
                                 == TESSERA_CLAVIS_PAGINA_SURSUM
                             || ev.clavis == TESSERA_CLAVIS_SURSUM)
                {
                    f.prima = (f.prima >= alt) ? f.prima - alt
                                               : ZEPHYRUM;
                }
                alioquin si (   ev.runa   == (s32)'g'
                             || ev.clavis == TESSERA_CLAVIS_DOMUS)
                {
                    f.prima = ZEPHYRUM;
                }
                alioquin si (   ev.runa   == (s32)'G'
                             || ev.clavis == TESSERA_CLAVIS_FINIS)
                {
                    f.prima = (_paginae(&f) > ZEPHYRUM)
                        ? (_paginae(&f) - I) * alt : ZEPHYRUM;
                }
                alioquin si (   ev.runa   == (s32)']'
                             || ev.clavis == TESSERA_CLAVIS_DEXTRA)
                {
                    f.currens  = (f.currens + I) % f.numerus;
                    f.prima    = ZEPHYRUM;
                    _involvere(&f);
                }
                alioquin si (   ev.runa   == (s32)'['
                             || ev.clavis == TESSERA_CLAVIS_SINISTRA)
                {
                    f.currens = (f.currens + f.numerus - I)
                        % f.numerus;
                    f.prima = ZEPHYRUM;
                    _involvere(&f);
                }
                frange;
            casus TESSERA_EVENTUM_AMPLITUDO:
            casus TESSERA_EVENTUM_RESUMPTUM:
                tessera_magnitudinem_renovare(f.opus);
                _involvere(&f);
                frange;
            ordinarius:
                frange;
        }
    }

    tessera_claudere(f.opus);
    si (f.arena != NIHIL)
    {
        piscina_destruere(f.arena);
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
