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

/* fabricam sponte invenire: ascensus ex initio (directorium),
 * signum quaerens (include/latina.h + lib/). Reddit radicem
 * absolutam aut NIHIL si nusquam. Superscriptiones (-f /
 * SILEX_FABRICA) vocatori manent - hoc gradus TERTIUS est. */
constans character*
silex_fabricam_invenire (
    Piscina*            piscina,
    constans character* initium);

/* ==================================================
 * VCS: status / condere / historia
 *
 * CONDITIO NON OBIECTUM SED PUNCTUM NOMINATUM IN CAUDA ACTORUM -
 * arbores = plicae; manifestum (plagulae) = plica currens. Volumen
 * residentia prima, arbor laborans proiectio. Praetermissa: bin/,
 * build/, *.volumen(-wal/-shm), occulta (.DS_Store et soror).
 * ================================================== */

nomen enumeratio {
    SILEX_PLAGULA_MUTATA = 0,   /* in disco != manifestum */
    SILEX_PLAGULA_NOVA,         /* in disco, manifesto ignota */
    SILEX_PLAGULA_ABSENS        /* in manifesto, disco ablata */
} SilexPlagulaStatus;

nomen structura {
    chorda             via;
    SilexPlagulaStatus status;
} SilexStatusRes;

nomen structura {
    b32                 successus;
    i32                 mundae;   /* congruentes (tacent) */
    Xar*                res;      /* SilexStatusRes - non-mundae */
    constans character* erratum;
} SilexStatusFructus;

/* lector purus - numquam scribit */
SilexStatusFructus
silex_status (
    Piscina*            piscina,
    constans character* proiectum_dir);

nomen structura {
    b32                 successus;
    s64                 seq;        /* actus conditionis */
    i32                 conditae;   /* mutatae + novae absorptae */
    i32                 remotae;
    constans character* erratum;    /* "nihil condendum" si vacuum */
} SilexConditioFructus;

/* absorptio: diff statûs -> eventa + actum 'conditio' {nuntius},
 * transactione UNA (omnia aut nihil) */
SilexConditioFructus
silex_condere (
    Piscina*            piscina,
    constans character* proiectum_dir,
    constans character* nuntius);

nomen structura {
    s64    seq;
    chorda momentum;
    chorda nuntius;    /* "(ortus voluminis)" pro ortu */
    i32    tactae;     /* plagulae ab introitu priore */
} SilexConditio;

/* conditiones (+ ortus) ordine temporis; NIHIL = erratum */
Xar*
silex_historia (
    Piscina*            piscina,
    constans character* proiectum_dir);

/* ==================================================
 * Proiectio: volumen arborem scribit (condere inversum)
 *
 * Consilium ordinarium (numquam scribit); scribere = applicatio.
 * ad_seq <= 0 = plica praesens (proicere = restitutio); ad_seq N =
 * arbor ut erat ad punctum N caudae (iter temporis SINE capite -
 * cauda tota manet, arbor vetus fit, conditio sequens praeteritum
 * praesens facit ut punctum NOVUM).
 *
 * FOEDUS: nihil inconditum umquam deletur. Contentum disci quod in
 * massis nusquam residet = OBEX - scriptio recusatur ("conde
 * prima"). Alienae (disco, plicae ignotae) numquam tanguntur nec
 * delentur - sed nominantur (consilium honestum).
 * ================================================== */

nomen enumeratio {
    SILEX_PROICIENDA_SCRIBENDA = 0,  /* disco, contentum aliud
                                      * (conditum alicubi - tutum) */
    SILEX_PROICIENDA_CREANDA,        /* in plica, disco absens */
    SILEX_PROICIENDA_OBEX,           /* contentum disci INCONDITUM */
    SILEX_PROICIENDA_ALIENA          /* disco, plicae ignota */
} SilexProiciendaStatus;

nomen structura {
    chorda                via;
    SilexProiciendaStatus status;
} SilexProiciendaRes;

nomen structura {
    b32                 successus;
    Xar*                res;        /* SilexProiciendaRes;
                                     * intactae tacent (numerus) */
    i32                 intactae;
    i32                 obices;
    i32                 scriptae;   /* si scribere */
    constans character* erratum;
} SilexProiectioFructus;

SilexProiectioFructus
silex_proicere (
    Piscina*            piscina,
    constans character* proiectum_dir,
    s64                 ad_seq,
    b32                 scribere);

#endif /* SILEX_H */
