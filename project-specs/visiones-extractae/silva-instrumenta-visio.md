# Visiones extractae — silva-instrumenta-visio.md

*Extracted and classified 2026-10-02 from
`project-specs/silva-instrumenta-visio.md` (2026-07-10, §XIV appended the
same day). A synthesis of a Fran + Claude conversation, not Fran verbatim;
claims the source attributes to Fran are marked (Fran). One claim per
bullet, worded as close to the source as possible; `Lnn` = source line.
The last section is my commentary, kept apart.*

*Marking: put `[+]` on the claims to keep for the distillate.*

*Tags (see `docs/taxonomiae.html`): `{PRINCIPIUM}` prime condition,
`{PRINCIPIUM+intervention, split}` a condition with its cure attached,
`{OCCASIO}` opportunity, `{INDICIUM}` clue, `{REGULA}` commitment,
`{VISIO-OPERIS}` project vision, `{DECRETUM}` load-bearing decision,
`{SUPELLEX}` furniture, `{nulla:…}` fits no tier (ratio, exemplum,
quaestio). `> Sn` = the item(s) it is evidence for; `> Rn` = the
project-wide regula it bears on.*

## I. The seed: extension without extension

- [ ] S1 A comment-based type and annotation layer for C89: the artifact stays pure C89, compilable by anything since 1990, and the discipline is a separate tool run when wanted (L28-31)  {VISIO-OPERIS > R2}
- [ ] S2 Prior art and where each died: Splint (its own C frontend, frozen pre-C99), Sparse (annotations as macros, no comment story), ACSL/Frama-C (broken inside macros), SAL (vendor-locked), cog (no tree), Closure/checkJs (proved comment typing works, JavaScript only) (L35-42)  {nulla:exemplum > S1}
- [ ] S3 "Comments and semantics live on opposite sides of the preprocessor, and every tool had to pick a side and pay" (L44-48)  {PRINCIPIUM}

## II. What silva already dissolves

- [ ] S4 The second-parser problem dissolves: silva IS the parser, so a checker on top implements zero C semantics; it is a query client (L55-63)  {DECRETUM}
- [ ] S5 The two-worlds problem dissolves: origo provenance chains hold pre- and post-expansion in one structure, so an annotation on a macro definition flows to every expansion site (L65-70)  {DECRETUM}
- [ ] S6 The attachment problem dissolves: one central table decides which declaration a comment governs (L72-80)  {DECRETUM}
- [ ] S7 All conditional arms are retained, so code no compiler ever compiled is checked (uuid.c's Linux arm, caught from a Mac) (L82-86)  {DECRETUM}
- [ ] S8 Validation by corpus sweep: classified diagnostics, every residual owned by a named class (L88-91)  {REGULA}
- [ ] S9 Annotations can flow backward into parsing: a `/*: typus */` hint can teach the resolver and collapse ambiguous nodes (L93-98)  {VISIO-OPERIS}
- [ ] S10 The house annotation vocabulary, in order of leverage (L100-123)  {VISIO-OPERIS}
  - [ ] S10.1 Latina landmines: "identifier shadows a latina.h define" as a pure query; lint rule #1, needing no annotations at all (L103-106)  {VISIO-OPERIS}
  - [ ] S10.2 standard-purity tags per node (C89, C99, extension): chorda.c's C99 `snprintf` was caught only by building on Linux (L107-113)  {VISIO-OPERIS > R2}
  - [ ] S10.3 piscina lifetime tags: arenas collapse Rust's general lifetime problem to "does tag A outlive tag B" (L114-118)  {VISIO-OPERIS}
  - [ ] S10.4 branded typedefs: nominal when annotated, at zero runtime cost (L119-120)  {VISIO-OPERIS}
  - [ ] S10.5 house-convention checks: i32 unsigned at signedness-sensitive sites, chorda non-termination at every `char*` boundary, length-coupled pairs (L121-123)  {VISIO-OPERIS}

## III. The three lanes

- [ ] S11 One input, three consumer kinds told apart by their verification contracts: checking (query → verdict), code generation (query → new trees), refactoring (query → mutation of code the tool does not own) (L127-152)  {DECRETUM}
- [ ] S12 Code generation here is post-typing: Rust's proc macros run before name resolution, while a silva derive reads field types with their offsets (L138-142)  {OCCASIO}
  - [ ] S12.1 X-macros, Qt's moc and Unreal's UHT: second parsers built to read annotations, "the Splint disease in codegen clothing" (L142-145)  {nulla:exemplum > S4}
- [ ] S13 The cog pattern with byte-exact roundtrip: the generator owns the bytes between its markers, regeneration is idempotent and diffable, and origo traces generated code back to the annotation that produced it (L145-150)  {VISIO-OPERIS}
- [ ] S14 Coccinelle proved pattern→template transformation over C at kernel scale, limited exactly where silva is strong (L153-157)  {nulla:exemplum > S16}
- [ ] S15 Machinula is a behavioural oracle built for another reason: the interpreter harness that matches native stdout byte for byte (L160-165)  {OCCASIO}
- [ ] S16 Refactorings are executed against the corpus before a human sees them: transform → re-type → interpret affected suites → compare stdout with the goldens (L161-167)  {VISIO-OPERIS}

## III-b. Regeneratable code with retained edits

- [ ] S17 Generated code can stop being read-only: mutable and immutable sections, merged to match a changed source of truth while keeping the user's edits (Fran) (L171-175)  {VISIO-OPERIS}
- [ ] S18 Generated code is read-only "not on principle but because regeneration is state-based clobbering and the only available merge is textual and unverifiable" (L179-181)  {PRINCIPIUM}
- [ ] S19 Prior art is "a tour of surrenders": spatial partition (partial classes, cog regions), ownership tags (EMF's `@generated NOT`, which orphans a whole method), round-trip engineering (the 90s CASE collapse); Terraform's drift model offers the missing exit, importing the hand-edit back (L182-194)  {nulla:exemplum > S20}
- [ ] S20 The user's customization IS a stored transformation pipeline: regeneration is a rebase, and conflicts arrive at semantic granularity with a sentence attached (L205-215)  {DECRETUM}
  - [ ] S20.1 the theory is bidirectional transformations; edit lenses propagate edit sequences, not states (L196-200)  {nulla:exemplum > S20}
- [ ] S21 Ownership becomes node-level provenance (sealed, managed, open); "DO NOT EDIT THIS FILE" dies as a concept (L217-225)  {DECRETUM}
- [ ] S22 A customization has three homes: float as a rebasing patch, become a standing generation stage, or move into the source of truth (L227-235)  {DECRETUM}
- [ ] S23 The read-only doctrine's real root is fear; a merge that must re-type and pass the goldens arrives with a certificate, so the policy loses its reason (L237-242)  {PRINCIPIUM+intervention, split}
- [ ] S24 House precedent: LAPIS, machine translation layered with human revision that survives regeneration; the generated-code lifecycle is its data model (L244-249)  {nulla:exemplum > S20}
- [ ] S25 Should the customization pipeline be keyed to the generator's output shape or to the source-of-truth elements? (L256-261)  {nulla:quaestio}

## IV. Plugins, the fact store, node identity

- [ ] S26 Babel is the best-documented case of a tree substrate growing an application ecology, failure modes included (L265-273)  {nulla:exemplum > S27}
- [ ] S27 Babel's mistakes, corrected: an explicit ordered plugin manifest, sealed node kinds, emission that leaves untouched bytes identical, and "output must re-type" as a gate (L275-283)  {DECRETUM}
- [ ] S28 Under the auteur model the plugin interface is "one sealed contract with yourself": it gets the ceremony and ships complete in 1.0; plugins proliferate outside the sealed core (L285-290)  {REGULA > R1}
- [ ] S29 The dogfood test: if the four named house plugins express in the plugin interface rather than beside it, the interface is real (L290-294)  {DECRETUM}
- [ ] S30 A fact store (proposed name acta): "the tree is truth about now; the database is truth about time, and truth ABOUT the code that isn't IN the code" (L296-299)  {VISIO-OPERIS}
  - [ ] S30.1 three fact kinds: derived (call graphs, stack depths), historical (instruction counts, timings), extrinsic (review states, trust tags, classifications) (L301-310)  {DECRETUM}
  - [ ] S30.2 extrinsic facts have no legitimate home in source text: a NOLINT comment per diagnostic is pollution, a row keyed to a node is clean (L308-310)  {nulla:ratio > S30}
  - [ ] S30.3 a blackboard: plugins produce and consume facts, coupled only to fact schemas; event-sourced and append-only (L315-319)  {DECRETUM}
  - [ ] S30.4 prior art: CodeQL, Meta's Glean, Google's Kythe, cscope (L312-313)  {nulla:exemplum > S30}
- [ ] S31 Node identity is THE ceremony decision: diff, merge, blame, literate anchors and fact keys all key off it (L321-323)  {PRINCIPIUM}
- [ ] S32 External code databases cannot change the code they observe, so their identity after edits is heuristic; here the refactoring lane is first-party and migrates identity in the same transaction as the edit (L323-328)  {OCCASIO}

## V. The semantic diff ladder

- [ ] S33 "A diff is an attempted reconstruction of intent from two snapshots that don't contain it" (L335-337)  {PRINCIPIUM}
- [ ] S34 Prior art: GumTree, difftastic, SemanticMerge (L337-340)  {nulla:exemplum > S35}
- [ ] S35 Four rungs of diff, the top two unavailable to anyone else (L340-341)  {VISIO-OPERIS}
  - [ ] S35.1 structural: content changes and style changes categorically distinct (L343-345)  {VISIO-OPERIS}
  - [ ] S35.2 typed: diff meaning, which changes when text doesn't; a typedef edit shows every expression whose conversions changed, in files with no textual edit (L346-352)  {VISIO-OPERIS}
  - [ ] S35.3 behavioural: run both versions' suites; a change carries a certificate, "goldens identical, instructions −3%" (L353-357)  {VISIO-OPERIS}
  - [ ] S35.4 recorded: edits made by first-party tools store the operation, so "diff stops being archaeology" (L358-362)  {VISIO-OPERIS}

## VI. Itinerarium: diff as a synthesized program

- [ ] S36 A solver that takes ASTs A and B and produces a minimum set of query + transform operations; that pipeline is the diff (Fran) (L370-372)  {VISIO-OPERIS}
- [ ] S37 A diff is a program and the ideal diff the shortest (conditional Kolmogorov complexity); "the operation alphabet defines the geometry of similarity" (L374-384)  {PRINCIPIUM}
- [ ] S38 Prior art: programming by example (FlashFill, PROSE, Refazer, Getafix's anti-unification, CEGIS), e-graphs, group theory (Thistlethwaite's staging), genome rearrangement, retrosynthesis, superoptimization (L386-421)  {nulla:exemplum > S36}
- [ ] S39 Tree edit distance with moves is NP-hard, but every standard mitigation maps onto a shipped asset: identity and recorded operations, executable queries, typing, behavioural verification, determinism (L423-440)  {OCCASIO}
- [ ] S40 Queries must be intensional, not extensional: a query enumerating forty node ids "is an edit list in a costume" (L444-447)  {DECRETUM}
- [ ] S41 Natural-and-short beats shortest: an anytime compressor that starts from the raw edit script and replaces clusters while description length drops (L449-455)  {DECRETUM}
- [ ] S42 The residual is the review surface: what wouldn't compress is the novel content, where human attention belongs (L459-464)  {VISIO-OPERIS}
- [ ] S43 Diff and codemod converge: a pipeline induced from one edit applies corpus-wide, with behavioural verification (L465-468)  {VISIO-OPERIS}
- [ ] S44 Merge becomes program composition: conflicts are "both branches renamed the same symbol differently", not line collisions (L469-472)  {VISIO-OPERIS}
- [ ] S45 A transformation re-induced across commits "is a convention trying to become a lint rule": the three lanes close into a loop (L473-475)  {VISIO-OPERIS}
- [ ] S46 The LLM slot: a superb proposer, a terrible verifier; the architecture wants a proposer whose every suggestion is checked by execution (L476-480)  {PRINCIPIUM+intervention, split}

## VII. Annales: a first-party VCS

- [ ] S47 A first-party version control system (proposed name annales) (L486-494)  {VISIO-OPERIS}
  - [ ] S47.1 precedents: drh's Fossil (SQLite's VCS, one file, anti-rebase), Unison (definitions addressed by AST hash; renames aren't diffs), Smalltalk ENVY (L487-494)  {nulla:exemplum > S47}
- [ ] S48 The unit of versioning is the symbol, not the file; files are projections (L498-502)  {DECRETUM}
- [ ] S49 Blame at node granularity, unpolluted by formatting (L507-509)  {VISIO-OPERIS}
- [ ] S50 Commits carry their semantics: typed-diff summary, diagnostics delta, behavioural certificate, and provenance (which session, which model, reviewed by whom) (L510-513)  {VISIO-OPERIS}
- [ ] S51 Textual merge's most dangerous failure is silent success: A renames f, B adds a caller of the old f, diff3 merges cleanly (L514-517)  {PRINCIPIUM+intervention, split}
- [ ] S52 Every merge exits through the gate: re-type and goldens before it is a merge (L518-519)  {DECRETUM}
- [ ] S53 Git coexistence by projection: git export is one more serialization of the store (L520-522)  {DECRETUM}

## VIII. Glossa: literate programming as projection

- [ ] S54 Knuth's fatal flaw was that tangle is one-way: the tangled output becomes a second source of truth every tool sees (L533-537)  {PRINCIPIUM}
  - [ ] S54.1 Jupyter, the modern cautionary tale: undiffable document-order JSON with outputs embedded (L537-539)  {nulla:exemplum > S55}
- [ ] S55 The document is a bidirectional projection over the code database, not a source that generates it: tangle doesn't exist (L541-549)  {VISIO-OPERIS}
- [ ] S56 Documents that cannot rot: every example is real nodes, every shown output interpreted at weave time; documentation drift becomes a build failure (L551-555)  {VISIO-OPERIS}
- [ ] S57 The data model is the catena: the medieval gloss page is the correct literate-programming format, and the Glossa Ordinaria had by layout what seventy years of tangle scripts failed to reinvent (L557-563)  {nulla:exemplum > S55}

## IX. Humans and LLMs on generated codebases

- [ ] S58 What rhubarb solves by discipline (CLAUDE.md protocols, phase-log liturgy) should become structure (L570-572)  {REGULA}
- [ ] S59 "A checker rule is compressed, permanent, non-decaying context": a convention in CLAUDE.md costs tokens every session and decays across compaction; as the checker grows, the system prompt shrinks (L574-580)  {PRINCIPIUM+intervention, split}
- [ ] S60 Review-surface compression: human review becomes the annotation diff, the diagnostics delta and the classified residuum, not line review of raw text (L581-586)  {VISIO-OPERIS}
- [ ] S61 Trust-gradient provenance: nodes carry session, model and reviewer as facts, so "all unreviewed generated code reachable from the boot path" is a query (L587-592)  {VISIO-OPERIS}
- [ ] S62 Learning the codebase: queries as tour guide, a literate projection with real node citations; these tools serve "Fran understands all the code", never replace it (L593-597)  {VISIO-OPERIS}
- [ ] S63 LLMs are strong at intent and synthesis, reliably weak at exhaustive mechanical precision: layouts, all the callers, expansions, conversion semantics (L599-601)  {PRINCIPIUM}
- [ ] S64 The MCP surface: every tool replaces a probabilistic text-read with ground truth (quaestio, typus, nexus, origo, forma, emitte, transforma, verifica) (L601-608)  {VISIO-OPERIS}
  - [ ] S64.1 `interpretare`: run the function in machinula, so the model stops reasoning about what code does and measures it (L609-613)  {VISIO-OPERIS}
  - [ ] S64.2 serve typed subtrees with exactly the needed symbols instead of pasting files (L614-616)  {VISIO-OPERIS}
  - [ ] S64.3 a reverse debugger as tool calls: "run to failure, step backward, inspect" (L617-621)  {VISIO-OPERIS}
- [ ] S65 "The fact store is the memory that survives compaction … the protocol at the top of silva/CLAUDE.md is a liturgy against context death; acta is the reliquary" (L622-626)  {VISIO-OPERIS}

## X. Domain moonshots

- [ ] S66 Aquinas, the OS: freestanding purity and interrupt discipline as queries; static worst-case stack depth per entry point; the linker script generated from annotations; machinula as the OS development loop, with time-travel debugging (L630-643)  {VISIO-OPERIS}
- [ ] S67 Solarium, the engine: officina makes C89 the engine's scripting language; interpreted and native code share structs, with no binding layer (L645-657)  {VISIO-OPERIS > R2}
- [ ] S68 A 3D IDE as the surface for what LSP cannot express: expansion layers in place, foreign-platform views, refactorings previewed with their verdict (L657-665)  {VISIO-OPERIS}
- [ ] S69 "Three facets of one editor-database-machine that has been assembling itself" (aquinas, solarium, silva/officina); Acme's plugins-as-programs, with a schema (L665-669)  {VISIO-OPERIS}

## XI. Grounding: the git-history experiment

- [ ] S70 The operation vocabulary should be discovered from the corpus, not imagined (L674-676)  {REGULA}
- [ ] S71 A 145-file link change compresses about 400:1, and its HTML churn was "the downstream shadow of a generator change": "in generated-artifact repos, most of the diff is provenance waiting to be recorded" (L678-692)  {INDICIUM > S35.4}
- [ ] S72 A schema change: its mechanical ~40% compresses to two operations, and the residual is where the idea of the commit lives (L694-705)  {INDICIUM > S42}
- [ ] S73 A retype across signatures is ~80% mechanical, 20% novel, and needs a typed query Coccinelle cannot express (L707-719)  {INDICIUM > S36}
- [ ] S74 Six or so operation schemas cover the mechanical majority of sampled history: pattern-wrap, add-field, retype-parameter, rename-symbol, delegate, regenerate (L725-731)  {INDICIUM > S36}

## XII. What existed then

- [ ] S75 The assets at the time: byte-exact roundtrip, 100% expression typing, symbol index and nexus, layouts, origo, comment attachment, the query surface, machinula matching native output 50/50 (L735-749)  {OCCASIO}

## XIII. Named questions and the path in

- [ ] S76 Ceremony decisions owed: node and symbol identity, the fact schema, the operation vocabulary, the annotation grammar (L753-765)  {nulla:quaestio}
- [ ] S77 Open tensions: intensional-query honesty, free-text reconciliation quality, document-order merge, git-projection fidelity (L767-772)  {nulla:quaestio}
- [ ] S78 Cheapest first pulls: the latina-landmine lint, the MCP read tier, `interpretare`, standard-purity tags; then the ceremony decisions, identity first (L774-785)  {DECRETUM}
- [ ] S79 "All three predecessors of silva died from data-model decisions that foreclosed features" (L790-791)  {PRINCIPIUM}
- [ ] S80 The unification: the repository is the fact store, a diff is a query over it, the document and the file are both projections, history is the event log, and every claim is verifiable (L787-790)  {VISIO-OPERIS}

## XIV. Delivery: push beats pull

- [ ] S81 "Every fresh agent session is a new user": declarative knowledge survives compaction, procedural intuition does not (L800-803)  {PRINCIPIUM}
- [ ] S82 Tool adoption is a marketing funnel, and rules-you-must-remember are friction that loses the conversion (Fran) (L803-804)  {PRINCIPIUM}
- [ ] S83 Hooks are the push channel: a post-edit check injects context only when something is wrong; generated files guard themselves by a marker (L807-813)  {DECRETUM}
- [ ] S84 "A push channel that speaks when nothing is wrong trains banner blindness" (L813-815)  {PRINCIPIUM}
- [ ] S85 Conversion happens on the failure path: every diagnostic carries its own next command, and docs are indexed by symptom, not tool name (L816-819)  {REGULA}
- [ ] S86 Triggers must be events (session start, gate, error, edit), never mental states ("remember when relevant") (L820-824)  {REGULA}
- [ ] S87 A silva-based LSP server would unify four deferrals: the persistent daemon, the checker's delivery, incrementality, and the editor surface (L826-835)  {VISIO-OPERIS}

## XV. Since then (my commentary, not in the source)

- Much of this has been built since July: the Latin identifier lint (S10.1) now blocks commits; the silva LSP (S87) replaced clangd; the legati MCP server is the read tier of S64; post-edit hooks (S83) are live; generated files carry a `GENERATUM` marker (S83).
- Lineage with other sources: S31 (node identity is the ceremony decision) is tabularium's T11; S3 (comments and semantics on opposite sides of the preprocessor) sits beside T24.1 (the preprocessor is why nobody has done Unison-for-C); S57's Glossa Ordinaria is the text stack's X68 page-layout problem, met from the other side.
- S59, S81, S82, S84 and S86 are the "push, don't pull" condition we arrived at again on 2026-10-02 for documentation: this document had already found it for tools in July.
