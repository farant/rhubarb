# materia_coctor — worklog

## 2026-09-15 — the genera table generated (oratio's wish 01M24Z4Q5Y, point b)

Born from oratio as a client: its registry was one flat array of 191
loci and 26 genera with hand-computed offsets and counts, and every
added locus meant array entry + enum entry + every following offset +
count + pin + seal + canon rule. The wish asked for the table to be
generated. Fran's constraints (2026-09-15): C89 implementation, usable
without Python (a wrapper may sit on top later); declaration in STML.

**Shape.** `materia_registrum_coquere(piscina, declaratio, via,
&coctio)` reads a `<registrum>` document (canon
`materia/grammatica/registrum.canon`, registered by root in
`canones.registrum`), retires it (kebab titles, identifier prefix and
typedef, unique genus titles, unique locus titles within a genus,
known species, at least one locus overall) and renders two texts in
memory: `<grammatica>_registrum_coctum.h` (genus enum with the
`_NUMERUS_GENERUM` companion, extern of the baked struct) and `.c`
(flat LOCI array with a range comment per genus, GENERA array with
offset and count, the baked `MateriaRegistrumCoctum`). Notes become
comments ABOVE their row, wrapped at 72 with 3 columns reserved for the
closer, so the output is a fixed point of the formator (verified:
`-vitia` CONFORMIS on the real oratio output). The library reads no
files, so client probationes call it in-process and compare bytes with
the committed files (`materia_coctio_aequalis` names the first
divergent line). The instrument `materia/instrumenta/coctor.c` behind
`./materia/coquere.sh <declaratio> [-scribere]` adds the file side and
judges the declaration against the canon first (exit 2 with the vitia).

**Scope, deliberately.** Locus enums stay hand-written in each
client's header: oratio's analysis genera share a common enum
(`ORATIO_ANALYSIS_LEMMA` …) and start their own at
`ORATIO_ANALYSIS_COMMUNIA_NUMERUS`, and every client's typedef names
are Latin genitives (`OratioLocusVocabuli`, `MdLocusVacuae`) that no
rule derives. That shape is the client's, not the substrate's (M8).
The aliases `ORATIO_GENUS_ANALYSIS_PRIMUM/ULTIMUM` became two `#define`
lines in the hand header, since they cannot live inside a generated
enum. The count pin in the client probatio and the seal in the client
canon are NOT generated: they are the gates, moved by hand with a cause.

**Migration acceptance (oratio).** Generated tables equal the
hand-written ones row for row (191 loci, 26 genera, same offsets, by
an independent script), the seal `c4752e42` unchanged (canon gate
271/271), registrum gate 1121/1121 with the new freshness block, the
coctor birth gate 64/64 with the byte-exact fixtures derived BY HAND
from the rendering rule before the emitter ran — they matched on the
first run, wrap rule included.

**Lessons.**
- A doc comment that quotes the comment closer (`" */"` in prose)
  closes the comment; the examen reports one syntax-error node with no
  line, and the three "vocatio implicita" warnings on the function
  defined right after it are the locator. Second instance of the
  memory rule about `*/` inside a block comment.
- `xar_obtinere` takes `constans Xar*`; casting away const to call it
  is a `-Wcast-qual` error, not a need.
- `<sedes>` in a comment is read as a tag anchor by the examen
  (domestic warning); write `sedes/...` instead.
- An English parameter name copied from a neighbouring struct field
  (`detail`, from `CanonVitium`) would meet the identifier lint at
  commit; renamed before staging.
- credo needs `credo_aperire(piscina)` before the first assertion — the
  suite says so loudly ("credo nunquam aperitum").

**Plants (birth).** The first plant, `g->offset + I` in the reader,
went red by a SEGFAULT: the renderer walked `offset..offset+numerus`
past the end of the loci array and dereferenced what `xar_obtinere`
returned. A crash is a red, but not the red the gate was designed to
give (the byte diff with the first divergent line), and it exposed two
hygiene points fixed the same hour: the duplicate-title loops bounded
by `xar_numerus(x) - I` (unsigned `i32`, the subtraction-derived
bound the memory warns about — rewritten as `k + I < numerus`) and the
render loops trusting their indices (now `NIHIL`-guarded). Re-planted
in the GENERA row emitter (`offset + I` in the text only): red in
part III with the diff printed, green on revert. The oratio plant that
changed a species in the generated `.c` was caught first by the older
species assertion, not by the freshness block; re-planted as a comment
edit in the generated file, which only the byte comparison can see.

## 2026-09-15 — tranche two: css and md migrated, the gate became one call

**Two findings the second and third clients brought.** (1) css locus
titles carry underscores (`tok_nomen`, `tok_terminator`); the kebab
rule refused them, so titles now admit `_` as well as `-` (both map
to `_` in the identifier; gate part V). (2) Both clients named the
genus companion `<P>_GENUS_NUMERUS`, oratio `_NUMERUS_GENERUM`; the
generator emits one name, so the css/md usages were renamed to
`_NUMERUS_GENERUM` (twelve sites; `renominare.sh` planned three splices
and called the rest "manual" because they sit inside CREDO macro
invocations — a regex over the six named files, reviewed by grep,
did the job). Every `<P>_GENUS_X` and `Md*Genus` name derived from the
titles as the rule says (`linea-vacua` → `LINEA_VACUA`, `html-inlineum`
→ `HTML_INLINEUM`).

**The gate became one library call.** With three clients the
freshness block would have been three copies of a file-reading
comparison, so it moved into the library:
`materia_registrum_recens(piscina, radix, via_declarationis, &rancor)`
reads the declaration, bakes, reads the two committed files (stdio,
so no client runner needs `filum`) and compares bytes; `rancor` names
the file and the first divergent line, or `absens`. Each client's
registrum probatio is now the same ten lines, and the instrument's
compare mode calls the same function, so instrument and gates cannot
disagree. Gate part VI exercises it on files written into
`materia/build` (absent → rancid line 0; written → recens; a line
appended → rancid at that line; missing declaration → refusal).

**Migration acceptance.** css 45 loci / 21 genera and md 94 / 28
equal row for row by the script; seals `fb55041d` and `ba282b8a`
unchanged (both canon gates green); `css_registrum.c` and
`md_registrum.c` DELETED — they held nothing but the tables, and an
empty translation unit is not C89 (`-pedantic` refuses it), so the
generated `.c` takes their place in the runners' globs. Plants: a
hand edit of a comment in each generated header went red at
`rancor.recens` in the client's registrum gate, green on revert.

**Two things the full runs taught.** (1) Gate part VI wrote its
fixture files into `materia/build` and the second run of the suite
found them already there: the "absent" case saw a rancid `.c` from
the previous run (line 28). The part now removes both files first —
a test that writes outside its arena must reset before it asserts.
(2) Deleting `css_registrum.c`/`md_registrum.c` left their `.o` in
four build directories (briar, css, md, oratio); the suites compile
sources by name and stayed green, but oratio's instruments link
`build/*.o` by glob, so `sententiae.sh` linked the old `md_registrum.o`
beside `md_registrum_coctum.o` — `ld: duplicate symbols _MD_REGISTRUM`,
and the pythonica gate went red through an instrument, not a suite.
A source deletion is the one change the `-nt` staleness guard cannot
see. Deleted by hand today; the runner-side pruning is a ledger
question. Briar's runner also listed `md_registrum` by name and now
lists `md_registrum_coctum`.

**Python.** `silva.registrum_coquere(via, scribere=False)` shells to
`./materia/coquere.sh` and returns `(rc, recens, viae, acta)`; the
pythonica gate checks the committed oratio declaration (rc 0, both
paths named) and a bad declaration (rc 2, nothing named). C89 first,
Python on top, as decided.
