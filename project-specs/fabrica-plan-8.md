# fabrica plan 8 - step 2: judge members from the trace (behind a flag)

Spec: `project-specs/fabrica-spec-8.md` (decided Q1-Q5; section VIII =
step 0 baseline: the trace misses nothing once raw writes are gone).
This plan covers spec VI.2 only: T3 + the tracked-file witness, behind
a flag, cross-checked against the snapshots on every run. T2
(prediction, foreign programs) and T4 (per-run walk, sampled audit) are
later plans. Inline, one task per turn, Fran approves each.

## What a member run does today (lib/fabrica_sanare.c `_membrum_agere`)

Two whole-tree walks (`vestigium_capere`, ~0.42 s each today) feed:

| job | function | input today |
|---|---|---|
| footprint | `fabrica_vestigia_comparare` -> "scripsit extra vestigium" | snapshot diff |
| footprint (transients) | `_scripturam_alienam` | trace S lines (already) |
| debt use | `_debita_notare` -> `<area>debita.txt` | snapshot diff + trace lines |
| cache invalidation | `fabrica_mutata_intra_locum` | snapshot diff + trace S lines |
| measurement | `_vestigia_conferre` (`-conferre`) | both |

## The modes (one variable, `FABRICA_VESTIGIUM`)

- `photographia` (default, today): unchanged.
- `umbra` (shadow): BOTH witnesses. Verdict from the snapshots as today;
  the trace-mode verdict is computed too and every disagreement is
  written (`<area>vestigia_umbra.txt`) and counted in mensor. Gates run
  here until trusted.
- `liber`: no walk. Footprint = trace S lines + a TRACKED-FILE snapshot
  (lstat of the ~12.5k tracked paths only); debts and cache
  invalidation from the same two. Taken by step 3 (T2) for members whose
  trace is complete; never the default in this plan.

The tracked-file snapshot has the SAME shape as a full one
(`FabricaVestigium` list, sorted) so `fabrica_vestigia_comparare`,
`_debita_notare` and `fabrica_mutata_intra_locum` take it unchanged -
the mode only decides which pair of lists they receive.

## Tasks

**T1 - snapshot timing in mensor.** DONE 2026-10-27. Hooks
`momentum_ms` + `photographiam_notare` (the core keeps no clock); fact
`fabrica.photographia` (ms, both walks) under each member's run in the
annals; fumus LXVII (planted). First reading, `probare -vis
probationes_toml`: median 452 ms per member = ~0.23 s per snapshot.
The tracked-snapshot fact (`fabrica.trackata`) comes with T2.

**G1 - lib/git opens a worktree.** ALREADY DONE (059fe759, found 2026-10-27): `git_aperire` follows `gitdir:` and `commondir`; probatio_git 'arborem operis' 9/9; fabrica's `_repositorium` resolves HEAD through it in quinta daily. Ledger …NGCM closed (was stale). Original scope: `git_aperire` follows a `.git`
FILE (`gitdir: <path>`) and the worktree's `commondir` (objects and refs
live in the main repository; HEAD and the index in the worktree's own
gitdir). Ledger …NGCM. Test: probatio_git with a fixture worktree
(plain files, no git binary): HEAD resolves, an object reads.

**G2 - lib/git reads the index.** DONE 2026-10-27: `git_indicem_legere` (v2/v3, v4 refused, trailer verified, extensions skipped); oracle `git ls-files -s` over the real index + hand-built indexes; 3 plants. Original scope: `git_indicem_legere` -> paths (and
mode, size, mtime as stored) from `.git/index` v2 (and v3/v4 if present:
v4 prefix-compresses paths - refuse loudly rather than misread).
Checksum verified (SHA-1 trailer). Test: a fixture index written by real
git once (committed as a fixture) + a hand-built one; corrupt trailer ->
refused; worktree index via G1.

**T2 - the tracked-file list and snapshot.** Sutura hook
`trackata_capere` -> sorted `FabricaVestigium` list of tracked paths
(lstat each). List from `git_indicem_legere`, read once per fabrica
process.
Test: probatio_fabrica_* with a fake sutura (shape, sort, absent file =
'absens' entry); fumus stage: edit a tracked file -> in the list's diff,
edit an untracked build file -> not.

**T3 - mode `umbra`.** Then `silva.porta` sets `FABRICA_VESTIGIUM=umbra`
for the composites (Q2). In `_membrum_agere`: take full AND tracked
snapshots; compute both verdicts (footprint, debts set, invalidation
bit); verdict = full; disagreements -> `vestigia_umbra.txt` + fact
`fabrica.umbra_discordia`. Test: fumus stages with a member that (a)
writes outside its footprint through filum (both FRACTUM, agree), (b)
writes a tracked file with a raw syscall from a test-built fake (full
FRACTUM, trace mode FRACTUM via tracked snapshot - agree), (c) writes an
untracked path outside its area with a raw syscall (full FRACTUM, trace
mode MISSES it - disagreement recorded; this is exactly what T2/T4
exist for). Plant: drop the comparison -> (c) not recorded.

**T4 - mode `liber`.** Same code path without the full walk; verdict
from trace + tracked snapshot. Test: (a) and (b) FRACTUM, (c) passes
(documented blind spot until spec T4's per-run walk). Plant: tracked
snapshot not compared -> (b) passes.

**T5 - shadow everywhere, measure.** Gates (`silva.porta` for the nine
composites) run `FABRICA_VESTIGIUM=umbra` + `-conferre`; one full
`-vis` pass over all composites; report disagreements per member and
the timing facts (full vs tracked). Expected: disagreements only on
members with foreign P lines (input to step 3) and on foreign installs
(~/.bin - input to spec T4).

## Out of scope (named so they are not forgotten)

- sanare waves and non-member actions (`_actionem_agere`, park …CH1CJ)
  keep their snapshots.
- credo's 5 TOLERA writes and the forum secret write no S line - T3's
  (c)-style disagreements will show if any land outside a member area.
- transient raw writes outside the area stay invisible to both.

## Decisions (Fran 2026-10-27)

- **Q1 tracked list**: teach `lib/git` first (G1 worktree, G2 index) -
  fabrica asks no foreign program.
- **Q2 shadow in gates**: commit-tier gates run `umbra` as soon as T3
  lands (verdict unchanged; ~2 x 20 ms per member that runs).

Order: T1, G1, G2, T2, T3 (+ gates umbra), T4, T5.
