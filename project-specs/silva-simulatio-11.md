# Silva Simulatio XI — fluxus stage 0 (CFG + attingibilitas)

**Date**: 2026-07-17. **Target**: the FLUXUS-0 INTENTIO's contested
surface — (a) the module boundary (what fluxus may depend on, what
semantica must lend it), (b) the builder walk against REAL tree
shapes, (c) the codex 63/64/65 predicates against REAL oracle
behavior. **Method**: pretend-implementation against SHIPPED code —
real reads (semantica walk + struct, silva_nodus.h, live arbor.sh
parse dumps), real oracle runs (Apple clang 16 specimens), and a
corpus-wide control-flow shape survey (subagent report, counts
below). Sim ledger going in: 6-for-6 at reducing work.

---

## Session log (five-minute ticks)

- **T+0:05** Specimens written (lapsus/inattingibilis/reditus_deest),
  house-flag baseline run. House wall is BLIND to fall-through and
  unreachable code; only -Wreturn-type lives in -Wall.
- **T+0:10** Candidate flags run. -Wimplicit-fallthrough fires at the
  RECEIVING label, path-based, comments do NOT suppress (GCC would);
  label stacking silent. -Wunreachable-code fires post-jump;
  return-after-infinite-loop needs -aggressive.
- **T+0:15** Substrate read: _elementum_ambulare dispatch, statement
  genus enum complete (FRANGE/PERGE/SALTA have nodes, no current
  consumer — fluxus is their first reader).
- **T+0:20** silva_nodus.h whole-read. Prospectus lista idiom;
  silva_nodus_liberi for generic child walks; nodes = uniform head +
  tagged slots — blocks can reference `constans SilvaNodus*` freely.
- **T+0:25** WALL: _canonicum is INTERIOR to semantica, and fluxus
  cannot include semantica (dependency points semantica → fluxus).
  Same story for the constant aestimator. Noted, kept walking.
- **T+0:30** Second oracle round (ansae.c): clang -Wreturn-type is
  SILENT on while(1), for(;;), **while(2-1)** — it FOLDS constants —
  and silent on the goto self-loop. Only while(x) fires. The goto
  case is pure reachability (free for us); the folding means the
  aestimator seam is PARITY-REQUIRED, not optional.
- **T+0:35** arbor.sh stress parse: **the grammar REJECTS Duff's
  device** — casus label inside a nested dum ⇒ whole-function ERROR
  node. Legal C89, foreclosed by the casus-group tree shape.
- **T+0:40** Bisect: statements-before-first-case parse fine (plain
  elementa before the first casus group); titulatum + salta inside a
  casus group parse fine. Only Duff dies.
- **T+0:45** Corpus survey lands (subagent). 33 salta, ALL forward
  cleanup/fallback jumps; one jumps ACROSS sibling alioquin arms
  (arbor_syntaxis.c:4460 → 4465). Zero backward gotos.
- **T+0:50** Survey continued: **exactly ONE genuine switch
  fall-through in the entire corpus** — arbor_syntaxis.c:3491→3504,
  conditionally guarded (`si` around the frange), marked with an
  English `/* Fall through */` comment, in FROZEN v1 code. Zero
  unconditional fall-throughs, zero brace-less switch bodies, zero
  statements-before-first-case in real code, zero Duff.
- **T+0:55** Survey continued: 32 infinite-loop sites (16 dum VERUM,
  4 fac-dum VERUM, 12 per(;;)); literal `dum (1)` never occurs —
  always VERUM (expands to 1 → post-expansion tree carries the
  literal). TWO functions whose ONLY exit is redde inside dum(VERUM):
  silva_lexema.c:206 _aspicere_eff, tabula_characterum.c:903/917.
  These are the named -Wreturn-type parity traps.
- **T+1:00** Survey: the every-arm-returns switch with NO trailing
  return is the DOMINANT idiom for classifier functions
  (officina_machinula.c:147 _canonicum, flatura.c:1932
  flatura_status_nuntium, the string-name switches). Codex 63 must
  be a reachability predicate, never a "has trailing return" check.
- **T+1:05** Stacking shape verified live (conflict between survey's
  worklog memory and genera-c89.md): arbor.sh says SIBLING groups,
  first with sententiae [0]. genera doc wins. Flat group scan holds.
- **T+1:10** API drafting begins. FluxusAuxilia seam decided: the
  canonicalization is a POLICY (the _canonicum comment says so
  itself — "politica: descensus canonicus"), and VISIO says
  editor-facing modes may want other policies. So it stays with the
  caller: `{canonicum fn, aestimator fn, contextus}` handed to the
  builder. Unit tests pass a 10-line toy aestimator; semantica
  passes thin wrappers over its interior machinery.
- **T+1:15** Struct drafting. FluxusBlocus BY VALUE in a Xar (xar
  element pointers are stable across appends — the 2026-07-02
  Correctio — so edges hold FluxusBlocus* safely, zero per-block
  allocations). Edges (FluxusMargo) by value in a per-block Xar.
  Caller's piscina throughout.
- **T+1:20** Edge genera drafted: SEQUENS, VERUS, FALSUS, CASUS,
  ORDINARIUS, RUPTURA, PERSECUTIO, SALTUS, REDDITUS, CADIT, LAPSUS.
  LAPSUS is the load-bearing one: the group-boundary-crossing
  sequential edge, tagged AT CONSTRUCTION, so codex 64 is a filter
  (LAPSUS edge + source reachable + previous group non-empty), not
  a graph search.
- **T+1:25** Builder walk, straight-line + si. Blocks are flow-runs,
  not scopes — nested corpus recurses into the SAME block. si closes
  the block with VERUS/FALSUS edges; constant conditions fold via
  the aestimator (uniform with loops — parity-closer).
- **T+1:30** Loops. dum: head block, VERUS→body, FALSUS→after
  (omitted when constant-true), body-end→head. fac_dum: body first,
  condition after, persecutio targets the CONDITION block (C
  semantics). per: persecutio targets the PASSUS block; absent
  condition = constant-true. One stack of Frangibile {ruptura,
  persecutio} contexts; commutatio pushes ruptura-only (perge skips
  it correctly by stack search).
- **T+1:35** Commutatio. Dispatch block ends the current run; flat
  scan of the corpus lista: plain pre-label elementa open a block
  with NO inbound edge (naturally unreachable — matches clang);
  each casus/ordinarius group opens a label-headed block with a
  CASUS/ORDINARIUS edge from dispatch; an open previous group end
  contributes the LAPSUS edge. No ordinarius ⇒ dispatch gets an
  edge to after-switch (the no-match path) — REQUIRED for codex 63
  parity (commutatio_incompleta specimen fired).
- **T+1:40** Labels + goto. Two-pass: pass 1 pre-creates a block per
  titulatum (function-scoped label table, chorda mensura+memcmp);
  pass 2's salta emits SALTUS edges to pre-created blocks — forward
  jumps and the cross-arm jump (arbor_syntaxis:4460) cost nothing.
  The label block also joins its arm's normal sequence (titulatum
  closes current with SEQUENS into it).
- **T+1:45** Function end: open block → CADIT edge to the synthetic
  exitus. Reachability = one DFS from introitus. Codex 63 predicate:
  non-void reditus_currens && a CADIT edge with reachable source.
  The goto self-loop, while(VERUM)-only-exit functions, and the
  every-arm-returns switches all come out RIGHT with zero special
  cases. This is the moment the design clicked.
- **T+1:50** WALL: noreturn. If the corpus ever does `ordinarius:
  exire(I);` in a non-void function with no return, clang stays
  silent (exit is noreturn via macOS headers) and codex 63 fires —
  a parity leak. v1 has no noreturn knowledge. DECISION: measure
  first — the differentia corpus sweep will COUNT the disagreements
  before we design the fix (options: tiny house list exit/abort/
  _exit/perire, alienity-table-style; or nothing if count is 0).
- **T+1:55** Robustness sweep: frange/perge with empty context stack
  → drop edge, never crash; ERROR statements → opaque, flow passes
  through (conservative); AMBIGUUS → canonicum callback, still-
  ambiguous → opaque; CONDITIONALIS → descend RAMUS_SUMPTUS
  transparently (taken track, per INTENTIO pin).
- **T+2:00** Integration point: _definitionem_ambulare, AFTER
  _corpus_ambulare (aestimator then has all typations). sem grows
  `Xar* fluctuum` — CFGs SURVIVE the walk, queryable later
  (code-is-a-database pin honored, solarium path open).
- **T+2:05** Codex 64 pass drafted: iterate LAPSUS edges of each
  CFG; fire at receiving label when source reachable && previous
  group non-empty; TOLERA-bilis. Expected census: ONE, frozen,
  comment-marked. The comment-idiom question nearly dissolves —
  recommendation: TOLERA-only machinery, frozen site stands as
  standing census like codex 61's (or one TOLERA if we ever touch
  arbor_syntaxis).
- **T+2:10** Codex 65 (stretch) drafted: unreachable non-empty block
  → fire at its first statement. Plain -Wunreachable-code semantics
  only; the -aggressive return variant stays deferred by name.
- **T+2:15** Unit suite sketch: parse snippets, build with toy
  auxilia, assert (block count, edge genera, attingibilitas set,
  cadit flag) over: linear / si / si-alioquin / all three loops /
  per(;;) / commutatio {full, no-default, stacking, pre-label
  statements, guarded-frange fall-through} / titulatum+salta
  {forward, backward, cross-arm} / statements-after-redde /
  conditionalis transparency. Then the semantica-side probes for
  63/64/65 in probatio_silva_examen (existing pattern).
- **T+2:20** Differentia dry-run reasoning: corpus compiles under
  -Werror ⇒ codex 63 census MUST be 0; every clang -Wreturn-type
  disagreement over 820 files = a CFG construction bug or the
  noreturn gap, each one attributable. The parity tier is the CFG's
  acceptance test. Only after it holds do 64/65 ship judgments.
- **T+2:25** Amalgam stance: fluxus is an INTERNAL module v1 —
  amalgamator manifest rows + semantica include, but NOT exported
  in silva.h (path back: solarium query surface exports a wrapped
  view when quaestio grows flow pseudo-selectors). silva.h
  hand-mirror only carries the new codex enum rows.
- **T+2:30** Size estimate settles: fluxus.{h,c} ~650-800 lines,
  semantica integration + tier 63 ~150, tier 64 ~80, tier 65 ~60,
  unit suite ~400, fixtures + vectis legs as usual. Chunkable
  exactly along the INTENTIO's consumer order.

## Register (four-part schema)

### XI-1 — Auxilia seam: canonicalization and folding are CALLER policy
- **discovered-while**: hitting the _canonicum wall (T+0:25) and the
  while(2-1) folding pin (T+0:30) in the same half-hour.
- **consists-in**: fluxus needs canonicalization at every dispatch
  and constant evaluation at every condition, but both live interior
  to semantica, which fluxus cannot include (dependency direction).
  _canonicum's own comment declares itself a POLICY; VISIO reserves
  alternate policies for editor modes. Clang FOLDS loop conditions
  (while(2-1) is infinite to -Wreturn-type), so a literal-only
  check is not parity.
- **consequences**: without a seam, either fluxus duplicates policy
  (two homes) or depends on semantica (cycle). With callbacks, unit
  tests need only a toy evaluator; alternate canonicalization
  policies stay possible.
- **handled-by**: `FluxusAuxilia {canonicum, aestimator, contextus}`
  parameter to the builder; NIHIL members = identity/nothing-
  constant. Semantica passes wrappers over _canonicum and
  _constans_probare.

### XI-2 — Grammar rejects Duff's device (whole-function ERROR)
- **discovered-while**: arbor.sh stress parse (T+0:35).
- **consists-in**: a casus label inside a nested loop within the
  switch body fails the casus-group grammar; the function parses as
  one ERROR node. Legal C89, unrepresentable in the current tree.
- **consequences**: SIMPLIFICATION — dispatch-edge discovery is a
  flat scan of the commutatio corpus lista (no recursive label
  hunt). But a legal-C89 program that parses only as ERROR is a
  named grammar limitation touching the robustness bar (tree +
  diagnostics still produced, so the bar technically holds).
- **handled-by**: flat scan in the builder; STORE ITEM filed for the
  grammar gap (adjudicate separately — likely accept + document;
  corpus census of Duff = 0).

### XI-3 — noreturn parity gap, measured before designed
- **discovered-while**: walking `ordinarius: exire(I);` shapes in
  the pretend build (T+1:50).
- **consists-in**: clang knows exit/abort are noreturn (host
  headers); fluxus v1 does not. A non-void function ending in a
  noreturn call with no redde: clang silent, codex 63 fires.
- **consequences**: potential false-positive class for the parity
  tier; unknown corpus frequency.
- **handled-by**: differentia corpus sweep COUNTS it first (the
  sweep is the tier's acceptance test anyway); if nonzero, a tiny
  house noreturn list (alienity-table pattern) is the named fix.
  No design until the number exists.

### XI-4 — Casus stacking = sibling groups with empty sententiae
- **discovered-while**: survey's worklog memory contradicted
  genera-c89.md (T+1:05); resolved by live parse.
- **consists-in**: `casus 0: casus 1: x;` parses as two SIBLING
  casus nodes, first with sententiae [0] — not nested.
- **consequences**: the clang rule "stacked labels are silent" maps
  exactly to "previous group's sententiae count == 0"; no special
  stacking machinery.
- **handled-by**: the LAPSUS filter's non-empty condition; a unit
  probe pins the shape so a future grammar change breaks loudly.

### XI-5 — Blocks by value in Xar; zero per-block allocations
- **discovered-while**: choosing storage (T+1:15) against the
  MEMORY.md xar pin (stable element pointers, segmented).
- **consists-in**: FluxusBlocus lives BY VALUE in Xar; edges hold
  FluxusBlocus*; pointers stay valid across appends by xar's
  segmented design; everything in the caller's piscina.
- **consequences**: no allocation churn, no ownership questions,
  CFG lifetime = piscina lifetime (matches semantica).
- **handled-by**: struct layout as drafted; swap-remove/sort never
  used on these xars (the disturbing operations).

### XI-6 — CADIT-reachability IS the codex 63 predicate
- **discovered-while**: checking the goto self-loop, the
  while(VERUM)-only-exit functions, and the every-arm-returns
  switch idiom against the drafted predicate (T+1:45).
- **consists-in**: "non-void && exists CADIT edge with reachable
  source" gets all three corpus idioms right with no special cases,
  PROVIDED constant-true conditions omit their FALSUS edge and
  no-default switches emit the no-match edge.
- **consequences**: the two only-exit-in-loop functions
  (silva_lexema.c _aspicere_eff, tabula_characterum.c:903) are the
  canonical validation specimens — if either disagrees with clang,
  the builder is wrong.
- **handled-by**: predicate as stated; those two functions become
  named unit fixtures.

### XI-7 — Fall-through census ≈ 1, frozen, comment-marked
- **discovered-while**: corpus survey (T+0:50).
- **consists-in**: one genuine fall-through corpus-wide
  (arbor_syntaxis.c:3491→3504), conditionally guarded, English
  /* Fall through */ comment, frozen v1 code. The jump-heuristic
  cannot see it (guarded frange); only the CFG can.
- **consequences**: the honor-the-comment-idiom question is nearly
  moot — one site, frozen, and clang ignores comments anyway.
- **handled-by**: recommendation TOLERA-only machinery; the frozen
  site stands as standing census (codex 61 precedent). Fran
  adjudicates at census time.

### XI-8 — Integration + queryability
- **discovered-while**: placing the build call (T+2:00).
- **consists-in**: build in _definitionem_ambulare after
  _corpus_ambulare (aestimator has typations); sem grows
  `Xar* fluctuum`; CFGs survive analysis.
- **consequences**: code-is-a-database honored; solarium's
  "show unguarded-return paths" stays a query away; tiers 63/64/65
  are all POST-BUILD passes over the stored CFG, not walk-time
  hacks.
- **handled-by**: sem field + build call + tier passes in the same
  place diagnostics already accumulate.

## Arc chunk plan (recommended)

1. fluxus.{h,c} + probatio_silva_fluxus (toy auxilia) — the module
   alone, unit-proven.
2. Semantica integration + codex 63 + THE DIFFERENTIA SWEEP (the
   CFG's acceptance test; noreturn count falls out here).
3. Codex 64 + census + adjudication (expected: one frozen site).
4. (stretch) Codex 65, plain semantics.

Each chunk ends green (suites + vectis + amalgam VERIFICATUM).

## Open questions for Fran

- noreturn stance if differentia count > 0: house list now, or
  standing-census until flow analysis grows summaries?
- Fall-through suppression: TOLERA-only (recommended) vs also
  honoring /* fall through */ comments?
- Duff grammar gap: accept + document (recommended) vs grammar work?
- fluxus internal-only v1 (recommended) vs exported in silva.h?
