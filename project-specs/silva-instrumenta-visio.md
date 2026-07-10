# Silva instrumenta — the tool ecology above the parse

*2026-07-10. Vision/brainstorm document distilled from a long working
conversation (Fran + Claude), following the arc: comment-based extension of
C89 → the three tool lanes → the fact store → semantic diff → diff as a
synthesized program → first-party VCS → literate programming → humans and
LLMs on generated codebases. Per house culture ("raw transcripts precede
specs"), the source conversation should be preserved alongside this
distillation; this document carries the vision and its grounding, not the
verbatim Q&A. Nothing here is DECISUS — proposed names are marked as
proposals; naming is ceremony and the seals are Fran's.*

*Grounding performed against the repo as of commit 5ee78fc (M2c near-seal):
silva M0 closed (100.00% expression typing over 1,503,915 nodes / 732
plagulae; auspex 3,753/0), officina M2c at 55/73 suites fully interpreted
with 50/50 byte-identical stdout, quaestio selector surface DECISUS
2026-07-06, and a git-history compression experiment on three real commits
(§XI). Also verified this session: the officina suite passes on Linux/gcc —
a foreign platform and compiler — after three mechanical build changes,
with the only genuine catches being a C99 `snprintf` in chorda.c and an
unannotated case fallthrough in the amalgam (both themselves arguments for
the lint lane below).*

---

## I. The seed: extension without extension

The original question: what would a comment-based type/annotation layer for
C89 look like — TypeScript/Flow-comment-mode or Rust-direction checking,
where the artifact remains pure C89, compilable by anything since 1990, and
the discipline is a separate tool run when wanted?

**Prior art, and where each one died or stalled:**

| Tool | Mechanism | Contribution | Structural failure |
|---|---|---|---|
| **Splint** (LCLint, 90s) | `/*@null@*/ /*@only@*/` comments | Ownership/null/alias checking a decade before Rust | Maintained its own entire C frontend; frontend froze pre-C99; annotations died with the parser they were welded to |
| **Sparse** (Linus, 2003) | Empty macros `__user __kernel __bitwise` | Address-space & branded-integer checking; still guards the kernel | Annotations occupy identifier space, gate behind `-D__CHECKER__`; **no comment story at all** — nothing richer than a qualifier-shaped macro is expressible |
| **ACSL / Frama-C** | `/*@ requires ... */` contracts | Deepest semantics: deductive proof | Inherited a post-preprocessor frontend with no expansion provenance; annotations inside macros notoriously broken |
| **Microsoft SAL** | `_In_ _Out_` macros | Shipped at industrial scale | Vendor-locked, macro-namespace pollution |
| **cog** (Batchelder) | Generator code in comments, output written between markers *in the source* | The in-source generated-region pattern | No tree, no types — pure text |
| **Closure Compiler / TS checkJs** | JSDoc comments | Proved zero-build-step type checking works at ecosystem scale | JavaScript-only; the C analog never got a substrate |

The pattern across all of it: **comments and semantics live on opposite
sides of the preprocessor**, and every tool had to pick a side and pay.
Splint picked the comment side and paid with a doomed second parser. Sparse
picked the semantic side and paid with the loss of comments entirely.
Frama-C tried to straddle and broke exactly at macro expansion.

## II. What silva already dissolves

Each historic failure mode maps onto a silva pin or shipped capability that
exists for independent reasons:

1. **The second-parser problem → silva IS the parser.** Tree-sitter
   robustness bar, byte-exact roundtrip as oracle, 100% expression typing
   clang-certified. A checker/generator/refactorer atop silva implements
   zero C semantics; it is a query client. Splint spent its whole budget
   rebuilding what silva.h now exports as rows:
   `silva_c89_typus_expressionis` / `conversio_expressionis` (the frozen
   {naturalis, conversus} pair), the symbol index + use-site nexus
   (`silva_c89_symbolum_nodi`, M1a), `formam_computare`,
   `constans_aestimare`, `silva_c89_chorda_decodere`.

2. **The two-worlds problem → origo.** Provenance chains hold pre- and
   post-expansion in one structure. An annotation on a macro *definition*
   flows to every expansion site; a diagnostic at an expansion site points
   back to the comment that governs it. This is precisely the capability
   whose absence cripples ACSL-in-macros — and silva has it as a product
   pin, not checker infrastructure.

3. **The attachment problem → the attachment table.** Which declaration
   does a comment govern? Every prior tool solved it ad hoc. Silva solved
   it once, centrally (arbor2-comment-spec.md: end-of-line →
   `commenta_post` on preceding node; tight-above → `commenta_ante` on
   following; blank-isolated → floating sibling). The same rules that make
   comments queryable make annotations bindable. And the comment node
   struct already reserves the seat: `Xar* fragmenta; /* Parsed
   sub-structure (NIHIL initially) */` — an annotation mini-grammar is a
   .stml file riding the existing generated-tables machinery.

4. **The configuration problem → all arms retained.** Splint and Sparse
   check only the preprocessed arm you run under; annotations in the #else
   are dead text. Silva checks arms no compiler ever compiled — existence
   proof already banked: typing found the illegal `(insignatus i8)` in
   uuid.c's Linux arm from a Mac that had never compiled it.

5. **The validation problem → the percursus methodology.** Corpus sweep,
   classified diagnostics, every residual owned by a named class. A new
   annotation pass gets corpus-scale validation for free; M0b is the
   template.

6. **Annotations can flow BACKWARD into parsing.** The oracle API
   (`silva_oraculum_typum_addere`, situ-scoped, in-place
   re-canonicalization) means a `/*: typus */` hint can teach the resolver
   and collapse AMBIGUUS nodes. Every prior tool was strictly downstream of
   parsing; this substrate permits the comment layer to be a parsing
   participant — which is also speculum's entry point wearing another hat.

**The rhubarb-native annotation vocabulary** (what to actually check, in
leverage order):

- **Latina landmines** — ten firings and counting (`nomen` as a parameter,
  `exire` silently renaming a struct field...). "Identifier shadows a
  latina.h define" is a pure query over the shipped symbol table. Lint rule
  #1; needs no annotations at all; would have paid for itself already.
- **Standard-purity tags** — per-node C89/C99/extension marking (the token
  field exists, unpopulated; :c89/:c99 pseudos already named in the
  quaestio DECISUS). Existence proof from this session: chorda.c:1684 calls
  `snprintf` (C99), invisible on macOS because Apple's headers declare it
  regardless; caught only by building on Linux. A tag makes that a query
  instead of a foreign-platform accident. Dependable C rules ride the same
  mechanism (named in silva-spec).
- **Piscina lifetime tags** — `/*: piscina(scaena) */`. The arena killer
  app: arenas collapse Rust's general lifetime problem to "does tag A
  outlive tag B" — a declared partial order plus an escape check (pointer
  tagged with shorter-lived arena stored into structure tagged with
  longer-lived one = diagnostic).
- **Branded typedefs** — nominal-when-annotated typedefs (GradusId vs
  PaginaId; phys vs virt addresses in aquinas), zero runtime cost.
- **House-convention checks** — i32-is-UNSIGNED at signedness-sensitive
  sites; chorda non-termination discipline at every `char*` boundary;
  length-coupled pointer/count pairs `/*: mensura(n) */`.

## III. The three lanes

One input (the typed, full-fidelity tree + annotations), three consumer
kinds, distinguished by their **verification contracts**:

1. **Checking / static analysis** — query → verdict. Read-only; emits
   diagnostica into the existing classification machinery. The back half
   (classified diagnostics, percursus sweeps, per-file coverage) is
   already shipped; the enforcement layer named in the phase-log is
   checker-plugin #0.

2. **Code generation / metaprogramming** — query → new trees. Contract is
   constructive: output must re-type cleanly and land in owned regions.
   Silva's decisive advantage over the famous prior art: it is
   **post-typing**. Rust proc macros run on raw token streams before name
   resolution — a derive macro cannot ask the type of a field expression.
   A silva `derive(serialis)` reads field types *with offsets*
   (formam_computare). C's own prior art makes the contrast starker:
   X-macros (types smuggled through discipline), Qt's moc and Unreal's UHT
   (entire second parsers built to read annotations — the Splint disease
   in codegen clothing). The cog pattern composes beautifully with
   byte-exact roundtrip: generator owns the bytes between its markers, the
   oracle guarantees everything else untouched, regeneration is idempotent
   and diffable — and origo gives generated code provenance chains back to
   the annotation that produced it ("show me everything this annotation
   generated" — source maps for metaprogramming, renderable in solarium).

3. **Automated refactoring** — query → mutation of code the tool does NOT
   own. The strongest contract: behavior preservation. Prior art:
   **Coccinelle**, the semantic-patch tool performing the Linux kernel's
   collateral evolutions — proof that pattern→template transformation over
   C works at scale, limited exactly where silva is strong (unpreprocessed
   source, heuristic typing). Silva's transform DSL
   (`<%adde>/<%praepone>/<%substitue>`, DECISUS-named) fires on trees where
   every expression carries {naturalis, conversus} and every use-site has
   its nexus edge. And the decisive asset built for another reason:
   **machinula is a behavioral oracle.** The refactoring loop: transform →
   re-type (static gate) → interpret affected suites → compare stdout
   byte-for-byte against captured goldens. The M2c captare/cursor harness
   (50/50 suites byte-identical vs native) IS a refactoring verification
   harness that happens to be labeled as an interpreter test. Nobody has
   this for C: Coccinelle patches are human-reviewed; these are *executed
   against the corpus* before a human sees them.

## III-b. Regeneratable code with retained edits (the fourth surrender refused)

*(Appended from the same source conversation, post-distillation: Fran's
observation that AST-aware machinery could make generated code stop being
read-only — mutable and immutable sections, with merge/transformation to
match a changed source of truth while retaining post-generation user
edits.)*

The deliberate two-sources-of-truth case: source of truth and generated
output live at different abstraction levels — the SoT can't express the
hand-tuning; the output can't express the schema. Generated code is
read-only today not on principle but because regeneration is **state-based
clobbering** and the only available merge is textual and unverifiable.
Prior art is a tour of surrenders: **spatial partition** (C# partial
classes, protobuf insertion points, cog regions, Rails generate-once —
customization only at pre-blessed extension points); **ownership tags**
(Eclipse EMF's JMerge `@generated NOT` — AST-aware, twenty years old, but
binary and member-level: edit one line of a generated method and the whole
method is orphaned from future generator improvements); **round-trip
engineering** (the 90s CASE collapse — no stable node identity,
text-level merges, model and code as lossy-converted artifacts rather than
projections; the WinForms InitializeComponent re-parse as the miniature
cautionary tale). Plus one from operations: **Terraform's drift model** —
drift detected and classified rather than forbidden, with the second exit
nobody offers for code: *import the hand-edit back into the source of
truth*.

Theory name: **bidirectional transformations** — lenses (Foster/Pierce),
where user edits with no preimage in S are the **complement**; the variant
matching this machinery is **edit lenses** (Hofmann/Pierce/Wagner 2012):
propagate edit sequences, not states — the itinerarium formulation
exactly.

The synthesis is three pillars of this document composing, not a new
subsystem:

1. **The user's customization IS a stored itinerarium pipeline.** Store
   pristine-generated output plus the transformation from it (recorded ops
   via first-party tools; solver-induced for free-text). Regeneration is a
   **rebase**: `gen(S_new)`, apply the pipeline. Intensional queries make
   the customization survive generator-output changes that preserve the
   pattern; where a query fails to match, the conflict arrives at semantic
   granularity with a sentence attached ("the retry-parameter rename has
   no target; the schema no longer generates a retry parameter") instead
   of `<<<<<<< HEAD`. EMF's binary flag becomes arbitrary node-level
   deltas; the orphaning problem dissolves — generator improvements keep
   flowing *inside* customized things.

2. **Ownership becomes node-level provenance, not file-level convention.**
   Ownership classes per node: **sealed** (regenerated always, edits
   flagged), **managed** (regenerated, edits captured-as-pipeline and
   rebased), **open** (user's; generator never touches). "DO NOT EDIT THIS
   FILE" dies as a concept. Structurally this is the expansion-layer
   pattern pointed forward: SoT → generated → customized is the same
   stratification as macro layers, with gen and pipeline as the
   derivations instead of expansion. Generated code is another layer
   genus.

3. **A customization has three possible homes**, and drift management is
   choosing among them: float as a rebasing patch (below the SoT's
   abstraction — the hand-tuned inner loop); promote to a **standing
   generation stage** (a permanent transform appended to the generator's
   own pipeline — where a floating patch graduates once stable); or
   promote **into the source of truth** when expressible there (the
   Terraform import direction, the reverse lens). The solver makes the
   middle option cheap; the tool can offer the classification since it can
   check expressibility against the SoT schema.

The verification story is what actually kills the read-only doctrine: its
real root is fear — nobody trusts a merge of generated code. Here the
rebased result must re-type and pass the captured goldens in machinula
before it *is* the regeneration; the merge arrives with a certificate. The
scary question has a mechanical answer, so the policy that existed to
avoid asking it loses its reason.

House precedent in production, another domain: **LAPIS** — machine
generation (translation) stratified with human revision that survives
re-generation of the base layer, provenance-complete, in SQLite. The
generated-code lifecycle is LAPIS's data model with gen swapped for
translation and itinerarium pipelines for revision layers. Near-term
instances from this conversation's own examples: hand-tuned precision
qualifiers in MSL/GLSL generated from C structs; the hand-placed alignment
hack in aquinas's generated linker script; one field's serialization
hand-overridden in a derive(serialis) — each is managed nodes with a small
floating pipeline, each currently solved in the wild by never
regenerating.

**Ceremony-list addition**: whether the customization pipeline is keyed to
the *generator's output shape* or to the *source-of-truth elements* —
rebase robustness differs sharply (SoT-keyed survives generator refactors;
output-keyed survives SoT refactors; probably SoT-keyed with output-keyed
fallback). Another place node identity must be designed with two parents
in mind — feeds ceremony decision #1.

## IV. The substrate commons: plugins, the fact store, node identity

**Babel is the best-documented case study** of a tree substrate growing an
application ecology, including the failure modes. What its core owned so
plugins never did — traversal (visitors; silva's selectors are strictly
better: context-expressing, declarative), binding analysis (path.scope;
silva: symbol index + nexus, shipped), tree-consistent mutation (paths;
silva: the transform DSL seat), and fragment construction (babel-template:
write source, get AST — silva has this almost by accident as snippet
parsing with oracle injection, designed as speculum's entry point, doubling
as the plugin ergonomics story).

What Babel got wrong, and the house corrections: implicit plugin ordering
(fix: explicit ordered manifest, named phases — determinism by
construction, the mmap-level house value); AST spec drift breaking the
ecosystem twice (fix: genera are ceremony-sealed like
vocabularium-medullae); transforms destroying formatting, hence recast
(fix: trivia model makes silva recast-native — transforms touch what they
touch, emission of everything else is byte-identical); and deliberate
untypedness shipping unsound transforms (fix: "output must re-type" is a
gate no JS transform ever had).

The ecosystem tension dissolves under the auteur model: Babel needed semver
negotiation with thousands of strangers; this needs **one sealed contract
with yourself** — Eskil's dictum at the meta level. The plugin *interface*
is the expensive artifact, gets the ceremony, ships complete in 1.0;
plugins are cheap implementations proliferating forever after, outside the
sealed core, no version treadmill. Dogfood test: the four house plugins
already named in existing documents — enforcement layer (checker #0),
formator (transform #0), latina-landmine lint (proof piece), Dependable C
pass (external-vocabulary test). If all four express in the plugin
interface rather than beside it, the interface is real.

**The SQLite fact store** (proposed name: **acta** — the appended record,
as in acta diurna; "tabularium" is taken as an identifier). Ontology: *the
tree is truth about now; the database is truth about time, and truth ABOUT
the code that isn't IN the code.* Three fact kinds:

- **Derived** — call graphs, reachability closures, static stack depths;
  too expensive to recompute per parse, incrementally maintained.
- **Historical** — instruction counts, parse times, peak arena bytes, suite
  tabulae. The phase-log's metric tables (55|0|10|2|1|0|5...) are this
  store maintained in markdown by discipline; CULTURA already mandates
  "the cost curve is visible history."
- **Extrinsic** — review states, trust tags, diagnostic classifications,
  explanations. These have no legitimate home in source text (a NOLINT
  comment per classified diagnostic is pollution; a row keyed to a node is
  clean).

Prior art tier: CodeQL, Meta's Glean, Google's Kythe (code-as-database at
scale), ancestor cscope. House pattern: this is **LAPIS applied to code** —
SQLite as provenance-complete application format. Architecture:
**blackboard** — plugins are producers/consumers of facts, coupled only to
fact schemas, never to each other. Write model: **event-sourced,
append-only** — the aquinas inspiration and the phase-log's iron rule
("sections are APPENDED, never inserted") as storage schema. Fact history
free: not "this function's instruction count" but its curve.

**Node identity is THE ceremony decision.** Everything downstream — diff,
merge, blame, literate anchors, fact keys — keys off it. The hard problem
that defeats external code-databases (Kythe/Glean observe code they cannot
change; identity reconciliation after edits is heuristic and lossy)
dissolves here because the refactoring lane is *first-party*: the tool
performing the rename migrates the identity mapping **in the same
transaction as the edit**. Sketch: DB-assigned stable symbol IDs; nodes
addressed symbol-relatively with content hashes for drift detection;
transform-DSL mutations carry identity deltas; free-text edits reconciled
by matcher (§VI) as the fallback. Deserves the vocabularium-medullae
treatment: interview, simulation, seal.

## V. The semantic diff ladder

A diff is an attempted reconstruction of intent from two snapshots that
don't contain it; every textual-diff failure mode is the reconstruction
guessing wrong. Tree-diff prior art guesses better: **GumTree** (the
standard algorithm — anchor matching, move as first-class edit),
difftastic/diffsitter (practical tree-sitter structural diffs),
SemanticMerge (proved the market). Silva enables four rungs, the top two
unavailable to anyone else:

1. **Structural** — difftastic-class, but on full-fidelity trees with the
   trivia split: content changes vs style changes are *categorically*
   distinct, not heuristically filtered.
2. **Typed** — diff *meaning*, which changes when text doesn't. Change a
   typedef int→long in a header: the typed diff shows every expression
   project-wide whose conversion behavior changed (addere.s32 →
   extendere.s32.s64 + addere.s64) in files with zero textual edits.
   Textual diff is structurally incapable of showing a header edit's blast
   radius. Same for macro edits diffed at the expansion layer; same for
   diffs evaluated under ALL conditional arms.
3. **Behavioral** — the captare harness as comparator: run both versions'
   affected suites in machinula, compare stdout byte-for-byte and
   instruction counts. A change carries a *certificate*: "goldens
   identical, instructions −3%" — refactor vs functional change,
   mechanically established.
4. **Recorded** — when first-party tools perform the edit, the diff is
   recorded, not recovered. A rename is stored as *the operation rename*
   with its identity delta, not forty line-edits to reverse-engineer.
   Inference (GumTree-class) becomes the fallback for free-text edits
   only. Diff stops being archaeology.

## VI. Itinerarium — diff as a synthesized program

*(Proposed name: the pipeline IS an itinerary from tree A to tree B; the
medieval genre — Itinerarium Egeriae, Bonaventure's Itinerarium — fits the
house. Ceremony pending.)*

**The idea (Fran's formulation):** a solver that takes AST graphs A and B
and produces a minimum set of quaestio+transform operations — a pipeline —
that maps A to B, and *that pipeline functions as the diff.*

**The deep framing:** a diff is a program; the ideal diff is the shortest
one. The minimal program transforming A into B is conditional Kolmogorov
complexity K(B|A) — uncomputable in general, so every practical field
restricts the description language to a domain DSL and searches within it.
The restriction is where the meaning lives: **the operation alphabet
defines the geometry of similarity.** Under line-edits a rename is forty
scattered changes; under {query, transform} it is one operation. Byte-level
diff is already literally a program — VCDIFF (RFC 3284) is a bytecode of
COPY/ADD/RUN instructions; git packfiles are delta chains — but nobody
treats `git diff` as a compiler target because its instruction set is too
dumb to mean anything. The proposal raises the ISA.

**Prior art map** (the answer to "where do people solve graph-A-to-graph-B
paths?"):

- **Programming-by-example synthesis** — the closest and it works:
  FlashFill (Gulwani 2011, shipped in Excel; version-space algebras),
  Microsoft PROSE, and above all **Refazer** (ICSE 2017): synthesizes
  tree-transformation programs from before/after AST pairs of code edits,
  in a rewrite DSL, to replay edits elsewhere. Meta's **Getafix** (2019)
  mines thousands of human bug-fixes, clusters tree-diffs, extracts
  generalized patterns via **anti-unification** (Plotkin 1970, least
  general generalization) — the algorithmic heart: given correspondences,
  cluster concrete edits, anti-unify each cluster into a candidate query
  pattern, synthesize the template, verify by execution. The verify loop
  is **CEGIS** (Solar-Lezama): propose from a subset of evidence; every
  disagreement between pipeline(A) and B is a counterexample refining the
  next proposal.
- **Term rewriting / equality saturation** — e-graphs (egg, POPL 2021)
  represent exponentially many rewrite-equivalent terms compactly;
  saturate from both ends, meet in the middle.
- **Group theory** — the transform ops generate a monoid acting on
  AST-space; the diff is the **word problem**: shortest word in the
  generators. God's number for the Rubik's cube (20, proved 2010) is this;
  Thistlethwaite's staged solution through a subgroup chain maps directly
  onto solver strategy: solve the rename/symbol layer first, then
  structural moves, then local expression edits — each stage shrinking the
  residual for the next.
- **Genome rearrangement** — Hannenhalli–Pevzner: minimum
  reversals/translocations between genomes; deliberately separates
  structural rearrangement from point mutations; inferring the operation
  sequence is inferring *history* — which is what diff is.
- **Chemical retrosynthesis** — target molecule, reaction library, minimal
  route; cracked by neural-guided tree search (MCTS + learned proposers,
  Segler 2018). The LLM slot in one lesson.
- **Superoptimization** — Massalin 1987, STOKE: shortest program with given
  behavior; proof that stochastic search + fast verifier goes further than
  expected.

**Honest hardness:** tree edit distance with *move* is already NP-hard;
synthesis is worse. Every standard mitigation maps onto a shipped asset:

- Correspondences (the search-space decomposer): prior tools infer them
  heuristically — GumTree's whole contribution. Silva has stable identity,
  the nexus, and *recorded operations* wherever edits came through
  first-party tools — in which case there is nothing to solve. The solver
  is the reconciliation path for free-text edits; recorded ops are ground
  truth everywhere else. No synthesis system ever had that luxury.
- Cheap falsification: quaestio queries are *executable* — run a candidate
  selector against A, instantly check match-set shape. Typing kills
  candidates whose output doesn't re-type (Refazer, syntactic, never had
  this).
- Final verification: machinula + goldens = behavioral verifier. A
  synthesized pipeline carries the certificate "A→B exact, goldens
  byte-identical" — plausible upgraded to proved.
- Determinism (the quiet precondition): a diff-program is only a diff if
  replay is exact.

**Two design pins learned from the literature:**

1. **Queries must be intensional, not extensional.** A "query" enumerating
   forty node IDs is an edit list in a costume; the compression is a lie.
   MDL (minimum description length) handles it if operations are costed
   honestly: a pattern only wins when it genuinely carries the information
   of its match set.
2. **Shortest is not quite the target; natural-and-short is.** Prefer
   named refactoring schemas over exotic-but-terse selectors. Graceful
   degeneracy: the node-level edit script is always available as fallback,
   so the architecture is an **anytime compressor** — start from the raw
   edit script, iteratively replace clusters with induced query+transform
   ops while description length drops. Diff as dictionary compression
   where the dictionary is semantic.

**The payoff stack:**

- **The residual is the review surface.** A→B = pipeline(A) + residual.
  The residual — what wouldn't compress — is precisely the novel content
  where human attention belongs; the pipeline part is machine-verified. A
  5,000-line change compressing to three ops + 40 residual lines is *a
  refactoring with a small idea in it*, and the diff now says so. MDL as
  intent detector.
- **Diff and codemod converge.** A pipeline induced from one A→B pair is
  by construction applicable elsewhere: make the edit by hand once, the
  solver generalizes, apply corpus-wide with behavioral verification
  (Refazer's premise + an oracle it never had).
- **Merge becomes program composition** — Darcs/Pijul commutation theory
  finally over patches that are semantic operations; conflicts become
  "both branches renamed the same symbol differently," not line
  collisions.
- **History mining feeds the checker lane**: a transformation the solver
  keeps re-inducing across commits is a convention trying to become a lint
  rule. The three lanes close into a loop.
- **The LLM slot** is the retrosynthesis lesson: a superb *proposer*
  ("this cluster looks like extract-function + signature change"), a
  terrible verifier — and this architecture wants exactly a proposer whose
  every suggestion is checked by execution. Neurosymbolic synthesis with
  silva as the symbolic half.

## VII. Annales — a first-party VCS

*(Proposed name: the append-only year-records; Tacitus. Ceremony pending.)*

The Linus precedent (build your own VCS for your own project) is right, but
the apter one is **drh and Fossil**: SQLite's VCS, by its author, for that
one project, the whole repository in a single SQLite file, explicitly
anti-rebase — history immutable, what happened happened. That is the LAPIS
pattern, the phase-log's append-only rule, and the honest-history instinct,
validated by twenty years of SQLite development. Two more ancestors:
**Unison** (content-addresses every definition by AST hash; names are
metadata; *renames aren't diffs at all*) and Smalltalk **ENVY** (versioned
methods, not files).

Design that assembles from parts already on the table:

- **The unit of versioning is the symbol, not the file.** A function's
  history is its own history as it migrates between files; files demote to
  *projections* — one grouping and ordering of definitions. (The aquinas
  inspiration list — "object database filesystems" — already gestures
  here.)
- **Storage: SQLite** (Fossil precedent, house pattern). **Write model:
  the event-sourced operation log** (aquinas's architecture; the
  phase-log discipline as storage schema). Content-addressed subtrees for
  dedup — git's blob store one level up.
- **Blame at node granularity, unpolluted by formatting** — the trivia
  split means running the formator never touches content history (git
  needs the .git-blame-ignore-revs bandaid forever).
- **Commits carry their semantics natively**: typed-diff summary,
  diagnostics delta, behavioral certificate, trust/provenance tags (which
  session, which model, reviewed by whom — the VCS concern git fakes with
  message conventions).
- **Merge as the showcase.** Textual merge's most dangerous failure is
  silent success: branch A renames f, branch B adds a caller of old-f,
  diff3 merges cleanly, the build breaks or worse. Operation-based merge
  replays A's recorded rename over B's new caller correctly, because the
  rename is an operation over symbol identity. Every merge exits through
  the machinula gate: re-type + goldens before it *is* a merge.
- **Git coexistence by projection**: git export is one more serialization
  of the store (the Fossil answer). The world speaks git; the store
  doesn't have to.

Honest residue, named so it stays a decision: free-text edits still need a
GumTree-class matcher (far better-informed here: typed anchors, nexus);
document-order sequence merge has mild CRDT flavor (mild — auteur + agents
means real concurrency is rare); the export path must stay first-class.

## VIII. Glossa — literate programming as projection

*(Proposed name: the gloss. Ceremony pending.)*

Knuth's WEB/CWEB was tangle + weave; the fatal flaw was never the idea but
that **tangle is one-way** — the tangled output becomes a second source of
truth that every tool sees (debugger shows tangled code; CWEB emits #line
directives as the apology; you can't edit where the tools live). Jupyter is
the modern cautionary tale and it ties back to diff: notebooks are
notoriously undiffable — document-order JSON with embedded outputs, the
worst VCS citizen.

Silva dissolves tangle structurally: with byte-exact roundtrip and stable
node identity, **the document is a bidirectional projection over the code
database, not a source that generates it.** Prose lives as attached
comments (attachment table + fragmenta — the "comments are content" pin,
flagged in silva/CLAUDE.md as the most easily smoothed-over; this pillar is
what it was protecting). Document order is a sequence of node references in
the fact store. Weave is a serialization; tangle doesn't exist because the
code never stopped being the code. Edit either projection, you've edited
the same tree; annales versions both consistently for free.

Machinula adds the feature no literate system ever had: **documents that
cannot rot.** Every example is real nodes; every shown output is
interpreted deterministically against the current version at weave time.
Documentation drift becomes a *build failure*. Instruction counts in the
margins if wanted.

Structural observation the house will appreciate: prose anchored to an
authoritative text, multiple commentators' glosses layered with provenance
— the data model is the **catena**. The medieval gloss page (text center,
commentary keyed to it in the margins) is the correct literate-programming
format; the industry spent seventy years of tangle scripts failing to
reinvent what the Glossa Ordinaria had by layout. The Catena Aurea packet
tooling and lapide.org are prior house art for exactly this data shape.

## IX. Humans and LLMs on generated codebases

The novel application category — the one that didn't exist when arbor v1
was written — and possibly the strongest retroactive argument for pins
adopted on principle (determinism, provenance-always, all-arms-retained).
Rhubarb itself is the lived evidence: the problems below are currently
solved by *discipline* (CLAUDE.md protocols, phase-log liturgy); the vision
is discipline becoming *structure*.

- **A checker rule is compressed, permanent, non-decaying context.** Every
  CLAUDE.md convention costs prompt tokens every session forever and
  decays across compaction anyway (the ten latina-landmine firings ARE
  convention-decay-under-generation bugs). The same convention as a plugin
  costs once and fires mechanically forever. As the checker grows, the
  system prompt shrinks. This inverts the economics of LLM-assisted
  development, where project knowledge is perishable and re-fed.
- **Review-surface compression is already the house methodology** — M0b
  closed not by reading 1.5M typings but by reading the classified
  residuum. At generation scale, human review = the annotation-layer diff,
  the diagnostics delta, the invariant dashboard, and (with itinerarium)
  the pipeline + residual. Structural review of a classified artifact, not
  line review of raw text.
- **Trust-gradient provenance.** Origo generalizes: nodes carry generation
  provenance (session, model, reviewer) as facts; trust becomes queryable
  ("all unreviewed generated code reachable from the boot path"; "what
  fraction of the frame loop has human eyes on it"). The
  reviewed/unreviewed boundary currently lives in memories and PR
  archaeology; nobody has this.
- **Learning the codebase**: queries as tour-guide substrate; the literate
  projection generated from the tree with real node citations; solarium
  renders the walk. The onboarding answer for codebases too large for any
  head — with the caveat that "Fran understands all the code" remains
  CULTURA: these tools serve that pin, never replace it.
- **The MCP surface** (silva/officina as tools for models). Design
  principle: LLMs are strong at intent and synthesis, reliably weak at
  exhaustive mechanical precision (layouts under alignment, ALL the
  callers, layer-3 expansions, C's conversion semantics). Every tool
  replaces a probabilistic text-read with ground truth:
  - `quaestio` (selector → nodes+types+locations), `typus` (the frozen
    pair at a position), `nexus` (find-references), `origo` (expansion
    story), `forma` (layouts — kills a whole LLM bug class alone),
    `aestima`, `emitte` (byte-exact subtree).
  - `transforma` (pattern→template → returns a DIFF for approval, never
    writes), `verifica` (diagnostics delta, classified).
  - **`interpretare`** — run the function in machinula with arguments;
    return result + instruction count. The model stops reasoning about
    what code does and *measures it*, deterministically; the instruction
    count is a free complexity signal ("this refactor tripled the count —
    intended?").
  - Snippet parsing with oracle injection answers context-window
    economics: don't paste files; serve typed subtrees with exactly the
    needed symbols, verified.
  - The v2 flagship completes it: **vindex over the debug format as MCP**
    — "run to failure, step backward, inspect" as tool calls. Debugging
    transformed from the worst LLM activity (multi-turn printf archaeology)
    into arguably the best (deterministic reverse navigation of a recorded
    execution).
- **The fact store is the memory that survives compaction** — shared
  ground between Fran, the models, and the tools; explanations written
  once persist; sessions start from facts, not re-orientation ritual. The
  protocol at the top of silva/CLAUDE.md is a liturgy against context
  death; acta is the reliquary.

## X. Domain moonshots

**Aquinas (OS).** Freestanding purity as a symbol-nexus query (currently a
late linker error). Interrupt-context discipline: tag ISRs, tag
sleepers/allocators, check reachability — Sparse's killer app generalized.
Physical-vs-virtual address branding (Linux fakes with __bitwise). Port-I/O
confinement to blessed drivers as a one-line selector. **Static worst-case
stack depth per entry point** (formam_computare + call graph) — on 4KB
kernel stacks not a nicety; no annotation tool has ever offered it because
none had layouts and call graphs in one tree. Codegen: **the linker script
generated from section annotations** — the two artifacts that always drift,
made one; syscall/interrupt tables from annotations. Moonshot: **machinula
as the OS dev loop** — allocator, filesystem, event-sourced page model are
pure logic; interpreted with snapshots, the QEMU reboot cycle becomes a
function call, and time-travel turns kernel debugging (printk archaeology)
into "run to the triple fault, step backward."

**Solarium (engine + 3D IDE).** Frame-arena escape checks with a 60Hz
deadline attached; render-thread vs main-thread tagging;
C-struct↔std140-uniform layout collapse (verify, or generate the MSL/GLSL
from the C structs). derive(serialis) for saves/replication/editor
inspection — what Unreal built UHT (a giant second parser) to get. The deep
one: **officina makes C89 solarium's scripting language.** Every engine
pays the perpetual tax at the script boundary (Lua bindings, marshaling,
two type systems); machinula's pointers ARE host pointers in the owned
region — interpreted and native code share structs by construction. Edit a
function in the 3D IDE → silva reparses → demissio lowers → machinula runs
it against live engine state: a REPL whose scripting language is the
engine's own language, typed against the engine's own headers, zero binding
layer. And the query layer IS solarium's database engine per silva's
CLAUDE.md — the 3D representation is queries rendered spatially: call
graphs as structure, expansion layers as literal layers, origo as visible
threads. The IDE is also the first-party surface for everything LSP
structurally cannot express (LSP is line/column over text — no tree, no
layers, no provenance): stepping through expansion layers in place, viewing
a file under a foreign-platform configuration, generated regions as live
widgets, refactorings previewed as tree diffs with their machinula verdict
attached. Aquinas (editor-as-OS, Acme, event sourcing), solarium
(code-as-database, 3D), silva/officina (parse + execution): three facets of
one editor-database-machine that has been assembling itself. Acme's genius
was plugins-as-programs over an exposed filesystem; acta is that idea with
a schema.

## XI. Grounding: the git-history compression experiment

Method: take real commits from rhubarb's own history (674 commits) and
hand-compress their diffs into hypothetical itinerarium form — the
operation vocabulary should be *discovered from the corpus*, per house
method, not imagined. Three specimens:

**Specimen 1 — `24b7c37` + `18046ec` "update links" (145 files, ~4,800
changed lines).** Bare mentions of "Demo NN" across the knotapel atlas
became anchors (`<a href="./entries/demo_NN.html">Demo NN</a>`), with a
non-obvious refinement: in runs like "Demos 02, 03, 04...", only the FIRST
number is linked. Pipeline form: ~2 rules (pattern-wrap demo references;
first-of-run restriction) ≈ a dozen lines. Compression on the order of
**400:1**, residual near zero. Two lessons: (a) quaestio being
grammar-agnostic ("works on any registrum") is load-bearing — this specimen
is HTML, and the sceletum/STML grammars make it in-scope; (b) the deeper
one: `generate_web.ts` changed 20 lines in the same commit — **the HTML
churn is the downstream shadow of a generator change.** With generation
provenance (§III lane 2), the true diff IS the generator edit plus a
"regenerate" op; 2,200 lines of derived churn would never be stored as
edits at all. In generated-artifact repos, most of the diff is provenance
waiting to be recorded.

**Specimen 2 — `da756c3` "working on refactoring type stuff" (768 ins / 86
del).** Schema evolution: specifier-order fields added to the declaratio
node (tok_unsigned/tok_signed/tok_long/tok_long2/tok_short +
specifiers_ordine), then initialized to NIHIL at *every construction site*
— the same 7-line block repeated across arbor2_c89.c, visible as stuttering
repetition in the raw diff. Pipeline form: one struct-field addition
(novel, residual) + one query over construction sites + one `<%adde>`
template. The mechanical ~40% of the diff compresses to ~2 operations; the
residual (the new emission-ordering logic in arbor2_scribere.c, the new
roundtrip fixtures) is exactly where the *idea* of the commit lives. The
residual-as-review-surface claim, demonstrated on the first C specimen
examined.

**Specimen 3 — `b04a83a` "migrate everything to color library" (325 ins /
313 del, 7 files).** The textbook Coccinelle collateral evolution: `i32
color` parameters retyped to `Color` across every signature; comment
cleanup ("(RGBA8888)" dropped); `palette_ad_rgb` renamed
`palette_ad_color`; bodies of the old helpers replaced by delegation to the
new library (`redde color_ex_palette(palette_index)`). Pipeline form: a
typed retype operation (parameter named color, typus i32 → Color — note
this is a TYPED query, inexpressible in Coccinelle's heuristic model), a
rename op (nexus-driven, all call sites), a comment transform, plus a
genuine residual: the new delegating bodies and the color.h include. Rough
split: ~80% mechanical / 20% novel — and a solver following the
Thistlethwaite staging (renames first, then structural, then local) would
peel it in exactly that order.

Also present in history: `8334540 "rename some files"` — file-level
renames, the VCS lane's recorded-operation case, where under annales there
is nothing to infer at all.

**What the specimens teach about the operation vocabulary:** the first ops
that earn seats are not exotic — pattern-wrap (text→structure), add-field +
init-at-construction-sites (schema evolution), retype-parameter (typed),
rename-symbol (nexus), replace-body / delegate-to (extract-and-call),
regenerate (provenance). Six or so schemas cover the mechanical majority of
the sampled history. The vocabulary wants to be sealed the way
vocabularium-medullae was: from evidence, with ceremony.

## XII. What exists today (the asset map)

| Pillar | Shipped asset |
|---|---|
| Full-fidelity parse | silva M0: byte-exact roundtrip oracle; trivia single-owner; all conditional arms retained; tree-sitter robustness bar |
| Typing | 100.00% expression coverage (1,503,915 nodes / 732 plagulae); {naturalis, conversus} frozen and public; auspex 3,753/0 clang-certified |
| Symbols | Symbol index + use-site→symbol nexus (silva_c89_symbolum_nodi, M1a); est_implicitum |
| Layout & constants | formam_computare; constans_aestimare; chorda_decodere |
| Provenance | origo chains (product pin); expansion layers; oracle with situ-scoped injection + in-place re-canonicalization |
| Comments | Attachment model specified (arbor2-comment-spec.md); fragmenta seat reserved; "comments are content" pin |
| Query | quaestio selector surface DECISUS 2026-07-06 (Latin pseudos, locus attributes verbatim, canonical-spine default, :sumptus/:omissus); silva_quaestio.{h,c} begun in fontes |
| Transform | DSL named (<%adde>/<%praepone>/<%substitue>), scoped as the milestone after selectors; formator rides it |
| Execution oracle | machinula: 55/73 suites fully interpreted; 50/50 byte-identical stdout vs native (captare.sh); deterministic; instruction counts; owned region; snapshots/time-travel = v2 flagship |
| IR as durable text | vocabularium-medullae sealed; byte-identical textual round-trip; golden fixtures — the precedent for pipeline-as-artifact |
| Corpus methodology | percursus sweeps; classified diagnostics; residuum-with-named-owners; the M0b audit discipline |
| Fact-store pattern | LAPIS (SQLite, provenance-complete); phase-log as hand-maintained metric store; smaragda as MCP house pattern |
| Portability datum | This session: officina suite green on Linux/gcc (3 build changes); catches: C99 snprintf in chorda.c, case fallthrough in amalgam — both lint-lane arguments |

## XIII. Named questions and the path in

**Ceremony-grade decisions (the vocabularium-medullae treatment each):**
1. **Node/symbol identity scheme** — everything keys off it: diff, merge,
   blame, literate anchors, fact keys. First-party transactional identity
   migration is the property none of the giants have; design it against
   origo + symbol paths + content hashes + the operation-log entry shape.
2. **Fact schema for acta** — the primitive fact kinds and their provenance
   columns; what lets plugins proliferate without coordination.
3. **Itinerarium operation vocabulary** — seeded from §XI's six schemas;
   grown from corpus evidence; sealed like medulla's ops. The pipeline
   deserves the thousand-year-surface treatment: textual form, byte-exact
   round-trip, goldens.
4. **Annotation grammar** — sealed semantics, extensible syntax
   (registry of comment patterns; `/*: ... */` interior as .stml).

**Open tensions, named so they stay decisions:** intensional-query honesty
(MDL costing so extensional edit-lists never masquerade as patterns);
free-text-edit reconciliation quality (the matcher is the fallback, not the
foundation); document-order merge (mild CRDT; auteur+agents keeps
concurrency rare); git-projection fidelity (the world speaks git; export is
one serialization).

**Cheapest first pulls, in leverage order:**
1. **Latina-landmine lint** — pure query over shipped tables; ten firings
   already paid for it; plugin-interface proof piece.
2. **MCP read tier** (quaestio/typus/nexus/forma/origo/emitte) — thin
   wrappers over shipped silva.h rows; changes how agents work on rhubarb
   immediately; smaragda precedent for the server shape.
3. **`interpretare`** — needs the M2c seal and little else; the
   epistemics tool.
4. **Standard-purity tag population** — the recensio item the :c89/:c99
   pseudos are waiting on; funded by this session's snprintf catch.
5. Then the ceremony decisions above, in order — identity first, since
   everything else keys off it.

*The unification, restated once: the repository is the fact store; a diff
is a query over it; the literate document and the file are both projections
of it; history is the event log; and every claim in any projection is
machinula-verifiable. All three predecessors of silva died from data-model
decisions that foreclosed features. This document exists so the features
above stay decisions — named, with paths back — instead of becoming
assumptions.*
