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

b32
figura_invenire (
    constans FiguraRegistrum*  reg,
                      Partes   partes,
                         i32   thema,
                    FiguraFn*  fn_ex,
                      vacuum** ctx_ex);


/* ==================================================
 * Pingere
 * ================================================== */

/* <purus/> arbor logica -> mandata. Coetus per componens; figura
 * (si registrata pro (partes, thema)) ante liberos. */
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
