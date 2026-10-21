
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

## 2026-10-10 - credo II T5: credo_area

`credo_area(c)` -> `<TMPDIR>/<suite>.<n>.<pid>` for a section (n = table
position from 1), `<suite>.suita.<pid>` for the suite context (callable
in parare_suitae / purgare_suitae). Suite title sanitised to
[A-Za-z0-9_-]; trailing '/' of TMPDIR stripped (macOS TMPDIR ends in '/',
and git normalises '//' - the git test had tripped on that before).

Design points:
- Two static slots (section, suite), each {context pointer, path,
  created}. The path is computed BEFORE the context runs; credo_area
  matches the context pointer, creates lazily (removing pid-reuse
  leftovers first), returns the same path afterwards. No struct change
  - the approved header was the function, and a field would invite
  reading c->area before creation.
- Removal after purgare is by PATH, not by the `created` flag: a
  CREDO_NON_RUIT child can create the area (fork copies the slot), the
  parent never learns, and must still remove it. Plant: delete only if
  the parent created it -> fumus XI "TMPDIR vacuum" red.
- credo has its own lstat/opendir/unlink/rmdir tree removal (~40
  lines) instead of filum_arborem_delere: the subsystem test builds
  (silva, materia, md, html, officina, oratio, tessera...) link credo
  from explicit lists WITHOUT filum/lectiones. Symlinks are unlinked,
  never followed. A failed removal prints `credo: area non deleta`.
- Removal failure is a printed warning, not a fracture - revisit if
  leftovers ever matter (a leak elsewhere: ledger …V3QXC, 312 empty
  `<word>-probatio-<epoch>` dirs from some unknown suite).

Three of the four hand helpers were in OLD-style suites, so T5 converted
them (helper replacement needs a context):
- git: suite fixture opens the repository + resolves HEAD under NECESSE
  (a broken repo = '(parare suitae)' ABORTA, which is what the old
  hand-rolled early return was for); 9 sections, old titles kept. The
  repository allocates in its CREATION piscina (the suite's), so section
  pools never leak into it - checked before converting.
- villa_agens: suite fixture = VILLA_SSH + cfg; each section takes a
  struct COPY of cfg (never mutates suite state); every section already
  set its own VILLA_STIPES_MODUS, so order-free. Section X's daemon in
  its section area.
- sententiae_horreum: ONE scenario section (its header says the blocks
  are NOT independent - V kills what IV tests), daemon in parare via
  credo_area, pkill in purgare, steps I-VIII as CREDO_GRADUS.
Assertion counts: git 101, villa 150 (149 + fixture NECESSE),
sententiae 35 (33 + two NECESSE in parare).

Fixture lesson in the fumus: a CREDO_NON_RUIT child exits via _exit, so
its stdout buffer is lost unless flushed - the "AREA filius" line was
missing until the fixture flushed. Not a credo bug, but worth knowing
when debugging child output.

The daemon start-up (tabulariumd -portus 0 in a dir, poll the port file
0.2 s x 25) now exists THREE times (cliens, sententiae_horreum,
villa_agens). Candidate for a shared test helper once a fourth appears.

## 2026-10-10 - credo II T6: CREDO_ORDO=mixtus

`CREDO_ORDO=mixtus:<seed>` -> Fisher-Yates over a table-position index
array, xorshift32 in credo itself (sors is not in every subsystem link
list). The seed is premixed (`seed * 2654435761 + golden`, 8 warm-up
rounds) because raw xorshift from tiny seeds (1, 2, 3...) gives nearly
identical first outputs - neighbouring seeds must give unrelated orders.
Bare `mixtus` picks a seed from time ^ pid and NAMES it
(`=== ORDO mixtus semen N ===` + record line `ORDO <suite> mixtus N`)
so any failing run can be repeated. `tabula`/empty = table order;
anything else refuses the suite before running (a typo must not
silently mean "table order"). The section index `i` stays the TABLE
position, so credo_area paths do not move with the shuffle.

Fumus XII lesson: my first "different seeds give different orders"
check counted orders that differ FROM TABLE ORDER - a plant that ignored
the seed entirely (same shuffled order every time) passed it 8/8. Now it
counts DISTINCT orders; the plant goes red at 1/8. Second time today a
check proved less than its name (cf. the planta summary, …FV3X).

The dependent fixture (`legens` relies on `ponens` having run) passes in
table order and fails under seed 1 - the shape the tool exists for.

FIRST REAL FIND (tools/credo_ordo_mixtus.sh, seeds 1-5): 
probatio_fabrica_gradus FRACTA under all five - `_gradus_areae_paratae
== II` received 17. Ten file-scope variables (call counters of the fake
steps, `_c_*` compile/link counters, three "last seen" buffers) were
never reset by `_mundum_purgare`; in table order the section reading
them ran first and saw zero by luck. Adoption test 1 (78cae599) had
reset the 14 POINTER fakes and missed every counter. All ten now reset;
20/20 seeds clean, then all 13 suites clean over seeds 100-114.
This is precisely T7 lint check (1) - "mutable file-scope state that no
purgare writes" - with a real historical instance to plant against
(revert the ten resets -> the lint must name them).

## 2026-10-10 - credo II T7: conventions as examen codes 92-95

Detail in silva/fontes/silva_c89_semantica.worklog.md. What it found on
first contact with the 13 v2 suites: probatio_fabrica_genera (5) and
probatio_fabrica_iudicium (4) mutate the shared fake world
(mundi_ambitus_ficti, mundi_effectus_effusio, mundi_identitas_ficta, ...)
with NO purgare at all - pilot suites converted mechanically. Latent, not
active: CREDO_ORDO=mixtus found nothing because no later section happens
to read those values today. Both now have `_mundum_reponere` as every
row's purgare. stml had two leftover `--- Probans` sub-headings inside
merged sections -> CREDO_GRADUS. And plant R1 showed the first version of
92 MISSED the very T6 bug it was built for (fake installed as a function
pointer) - a lint must be planted against the real case it exists for,
not only against unit snippets.

## 2026-10-10 - credo II T8: converter + convertible note

tools/credo_convertere.py replaces the scratch regex script. Structure from
silva (selectors over `definitio-functionis:definit(main)` - NB `main`, not
`principale`: :definit matches the declarator AFTER latina expansion, which
cost a probe), text from line ranges, so comments/format are untouched.
Shapes: A (title in every top-level block), B (title before the section;
title nested in a block -> CREDO_GRADUS). Everything else is refused with
a named cause and the file untouched; -probare writes nothing; an already
converted suite exits 2.

The AST version is MORE right than the scratch: stml's original has a title
split over two source lines; the scratch's one-line regex merged it into
the previous section, which is precisely what T7's code 95 later flagged.
So stml's oracle differs from the scratch output in two documented places
(the split title is section 68; the nested title becomes a step); xar is
byte-identical.

Function names: '_sectio_' + Latin ordinal (prima, ..., vicesima_prima,
..., centesima_...). Tested against the lint first: 'XLVII' is a NEW word,
all 19 ordinal words are known.

Commit note (lint 5): silva.notae_convertibilium(viae) - touched old-style
probationes/probatio_*.c that the converter accepts get one 'nota:' line
naming the command; never blocks. Path match is suffix-based so the fumus
can test it from a temp dir without writing into the tree.

Two tooling finds (ledger): selecta.sh relinks its binary on EVERY call -
running the converter census 6-wide made the concurrent credo fumus fail
in 10 places (…WB6Q3H; alone it is green); silva.planta on a .py file goes
through Editio's whitespace-tolerant C-lexeme replace and re-indented the
plant (IndentationError), so all four planta runs "broke the build" - the
plants were done by hand instead (…6DP7RG; memory updated).

## 2026-10-10 - credo II close: converter census (246 old root suites)

`./tools/credo_convertere.py -probare` over every root probationes/*.c with
credo_aperire and without credo_suitam_currere, run SERIALLY (6-wide made
selecta race, …WB6Q3H):

  convertible 50 (shape A 42, shape B 8)
  refused 196:
    149 no '--- Probans T ---' titles - of these 127 DO use '--- T ---':
        ~52 unnumbered titles (look like independent sections: a third
            mechanical shape is plausible - widen the title pattern?)
        ~49 numbered ('--- I: ...', '--- I. ...', '--- 0. ...') = the
            SCENARIO style (vicus, villa, sententiae): steps often depend
            on each other; converting them to independent sections is
            wrong when they share statics/daemons, which the shared-local
            check cannot see -> CREDO_GRADUS in one section, by hand
        22 no '---'/'===' titles at all
     11 statement before the blocks, 10 between blocks, 8 block without a
     single title, 7 statement before the first title, 9 principale local
     shared by sections (= suite fixture by hand), 2 redde in a section

Reading: incremental migration through the commit note reaches ~20% of
the old suites mechanically; the rest are judgement conversions (the T5
kind). Widening to unnumbered '--- T ---' titles is the obvious next
lever - Fran's call (numbered titles must stay excluded).

## 2026-10-14 - a crashed section is named (analytica T4 2b-ii)

Fran: "is there a way to catch segfaults in test sections and ... log
'this section segfaulted'?" The doctrine in credo.h stands (never catch
SIGSEGV in-process; fork is the only sound isolation), so no handler:
credo now writes `SECTIONES <suita> <n>` first (declared count, filter
respected) and a FLUSHED `INCIPIT <suita> <titulus>` before each
section. A real crash (SEGV, KILL) flushes nothing, so "started, no
SECTIO, no SUITA" = the section that died. The reader is fabrica's
(lib/fabrica_probationes_c.c): it synthesizes a RUIT section and
counts `non_cursae`; the member cause reads "sectio 'beta' RUIT; 1 non
cursae" instead of "SUITA absens (ruina post sectionem ultimam
notatam)", and the analytics get a fractus `#beta` run. credo fumus II
(new line types with exact field counts) and VI (SIGKILL fixture, now
with a section after the crash). Opt-in per-section fork isolation
(full report despite a crash) parked: fork per section + Cocoa
fork-unsafety.

## 2026-10-17 - Linux -O2 build (hospitium v3 T9)

- `_credo_arborem_delere` builds child paths with memcpy after the
  existing length check: gcc -O2 under musl/glibc reports
  -Wformat-overflow on the sprintf because it cannot prove the bound.

## 2026-10-19 - credo III slice A: values in failures

Wish 01M5B3DF4Z29 (parent 01M5B3AQ1RXB). Before: CREDO_CHORDA_AEQUALIS,
_CONTINET, _INCIPIT, _TERMINATUR were thin wrappers over CREDO_VERUM, so a
failure printed only the expression and an empty Speratus (484 uses);
CREDO_CHORDA_AEQUALIS_LITERIS copied Receptus into a 256-byte buffer and
silently cut it at 255; AEQUALIS_I32 / INAEQUALITAS_I32 printed unsigned
i32 with %d (>= 2^31 showed negative); F64_PROXIMUS never printed the
difference.

Now: one renderer (`_credo_chordam_reddere`) - by length, quoted, escaped
(\t \n \r \\ \" and \xNN for bytes < 0x20 and 0x7f; bytes >= 0x80 raw so
UTF-8 stays readable - the Differentia line carries hex), windowed around
the first difference when > CLX octets ("fenestra [a, b)"). A third line
`Differentia:` via `_credo_notare_plenum` (the old `_credo_notare` is a
wrapper, so manus.c and every other caller are unchanged). New:
LITERAE_AEQUALES, OCTETI_AEQUALES (hex window, `[ff]` marks the byte),
full S64 and I64 families, F32_PROXIMUS, F64_RELATIVE, F64_ULP (IEEE
ordering trick: sign-magnitude -> monotone u64, so -0.0 == +0.0; NaN never).

Gotcha caught by the test: helper macros that stringify (`#a`) must do it
in the OUTER macro - arguments handed to an inner macro are already
expanded, so `(s64)-V` printed as `(s64)-5`.

Naming: `CREDO_REDDITIO` (redditio is real Latin but unknown to the lint
lexicon) renamed `CREDO_ALVEUS_EFFUSIONIS` rather than glossed; `ulp` got
an `ignotum-permissum` glossary entry (not a word in either corpus).
Test: tools/credo_fumus.sh stage XIV (red first: rc 99 before the
macros existed), 16 named failures + 21 passing forms.

## 2026-10-19 - credo memoria T2: memory per section

project-specs/credo-memoria-spec.md par. III. Each section's SECTIO line
gains three KiB fields (XII-XIV): arena peak (`piscina_summa_apex_usus`
of the section's own piscina, read before `piscina_destruere`), process
footprint at section end, and the rise of the lifetime peak while the
section ran (via `memoria_processus_legere(ZEPHYRUM, ...)` before
`_credo_sectionem_agere` and after `purgare`). KiB, not octets, because
fabrica's `_numerum_legere` accepts at most nine digits (49 GB in octets
is eleven; nine digits of KiB is ~950 GB). The fabrica reader now accepts
X | XI | XIV fields in the SAME commit - before, any other count made the
whole member "deformis".

Gotchas: the OMISSA path reuses the previous section's `summa`, so it
zeroes the memory fields itself; credo now links `memoria_processus`, so
every hand-written dependency list that names "credo" needed
"memoria_processus_macos" too (briar, ludus_tessera, officina, oratio,
tessera, credo_fumus struere, venenum BASIS; gesta regenerated).
Measured in credo_fumus XV: a 32 MiB section reads 32768 / 33824 / 32800,
the light section after it 0 / 1056 / 0.

## 2026-10-20 - memory ceiling per section (credo memoria T4)

Defaults come from the T3 measurement (`mensor quaestio graves` after a
forced run of the root suites and the seven subsystem composites):
3116 sections, the heaviest normal one is 135 MiB, and two are over 256
(probatio_cellae phasis 389, pictor_imago IV 317). Fran chose ordinary
256 / gravis 1024, with those two tagged `gravis`.

The check sits between `purgare` and `_credo_sectionem_summare`, which
is why the memory readings moved before the summary: the failure is
recorded with `_credo_notare_plenum` while the section's notations are
still open, so the summary, exit code and SECTIO fracture all pick it
up with no new path. filum = suite title, versus = section number, so
the line reads `at <suite>:<n>`. The message names whichever of arena
peak / peak rise is larger: in the fumus fixture the rise (32800 KiB)
beat the arena (32768), and my first regex wrongly required "arenae".

Semantics chosen where the spec was ambiguous: `CREDO_MEMORIA_MB`
replaces only the ordinary ceiling (0 disables it); gravis always uses
GRAVIS. strtol garbage reads as 0 = off.

Rename slip: a regex rename `vestigium_kib -> vestigium` with
lookbehind `[\w.>]` protected `summa.vestigium_kib` but not the struct
field's own declaration. The compile caught it. For locals next to
same-named members, rename by sites, not by pattern.

The subsystem suites (133) are all v1 (SYNTHETICA), so they're
invisible here; whole-process `/usr/bin/time -l` shows six silva corpus
suites at 4.1-7.6 GB (park 01M5CQYDD841) - relevant for T6, not T4.

## 2026-10-20 - the ceiling under AddressSanitizer

The quinta merge check went 42/43; venenum was red: probatio_plexus
"partitio contra Artin" had a 269 MiB peak rise against 256 MiB. In the
plain root run the same section is 37 MiB. Same binary, two runs: arena
37.3 MiB both times; rise 268.9 MiB with ASan's default quarantine, 38.7
MiB with `ASAN_OPTIONS=quarantine_size_mb=0`. ASan's quarantine and
shadow memory are in the process footprint, so "how much the process
peak rose" isn't a property of the section under a sanitizer. My T4
design didn't consider that.

Fix: compile-time `CREDO_SUB_ASAN` (`__SANITIZE_ADDRESS__`, or
`__has_feature(address_sanitizer)` behind `defined(__has_feature)` so
non-clang compilers and silva's model skip it), and under it the weight
is the arena peak alone. The parameter then goes unused, which
-Werror rejects, so the ASan branch has `(vacuum)auctus;`. The first
ASan fixture build caught this, and venenum's build would have failed
on it the same way.

Fixture lesson: a fixture can't produce a quarantine-sized rise. 16 MiB
blocks bypassed the quarantine (rise 0), and 4096 × 32 KiB freed blocks
gave about 1 MiB, presumably because idle uniform pages get compressed.
The stage instead makes the rise outside the arena directly (64 MiB
malloc, non-uniform fill, held for the section), which tests what
matters: under ASan, the rise isn't judged.

## 2026-10-20 — suite fixture: the interning exception, and how deep examen 94 looks

Step 0 of the subsystem migration (01M5B3BT9R71) was to write down Fran's
10-19 decree (01M5B814PFMY): the suite fixture is read-only except that
sections may intern into the fixture's own interning table. Interned
strings are compared by pointer, so a per-section table would silently
break identity with the fixture's names. Interning is also order-free:
the same text gives the same chorda whichever section interns it first.

While reading the examen rule I found that code 94 only checks the first
level. It flags `c->suita` being assigned, or converted (explicitly or
by scope) to a pointer to non-const. Every fixture suite reads through
`constans T* o = (constans T*)c->suita;`, and `constans` on the struct
doesn't reach its pointer members: `o->lista` is still `Xar*`, so
`xar_addere(o->lista, ...)` compiles and examen says nothing. The
exception is already in use this way: probatio_natura_canones.c passes
`ex->intern` from its fixture into canon_* / stml_legere in sections.

Fran chose (a) for now: document the exception (credo.h, examen's
credo-rules comment, the 94 message and English gloss) and say that 94
is first-level only. The deeper rule is (b), with the exception built
in, as part of step 1: track locals that come from c->suita, and flag
writes through their member chains plus mutable-pointer members passed
to non-const pointer parameters, unless the callee is internamentum_*.
Its false-positive rate depends on how const-correct the house APIs are.
That has to be measured on the root fixture suites before deciding how
strict to make it.

## 2026-10-20 — examen 94 now follows the fixture's pointer members ("94 profundum")

Step 1 (b) of the subsystem migration. Code 94 used to stop at `c->suita`
itself. It now also follows pointer members through dataflow provenance.
Same pattern as code 82: `c->suita` is the root (FLUXUS_STIRPS_PRIMA),
carried through locals, `->`/`.`/`[]`/`*`/`&`/casts; a join where paths
disagree is AMISSA and silent, and so is an escaped variable. At each
use of a fixture-rooted local the chain is followed up to its outermost
expression, which is flagged if it is:
- a write through a dereference (`o->p[0] = 1`, `*o->p = 2`, `++`), or
- a mutable pointer passed to a prototyped non-const parameter.
Unknown, K&R and variadic callees stay silent. Limit (pinned by a test):
a helper that receives the fixture as a parameter isn't followed, so the
rule works within one function only.

The decree's exception is keyed on the ARGUMENT TYPE, not the callee's
name. Passing the fixture's `InternamentumChorda*` anywhere is allowed:
canon_legere(..., intern) and stml_legere(..., intern) intern too. The
first draft keyed on `internamentum_*` and flagged ~20 legitimate sites.

Measured over all 344 v2 suites: 80 hits in 5 suites, then 58 after
the type-keyed exception. All of them are call arguments; there were no
direct writes. Classification:
- Pure reads behind a non-const API:
  - `insula_attributum` now takes `constans InsulaRepositorium*` (Fran
    OK'd it; the body is unchanged because const is shallow:
    `repo->radix` stays `StmlNodus*`).
  - Three test helpers in probatio_natura_canones.c went const.
  - `_documentum_iudicare` (forwards `Canon*` into
    canon_iudicare → nodum_iudicare) and `_titulo_colligere` (its Xar
    consumer calls stml_attributum_capere) can't go const without
    library changes. They are tolerated, park 01M5DWFJ3YVRT9HSBRYKEA7J8D.
- Real hidden mutation:
  - git read functions allocate scratch in `repositorium->piscina`;
  - pictor_imago_fons allocates in `pf->doc->piscina`.
  So every section reading a git object grows the fixture's arena. The
  tests stay deterministic, which is why nothing failed. 30 sites are
  tolerated in probatio_git.c and probatio_pictor_figurae.c, park
  01M5DWCWHCV4Q1Q8Z8767WTA47 (library fix: scratch memory into the
  caller's piscina).
A tolerated site that stops firing turns IRRITUM, so once a library is
fixed the tolerations have to be removed. After that, all 344 suites
pass with 0 hits and 0 IRRITUM.

Plants: disabling the collector turns exactly the 4 firing cases red;
disabling the type exception turns exactly the interner case red.

## 2026-10-21 — conversion checker for the subsystem migration (tools/credo_conversionem_probare.py)

Step 1's other half. The audit's verifier (scratchpad, 10-17) built a
private copy of a root suite with `sonda.sh -domus`. Subsystem suites
need their subsystem's include and link lists, so this checker builds
both versions through the subsystem's own runner. To get the HEAD
numbers, it puts the HEAD text in the file, builds and runs it, then
restores the converted text in a `finally` (plain write + utime, never
`cp -p`).

It runs the built binary itself from the root, with RHUBARB_RADIX and
CREDO_VERDICTA set: `<sub>/build/<suite>` for direct runners,
`build/fabrica/area/probationes_<sub>/<suite>/<suite>` for composites,
`build/probationes/<suite>` for root, choosing whichever was built after
the run started. Reasons:
- the runners filter by substring (`probatio_gesta` builds 3 suites,
  `probatio_silva_arbor` 5), so their output can't be attributed;
- fabrica passes an env allowlist (`<ambitus>`), so CREDO_ORDO never
  reaches a composite member.
v1 binaries don't write CREDO_VERDICTA (only credo_suitam_currere does),
so v1 numbers come from the compendium. "Totalis:" is taken from its
LAST occurrence anywhere on the line, since progress dots share lines.

Policy built in: **a conversion keeps the counts.** The static CREDO_*
count and the run's Totalis/Praeteriti must equal HEAD's. Splitting a
loop into per-item sections changes Totalis, so restructuring goes in a
separate commit after the conversion. That way the checker proves
nothing was lost. A memory hit (SECTIO failure genus credo_memoria) has
its own verdict, MEMORIA: the suite is pulled out for hands-on work and
never handed to an agent (Fran 10-20).

css pilot finding: credo_convertere refuses all 10 css suites. Most
build a lexicon `ratum` before their blocks (fixture needed), and
corpus/computus/canon have no '--- Probans ---' titles. css is hand
conversion. First one: probatio_css_corpus, one section (one loop
with tallies), BONUM 7>7 assertions, 24>24 Totalis, 3 mixed seeds.

Plants on the converted suite, each through silva.planta, red then
green after revert:
- drop an assertion: MALUM 7->6, 24->23.
- two sections sharing a static: mixed order breaks on seeds 1,3,4,8
  of 8, and examen independently rejects it (92).
- 300 MiB in the section's piscina: MEMORIA, the section is named.

## 2026-10-21 — css pilot done: 10 suites, 43 sections, all BONUM (briefing for step 3)

All 10 css suites converted by hand. credo_convertere refused every
one. Every result was BONUM under tools/credo_conversionem_probare.py:
equal assertion count and Totalis, k/k sections, 3 mixed seeds, examen
ACCIPE. Sections per suite: corpus 1, computus 2, registrum 5,
diagnostica 3, stml 3, totalitas 1, selector 8, adaptare 10, canon 2,
arbor 8. The checker takes ~20-35 s per suite.

Patterns, roughly in order of frequency:
- **Prelude before the blocks** (lexicon `ratum`, `consilium`, canon
  loading, interner): becomes the fixture in `parare_suitae`, with
  `c->datum = f` set only on success. Sections read it as
  `constans FixuraX* f = c->suita`. The prelude's assertions move with
  it and still count toward Totalis. If only ONE block uses the prelude
  value (arbor: ratum only in the STML block), that section builds it
  itself and there's no fixture.
- **Guarding the fixture allocation:** a plain `si (f == NIHIL) redde;`
  in parare, and `si (c->suita == NIHIL) redde;` in sections. That
  gives VACUA sections and the suite FRACTA. NOT CREDO_NECESSE: it's
  counted (the checker caught 13->14 on computus).
- **File-scope statics written between blocks:** examen 92 flags them
  in sections.
  - Counters (stml SEDES_ELEMENTA): become per-section locals passed
    by pointer.
  - Configuration (totalitas RADIX_VIAE): becomes a function.
  - Globals read by helpers (selector canon/intern/consilium): go into
    the fixture, passed to helpers as `constans FixuraX*`.
- **Aggregate assertions across blocks** ("cases >= CC", "documenta >
  VI"): the blocks they span stay ONE section (totalitas, canon's
  iudicium). Splitting is a separate commit that decides where the
  floor lives. stml's "elementa > 0" moved into the corpus section
  instead (stricter, same count).
- **Several '--- Probans ---' titles in one block:** the first becomes
  the section title (better: the banner's words), the rest become
  `CREDO_GRADUS ("...")`. Not counted, and examen 95 flags hand-printed
  titles.
- **A block-local `c`** (MateriaNodus* c, MateriaArborConsilium c)
  shadows the section's parameter: rename it (compositus, consilium).
- **C89:** declarations go before the NIHIL guard (-Wdeclaration-after-
  statement). A section that doesn't use piscina/ratum mustn't declare
  them (-Wunused-variable, -Wunused-but-set-variable). Detecting
  variables by word match catches comments too ("spatia significantia").
- **Helper signatures:** a fixture member passed to a non-const helper
  parameter trips 94-deep. canon's `_documentum_iudicare` took
  `Canon*` and now takes `constans Canon*`.
- **Before commissio:** run `VOCABULA_VIAE_ADDITAE=... ./oratio/
  vocabula.sh -nova`. New section names hit unknown words 5 times
  (rancorem, inlineos, adaptationis, derivae, nidificationem). The
  formator wraps table rows badly after a rename, so write those rows
  by hand. Banners are exactly 50 '='.

## 2026-10-21 — toml and saltuarius done; step 2 complete

toml (13) and saltuarius (13) are v2. Every suite is BONUM under the
checker (same counts as HEAD, k/k sections, 3 mixed seeds). Step 2 is
finished: css 10 + toml 13 + saltuarius 13 = 36 suites converted by
hand or semi-mechanically. Only toml_api and saltuarius_res converted
with credo_convertere.

New patterns beyond the css list above:
- **The prelude builds a mutable world** (nexus, liber, origo):
  `parare_suitae` asserts its creation once on a throwaway instance,
  and each section builds its own through a non-asserting helper
  (`_mundum_parare`). A fixture can't hold it, because examen 94
  forbids sections mutating fixture state.
- **A later block depends on an earlier block's state on purpose**
  (bibl LRU: "a b iam insunt"): the section rebuilds that state with
  non-asserting calls. Counts are equal, and mixed order proves it.
- **Scenario suites** (quaestio, claves, visum, columnae, limes,
  structura): an interactive session where every block continues the
  last. These are ONE section, with the sub-titles as CREDO_GRADUS.
  Replaying the session per section would be verbose and fragile.
- **Disk fixture at a fixed /tmp path** (salve, with a cross-block
  file): moved into `credo_area(c)` in parare_suitae. The path is
  private to the run and deleted after purgare. It used to be shared
  across concurrent runs.
- **Static counters that only label failures** (toml arbor): removed;
  the failure message names the case's source text.
- **A hidden order dependence found by the checker** (saltuarius
  liber): 'iunctio' only parsed because 'strata' had prepared silva on
  the SHARED nexus earlier. In v2, mixed order breaks it on every seed.
  The section now prepares silva on its own nexus.

Tooling (scratchpad, session-local; promote for step 3):
- conv_sectiones.py: banner + block, optional ratum fixture, -opus,
  -nexus.
- conv_unus.py: whole main into one section.
Bug found in my own tool: renaming a shadowing local `c` also hit a
string literal ("/a/b/c.txt" -> "/a/b/compositus.txt"), and the
checker caught it as a failed assertion. The rename now skips string
literals. An audit of literals added or removed, across all 36
conversions against 7f1c85c5, found only intended changes and one
lost print, restored (toml canon "canon ABEST").

Memory: toml_stml was the only MEMORIA hit. It was handled hands-on
(xar's growable segment table, fac4fc74).

## 2026-10-21 — converters promoted to tools/ (step 3 prerequisite)

The scratchpad converters are now house tools, so step 3 agents in
their own worktrees can use them (Fran's OK, 10-21):
- `tools/credo_convertere_sectiones.py`: banner + block -> sections.
  Optional ratum fixture (LEXICON), `-opus=T,N`, `-nexus`.
- `tools/credo_convertere_scaenam.py`: scenario suite -> ONE section.
Each header lists what the tool doesn't handle (the manual patterns in
the 10-21 entries above) and the after-steps: formator, the checker,
the Latin lint. Regression: re-running on the v1 text of toml
registrum (fixture), toml coctum (-opus) and saltuarius nexus (-nexus)
reproduces the committed conversions byte for byte. saltuarius
structura (scaenam) differs by one blank line after the declarations,
because the committed file predates that fix; the new output is the
house style. Refusals are named (exit 1, nothing written) where the
scratch versions asserted.

## 2026-10-21 — step 3 batch 1 (agents) and converter fixes

Four agents (html, materia, md, gesta), one subsystem each in its own
worktree. 46 of 49 suites converted and BONUM; 3 MEMORIA pulled out for
hands-on work:
- gesta tabularium: arena 294.5 MiB, one world through 55 headings;
- html html_md: arena 259 MiB, "porta: initus md per html";
- md md_stml: footprint +308 MiB, corpus STML circuit (a twin of toml_stml).
Two more hidden order dependences were found and fixed: materia pictor
(block II read block I's aurum_mensura) and md census ('cumulationem'
continues block 1).

Setup trap: the agent worktrees were cut from the main checkout at
91907748, not from quinta, so 81 commits were missing (tools, checker,
examen 94). The html agent caught it. All four moved their clean
branches forward with `git reset --keep 8d1a7b31` before any edit.
Future briefs: check the base first.

Converter bugs the agents found, now fixed (refuse rather than drop
anything silently):
- credo_convertere_sectiones.py:
  - Two-line titles ("..." "---\n") were missed. Worse, "remove the
    first title" then deleted the first single-line STEP title (html
    arbor O7b/O7c). The counts didn't show it; a title audit did.
    Titles are now joined across literals.
  - A main prelude (radix_viae, a variable reached through a macro)
    was dropped while the tool reported success. Now: a prelude or tail
    holding anything beyond the known boilerplate is REFUSED, naming
    the lines. Known: piscina setup, credo_aperire, the ratum lines
    with LEXICON, radix via RHUBARB_RADIX, opus with -opus, nexus with
    -nexus; on the tail side, compendium, destruction, redde.
  - `(vacuum)c` is decided from the final generated text.
  - Title removal no longer leaves two blank lines.
  - A shadowing local `c` needs `-c=NAME` (a name that fits its type),
    or the tool refuses. It no longer renames to `compositus` blindly.
- credo_convertere_scaenam.py:
  - Every "FRACTA: piscina..." guard variant is stripped.
  - A tail with statements after the compendium (frigida's _purgare)
    is refused; such cleanup belongs in a purgare slot, by hand.
  - No blank line before the closing brace.
  - `-c=NAME`, as above.
Regression: toml coctum reproduces byte for byte. toml registrum and
saltuarius nexus differ only where the committed files carry the old
double-blank bug. md lexema v1 is refused, naming its radix_viae
prelude. html arbor v1 converts with 48 CREDO_GRADUS and 0 hand-printed
titles left.

Also reported, not fixed here: the post-edit EXAMEN hook says REICE in
agent worktrees, because its path (with the .claude/worktrees/ prefix)
doesn't match the canalis_debita keys; ./silva/examen.sh says ACCIPE.
The scratchpad is shared between agents (two overwrote each other's
helper scripts). The worktree sandbox blocks bash arguments containing
'|' or 'git'.

## 2026-10-21 — step 3 batch 2: oratio, crusta, officina, briar, tessera, ludus_tessera

Six agents. Converted and BONUM:
- oratio 16/19, crusta 19/20, officina 13/15;
- briar 19/20, tessera 15/15, ludus_tessera 8/8.
Each was re-verified in quinta before its commit.

Hidden order dependences and shared state found and fixed:
- officina compendium ('ordinem' used block 1's declarations);
- officina puncta (the instruction count came from fundamentum);
- briar plagulae (block 4 changed block 1's `membra` in place);
- oratio resolutio (a `hic_manens` text buffer written from two
  sections; examen 92 caught it).

MEMORIA, pulled out:
- STML circuit: oratio_stml (+831 MiB), md_stml (+308).
- In-process legati index: officina renovatio (+405), legatus (RSS 708).
- Accumulation candidates: crusta effectus (+1370, every house .sh in
  one piscina), briar amalgama (+321), oratio_partes (+390).
- oratio_vocabula (268): a shared vocabulary fixture is blocked by the
  non-const `oratio_vocabula_creare`.
Done by hand earlier (ac76e355): html_md and gesta tabularium, both
accumulation (a piscina never reset, or the suite piscina handed to an
API as its ephemeral per-message one).

Converter fix (credo_convertere_scaenam.py), from tessera's report:
- A print starting "\n--- " became a CREDO_GRADUS even when it was
  data output ("\n--- intrati:" before a number loop, tessera modi).
  That silently changed the output; the counts can't see it, a literal
  audit can.
- Now only text that ends in "---\n" and has no '%' is a title. A
  title still left behind is refused.
- The block-scoped tail `{ b32 praeteritus = ...; }` (most oratio
  suites) is accepted.
Still open: credo_convertere.py requires "Probans" in titles, so the
roman-numbered "--- I. x ---" form (oratio, ludus_tessera) is refused;
the agents converted those by hand.

## 2026-10-21 — step 3 leftovers: gravis where the memory IS the test, and a file that grew with the repo

Fran approved `gravis` for four MEMORIA suites (md_stml, oratio_stml,
officina renovatio, officina legatus) and the const change for
oratio_vocabula. All five are now v2 and BONUM (counts == HEAD, mixed
order sound).

**gravis goes on EVERY section that can run first, not just the one that
tripped.** renovatio and legatus run `legatus_currere` in-process, and it
loads the whole nexus index. The cost shows up in `apex_auctus` (lifetime
peak growth), which rises only once per process. So whichever section runs
first pays it: in table order that was 'renovare recentem'; under
CREDO_ORDO=mixtus another section paid. A signum on the first table row
alone would pass in table order and fail at random under mixed order. The
rule: if the cost is process-wide (an index, a cache, RSS high water),
mark every section that touches it, and say why in the source comment.

**oratio_vocabula IV was not "a big corpus", it was one 172 MB read.**
The section read build/nexus.tsv whole into its piscina: 209 MiB of
arena, 268 MiB of peak growth. That file grows with the tree, so the
section would have crossed any ceiling eventually. The fix: stream it
through a 16 MiB buffer (`filum_octeti_*`), line-aligned chunks into both
vocabularies. `oratio_vocabula_symbola` keeps nothing from its input:
verb, path and identifier are all copied. The result is 19 MiB arena and
122 MiB peak growth, and it now stays flat as the repo grows. Assertion
count is unchanged: each chunk's result is ANDed and asserted once per
vocabulary, as in HEAD. The vocabulary + glossary + Moby moved into a
suite fixture, possible only because `oratio_vocabula_creare(_anglice)`
now takes const tables. The two Moby load assertions moved from VI to
parare_suitae.

The lint tool (oratio/instrumenta/vocabula.c, run on every commit) reads
nexus.tsv the same whole-file way. Parked rather than fixed: the right
fix is a streaming entry point in oratio_vocabula that both share
(library API).

The ceiling also caught an agent's coinage: the md_stml draft had
`_casus_inlineos_probare`, and the live-tree lint inside oratio_vocabula
IV b flagged it as IGNOTA NOVA. HEAD went red on the same tree, because
the suite reads the live index. Renamed `_casus_intra_lineam_probare`.

## 2026-10-21 — briar amalgama and crusta effectus: accumulation, plus a latent chorda bug

Both hit MEMORIA in step 3 because of the html_md / tabularium pattern:
a loop over real inputs allocating everything into the one section
piscina.
- crusta effectus X/XI derives + judges every house `.sh`. A per-script
  `opus` pool, destroyed each iteration, takes peak growth from 1370 to
  36 MiB.
- briar amalgama "corpus verum" fabricates + amalgamates three real
  fixtures. A per-fixture pool takes it from 321 to 128 MiB.
  `silex_fons_disci` keeps no cache; it reads into the caller's piscina,
  which is what made the per-fixture pool sufficient.

The effectus draft also had sections come back VACUA at random (XIII,
XIX, XX on different runs). The world setup copies
crusta/effectus_mandata.stml into the section's fake tree as
`filum_legere_totum(...).datum` cast to a C string, but a chorda is NOT
null-terminated. HEAD did this once, on fresh zeroed pages, so it always
worked. Per section in a reused arena, the bytes after the file were
leftover data, and the STML table sometimes parsed as garbage. Fix:
`chorda_ut_cstr`. The world-setup failure now prints its cause; it had
been a silent VACUA. Takeaway: rebuilding a world per section is where
latent "works on fresh memory" bugs surface. A VACUA section that comes
and goes points at uninitialised or unterminated data before it points
at order dependence.

Own slip worth recording: in the amalgama loop I renamed `piscina` ->
`opus` with a blanket replace, and it hit two string literals the test
asserts on (`_allocare_interna_piscina`, `#undef _credo_piscina`). The
checker caught it (MALUM, 6 failed asserts). Same trap as the converter's
`c` rename: never substitute identifiers without skipping literals.
