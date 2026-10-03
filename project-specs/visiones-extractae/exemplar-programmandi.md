# Visiones extractae — exemplar programmandi (ledger)

*Extracted and classified 2026-10-02 from the ledger vision record
01M3XQVDSY87M80CKNDTFA71YH, "Exemplar programmandi: actores gradibus
puris + effectus per servos + 'arbor cursus' (runtime AST) - bibliotheca
exploratoria" (desideratum natura:visio, created 2026-10-02 07:21, eight
notes to 14:43). The first source taken from the ledger rather than from
project-specs. Fran's ideas from that day's conversations (BEAM,
microkernels, the Dreams interview, Anton's "large arrays of things"),
recorded in Latin by Claude (signed Opus 5.5); every note opens "Fran".
The English here is MY TRANSLATION: one claim per bullet, as close to the
Latin as I could keep it. `(c)` = the core text, `(n1)`…`(n8)` = the
notes in order. The last section is my commentary, kept apart.*

*Marking: put `[+]` on the claims to keep for the distillate.*

*Tags (see `docs/taxonomiae.html`): `{PRINCIPIUM}` prime condition,
`{PRINCIPIUM+intervention, split}` a condition with its cure attached,
`{OCCASIO}` opportunity, `{INDICIUM}` clue, `{REGULA}` commitment,
`{VISIO-OPERIS}` project vision, `{DECRETUM}` load-bearing decision,
`{SUPELLEX}` furniture, `{nulla:…}` fits no tier (ratio, exemplum,
quaestio). `> An` = the item(s) it is evidence for.*

## I. The core: actors, effects, the runtime tree

- [ ] A1 "No threads by design" was prudence for the start, not dogma: in time, safe and transparent parallelism will be wanted (c)  {nulla:ratio}
- [ ] A2 But the bigger gain is not speed: it is the right decomposition (c)  {nulla:ratio > A3}
- [ ] A3 Actors are step functions with state, a pool and a mailbox; input is traced, effects go through servants (an interface); nothing mutable is shared, and the silva lint enforces it (c)  {DECRETUM}
- [ ] A4 This gives a RUNTIME TREE: as silva gives the tree of structure, a run gives a queryable graph of behaviour (c)  {VISIO-OPERIS}
  - [ ] A4.1 provenance: why does this byte exist → which reads produced it (c)  {VISIO-OPERIS}
  - [ ] A4.2 exact replay, and stepping backwards (c)  {VISIO-OPERIS}
  - [ ] A4.3 diffing runs step by step (c)  {VISIO-OPERIS}
  - [ ] A4.4 tests about behaviour: "the generator never touches the clock" (c)  {VISIO-OPERIS}
  - [ ] A4.5 measurement per step (c)  {VISIO-OPERIS}
- [ ] A5 Textual logs are the weak form; here the trace is COMPLETE, because every exchange passes through a recorded channel (c)  {PRINCIPIUM+intervention, split}
- [ ] A6 The same shape at every size: fabrica already is it (jobs = actors, needs = traces, plans = effects, runs = traces); the library gives the same inside one program (c)  {nulla:exemplum > A3}
- [ ] A7 Suggested order (c)  {DECRETUM}
  - [ ] A7.1 hand-written step actors (protothreads, Dunkels), a deterministic single-threaded scheduler, effects through an interface, the trace primary (c)  {DECRETUM}
  - [ ] A7.2 prove it on something small and real (c)  {DECRETUM}
  - [ ] A7.3 silva generates the state machines (async as in C# and Rust; reductions inserted in loops as in BEAM) (c)  {VISIO-OPERIS}
  - [ ] A7.4 separately, a parallel scheduler (threads, nothing shared, pools handed over), with the same trace semantics (c)  {VISIO-OPERIS}
- [ ] A8 A virtual machine is NOT needed: arbitrary preemption, hot code loading, foreign code are things we don't need (c)  {DECRETUM}
- [ ] A9 Precedents: BEAM/OTP, Pony, Go, Rust async, libdill, Cilk, microkernels and Plan 9, effect handlers (Koka), Erlang's seq_trace, OpenTelemetry (c)  {nulla:exemplum > A3}
- [ ] A10 An exploratory library of its own, outside fabrica; the API after exploration (Eskil) (c)  {DECRETUM}

## II. From the Dreams interview and Anton, part I

- [ ] A11 Double state in Dreams: new G = f(const old G, bundle of inputs); other threads (render, audio) read the immutable G without locks; deletion = not copying; no fragmentation; cache order sorts itself; "uniformly slow" (the worst case is the always case) (n1)  {nulla:exemplum > A12}
- [ ] A12 In the house: two alternating pools (read from A, build B, empty A, swap), a semispace copying collector by program semantics; an actor's step is new state = step(const old state, messages) (n1)  {DECRETUM}
- [ ] A13 Determinism checked at run time: run a frame twice and compare byte for byte (fabrica's memo audit is the same) (n1)  {VISIO-OPERIS}
- [ ] A14 A crash carries its own reproduction: the image plus initial state, all inputs and the last good state (after the PNG's end); reproduction without clicking (n1)  {VISIO-OPERIS}
- [ ] A15 Reproduction holds ONLY inside a hermetic boundary: anything unrecorded (the network) makes a run irreproducible, so "the channel must not be bypassed" (n1)  {PRINCIPIUM}
- [ ] A16 Alex's metric: time to reproduction, not compile time (n1)  {nulla:ratio > A14}
- [ ] A17 A spectrum of data by rate of change: recreated every frame / written once (a content-addressable store) / the middle, persistent copy-on-write Merkle trees (Hickey) (n1)  {PRINCIPIUM}
  - [ ] A17.1 the middle of that spectrum is missing from the house (n1)  {nulla:ratio > A17}
- [ ] A18 LBP2 discovered its dependencies one at a time, a round trip each; Dreams enumerates them all before running (monadic → applicative) (n1)  {nulla:exemplum}
- [ ] A19 Assertions are hard crashes even in release builds, and only about invariants, never about input (n1)  {REGULA}
- [ ] A20 Anton's "large arrays of things": one fixed pool, INDICES not pointers, index 0 as NIL (a real nulled thing: reads give zeros, writes land in a garden, so `kid.kid.sib.type` never crashes), zero-is-initialization, intrusive lists and trees, a slot map with generations ("thing refs"), invalid states impossible by construction (n1)  {DECRETUM}
- [ ] A21 INDICES ARE THE KEY TO CONVERGENCE: double state (copying between pools), relocatable snapshots (FR-012, memcpy), diffs and snapshots all break with pointers (n1)  {PRINCIPIUM}
- [ ] A22 Note, not a proposal: materia's nodes (`loci*`, `pater*`) rest on pointers (n1)  {nulla:ratio > A21}
- [ ] A23 Fran: "too much" for a job, but sensible for interactive and realtime programs; and the "too much" version will give jobs unexpected gains too (fabrica itself is "recompute everything, cache on top", Alex) (n1)  {nulla:ratio}

## III. Anton, part II: the pool of things

- [ ] A24 ThingRef = {index, generation}: dereferencing gives the index, or 0 (nil) if the generation doesn't match, weak-pointer semantics without machinery; get = things[deref(ref)], with no `if` (n2)  {DECRETUM}
- [ ] A25 Guard nil: a setter ignores writes to nil (assert in debug; a trick: nil in read-only memory); refs can carry debug information (frame of creation) (n2)  {SUPELLEX}
- [ ] A26 An intrusive free list (first_free, next_free[]) in O(1); zero-is-initialization: find_empty = first_free (0 = nil = no slot); untouched slots prepopulated, or a high-water index (n2)  {SUPELLEX}
- [ ] A27 FIFO reuse spreads generations across the pool, so a 32-bit ref (~20 bits index, ~10 generation) suffices; 2^31 at 60 fps ≈ 414 days: ignore it, and say why (n2)  {SUPELLEX}
- [ ] A28 Iterate up to MAX, never to a count; or a function plus `void* user_data` (never silently slower; parallel application for stateless mutations), the house's seam shape (n2)  {DECRETUM}
- [ ] A29 Sorting or compaction breaks indices: Dreams builds a remap table (old → new) and rewrites every ref inside the pool, which is possible because the pool owns its refs; double state needs the same (n2)  {DECRETUM}
- [ ] A30 Refs outside the pool would falsely match after compaction: they must either not survive or go through one indirection, and a lint rule should enforce it (n2)  {PRINCIPIUM+intervention, split}
- [ ] A31 "Optimizable code": stay near optimal at no cost; getters and setters let SoA come later, invisibly (n2)  {REGULA}
- [ ] A32 Particles are not things: no stable ref; dense SoA, SIMD (n2)  {DECRETUM}
- [ ] A33 Anton types code by hand "so it gets into your bones", which is Fran's own practice (n2)  {REGULA}
- [ ] A34 The suggested FIRST STEP if this is explored: a LIBRARY FOR A POOL OF THINGS (true nil, generational refs, an intrusive FIFO free list, iteration by function): the foundation for actors, double state and FR-012 snapshots; its API = the ref type + nil semantics (Eskil) (n2)  {VISIO-OPERIS}
- [ ] A35 Friendly foundations already in the house: xar fills with zeros and never moves, piscina, seams through function tables (n2)  {OCCASIO}

## IV. The modular monolith

- [ ] A36 Microservice disciplines without the network: their cost (partial failure, latency, disagreeing clocks, deploy coordination, eventual consistency) comes from many machines, not from the disciplines (n3)  {PRINCIPIUM}
- [ ] A37 The disciplines that transfer: explicit contracts (nuntium.h schemas), correlation ids and tracing (the "cause" link of the runtime tree), the event log as truth (as the ledger already does), bounded queues and backpressure (fixed-capacity pools), bulkheads (a pool per actor), supervision and health (fabrica supervising jobs), idempotent writes (content-addressed, only if changed), per-servant metrics, location transparency (an actor can later move to a process or machine without code changes: an option, not a cost) (n3)  {DECRETUM}
- [ ] A38 Free in a monolith: one total order (the deterministic scheduler), unambiguous failures, cheap messages (refs into pools, not serialized bytes), debugging in one place (one snapshot) (n3)  {OCCASIO}
- [ ] A39 THE DANGER IS EROSION: physical separation enforces boundaries for free; in a monolith every boundary breaks with one shortcut (n3)  {PRINCIPIUM}
- [ ] A40 SILVA AS THE NETWORK'S SUBSTITUTE for enforcing boundaries: lint rules (no mutable globals, no pointers into another's pool, effects only through servants, messages of declared types), the same hardness at compile time, at no runtime cost (n3)  {DECRETUM}
- [ ] A41 The proper gain: parts with their own contracts and traces are intelligible ALONE, which is the CLAUDE.md documentation problem (30+ libraries, short sessions without context) (n3)  {VISIO-OPERIS}
- [ ] A42 Conway's law: our communication structure is Fran plus short sessions, so the parts must be intelligible on their own (n3)  {PRINCIPIUM}

## V. Two modularities

- [ ] A43 Libraries give modularity of IMPLEMENTATION: units of code that belong to no program ("what can be done"), divide and conquer over time, most useful (n4)  {nulla:ratio > A44}
- [ ] A44 But once a large program is built (briar, silex, vitrea, the residents), its libraries share one space and one call graph: state ownership is implicit, source boundaries vanish at run time, and the result is A MONOLITH; each program invents its own internal order (n4)  {PRINCIPIUM}
- [ ] A45 What is wanted is modularity of the INSTANCE (running parts: who owns state, who talks to whom, what happens when a part fails), which actors, servants and supervision give (OTP: an "application" plus a supervision tree) (n4)  {VISIO-OPERIS}
- [ ] A46 A program is a declared TOPOLOGY of running parts built from library code (actors, kinds and capabilities, joined servants, supervision), perhaps in STML, as aedificatio.stml declares how a program is built (n4)  {VISIO-OPERIS}
  - [ ] A46.1 static analysis of the running structure BEFORE the run: "which parts of briar can reach the network?" (n4)  {VISIO-OPERIS}
  - [ ] A46.2 the runtime tree gets a skeleton: static topology plus dynamic trace (n4)  {VISIO-OPERIS}
- [ ] A47 They reinforce each other: the rules that make actors safe (no mutable globals, IO through a handed-over servant, no hands in another's memory) are the rules of LIBRARY HYGIENE; one lint enforces both, and library discipline pays again over years (n4)  {OCCASIO}
- [ ] A48 Documentation: the program's structure becomes a readable, queryable document, like a library's header (n4)  {VISIO-OPERIS}

## VI. Vertical slices; four kinds of program

- [ ] A49 VERTICAL SLICES: code organised by FEATURE, not by layer; one feature's interface, logic, state, message types, tests and trace queries in one place (n5)  {DECRETUM}
- [ ] A50 The two modularities are TWO AXES: HORIZONTAL libraries (layers belonging to no program) and VERTICAL slices (units of one program's instance); a program = vertical slices built from horizontal libraries (n5)  {PRINCIPIUM}
- [ ] A51 In the actor model a slice is a group of actors + declared messages + views: the unit of declared topology, and what a short session can understand alone (n5)  {DECRETUM}
- [ ] A52 The house already leans this way: subsystems (briar/, silva/, tessera/ with their sources, tests, tools and aedificatio.stml); a briar .thistle is a vertical slice in one file (prose, C, html/css/js); vitrea (C plus an asset bundle) (n5)  {INDICIUM > A49}
- [ ] A53 What crosses between slices: only declared messages, never shared state (silva enforces) (n5)  {DECRETUM}
- [ ] A54 FOUR KINDS OF PROGRAM, distinguished by WHAT MOVES THE NEXT STEP: job, application, server (daemons too), simulator (n5)  {PRINCIPIUM}
  - [ ] A54.1 job: moved by input; runs to the end; termination and determinism; memory of the whole run (fabrica) (n5)  {nulla:ratio > A54}
  - [ ] A54.2 application: moved by human events; idle until something happens; human response time; undo/redo, session replay (n5)  {nulla:ratio > A54}
  - [ ] A54.3 server: moved by other programs' requests; runs long; supervision and isolation; a trace per request (n5)  {nulla:ratio > A54}
  - [ ] A54.4 simulator: moved by the clock; advances with no event; deadlines, determinism per frame; frame snapshots, running twice (Dreams) (n5)  {nulla:ratio > A54}
- [ ] A55 In the machine these are SCHEDULER POLICIES over the same step actors: run to the end / wake on event / on request / on clock tick (n5)  {DECRETUM}
- [ ] A56 Real programs are COMPOSITIONS: a game = simulator + application; silex = server + application; briar = job (building) + application (viewer); the MCP residents = servers whose requests are small jobs; fabrica a job, perhaps a watching server later (n5)  {nulla:ratio > A54}
- [ ] A57 The earlier kinds (generator, query, test, installer) are subdivisions WITHIN the job: two levels, where the kind says how it is driven and a bundle of capabilities says what it may touch (n5)  {DECRETUM}

## VII. Classes of state

- [ ] A58 A third axis: state classified by ORIGIN and LIFE, and the class DECIDES how the state is handled (n6)  {PRINCIPIUM}
- [ ] A59 A first sketch of the classes (n6)  {DECRETUM}
  - [ ] A59.1 persistent: written with authority (documents, the user's work, the ledger); event log as truth, copy-on-write versions, undo, backup; never derived (n6)  {DECRETUM}
  - [ ] A59.2 kinetic: animation, particles, physics in motion; recreated every tick, dense SoA, double state; never persistent, no undo; outside crash snapshots, or reconstructible (n6)  {DECRETUM}
  - [ ] A59.3 input log: what was observed (controller bundles, clock reads, network replies, the file-reading log); append-only and small; with persistent state it gives replay (n6)  {DECRETUM}
  - [ ] A59.4 derived: caches and projections (fabrica's memo, nexus, the ledger's .db, layouts); always deletable, keyed by the hash of its sources, never backed up, correct only if equal to its recomputation (n6)  {DECRETUM}
  - [ ] A59.5 content: immutable (audio, textures, sprites); written once, content-addressable, shared (n6)  {DECRETUM}
  - [ ] A59.6 session/view: the focused tab, scroll, selection; small, kept for a warm return ("the fourth tab", Alex), outside the document (n6)  {DECRETUM}
  - [ ] A59.7 run/coordination: mailboxes, locks, requests in flight; owned by the scheduler, never in persistent state, restored on restart (n6)  {DECRETUM}
  - [ ] A59.8 scratch: a pool per step, emptied after it (n6)  {DECRETUM}
  - [ ] A59.9 trace: the runtime tree, runs, tempora.tsv; append-only, with a retention policy rather than backup (n6)  {DECRETUM}
- [ ] A60 The gain: a declared class gives a pool its handling for free: serialization and backup, double state, undo, crash snapshots (persistent + input log, as in Dreams), verification (derived against recomputation, persistent against the log) (n6)  {VISIO-OPERIS}
- [ ] A61 SILVA CAN ENFORCE THE FLOW OF INFORMATION: persistent state only from persistent state and the input log (never from kinetic: animation doesn't leak into the document); derived state written only by its derivation; run state never in persistent state (n6)  {VISIO-OPERIS}
- [ ] A62 The house already does this informally: the ledger's .jsonl persistent and .db derived; build/ derived; GENERATUM = derived and committed (n6)  {INDICIUM > A58}
- [ ] A63 The classes mix by program kind: a simulator mostly kinetic, an application persistent + session, a server persistent + coordination, a job input + content → derived (n6)  {nulla:ratio > A58}
- [ ] A64 Uncertain: the boundaries (session vs persistent: is a document's selection one or the other?); some classes may merge; real use will decide (n6)  {nulla:quaestio}

## VIII. Session vs persistent

- [ ] A65 The class is decided by SCOPE; RETENTION is a separate dial (n7)  {DECRETUM}
- [ ] A66 Session state is bound to a session with a client (an app window, an MCP client, a tab, a connection); persistent state is shared across all sessions (the "database"); so a document's selection and scroll are session state (two sessions on one document have their own), and its content is persistent (n7)  {DECRETUM}
- [ ] A67 Session retention is chosen per application: discard at the end; keep while the session runs, without writing to disk; FREEZE and restore (scroll, focused tab); perhaps migrate to another machine (n7)  {DECRETUM}
- [ ] A68 Two dimensions: scope (step / session / shared) × durability (in memory / restorable / durable with a log) (n7)  {DECRETUM}
- [ ] A69 A session is an ENTITY with its own life (its own pool, its own index region), so freezing it is cheap when its state lives in its own index pool (memcpy; FR-012 again) (n7)  {VISIO-OPERIS}
- [ ] A70 Related: the park "Sessio et actor ut entia" (2026-07): the ledger resident must never hold session state ("parametric everywhere"), the same distinction found from the server side (n7)  {INDICIUM > A66}

## IX. Identity follows scope

- [ ] A71 IDENTITY FOLLOWS SCOPE (n8)  {PRINCIPIUM}
  - [ ] A71.1 persistent/shared → a public id: unique, stable, never reused, linkable from outside (a ULID, like the ledger's res_id) (n8)  {DECRETUM}
  - [ ] A71.2 content → the id is the hash of the content: same content, same id everywhere (n8)  {DECRETUM}
  - [ ] A71.3 derived → a key computed from the hashes of its sources (fabrica's memo), never assigned (n8)  {DECRETUM}
  - [ ] A71.4 session → LOCAL handles (a pool ref: index + generation, 32 bits): cheap, meaningless outside the session, reusable because generations protect them (n8)  {DECRETUM}
  - [ ] A71.5 kinetic → often none, or an index into a table ("particles are not things") (n8)  {DECRETUM}
- [ ] A72 At the boundary, whatever passes from a session into the shared world (saving, a message to another session or server, a written link) must translate local handles into public ids; the session keeps a table from public id to local handle, like internamentum (n8)  {DECRETUM}
  - [ ] A72.1 precedents: a file descriptor (per process) vs the file's global identity; Erlang's local pid vs registered names; rowid vs UUID (n8)  {nulla:exemplum > A72}
- [ ] A73 A silva rule of information flow: a local handle is never persisted nor sent outside its session (the same fault as a pointer written to a file) (n8)  {DECRETUM}
- [ ] A74 A frozen session carries its handles inside its frozen pool, so they stay consistent and are restored together (n8)  {nulla:ratio > A69}
- [ ] A75 The house already has the public side (ULIDs with a six-letter human suffix, fabrica's hashes, aedilis's paths); the local side is what the pool-of-things library would add (n8)  {OCCASIO}

## X. Since then (my commentary, not in the source)

- This is the densest vision source so far and the most recent: one day, from 07:21 to 14:43, while the vision documents from project-specs span July to September.
- Lineage with earlier sources: A39-A40 (silva as the network's substitute against erosion) is tabularium's T1 and T2 one level up, from tools to the parts of one program; A71 (identity follows scope) generalises T11 and S31 (identity is the decision that outlives everything); A41-A42 name the documentation problem the book itself answers; A4 is the "testing and observation" domain made whole.
- A21 (indices are the key to convergence) cuts against the house: materia, silva's trees and most libraries hold pointers (A22). The source says so plainly and makes no proposal.
- A1 reopens a recorded decision: the ledger's decretum "if parallelism ever comes, it will be of PROCESSES, not threads" (…XBK68Y), and the house memory "NO threads by design". A7.4 imagines threads with nothing shared.
