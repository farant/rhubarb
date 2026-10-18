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

## 2026-10-17 - S5b: the journal (diarium) and resume

Fran's call: the tree fingerprint ignores build/ and other gitignored
output (builds rewrite it constantly). The library never computes it -
the consumer passes it in; the library only compares. Comparison is
against the LAST recorded fingerprint (an ARBOR line after every step,
or the header), not the run's start: steps that write tracked files
(generators) legitimately move the fingerprint, and what matters for
resume is "nobody changed the tree since the journal last spoke". A
step interrupted mid-write of a tracked file moves it too - resume then
refuses, which is the conservative answer.

Runs chain: the last run plus earlier `resumptum` runs back to the
first `novum` (included). A fresh (`novum`) run never inherits an older
run's done steps, even for the same plan. Carried steps still go
through `notare` (journaled again as ACTUM 'ex actis'), so the journal
of a resumed run reads complete on its own.

The plan seal uses length-prefixed fields (`N:bytes`) so a tab or colon
inside a path cannot make two different plans hash alike. S5c MUST add
`mandatum` (the argv) to the seal when it lands - changing the command
is changing the plan.

A plant taught something: removing only the sequitur FIELD from the
seal left the 'S' marker byte, so the hash still differed and the test
stayed green ("porta muta"). The test was right; the plant was too
weak. Neutralizing the whole loop went red.

## 2026-10-17 - S5c1: `bin/fabrica consilium -agere`

The first consumer that really runs things. `<mandatum><argumentum
valor=".."/>` is the step's argv (no shell: processus_exsequi vector) and
is part of the plan seal. STML attribute values are RAW both ways (only
text decodes entities): `&&` is written as is, `&amp;` would reach sh
literally (the first smoke run failed exactly so) - and an argument
containing `"` cannot be written at all. Known limit; commissio's paths
never carry quotes (pythonica already refuses them).

Observation reuses fabrica's own snapshot (`suturae_photographare`,
exported from tools/fabrica_sutura.c: same walk, thesaurus and
residents skipped) plus `~/.bin` always and the directories of any
`~/...` effect. The executor's own diary and logs live under
build/consilium/acta/ and are filtered out - they are written between
the two snapshots (INCEPIT line), so without the filter every step
would have "written" the diary.

Header written lazily (first INCEPIT/ACTUS): a plan refused by
consilium_exsequi leaves nothing in the diary. Fingerprint = HEAD +
`git diff HEAD --binary` + untracked-not-ignored names and contents;
no git -> 'ignota' and -resumere refuses.

Real interruption tested in fumus LXI: step B kills its parent (the
executor, kill -9) - resume carries A, reruns B and C. Plants (each
red on its own condition): resume ignored (A ran twice), ~/.bin not
observed (undeclared write passed), diff left out of the fingerprint
(changed tree resumed), mandatum check skipped (empty argv ran: 3
instead of 1).

## 2026-10-17 - correction: `"` in arguments IS writable

The S5c1 entry above says an argument containing `"` cannot be written.
Wrong (Fran): STML spells any attribute as an attribute-element,
`<argumentum><@valor=>dic "salve" &amp;&amp; vale</></argumentum>`
(docs/stml-reference.md par. 5.3) - content follows text law, so `"` is
fine and entities decode; inline values stay raw. `stml_attributum_capere`
already scans the child prefix, so the reader needed no change; credo
XIX now asserts it, and a live `-agere` run passed a quoted `sh -c`
string through. Lesson: I read the stml.h header line about inline
values and stopped - the reference's element-family table lists `<@a=>`
on the same page. Fran's follow-up wish: a builder that chooses the
spelling itself, so writers never have to.

## 2026-10-17 - S5c2: commissio onto `-agere`

commissio no longer executes plans: `_consilium_sanare` (the Python
wave loop) and `_classis_viae` (the Python copy of the scope
classifier) are gone. Plans carry mandata (`tools/fabrica_struere.sh`
for the judge, `bin/fabrica sanare X` for the rest), the already-done
format step is `factum="verum"` (judged and sealed, never run, ACTUM
'factum ante consilium'), and the gates step left the plan (gate
scheduling has its own judge). `_consilium_agere` writes the document
and calls `bin/fabrica consilium -agere -machina`.

The reader now infers `classis` from `res` when absent (only the reader:
`consilium_effectus()` keeps ARBOR for programmatic callers), so policy
rules moved from verbs to scopes: `arbor_secundaria` (domus VETITUM,
worktree only, FIRST), `domus_roganda`, `externa_roganda`. The producer
plan used to filter "outputs all in bin/ or build/" in Python; now it
judges a plan of ALL candidates and drops the steps policy does not
allow - the C classifier decides, and the rule that blocks is the
reason a producer is left out.

Heal steps get vestigium `bin/*`, `build/*` plus every declared output:
`bin/fabrica sanare X` legitimately builds preconditions (objects,
capsules, other tree binaries), and without the vestigium every such
write would be "extra propositum". A write anywhere else (a committed
source, home) still fails the step - which is the point.
