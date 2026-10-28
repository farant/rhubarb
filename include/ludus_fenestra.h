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
 * Rotula: EVENTUS_MUS_ROTULA ex ansa perscrutandi fenestrae venit
 * (fenestra_macos.m, NSEventTypeScrollWheel). Methodus scrollWheel:
 * ibi vacua manet - eventus non inde oriuntur.
 *
 * Ansa: ludus_fenestra_currere (fenestra perscrutabilis - macOS, testa
 * lib/testa.c, programmata briar); Android partes solas ordinat
 * (tractare, pingendum, quadrum, mora; lib/testa_android.c).
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
#include "reactor.h"

nomen structura {
    i32 quadra;
    s64 ms_compositionis;     /* intra pulsum */
    s64 ms_pingendi;
    s64 ms_delineandi;
    s64 ms_quadri_maximum;
    /* vicus quies T1: circuitus ansae (ludus_fenestra_currere, post
     * exspectationem quamque) et vocationes pulsatoris */
    s64 excitationes;
    s64 pulsus;
} LudusMensurae;

/* pulsus applicationis vivae (vicus-latera S1c): vocatur iteratione
 * ansae quaque; VERUM = aliquid mutatum, pingendum */
nomen b32 (*LudusPulsator)(vacuum* ctx);

/* vicus quies T5: mora applicationis - ms usque ad pulsum proximum
 * necessarium; -1 = nullus (solum eventus aut fossae) */
nomen Mora (*LudusMoraProxima)(vacuum* ctx);

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
                                           * ultimo */
    /* vicus quies T5 (ludus_fenestra_quietem_ponere) */
                b32  quies;
   LudusMoraProxima  mora_proxima;        /* NIHIL licet */
             vacuum* mora_ctx;
            Reactor* reactor;             /* NIHIL licet */
                b32  fossae_positae;      /* fenestrae traditae */
                i32  fossae_versio;       /* reactoris, tunc */
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
 * statim), deinde quies dispensatoris - cum quiete
 * (ludus_fenestra_quietem_ponere) minimum morarum debitarum, -1 si
 * nulla. fenestra_expectare_eventus
 * eventu adveniente excitatur, ergo tractus non tardatur; fenestra
 * otiosa ~0% CPU (olim perscrutari sine mora: C%). */
Mora
ludus_fenestra_mora (
    constans LudusFenestra* lf);

/* nec Android nec iOS (testa-ios R1: clang praedefinit
 * __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ in machina et in
 * simulatore): testae eorum quadra per systema ducunt */
#if !defined(__ANDROID__) \
    && !defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
/* ansa vera: exspectare (ludus_fenestra_mora), tractare, quadrum,
 * praesentare; finis in EVENTUS_CLAUDERE aut post quadra_maxima
 * (> 0), deinde dispensator_finire (pendentia effunduntur).
 * Mensurae ad stdout. */
s32
ludus_fenestra_currere (
    LudusFenestra* lf,
         Fenestra* fenestra,
              i32  quadra_maxima);
#endif /* nec Android nec iOS */

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

/* Quies: cum posita, ansa non XVI ms sed usque ad minimum exspectat -
 * mora applicationis, reactor (reactor_mora_proxima), dispensator
 * (dispensator_mora_proxima); fossae reactoris ansam expergefaciunt
 * (fenestra_fossas_ponere, renovatae solum cum reactor_versio_fossarum
 * mutatur). Nihil debitum: ludus_fenestra_mora -1 et ansa sine fine
 * exspectat. mora et reactor NIHIL licent. Sine hac vocatione mos
 * vetus (XVI ms dum pulsator adest). */
vacuum
ludus_fenestra_quietem_ponere (
       LudusFenestra* lf,
    LudusMoraProxima  mora,
              vacuum* ctx,
             Reactor* reactor);

/* communio-plan-5 T5: reactor SINE quiete - ansa eum omni excitatione
 * agit (reactor_poll 0) et non diutius quam usque ad horam eius
 * proximam exspectat; fossae ansam non expergefaciunt (excitatio
 * ordinaria, quies_ms dispensatoris, sufficit). NIHIL tollit. */
vacuum
ludus_fenestra_reactorem_ponere (
    LudusFenestra* lf,
          Reactor* reactor);

#endif /* LUDUS_FENESTRA_H */
