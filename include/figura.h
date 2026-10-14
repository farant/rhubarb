/* figura.h - Figurae: registrum (partes, thema) -> deponere, et
 * PINGERE, arbor logica -> mandata
 *
 * Duae arbores numquam confusae (pictor-spec §2.1): componens dicit
 * QUID (partes, fines, titulus), figura dicit QUOMODO pingatur.
 * Registrum per (partes, thema) resolvit - nulla functio in
 * componente, nullus typus per partes (brainstorm XVI §5: partes
 * sunt DATA). Figura in spatio PROPRIO componentis emittit (origo =
 * angulus finium); pingere coetum aperit (fines, sectio, translatio,
 * scala, provenientia = id) et liberos post figuram ambulat.
 *
 * <purus/>: pingere et omnis figura nihil scribunt praeter mandata.
 * Lint L2. Probantur semel per thema (gradus VI), numquam per
 * widget.
 */

#ifndef FIGURA_H
#define FIGURA_H

/* <aedilis corpus="lib/figura.c"/> */

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "componens.h"
#include "mandatum.h"
#include "thema.h"
#include "exemplaria.h"


/* ==================================================
 * Typi
 * ================================================== */

nomen vacuum (*FiguraFn)(
    constans Componens* c,
              Mandata* m,
                  i32  thema,
              vacuum* ctx);

nomen structura {
      Partes  partes;
         i32  thema;
    FiguraFn  fn;
      vacuum* ctx;
      chorda  spatium;    /* "" = hospes (vicus-latera S2a) */
} FiguraIntroitus;

nomen structura {
        Xar* introitus;     /* Xar de FiguraIntroitus */
    Piscina* piscina;
} FiguraRegistrum;


/* ==================================================
 * Registrum
 * ================================================== */

FiguraRegistrum*
figura_registrum_creare (
    Piscina* piscina);

/* FALSUM si fn NIHIL aut (partes, thema) iam registratum */
b32
figura_registrare (
    FiguraRegistrum* reg,
             Partes  partes,
                i32  thema,
           FiguraFn  fn,
             vacuum* ctx);

/* vicus (T2a): registrum vacuum reddere (memoria servatur) */
vacuum
figura_registrum_vacare (
    FiguraRegistrum* reg);

/* introitus fontis in reg addere; collisio ulla (partes + thema) =
 * FALSUM et nihil additur */
b32
figura_registrum_miscere (
             FiguraRegistrum* reg,
    constans FiguraRegistrum* fons);

b32
figura_invenire (
    constans FiguraRegistrum*  reg,
                      Partes   partes,
                         i32   thema,
                    FiguraFn*  fn_ex,
                      vacuum** ctx_ex);

/* Spatia (vicus-latera S2a; status-plan S8c): introitus fontis SINE
 * spatio in spatium datum (internatum - vivit cum registro);
 * introitus qui iam spatium habent (organa nidificata, a parente
 * mixti) suum servant - via absoluta est. Collisio solum intra idem
 * spatium. figura_registrare et figura_invenire = spatium "";
 * figura_registrum_miscere spatium cuiusque introitus servat. */
b32
figura_registrum_miscere_in_spatio (
             FiguraRegistrum* reg,
    constans FiguraRegistrum* fons,
                      chorda  spatium);

/* strictum: introitus solum eiusdem spatii */
b32
figura_invenire_in_spatio (
    constans FiguraRegistrum*  reg,
                      chorda   spatium,
                      Partes   partes,
                         i32   thema,
                    FiguraFn*  fn_ex,
                      vacuum** ctx_ex);


/* ==================================================
 * Pingere
 * ================================================== */

/* <purus/> arbor logica -> mandata. Coetus per componens; figura
 * (si registrata pro (spatium, partes, thema) - spatium efficax
 * componentis, S2a) ante liberos. */
vacuum
pingere (
          constans Componens* radix,
    constans FiguraRegistrum* reg,
                         i32  thema,
                     Mandata* m);

/* Figura minima: fines vacui colore COLOR_BORDER. Pro probationibus
 * et pro partibus quibus nemo figuram dedit. */
vacuum
figura_finium (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx);

/* color thematis ut ColorMandati (genus COLOR_MANDATI_THEMA: thema in
 * pingendo resolvitur) - scriba, pictor, calendarium, terminale,
 * vicus */
ColorMandati
figura_color_thematis (
    ColorThema color);

/* indicium foci: exemplar mensae lateris focati */
#define FIGURA_EXEMPLAR_FOCI  EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA

/* mensa (scriba, pictor, calendarium): fines toti colore
 * COLOR_SUPERFICIES; focatum = exemplar foci (COLOR_BORDER) supra */
vacuum
figura_mensam_pingere (
    Mandata* md,
      Fines  fines,
        b32  focatum);


/* ==================================================
 * Instrumenta visus (V1): forma communis "plana cum exemplaribus"
 * (Mac classicus): margo 1px COLOR_TEXT, fundus COLOR_BACKGROUND,
 * electum COLOR_SELECTION, umbra plana COLOR_TEXT; textus 6x8
 * centratus et in piscinam mandatorum transcriptus. Omnia in spatio
 * vocantis. Cum FIGURA_UMBRA corpus II pixela minus et umbra INTRA
 * fines datos (nihil extra componentem pingitur).
 * ================================================== */

/* status instrumenti (larva) */
#define FIGURA_ELECTUM   I    /* electum / pressum: plenum */
#define FIGURA_UMBRA     II   /* umbra plana II px infra-dextrorsum */
#define FIGURA_OBSCURUM  IV   /* non praesto: textus obscurus */

/* positio segmenti in controllo segmentato */
#define FIGURA_SEGMENTUM_PRIMUM   ZEPHYRUM
#define FIGURA_SEGMENTUM_MEDIUM   I
#define FIGURA_SEGMENTUM_ULTIMUM  II

/* globulus: margo 1px, fundus, textus centratus */
vacuum
figura_globulum_pingere (
    Mandata* md,
      Fines  f,
     chorda  textus,
        i32  status);

/* globulus cum sagitta (triangulum): directio -1 sinistra, +1 dextra */
vacuum
figura_sagittam_pingere (
    Mandata* md,
      Fines  f,
        s32  directio,
        i32  status);

/* segmentum unum controlli segmentati: quisque margines superum,
 * inferum et sinistrum (divisor) pingit, ULTIMUM etiam dextrum -
 * margines communes sine duplicatione; quisque componens suum */
vacuum
figura_segmentum_pingere (
    Mandata* md,
      Fines  f,
     chorda  textus,
        i32  positio,
        i32  status);

/* capsa IX x IX ad (x, y); signum si factum */
vacuum
figura_capsam_pingere (
    Mandata* md,
        s32  x,
        s32  y,
        b32  factum);

/* ordo electus: fundus selectionis, fascia accentus sinistra (III) */
vacuum
figura_ordinem_electum_pingere (
    Mandata* md,
      Fines  f);

/* fascia: fundus et exemplar leve (titulus gregis, dies hebdomadis) */
vacuum
figura_fasciam_pingere (
    Mandata* md,
      Fines  f);

/* umbra plana sub fines (II infra-dextrorsum; tabellae, menus):
 * vocans tabellam postea pingit */
vacuum
figura_umbram_pingere (
    Mandata* md,
      Fines  f);

/* campus: fundus superficiei et margo (textus a vocante) */
vacuum
figura_campum_pingere (
    Mandata* md,
      Fines  f);

/* textus audax: bis, secundus +1 px */
vacuum
figura_textum_audacem (
         Mandata* md,
             s32  x,
             s32  y,
          chorda  textus,
    ColorMandati  color);

#endif /* FIGURA_H */
