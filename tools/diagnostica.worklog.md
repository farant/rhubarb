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

## 2026-09-25 — the GLR names where it died (silva-migratio T15)

The death point was never missing — `silva_glr_parsare` has always
returned `positio` (index of the lexeme at which the whole frontier
died) and `terminalis`; the driver's recovery branch wrapped the segment
into an ERROR node and dropped both. T15 keeps them: `SilvaParsura.mortes`
(one `SilvaMors` per ERROR node: node, death lexeme, kind SYNTAXIS /
LIMEN / INTERMISSIO, terminal), appended as the LAST field so consumers
reading silva.h against a not-yet-regenerated silva.c keep valid offsets.
`silva_mortes_diagnostica` (silva_frons, where the origin hook lives)
turns them into materia diagnostics: primary at the death lexeme ("hic
exspectatur"), related at the node's first lexeme ("hic coepit"); the
frontier-limit and interruption kinds become MONITUM with honest causes
(they are the apparatus's limits, not the source's faults — before T15
they printed the syntax message).

Two things surfaced by failing runs:

- **aedilis refuses absolute paths** (it prefixes the repo root: every
  absolute path reads as unreadable → "plagula imparsabilis", which
  looked like a syntax refusal). T14's gate fixture lived in mktemp, so
  its closure silently fell back to the latina lexicon — the gate had
  never exercised the closure path for the plant. Now: absolute paths
  under the root are made relative; outside the repo, a MONITUM names
  the reason and judgment proceeds on the fallback. The gate's plant
  lives in build/fumus_diagnostica/ (cleaned on exit); a second fixture
  outside the repo pins the MONITUM.
- **emissa must go to the containing root's call.** The supersede rule
  (materia) acts within one derivation; conditional weaving can nest an
  ERROR node inside a top-level `conditionalis`, so each emitted
  diagnostic goes to the root whose extent contains its node's start.

Plants (both compile): supersede disabled → gate "diagnosticum unum"
red (+ materia XII 5 red); death points not recorded → the primary
falls back to the declared default at IX:1 (4 red). 430 house C files
still 0 diagnostics. Untested by fixture: the LIMEN and INTERMISSIO
kinds (no cheap way to trip the frontier limit from the tool). Cosmetic:
"hic coepit" on a DERIVED first lexeme is a zero-width caret (the
effective sedes of a derived token is a point at its root).

Addendum (same day): the first commissio was REFUSED by
probatio_silva_computus — its gold (silva/probationes/fixa/computus/
basis.tsv) pins deterministic memory per fixture. Cause, named: +8 bytes
of usus/apex on every fixture = the new `SilvaParsura.mortes` pointer;
base64.c +336 bytes / +2 allocations and color.c +432 / +3 = the death
records of their 4 and 5 ERROR nodes (the fixtures parse -nudum, without
the header closure). lexemata/errores columns identical. Gold regenerated
with COMPUTUS_SCRIBERE=1 under this cause.

## 2026-09-25 — the system lexicon (silva-migratio T16a)

tools/diagnostica now composes the system lexicon with
`silva_lexicon_componere` — the common module examen already uses
(ISO whole + POSIX sections derived from the file's own `#include <…>`
+ its `externa` blocks) — so the house's two C judges agree on context.
Before, it had only house headers via aedilis (`<stdarg.h>` is `S`,
nothing supplied), so `va_arg`/`offsetof` were syntax errors here even
after the stand-in fix. A malformed `externa` annotation stops judgment
by name (never a clean result); an unreadable stand-in is a MONITUM.
Sweep: 431 files (incl. vendor/sqlite3.h) — 0 diagnostics: unlike latina
at T14, the ISO stand-in does not clash with vendor headers.
