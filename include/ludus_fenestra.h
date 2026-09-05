/* ludus_fenestra.h - glutinum fenestrae: eventus -> dispensator,
 * quadrum -> pingere -> delineare -> praesentare
 *
 * Unicum locus ubi ludus horologium tangit (fenestra_tempus_ms) et
 * unicus qui fenestram videt. Quadrum = pulsus (sedes quietis +
 * recompositio), pingere arboris ultimae in piscinam quadri,
 * rasterizatio. Mensurae per quadrum: compositio (in pulsu),
 * pingere, delineare - causa optionis status duplicati (brainstorm
 * XVI §3): si delineare quadrum dominatur, rasterizare quadrum N dum
 * quadrum N+1 dispensatur.
 *
 * Rotula: fenestra_macos.m scrollWheel VACUUS est (2026-09-05) -
 * tractio P4 (zoom ad cursorem).
 */

#ifndef LUDUS_FENESTRA_H
#define LUDUS_FENESTRA_H

/* <aedilis corpus="lib/ludus_fenestra.c"/> */

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "dispensator.h"
#include "figura.h"
#include "mandatum.h"
#include "delineare_mandata.h"

nomen structura {
    i32 quadra;
    s64 ms_compositionis;     /* intra pulsum */
    s64 ms_pingendi;
    s64 ms_delineandi;
    s64 ms_quadri_maximum;
} LudusMensurae;

nomen structura {
        Dispensator* d;
    FiguraRegistrum* figurae;
                i32  thema;
          ImagoFons  fons;
             vacuum* fons_ctx;
    TabulaPixelorum* tabula;
            Piscina* piscina_quadri;
            Mandata* mandata;        /* quadri ultimi */
      LudusMensurae  mensurae;
} LudusFenestra;

LudusFenestra*
ludus_fenestra_creare (
            Piscina* piscina,
        Dispensator* d,
    FiguraRegistrum* figurae,
                i32  thema,
          ImagoFons  fons,
             vacuum* fons_ctx,
    TabulaPixelorum* tabula);

/* eventus in dispensatorem; tempus ZEPHYRUM stampatur 'nunc' */
vacuum
ludus_fenestra_tractare (
       LudusFenestra* lf,
    constans Eventus* ev,
                 s64  nunc);

/* quadrum unum: pulsus, pingere, delineare (non praesentat) */
vacuum
ludus_quadrum (
    LudusFenestra* lf,
              s64  nunc);

/* ansa vera: perscrutari, tractare, quadrum, praesentare; finis in
 * EVENTUS_CLAUDERE aut post quadra_maxima (> 0). Mensurae ad stdout. */
s32
ludus_fenestra_currere (
    LudusFenestra* lf,
         Fenestra* fenestra,
              i32  quadra_maxima);

#endif /* LUDUS_FENESTRA_H */
