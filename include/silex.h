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
#include "differentia.h"


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
        SilexFonsGenus  genus;
    constans character* fabrica;   /* DISCUS: radix arboris */
               Capsula* capsula;   /* CORPUS: corpus apertum */
    constans character* titulus;   /* pro nuntiis: via aut stampa */
} SilexFons;

/* DISCUS: include/ adsit; NIHIL si invalida */
SilexFons*
silex_fons_disci (
               Piscina* piscina,
    constans character* fabrica);

/* CORPUS: capsulam aperit; titulus e clave 'corpus.versio'
 * (absente: "(corpus sine stampa)"); NIHIL si capsula fracta */
SilexFons*
silex_fons_corporis (
                  Piscina* piscina,
    constans CapsulaEmbed* embed);

b32
silex_fons_existit (
    constans SilexFons* fons,
    constans character* via_relativa,
               Piscina* piscina);

/* *inventum FALSUM si via ignota */
chorda
silex_fons_legere (
    constans SilexFons* fons,
    constans character* via_relativa,
               Piscina* piscina,
                   b32* inventum);

/* plagula colligenda: via relativa proiecti + contentum + origo */
nomen structura {
                chorda  via;
                chorda  contentum;
    constans character* origo;   /* "vendicata:<via>" | "genita" */
} SilexRes;

nomen structura {
    constans SilexFons* fons;         /* unde bibliothecae */
    constans character* destinatio;   /* directorium parens ("."...) */
    constans character* titulus;      /* nomen proiecti */
                   b32  vitrea;       /* semen vitreum (cor voluminis)
                                       * + ordines IV generati */
} SilexNovumOptiones;

nomen structura {
                   b32  successus;
                   i32  vendicatae;   /* plagulae e fabrica */
                   i32  genitae;      /* plagulae genitae */
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

/* Vexilla quae ordines generati ferunt. PUBLICA (plan 2 briar): clavis
 * fabricae briar chordam vexillorum ipsam friat - vexilla in proiecto
 * cocta in clave eius sint. */
#define SILEX_VEXILLA_COMPILATIONIS \
    "-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion" \
    " -Wsign-conversion -Wcast-qual -Wstrict-prototypes" \
    " -Wmissing-prototypes -Wwrite-strings -Wno-long-long"
#define SILEX_VEXILLA_VITREA SILEX_VEXILLA_COMPILATIONIS \
    " -Wno-overlength-strings -fbracket-depth=512"
#define SILEX_VEXILLA_VENDITORIA \
    "-O2 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0" \
    " -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0" \
    " -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_OMIT_DEPRECATED" \
    " -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"

/* clausura e TEXTIBUS fontium: semina = lineae '#include "x.h"'
 * contentorum (dedup; <t>_pipa.h et capsula_* omissa - capita
 * templi propria); NIHIL si fracta. Consumptor secundus: briar
 * (regiones C + principale genitum). */
Xar*
silex_clausuram_e_contentis (
               Piscina* piscina,
    constans SilexFons* fons,
       constans chorda* contenta,
                   i32  numerus);

/* Ordines generati (PROIECTIO: mutare licet). 'fontes' = viae
 * fontium proiecto relativae, ordine scripto: silex fontes/<t>.c
 * [+ fontes/<t>_pipa.c]; briar fontes/<t>.c fontes/<t>_regiones.c.
 * Plani: fontes citati + globus bibliothecarum; vitrei: fontes +
 * assets/capsula_<t>.c + clausura explicata (numquam globus) +
 * frameworks. */
chorda
silex_ordinem_fingere (
                       Piscina* piscina,
            constans character* titulus,
    constans character* constans* fontes,
                           i32  numerus_fontium);

chorda
silex_ordinem_probandi_fingere (
                       Piscina* piscina,
            constans character* titulus,
    constans character* constans* fontes,
                           i32  numerus_fontium);

chorda
silex_ordinem_vitreum_fingere (
                       Piscina* piscina,
            constans character* titulus,
    constans character* constans* fontes,
                           i32  numerus_fontium,
                           Xar* clausura_app,
                           Xar* clausura_instrumenti,
            constans character* fons_titulus);

chorda
silex_ordinem_probandi_vitreum_fingere (
                       Piscina* piscina,
            constans character* titulus,
    constans character* constans* fontes,
                           i32  numerus_fontium,
                           Xar* clausura_probationis,
            constans character* fons_titulus);

/* proiectum novum excudere; vide silex.h caput pro forma */
SilexNovumFructus
silex_novum (
                        Piscina* piscina,
    constans SilexNovumOptiones* optiones);

/* fabricam sponte invenire: ascensus ex initio (directorium),
 * signum quaerens (include/latina.h + lib/). Reddit radicem
 * absolutam aut NIHIL si nusquam. Superscriptiones (-f /
 * SILEX_FABRICA) vocatori manent - hoc gradus TERTIUS est. */
constans character*
silex_fabricam_invenire (
               Piscina* piscina,
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
                chorda via;
    SilexPlagulaStatus status;
} SilexStatusRes;

nomen structura {
                   b32  successus;
                   i32  mundae;   /* congruentes (tacent) */
                   Xar* res;      /* SilexStatusRes - non-mundae */
    constans character* erratum;
} SilexStatusFructus;

/* via voluminis proiecti: conventio <dir>/<nomen>.volumen via
 * laeta; ea absente *.volumen solitarium vincit; plura = recusatio
 * clara + NIHIL. (Identitas documenti - decisum red-team IX.) */
constans character*
silex_volumen_viam_invenire (
               Piscina* piscina,
    constans character* proiectum_dir);

/* lector purus - numquam scribit */
SilexStatusFructus
silex_status (
               Piscina* piscina,
    constans character* proiectum_dir);

nomen structura {
                   b32  successus;
                   s64  seq;        /* actus conditionis */
                   i32  conditae;   /* mutatae + novae absorptae */
                   i32  remotae;
    constans character* erratum;    /* "nihil condendum" si vacuum */
} SilexConditioFructus;

/* absorptio: diff statûs -> eventa + actum 'conditio' {nuntius},
 * transactione UNA (omnia aut nihil) */
SilexConditioFructus
silex_condere (
               Piscina* piscina,
    constans character* proiectum_dir,
    constans character* nuntius);

nomen structura {
       s64 seq;
    chorda momentum;
    chorda nuntius;    /* "(ortus voluminis)" pro ortu */
       i32 tactae;     /* plagulae ab introitu priore */
       b32 renovatio;  /* conditio a 'renovare -scribere' posita -
                        * STRUCTURALITER lecta (clavis 'renovatae'
                        * in dato), non e nuntio */
} SilexConditio;

/* conditiones (+ ortus) ordine temporis; NIHIL = erratum */
Xar*
silex_historia (
               Piscina* piscina,
    constans character* proiectum_dir);

/* historia plagulae UNIUS: actus plagulae viae datae, quisque
 * conditioni SEQUENTI attributus (regula eadem qua 'tactae' supra -
 * actus ante conditionem primam ad eam pertinent; ortus ipse
 * vacuus). Xar de SilexPlagulaConditio ordine temporis; vacuum =
 * via numquam tacta; NIHIL = volumen legi non potuit. */
nomen structura {
       s64 seq;        /* conditionis */
    chorda momentum;   /* conditionis */
    chorda nuntius;
    chorda sigillum;   /* hex post conditionem; vacua si remota */
    chorda origo;      /* "condita" | "vendicata:<via>" | ... */
       b32 remota;
       b32 renovatio;
} SilexPlagulaConditio;

Xar*
silex_historia_plagulae (
               Piscina* piscina,
    constans character* proiectum_dir,
    constans character* via);


/* ==================================================
 * Differentia: QUID mutatum est, textu (desideratum 01M08Q43BT -
 * '62 plagulas fide condidi'; status QUAE dicit, hoc QUID).
 *
 * Latera vetus -> novum:
 *   laborans: vetus = plica (a_seq; <= 0 praesens), novum = DISCUS
 *   plicarum: ambo plicae (a_seq/ad_seq; <= 0 = praesens)
 * res ordine viae; aequales tacent (numerus solum). textus =
 * emissio unificata (differentia.h, contextus III) titulis
 * a/<via> b/<via>; !cum_textu: summa tamen computatur (differentia
 * currit, emissio sola omittitur), textus vacua.
 * ================================================== */

nomen structura {
                chorda via;
    SilexPlagulaStatus genus;    /* MUTATA | NOVA | ABSENS */
      DifferentiaSumma summa;
                chorda textus;
                chorda textus_vetus;  /* latus crudum (semper) */
                chorda textus_novus;  /* - consumptores unitatum */
} SilexDifferentiaRes;

nomen structura {
                   b32  successus;
                   Xar* res;       /* SilexDifferentiaRes */
                   i32  aequales;
    constans character* erratum;
} SilexDifferentiaFructus;

SilexDifferentiaFructus
silex_differentia_laborans (
               Piscina* piscina,
    constans character* proiectum_dir,
                   s64  a_seq,
                   b32  cum_textu);

SilexDifferentiaFructus
silex_differentia_plicarum (
               Piscina* piscina,
    constans character* proiectum_dir,
                   s64  a_seq,
                   s64  ad_seq,
                   b32  cum_textu);


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
                   chorda via;
    SilexProiciendaStatus status;
} SilexProiciendaRes;

nomen structura {
    b32  successus;
    Xar* res;        /* SilexProiciendaRes;
                                     * intactae tacent (numerus) */
                   i32  intactae;
                   i32  obices;
                   i32  scriptae;   /* si scribere */
    constans character* erratum;
} SilexProiectioFructus;

SilexProiectioFructus
silex_proicere (
               Piscina* piscina,
    constans character* proiectum_dir,
                   s64  ad_seq,
                   b32  scribere);


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
                  chorda via;
    SilexRenovatioStatus status;
} SilexRenovatioRes;

nomen structura {
                   b32  successus;
                   Xar* res;        /* intactae tacent (numerus) */
                   i32  intactae;
                   i32  renovatae;  /* si scribere: scriptae */
                   i32  additae;
    constans character* erratum;
} SilexRenovatioFructus;

SilexRenovatioFructus
silex_renovare (
               Piscina* piscina,
    constans character* proiectum_dir,
    constans SilexFons* fons,
                   b32  scribere);


/* ==================================================
 * Partes: oraculum clausurae ut verbum. Clausura ex seminibus
 * auctoratis proiecti (aut plagula data), cum examine
 * adest-in-arbore: ABEST = quod 'renovare -scribere' vendicaret.
 * ================================================== */

nomen structura {
                chorda  via;
    constans character* origo;
                   b32  adest;   /* in arbore proiecti */
} SilexPartesRes;

/* plagula data (via in disco proiecti) aut NIHIL = auctoratae
 * omnes; Xar de SilexPartesRes; NIHIL si clausura colligi nequit */
Xar*
silex_partes (
               Piscina* piscina,
    constans character* proiectum_dir,
    constans SilexFons* fons,
    constans character* plagula);

#endif /* SILEX_H */
