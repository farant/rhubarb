# analytica - interview (Q1-Q11)

Census: `project-specs/analytica-census.md`. Vision: tabularium …QB5SQ.
Each question has my proposal; Fran decides. Acceptance for the slice =
the six questions in census §II, all answerable by named queries.

**Q1 - Identity of a job.** What is "the same job" across runs?
*Proposal:* a declared `titulus` when the job carries the chassis
annotation (`# <cursus titulus="..." versio="I"/>`), else the script's
path (relative to the root) for chokepoint-only records; fabrica actions
keep their action titles; gates `porta:<nomen>`. Arguments are recorded
but not part of identity (a filtered run is the same job).

**Q2 - Home.** *Proposal:* the mensura volume in `~/.rhubarb` (already
shared across trees), every run row carrying `arbor` (the worktree root)
- so "when did frigida last pass, anywhere" is one query.

**Q3 - Folding the per-tree stores.** *Proposal:* tempora.tsv and
fabrica's `cursus` become PRODUCERS into the run table (their writers
also emit a run record); their own files stay for now (fabrica still
needs `cursus` for its estimates), as caches, not truths. Retire tempora
once the query covers its readers.

**Q4 - Granularity / levels.** *Proposal:* a run is anything that has a
start, an end and an outcome; children via `parens` (as mensura already
nests). v1 levels: job (script / gate / fabrica command) -> step (fabrica
action, commit phase, entry-point stage). Measurements inside a run stay
mensura measurements (valor + unitas), attached to the run.

**Q5 - Invoker.** *Proposal:* `vocator` = homo | commissio | uncus
(hook) | fabrica | frigida | agens, passed down by environment
(`CURSUS_VOCATOR`, set by each invoker, inherited like MENSOR_PARENS);
absent = homo.

**Q6 - Commit and machine.** *Proposal:* commit + dirty flag computed
ONCE per top-level session (git costs tens of ms; children inherit by
environment), machine as mensura already does. A tree digest is not in
v1.

**Q7 - Log capture.** *Proposal:* every chassis run captures stdout +
stderr into the content-addressed store (identical quiet logs dedup to
one blob); chokepoint-only runs record no body in v1. A size cap per body
(e.g. 8 MB, tail kept). The single "store this body" function is the seam
where redaction plugs in later; the chassis never dumps the environment.

**Q8 - Retention defaults.** *Proposal:* metadata forever; bodies full
for 7 days, gzip (flatura) to 30 days, then deleted - EXCEPT failures
(full 30 days, gzip to 90), the last body of every job (forever), and
pinned bodies (forever; a ledger item or worklog naming the run id pins
it). Declared as data (one STML file), not constants.

**Q9 - Query surface and its name.** *Proposal:* named queries with
parameters in C, no query language (canon rule): `ultimum`, `historia`,
`tendentia`, `numquam`, `fracta`, `acta`, `discus`. Home: `bin/mensor
quaestio <nomen> par=valor ...` (-machina TSV), pythonica
`silva.cursus(nomen, **par)` returning rows. Or a new binary? (name to
decide - mensor already means "the measurer").

**Q10 - Chassis v1 and pilot.** *Proposal:* a sourced prologue
`tools/cursus.sh` (opens the run, sets `trap EXIT` to close it with the
exit code, captures output) + the annotation for identity and version.
Pilot: the four converted entry points, frigida, one hook, one gate
script.

**Q11 - Cleanup trigger.** *Proposal:* opportunistic - `bin/mensor
purgare` runs at most once a day after a journal fold (itself a run with
the chassis); `-siccum` reports what it would compress and delete.

## Decisions (Fran 2026-10-13)

All proposals accepted ("that all makes sense"), Q9 as proposed
(`bin/mensor quaestio`, no new binary). Fran's addition to Q2/Q6: record
**worktree and branch** explicitly - the commit implies neither (a commit
sits on several branches and a branch moves; quinta, secunda and main can
share one commit; frigida runs in a temporary tree on `frigida-<sha>`).
So every top-level session records `commissum`, `sordidum`, `ramus`
(branch at run time, or `detached`) and `arbor` (worktree root),
computed once and inherited by children.
