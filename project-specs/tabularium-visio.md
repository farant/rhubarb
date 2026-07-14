# TABULARIUM — the project as a single event-sourced store
## (vision seed — NOTHING DECISUS; names unsealed)

Born 2026-07-14, from a chat conversation (Fran + Claude) that started at
"what do VS/Xcode project manifests actually contain" and ended at "the
whole project is one SQLite file and the filesystem is a projection."
This note exists so the synthesis survives compaction with its reasoning
attached. It is a seed in the silva-instrumenta-visio / officina-acta-visio
tradition: named, not scheduled — except §IX phase one, which is the first
concrete deliverable if this is pulled.

Working name TABULARIUM: the Roman state archive on the Forum, where the
tabulae publicae were kept. Candidates also considered: arca, regesta.
NB collisions to avoid when sealing names: ACTA is already the candidate
name for the officina side-effect trace (officina-acta-visio.md);
`eventus` is already the event vocabulary of entitas_repositorium — reuse
it here deliberately, it is the same paradigm.

---

## I. The thesis

The toolchain ecosystem (silva, officina, vindex, LSP, lint, MCP,
saltuarius, tessera hosts) currently interoperates the way all toolchains
do: each tool re-derives the world from source text, and integration means
lossy handoff formats. The unix-philosophy failure is not "do one thing
well" (keep that) but "know nothing about each other, communicate through
untyped streams" (drop that). The alternative demonstrated by Roslyn and
rust-analyzer/salsa is not a monolith — it is **integration at the data
layer, modularity at the code layer**: one identity scheme, one store, one
AST substrate; separate components whose interop is function calls and
schema reads.

Discipline that keeps it from becoming mud: a strict dependency DAG.
silva + tabularium at the bottom; build engine, officina, annales stand on
those; vindex, LSP, lint, MCP surface are leaves. Leaves depend downward
and on the store, never sideways — the LSP never links the debugger, but
either may read facts the other produced, through the store. Consequence:
**the store schema and identity scheme are the real internal API.**
Function signatures between modules can churn cheaply; a stored fact's
shape cannot. Stability budget concentrates there.

We are in the Cargo/Fossil position, not the npm position: single
authority over every layer, so the manifest/store can be the sole source
of truth every tool reads. The rest of this document is what that store
is.

FRAMING (Fran, same day): **an ERP system for software projects.** The
mapping is tight because ERP is the one mature genre built on exactly
this thesis — modules integrated at the data layer: general ledger =
canonical log (journal entries = eventus); master data = the identity
scheme (§III), with master-data governance = "stability budget
concentrates on the schema"; MRP/BOM = the build fold (§IX): net
requirements = targets minus valid cached artifacts, pegging = build
provenance; inventory = the CAS; subledgers and reports = folds; audit
trail = actor-attributed events; period close = closing the books (§VII
already borrowed the term); trial balance = the self-verifying refold
check (§IV). Software development is arguably the last operational
domain never consolidated onto a system of record — it still runs
"islands of automation" (git + CI + tracker + wiki + APM), and the
closest existing attempt (GitLab's "single application") integrates at
the UI/SaaS level, not the data-model level. Where the frame breaks,
usefully: no ERP has a parser of reality in the loop (silva makes the
folds semantically deep in a way ERP reports never were), and
single-writer auteur scale skips the locking/permissions/workflow
bureaucracy that is most of ERP's mass.

## II. The store: one SQLite file, three strata

The project IS a single SQLite database. The filesystem is a projection
of it (§V). Precedents: Fossil (repo = SQLite db; checkout = regenerable
projection; relational tables = documented-rebuildable cache), the SQLite
team's own "SQLite as application file format" advocacy, Unison (codebase
= content-addressed definitions in SQLite; scratch files = ephemeral
editing surface).

Three strata inside the one file, with different retention law:

1. **Canonical log** — append-only events recording *decisions and
   inputs*: source ingests, manifest/config changes, annotations, issue
   actions. This is what the project IS. Never compacted except by
   explicit history-rewrite ceremony. Annales (§VII) is a view over it.
2. **Blob store** — content-addressed artifacts (CAS): source blob
   versions, build outputs, rendered projections. A cache: GC-able by
   reachability, delta-compressed (Fossil's delta encoding is the prior
   art), never required for replay.
3. **Observational log** — append-only facts *about* states: test
   outcomes, perf counters, timings, stdout captures. Keyed by
   (state hash, action, actor). Precious because expensive to reproduce;
   truncatable/summarizable without changing project identity.

The stratum test for any new record type: if losing it changes what the
project is → canonical. If it merely costs recomputation → blob cache.
If it loses knowledge about the past but not the past itself →
observational.

Event vocabulary: **coarse, content-addressed, dumb.** "Blob H exists",
"entity E: hash A → hash B (actor, time)", "manifest M supersedes M'".
NOT semantic events ("renamed X to Y") — that is the darcs/pijul road
where patch algebra eats the project and every vocabulary gap is an
unrepresentable change. Semantic descriptions (itinerarium's territory)
are DERIVED from coarse events by silva, or recorded as annotations —
never load-bearing for replay. Truth stays dumb; intelligence stays in
the folds. This also kills the classic event-sourcing wound (event
schema migration): there is nothing to upcast, a hash is a hash.

In-house precedent that the paradigm works: entitas_repositorium
(lib/entitas_repositorium_impl.c) is already event-sourced with
replay-based init, nuntium persistence, and a RetentionPolicy design
(unified-state-paradigm-v2.md). Tabularium is the same idea promoted from
app state to project state. DECISION OWED (not here): whether tabularium
eventually absorbs/generalizes entitas_repositorium's engine or they
remain siblings sharing a paradigm.

Dependency note: SQLite would be the one vendored exception to
zero-dependency. Defense: public domain, single amalgamation of
conservative C, zero deps of its own, support pledged to 2050, and the
Fossil precedent — Hipp built the auteur all-in-one tool ON SQLite rather
than a bespoke store, because durability engineering (fsync semantics,
torn pages, corruption recovery) is the one domain where failure silently
destroys the entire project state. Vendor the amalgamation; treat it like
libc. DECISION OWED: formal acceptance of this exception.

## III. Identity: the decision that outlives everything

Everything below stands on the identity scheme, so it gets stated first.

- **Content-addressed, never mtimes.** Modification times are the root
  cause of Make's flakiness; content hashing is what Bazel, Nix, ccache
  converged on independently. Mtimes appear NOWHERE in tabularium.
- **Key space richer than files from day one.** Phase one implements
  file-level staleness, but keys are (kind, id) where kind can later be a
  declaration/segment/subtree. Affected-test selection and per-function
  metrics across renames both live or die on subtree identity;
  retrofitting a key space is far worse than under-using one.
- Silva already holds the sub-file identity machinery this will want:
  per-segment parsing, and the journal (macro state per stream position)
  + oracle (typedefs per position) that silva-incrementalitas.md
  identifies as the natural segment-validity predicate. Same predicate,
  new consumer.

## IV. Folds: projections with per-fold policy

Everything downstream of the canonical log is one kind of thing: a
**fold** — a memoized function of content-addressed inputs, materialized
as tables in the same file. Current-state tables, symbol indexes, build
artifacts, metrics rollups, rendered docs: all folds. Properties:

- **Transactional with the log.** Append event + update materialized
  views in ONE transaction. Single-writer in-process SQLite dissolves
  every distributed-event-sourcing pain (lagging projections, eventual
  consistency, rebuild orchestration). No window where view disagrees
  with log, ever.
- **Two dials per fold**, set individually (this is the payoff of owning
  the stack): *scope* — wholesale refold vs incremental maintenance
  (derived rows record their input hashes; a mutation displaces a hash
  set; "affected rows" is a join); *timing* — eager (in the append's
  transaction; anything on an interactive read path, e.g. LSP symbol
  index) vs lazy (invalidate now, refold on next read; expensive
  whole-program analyses).
- **Disposable and code-versioned.** A bug in a fold is not data
  corruption: fix the fold, bump its version, drop the table, refold.
  Derived tables are cheap to add speculatively and free to be wrong.
- **Self-verifying store.** Refold from the log, diff against standing
  tables → corruption/fold-bug detection as a health command.
- Snapshot problem answers itself: the current-state tables ARE the
  standing snapshot; the log is consulted for history, not for "now."

## V. Files as projections; leased checkouts

The filesystem becomes one view among several. Two-way binding is the
staleness problem at the boundary, so it is solved with the same
machinery, not with faith in watchers:

- **Explicit verbs**: `materializare` (DB → files) and `ingerere`
  (files → DB) are the semantics. Watchers, if added, merely TRIGGER the
  verbs. Never trust a watcher event; always rehash. (The
  manutheca ingest/materialize pipeline and smaragda's
  run_serialization/import_filetree are this pattern, already built
  twice in other domains.)
- **Three-hash reconciliation**: per projected file store the hash it
  was projected as; at sync time compare (last-projected, file-now,
  db-now) → ingest / re-project / real conflict surfaced. Projection and
  ingestion are nodes in the build graph — cached, invalidated like any
  action.
- **Leased checkouts**: a checkout is not "the working directory"; it is
  a projection bound to (actor, branch). The human editor session gets
  one; each agent session gets one on its own branch. Everything arriving
  through a checkout is attributed to its lease — solving the watcher's
  attribution blindness. Editor atomic-rename saves and inotify
  coalescing are handled by rehash-on-trigger.

CANONICAL-FORM FORK (the one that decides good vs catastrophic):
**text-canonical at 1.0.** The canonical column is source text; ASTs,
symbols, metrics are derived tables keyed by content hash. Byte-exact
round-trip is trivial, any editor works, and the derived tables still
answer every query. AST-canonical (the Unison move: text is a
pretty-print) is the maximalist upgrade but stands on two load-bearing
requirements — silva preserving every trivium with full fidelity, and
the pretty-printer becoming sole arbiter of formatting forever — and the
C preprocessor (ifdef regions, macro bodies) is why nobody has done
Unison-for-C. Schema treats "canonical column" as a designated role so
promotion stays possible once silva's lossless round-trip is proven
against the whole monorepo. DECISION RECORDED, not owed: text first.

## VI. Edits as events; commits demoted to annotations

Precedents that each did a piece: Smalltalk's changes file (every method
edit logged, replayable), JetBrains Local History (mini-commit per save,
cost unnoticed), Google CitC (db-backed store presented as a
filesystem), LSP didChange (the per-edit delta stream already exists in
transit — tabularium writes it down).

- **Canonical grain = content transitions** (save/ingest level). Not
  keystrokes: volume is fine for SQLite, but keystroke history is noise
  with no replay value. If session-replay/fine-undo is wanted, the
  didChange stream goes in the OBSERVATIONAL stratum and closing-the-books
  compacts it to transitions.
- **A commit is an annotation over a log interval.** Units of work,
  checkpoints, releases: named, signed ranges — written at the time or
  retroactively, reorganizable without touching truth. Inversion of git,
  where the commit is the storage atom and everything between is
  destroyed.
- **Branches** = refs into the log + their own materialized projections
  (git's trick, kept). Per-agent-session branches make "what did the
  agent do while I was gone" a query returning every intermediate state
  WITH its test results (§VII), since continuous CI is just the
  staleness fold running on each transition.
- **Agent integration, two doors, one event stream**: (a) MCP tools
  against the store — perfect attribution, plus semantic edit ops
  (rename, wrap, insert) that silva validates BEFORE commit: commit
  clean, commit flagged-unparseable, or reject. Line-based editing
  replicates trivially since the projection is deterministic text.
  (b) Leased checkout + stock file tools for humans and unmodified
  agent tooling. Both produce identical eventus rows.
- Merge is NOT dissolved by fine-grained history: divergent branches
  still need three-way text merge at minimum; AST-aware merge is the
  itinerarium-flavored upgrade, later. Named pull, not phase one.

## VII. Annales, telemetry, and closing the books

**Annales (VCS) stops being a component and becomes a view.** If annales
and the build cache key on the same content hashes, "what changed since
X" and "what is stale relative to X" are the same query against the same
store — a sentence no git+make+CI stack can say. Fossil is the proof the
auteur VCS is sane; delta compression of the blob store is the growth
control (log grows with churn, not with edits).

**Observational stratum** holds run telemetry keyed (state hash, action,
actor): test outcomes, perf counters, stdout (bulky → blob store,
aggressive retention: raw for the current period, summaries forever).
Unlocks the queries the sediment world cannot ask: which edit event
flipped this test; perf trajectory of one function across an afternoon;
was the agent's branch ever green before merge.

**Closing the books** = periodization, in the accounting sense event
sourcing came from: seal a period, retain canonical journal + summaries,
archive raw detail. Mechanism: SQLite ATTACH — one hot database, N
sealed period files, query layer unchanged. RetentionPolicy shapes
(FOREVER / SNAPSHOT_N / TTL / MANUAL) already designed in
unified-state-paradigm-v2.md; reuse the vocabulary.

## VIII. Annotations, issues, and the documentation layer

**Anchor rot is the disease; semantic identity is the cure.** Every
text-era annotation system anchors to (file, line) and dies when code
moves. Tabularium annotations anchor to entity identity (§III), survive
rename/move/reformat, and when the anchored entity's hash IS displaced,
the log names the exact intervening events — annotations that know they
are stale instead of lying forever.

- **Annotation is data; rendering is a fold.** One record → inline
  comment in the text projection (render-only, or strictly
  sigil-delimited so ingest can re-absorb — the one two-way-hard case),
  sidecar files, LSP diagnostics/lenses at the anchored site, MCP
  retrieval context (an agent asking about a function receives its
  invariants, open issues, perf history — precision context injection
  instead of CLAUDE.md sprawl).
- **Typed, small, closed vocabulary** + a prose escape hatch. Lint
  suppression becomes a record with author/reason/expiry instead of a
  magic comment. The auteur version of Jira is still Jira; the power is
  anchoring + projection, not taxonomy.
- **Stratum rule**: machine-derived facts are folds (lint findings,
  coverage — recomputable, never stored as annotations); human/agent
  judgments ABOUT machine facts are canonical events (suppressions,
  acknowledgments, triage). Issues are entities with typed edges
  (reproduced-by test, introduced-in interval, anchored-to entity,
  fixed-by event) — resolvable ON A BRANCH, merging like the fix itself.
  Auto-filing fold: test flips red at event e → issue anchored to the
  entities e displaced, repro state attached by construction.
- Annotations on non-textual targets become possible: call-graph edges,
  entity relationships, build configs, log intervals.

**Documentation = the smaragda palace pattern as a projection over
annotations.** Rooms are curated views anchored to live entities (facts
render current-state, only connective prose is authored); scrolls are
dated session annotations; NPC dialogue trees are structured rationale
with `requires`/`unlocks` as machine-enforced prerequisite structure.
Because rooms are anchored, doc-rot detection is MECHANICAL (anchor
displacement flags the room and queues gardening). The palace serves
cold-start orientation; the raw query API serves known-item retrieval —
two doors over one store. glossa (literate programming) is a sibling
projection over the same annotations: woven document instead of spatial
traversal, no tangle step.

Ordering principle (also in CLAUDE.md's context-is-non-commutative
passage): semantics behaves as a curved manifold — travel is
non-transitive, so presentation ORDER changes what arrives, for humans
and LLMs alike. Cosine-similarity RAG is a flatness assumption; the
palace stores paths, not just positions. Unit of doc authorship = the
route, not the page. Layered access = an atlas of charts over one
territory (also the answer to sprawl). Entry rooms carry
disproportionate weight: they set the frame everything downstream is
transported through.

## IX. Phase one: the build fold (the first thing actually built)

Deliverable, precisely: **the identity scheme + store schema + the build
engine as the first registered fold** — proof-of-concept query, not a
standalone tool with a private cache (a private cache re-creates the
sediment locally and annales would need an adapter later).

- Target properties, in Build-Systems-à-la-Carte vocabulary: minimal
  rebuilds, early cutoff (byte-identical recompile stops the cascade —
  huge for C header churn), constructive traces (artifacts cached by
  input hashes; branch-switching costs nothing; "clean build" stops
  being a concept).
- Node = (action, input hashes) → artifact hash, artifacts in the CAS.
  Mechanically ccache generalized.
- **Dependency discovery is the unfair advantage**: no `gcc -M`
  scraping — silva IS the include graph; the build layer queries the
  parser that already exists. First concrete payoff of the integration
  thesis.
- Tests join for free via Bazel's one great insight: a test is a build
  action whose artifact is its result → test caching and
  affected-test selection are the same staleness query. (Officina's
  golden traces — officina-acta-visio.md — are the stronger future net
  for "did behavior change"; format-first discipline applies.)
- Current compile_*.sh scripts + Makefile + compile_flags.txt are the
  system being subsumed; they remain the bootstrap path until the fold
  proves itself against the monorepo.
- File-level staleness only, on the §III key space. Segment-level
  arrives later by pull; silva-incrementalitas.md already establishes
  nothing forecloses it (and names the arena-lifetime question as the
  one refactor to consult first).

Name candidates for the build fold: structor, fabrica (NB check against
existing fabrica.tsv at repo root before sealing).

## X. Scope discipline (the Eskil rule)

Phase one is §IX and only §IX. Everything else is a named pull with its
consumer recorded: leased checkouts pull when an editor/agent needs
write-through; annales pulls when history queries are wanted; telemetry
pulls with the first perf question; annotations pull with the first
anchor consumer (probably LSP lenses or MCP context); the palace pulls
when the doc problem CLAUDE.md already names ("eventually neither of you
understand the project") bites at toolchain scale. The interface
decision worth making knowingly NOW is §III identity + the stratum
test + text-canonical — those are the load-bearing walls; everything
else is furniture.

What this is NOT: not a distributed system (single-writer, in-process);
not a registry/package manager (zero-dep philosophy unchanged); not
AST-canonical storage (recorded above); not a merge-theory project
(pijul's fate declined); not a taxonomy engine (annotation vocabulary
stays small).

## XI. Decisions owed (ledger)

1. Formal acceptance of SQLite as the vendored exception (§II).
2. tabularium vs entitas_repositorium: absorb or siblings (§II).
3. Event schema in detail: eventus table shape, actor model, branch refs.
4. Per-fold policy defaults (eager/lazy, wholesale/incremental) for the
   first fold set: build, symbol index, current-state text.
5. Sealing of names: tabularium/arca/regesta; structor/fabrica;
   materializare/ingerere.
6. Blob delta encoding: adopt Fossil's scheme vs own.

## Cross-references

silva-incrementalitas.md (segment validity predicate = journal/oracle;
arena-lifetime constraint; offsets intra-parse invariant) ·
officina-acta-visio.md (trace/snapshot/diff seed; ACTA name collision;
format-first discipline; consumer-list genre) · unified-state-paradigm-v2.md
(in-house event sourcing + RetentionPolicy + WAL/debounce prior art) ·
officina-probatio-pillar.md (typed-heap snapshot testing, future
observational consumer) · smaragda.md / smaragda-v2.md (palace pattern,
branch/workspace kernel) · silva-lsp-spec.md (first eager-fold consumer)
· CLAUDE.md (non-commutative context passage; the documentation problem;
worklog convention).
