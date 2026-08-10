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
#include "capsula.h"

/* ==================================================
 * Fons bibliothecarum: DISCUS (fabrica in disco) aut CORPUS
 * (capsula in binario infixa). Lector unicus clausurae - omnes
 * verbi (novum/renovare/partes) per eundem fontem legunt.
 * ================================================== */

nomen enumeratio {
    SILEX_FONS_DISCUS = 0,   /* fabrica in disco */
    SILEX_FONS_CORPUS        /* capsula in binario infixa */
} SilexFonsGenus;

nomen structura {
    SilexFonsGenus      genus;
    constans character* fabrica;   /* DISCUS: radix arboris */
    Capsula*            capsula;   /* CORPUS: corpus apertum */
    constans character* titulus;   /* pro nuntiis: via aut stampa */
} SilexFons;

/* DISCUS: include/ adsit; NIHIL si invalida */
SilexFons*
silex_fons_disci (
    Piscina*            piscina,
    constans character* fabrica);

/* CORPUS: capsulam aperit; titulus e clave 'corpus.versio'
 * (absente: "(corpus sine stampa)"); NIHIL si capsula fracta */
SilexFons*
silex_fons_corporis (
    Piscina*               piscina,
    constans CapsulaEmbed* embed);

b32
silex_fons_existit (
    constans SilexFons* fons,
    constans character* via_relativa,
    Piscina*            piscina);

/* *inventum FALSUM si via ignota */
chorda
silex_fons_legere (
    constans SilexFons* fons,
    constans character* via_relativa,
    Piscina*            piscina,
    b32*                inventum);

/* plagula colligenda: via relativa proiecti + contentum + origo */
nomen structura {
    chorda              via;
    chorda              contentum;
    constans character* origo;   /* "vendicata:<via>" | "genita" */
} SilexRes;

nomen structura {
    constans SilexFons* fons;         /* unde bibliothecae */
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
 * e.g. "chorda.h"); Xar de SilexRes (include/... et lib/... et
 * vendor/...); NIHIL si fons NIHIL */
Xar*
silex_clausuram_colligere (
    Piscina*                      piscina,
    constans SilexFons*           fons,
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

/* via voluminis proiecti: conventio <dir>/<nomen>.volumen via
 * laeta; ea absente *.volumen solitarium vincit; plura = recusatio
 * clara + NIHIL. (Identitas documenti - decisum red-team IX.) */
constans character*
silex_volumen_viam_invenire (
    Piscina*            piscina,
    constans character* proiectum_dir);

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

/* ==================================================
 * Renovatio: bibliothecae vendicatae e fabrica renovantur
 *
 * Comparatio sigillorum TRIUM per plagulam vendicatam:
 *   missum-tunc   (actum vendicata ultimum in actis - remota
 *                  scopum demit: quod consulto amovisti non
 *                  resuscitatur)
 *   proiectum-nunc (discus hodie)
 *   missum-novum  (fabrica hodie, per clausuram recomputatam)
 * NUMQUAM timestamp - solum contentum "editum a te" ab "novius"
 * distinguit. Plagulae genitae (origo non-vendicata) numquam
 * spectantur - tuae sunt.
 *
 * Consilium ordinarium; scribere = applicatio (disco + volumini
 * origine vendicata, transactione una + actum conditionis -
 * renovatio punctum nominatum caudae est, proicere -ad revertit).
 * Vulnera/conflictus nominantur, numquam tanguntur, scriptionem
 * ceterorum non obstant (retentio honesta, non recusatio).
 * ================================================== */

nomen enumeratio {
    SILEX_RENOVATIO_RENOVANDA = 0,  /* tunc==nunc, novum aliud
                                     * (aut disco absens) - tuta */
    SILEX_RENOVATIO_ADDENDA,        /* clausurae novae, proiecto
                                     * ignota - dependentia nova */
    SILEX_RENOVATIO_VULNUS,         /* manu edita, fabrica immota */
    SILEX_RENOVATIO_CONFLICTUS,     /* ambae motae (aut collisio
                                     * cum plagula tua) */
    SILEX_RENOVATIO_DERELICTA       /* vendicata, fabricae hodie
                                     * ignota - upstream demisit */
} SilexRenovatioStatus;

nomen structura {
    chorda               via;
    SilexRenovatioStatus status;
} SilexRenovatioRes;

nomen structura {
    b32                 successus;
    Xar*                res;        /* intactae tacent (numerus) */
    i32                 intactae;
    i32                 renovatae;  /* si scribere: scriptae */
    i32                 additae;
    constans character* erratum;
} SilexRenovatioFructus;

SilexRenovatioFructus
silex_renovare (
    Piscina*            piscina,
    constans character* proiectum_dir,
    constans SilexFons* fons,
    b32                 scribere);

#endif /* SILEX_H */
