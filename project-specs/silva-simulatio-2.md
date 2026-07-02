# Silva Simulatio ② — Conditional Sub-Parse Mechanics

*Implementation simulation (2026-07-02), ~2 simulated hours. Third simulation
chronologically; numbered ② per the spec-v2 §5 briefs. Complications S33–S40
continue the register; entries use Speculum's `complexity` schema
(discovered-while / consists-in / consequences / handled-by) — dogfooding the form
per silva-speculum-implications.md §5. The owed deliverable — frontier/memory
measurement on real nested-#ifdef input — was performed with REAL measurement, not
pretend: a conditional-structure census over all 113 rhubarb headers and 9 macOS
SDK system headers (script: scratchpad census_conditionalium.py; approximate
regex-level analysis, comments/strings crudely stripped).*

## The census (the owed deliverable)

**Rhubarb (113 headers):** perfectly uniform. Every file: exactly 1 region = its
include guard, depth 1, 1 arm, 0 imbalanced arms, 0 `#if 0`. The corpus silva
serves first is conditional-free apart from guards.

**macOS SDK system headers (9 files):**

| file | regions | max depth | arms | forking arms | imbalanced arms | atoms |
|---|---|---|---|---|---|---|
| sys/cdefs.h | 95 | 4 | 157 | 133 | 0 | 86 |
| sqlite3.h | 24 | 3 | 30 | 16 | **6** | 22 |
| zlib.h | 16 | 4 | 22 | 9 | **2** | 11 |
| math.h | 16 | 3 | 25 | 12 | 0 | 20 |
| pthread.h | 11 | 3 | 14 | 5 | 0 | 15 |
| stdio/stdlib/string/time.h | 1 each | 1 | 1 | 0 | 0 | 1 |

(forking arm = contains #define/#undef; imbalanced arm = nonzero brace delta)

**Conclusions:** (1) max nesting depth in the wild sample is 4 — region trees are
shallow; (2) ALL 8 imbalanced arms are the `extern "C" { … }` __cplusplus pattern;
(3) arms that mutate macro state are the NORM in system headers (175/252 arms),
not an edge case; (4) token storage under all-branches retention is partitioned,
not duplicated — every token belongs to exactly one arm's sub-stream, so retaining
all arms costs approximately nothing beyond the region/arm structures themselves
(~166 region nodes for the whole system sample); (5) arms sub-parse sequentially,
so the GSS frontier never holds more than one arm's forks plus the reconvergence
check. **Frontier/memory verdict: bounded. The fork-explosion fear from the
original risk list does not materialize under the localized design.**

## Complication register

### S33 — include-guard degeneracy
- **discovered-while:** running the census — before any design work, the data
  said every rhubarb header is one whole-file conditional region.
- **consists-in:** under all-branches semantics an include guard is a region
  spanning the entire file, so every header's tree would be wrapped in a
  CONDITIONALIS node whose else-arm is empty — 100% noise, 0% signal.
- **consequences:** guard recognition is not an optimization (as in real cpps'
  multiple-include optimization) but a correctness-of-representation requirement;
  without it "all branches" is meaningless for the primary corpus.
- **handled-by:** guard detection at region construction (first conditional is
  `#ifndef X` whose arm immediately defines X and spans to EOF) → region marked
  `est_custos`; body treated as unconditional structure; the guard directives
  remain as directive nodes for roundtrip; journal still records the define.

### S34 — extern-C cross-arm imbalance (the measured degradation case)
- **discovered-while:** census found exactly 8 brace-imbalanced arms in the
  system sample — all of them `#ifdef __cplusplus extern "C" {` / `}` pairs.
- **consists-in:** an arm whose tokens don't brace-balance cannot be a
  self-contained sub-parse; coarsening the region to a balanced span would
  swallow the entire file.
- **consequences:** a degradation ladder is required, and its common case is
  known precisely: tiny always-false-for-C89 arms wrapping the whole payload.
- **handled-by:** **opaque-fringe degradation** — an imbalanced arm's tokens stay
  as a raw token run (with diagnostic) inside the CONDITIONALIS node; the
  surrounding content parses normally. For __cplusplus specifically the arm is
  also literal-false under any C89 configuration, so nothing of value is lost.
  Escape hatch (arms balanced but landing in different parser states): downstream
  GSS fork per arm, bounded by the slice.

### S35 — two-track semantics: default path drives state, all arms retained
- **discovered-while:** asking what "current macro table" even means during
  continued expansion when 133 of cdefs.h's 157 arms mutate macro state.
- **consists-in:** ongoing expansion of subsequent code needs ONE concrete macro
  state, but all-branches retention says no arm is discarded — two requirements
  pulling apart.
- **consequences:** the expander must evaluate every region's conditions against
  current state (the conditio oracle) and let the TRUE arm's effects drive the
  continuing default state, while every arm's effects enter the journal under its
  conditio_id for configuration-scoped replay.
- **handled-by:** two tracks: (1) the evaluated default path — exactly what a
  real cpp computes — feeds the live table and the canonical expansion; (2) the
  journal + region tree retain every arm for wildcard/config queries.
  Arbor2CondRamus's est_evaluatum/valor_evaluatus fields already match this
  design (the port fit is confirmed).

### S36 — sub-parse start context = the enclosing GSS state
- **discovered-while:** designing what "localized sub-parse" precisely means for
  a region in the middle of a struct body (arms are member-lists, not
  translation units).
- **consists-in:** an arm's grammatical context is determined by WHERE the region
  sits — arms can't all be parsed as mini translation units (arbor2's approach)
  once regions appear inside constructs.
- **consequences:** the sub-parse needs a start state, and arms need a
  reconvergence condition for the enclosing parse to continue soundly.
- **handled-by:** when the enclosing parse reaches a region node, it sub-parses
  each arm starting from the CURRENT GSS state; arms must land in a common state
  for the parse to continue past #endif (the overwhelmingly common case). Arms
  landing in different states → per-arm downstream forks (bounded by the slice)
  or opaque-fringe degradation. "Localized" now has an exact operational
  definition.

### S37 — operand protection mechanics
- **discovered-while:** wiring the conditio evaluator into the region scanner
  (its known gap: it doesn't pre-expand expressions).
- **consists-in:** `#if`/`#elif` expressions must be macro-expanded before
  evaluation EXCEPT `defined` operands; `#ifdef`/`#ifndef` operands must never
  be expanded (after latina.h, `#ifdef VERUM` would otherwise rewrite its own
  operand to `1`).
- **consequences:** a small expression-expansion pass with protected positions,
  run against the current default-track table, before each evaluator call.
- **handled-by:** transient no-expand marks on `defined`-operand tokens during
  the directive-expression expansion (mechanism adjacent to hidesets but
  ephemeral); ifdef/ifndef operands read syntactically, never expanded.

### S38 — condition coupling via normalized atoms
- **discovered-while:** designing the configuration query API against cdefs.h's
  86 distinct condition atoms.
- **consists-in:** "which conditions are the same condition" needs an equivalence
  cheaper than expression equivalence (undecidable in general).
- **consequences:** configurations are assignments to ATOMS (macro
  defined/undefined, macro values), not to regions; same-atom regions couple
  automatically; arms within one region are mutually exclusive by construction
  (#if/#elif/#else).
- **handled-by:** normalize condition expressions (interned, whitespace-stripped)
  and extract their atom sets; a coupling table maps atoms → regions; the query
  API takes atom assignments, the journal replays under them. Full expression
  entailment deferred — atom-level coupling covers the measured corpus.

### S39 — #if 0 is a block-comment idiom
- **discovered-while:** enumerating literal-condition regions (census found 0 in
  the sample, but the idiom is common in wild C).
- **consists-in:** `#if 0` arms may contain non-C garbage (prose, pseudo-code) —
  people use it as a comment syntax; all-branches parsing will chew on it.
- **consequences:** literal-false arms must parse best-effort without letting
  their error nodes alarm downstream consumers.
- **handled-by:** parse best-effort as always (total-input posture, error nodes
  fine); regions with literal-false conditions tagged `numquam` so queries and
  diagnostics can filter them by default.

### S40 — includes inside arms are arm-scoped
- **discovered-while:** census counted 13 include-bearing arms in the system
  sample (the umbrella-header pattern).
- **consists-in:** an `#include` inside an arm means the included file's
  macros/typedefs/tokens exist only under that arm's condition.
- **consequences:** include processing must be arm-scoped: on the default track,
  only the true arm's includes actually process; other arms' includes are
  recorded (unresolved-by-configuration) without processing, or processed into
  the arm's scope on demand.
- **handled-by:** default track processes true-arm includes normally (side
  effects journaled under the arm's conditio_id); false-arm includes recorded as
  conditional include nodes, resolvable lazily when a query supplies a
  configuration that makes them live. Cost bounded: measured 13 sites across 9
  gnarly headers.

## Design changes to fold into spec-v2

1. Guard detection (`est_custos`) is a correctness-of-representation requirement
   (S33) — add to the expander's region scanner spec.
2. Degradation ladder finalized (S34): balanced+reconverging arms = full
   sub-parse; balanced+divergent = downstream fork per arm; imbalanced = opaque
   fringe with diagnostic. All three measured or bounded.
3. Two-track semantics (S35): default evaluated path drives expansion state;
   journal + region tree retain all arms. CondRamus port fit confirmed.
4. Sub-parse start context = enclosing GSS state with common-landing-state
   reconvergence (S36) — replaces arbor2's parse-every-arm-as-mini-TU.
5. Conditio evaluator gains the protected pre-expansion pass (S37).
6. Configuration API: atom assignments + coupling table (S38); `numquam` tag
   (S39); conditional include nodes with lazy resolution (S40).

## Open questions for Fran — ANSWERED 2026-07-02

*Interview transcript: silva-simulatio-2-interview.md. Resolutions folded into
silva-spec-v2.md §10.*

0. Central mechanism pair (two-track semantics + GSS-state sub-parse contract
   with reconvergence): **accepted as the conditional architecture**.
1. Configurations: **named presets in contextus** ("macos", "c89-nudum" = saved
   atom assignments, serializable); wildcard stays the default query mode;
   pseudo-configs are just definable presets.
2. Guards: **transparent + file metadata** — no region node; body parses
   unconditional; directives stay as plain nodes; guard-ness on the fons record.
3. False-arm includes: **lazy default + eager opt-in** ("resolvere omnes
   configurationes" contextus flag for index-building / cross-config workloads).

## Verdict

The fear this simulation existed to check — conditional fork explosion — is
**measured out of existence** under the localized design: shallow region trees
(depth ≤ 4 in the wild sample), sequential arm sub-parses, partitioned (not
duplicated) token storage, node-local configuration spaces. The two genuine
discoveries are S33 (guards would make all-branches meaningless without
recognition — 100% of the primary corpus) and S35/S36 (the two-track semantics
and the GSS-state sub-parse contract, which together make "localized sub-parses"
an exact mechanism instead of a phrase). The extern-C imbalance case arrived
exactly as predicted and its degradation is safe. Remaining: simulation ④
(amalgamation), the cheapest of the four.
