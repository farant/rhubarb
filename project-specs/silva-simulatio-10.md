# Silva Simulatio X — the oracle dance (M2b Chunk 0)

**Date**: 2026-07-04. **Target**: the three contested designs of
M2b Chunk C — (a) the driver hook for typedef registration,
(b) the ambiguous-typedef policy, (c) the three-pass interplay
(collapse-resolutor / policy-recanonicare / registration).
**Method**: pretend-implementation against SHIPPED code (real
reads: silva_parsare.c driver loop, silva_commissio.{h,c}
oraculum + committere + recanonicare, silva_glr.c action path).
Sim ledger going in: 5-for-5 at reducing work. Register below in
the four-part schema.

---

## X1 — THE BOMBSHELL: there is no GLR-time oracle. Per-segment
## registration is unnecessary.

- **discovered-while**: pretend-writing the per-segment hook the
  INTENTIO proposed ("registration must run BETWEEN segments,
  before segment N+1 parses").
- **consists-in**: the shipped engine consults the oraculum in
  exactly ONE place — the resolutor call inside silva_committere
  — and commit happens ONCE, at the END of the driver
  (silva_parsare.c:366), after ALL segments have parsed. The
  GLR itself never asks the oracle anything (the action-filter
  pruning hook is the Phase-4 EVIDENCE-GATED, UNBUILT item).
  Parsing segment N+1 is identical whether or not segment N's
  typedefs are registered — all forks happen regardless, all
  readings are retained, and knowledge is applied wholesale at
  commit.
- **consequences**: the "dance" is not interleaved at all today.
  The correct minimal mechanism is: parse everything → ONE
  registration walk over the uncommitted radix → commit with
  the real resolutor. No per-segment callback, no driver-loop
  surgery. The INTENTIO's Chunk C shrinks. (If the pruning hook
  is ever evidence-forced by watchdog numbers, per-segment
  registration becomes necessary FOR PERFORMANCE — the design
  below leaves that door open but does not build it.)
- **handled-by**: ONE pre-commit hook (X2), positioned between
  _texere and silva_committere.

## X2 — Hook shape: the GRAMMATICA bundle grows an optional
## praecommissio member

- **discovered-while**: choosing where the hook parameter lives
  (silva_parsare already has 8 parameters; every public
  signature change ripples to silva.h + hospes).
- **consists-in**: the hook is grammar-SPECIFIC behavior, and
  the codebase already has exactly one home for grammar-specific
  behavior: SilvaGrammatica (tabula, tabularium, constructor,
  fabrica). Proposal: a 5th member
  `vacuum (*praecommissio)(SilvaValor radix, vacuum* datum)` —
  driver calls it (if non-NIHIL) after _texere, before
  committere, passing datum_resolutoris. CORRECTED (verified by
  compile, post-sim): C89 zero-fill makes 4-field initializers
  LEGAL, but the flag wall's -Wextra fires
  -Wmissing-field-initializers → -Werror FAILS every existing
  bundle initializer. Ripple: add explicit NIHIL at ~8 sites —
  trivial, and the compiler enumerates every site loudly
  (clamat, not silent). The "no consumer edits" claim in the
  original sim text was wrong; the edit is mechanical.
- **consequences**: no new silva_parsare variant; the sceletum
  bundle is untouched; the oraculum stays `constans` in the
  driver signature (the hook mutates the oracle through its own
  datum closure — the c89 wrapper packs the mutable oraculum
  into datum). The hook sees the WOVEN radix (post-_texere):
  the walk must descend conditionalis → ramus-sumptus →
  contentum (typedefs inside taken arms register; untaken arms
  are cruda tokens, nothing to register — correct, cross-arm
  knowledge is the config-query milestone's).
- **handled-by**: M2b Chunk C implements; hospes gains a bundle
  with the hook as the type-drift guard.

## X3 — THE CORRECTNESS FIND: a flat oracle breaks C89
## visibility order. The oracle must be POSITIONAL.

- **discovered-while**: pretend-running `foo * bar; typedef int
  foo;` through the X1 mechanism.
- **consists-in**: commit-time resolution sees the FULL oracle —
  including typedefs declared AFTER an ambiguous segment. C89
  visibility says foo is not a type in segment 1; a flat oracle
  would confidently collapse segment 1 to a declaration —
  WRONG, and silently so. This is not an exotic case: headers
  with forward uses, amalgamated files, and any ERROR-recovery
  reordering hit it.
- **consequences**: oracle entries need a POSITION (byte offset
  of the declaring token). The oraculum's tabula_dispersa
  currently stores a dummy value pointer per name — that slot
  is free real estate for a position record. API growth:
  addere-with-position + a query returning the position
  (existing preload API = "visible from 0", so latina/API-
  injected types behave as today; -1 = unknown). The resolutor
  counts a type as known FOR A GIVEN AMBIGUUM only if
  declaration-position < ambiguum's first-token position.
  Block SCOPING (shadowing, scope exit) is explicitly NOT this
  — M2c/M2d territory, deferred by name; positions give
  file-scope C-correctness only.
- **handled-by**: M2b Chunk C: silva_oraculum_typum_addere_
  positus (name TBD ≤31 chars) + positional novit; fixture:
  typedef-after-use stays AMBIGUOUS (both readings retained,
  policy-flipped to expression-canonical — graceful).

## X4 — Ambiguous-typedef policy: conservative-exclusive

- **discovered-while**: deciding registration for `typedef foo
  bar;` when foo is unknown (the whole element is itself an
  AMBIGUUS: declaration-reading vs expression-reading).
- **consists-in**: two candidate policies. INCLUSIVE (register
  bar from any declaration READING of an ambiguous element)
  can cascade a wrong reading into confident wrong collapses
  downstream. EXCLUSIVE (register only from UNAMBIGUOUS
  declaratio elements) under-resolves: bar stays unknown, its
  uses stay ambiguous — but every reading is retained and
  nothing wrong is ever asserted.
- **consequences**: exclusive aligns with the wildcard pin
  ("retain unless sure") and with X3's spirit (never collapse
  on uncertain knowledge). Real-world cost is near zero: files
  declare foo before typedef-ing it.
- **handled-by**: EXCLUSIVE, pinned by fixture (`typedef foo
  bar; bar x;` with foo unknown → BOTH elements remain
  ambiguous; then the same input with `typedef int foo;`
  prepended → everything resolves — the dance visible in one
  fixture pair).

## X5 — The registration walk IS the vista walk

- **discovered-while**: pretend-writing titulus extraction for
  `typedef int a, *b, (*c)(void);` — three names, each behind a
  different declarator recursion (bare, monstrator,
  parenthesis→monstrator→functionis).
- **consists-in**: registration needs "declaratio → for each
  init-declarator, chase the declarator chain to its titulus
  token" — which is EXACTLY the traversal the
  SilvaDeclaratioVista (Chunk D) needs for its titulus column,
  and what saltuarius TOC rows need. One grammar-specific
  helper serves all three: declaratoris-titulus (recursive
  through monstrator/aciei/functionis/parenthesis internum).
- **consequences**: the c89 module's walk helpers are the
  vista's foundation — Chunk C builds them, Chunk D exposes
  them. No duplication.
- **handled-by**: fontes module (name at review): the shared
  walker + typedef detector (TYPEDEF token in specificatores —
  the M2.0 semantic-mark decision executing).

## X6 — The real resolutor: reading-scoring, not name-peeking

- **discovered-while**: generalizing M2a's _titulus_typi
  resolutor to M2b's new ambiguus shapes (declaratio-vs-
  expression at statement level; `foo bar;` specifier-boundary
  forks; still conversio/magnitudo).
- **consists-in**: M2a's probatio resolutor peeked at ONE name.
  The real rule: a reading DISQUALIFIES if it contains a
  typus-nominatus whose name is NOT known-at-position; a
  reading is TYPE-POSITIVE if it contains ≥1 typus-nominatus
  and ALL are known-at-position. Victor = the type-positive
  reading when exactly one exists; otherwise no answer
  (survivor → policy pass). Readings with no typus-nominatus
  (pure expression) never win by oracle — they win by DEFAULT
  through the canonical policy, which is the correct asymmetry:
  the oracle only ever learns types.
- **consequences**: one resolutor covers every fork shape M2a+
  M2b produce, including nested-ambigui elements (resolutor
  runs per-ambiguus in the commit walk — inner and outer both
  visited). Multiple type-positive readings: not constructible
  in the current family (named; if a future shape produces one,
  the resolutor answers nothing and the case surfaces in
  fixtures rather than resolving arbitrarily).
- **handled-by**: Chunk C ships it; M2a probatio hand-resolutors
  retire in favor of the real one (probatio keeps one tiny
  contrary resolutor for the recanonicare-flip test).

## X7 — Pre-commit walk safety: verified

- **discovered-while**: checking what the hook may touch.
- **consists-in**: pre-commit values have pater UNSET (S27) and
  AMBIGUUS wrappers ALREADY EXIST (packing happens in the GLR
  drain/accept, not at commit — commit only collapses/journals).
  The walk reads genus + loci only, never pater.
- **consequences**: the X2 hook is safe on the uncommitted
  radix; the X4 policy sees the wrappers it needs.
- **handled-by**: nothing to build — a constraint stated in the
  hook's doc comment (walk = read-only, no pater).

## X8 — The wrapper owns the sequence; consumers get one call

- **discovered-while**: pretend-writing arbor.sh's switch to the
  full dance (the M2b manual bar: watching names resolve).
- **consists-in**: the full sequence is: silva_parsare (bundle
  hook registers during it, commit resolves+collapses with the
  real resolutor) → post-commit policy recanonicare (surviving
  ambigui flip to expression-canonical). Three passes, one
  owner: a c89 convenience wrapper (silva_c89_parsare-shaped)
  that packs the mutable oraculum into datum, passes the real
  resolutor, runs the policy pass, returns the parsura.
  Consumers (arbor, hospes, saltuarius-at-M2d, solarium) call
  ONE function; the pieces stay public for callers who want
  custom control.
- **consequences**: the canonical-policy shipping location
  (M2a's named handoff) is ANSWERED: it lives in the wrapper.
- **handled-by**: Chunk C; arbor.c switches to it (manual bar).

## X9 — Named degradations (correct, pinned, not fixed)

- ERROR segments register nothing (no tree) — a typedef inside
  a malformed segment is invisible to later segments; uses stay
  ambiguous. Correct under the tree-sitter bar; fixture named.
- Redefinition (`typedef int foo; typedef long foo;`): keep the
  EARLIEST position (visibility semantics); legality is the
  lint layer's, never the parser's. Verify tabula_dispersa
  insert-duplicate semantics at implementation.
- Within-declaration self-reference (`typedef int foo,
  bar[sizeof(foo)]`): C89 makes foo visible after ITS
  declarator; positional granularity is per-DECLARATION
  (registration after the whole element) — divergence named,
  cost negligible, revisit only if a corpus forces it.

---

## Verdict

The INTENTIO's Chunk C was designed against an interleaved
dance that does not exist in the shipped engine (X1). The real
mechanism is simpler: bundle hook (X2) + positional oracle (X3)
+ exclusive ambiguous-typedef policy (X4) + one shared walker
(X5) + scoring resolutor (X6) + wrapper-owned sequence (X8).
Driver change = ~6 lines + one optional bundle member. The
positional oracle (X3) is the one piece the INTENTIO missed
entirely — found by pretend-running a two-line input. Sim
ledger: 6-for-6.

**Decisions for Fran** (interview if wanted):
1. X2 bundle-member hook vs a new silva_parsare parameter.
2. X3 positional oracle NOW vs flat-with-named-divergence.
3. X4 exclusive vs inclusive ambiguous-typedef registration.
4. The c89 module's name (proposal: silva_c89_oraculum.{h,c} —
   or Fran coins something better; it will hold the walker,
   resolutor, policy, wrapper).
