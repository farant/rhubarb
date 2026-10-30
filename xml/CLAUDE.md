# xml — orientation

*You are reading this because you touched a file in `xml/`.*

XML 1.0 (Fifth Edition) + Namespaces 1.0 as a materia client (the
eighth). Spec: `project-specs/xml-arbor-spec.md` (decisions X1–X12).
Plan: `project-specs/xml-arbor-plan.md` (P1–P17, one task per turn
with Fran). Ledger: parcum 01M65252W1DKBJE10VX0NBQDJH. It replaced
`lib/xml.c` / `include/xml.h` (deleted in P15); the only XML library in
the tree is this one.

## Using it (consumers)

```c
#include "xml_documentum.h"      /* the ONLY header a consumer needs */

XmlDocumentum* d = xml_documentum_legere(textus, piscina);
si (!xml_documentum_successus(d)) {
    xml_documentum_diagnostica_scribere(d, "a.xml", aedificator);
}                                 /* every error: via:linea:columna */
radix = xml_documentum_radix(d);
per (p = xml_filius(radix, "part"); p; p = xml_frater(p, "part")) {
    xml_attributum(p, "id", &id);           /* FALSUM if absent */
    xml_textus_totus(p, piscina, &textus);  /* all descendant text */
}
```

Titles are QNames as written; namespaces via `xml_elementi_spatium` /
`xml_attributum_spatii`. `xml_liberum` walks ALL children (whitespace
text is kept — byte law), `xml_filius`/`xml_frater` elements only.
External entities are never fetched (X3); only UTF-8 (X8). Live
examples: `lib/codex.c` (EPUB), `lib/horreum.c` (S3), `lib/plist.c`,
`briar/probationes/fixa/thistle/xml_partitura.thistle`.

## Status

**Closed 2026-10-30 (P17).** As built: spec §X. Next: the MusicXML
reader (desideratum 01M64ZZYQ59DY0G0104SRTK439), on this API.

- **P1 (oracle material) done.**
- **P2 (scaffold) done**: declaration `grammatica/xml.registrum.stml`
  → GENERATED `fontes/xml_registrum_coctum.{h,c}` (15 genera, 33 loci;
  `bin/fabrica` regenerates them itself, action `registrum_xml`); slot
  enums by hand in `fontes/xml_registrum.h`; lexicon (25 token genera,
  prefix `xml-`) in `fontes/xml_lexicon.{h,c}`; runner
  `./xml/compile_probationes.sh [filtrum]` (thin door to `bin/fabrica
  probare probationes_xml`); gate `xml` in pythonica.

- **P3–P7 done**: `xml_nomina` (Char/Name tables), `xml_lector` (six
  self-chosen modes), `xml_arbor` (iterative builder, byte law holds on
  every corpus; fragment entry for entity text), STML round trip,
  `grammatica/xml.canon` (76 rules, seal pinned).
- **P8–P11 done as one slice**: `xml_coctum` (cooked view: DTD, entities,
  attributes, namespaces, every WFC), `xml_oraculum` (canonical forms).
  **W3C: not-wf 922/922, valid+invalid 948/948, outputs 309/309**
  (`probatio_xml_oraculum`, pinned; `XMLTS_CASUS=<ID>` prints one case's
  diagnostics, `XMLTS_OMNIA=1` every failure).

- **P12 done**: differential vs expat/libxml2 over the wild corpus
  (`probatio_xml_differentia`; golden `xml/build/aurum_silvestre.tsv` by
  fabrica action `xml_aurum_silvestre`): 7,799 agree, 114 codificatio,
  4 libxml2 with us, 0 unnamed.
- **P13 done**: public API `fontes/xml_documentum.h` (opaque
  `XmlDocumentum`; `xml_filius`/`xml_frater`/`xml_attributum`/
  `xml_textus_totus` …). Use THIS, not `xml_coctum.h` internals, in
  consumers. Header awaiting Fran's review (written overnight).
- **P14 done**: totality (mutations, truncations, random, extremes:
  0 failures) and `computus` (counts tool over the cooked view).
- **P15 done**: every consumer migrated — codex, horreum (+
  `probationes/horreum_simulacrum.h`), the pecunia generator (output
  byte-identical, `-probare` says recens), plist. plist now WRITES
  directly (no XML tree; old pretty format kept byte for byte — empty
  leaf `<t/>`, blank last line) and READS the cooked view; it gained
  `PLIST_PROFUNDITAS_MAXIMA` (512) for reader and writer, which also
  refuses self-containing lists. Old `probatio_xml.c` retired (its two
  house cases live in `probatio_xml_api.c`); the 12 `sondae_auditus/
  xml_*` probes deleted with their parks.

- **P16 done**: `xml/fontes` is briar's third client root — a
  `.thistle` uses `xml_documentum.h` with no `#include` (fixture
  `briar/probationes/fixa/thistle/xml_partitura.thistle`, fumus XXV).
  Adding a root means FOUR homes (see briar/CLAUDE.md). File-local
  type names in xml sources must not equal public names (the amalgam
  renames by rule and skips public ones) — hence `GradusArboris`,
  `Ens`, `IntervallumRunarum`.

## The shape (P2, read before P4/P5)

Borrowed from html: `<titulus` and `</titulus` are single tokens; `>`
and `/>` their own. `elementum` = tok_apertura · attributa · tok_finis
· liberi · tok_clausura · tok_clausura_finis; an empty element stops
after tok_finis `/>`. End-tag errors are the builder's (no `absentia`).
Whitespace in content and in prolog/epilog is `textus`; trivia
(`SPATIA`) only inside tags and declarations; comments and PIs are
content. DTD declarations: one genus `declaratio` (opening token, part
tokens, `>`), meaning read by `xml_dtd` (P8).

## Oracles (P1)

- **W3C suite** — NOT in the repo (James Clark's licence forbids it):
  `./xml/xmlts_arcessere.sh` fetches it into `xml/build/xmlts/` and
  checks it against `probationes/fixa/xmlts.manifestum`. The cases we
  judge, with exclusions by cause: `probationes/fixa/xmlts/
  selectio.tsv` (regenerate with `python3 -I
  xml/instrumenta/xmlts_catalogus.py xml/build/xmlts/xmlconf`; add
  `-mensura` for the expat/libxml2 agreement). Everything measured —
  counts, licence, canonical forms, oracle credibility — is in
  `probationes/fixa/xmlts/PROVENIENTIA.md`.
- **expat** — `instrumenta/expat_canonicum.py <via>`: the suite's
  canonical form, or `INVALIDUM: l:c cause`. Never reads an external
  entity. Known divergence: editions 1–4 name rules.
- **libxml2** — `xmllint --noout --nonet <via>`. Known divergence:
  namespace errors are not failures.
- **Wild corpus** — `probationes/fixa/silvestria.manifestum` (7,931
  real files on this machine, sha256 + path only); rebuilt by
  `instrumenta/silvestria_indicem.py`.

Harnesses read untrusted files: run them with `python3 -I`, paths as
arguments.
