# Silva quaestio — the selector surface (design draft)

*2026-07-06, drafted for Fran's correction (M2.0 NOMINA method). The
selector syntax is a thousand-year surface: it outlives implementations the
same way genus tags do — this doc decides the LANGUAGE; the INTENTIO that
follows decides the build. Sources: silva-brainstorm.md (Latin tags DECISUS;
"query engine first among post-1.0"), arbor_ce_lxxxix-brainstorm.md (CSS
syntax, $captures, pseudo classes, transform DSL), v1 arbor_quaestio
(model + 100-assertion bar + four catalogued bugs), VISIO pins.*

## I. What carries over from v1 (the proven model)

- **Compiled query object**: compilare(selector) → SilvaQuaestio*, reusable;
  exsequi(q, tree) → results; congruit(q, node) single test.
- **Selector structure**: chains of compound selectors joined by
  combinators; each compound = type test + attribute tests + pseudos +
  captures.
- **Results**: {nodus, capturae} — capture map per result.
- **Attribute operators**: = ^= $= *= plus bare-existence.
- **Pseudo classes**: standard (structural) + semantic + user-registered.

## II. What silva changes (the new axes)

1. **Tags are the DECISUS genera, mechanically** — `definitio-functionis`,
   `declaratio`, `commutatio`. The engine is GRAMMAR-AGNOSTIC: compilare
   takes the baked REGISTRUM (tag string → genus index), so quaestio works
   for any silva grammar, c89 included, sceletum included. Zero new naming:
   M2.0 already decided the vocabulary. The v1 bar's English inputs
   (function-definition, if-statement…) are TRANSLATED at harvest, exactly
   as the lapifex bars were re-asserted against DECISUS genera.
2. **Attributes are LOCUS NAMES** — the grammar's own slot vocabulary.
   `binarium[tok_operator="*"]`, `declaratio[tok_titulus^="silva_"]`
   (proposal — see question Q2 on verbatim-vs-alias). Token loci compare
   against the token's valor (chorda comparison, mensura-aware). This makes
   the annotation format's locus names the attribute surface: one
   vocabulary, greppable against the grammar file, no synonym table.
3. **Ambigua**: matching walks the CANONICAL SPINE by default (the vista
   precedent; pater threading precedent), with opt-in pseudos to widen:
   `:lectiones` (match inside ALL retained readings) and `:ambiguum`
   (match wrapper nodes themselves — "show me everything unresolved" is a
   one-liner: `:ambiguum`). Question Q3.
4. **Conditional arms**: taken arms are ordinary tree — matched always (the
   wildcard pin honored structurally). Untaken arms are cruda bytes until
   the config-query milestone — quaestio-v1 CANNOT match into them; named,
   with `:sumptus`/`:omissus` ramus pseudos shipping now so arm-aware
   queries exist from day one.
5. **Tokens are matchable leaves** via attributes on their owning node's
   loci (option 2 above), NOT as standalone selector subjects — nodes stay
   the subject universe (v1 precedent; token-level queries are the strata/
   lexer surface, not the tree surface).

## III. The surface (proposal — Latin per Q1)

```
selector      := catena (virgula catena)*          /* a, b = union */
catena        := compositum (combinator compositum)*
combinator    := ' ' (descendens) | '>' (filius)
               | '+' (frater proximus) | '~' (frater sequens)
compositum    := genus-tag? pars*
pars          := [locus op "valor"] | :pseudo | :pseudo(arg) | $captura
op            := = | ^= | $= | *=
```

- `*` universal selector (any node).
- Pseudos, structural: `:primus` / `:ultimus` (first/last node child),
  `:habet(selector)` (node contains a match — the big v1 gap; makes
  "functions that call malloc" expressible:
  `definitio-functionis:habet(vocatio[tok_titulus="malloc"])`),
  `:non(selector)` (negation).
- Pseudos, semantic (v1 bar parity, Latin names per Q1): `:vocat(f)`
  (calls), `:definit(x)` (defines), `:utitur(x)` (uses), `:reddit`
  (has return). Norma pseudos `:c89`/`:c99`/`:extensio` ship WHEN the
  standard-tag population lands (recensio item #5 — the token field
  exists, unpopulated; the pseudos are the pull that funds it).
- Pseudos, silva-specific: `:ambiguum`, `:lectiones`, `:sumptus`,
  `:omissus` (§II.3-4).
- Custom pseudos: registrable (v1 API, made functional — bug #4).
- Captures: `$nomen` after any compound; ALL captures along the matched
  chain bind into the result (bug #1 designed out: the match records
  every hop, not just the subject).

## IV. Engine notes (design, not yet INTENTIO)

- Matching: top-down walk with per-node chain-state (v1 model), results
  deduped by subject-node pointer (bug #2 designed out). children() +
  uniform loci = the generic child enumeration v1 hand-built
  (_iterare_liberos) comes free from silva's node model.
- compilare(piscina, tabularium, selector) — registrum-driven tag lookup;
  unknown tag = LOUD compile error (v1 silently matched nothing — the
  suite pins "nonexistent-node-type" behavior; silva flips it to fractura,
  with the suite's expectation updated at harvest and the change RECORDED).
- Attribute lookup: locus name → layout index via registrum (dead-path
  bug #3 designed out by going through the same tables the emitter trusts).
- Results in caller piscina; query object reusable across trees.
- Module: fontes/silva_quaestio.{h,c}; public surface in silva.h + hospes
  same-change; c89-independent (works on any registrum).

## V. Scope (proposal, Q4)

Quaestio milestone = SELECTORS ONLY (this doc). The transform DSL
(<%adde>/<%praepone>/<%substitue> — pattern→template with mutation tags)
is its own follow-on that TARGETS selectors; the formator rides it. The
v1 100-assertion bar (translated) + new-axis fixtures (ambigua, rami,
:habet, captures-fixed) = the acceptance bar.

## VI. DECISUS (Fran, 2026-07-06 — all four on the recommendation)

1. **Pseudos speak LATIN**: :vocat(f), :definit(x), :utitur(x), :reddit,
   :habet(sel), :non(sel), :primus, :ultimus, :ambiguum, :lectiones,
   :sumptus, :omissus, :c89/:c99/:extensio (norma family gated on
   standard-tag population, recensio #5). One language across the whole
   surface.
2. **Attributes = locus names VERBATIM** ([tok_titulus="main"],
   [tok_operator="*"]) — one vocabulary, greppable against the grammar,
   no synonym table ever.
3. **Ambigua: canonical-spine default**, :lectiones widens to all retained
   readings, :ambiguum matches wrappers. Predictable counts; the wildcard
   pin is honored by the OPT-IN being first-class, and structurally for
   conditional arms (taken arms always in the walk).
4. **Milestone scope: SELECTORS ONLY.** The transform DSL
   (<%adde>/<%praepone>/<%substitue>) is its own follow-on targeting
   selectors; the formator rides transforms.

This section is the quaestio surface's DECISUS — names permanent, same
standing as genera-c89.md.
