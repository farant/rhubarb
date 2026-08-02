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
b32 numerus_romanus_legere (chorda s, i32* valor);

#endif /* NUMERUS_ROMANUS_H */
