# consilium worklog

## 2026-10-16 - S2: the pure core

Started from the S1 mapping (project-specs/consilium-spec.md §XI): the
record grows out of effectus, the plan is a separate small model. The
core needs NO sutura - a nice surprise that keeps the tests trivial
(in-memory plans only). House `i32` is UNSIGNED: the cycle walk first
used `-I` sentinels in i32 arrays and a descending `i >= 0` loop, both
of which would wrap - caught by reading, before compiling; sentinels now
s32, the reversal loop counts down from n to 1.

The first M1 test could not fail: nobody else wrote the file, so
removing the rule changed nothing. Strengthened (a second step writes
the same file) before planting; the plant then went red at the wave
assertion. Plants must compile under -Werror: neutralize with
`(VERUM || f(x))` rather than deleting a call (an unused static function
is an error). Three words the lint did not know were renamed to known
ones instead of glossary entries: PARTIALIS -> EX_PARTE, QUODVIS ->
OMNIA, deduplicatio section -> semel.

## 2026-10-16 - S3: dialect, CLI, commissio pilot

`ordinarius` is a latina macro (default) - hit while writing the
reader's `_enumeratum(..., ordinarius)`; renamed `praestitutum`. The
reader test caught unknown root attributes going unchecked. The pilot's
first end-to-end run failed for the right reason: fabrica refuses to
heal while the judge (bin/fabrica) is itself stale - the plan now
bootstraps the judge in its own wave and orders every other heal after
it. The setup of that test matters: lib/aedilis.c is in BOTH bin/aedilis'
and bin/fabrica's closures, so both must be rebuilt from the
misformatted bytes before the guard runs, or both are stale BEFORE
formatting and the guard (rightly) does nothing.

## 2026-10-17 - S5a: the executor core

`consilium_exsequi` judges the plan itself (cycle, unknown sequitur,
conflict, non-LICET verdicts) and refuses BEFORE any `agere` - so a
caller cannot run a plan it forgot to judge. Dependents are found
through `ordo->arcus`, which means explicit `sequitur` and the blind
wall are dependencies too (a step after a failed blind step is
OMISSUM): conservative on purpose - `sequitur` is used today for "heal
the judge first", where running after a failed judge build is wrong.
The first test of a blind step put it LAST and it was (correctly)
omitted behind the failed steps; the test now adds it first.

Second test-side catch: a step whose own proposed write lies outside
its `vestigium` is VETITUM at judgment - the plan was refused and the
section crashed on a NIHIL acta. Fixed the test (vestigium build/*),
and the sections now use CREDO_NECESSE_NON_NIHIL(acta) so a refusal
stops the section instead of segfaulting it.

Observed writes = snapshot diff + the step's own recorded writes
(`actum.notata`, a lectiones book) - the second is the only way to see
house-tool writes outside the snapshot. Extra writes fail only a step
that PROMISED (not blind, every writer effect PLENA); otherwise they
are reported ('extra propositum, non promissum'). Serial only:
per-step snapshots make attribution exact; the wave-wide unknown
writer rule needs simultaneous waves (S5d). Four plants (dependency
propagation, SISTERE, extra check, refusal) each red at its own
assertion.
