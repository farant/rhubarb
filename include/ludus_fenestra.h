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

/* pulsus applicationis vivae (vicus-latera S1c): vocatur iteratione
 * ansae quaque; VERUM = aliquid mutatum, pingendum */
nomen b32 (*LudusPulsator)(vacuum* ctx);

nomen structura {
        Dispensator* d;
    FiguraRegistrum* figurae;
                i32  thema;
          ImagoFons  fons;
             vacuum* fons_ctx;
    TabulaPixelorum* tabula;
            Piscina* piscina;        /* 013 B3b: tabula renovanda */
            Piscina* piscina_quadri;
            Mandata* mandata;        /* quadri ultimi */
      LudusMensurae  mensurae;
                b32  magnitudo_nuntiata;  /* 013 B1: initialis missa */
      LudusPulsator  pulsator;            /* S1c: NIHIL = nullus */
             vacuum* pulsator_ctx;
                i32  versio_picta;        /* S1c: repositorii, quadro
                                           * ultimo (summa generum) */
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

/* eventus in dispensatorem; tempus ZEPHYRUM stampatur 'nunc'.
 * MUTARE_MAGNITUDINEM (puncta contenti fenestrae): tabula ad fenestram
 * aptatur SCALA SERVATA (013 B3b) et eventus in PIXELA NOSTRA
 * (tabulae) rescribitur ante traditionem. Ante
 * eventum primum (aut quadrum primum) magnitudo tabulae semel
 * nuntiatur (MUTARE_MAGNITUDINEM) - superficies status est (013 B1). */
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

/* mora ante quadrum proximum: ZEPHYRUM ante primum (quadrum
 * statim), deinde quies dispensatoris. fenestra_expectare_eventus
 * eventu adveniente excitatur, ergo tractus non tardatur; fenestra
 * otiosa ~0% CPU (olim perscrutari sine mora: C%). */
Mora
ludus_fenestra_mora (
    constans LudusFenestra* lf);

/* ansa vera: exspectare (ludus_fenestra_mora), tractare, quadrum,
 * praesentare; finis in EVENTUS_CLAUDERE aut post quadra_maxima
 * (> 0), deinde dispensator_finire (pendentia effunduntur).
 * Mensurae ad stdout. */
s32
ludus_fenestra_currere (
    LudusFenestra* lf,
         Fenestra* fenestra,
              i32  quadra_maxima);

/* Quadrum ultimum (tabulam) in plagulam PNG scribere - pixela ipsa
 * quae fenestrae praesentata sunt aut praesentarentur (etiam sine
 * fenestra, in tabula_pixelorum_creare_nuda). FALSUM si nullum
 * quadrum adhuc aut scriptio fracta. Memoria ex piscina quadri
 * (quadro proximo vacatur). */
b32
ludus_fenestra_imaginem_scribere (
    constans LudusFenestra* lf,
        constans character* via);

/* vicus-latera S1c: pulsum ponere (NIHIL tollit). Cum pulsu ansa
 * fenestrae XVI ms ad summum exspectat et quadrum SOLUM pingit cum
 * ludus_fenestra_pingendum VERUM reddit. */
vacuum
ludus_fenestra_pulsum_ponere (
    LudusFenestra* lf,
    LudusPulsator  fn,
           vacuum* ctx);

/* decisio iterationis (publica ut sine fenestra probetur): pulsum
 * vocat; si nulla eventa et pulsus nihil novi vidit, dispensatorem
 * SOLUM pulsat (horologia, e.g. scriptura differta scribae) et
 * reddit an recomposuit. VERUM = quadrum pingendum. Sine pulsu
 * semper VERUM (mores prior). */
b32
ludus_fenestra_pingendum (
    LudusFenestra* lf,
              b32  eventa,
              s64  nunc);

#endif /* LUDUS_FENESTRA_H */
