# tools/diagnostica — worklog

## 2026-09-25 — C89 joins the pipeline (silva-migratio T14)

Re-scoped by its measurement: C89 already HAS a registry (the grammar
derives 58 genera / 176 loci; a `c89.registrum.stml` would be a second
source), so what was missing was the declared-diagnostics half
(`SILVA_C89_DIAGNOSTICA`, declared on `<genus titulus="error">` in
c89.stml, emitted by silva_coquere only when something is declared)
and a route in this tool. crusta's 36 declared diagnostics are mostly
ABSENTIA/VACUA — products of a recursive-descent parser's partial
nodes; the GLR never builds those, so `error` is the one honest C89
declaration until T15 brings the death point through `emissa`.

What the route needed, each found by a failing run:

- **The root is a LIST.** C89's start symbol `elementa` has no genus
  (adding one would move the registry seal and every oracle document),
  so `Cliens.radices` carries the top-level nodes; derivation and the
  'scriptura' projection run per node, concatenated in source order.
- **Positions came out in latina.h** — silva_frons's sedes hook fixed
  (see silva/fontes/silva_frons.worklog.md).
- **Header closure required.** latina alone gave false `error` nodes
  where a header's object-like macro sits in syntactic position
  (lib/silex.c: `SILEX_VEXILLA_...` between string literals). The route
  asks `bin/aedilis <file> --partes` for the closure, as ./silva/arbor.sh
  does, via `processus_exsequi` (no shell). Failure = named MONITUM and
  latina-lexicon fallback, never silence.
- **Include ORDER matters.** latina as a LEXICON applies before
  everything — including vendor headers included BEFORE latina.h:
  `char C` in vendor/sqlite3.h became `char 100`. latina now arrives
  through the closure in its true position; the lexicon is only the
  fallback.
- **Diagnostics belong to their file.** With headers supplied, their
  declarations are top-level nodes of the unit; only nodes whose extent
  lies in the judged file (`fons_princeps`) are walked — a header's
  errors are the header's judgment, and printing them against this
  file's text was nonsense (scrinium.c "line 8883").
- Pre-existing: `_clientem_parsare` returning FALSUM on a NIHIL tree was
  SILENT; now "parsura fracta" is named.

Measured: 430 house C files (lib, include, materia, silva, crusta, css,
html) — 0 diagnostics, 0 MONITUM, 57 s (≈130 ms/file, mostly the aedilis
subprocess; a shared long-lived context is possible — contexts are
const at parse — if this ever runs over the corpus routinely).
Gate `diagnostica` section XXII, both assertions born red by plants
that compile (def-site sedes; latina lexicon unconditionally — the
second only discriminates on vendor/sqlite3.h judged ITSELF, since the
principal-file filter hides header errors from .c judgments).
