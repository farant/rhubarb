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

## Round V — scope addendum: mutation, relations, dispatch, ingress
## (2026-08-27, pre-formal-interview)

Provenance: the post-DISTRIBUTIO scoping conversation (Fran +
Claude). Sequencing DECIDED: the Q3 measurement runs BEFORE the
formal interview, so Q3 closes on numbers. Fran's framing prompts:
(1) mutation commands (`<%`) as a PRIMARY long-term consumer of
EXEMPLAR patterns; (2) work BACKWARDS from operations the
multi-dialect substrate should support (silva-style parsers
eventually for C, obj-C, JS, CSS, HTML, bash, filetrees — STML as
one substrate for lint/transform across all of them); (3) macros as
the abstraction ladder over AST granularity, composable "eventually
very high, almost business level"; (4) EXEMPLAR-as-tag composing
inside macros = a pure functional language; (5) PARSE ingress
sketches; (6) least surprise for authors human or LLM.

### V.1 Mutation commands = the in-place consumer family

Settled doctrine already forces most of the interop. Round I:
mutation targets = bindings; copy-vs-in-place is the CONSUMER's
verb. Mutatio-visio: closed decreed-verb vocabulary, G1 `%`
ancestral names. Ergo `%` verbs ARE the alias-consumers of EXEMPLAR
matches (templates are the copy-consumers). The sigil TRIANGLE:
`$` binds (match side), `&@x;` fills (template side), `%` acts on
what is bound (mutation side) — consistent with data-plane `%`
(augmentation also operates on an existing thing; it stays the lone
data-plane admit).

The one real fork ("resolving to / feeding into" — Fran's two
prepositions): commands-AT-positions (mutatio-visio §3 — verbs
interspersed in the pattern; the pattern IS the address space;
match-one-edit-another unrepresentable) vs bridge-consumer (a
mutation form consumes an output= relation the way PER does —
per-match instantiation that LANDS AS AN EDIT; "a PER that writes
back", the degenerate-PER rhyme a third time). Proposed synthesis
(Claude, unratified): positional form = SUGAR over the bridge
(implicit capture at the command's position + per-match command
instantiation); one IR. The bridge form must exist regardless —
cross-position edits (definition + call sites) fill verb bodies with
`&@n;` template material captured at OTHER positions.

Convergences to PIN at the interview:
- Quantifier vocabularies are ALREADY IDENTICAL (`modus=`
  omnia/unum/primum/optional ≡ mutatio's scope quantifiers). Decree
  ONE vocabulary; the mutation consumer adds the transactional
  reading (unum = exactly-one-else-refuse; fan-out failure =
  abort-all; pattern = transaction; zero partial edits).
- Plan-first becomes STRUCTURAL: the match relation IS the plan
  (renominare culture as type system). Open: is relation-consumption
  MANDATORY for verbs?
- Non-linear capture equality = token-series equality = the differre
  normalized seal; one definition serves both sides.
- Verbs stay tool-plane FOREVER (doctrine 8); augmentation remains
  the lone data-plane `%`.
- Components-with-slots (mutatio §5) vs parameterized fragments =
  the same reuse layer approached from two directions; the interview
  decides whether they ARE the same thing (see V.3 — third pull).

### V.2 The relational layer (worked scenarios)

Three scenarios run backwards (typed-allocation lint; cross-language
internuntius audit — dead handlers / unbound calls between C
registration and JS voca; convention/policy lints, e.g. "new genus
in code needs its seed row same commit"). Every gap found was
RELATIONAL; none was pattern-side:

1. ANTIJOIN/DIFFERENCE pulled early: inequality constraints
   decompose into equality (non-linear captures) + set difference
   ("in A but not B") — the no-expression law survives ONLY if a
   difference projection exists. Rounds I-IV are silent on
   absence-across-relations. Stratification makes negation lawful
   (document-order strata = the stratified-Datalog condition,
   already house law).
2. Joins move from "reserved" to "v2 must design the RELATION
   SCHEMA" even if operators lag: first-class provenance (document
   + sedes span), serialized STML form, canon-judged. ONE artifact
   = lint report = mutation plan = join operand.
3. DOCTRINE candidate: patterns are PER-DIALECT; relations are the
   LINGUA FRANCA. No universal AST genus (the tree-sitter trap) —
   trees stay honest per dialect; sameness is manufactured in
   relations + the shared pattern GRAMMAR. Nothing in EXEMPLAR may
   ever mention a language or a file.
4. examen's 88 codices = hand-coded C at exactly this layer;
   endgame = codices as pattern documents.
5. The abstraction ladder: L1 syntax shapes → L2 idioms → L3
   conventions → L4 policy. Lower rungs = macros; upper rungs =
   relations. The relational algebra cannot tell policy from lint —
   which is why the ladder reaches "business level".

### V.3 Language identity, dispatch, staging

The macro layer is a TOTAL pure functional language: document-order
strata forbid recursion → termination guaranteed; single assignment
→ referential transparency; subtree arguments → higher-order-ish
composition; EXEMPLAR-as-tag adds structural matching;
pattern-cases add case analysis. Totality is WHY canon can judge
the plan — sub-Turing by design, stated as identity, not accident.

Dialect dispatch scenario (Fran): a macro receives a parsed file as
a subtree argument WITHOUT knowing its language; root tags are the
CONSTRUCTORS of a sum type (`<parsura>` = C, etc.); COMMUTATIO with
pattern-cases (item 4's block form — Fran's own 2026-08-26 sketch)
discriminates and delegates to per-dialect macros. The INTERFACE is
the capture schema (each arm produces `$nomen/$sedes/...`;
downstream never knows which arm ran) — typeclass dispatch. Purity
survives dispatch: patterns match ARGUMENTS (bridge-scoped), never
the world; file-ignorance holds. Item 4 is hereby PROMOTED:
load-bearing, not a door. ("One lockstep matcher at three seams"
reserved CASUS dispatch before its use case arrived.)

NEW questions exposed:
- STAGING (now the central architecture question, feeding Q3):
  STAGED (expansion EMITS patterns; a downstream engine runs them;
  macros = pattern compilers; two evaluation times, one grammar —
  Lisp's macroexpansion-time vs runtime) vs INTERLEAVED (matcher =
  engine subroutine; the whole lint = ONE pure call,
  expandere(lint-doc, file-doc) → report-doc; no separate pattern
  engine on this path). FRAN LEANS INTERLEAVED (closer to the
  mental model); tradeoffs to table at the interview: expansion
  cost no longer bounded by the document (parsing + matching
  inside); the vitium taxonomy grows world-state classes;
  determinism becomes relative to a world snapshot; the
  caching/resident story. The boundary law SURVIVES interleaving
  verbatim via §6.2's CAPS grammar: the engine acts only on
  @-space + CAPS builtins; unmarked content stays untouched (the
  DISTRIBUTIO boundary argument, restated).
- The bridge's MISSING DIRECTION: captures→arguments exists;
  dispatch needs argument→scope (`@fons` becoming what `de=`
  denotes). The one conversion no decree covers.
- User-raisable vitium (`<VITIUM nuntius=>`?): "unknown dialect" in
  ORDINARIUS must be LOUD (a silently-empty ORDINARIUS is a silent
  gate). Small new surface; needs a decree.
- Declared capture signatures: THIRD independent pull (macro
  hygiene when captures thread through composition textually;
  mutatio §5's private-by-default + declared exports; the dispatch
  interface). Three pulls from three directions = real.

### V.4 Ingress: PARSE (Fran's sketches)

`<PARSE output="$fons" via="something.c"/>` and inline
`<PARSE output="$snippet" lingua="c">c code here...</>`.

Readings banked (Claude, unratified):
- A parsed document enters as a SINGLETON RELATION (one row, root
  binding) so `de=` consumes it uniformly — everything `de=` takes
  is a relation; piping stays "reference the previous output".
  Multi-file ingress = an N-row relation with `via` as a PROVENANCE
  COLUMN (glob vs harness-bound: open). EXEMPLAR over an N-row
  scope = per-row matching that EXTENDS the input rows —
  composition as relational extension, not a new join operator.
- PARSE fits stratification: another output=-producing builtin,
  single-assignment, referenceable only below — the leaf-most
  producer, binding from the WORLD instead of from a match.
- Purity becomes DECLARED-INGRESS purity: expansion = a function of
  (document + named world snapshot). Because the language is total,
  expansion can complete before any ingress happens → the ingress
  set is statically enumerable per document (expand → enumerate
  PARSE tags → hash/approve → ingress → match). Direct fabrica
  rhyme (01KZYN4VPZ: declared IO + content hashes); a lint document
  is a fabrica artifact with declared inputs.
- The INLINE form is the QUASI-QUOTATION door: patterns/templates
  in CONCRETE dialect syntax with capture holes — the Coccinelle
  precedent (semantic patches = C with declared metavariables;
  decades of kernel practice; the known-good authoring UX).
  Requires per-dialect hole-blessing in the parser (`$names` legal
  under a flag) — open. The AST-element form remains the desugared
  truth.

### V.5 The acceptance criterion (Fran, decree-adjacent)

Every surface decision above is judged by LEAST SURPRISE for the
authoring party, HUMAN OR LLM — "which is why it's important to see
the syntax in front of us." For LLM authors, surprise ≈ distance
from the training distribution (concrete-syntax-with-holes is
in-distribution; deep genus trees are not); canon loudness is the
other half (surprise converted to load-time error, never silent
misbehavior). Interview method: candidate syntaxes go ON THE PAGE
before ratification.

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

   **SEQUENCING DECIDED (Round V): the measurement runs BEFORE the
   formal interview.** Note V.3's staging question feeds this item:
   the interleaved model strengthens the one-engine case.

   **MEASURED 2026-08-27 — see "Q3 mensura peracta" section below.
   The prediction survived: one engine, with two named engineering
   obligations (expansion provenance rule; document caching).**
4. How CASUS patterns share this grammar (COMMUTATIO's literal
   cases → pattern cases; presumably `<CASUS>` body IS an EXEMPLAR
   body in loose mode). Fran's sketch (2026-08-26, at COMMUTATIO
   build time): the §6.3 attribute-element BLOCK form is the
   growth path — `<CASUS><@est=><EXEMPLAR>...</></>...` gives
   `est` a subtree-valued case on a BUILTIN with zero new syntax,
   exactly as calls take subtree arguments. v1 `est` stays literal
   scalar equality; the block-form-on-builtins door stays open.
   **PROMOTED (Round V): load-bearing, not a door — the
   dialect-dispatch scenario (V.3) depends on it.** Full residue
   for the decree + CATENA-as-condition + the composability
   requirement (macro-produced patterns in est slots): see the
   "CASUS-conditio addendum" section (2026-08-31).
5. ~~DISTRIBUTIO ratification + the item-attr collision rule.~~
   RATUM 2026-08-26, BUILT 2026-08-27 (spec §6.4).
6. `<MATCH/>` / computed views (decide the reservation's fate before
   any splice syntax).
7. Joins (`$abc.n`) — reserved; revisit when the control plane
   pulls. (Narrowed by item 9: the relation SCHEMA lands in v2;
   join operators still wait for the pull.)
8. Mutation interop (V.1): positional vs bridge surface (one IR?);
   the unified quantifier decree + transactional reading;
   plan-first as law (mandatory relation consumption for verbs?);
   the components/signatures merge.
9. The relational layer (V.2): the antijoin/difference projection;
   the relation schema (provenance columns, serialized STML form,
   canon) — schema in v2 even if operators lag; the
   per-dialect-patterns / lingua-franca-relations doctrine.
10. Staging (V.3): interleaved vs staged (Fran leans interleaved);
    the argument→scope conversion; user-raisable vitium; declared
    capture signatures (exports).
11. Ingress (V.4): PARSE both forms; the singleton-relation +
    N-row-provenance reading; static ingress enumerability
    (fabrica); concrete-syntax capture holes (per-dialect
    hole-blessing).
12. Identity-bearing genera vs anonymous DISTRIBUTIO (Simulatio
    baubellarium, finding 12): distributed siblings are
    indistinguishable except by position; genera whose instances
    are citable individua need a decree — refuse wrapper-form
    authoring (loud, named alternative), demand per-item ids, or
    (dangerous) mint at distribution. Refusal leaned.
13a. **CATENA — BUILT AND GATED 2026-08-31** (name chosen over
    PIPE/FISTULA; decree in the "CATENA sketched" section below +
    build-time ratifications; commits 88fe15d7→5359c5e2): pipe
    SUGAR over named chaining, engine-read wrapper, pointer-
    threaded stages, `<(>` decoration works as sketched. The
    sequencing held: L1 lint shipped first with explicit names,
    then the differential gate — conditio_nihilum_catena.stml
    yields a byte-identical relatum vs the named form, 233/233
    plagulae. Still open: the tee (reserved vitium-loud); capture
    threading across links (second pull on V.4).
13. Business-data querying (Simulatio, findings 13-15): the
    decreed-aggregate vocabulary (numerus/summa/minimum/maximum —
    Round II's "aggregations = decreed projections" gets its first
    pull; the closed-verb house pattern, fourth appearance), AND
    the underlying fork: is EXEMPLAR the query surface for
    value-heavy business data at all, or does the store own
    values while patterns own structure? Q3's sibling — measured
    argument, not preference (keep it a SEPARATE measurement from
    the pre-interview Q3 run).

All of the above are judged under V.5's least-surprise criterion
(human or LLM author); candidate syntax goes on the page before
ratification.

## Simulatio: baubellarium (2026-08-27) — the pretend-SaaS exercise

House technique (the mutatio-visio §9 precedent): a fictional
domain — baubellarium, a SaaS for bauble businesses (baubellum =
genuine medieval Latin for trinket) — C89/hospitium backend,
volumen store, vitrea desktop POS, JS/HTML/CSS web client, mid-
strangle semantic layer (~70% generated with declared overrides).
Simulated: one brownfield feature (gift wrap through the whole
stack), one shipped bug (#412: rush fee $0.00 on desktop only),
one greenfield feature (delivery routes with a lifecycle machine).
FIFTEEN findings; none contradicted a banked decision — all edges.

Interview-feeding (now items 12-13 above): identity vs anonymous
distribution; aggregate vocabulary + the query-surface fork.

Trajectory-feeding (recorded in stml-visio.md §10.4): instance-
migration doc form demanded by the adequacy loop; PRAETERITUM =
third verdict class (override complete against an OLD schema) and
the shipped-bug proof that it must GATE; severity policy for
verdict classes; lintability in dynamic dialects MANUFACTURED by
strangling seams (generated accessors, perimeter lints — never
dataflow analysis of arbitrary JS); L4 policies = COMPOSITE named
units (generation decision + perimeter patterns, apparatus-
shaped); version skew = deployment-granularity staleness (fabrica
stamps + census-style CI gate); greenfield adequacy = the spec
interview mechanized; machine-driven scaffolding = tabularium's
architecture generalized (lived precedent); adequacy ≠ omniscience
(closure catches structural holes, not semantic omissions);
overrides declared AT BIRTH beat retrofitted ones.

Confirmations: the composition property held throughout (every
wished-for affordance assembled from banked tools — differre +
scratch generation, harvest + canon = contract tests); DISTRIBUTIO
item-wins/tombstone did exactly its job on seed data; INCOMPLETUM
checklists read as pleasant to-dos, not errors. Best structural
result: the morning's advisory staleness flag and the afternoon's
shipped bug were THE SAME PHENOMENON — the simulation generated an
argument, not just findings.

## The v1 agenda (2026-08-27) — triage + first applications

The 13 open items sorted by a builder's question: what blocks the
FIRST EXEMPLAR implementation? Ratified by Fran as the interview's
spine ("that all makes sense to me").

**Already decreed, no decision needed**: capture forms (`$x`
bool-attr = node, value position = value); modus vocabulary
(omnia/unum/primum/optional); non-linear captures = equality (the
parametrum matcher already enforces forests-equal); the bridge
(captures → argument map, built + fixture-pinned); PER's per-row
instantiation shape; loose-by-default polarity for query patterns.

**v1-BLOCKING (the seven interview questions, in order):**

1. Staging commitment (ratify interleaved) + the v1 `de=`
   resolution set: document root default / macro argument / prior
   `output=` — which subset; includes the argument→scope
   conversion (the missing bridge direction).
2. The match relation's C representation: binding environment =
   name → node/value/forest + provenance; PER sole v1 consumer,
   or also a serialized form?
3. **Anchored vs floating application** (NEW — nothing banked
   covers it): match at scope root only, or try at every subtree
   root? Lint wants floating; dispatch wants anchored. Floating
   substitutes for `<**>` in lint cases — and `<**>` is the
   algorithmic cliff (lockstep is linear; descendant wildcards
   introduce search). Rec: explicit attribute, both modes in v1,
   NO `<**>` in v1.
4. The v1 quantifier subset: root `modus=` yes; child occurrence
   suffixes (`? * +`) rec OUT (next increment); `<*>` rec IN
   (one lockstep rule); `<**>` OUT per item 3.
5. Polarity flag spelling + whether v1 exposes strict-mode
   EXEMPLAR or strict stays implicit in template matching.
6. Matcher extraction (ratify): promote `_par_congruere` out of
   silva_arbor.c into the engine as the general lockstep matcher;
   parametrum recognition rebuilt ON it — the parametrum corpus
   measurements become EXEMPLAR's day-one regression gate.
7. First consumer: rec parametrum-on-EXEMPLAR first (gate exists,
   zero new surface), then the decreed §9.2 slice (ONE
   declarative lint end-to-end) second. Vitium enum falls out
   mechanically at spec time.

**Explicitly NOT v1** (designed enough to wait): CASUS
pattern-cases (spelling decreed); PARSE builtin (harness hands
trees in, arbor-loader precedent); antijoin / serialized
relations / joins; mutation verbs; the DISTRIBUTIO-identity
decree (item 12); aggregates (item 13).

**First applications — the three-level ladder (how EXEMPLAR
changes resolved STML):**

INVARIANT: EXEMPLAR never appears in resolved STML (a match set
is not emittable; engine-space material is consumed, not
emitted). Resolution's contract is unchanged — resolved documents
stay plain, sigil-free, canon-judgeable content. What widens is
where instantiation ARGUMENTS come from: macros take them from
the call site, EXEMPLAR from matches. "Macros let a document
abbreviate itself; EXEMPLAR lets a document observe itself; PER
turns observations into content."

- **Level 0 — changes nothing, on purpose**: parametrum (then
  folia) recognition rebuilt on EXEMPLAR; resolved output
  byte-identical; the corpus is the oracle. Authorship migrates
  from C recognizers to declared patterns.
- **Level 1 — new derived documents**: expandere(lint-doc,
  corpus) → report-doc; artifacts with no authored source. First
  candidate: one examen codex re-expressed as a pattern document,
  DIFFERENTIALLY gated against the C codex (identical site sets —
  measurement B in miniature).
- **Level 2 — in-document derivation**: content as a function of
  the document's own content (self-maintaining indexes, summary
  tables; eventually arbor `<fontes>`, speculum listings — the
  regex-scrape victim replaced by patterns). Lawful by the macro
  precedent (recipe on the page, total, deterministic — NOT the
  computed-views violation); SAFE by strata (EXEMPLAR observes
  only content ABOVE itself — the spliced result can never be
  re-matched; cycles impossible by construction). Note: the
  self-index example needs FLOATING application — item 3 above is
  load-bearing here.

Each level inherits its oracle from the previous one: identity
first, then differential, then derivation.

## Q3 mensura peracta (2026-08-27) — the numbers item 3 asked for

Probe: `scratchpad mensura_q3/mensura_q3.c` (~1750 lines, C89,
links the real silva/build objects + apparatus; runs both corpus
tiers: 78 roundtrip plain-C + 155 lib/*.c with closures). Pipeline
per file: apparatus_parsare → silva_arbor_scribere_parsuram →
stml_legere → stml_expandere; three queries hand-written BOTH ways
(proto-EXEMPLAR walks over the expanded StmlNodus tree, _par_congruere
style; native walks over SilvaValor/SilvaNodus); differential gate =
sorted signature multisets; join audit = writer sedes_valorum
(pointer-keyed) vs reader positus. Zero pipeline fractures.

Queries: Q1 pointer-params (`<parametrum>` containing
`<declarator-monstrator>`; sig = last identifier = param name);
Q2 `piscina_allocare` calls with a `magnitudo()` argument
(`<vocatio>` + `<functio>` first-identifier + `<argumenta>`
contains `<lex-sizeof>`; sig = first arg identifier); Q3 si-vs-NIHIL
(`<si>` + `<conditio>` contains identifier `NULL`; sig = first
identifier of the condition).

**B. CORRECTNESS — the headline: 233/233 files, all three queries,
IDENTICAL site multisets including captured signatures** (latinized
tier: Q1 40,965 sites, Q2 689, Q3 2,861 — sites span the include
closure, both sides walk the same TU). The projection + template
expansion is semantically complete for matching: compression
(parametrum family, folia, spatia) round-trips through the content
view invisibly. The walks needed exactly the rules the engine will
need, all derivable from banked doctrine: skip origin wrappers
(expansio/pasta/stringificatio/api — provenance is metadata, not
content; silva carries it in fields so a symmetric walk must skip
the elements), skip ante/post trivia, count fragment DEFINITIONS
and skip transclusions (identity dedup — the ambiguus-arms sharing
law), skip `@`-space fragments, dedup sites by node identity.

**A. Projection tax (latinized tier, whole corpus)**: parse 12.0 s;
scriptura 26.6 s + stml_legere 18.0 s + expansio 8.1 s =
**projection 4.4× parse** (plain tier: 1.03×). Documents 32× source
bytes (12.7 MB → 414 MB — pretty indentation ~55% + NIHIL-oracle
maximal ambiguity, both known upper-bound caveats). Arena peaks
per file avg: parse 179 MB / scriptura 124 MB / lectio+expansio
278 MB (monster-file-dominated).

**Warm walk tax**: expanded tree = 10.1 M elements vs 3.7 M silva
nodes+tokens (**2.7× nodes** — locus wrappers, trivia elements,
origin elements, tok_ wrappers); full walk 19× slower. Queries with
ENGINE-GRADE tag comparison (interned pointers — the tree's own
intern, the canon lesson again; naive byte-compares cost a further
1.3–1.5×): **Q1 2.0×, Q2 10.6×, Q3 11.1× native**. Absolute warm
cost is practical: each query over all 155 files ≈ 0.8–1.2 s.
LOC of hand-written implementations ≈ equal (315 silva / 265 stml).

**C. Amortization**: cold pipeline+walk 65 s vs warm walk 1.4 s =
**45×** — with cached/resident expanded documents the one-engine
path collapses to "how often do files change," as the plan
conjectured. Warm is NOT ≈ native (2–11×), but constant-factor.

**THE FINDING WORTH MORE THAN THE TIMINGS (the predicted provenance
hole, measured at 100%)**: the expanded content view carries ZERO
document coordinates — 0/44,515 STML-side sites had positus.
Mechanism confirmed in code: `_duplicare_recursivum` (lib/stml.c)
deliberately zeroes positus on every clone ("duplicatum non e
parsatione venit") and `_expandere_nodum` superficially duplicates
EVERY node it walks, so expansion strips coordinates from the whole
tree — including untouched content. Silva-side sedes_valorum joins
only uncompressed values: Q2/Q3 100%, but Q1 31% (12,563/40,965 —
compressed parametra lose node-level sedes). Consequences for the
interview: (1) the v2 spec must decree an EXPANSION PROVENANCE rule
— carry positus through for untouched/moved content, or a side
table joining expanded↔document nodes (tabula_expansionum is the
precedent) — BEFORE formator-first transforms or mutation verbs can
ride EXEMPLAR matches; (2) linea IS copied by the duplicator, so
line-grade provenance survives today, but the house already ruled
line-approximation "tacite mentitur" for extents.

**Verdict for item 3**: one engine. Correctness equivalence is
measured, the warm tax is a practical constant factor, the cold tax
amortizes behind caching, and the one-engine dividends (canon
judges patterns; macros compose inside patterns; one grammar) stand
unrefuted. The two-engine fallback remains what it was — one
pattern grammar, two backend walks — to be revisited only if a
consumer needs sub-100 ms cold latency per file. Named
obligations: expansion provenance rule (above), resident/cached
documents, and eventually the pretty-flag lever on document size.

Probe trap for the record: qsort on a Xar is memory corruption —
Xar is SEGMENTED (stable element pointers, non-contiguous
storage); copy contiguous before sorting, always. Bit this probe
at 681 sites/file (first segment masked it below that).

## Colloquium formale (2026-08-27) — THE SEVEN DECISIONS, SEALED

The formal interview ran against the ratified agenda, measurement in
hand. Every recommendation below was accepted by Fran as presented
(syntax on the page, per V.5). These are DECISIONS now, not leans.

**1a. STAGING = INTERLEAVED.** The matcher is an engine subroutine;
stml_expandere resolves EXEMPLAR where it finds it; a lint is ONE
pure call over (lint-doc + corpus doc). The boundary law rides the
CAPS grammar unchanged.

**1b. v1 `de=` resolution set = root default + prior output.**
`de=` absent → document scope; `de="$x"` → a prior EXEMPLAR's
output relation (piping = naming the previous output). The
argument→scope conversion (`de="@arg"`) is the NAMED DOOR for the
composition/dispatch increment — not v1.

**2a. Match relation = in-memory only; PER sole consumer.**
Match set = Xar of binding environments: {radix: StmlNodus*,
ligamina: Xar of {titulus (interned) → nodus | valor | silva}}.
Every match implicitly binds its root. No serialized relation form
in v1 (the lint REPORT is already a document PER produces); the
V.2 relation SCHEMA is still designed in the v2 spec for the
joins/antijoin increment.

**2b. Provenance = linea-grade in v1; the EXTENT rule decreed.**
Matches carry nodes + linea (survives expansion cloning — measured).
The expansion provenance rule (positus carried through for
untouched content, or an expanded↔document side table;
tabula_expansionum is the precedent) is DECREED in the v2 spec as
a named obligation and BUILT when the first transform/mutation
consumer pulls.

**3a. Application mode = bare flag `ancorata`; absent = FLOATING.**
Floating = try the pattern at every subtree root within the scope
(the query case, and what all three measured queries were);
`ancorata` = the scope root itself must match (the dispatch case).
Least surprise decided the default: anchored-by-default would give
the bare query zero matches against a <parsura> root.

**3b. Floating bodies are single-rooted, loud.** A forest body
under floating application = named vitium naming the alternatives
(wrap in the real parent element, or ancorata). Keeps floating
strictly linear; sibling-run matching = named future increment.

**3c. `<**>` OUT of v1** — reserved; floating covers the lint uses;
descendant wildcards inside a pattern reintroduce search into the
lockstep walk.

**4a. Quantifier subset: root `modus=` IN; `<*>` IN (one lockstep
rule); child occurrence suffixes (`? * +`) OUT** (next increment —
loose omitted-is-unconstrained covers most of `?` already).

**4b. Loose child discipline = ORDERED SUBSEQUENCE.** Written
children must match in the same relative order, gaps allowed;
omitted children unconstrained. AST child order is meaning; a lint
that says "A before C" can say so. (Alternatives on the page:
unordered existential — rejected, silently accepts order-swapped
code; exact run — rejected, barely loosens.)

**4c. `modus=` absent = omnia.** optional = 0 or 1, ≥2 = loud
vitium; unum = exactly one else vitium; primum = first in document
order.

**5a. Strict mode ENGINE-INTERNAL in v1.** EXEMPLAR documents are
always loose; strict matching lives where it already lives —
templates run backwards ("the definition IS the pattern"). Surface
flag `strictum` RESERVED, spelled now, exposed when a document
consumer pulls.

**5b. The loose escape hatch ("exactly these children, no others")
RESERVED, unspelled** — no consumer yet; per-node polarity is
exactly what the root-polarity law warns against, so the eventual
spelling gets its own care at pull time.

**6a. Matcher extraction RATIFIED; home = lib/stml_macros.c.**
`_par_congruere` promoted from silva_arbor.c into the engine as
the general lockstep matcher (strict mode); loose mode built
beside it; parametrum recognition rebuilt ON it. No new object
file (the canon.o→stml_macros.o link-list lesson); split later if
taste demands. DONE = the plagula gate (78+155 byte-exact +
compression presence) green through the extracted matcher — the
parametrum corpus is EXEMPLAR's day-one regression gate.

**6b. TRANSPARENTIA — NEW DECREE, measurement-earned.** The engine
learns which tags are metadata from a DOCUMENT-LEVEL declaration,
strata-scoped like template definitions:
`<TRANSPARENTIA tags="expansio pasta stringificatio api extentum
ante post"/>`. Loose matching does not see into declared-transparent
subtrees (`@`-space is engine-inherent, not declared). The engine
stays dialect-ignorant; dialect knowledge stays in dialect
documents; the declaration is shareable across lint docs as a
fragment/macro. The probe PROVED the need: without these skips a
loose query for identifier NULL also sees the NIHIL spelling
inside the token's own <expansio> wrapper — wrong content, wrong
sites, silently. (Canon-declared was rejected for the circular
runtime coupling; per-EXEMPLAR attribute rejected because
forgetting it is a silent wrong-content bug.)

**7a. Consumer order: parametrum recognition first (L0), then one
lint (L1).** Byte-identical corpus is L0's oracle; each level
inherits its oracle from the previous.

**7b. The L1 lint = a PROBE QUERY PROMOTED** (si-vs-NIHIL or
piscina_allocare-with-magnitudo): its differential oracle already
exists and is proven green (the probe's silva-side walk, 233/233).
An examen codex re-expression is the SECOND lint, chosen at spec
time by fit (v1 subset; has a C oracle).

**7c. All NOT-v1 deferrals RATIFIED as they stand**: CASUS
pattern-cases; PARSE builtin; antijoin / serialized relations /
joins; mutation verbs; DISTRIBUTIO-identity decree (item 12);
aggregates (item 13). The v1 fence is CLOSED: engine + matcher +
EXEMPLAR/PER + TRANSPARENTIA + the two consumers, nothing else.

**Next: assemble the v2 spec FROM this file** — rounds I–V +
simulatio + agenda + Q3 mensura + these seals. Never re-invent.

## CATENA sketched (2026-08-31) — pipe sugar over named chaining

Fran's proposal (spelled `<PIPE>` in the raw sketch; **name chosen:
CATENA** — the CAPS vocabulary is Latin throughout, and FISTULA was
the other candidate, being the house word for pipeline in
silva_parsare):

```stml
<CATENA>
  <(><EXEMPLAR><conditio/></EXEMPLAR>
  <(><EXEMPLAR output="$nihila">
       <lex-identificator $n>NULL</lex-identificator>
     </EXEMPLAR>
</CATENA>
```

Sibling stages thread implicitly: each stage's output becomes the
next stage's scope; the LAST stage carries the explicit `output=`
a later PER (or, eventually, a mutation verb) consumes.

**Doctrine check, recorded**: Round I's finding "PIPE does not need
to exist — piping = referencing the previous output" SURVIVES as
the desugaring target. CATENA is sugar, not semantics: the engine
mints internal names for the intermediate seams (stage N `output=`
= stage N+1 `de=`); single assignment, strata, and the consumption
law apply unchanged; intermediates are consumed BY CONSTRUCTION so
the dead-code vitium concentrates on the tail. One IR — the same
synthesis move as mutation's positional-form-over-the-bridge (V.1).

**Mechanical fact**: the engine must read the WRAPPER form
directly — distribution runs AFTER expansion, so the
wrapper→siblings transform cannot literally be DISTRIBUTIO's
(and literal attr carry-down would wrongly stamp `output=` onto
every stage). Precedent: COMMUTATIO reads its own CASUS children.
The `<(>` item sugar remains the stage delimiter, engine-read; the
DISTRIBUTIO rhyme survives as the mental model (wrapper dictates,
items are instances), not as machinery.

**Falls out free**: `modus=` per stage (`unum` mid-chain = a
transactional assertion on the pipeline's premise); stages later
generalize to mutation verbs (the V.1 one-IR landing site: match
stages narrow, a verb stage consumes the relation as its plan).

**Named open sub-question**: may an INTERMEDIATE stage also carry
an explicit `output=` (a tee — lawful under single assignment, but
added surface)? Decide at decree time, not by accident.

**Sequencing (Fran, ratified)**: the L1 lint builds FIRST with
explicit names — proving the desugared form. CATENA is the first
post-v1 increment, and its gate is free and fitting: the lint
document rewritten in CATENA form must produce a byte-identical
report to the named form — the sugar differentially gated against
its own desugaring. Also to note in the decree: visio §9.4's
parked CLI pipe verbs are a different plane (shell tool vs document
construct); the spec should say so explicitly since "pipe" will
mean something in both.

**REUSE ADDENDUM (Fran, 2026-08-31)**: links may be macros
expanding to EXEMPLAR, may be CATENA, and may be macros expanding
to CATENA — the chain is the natural REUSE mechanism for stored
pattern libraries. Consequences worked out, unratified where
marked:

- **Nested chains FLATTEN** — the DISTRIBUTIO same-type-flattening
  law, third appearance: `CATENA[A, CATENA[B,C], D]` ≡
  `CATENA[A,B,C,D]`. Inner head receives the enclosing incoming
  relation; inner tail feeds the next outer link. Associativity by
  construction; trivial in the desugared IR (all intermediate
  names engine-minted). A link-position macro expanding to a
  FOREST of stages splices as consecutive links — stored
  multi-stage narrowings work bare or CATENA-wrapped.
- **Genus in = genus out in link position**: link expands first
  (call → EXEMPLAR/CATENA), then the chain reads the result; a
  macro-produced link is indistinguishable from a literal one.
  The vitium-XIII refinement covers link positions the way it
  covers est slots (application context = the chain).
- **OUTPUT PLACEMENT NOW IN TENSION (Fran to decide at decree)**:
  the original sketch put `output=` on the LAST stage; the reuse
  story wants stored fragments OPEN AT BOTH ENDS (no de= on head,
  no output= on tail; the use site decides both), which pulls the
  name onto the CATENA element itself (`<CATENA output="$x">`).
  Wrapper-placement keeps stored fragments name-free, makes
  nesting clean, and separates the tee question crisply (a STAGE
  carrying output= = a tee). Engine-native reading makes it safe
  (no literal attr carry-down). Claude leans wrapper; reverses the
  first sketch, so it is Fran's call.
- **Capture threading across links = the SECOND pull on
  relational extension** (V.4's per-row-matching-EXTENDS-rows):
  as modeled, each link's relation carries its own ligamina, so a
  stored fragment's mid-chain captures are invisible at the tail
  unless re-captured. Row-extension is what would let upstream
  captures flow through a chain — name the pull, build at its own
  decree.

**RATIFICATIONS (Fran, 2026-08-31)**:

1. **`output=` lives ON THE CATENA ELEMENT** — wrapper placement
   ratified. Stored chain fragments are open at both ends; the
   use site decides scope and name. (The tee sub-question — a
   STAGE carrying its own output= — remains open, now cleanly
   separated.)

2. **THE PURITY LAW: every CATENA link is a PURE function of the
   incoming relation producing an output relation for the next
   link.** The link vocabulary is CONSERVATIVE: EXEMPLAR, CATENA,
   and macros expanding to those — nothing effect-like, mutation
   verbs explicitly excluded. **This SUPERSEDES the earlier
   "stages later generalize to mutation verbs" phrasing above**:
   the correct V.1 one-IR reading is verb-as-CONSUMER-of-the-
   chain, never verb-as-stage — the chain produces the PLAN
   (mutatio's plan-first doctrine made structural), and effects
   consume the tail relation at a SINGLE point outside the chain.
   What this buys, recorded: chains stay inside the total-pure-
   language identity (every prefix deterministic, re-runnable,
   cacheable, canon-judgeable); transactional semantics (abort-
   all) concentrate at the one consumption seam; and the TOOL
   plane's pipes (visio §9.4) remain the separate place where
   effectful verbs chain, under different rules — the two planes
   now differ by exactly this law.

**BUILD-TIME RATIFICATIONS (Fran, 2026-08-31 — the four residues,
settled at build start; numbering continues)**:

3. **The tee is RESERVED, vitium-loud**: a stage carrying its own
   `output=` is a loud vitium until the tee's own decree. Lawful
   under single assignment but added surface — the door stays
   marked, not open.

4. **Attribute law**: the CATENA element takes `de=` under the
   SAME law as EXEMPLAR (root default + $prior), feeding the head
   link, plus required `output=`. Links carry NO `de=`/`output=`
   (vitium either way — a link `de=` would break threading, a link
   `output=` is the reserved tee) but MAY carry `modus=` per stage
   (mid-chain `unum` = a transactional assertion on the pipeline's
   premise).

5. **`<(>` is permitted DECORATION, never load-bearing**: found at
   build start that `<(>` already lexes as the anonymous capturing
   fragment, and fragments DISSOLVE in the effective-children
   computation — so the original sketch parses as written AND bare
   element children work identically. The engine reads effective
   children; fragment grouping is harmless because a forest
   splices as consecutive links anyway. Zero new lexer machinery.

6. **Canon CAPS row, same pattern**: CATENA's static form judged
   engine-side under CANON_MACHINAE_MALFORMATUM (XXIII) like
   EXEMPLAR/PER/TRANSPARENTIA — `output=` required and $-spelled,
   `de=` $-spelled when present, body non-empty, links judged as
   elements, nested EXEMPLAR bodies stay QUOTED per the existing
   law. probatio_canon gets the malformed set; silex's bundled
   canon stays stale until its next rebuild, as before.

**Implementation note (recorded before code)**: the minted-names
desugaring stays the MENTAL model; the code threads the
`StmlExemplarRelatio*` pointer stage-to-stage directly — same
semantics, "intermediates consumed by construction" becomes
literally true, and no reserved-spelling problem for names users
cannot write. Only the tail relation is registered in
`ctx.relationes` under the wrapper's `output=` name, so the
XVIII end-guard (unconsumed output) concentrates on the tail
exactly as the sketch's dead-code argument wanted.

## CASUS-conditio addendum (2026-08-31) — the pattern-case residue,
## CATENA-as-condition, and the composability requirement

Recorded so the CASUS decree finds everything in one place when the
door opens (coupled with `de="@arg"` — they open together).

**Already pinned**: the block-form spelling (Fran 2026-08-26,
`<CASUS><@est=><EXEMPLAR>…</></>` — zero new syntax); load-bearing
status (Round V, dialect dispatch); THE LINE preserved (a match is
enumerable structure, not a computed predicate); boolean = set
non-empty (cardinality thesis); first-congruent-arm-wins carries
over from literal COMMUTATIO.

**Unpinned residue for the decree**:
- Is `ancorata` the DEFAULT in case position? (Dispatch wants
  root-only discrimination; the flag now exists to say so.)
- Captures→arm-body mechanism: almost certainly the bridge again
  (arm body filled with the match's argument map, PER-like), but
  never formally stated. The arm interface = capture schema.
- A case-position EXEMPLAR carrying `output=` should be LOUD
  (it is a predicate + capture source, not a binding) — decree it,
  don't let it be silent.
- Exhaustiveness: unknown-constructor-reaches-ORDINARIUS must be
  raisable LOUD — the `<VITIUM nuntius=>` instrument, itself
  undecreed.

**CATENA as condition (minted this conversation)**: coherent for
free — a chain's tail yields a relation, non-empty = arm taken;
since CATENA desugars to named EXEMPLARs, a CATENA-valued case
desugars to a chain feeding the case test. Buys expressive
predicates ("a C file THAT USES the arena idiom") while staying
inside the no-expression law: every stage is still enumerable
structure. Nothing decreed yet; record only.

**THE COMPOSABILITY REQUIREMENT (Fran, 2026-08-31)**: fragments
and macros that EXPAND TO EXEMPLAR/CATENA must work in the `est`
slot — pattern-cases must receive patterns through the macro
layer, not only literally. Technical facts for the decree:

1. TODAY'S GUARD IS THE PLACEHOLDER: gradus II made
   EXEMPLAR/PER/TRANSPARENTIA inside a fill context a LOUD vitium
   XIII. When @arg lands, that guard REFINES (not merely lifts):
   application inside bodies binds to the argument scope; and —
   distinctly —
2. CONVEYANCE vs APPLICATION: a pattern being carried INTO an
   engine-space consumer slot (an est block) is INERT — quoted,
   not applied. Patterns apply only at their consumption site
   (the case judge). Mirrors "nested template definitions opaque
   in bodies." Without this rule, est-block expansion would try
   to resolve the EXEMPLAR mid-conveyance.
3. Order in the est slot: block children EXPAND first (a template
   call resolves to its EXEMPLAR/CATENA body — genus in = genus
   out), THEN the judge requires an engine-space root. A macro-
   produced pattern must be indistinguishable from a literal one.
4. The ALIAS flavor is a sub-decision: content transclusions
   (`<<#x>>`) survive expansion UNRESOLVED (no library resolver
   exists; every consumer rolls its own). The case judge either
   resolves transparently (the canon precedent — it resolves for
   judging) or refuses loud. Pick one at decree time.
5. THE GATE when built: the same dispatch fixture written THREE
   ways — literal EXEMPLAR in est / template call in est /
   transclusion in est — must behave identically (a composability
   differential, the CATENA-gate pattern again).

## The pure-command CLASS: switches as links (Fran, 2026-08-31,
## post-CATENA-gate)

Fran's proposal, minted the evening CATENA shipped: **a switch
whose children are only other switches, CATENA, or EXEMPLAR is
itself a pure resolution command** — and should therefore be
admissible wherever pure commands go: as a CATENA link, stored in
fragments/templates, conveyed through the macro layer.

**What this IS**: the closure of the purity law's category. The
ratified link vocabulary was an ENUMERATION (EXEMPLAR, CATENA,
macros-to-those); this names the CLASS — pure commands are
inductively built from EXEMPLAR by sequential composition (CATENA
= the pipeline's AND) and case analysis (pure-CASUS = the missing
OR: dispatch, alternatives, fallbacks). Chains-as-conditions is
already banked above; switches-as-links is its mirror — the
constructs interpenetrate symmetrically. Purity is enforced
STRUCTURALLY (arm vocabulary = the closed class, recursively —
the chain collector's link check pointed at arm bodies), never by
analysis.

**THE SEQUENCING INSIGHT — decoupled from `de="@arg"`**: the
fill-context pattern-case door needs the argument bridge because
its scrutinee is a macro argument. The CHAIN-PLANE switch's
scrutinee is the incoming relation the chain already threads —
condition = pattern/chain run against it, non-empty = arm taken,
first-congruent-arm-wins from COMMUTATIO. No @arg, no fill-context
refinement; it can ship BEFORE the CASUS-in-fill door, on the
ctx->applicatio conveyance substrate (the new construct joins the
conveyed-CAPS set + the link vocabulary + the canon CAPS rows).

**Decree residue (recorded, undecreed)**:
1. SET-LEVEL vs PER-ROW dispatch: one arm wins for the whole
   relation (v1 lean — mirrors "tail non-empty = arm taken") vs
   each row routed through its first matching arm (partition;
   later door, rhymes with PER's per-row shape).
2. GUARD ≠ ALTERNATION: a guarded switch (condition separate,
   arm receives the incoming relation) is NOT "try each chain,
   first non-empty output wins" — under first-non-empty, an arm
   whose correct result is legitimately EMPTY falls through to
   the wrong arm. Fran's framing = the guarded combinator;
   alternation is a possible SECOND combinator, not a spelling
   of the same one.
3. No-match semantics: ORDINARIUS optional; without it, lean =
   empty relation out (the chain's emptiness algebra), ORDINARIUS
   as the explicit always-arm. Loud-on-no-match stays available
   via the undecreed `<VITIUM nuntius=>`.
4. Spelling: COMMUTATIO is TAKEN by the fill plane (value
   dispatch during instantiation). Position-determines-plane has
   precedent (engine-read wrapper forms), but a distinct CAPS
   name for the pure-plane switch may be cleaner — Fran's call.
5. Condition captures do NOT flow to arms in v1 — that is the
   captures→arm bridge residue of the fill door; decree it ONCE
   for both planes when either opens.
6. Does the arm receive the ORIGINAL incoming relation (condition
   purely boolean — lean) or the condition's matched output
   (guard-and-narrow, collapses condition into arm head)? Decide
   at decree; option (i) keeps guard/transform orthogonal.

**Gate when built**: the CATENA-gate pattern a third time — a
dispatch lint expressed (a) as a pure-CASUS link in a chain and
(b) as N separate named-form documents whose reports are
concatenated by hand; byte-identical per-arm reports. Plus the
composability differential (literal switch in link position /
template expanding to one).

Status: banked with reasoning; no decree. Pull: the first stored
chain that needs per-shape narrowing mid-pipeline (dialect
dispatch inside a lint chain is the standing candidate).

**DECREE (Fran, 2026-08-31, late evening — the four-question
round, all recommended options ratified)**:

1. **Name: DIRIBITIO** (the sorting of ballots — dispatch).
   Distinct from COMMUTATIO so the two planes stay visually
   unconfusable; arms reuse the CASUS/ORDINARIUS spellings.
2. **Scope: chain-plane ONLY this increment** — DIRIBITIO works
   at document level (output= required, de= optional, same law as
   CATENA), as a CATENA link (bare), and as a condition inside
   est slots (which in this increment exist only inside DIRIBITIO
   itself — recursion covers it). Fill-plane pattern-cases
   (COMMUTATIO + de="@arg" + captures→arm) stay behind their own
   door.
3. **Semantics bundle, all ratified as leaned**: SET-LEVEL
   dispatch (one arm wins for the WHOLE relation); GUARDED
   combinator (condition separate; the arm receives the ORIGINAL
   incoming relation, never the condition's matches);
   first-congruent-arm-wins in document order; no arm taken =
   EMPTY relation out (the chain's emptiness algebra);
   ORDINARIUS = optional always-arm, must be LAST (an ORDINARIUS
   shadowing later arms = dead code, loud); condition ROOT is
   BARE — no output=/de=/modus (a pure boolean test; `ancorata`
   IS allowed — it is pattern semantics, not relation plumbing;
   a chain-condition's internal stages may still use modus=, and
   a mid-chain unum violation there is the ordinary hard vitium);
   arm body = the est slot plus exactly ONE pure command (wrap
   forests in CATENA — the one-element law again); condition
   captures do NOT flow to arms. Per-row routing, alternation
   (first-non-empty), guard-narrows-arm, and capture-flow each
   remain named doors.
4. **Gate: the corpus differential, third apparition** — lint II
   re-expressed as a SELF-GUARDED chain (a DIRIBITIO link whose
   condition is the usleep pattern, arm = the capturing
   narrowing) must yield a byte-identical relatum vs the plain
   lint II document across the 161-file corpus; plus the
   composability fixture (literal switch ≡ template-produced
   switch).

Vitium XXI = DIRIBITIO_MALFORMATA reserved for the construct's
form family; document-level output= reuses XII (sine exitu) and
unknown de= reuses XIV, as CATENA did.

**SPELLING REVISED AT BUILD (Fran, 2026-08-31, same night)**: the
pinned `<@est=>` block form CANNOT PARSE in arm position —
attr-elements carry tree children only in invocation-argument
position (modus I); elsewhere the §6.3 text-only law is enforced
AT PARSE TIME, and generic STML cannot special-case CASUS without
a layering violation. The 2026-08-26 "zero new syntax" claim was
false for structured conditions. **Ratified: the est slot is the
CAPS element `<EST>`** — `<CASUS><EST><EXEMPLAR>…</EXEMPLAR></EST>
<EXEMPLAR>…</EXEMPLAR></CASUS>`; zero parser change, engine-owned
space, no content collision. **Backport (Fran): the fill-plane
CASUS door's pinned spelling is REVISED to `<EST>` as well** (the
same wall applies there — structured patterns in est). Named small
increment recorded, undecreed in detail: migrating live
COMMUTATIO's value-form `est="x"` ATTRIBUTE to `<EST>x</EST>`
(text children parse trivially; a mechanical migration with a
compatibility question — both forms during transition, or
flag-day).

## Lint II — BUILT AND GATED (2026-08-31, same evening; ffcc6837).
## The gate went green on the FIRST full run: 155 lib + 6 named
## probationes, 27/27 sites, zero divergence — the minimal
## TRANSPARENTIA (L1 wrappers + regio-cruda) needed no additions.
## One parse feeds both judges; the oracle is the examen recipe
## verbatim (the apparatus path would be false-green: it composes
## no systema, so codex 87 never fires on it). Selection record:

## Lint II candidate chosen: codex 87 (2026-08-31, evening)

Survey of the examen codex inventory (89 codices, census over 820
plagulae) against the v1 pattern subset. THE FINDING: the inventory
is overwhelmingly type-driven (54 conversio signi), flow-driven
(71-73 ininitiata), or symbol-stateful (51 vocatio implicita) —
none pattern-expressible; the structural tail is thin, exactly as
the strangler doctrine predicts (per-region truth-by-gate — the
pattern plane eats what it can judge exactly).

**CHOSEN (Fran): codex 87 OBSOLETUM_ADHIBITUM** — obsolete POSIX
symbol used (table = `<gradus>` rows in fontes/systema_posix.h;
currently ONE entry: usleep→nanosleep). 27 live sites / 7 files
(24 in probationes, 1 in lib/tcp_posix.c). Why it fits: exact-text
lexeme match (`<lex-identificator>usleep</>`), and the oracle is
the SHIPPING ANALYZER in-process — a far stronger differential
than L1's hand-written walk.

**The scoping question RESOLVED by scouting**: examen counts the
principal file's stratum-0, taken branches only. Measured on
lib/tcp_posix.c's parsura document (18512 lines): system headers
NEVER enter the tree; the document carries exactly ONE usleep
lexeme, bare — already agreeing with examen's count. Omitted
conditional arms appear as **`<regio-cruda>`** wrappers — hidden
by the EXISTING TRANSPARENTIA mechanism (skipped whole), no new
vocabulary needed. So the lint document is L1's shape: a
TRANSPARENTIA row (L1's origin wrappers + regio-cruda, possibly
directiva/regio-directiva if divergence appears), one EXEMPLAR,
one relatum/PER. The `pro=` replacement (nanosleep) can ride as
static relatum content.

**Build sketch**: corpus = the L1 tiers PLUS a probationes tier
(without it presence is 1 site — the coverage law); oracle =
semantica diagnostics filtered to codex 87 per file, count
equality (positions stay blocked by the extent-provenance hole,
as in L1); diagnosis seams = the LINT_INDAGO pattern carried over.

**Recorded regardless of the pick**: codices 69/70 (unused
var/param — the census's best distribution, 35 sites/16 files) are
ANTIJOIN-shaped: the survey independently confirmed the relational
door's first pull, with concrete codices waiting on it. Codex 83
(reserved identifiers) names a second vocabulary door: PREFIX
text-matching. Codex 86 (vernaculum) falls out free after 87 but
has zero live sites today.

## SINE — the antijoin seed (Fran + design conversation,
## 2026-08-31, after DIRIBITIO shipped)

The relational door's first operator, sketched from Fran's
composition question ("could you do <EXEMPLAR>… <SINE><EXEMPLAR>…
</SINE> if you need a pair?"). THE ANSWER RESHAPED THE DESIGN:

**Negation doctrine**: a pattern cannot match absence — a general
NOT would smuggle in a predicate language. Antijoin is the only
lawful not: rows of A with no partner in B, both sides enumerable
match-sets, the negation living in a SET COMBINATOR. Stratified
negation's soundness condition (negate only completed relations)
is already guaranteed by single assignment + the strata law —
the doctrine was ready before the operator.

**THE LINK FORM (Fran's instinct, refined)**: in chain position
SINE carries only the B side — the chain already threads A:

    <CATENA output="$inutilia">
      <(><EXEMPLAR><declaratio-localis>
           <lex-identificator $n/></declaratio-localis></EXEMPLAR>
      <(><SINE><expressio><lex-identificator>&@n;
           </lex-identificator></expressio></SINE>
    </CATENA>

**THE BRIDGE IS THE JOIN**: the row's captures fill the body
pattern via `&@n;` (PER's existing per-row instantiation), then
the filled pattern runs; zero matches = row kept. Fully
correlated NOT-EXISTS with no clavis=/key machinery — the
correlation is SPELLED IN THE PATTERN by where `&@n;` appears.
Dissolves the keyed form entirely.

**THE DUALITY**: an ordinary chain stage multiplies rows by their
inner matches; SINE is the same per-row step sign-flipped (keep on
ZERO inner matches). It is the negation of the chaining step
itself — hence pure-command-class membership for free (SINE-
bearing chains as DIRIBITIO conditions: "if anything is MISSING,
take this arm").

**Residue for the decree**: (1) test scope — row subtree (the
per-row law, lean) vs document; with row-extension + scope-rows,
subtree scoping does codices 69/70 exactly; (2) body = bare
pattern vs one pure command; (3) the uncorrelated flavor
(subtract a NAMED relation by row identity) = separate smaller
operator, parked unnamed; (4) SINE = pure FILTER (rows pass
unchanged, no new captures) — lean yes; (5) sequencing: the door
opens rows-as-tuples → extension law → SINE, gate = codex 69
differential (CAVEAT: census shows all 35 sites in fixa/ — the
gate corpus must include examen fixtures for presence; and
SHADOWING is a named expressibility risk for exact agreement).

## THE RELATIONAL DOOR — DECREED (Fran, 2026-08-31, late night;
## two four-question rounds, all recommended options ratified)

The door opens in order: rows-as-tuples → the extension law →
SINE; gate = codex 69 differential.

**Round I — the row model:**
1. **CARRY-FORWARD IS THE LAW**: every chain stage's output rows
   inherit the incoming row's captures and add their own — rows
   are tuples accumulating down the chain. No flag surface; one
   semantics. (Existing gates stay green: no current document
   reuses names across stages; PER just sees a richer map —
   VERIFY at build with all three corpus gates.)
2. **Cross-stage name collision = VITIUM, loud.** Distinct names
   required along a chain; correlation is spelled via the `&@n;`
   bridge, never by accidental reuse. **Join-by-naming (regula V
   lifted across stages: reused name = equality constraint) is a
   NAMED DOOR**, not v1.
3. **Row radix = the LATEST stage's match**, always; earlier
   stage roots survive only if explicitly captured. The chain
   keeps narrowing; history rides in captures.
4. **PER's substitution map = ALL accumulated captures** — a
   report splices stage-1's scope name beside stage-3's site.

**Round II — SINE:**
5. **Test scope = the ROW'S SUBTREE** (the per-row law,
   consistent with de= chaining and PER; scope-rows + subtree
   scoping do codices 69/70 exactly). Document-wide absence = a
   later spelled door.
6. **Body v1 = bare PATTERN** (one element, bridge-filled, may
   use ancorata). Widening to full pure-command bodies
   (CATENA/DIRIBITIO absence tests) = additive later.
7. **SINE is a pure FILTER**: surviving rows pass UNCHANGED
   (same radix, same captures); SINE itself carries no
   modus=/ancorata; a capture-adding SINE explicitly rejected
   (kept rows are those where the body matched nothing — there
   is nothing to capture).
8. **Gate = the codex 69 attempt**: corpus = examen fixa/
   fixtures (all 35 live sites) + lib (zero-agreement breadth);
   the SHADOWING expressibility risk is NAMED and
   divergence-driven refinement is the method; fallback recorded
   (house absence-lint with hand oracle) so a wall cannot strand
   the increment. Oracle = the examen recipe in-process, the
   lint-II pattern.

Build shape: extension in `_exemplar_petere`/row construction
(ligamina inherit + collision check), PER map widening,
`_sine_implere`/link-branch on the `_mandata_colligere` substrate,
vitium XXIII (XXII went to CAPTURA_COLLISA at build), canon CAPS
row, fixtures, then the gate probatio. [Extension law SHIPPED
9d8c0e7e — all gates re-verified green.]

**[SINE BUILT 2026-08-31, third night — the arc is COMPLETE.**
Engine `_sine_processare` (6ea2eccb): all eight seals honored;
ancorata resolved floating-only (seal 7 governs — pattern-element
attributes are match literals, so there is nowhere lawful to put
an engine flag; anchored SINE = named door). Canon row 8f0bbbda.
THE CODEX 69 GATE (probatio_silva_exemplaria_inutilia): first run
865 false sites → THREE MATCHER REFINEMENTS (14ea9405: symmetric
blindness; TRANSPARENTIA attributa=, Fran-ratified; content-alias
resolution, Fran-ratified) → EXACT agreement, 206 files, 0
divergence, L1/lint II byte gates intact. The shadowing/first-
binding residue classes are measurably ABSENT from the corpus;
divergentes==0 pinned. The fallback (house absence-lint) was
never needed.]**

## EXEMPLAR as canon-rule vocabulary (2026-08-31, Fran's question)

Not previously banked (nearest: V.2's codices-as-pattern-documents
endgame — but that is EXAMEN, the language judge; this is CANON,
the schema judge, in the reverse of the banked direction).

**The shape**: canon today = grammar-shaped schema (closed row
vocabulary, engine-interpreted) with a long tail it cannot reach
("X must contain Y before Z"; "no A inside B unless C"). A
pattern-rule = **pattern + cardinality constraint + verdict**:
`modus=` is already the cardinality language (prohibition =
must-match-zero; requirement = unum/non-empty); the undecreed
`<VITIUM nuntius=>` is exactly the verdict instrument; everything
stays enumerable structure — canon's no-expression law unbroken.

**The precedent**: the grammar-vs-pattern schema split — RelaxNG
on one side, SCHEMATRON on the other; decades of practice say
LAYERED COEXISTENCE: grammar rows for the spine, `<regula>`
pattern sections for the tail. One judge machinery underneath
(homogeneous tower §10.2 applied to canon itself — the same
lockstep matcher).

**Convergences making it cheap**: canon already judges the SENSE
(expanded/distributed projections) — pattern-rules run over the
same views, zero new plumbing; judging is pure, so rules satisfy
the CATENA purity law trivially; existing rules migrate
DIFFERENTIALLY (the L1 method); and **the v1-closing canon
vocabulary slice is this arc's load-bearing first step
regardless** — canon.canon self-hosts, so canon files CONTAINING
exemplar rules presupposes canon judging EXEMPLAR as content.

**Tensions to honor at decree time**: canon's load-time REFUSAL
discipline (unknown rule vocabulary refuses loudly — pattern-rule
rows must keep that); the generated-canon path (natura_canones
projections — do generated canons ever EMIT pattern rules, or are
they hand-authored only at first?); and rule provenance in
verdicts (a vitium naming the RULE that raised it — the
user-raisable VITIUM decree should carry this).

Status: recorded, no decree. Pull: the first structural rule that
canon's row vocabulary cannot express without engine code.

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

## Simulatio findings + the INDAGO seed (Fran + design
## conversation, 2026-08-31, after the arc completed)

Usage simulation of the shipped algebra (theorem-lint + guarded
alarm ran live; leak-lint + cross-stage join on paper). THE
META-FINDING: everything that chafed was REACH (descent) or
PLURALITY (bindings, routing) — never the algebra; composition
held in every scene including untested ones.

**NEW DOOR — DESCENDANT AXIS (head of the list):** patterns match
level-by-level; "X somewhere below Y" is unwritable when paths
vary by context (calls: assignatio-RHS vs init vs statement...).
Blocked the paired-call antijoin family (leak lints) outright;
codex 69's three shape-chains are hand-unrolled descent. Note:
selecta/quaestio CSS selectors HAVE descent — the gap between the
two query planes is measured, not suspected.

**NEW SEED — INDAGO (Fran's shape: commands resolve into trace
COMMENTS):** document-declared `<INDAGO/>` CAPS element; each
consumed command leaves a tombstone comment at its site with
per-stage counts (rows in->out, capture names, SINE `necati`,
DIRIBITIO per-arm verdicts + SUMPTUM). Comment-not-element is
LOAD-BEARING: traces must be invisible to the semantics they
describe by NATURE (the attributa= bug = cost of
visibility-plus-forgotten-blindness); arbor's element-provenance
is the queryable tier, traces are the erasable tier (rhymes with
annotation vision 01KXTX7FA3). Counts already computed at every
stage; zero cost when absent. Would have made the 865-hunt a
one-glance find (`necati 0`). Open for decree: PER-inside-relatum
trace placement (probably yes, opt-in bytes differ by design);
per-command narrowing (collides with bare-link laws — doc-wide
first); INDAGO's own canon/vitium rows. Cost-benefit likely #1
of all open doors.

**[INDAGO BUILT same night — three-question decree (switch form
= <INDAGO/> CAPS element; full stage story; PER uniform incl.
in-relatum), all recommended ratified. Format live-verified then
pinned exact (197/197). Vitium XXIV; canon row; DIRIBITIO
conditions = verdict count only (scriptor suspended); unreached
arms absent from trace. The stage-census door is CLOSED.]**

Felt-evidence re-rank of known doors: guard-narrows-arm (the
alarm doc contains its own filter TWICE — condition and arm);
multi-binding (sibling plurality in one scope = dangling-ref
lints); join-by-naming (regula V works WITHIN a form today —
self-proof `<punctum index="$i"><probatio ref="$i"/></punctum>`
is live; cross-STAGE equality is the door); per-row routing
(parallel literal-guard chains = today's workaround).

## INDAGARE — the debugger seed (Fran's shape, 2026-08-31,
## after INDAGO shipped)

Fran: a step-through STML debugger, CLI, run a document — and the
key instinct: NOT re-reading the document per step, but a STREAM
OF EVENTS ("compared such-and-such", "emitted such-and-such").

**The stream is the substrate; everything is a consumer.** The
gesta/metrum doctrine applied to the engine: the event stream =
gesta (lossless, what happened); INDAGO = metrum (lossy fold —
counts per stage, materialized as comments). One auditor callback
seam in the engine (the silva aux-seam pattern), firing at the
INDAGO sites PLUS the per-candidate comparison sites in the
matcher — the cause layer INDAGO deliberately omits. INDAGO
eventually reimplementable as a fold over the seam.

**Events as STML** (multi-root, streamed, tail-able):
`<comparatio forma= candidatus= exitus=/>`, `<necatus ordo=
causa=/>`, `<captura titulus= valor=/>`, `<emissio/>`, `<relatio
titulus= ordines= consumpta=/>`. Then breakpoints ARE patterns —
the exemplar language filtering its own execution — and the
breakpoint doubles as the EMISSION FILTER (per-candidate volume
demands source-side filtering; journal one file's run, never a
corpus).

**Determinism = time travel free.** No clock, no randomness,
single assignment: step-backward = re-run to event N-1, zero
state capture.

**Two consumers, one stream**: interactive stepper (human) +
filtered journal dump (agent/CI: `stml indagare -acta doc |
grep NEGATUM`).

**Landing spot**: the PARKED stml CLI (Plan A substrate shipped
on stml-incolumitas; Plan B never written) — the debugger is the
strongest reason Plan B has had to exist. Name: indagare = the
deep tier of INDAGO (one word, two depths). Worth its build the
day someone is deep in matcher-semantics questions — i.e. every
future codex-as-pattern gate.

## SECOND-LOOK PRICING of the simulation doors (design
## conversation, 2026-08-31 night) — three dissolutions

Pricing the doors at the seam instead of by their symptom-names:

1. **"Multi-binding enumeration" DISSOLVES into RADIX RETENTION**
   — the nucleus already enumerates every match per row (one row
   per declaration); only the radix narrows. The door = a stage
   flag (`radix="fontis"`-shaped) keeping the incoming row's
   radix — near one assignment in `_exemplar_nucleus`. Unlocks
   second-same-shape declarations AND the sibling
   cross-reference/dangling-ref lint family.
2. **"Join-by-naming" DISSOLVES into BRIDGE UNIFORMITY** — not
   name-reuse-as-equality (rightly rejected; XXII stays loud) but
   lifting the restriction that `&@n;` fills only SINE bodies:
   a positive stage whose pattern carries references gets per-row
   instantiation (SINE's machinery, opposite sign). Correlation
   stays SPELLED, never coincidental. Compat note: literal
   `&@n;` in a link body currently matches literal transclusion
   nodes (raw-transclusion law) — vanishingly rare.
3. **"Positional descent" DISSOLVES into COMPOSITION** (Fran's
   observation closed it: no positional vocabulary exists beyond
   implicit sibling order + spelled containment) — `<**>` inside
   a `<*>` occupying a cursor slot = positional descent, spelled
   visibly. Existential composes up; positional could never
   compose down. Pinned by the fixture pair.

Remaining doors after the axis: radix retention (top,
value/cost), bridge uniformity, guard-narrows-arm (`<CASUS
angustans>`-shaped, five lines), per-row routing (parked - the
parallel-literal-guard-chains workaround is honest).

## DESCENSUS BUILT (same night — four-question round, all
## recommended ratified)

Existential bundle (hoisted, one-child, first-COMPATIBLE,
no joint backtracking); visibility = petere's exactly; star-run
lexer + XXV + strict refusal; gate document unified as its own
MEASURED step (identical 1/1 + 0/0; residue shifted to
first-per-block-any-shape; plurality = radix retention's job).
The leak-lint family and deep self-joins are now writable.

## RETENTIO RADICIS BUILT (2026-09-01 — three-question round,
## all recommended ratified)

radix="fontis" bundle; canon's three judges mirror (doc judge
catches fontis-without-de statically); gate as consumer. THE
BUILD'S TWO HARD LESSONS: (1) corpus-scoped rows NEST — the
row-multiplication x subtree-walk product OOM-killed the gate on
arbor2_glr (723 corpora, 811 declarations); scope rows =
definitio-functionis, which C89 CANNOT nest — the
nested-duplicate residue class dissolves by language structure,
not workaround. (2) SINE asks existence only — swapped onto
_alicubi_congruere (ratified same-visibility, early exit):
31s -> 8s on the killer file. Per-declaration codex 69 is REAL
now (formae.c 4/4 exact; second-declaration residue killed).
PROCESS LESSON, expensive: scratchpad probe binaries went STALE
across two engine increments and produced a false regression
(unified doc "0 on formae") + a false cost figure (1.3s) — the
gate binary (always fresh-built) was the only honest oracle.
Rebuild probes before believing them; recorded in
debugging-lessons.

## PONS UNIFORMIS + ANGUSTANS BUILT (2026-09-01 — two-question
## round, both recommended ratified)

Bridge uniformity: THE REFERENCE IS THE OPT-IN (no ceremony);
shared fill machine `_corpus_ordini_implere`, SINE refactored
onto it (NON-EXSTAT and EXSTAT are one correlation machine with
opposite signs — the symmetry completed); nucleus gained
stratum/tectum (mechanical ripple). Cross-stage joins live
(fixture: per-declaration rows joined to their own probatio).
Angustans: `<CASUS angustans>`; ORDINARIUS+angustans = XXI both
judges; the IDENTITY IDIOM discovered and pinned — `<EXEMPLAR
ancorata>` + wildcard = each row maps to itself, so the alarm
document spells its filter ONCE (condition filters, identity arm
carries). Two doors closed; remaining named doors: per-row
routing (parked), document-scope SINE, tee, anchored SINE,
petere-through-alias, multi-binding-within-one-candidate,
indagare (the debugger), descensus cost measurement (mensor).

## COMPOSITION PREMISE EXPLORED (2026-09-01, conversation — not yet
## built; banked so the design survives compact)

Fran's premise to test: climb the abstraction ladder by capturing
query shapes as `#@` templates and composing them, instead of always
writing raw arbor vocabulary. Grounding measurements taken:

1. **Subtree-valued template arguments EXIST** (fixture 6.1b:
   `<<#@f>><@c=><x/><y/></>` splices a subtree, expanded in the
   CALLER's context). The load-bearing feature for pattern-valued
   parameters — a lint SCHEMA is mechanically possible today.
2. **THE WALL: the two planes share the `&@x;` namespace.** The macro
   expander runs first and claims every reference in a template BODY:
   `&@ignotum;` = LOCULUS_IGNOTUS **at collection time, without any
   vocation** (probatio_stml_macros.c, T4 f). So a template cannot
   bake a relational bridge (`&@n;`) into its body — it dies at load.
3. **THE DOORWAY: relational references travel in ARGUMENT SUBTREES,
   never template bodies.** Caller-side argument blocks expand in the
   caller's (top-level) context where `&@n;` passes untouched. Schema
   shape: `#@non-usitatum` takes `ordines=` and `absens=` as subtree
   args carrying the stages (bridges included); the body is pure
   CATENA skeleton with only `$` captures. Discipline, not mechanism.
   If it chafes, the named resolutions are: an escape form for
   literal `&@` in bodies, or a declared pass-through namespace on
   the definition. Silent pass-through of unknown slots is ruled out
   (refusal-loud).
4. **Slot-name law measured the same day** (vertere §6 example was
   broken by it): call arguments address SLOT names (the `@`-value),
   never the declaring attribute's name. Convention: mirror them.

**THE EXPERIMENT, ready to run**: codex 70 (unused parameters)
written TWICE — once raw like the codex 69 doc, once through the
`#@non-usitatum` schema — with the examen recipe as differential
oracle for BOTH. Same truth anchor, two abstraction levels; the
unused-static lint nearly free as a third instantiation (it pulls
document-scope/anchored SINE — a parked door gets its consumer).
Predicted next chafe: the PRELUDE problem — level-1 shape atoms want
one shared file that lint documents import; macros are per-document
today (same shape as silva/quaestiones.stml incl. its promotion
doctrine). Lint taxonomy from the same conversation: correlation
lints = the sweet spot; ordering lints come FREE (sibling
subsequences are ordered — codex 85 include-order is writable);
blocked classes = counting (M4) and value predicates (decreed dead).
`stml expandere` is the shell harness for all of it.
