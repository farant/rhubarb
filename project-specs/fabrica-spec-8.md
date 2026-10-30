# fabrica spec 8 - trust the trace where it is complete

Ledger: nota …NS2M (the plan: P lines, sqlite VFS, trace-only
judgment), nota …KEY4 (the measurement below). Builds on spec 7 (root
suite as steps, member areas, write debts) and the trace
(FABRICA_LECTIONES, `include/lectiones.h`).

*The question: every member run takes two whole-tree snapshots to
catch writes outside its footprint. Can fabrica judge most members
from their own trace instead - without losing an independent witness -
and what does it take to make that a guarantee, not a habit?*

Status: DECIDED (Fran 2026-10-24: Q1-Q5 below). Order of work in VI.

## I. Data (2026-10-24, quinta, macOS 27.0.1 / clang 21)

- **Snapshot cost**: ~190 ms per snapshot, two per member run; ~90%
  is the kernel directory walk (bare walk with fabrica's exclusions
  ~170 ms, sort 8 ms; `lstat` of the ~12.3k TRACKED files alone:
  ~20 ms). radix -vis: 520 s wall, 216 s sys - snapshots are most of
  the sys time. Memoized runs pay nothing (cached members don't run).
- **Tree**: ~12.3k tracked files, ~30.5k ignored (fabrica state ~9.5k,
  other build outputs ~7k, tool leftovers ~10k, gutenberg-data 3.3k).
  The walk's cost is mostly files no member should ever touch.
- **Step 1 - exec in the trace** (40fda90e): `P\t<argv[0]>` before
  every fork (processus, pseudoterminale, the `system()` calls).
  Members that execute ANY program: radix 41/430, silva 5/56, the
  other seven composites 0. radix: /bin/sh 24, tls/generare.sh 8,
  tmux 4, chmod, curl, git, clang, house binaries, test-built fakes.
- **Step 2 - sqlite in the trace** (0aa47f04): scrinium opens through
  the trace VFS "rhubarb-vestigium" (L on read/fetch, S on
  write/truncate/delete, S for `-shm` at xShmMap). It found three
  tests writing `-journal`/`-wal` siblings no debt covered; fixed.
- **The measurement** (6a393265, `bin/fabrica probare -conferre`):
  per member, snapshot changes vs S lines, both OUTSIDE the member's
  location (area, liber, acta), all nine composites, 554 members, all
  green:

  | composite | members | snapshot saw, trace didn't | trace saw, snapshot didn't |
  |---|---|---|---|
  | radix | 414 | 3 paths, 2 members (all under debts) | 1124 paths, 7 members |
  | css, html, materia, md, saltuarius, toml, silva, veneni | 140 | 0 | 0 |

  The 3: `probatio_lectiones` (the trace's own test points
  FABRICA_LECTIONES at its own file, by design) and
  `probatio_natura_glossae` (`tools/natura_glossae.c:689` raw `fopen`
  write - on the code-96 debt list). The 1124: transient files
  (sqlite siblings, temp scripts, sockets) and directories - the trace
  sees strictly MORE than the snapshots.
- **Write-side gap (static)**: raw `remove/unlink/rename/mkdir/rmdir/
  chmod` in 49 non-archived .c files (22 tests, 12 tools, 6 lib,
  knotapel 4, officina 2, briar, apps, vendor). Code 96 lists only
  `fopen freopen open opendir stat lstat access`; its debt ratchet
  (`silva/probationes/fixa/examinis/canalis_debita.tsv`) holds 164
  files. filum already notes S for delete, move, mkdir, copy, write,
  append (`lib/filum.c`); `chmod` notes nothing. None of the 49
  produced a miss in the measured run.

## II. What the snapshots do (each needs a trace replacement)

1. **Footprint**: `fabrica_vestigia_comparare` -> "scripsit extra
   vestigium". The trace already judges S lines
   (`_scripturam_alienam`).
2. **Debt use**: `_debita_notare` -> `<area>debita.txt` (dead-debt
   detection). Already merges trace lines; without snapshots it writes
   nothing (IGNOTUM).
3. **Cache invalidation**: `fabrica_mutata_intra_locum` - did the
   member change anything outside its location (then the per-run
   generator results are discarded). Without snapshots: FALSUM =
   discard (safe, slow).

## III. When is a trace complete?

A member's trace is complete when every process it ran does its file
I/O through the house layer that notes lines. It is NOT complete when:

(a) it executed a FOREIGN program (P line of a program not built from
    this tree with lectiones linked);
(b) house code did I/O around the layer - raw reads are code 96;
    raw writes (I, last bullet) are not yet linted;
(c) a write happened through mmap (sqlite's `-shm` is handled) or an
    inherited descriptor - in practice (a) or (b).

## IV. Proposal

**T1 - the write side of the layer (Q3: strict, all at once).** An
examen rule for `remove unlink rename mkdir rmdir chmod symlink
truncate mkstemp` outside filum/lectiones, born strict: NO debt list -
every occurrence (49 files, I) is fixed in the same commit, callers
move to `filum_*` (filum's chmod gains an S line). The same sweep fixes
raw `fopen(..., "w"/"a")` writes among the 164 code-96 debt files
(e.g. `tools/natura_glossae.c:689`); read-only debts may stay. Born
red by a plant that compiles. Afterwards a member is trace-complete
unless it executes a foreign program (III a).

**T2 - the decision, per member, before the run (Q1).** Predict from
the member's previous trace (stored with its memo entry): no foreign P
line -> run without the full walk; otherwise snapshots as today.
FOREIGN (Q4) = any P program fabrica cannot vouch for: house-built
means fabrica built it from this tree (or knows its install action,
e.g. `~/.bin/mensor`) AND its link closure contains lectiones; never a
path rule (a script under `bin/` is foreign). Lookup cached per
program path. No previous trace (new member,
cold clone) -> snapshots. SURPRISE (the new trace has a foreign P) ->
rerun that member with snapshots and use that verdict.

**T3 - the three jobs from the trace.** Footprint = S lines (as now);
debts = trace lines (as now); cache invalidation = any S line outside
the member's location.

**T4 - keep independent witnesses.**
- Per trace-complete member: `lstat` of the TRACKED files only
  (~20 ms, list from the git index) before/after - nothing can
  silently change a source or fixture, whatever the trace says.
- Per RUN (Q5): one full walk before the first member and one after
  the last; any change outside every member's location that no trace
  names fails the run, paths listed ("written by no traced member, or
  edited by hand during the run") - the wave logic
  (`_scripturas_ignotas`) reused. ~0.4 s per run, not per member.
- Sampled audit (Q2): commits trust the trace but audit a
  deterministic sample of the members that RUN - hash(member, commit)
  mod X (~1 in 10), every member audited eventually - plus every member
  whose own source changed. Audit = both snapshots + the `-conferre`
  comparison; a `photographia` line not under a debt = member FRACTUM,
  named ("vestigium: scriptura sine linea S: <via>").
- `fusio_probare` (merge tier) and `-audit`: full snapshots for all.
- Plant via silva.planta: a fixture member writing outside its area
  with a raw syscall; the sampled audit must turn it red.

## V. Expected effect

~508 of 554 members trace-complete (all but the 46 that execute
programs). Per run of such a member: ~380 ms of walks -> ~40 ms
(tracked-file lstat twice). radix -vis: roughly 370 x 0.34 s ≈ 125 s
less (520 s now). Commits that rerun many members save in proportion;
memoized runs are unchanged. To be confirmed by measurement after T2.

## VI. Order of work

1. T1: the write rule + every occurrence fixed, one commit, plant.
2. T3 + the tracked-file check, behind a flag, cross-checked against
   snapshots with `-conferre` on every run until trusted.
3. T2: prediction from the previous trace (+ the house-built lookup).
4. T4: the walk around the whole run, then the sampled audit.

## VII. Decisions (Fran 2026-10-24)

- **Q1** completeness predicted from the previous trace (no
  declarations to keep in sync).
- **Q2** commits: trust + sample ~1 in 10 of the members that run
  (hash of member and commit) + members whose own source changed;
  merge tier: full snapshots.
- **Q3** an examen rule for the write functions, every occurrence
  fixed at once (no debt ratchet).
- **Q4** house-built = fabrica built it (or installs it) and its
  closure contains lectiones; unknown = foreign.
- **Q5** per member the tracked-file check; per run one full walk
  before/after, unnamed changes fail the run.

## AUDIENDA (not verified)

- sanare waves (one snapshot per wave, lectiones="verum") were not
  measured; `-conferre` hooks `_membrum_agere` only.
- One run per composite: a raw write that only happens on some code
  path (error paths, first run on a cold tree) would not appear.
- Transient raw writes (created and removed within a run) are
  invisible to BOTH witnesses today; trace-only judgment is not worse
  there, but nothing catches them.
- The 20 ms tracked-file figure is from an earlier measurement
  session, not re-measured under macOS 27.
- Saving estimate (V) assumes walk cost dominates the per-member
  overhead; area preparation and liber handling were not timed.

## VIII. Step 0 - baseline after T1 (2026-10-27, quinta)

T1 shipped as fabrica-8 step 3 (code 97 SCRIPTURA_PRAETERITA = VIOLATIO,
transformare + sweeps I-VIII, in main ff 7273f73a). Not "no debts" as IV
proposed: the channel itself, credo's own area handling, machinula (notes
first), utime-only test sites and the forum secret keep per-site TOLERA
(~48 sites). Of those, only credo (5) and the secret (3) write without an
S line - credo's land inside the member area (to verify in step 2).

`bin/fabrica probare -vis -conferre` over all nine composites, 587 members:

| composite | members | snapshot saw, trace didn't | trace saw, snapshot didn't |
|---|---|---|---|
| css html materia md saltuarius toml silva | 135 | 0 | 0 |
| radix | 436 | 2 paths, 1 member (both declared debts) | 1350 paths, 8 members |
| veneni | 16 | 4 paths, 2 members (~/.bin, foreign install) | 0 |

- radix: `probatio_lectiones` points FABRICA_LECTIONES at its own files
  by design (`<debitum_scripturae>` x2). The 10-24 real miss
  (`natura_glossae` raw fopen write) is GONE.
- veneni: ~/.bin/mensor, briar, briar-spectator, silex replaced at
  14:56-14:58 by another session's main install while cellae and plexus
  ran - the "written by no traced member" class (IV T4, per-run walk).
  Both members FRACTUM on it today; no trace gap.
- trace-only: transients the snapshots cannot see (probatio_thesaurus
  1135 store blobs, probatio_silex 179, vigilia 24, ...).

Snapshot cost re-measured (`sample` on `probare -vis probationes_toml`):
the before/after `_arborem_photographare` = ~1/3 of the run, ~0.42 s per
snapshot (other sessions active - upper bound; spec I said ~190 ms).
The walk sees ~49k entries (store parts excluded; the tree is ~352k).
fabrica records no snapshot timing - add a mensor fact in step 2.

Conclusion: the trace is complete for every member that executes no
foreign program; step 2 (T3 behind a flag, cross-checked) may start.
