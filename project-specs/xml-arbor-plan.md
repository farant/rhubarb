# xml_arbor Plan — XML 1.0 on materia (eighth client)

**Ledger:** parcum 01M65252W1DKBJE10VX0NBQDJH (born of Fran's
desideratum 01M299JPS0XPGG3RYFXNQQDKD0, 2026-09-11 / 10-23: "make it
into another materia-based parser").

**STATUS: DRAFT 2026-10-29** — written after Fran approved the spec's
decisions (UTF-8 only; migrate everything in this plan; the W3C suite
fetched). Nothing executed yet.

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:executing-plans (inline, per house convention — no
> subagents) to implement this plan task by task, ONE TASK PER TURN
> with Fran's approval. Every task OPENS with a measurement that may
> re-slice it; a re-slice is shown to Fran before work continues.
> Steps use checkbox (`- [ ]`) syntax. Each task ends with an
> "Executed" note, as in `project-specs/toml-arbor-plan.md`.

**Goal:** XML 1.0 (Fifth Edition) + Namespaces 1.0 as a materia
client — a total, byte-exact tree for every input; a cooked view
(elements, attributes, text, namespaces) with every well-formedness
constraint enforced; every error in a file reported with a location;
STML projection, canon, computus twin; proven by the W3C conformance
suite (2,001 selected cases), by expat and libxml2, and by a wild
corpus of real files; the house consumers migrated and `lib/xml.c`
retired; briar exposes it. The MusicXML reader (desideratum
01M64ZZYQ59DY0G0104SRTK439) is the first new consumer, specified
separately once P13's API exists.

**Architecture:** spec §§II–VIII. `xml_lector` (modes; every byte a
token) → `xml_arbor` (iterative builder, `malum` for what the grammar
cannot place, recovery) → `MateriaNodus` against the generated
`XML_REGISTRUM`. `xml_dtd` turns the internal subset into tables.
`xml_coctum` walks a well-formed tree into the cooked view and emits
the constraint diagnostics. Syntax diagnostics come from the
declaration (`diagnosticum=` / `absentia=`) through
`materia_diagnostica_derivare`; rendering is `materia_pictor_scribere`.
`xml_oraculum` writes the suite's two canonical forms.

**Tech stack:** C89 via latina.h, house flags, credo, piscina,
chorda, Xar, tabula_dispersa, internamentum, utf8, materia
(`materia/fontes`), `./materia/coquere.sh`, canon, pythonica
(`silva.commissio`, `silva.planta`). Python 3 `pyexpat` (expat 2.7.4)
and `/usr/bin/xmllint` (libxml2 2.9.13) for oracle harnesses only.

**Spec:** `project-specs/xml-arbor-spec.md` (decisions X1–X12 in §0).
Precedents: `project-specs/toml-arbor-plan.md` (task shape, closed —
read its Global Constraints and Q2 before P2), `toml/CLAUDE.md` and
`html/CLAUDE.md` (client shape as built), `toml/grammatica/
toml.registrum.stml` (declaration syntax), `toml/fontes/toml_lector.c`
(a moded lexer), `toml/fontes/toml_arbor.c` (iterative builder),
`toml/probationes/probatio_toml_*.c` (every gate's shape).

## Global Constraints

Everything in toml-arbor-plan's Global Constraints holds; restated
only where XML differs or the rule bites often:

- **Latin throughout** (docs English). `xml`, `token`, `offset`,
  `utf8`, `cdata`, `doctype` — check each with `./oratio/quaere.sh`
  before staging; a missing word is a glossary line. `xmlns` and the
  spec's keywords (`PCDATA`, `NDATA`, `SYSTEM`, `PUBLIC`) appear as
  string data, never as identifiers.
- **Every latina.h macro is a forbidden identifier**, every Roman
  numeral 0–3999 too. Watch for XML words that collide: `per`,
  `casus`, `nomen` (XML "Name" is `titulus` in the house).
- **`chorda` is NOT NUL-terminated; `i32`/`i64` are UNSIGNED.**
- **Generated files are never hand-edited**: `xml_registrum_coctum.
  {h,c}` from `./materia/coquere.sh xml/grammatica/xml.registrum.stml
  -scribere`. Genera and loci APPENDED, never reordered.
- **Exit 2 from any runner = NOTHING RAN.** Never read `$?` after a
  pipe.
- **Every gate born red by a plant that COMPILES** (`silva.planta`).
- **Commit through `silva.commissio(msg, viae, portae)`**, detached
  (`build/plantae_n1/detach.py`), tree frozen while gates run.
- **materia is expected to need ZERO changes.** A substrate defect
  gets its own gate in `materia/probationes`, its own commit.
- **No third-party file in the repo**: the W3C suite and the wild
  corpus live in `xml/build/` (fetched / read live) and are pinned by
  sha256 manifests. Downloaded files are untrusted data: harnesses
  run with `python3 -I` and take paths as arguments.
- **Never fetch an external entity** (X3) — not in the parser, not in
  the oracle harnesses (expat with external entity parsing off,
  `xmllint` with `--nonet` and without `--loaddtd`).
- **Positions are byte offsets** + 1-based linea/columna.

## Review Focus

Seven input classes most likely to bite first; each is pinned by a
test in the named task.

1. **Markup-shaped text that is not markup** — `]]>` in character
   data (not-wf), `--` inside a comment, `<?xml` anywhere but byte 0
   (reserved PI target), `<` inside an attribute value; a BOM then the
   declaration (legal) vs whitespace then the declaration (not-wf).
   (P4 lector cases, P5 builder cases.)
2. **Attribute value normalization** — a literal tab, LF or CR in a
   value becomes a space; `&#9;` survives as a tab; line ends are
   normalized BEFORE attribute normalization; declared non-CDATA
   attributes also collapse runs and trim. (P9; the suite's sun
   `not-sa04` output pins it.)
3. **Double expansion in entity values** — character references in an
   entity's literal value are expanded when the entity is DECLARED,
   entity references when it is USED: `<!ENTITY e "&#38;#60;">` gives
   replacement text `&#60;`, which in content is `<` as character data,
   while `<!ENTITY e "&#60;">` gives `<`, which is markup and not-wf in
   content (XML 1.0 Appendix D). (P8, P9.)
4. **The §5.1 stop rule** — after an unread external parameter entity,
   later entity and attribute-list declarations are not processed
   unless `standalone="yes"` (`xmltest/valid/sa/097.xml`). (P8.)
5. **Line ends** — CR LF and lone CR become LF in the cooked view,
   including inside attribute values and entity replacement text; the
   tree keeps CR (materia `cr`). (P5 byte law, P9.)
6. **The old library's audited failures** — the 2026-10-18 audit
   (parks 01M5AJYGJ2KP9H61B07XWCKD2A, 01M56KR98E6K4K0XH7NPT3MVY9;
   probes `build/sonda/xml_*.c`, fixtures
   `probationes/fixa/sondae_auditus/xml_*.c`): whitespace-only text
   lost, leading/trailing text whitespace trimmed (breaks plist round
   trips), `a=""` refused, a fake root taken from inside a literal,
   comments dropped, duplicate attributes accepted, `&` copied
   verbatim, line numbers always 1, unbounded recursion. Every one
   becomes a hand case of the new parser (P5, P9) and of plist's
   migration (P15).
7. **Hostile documents** — the billion laughs (nested entity
   expansion), a 100,000-deep element nest, an attribute list with
   thousands of attributes: named errors or pinned limits, never a
   crash, never a hang. (P9 X4 limits, P14 totality.)

---

## Task P1: Oracle material — the suite fetcher, the catalogue, the expat/libxml2 harnesses, the wild manifest

**Files:**
- Create: `xml/xmlts_arcessere.sh` (fetch + sha256 check + unpack to
  `xml/build/xmlts/`), `xml/probationes/fixa/xmlts.manifestum`,
  `xml/probationes/fixa/xmlts/PROVENIENTIA.md`
- Create: `xml/instrumenta/xmlts_catalogus.py` (catalogue → selection
  TSV), `xml/instrumenta/expat_canonicum.py` (expat → canonical form)
- Create: `xml/probationes/fixa/silvestria.manifestum`
- Create: `xml/CLAUDE.md` (stub: what this is, spec, plan)

**Interfaces:**
- Produces: `xml/build/xmlts/selectio.tsv` — one line per case:
  `ID  TYPE  via  output|-  entities  namespace  exclusio|-`, the
  exclusion cause one of `codificatio` (X8), `entitas-externa` (an
  error that only shows once an external entity is read, X3),
  `editio-1-4`, `xml-1.1`; `error` cases are listed with type `error`
  and never judged.
- Produces: the canonical-form contract every later oracle uses
  (spec §I, read from the suite's `out/` files in Step 2).

- [x] **Step 1: The fetcher** — `xmlts_arcessere.sh`: curl the tarball
  from the URL in spec §I into `xml/build/`, refuse unless sha256
  matches, unpack into `xml/build/xmlts/` (fresh directory each
  time), write nothing else. Manifest = `shasum -a 256` over every
  unpacked file, sorted by path; `PROVENIENTIA.md` records URL, date,
  tarball sha256, file count, and the licence finding with James
  Clark's sentence quoted.
- [x] **Step 2: The catalogue reader** — port the throwaway counter
  (scratchpad `xmlts_scripta/catalogus.py`) into
  `xmlts_catalogus.py`: resolve the 21 sub-catalogues by their entity
  declarations, wrap fragment catalogues (sun) in a root, apply the
  selection of spec §I, assign exclusions. Re-measure and record the
  counts (expected 2,001: not-wf 1,017 · valid 728 · invalid 229 ·
  error 27; 387 outputs). Read five `out/` files of each form and
  write the canonical-form rules into PROVENIENTIA (attribute order,
  escapes, what is dropped, the NOTATION DOCTYPE of the second form).
- [x] **Step 3: The expat harness** — `expat_canonicum.py <via>`:
  pyexpat with `SetParamEntityParsing(XML_PARAM_ENTITY_PARSING_NEVER)`
  and no external entity handler; on success print the canonical
  form, on error `INVALIDUM: <line>:<col> <msg>` and exit 1.
  `xmllint --noout --nonet <via>` gives libxml2's verdict.
- [x] **Step 4: Measure the oracles against the suite** — over the
  selection: expat's and libxml2's accept/reject against each case's
  TYPE, expat's canonical output against each `out/` file. Record
  both agreement rates and every disagreement in PROVENIENTIA — the
  credibility of oracle 2, measured before we depend on it.
- [x] **Step 5: The wild corpus** — find `*.xml *.svg *.plist *.xhtml
  *.xsd *.xsl` under `~/Documents/projects` (excluding `build/`,
  `node_modules/`, worktree duplicates by hash) and `/opt/homebrew`,
  plus text plists under `/System/Library/CoreServices`; keep files
  that start (after an optional BOM) with `<`; deduplicate by sha256.
  Record count, bytes, largest, and **how many are not UTF-8** (spec
  AUDIENDA, X8). Manifest = `sha256  path`.
- [x] **Step 6: Commit** (no C yet).

```python
silva.commissio("xml: materia oraculi - xmlts20130923 per arcessitorem (sha256; licentia non sinit redistributionem: manifestum solum), selectio MMI casuum (XML 1.0 ed. V, NS 1.0), generator formae canonicae per expat, concordia expat/libxml2 mensurata; corpus silvestre (manifestum)\n\nCo-Authored-By: ...",
    ["xml/xmlts_arcessere.sh", "xml/probationes/fixa/xmlts.manifestum",
     "xml/probationes/fixa/xmlts/PROVENIENTIA.md",
     "xml/instrumenta/xmlts_catalogus.py", "xml/instrumenta/expat_canonicum.py",
     "xml/probationes/fixa/silvestria.manifestum", "xml/CLAUDE.md"], [])
```

**Executed 2026-10-30 (615a5ee8).** Selection 2,001 as predicted;
exclusions by cause in `selectio.tsv` (codificatio 51, sine-spatiis 9,
entitas-externa 60, exitus-externus 67) → 1,861 verdicts + 308 outputs
judged. Deviations: the selection TSV is COMMITTED as data under
`fixa/xmlts/` (IDs and paths only, no suite content) so the C gate
needs no Python; every data file carries `DATA` on line 1, not
`GENERATUM` (the generata gate would demand a producing action, and
these depend on a download or on this machine's disk); manifest
hashing batched in one `shasum` (one per file took minutes). Oracle
credibility: expat 1,548/1,861 — all misses its editions 1–4 name
rules; libxml2 1,837/1,861 — namespace errors not fatal, plus one
external-entity case; expat canonical output 308/308 after placing the
second form's DOCTYPE at its document position. Wild corpus 7,931
unique files (333 MB, 123 not UTF-8); the manifest is 2.0 MB.

---

## Task P2: Scaffold — declaration, generated registry, lexicon, runner, registrum gate

**Files:**
- Create: `xml/grammatica/xml.registrum.stml`; generate
  `xml/fontes/xml_registrum_coctum.{h,c}`
- Create: `xml/fontes/xml_registrum.h` (slot enums),
  `xml/fontes/xml_lexicon.{h,c}`
- Create: `xml/compile_probationes.sh` (from toml's runner),
  `xml/probationes/probatio_xml_registrum.c`
- Modify: `pythonica/silva.py` (`PORTAE['xml']`, `FORMAE['xml']`,
  `SUITAE['xml']`), `xml/CLAUDE.md`

- [x] **Step 1: Measure the vocabulary against spec §IV** — every
  production of XML 1.0 §2–§4 that the suite's not-wf cases exercise
  has a home; a genus is added only with a cause in the commit.
- [x] **Step 2: The declaration** — genera in this order, loci in
  byte order (final Latin names checked by the lint first):
  `documentum` (liberi, cauda) · `declaratio-xml` (tok_apertura,
  pseudo-attributa, tok_clausura) · `doctype` (titulus, id_externum,
  subsetum, tok_clausura) · `subsetum` (tok_apertura, liberi,
  tok_clausura) · `declaratio-elementi` · `declaratio-attributorum` ·
  `definitio-attributi` · `declaratio-entitatis` ·
  `declaratio-notationis` · `referentia-parametri` · `elementum`
  (tag_initialis, liberi, tag_finalis — tag_finalis absent for an
  empty-element tag) · `tag` (tok_apertura, titulus, attributa,
  tok_clausura) · `attributum` (titulus, tok_signum, valor) · `valor`
  (partes: text and references, quotes) · `textus` · `referentia` ·
  `cdata` · `commentarium` · `instructio` · `malum`. `absentia=`
  messages for missing `>`, `=`, a value, an end tag.
- [x] **Step 3: Generate**, recens check, read the generated header.
- [x] **Step 4: Slot enums** (`xml_registrum.h`), one per genus.
- [x] **Step 5: The lexicon** — token genera by mode (spec §III):
  content (`TEXTUS`, `REFERENTIA_ENTITATIS`, `REFERENTIA_DECIMALIS`,
  `REFERENTIA_HEXADECIMALIS`, `CDATA`, `COMMENTUM`, `INSTRUCTIO`),
  tag (`TAG_APERTURA` `<`, `TAG_FINIS_APERTURA` `</`, `TITULUS`,
  `SIGNUM`, `VALOR_GEMINUS`, `VALOR_SIMPLEX`, `TAG_CLAUSURA` `>`,
  `TAG_VACUA_CLAUSURA` `/>`), declarations (`DOCTYPE_APERTURA`,
  `DECLARATIO_APERTURA` per keyword, `REFERENTIA_PARAMETRI`,
  `SUBSETUM_APERTURA`/`CLAUSURA`, keywords and literals), trivia
  (`SPATIUM` in tags and declarations only, `LINEA`), `IGNOTUM`.
- [x] **Step 6: The runner** — copy toml's, rename; `RADIX_FONTES`
  gains `utf8`.
- [x] **Step 7: The failing registrum probatio** — transcribe
  `probatio_toml_registrum.c` (lexicon order, genus order, recens
  gate, loci contiguity, slot enums against titles, munus facts, one
  minimal tree `<a/>` through writer → reader → writer byte-equal).
- [x] **Step 8: Register in pythonica now.**
- [x] **Step 9: Red, then green; plants** (two lexicon rows swapped;
  one generated byte).
- [x] **Step 10: Words, format, commit** (portae `xml`, `pythonica`).

**Executed 2026-10-30.** Vocabulary re-cut against html's client, which
already solved XML-shaped markup: `<titulus` and `</titulus` are ONE
token each (XML forbids space after `<`), `>` and `/>` their own; the
`elementum` genus carries tok_apertura · attributa · tok_finis · liberi
· tok_clausura · tok_clausura_finis (html's shape) — no separate tag
genera, and NO `absentia` on the end tag: an empty element (`/>`)
legitimately has none, so a missing or mismatched end tag is the
builder's diagnostic (WFC: Element Type Match), not a derived one.
DTD declarations are ONE genus `declaratio` (opening token + list of
part tokens + `>`); `xml_dtd` reads their meaning (X2). Whitespace in
content AND in prolog/epilog is `textus` (the lexer never tracks depth;
cooking skips whitespace-only top-level text and refuses the rest);
trivia = `SPATIA` only, inside tags and declarations; comments and PIs
are content. Result: **15 genera, 33 loci; 25 token genera** (prefix
`xml-`; one REFERENTIA for `&name;`/`&#N;`/`&#xH;`, html's choice;
munus LINEA absent). Wiring found by reading toml/html as built, not
the toml plan (the runner world moved since): `aedilis.stml` gains
`xml/fontes` NOW (each probatio is a `bin/fabrica` action with an
aedilis closure), `fabrica.stml` gains `<subsystema via="xml"/>`, the
root `aedificatio.stml` gains action `registrum_xml`, `xml/
aedificatio.stml` declares `probationes_xml`, pythonica gains PORTAE /
FORMAE / PORTAE_GRADUUM / SUITAE / mensor prefix. `bin/fabrica` had to
be rebuilt in-tree first (`./tools/fabrica_struere.sh` — build only,
no install). Gate green first run: 236 assertions. Plants: two lexicon
rows swapped → red; COMMENTARIUM made trivium → red; a byte in the
generated table → **stayed green, by design**: `bin/fabrica` regenerates
the table from its declaration (`SANATUM registrum_xml`) before the test
runs, so the rancor check now guards declaration/commit drift and the
plant moved to the DECLARATION (a slot renamed → red on the named-slot
table); all reverted green, tables recens.

---

## Task P3: `xml_nomina` — the character and name tables

**Files:** Create `xml/fontes/xml_nomina.{h,c}`,
`xml/probationes/probatio_xml_nomina.c`

**Interfaces:** `b32 xml_runa_licita (s32 runa)` (§2.2 `Char`),
`b32 xml_runa_initialis (s32 runa)` (`NameStartChar`), `b32
xml_runa_tituli (s32 runa)` (`NameChar`), `b32 xml_runa_pubid
(s32 runa)` (`PubidChar`), `b32 xml_spatium (s32 runa)` (`S`). Pure,
total, no allocation.

- [x] **Step 1: Measure** — transcribe the ranges from XML 1.0 5th ed.
  §2.2 and §2.3 into the test first, each range's both ends and both
  neighbours outside (e.g. `#xC0` in, `#xD7` out, `#x2070`..`#x218F`).
- [x] **Step 2: Red, then the tables** (sorted range arrays, binary
  search), green.
- [x] **Step 3: Plants** — one range end off by one → red at that
  boundary.
- [x] **Step 4: Commit** (porta `xml`).

**Executed 2026-10-30.** `xml_nomina.{h,c}`: Char, NameStartChar,
NameChar, PubidChar, S, plus `xml_titulum_metiri` (byte length of a
Name or Nmtoken at a position, over `utf8_decodere`; invalid UTF-8
ends the name) — the lexer's one entry point. Test: 72 code points at
every range end and outside neighbour, transcribed from the spec text
separately from the table. Plants (all red): a NameStartChar range end
+1, a NameChar range end +1, Char upper bound +1, the first-rune rule
removed. Lint: `pubid`, `nmtoken` entered in the glossary as XML
terms; the second table is `SEQUENTES`.

---

## Task P4: The lector — modes, every byte a token

**Files:** Create `xml/fontes/xml_lector.{h,c}`,
`xml/probationes/probatio_xml_lector.c`

**Interfaces:** `XmlLector` over a `chorda`, `xml_lector_proximum`
returning a materia token; the mode is internal state chosen by the
lexer itself (X5); UTF-8 validated with `utf8.h`, a bad byte or a
non-`Char` rune is an `IGNOTUM` token at its byte.

- [x] **Step 1: Measure** — list every mode transition of spec §III
  as a table in the test (from-mode, bytes, token, to-mode) before
  writing the lexer.
- [x] **Step 2: Hand cases** — one per transition, plus Review Focus 1
  (`]]>`, `--`, `<?xml` placement, `<` in a value, BOM).
- [x] **Step 3: Byte coverage over the suite and wild corpus** — for
  every file: tokens are contiguous, cover every byte, and their
  concatenation is the input. Suite absent → SKIP-WITH-COUNT.
- [x] **Step 4: Plants** (a mode that swallows one byte too many; the
  `]]>` rule removed) → red.
- [x] **Step 5: Commit.**

**Executed 2026-10-30.** `xml_lector.{h,c}`: six modes (CONTENTUM, TAG,
DECLARATIO_XML, DOCTYPE, PARS_INTERNA, DECLARATIO) chosen by the lexer
itself; recovery = `<` in TAG/DECLARATIO_XML/DOCTYPE returns to content
and relexes (unclosed tag), `<` or `]` in DECLARATIO returns to the
internal subset; an unclosed attribute value stops before `<` (in DOCTYPE
and DTD declarations it runs to the closing quote: entity values may
carry `<`); comments, PIs, CDATA are whole tokens up to their closer or
EOF. Character legality is NOT judged here (a later pass). Shared test
helper `xml/probationes/xml_corpus_ambulare.{h,c}` (W3C selection +
wild manifest, sha256-checked, skipped with count; `XML_CASUS` = 61
inline cases incl. the 2026-10-18 audit cases). Gate: transition table
(27 cases) + totality over 61 inline, 2,001 W3C and 7,921 wild files —
49.4 M tokens, contiguous, every byte, line/column exact; 10 s. Found
while running: an empty file lexes against a static `""` (pointer check
skipped for empty input); keeping every token of the 35 MB expat test
file blew the 256 MiB section ceiling → the test lexes into a scratch
pool emptied every 4,096 tokens. `casus` and `L` are latina.h macros
(renamed). Plants (all red): `]]>` rule removed, value not stopping at
`<`, line counting removed, XML declaration accepted anywhere, unclosed
tag not recovered. Glossary: `xmlts`.

---

## Task P5: The builder — prolog, elements, content, DOCTYPE shape, mala, recovery; byte law

**Files:** Create `xml/fontes/xml_arbor.{h,c}`,
`xml/probationes/probatio_xml_arbor.c`

- [x] **Step 1: Measure** — run the lexer over the selection and
  count the token sequences the builder must place (top-level
  misc, DOCTYPE positions, element nesting depth max, CDATA, PIs).
- [x] **Step 2: The builder** — iterative over an explicit stack
  (toml_arbor's shape): prolog (declaration only at byte 0, DOCTYPE
  at most once before the root, misc), one root element, end-tag
  matching (mismatch → the `WFC: Element Type Match` diagnostic with
  the start tag related, recovery by popping to the matching open
  element if any), the DOCTYPE and internal subset as STRUCTURE only
  (declarations are typed nodes; their meaning is P8), `malum` for
  the rest.
- [x] **Step 3: Byte law** — `emit(parse(x)) == x` for every
  hand case, every selected suite file (well-formed or not) and
  every wild file. Totality: no byte outside a token, no token
  outside a node.
- [x] **Step 4: Absent-slot rows** — unclosed tag, missing `=`,
  missing end tag at EOF, second root element: each a named
  diagnostic at the right offset.
- [x] **Step 5: Plants** — end-tag matching disabled; one token
  dropped by the builder → red (byte law names the file).
- [x] **Step 6: Commit.**

**Executed 2026-10-30.** `xml_arbor.{h,c}`: iterative frame stack
(DOCUMENTUM, TAG, CONTENTUM, TAG_FINALIS, DECLARATIO_XML, DOCTYPE,
PARS_INTERNA, DECLARATIO, MALUM_TAG); nodes appended to their parent at
creation; a frame that cannot take a token closes and hands it down
("slot-order law": a token arriving after a later slot is filled closes
the node and lands in the parent, so emission stays in byte order);
contiguous unplaceable tokens merge into one `malum`. End tags search
the contiguous run of open elements: top = end tag, deeper = the
intermediates close with `elementum/tag-finalis-deest`, none = stray
end tag in a malum (`elementum/tag-finalis-solus`) swallowing to its
`>`. An unclosed start tag leaves the element OPEN (the `>` absence is
derived from the declaration). Document order judged after building
(`documentum/radix-deest|radix-altera|doctype|textus|contentum`).
Diagnostics the builder emits go in `XmlParsura.diagnostica`
(MateriaDiagnosticum, for `materia_diagnostica_derivare`'s `emissa`).
Ligator: all SPATIA attach BEFORE the next token. Gate: 18 structure
cases (children, mala, emitted codes, sana), element details, the byte
law over 61 inline + 2,001 W3C + 7,917 wild files — **zero failures
on the first run** — and 100,000-deep nesting parsed (10,000 emitted).
Measured on the way: the tree costs 13–36× the source for typical
files (2× for expat's 10 MB single-construct stress files), but
douay-rheims.usfx.xml costs 60× (16.7 MB → 1.0 GB) — over the 1 GiB
section ceiling; the byte-law gate skips files above 12 MB BY NAME
(four today; the lexer gate still reads them) and P14's computus owns
the per-byte cost. Plants (all red): end-tag name not compared, trivia
not attached, a stray-end-tag token dropped, second root not reported,
intermediates not closed — the last survived first (the code was
checked, not the element it named) and the test now asserts the
diagnostic's offset and which element took the end tag.

---

## Task P6: STML projection — two cycles, the comparator

Transcribe toml Q5: write, read, write; comparator on the re-read
tree; the `cr` attribute for CR bytes; every hand case and the
selection. Commit.

**Executed 2026-10-30.** `probatio_xml_stml.c` transcribed from toml's
(two cycles, structural + fidelity comparator, reread tree emits the
source, sedes verified, separation pin). First run: every wild file
round-tripped, but 10 W3C not-wf cases were REFUSED by the substrate
writer ("valor lexematis textui non tutus (mixtum)"): an unclosed
literal in a DOCTYPE/DTD declaration runs to EOF and ends in a newline;
a value with newline-bearing edge whitespace can only be written raw,
and a token carrying trivia cannot be raw. Fix in the BUILDER, not the
substrate: the ligator now hangs pending trivia AFTER the prior token
when the next token's value is unsafe (`_tutus`, mirroring
`materia_arbor.c _textus_tutus`) — 2,001/2,001 W3C, 7,682/7,682 wild.
Cost: the full 1 MB-capped run took 525 s and 1.9 GB of STML, so wild
files above 128 KB are skipped WITH COUNT (239; 97% of the wild corpus
kept; the W3C suite is never capped); 122 s, section marked `gravis
lentus`. Plant: the post-trivia rule removed → the ten refusals return
(red).

---

## Task P7: `xml.canon` — hand-written, drift-guarded, seal pinned

Transcribe toml Q6. Commit.

**Executed 2026-10-30.** `xml/grammatica/xml.canon` (76 rules: the
involucrum, two trivia, 15 genera, 33 slots, 25 token kinds) — first
form written by a throwaway script from the registry and the builder's
allowed children per slot, then maintained by hand like toml's; the
seal pinned is the live `86121710`. `probatio_xml_canon.c` transcribed
(drift guard both ways, seal pin, judgment of every inline case, all
2,001 W3C cases and the wild files up to 128 KB — 239 larger skipped
with count, as in P6): **zero violations on the first judged run**
(the only miss was the placeholder seal). 39 s; corpus section `gravis
lentus`. Plants (all red): a token rule renamed, the pinned seal
changed, an allowed child of `documentum/liberi` renamed, `xml-valor`
removed from `doctype/id_externum`.

---

## Task P8: `xml_dtd` — the internal subset as tables

**Files:** Create `xml/fontes/xml_dtd.{h,c}`,
`xml/probationes/probatio_xml_dtd.c`

**Interfaces:** `XmlDtd` built from a `doctype` node: general and
parameter entities (internal replacement text / external id /
NDATA notation), attribute-list definitions per element (type,
default kind, default value), notations, element declarations (kept,
not validated, X11). Every table entry points at its declaration
node.

- [ ] **Step 1: Measure** — count in the selection: declarations by
  kind, PE references in the internal subset, external PEs, cases
  carrying `standalone=`.
- [ ] **Step 2: Entity values** — character references expanded at
  declaration (Review Focus 3), PE references inside entity values in
  the internal subset refused (`WFC: PEs in Internal Subset`), first
  declaration wins (later ones ignored, not an error).
- [ ] **Step 3: Internal PEs between declarations** — replacement
  text parsed as declarations, recursion refused (`WFC: No
  Recursion`), X4 limits.
- [ ] **Step 4: The §5.1 stop rule** (Review Focus 4) — pinned by
  `xmltest/valid/sa/097.xml` and a hand case each way
  (`standalone="yes"` / absent).
- [ ] **Step 5: Plants** (stop rule removed; first-declaration-wins
  reversed) → red.
- [ ] **Step 6: Commit.**

---

## Task P9: The cooked view — references, defaults, normalization, the WFCs, entity markup, the limits

**Files:** Create `xml/fontes/xml_coctum.{h,c}`,
`xml/probationes/probatio_xml_coctum.c`

- [ ] **Step 1: Measure** — the WFCs of XML 1.0 one by one: which
  builder or cooking step owns each; a table in the test file.
- [ ] **Step 2: Cooking** — elements, attributes in order (`WFC:
  Unique Att Spec`, `WFC: No < in Attribute Values`, `WFC: No
  External Entity References` in values), text with references
  expanded (`WFC: Legal Character`, `WFC: Entity Declared` with the
  standalone nuance, `WFC: Parsed Entity`), CDATA marked, line ends
  normalized, attribute normalization (Review Focus 2), defaults
  applied and marked as defaulted.
- [ ] **Step 3: Entity replacement with markup** — parse the
  replacement text as content with the same lexer/builder, located
  in the declaration; balanced or refused; X4 limits measured on a
  billion-laughs file (set the numbers so every suite and wild file
  passes with margin; record them in the spec).
- [ ] **Step 4: Unread external entities** stay named unexpanded nodes
  (X3).
- [ ] **Step 5: Plants** — one WFC switched off at a time → red on
  its own case.
- [ ] **Step 6: Commit.**

## Task P10: Namespaces 1.0

**Files:** Modify `xml_coctum`; create
`xml/probationes/probatio_xml_spatia.c`

- [ ] Scoping of `xmlns` / `xmlns:p`, the reserved prefixes, `NSC:
  Prefix Declared`, no prefix undeclaring (1.0), duplicate expanded
  attribute names, element and attribute names with more than one
  colon; pinned by the eduni `ns10` cases and hand cases; plants;
  commit.

---

## Task P11: The W3C oracle — `xml_oraculum`, pinned rising

**Files:** Create `xml/fontes/xml_oraculum.{h,c}`,
`xml/probationes/probatio_xml_oraculum.c`

- [ ] **Step 1: The canonical writer** — both forms from P1 Step 2.
- [ ] **Step 2: The gate** — over `selectio.tsv`: not-wf rejected,
  valid and invalid accepted, outputs byte-equal; counts printed per
  class and pinned (rising only); every exclusion counted by cause;
  `XMLTS_OMNIA=1` prints every failure, `XMLTS_CASUS=<ID>` runs one.
- [ ] **Step 3: Climb** — fix by cause, one cause per commit, until
  every non-excluded case passes or its failure is named in the spec
  with a reason. Expect re-slicing here (html's oracle climbed over
  seven tasks).
- [ ] **Step 4: Commit** per climb.

**Executed 2026-10-30 — P8, P9, P10 and P11 as ONE slice (re-slice,
recorded here).** The four are not separable in practice: the W3C count
only means something once the DTD, cooking, namespaces and the
canonical writer all exist, and the cheapest feedback loop was the
oracle itself. Modules: `xml_coctum.{h,c}` (one coctor: token checks,
XML declaration, DOCTYPE, the four DTD declaration grammars with a
token cursor and byte-gap whitespace checks, entity tables with the
§5.1 stop rule, content cooking with entity expansion through
`xml_arbor_parsare_fragmentum` — new: content and internal-subset
fragment modes in lexer and builder — attribute normalization, defaults,
namespaces; every walk iterative), `xml_oraculum.{h,c}` (both canonical
forms; DTD PIs become document children before `locus_typi`), gates
`probatio_xml_oraculum.c` (pinned rising) and `probatio_xml_coctum.c`
(what the suite does not stress). **First oracle run: 921/922 not-wf
rejected, 939/939 accepted, 304/308 outputs.** Causes fixed one by one:
replacement text must NOT be line-normalized again (a CR from `&#13;`
in an entity value); PIs in the internal subset are output before the
DOCTYPE; a colon in a notation name (NS 1.0); `locus_typi` was `i32` —
unsigned in this house, so the `-1` sentinel never compared below zero.
The 9 `sine-spatiis` cases are now JUDGED with namespaces off
(`xml_coquere(…, FALSUM)`) instead of excluded. **Result, pinned:
not-wf 922/922, valid+invalid 948/948, outputs 309/309**; excluded only
44 non-UTF-8 (X8) and 60 errors hidden in unread external entities (X3).
The cooking test found the billion-laughs cost: each expansion parsed a
fresh fragment tree (2.3 GB) — each entity's fragment is now parsed ONCE
and cached; limits X4 = 64 levels, 16 MiB expanded text. Plants (all
red): `--` in comments, duplicate attributes, the §5.1 rule, WFC Entity
Declared, CR LF collapse in attribute values, undeclared prefixes,
canonical CR escape, fragment text re-normalized, ELEMENT grammar
switched off. Glossary: `qname`.

---

## Task P12: The expat/libxml2 differential and the wild corpus

- [ ] Over the selection and the wild corpus: our verdict vs expat
  and libxml2, our canonical form vs expat's; every disagreement
  named (our bug, a documented divergence, or an oracle bug where
  the spec text decides). Wild byte law is a hard gate. Commit.

---

## Task P13: The public API and diagnostics rendering

**Files:** Create the public header (name chosen here so it does not
collide with `include/xml.h` while that lives — e.g.
`xml/fontes/xml_documentum.h`), its implementation over `xml_coctum`,
`xml/probationes/probatio_xml_api.c`

- [ ] The API of spec §V (names polished; lint first); diagnostics
  rendered by `materia_pictor_scribere`; edge cases (empty document,
  BOM only, missing attribute, non-element children) return NIHIL /
  FALSUM, never crash. Header text shown to Fran before
  implementation. Commit.

**After P13 the MusicXML spec can start in parallel with P15.**

## Task P14: Totality and the computus twin

Transcribe toml Q11: generated fuzz cases (truncations, byte flips,
deep nests), depth pins, the billion laughs, the arena columns of
the twin pinned against a golden. Commit.

---

## Task P15: Migration — every consumer, then retire `lib/xml.c`

**Files:** Modify `lib/plist.c`, `lib/horreum.c`, `lib/codex.c`,
`tools/pecunia_monetae_generare.c`, `probationes/probatio_xml.c` (moves
or retires), `probationes/horreum_simulacrum.h`, the six
`probationes/fixa/sondae_auditus/xml_*.c`; delete `lib/xml.c`,
`include/xml.h`; aedilis client roots; silex client roots (one list
in two homes, toml §X).

- [ ] **Step 1: Measure strictness on real inputs** (spec §VIII risk)
  — the new parser over each consumer's real inputs: every text
  plist on disk, horreum's stored XML, every EPUB codex can open,
  pecunia's data file. Any rejection is studied before the consumer
  moves.
- [ ] **Step 2: plist's writer** — decide with Fran: plist writes its
  element kinds directly, or the writer half survives as a small
  library (spec §I).
- [ ] **Step 3: One consumer per commit**, each with its own tests
  green and its own gates.
- [ ] **Step 4: The fixtures** — the `sondae_auditus` files are
  examen-corpus inputs: re-point or retire them with the examen
  gate's owner (ask Fran if the owner is unclear).
- [ ] **Step 5: Retire** — `./silva/nexus.sh xml_legere` shows zero
  uses; delete; `bin/fabrica sanare` every producer whose closure
  changed; aedilis gate proves the closures.

## Task P16: briar exposure

Transcribe toml Q13: the corpus lists the library, the header shows,
changelog line marked FRANGIT with the replacement calls. Commit.

## Task P17: Closure — records, ledger, memory

As-built §X in the spec (deltas, numbers, AUDIENDA answered),
`xml/CLAUDE.md` final, phase-log entry in `materia/phase-log.md`,
ledger: the MusicXML desideratum unblocked, a closed desideratum for
this library, memory pointer. Commit.

---

## Self-Review (2026-10-29, before execution)

- **Spec coverage:** X1 (P11 selection), X2 (P8), X3 (P1 harness
  flags, P9 Step 4), X4 (P9 Step 3, P14), X5 (P4), X6 (P5), X7 (P9),
  X8 (P1 exclusions, P4 declaration check), X9 (P1, P11, P12), X10
  (P15), X11 (nothing built), X12 (P2 PORTAE, P15 aedilis, P16).
- **Biggest uncertainty:** P11's climb. html needed seven oracle
  tasks to go from 887 to 1,504 of 1,700; XML has no recovery
  algorithm to imitate, so the climb should be shorter, but the DTD
  corner cases (IBM's 731 not-wf cases are mostly productions of
  §2.8–§4.3) may re-slice P8.
- **Order choice:** the oracle material comes first (P1), as in
  toml, so every later task has its measurement ready; the cooked
  view waits for the tree laws (P5–P7) so a cooking bug is never
  confused with a byte-law bug.
