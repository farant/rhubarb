#ifndef TESTA_INITUS_H
#define TESTA_INITUS_H

/* testa_initus.h - partes PURAE testarum Android et iOS (testa-spec
 * II.5-II.6; testa-ios-spec II.2): geometria (scala, margo), tactus ->
 * Eventus, phases UITouch -> actiones, decisio quadri, transfusio
 * pixelorum. Valores plani intrant et exeunt - nullum caput NDK aut
 * UIKit: in hospite et in tabula probantur. Constantae NDK et UIKit
 * hic redditae (android/input.h NDK r27d; UITouchPhase);
 * lib/testa_android.c et lib/testa_ios.m congruentiam in compilatione
 * probant. Olim testa_android_initus (testa-ios T1). */

#include "latina.h"
#include "eventus.h"
#include "fasti.h"
#include "tabula_pixelorum.h"

/* AMOTION_EVENT_ACTION_* (actio & AMOTION_EVENT_ACTION_MASK) */
#define TESTA_TACTUS_DEPRESSIO              ZEPHYRUM  /* _DOWN */
#define TESTA_TACTUS_LIBERATIO              I         /* _UP */
#define TESTA_TACTUS_MOTUS                  II        /* _MOVE */
#define TESTA_TACTUS_ABRUPTIO               III       /* _CANCEL */
#define TESTA_TACTUS_INDICATOR_DEPRESSIO    V        /* _POINTER_DOWN */
#define TESTA_TACTUS_INDICATOR_LIBERATIO    VI        /* _POINTER_UP */

/* AMOTION_EVENT_TOOL_TYPE_* */
#define TESTA_INSTRUMENTUM_DIGITUS  I     /* _FINGER */
#define TESTA_INSTRUMENTUM_STILUS   II    /* _STYLUS */
#define TESTA_INSTRUMENTUM_MUS      III   /* _MOUSE */
#define TESTA_INSTRUMENTUM_GUMMI    IV    /* _ERASER (stili cauda) */

/* UITouchPhase (UIKit, iOS 17+: Began 0, Moved 1, Stationary 2,
 * Ended 3, Cancelled 4) - lib/testa_ios.m congruentiam probat */
#define TESTA_PHASIS_INCIPIT        ZEPHYRUM
#define TESTA_PHASIS_MOVETUR        I
#define TESTA_PHASIS_QUIESCIT       II
#define TESTA_PHASIS_FINIT          III
#define TESTA_PHASIS_ABRUPTA        IV

#define TESTA_INDICATORES_MAXIMI    X     /* plures praetermittuntur */


/* ==================================================
 * Geometria: fenestra (pixela tabulae Android) -> pixela nostra
 * ================================================== */

/* latitudo = fenestra / scala (pavimentum) - IDEM ac
 * tabula_pixelorum_ad_fenestram; residuum (< scala) margo est,
 * centratus (pars minor sinistra/summa) */
nomen structura {
    i32 fenestra_latitudo;
    i32 fenestra_altitudo;
    i32 scala;
    i32 latitudo;      /* pixela nostra */
    i32 altitudo;
    i32 margo_x;       /* pixela fenestrae */
    i32 margo_y;
} TestaGeometria;

/* FALSUM si scala < I aut fenestra minor scala */
b32
testa_geometriam_computare (
               i32  fenestra_latitudo,
               i32  fenestra_altitudo,
               i32  scala,
    TestaGeometria* g);

/* idem intra rectangulum CONTENTI [x0,x1) x [y0,y1) fenestrae (T3:
 * vectes systematis - status, navalis - tactus suos devorant; area
 * eorum margo fit). Rectangulum ad fenestram secatur; FALSUM si
 * vacuum aut minor scala. fenestra_* = fenestra TOTA (transfusio
 * margines extra rectangulum implet). */
b32
testa_geometriam_in_area (
               i32  fenestra_latitudo,
               i32  fenestra_altitudo,
               s32  x0,
               s32  y0,
               s32  x1,
               s32  y1,
               i32  scala,
    TestaGeometria* g);

/* punctum fenestrae -> pixela nostra, PAVIMENTO (negativa recte):
 * in margine extra [0, latitudo) cadit, ut mus extra fenestram (A3c) */
vacuum
testa_punctum_mappare (
    constans TestaGeometria* g,
                        f32  x,
                        f32  y,
                        s32* nx,
                        s32* ny);


/* ==================================================
 * Tactus -> Eventus (digitus primus solus ducit, B1)
 * ================================================== */

/* AMotionEvent in valores planos (involucrum in testa_android.c) */
nomen structura {
    s32 actio;                         /* TESTA_TACTUS_* */
    i32 index_actionis;                /* indicator quem actio tangit */
    i32 numerus;                       /* indicatores (<= MAXIMI) */
    s32 ids[TESTA_INDICATORES_MAXIMI];
    s32 instrumenta[TESTA_INDICATORES_MAXIMI];
    f32 x[TESTA_INDICATORES_MAXIMI];
    f32 y[TESTA_INDICATORES_MAXIMI];
    s64 tempus;                        /* ms (CLOCK_MONOTONIC) */
    i32 numerus_historiae;             /* <= EVENTUS_EXEMPLA_MAXIMA */
    f32 historia_x[TESTA_INDICATORES_MAXIMI][EVENTUS_EXEMPLA_MAXIMA];
    f32 historia_y[TESTA_INDICATORES_MAXIMI][EVENTUS_EXEMPLA_MAXIMA];
    s64 historia_tempus[EVENTUS_EXEMPLA_MAXIMA];
} TestaTactusCrudus;

/* status inter eventus; exempla = repositorium VISUS eventus ultimi
 * (valet donec vertere iterum vocatur) */
nomen structura {
                b32 activus;     /* ducens depressus */
                b32 exspectat;   /* ducens liberatus, alii adhuc */
                s32 indicator;   /* id ducentis */
    EventusExemplum exempla[EVENTUS_EXEMPLA_MAXIMA];
} TestaTactus;

/* VERUM + ev impletus, aut FALSUM (nihil tradendum) */
b32
testa_tactum_vertere (
                   TestaTactus* s,
    constans TestaTactusCrudus* c,
       constans TestaGeometria* g,
                       Eventus* ev);

/* phasis tactus unius (UITouch) -> actio machinae tactus
 * (TESTA_TACTUS_*): activi_ante / activi_post = tactus activi ante et
 * post mutationem huius - primus DEPRESSIO, sequentes
 * INDICATOR_DEPRESSIO, ultimus sublatus LIBERATIO, alius sublatus
 * INDICATOR_LIBERATIO, abrupta semper ABRUPTIO. -I = nihil tradendum
 * (quiescit, phasis ignota). */
s32
testa_actio_ex_phase (
    i32 phasis,
    i32 activi_ante,
    i32 activi_post);

/* mensurae imaginis intra 'maxima' (testa-ios C1b R4: photographia
 * electa, latus longius <= MXXIV): utraque <= maxima -> immutatae;
 * aliter latus longius = maxima, alterum proportione, rotundatum
 * (dimidium sursum), minimum I. FALSUM si latitudo, altitudo aut
 * maxima ZEPHYRUM. */
b32
testa_mensuras_aptare (
    i32  latitudo,
    i32  altitudo,
    i32  maxima,
    i32* lat,
    i32* alt);

/* EVENTUS_FACULTATES tactus: super (hover) FALSUM, pressio FALSUM
 * (pressio digiti Fire 0.10 synthetica, spica) */
vacuum
testa_facultates_tactus (
    Eventus* ev,
        s64  tempus);


/* ==================================================
 * Decisio quadri (R4: ansam macOS imitatur)
 * ================================================== */

nomen enumeratio {
    TESTA_QUADRUM_NULLUM = ZEPHYRUM,
    TESTA_QUADRUM_STATIM,
    TESTA_QUADRUM_POST
} TestaQuadriGenus;

nomen structura {
    TestaQuadriGenus genus;
                Mora post_ms;    /* POST solum */
} TestaQuadriPetitio;

nomen structura {
     b32 fenestra;           /* fenestra adest */
     b32 vivus;              /* resumptus, non in pausa */
     b32 pendens_statim;     /* vocatio statim iam petita */
     b32 pendens_post;       /* vocatio dilata iam petita */
     b32 debitum;            /* eventa post quadrum ultimum */
    Mora mora;               /* ludus_fenestra_mora */
     b32 pulsus;             /* forma pulsum habet */
     i32 fumus_reliqua;      /* quadra fumi restantia; 0 = nullus */
} TestaQuadriStatus;

TestaQuadriPetitio
testa_quadrum_petere (
    constans TestaQuadriStatus* s);


/* ==================================================
 * Transfusio: tabula (pixela nostra) -> alveus fenestrae
 * ================================================== */

/* proximus vicinus, scala integra, margo colore; gradus = pixela per
 * lineam alvei (>= fenestra_latitudo). RGBA_8888: octeti R,G,B,A in
 * memoria - forma tabulae ipsa */
vacuum
testa_pixela_transferre (
    constans TabulaPixelorum* t,
     constans TestaGeometria* g,
                         i32* destinatio,
                         i32  gradus,
                         i32  color_marginis);

#endif /* TESTA_INITUS_H */
