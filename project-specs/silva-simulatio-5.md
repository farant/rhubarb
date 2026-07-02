# Silva — Simulatio ⑤: SPPF-style metamorphosis + same-wave packing (2026-07-02)

**Question**: the Phase 4 Chunk A complexity analysis identified an escalation
path (SPPF-style node metamorphosis / Farshi re-reduction) for GLR packing
misses, with three named downsides. Fran asked: pretend-implement it and see
whether the downsides can be designed around.

**Target**: same-wave reduction packing + in-place node metamorphosis inside
the REAL silva_glr.c (Phase 4 Chunk A, 71/71), against the real silva_nodus.c
S32 machinery and the baked sceletum tables.

**Context documents**: phase-log Phase 4 INTENTIO + Chunk A complexity
("list-append purity vs forks"); the escalation-map conversation (unequal-depth
reconvergence is ROUTINE in real C89 — declaration arm reaches block-item in 2
waves, expression arm in 3); the A½ length-view list design (correctness floor,
agreed, not yet implemented).

---

## Update stream (pretend-time)

- **t+5** — Scope: packing + metamorphosis vs downsides (1) S32 purity breach,
  (2) retroactively-wrong equality/resolutor decisions, (3) test-grammar cost.
- **t+10** — Fabrica callbacks: engine can't know the AMBIGUUS genus (it is
  table-agnostic). `ambiguum_creare(piscina, a, b)` + `ambiguo_addere(amb, c)`
  supplied at creare, implemented in the GENERATED file. Needed for plain
  Chunk B packing anyway — metamorphosis adds no new plumbing.
- **t+15** — Detection index per drain, keyed `(status, basis)` — status alone
  is WRONG (same state via different basis = different extent; packing across
  extents corrupts spans). Reduce-created nodes have exactly one predecessor at
  creation → key is two pointers.
- **t+20** — The index dispatch IS the instrumentation: hit+undrained → pack;
  hit+drained → metamorphose; same status different basis → Farshi-class event
  (count, keep separate arms, graceful ladder). Counters fall out for free.
- **t+25** — `_transmutare` draft 1 mutated genus/loci by hand = the feared S32
  breach. Draft 2: **build-then-swap** — clone n_a through silva_nodus_creare;
  build fresh AMBIGUUS through creare+ponere (ALL S32 checks run); then ONE
  struct copy `*n_a = *ambiguum;`. Old allocation becomes the AMBIGUUS node;
  downstream pointers see it; no slot check bypassed.
- **t+30** — Downside 1 DISSOLVED. Contract restated: *contents always
  constructed through ponere; node identity re-bindable by the engine until
  commit.* pater NIHIL on both sides during speculation (S27) — copy carries
  no stale links.
- **t+35** — Downside 2 attack-surface inventory: constructions consume child
  POINTERS only; the only content readers in a drain are structural equality
  and the (future) resolutor. Narrow surface.
- **t+40** — Adversarial case 1 (equal-but-distinct twins, one containing the
  transmutee): UNREPRESENTABLE. Twins at one `(status, basis)` key are packed
  at creation by the index itself; they never coexist past their birth wave.
- **t+45/50** — Adversarial case 2 (cross-key equal-value fusion inside a
  drain, later invalidated): cannot be ruled out by construction → closed by
  an ORDERING RULE instead: structural equality is consulted only (a) at pack
  time on the two candidates at one key, (b) at the frontier merge (post-
  drain). Cross-key structural fusion inside drains is banned. Chunk A never
  fuses in-drain, so this bans nothing existing; it constrains Chunk B by one
  sentence.
- **t+55** — Cross-token safety: transmutation only targets values of nodes in
  the CURRENT drain's index; prior-token values are reachable only as popped
  children. Metamorphosis window = exactly one drain. Frontier-merge fusions
  are permanently safe. Downside 2 CLOSED.
- **t+60** — Lurking edge: reduce-created node colliding with a shift-created
  frontier node at the same state would evade the index. LR theory: every
  state has a unique accessing symbol → shift-entered and goto-entered states
  are disjoint. CHECKABLE FROM THE BAKED TABLES: per state, collect incoming
  symbols (shift actions targeting it + goto rows), assert singleton. Added to
  silva_glr_tabulam_validare (pretend). Theory property → load-time assertion.
- **t+65** — Resolutor reads share the case-2 hazard shape → move ALL
  semantics out of the drain: pack always wraps AMBIGUUS (pure structure);
  oracle resolution/collapse happens in the COMMIT WALK (post-accept — which
  Chunk C builds anyway for pater fixup + A½ list normalization). Drain
  becomes semantics-free. Chunk C design SIMPLIFIES (resolutor-at-merge
  deleted from the plan).
- **t+70** — Honest cost of semantics-free drains: known-typedef statements
  still fork and build both interpretations (real C = mostly known typedefs).
  Arms live from the STAR conflict to statement end — bounded ~2× per
  declaration-statement. Mitigation NAMED, not built: optional action-filter
  hook at conflict cells (classic lexer-feedback pruning), off by default,
  gated on mensura data.
- **t+75** — Test grammar: ONE extra unit production on the expression arm of
  a sceletum VARIANT (sententia → mandatum, mandatum → sententia-expressionis)
  → arms reconverge at depths 2 vs 3 → guaranteed same-wave miss → guaranteed
  metamorphosis fire. generare.sh emits variant tables beside the main ones.
  Downside 3 CHEAP.
- **t+80** — Trace on the variant: arm A (declaratio) drains wave 2, appends
  in place; arm B arrives wave 3 → index says drained → transmutare → the
  list's already-stored element pointer now shows AMBIGUUS(decl, expr),
  canonica 0. List appended ONCE. Localization statement-level, perfect.
- **t+85** — Three staggered arms: hit+drained+already-AMBIGUUS →
  ambiguo_addere appends interpretation (one extra branch). A½ interaction:
  none (interpretationes list built fresh through the normal path). A½ stays
  underneath as correctness floor — segmentation-disagreement forks can't be
  localized by ANY packing; views are what keep them safe.
- **t+90** — Counters (fusiones, transmutationes, eventa_farshi) → mensura
  output; the C89 corpus will report real-world frequencies. Incolumitas needs
  nothing new (interior mechanism; deterministic specimens already run the
  full pipeline).
- **t+95** — Verdict + complexity entries written. ~1h35m pretend-time; the
  t+40–65 crisis/resolution was the substance.

---

## Complexities (four-part schema)

**C1 — The S32 breach dissolves via build-then-swap.**
Discovered-while: drafting _transmutare against the real silva_nodus_ponere.
Consists-in: metamorphosis needs to re-bind a node's identity, which looked
like slot mutation; but building the clone AND the ambiguus wrapper through
the normal checked constructors and then struct-copying onto the old
allocation achieves identity re-binding with zero bypassed checks.
Consequences: the purity contract splits cleanly into "contents via ponere
always" + "identity re-bindable by the engine until commit"; one interior
function owns the swap.
Handled-by: _transmutare in silva_glr.c (Chunk B scope), contract documented.

**C2 — Retroactivity closes with an ordering rule, not machinery.**
Discovered-while: constructing adversarial traces for stale equality
decisions.
Consists-in: same-key twins are unrepresentable (the index packs at
creation); the only remaining hazard is cross-key structural fusion inside a
drain; cross-token fusion is safe because the metamorphosis window is one
drain (per-drain index lifetime).
Consequences: structural equality is legal only at pack-time-per-key and at
the frontier merge. Resolutor/semantics move to the commit walk entirely.
Handled-by: Chunk B design constraint (one sentence) + per-drain index +
semantics-free drains.

**C3 — Accessing-symbol uniqueness becomes a load-time table assertion.**
Discovered-while: checking whether shift-created frontier nodes can collide
with reduce-created nodes at one state (would evade the index).
Consists-in: LR states have a unique accessing symbol, so shift-entered and
goto-entered state sets are disjoint — an assumed theory property the
validator can verify from the baked rows.
Consequences: assumption → assertion; any future generator bug that breaks it
fails at creare, not as a silent packing miss.
Handled-by: extend silva_glr_tabulam_validare (Chunk B).

**C4 — Semantics-free drains cost ~2× on known-typedef statements.**
Discovered-while: moving the resolutor to commit (C2 fallout).
Consists-in: without in-parse pruning, every typedef-shaped fork builds both
interpretations even when the oracle knows the answer; bounded (conflict to
statement end) but pervasive in real C.
Consequences: throwaway AMBIGUUS nodes + double construction per declaration
statement; collapsed at commit.
Handled-by: mensura counters decide; optional action-filter hook (lexer-
feedback pruning) is the named mitigation, off by default, post-M1.

**C5 — Farshi-class events: detected precisely, still graceful-only.**
Discovered-while: index dispatch design (t+20).
Consists-in: same-status different-basis arrivals (late NEW-EDGE
reconvergence) are the case metamorphosis cannot fix; they keep separate arms
and degrade per the graceful ladder.
Consequences: if eventa_farshi ever fires meaningfully on the real corpus,
the principled response is identity-handle values in the merge layer, not
bolted-on Farshi.
Handled-by: counter now; decision deferred to corpus evidence.

---

## Verdict

Downsides (1) and (2) dissolve under concrete design; (3) costs one variant
grammar file. Remaining honest costs: bounded always-fork overhead (C4, with
named mitigation), Farshi-class staying graceful (C5, now precisely counted),
and ~half a chunk of added Chunk B scope (index + fabrica + transmutare +
variant grammar + tests).

**Recommendation change vs the pre-simulation plan**: metamorphosis moves
from "escalation, build on measured need" to "fold into Chunk B now" — it
shares the index, fabrica plumbing, and ordering rule with same-wave packing;
building them together is cheaper than staging, and the localization
guarantee stops depending on grammar depth accidents entirely.

Revised sequence:
1. **A½** — length-view lists (correctness floor; unchanged).
2. **Chunk B** — structural equality + frontier merge + same-wave packing +
   metamorphosis + counters + accessing-symbol validation + variant grammar
   probatio.
3. **Chunk C** — SIMPLIFIED: commit walk = pater fixup + list normalization +
   oracle resolution/collapse + re-canonicalization API (resolutor-at-merge
   deleted).
4. Post-M1, evidence-gated: action-filter pruning hook (C4), identity-handle
   refactor (C5).

## Open questions for Fran (interview candidates)

1. C4 accept-or-mitigate-now: is bounded 2× on known-typedef statements
   acceptable for M1, with the pruning hook deferred? (Simulation says yes;
   mensura will tell us the real number.)
2. Should the variant grammar live permanently in grammatica/ as a second
   committed test grammar (drift-guarded like sceletum), or be generated
   on-the-fly by the probatio?
3. The ordering rule bans cross-key structural fusion inside drains — this
   means within-drain duplicate arms (genuinely equal derivations) survive to
   the frontier merge. Frontier width grows slightly in exchange for safety.
   Any concern, or accept and measure?
4. Does the "identity re-bindable until commit" contract belong in
   silva/CLAUDE.md VISIO-adjacent notes (it is a data-model commitment), or
   is the phase-log + header comment enough?
