# fabrica — spec 2 (reads observed, objects shared)

*Born 2026-10-02 from the slice-2 interview
(`project-specs/fabrica-interview-2.md`, Q1–Q24). Predecessors:
`fabrica-spec-v1.md` (§XI slices), `fabrica-spec-v2.md` (1a as built),
`fabrica-spec-1b.md` (model + executor, §X as built + addendum).
Question that prompted it: the build now knows what is stale and can
heal it — but its inputs are hand-proved, its object stores are run
blind, and every gate and runner is serial because builds share
object paths. Spec v1 named slice 2 "test runners on the fabrica; then
the shared object store". This spec keeps that destination and puts a
read ledger under it.*

*Status: v2 (2026-10-02) - interview spec plus the inline codebase
pass in §XII, which corrects §II and §VI in three places (marked
"v2:").*

## 0. What the measurements say (dated 2026-10-02)

- Object stores are `ignota` preconditions realized blind:
  `obiecta_radicis` up to 23.9 s, `obiecta_briar` up to 17.1 s after a
  header change (`cursus`, PRAEPARATUM rows). 1,737 objects in 15
  `build/` dirs, 5.5 GB; `piscina.o` compiled 24 times (spec v1 §0).
- The aedilis gate (`tools/aedilis_porta.sh`, 199 tests) is serial:
  ~205 s after today's `--memoria-oraculi` (was 274 s). It stays
  serial because builds write shared object paths.
- Hand-proved inputs are the weak point: the amalgam chain became
  memorabilis only after a soundness argument ("a manifest dir is
  written by an action whose own key reads it", 7d503807); three
  generators that rewrote unchanged output silently caused relinks and
  ~100 s of memo misses (a7a7d6f2).
- aedilis re-derives the same headers in every process: `--corpus`
  over silva's 25 roots 2.1 s vs 13.5 s per file; snippet generators
  pay 41–59 s per memo miss.

## I. Framing

Slice 2 makes two things true:
1. **Reads are observed, not proved.** A house tool reports what it
   actually read (files, failed lookups, directory listings, its own
   writes, environment). That record *is* the action's key next time
   (verifying traces). clang, which is not ours, reports through its
   depfile; shadowing is covered by digesting include-root listings.
2. **Derived bytes have one home.** Objects (and aedilis extraction
   records) live in a content-addressed store keyed by everything that
   determines them. Two builds needing the same object share one blob;
   two jobs can run at once because no two jobs write the same path.

Everything else in this spec is how those two land without breaking
the slice-1 guarantees (honest IGNOTUM, oracle agreement, plants that
compile).

Out of scope (vision, later slices): program kinds as capability
bundles, jobs returning effect plans, pulled inputs inside a job, the
full filum migration, tests as fabrica actions (slice 3).

## II. The read ledger (`filum` liber lectionum)

### II.1 Contract
- Activated per process by an environment variable naming a file
  (working name `FABRICA_LECTIONES`; AUDIENDA A1). Unset = no cost, no
  behaviour change.
- Every `filum` entry point appends one line per event, append-only,
  safe under concurrent children (one `write` per line, O_APPEND):
  - `L via` — file read (whole or streamed)
  - `A via` — lookup that found nothing (negative dependency)
  - `X via` — existence confirmed without reading the content
    (v2: deleting the file changes the answer, so it is a dependency
    too; `via_existit` is how aedilis resolves includes)
  - `D via` — directory listed (its name set is an input)
  - `S via` — file written (the write footprint)
  - `E nomen valor` — environment variable read, through a new
    recorded wrapper (pilot path only in this slice; Q11)
- Paths, not digests: fabrica digests afterwards. A file changing
  while the action runs is caught by the existing write/footprint
  check.

### II.2 Use (Q9: replace declarations)
- After a successful run, fabrica stores the action's read set with a
  digest of each entry (content for `L`, absence for `A`, name list
  for `D`) in its record. Next judgement: if every entry still digests
  the same, the action is RECENS without re-running; any difference →
  regenerate.
- Declarations shrink to what can't be observed: argv, the binary of a
  non-house tool, and actions that call external programs.
- First run (no record) → IGNOTUM, as today.
- A crashed or failed run never leaves a trusted ledger.

### II.3 Scope of filum migration (Q10)
Only the pilot path: aedilis and the libraries/tools fabrica runs for
the toml pilot. A silva lint lists every other direct `fopen` /
`opendir` / `stat` as a WARNING (54 + 21 files today); full migration
is a later tabularium batch job.

### II.4 Spike first (Q18)
Before any of II.2 is built: hook the read, exists and list calls in
`filum`, run `aedilis` on one silva root, compare its ledger with the
closure manifest it writes. Pass = ledger ⊇ manifest, and every extra
read (`aedilis.stml`, `latina.h`, failed lookups) named and justified.
One unexplained gap = the design isn't ready.

### II.5 Memo audit (Q20)
A sampled fraction of memo hits (and every hit under an explicit flag)
regenerates anyway and compares. A disagreement is loud and is
recorded in `cursus`. This measures the trust the ledger grants
instead of assuming it.

## III. The store (`obiecta`)

### III.1 Shape (Q16, Q7)
Per tree, under `build/aedilis/obiecta/`:
- **CAS:** blobs named by the SHA-256 of their bytes.
- **Action cache:** key → output digest(s). Small and rebuildable.

### III.2 Object key (Q2, Q5, Q6)
SHA-256 over: source path and bytes; every header in clang's depfile
for the previous build of that key (path + bytes); the include-root
listing digest (shadowing); the exact flag list; the clang binary's
digest (hashed once per run). Paths stay tree-relative so a shared
store later is a move, not a redesign (Q7 note).

### III.3 Writes and reads
- Written atomically (temp name + rename); two jobs producing the same
  blob is benign (identical bytes, last rename wins).
- Reads verified by sampling: every Nth read, every read under
  `iudicare -plenus`, and `bin/fabrica purgare -verificare` checks all
  (Q19).
- A failed compile is never cached.

### III.4 Eviction (Q8)
`bin/fabrica purgare`: keep every blob referenced by the latest
records plus N recent generations; delete the rest. Never automatic.

### III.5 aedilis extraction records (Q21)
Second blob kind: a file's extraction record (directives, annotations,
angle includes) keyed by its bytes + aedilis configuration. First
non-object user of the CAS; speeds closure derivation and the snippet
generators.

## IV. Actions: compilatio, nexus, familia

- `compilatio` — one source → one object, resolved through the store
  (hit = no clang). Not declared per object (Q2): coarse actions
  instantiate compilations internally.
- `nexus` — objects + link line → binary (judged by relatio, as
  installed binaries are; binaries are not byte-reproducible).
- `familia` — a declaration naming a directory and a pattern
  (`praefixum`, `suffixum`), from which fabrica instantiates one
  action per matching file (Q17). Pure data; no expression language.

## V. The toml pilot (Q4, Q12, Q13)

- `toml/compile_probationes.sh` becomes thin: it asks fabrica for the
  13 test binaries (built through the store), then runs and reports
  exactly as today.
- Oracle against the old runner: every `.o` byte for byte; binaries by
  identical object set + link line; test runs by verdicts and output
  lines. Agreement across real commits, not one run.

## VI. Parallel execution (Q3, Q14, Q22)

- After the store exists: the executor runs independent actions
  through the async `processus` path, default fan-out = performance
  cores (`sysctl hw.perflevel0.physicalcpu`), `FABRICA_FILA=N`
  override. Records are written in deterministic order.
- On a failure: running actions finish (and are cached), nothing new
  starts, dependents OMISSUM, the failure is reported first.

## VII. excubitor (Q15)
Run beside the store for the toml pilot as an oracle; when they agree
on real commits, delete excubitor and `fabrica.tsv` (Q35 closed).

## VIII. Order (Q23)
1. Ledger spike (II.4).
2. Ledger in `filum` + verifying traces + memo audit (II.1–II.3, II.5).
   v2: `S` (write) events are part of this step, not optional - they
   are what lets step 6 attribute writes to actions (§XII.3).
3. CAS + action cache, aedilis records as first user (III).
4. `compilatio` / `nexus` / `familia` (IV).
5. toml pilot, thin runner + oracle (V).
6. Parallel executor (VI).
7. excubitor oracle, then delete (VII).

## IX. Done means (Q24)
- toml runner on fabrica; oracle agrees across real commits.
- The ledger keys at least one real action family, with plants for
  the shadowing and new-file cases.
- Measured speedups recorded: cold/warm toml build, aedilis gate,
  snippet generators (before/after in `tempora.tsv` / `cursus`).
- excubitor deleted.

## X. Review focus (failure modes no step above tests by default)
1. **A read that bypasses `filum`** (a raw `fopen` left in a pilot-path
   library) → a stale memo hit. The lint must be clean on the pilot
   path before II.2 trusts any ledger from it; a plant adds a raw read.
2. **A new header shadowing an old one** on the `-I` path → clang's
   depfile can't see it; the include-root digest must. Plant: add a
   same-named header earlier on the path.
3. **Concurrent writers of one blob or one ledger** → corrupt store or
   interleaved lines. Plant: two jobs producing the same key at once;
   ledger lines from parallel children must stay whole.
4. **Compiler upgrade** → the clang digest must change every key.
   Plant: a wrapper `clang` with different bytes.
5. **Absolute paths in debug info** (`-g`) make objects tree-specific;
   per-tree store makes this harmless now, but the oracle must compare
   objects built in the same tree.

## XI. AUDIENDA — open, for Fran
- A5. (v2) `-fdebug-prefix-map=$PWD=.` in the house flags: objects
  become identical across worktrees (a later shared store works
  unchanged), at the cost of relative paths in debug info (lldb needs
  a source-map setting or must run from the tree root). Alternative:
  the working directory joins the object key.
- A1. Names: ledger variable (`FABRICA_LECTIONES`?), the CAS directory
  layout, kind names `compilatio` / `nexus` / `familia`, the GC verb
  (`purgare`?).
- A2. Memo-audit sampling rate, and blob-verify N.
- A3. How many generations GC keeps.
- A4. Whether `E` (environment) lines should already cover tools
  outside the pilot path.
- Not verified in this spec (to be checked in the v2 codebase pass):
  how `filum` is structured internally for a single hook point; how
  `processus` async interacts with the executor loop; how
  `toml/compile_probationes.sh` builds today; whether clang objects
  here are byte-deterministic with our flags.

## XII. Codebase pass (v2, 2026-10-02)

Read inline; every claim has a file:line or a command.

### XII.1 Where the ledger must hook (corrects "every filum entry point")
aedilis reads through `filum` almost everywhere (`tools/aedilis.c:444`
sources, `lib/aedilis.c:212` configuration), and the silva amalgam
does no file IO of its own (no `fopen`/`opendir`/`stat` in
`silva/amalgama/silva.c`). The exceptions are exactly the reads that
matter most:
- `lib/via.c:502` `via_existit` - a raw `stat`, used by
  `lib/aedilis.c:95` for include resolution: the negative lookups
  (`A`) and existence checks (`X`).
- `tools/aedilis.c:928` and `:1012` - raw `opendir` over include roots
  and corpus directories: the listings (`D`) that carry shadowing.
- `lib/iter_directoria.c:85,176,335` - raw `opendir`/`stat`.
- Inside `lib/filum.c` the syscalls sit in ~20 functions (`fopen` at
  :104 :149 :165 :210 :467 :649 :686 :722 :759 :949 :956; `stat` at
  :93 :1056 :1086 :1197 :1222; `opendir` :866; `unlink` :837; `rename`
  :921; `mkdir` :1029 :1110).
Design: one small recorder module (one function, e.g. `notare(genus,
via)`, lazily opening the file named by the environment variable once
per process) called from those points. The spike (step 1) needs only
the read/exists/list sites plus `via_existit` and the two aedilis
`opendir` loops. `getenv` does not occur on the aedilis path
(`tools/aedilis.c`, `lib/aedilis.c`, `lib/filum.c`, `lib/via.c`), so
`E` lines are empty for the spike; the wrapper matters for later tools.

### XII.2 Objects are deterministic - per working directory
House flags include `-O2 -g` (`tools/vexilla.sh`). Measured on
`lib/chorda.c`: two builds a second apart -> identical; different
output path -> identical; different working directory with absolute
paths -> DIFFERENT (`-g` embeds the working directory once,
`strings | grep $PWD` = 1). So the oracle compares objects built in
the same tree (as §X.5 says), and either the key includes the working
directory or `-fdebug-prefix-map` removes it (A5).

### XII.3 Parallelism needs observed writes (corrects §VI)
The executor runs each action with `processus_exsequi` after
`setenv("FABRICA_AGIT")` (`tools/fabrica.c` `_agere`) and checks writes
by snapshotting the whole tree before and after each action
(`_vestigium_capere`, `tools/fabrica.c:586`). With several actions
running at once the snapshots overlap and a write cannot be attributed
to the action that made it. Therefore:
- Only actions whose writes are KNOWN may run in parallel: house tools
  with a ledger (`S` events), and `compilatio`/`nexus`, whose single
  output fabrica itself names (`-o` into the store, atomic rename).
- Other actions (bash scripts calling external programs) stay serial
  and keep the snapshot check.
- The async path fits: `processus_incipere` / `processus_pulsare`
  (never blocks; `select` with zero timeout) / `processus_metere`
  (`include/processus.h:120-200`). A per-action ledger file is set by
  `setenv` immediately before each `processus_incipere` (the child
  inherits the environment at fork).

### XII.4 The toml runner today
`toml/compile_probationes.sh` (276 lines): its own lock (`tools/sera.sh`);
hand-written source lists (13 materia sources at :149-151 "toml
CONSUMES it"); mtime staleness with the same-second rule documented
inline (:87-96); and the coarse rule "the newest header anywhere makes
EVERY object stale" (:97-129). Each test links all objects (:244). The
pilot replaces lists with closures and the coarse header rule with
per-object keys; the runner keeps its lock, its logging (`tee
build/test_logs/toml.log`) and its reporting.

### XII.5 excubitor
`./excubitor.sh` (repository root), run at the end of
`compile_tests.sh:1225` with `-tacitus` and `|| true` - it reports,
never fails. It reads `fabrica.tsv` (84 lines, hand-written; its header
says it was meant to be absorbed by aedilis manifests). The oracle
period (step 7) compares its verdicts with the store's for the toml
objects.

## XIII. As built (2026-10-03)

Plan `fabrica-plan-2.md`, executed inline on main over two days; commits
cadca60e + b9d03e84 (T1), 76708a1b / 26e34570 (T2), d664f6ad (T3),
6b357b86 / 1db36de7 (T4), 215f88cc (T5), b391f68b (T6), 0877b6b7 (T7),
6bbc48cd (T6b, parallel judging - Fran's follow-up, not in the plan).
Side fix in between: 7ed77ac6 (commissio refuses when its own format
step stales a just-built binary, park …ET4262).

| §IX done means | after slice 2 |
|---|---|
| toml runner on fabrica; oracle agrees across real commits | runner compiles through bin/compilator from aedilis closures (hand lists, the "newest header anywhere" rule and its excubitor call gone); `tools/toml_oraculum.sh` (old and new runner in the SAME tree; `-arbor REF`) agrees on HEAD and cadca60e / 8d7e6fba / 26e34570: 44-45 objects byte-identical, 370 test lines identical. NOT a fabrica-declared familia of test binaries (Fran, T5: nothing can judge a test binary yet - per-test verdict actions are slice 3) |
| the ledger keys a real action family, with shadowing and new-file plants | the 19 snippet generators are `lectiones="verum"`: keyed by their last congruent run's read ledger (L content, A absence, D listing); real plants: header comment -> exactly the 4 tessera snippets; shadow header -> the same 4; new gesta test -> fragmentum_gesta only. Ordering edges from traces (silva.c plant -> all 19 after amalgama_silva) |
| measured speedups (toml cold/warm, aedilis gate, snippet generators) | aedilis gate 208-262 -> 90.5 s; fragmentum_compile_tests per miss ~64 -> 11 s warm (17 s cold store), fragmenta_silva ~34 -> 3 s; second -plenus 44 -> 24 s; toml suite warm 31.8 -> 31.1 s, header edit 38.0 -> 31.2 s, wiped build + warm store 39.2 -> 31.5 s (tests themselves = 27.9 s of every run); 19-snippet heal serial 93.0 -> parallel 65.7 s; -plenus with all snippet traces cold 112.6 -> 96.2 s |
| excubitor deleted | NOT YET, by decision (Fran, T7): the record (4 trees x 4 scenarios) shows false alarms on touched headers and blindness to -I shadowing, but ~18 mtime runners still use it as their guard. Deletion is the last row of batch job …W0ZBW ('migratio ad compilatorem', 20 rows, inventory …MEES9X) |

Built beyond the plan or differently (rulings in the progress ledger):
the trace key carries the exitus (multi-output actions kept only their
last trace); a memo audit (1 in 20 under -plenus, every hit under
`-audit`; AUDITUM DISCORS) - 0 discords over a full audit; the store
(`lib/thesaurus.c`, filum + sigillum, atomic writes, sampled read
verification) with a GENERATION = an orchestrating run named by
THESAURUS_GENERATIO, not a process (~200 aedilis processes per -plenus);
`bin/fabrica purgare`; the store is cache, not input - the judge drops
its blobi/actiones/generationes from traces and lib/thesaurus.c is exempt
in the raw-IO lint (Fran may overrule); aedilis records keyed on its own
binary + aedilis.stml + file bytes (silva's parse does no IO);
bin/compilator keys include cwd (-g), sorted .h listings of every -I
root and the source dir (shadowing), and the REAL clang binary's digest
memoized by file identity (/usr/bin/clang is a 119 KB trampoline);
hits copy, never hard-link; `familia` = a directory listing + one STML
template (`&@basis;`, `&@fons;`, house macro syntax) instead of four
fixed binding rules; parallel heal checks writes twice (per member S,
per wave snapshot - S alone would have left bash generators' own writes
unchecked) and stops starting work after a failure while still judging;
the store and per-action ledgers are part of every envelope.

Found on the way: filum's mkdir -p returned failure when another
process created the same directory at the same moment (fixed, pinned by
a race test); Apple clang 16 compiles toml/fontes/toml_scalaris.c
non-deterministically (loop vectorizer; park …ACYVJ for Fran); the
oracle's first version reported "consensus" on a commit without toml
(silent gate - now exit 2); the commit's format step staled freshly
built binaries twice (…ET4262, fixed); the first real-tree parallel heal
failed all 19 snippets because the store was outside their envelopes
(T3 had only exercised scratch judging).
Core assertions 344 -> 443; fabrica fumus 25 -> 31 stages; new gate
`compilator` (fumus VIII); new tools `tools/lectiones_lint.sh`,
`tools/toml_oraculum.sh`, `tools/compilator_struere.sh`.

Known limits / deferred: excubitor deletion (…W0ZBW); the judge is now
dominated by six NON-traced generators regenerating serially
(corpus_silicis, excludenda_silva, cocti, tabulae, numeri - ~47 s;
desideratum …VFF5D); heal by promoting the judge's scratch output (one
run instead of three, design question …XZQG); a directory listing's
digest covers names, not entry types (gap noted at the call); a header
edited DURING a compile could be stored under the wrong key; on a fresh
database converted actions have no trace edges for their first run;
per-test verdict actions and a store shared across worktrees (slice 3+).
