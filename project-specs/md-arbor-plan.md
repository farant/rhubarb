# md_arbor — plan (re-cut 2026-09-03 from `md-arbor-spec.md` v1.1)

> Status: EXECUTABLE. The draft of 2026-09-02 (its six open questions)
> is superseded by the interview (`md-arbor-interview.md`) and the spec.
> House convention: inline execution, no subagents; every gate born
> with a planted fault; gates and commits through pythonica
> (`silva.commissio_umbra(..., ['md', 'materia', 'pythonica'])` once
> the `md` runner exists — registered in `PORTAE` and the
> `commissio_umbra` prefix map in the SAME commit that creates it).
> Frame (Fran, 2026-09-03): arc B is EXPLORATORY DESIGN — the program's
> readability is a finding; log "quid STML voluit" as you go.

**Goal:** the spec's §0. **Constraints:** the spec's §1 standing block
plus the house block (Latin, latina.h words forbidden, chorda not
NUL-terminated, i32/i64 unsigned, exit 2 = nothing ran, never stage
Fran's staging files, silva frozen, positions = byte offsets + 1-based
linea/columna, `./tools/natura_struere.sh` after STML edits, planted
fault at every gate's birth).

---

## Arc A — the parser (materia client)

### A2 Registry, lexicon, line lexer, runner
Files: `md/fontes/md_registrum.{c,h}` (spec §4 genera + slots as the
css table: flat slot array + genus windows, named slot enums),
`md/fontes/md_lexicon.{c,h}` (spec §4 lexicon, prefix `md-`, LINEA has
munus LINEA), `md/fontes/md_lexema.{c,h}` (line lexer: prefix runs —
indentation, `>`, list markers, task box, ATX marks, fence/info,
setext, thematic break, pipes — then the rest of the line as one
TEXTUS token, LINEA, FINIS; zero trivia), `md/compile_probationes.sh`
(the css runner, `md` substituted, mensor prefix `md.`),
`probatio_md_registrum`, `probatio_md_lexema`; pythonica `PORTAE['md']`
+ prefix map + a `Porta` test. Exit: lexer round trip byte-exact over
all 1,120 files with coverage asserted (files > 0, bytes > 0);
registry/lexicon counts asserted by TITLES; runner exit 2 on a bad
filter; planted fault (drop a marker) red.

### A3 Leaf blocks
`md/fontes/md_arbor.{c,h}` — `md_arbor_parsare(piscina, fons, mensura)`
→ `documentum` node, source never copied (tokens point into it — the
css B9 birth lesson). Paragraphs (raw content lines for now), ATX +
setext headings, thematic breaks, fenced + indented code, blank lines,
html blocks (seven start conditions, opaque), praefatio. Exit:
parse→emit byte-exact on inline fixtures + the corpus subset without
containers; clean files have no `mala`; headings/fences counted > 0.
The indented-code count is PRINTED (spec §11).

### A4 Containers
Lists (bullet/ordered, start, tight/loose → `laxa`, `nudus` pushed down
to first-child paragraphs), nesting, block quotes, lazy continuation,
item indentation rules, task boxes → `officium`. Container markers are
line prefixes (spec §3). Exit: every CommonMark container example
INPUT round-trips byte-exact; `laxa`/`nudus`/`officium` pinned on 20
chosen examples; planted fault (skip lazy continuation) red.

### A5 Tables + link reference definitions
GFM pipe tables (delimiter row → `ordinatio` on every cell);
definitions collected document-wide and RESOLVED onto `nexus` nodes as
derived tokens at parse end (needs A7's nexus nodes — A5 builds the
definition table and the derived-token machinery; resolution lands in
A7). Exit: tables round-trip; definition table pinned; a derived token
(source 1) is omitted by the emitter and carries `f="1"` in the
projection (the first derived-token gate).

### A6 The corpus gate
`probatio_md_corpus`: byte oracle over 1,120 files + every
CommonMark/GFM example input (the spec.txt reader lives here first);
green INTERROGATED (structure on clean files, zero `mala`, real
headings/lists/tables counted); spec §11 measurements printed. Exit:
all byte-exact; planted fault red; the indented-code sample eyeballed
and recorded in the worklog.

### A7 Inline tree (owns its bytes)
`md/fontes/md_inlinea.{c,h}`: paragraph/heading/cell content re-lexed
into inline tokens (TEXTUS runs, DELIMITATOR, GRAVIS, EFFUGIUM, ENS,
NEXUS-*, AUTONEXUS, HTML, SPATIA-FINALIA, LINEA) and REPLACED by the
inline tree (spec §3 deviation b): textus (crudum + derived valor when
decoding differs), emphasis/fortis/deletio by the delimiter-run
algorithm, verbatim, nexus (inline/reference/auto, resolved url +
descriptio as derived tokens), imago, fractura-mollis/dura owning the
newline and the next line's prefixes, html-inlineum, bare-URL
autolinks. Exit: corpus gate still byte-exact (inline included); spec
§6.2 emphasis cases pinned; decoded values pinned for escapes/entities;
planted fault (wrong delimiter flanking) red.

### A8 Projection, canon, totality, computus
md `MateriaArborConsilium` with origo hook + frons for source-1 tokens
(`cursorem_movere` VERUM for them); `probatio_md_stml` (write→read→
write twice, STRUCTURALIS + FIDELIS — md has LINEA); `md/grammatica/
md.canon` hand-written + drift guard + sigillum (`probatio_md_canon`);
`probatio_md_totalitas` (random bytes, mutated/truncated corpus,
depth); `md/computus.sh` + `probatio_md_computus` (css columns, golden
`fixa/computus/basis.tsv`); `md/arbor.sh`. Exit: four gates green,
each seen red; derived tokens round-trip with position portata.

## Arc B — the transform by composition (exploratory)

### B1 Engine: five pieces (spec §6)
`lib/stml_macros.c`: slot projections `&@n.slot;` · `de="@arg[.slot]"`
scope threading · PER over a forest with `voca=` (+ `ut=` body form) ·
pattern CASUS with `<EST>` + captures→arm + `tag=` sugar · self-call as
a CHECK (descent guard). Fixtures in `probationes/fixa/exemplaria/`
(the spike documents rewritten on the new forms + one per piece,
expected output pinned); vitia named; INDAGO traces; canon CAPS rows.
Exit: the spike's nested list produces one `<ul>` with a nested `<ul>`
inside its `<li>`; L1 / lint II / codex-69 byte-identical; `stml`
CLI rebuilt (`./tools/stml_struere.sh`).
**STATUS 2026-09-03: B1.1 d50ade07 · B1.3 20f192e2 · B1.4 8fbf71d7 ·
B1.5 f33d953a — exit met (`exhibita/md-html-b1.stml` through `stml
expandere`), gates root + silva green at every landing, fixtures in
`probationes/probatio_stml_exemplaria.c` (348 assertions), vitia
XXVI–XXXI. B1.2 (`de="@n"` on EXEMPLAR/CATENA/DIRIBITIO inside bodies)
NOT BUILT — no md consumer; see spec §6.2 as-built note; Fran's call.**

### B2 The md→html program
`md/html/md-html.stml` per spec §5 (helpers, the one dispatcher, table
arms, tight lists via `nudus`, `title=` presence, fences). `md/html.sh`.
Worklog section "quid STML voluit" — every verbosity, wrapper fight,
missing form, workaround. Exit: the spike's three examples + 30 chosen
CommonMark examples produce the spec's html after normalization.

### B3 The html oracle
`probatio_md_html`: vendored `spec.txt` (0.31.2) + GFM `spec.txt`
(extension sections), parse → project → expand → `vertere` (fragment
option) → C normalizer over `html_lexare` (forgives inter-tag
whitespace outside `<pre>`, attribute order, void-element spelling,
entity spelling — each COUNTED and reported) → compare. Raw html
passthrough decided here (spec §13 f). Exit: pass count pinned and only
rising; failures listed by section; planted fault in B2 red.

## Arc C — the consumer

### C1 pythonica `Prosa`
`md/extenta.sh` (selectio over the projection → TSV extents);
`silva.Prosa(via)` per spec §9 (`selecta`, `capitulum`, `elementum`,
`saeptum`, `substituere`, `inserere_*`, `corpus`, `applicare` with the
re-parse judge); pythonica gate edits a copy of a real spec by heading
and by list item; refusals name lines. Exit: gate green; stale read
refused; a wrong-count anchor refused with line numbers.

### C2 Wire-up, closure, debrief (the B10 shape)
Spec as-built notes, `md/CLAUDE.md`, worklog, ledger RELATIO +
instrument debrief, memory, `md/CENSUS.md` (the 2026-09-03 census as a
committed measurement), exemplaria interview + visio updated with the
"quid STML voluit" findings.

---

## Order and size
A2 → A3 → A4 → A5 → A6 → A7 → A8; B1 after A3 (spike documents
suffice); B2 after A8; B3 after B2; C1 after A8; C2 last. About 20
tasks; A7 and B1 are the hard ones. Re-entry: this file, the spec, the
parcum "Parsator markdown in materia" (01M1JJMQBT), the interview.
