
## 2026-07-11 — claudere-then-query trap (M4b chunk A find)

`credo_claudere` RESETS state ("conditio solum reficitur",
credo.c) — calling `credo_omnia_praeterierunt()` AFTER it always
returns VERUM (zero failures visible). A new probatio that ended
`credo_claudere(); redde credo_omnia_praeterierunt() ? 0 : 1;`
exited 0 despite a failing assert — caught only by a deliberate
harness-teeth test (broke an expectation on purpose; suite stayed
green; investigated). Correct order (probatio_piscina is the
canonical template): QUERY first, then compendium, then claudere:

    praeteritus = credo_omnia_praeterierunt();
    credo_imprimere_compendium();
    credo_claudere();
    redde praeteritus ? ZEPHYRUM : I;

Lesson doubled: (1) query-before-claudere; (2) the teeth test
(deliberate break → expect FRACTAE) is cheap and catches exactly
this class — a suite that cannot fail.

## 2026-07-31 — fflush on every fracture (desideratum 01KYWC5ZNX)

Measured failure mode (probatio_silva_quaestiones, this morning): a
failed CREDO_NON_NIHIL printed its F report, the probatio then
dereferenced the NIHIL and segfaulted — and because suite runners
capture through a PIPE, stdout was fully buffered and the report
died with the process. The runner showed only "Segmentation fault";
an lldb round (pty = flushed) was needed to re-see what credo had
already said. Fix: one fflush(stdout) at the end of the fracture
branch in _credo_notare — the single choke point (all typed
variants delegate). Dots stay buffered (hot path untouched); only
fractures pay a flush, and a fracture is exactly the moment the
bytes must be on the wire before anything else happens. Verified
with a deliberate crash demo through a pipe: exit 139 AND the full
FRACTA report present. Sibling of the cursor "-sola fflush before
_exit" lesson — same law, now in the framework itself.

## 2026-09-02 — CREDO_RUIT_CUM_INTRA

The bare RUIT_CUM waits CREDO_MORA_ORDINARIA_MS (5 s) before SIGKILL.
For a child that never returns (the infinite-loop plant in
probatio_credo_processus) the whole 5 s is always paid, and the poll
loop counts 1 ms iterations rather than time, so it ran 6.3 s. The
_INTRA form names the deadline; the test uses 300 ms. 6.3 → 0.76 s.
The iteration-counting drift (about 25% long) is noted, not fixed.

## 2026-09-02 — NIHIL PROBATUM: zero assertions is not a pass

Found by accident: launching the silva corpus tests from the wrong
directory printed `FRACTA: corpus non apertum` and then, four lines
below, `Conditio: OMNIA PRAETERIERUNT` with `Totalis: 0`. The exit
code was 1 only because the author had hand-written `redde I` after
the bail. The general class is worse: a test whose corpus loop finds
nothing, or whose fixture directory moved, reaches its normal ending
with zero assertions and exits 0 through `credo_omnia_praeterierunt`
— 259 test files exit through that helper, and it returned "no
failures" for "nothing ran". Fix at the choke point: totalis 0 now
returns FALSUM, and the compendium prints a third condition, `NIHIL
PROBATUM`. Nothing machine-parses the summary line (runners gate on
exit codes; only old docs quote it). Sweep of the latest root and
silva logs: no test ended with zero assertions, so nothing went red;
the other sub-suites keep no per-test logs — their first run is the
check. Also `CREDO_CULPA(nuntius)`: an unconditional recorded
failure for the bail-out sites (245 hand-written `imprimere("FRACTA`
lines in 184 files), so the summary and the exit code can agree;
migrating those sites is a lazy, mechanical pass, not done here.
Proof: probatio_credo reads the helper BEFORE its first assertion
(the only place totalis is 0 without resetting the table) and tests
CREDO_CULPA in a forked child so the deliberate failure never enters
the parent's tally; plant = early return before any assertion, gate
red, revert, green (silva.planta). Eighteen test files compare
`credo_numerus_fracti()` by hand instead of calling the helper and
inherit nothing from this — same lazy pass.

Addendum, same day: the first full root gate under the new rule went
red on three tests that had passed every assertion — calendarium,
exemplar, iter_directoria (124/109/38 assertions, 0 failed). All three
called `credo_claudere()` and THEN asked `credo_omnia_praeterierunt()`
for the verdict: a closed session has totalis 0, which used to read
as "no failures" and now reads as "nothing ran". They were green for
the same reason the hole existed. Fix: read the verdict into a local
before closing. Sweep (all suites, claudere line before the read
line): exactly those three plus probatio_manus, which re-opens before
its read and was already fine. Lesson for the substrate: a gate whose
"pass" is the absence of a failure record cannot distinguish "closed"
from "passed"; only a positive count can.

## 2026-10-09 - credo v2: sections, verdict file, NECESSE (header approved by Fran)

Additive: `CredoSectio` tables + `credo_suitam_currere(suita, sectiones)`;
the 488 existing credo files are untouched and move when touched. Each
section gets its OWN piscina (destroyed after `purgare`), runs
parare -> probare -> purgare (purgare always, also after a NECESSE abort),
and gets one TSV record (`CREDO_VERDICTA=<path>`): SECTIO suita titulus
exitus praeteriti totales ms filum:versus genus expressio; then SUITA.
Lines are written AND flushed per section, so a missing SUITA line =
the suite crashed, and the record still names what finished.
`CREDO_SECTIO=<titulus>` runs one section; an unknown title is a named
refusal (rc 1, SUITA FRACTA 0/0). Human output unchanged (Probans
headers, dots, FRACTA lines, compendium) plus one `=== SECTIONES: k/n
===` line and a one-line note per non-passing section - runners,
pythonica's relatio and fabrica (exit code + tail) need no change.

NECESSE = record, then `longjmp` to the runner. setjmp sits in a tiny
function whose locals never change after it (post-longjmp locals are
indeterminate otherwise). In a CREDO_NON_RUIT child the copied jmp_buf
would resume the RUNNER in the child and run the rest of the suite
twice - so credo now tracks `_credo_in_filio` (set in
credo_processus_incipere) and a child's NECESSE `_exit`s. Time via
gettimeofday: in the POSIX lexicon; clock_gettime is not, and a tempus
dependency would ripple into every test's closure.

Gate `credo` = tools/credo_fumus.sh (three inline fixture suites:
mixed outcomes, all-green + filter + no-file, crash mid-suite). Plants
(silva.planta, fumus as callable gate), all red: purgare skipped after
abort; NECESSE not jumping; child jumping; no per-section flush; tab
not escaped; VACUA read as TRANSIIT. TRAP: the no-flush plant stayed
GREEN while the crash fixture used abort() - macOS abort() flushes
stdio. A real crash (SIGKILL, SEGV) flushes nothing: fixture now
raise(SIGKILL).

Pilot: the six fabrica suites (38 sections) converted by script - each
`{ ... }` block in principale became `_probare_<title>` with the block
kept VERBATIM as a nested block (no re-indentation), `piscina =
c->piscina` only where used, the Probans line dropped (the runner
prints it). Oracle: per-section passed/total from the OLD run (dots
between headers; sums matched the known totals) == the new verdict
records: 38/38 sections, same titles and order, 836 assertions. A
fabrica plant now lands as `SECTIO ... familias FRACTA 40/42
<file:line> credo_nihil <expression>`. Lint: function names use forms
the lexicon knows (provenientia_exclusa, regenerationes_praevidere, no
'chassis'); glossary: 'suita' entry, 'strategias' form.
