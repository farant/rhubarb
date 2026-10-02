# fabrica — spec 1b (the model, then the executor)

> Written 2026-09-30 after slice 1a closed (spec v2 §VII "As built").
> Revised the same day after talking it through with Fran: instead of
> adding special cases (a new action kind here, a new provenance there),
> 1b first states the **vocabulary** fabrica is built from — artifact
> types defined by an interface of verbs, composable — and the executor
> is built on it. Builds on spec v1 §II (model), §VII (executor and
> envelope), §IX (parallelism), decisions Q10, Q26, Q32. Measured from
> the ledger inventory **'producentes (fabrica)'** (76 rows; six lenses
> added 2026-09-30). Questions for Fran: §VIII (Q38–Q47). Nothing here
> is decided until Fran answers.

## 0. What the measurement found

1. **The judge has a plan; nobody runs it.** `bin/fabrica iudicare`
   prints SANATIO (ordered commands) for 64 declared actions / 145
   artifacts. Today a human copies the lines. After the quadrans merge
   that was 15 installers + one generator, by hand, in order.
2. **Eight producers are launchers, not build steps.** apps/forum,
   villa, pictor (`exec bin/X`: a window), tessera/effigies.sh (TUI),
   legatus, tabularium, tabulariumd, fori (residents/daemons). An
   executor that ran them would open windows or start servers. Their
   build part is the aedilis-generated `build/aedilis/<t>/struere.sh`.
   None of the eight is declared in fabrica today.
3. **Undeclared preconditions — the blind link.** silex, stml, briar,
   briar-spectator and compile_tools refuse to run (or link blind)
   unless the ROOT test runner's `build/*.o` exist. No declaration says
   so. `./compile_tests.sh --libs-only` builds them (228 objects, 6.6 s
   warm) — but `--libs-only` is not a flag: it is a test filter that
   matches nothing, so the runner builds the libraries and exits **2**
   ("nothing ran"); compile_tools relies on that and ignores the code.
4. **Producers call producers.** briar_struere runs four capsule
   scripts and corpus_infixum; natura_canones.sh calls
   canon_coquere.sh; every snippet generator rebuilds aedilis if it is
   missing; installers rebuild bin/fabrica if missing. The healing plan
   lists only the outer command; the inner ones are invisible edges.
5. **Writes outside declared outputs.** `~/.bin/{briar,
   briar-spectator,silex,stml}` (outside the tree);
   `mensor_ui_struere.sh` regenerates a COMMITTED file
   (apps/mensor/assets/capsula_mensor.{c,h}) as a side effect;
   `compile_tests.sh --clean` deletes `bin/*` — every installed binary.
6. **Exit contract: one masked failure class.** All link lines
   propagate failure (`-o bin/X || exit 1`). But the four `~/.bin`
   installers end with `cp bin/X ~/.bin/X` then
   `echo "institutum: …"` — a failed copy exits **0** and says
   "installed".
7. **Reproducibility splits cleanly.** Every generator (21) is
   byte-reproducible (proven by 1a's regeneration verdicts). No linked
   binary is (LC_UUID + code signature, measured T6). The executor
   therefore verifies an install by RE-JUDGING it (relatio) — never by
   comparing bytes.
8. **Conflict sets are known but undeclared.** Directories producers
   EMPTY: each snippet's `build/fabrica/clausurae/<s>/`, each amalgam
   statio, per-process temp dirs (safe). Two judges running at once
   already destroyed each other's clausurae (T6) — the lock exists for
   the judge; producers have none.
9. **Timings are mostly unknown** (41 of 70 cells blank on 09-29).
   Known: installs ~10 min total for 15 binaries (briar dominant),
   `-plenus` 47 s with records / ~165 s cold, silva snippets 36 s,
   compile_tests snippet ~60 s, root objects 6.6 s warm / ~150 s cold.

## I. Framing: a control plane for derived state

fabrica is the repository's **control plane for derived state**:
declared **desired state** (every artifact derived from today's
sources), **observed** state (`iudicare`), and a **reconcile** loop
(`sanare`, this slice). The scripts, compilers and generators are the
**data plane** — they do the work; fabrica decides what work, in what
order, and checks the outcome.

The boundary is explicit: **sources, commits and the ledger are
human-owned.** fabrica observes and reconciles what is DERIVED from
them — files, binaries, and later verdicts and running processes. That
is why `sanare` never commits (Q43 follows from the frame).

## II. The vocabulary

### II.1 Two verbs

- **quaerere** (query): read-only observation — `iudicare` today,
  slice 4's `quaere` later. Cheap, no side effects, safe at any time
  (the session hook).
- **agere** (act): run an action. Takes time, writes its footprint, its
  result is recorded.

"Verify" is not a third verb: running a gate is **act**, whose output
is a **verdict**. "Operate" (starting a resident, opening a window) is
**act**, whose output is a **process**. The variety lives in the
artifact types, not in the verbs.

### II.2 Artifact types × currency strategies

Two interfaces, deliberately separate (revised after review: judging
currency needs the PRODUCING ACTION — regeneration means "rerun my
producer" — so it cannot be a property of the type alone; and inputs are
only ever digested, never judged):

**An artifact type says how to OBSERVE** something:

| verb | question | used by |
|---|---|---|
| **enumerare** | what concrete things are you made of? (paths, a pid…) | commit check (`-tacta`), footprint, orphans |
| **sigillare** | your current state, as a digest | input keys, records |
| **locare** | where do you live — what does writing you mean | footprint check, parallelism |

**A currency strategy says how to know an OUTPUT is current**, given its
type and its producing action: `iudicare(artifact, action, key) →
RECENS / STALUM / IGNOTUM + reason`. The strategies are few and closed:

| strategy | how | valid for |
|---|---|---|
| **regeneratio** | rerun the producer in scratch, compare bytes | reproducible types (files) |
| **relatio** | the artifact reports the key it was made from | binaries (`-provenientia`), later processes |
| **memoria** | a record keyed by input digest + argv (a shortcut in front of another strategy) | any output whose inputs are provably complete |
| **ignota** | cannot be known — realized before dependants, never judged (§II.3) | the root object store (Q38) |

The TYPE constrains which strategies are valid (a linked binary is not
byte-reproducible, so never regeneratio — measured T6). An output is a
(type, strategy) pair; an input needs only its type. This is still ONE
dispatch point per interface — no growing enums — but it no longer
claims inputs and provenances are "one thing".

C89: two structs of function pointers (the `FabricaSutura` / aedilis
extractor pattern), registries by name; declarations name the type
(`genus="…"`) and, on outputs, the strategy (`provenientia="…"`, as
today); the core never switches on a kind.

### II.3 Base types, combinators, composites — an algebra

- **Base types** (few): file bytes · directory listing · binary ·
  recorded verdict · (later) running process.
- **Combinators** build types from types: *listed by* (a manifest's
  files), *contained in, filtered* (a directory's `.c` files), *named by*
  (a configuration's include roots). A combinator IS a query — the
  interview's "an input is a live query" (Q1), and the general form of
  slice 4's query idiom.
- **Composites**: an artifact made of other artifacts. Its verbs
  delegate: enumerate = union; digest = Merkle (sorted part name +
  part digest — exactly 1a's input-set digest); locate = union;
  **judge = the worst of its parts** on the order
  RECENS < IGNOTUM < STALUM, naming the parts responsible. "Worst of" is
  associative, so composites of composites work at any depth.
  - a **selection** composite finds its parts by a query (a manifest's
    files) — an input type;
  - an **aggregate** composite lists parts produced by DIFFERENT actions
    ("briar" = binary + `~/.bin` copy + capsules; a subsystem; the
    repo). No single action produces it; `iudicare briar` gives one
    verdict, `sanare briar` heals whichever parts are stale.
- **Preconditions are not parts.** An artifact whose strategy is
  *ignota* (the root object store) would make every composite containing
  it permanently IGNOTUM — "worst of" with a part that is never known.
  So such artifacts are **preconditions**: an edge says "realize this
  before me", the executor realizes it, and it never enters a
  composite's verdict.

**The Canon boundary:** combinators are implemented **in C**, as new
named types; aggregates are declared as **flat lists of named parts**
(like `fabrica.stml` lists subsystems). Declarations never contain
expressions — the decree holds. Membership is acyclic (the existing
cycle check covers it).

### II.4 Actions: one output type each

An action is: inputs (artifacts) → outputs (artifacts) via a command.
**Rule: one action, one output type, one footprint.** If an action's
outputs need two different ways of being judged, it is two actions.
Same-type multi-output actions stay one (silva's 22 snippets).
Decomposition is the dual of composition: **actions decompose** so each
piece is judged, cached and timed on its own; **artifacts compose** so
humans still name "briar". Action *kinds* (generator / formatio /
institutio) become documentation — what matters is the output type.

### II.5 Footprint

An action's **footprint** has three categories:

| category | example | parallel-safe? |
|---|---|---|
| **owned outputs** | what its outputs `locare` | yes, if disjoint |
| **owned workspace** | scratch, staging, emptied dirs (declared) | yes, if disjoint |
| **shared idempotent cache** | `build/aedilis/obiecta/` — many actions write the SAME `lib__chorda.o`, benignly | **only once content-addressed** (slice 2); until then, actions touching it run serially |

**Rule: an action writes only inside its footprint.** The envelope checks it: snapshot the
tree and `bin/` before and after (the technique that caught T6's
`bin/aedilis` relink). One mechanism for a whole class of past bugs:
T6's generator relinking `bin/aedilis`, `mensor_ui` regenerating a
committed capsule, `compile_tests --clean` deleting `bin/*`, two judges
emptying each other's directories. Two actions may run in parallel iff
their owned footprints are disjoint AND neither touches an
un-content-addressed shared cache (Q10, made concrete — and the reason
1b's parallelism depends on slice 2 more than the first draft admitted).

### II.6 Edges and the envelope

- **One kind of edge:** an action's input is another action's output.
  "Nested producers" and "preconditions" (§0.3, §0.4) are **undeclared
  edges** — declaration gaps, not categories.
- **The envelope** is identical around every action: start/end and
  duration · log in `build/fabrica/acta/<titulus>.log` (last lines
  echoed on failure) · exit contract · deadline · lock · **post-condition
  = the outputs' `iudicare` must say RECENS** (exit 0 without a current
  output is a FAILURE — this catches §0.6's masked copy) · **footprint
  check** · record in `cursus`.

### II.7 Known limits (stated, not hidden)

1. **Writes are checked; reads are not.** The model is only as correct
   as the declared inputs, and nothing enforces them — Bazel and
   Concourse get this from sandboxing; we trade enforcement for
   simplicity. Compensations: *memoria* only for provably complete
   inputs; oracle gates cross-check. A possible later ORACLE (not
   enforcement): audit an action's reads with macOS `fs_usage` and flag
   undeclared ones.
2. **The interface is file-shaped.** It was derived from files and
   binaries. *Processes* need a lifecycle (start, stop, restart, health)
   and do not terminate — "operate" will likely need more verbs.
   *Verdicts* can be nondeterministic — a flaky test cached by its input
   key hides the flake; slice 3 needs a policy (at least: never cache a
   failure). Both are untested against the interface.
3. **A new type is still a code change.** By design (combinators live in
   C to keep the Canon decree): the model removes the DISPATCH change,
   not the need to write and compile the type.
4. **Declaration volume grows** with decomposition (briar: 1 → 5
   actions; 64 hand-written today). Declarations will eventually want to
   be generated or queried (slice 4).

## III. Today's special cases, as instances

| today | in the vocabulary |
|---|---|
| input kinds fasciculus, configuratio, instrumentum | base type *file bytes* |
| directorium | base type *directory listing* |
| manifestum | combinator *listed by* over file bytes |
| plagulae | combinator *contained in, filtered* |
| manifesta | composite of manifests in a directory (combinator over combinator) |
| radices | combinator *named by* (configuration → directories) |
| provenance regeneratio | the *regeneratio* strategy, valid for reproducible types |
| provenance relatio | the *relatio* strategy, valid for binaries |
| memorabilis records | the *memoria* strategy, in front of another strategy, only for provably complete inputs |
| 1b draft's "copia" (`~/.bin` copy) | not new: a file, judged by regeneration (its generator is `cp`) — the installer decomposes (link → binary; copy → file) |
| 1b draft's "praeparatio" (root objects) | not a kind: strategy *ignota*, used as a PRECONDITION, never a composite part (Q38) |
| launchers (§0.2) | two units welded: a build action (binary) + an operate action (process, later) |
| briar_struere | five units welded: corpus capsule + three capsules (files) + link (binary) + copy (file) |
| mensor_ui's side effect | dissolves: capsula_mensor is already its own action; the installer just links |
| natura_canones → canon_coquere | two actions with a declared edge (already declared in fabrica) — the nested call is redundant |

## IV. What 1b lands, in order

1. **The interface, behaviour-preserving.** Refactor `lib/fabrica.c`'s
   switches on kind into the two interfaces and their registries;
   today's eight input kinds become types and combinators, the two
   provenances (plus memoria) become strategies. Oracle: the 166 core assertions, the 14 smoke stages,
   the oracle gate, and **all 145 verdicts unchanged** under `-plenus`.
   If one existing kind does not fit the four verbs cleanly, the
   interface is wrong — learned before anything is built on it.
2. **Aggregate composites** in the declarations (flat lists) and in
   `iudicare` (worst-of). Cheap — judge-level only — and they give
   `sanare` its targets ("briar", a subsystem).
3. **`sanare`** (Q45): judge, then run the actions of every stale or
   unknown artifact in dependency order inside the envelope; re-judge;
   report healed / failed / skipped-because-a-dependency-failed. Serial
   (Q41); lock shared with `-plenus`; `-siccum` dry run with estimated
   times from `cursus`. Generators run IN PLACE; never commits.
4. **Footprints** declared on actions that own workspace; the envelope's
   footprint check.
5. **Decompositions** (Q40, Q42): installers split into link + `~/.bin`
   copy; briar into its five units. Each new unit is a declared action
   of one output type.
6. **Records:** `cursus` (titulus, initium, duratio_ms, exitus,
   verdictum post, causa) — the timing lens fills itself.

## V. Fixes from producers

1. The four `~/.bin` installers: `cp … || exit 1` (§0.6) — and after
   the decomposition the copy is its own judged action anyway.
2. `compile_tests.sh`: a real objects-only mode (exit 0) instead of the
   `--libs-only` filter that matches nothing (§0.3).
3. `mensor_ui_struere.sh`: stop regenerating the committed capsula.

## VI. Oracle and gates

- **The refactor's oracle** (IV.1): identical verdicts, before/after.
- **sanare's oracle:** the scripts. Plant a source edit → STALUM →
  `sanare` → RECENS, `-provenientia` digest == `digestum`, same verdict
  as running the installer by hand (relatio equality — bytes differ by
  design, §0.7).
- **fabrica-fumus grows** (XV+): healed; failed-with-exit-0-but-not-
  RECENS (post-condition); write outside the footprint caught;
  dependency-failed-so-skipped; lock refusal; a composite's worst-of
  verdict naming the stale part.
- Every new stage born red by a plant that compiles.

## VII. Numbers 1b is measured against

| today | target after 1b |
|---|---|
| post-merge healing: 15 installers + 1 generator copied by hand, in order | `sanare`, one command, same verdicts |
| input kinds and provenances: two growing enums (8 + 2), each new one a core change | one interface; new types are instances |
| install durations unknown (41/70 blank) | `cursus` records every run |
| masked `~/.bin` copy failure exits 0 | FAILED (post-condition) |
| footprint violations found by accident (T6, mensor_ui) | caught by the envelope |

## VIII. AUDIENDA — questions for Fran

**Q38. The blind link (silex, stml, briar, spectator need root
`build/*.o`).** In the vocabulary: the root object store has strategy
*ignota* (mtime-built, no digests) and is a PRECONDITION of those four,
never a composite part. (a) Declare it as such — `sanare` realizes it
whenever a dependant runs (6.6 s warm, ~150 s cold); slice 2 replaces it
with digest-keyed objects. (b) Leave
those four to manual healing until slice 2. **Recommendation: (a).**

**Q39. Launchers.** Decompose (build action now, operate action
later), or leave them out of 1b entirely? **Recommendation: out of 1b**
— none is declared today; their build part joins when the operate verb
does.

**Q40. Decomposing briar's installer** (five units) in 1b, or opaque
for now? **Recommendation: decompose in 1b** — it is the slowest
installer, the one healed most often ("rebake after lib/"), and its
capsules are reproducible files that could be skipped when unchanged.

**Q41. Parallelism.** Serial in 1b with footprints declared and
`-siccum` showing what WOULD run together; parallel once footprints are
proven by the envelope's check. **Recommendation: serial first.**

**Q42. `~/.bin` copies** as their own judged file actions (decomposed
installers)? **Recommendation: yes** — the session hook then sees a
`~/.bin/briar` lagging `bin/briar`.

**Q43. Committed generated files.** `sanare` writes in place, never
commits? **Recommendation: yes** (follows from §I).

**Q44. mensor_ui** stops regenerating its committed capsula?
**Recommendation: yes** (§III: the side effect dissolves).

**Q45. Names.** The type interface's three verbs (placeholders:
enumerare, sigillare, locare), the strategy verb (iudicare) and the new
strategies (memoria, ignota), the precondition edge, the composite
element in declarations, the command (`sanare`), the records table
(`cursus`), the log directory (`build/fabrica/acta/`).

**Q46. Aggregate composites in 1b** (judge-level, cheap), or later?
**Recommendation: in 1b** — they give `sanare` human-sized targets and
make the worst-of rule visible early.

**Q47. Where types live.** All base types implemented over the seam in
`lib/fabrica.c` (pure, testable on fixtures), with the tool supplying
only the seam — or types registrable from the tool (needed when a type
must touch sqlite or processes)? **Recommendation: types in lib over
the seam** (the blind-link rule forbids lib referencing sqlite); a
type needing more gets a new seam member, as `meminisse` did.

## IX. Decisions (Fran, 2026-09-30)

All recommendations accepted; the Q45 placeholders are the names.

- **Q38 → (a)**: the root object store is declared with strategy
  *ignota* as a PRECONDITION of silex, stml, briar, briar-spectator;
  `sanare` realizes it before them; slice 2 replaces it.
- **Q39 → out of 1b**: launchers stay undeclared until the operate verb.
- **Q40 → decompose briar in 1b** (corpus capsule, three capsules,
  link, `~/.bin` copy).
- **Q41 → serial**: footprints declared, `-siccum` shows what could run
  together; parallel after the footprint check has proven itself (and
  near the shared cache, after slice 2).
- **Q42 → split installers** into link (binary, relatio) + `~/.bin` copy
  (file, regeneratio); the `|| exit 1` fix too.
- **Q43 → `sanare` never commits.**
- **Q44 → mensor_ui stops regenerating its committed capsula.**
- **Q45 → names**: type verbs `enumerare`, `sigillare`, `locare`;
  strategy verb `iudicare`; strategies `regeneratio`, `relatio`,
  `memoria`, `ignota`; command `sanare`; records table `cursus`; logs
  `build/fabrica/acta/`. (The precondition edge and the composite
  element are named in the plan's declaration task, as T2 did in 1a.)
- **Q46 → aggregate composites in 1b.**
- **Q47 → types in lib over the seam**; a type needing more gets a new
  seam member (as `meminisse` did).

## X. As built (2026-10-01)

Plan `fabrica-plan-1b.md`, executed inline in one day; commits
78804873 (T1), 8f813101 (T2), 38bcf610 (T3), ba244fac (T4), d8327b36 /
4da395a9 / 507f0ace / b4a77df0 (T5 A1/A2/B1/B2), e2d39037 (T6), T7 below.

| §VII target | after 1b |
|---|---|
| post-merge healing: 15 installers + 1 generator copied by hand | `bin/fabrica sanare [composite]`: judge, run in dependency order, re-judge; preconditions realized once; logs in build/fabrica/acta/ |
| two growing enums (8 + 2) | two registries (types: sigillare/enumerare/locare; strategies: regeneratio, relatio, ignota; memoria in front of regeneratio) - no switch on a kind |
| install durations unknown (41/70 blank) | `cursus` table records every action; `-siccum` estimates; timing lens filled (warm measurement of all 70 actions) |
| masked ~/.bin copy failure exits 0 | copies are actions (`institutio_X`, `tools/instituere.sh`, `|| exit 1`): FRACTUM by exit code; exit 0 but output not current = FRACTUM by post-condition |
| footprint violations found by accident | envelope snapshot check: 9 defect classes found and fixed; done check 70/70 actions, 0 writes outside a footprint (warm) |

Beyond the spec (found by measuring, decided with Fran): ordering edges
from `enumerare` (manifest closures were invisible to ordering); the
one bootstrap cycle (aedilis -> amalgam -> fontes -> aedilis) broken by
`instrumentum` enumerating nothing; FABRICA_AGIT (scripts skip nested
producers under the executor, keep conveniences by hand); families
canon (2) and natura (4) as one action each, the whole provenance
directory excluded from digests; `silva_latina_datum` and the spliced
tables of silva.h/hospes.c were undeclared GENERATED outputs, also
written by the judge in place - now declared, generated in scratch;
mechanism and object stores as `ignota` preconditions with real
`-obiecta`/`--obiecta` modes; `celer` (cheap regeneration under the
quick judge, so the session hook sees ~/.bin lag); the corpus stamp was
FUNCTIONAL (briar cache key) - now a content sigillum, reproducible;
shadow root for root-glob capsules; the snapshot no longer follows
symlinks. 166 -> 344 core assertions; fumus 14 -> 25 stages.

Known limits after 1b: the judge, not the actions, now dominates time
(`sanare briar`: 7 s of actions, 2-3 min total - the amalgam chain is
re-verified by regeneration, not memorabilis); writes are checked only
warm (cold paths measured piecemeal); reads are not checked;
directory-listing edges cascade OMISSUM widely; serial execution
(waves only shown). Next: slice 2 (content-addressed objects) and the
desideratum on granular actions (ledger ...V2H6Q).

**Addendum 2026-10-02 (park …AR15, closed).** The first limit above is
resolved, and it had two causes, not one. (1) The amalgam chain is now
memorabilis: fontes_X and excludenda_X write the closure manifests their
own keys read (`build/fabrica/clausurae/X__amalgama`, `X__mechanismus`),
amalgama_X reads both; `radices` digest include-root listings but no
longer make ordering edges (they made fontes_silva wait on
amalgama_silva). (2) Most of the missing wall time was not the judge at
all: the footprint snapshot was sorted with `xar_ordinare`, then an
O(n^2) selection sort - ~8 s per snapshot, two per action; now a stable
merge sort. No-op `sanare -siccum installata` 41.5 -> 18.5 s; `-plenus`
74.6 -> 22.3 s; a rebake's unrecorded time ~200 s -> ~17 s. The judge's
own regenerations are recorded in `cursus` (IUDICIUM), and gate timings
in `build/portae/tempora.tsv`. Still open: cold footprints (…6X0);
lectores_cocti and tabulae_silvae not memorabilis; the snippet
generators run aedilis once per file (fragmentum_compile_tests ~59 s per
miss) - batching and a content-digest cache of aedilis results belong
to slice 2.
