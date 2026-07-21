#ifndef AEDILIS_H
#define AEDILIS_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"

/* ====================================================
 * AEDILIS - Stratum Constructionis (machina clausurae)
 *
 * Magistratus operum publicorum: plagulae datae clausuram
 * dependentiarum VERAM derivat - quae obiecta, quae capita,
 * quae vexilla - et manifestum STML emittit. Machina PURA est:
 * extractio directivarum per suturam inicitur (silva in binario
 * bin/aedilis, tabulae fixturarum in probationibus) - machina
 * ipsa tantum viam/filum/stml tangit, numquam silvam.
 *
 * Postura defectus: RECUSARE CLAMOSE - nullus fructus, causa
 * nominata. Praecisio aut silentium.
 *
 * Spec: project-specs/aedilis-spec-v2.md; parcum 01KXJ2HV.
 * ==================================================== */

/* Extractor: plagulam datam (via iungibilis, radice iam praefixa)
 * legere et reddere:
 *   directivae_out:   Xar de chorda - textus directivarum
 *                     inclusionum ut rogatus ("via.h")
 *   annotationes_out: Xar de chorda - textus post "aedilis:"
 *                     ancoratum, tersus ("obiectum lib/x.c")
 *   ex_oraculo_out:   VERUM = cursus minoritatis (-MM pro .m);
 *                     directivae tunc viae IAM RESOLUTAE
 *                     relativae radici sunt
 * Chordae redditae in piscinam datam copiandae sunt (vita earum
 * vitam extractionis excedit). FALSUM = plagula illegibilis aut
 * imparsabilis -> machina recusat. */
nomen b32 (*AedilisExtractor)(
    vacuum*             datum,
    constans character* via,
    Piscina*            piscina,
    Xar**               directivae_out,
    Xar**               annotationes_out,
    b32*                ex_oraculo_out);

/* ====================================================
 * Configuratio (aedilis.stml)
 * ==================================================== */

nomen structura {
    chorda obiectum;   /* basis obiecti, e.g. "fenestra_macos" */
    Xar*   vexilla;    /* chorda */
} AedilisRegulaNexus;

nomen structura {
    chorda fons;       /* e.g. "vendor/sqlite3.c" */
    Xar*   vexilla;    /* chorda */
} AedilisRegulaVendor;

nomen structura {
    chorda caput;      /* e.g. "include/aliquid.h" */
    Xar*   obiecta;    /* chorda: fontes obiectorum */
} AedilisIrregulare;

nomen structura {
    chorda radix;      /* directorium aedilis.stml (radix probarum) */
    Xar*   inclusa;    /* chorda: directoria -I, ordine */
    Xar*   vexilla;    /* chorda: vexilla basis compilationis */
    Xar*   variantes;  /* chorda: praelatio variantium, ordine */
    Xar*   regulae_nexus;   /* AedilisRegulaNexus (valore) */
    Xar*   regulae_vendor;  /* AedilisRegulaVendor (valore) */
    Xar*   irregularia;     /* AedilisIrregulare (valore) */
} AedilisConfiguratio;

/* Legere configurationem ex via data; radix = directorium eius.
 * NIHIL + causa si absens aut malformata. */
AedilisConfiguratio*
aedilis_configurationem_legere(
    Piscina*            piscina,
    constans character* via_stml,
    chorda*             causa_out);

/* ====================================================
 * Fructus derivationis
 * ==================================================== */

nomen enumeratio {
    AEDILIS_ORIGO_DERIVATUM = ZEPHYRUM,  /* proba conventionis */
    AEDILIS_ORIGO_CONFIGURATIO,          /* regula irregularis */
    AEDILIS_ORIGO_ANNOTATIO,             /* annotatio obiecti in
                                          * commento fontis (NB hic
                                          * ancoram ipsam scribere
                                          * = se-annotatio!) */
    AEDILIS_ORIGO_ORACULUM,              /* cursus -MM (.m) */
    AEDILIS_ORIGO_CORPUS                 /* annotatio corporis IN
                                          * CAPITE: implementator
                                          * declaratus - AMBULATUR
                                          * (limes 1MB manet);
                                          * absens = recusatio */
} AedilisOrigo;

nomen structura {
    chorda       via;            /* fons obiecti (lib/x.c) */
    chorda       caput;          /* caput vocans; mensura 0 licet */
    AedilisOrigo origo;
    b32          absens;         /* declaratum, non in disco
                                  * (generata - annotatio sola) */
    Xar*         vexilla_nexus;  /* chorda ex regulis; NIHIL licet */
} AedilisObiectum;

nomen structura {
    chorda       via;
    AedilisOrigo origo;
    Xar*         inclusa;  /* chorda: viae resolutae quas HOC caput
                            * includit (aristae graphi - Phasis
                            * amalgamatis A); NIHIL = nullae
                            * (numquam ambulatum aut sine
                            * directivis). Corpora ordine libera
                            * sunt (probatio permutationis) - aristae
                            * in capitibus solis. */
} AedilisCaput;

nomen structura {
    chorda fons;       /* vendor/x.c */
    Xar*   vexilla;    /* ex regula; NIHIL licet */
} AedilisVendor;

nomen structura {
    chorda scopus;
    chorda varians;
    Xar*   obiecta;           /* AedilisObiectum (valore) */
    Xar*   capita;            /* AedilisCaput (valore) */
    Xar*   systemata;         /* chorda */
    Xar*   vendores;          /* AedilisVendor (valore) */
    Xar*   vexilla_annotata;  /* chorda */
} AedilisFructus;

/* Clausuram scopi derivare (punctum fixum trans plagulas).
 * varians NIHIL = praelatio prima configurationis.
 * NIHIL + causa in recusatione - nullus fructus partialis. */
AedilisFructus*
aedilis_derivare(
    Piscina*                      piscina,
    constans AedilisConfiguratio* configuratio,
    constans character*           scopus,
    constans character*           varians,
    AedilisExtractor              extractor,
    vacuum*                       extractor_datum,
    chorda*                       causa_out);

/* Capita ordine topologico reddere (inclusa ante includentia;
 * intra gyrum ordo inventionis servatur - determinismus). Xar de
 * AedilisCaput* in fructum monstrantium (stabilia - xar segmentata).
 * NIHIL + causa in cyclo, viis cycli nominatis. */
Xar*
aedilis_capita_ordinare(
    constans AedilisFructus* fructus,
    Piscina*                 piscina,
    chorda*                  causa_out);

/* Fructum ut manifestum STML scribere (chorda emissa).
 * commissum NIHIL = attributum omissum. */
chorda
aedilis_manifestum_scribere(
    constans AedilisFructus* fructus,
    Piscina*                 piscina,
    constans character*      commissum);

/* Fructum ut scriptum bash scribere (chorda emissa).
 * solitarius FALSUM = cache-reutens (obiecta communia in
 * build/aedilis/obiecta/, vetustas per capita manifesti);
 * solitarius VERUM = hermeticum (obiecta propria, semper
 * recompilat). Scriptum ex radice repositorii currendum
 * (custos intus); binarium in build/aedilis/<basis>/<basis>.
 * bash-3.2-tutum, porta per codicem exitus, sine ANSI. */
chorda
aedilis_scriptum_scribere(
    constans AedilisFructus*      fructus,
    constans AedilisConfiguratio* configuratio,
    Piscina*                      piscina,
    b32                           solitarius,
    constans character*           commissum);

#endif /* AEDILIS_H */
