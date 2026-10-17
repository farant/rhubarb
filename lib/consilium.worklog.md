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
