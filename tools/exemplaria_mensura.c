/* exemplaria_mensura.c - MENSURA EXEMPLARIORUM: ms per quadrum
 * (atkinson, classica, velox) in fixis probationes/fixa/exemplaria/,
 * lapis ad CCCLX, DC, DCCXX latitudinis, pratum ad DC; media N
 * vocationum (clock, ut xar_mensura). Ad DC imagines scribit in
 * build/exemplaria_velox/: <imago>_classica.png, _velox.png, _par.png
 * (classica sinistra, velox dextra) et fractionem pixelorum diversam.
 *
 * Vocatio quaeque piscinam recentem accipit (post destruitur): tempus
 * solam vocationem metitur; arena = piscina_summa_usus post vocationem
 * (MB), classica et velox.
 *
 * Usus: ./bin/aedilis tools/exemplaria_mensura.c && bash
 *   build/aedilis/exemplaria_mensura/struere.sh &&
 *   ./build/aedilis/exemplaria_mensura/exemplaria_mensura [-n N]
 */
#include "latina.h"
#include "piscina.h"
#include "imago.h"
#include "imago_opus.h"
#include "imago_png.h"
#include "filum.h"
#include "thema.h"
#include "dithering.h"
#include "dithering_exemplaria.h"
#include "dithering_exemplaria_velox.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DIRECTORIUM "build/exemplaria_velox"
#define LATITUDO_FIXI DC

nomen enumeratio {
    MODUS_ATKINSON = 0,
    MODUS_CLASSICA,
    MODUS_VELOX
} ModusMensurae;

nomen structura {
    constans character* titulus;
    constans character* via;
                   s32  latitudo;   /* scalata ex fixo DC */
} Ordo;

hic_manens constans Ordo ORDINES[IV] = {
    { "lapis",  "probationes/fixa/exemplaria/lapis_600.png",  CCCLX },
    { "lapis",  "probationes/fixa/exemplaria/lapis_600.png",  DC },
    { "lapis",  "probationes/fixa/exemplaria/lapis_600.png",  DCCXX },
    { "pratum", "probationes/fixa/exemplaria/pratum_600.png", DC }
};

interior DitheringFructus
_dithering_facere (
      ModusMensurae  modus,
     constans Imago* im,
       constans b32  activi[XVI],
            Piscina* piscina)
{
    commutatio (modus)
    {
        casus MODUS_ATKINSON:
            redde dithering_atkinson_colorum(im->pixela, im->latitudo,
                im->altitudo, activi, piscina);
        casus MODUS_CLASSICA:
            redde dithering_exemplaria_colorum(im->pixela, im->latitudo,
                im->altitudo, activi, NIHIL, NIHIL, piscina);
        ordinarius:
            redde dithering_exemplaria_velox_colorum(im->pixela,
                im->latitudo, im->altitudo, activi, NIHIL, NIHIL,
                piscina);
    }
}

/* media ms N vocationum; arena_mb = usus vocationis ultimae */
interior f64
_metiri (
      ModusMensurae  modus,
     constans Imago* im,
       constans b32  activi[XVI],
                s32  n,
                f64* arena_mb)
{
    clock_t summa = 0;
        s32 k;

    per (k = 0; k < n; k++)
    {
        Piscina* p = piscina_generare_dynamicum("exemplaria_mensura",
            (memoriae_index)(LXIV * MXXIV * MXXIV));
        clock_t t0 = clock();
        (vacuum)_dithering_facere(modus, im, activi, p);
        summa += clock() - t0;
        si (arena_mb != NIHIL) *arena_mb = (f64)piscina_summa_usus(p)
                                   / (1024.0 * 1024.0);
        piscina_destruere(p);
    }
    redde (f64)summa * 1000.0 / (f64)CLOCKS_PER_SEC / (f64)n;
}

interior b32
_png_scribere (
           constans i8* indices,
                   s32  lat,
                   s32  alt,
    constans character* titulus,
    constans character* suffixum,
               Piscina* piscina)
{
        Imago out;
    character via[CCLVI];

    out.latitudo = (i32)lat;
    out.altitudo = (i32)alt;
    out.pixela    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(lat * alt * IV));
    dithering_indices_ad_rgba(indices, out.pixela, out.latitudo,
        out.altitudo);
    sprintf(via, "%s/%s_%s.png", DIRECTORIUM, titulus, suffixum);
    redde imago_png_scribere(&out, via, piscina).successus;
}

/* classica et velox, scriptae et iuxta positae; fractio diversa */
interior f64
_comparare (
        constans Imago* im,
          constans b32  activi[XVI],
    constans character* titulus,
               Piscina* piscina)
{
    DitheringFructus c = _dithering_facere(MODUS_CLASSICA, im, activi,
        piscina);
    DitheringFructus v = _dithering_facere(MODUS_VELOX, im, activi,
        piscina);
    s32 lat = (s32)im->latitudo, alt = (s32)im->altitudo, x, y;
    s32  diversa = 0;
     i8* par;

    si (!c.successus || !v.successus) redde -1.0;
    par = (i8*)piscina_allocare(piscina,
        (memoriae_index)(II * lat * alt));
    per (y = 0; y < alt; y++)
    {
        per (x = 0; x < lat; x++)
        {
            i8 a = c.indices[y * lat + x], b = v.indices[y * lat + x];
            si (a != b) diversa++;
            par[y * II * lat + x]        = a;
            par[y * II * lat + lat + x]  = b;
        }
    }
    (vacuum)_png_scribere(c.indices, lat, alt, titulus, "classica",
        piscina);
    (vacuum)_png_scribere(v.indices, lat, alt, titulus, "velox",
        piscina);
    (vacuum)_png_scribere(par, II * lat, alt, titulus, "par", piscina);
    redde 100.0 * (f64)diversa / (f64)(lat * alt);
}

s32
principale (
      integer   argc,
    character** argv)
{
    s32 n = X, i;
    b32 activi[XVI];

    si (argc == III && strcmp(argv[I], "-n") == 0) n = (s32)strtol(
                                                       argv[II], NIHIL,
                                                       X);
    alioquin si (argc != I)
    {
        fprintf(stderr, "usus: exemplaria_mensura [-n N]\n");
        redde II;
    }
    si (n < I) n = I;
    thema_initiare();
    dithering_praeparare_omnes(activi);
    si (!filum_directorium_creare_cum_parentibus(DIRECTORIUM))
    {
        fprintf(stderr, "directorium %s creari non potest\n",
            DIRECTORIUM);
        redde I;
    }
    imprimere("N = %d (media), ms per quadrum, arena MB\n", (integer)n);
    imprimere("imago   magnitudo  atkinson  classica     velox"
              "  velocitas  arena c/v    differentia\n");
    per (i = 0; i < IV; i++)
    {
        Piscina* p = piscina_generare_dynamicum("exemplaria_imago",
            (memoriae_index)(LXIV * MXXIV * MXXIV));
        ImagoFructus f = imago_caricare_ex_file(ORDINES[i].via, p);
               Imago im;
        f64 ta, tc, tv, mc = 0.0, mv = 0.0, d = -1.0;

        si (!f.successus)
        {
            fprintf(stderr, "%s legi non potest\n", ORDINES[i].via);
            piscina_destruere(p);
            redde I;
        }
        im = f.imago;
        si (ORDINES[i].latitudo != (s32)im.latitudo)
        {
            s32 lat = ORDINES[i].latitudo;
            s32 alt = (s32)((f64)im.altitudo * (f64)lat
                / (f64)im.latitudo + 0.5);
            im = imago_scalare(&f.imago, (i32)lat, (i32)alt,
                lat < (s32)im.latitudo ? IMAGO_SCALA_AREA
                : IMAGO_SCALA_BILINEARIS, p);
        }
        ta = _metiri(MODUS_ATKINSON, &im, activi, n, NIHIL);
        tc = _metiri(MODUS_CLASSICA, &im, activi, n, &mc);
        tv = _metiri(MODUS_VELOX, &im, activi, n, &mv);
        si (ORDINES[i].latitudo == LATITUDO_FIXI)
        {
            d = _comparare(&im, activi, ORDINES[i].titulus, p);
        }
        imprimere("%-7s %4ux%-4u %6.1f ms %6.0f ms %6.0f ms %8.1fx"
                  "  %4.0f/%-4.0f  ", ORDINES[i].titulus,
            (insignatus)im.latitudo, (insignatus)im.altitudo, ta, tc,
            tv, tc / tv, mc, mv);
        si (d >= 0.0)
        {
            imprimere("%6.2f %% (%lu px)\n", d, (insignatus longus)(d
                * (f64)im.latitudo * (f64)im.altitudo / 100.0 + 0.5));
        }
        alioquin imprimere("     -\n");
        piscina_destruere(p);
    }
    imprimere("imagines: %s/<imago>_{classica,velox,par}.png\n",
        DIRECTORIUM);
    redde 0;
}
