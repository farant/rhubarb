# md — orientation

*You are reading this because you touched a file in `md/`.*

Markdown parser on **materia** (third client after the C89 shim and
css), html by STML COMPOSITION, and two consumers: pythonica `Prosa`
(edit markdown by structure) and `citata` (documentation rot). Design:
`project-specs/md-arbor-spec.md` (v1.1 + as-built notes); plan
`md-arbor-plan.md`; interview `md-arbor-interview.md`; spike
`md-arbor-spica.md`; findings ("quid STML voluit", 1–16 + C1) in
`md/fontes/md_arbor.worklog.md`; ledger parcum 01M1JJMQBT (closed at C2),
polish list desideratum 01M1NBEVM0.

## Map

- **Parser** (`md/fontes/`): `md_registrum` (28 genera, 94 slots, named
  slot enums), `md_lexicon` (27 token genera, `md-` prefix, LINEA is the
  terminator), `md_lexema` (line table, scanners, token factory),
  `md_arbor` (blocks: containers, leaves, tables, definitions),
  `md_inlinea` (inline tree owning its bytes), `md_decoctum` (entity and
  escape decoding, url encoding), `md_exempla` (spec.txt reader).
- **Projection + html**: `md_stml` (consilium with the origin hook for
  derived tokens), `md/grammatica/md.canon` (hand-written, seal pinned),
  `md_html` (parse → project → compose with `md/html/md-html.stml` →
  expand → distribute → `stml_html_vertere_liberos`).
- **Consumers**: `md_extenta` (selector → source bytes by pre-order token
  correspondence), `md_census` (counts from the parsed tree; code-span
  dump), `md_computus` (bench twin).
- **Instruments** (`md/*.sh`, need `md/build/*.o` from the runner):
  `arbor.sh <x.md>` projection · `html.sh <x.md> [-stml]` · `extenta.sh
  <x.md> '<selector>'` (TSV, rc 0/1/2) · `census.sh [-machina | -citata
  [via] | -scribere]` (`-scribere` regenerates `md/CENSUS.md`) ·
  `computus.sh <x.md> [-machina] [-iter N]`.
- **pythonica**: `silva.Prosa(via)` (`capitulum`, `sectio`, `elementum`,
  `saeptum`, `substituere`, `applicare` with the round-trip judge),
  `silva.citata()` (paths and function names in code spans against the
  tree and `build/nexus.tsv`), `silva.oraculum([n])` (CommonMark oracle
  failures and raw examples), gate tables (`PORTAE['md']`).

## Gates (`./md/compile_probationes.sh [filter]`; exit 2 = NOTHING RAN)

registrum · lexema (corpus byte-exact, every tracked `*.md`) · decoctum
· arbor (CommonMark §4/§5 structure + corpus parse→emit) · exempla (1,324
spec inputs byte-exact) · inlinea (§6 rules) · stml (STRUCTURALIS +
FIDELIS round trip) · canon (drift guard both ways, seal, corpus judged)
· totalitas (random/mutated/nested to 50k) · computus (golden
`fixa/computus/basis.tsv`, `COMPUTUS_SCRIBERE=1` + a named cause) ·
html (twelve inputs byte-exact) · oraculum (1,270/1,324 pinned, only
rising; `ORACULUM_OMNIA=1`, `ORACULUM_EXEMPLUM=N`) · extenta (57) ·
census (41). Every gate was born red by a planted fault.

## Laws to keep (spec §3)

- **Line model, zero trivia**: every byte is a token in ONE slot; every
  line is owned by a LEAF block; container markers are prefix tokens of
  the line they appear on (so a paragraph inside a list item owns its
  marker bytes); containers own no bytes; blank lines are `linea-vacua`
  blocks of the open container.
- **Inline tree owns its bytes** (paragraph = praefixa + nudus + inlinea).
- **Derived tokens** (`fons_index` 1) carry decoded text, resolved links,
  fence language and content, alt text, newlines the program must emit;
  the emitter omits them; present ONLY when they differ.
- **Push-down law**: parent context (laxa, nudus, officium, ordinatio)
  is decided by the parser and stored as INDEX slots.
- Scanners return EXTENTS; the parser decides ownership; a tab is never
  split between owners.
- Genera and token genera are APPENDED, never reordered; the tests
  assert both tables by TITLE. Materia slots are WRITE-ONCE: an absent
  list is empty, never pre-filled (the program pays with arm pairs).
- The projection carries NO per-token offsets (the reader's byte cursor
  reconstructs them); consumers map by correspondence, self-checked.

## Status (2026-09-04)

Arc A (parser, 10 gates), arc B (B1 engine pieces in `lib/stml_macros.c`;
B2 program; B3 oracle 1,270/1,324) and arc C (C1 extenta + Prosa; C2
census, citata, closure) are DONE. Open, by name: the oracle polish
classes (desideratum 01M1NBEVM0: entity table cheapest, tabs most
instructive); B1.2 parked (spec §6.2); finding 14 (fills dropping layout
trivia) deferred to the html autoformatter; the fence hook (C fences as
silva trees inside the projection) named, not designed.

## Birth lessons

- `L` is the Roman numeral 50 in latina.h — a variable named `L` fails
  with a cryptic "expected identifier". Same for `C D M V X I`, and
  `nomen` is `typedef`. Lowercase single letters are safe.
- `i - ab < longitudo_min` with s32 minus i32: `-Wsign-compare`; cast.
- Examen's "vocatio implicita" suspecta are right: read them before
  compiling (`credo_initium`, `chorda_ad_literas` did not exist).
- Delimiter extents must shrink in EVERY match branch or the inline
  matcher loops.
