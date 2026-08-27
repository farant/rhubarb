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

## 2026-08-07 — citatio ad= accipit indicem titulorum

`ad="elementum/attributum"` grew a list form: `ad="animal apis
canis homo/nomen"` — space-separated titles before the solidus,
split with the same fissio+praecidere idiom super= already used
(the membership test on the check side is likewise a near-copy of
the super= loop twenty lines below it; three symmetries for one
feature). One title = the old form, byte-compatible.

WHY: canon collects keys by EXACT title, natura's references are
subsumptive (a species IS its genus). The natura generator now
compiles subsumption into enumeration — the closure of a target
kind becomes the ad= list — instead of refusing to cite non-leaf
targets. Canon still never learns inheritance.

Hardening found en route: `ad=" /n"` (whitespace-only element
part) used to intern a one-space title that matched nothing — a
citation that silently guarded nothing. Now clamat like the
missing-solidus form. Pre-fix oracle held: the whitespace test and
the closure-pendens test both fail on the old code.

## 2026-08-07 — signa: identitas + referentia (the # / . trichotomy)

Two new value-genera. `identitas` = mandatory `#` + compositum body
(nomen= declarations); `referentia` = `#` or `.` + body (relation
values). Bare in referentia is a TYPE error — so "references
announce themselves" holds at every relation site, including the
open-target and multiplex ones citation can't reach.

Citatio pass II dispatches on the first character: `.` → vocabulary
check against the ad_elementa titles (no keys needed — works
per-module); everything else → verbatim key lookup, which covers
BOTH `#` (keys are sigiled too, so declaration and reference match
byte-for-byte, no stripping anywhere) AND the legacy bare form.
The legacy branch is load-bearing: natura.canon itself cites bare
values and the .genera dialect keeps them by ruling.

New vitium CANON_VOCABULUM_IGNOTUM (XIII — first attempt at XI
collided with CANON_TEXTUS_MALUS; the vitium enum runs past the
window I had sampled. Grep a WINDOW, collide with the enum).
Suggestion via the existing _suggestio idiom.

## 2026-08-08 — fines: structured value constraints

`fractio="verum"` + inclusive `minimum=`/`maximum=` on attributa,
numerus only (spec fines; forced by documentation-loop miss L4 —
probabilitas documented "inter 0 et 1" in a nota could legally hold
only 0 and 1). The ruling line: canon may state a value's WRITTEN
FORM and value space, never its storage — dies always did this for
dates; numerus now can for decimals and ranges. Vocabulary stays a
closed set of declarative attributes (no expression language, ever).

Implementation notes:
- Comparison WITHOUT floats: _numerum_secare strips sign/leading/
  trailing zeros, _magnitudines_comparare compares digit strings
  with fraction padding. '-0' == '0' handled at parse.
- New vitium VALOR_EXTRA_FINES (XIV) fires only on FORM-VALID
  values outside bounds; form failures stay VALOR_MALUS. Ordering
  in the judgment: congruit first, fines second.
- Lying canons break at load (causa): fines on non-numerus, ill-
  formed bounds, minimum>maximum, ordinarius outside fines. Bounds
  themselves obey the attribute's own fractio.
- canon.canon self-hosts: its minimum/maximum declared
  genus="numerus" fractio="verum", judged by the code they set up.
- TRAP paid twice this feature: an Edit whose old_string starts at
  the function NAME orphans the `interior b32` specifier line above
  it — "specificatores mixti". Anchor edits at the specifier line.
- The name-reuse is deliberate: minimum/maximum on <liberum> =
  cardinality, on <attributum> = value bounds. The corpus-audit
  gate in probatio_natura_canones had to learn the difference
  (element-aware counter) — leniency doctrine unchanged.

## 2026-08-08 — super adstrictum: path-scoped citations

super= entries learn 'parens/titulus' (_super_congruit): child title
AND parent title must match. One helper, one swapped comparison in
pass II; pass I (keys from ad= closures) untouched. This was the
whole missing piece for citing multiplex relation children — their
names collide across parents ('continet' has seven petita), so
global title-match could never aim at them and the generator
refused to emit (its own comment named the reason). Fixture: auctor
under liber cites men, under navis cites ports; swapping targets
fires CITATIO_IRRITA twice.

## 2026-08-10 — Signa migration: `#` returned to document-space, individua become `&name;`

Fran's ruling (decretum 01KZPEXT74), sparked by the fragment-vs-individuum
sigil collision surfacing during the cross-document citation design. The
discovery that settled it: STML_SPEC had already RESERVED `&name;` as an
entity reference ("named singleton, deduplicated, semantically a
wikilink") — the signa trichotomy of 2026-08-07 had squatted on `#`,
which STML assigns to fragments. Migration = convergence, not invention.

Three spaces, three sigils: `#` = document-space (fragments, anchors),
`&name;` = world-space (individua; C address-of, the et-ligature),
`.` = kind-space (unchanged). Semicolon EVERYWHERE including attributes:
the terminator is what makes `&` prose-safe (AT&T stays literal — the
corpus case is real) and keeps one grammar between attribute and future
text positions (wikilink layer for sententiae/forum).

Implementation notes:
- The ENTIRE C change is valor_congruit's IDENTITAS/REFERENTIA case:
  `&` requires trailing `;`, `.` unchanged (referentia only), `#` now
  FALSUM — the migration tripwire (proven with a planted fault: old
  sigil = line-anchored vitium, exit 1, never a silent literal).
- The citation machinery needed ZERO changes: keys and references
  compare VERBATIM (both carry the sigil), so migrating both sides
  preserved symmetry for free.
- Consumers found by grep-then-verify: natura_canones_emissio (sigil
  chooser + semina writer + reference writer with conditional `;`),
  census_orbi.sh (awk patterns + RLENGTH arithmetic — a consumer the
  first recon MISSED; the lesson is the usual one, the sigil had
  quietly acquired a parser outside the canon layer).
- glossae.html's `#` are HTML anchors — untouched, and now
  unambiguously distinct from individuum references, which is the
  whole point of the ruling.
- probatio_canon keeps ONE `#` on purpose: the tripwire fixture
  (`#agna-vetus`). The perl sweep needed a lookahead guard to avoid
  migrating its own tripwire — worth remembering for future sigil
  migrations.
- Reader-gate refused (exit 2) mid-migration because bin/canon_examen
  was stale after canon.c changed — canon_struere.sh, then green. The
  staleness refusal doing its job.

## 2026-08-10 — genus 'titulus' (dotted element names)

canon.canon typed elementum/liberum @nomen as COMPOSITUM, whose
charset has no `.` — so `nomen=".genus"` was vitium IV and the arbor
porphyriana rename was blocked at the grammar's grammar. New
CanonGenusValoris CANON_GENUS_TITULUS = compositum + one optional
leading dot; elementum/@nomen, elementum/@intra, liberum/@nomen moved
to it; attributum/@nomen STAYS compositum. The asymmetry is the
point: the canon grammar mirrors the stml lexer exactly (tags may
dot, attributes never), so a schema cannot describe what the parser
cannot parse. canon.canon self-judges VITIA 0 with the new genus.
Note valor_congruit's dash rule flipped from `== COMPOSITUM` to
`!= NOMEN` when TITULUS joined the case — titulus admits kebab too.

## 2026-08-10 — librarium W1: claves externae

The citation scope grew its second tier: document keys first, then
the canon-held external table (claves-externae block, parsed once).
Design choice vs the scout's sketch: NO per-citation seeding — the
lookup falls back to `canon->claves_externae` (one TabulaDispersa
built at parse), which W2's collision check will read too.

Traps hit/avoided:
- The top-level dispatch loop skips children lacking nomen= — the
  claves-externae branch sits BEFORE that guard, and the suite gate
  proves the block is read (canon-without-block must fail the
  citation). A wrapper that parsed-but-did-nothing would have been
  the perfect silent gate.
- `<clavis>` text is read with stml_textus_internus (VERBATIM) and
  unknown entities stay literal, so `&laika;` arrives as the same
  bytes citation comparison uses. Any future normalization of that
  path would corrupt every key silently.
- Emission: NcEns xar stores VALUES inline (`(NcEns*)xar_obtinere`),
  not pointers — `*(NcEns**)` segfaulted the generator (exit 139,
  loud). Same idiom as _censum_seminum_scribere; copy from there.
- Retired openly: the inscription requirement for world keys
  (probatio_natura_canones DOC_NON_INSCRIPTUS) — that doctrine WAS
  the gap W1 closes; the fixture now guards true-unknown keys.

## 2026-08-10 — augmentation judgment (librarium W3 gradus II)

`<%` nodes are judged in a DEDICATED pass at the end of
canon_iudicare, not inline in nodum_iudicare — because the document's
minted keys (identitas-genus attribute values) must be collected in
FULL before any augmentation is judged (a mint may legally appear
after the augmentation in document order; pinned in probatio).
nodum_iudicare treats augmentations like the infixus: invisible to
licentia, cardinality, and recursion (early-return guard + child-loop
skip), so a parent's content model never has to declare "%".

One collection walk (`_augmenta_cusasque_colligere`) gathers both the
mint table (clavis → minting node) and the augmentation list; it
descends INTO augmentations (nested ones judged independently, each
collected on its own). Target resolution order: home mints first
(genus = the minting node's own element def via elementum_quaerere,
so intra= scoping just works), then claves_externae (genus= kebab →
c->elementa lookup). Unknown key = CITATIO_IRRITA reused — an
augmentation IS a citation; no new code needed for that case.

**CANON_AUGMENTUM_PUGNANS = XV** is the one new vitium: the additive
test is purely mechanical, exactly as the spec promised — a member
found among the kind's liberi WITHOUT maximum= is multiplex (lawful,
then judged recursively in full), a member that is an attributum OR a
bounded liberum is singular (pugna), anything else is ordinary
LIBERUM_ILLICITUM. Measured surprise: the spec's "(+ universal
nota/historia)" parenthetical is half-wrong against the real fold —
nota is unbounded on all 460 kinds (lawful via the ordinary rule, no
special case), but historia carries maximum="1" everywhere, so
augmenting it is CORRECTLY a conflict (a second historia would break
the base canon). The mechanical rule governs; the parenthetical dies.

W2 note: CLAVIS_COLLISA becomes XVI, not XV as the spec sketched.
The stale "omnes decem" banner in probatio_canon fixed while here.

## 2026-08-10 — census migration + the intra= bug the corpus found (W3 gradus III)

rhubarb.census migrated: the &c; stub died (its MESOFACTUM sentence
— "citatio trans documenta nondum exsistit" — was made false by W1
itself), nota lives on as `<% &c;>`; &aedilis; became `<% &aedilis;>`
carrying adhibet+nota (library genus instrumentum-aedificationis
stands per Fran's ruling).

The canon-custos hook caught a REAL W3-2 bug on first contact with
the corpus that all eight unit fixtures missed: overlay members with
intra=-scoped definitions (adhibet intra="instrumentum-…") failed
ELEMENTUM_IGNOTUM, because elementum_quaerere keys the scoped lookup
on the TREE parent — which inside an overlay is "%". nota (global)
passed, adhibet (intra) failed — fixtures had only global members.
Fix: `_elementum_quaerere_vi(c, n, parens_vi)` — a semantic-parent
override; nodum_iudicare threads it (NIHIL everywhere except
augmentation members, which get the target genus title). Pinned in
probatio with an intra= fixture member (folium intra=quercus).
Corpus-contact law confirmed again: the real store carries shapes
fixtures don't imagine.

Tripwire probes against the REAL individua.canon (scratch): PUGNANS
on typatio-vis (single-valued attribute), IRRITA on &nemo_omnino;,
ILLICITUM on rota-quadrata — all scream, all self-describing.
Citations INSIDE overlays (adhibet ad="&silva;") are judged by the
ordinary citation pass unchanged — the walk doesn't skip overlay
interiors, so that property came free.

## 2026-08-10 — collision tripwire (librarium W2, the arc closes)

CANON_CLAVIS_COLLISA = XVI: a document minting an identitas key
that lives in claves-externae. The check rides the W3 judgment
pass for free — `_augmenta_cusasque_colligere` already builds the
mint table, so collision = iterate cusa, one claves_externae
lookup each (the spec's :1820 unicitas-mirror sketch died; W3's
walk was the cheaper home). Message carries the remedy: "cita aut
auge, ne itera".

The semina exemption: generated semina root now carries
fons="natura" (same literal as the claves-externae block);
collision skips documents whose root fons= equals canon->
claves_fons — the library cannot collide with itself. The
individua root element def admits fons= (genus nomen, optional).
A dishonest fons on an authored document is a hand-edit of a
signed generated file — generata-custos territory, pinned anyway
(the exemption mechanism itself is unit-tested both ways).

The tripwire found TWO REAL latent collisions in the natura_canones
fixtures the moment it went live: DOC_INSCRIPTUS minted
&carl_linnaeus; (a library person) and the causat-multiplex fixture
minted &b; — which is lingua B, an actual library individuum.
Fixture keys renamed to home-scoped names; the fixtures' purposes
(local citation mechanics, causat cardinality) survive intact.
That's the tripwire's value demonstrated before it ever reached an
authored document.

Proofs: scratch census in the exact pre-migration shape (minting
&c;) screams COLLISA against the real individua.canon; scratch
with fons="natura" is exempt; live world (rhubarb.census + semina)
VITIA 0; both suites PLENUS; -probare chain exit 0.

## 2026-08-10 — external genus check + star citations (librarium closes its type gap)

Fran asked whether canon kind-checks relation ends or just key
existence — the answer exposed an asymmetry and an artifact:

1. **CANON_CITATIO_ALIENA = XVII**: the cross-document path checked
existence only. The intra-document path was always kind-strict
(keys collected solely from ad_elementa-matching elements, with
compiled descendant closure), but the W1 external fallback did a
bare membership test — `scripta-in="&laika;"` (a dog where a
language is demanded) resolved silently. The genus= was already
IN the external table (emitted for W3's content models); only the
comparison was missing. Now ce->genus is checked against the
rule's ad-index (closure already compiled in, so subsumption is
free); star admits any genus.

2. **Star citations — ad="*/nomen"**: wildcard slots emitted NO
rule at all (NC_CIT_APERTA, "clavis nulla") — a true artifact of
the pre-librarium age, when a wildcard's key-space could not be
computed and cross-document references would have false-flagged.
With cusae + claves_externae the space is computable: the judge's
star claviger is any element whose DEFINITION carries an identitas
attribute of the rule's name; externals pass at any genus. Emitted
in the MONOLITH ONLY (module canons lack claves-externae and would
false-flag library keys). 449 rules materialized; the "428 petita
aperta" audit tail is now checked.

Corpus-contact finds, both pinned: (a) genus-sigil values in
wildcard slots (semina's geritur_a=".processus") hit the
'.'-branch, whose vocabulary for a star rule is the canon's WHOLE
element table, not the ad-index; (b) the L2 fixture deliberately
cited a nonexistent key BECAUSE apertae were unchecked ("clavis
nulla" in its own comment) — now cites &laika;, proving
quamlibet-ness with an existing key. Audit updates: emissae =
citatae + apertae; star is a lawful index entry.

## 2026-08-26 (nocte) — canon_iudicare_expansum: canon judges the SENSE

The doctrinal debt of the compression arc, paid same-day: template
calls contribute nothing to the file-view judgment (quoted
material), so at 84.7% parametrum compression canon effectively
stopped seeing parametra in files. New entry
`canon_iudicare_expansum(canon, radix, piscina, intern)`: runs
stml_expandere, then the ORDINARY judgment on the instantiated
tree — which needs zero new canon logic, because the expanded tree
is an ordinary content document (calls instantiated, arguments
consumed, template definitions dropped; content fragments/aliases
remain and the existing transclusion transparency handles them).
Expansion failure = CANON_EXPANSIO_FRACTA (XXI), one loud vitium
(detail = offending fragment/loculus, numerus = StmlExpansioVitium,
limes = linea).

THE TRAP FOUND AT BIRTH: my first version created an ephemeral
internamentum for the expansion, on the claim "judgment compares
values, not pointers." Wrong layer — the EXPANSION ENGINE compares
definition ids to call ids by interned-pointer equality
(fragmentum_id interned at PARSE time vs call id interned at
expansion time), so a foreign intern made every call
FRAGMENTUM_IGNOTUM. The probe caught it in one run (direct
stml_expandere with the shared intern succeeded; the new function
failed). Contract now explicit in canon.h: intern MUST be the
tree's own internamentum. This is the same pointer-identity
discipline the engine documents internally — now it has a
consumer-facing statement.

Link-list ripple: canon.o now pulls stml_macros.o —
tools/natura_struere.sh and tools/canon_struere.sh FONTES lists
extended (hand-maintained lists; the aedilis-derived test closures
picked it up automatically).

Corpus gate: probatio_silva_canon_corpus now judges the EXPANDED
view of every document too — 73+155 purae_expansae PINNED. First
canonical judgment of the real content since compression began;
the probe's corrupted-document section is caught by the expanded
view as well.

## 2026-08-27 — canon_iudicare_distributum (fifth touch of the sense line)

expand → distribute → judge, mirroring canon_iudicare_expansum;
the expansion-failure vitia builder is now a shared helper
(`_vitia_expansionis_fractae`). New vitium XXII
CANON_DISTRIBUTIO_FRACTA (detail = wrapper titulus, numerus =
StmlDistributioVitium, limes = linea). Same intern contract as
expansum: the tree's OWN internamentum.

The distinguishing fixtures pin what the file view CANNOT see:
- cardinality — one wrapper with two items is ONE child to the file
  view (maximum=1 satisfied) and TWO siblings distributed
  (LIBERI_MULTI);
- attribute presence — the wrapper's t carries down, an item
  tombstone deletes it, and only the distributed view reports
  ATTRIBUTUM_DEEST on the second nota.
Both fixtures assert the file view is 0 vitia first, so each is a
differential oracle, not a single opinion.
