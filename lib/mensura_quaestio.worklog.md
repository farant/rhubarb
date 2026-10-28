# mensura_quaestio worklog

## 2026-10-15 — empty results name their cause (cursus_quaerere_similes)

**Friction.** A query whose title filter matched nothing printed nothing and
exited 0 - identical to "this job has never failed". Bit three times in one
session: `historia titulus=probatio_tcp_servus` (the job is
`probationes_radicis/probatio_tcp_servus`), `tendentia titulus=...` (same),
`limes=5` used as a row limit (it is lentescentes' per-mille threshold; rows
are `n=`). An agent checking "is this flaky?" would have concluded "never
failed" from silence.

**Library.** `cursus_quaerere_similes(index, filtrum, maximi, &noti, p)`:
`noti` counts runs matching the TITLE filter alone (titulus/praefixum; status,
post, nota, arbor, ramus ignored) - so "known title, other filters excluded
it" is told apart from "never recorded". If noti == 0: distinct titles that
contain the query, or whose last segment (after `/`, `#`, `:`) equals the
query's, ranked by run count then title, capped at `maximi`. NIHIL when there
is no title filter. Tests: section XII of probatio_mensura_quaestio (bare
name -> 3 similar, ranking + cap, segment-only match where the query is NOT
a substring, known-but-filtered, nothing similar, no filter). Plants: drop
the segment rule (16/17), count noti with the full filter (15/17).

**mensor.** One hook at `_quaestio`'s common exit (`ordines` empty ->
`_nihil_congruit_nominare`), on STDERR and in HUMAN mode only - `-machina`
stays silent and the exit code is unchanged. First attempt printed in both
modes; mensor_cursus_fumus XI refused it (`sectiones titulus=nemo -machina
2>&1` must be empty - consumers often merge stderr). XI now also asserts the
human hint (`titulus=xi` -> `simillimi: porta:xi (n=1)`, the ':' segment
rule); plant (hook removed) -> XI red. `ordines` now starts
NIHIL (it was read at the exit; not every branch assigns it). `mensor
quaestio` without a name, or with an unknown one, prints a usage table: each
query's question, the parameters (`n=` vs `limes=`), and the title shapes.
Spec analytica §V gained worked examples ("how to ask").

## 2026-10-27 - `quaestio fusio` (budget B0)

Fran wanted the merge's three tables (merge commit phases + gates, fusio
gates, timeline) in one command. The gap was upstream: `silva.fusio_probare`
never opened a run, so its ~50 gates were parentless top-level runs found
only by tree + commit + time window (and my first gate query, capped at 80
rows, silently dropped eight). Fix in two parts: (1) pythonica -
`fusio_probare` is `@_cursu_notatum('fusio')`, so its phases and gates nest
under it like the commit's; (2) `cursus_quaerere_fusio`: the latest `fusio`
of the tree and the latest `commissio` of the same tree that STARTED before
it (a commit made in the tree while the fusio runs is not the merge commit),
children longest first. `mensor quaestio fusio [arbor=] [-machina]` prints
the commit block, the fusio block (a run without an end: time so far, and
'currit' only if its keeper process is alive - judged by the same
`_vitam_iudicare` as `currentes`) and a timeline. The library stays pure:
running time is computed in tools/mensor.c from the clock.

Tests: probatio_mensura_quaestio XVI (older/later commits, a fusio in another
tree, a running fusio with a commit started during it); mensor_cursus_fumus
XIX end to end through incipere/finire. Plants: commit-before-fusio rule and
child ordering, each red. Note: a root run's tree comes from cwd -
MENSOR_ARBOR is inherited by children only.

## 2026-10-27 — budget B9 2b: `mensor interventio notare`

Marks a performance intervention as an `interventio` cursus: commissum =
full sha of the marked commit, nota = "titulus [rubrica]", acta = the
sorted, deduped set of affected titles (LEGENS titles from
`bin/fabrica legentes` over the commit's diff against its FIRST parent,
plus "porta:<name>" for every gate `tools/portae_debitae.sh` owes on that
diff). `-scopus a,b` narrows by prefix. The affected set is a snapshot at
marking time on purpose: closures drift, and the cohort question later
("did titles X get faster after this commit?") wants what was true then.

Real history (scratch diary): piscina commit 7c071b73 -> 677 affected,
.DS_Store 0ac92e25 -> 145, fabrica T1 ec8f4d64 -> 68. Unaffected titles
form the control group for 2c.

Test trap: the first fixture marked HEAD, so a plant that stored the
*context* commissum (mensura_contextum_parare already fills it with HEAD)
instead of the resolved sha stayed green. The fixture now has three
commits and marks HEAD~1; the plant goes red. General rule: when the
thing under test and the ambient default can coincide, make the fixture
pull them apart.

## 2026-10-27 — budget B9 2c: `cursus_quaerere_cohortes` / `mensor quaestio cohortes`

Splits a title's passing runs by which marked interventions their code
CONTAINS (git ancestry via a caller callback — `git_continet` in mensor, a
fake linear table in the test), not by time. Decisions worth keeping:

- **What "the code of a run" is.** Inside a passing commit attempt the run
  tested the commit it produced, but `initium.commissum` is the parent and
  the commissio nota only has an 8-char sha (lib/git can't resolve short
  shas). So: contains(I) = I starts with the short sha, OR parent contains
  I. Clean run = its HEAD. Dirty run outside a commit = unknown code,
  omitted and counted (`sordidi`) — those are mostly WIP runs of the very
  change being measured, and would poison the "before" cohort.
- **Base for delta** = the earlier cohort whose mask is the largest proper
  subset. On linear history that's just the previous cohort; on diverged
  worktrees it avoids comparing against an unrelated branch's cohort.
- **Control ("moderatio")** = for the latest intervention, leaf titles (no
  child runs — commissio/fusio contain affected gates) touched by NO
  marked intervention, ≥3 runs each side; median of per-title
  (after−before)/before. Reported as a change so it reads against the
  cohort deltas directly.
- Ancestry answers are memoized per (commit, intervention); the test
  counts callback calls so losing the cache goes red.
- Mask is i64 with bit 63 unused (cap LXIII interventions per query).
- **xar is segmented** — `xar_obtinere(x, 0)` is NOT a flat array. First
  draft sorted it as one; copy into a piscina array first
  (`_mediana_xar`).

Real data (copy of the live volume, piscina 7c071b73 marked):
probatio_piscina 159 ms → 20 ms (−87%), control +2.6% over 79 titles;
19 dirty runs omitted. ~2.5–5 s per query (reads every run, as the other
queries do).
