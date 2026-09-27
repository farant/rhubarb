#ifndef NUMERUS_ROMANUS_H
#define NUMERUS_ROMANUS_H

#include "latina.h"
#include "chorda.h"


/* ====================================================================
 * NUMERI ROMANI - lectio formae subtractivae STRICTAE
 *
 * Natus in capitula.c (indices capitulorum: "XIV - De materia"),
 * hinc promotus cum secundus consumptor advenit (designationes
 * paginarum: prooemium i-xlii, deinde 1-380). Bibliotheca propria
 * quia numerus Romanus neque capitulum neque pagina est.
 *
 * STRICTITUDO NON EST FASTIDIUM. Regula laxa - 'omnes litterae in
 * IVXLCDM' - haec verba Anglica ut numeros accipit:
 *
 *     DID  MILD  CIVIC  VIM  DIM  LIVID  MIMIC
 *
 * et quisque eorum in indice aut in nota paginae stare potest. Forma
 * subtractiva ea omnia respuit quia coniunctiones eorum illicitae
 * sunt (I ante D, I ante L, ...).
 *
 * MIX superest, et superesse DEBET: numerus verus est (MIX). Nulla
 * regula id sine numeris veris damnandis excludit.
 * ==================================================================== */

/* Minuscula ACCIPIUNTUR hic (paginae prooemii 'xii' scribuntur), sed
 * MIXTA non ('Xii'): casus mixtus verbum est, non numerus.
 *
 * Redde VERUM si s numerus Romanus validus est; valor (si non NIHIL)
 * summam accipit. Chorda vacua FALSUM. */
b32
numerus_romanus_legere (
    chorda  s,
       i32* valor);

/* Forma canonica subtractiva, maiuscula: 106 -> "CVI", 3999 ->
 * "MMMCMXCIX". Numerus Romanus classicus ad MMMCMXCIX finit (ultra
 * Romani vinculo - linea super numerum = mille - utebantur); ergo
 * 0 (numerum Romanum non habet) et n > 3999 -> chorda vacua. Vide
 * numerus_romanus_exprimere pro omni n. Par: legere(scribere(n)) == n. */
chorda
numerus_romanus_scribere (
         i32  n,
     Piscina* piscina);

/* Expressio C89 canonica valoris n in vocabulario latina.h - quam
 * formator pro digitis scribit (desideratum ...QCHQ):
 *
 *   n == 0              -> "ZEPHYRUM"
 *   n <= 3999           -> numerus ipse: "CVI"
 *   multiplum MXXIV     -> "IV * MXXIV" (4096), "MXXIV * MXXIV" -
 *     (non milium)         intentio binaria servatur
 *   ceteri              -> vinculum ut '* M', gregibus milium:
 *                          "IV * M" (4000), "V * M + CCLXXX" (5280),
 *                          "IV * M * M + D * M" (4500000)
 *
 * Milia rotunda milia manent: 128000 -> "CXXVIII * M", non
 * "CXXV * MXXIV". Terminus cuius valor int excedit '(i64)' ante
 * factorem primum fert (productum int involveretur - UB).
 * *compositum (si non NIHIL) = VERUM si expressio operatorem habet:
 * vocator parentheses ponit ubi praecedentia poscit (x / 4096 ->
 * x / (IV * MXXIV)). */
chorda
numerus_romanus_exprimere (
         i64  n,
         b32* compositum,
     Piscina* piscina);

#endif /* NUMERUS_ROMANUS_H */
