#ifndef PAGINATIO_H
#define PAGINATIO_H

#include "latina.h"
#include "chorda.h"


/* ====================================================================
 * PAGINATIO - designatio paginae libri
 *
 * (Nomen 'pagina' iam ab editore textus 2D occupatum est; hoc de
 * SCHEMATE numerandi agit, non de pagina ipsa.)
 *
 * Liber verus DUAS series paginarum fert, non unam:
 *
 *     i, ii, iii, ... xlii      prooemium (praefatio, index, tabulae)
 *     1, 2, 3, ... ccclxxx      corpus
 *
 * et 'xii' et '12' paginae DIVERSAE sunt in eodem libro. Ergo numerus
 * integer solus designationem paginae exprimere NON potest, et nota
 * lectionis quae integrum poscit prooemium omnino perdit.
 *
 * TYPUS, non conventio clientis: machina generum hoc iudicat
 * (gesta.c _campum_iudicare), ergo designatio prava conditorium non
 * intrat - neque per applicationem neque per MCP.
 * ==================================================================== */

nomen enumeratio
{
    PAGINATIO_NULLA,      /* vacua aut invalida */
    PAGINATIO_ROMANA,     /* prooemium: i, xlii, XIV */
    PAGINATIO_ARABICA     /* corpus: 1, 380 */
} GenusPaginationis;

nomen structura
{
    GenusPaginationis genus;
                  i32 valor;
} Paginatio;

/* --------------------------------------------------------------------
 * LECTIO
 *
 * Accipiuntur: numeri Romani (forma stricta, maiusculis AUT
 * minusculis) et integri positivi. ZEPHYRUM RESPUITUR - pagina nulla
 * in libro est, ergo '0' mendum est, non designatio.
 *
 * Spatia lateralia praeciduntur; nihil aliud toleratur. '12a', 'A-3',
 * 'p. 12' RESPUUNTUR consulto: laxitas paginam in ordine falso
 * silenter poneret, et ordo falsus notam ad locum falsum ligat -
 * eadem lex qua inscriptio capituli regitur.
 * -------------------------------------------------------------------- */

Paginatio
paginatio_legere (
    chorda s);

/* --------------------------------------------------------------------
 * ORDO - clavis unius numeri
 *
 * Paginae Romanae ANTE Arabicas stant (prooemium ante corpus),
 * quaeque series in se numerice. Clavis UNA utrumque ordinem portat,
 * ergo ordinator quivis - conditorium, velamen, quodlibet futurum -
 * eodem numero comparat et divergere NEQUIT.
 *
 * Pagina nulla ULTIMA stat: nota sine pagina ordinem notarum
 * paginatarum non interrumpat.
 * -------------------------------------------------------------------- */

#define PAGINATIO_BANDA 1000000

i64
paginatio_clavis (
    Paginatio p);

/* Commoditas: chorda -> clavis uno gradu */
i64
paginatio_clavis_chordae (
    chorda s);

#endif /* PAGINATIO_H */
