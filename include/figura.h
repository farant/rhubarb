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

/* Spatia (vicus-latera S2a): introitus fontis in spatium datum
 * (internatum - vivit cum registro); collisio solum intra idem
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

#endif /* FIGURA_H */
