# md — orientation

*You are reading this because you touched a file in `md/`.*

Markdown parser built on **materia** (third client after C89-shim and
css). Design: `project-specs/md-arbor-spec.md` (v1.1, 2026-09-03);
plan: `project-specs/md-arbor-plan.md` (A2..A8 parser, B1..B3
html transform by STML COMPOSITION, C1..C2 pythonica `Prosa`);
interview + census: `project-specs/md-arbor-interview.md`; spike:
`md-arbor-spica.md`. Ledger parcum 01M1JJMQBT.

## Praesens status (2026-09-03)

**A2 DONE**: `md_registrum` (28 genera, 92 slots since B2.1, named slot enums),
`md_lexicon` (26 token genera, prefix `md-`, LINEA is a TERMINATOR
with munus LINEA — the FIDELIS comparator can run here, unlike css),
`md_lexema` (line table + scanners + token factory + the crude
stream). Gates: `probatio_md_registrum` (titles, contiguity, a
materia round trip), `probatio_md_lexema` (line table, positions,
scanners vs CommonMark examples, CORPUS: every git-tracked `*.md`
byte-exact — 1,122 files / 17.2 MB / 600k tokens). Runner
`./md/compile_probationes.sh [filter]` (exit 2 = nothing ran) writes
`md/build/corpus_md.txt` from `git ls-files` first: a directory walk
finds 3,110 files, most of them scratch. Registered in pythonica
`PORTAE['md']` (+ FORMAE, SUITAE, mensor prefix `md.`).

**A3 DONE**: `md_arbor.{c,h}` — `md_arbor_parsare` builds the leaf
blocks (paragraphs with their final shape praefixa+nudus+inlinea+finis,
ATX + setext headings, thematic breaks, fenced + indented code, blank
lines, opaque html blocks by the seven start conditions, front matter).
Gate `probatio_md_arbor`: CommonMark §4 structure fixtures + the WHOLE
corpus parse→emit byte-exact (1,124 files; capitula 18,520, saepta
3,893 of which 65 indented — the census heuristic was list
continuations, sites eyeballed). Lists and quotes are still paragraphs.

**A4 DONE**: containers (lists with `genus`/`initium`/`laxa`, items
with `officium`, block quotes, nesting, lazy continuation, tight-list
`nudus` push-down) — the appendix-A stack in the line model; markers are
prefix tokens of the owning leaf; blank lines PEND until the next
non-blank line names their container. Corpus: 10,435 lists, 46,707
items, 160 quotes, still byte-exact.

**A5 DONE**: GFM tables (`md_scan_ordo`, header from the paragraph's
last line via the list VIEW, rows padded to the header's cell count,
1,367 in the corpus) and link reference definitions (extracted at
paragraph close; label normalized into the parser's table; `titulus` /
`url` / `descriptio` as DERIVED tokens, source 1, decoded by
`md_decoctum` — emitter omits them with `consilium.fons_index = 0`,
and the gate proves it both ways). Gates: `probatio_md_decoctum` +
the arbor fixtures.

**A6 DONE**: `md/probationes/fixa/{commonmark,gfm}/spec.txt` vendored
(FONTES.md has sources + pins), `md_exempla` reader (one form, both
files, `→`→tab, extension tag kept), gate `probatio_md_exempla`: 652 +
672 example INPUTS byte-exact through parse→emit. The house-corpus
half stays in `probatio_md_arbor`.

**A7 DONE**: `md_inlinea` — the inline tree built in place over each
`inlinea` (item list → delimiter stack + bracket stack → materia nodes);
text `valor` only when decoding differs; code spans, emphasis/strong/
strikethrough, inline + reference + auto links (definition table),
images, hard/soft breaks (dura OWNS its signum), inline html. Corpus
and both spec example sets stay byte-exact. Gate `probatio_md_inlinea`
(§6.2 rules pinned). Birth lesson: delimiter extents must shrink in
EVERY match branch or the matcher loops.

**A8a DONE**: `md_stml_consilium` (origin hook: derived tokens carry
position, root = empty sentinel); gate `probatio_md_stml` with
STRUCTURALIS + FIDELIS over 17 cases + the corpus; `./md/arbor.sh
<x.md>` prints the projection. Substrate fixes: slot TERMINATOR crlf
(writer + reader); token values never contain `</md-` (split).

**A8b DONE**: `md/grammatica/md.canon` hand-written (143 rules; no
trivia wrappers by design; seal `de590d67` pinned); gate
`probatio_md_canon` = drift guard both ways + seal pin + every corpus
projection judged (0 faults). Manual judgment: `bin/canon_examen -canon
md/grammatica/md.canon <doc.stml>`. Parser change → canon change → the
corpus judgment falsifies it.

**A8c DONE**: `probatio_md_totalitas` (random/mutated/truncated/
nested to 50k — found and fixed the depth-cap NIHIL) and the computus
twin (`./md/computus.sh <x.md> [-machina] [-iter N]`, golden
`fixa/computus/basis.tsv` over `fixa/md/` snapshots; `COMPUTUS_SCRIBERE=1`
+ a named cause). Arc A COMPLETE: 9 gates.

**B1 IN PROGRESS** (engine, `lib/stml_macros.c`, spec §6; fixtures in
`probationes/probatio_stml_exemplaria.c`; gates root + silva in shadow
clones). **B1.1 DONE** (d50ade07): `&@n.slot.slot;` = CONTENT of the
named child wrapper by literal child steps (text = scalar, else forest,
never the wrapper itself); `&@n.slot!;` = bytes of text + raw leaves
(explicit — no implicit element→text rule; structure = vitium VII);
vitia XXVI–XXVIII. Findings so far in the worklog's "quid STML voluit"
entries: token slots need `!`; NODUS slots are double-wrapped; `est=""`
is a tombstone in the base grammar (empty wrapper = PRESENT empty
scalar; absence = missing wrapper, XXVII, discriminated by B1.4's
pattern arm). **B1.3 DONE**: `<PER de="@n.bloci" voca="#@f"/>` (rows =
ELEMENTS of the projected forest into the delegated template's sole
required slot, a true call) and `<PER de="@n.x" ut="e">…</PER>` (same
fill, outer scope visible, row name local and non-shadowing); empty
wrapper = zero rows; vitium XXIX. **B1.4 DONE**: `<CASUS tag="x">` and
`<CASUS><EST><EXEMPLAR>…</EXEMPLAR></EST>arm</CASUS>` in COMMUTATIO —
pattern filled with the arguments, anchored at the scrutinee's root,
captures join the arm's table; vitium XXX (scalar in tree position,
mirror of VII). **B1.5 DONE**: a call or PER delegation to the
template being filled is admitted iff every subtree argument descends
STRICTLY from the current fill's arguments (parent pointers), else
XXXI RECURSIO_NON_DESCENDENS; no depth cap (none existed); the spike's
nested list through a four-arm dispatcher yields `<ul><li>one<ul>…`.
B1.2 (`de="@n"` on EXEMPLAR inside bodies) NOT BUILT — no md consumer,
needs fill-scoped relations; parked pending Fran (spec §6.2 note).
Exhibit: `project-specs/exhibita/md-html-b1.stml` through `stml
expandere`.

**B2 IN PROGRESS.** **B2.1 DONE**: four derived tokens for the html
program — `saeptum.valor` (decoded fence content, indentation stripped,
lines + `\n`), `saeptum.lingua` (first word of the decoded info),
`imago.alt` (plain text of the description), `fractura-mollis.valor` /
`fractura-dura.valor` = `\n` (always present: the terminator has no
bytes in the projection, and a template cannot manufacture whitespace —
finding 13). Registry 92 slots, seal `81c120c4`. **B2.2 DONE**: `md/html/md-html.stml`
(one dispatcher, DENSE form — finding 14: template layout leaks into
html as trivia), `md_html_reddere` (`md/fontes/md_html.c`: parse →
project → compose → expand → distribute → `stml_html_vertere_liberos`,
the new doctype-less entry in `lib/stml_html.c`), `./md/html.sh <x.md>
[-stml]`, gate `probatio_md_html` (twelve inputs byte-exact, our bytes).
Open for Fran: should fills drop layout trivia (finding 14)? **NEXT =
B3** the CommonMark/GFM oracle with the C normalizer (`probatio_md_html`
grows the spec loop; raw html passthrough decided there).

## Laws to keep (spec §3)

- **Line model, zero trivia**: every byte is a token in ONE slot;
  every line is owned by a LEAF block; container markers are prefix
  tokens of the line they appear on; containers own no bytes; blank
  lines are `linea-vacua` blocks of the open container.
- **Inline tree owns its bytes** (A7 replaces raw content lines with
  the inline tree; one shape from A3 on: paragraph = praefixa + nudus +
  inlinea).
- **Derived tokens** (`fons_index` 1, `MD_FONS_DERIVATUS`) carry
  decoded text / resolved links / lingua; the emitter omits them
  (`consilium.fons_index = 0`); present ONLY when they differ.
- **Push-down law**: parent context (laxa, nudus, officium, ordinatio)
  is decided by the parser and stored as INDEX slots.
- Scanners return EXTENTS; the parser decides ownership; a tab is
  never split between owners (residual columns are content).
- Genera and token genera are APPENDED, never reordered; the tests
  assert both tables by TITLE.

## Birth lessons (A2)

- `L` is the Roman numeral 50 in latina.h — a variable named `L` fails
  to compile with a cryptic "expected identifier". Same for `C D M V
  X I`. Lowercase single letters are safe.
- `i - ab < longitudo_min` with s32 minus i32: `-Wsign-compare`; cast
  the difference.
- Planted faults at birth: ATX accepting seven `#` → scanner red;
  dropping the INDENTATIO token → corpus red (1,122 files named).
