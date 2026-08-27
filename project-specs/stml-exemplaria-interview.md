# STML Exemplaria — the informal interview (rounds I–IV)

> Provenance: conversational design session 2026-08-26, same day the
> macros v1 milestone closed (parcum 01M0XJQAVP → decretum
> 01M0ZDTCAB). These four rounds happened organically after the three
> v1.5 decrees (macros spec §6.1–6.3) and constitute most of the v2
> pattern-language interview. Per house culture (raw transcripts
> precede specs), this file preserves the reasoning; the ledger notas
> on 01M0ZDTCAB are the compressed twin. A short FORMAL interview on
> the remaining open items should precede the v2 spec; that spec
> should be assembled from this file, not re-invented.
>
> Standing doctrine this all sits on: the pattern-cardinality thesis
> (control flow from match cardinality; reduce deliberately absent);
> "the definition IS the pattern" (templates = patterns with zero
> permissiveness; one lockstep matcher at three seams — emitter
> recognition, CASUS dispatch, query fragments); the no-expression-
> language law (everything enumerable, canon judges the plan);
> `$` binds / `&@x;` fills (match-side and fill-side visually
> distinct); document-order stratification everywhere.

## Round I — the landscape (Fran's list, organized)

Fran's raw list: inputs (file/dir scope, quaestio results, previous
pattern results, PIPE, @variable); uses (expand to nodes — alias or
copy?, narrowing, mutation targets, boolean for conditionals, value
extraction for SWITCH); requirements (composable, storable as
fragments/macros, parameterizable, promotable to quaestio pseudos,
maybe a switch construct); primitives (tag exists, has attr, attr
with value, has child, child counts, some-descendant, matches-
quaestio).

**Settled:**

1. **The unifying result type**: a pattern application yields a SET
   OF MATCHES; each match is a BINDING ENVIRONMENT (named captures →
   nodes/values). All five uses are projections of this one type:
   nodes = root bindings; narrowing = match-within-bindings; mutation
   targets = bindings; boolean = set non-empty (the cardinality
   thesis's conditional); SWITCH input = captures. One type, five
   consumers — no per-use syntax exists.
2. **Alias, never copy**: patterns BIND existing nodes. Whether a
   consumer copies (template splice) or works in place (mutation) is
   the consumer's verb, not the pattern's. (Same dual-law logic as
   transclusion.)
3. **Inputs collapse**: `de="$x"` names a binding set (default:
   document root). File/dir/glob scope is control-plane/harness
   configuration — the pattern language stays ignorant of files.
   PIPE does not need to exist (piping = referencing the previous
   pattern's binding name).
4. **Requirements are already owned**: patterns are fragments
   (storage), loculi (parameterization), calls (reuse — incl.
   subtree-valued pattern parameters via the §6.1 argument forms),
   quaestio's named-query library + user-pseudo registry (promotion
   bridge), COMMUTATIO (the switch). The only NEW build is the match
   semantics.
5. **Primitives are permissiveness markers on the template trunk**;
   the key design axis is STRICTNESS POLARITY: templates are
   strict-by-default (everything written required exactly, nothing
   omitted); query patterns are LOOSE-by-default (written = required
   existentially; omitted = unconstrained). One document form, a
   polarity flag at the pattern root — never per-node.

## Round II — surface: EXEMPLAR, captures, scoping, MAP

**Settled:**

1. `<EXEMPLAR de="$scope" modus="omnia|unum|primum|optional">` —
   CAPS builtin per §6.2's mint; `modus` = the banked quantifiers
   governing MATCH cardinality at the root.
2. **Captures**: `$`-boolean-attr binds the NODE (`<parametrum
   $par>`); `$x` in a value position binds the VALUE (`n="$n"`).
   The polarity rhyme holds: `$x` binds, `&@x;` fills.
3. **THE BRIDGE IS THE SCOPING MECHANISM** (the elegant collapse):
   inside a per-match consumer, each match's captures arrive as that
   instantiation's ARGUMENT MAP — the same mechanism a macro call
   uses. `$n` in the pattern, `&@n;` in the template, the builtin is
   the bridge. NO new variable-scoping rules; scoping = fragment
   arguments (built, tested, threaded).
4. **`<PER congruentia="abc">` absorbs MAP**: map is not a loop —
   `modus="omnia"` + a template on the bridge = one instantiation
   per match. Delegation form `voca="#@f"` free (captures-as-
   arguments IS a call). Reduce stays deliberately absent
   (aggregations = decreed projections).
5. **Wildcards**: `<*>` = any ONE element (quaestio's UNIVERSALIS,
   one vocabulary with the 1D language); `<**>` = zero-or-more
   levels (glob lineage) — answers the descendant question.
   Whole-name forms only, never suffixes.
6. **`<tag?>`** = optional node — `?` joins the `!`/`\` suffix-sigil
   family as a node flag; optional captures flow across the bridge
   as OPTIONAL ARGUMENTS, landing on §6.2's `@p?` + narrowing
   machinery (same `?`, same meaning, both sides). Lexer stays
   permissive (`.`-tag precedent); per-dialect canons judge.
7. **Qualified names `$abc.n` = the JOIN surface, reserved**: under
   the bridge model single-pattern consumption never needs
   qualification. Cross-pattern variable reference = Datalog join
   (shared `$x` within one pattern = equality constraint, already
   law; across patterns = join condition) — named and reserved,
   built when the control plane pulls.
8. **`<MATCH/>` may never need to exist in the data plane**: the
   "expand to result nodes" (alias-splice) use is the computed-VIEWS
   reservation (`<<x | transform>>` earmark, never in truth-files).
   Decide THAT before any splice-placeholder syntax.

## Round III — output=, occurrence quantifiers, the type ladder

**Settled:**

1. **`output=` sigil-typed and REQUIRED-sigil**: `output="$abc"`
   binds a MATCH RELATION (what EXEMPLAR produces — replaces
   `nomen=`); `output="@rows"` binds a FOREST (what PER/COMMUTATIO
   produce). `$` names consumed by `de=`/joins; `@` names consumed
   by `&@rows;` — a deferred forest output is just a document-scoped
   forest argument; the fill walk already splices those. ZERO new
   machinery.
2. **Asymmetric defaults from the types**: tree-producing builtins
   emit in place unless `output=` defers; EXEMPLAR REQUIRES
   `output=` (a match set is not emittable; an unconsumed pattern is
   dead code = loud vitium).
3. **Document-order stratification for outputs**: single-assignment,
   referenceable only BELOW the binding — the fragments discipline
   again. Cycles impossible by construction.
4. **The convergence**: patterns = relations, outputs = derived
   relations (IDB), consumers project, everything stratified — the
   §9.2 control-plane vision (stratified Datalog over trees)
   assembling itself from locally-justified decisions.
5. **Occurrence quantifiers, namespace disentangled**: whole-name
   wildcards (`<*>`, `<**>`) vs occurrence SUFFIXES on named
   children: `<child?/>` 0–1, `<child*/>` 0+, `<child+/>` 1+.
   NO `**` suffix. Ranges via ATTRIBUTES (`minime=`/`maxime=`),
   never `{n,m}` in tag names (lexer + reading hostility; attrs are
   canon-judgeable). Bare `<child/>` in loose mode = existential
   ("at least one") — the stated default everything deviates from.
6. **THE TYPE LADDER** (the deep consequence): scalar → forest →
   LIST-OF-BINDINGS. Quantified captures (`<child* $c/>`) bind ALL
   occurrences = list bindings. Consumption discipline mirrors the
   argument rules: lists consumable only by `<PER>` (per-element
   instantiation — nested PER = nested quantifiers, still not a
   loop) or wholesale splice; a scalar position fed a list refuses
   loud (the seventh-vitium pattern). One type discipline, three
   rungs, every mismatch loud.

## Round IV — DISTRIBUTIO (anonymous-item lists)

**RATUM 2026-08-26 (Fran, at parametrum close)** — including the
two open calls: collision rule = ITEM-WINS, completed by the §6.3
tombstone (`<@attr=/>` in an item's prefix DELETES the inherited
attribute — override AND delete verbs both exist); `<>`
rehabilitation as `<#` sugar approved (named strictum fixture
change). Build = parcum step 4.

**BUILT 2026-08-27** — spec §6.4 carries the full build record
(`stml_distribuere` in lib/stml_macros.c;
`canon_iudicare_distributum` = the fifth canon touch; `<>`
rehabilitated in base stml with `</>` closing fragments; amalgam
public surface + hospes #40). Build order steps 1-5 now ALL built;
only step 6 (formal interview → v2 spec) remains.

Fran's proposal, PROPOSED + REFINED:

```stml
<nota label="todo">
  <(> buy milk
  <(> fix the door
</>
```

≡ two sibling `<nota label="todo">` elements at the wrapper's
position — the wrapper DICTATES the item type, then dissolves.

- **Measured**: `<>x</>` lexes TODAY as an empty-titulus element,
  condemned by strictum (TITULUS_VACUUS, fixture ~3890) — a
  tolerated form with no meaning. The decree would rehabilitate it.
- **Recommended path**: `<>` becomes sugar for `<#>` (parallel to
  `<(>` = `<# (>` with writer normalization) → ALL anonymous forms
  are fragments, one model; the empty-titulus vitium survives for
  genuinely malformed elements (strictum fixture changes — a NAMED
  behavior change).
- **Semantics**: wrapper dissolves; N items → N wrapper-typed
  siblings; attributes carry down (fragments already carry attrs →
  per-item overrides free; collision rule OPEN: item-wins leaned,
  vitium possible); homogeneity LOUD (mixed anonymous/named children
  = vitium); empty item = empty element; items are forests;
  nesting recursive.
- **Layer**: a SECOND content-view projection — `legere → expandere
  → distribuere → consumer`. NOT inside stml_expandere: the
  engine's contract is template-space (`@`) only; distribution is
  unmarked content-space; the boundary stays clean. Fidelity: the
  tree keeps the authored sugar byte-exact; consumers opt into the
  pipeline.
- **Kinship**: a degenerate PER (wrapper = one-element template,
  items = instances) — build it as a ten-line projection, but the
  rhyme confirms coherence.
- **Canon**: judges the DISTRIBUTED meaning (fifth touch in the
  "canon judges what the document means, not its surface" line).

## Consolidated: still open for the FORMAL interview

1. Strictness escape hatches inside loose mode ("exactly these
   children, no others" — the dual of wildcards).
2. Per-position vs root quantifier attachment details (interaction
   of `modus=` with child suffixes).
3. The two-engine question: build the match engine once on the STML
   side with silva reached through the arbor projection, vs native
   silva patterns; quaestio stays the fast 1D path with
   pattern→pseudo promotion either way. Deserves a measured
   argument, not a preference.

   **MEASUREMENT PLAN (banked 2026-08-27, pre-interview; ~1 day —
   run BEFORE or DURING the interview so Q3 closes on numbers):**
   - **A. Projection tax**, per corpus file over both tiers:
     silva parse (RP baselines exist) → arbor scriptura →
     stml_legere → stml_expandere (compressed docs need the content
     view), time + peak piscina per stage. Folds in the banked
     "mensor: expansion load cost" item for free. Output: the tax
     as a multiplier over a native silva walk.
   - **B. Same real queries, both ways**: 2-3 pattern shapes we
     actually want (pointer-to-T params; piscina_allocare calls
     with a magnitudo() argument; si-conditions vs NIHIL), each
     hand-written as a walk over the arbor STML tree (proto-
     EXEMPLAR; _par_congruere is the style guide) AND over the
     silva tree. Measure wall time + LOC per implementation, and —
     the part that matters most — a DIFFERENTIAL GATE: both walks
     must yield the SAME site set, cross-checked through the
     coordinate join. Disagreement = projection-fidelity finding,
     worth more than the timings. The join audit doubles as the
     transform-direction check (formator-first: matches must map
     back to silva nodes; macro-expansion provenance is the likely
     hole).
   - **C. Amortization**: cold (project + match) vs warm (match
     only, doc cached — arbor docs are the canonical artifacts;
     a resident can hold them). If warm ≈ native, performance
     collapses to "how often do files change" and the decision is
     purely semantic.
   - **What tips it beyond speed**: one-engine dividends — patterns
     ARE STML documents, so canon judges patterns and the macro
     engine works inside patterns for free; two-engine honest
     fallback = one pattern GRAMMAR, two backend walks (drift risk
     concentrated, not eliminated). PREDICTION (labeled as such):
     one engine wins — the byte-circuit already pays projection
     over 233 files in seconds. Measure before believing.
4. How CASUS patterns share this grammar (COMMUTATIO's literal
   cases → pattern cases; presumably `<CASUS>` body IS an EXEMPLAR
   body in loose mode). Fran's sketch (2026-08-26, at COMMUTATIO
   build time): the §6.3 attribute-element BLOCK form is the
   growth path — `<CASUS><@est=><EXEMPLAR>...</></>...` gives
   `est` a subtree-valued case on a BUILTIN with zero new syntax,
   exactly as calls take subtree arguments. v1 `est` stays literal
   scalar equality; the block-form-on-builtins door stays open.
5. ~~DISTRIBUTIO ratification + the item-attr collision rule.~~
   RATUM 2026-08-26, BUILT 2026-08-27 (spec §6.4).
6. `<MATCH/>` / computed views (decide the reservation's fate before
   any splice syntax).
7. Joins (`$abc.n`) — reserved; revisit when the control plane
   pulls.

## Build order (v1.5 before v2)

1. `<@attr=>` lexer blessing + capere dual lookup (base stml, own
   gates) — §6.3.
2. Engine argument map + subtree splice + seventh vitium — §6.1.
3. COMMUTATIO + optional slots/narrowing — §6.2.
4. (If ratified) DISTRIBUTIO projection.
5. Parametrum family + the lockstep matcher (hand-recognizer
   structured as match→argument-map, replaceable by the general
   matcher) — the v2 walking skeleton.
6. Formal interview on the open list → v2 spec → EXEMPLAR/PER.
