# Silva Simulatio ③ — Annotated Grammar → Generated Construction

*Implementation simulation (2026-07-02), ~2.25 simulated hours. Second simulation
chronologically; numbered ③ to match the spec-v2 §5 brief. Complications continue
from sim ① (S18+, R3+). Inherited obligations from spec-v2 §8.4: region-node genera
and the cost-ready recovery action kind in the baked format. Exit criterion:
annotation format v0 frozen.*

## Pre-sim research findings

- **R3 — Positional identity already causes drift in shipped code.** The reduction
  callback's doc-comment block (`lapifex_c89_grammatica.c:2420-2488`) is off-by-one
  vs the grammar string's `/* Pnn */` labels (comment says P96 for the production
  the grammar labels P97). Executable `casus` labels match; the comments rotted.
  Annotations + generated dispatch kill this class (comments are generated).
- **R4 — STML has no schema/validation facility** (attributes allowed on any
  element, no validate call). The generator IS the validator — annotation
  validation is a first-class generator responsibility, not a lint afterthought.
- **R5 — arbor2's union node costs max-arm for every node.** `datum.declaratio`
  has 21 fields; every BINARIUM pays for them. Relevant to the node-representation
  decision (S21).

## Complication register

- **S18 — Slot mapping must be inline per-symbol, not parallel attributes.**
  `loci="sinister,tok,dexter"` (position-indexed attribute) re-creates the
  renumbering hazard at symbol level — editing the RHS silently shifts the map
  (R3 is this bug's documentation-level twin). Decision: annotations ride the
  symbols — `<productio genus="binarium">expressio@sinister PLUS@tok_operator
  terminus@dexter</productio>`. Reader change: split atoms at `@`.
- **S19 — The trivia invariant becomes a GENERATION-TIME check.** Rule: a
  production with one RHS symbol and no genus is a pass-through; any production
  with >1 symbols MUST declare a genus, and EVERY terminal in it must map to a
  token slot. The generator errors on unowned tokens — v1's entire
  trivia-LOSS class (17-fix graveyard) becomes statically impossible, before any
  code runs.
- **S20 — Multi-production genus unification.** Productions sharing a genus get
  the UNION of their slots; unfilled slots are NIHIL per production; slot-kind
  conflicts (node in one production, token in another) are generation errors.
  This dissolves per_clausula's 8 variants into one genus with optional slots.
- **S21 — Node representation (the sim's central decision).** Chose: uniform node
  = fixed header (genus, span, fons, standard-tag, pater, semantic fields) + a
  per-genus SLOT ARRAY of tagged SilvaValor slots, with generated layout tables,
  generated typed accessors (`silva_binarium_sinister(n)` — checked), and
  generated children-iteration driven by the tables. Rejected: generated union
  structs (arbor2-style — every node pays max-arm size, R5; children() needs a
  giant generated switch) and hand structs + glue (drift returns). Slot-array
  nodes are SMALLER than the union (binarium: header+3 slots vs 21-field arm),
  generically traversable (selectors/STML/transforms all table-driven for free),
  and type safety is recovered through generated accessors. Semantic passes write
  header fields, never slots.
- **S22 — Lists kill 0xDEAD structurally.** SilvaValor gets a XAR variant; list
  rules are annotated `modus="lista-initium"` / `modus="lista-appendere"` and
  ALWAYS produce list values (a one-element list is a list — lapifex's
  single-node-or-wrapper dual representation, the 0xDEAD bug class, cannot be
  expressed).
- **S23 — Separated lists interleave.** `lista COMMA elem` appends BOTH the
  separator token and the element into one ordered list of tagged values
  [node, token, node, ...]. Commas owned (invariant), order explicit (roundtrip
  trivial), children() filters to nodes, serializer walks all entries.
- **S24 — per-clausula becomes a real tree node.** The 8 grammar variants all
  declare genus="per-clausula" with optional slots; the smuggled PerClausula
  struct and its pointer-reinterpretation die. The for node holds a clausula
  child — honest, queryable structure (`per-sententia > per-clausula`).
- **S25 — specifiers_ordine emerges instead of being maintained.** decl_specifiers
  is a list rule accumulating specifier tokens/nodes in source order; declaratio
  maps that list into one slot. Order preservation is a property of the list, not
  35 hand-maintained handler updates (arbor2 worklog P148–P547). Semantic flags
  (storage class, qualifiers, est_typedef) move to a post-pass reading the list —
  construction stays purely structural.
- **S26 — GLR speculation forbids two things constructions always did (biggest
  catch).** Reductions are speculative — forks die. Therefore: (a) constructions
  must be PURE (allocate + fill only); typedef/oracle registration moves to a
  post-accept pass per top-level slice — the generator can't even express side
  effects, which forces the discipline; (b) **pater pointers cannot be assigned
  during construction** — a subtree shared by two live forks gets its pater
  overwritten by whichever reduces last (latent cross-fork corruption bug present
  in arbor2/lapifex today: `_nodus_binarium` sets `sinister->pater` at
  `lapifex_c89_grammatica.c:645-646`). Decision: pater is assigned by a
  post-accept fixup walk along the committed tree. Span propagation stays in
  construction (reads children, writes own node — pure).
- **S27 — pater × AMBIGUUS.** Shared children of ambiguous alternatives get pater
  from the canonical spine; re-canonicalization re-runs local fixup on the
  affected region. Rule recorded; needs a test with a re-canonicalized tree.
- **S28 — The manu escape hatch may have ZERO users in the core grammar.** Every
  hard case that motivated hand code in lapifex (0xDEAD, PerClausula, specifier
  chains) dissolved into annotation features. Kept anyway (`manu="fn"` emits a
  typed call + prototype; missing implementation = compile error) as cheap
  insurance for genuinely irregular future cases.
- **S29 — Generated artifact inventory** (dev-time tool output, all committed):
  genus enum; slot-layout tables; selector-tag string table (genus names ARE the
  kebab selector/STML tags); the single construction dispatch (one function,
  called from both GLR passes — the dual-ordinarius lesson enforced by
  generation); typed accessors; children tables; baked ACTION/GOTO/production
  tables with action kind RECUPERARE + `pretium` (cost) field reserved per §8.2;
  terminal map for the adaptator. Grammar moves from a C string literal to
  `grammatica/c89.stml` on disk — read by the dev-time generator, so
  -Wno-overlength-strings pressure drops.
- **S30 — One genus registry, two sources.** CONDITIONALIS regions, COMMENTUM,
  DIRECTIVUM, ERROR, AMBIGUUS are not built by productions but need selector
  tags, children tables, and serializer coverage. A `<genera-extra>` section in
  the grammar file declares structural genera with explicit slot layouts;
  generator merges both sources and emits a total registry. "Formatter fails
  loud on unknown genus" becomes checkable: the emitter skeleton is generated
  with a case per genus; structural genera get manu emitters.
- **S31 — The generator's validation suite** (it is the schema, per R4): unknown
  symbols; unannotated terminals in node productions (S19); slot-kind conflicts
  (S20); genus collisions across grammar/structural sources; list-mode misuse;
  missing manu implementations (via emitted prototypes); optional per-production
  `id=""` for stable error messages and debugging (kills R3's drift).
- **S32 — Debug-mode generated assertions.** The dispatch asserts value-tag
  matches slot kind on every store (annotation bugs surface at first parse, not
  as garbage trees), and counts token consumption per reduction (each terminal
  stored exactly once — the single-owner invariant checked dynamically too).

## Frozen annotation format v0 (the exit deliverable)

The test grammar, fully annotated:

```xml
<grammatica>
  <terminalia>
    <terminalis titulus="NUMBER" genus="SILVA_LEX_NUMBER"/>
    <terminalis titulus="PLUS"   genus="SILVA_LEX_PLUS"/>
    <terminalis titulus="STAR"   genus="SILVA_LEX_STAR"/>
    <terminalis titulus="LPAREN" genus="SILVA_LEX_LPAREN"/>
    <terminalis titulus="RPAREN" genus="SILVA_LEX_RPAREN"/>
    <terminalis titulus="EOF"    genus="SILVA_LEX_EOF"/>
  </terminalia>

  <regulae>
    <regula titulus="expr">
      <productio genus="binarium" id="expr-plus">
        expr@sinister PLUS@tok_operator term@dexter
      </productio>
      <productio>term</productio>                      <!-- pass-through -->
    </regula>
    <regula titulus="term">
      <productio genus="binarium" id="term-star">
        term@sinister STAR@tok_operator factor@dexter
      </productio>
      <productio>factor</productio>
    </regula>
    <regula titulus="factor">
      <productio genus="parenthesis" id="factor-paren">
        LPAREN@tok_apertum expr@internum RPAREN@tok_clausum
      </productio>
      <productio genus="folium-numerus" id="factor-num">
        NUMBER@tok_valor
      </productio>
    </regula>
  </regulae>

  <genera-extra>
    <genus titulus="error"     slots="tokens:lista-token"/>
    <genus titulus="ambiguus"  slots="interpretationes:lista-nodus canonica:index"/>
  </genera-extra>

  <initium>expr</initium>
</grammatica>
```

Slot-kind vocabulary: `@name` on a nonterminal → node slot; on a terminal → token
slot; `@name+` → append to accumulating list slot; `modus="lista-initium|
lista-appendere"` for list rules; slot kinds in genera-extra: `nodus`, `token`,
`lista-nodus`, `lista-token`, `lista-mixta`, `index`.

## Design changes to fold into spec-v2

1. Node representation: uniform header + per-genus slot array + generated layout
   tables/accessors/children (S21) — replaces any implied per-genus union.
2. Constructions are PURE; oracle registration and pater assignment are
   post-accept passes (S26, S27). This is a GLR-correctness requirement, not a
   style choice — and it names a latent cross-fork pater bug in arbor2/lapifex.
3. Semantic classification (storage flags, est_typedef, qualifiers) moves out of
   construction into a post-pass over specifier lists (S25).
4. The generator is the schema (R4): validation suite per S31; single total genus
   registry from grammar + genera-extra (S30); debug assertions generated (S32).
5. Baked action entries carry `pretium` cost field + RECUPERARE kind now (§8.2
   obligation discharged in the table format).

## Open questions for Fran — ANSWERED 2026-07-02

*Interview transcript: silva-simulatio-3-interview.md. Resolutions folded into
silva-spec-v2.md §9.*

1. Node representation: **uniform header + slot array** with generated layout
   tables + typed accessors (this IS solarium's code-as-database representation).
2. pater: **post-accept fixup pass**; constructions pure.
3. per-clausula: **real node in the tree**.
4. Annotation syntax: **symbol@slot inline** — frozen.
5. Structural genera: **<genera-extra> inside the grammar file** — one registry.

Also decided: generated code fully readable + Latin-commented (same standards as
hand code); production `id` mandatory on node productions. Context recorded:
solarium = 3D IDE (code as database + 3D structure + literate programming);
speculum meta-language layer to be shared later.

## What got EASIER than spec'd

- The manu escape hatch may be unused in the entire core grammar (S28) — every
  known-hard construction case dissolved into annotation features.
- specifiers_ordine stops being maintenance (S25) — it's just a list slot.
- Dangling-else grammar factoring (compar/incompar) needs nothing special: both
  factored productions declare genus="si-sententia" and the factoring disappears
  from the tree.
- The dual-ordinarius bug class, the 0xDEAD class, the valori-indexing class, and
  the trivia-LOSS class are all eliminated at generation time rather than
  defended against in review.

## Verdict

No blockers. The annotation format froze cleanly (v0 above) and is smaller than
expected — two attributes (genus, modus/manu/id) plus one inline suffix (@slot,
@slot+) covers the entire C89 grammar as far as simulated. The deepest finding is
S26: GLR speculation makes construction purity and deferred pater assignment
*correctness requirements*, and names a real latent bug in the existing engines.
The node-representation decision (S21) wants Fran's sign-off before the generator
work starts. Recommend the post-sim interview, then Simulation ② (conditional
mechanics — frontier measurement still owed) or straight to ④ (amalgamation,
cheapest).
