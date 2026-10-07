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
#include <stdio.h>

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

/* FOPEN PER LIBRUM (fabrica plan 5 T5a) - fulcrum TRANSITORIUM pro
 * vocationibus stdio crudis quae manent (codex novus filum adhibet):
 * modus 'r'/'rb' (sine '+'): L si apertum, A si non; ceteri (w, a,
 * r+): S. Viae sub /dev/ (machinae: entropia, terminalia) NON
 * notantur - ingressus non sunt, et fabrica machinam sigillare nequit
 * (IGNOTUM: verdictum numquam reutile). */
FILE*
lectiones_fopen (
    constans character* via,
    constans character* modus);

/* STAT PER LIBRUM: stat(2) idem; X si exstat, A si non (sub /dev/
 * nihil). Status per 'structura stat' (sys/stat.h vocantis). */
structura stat;
integer
lectiones_stat (
    constans character* via,
       structura stat* status);

#endif /* LECTIONES_H */
