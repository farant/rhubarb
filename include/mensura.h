/*
 * mensura.h - Mensurae nidificatae, per cursum ligatae
 *
 * Quaestio Franis (2026-08-13): metra probationum, compilationis,
 * modulorum singulorum - cum machina et commisso adnexis, ut per
 * tempus depingi possint.
 *
 * ==========================================================
 * NON EST METRUM (01KYAMZVFN)
 * ==========================================================
 *
 * Metrum parcatum est bibliotheca serierum temporalium forma RRD:
 * exempla CONTINUA, CUM DAMNO consulto, magnitudine fixa, resolutio
 * senescens degradatur. Aptum est ad "quantum memoriae per mensem".
 *
 * Mensura contraria est: eventa DISCRETA, SINE DAMNO, cursui uni
 * ligata, NIDIFICATA. "Quod commissum compilationem tardiorem
 * fecit" quaestio est quam RRD respondere NON potest, quia
 * exemplum illud in medium per aetatem fusum erit.
 *
 * Sorores sunt, non idem. Nomina distincta consulto.
 *
 * ==========================================================
 * NIDIFICATIO
 * ==========================================================
 *
 * Mensura quaeque parentem habere potest. Ita tempus compilationis
 * TOTIUS operis mensuras singulorum modulorum CONTINET, et summa
 * partium contra totum probari potest.
 *
 * Trans processus nidificatio GRATIS venit: sessio et parens in
 * AMBITU vivunt (MENSOR_SESSIO, MENSOR_PARENS), quos proles sponte
 * hereditat. Intra processum unum (ansa concharum quae per plagulas
 * currit) heredidatio nihil dat - ibi parens EXPLICITE dandus est.
 *
 * ==========================================================
 * DIARIUM PRIMUM, VOLUMEN POSTEA
 * ==========================================================
 *
 * Scriptio in volumen sqlite aperit et transactionem committit -
 * decem millisecunda fortasse. Per plagulas CL id secunda addit ad
 * ipsam aedificationem quam metimur: instrumentum quod rem mutat
 * quam metitur inutile est, et mox exstinguitur.
 *
 * Ergo via calida est APPENDIX PLAGULAE PLANAE (diarium): linea
 * una, sine sqlite. Deinde mensura_condere totum fascem in volumen
 * transactione una condit, cursu perfecto.
 */

#ifndef MENSURA_H
#define MENSURA_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "volumen.h"
#include "xar.h"


/* ============================================================
 * Typi
 * ============================================================ */

/*
 * Mensura - Momentum unum mensuratum
 *
 * @sessio:   clavis cursus (omnes mensurae unius cursus eam ferunt)
 * @id:       clavis huius mensurae (parens aliarum esse potest)
 * @parens:   VACUA = radix
 * @titulus:  quid mensuratum sit, e.g. "compilatio.lib/chorda.c"
 * @valor:    numerus
 * @unitas:   "secunda", "octeti", "numerus" - SINE HAC axis
 *            depingi non potest nec valores tuto sumi
 * @momentum: millisecunda ab epocha (horologium muri)
 */
nomen structura {
    chorda sessio;
    chorda id;
    chorda parens;
    chorda titulus;
    duplex valor;
    chorda unitas;
       s64 momentum;
} Mensura;

nomen structura {
       b32 successus;
       i32 conditae;
    chorda causa;
} MensuraFructus;


/* ============================================================
 * Claves
 * ============================================================ */

/*
 * mensura_sessionem_novam - Clavis cursus nova
 *
 * Tempore praefixa, ergo ordine temporis ordinatur.
 */
chorda
mensura_sessionem_novam (
    Piscina* piscina);

/*
 * mensura_id_novum - Clavis mensurae nova
 *
 * Unica etiam inter processus simul currentes (tempus + pid +
 * numerator intra processum).
 */
chorda
mensura_id_novum (
    Piscina* piscina);

/*
 * mensura_momentum_nunc - Millisecunda ab epocha
 *
 * s64 quia deltae temporis natura signatae sunt.
 */
s64
mensura_momentum_nunc (vacuum);


/* ============================================================
 * Forma lineae (NUCLEUS PURUS - probabilis sine plagula)
 * ============================================================ */

/*
 * mensura_ad_lineam - Mensuram in lineam TSV vertere
 *
 * Campi: sessio TAB id TAB parens TAB titulus TAB valor TAB
 *        unitas TAB momentum, linea nova terminata.
 *
 * Redde: chorda vacua si titulus/unitas TAB aut lineam novam
 *        continet - forma quae se ipsam frangere sinit datum
 *        tacite corrumpit.
 */
chorda
mensura_ad_lineam (
    constans Mensura* mensura,
             Piscina* piscina);

/*
 * mensura_ex_linea - Lineam in mensuram vertere
 *
 * Redde: FALSUM si linea malformata est (campi pauciores, valor
 *        non numerus).
 */
b32
mensura_ex_linea (
     chorda  linea,
    Mensura* exitus,
    Piscina* piscina);


/* ============================================================
 * Diarium et conditio
 * ============================================================ */

/*
 * mensura_annotare - Mensuram in diarium APPENDERE
 *
 * Via calida: apertio, appensio, clausura. Nullum sqlite.
 */
b32
mensura_annotare (
                constans character* via_diarii,
                  constans Mensura* mensura,
                           Piscina* piscina);

/*
 * mensura_condere - Diarium in volumen transferre
 *
 * Omnes lineae in actis voluminis conduntur (genus "mensura"),
 * transactione una. Diarium POST successum truncatur.
 *
 * Lineae malformatae NON silentur: numerantur et causa eas nominat.
 */
MensuraFructus
mensura_condere (
      constans character* via_diarii,
                 Volumen* volumen,
                 Piscina* piscina);


/* ============================================================
 * CURSUS (analytica T1): res cum initio, fine, exitu
 * ============================================================
 *
 * Lineae DUAE in diario, eodem quo mensurae (via calida = appensio):
 *
 *   cursus TAB initium TAB id TAB parens TAB sessio TAB titulus TAB argv
 *       TAB vocator TAB commissum TAB sordidum(0|1) TAB ramus TAB arbor
 *       TAB machina TAB initium(ms)
 *   cursus TAB finis TAB id TAB momentum(ms) TAB duratio_ms TAB exitus
 *       TAB rss_maxima TAB cpu_usoris_ms TAB cpu_systematis_ms TAB acta
 *       TAB acta_mensura
 *
 * Initium SINE fine = INTERRUPTUS (necatus, pendens, concha fracta) -
 * visibilis, non absens (spec analytica par. II). Mensurae intra cursum
 * per parens ei adhaerent (parens = id cursus). argv sola effugitur
 * (\\ \t \n \r): mandatum ut datum, non identitas; ceteri campi TAB
 * aut lineam novam continentes RECUSANTUR (linea vacua). mensura_condere
 * lineas 'cursus' genere "cursus" condit. Spec:
 * project-specs/analytica-spec.md. */

nomen structura {
    chorda id;          /* clavis cursus (mensura_id_novum) */
    chorda parens;      /* VACUA = radix */
    chorda sessio;
    chorda titulus;     /* identitas operis (titulus chassis aut via) */
    chorda argv;        /* mandatum ut datum, non identitas */
    chorda vocator;     /* homo|commissio|uncus|fabrica|frigida|agens */
    chorda commissum;
       b32 sordidum;    /* arbor operis mutata ad initium */
    chorda ramus;       /* ramus ad cursum, aut 'detached' */
    chorda arbor;       /* radix arboris operis (worktree) */
    chorda machina;
       s64 initium;     /* millisecunda ab epocha */
} CursusInitium;

nomen structura {
    chorda id;
       s64 momentum;            /* finis, millisecunda ab epocha */
       s64 duratio_ms;          /* horologio monotono */
       s32 exitus;              /* codex exitus (CXXVIII + signum) */
       s64 rss_maxima;          /* octeti; 0 = ignota */
       s64 cpu_usoris_ms;
       s64 cpu_systematis_ms;
    chorda acta;                /* referentia corporis actorum; vacua =
                                 * nulla */
       s64 acta_mensura;
    chorda nota;                /* quomodo finivit, verbis (analytica
                                 * T3): compendium portae, eventus
                                 * fabricae; effugitur ut argv,
                                 * praeciditur ad CURSUS_NOTA_MAXIMA
                                 * octetos (charactere integro) */
} CursusFinis;

#define CURSUS_NOTA_MAXIMA CCLVI

nomen enumeratio {
    CURSUS_TRANSIIT = ZEPHYRUM,   /* exitus 0 */
    CURSUS_FRACTUS,               /* exitus alius */
    CURSUS_INTERRUPTUS            /* initium sine fine (aut adhuc
                                   * currens) */
} CursusStatus;

nomen structura {
    CursusInitium initium;
      CursusFinis finis;    /* validus nisi status INTERRUPTUS */
     CursusStatus status;
} Cursus;

/* lineae (nucleus purus); vacua si campus impurus */
chorda
mensura_cursus_initium_ad_lineam (
    constans CursusInitium* initium,
                   Piscina* piscina);

chorda
mensura_cursus_finis_ad_lineam (
    constans CursusFinis* finis,
                 Piscina* piscina);

/* lineam cursus legere: initium aut finis (*est_finis_out dicit
 * utrum). FALSUM si non linea cursus aut malformata. */
b32
mensura_cursus_ex_linea (
           chorda  linea,
    CursusInitium* initium_out,
      CursusFinis* finis_out,
              b32* est_finis_out,
          Piscina* piscina);

/* in diarium appendere (via calida, sine sqlite) */
b32
mensura_cursum_incipere (
        constans character* via_diarii,
    constans CursusInitium* initium,
                   Piscina* piscina);

b32
mensura_cursum_finire (
      constans character* via_diarii,
    constans CursusFinis* finis,
                 Piscina* piscina);

/* cursus ex actis voluminis (genus "cursus"): initium et finis per id
 * coniuncti, ordine initii (aequalitas: ordine actorum). Finis sine
 * initio praetermittitur. Xar de Cursus; NIHIL si volumen legi nequit. */
Xar*
mensura_cursus_legere (
    Volumen* volumen,
    Piscina* piscina);

/* FACTA CURSUUM (colloquia T8): facta numerica de cursu ut MENSURAE
 * ordinariae (parens = id cursus). Forma lineae 'cursus' NON mutatur:
 * diarium commune omnium arborum est, et binarium vetus lineam
 * deformem in conditione ABICIT (campi XIV/XII stricti) - campus novus
 * ab arbore non conflata deleretur. */
#define MENSURA_CUSTOS    "cursus.custos"     /* pid possessoris */
#define MENSURA_AMBITUS   "fabrica.ambitus"   /* membra in ambitu */
#define MENSURA_UNDAE     "fabrica.undae"     /* undae formatae */
#define MENSURA_IUDICATA  "fabrica.iudicata"  /* membra iudicata
                                               * hucusque (per undam) */
#define MENSURA_PORTAE    "consilium.portae"  /* commissio: portae
                                               * debitae */
#define MENSURA_COLLOQUIUM  "colloquium:"   /* + id sessionis Claude
                                             * (CLAUDE_CODE_SESSION_ID);
                                             * cursui SUMMO solum, valor I
                                             * (colloquia T9) */

/* MEMORIA SECTIONIS (credo memoria T3): facta cursui sectionis
 * (<membrum>#<sectio>) adhaerentia, unitas "octeti" - ex campis XII-XIV
 * lineae SECTIO credo (ibi KiB). Sectio non mensa (plagula X/XI
 * camporum) nulla facta habet. */
#define MENSURA_CREDO        "credo."             /* praefixum */
#define MENSURA_ARENA_APEX   "credo.arena_apex"   /* apex piscinae
                                                   * sectionis */
#define MENSURA_APEX_AUCTUS  "credo.apex_auctus"  /* quantum apex vitae
                                                   * processus crevit */
#define MENSURA_VESTIGIUM    "credo.vestigium"    /* vestigium in fine
                                                   * sectionis */

/* mensurae ex actis voluminis (genus "mensura") quarum titulus
 * 'praefixum' incipit (vacuum = omnes), ordine actorum. Xar de
 * Mensura; NIHIL si volumen legi nequit. */
Xar*
mensura_mensuras_legere (
     Volumen* volumen,
      chorda  praefixum,
     Piscina* piscina);

#endif /* MENSURA_H */
