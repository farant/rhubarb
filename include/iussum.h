/* iussum.h - iussa in textu, stilo acme (vicus-latera S3a)
 *
 * '$verbum' aut '$verbum(arg, arg)' in tabula characterum: verbum
 * '[a-z][a-z0-9_]*', '$' in initio lineae aut post characterem non
 * verbalem (littera, numerus, '_'), ergo 'a$b' iussum non est.
 * Argumenta: '(' statim post verbum, ')' prima in EADEM linea;
 * commatibus divisa, spatia extrema dempta; '()' = nulla. '$verbum('
 * sine ')' iussum NON est (dimidium iussi numquam currit). Solum
 * verba NOTA iussa sunt (hospes respondet): cetera prosa manent
 * ('$5.00', '$foo(' - nullum effugium necessarium).
 *
 * <purus/>: tabulam legit, nihil mutat; chordae exitus in piscina
 * (copiae - tabula postea mutari potest). */

#ifndef IUSSUM_H
#define IUSSUM_H

/* <aedilis corpus="lib/iussum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabula_characterum.h"

/* verbum notum? NIHIL = omnia nota (probationes) */
nomen b32 (*IussumNotum)(chorda verbum, vacuum* ctx);

nomen structura {
       s32  linea;
       s32  initium;            /* columna '$' */
       s32  finis;              /* post ultimum (')' aut verbum) */
    chorda  verbum;             /* sine '$' */
       i32  numerus_argumentorum;
    chorda* argumenta;          /* in piscina; spatia extrema dempta */
} Iussum;

/* iussum quod cellulam (linea, columna) tegit (initium <= columna <
 * finis); FALSUM si nullum aut argumenta invalida */
b32
iussum_ad_locum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  columna,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus);

/* iussum proximum in linea cuius '$' a columna data aut post iacet
 * (figura: omnia colorare - iterum a exitus->finis); FALSUM si
 * nullum */
b32
iussum_proximum (
    constans TabulaCharacterum* t,
                           s32  linea,
                           s32  a_columna,
                   IussumNotum  notum,
                        vacuum* ctx,
                       Piscina* piscina,
                        Iussum* exitus);

#endif /* IUSSUM_H */
