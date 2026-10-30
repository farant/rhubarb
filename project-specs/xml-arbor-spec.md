# xml_arbor — XML as a materia client, the substrate's eighth

*2026-10-29. Decisions approved by Fran the same day (X8 UTF-8 only,
X10 migrate everything in this plan, the suite fetched). Born in sexta while bringing
the first polyphony into the house book (Tallis, O nata lux, from a
CPDL MusicXML file): Fran asked whether the MusicXML reader should
stand on "a proper materia based parser library and be a more
thorough xml library maybe with some kind of oracle test suite from
something else we can use to drive coverage". Eighth materia client
after the C89 shim, css, md, oratio, html, crusta and toml; this spec
copies toml's shape (`project-specs/toml-arbor-spec.md`, as built
§X) wherever the language allows. The first consumer is the MusicXML
reader, desideratum 01M64ZZYQ59DY0G0104SRTK439.*

**What it is.** A parser for XML 1.0 (Fifth Edition) with Namespaces
1.0 as a materia tree — total, byte-exact through the emitter for
every input, well-formed or not — plus a **cooked view** (elements,
attributes, text with references expanded, namespaces resolved) that
consumers query, located diagnostics for every error in a file, an
STML projection, a canon and a benchmark twin, proven against three
oracles: the W3C XML Conformance Test Suite, two independent parsers
already on this machine (libxml2 and expat), and a wild corpus of
real files. It replaces `lib/xml.c` and `include/xml.h`; their four
consumers migrate to the new API (§VIII).

**Why "xml".** Like css, md, html and toml, the client keeps the
format's own name.

---

## 0. Decisions

| # | decision | rationale |
|---|---|---|
| X1 | **Non-validating XML 1.0 (5th ed.) + Namespaces 1.0**, judged by the W3C suite's XML1.0 and NS1.0 cases — not-well-formed cases rejected, valid AND invalid cases accepted (the suite's matrix for non-validating processors) | Rejecting bad input is half the spec (toml T1). Validation against a DTD is a separate, optional layer that no consumer has asked for (PULL rule). |
| X2 | **The internal DTD subset is parsed in full** — element, attribute-list, entity and notation declarations, parameter-entity references between declarations, conditional sections refused there as the spec says | Well-formedness depends on it: undeclared entities, attribute defaults, normalization of declared non-CDATA attributes, and most of the suite's not-wf cases live in the DOCTYPE. Today's `xml.c` skips the DOCTYPE and refuses an internal subset outright. |
| X3 | **External entities are recorded, never fetched** — not the external DTD subset, not external parsed entities, not by any flag. A reference to an unfetched external entity stays a named unexpanded node in the cooked view | A non-validating processor may skip them (XML 1.0 §5.1). Fetching is the classic XXE hole, and every consumer reads local files. |
| X4 | **Entity expansion is bounded**: nesting depth and total expanded size have limits (numbers set in the plan from measurement); exceeding them is a named error, never a crash or a hang | The "billion laughs" document is a well-formed XML file; a total parser must survive it. |
| X5 | **Hand-written lexer with modes, the toml/crusta shape, not an LR table.** Modes: content, tag, attribute value, comment, processing instruction, CDATA section, DOCTYPE, internal subset, declaration | XML was designed to be tokenized without feedback from the parser: `<` and `&` decide everything in content, quotes inside tags. The mode is chosen by the lexer itself (crusta C6: nothing relabelled afterwards). |
| X6 | **Totality**: every byte lands in the tree; what the grammar cannot place is a `malum` node; the builder recovers and keeps going, so **every** error in a file is reported (toml T4) | XML 1.0 §1.2 lets a processor continue looking for errors after a fatal error, but forbids passing data on normally — so the tree is total and the diagnostics complete, while the cooked view (X7) is refused for a not-well-formed document. |
| X7 | **The cooked view exists only for a well-formed document.** It is where references are expanded, attribute defaults applied, attribute values normalized and namespaces resolved; the tree keeps the author's bytes | Matches the spec's fatal-error rule and keeps the two layers honest: the tree is what was written, the cooked view is what it means. Entity replacement text containing markup is parsed into the cooked view with its location in the declaration (§V). |
| X8 | **UTF-8 only in v1** (with or without BOM; US-ASCII is UTF-8). A document declaring or carrying another encoding (UTF-16 BOM, `encoding="ISO-8859-1"`) gets one named diagnostic. Those suite cases are named exclusions, counted | Every consumer file is UTF-8 (MusicXML, plist, EPUB, the wild corpus — measured in the plan). Transcoding before lexing would break the byte law unless designed for; it is a named future (§IX), not a hidden hole. **Fran, 2026-10-29: "only utf8 for now".** |
| X9 | **Three oracles** (§VII): W3C suite pinned by sha256 and fetched by a script (its licence forbids redistribution, §I), libxml2 + expat differential (both local), wild corpus byte law (local, manifest only) | The shape of toml T9; the two differential parsers come free with macOS. Fran allowed the fetch, 2026-10-29. |
| X10 | **New API over the cooked view; consumers migrate; `lib/xml.c` and `include/xml.h` retire** — as the last phase of the plan, after the oracles are green | toml T2. Names during the build must not collide with `include/xml.h` (`XmlNodus`, `xml_legere`, …); the plan picks them. **Fran, 2026-10-29: "migrate everything now".** |
| X11 | **Out of scope: XML 1.1, DTD validation, writing XML from a cooked view, XPath/XSLT/Schema** | Named futures (§IX). The byte-faithful tree is what makes format-preserving edits possible later. |
| X12 | **Wiring is part of the deliverable**: aedilis learns `xml/fontes`; gate `xml` in pythonica PORTAE; briar's corpus exposes the library | toml T11; a runner outside the gate table is dead (materia memory, B10). |

---

## I. The census (2026-10-29)

**The library being replaced.** `include/xml.h` (285 lines) +
`lib/xml.c` (1,753). DOM-style tree, interned strings, piscina
allocation. Its own header says: "Supportat: elementa, textus,
commenta. Non supportat: namespaces, DTD, CDATA" (`include/xml.h:14`).
A DOCTYPE is skipped and an internal subset refused
(`XML_ERROR_PARS_INTERNA`, `include/xml.h:51`; commit 351ab229).
It also has a builder and writer (`xml_elementum_creare` …
`xml_scribere_ad_aedificator`, `include/xml.h:187-279`).

**Consumers** (`./silva/nexus.sh xml_legere`: 19 uses in 13 files):

| file | use |
|---|---|
| `lib/plist.c:970` | XML property lists (binary `bplist00` refused before) |
| `lib/horreum.c:2049` | XML inside stored documents |
| `lib/codex.c:128` | EPUB container index and OPF (chapters go through `html_lexema`) |
| `tools/pecunia_monetae_generare.c:204` | currency data generator |
| `probationes/probatio_xml.c`, `probationes/horreum_simulacrum.h` | the library's test and a horreum test helper |
| `probationes/fixa/sondae_auditus/xml_*.c` (6 files) | audit-probe fixtures (examen corpus) |

The writer half (`xml_*_creare`, `xml_scribere`) has exactly one
consumer outside its own test: `lib/plist.c` writes plists with it
(`xml_elementum_creare` ×10 from line 381, `xml_scribere` at 563;
`./silva/nexus.sh`). Writing is out of scope here (X11), so before X10
retires `xml.c` the plan decides: plist writes its few element kinds
directly, or the writer survives as a small library of its own.

**MusicXML needs** (the first consumer, PULL): elements, attributes,
text, a DOCTYPE with a public identifier and no internal subset
(Finale/MuseScore/CapXML exports), UTF-8. The `.mxl` container is a
ZIP with `META-INF/container.xml` — the same layout as EPUB, already
read by `include/sarcina.h` (ZIP, stored + DEFLATE via `flatura`) for
codex. So MusicXML adds no archive work.

**Oracle material on this machine.**
- `/usr/bin/xmllint` — libxml2 2.9.13 (`xmllint --version`: 20913).
- Python `pyexpat` — expat 2.7.4.
- Real files (`find … -name '*.EXT' | wc -l`, not yet deduplicated):
  `~/Documents/projects` — .xml 4,014 · .svg 2,852 · .plist 1,654 ·
  .xhtml 479 · .xsd 243 · .xsl 47; `/opt/homebrew` — .xml 1,186 ·
  .plist 175 · .svg 143 · .xsl 11 · .xsd 1. Text plists also under
  `/System/Library` (215 of the first 2,000 in CoreServices begin
  `<?xml`).
- **The W3C suite, fetched 2026-10-29**: `https://www.w3.org/XML/Test/
  xmlts20130923.tar.gz`, 641,522 bytes, sha256 `9b61db9f5dbffa54
  5f4b8d78422167083a8568c59bd1129f94138f936cf6fc1f`; 3,697 tar
  entries, 15 MB unpacked. Catalogue `xmlconf/xmlconf.xml` pulls 21
  sub-catalogues by external entity: **2,585 cases** (not-wf 1,498 ·
  valid 812 · invalid 242 · error 33), 432 with an `OUTPUT` file.
- **Our selection** (X1): cases whose `RECOMMENDATION` is XML 1.0 (any
  erratum) or NS 1.0, `VERSION` not 1.1, and `EDITION` absent or
  containing 5 = **2,001 cases** — not-wf 1,017 · valid 728 · invalid
  229 · error 27 (optional errors: not judged); 387 with an output
  file. (Counted by a throwaway reader of the catalogue, kept for Q1.)
- **Encodings in the selection**: UTF-16 with a BOM 42 (valid 7,
  invalid 2, not-wf 33) — the X8 exclusions; Japanese and Latin-1
  declarations 9 (8 of them `error` cases, 1 not-wf); malformed
  encoding names ~20 (all not-wf — rejected either way); 14 not-wf
  cases are bad UTF-8 bytes (rejected by the UTF-8 rule, correctly).
- **Licence**: the suite has no single licence; each collection keeps
  its owner's. James Clark's `xmltest/readme.html`: "Permission is
  granted to redistribute the file xmltest.zip … provided that no
  modifications of any kind are made to this file. Note that
  permission to distribute the collection in any other form is not
  granted." So the suite is **not** stored in the repo: a manifest
  (sha256 per file) and a fetch script that checks the tarball's
  sha256 — the same rule as toml's wild corpus.
- **Two output forms**, read from the files: James Clark's canonical
  XML (`xmltest/valid/*/out/`: no DOCTYPE, attributes sorted, `&#10;`
  style escapes) and the "second canonical form" with a DOCTYPE
  listing NOTATIONs (`sun/valid/out/not-sa04.xml`). `xml_oraculum`
  writes both.

---

## II. Architecture and files

A client directory `xml/`, mirroring `toml/`:

| module | job |
|---|---|
| `xml_lector` | lexer, modes (X5); every byte is a token (whitespace, comments and newlines included) |
| `xml_lexicon` | token genera (prefix `xml-`) |
| `xml_registrum` | node vocabulary, declared in `grammatica/xml.registrum.stml`, cooked by `./materia/coquere.sh` |
| `xml_arbor` | builder: tokens → materia tree, iterative over an explicit stack; `emit(parse(x)) == x` for every x |
| `xml_nomina` | the XML Name / NameChar tables (5th edition ranges) and the character rule (`Char`, refusing C0 controls except tab/LF/CR, surrogates, U+FFFE/FFFF) — over the house `utf8` library |
| `xml_dtd` | internal subset: declarations into tables (entities, attribute lists, notations); parameter-entity expansion between declarations |
| `xml_coctum` | cooked view (X7): references, defaults, normalization, namespaces; the well-formedness constraints that are not grammar |
| `xml_oraculum` | cooked view → the suite's canonical output form |
| `xml_computus` | benchmark twin, arena columns pinned |
| `xml.h` (new name in the plan, X10) | public API (§V) |
| STML projection, `xml.canon`, exempla | as in css, md, html, toml |

Tests in `xml/probationes/`, runner `xml/compile_probationes.sh`.

---

## III. The lexer (`xml_lector`)

Content mode: character data up to `<` or `&`; `&name;`, `&#N;`,
`&#xH;` references; `]]>` in character data is an error token.
After `<`: start tag, end tag, `<!--`, `<?`, `<![CDATA[`, `<!DOCTYPE`
— each opens its own mode. Tag mode: names, whitespace, `=`, quoted
values (attribute-value mode: references and `<` refused inside),
`>`, `/>`. Comment mode refuses `--` inside and `--->` at the end.
PI mode: target (the name `xml` in any case is reserved except for
the declaration at byte 0). DOCTYPE mode: name, `SYSTEM`/`PUBLIC`
literals, `[` opens internal-subset mode: `<!ELEMENT`, `<!ATTLIST`,
`<!ENTITY`, `<!NOTATION`, PIs, comments, `%name;` references.

Names: the Fifth Edition defines `NameStartChar` / `NameChar` as a
fixed list of code-point ranges (§2.3), deliberately independent of
Unicode character properties — so `xml_nomina` is a small hand table
over `utf8.h` decoding (`utf8_decodere`, `utf8_longitudo_byte`), and
needs nothing from `runae`. Cases marked editions 1-4 only (the old
property-based name rules) are outside the selection.

Newlines: XML normalizes CR LF and lone CR to LF for the application
(§2.11) — in the cooked view only; the tree keeps CR through the
materia `cr` attribute (B6). The input is UTF-8-validated; characters
outside `Char` are error tokens at their byte.

The XML declaration (`<?xml version=… encoding=… standalone=…?>`) is
its own node with its three pseudo-attributes typed; `version` other
than `1.0` is a diagnostic (1.1 out of scope, X11); `encoding` other
than UTF-8 (case-insensitive) is the X8 diagnostic.

---

## IV. The tree (`xml_registrum`, `xml_arbor`)

Node vocabulary (sketch — final Latin names in the plan, checked
against the glossary and latina.h):

- `documentum`; `declaratio_xml`; `doctype` (name, external id,
  `subsetum_internum`);
- in the subset: `declaratio_elementi`, `declaratio_attributorum`
  (with `definitio_attributi` children: name, type, default),
  `declaratio_entitatis` (general / parameter; internal value /
  external id / NDATA), `declaratio_notationis`, `referentia_parametri`;
- content: `elementum` (start tag, content, end tag — or empty-element
  tag), `attributum` (name, `=`, value with its references),
  `textus`, `referentia` (entity / decimal / hex), `cdata`,
  `commentarium`, `instructio` (PI);
- trivia: whitespace (inside tags and declarations only — whitespace
  in content is character data), newline;
- `malum`: a span the grammar cannot place (X6).

Laws, gated as in every client: **byte law** (`emit(parse(x)) == x`
for every file, well-formed or not); **totality**; **STML round
trip** (two cycles + comparator); **totalitas** (every genus in the
fixtures); depth pinned (the builder is iterative; materia's own
emission/STML depth limits stay on park …FAD8).

---

## V. The cooked view and the API (`xml_coctum`)

Built only for a well-formed document (X7). It holds:

- **elements** — namespace URI + local name + prefix as written,
  attributes in document order, children in order; every cooked
  object points at its tree node (toml T8);
- **attributes** — normalized value (§3.3.3: whitespace to space;
  declared non-CDATA types also collapse), namespace, and whether it
  was **specified or defaulted** from an `<!ATTLIST>`;
- **text** — character references and entity references expanded,
  CDATA merged into text but marked, line ends normalized; adjacent
  text joined;
- **comments and PIs** — kept, in order;
- **entity replacement with markup** — `<!ENTITY e "<a>x</a>">` then
  `&e;` in content: the replacement text is parsed (by the same lexer
  and builder) as content and grafted into the cooked view, each
  object located in the declaration's value. It must be balanced
  (§4.3.2); recursion is an error; X4's limits apply here;
- **unread external parameter entities stop declarations** — after a
  reference to an external parameter entity that is not read (X3),
  a non-validating processor must not process later entity or
  attribute-list declarations unless the document says
  `standalone="yes"` (XML 1.0 §5.1). The suite tests it: in
  `xmltest/valid/sa/097.xml` the `ATTLIST` before `%e;` gives its
  default and the one after does not (`out/097.xml`: `a1` only);
- **namespaces** — `xmlns` / `xmlns:p` scoping, the reserved `xml` and
  `xmlns` prefixes, undeclared prefixes and duplicate expanded
  attribute names as namespace-well-formedness errors.

API sketch (names polished in the plan, not colliding with
`include/xml.h` while it lives):

```c
XmlDocumentum* xml_documentum_legere (chorda textus,
                    constans character* via, Piscina* piscina);
                                            /* numquam NIHIL */
b32   xml_documentum_successus (constans XmlDocumentum* doc);
Xar*  xml_documentum_diagnostica (constans XmlDocumentum* doc);
constans XmlElementum* xml_documentum_radix (constans XmlDocumentum* doc);
                                            /* NIHIL si non bene formatum */
chorda xml_elementi_titulus   (constans XmlElementum* e);   /* nomen localis */
chorda xml_elementi_spatium   (constans XmlElementum* e);   /* URI */
i32    xml_liberorum_numerus  (constans XmlElementum* e);
/* filius i: elementum, textus, commentarium, instructio */
b32    xml_attributum         (constans XmlElementum* e,
                               constans character* titulus,
                               chorda* valor);            /* VERUM si adest */
constans XmlElementum* xml_filius_primus (constans XmlElementum* e,
                               constans character* titulus);
b32    xml_textus_internus    (constans XmlElementum* e,
                               Piscina* p, chorda* textus);
```

The MusicXML reader is written against this API (its own spec,
after this one).

---

## VI. Diagnostics

Syntax errors from the builder ("expected …", the rejected text in
the author's spelling); constraint errors from cooking — each
well-formedness constraint of XML 1.0 and Namespaces 1.0 by its own
code, named after the spec's constraint ("WFC: Element Type Match",
"WFC: Unique Att Spec", "WFC: No Recursion", "NSC: Prefix
Declared"…), with a related location where one exists (the start tag
for a mismatched end tag, the first attribute for a duplicate). All
are `MateriaDiagnosticum`, rendered by `materia_pictor_scribere`.
Every error in the file is reported (X6).

---

## VII. Oracles and gates

1. **W3C XML Conformance Test Suite, pinned.** `xmlts20130923`,
   fetched by `xml/xmlts_arcessere.sh` into `xml/build/xmlts/` (never
   committed) after checking the tarball's sha256 (§I); the repo
   holds `xml/probationes/fixa/xmlts.manifestum` (sha256 per file)
   and `PROVENIENTIA.md` (URL, date, sha256, the licence finding).
   With the suite absent the gate SKIPS-WITH-COUNT, loudly. The gate
   runs the **XML1.0 and NS1.0 cases** from the suite's own
   catalogue: `not-wf` rejected; `valid` and `invalid` accepted
   (X1); where an output file exists, the cooked view through
   `xml_oraculum` equals it byte for byte. Pass counts pinned, only
   rising; target 100%; every exclusion named with its cause (X8
   encodings, X3 external entities whose errors only show once
   fetched, XML 1.1).
2. **libxml2 + expat differential.** A small Python harness renders
   expat's events (`pyexpat`, external entities off) into the same
   canonical form; `xmllint --noout` gives libxml2's accept/reject.
   Over the suite and the wild corpus: agreement on accept/reject and
   on the canonical form; each disagreement named — our bug, or a
   documented divergence. libxml2 2.9.13 is old, so where the two
   disagree with each other the spec text decides.
3. **Wild corpus.** The local files of §I, deduplicated by sha256,
   kept as a manifest only (no third-party files in the repo); byte
   law 100% (hard gate), accept/reject agreement with expat.
4. **Per-client suites**: registry, lexer, tree, DTD, cooked, STML,
   canon, totalitas, computus — gate `xml`, owed by paths under `xml/`.

Gates are born red by a plant that compiles.

---

## VIII. Migration and wiring

- Last phase (X10): plist, horreum, codex, the pecunia generator,
  `probatio_xml`, `horreum_simulacrum.h` move to the new API; the
  six `sondae_auditus` fixtures are examen-corpus inputs and are
  re-pointed or retired with the examen gate's owner.
- `lib/xml.c` / `include/xml.h` deleted when `./silva/nexus.sh`
  shows zero uses; the writer half's one user, plist, is settled
  first (§I).
- **Risk to measure, not assume:** the old parser was lax. Before a
  consumer moves, the strict parser runs over that consumer's real
  inputs (plists on disk, horreum's stored documents, the EPUBs codex
  has read); any rejection is studied — a tool-local lenient helper,
  as toml did for `librarium_merge`, only if the input is right and
  the strictness is not.
- aedilis learns the `xml/fontes` root; briar exposes the library;
  silex client roots updated (toml §X: one list in two homes).

---

## IX. Named futures (nothing built)

- **Other encodings** (X8): a transcoding front end (UTF-16, Latin-1)
  with offsets mapped back to source bytes.
- **DTD validation** over the parsed subset; the suite's `invalid`
  cases then become "rejected by the validating mode".
- **Writing XML** from a cooked view, and format-preserving edits
  through the tree.
- **XML 1.1** (the suite's XML1.1 and NS1.1 cases).
- **Consumers**: MusicXML (next); SVG for pictor; EPUB OPF/NCX
  through the cooked view instead of string matching.

---

## AUDIENDA (not verified when written)

Answered 2026-10-29 by the fetch (§I): the licence (no redistribution
of James Clark's collection in any other form → manifest only), the
counts (2,585 cases; 2,001 selected), the two output forms.

- Whether any wild-corpus file is not UTF-8 (X8 assumes none matter).
- The X4 limits (depth, expansion size) — set from measurement.
- Whether any materia substrate change is needed (toml and html: none;
  crusta: one). Recorded as found.
