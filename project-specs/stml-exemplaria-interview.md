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
4. How CASUS patterns share this grammar (COMMUTATIO's literal
   cases → pattern cases; presumably `<CASUS>` body IS an EXEMPLAR
   body in loose mode). Fran's sketch (2026-08-26, at COMMUTATIO
   build time): the §6.3 attribute-element BLOCK form is the
   growth path — `<CASUS><@est=><EXEMPLAR>...</></>...` gives
   `est` a subtree-valued case on a BUILTIN with zero new syntax,
   exactly as calls take subtree arguments. v1 `est` stays literal
   scalar equality; the block-form-on-builtins door stays open.
   **PROMOTED (Round V): load-bearing, not a door — the
   dialect-dispatch scenario (V.3) depends on it.**
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
