# Visiones extractae — tabularium-visio.md

*Extracted 2026-10-02 from `project-specs/tabularium-visio.md`
(2026-07-14). Unlike brighton-visio.md this is NOT Fran verbatim: it is
a synthesis of a Fran + Claude conversation. Claims the source attributes
to Fran explicitly are marked `(Fran)`. One claim per bullet, worded as
close to the source as possible; `Lnn` = source line; `(illatum)` = my
inference. Section XII is my commentary, kept apart.*

*Marking: put `[+]` on the claims to keep for the distillate; leave the
rest as they are.*

*Classified 2026-10-02 into the vision taxonomy (`docs/taxonomiae.html`):
`{PRINCIPIUM}` prime condition (no intervention inside), `{OCCASIO}` opportunity, `{INDICIUM}` clue, `{REGULA}` commitment, `{VISIO-OPERIS}` project vision, `{DECRETUM}`
load-bearing decision, `{SUPELLEX}` furniture (not vision), `{nulla:…}`
fits no tier (ratio = a reason or argument, exemplum = a precedent,
quaestio = an open question). No item is telos.*

## I. The thesis and why

- [ ] T1 Toolchains interoperate badly: each tool re-derives the world from source text, and integration means lossy handoff formats (L23-26)  {PRINCIPIUM}
  - [ ] T1.1 keep unix's "do one thing well"; drop "know nothing about each other, communicate through untyped streams" (L26-28)  {REGULA}
- [ ] T2 The alternative (Roslyn, rust-analyzer/salsa) is not a monolith: **integration at the data layer, modularity at the code layer** (L28-32)  {REGULA}
  - [ ] T2.1 one identity scheme, one store, one AST substrate; components interoperate by function calls and schema reads (L30-32)  {VISIO-OPERIS}
- [ ] T3 A strict dependency DAG keeps it from becoming mud: silva + tabularium at the bottom, leaves depend downward and on the store, never sideways (L34-39)  {DECRETUM}
  - [ ] T3.1 **the store schema and identity scheme are the real internal API**: function signatures can churn cheaply, a stored fact's shape cannot; the stability budget concentrates there (L39-41)  {PRINCIPIUM+intervention, split}
- [ ] T4 We are in the Cargo/Fossil position, not npm's: single authority over every layer, so one store can be the sole source of truth every tool reads (L43-45)  {OCCASIO}
- [ ] T5 **An ERP system for software projects** (Fran) (L48)  {VISIO-OPERIS}
  - [ ] T5.1 ERP is the one mature genre built on this thesis: modules integrated at the data layer (L49-50)  {nulla:ratio}
  - [ ] T5.2 the mapping: general ledger = canonical log; master data = identity scheme; MRP/BOM = the build; inventory = the blob store; reports = folds; audit trail = attributed events; period close = closing the books (L50-58)  {nulla:ratio}
  - [ ] T5.3 software development is "arguably the last operational domain never consolidated onto a system of record": it still runs islands of automation (git + CI + tracker + wiki + APM) (L58-61)  {OCCASIO}
  - [ ] T5.4 GitLab's "single application" integrates at the UI level, not the data-model level (L61-62)  {INDICIUM}
  - [ ] T5.5 where the frame breaks: no ERP has a parser of reality in the loop (silva makes the folds semantically deep), and single-writer scale skips ERP's locking/permissions/workflow bureaucracy (L62-66)  {OCCASIO}

## II. The store

- [ ] T6 The project IS a single SQLite database; the filesystem is a projection of it (L70-71)  {DECRETUM}
  - [ ] T6.1 precedents: Fossil (repo = SQLite db, checkout = regenerable projection), SQLite as application file format, Unison (L71-75)  {nulla:exemplum}
- [ ] T7 Three strata with different retention law (L77)  {DECRETUM}
  - [ ] T7.1 canonical log: append-only decisions and inputs; "what the project IS"; never compacted except by explicit ceremony (L79-83)  {DECRETUM}
  - [ ] T7.2 blob store: content-addressed artifacts; a cache, garbage-collectable, never required for replay (L84-86)  {DECRETUM}
  - [ ] T7.3 observational log: facts *about* states (test outcomes, timings, stdout), keyed (state hash, action, actor); precious because expensive to reproduce, but truncatable (L87-90)  {DECRETUM}
  - [ ] T7.4 the stratum test: loses what the project is → canonical; costs recomputation → blob; loses knowledge about the past but not the past → observational (L92-95)  {DECRETUM}
- [ ] T8 Events are **coarse, content-addressed, dumb**: "blob H exists", "entity E: hash A → hash B" (L97-99)  {DECRETUM}
  - [ ] T8.1 not semantic events ("renamed X to Y"): that is the darcs/pijul road, where patch algebra eats the project (L99-101)  {nulla:ratio}
  - [ ] T8.2 "Truth stays dumb; intelligence stays in the folds" (L103-104)  {REGULA}
  - [ ] T8.3 this removes event-schema migration: a hash is a hash (L104-105)  {nulla:ratio}
- [ ] T9 In-house precedent: entitas_repositorium is already event-sourced; tabularium promotes the idea from app state to project state (L107-111)  {nulla:exemplum}
- [ ] T10 SQLite would be the one vendored exception to zero dependencies (L115)  {DECRETUM > R3}
  - [ ] T10.1 defence: public domain, conservative C, no deps of its own, support pledged to 2050 (L116-117)  {nulla:ratio > T10}
  - [ ] T10.2 durability engineering is "the one domain where failure silently destroys the entire project state"; vendor it and treat it like libc (L118-121)  {nulla:ratio > T10}

## III. Identity

- [ ] T11 Identity is "the decision that outlives everything"; everything else stands on it (L124-126)  {PRINCIPIUM}
- [ ] T12 Content-addressed, never mtimes: mtimes are the root of Make's flakiness; Bazel, Nix and ccache converged on hashing independently (L128-130)  {DECRETUM}
- [ ] T13 The key space is richer than files from day one: (kind, id), where kind can later be a declaration or subtree (L131-133)  {DECRETUM}
  - [ ] T13.1 retrofitting a key space is far worse than under-using one (L134-135)  {nulla:ratio}
- [ ] T14 Silva already holds the sub-file identity machinery (per-segment parsing, journal + oracle) (L136-140)  {OCCASIO}

## IV. Folds

- [ ] T15 Everything downstream of the log is one kind of thing: a **fold**, a memoized function of content-addressed inputs, materialized as tables (L144-147)  {DECRETUM}
  - [ ] T15.1 current-state tables, symbol indexes, build artifacts, metrics, rendered docs are all folds (L146-147)  {DECRETUM}
- [ ] T16 Transactional with the log: append + update views in one transaction; no window where a view disagrees with the log (L149-153)  {DECRETUM}
- [ ] T17 Two dials per fold, the payoff of owning the stack: scope (wholesale vs incremental) and timing (eager vs lazy) (L154-160)  {SUPELLEX}
- [ ] T18 Folds are disposable and versioned: a bug in a fold is not data corruption; fix, bump, drop, refold (L161-163)  {DECRETUM}
- [ ] T19 Self-verifying store: refold from the log and diff against the standing tables (L164-165)  {SUPELLEX}

## V. Files as projections

- [ ] T20 The filesystem is one view among several (L171)  {DECRETUM}
- [ ] T21 Explicit verbs, `materializare` (DB → files) and `ingerere` (files → DB); never trust a watcher event, always rehash (L175-180)  {DECRETUM}
- [ ] T22 Three-hash reconciliation (last projected, file now, db now) → ingest, re-project, or a real conflict (L181-185)  {SUPELLEX}
- [ ] T23 Leased checkouts: a checkout is a projection bound to (actor, branch); each agent session gets its own; everything through it is attributed (L186-191)  {DECRETUM}
- [ ] T24 **Text-canonical at 1.0** (decision recorded): the canonical column is source text; ASTs and symbols are derived (L193-204)  {DECRETUM}
  - [ ] T24.1 AST-canonical (Unison's move) needs lossless silva and a pretty-printer as sole arbiter of formatting forever; the C preprocessor is why nobody has done Unison-for-C (L197-202)  {nulla:ratio}
  - [ ] T24.2 the schema keeps "canonical column" a role, so promotion stays possible once silva's round-trip is proven on the whole monorepo (L202-204)  {SUPELLEX}

## VI. Edits as events

- [ ] T25 Canonical grain = content transitions (save/ingest), not keystrokes (L214-218)  {DECRETUM}
- [ ] T26 **A commit is an annotation over a log interval**: named ranges, written at the time or later, reorganizable without touching truth; the inversion of git (L219-223)  {DECRETUM}
- [ ] T27 Per-agent-session branches make "what did the agent do while I was gone" a query, every intermediate state with its test results (L224-228)  {VISIO-OPERIS}
- [ ] T28 Agents enter by two doors producing one event stream: MCP tools with semantic edits that silva validates before commit, or a leased checkout with stock file tools (L229-235)  {DECRETUM}
- [ ] T29 Merge is not dissolved: divergent branches still need at least three-way text merge (L236-238)  {nulla:ratio}

## VII. History, telemetry, closing the books

- [ ] T30 Version control stops being a component and becomes a view (L242)  {VISIO-OPERIS}
  - [ ] T30.1 if history and the build cache key on the same hashes, "what changed since X" and "what is stale relative to X" are the same query (L243-245)  {nulla:ratio}
- [ ] T31 Telemetry unlocks questions the "sediment world" cannot ask: which edit flipped this test; one function's perf across an afternoon; was the agent's branch ever green before merge (L249-254)  {VISIO-OPERIS}
- [ ] T32 Closing the books, in the accounting sense: seal a period, keep the canonical journal and summaries, archive raw detail (SQLite ATTACH) (L256-261)  {VISIO-OPERIS}

## VIII. Annotations, issues, documentation

- [ ] T33 **Anchor rot is the disease; semantic identity is the cure**: annotations anchored to (file, line) die when code moves (L265-266)  {PRINCIPIUM+intervention, split}
  - [ ] T33.1 annotations anchored to entity identity survive rename and move, and know when they are stale "instead of lying forever" (L267-270)  {VISIO-OPERIS}
- [ ] T34 Annotation is data; rendering is a fold: inline comment, sidecar, LSP lens, or MCP context (L272-276)  {DECRETUM}
  - [ ] T34.1 an agent asking about a function receives its invariants, open issues and perf history: "precision context injection instead of CLAUDE.md sprawl" (L276-278)  {VISIO-OPERIS}
- [ ] T35 A typed, small, closed vocabulary plus a prose escape hatch; "the auteur version of Jira is still Jira; the power is anchoring + projection, not taxonomy" (L279-282)  {DECRETUM}
  - [ ] T35.1 a lint suppression becomes a record with author, reason and expiry (L280-281)  {SUPELLEX}
- [ ] T36 Machine-derived facts are folds; human/agent judgements about them are canonical events (L283-286)  {REGULA}
- [ ] T37 Issues are entities with typed edges (reproduced-by, introduced-in, anchored-to, fixed-by), resolvable on a branch (L286-289)  {DECRETUM}
  - [ ] T37.1 auto-filing: a test flips red at event e → an issue anchored to what e displaced, repro attached by construction (L289-290)  {SUPELLEX}
- [ ] T38 **Documentation = curated views anchored to live entities** (the smaragda palace pattern): facts render current state, only connective prose is authored (L294-296)  {VISIO-OPERIS}
  - [ ] T38.1 because views are anchored, doc rot is detected mechanically (L299-300)  {VISIO-OPERIS}
  - [ ] T38.2 two doors over one store: the palace for cold-start orientation, the query API for known-item retrieval (L300-302)  {DECRETUM}
  - [ ] T38.3 literate programming (glossa) is a sibling projection: a woven document instead of spatial traversal (L302-304)  {SUPELLEX}
- [ ] T39 Presentation ORDER changes what arrives, for humans and LLMs alike; "the palace stores paths, not just positions" (L306-310)  {PRINCIPIUM+intervention, split}
  - [ ] T39.1 "Unit of doc authorship = the route, not the page" (L311)  {DECRETUM}
  - [ ] T39.2 entry rooms carry disproportionate weight: they set the frame everything downstream is read through (L312-314)  {nulla:ratio}

## IX. Phase one: the build

- [ ] T40 The first thing built: identity scheme + store schema + the build engine as the first fold, not a standalone tool with a private cache (L318-321)  {DECRETUM}
- [ ] T41 Minimal rebuilds, early cutoff, constructive traces; "clean build" stops being a concept (L323-327)  {VISIO-OPERIS}
- [ ] T42 **Dependency discovery is the unfair advantage**: no `gcc -M` scraping; silva IS the include graph (L330-333)  {DECRETUM}
- [ ] T43 A test is a build action whose artifact is its result, so test caching and affected-test selection are the same staleness query (L334-336)  {DECRETUM}

## X. Scope discipline

- [ ] T44 Phase one only; everything else is a named pull with its consumer recorded (the Eskil rule) (L352-358)  {REGULA}
- [ ] T45 The load-bearing walls to decide knowingly now: identity, the stratum test, text-canonical; "everything else is furniture" (L358-361)  {nulla:ratio}
- [ ] T46 What it is NOT: not distributed, not a package manager, not AST-canonical, not a merge-theory project, not a taxonomy engine (L363-367)  {DECRETUM > R3}

## XI. What the text leaves open

- [ ] T47 SQLite as the formal vendored exception (L371)  {nulla:quaestio}
- [ ] T48 tabularium vs entitas_repositorium: absorb or siblings (L372)  {nulla:quaestio}
- [ ] T49 the event schema in detail; per-fold policy defaults; delta encoding (L373-378)  {nulla:quaestio}

## XII. Since then (my commentary, not in the source)

- What exists today under the name tabularium is the project LEDGER
  (lifecycle records, regions, inventories), with truth in a committed
  JSONL journal and the `.db` a rebuildable projection: the canonical-log
  idea of T7.1, applied to records rather than to the whole project.
- The build fold of §IX became **fabrica** (the name candidate at L347)
  and **aedilis**, as separate tools, not folds inside one store.
- The project is not (yet) one SQLite file; the filesystem is still the
  truth for code.
- §VIII is a design for the documentation project we started today:
  anchored views, mechanical rot detection, "the route, not the page",
  entry rooms carrying the frame. Worth reading again for the book.
