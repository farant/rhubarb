# xmlts — provenance of the W3C oracle (plan xml-arbor P1)

*Measured 2026-10-29/30 in sexta. Frozen input: never edited; a new
release is a new commit with a cause.*

## The suite

- **Release:** XML W3C Conformance Test Suite 20130923.
- **URL:** `https://www.w3.org/XML/Test/xmlts20130923.tar.gz` (Fran
  allowed the fetch, 2026-10-29).
- **Tarball:** 641,522 bytes, sha256
  `9b61db9f5dbffa545f4b8d78422167083a8568c59bd1129f94138f936cf6fc1f`.
- **Unpacked:** 3,386 files (`xmlts.manifestum`, sha256 per file),
  15 MB, under `xml/build/xmlts/xmlconf/` — never committed.
- **Fetch / verify:** `./xml/xmlts_arcessere.sh` (fetches if absent,
  checks the tarball's sha256, unpacks into a fresh directory,
  compares every file against the manifest; deletes nothing).

## Licence — why only a manifest is committed

The suite has no single licence; each collection keeps its owner's
notice. James Clark's `xmltest/readme.html` (1998):

> Permission is granted to redistribute the file xmltest.zip
> containing this collection to third parties provided that no
> modifications of any kind are made to this file. Note that
> permission to distribute the collection in any other form is not
> granted.

Copying the unpacked files into the repo would be "another form".
The catalogue `xmlconf.xml` carries Sun and OASIS copyright lines
without a redistribution grant. Hence: manifest + fetch script, the
same rule Fran set for toml's wild corpus (2026-09-28).

## The catalogue and our selection

`xmlconf/xmlconf.xml` pulls 21 sub-catalogues by external entity
(the Sun ones are fragments with several roots).
`xml/instrumenta/xmlts_catalogus.py` reads them.

- **All cases:** 2,585 — not-wf 1,498 · valid 812 · invalid 242 ·
  error 33; 432 carry an `OUTPUT` file.
- **Selection** (spec X1: RECOMMENDATION XML 1.0 any erratum or NS
  1.0; VERSION not 1.1; EDITION absent or containing 5): **2,001** —
  not-wf 1,017 · valid 728 · invalid 229 · error 27 (optional errors,
  never judged).
- **Exclusions inside the selection** (each case's cause in
  `selectio.tsv`, column 7):

| cause | count | meaning |
|---|---|---|
| `codificatio` | 51 (not-wf 35, valid 7, invalid 2, error 7) | UTF-16 BOM or a well-formed `encoding=` naming a non-UTF-8 encoding (X8) |
| `sine-spatiis` | 9 (valid 7, invalid 2) | `NAMESPACE="no"`: written for processors without namespaces; we are namespace-aware (X1) |
| `entitas-externa` | 60 (not-wf) | the error lives only in an external entity: expat, reading no external entity, accepts it (X3) |
| `exitus-externus` | 67 (valid 58, invalid 9) | the expected output depends on an unread external entity: accept/reject still judged, output not |

- **Judged:** 1,861 accept/reject verdicts (not-wf 922 to reject;
  valid 714 + invalid 225 to accept) and **308 outputs**.

## The canonical forms (read from the `out/` files)

- **First form** (James Clark): no XML declaration, no comments, no
  DOCTYPE; an empty element is `<a></a>`; attributes sorted by name
  (code point order); in text and attribute values `&` `<` `>` `"`
  become `&amp;` `&lt;` `&gt;` `&quot;` and TAB, LF, CR become
  `&#9;` `&#10;` `&#13;`; a PI is `<?target data?>`.
- **Second form**: when the DTD declares notations, a
  `<!DOCTYPE root [\n` … `]>\n` block lists them sorted by name —
  `<!NOTATION n PUBLIC 'p' 's'>`, `<!NOTATION n PUBLIC 'p'>` or
  `<!NOTATION n SYSTEM 's'>`, one per line — **at the DOCTYPE's place
  in the document** (a PI before the DOCTYPE stays before it:
  `ibm/valid/P29/out/ibm29v01.xml`). 22 of the 440 output files use
  it (xmltest 4, sun 9, ibm valid 7, ibm invalid 2).
- `xml/instrumenta/expat_canonicum.py` writes both; with the rules
  above it equals **308 of 308** judged outputs.

## The two differential oracles, measured before use

Over the 1,861 judged verdicts:

- **expat 2.7.4** (Python `pyexpat`; external entities never read;
  a second pass with namespace processing): **1,548 right, 313
  wrong** — every miss is one cause: expat keeps the name-character
  rules of editions 1–4, so it rejects names the Fifth Edition allows
  (IBM P85 196, P87 84, P88 13, P89 10, P86 4; `x-rmt5-014/016/019`,
  `x-ibm-1-0.5-valid-P04`, `invalid-sa-140/141`). Outputs: 308/308.
- **libxml2 2.9.13** (`xmllint --noout --nonet`): **1,837 right, 24
  wrong** — 23 namespace not-wf cases accepted (`rmt-ns10-*` 20,
  `rmt-ns-e1.0-13a/b/c`; xmllint reports namespace errors without a
  failing exit status) and `rmt-e2e-18` (valid) rejected because its
  parameter entity is external and unread.

So in P12 an expat disagreement on a name is expected and named; a
libxml2 disagreement on a namespace error likewise.

## The wild corpus (`../silvestria.manifestum`)

`xml/instrumenta/silvestria_indicem.py`, 2026-10-30: `*.xml *.svg
*.plist *.xhtml *.xsd *.xsl` under `~/Documents/projects` and
`/opt/homebrew`, `*.plist` under `/System/Library/CoreServices`;
`build/`, `node_modules/`, `.git/` skipped; only files starting with
`<` (after an optional BOM and whitespace); unique by sha256.

- **7,931 files**, 333,068,616 bytes; largest 35,651,895
  (`fil-c/projects/expat-2.7.1/testdata/largefiles/ns_att_test.xml`).
  By extension: xml 4,182 · svg 2,417 · plist 1,079 · xsd 176 · xsl
  44 · xhtml 33.
- **Not UTF-8: 123** (iso-8859-1 104, utf-16 10, iso-8859-5 3, bad
  UTF-8 bytes 3, euc-jp 1, windows-1252 1, a test file declaring
  `unsupported-encoding` 1) — the X8 exclusions of the wild gate.
- **expat baseline:** accepts 7,858, rejects 73; with namespace
  processing 7,826 accepted (32 more namespace errors). The corpus
  contains other parsers' test data (expat's own `testdata/` among
  them), so rejections are expected and useful.
