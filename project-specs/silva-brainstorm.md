# silva - Interview Brainstorm

Raw transcription of the silva interview (2026-07-01). Silva is the 4th attempt at the
C89 parsing substrate, following arbor v1 (recursive descent), arbor2 (hand-written GLR),
and lapifex (generated LALR).

## Context settled in conversation before the interview

These decisions were made during the orientation discussion that preceded the interview
(reading arbor_ce_lxxxix-brainstorm.md, arbor-v2-all-brainstorms.md, and an external
retrospective document, with claims spot-checked against the source):

- **Name: silva.** After three arbors, a forest — the layered model is literally many
  trees (expansion layers, conditional forks, files of a project).
- **Development in rhubarb, new top-level folder** (knotapel precedent for a
  semi-autonomous subproject). Deliverable is a **single-file `silva.c`** (SQLite
  amalgamation style: develop modular, ship generated) consumed by the **solarium**
  project.
- **Maximalist rule:** the feature vision of the ce + v2 brainstorms takes precedence.
  The lapifex failure analysis: the corners were data-model decisions that foreclosed
  features (s64-only reduction values, Lexema* instead of Token* through the parser,
  conflict-free grammar making the fork machinery dormant), not missing features.
  Silva's data model is maximalist from day one; implementation order stays pragmatic.
- **Lapifex-as-lemon:** parse tables are generated at development time and baked into
  the amalgamated silva.c (kills the ~7s runtime table build).
- **Keystone recombination (verified against source):** the lapifex generator already
  supports conflict-preserving tables (`conflictus_intentus`, multi-action lookup) and
  lapifex has its own 716-line GSS GLR runtime that forks/merges. The C89 grammar never
  forks because it was *written* conflict-free. The genuinely missing piece is
  **localized** ambiguity packing — arbor2's `_mergere_compatibiles` + AMBIGUUS
  mechanism — which must be ported into the generated-table engine. Unify typedefs,
  macros, and conditionals as one mechanism: fork on uncertainty, prune with a
  non-destructive oracle (`arbor2_conditio_evaluare` is a complete, orphaned #if
  evaluator that becomes the conditional oracle).
- **Implementation simulation(s)** before committing (the two 3-hour simulations before
  arbor2 phase 1 surfaced 27 real complications and their decisions held up).

---

## Q: silva.c must contain its own arena/string/vector/hash/interning internals (piscina, chorda, xar, tabula_dispersa, internamentum). If solarium also links rhubarb libraries directly, duplicate symbols collide at link time. How should the amalgamation handle this?

**A:** Prefix-rename at amalgamation. Canonical libs stay single-source in rhubarb/lib;
the amalgamator mechanically renames symbols (piscina_creare → silva_piscina_creare) and
makes everything internal static where possible. Bug fixes flow in on re-amalgamation;
zero collision risk; SQLite-style self-containment.

## Q: The GLR reduction value contract is where lapifex got cornered (s64 casts, 0xDEAD wrapper hacks, Lexema* instead of Token* so provenance died at the parser boundary). What should flow through the GSS and reduction callbacks in silva?

**A:** Tagged value struct. Reductions pass/return a SilvaValor { genus; union { Token*;
Nodus*; Xar*; ... } }. Runtime-checkable, kills blind casts and sentinel wrappers, lets
intermediate values (specifier lists, for-clauses) be honest types instead of smuggled
pointers.

## Q: arbor2_expandere has 134 passing tests but was built on the old token model, lacks all conditional directives (#if/#ifdef/#else/#endif), and has the #undef/#define-after-first-line token leak. For silva's expander?

**A:** Fresh rewrite, carry the tests. Same move v2 made against v1: new implementation
against silva's Token contract with conditionals designed in from day one (they're forks
in the layer model, not an afterthought); the 134-test suite plus v1's preprocessor tests
become the compatibility bar.

## Q: Where do silva's development files and the lapifex generator live?

**A:** silva/ owns everything. New top-level silva/ with its own sources, tests, grammar
STML, amalgamator, and a copy/fork of the lapifex generator as silva's table tool. Fully
self-contained subproject; rhubarb lib/ keeps its lapifex untouched as the historical
artifact.

---

## Q: Conditionals are the feature no attempt ever built and where fork explosion genuinely lives. When the parser encounters an #if/#else region, what should the GLR actually see?

**A:** Localized sub-parses. Each arm is parsed as its own bounded sub-parse; results
land as rami (branches) of a CONDITIONALIS node at that point in the enclosing tree.
Fork cost is local to the region, arms sharing a condition are coupled by construction,
and the enclosing parse never multiplies. Cross-arm syntax (an #if that closes a brace
opened outside) degrades to a coarser region.

## Q: When ambiguity survives to the final tree (unknown typedef from a missing header → AMBIGUUS node holding both readings), what should downstream consumers see by default?

**A:** Canonical reading + alternatives. Every AMBIGUUS node designates one
interpretation as canonical (position heuristics: typedef wins in type position, etc.).
Formatter, queries, and index follow the canonical spine unless a consumer explicitly
asks to see alternatives. Roundtrip is unaffected either way since all readings share
the same tokens.

## Q: Lapifex builds AST via a hand-written reduction callback — a ~200-case switch that grows with every production and is where the 0xDEAD-style hacks lived. For silva's full C89 grammar (likely 300+ productions once type-ambiguity and roundtrip slots are in), how should tree construction be specified?

**A:** Annotated grammar. The STML grammar declares, per production, the node genus and
how RHS values map to child/token slots; the generator emits construction code alongside
the tables. One source of truth — adding a production can't forget its handler (the
exact arbor2 dual-ordinarius bug class). Hand-written escape hatch for the genuinely
irregular cases.

## Q: What does silva 1.0 mean — the milestone that ships silva.c into solarium for the first time?

**A:** Substrate proof. Byte-exact roundtrip of the entire rhubarb corpus (through
latina.h macros and real headers) + STML serialization with load→emit fidelity. The two
most ruthless correctness oracles pass before any feature ships; highlighting and
queries follow as fast follows.

---

## Q: The memory audit found arena accumulation is the real leak class: dead GLR forks and per-parse scratch pile up for the arena's life (1 reset site vs 189 allocation sites in arbor2). For silva — which will live inside long-running solarium and possibly an MCP server — what's the memory model?

**A:** Two-arena split + designed-in reset. Each parse draws from a scratch arena (GSS,
dead forks, worklists — wholesale vacare when the parse ends) and a result arena
(tokens, tree, index — lives until the caller releases the parse result). Loser-fork
reclamation at merge points becomes optional optimization, not correctness. Peak memory
≈ live data, structurally.

## Q: Once silva reaches parity, what happens to the three existing generations (arbor v1's ~8 modules, arbor2's ~9,500-line engine + 27,600-line tables, lapifex)?

**A:** Freeze until parity, then delete. All three stay untouched as reference and as
the source of the compatibility test corpus while silva is built. When silva passes the
ported suites and roundtrips the rhubarb corpus, the old generations are deleted — git
history is the archive. No permanent double-maintenance.

## Q: Best-in-niche is won on the coverage mountain — robustly roundtripping real C, not curated snippets. Beyond rhubarb itself, what's silva's test corpus strategy?

**A:** Rhubarb first, then classic C89 codebases. Phase 1: every .c/.h in rhubarb
roundtrips byte-exact (this is the codebase silva must serve, latina.h and all).
Phase 2: progressively gnarlier external C89 — e.g. Lua, zlib, the SQLite
amalgamation — vendored as fixture snapshots so the corpus is stable and offline.

## Q: A parse produced AMBIGUUS nodes because a typedef definition was missing; later the oracle learns the answer (user injects the definition, or an include gets resolved). What happens to the existing tree?

**A:** Re-canonicalize in place. Both readings were retained, so resolving means
flipping which interpretation is canonical on matching AMBIGUUS nodes — no reparse,
index updates incrementally. Full reparse only if the caller asks. This is the v2
brainstorm's "targeted update" made concrete by the canonical+alternatives model.

---

## Q: What's the outermost shape of silva's public API — the thing solarium actually holds?

**A:** Project context + parse handles. An opaque SilvaContextus owns configuration
(include paths, injected macros/typedefs, latina.h toggle, compiled-in stdlib defs) and
long-lived state (interning, oracle, index). Parsing a file returns an opaque result
handle queried for layers/tree/errors; contexts are reusable across files — matching the
two-arena model and the v2 brainstorm's opaque-handle answer.

## Q: The grammar is now the single source of truth (tables + AST construction both generated from it). Where does silva's C89 grammar come from?

**A:** Author fresh from the standard. Write silva's grammar from the C89 standard's
Annex A grammar, with ambiguity deliberately present (IDENTIFICATOR in type positions),
node-genus/slot annotations, and roundtrip token slots designed in from the first
production. Lapifex's grammar stays as reference — its shape was dictated by
conflict-avoidance, the exact thing silva abandons.

## Q: After the substrate 1.0 (byte-exact roundtrip + STML) ships to solarium, which downstream capability gets built first?

**A:** Query engine + selectors. The CSS-selector engine over the proven tree — it's the
dependency of nearly everything else (transform DSL targets selectors, coloratio is
roughly a query for semantic classes, the index is queries persisted) and it's the
agent-oracle lever with the tightest feedback loop against rhubarb.

## Q: The amalgamator — the tool that assembles silva.c from modular sources with prefix-renaming. What is it?

**A:** C89 tool in silva/, ships nothing. A small silva/instrumenta program: resolves
internal includes in dependency order, applies the symbol prefix map, emits silva.c +
silva.h. Dogfoods the no-dependency philosophy, testable with credo, and there's a
satisfying endgame where it uses silva's own parser instead of text rules.

---

## Q: The public symbol prefix — what callers type and what the amalgamator renames vendored internals to. Which prefix family?

**A:** silva_ throughout. silva_contextus_creare(), silva_parsare(); vendored internals
become silva_piscina_*, silva_chorda_*. Fully legible, consistent with rhubarb library
style (piscina_, chorda_ are unabbreviated), and the deliverable reads as what it is.

## Q: Selector tag names — the query DSL's kebab-case node tags (the ce brainstorm settled kebab-case but not the language). What does a user type to match a function definition?

**A:** Latin tags. definitio-functionis, declaratio, si-sententia — tags derive
mechanically from node genus names, everything in the system speaks one language, and
the Latin-as-filter philosophy applies to the query surface too. STML serialization tags
match for free.

## Q: The original brainstorm floated an optional mode where the latina.h words are native keywords — si/redde/vacuum parse as keywords even with no latina.h available. With compiled-in latina.h macro definitions already flowing through the normal expansion layer (full provenance included), is native-keyword mode still wanted?

**A:** Defer — macros suffice. The compiled-in definitions make si parse correctly
through the standard layer model with provenance intact, which is strictly more honest
than keyword-hacking the lexer. Revisit native mode only if a real use case appears
(e.g. speculum-as-language work where latin source has no C expansion).

## Q: Which areas should silva's pre-implementation simulations attack? (multi-select)

**A:** All four:
1. Phase-1 vertical: token contract + expander (Token/trivia/provenance structures and
   the fresh expander with conditionals-as-regions)
2. Conditional sub-parse mechanics (region detection, arm token-stream construction,
   cross-arm brace imbalance, condition coupling, CONDITIONALIS assembly, frontier
   measurement on a real nested-#ifdef header)
3. Annotated grammar → generated construction (slot-mapping syntax in the STML, the
   escape hatch for irregular productions, how token slots + trivia land in nodes)
4. Amalgamation + prefix-rename (identifier-renaming rules on real vendored sources,
   solarium-side compile of the assembled silva.c)

---

## Q: Development sequencing philosophy for the substrate — how should silva sequence toward the substrate milestone, given arbor2 proved you can build excellent modules and still discover integration walls late (lapifex M3 is where provenance died)?

**A:** Walking skeleton first. Milestone 1 is a thin end-to-end thread: tiny grammar
subset (declarations + expressions), real Token contract, real expander core, generated
tables + construction, GLR with one genuine fork/AMBIGUUS case, byte-exact roundtrip of
one real file — through the amalgamator into a solarium-side compile. Every interface
proven under load before any module is "finished"; then widen coverage module by module.

## Q: Full-file reparse is the model, and solarium is an interactive app — if silva powers highlighting or code views on edit, reparse latency is user-facing. What's the performance posture for 1.0?

**A:** Correctness first, measure, no budget yet. The v2 brainstorm's "solve completely
first" stance: no optimization targets in 1.0, but the harness measures per-file parse
time and peak arena bytes over the rhubarb corpus from day one, so the cost curve is
visible history by the time solarium integration makes demands.

---

## Q: Any final thoughts, edge cases, or concerns before the brainstorm transcript and spec are written?

**A:** i think long term it is possible that there will be a runtime c compiler so that
c can be used in a repl environment or as a kind of scripting language at times? like
probably at some point solarium will end up being an immersive ide environment. we don't
have to worry about implementing a compiler or anything now but if it's helpful to know
that some of the code might be applied in that direction in the future i would throw it
out there
