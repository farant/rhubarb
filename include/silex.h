/* silex.h - silex (lapis ignarius): instrumentum distributionis -
 * proiecta nova e fabrica excudere
 *
 * NOMEN NONDUM SIGILLATUM. Visio (res 01KZ4Y9A8T): binarium unum
 * quod fontes bibliothecarum omnium fert, proiecta scaffoldat,
 * renovationes per sigilla offert, et VCS fit. Hoc = versio 0,
 * verbum primum NOVUM:
 *
 *   - clausura bibliothecarum ex FABRICA legitur (arbor rhubarb;
 *     capsula embedenda = decisio diei distributionis, non nunc)
 *   - clausura = BFS super inclusiones citatas, conventione
 *     aedilis: include/X.h -> lib/X.c sponte
 *   - proiectum natum = VOLUMEN (veritas) + proiectio in systema
 *     plagularum (plagulae ex volumine promuntur, non e memoria -
 *     via promendi ab ortu probata)
 *
 * Scrutatio inclusionum v0 TEXTUALIS est (lineatim); promotio ad
 * silvam cum tractione. Inclusio citata quae in fabrica non
 * exsistit = monitio (non mors) - commentaria falso positiva
 * possunt, dependentiae verae in fabrica semper exsistunt.
 */

#ifndef SILEX_H
#define SILEX_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

/* plagula colligenda: via relativa proiecti + contentum + origo */
nomen structura {
    chorda              via;
    chorda              contentum;
    constans character* origo;   /* "vendicata:<via>" | "genita" */
} SilexRes;

nomen structura {
    constans character* fabrica;      /* radix arboris rhubarb */
    constans character* destinatio;   /* directorium parens ("."...) */
    constans character* titulus;      /* nomen proiecti */
} SilexNovumOptiones;

nomen structura {
    b32                 successus;
    i32                 vendicatae;   /* plagulae e fabrica */
    i32                 genitae;      /* plagulae genitae */
    constans character* volumen_via;
    constans character* erratum;      /* si !successus: causa */
} SilexNovumFructus;

/* clausura bibliothecarum: BFS ex seminibus (nomina capitum,
 * e.g. "chorda.h"); Xar de SilexRes (include/... et lib/...);
 * NIHIL si fabrica invalida */
Xar*
silex_clausuram_colligere (
    Piscina*                      piscina,
    constans character*           fabrica,
    constans character* constans* semina,
    i32                           numerus_seminum);

/* proiectum novum excudere; vide silex.h caput pro forma */
SilexNovumFructus
silex_novum (
    Piscina*                  piscina,
    constans SilexNovumOptiones* optiones);

#endif /* SILEX_H */
