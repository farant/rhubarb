#ifndef SENTENTIAE_H
#define SENTENTIAE_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "sigillum.h"

/* ====================================================================
 * SENTENTIAE - lector documentorum LOCIS INSCRIPTORUM
 *
 * Documentum sententiarum est schema cuius nidificatio TOTA in loco
 * scripto est, numquam in indentatione. Exemplar: de-imagine.md
 * (CCLXXIII sententiae, XIV partes).
 *
 *     1. Quid dissolutionem a caecitate distinguit?
 *     1.1. Dissolutio rationem reddit cur problema urgere videbatur
 *          et id quod illud genuit NOMINAT. [principle]
 *     1.1.1. Probatio diagnostica est, non rhetorica: ...
 *     4.obj.1. Obiectio: substratum alia personarum sustinet.
 *     4.obj.1.1. Responsio: concessum, sed ...
 *
 * LEX FUNDAMENTALIS: liber VERITAS est, arbor PROIECTIO. Hic lector
 * numquam scribit. Textus fontis semper intactus manet; quidquid hinc
 * redditur abici et reaedificari potest sine damno. Ideo lector in
 * perpetuum emendari potest sine migratione datorum.
 *
 * ANCORAE ad locum AB AUCTORE SCRIPTUM adhaerent, cum sigillo textus -
 * NUMQUAM ad indicem aut identitatem quam lector generat. Aliter
 * historia commentariorum versioni lectoris colligaretur et libertas
 * supra dicta periret.
 * ==================================================================== */

/* --------------------------------------------------------------------
 * I. DECIPULAE - quas lector ingenuus TACITE fallit
 *
 * Omnes mensuratae in de-imagine.md, non coniectatae. Numeri hic
 * positi sunt ut probationes eos figant (vide probatio_sententiae.c).
 *
 * (a) TAGS PER LINEAS CONTINUATIONIS EXTENDUNTUR. SEPTEM sententiae
 *     retractae sunt; SEX earum per lineas extenduntur; ergo extractor
 *     per lineas operans UNAM invenit - et successum nuntiavit:
 *
 *         29.2. "Unitas per artem defectus est." [retracted: lapide
 *               caelato victum - ars perficit ...]
 *
 *     ERGO ORDO FIXUS: continuationes IUNGERE, deinde tags extrahere,
 *     deinde remissiones. Quicumque tags ante iuncturam inspicit
 *     errat, et TACITE errat.
 *
 * (b) PRIMUM SEGMENTUM LOCI NUMERICUM EST. Si segmentum initiale
 *     nomen esse potest, quodlibet verbum Anglicum puncto terminatum
 *     locus fit: grammatica laxa CCLXXV congruentias reddit ubi
 *     CCLXXIII sunt. Duo falsi (de-imagine.md:15, :21) prosa
 *     prooemii sunt. Segmenta nominata (obj, XXI vicibus) SOLUM in
 *     sede non-initiali occurrunt.
 *
 * (c) PROOEMIUM AD PRIMUM SAEPTUM FINIT, non ad primam lineam locum
 *     simulantem. Regula ingenua prooemium ad lineam XV truncat -
 *     MEDIA sententia quae gradus declarat - et vocabularium
 *     tabulamque siglorum destruit DUM successum nuntiat.
 *
 * (d) COLUMNA ZEPHYRA ONUS FERT. XLII lineae indentatae grammaticae
 *     loci congruunt si quis spatia ante probationem radit. Sola
 *     disciplina columnae zephyrae locum a sententia distinguit quae
 *     forte verbo et puncto incipit.
 *
 * (e) PARENS ABESSE POTEST, ET LEGITIME. Novem loci in de-imagine.md
 *     parentem non habent - 4.obj.1, 7.obj.1, 10.obj.1, 14.obj.1,
 *     15.obj.1, 15.obj.2, 18.obj.1, 24.obj.1, 24.obj.2. Omnes eandem
 *     formam habent: segmentum NOMINATUM gradum inducit qui punctum
 *     possidens non habet. "4.obj" numquam exstat; "4.obj.1" prima
 *     obiectio est.
 *
 *     ERGO parens absens culpa STRUCTURAE esse NON potest - documentum
 *     verum primo cursu obstrueret. Anomalia est, et solum tunc
 *     flagrat cum omnia segmenta parentis NUMERICA sunt: ita mendum
 *     verum ("1.1.1" sine "1.1") deprehenditur dum rami nominati
 *     accipiuntur. In de-imagine.md nullies flagrat, quod rectum est.
 *
 * (f) NON OMNE UNCINUM GRADUS EST. 15.1 gradum [conjecture] ET
 *     annotationem liberam fert ([Fran's thesis; ...]). Gradus verba
 *     minuscula singula sunt; annotationes maiusculis incipiunt et
 *     spatia continent. Forma sola eas discernit.
 * -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
 * II. GRAMMATICA
 *
 *     liber       := prooemium pars*
 *     pars        := titulus_partis? saeptum sententia* saeptum
 *     sententia   := locus SP textus continuatio*
 *     locus       := numerus ( "." segmentum )* "."
 *     numerus     := [0-9]+
 *     segmentum   := [0-9]+ | [a-z]+
 *     continuatio := linea cuius character primus album est
 * -------------------------------------------------------------------- */

#define SENTENTIAE_PROFUNDITAS_MAXIMA XVI

/* --------------------------------------------------------------------
 * III. GRADUS - vocabularium a documento ipso declaratum
 *
 * Vocabularium NON per phrasin agnoscitur sed per SEDEM: quodcumque
 * [nomen] in prooemio (ante primam sententiam) apparet gradus
 * declaratus est, deduplicatum. Regula Franis, mensurata: prooemium
 * de-imagine.md exacte VIII gradus reddit, nullum falsum positivum,
 * nullum gradum in corpore indeclaratum.
 *
 * Melior est quam phrasin parsare quia reformulationem sententiae
 * declarantis superstes, et nullam mutationem FORMAE poscit - et
 * forma API est (in digitis auctoris vivit; lector gratis mutatur).
 *
 * Declaratio arietatem suam ipsa nuntiat: [retracted: ...] cum
 * ellipsi litterali onus ferre significat.
 * -------------------------------------------------------------------- */

nomen structura
{
    chorda vocabulum;   /* "principle", "retracted" - sine uncinis */
    b32    fert_onus;   /* declaratum ut [x: ...] */
} Gradus;

/* --------------------------------------------------------------------
 * IV. REMISSIONES
 *
 * Internae tribus formis: "(see 11.)", "(see 8.2)", "(10.obj.1)".
 * Externae siglo praefixae: "ST I q.21 a.3" - inertes manent donec
 * liber illud siglum declarans in conditorio exstet.
 * -------------------------------------------------------------------- */

nomen enumeratio
{
    REMISSIO_INTERNA,
    REMISSIO_EXTERNA
} GenusRemissionis;

nomen structura
{
    GenusRemissionis genus;
    chorda           siglum;    /* "ST"; vacuum si interna */
    chorda           scopus;    /* "13.5" aut "I q.21 a.3" */
    chorda           textus;    /* span integer, verbatim */
    b32              resoluta;  /* interna: meta in hoc libro exstat */
} Remissio;

/* Tabula siglorum ex prooemio - "| ST | Aquinas, Summa Theologiae |".
 * Parsanda, non solum reddenda: resolutio remissionum externarum
 * scire debet quae verba sigla OMNINO sint. */
nomen structura
{
    chorda siglum;
    chorda descriptio;
} Auctoritas;

/* --------------------------------------------------------------------
 * V. SENTENTIA
 *
 * `textus` sicut scriptus est (continuationibus iunctis, loco
 * detracto, gradibus INTACTIS). `textus_normatus` forma sigillanda
 * est: spatiis coactis, gradibus detractis. Ambo servantur quia
 * consumptores differunt - inscriptio verbatim ex fonte legit,
 * quaestio ex normato.
 * -------------------------------------------------------------------- */

nomen structura
{
    chorda   locus;              /* "4.obj.1.1" ut scriptus */
    i32      profunditas;        /* numerus segmentorum */
    chorda   textus;             /* iunctus, gradibus intactis */
    chorda   textus_normatus;    /* forma sigillata */
    Sigillum sigillum;

    s32      gradus;             /* index in vocabularium; -I si nullus */
    chorda   onus_gradus;        /* payload de [retracted: ...] */
    Xar*     annotationes;       /* chorda - uncina non-graduum */
    Xar*     remissiones;        /* Remissio */

    i32      pars;               /* index in partes */
    i32      linea;              /* linea loci, ab I - pro nuntiis */

    /* IUDICIA DERIVATA - semel hic computata, ne quisque consumptor
     * chordas iterum comparet (et alius aliter). */
    b32      retractum;
    b32      inresolutum;
} Sententia;

nomen structura
{
    chorda titulus;   /* "Part II - On the mask" */
    i32    prima;     /* index primae sententiae */
    i32    numerus;   /* quot sententiae */
} Pars;

/* --------------------------------------------------------------------
 * VI. DUO GRADUS CULPARUM
 *
 * CULPA STRUCTURAE insertionem OBSTAT et lineam nominat: proiectionem
 * FALSAM redderet. Locus duplex praecipue - duae sententiae unam
 * ancoram vindicarent, et commentaria ad claim FALSAM adhaererent.
 * Ancora mentiens peior est quam fracta, quia nihil eam signat.
 *
 * ANOMALIA accipitur et enumeratur: proiectionem IMPERFECTAM reddit,
 * non falsam.
 * -------------------------------------------------------------------- */

nomen enumeratio
{
    SENTENTIAE_BENE,
    SENTENTIAE_LOCUS_DUPLEX,
    SENTENTIAE_UNCINUS_APERTUS,
    SENTENTIAE_LOCUS_MALEFORMATUS,
    SENTENTIAE_SAEPTUM_IMPAR,
    SENTENTIAE_PROFUNDITAS_NIMIA
} CulpaStructurae;

nomen enumeratio
{
    ANOMALIA_GRADUS_IGNOTUS,        /* uncinum gradui simile, indeclaratum */
    ANOMALIA_REMISSIO_INRESOLUTA,   /* (see 99.9) - meta abest */
    ANOMALIA_LINEA_VAGA,            /* intra saeptum, nec locus nec continuatio */
    ANOMALIA_VOCABULARIUM_ABEST,    /* prooemium nullum gradum declarat */
    ANOMALIA_PARENS_ABEST           /* vide decipulam (e) */
} GenusAnomaliae;

nomen structura
{
    GenusAnomaliae genus;
    i32            linea;
    chorda         textus;   /* fragmentum offendens */
    chorda         causa;    /* semper NOMINAT - numquam vacua */
} Anomalia;

/* --------------------------------------------------------------------
 * VII. LIBER - fructus lectionis
 *
 * Recusatio semper causam NOMINAT, more villae: successus falsus cum
 * culpa muta inutilis est.
 *
 * Omnis chorda hic reddita in piscinam datam TRANSCRIBITUR. Nulla
 * visio in fontem incidentem superstes est. Consulto: fons in arena
 * vocationis vivere potest (quae per singulos gressus reficitur) dum
 * liber per gressus manet; visio superstes ruinam TACITAM pareret.
 * -------------------------------------------------------------------- */

nomen structura
{
    b32             successus;
    CulpaStructurae culpa;
    i32             linea_culpae;
    i32             columna_culpae;
    chorda          causa;

    /* prooemium */
    chorda titulus;
    chorda siglum;          /* "DI" - clavis canonica */
    chorda status;          /* "DRAFT" */
    chorda prooemium;       /* textus crudus ante primum saeptum */
    Xar*   vocabularium;    /* Gradus */
    Xar*   auctoritates;    /* Auctoritas */

    /* corpus */
    Xar*   partes;          /* Pars */
    Xar*   sententiae;      /* Sententia */
    Xar*   anomaliae;       /* Anomalia */
} Liber;

/* --------------------------------------------------------------------
 * VIII. LECTIO
 * -------------------------------------------------------------------- */

Liber sententiae_legere (chorda fons, Piscina* piscina);

/* --------------------------------------------------------------------
 * IX. NORMALIZATIO ET SIGILLUM - duae functiones, consulto
 *
 * Divisae sunt ut fixa chordam NORMATAM figere possint, non solum
 * digestum. "Paragraphus aliter involutus sigillum non mutat"
 * probatum contra digestum solum dicit ALIQUID differre; contra
 * chordam normatam dicit QUID.
 *
 * Normalizatio:
 *   0. fines linearum normare (\r\n -> \n, \r solum -> \n)
 *   1. continuationes spatio uno iungere
 *   2. locum et spatium sequens detrahere
 *   3. spans graduum detrahere (cum oneribus)
 *   4. cursus spatiorum in spatium unum cogere
 *   5. praecidere
 *
 * NULLA plicatura Unicode. Mensuratum: totum corpus LIII characteres
 * non-ASCII habet, omnes lineolas (em-dash LII, en-dash I); nulli
 * apices curvi, nulli tabuli, NFC et NFD cum plagula congruunt.
 * Si insertio futura apices curvos inducat, sigillum id ut mutationem
 * textus recte notabit - quia mutatio EST. Plicatura editionem veram
 * celaret ne signum verum daret.
 *
 * SHA-256, numquam familia XXXII bitorum: friatio ad DISPERGENDUM
 * friat, sigillum ad IDENTIFICANDUM. Ad XXXII bita CCLXXIII
 * sententiae collisionem circa I ad X^V ferunt, et collisio sigilli
 * ancoram INTEGRAM contra claim falsam resolvit.
 * -------------------------------------------------------------------- */

chorda   sententiae_normalizare (chorda textus, constans Xar* vocabularium,
                                 Piscina* piscina);
Sigillum sententiae_sigillare   (chorda textus_normatus);

/* --------------------------------------------------------------------
 * X. QUAESTIO ET ANCORAE
 *
 * MOTUM deprehenditur sed NON sponte sequitur. Sequi involutionem
 * puram invisibiliter tractaret, sed coniectare debet ubi textus et
 * motus et mutatus est - et coniectura falsa ancoram MENTIENTEM
 * facit, quae peior est quam fracta.
 * -------------------------------------------------------------------- */

nomen enumeratio
{
    ANCORA_INTEGRA,   /* locus adest, sigillum congruit */
    ANCORA_MOTA,      /* sigillum alibi congruit */
    ANCORA_MUTATA,    /* locus adest, sigillum differt */
    ANCORA_ORBA       /* locus abest, sigillum nusquam */
} StatusAncorae;

nomen structura
{
    StatusAncorae status;
    chorda        locus_novus;   /* si MOTA */
} IudiciumAncorae;

Sententia*      sententiae_per_locum   (constans Liber* liber, chorda locus);
IudiciumAncorae sententiae_ancoram_iudicare (constans Liber* liber, chorda locus,
                                             Sigillum sigillum);

/* Parens de "4.obj.1.1" est "4.obj.1". FALSUM si locus radix est. */
b32 sententiae_parentem_capere (chorda locus, chorda* parens);

/* --------------------------------------------------------------------
 * XI. NOMINA - functiones publicae, ne consumptor tabulas repetat
 * -------------------------------------------------------------------- */

constans character* sententiae_culpae_nomen   (CulpaStructurae culpa);
constans character* sententiae_anomaliae_nomen (GenusAnomaliae genus);
constans character* sententiae_ancorae_nomen  (StatusAncorae status);

#endif /* SENTENTIAE_H */
