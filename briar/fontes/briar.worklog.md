# briar — worklog

## 2026-09-04 — plan 1 (parser) sealed

Ten tasks, nine gates, every gate seen red by a plant before it counted.
Findings, as measured:

- **Extent of an STML element** = column-0 `</name>` or a single
  self-closing line (spec §2 amended). The lexeme-walk design was
  dropped: the column-0 law already governs every other boundary. The
  one edge it costs, a self-closing element spanning lines
  (`<fenestra\n titulus=…/>`), becomes `elementum-non-clausum` — named,
  not silent.
- **Fence law** toggles only in the lexer's prose state; inside a raw
  region the arbor scans for the close and never classifies content
  lines, so a ``` inside C never toggles anything. The crude lexer
  (per-line, no region state) DOES toggle on a ``` inside a region: it
  is a classification gate, not the arbor.
- **FIDELIS comparator passed on all 14 fixtures on first run**, CRLF
  included: materia's `_cursorem_promovere` counts `\n` inside token
  values, so briar needs no LINEA genus and no frons hook. The `cr`
  attribute carried the `\r` offsets through the projection.
- **The byte law is structure-independent.** Task 3's plant (the
  close never found) went red on seven structural assertions and stayed
  green on every emission check. Consequence for plants: a fault that
  only moves structure cannot be caught by a byte gate, and a plant
  for a byte gate must drop or move bytes (Task 7's plan-suggested
  guard removal was replaced by "drop the final unterminated line",
  which the truncation class caught in 22 cases).
- **Examen caught a real bug at birth:** `_clausuram_quaerere` returned
  `i32` (unsigned) with a `-1` sentinel — four domestica ("sentinella
  negativa in functione insignata", "comparatio vana") on the first
  scribe. An unterminated region would have indexed past the line
  table. Fixed to `s32` before the first test ran.
- **Two plants were inert, both instructive:** (1) leaving the `!` in
  the rewritten open tag parses fine — `<c! a="x"/>` is LEGAL STML (a
  raw self-closing element with attributes); (2) skipping the rewrite
  entirely is a compile error, not a runtime red (unused function under
  `-Werror`). The real plant: emit `>` instead of `/>` → unclosed
  element → four reds.
- **STML accepts `bad=` (attribute without value) as an empty value**;
  the nexus fixture for a malformed open tag uses an unterminated quote
  instead, which STML refuses at 1:10.
- **Canon** loaded by path (shared `<arbor>` root, as css/md); seal
  pinned `9a77c465`; derivation guard covers 5 genera, 14 loci, 8
  lexeme tags, the envelope, and the reverse direction. The attribute
  nodes of the envelope rule are direct `liberi` of the `<elementum>`
  node in the parsed canon, as assumed.
- **Computus golden born** with cause "nativitas portae": salve 507
  bytes / 12 tokens / 6 nodes; salve_vitreum 1315 / 20 / 10. The
  parse of the larger fixture is 0.05 ms; the STML write+read 0.2 ms.
- **Environment finding (worktree):** the pythonica gate depends on
  `oratio/build` objects (`vocabula.sh` inside `probatio_silva.py`);
  a fresh tree must run `./oratio/compile_probationes.sh` once before
  any commit that touches `pythonica/silva.py`, or the shadow gate
  reports `signum absens` with a traceback in its acta.
- `silva.arbor(via)` dispatch for `.thistle` is not wired in pythonica
  (no consumer yet); `./briar/arbor.sh` is the instrument.
