#ifndef TESTA_CURSUS_H
#define TESTA_CURSUS_H

/* <aedilis corpus="lib/testa_cursus.c"/> */
/* testa_cursus.h - contractus applicationis (testa-spec II.1) et
 * cursus communis testarum: initium -> fenestra -> eventus/quadra ->
 * finis. Testae (lib/testa.c, lib/testa_android.c) id vocant;
 * probationes quoque, quia principale NON affert (testa.h affert). */

#include "latina.h"
#include "piscina.h"
#include "imago.h"
#include "eventus.h"
#include "dispensator.h"
#include "figura.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "reactor.h"
#include "veredarius.h"
#include "claviatura.h"

/* clipboard: idem genus ac actiones_ctx.imago_capere pictoris */
nomen ImagoFructus (*TestaImagoCapere)(vacuum* ctx, Piscina* piscina);

/* clipboard: textum ponere (idem genus ac actiones_ctx.textum_exportare
 * scribae); FALSUM = non positum */
nomen b32 (*TestaTextumPonere)(vacuum* ctx, chorda textus);

/* testa-ios C1b: usorem imaginem eligere sinere (non obstruens).
 * VERUM = electio ostensa; imago, si eligitur, POSTEA ut
 * EVENTUS_DEPOSITIO (via plagulae) venit. */
nomen b32 (*TestaImaginemEligere)(vacuum* ctx);

/* quod testa applicationi praebet */
nomen structura {
               Piscina* piscina;            /* testa possidet et destruit */
                   s32  argc;               /* argv integer: app suas */
            character** argv;               /* optiones legit */
                   b32  fumus;              /* -fumus */
                   i32  schirmi_latitudo;   /* puncta (macOS) / pixela */
                   i32  schirmi_altitudo;   /* (Android); 0 = ignotus */
                   i32  utilis_x;           /* area utilis (sine menu, */
                   i32  utilis_y;           /* navali), origo fenestrae */
                   i32  utilis_latitudo;    /* 0 = ignota */
                   i32  utilis_altitudo;
    constans character* sedes_datorum;      /* macOS ".", Android
                                             * internalDataPath (= cwd) */
                   i32 superficies_latitudo; /* puncta quae testa */
                   i32 superficies_altitudo; /* IMPONIT (Android: area
                                             * contenti; documenta nova
                                             * ei aptanda); 0 = applicatio
                                             * magnitudinem eligit (macOS) */
                   i32 densitas;           /* Android dpi (Fire 7: 160);
                                             * 0 = ignota (macOS: puncta
                                             * iam scalam systematis ferunt) */
      TestaImagoCapere  imago_glutinata;    /* Android B1: semper vacua */
                vacuum* imago_ctx;
     TestaTextumPonere  textum_exportare;   /* registrum scribae (yy, y,
                                             * dd); Android: FALSUM */
                vacuum* textus_ctx;
    /* iOS solum (C1b); macOS et Android NIHIL, NUMQUAM truncus -
     * pictor quadratum imaginis inde ostendit */
  TestaImaginemEligere  imaginem_eligere;
                vacuum* imaginem_eligere_ctx;
    /* communio-plan-5 T5 (C1): rete et ansa ab hospite. reactor testae
     * est et ab ea agitur (macOS: in ansa fenestrae, omni excitatione;
     * iOS: horologia sola); applicatio eum in forma.reactor reddit ut
     * agatur. veredarius: macOS domesticus, iOS URLSession, Android
     * NIHIL (nullum rete adhuc). machina: "Mac" | "iPhone" | "Android"
     * - titulus apparatus in inscriptione. */
               Reactor* reactor;
            Veredarius* veredarius;
    constans character* machina;
    /* plan-5 T7/T9: claviatura systematis (iOS); NIHIL = physica */
TestaClaviaturamPonere  claviaturam_ponere;
                vacuum* claviatura_ctx;
} TestaAmbitus;

/* quod applicatio testae reddit (testa eam ZEPHYRUM implet ante) */
nomen structura {
    constans character* titulus;
                   i32  latitudo;           /* pixela NOSTRA */
                   i32  altitudo;
                   i32  scala;              /* puncta per pixelum */
                   b32  plena_visio;
           Dispensator* d;
       FiguraRegistrum* figurae;
                   i32  thema;
             ImagoFons  fons;
                vacuum* fons_ctx;
         LudusPulsator  pulsator;           /* NIHIL = nullus */
                vacuum* pulsator_ctx;
    /* vicus quies T6: mora_proxima non NIHIL = quies
     * (ludus_fenestra_quietem_ponere); macOS solum - iOS/Android
     * ignorant */
      LudusMoraProxima  mora_proxima;
                vacuum* mora_ctx;
               Reactor* reactor;
                vacuum  (*finis)(vacuum* ctx);   /* NIHIL licet */
                vacuum* finis_ctx;
} TestaForma;

nomen b32 (*TestaInitium)(constans TestaAmbitus* a, TestaForma* forma);

/* optiones TESTAE ex argv (applicatio argv totum quoque videt) */
nomen structura {
    constans character* via_imaginis;       /* -imago <via>; NIHIL */
                   b32  fumus;              /* -fumus */
} TestaOptiones;

vacuum
testa_optiones_legere (
              s32   argc,
        character** argv,
    TestaOptiones*  o);

/* basis programmatis ("bin/pictor" -> "pictor"): titulus piscinae, ut olim
 * per applicationem; "applicatio" si NIHIL aut vacua */
constans character*
testa_titulus_processus (
    constans character* programma);

/* forma valida? FALSUM + causa (litterae fixae) nominata */
b32
testa_formam_probare (
     constans TestaForma*  f,
      constans character** causa);

nomen structura {
       TestaForma  forma;
    LudusFenestra* lf;
              b32  initiatus;    /* initium VERUM reddidit */
              b32  finitus;      /* finis vocatus */
} TestaCursus;

/* initium vocat (forma ZEPHYRUM ante), formam probat. FALSUM: causa
 * nominata; si initium VERUM erat, dimittere finem tamen vocat */
b32
testa_cursum_incipere (
              TestaCursus*  c,
    constans TestaAmbitus*  a,
             TestaInitium   initium,
       constans character** causa);

/* ludum ex forma struere (semel; iterum FALSUM) */
b32
testa_cursum_ludum_struere (
        TestaCursus* c,
            Piscina* piscina,
    TabulaPixelorum* tabula);

/* finis ansae: dispensator_finire + linea mensurarum "ludus: ..." */
vacuum
testa_cursum_finire (
    TestaCursus* c);

/* forma.finis semel, solum post initium verum */
vacuum
testa_cursum_dimittere (
    TestaCursus* c);

#endif /* TESTA_CURSUS_H */
