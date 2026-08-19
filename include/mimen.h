/* mimen.h - Tabula MIME communis (extensio -> typus)
 *
 * Extracta ex vitrea (tabula XIV ordinum verbatim). Consumptores:
 * vitrea (capsula://), hospitium (servitio statica).
 *
 * USUS:
 *   constans character* m = mimen_pro_via("stilus.css");  (text/css)
 *   constans character* n = mimen_pro_via_chorda(via);    (non NUL-terminata)
 */

#ifndef MIMEN_H
#define MIMEN_H

#include "latina.h"
#include "chorda.h"

/* Typus MIME pro via - extensio = post ultimum '.' post ultimum '/'
 *
 * Redde: typus MIME vel "application/octet-stream" si ignotum
 */
constans character*
mimen_pro_via (
    constans character* via);

/* Idem pro chorda non NUL-terminata (longitudine limitata) */
constans character*
mimen_pro_via_chorda (
    chorda via);

#endif /* MIMEN_H */
