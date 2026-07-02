# Silva — Specification (v1)

*The 4th attempt at the C89 parsing substrate. Successor to arbor v1 (recursive
descent), arbor2 (hand-written GLR), and lapifex (generated LALR). Written from the
silva interview (see silva-brainstorm.md, 2026-07-01). A v2 of this spec with
codebase-specific planning follows after agent exploration of the existing code.*

---

## 1. Purpose and positioning

Silva is a **code-as-data substrate for C89**: it turns C source into a structured,
queryable, transformable, losslessly-reconstructable representation. It is not a
compiler and does not compete with clang/gcc on their axis. The honest target:

> **Best-in-niche code-intelligence and source-manipulation substrate for C89** —
> provenance-complete, byte-exact roundtrippable, queryable, transformable,
> dependency-free, and able to parse through missing information.

No existing tool unifies provenance layers + byte-exact roundtrip + selector queries +
transform DSL + type/index + parse-all-conditional-branches + ambiguity-for-unknowns.
tree-sitter is syntactic-only; clang is lossy-by-design and a heavy dependency;
Coccinelle and srcML each cover one slice.

**Consumers**, in rough order of arrival: solarium widgets (STML-rendered code views),
coloratio-for-C (semantic highlighting), the code formatter, the query/index oracle for
coding agents (MCP), smaragda knowledge-graph feeding, the transform DSL ("library of
code surgeries"), AST-level diff.

**Future direction (recorded, not designed):** a runtime C compiler enabling a REPL /
C-as-scripting inside solarium as an immersive IDE is a plausible long-term consumer.
Implication for silva now: *nothing in the data model or API may assume "analysis
only."* The semantic layer (types, symbols, layout-relevant facts) should be complete
enough that evaluation could someday be a consumer; the AST is a faithful program
representation, not a documentation artifact.

## 2. The maximalist rule

The feature vision of the ce + v2 brainstorms takes precedence over subset shortcuts.
The three prior attempts each failed not for missing features but because **data-model
decisions foreclosed features**: v1's heuristic commitment, arbor2's hand-maintained
tables, lapifex's s64 value system / Lexema*-not-Token* / deliberately conflict-free
grammar. Silva's data model is maximalist from day one; implementation *order* stays
pragmatic. Any proposal that makes a brainstorm feature unrealizable is rejected at
design time, even if the feature itself ships later.

## 3. Requirements (consolidated)

**Parsing scope**
- Full C89 grammar including the preprocessor (real macro expansion, layered).
- latina.h keyword macros first-class via compiled-in definitions flowing through the
  normal expansion layer (full provenance). Native-keyword mode deferred.
- Common C99-isms (`//` comments, designated initializers, VLAs, mixed declarations)
  parsed but tagged by standard; lint queries can flag them. Nothing past C99.
- Dependable C rules annotated for the lint layer (post-parse or on-demand).
- Platform-specific constructs (`__attribute__` etc.) parsed as opaque.

**Fidelity and provenance**
- Byte-exact roundtrip: source → parse → emit == source. Also from the AST itself
  (nodes own their tokens+trivia), and through STML: code → parse → STML → load →
  emit == source.
- Layers: layer 0 = source; layer N+1 = one full expansion pass over layer N, until
  fixpoint. Every expanded token carries a full provenance chain (nested macros = full
  chain; `##` = merged provenance linking both parents, left primary). Provenance
  metadata always allocated (no NULL-check discipline).
- Comments are first-class nodes with explicit position metadata (own-line /
  end-of-line / inline / multiline), auto-attached by heuristic. Trivia = whitespace +
  line info; NEWLINE tokens exist; line continuations handled with roundtrip metadata.
- Trivia ownership is a **formal invariant** decided once (every byte belongs to
  exactly one token; innermost node owns leading trivia), enforced in construction —
  not per-construct whack-a-mole.

**Robustness**
- Parse through missing information: unknown typedefs/macros yield represented
  ambiguity, never failure. Error recovery always produces error nodes containing
  skipped tokens (recovery granularity configurable).
- Ambiguity preserved, not guessed: genuine undecidables land in AMBIGUUS nodes
  carrying every surviving reading.
- All conditional-compilation branches parsed and queryable.

**Downstream (the actual point)**
- CSS-selector query DSL: Latin kebab-case tags derived from node genera
  (definitio-functionis, si-sententia), attribute predicates, `$name` captures,
  standard + semantic + user-defined pseudo-selectors.
- Transform DSL: pattern → template with `<%append>` / `<%prepend>` /
  `<%replace selector="…">`, AST-level with token-level escape.
- Two-pass semantics: expansion + symbol table, then type resolution. Full project
  index (memory-only) with proper C visibility rules.
- STML serialization, one-to-one with the tree, trivia preserved.
- Semantic tokens for highlighting.

**Constraints**
- Pure C89, zero dependencies, passes the rhubarb flag set (-pedantic -Wall -Wextra
  -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes
  -Wmissing-prototypes -Wwrite-strings).
- Single-threaded. Everything in Latin (identifiers, comments, messages).

## 4. Distribution

- Deliverable: **`silva.c` + `silva.h`**, assembled by the amalgamator. Consumers
  (solarium first) compile one file and include one header.
- **Amalgamator**: a small C89 tool in `silva/instrumenta/` — resolves internal
  includes in dependency order, applies the symbol prefix map, emits the pair. Tested
  with credo. Endgame: it uses silva's own parser instead of text rules.
- **Vendoring by prefix-rename**: canonical rhubarb libs (piscina, chorda, xar,
  internamentum, tabula_dispersa) stay single-source in rhubarb/lib; the amalgamator
  renames their symbols (piscina_creare → silva_piscina_creare) and internalizes
  (static) everything non-public. Zero collision risk if the host also links rhubarb
  libs; fixes flow in on re-amalgamation.
- **Tables baked in**: the generator runs at development time (lemon model); generated
  tables + construction code are ordinary committed C that the amalgamator includes.
  No runtime table construction.
- Public prefix **silva_** throughout.

## 5. Architecture

### 5.1 Layout

```
silva/
  fontes/           modular sources (silva_lexema.c, silva_expandere.c, ...)
  grammatica/       annotated C89 grammar (STML) + generated tables/construction
  instrumenta/      generator (lapifex fork), amalgamator, measurement harness
  probationes/      credo test suites + fixture corpus (incl. ported suites)
  silva.c silva.h   generated deliverable (committed)
  *.worklog.md      per-module worklogs
```

Fully self-contained subproject (knotapel precedent). rhubarb lib/'s lapifex stays
untouched as the historical artifact until the freeze-then-delete milestone (§8).

### 5.2 Core data model

- **SilvaToken**: interned text, genus, trivia (leading/trailing), byte offset +
  line/column (both cached), file of origin, provenance chain (always allocated;
  layer-0 tokens carry an explicit FONS origin). Comments and preprocessor directives
  are nodes, not trivia.
- **SilvaValor** — the reduction value contract: tagged struct
  `{ genus; union { Token*; Nodus*; Xar*; …opaque } }` flowing through the GSS and
  every reduction. Runtime-checkable; intermediate accumulations (specifier lists,
  for-clauses) are honest types. This replaces lapifex's s64 casts and 0xDEAD
  wrappers and is the single most load-bearing contract in silva.
- **SilvaNodus**: genus (maps mechanically to the Latin kebab selector/STML tag),
  child slots, token slots with trivia, standard/Dependable-C tags, condition
  membership. Nodes own enough to roundtrip from the tree alone.
- **Layers**: token streams per expansion layer, linked by provenance; segments
  (macro-table states, created only on #define/#undef) queryable at line granularity.
- **AMBIGUUS nodes**: alternatives share tokens; one interpretation marked canonical
  by position heuristics (typedef wins in type position, etc.). Downstream follows
  the canonical spine unless it asks for alternatives.

### 5.3 Modules

1. **silva_lexema** — lexer (v2-lexer lineage: NEWLINE tokens, line continuations
   with roundtrip metadata, C89 comments; C99 `//` tagged).
2. **silva_expandere** — **fresh implementation** against the Token contract, carrying
   arbor2's 134-test suite + v1's preprocessor tests as the compatibility bar.
   Layered expansion (strict layers), segments for #undef, stringify/paste per
   standard with merged provenance, include resolution (learning vs full includes),
   compiled-in stdlib + latina.h definitions, API-injected macros/typedefs with
   graceful provenance. **Conditionals designed in from day one** (below). Fixes the
   #undef/#define-after-first-line leak by construction.
3. **Generator** (lapifex fork in instrumenta/) — reads the annotated grammar, emits
   conflict-preserving LALR tables **and AST-construction code**. Per-production
   annotations declare node genus + RHS→slot mapping; a hand-written escape hatch
   covers irregular productions. Adding a production cannot forget its handler.
4. **silva_glr** — GSS runtime consuming generated tables: fork on conflict, merge on
   reconvergence, **localized AMBIGUUS packing ported from arbor2's
   `_mergere_compatibiles`** (reconverging forks share structure; differences pack
   into AMBIGUUS in place — never N whole-file results).
5. **Oracles** — non-destructive fork pruning: the typedef table (demoted from
   lapifex's destructive pre-scan) and the conditional evaluator (arbor2's orphaned
   `conditio_evaluare` finally wired in). When the oracle knows, the dead fork dies;
   when it doesn't, both readings survive to AMBIGUUS.
6. **silva_scribere** — roundtrip emitter (byte-exact) + the opinionated formatter
   (single style, refinable).
7. **silva_stml** — one-to-one STML serialization with trivia, load→emit fidelity.
8. **Post-1.0**: silva_quaestio (selector engine — algorithms mined from
   arbor_quaestio, rewritten against the generic node interface), silva_typus +
   silva_index (two-pass semantics, full visibility rules), transform DSL, semantic
   tokens.

### 5.4 Conditional compilation: localized sub-parses

When expansion encounters an #if/#elif/#else region, each arm is parsed as its own
bounded sub-parse; results land as rami of a **CONDITIONALIS node** at that point in
the enclosing tree. Fork cost is local; arms sharing a condition are coupled by
construction (mutual exclusion respected); the enclosing parse never multiplies.
Cross-arm syntax (an arm whose tokens don't brace-balance) degrades to a coarser
enclosing region. Queries default to wildcard-across-branches; a configuration API
selects concrete conditions, evaluated by the oracle. Directives persist as
interleaved-sibling nodes (conditional directives live in layer 0 only). The frontier
is **measured on a real nested-#ifdef header** during simulation before anything
depends on it.

### 5.5 Late information

When the oracle learns an answer after a parse (injected typedef, resolved include),
matching AMBIGUUS nodes are **re-canonicalized in place** — flip the canonical
interpretation, update the index incrementally; no reparse unless requested. An
identifier table per file supports "should this reparse?" checks.

### 5.6 Memory model

**Two-arena split**: per-parse scratch arena (GSS, dead forks, worklists — wholesale
`vacare` at parse end) and result arena (tokens, tree, index — lives until the caller
releases the result). Loser-fork reclamation at merge points becomes optional
optimization, not correctness. Peak memory ≈ live data, structurally — correct by
construction for long-running solarium / MCP hosting.

### 5.7 Public API shape

Opaque **SilvaContextus** owns configuration (include paths, injected macros/typedefs,
latina.h toggle, stdlib definitions, conditional configuration) and long-lived state
(interning, oracle, index). Parses return opaque result handles queried for
layers/tree/errors (a full-result struct API exists as a convenience). Contexts are
reusable across files; implicit single-file project for bare parses; filetree/project
construction utilities. Errors are objects returned to the caller, with a separate
formatted-printing utility. Both streaming/callback and tree-building modes.

## 6. Development process

- **Simulations first** (all four approved): ① token contract + fresh expander
  vertical; ② conditional sub-parse mechanics incl. frontier measurement;
  ③ annotated-grammar → generated construction (annotation syntax committed only
  after this); ④ amalgamation/prefix-rename on real vendored sources with a
  solarium-side compile.
- **Walking skeleton** as milestone 1: tiny grammar subset (declarations +
  expressions), real Token contract, real expander core, generated tables +
  construction, one genuine fork→AMBIGUUS case, byte-exact roundtrip of one real
  file, through the amalgamator, compiled in solarium. Every interface proven under
  load before any module is "finished"; then widen.
- **Grammar authored fresh** from the C89 standard's Annex A, ambiguity deliberately
  present (IDENTIFICATOR in type positions), annotations and roundtrip token slots
  from the first production. Lapifex's grammar is reference only (its factoring
  encodes conflict-avoidance, which silva reverses); its expression precedence
  hierarchy and dangling-else treatment are known-good references.
- **Formatter-first correctness oracle**: byte-exact roundtrip runs continuously from
  the walking skeleton on.
- **Performance posture**: correctness first, no budget in 1.0 — but the harness
  measures per-file parse time and peak arena bytes over the rhubarb corpus from day
  one, so the cost curve is visible history before solarium makes demands.

## 7. Testing

- credo suites per module in silva/probationes/, runnable via a compile script
  mirroring ./compile_tests.sh.
- **Carried compatibility suites**: arbor2 expander (134 tests), v1
  preprocessor/roundtrip fixtures, lapifex parsing suites (expr/decl/sent/func),
  arbor2 roundtrip fixtures — ported as silva coverage widens; parity on these gates
  the delete milestone.
- **Corpus phases** (amended 2026-07-02 — Fran added solarium): (1) every .c/.h in
  rhubarb roundtrips byte-exact (latina.h and all); (1.5) **../solarium** — ~150
  files of plain English C89 with ZERO latina usage: the dimension rhubarb cannot
  test (the parser WITHOUT the latina definition set), and the literal deployment
  host, so the amalgamation's host-pollution and symbol-collision tests target the
  real thing; (2) classic external C89 (Lua, zlib, the SQLite amalgamation)
  vendored as stable offline fixture snapshots. Solarium enters at
  integration-test time (expander + parser phases onward), not as vendored
  fixtures — it lives next door and evolves.

## 8. Fate of the predecessors

Freeze all three generations untouched as reference + test-corpus source while silva
is built. When silva passes the ported suites and roundtrips the rhubarb corpus,
delete arbor v1, arbor2, and lapifex from lib/ — git history is the archive. What is
mined before deletion: v1's downstream algorithms and trivia-ownership lessons (rules,
not bindings), arbor2's local-merge mechanism and GLR debugging lessons, lapifex's
generator core and grammar reference.

## 9. Milestones

- **M0** — four simulations; decisions folded back into this spec.
- **M1** — walking skeleton (§6), end-to-end through amalgamation.
- **M2…** — widen: full expression/statement/declaration grammar; expander to full
  directive coverage incl. conditionals; oracle wiring; error recovery; comment
  attachment; roundtrip over growing rhubarb corpus.
- **1.0 (substrate proof)** — byte-exact roundtrip of the entire rhubarb corpus +
  STML load→emit fidelity + amalgamated silva.c compiling clean in solarium.
- **Post-1.0, in order** — query engine + selectors → coloratio/semantic tokens →
  type resolver + project index → transform DSL. External corpus phase 2 runs
  alongside.

## 10. Risks

- **Reduce-reduce correctness** in the generated-conflict runtime: preserving
  conflicts is easy; handling every configuration the generator now emits is where
  new bugs live. Mitigated by the walking skeleton's genuine-fork requirement and
  ported arbor2 test knowledge.
- **Conditional frontier explosion**: bounded structurally by localized sub-parses,
  but measured early (simulation ②) before anything depends on it.
- **Annotation format lock-in**: the grammar annotation syntax is API — simulated
  (③) before commitment, escape hatch mandatory.
- **Prefix-rename correctness**: text-level renaming of C identifiers is haunted
  (strings, macros, partial matches); simulated (④) on real sources; endgame is
  silva-powered renaming.
- **The coverage mountain**: architecture doesn't climb it; the corpus discipline
  (§7) is the plan, and it is grinding work.

## 11. Non-goals for 1.0

Incremental re-parsing (full-file reparse is the model) · threading/re-entrancy ·
native latina keyword mode (deferred; revisit if speculum-as-language needs it) ·
performance budgets · disk-persisted index/caching · anything past C99 · compiling
or evaluating code (recorded as a future direction only, §1).

## 12. Open questions for spec v2 (agent exploration)

1. Exact reuse boundary of the lapifex generator: what of lapifex_generare.c /
   lapifex_glr.c carries into the fork vs gets rewritten (LALR core, conflict
   emission, table format for baked generation).
2. Port shape of arbor2's `_mergere_compatibiles` + AMBIGUUS structures into a
   generated-table GSS with SilvaValor.
3. What of arbor2_token/arbor2_lexema survives contact with the new Token contract
   (fields, provenance structs, trivia representation).
4. Full inventory of the vendored-lib public surfaces for the prefix map + include
   dependency order for amalgamation.
5. Annotation syntax proposal grounded in the actual STML tooling.
6. Test fixture inventory across all three generations (what exactly gets ported,
   where it lives).
7. What v1's arbor_quaestio/arbor_formator/arbor_typus/arbor_index contain that must
   be mined before deletion (rules, heuristics, algorithms).
