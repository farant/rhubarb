#ifndef AEDILIS_H
#define AEDILIS_H

#include "latina.h"
#include "sigillum.h"
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
 *   annotationes_out: Xar de chorda - verbum argumentumque ex
 *                     <aedilis verbum="arg"/> ("obiectum lib/x.c")
 *   ex_oraculo_out:   VERUM = cursus minoritatis (-MM pro .m);
 *                     directivae tunc viae IAM RESOLUTAE
 *                     relativae radici sunt
 *   angulatae_out:    Xar de b32, directivis parallela - VERUM =
 *                     forma angulata, FALSUM = citata. NIHIL
 *                     licet (forma ignota): inresoluta tunc ut
 *                     olim systema solum habetur
 * Chordae redditae in piscinam datam copiandae sunt (vita earum
 * vitam extractionis excedit). FALSUM = plagula illegibilis aut
 * imparsabilis -> machina recusat. */
nomen b32 (*AedilisExtractor)(
    vacuum*             datum,
    constans character* via,
    Piscina*            piscina,
    Xar**               directivae_out,
    Xar**               annotationes_out,
    b32*                ex_oraculo_out,
    Xar**               angulatae_out);


/* ====================================================
 * Configuratio (aedilis.stml)
 * ==================================================== */

nomen structura {
    chorda  obiectum;   /* basis obiecti, e.g. "fenestra_macos" */
       Xar* vexilla;    /* chorda */
} AedilisRegulaNexus;

nomen structura {
    chorda  fons;       /* e.g. "vendor/sqlite3.c" */
       Xar* vexilla;    /* chorda */
} AedilisRegulaVendor;

/* Regula compilationis per fontem NOSTRUM (sectio 'compilatio'):
 * vexilla post communia addita; nomen obiecti ea fert
 * (lib__sigillum__O2.o), ut obiectum commune vexillis aliis numquam
 * per mtime recens videatur (fabrica T8: sigillum -O2). */
nomen structura {
    chorda  fons;       /* e.g. "lib/sigillum.c" */
       Xar* vexilla;    /* chorda */
} AedilisRegulaCompilationis;

nomen structura {
    chorda  caput;      /* e.g. "include/aliquid.h" */
       Xar* obiecta;    /* chorda: fontes obiectorum */
} AedilisIrregulare;

/* Oraculum -MM per variantem (sectio 'oracula', testa-ios T3): fontes
 * .m silva non parsat - clang -MM capita eorum dicit, sed clang nudus
 * capita macOS solum videt (UIKit in SDK iOS solo) */
nomen structura {
    chorda varians;    /* e.g. "ios" - suffixum basis "_ios" */
    chorda mandatum;   /* compilator pro "clang" (sine -MM) */
} AedilisOraculum;

nomen structura {
    chorda  radix;      /* directorium aedilis.stml (radix probarum) */
       Xar* inclusa;    /* chorda: directoria -I, ordine */
       Xar* vexilla;    /* chorda: vexilla basis compilationis */
       Xar* variantes;  /* chorda: praelatio variantium, ordine */
       Xar* regulae_nexus;   /* AedilisRegulaNexus (valore) */
       Xar* regulae_vendor;  /* AedilisRegulaVendor (valore) */
       Xar* regulae_compilationis; /* AedilisRegulaCompilationis */
       Xar* irregularia;     /* AedilisIrregulare (valore) */
       Xar* oracula;         /* AedilisOraculum (valore) */
} AedilisConfiguratio;

/* Legere configurationem ex via data; radix = directorium eius.
 * NIHIL + causa si absens aut malformata. */
AedilisConfiguratio*
aedilis_configurationem_legere (
               Piscina* piscina,
    constans character* via_stml,
                chorda* causa_out);

/* Praelationem variantium hospiti aptare: extra macOS 'macos' tollitur
 * (Cocoa, libproc.h, mach absunt), ergo posix vincit; in macOS nihil
 * agit. Instrumenta (bin/aedilis, fabrica) post configurationem VERAM
 * lectam vocant. Bibliotheca ipsa praelationem configuratam semper
 * honorat - probationes fixturis utuntur, hospite quovis. */
vacuum
aedilis_variantes_hospiti_aptare (
    AedilisConfiguratio* configuratio,
                Piscina* piscina);

/* Compilator oraculi -MM pro fonte .m: mandatum regulae 'oracula'
 * cuius varians suffixum basis est ("lib/testa_ios.m" -> regula
 * "ios"); nulla congruit -> "clang" (capita macOS). */
chorda
aedilis_oraculum_eligere (
    constans AedilisConfiguratio* configuratio,
              constans character* via,
                         Piscina* piscina);


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
          chorda via;            /* fons obiecti (lib/x.c) */
          chorda caput;          /* caput vocans; mensura 0 licet */
    AedilisOrigo origo;
             b32 absens;         /* declaratum, non in disco
                                  * (generata - annotatio sola) */
    Xar* vexilla_nexus;  /* chorda ex regulis; NIHIL licet */
} AedilisObiectum;

nomen structura {
          chorda  via;
    AedilisOrigo  origo;
             Xar* inclusa;  /* chorda: viae resolutae quas HOC caput
                            * includit (aristae graphi - Phasis
                            * amalgamatis A); NIHIL = nullae
                            * (numquam ambulatum aut sine
                            * directivis). Corpora ordine libera
                            * sunt (probatio permutationis) - aristae
                            * in capitibus solis. */
} AedilisCaput;

nomen structura {
    chorda  fons;       /* vendor/x.c */
       Xar* vexilla;    /* ex regula; NIHIL licet */
} AedilisVendor;

nomen structura {
    chorda  scopus;
    chorda  varians;
       Xar* obiecta;           /* AedilisObiectum (valore) */
       Xar* capita;            /* AedilisCaput (valore) */
       Xar* systemata;         /* chorda */
       Xar* inresolutae;       /* chorda: citatae nusquam inventae -
                                * pars systematum, sed manifestum
                                * sine eis INCOMPLETUM est (fabrica
                                * P1); forma ignota = nullae */
       Xar* vendores;          /* AedilisVendor (valore) */
       Xar* vexilla_annotata;  /* chorda */
       Xar* pura;              /* chorda: plagulae clausurae quae
                                * <aedilis nexus="purus"/> ferunt
                                * (eventus A1b) */
       Xar* facultates;        /* chorda: <aedilis facultas="X"/>
                                * SCOPI solius (fabrica-6 T6): fenestra
                                * | rete | repositorium. In plagula
                                * non-scopo aut valor ignotus ->
                                * recusatio nominata */
       Xar* instrumenta;       /* chorda: <aedilis instrumentum=
                                * "bin/X"/> SCOPI solius (fabrica-7
                                * T2): binaria domus quae scopus
                                * currit (clavis membri identitatem
                                * eorum sequitur). Extra bin/, in
                                * plagula non-scopo aut duplex ->
                                * recusatio nominata */
} AedilisFructus;

/* Clausuram scopi derivare (punctum fixum trans plagulas).
 * varians: nomen unum ("posix") aut index ordine praelationis
 * ("android,posix": X_android, aliter X_posix); NIHIL = praelatio
 * configurationis. Index malus (elementum vacuum) = recusatio.
 * NIHIL + causa in recusatione - nullus fructus partialis. */
AedilisFructus*
aedilis_derivare (
                         Piscina* piscina,
    constans AedilisConfiguratio* configuratio,
              constans character* scopus,
              constans character* varians,
                AedilisExtractor  extractor,
                          vacuum* extractor_datum,
                          chorda* causa_out);

/* Capita ordine topologico reddere (inclusa ante includentia;
 * intra gyrum ordo inventionis servatur - determinismus). Xar de
 * AedilisCaput* in fructum monstrantium (stabilia - xar segmentata).
 * NIHIL + causa in cyclo, viis cycli nominatis. */
Xar*
aedilis_capita_ordinare (
    constans AedilisFructus* fructus,
                    Piscina* piscina,
                     chorda* causa_out);

/* NEXUS PURUS (eventus A1b, Franus 2026-10-01): plagula quae
 * <aedilis nexus="purus"/> fert promittit clausuram SUAM nullum
 * obiectum regula nexus (aedilis.stml sectio 'nexus': frameworks,
 * e.g. fenestra_macos -> Cocoa) ferre - nihil ultra bibliothecam C.
 * Plagulam ut scopum derivat; VERUM = pura aut nihil promissum
 * (sine annotatione); FALSUM + causa: obiectum, vexilla regulae,
 * catena capitum ab ipsa plagula ("a.h -> b.h -> lib/b_macos.m
 * [-framework Cocoa]"). Recusatio derivationis -> FALSUM + causa
 * eius. */
b32
aedilis_nexum_purum_probare (
                         Piscina* piscina,
    constans AedilisConfiguratio* configuratio,
              constans character* via,
                AedilisExtractor  extractor,
                          vacuum* extractor_datum,
                          chorda* causa_out);

/* Vexilla nexus clausurae: regulae obiectorum (aedilis.stml sectio
 * 'nexus', ordine obiectorum) deinde annotata, deduplicata - eadem
 * quae linea nexus scripti fert. Xar de chorda (vacuum licet).
 * Consumptores sine scripto (tools/ios/app.sh per 'aedilis --nexus',
 * testa-ios T3) ea legunt. */
Xar*
aedilis_vexilla_nexus (
    constans AedilisFructus* fructus,
                    Piscina* piscina);

/* Fructum ut manifestum STML scribere (chorda emissa).
 * commissum NIHIL = attributum omissum. */
chorda
aedilis_manifestum_scribere (
    constans AedilisFructus* fructus,
                    Piscina* piscina,
         constans character* commissum);

/* Fructum ut scriptum bash scribere (chorda emissa).
 * solitarius FALSUM = cache-reutens (obiecta communia in
 * build/aedilis/obiecta/, vetustas per capita manifesti);
 * solitarius VERUM = hermeticum (obiecta propria, semper
 * recompilat). Scriptum ex radice repositorii currendum
 * (custos intus); binarium in build/aedilis/<basis>/<basis>.
 * bash-3.2-tutum, porta per codicem exitus, sine ANSI. */
chorda
aedilis_scriptum_scribere (
         constans AedilisFructus* fructus,
    constans AedilisConfiguratio* configuratio,
                         Piscina* piscina,
                             b32  solitarius,
              constans character* commissum);

/* Idem ad directorium EXITUS datum (EXITUS_DIR, et solitarius
 * <exitus>/solitarius) pro build/aedilis/<basis>, et thesaurum
 * OBIECTORUM datum (OBIECTA_DIR) pro build/aedilis/obiecta -
 * consumptores qui binarium probant neque manifestum basis communis
 * neque obiecta communia tangere debent (porta instrumentorum,
 * unitates-codicis U5 b: obiecta provenientiae in thesauro communi ab
 * installatoribus manu scriptis deleta 'scripsit extra vestigium'
 * fecerunt). exitus/obiecta NIHIL = ordinarium, octetim. */
chorda
aedilis_scriptum_scribere_ad (
         constans AedilisFructus* fructus,
    constans AedilisConfiguratio* configuratio,
                         Piscina* piscina,
                             b32  solitarius,
              constans character* commissum,
              constans character* exitus,
              constans character* obiecta);

/* CLAVIS THESAURI (circulus commissionis P1): praefixum quod
 * aedilis_silva_creare accipit (hic, non in aedilis_silva: silvam
 * non poscit - sigillum et filum sola). ingressus = digestum actionis
 * 'aedilis' IN AEDIFICATIONE infixum (provenientia: bin/aedilis suum,
 * bin/fabrica per provenientiam 'extractor') - LXIV hex minusculae:
 * clavis = sigillum("extractor:" + ingressus + configuratio).
 * Binaria ex fontibus extractoris iisdem clavem EANDEM habent
 * (thesaurus communis), et editio extra extractorem (fabrica, crusta,
 * portae) clavem non mutat. Aliter (NIHIL, 'ignotum' bootstrap, forma
 * prava) = sigillum(binarium + configuratio) ut olim. FALSUM si
 * configuratio aut (cum opus est) binarium legi nequit. *fons_out
 * (NIHIL licet) = "digestum" aut "binarium". */
b32
aedilis_thesauri_praefixum (
     constans character*  ingressus,
     constans character*  via_binarii,
     constans character*  via_configurationis,
                Piscina*  piscina,
               Sigillum*  praefixum_out,
     constans character** fons_out);

#endif /* AEDILIS_H */
