
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

## 2026-10-10 - adoption test 1: probatio_fabrica_gradus uses parare/purgare/NECESSE

Before credo slice 2, the v2 API was tried properly on one pilot suite
(Fran: test adoption before building more on it). The six pilot suites
had been converted mechanically only: section table + per-section piscina,
but parare/purgare NIHIL in 41/41 sections and 0 uses of CREDO_NECESSE.

Change: `MundusSectionis` (fake disk + sutura + intern) built by two
parare variants (`_mundum_ludicrum_parare`, `_mundum_c_parare`) into
`c->datum`; `_mundum_purgare` resets every fake global the sections set;
the shape `CREDO_NON_NIHIL(x); si (x != NIHIL) { ... }` -> 
`CREDO_NECESSE_NON_NIHIL(x);` + dedented body (15 sites). Numbers: guards
22 -> 7, fixture setups 6 -> 1 (8-12 duplicated lines gone per section),
file size unchanged (2399 -> ~2400: what went out of the sections came
back as parare/purgare + one-line pointers), assertions 245 -> 251 (+1
per section: the NECESSE inside parare).

What the API felt like - findings for slice 2:
1. In C89 the guard block was ALSO a declaration scope. Where the guarded
   body starts with declarations (1 of 15 here) NECESSE needs a bare
   `{ ... }` after it - the conversion is not purely "delete the if".
2. NECESSE makes purgare MANDATORY, not optional. Old style: a failed
   assertion did not stop the section, so the hand resets at its end
   always ran. With NECESSE they are skipped. Shown by plants: a section
   aborted after setting `_gradus_fractus` -> with purgare resetting it,
   6/7 sections pass; with that one reset removed from purgare, two LATER
   sections (post et composita, debita) fail. Docs (and maybe a lint)
   should say: a suite with NECESSE and file-scope state needs purgare.
3. `c->datum` is `vacuum*`; every section opens with
   `&((MundusSectionis*)c->datum)->discus` - verbose; a per-suite accessor
   macro would read better (C89 has no generics; that is the honest cost).
4. The 7 guards left are compound (`x != NIHIL && xar_numerus(x) == I`):
   making them NECESSE would turn a count check fatal - left as is.
5. Assertion totals move under refactoring (parare's own NECESSE counts) -
   fine for the oracle (both runners count the same binary) but worth
   knowing before anyone treats totals as fixed.

## 2026-10-10 - adoption test 2: four non-fabrica suites, four shapes

| suite | shape | result |
|---|---|---|
| probatio_xar | 34 independent blocks, title INSIDE each | 34 sections, mechanical (script) |
| probatio_stml | 67 sections, title BEFORE the block, some bare calls, shared `intern` | 67 sections, mechanical (script, 2nd mode); per-section intern - no block depended on a shared one |
| probatio_cliens_tabularii | ONE daemon shared by dependent steps | 6 sections, daemon per section in parare/purgare (+0.8 s); 6 build/ debts gone ($TMPDIR scratch) |
| probatio_vicus_iussa | scenario: one app fixture, steps I-VIII build on each other | ONE section; parare builds the app, purgare closes the volume |

Findings (for credo slice 2):
1. VACUA paid off at once: xar's "status_imprimere" block had asserted
   nothing for months; v2 refused to call it a pass. Now asserts.
2. purgare is what makes NECESSE safe - confirmed twice more: the daemon
   is killed and its scratch removed after an aborted section; the
   temporary volume is closed (plant without the close -> the
   .volumen/-shm/-wal files stay in /tmp; tests leaking volumes once
   turned a gate red on unchanged code).
3. GAP - no suite-level fixture. Sections must be order-independent and
   only per-section parare/purgare exist, so an expensive shared fixture
   (daemon, app) is either rebuilt per section (cliens: cheap enough,
   +0.8 s) or forces one big section (vicus: an app per step would be
   absurd). Slice 2 candidate: suite parare/purgare.
4. GAP - no named STEP inside a section. Scenario tests keep their
   "--- VI: ... ---" prints; a failure says "section X FRACTA at line N",
   not "step VI". Candidate: CREDO_GRADUS("VI ...") recorded in the
   fracture.
5. CANDIDATE HELPER - per-section scratch dir: the $TMPDIR/<name>.<pid>
   helper now exists in 4 test files (sententiae_horreum, villa_agens,
   git, cliens_tabularii). credo could hand each section `c->area` under
   TMPDIR, removed after purgare (API change - needs Fran).
6. CONVERTER: two old shapes are purely mechanical (title inside the
   block / title before it); a scratch script did xar (34) and stml (67)
   with refusals for anything else (statement between blocks, untitled
   block, redde inside, shared variables). Traps it hit: a stale
   "Compendium" banner before blocks added later, an 8-space banner,
   blocks that never touch the shared pool (-Werror unused var), and
   TITLE -> FUNCTION NAME brought English words into identifiers (the
   Latin lint refused 16) - a real converter should name functions by
   its own rule, not by the title.

Lint (Fran 10-10: would silva linting help?). Conventions these suites
showed that a silva check could enforce, from most to least valuable:
- NECESSE in a suite that has file-scope mutable state (`hic_manens`
  non-const) and no purgare that writes it -> warn (finding 2; the leak
  is invisible until a section aborts).
- a section function with no CREDO_* call reachable -> VACUA at run time
  already; a lint would say it BEFORE running (finding 1).
- `imprimere("--- Probans ...")` inside a converted suite -> credo
  prints the header itself (duplicate headers seen in stml).
- old-style suite (principale with credo_aperire + compendium) touched
  by a commit -> inform "convertible" (drives incremental migration).
- writes to build/ from a test without a debt -> fabrica already refuses
  at run time; lint could point at TMPDIR earlier.
These are pattern checks on the AST (calls inside functions referenced
from a CredoSectio table, file-scope declarations) - silva selectors can
express them; none needs flow analysis except "purgare writes X".

## 2026-10-10 - credo II T1: signa and OMISSA

`signa` (reserved since slice 1) is live: words from a CLOSED vocabulary
(`lentus rete fenestra repositorium` - the fabrica facultas words plus
'lentus'). Checked BEFORE anything runs, in the table AND in
`CREDO_SIGNA_OMITTE` - a typo in the environment would otherwise skip
nothing silently; both refusals print the vocabulary and write
`SUITA FRACTA 0/0` via `_credo_suitam_recusare` (the CREDO_SECTIO
refusal now shares it). A section carrying an omitted tag is printed
`--- OMISSA <t> (signum <s>) ---` and recorded `SECTIO ... OMISSA 0 0 0`
(10 fields, fractura empty); it counts neither as run nor as failed.
Suite exitus gains VACUA: nothing ran (all omitted) is not a pass
(exit 1). The summary line names omissions: `=== SECTIONES: 1/1
transierunt (2 omissae) ===`.

Fabrica: `_sectiones_explicare` counted every non-TRANSIIT section as
failing; OMISSA is now counted apart and named in the pass note
(`sectiones 1/1, omissae 1`).

Proof: credo_fumus VII (6 checks); gradus IIb. Plants (planta): omission
not recorded, unknown tag accepted, all-omitted suite TRANSIIT ->
credo fumus red; OMISSA counted as failure -> gradus red.

## 2026-10-10 - credo II T2: known failures (reason in the source)

`exspectata_fracta` is live. Fran changed the design in the interview:
the field is a REASON written in the source, never a ledger id that
credo or a gate would have to look up (principle …RBA4W73: what changes
an outcome lives in the repo). Empty reason -> refused before anything
runs.

After a section runs: FRACTA/ABORTA + reason -> NOTA_FRACTA (counted as
passing; its failed assertions go into `_credo_fracti_exspectati`, reset
by credo_aperire/claudere); TRANSIIT + reason -> INOPINATA (counted as
failing: "fractura exspectata sanata ... linea delenda"); VACUA stays
VACUA. Suite pass = all sections passed-or-noted AND fracti == expected.
The compendium prints `Exspectati: N (NOTA_FRACTA - causa in fonte)` and
`Conditio: OMNIA PRAETERIERUNT (fracturis exspectatis exceptis)` - an
exit-0 suite never says "Conditio: FRACTA". credo_omnia_praeterierunt
uses the same rule (old suites: expected 0, unchanged).

Fabrica member note: NOTA_FRACTA counts as passing and is named
(`sectiones 2/2, notae fractae 1`); the note is now composed from parts
(omissae, notae fractae) instead of the T1 if/else.

Census: `tools/credo_fracturae_notae.py [-tabularium] [VIA...]` - rows of
CredoSectio tables via silva (`declaratio:habet(typus-nominatus
[tok_titulus="CredoSectio"]) congeries congeries`), sixth field = reason,
adjacent literals joined like C, commas inside strings kept. Default scope
= converted suites (probationes/, */probationes/ calling
credo_suitam_currere); 0 known failures in the repo today. `-tabularium`
is a REPORT: ids in reasons (…XXXXXX) looked up via frigida -res, closed
-> "fortasse sanata"; never part of a gate (the fumus checks the census
only). Trap: `^\[(.*):` under re.S swallowed every row but the last -
path pattern must not cross lines.

Plants: NOTA not set, INOPINATA not set, empty reason accepted, expected
fracti not subtracted -> credo fumus red; census reads the 5th field ->
red; fabrica counts NOTA_FRACTA as failing -> gradus red.

## 2026-10-10 - credo II T3: suite fixture

`credo_suitam_currere_cum(titulus, sectiones, parare_suitae,
purgare_suitae)`; `credo_suitam_currere` is now a one-line wrapper (NIHIL,
NIHIL). The suite context has its own piscina (1 MB); parare_suitae runs
through `_credo_sectionem_agere` as a pseudo-section "(parare suitae)",
so a NECESSE inside it is caught like any section's. Failed -> no section
runs, the pseudo-section is recorded (ABORTA/FRACTA - fabrica names it),
purgare_suitae still runs, SUITA FRACTA. Otherwise its datum reaches every
section as `c->suita` (CredoContextus gained `suita` at the end; credo
builds the struct, no user code initialises it). `_credo_suitam_purgare`
runs once on every exit after parare: the normal end (before the summary,
so its output follows the last section), the section-pool failure, the
failed parare.

Real use: cliens_tabularii - suite daemon once; each section builds its
own CLIENT in its own parare from c->suita (the client's request counter
is section state: sections must not mutate suite state); "defectus citus"
kills a daemon, so it has a PRIVATE daemon (own parare/purgare, scratch
`probatio_ct_privatus.<pid>` - same pid-based name would have deleted the
suite's). Time: 0.96 s vs 1.81 s per-section (-47%) - the cost was not the
daemon but its start-up wait (port polled every 0.2 s), paid once now.
Original shared version: 1.77 s.

I named a parameter `nomen` AGAIN (second time today; latina.h macro =
typedef) - compiler error at once, but a candidate for the T7 lint list
(identifier equal to a latina.h macro), cheaper than relying on habit.

Plants: purgare_suitae not called at the end (hand-done: the call exists
three times, only one at that indent), c->suita not set, failed parare
ignored -> credo fumus IX red.

## 2026-10-10 - credo II T4: named steps (CREDO_GRADUS)

`CREDO_GRADUS("VI: nuntius")` = `credo_gradum_ponere(titulus)`: sets a
static `_credo_gradus` (the pointer only - title must outlive the step,
string literals do) and prints `  -- gradus VI: ... --`. `_credo_notare`
copies it into the notatio (`CredoNotatio.gradus`, new LAST field - the
struct is credo-allocated, so position is free, but end keeps the rule)
and appends ` (in gradu ...)` to the FRACTA line. The SECTIO record gains
field 11 = step of the first fracture (empty otherwise, three tabs in the
pass branch). The step is cleared before AND after each section, so a step
never leaks into the next section (plant: both resets removed -> fumus X
"sine gradu campus vacuus" red). Reset in aperire/claudere too: old-style
suites can call CREDO_GRADUS (it's just a global) - a migration bridge.

fabrica reader: SECTIO with 10 OR 11 fields (old binaries in build/ still
read); field 11 non-empty -> fractura gets ` [gradus ...]`, so the tail and
`fabrica sanare` name the step. vicus_iussa: its eight `--- I: ... ---`
prints became CREDO_GRADUS.

Found on the way: `CREDO_NECESSE` failures printed `Speratus: (null)`.
`chorda_ut_cstr` returns NIHIL for an empty chorda (datum NULL), and
`%s` with NULL is UB (macOS prints "(null)", glibc too, others crash).
Hit every failure with an empty expected/received value: NECESSE, fork
failure, string compare against "". Fixed by printing `%.*s` with the
chorda's own mensura (the house idiom). fumus I now asserts no "(null)"
in the human output (red before the fix).

Tool note: `silva.planta`'s red summary ("fractae: credo (ok I suita
mixta ...)") quoted an `ok` line, not the failing one, for credo fumus -
the acta had the right line (`FRACTUM X ...`). Read the acta, not the
summary, until the summarizer picks FRACTUM lines.
