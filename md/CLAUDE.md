# md — orientation

*You are reading this because you touched a file in `md/`.*

Markdown parser built on **materia** (third client after C89-shim and
css). Design: `project-specs/md-arbor-spec.md` (v1.1, 2026-09-03);
plan: `project-specs/md-arbor-plan.md` (A2..A8 parser, B1..B3
html transform by STML COMPOSITION, C1..C2 pythonica `Prosa`);
interview + census: `project-specs/md-arbor-interview.md`; spike:
`md-arbor-spica.md`. Ledger parcum 01M1JJMQBT.

## Praesens status (2026-09-03)

**A2 DONE**: `md_registrum` (28 genera, 87 slots, named slot enums),
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

**NEXT = A4** (containers: lists, block quotes, lazy continuation).

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
