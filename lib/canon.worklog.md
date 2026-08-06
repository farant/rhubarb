# canon.worklog.md

## 2026-08-06 — nativitas: schema STML generale (gradus II)

CUR: domus quinque dialectos STML scribit - natura (33 plagulae),
librarium (3, ~CXL milia linearum), grammatica (4, c89.stml solum
MCCXVII lineae quae parserem C regunt), aedilis (2), quaestiones
(1). UNA sola custodem habebat. Vitium schematis in c89.stml aut
aedilis.stml hodie ut mos mirus infra apparet, non ut diagnosticum.

FORMAE CONTENTUS DERIVATAE, NON CONIECTATAE. Instrumentum scriptum
quod corpus ambulat et pro quoque elemento liberos cum min/max et
attributa cum frequentia refert. Duo momenti:
  - CARDINALITAS SINE ORDINE SUFFICIT. Nullus ordo in natura
    postulatur, ergo NULLA opus est forma contentus regularis
    (DTD-stylus). Saccus inordinatus cum numeris omnia tegit -
    lingua multo minor.
  - OBSERVATUM NON EST POSTULATUM. cultivar semel apparet, ergo
    liberi eius {1,1} videntur; fons/@verificatus in XIII ex XIII
    adest sed METAMODULUS 'ordinarius falsum' dicit. Canon ex uno
    exemplo GENERATUS corpus fossilizat - ideo manu SCRIPTUS est,
    derivatione ut teste, non ut auctore.

LINGUA EXPRESSIONUM NON AEDIFICATA, CONSULTO. Assertiones
('valens_a <= valens_ad', 'ad AUT a') linguam expressionum poscunt,
et illa via schemata in linguas programmandi vertit (XSD testis).
Canon gradum II tenet; cetera vocanti relinquuntur.

PRIMA CONTACTIO CORPORIS TRIA INVENIT:
  1-2. 'inversa' DUOBUS MODIS adhibetur: 'verum' (booleanum, ut
       METAMODULUS par.3 dicit) et nomen relationis quam invertit
       ('coquitur' in pharmacon, 'continet' in plagula_
       computatralis). Corpus SIBI dissentit; forma nomen ferens
       plus dicit, sed spec booleanum poscit. QUAESTIO DOCTRINAE.
  3.   quando="-312" (via Appia, CCCXII a.C.) formam 'dies' non
       implet, quae annos ante Christum exprimere NON POTEST.
       ET GRAVIUS: onerator 'quando' NUMQUAM probavit - regula VII
       valens_a/valens_ad sola tegit. Canon probationem invenit
       quam onerator non habebat, primo cursu.

Id est quod stratum schematis praestare debet: non minus codicis in
natura (regulae IX-XIII manent, et illae sunt gemmae), sed
FUNDAMENTUM ubi nullum erat, et vocabularium quod in tribus locis
manu servatum non amplius est.

## 2026-08-06 — dialectus SECUNDUS (aedilis): duo defectus linguae

Aedilis scriptus est ut PROBATIO GENERALITATIS, non ut lucrum. Duo
defectus statim invenit quos natura sola numquam ostendisset - quod
est tota ratio dialecti secundi ante fidem.

I. FORMAE CONTEXTU DEPENDENTES. <regula> duas formas fert:
     intra <nexus>        obiectum= poscit, <vexillum> continet
     intra <irregularia>  caput= poscit, <obiectum> continet
   Declarationes GLOBALES (mos DTD) unionem solam dicere possunt,
   unde '<regula obiectum="x"><obiectum/></regula>' transiret.
   Attributum 'intra=' additum (mos RELAX NG); clavis tabulae fit
   "parens/nomen", et specialius vincit. Natura eo caruit quia
   nullum nomen bis diverse adhibet.

II. REGISTRUM PER EXTENSIONEM SOLAM FALSUM EST. Primo scripsi
    '.stml -> aedilis.canon'. Sed '.stml' QUATTUOR dialectos fert
    (librarium, grammatica, aedilis, quaestiones): illa linea tres
    alios dialectos canone alieno iudicasset. Extensio contractum
    nominat SOLUM ubi extensio dialecto propria est - quod '.genera'
    est et '.stml' non.
    Additum: clavis per ELEMENTUM RADICIS ('<aedilis>'), quae
    dialectum semper nominat quia documentum se ipsum profitetur.
    RADIX VINCIT, extensio cadit.

ET UNCUS BINARII STALI RURSUS. Mutationem 'intra' per
`clang -c ... -o /dev/null` probavi - quod SYNTAXIN probat et
BINARIUM NON SCRIBIT - deinde bin/canon_examen VETUS cursus est.
Scopus tacite fallebat et causam in tabula dispersa quaerebam.
Aedificatio depurationis (flagra pauciora) rem statim solvit, quia
illa RE VERA compilata est.
Eadem lectio quam natura_visus porta stala custodit; canon_examen
eam nondum habet. '-o /dev/null' probat te compilare posse, non te
compilavisse.

MENSURATUM: natura 33 plagulae / 3 vitia (vera, supra); aedilis
2 plagulae / 0 vitia.

## 2026-08-06 (pars altera) — from judge to platform, in one day

The library grew from "layer-2 judge" to a platform in one
continuous pull. Entries here are the non-obvious parts only;
the narrative lives in memory/canon-project.md and the ledger.

**Self-hosting (canon.canon)**: `canon_legere` tolerates unknowns
by design (a reader), which means every typo in a canon silently
weakens it. The fix is not a stricter reader but a canon FOR
canons — the registry maps `<canon>` to canon.canon, so every
canon (including itself) is judged. Four silent-swallow classes
became vitia. Law: *lector tolerat, contractus poscit.*

**Canon infixus + ambo iudicant**: a document may carry its canon
as the FIRST element child of its root (DTD internal subset, minus
entities). Registry canon = floor that cannot be shed; the infixus
can only add, and is itself judged against canon.canon — so a
document cannot quietly weaken its own contract (the XSD
schemaLocation failure, closed structurally).

**Each dialect grows the language** (now law, thrice confirmed):
aedilis forced `intra=` on elementum; grammatica forced the
`compositum` datatype (kebab names — canon.canon itself could not
say `nomen="genera-extra"` without it); quaestiones + rule XII
forced instance scoping. Write the next dialect to find the next
feature.

**Instance scoping mechanics**: `_scopos_colligere` collects ALL
instances (nested ones too — each is its own scope);
`_subarborem_colligere` stops at nested instance boundaries, so an
outer machine never sees an inner machine's statuses. The infixus
subtree is pruned from every walk.

**citatio 'ad' malformed = whole canon load fails.** A citatio
missing its slash would otherwise be a checker-shaped object that
checks nothing. Same rule for a lying `ordinarius=` (default not
conforming to its own genus). Loud beats plausible.

**praestitutum, not ordinarius, as the C field** — `ordinarius`
is a latina macro (`default`); the examen hook caught the broken
typedef within seconds. The STML attribute keeps the natural name.

**Migration pattern (rules V, VII-forma, VIII, XII, XV)**: plant a
differential BEFORE deleting the loader site — both judges must see
the same planted faults and the same clean corpus. The differential
immediately caught a stale natura_examen binary whose old canon
reader silently ignored `<citatio>` elements. Presentation keeps
historical rule numbers (examen maps vitium genus → 5/8/12/15).

**canon_coquere**: the tiny type system is what makes codegen
clean (JAXB drowned in XSD's rich types). Generated readers APPLY
ordinarius= (canon states, reader applies); structs always keep
StmlNodus* (hybrid depth; linea rides free). Generated files must
bear the EXACT house signum "NOLI MANU MUTARE" — my synonym
("EDERE") fell outside generata-custos's regex and the readers
were silently unprotected. The staleness gate (cocta.registrum +
canon_coquere.sh -probare) is SEMANTIC: it diffs regenerated
output, so comment-only canon edits stay fresh; the probe copy
needs three path substitutions (regen command, include guard,
#include basename) or the paths themselves fake a diff.

**stml linea**: the tokenizer had counted lines since birth;
nobody had copied the number onto nodes. Ten lines at
_parser_creare_nodus. En route: clausura_anonyma was never
initialized in the copy function or three builder constructors —
uninitialized memory in every copied node until today.
