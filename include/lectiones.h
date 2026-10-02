/* lectiones.h - liber lectionum: quod processus legit, quaesivit,
 * exstare confirmavit, enumeravit, scripsit (fabrica slice 2).
 *
 * Variabilis ambitus FABRICA_LECTIONES viam plagulae nominat; absens =
 * nihil fit (nulla allocatio, nullus mos mutatus). Linea una per
 * eventum, '<littera>\t<via>\n', una vocatione write() cum O_APPEND -
 * filii paralleli lineas integras servant. Via ut data scribitur
 * (relativa manet relativa); fabrica postea sigillat.
 *
 * Hoc modulo SOLO linea libri scribitur: filum, via_existit,
 * iter_directoria et ansae directoriorum aedilis eum vocant. Ipse
 * open/write crudos adhibet - canalis est, non cliens canalis.
 */
#ifndef LECTIONES_H
#define LECTIONES_H

#include "latina.h"

nomen enumeratio {
    LECTIO_LEGIT = 0,     /* 'L' plagula lecta */
    LECTIO_ABSENS,        /* 'A' quaesita, non inventa */
    LECTIO_EXSTAT,        /* 'X' exstare confirmata, non lecta */
    LECTIO_ENUMERAVIT,    /* 'D' directorium enumeratum */
    LECTIO_SCRIPSIT,      /* 'S' plagula scripta, deleta, mota */
    LECTIO_AMBITUS        /* 'E' variabilis ambitus lecta */
} LectioGenus;

/* lineam unam appendere; nihil si variabilis absens aut via nimis
 * longa (linea numquam scinditur) */
vacuum
lectiones_notare (
                 LectioGenus  genus,
          constans character* via);

/* getenv per librum: valorem reddit (aut NIHIL) et 'E' notat -
 * linea 'E\t<titulus>\t<valor>' (absens: 'E\t<titulus>' sine
 * tabulatore secundo) */
constans character*
lectiones_ambitus (
         constans character* titulus);

#endif /* LECTIONES_H */
