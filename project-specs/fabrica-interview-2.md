# fabrica — interview raw, slice 2 (2026-10-02)

Feature: slice 2 of the house build layer. Spec v1 §XI: "one
subsystem's runner on the fabrica; then the shared object store in
`build/aedilis/obiecta/` on digests (Q27, Q31); oracle = the old
runner, byte for byte".

Context carried into the interview (sessions 2026-10-01/02):
- Slices 1a and 1b complete; park …AR15 closed (amalgam chain
  memorabilis, xar sort, gate timings in `build/portae/tempora.tsv`,
  owed gates via `aedilis --corpus`).
- Gate work the same day: `fabrica` gate 272 s → ~60 s (with a live
  generata receipt) / ~180 s; `aedilis` gate 274 s → ~205 s. The
  aedilis gate's remaining cost is a SERIAL loop, kept serial because
  builds share the object cache `build/aedilis/obiecta/` (many actions
  write the same `lib__chorda.o`).
- Measured earlier: 1,737 objects in 15 build dirs, 5.5 GB;
  `piscina.o` compiled 24 times. Object stores today are `ignota`
  preconditions run blind (`obiecta_radicis`, `obiecta_briar`,
  `obiecta_mechanismi_X`; 17–24 s after a header change).
- Deferred to slice 2 by earlier specs: action kinds
  `compilatio`/`nexus`; excubitor oracle-then-delete (Q35); per-object
  closures (generated `struere.sh` treat every header in CAPITA as a
  dependency of every object).
- Design strands raised by Fran this week (ledger desideratum
  01M3XNM1HNEN8VD3YYPYZDQNCC, vision 01M3XQVDSY87M80CKNDTFA71YH):
  all file IO through `filum` with a lint; a `filum` READ LEDGER
  (verifying traces, Mokhov/Mitchell/Peyton Jones 2018); program kinds
  as capability bundles; pure jobs returning plans of effects; inputs
  pulled (applicative vs monadic).
- aedilis per-file results are cheap to cache by content digest
  (`aedilis --corpus` 6× faster in one process; snippet generators
  `fragmentum_compile_tests` 59 s / `fragmenta_silva` 41 s per miss).

## Round 1 — shape

**Q1. How wide should slice 2 be?**
Options: ledger first, then store / narrow as spec v1 / wide (absorb
filum lint, ledger, kinds, plans).
**A: Ledger first, then store.** Open with the read-ledger spike; its
result decides how compile/link actions get their inputs. Then the
content-addressed store and one pilot runner. Kinds and plans stay in
the vision for later slices.

**Q2. Unit of a compile action?**
Options: object = cache entry, not declaration / one declared action
per object / per test binary only.
**A: Object = cache entry, not declaration.** Coarse declared actions
(a test binary, a suite); inside them each `.o` is looked up in the
store by a key (source digest + header-closure digests + flags +
compiler). Bazel-style action cache: thousands of objects, few
declarations.

**Q3. Parallel execution in slice 2?**
Options: yes, once the store exists / no / only inside runners.
**A: Yes, once the store exists.** The store makes parallel builds
safe (no two jobs writing the same `.o` path). Executor runs
independent actions through the async `processus` path, N at a time,
deterministic record order.

**Q4. Pilot runner?**
Options: toml / css / root runner subset.
**A: toml.** 13 tests, newest materia client, landed with zero
substrate changes.

## Round 2 — inputs and the store

**Q5. clang can't write our ledger. How is a compile's read set
captured?**
Options: depfile + radices digest / depfile only / tracing shim.
**A: Depfile + radices digest.** `clang -MD -MF` reports the headers
actually read; it can't report failed lookups, so shadowing (a new
header earlier on the `-I` path) is covered by digesting include-root
listings, as the `radices` input kind does today.

**Q6. Compiler identity in the object key?**
Options: clang binary digest / `clang --version` / keep ambient.
**A: Yes, the clang binary digest.** Hashed once per run; a
Homebrew/Xcode upgrade invalidates objects instead of silently mixing
compiler versions in one link.

**Q7. Where does the store live?**
Options: per tree `build/aedilis/obiecta` / shared `~/.rhubarb/obiecta`
/ per tree with tree-independent keys.
**A: Per tree, `build/aedilis/obiecta`.**

**Q8. Eviction?**
Options: mark from live records / size cap LRU / never automatic.
**A: Mark from live records.** GC keeps every object referenced by
the latest fabrica records plus N recent generations; everything else
is deleted by an explicit `bin/fabrica purgare`.

## Round 3 — the ledger

**Q9. What does fabrica do with a house tool's recorded reads?**
Options: replace declarations / audit declarations / audit first,
then replace.
**A: Replace declarations.** Verifying traces: the recorded read set
(with digests) IS the key for the next run; manifests stop being
hand-proved. Declarations shrink to what can't be observed (argv,
environment, external tools). First run is IGNOTUM, as today.

**Q10. How much of the filum migration (54 files call fopen
directly, 21 opendir) belongs in slice 2?**
Options: only the pilot path / all, lint blocking / none.
**A: Only what the pilot path needs.** Migrate the tools fabrica runs
for the pilot (aedilis and its libraries); the lint lists the rest as
a WARNING. Full migration becomes a batch job (tabularium expeditio)
later.

**Q11. What should the ledger record beyond "file read"?**
Options (multi): failed lookups / directory listings / writes /
environment reads.
**A (Fran):** "as many as you think is good for this slice, all of
the options sound promising".
**Claude's position (stated in conversation):** all four. Failed
lookups, directory listings and writes come straight out of `filum`;
environment reads need a small recorded `getenv` wrapper, used only
along the pilot path in this slice.

## Round 4 — the pilot and the oracle

**Q12. After slice 2, what is the toml runner's job?**
Options: thin (fabrica builds, runner runs) / fabrica builds and runs
/ runner unchanged with fabrica beside it.
**A: Thin.** `toml/compile_probationes.sh` asks fabrica for the test
binaries (built through the store), then runs and reports as today.
Running tests as fabrica actions stays slice 3.

**Q13. Binaries aren't byte-reproducible (LC_UUID). What does the
oracle compare?**
Options: objects + test output / test output only / normalized
binaries.
**A: Objects + test output.** Each `.o` byte for byte (clang objects
are deterministic given the same inputs, flags and paths); linked
binaries compared by the same object set + link line; test runs by
verdicts and output lines.

**Q14. Default fan-out?**
Options: performance cores / all logical CPUs / fixed 4.
**A: Performance cores** (`sysctl hw.perflevel0.physicalcpu`),
override `FABRICA_FILA=N`, as `PROBATIONES_FILA` does for runners.

**Q15. excubitor?**
Options: oracle for the pilot, delete after / delete directly / leave.
**A: Oracle for the pilot, then delete** (with `fabrica.tsv`), per
Q35.

## Round 5 — store shape and declarations

**Q16. Store structure?**
Options: two levels (action cache + CAS) / one level (key → object).
**A: Two levels, Bazel-style.** Action cache: key → output digest;
CAS: digest → bytes. Identical outputs from different keys share one
blob; the action cache is small and rebuildable; the CAS holds old
versions (which enables the later change-classification idea).

**Q17. Declaring the 13 toml test binaries without an expression
language?**
Options: family declaration as data / generated declarations /
hand-written.
**A: Family declaration as data.** One element naming a directory and
a pattern; fabrica enumerates the files and instantiates one action
per file. Data, like the existing `plagulae` input kind.

**Q18. What does a successful ledger spike look like?**
Options: ledger ⊇ manifest, extras explained / exact equality / drive
one real action.
**A: Ledger ⊇ manifest, extras explained.** Every file the manifest
lists appears as a read; every extra read (aedilis.stml, latina.h,
failed lookups) is named and justified. One unexplained gap means the
design isn't ready.

## Round 6 — failure modes and scope

**Q19. Verify a blob's digest on read?**
Options: sampled + on demand / always / never.
**A: Sampled + on demand.** Every Nth read, every read under
`iudicare -plenus`, and `bin/fabrica purgare -verificare` checks all.

**Q20. Memo audit in slice 2?**
Options: yes, with the ledger / later.
**A: Yes, with the ledger.** Ledgers replace hand-proved
declarations, so trust grows exactly here; a sampled re-run on memo
hits (and always under an explicit flag) measures that trust.

**Q21. Cache aedilis's per-file results in slice 2?**
Options: yes, in the CAS / no.
**A: Yes, in the CAS.** A file's extraction record keyed by its bytes
+ configuration; the first non-object user of the store; speeds the
snippet generators and every closure derivation.

**Q22. A parallel build fails partway — the other running actions?**
Options: let them finish, start nothing new / kill all / keep going.
**A: Let them finish, start nothing new.** Their results are valid and
cached; dependents OMISSUM; the failure is reported first.

## Round 7 — order and done

**Q23. Order: (1) ledger spike, (2) ledger in filum + verifying traces
+ memo audit, (3) CAS + action cache (aedilis records first user),
(4) compilatio/nexus kinds + family declarations, (5) toml pilot thin
runner + oracle, (6) parallel executor, (7) excubitor oracle then
delete?**
**A: Yes, that order.**

**Q24. What does "slice 2 done" mean?** (multi)
**A: all four:** toml runner on fabrica with the oracle agreeing
(objects byte for byte, same verdicts/output, across real commits);
the ledger keys at least one real action family (with plants for
shadowing and new-file cases); measured speedups recorded (cold/warm
toml build, aedilis gate, snippet generators); excubitor deleted.
