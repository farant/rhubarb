# mensura worklog

## 2026-08-13 — birth (library half)

Fran's design: a CLI that records any metric — benchmark a command, or
pipe a value in with a label — with push/pop nesting scoped to a run,
stored in a volumen. This commit is the library the CLI will sit on.

### Deliberately NOT Metrum

`Metrum` (parked, 01KYAMZVFN) is RRD ring-buffers: continuous samples,
**lossy by design**, fixed size forever, resolution degrading with age.
Right for "memory over a month".

Mensura is the opposite: discrete events, **lossless**, bound to a run,
**nested**. "Which commit made compilation slower" is a question RRD
structurally cannot answer, because by the time you ask, that sample has
been averaged into a bucket. Sisters, not the same thing — hence
separate names, decided before either could squat on the other's.

### Journal first, volumen second

Writing to sqlite means open + transaction, maybe 10ms. Across 150
files that adds a second to *the very build being measured*. An
instrument that changes what it measures is worse than useless — it
gets switched off.

So the hot path is a **plain file append**: one line, no sqlite.
`mensura_condere` folds the whole batch into the volumen in one
transaction after the run.

**The journal is truncated after a successful fold.** Without that,
every fold re-imports everything already imported and the numbers
inflate silently — a graph that looks like real growth. Poisoned it
(skipped truncation): probatio line 226 went red. That assertion is the
whole defence against double-counting.

### Format decisions

TSV, seven fields, one line per measurement. `mensura_ad_lineam` /
`mensura_ex_linea` are a pure round-trip pair, so the format is fully
testable with no file and no volumen.

**A field containing a TAB or newline is refused, not escaped.** A
format that permits breaking itself corrupts data *silently* — the
reader counts fields, finds a different number, and produces a
plausible wrong record. Refusing at write time is the only place the
error is still attributable.

`unitas` is mandatory. Without it you cannot label an axis or safely
aggregate, and adding it after data exists means backfilling.

Value is `duplex`, formatted `%.6f` (microsecond resolution). Locale
stays "C" because nothing calls `setlocale`, so the decimal point is
stable across machines — worth knowing before someone adds `setlocale`
somewhere and quietly breaks every stored number.

`momentum` is **s64** milliseconds. gettimeofday is not monotonic (NTP
can step it), so a delta can be negative; signed makes that visible
rather than wrapping to an enormous positive and producing a branch
that silently never runs (codex 80's whole point).

### Tooling gaps closed on the way

silva's POSIX shim lacked `getpid` — added, same as `sys/utsname.h` and
`sysconf` earlier today. The shim only describes what our analyser
knows; real compilation uses real headers, so a missing entry shows up
as REICE on code clang accepts happily.

### Note to self

Twice today I wrote a plausible house API name from surrounding
convention instead of reading the header —
`chorda_aedificator_addere_literas` (real:
`..._appendere_literis`) and `CREDO_MAIOR_S64` (does not exist; there is
`CREDO_AEQUALIS_S64` but no S64 ordering macro). The post-edit hook
caught both before compilation. Naming here is regular enough that
wrong guesses look right.

### Not built yet

The `mensor` CLI: `sessio`, `mensurare -- <cmd>`, `addere`, `condere`.
Nesting plan — `MENSOR_SESSIO` / `MENSOR_PARENS` in the environment so
child processes nest for free, plus an explicit `-parens` for
same-process shell loops where inheritance gives nothing.

## 2026-08-13 — wired into compile_tests.sh, and a correction

### The metrics contradicted my own earlier claim

Earlier today I told Fran "compilation dominates, not test execution",
inferred from the only timing the suite printed: its top-5 slowest
tests (38.5s of a 187s run). That inference was **backwards**. First
full instrumented run, 131 tests:

| phase | seconds | share |
|---|---|---|
| test execution (`cursus`) | 95.1 | 53% |
| test compile+link | 19.4 | 11% |
| **unaccounted pre-flight** | **66.1** | **37%** |
| total | 180.6 | |

Per-test compile is *tiny* — the slowest single link is 0.30s. What
actually costs is (a) test execution and (b) a large fixed pre-flight.

That pre-flight is visible in a filtered run: `./compile_tests.sh
imago_png` took **43.9s for a 1.2s test**, with "Libraries up to date".
It is the JS/oracle probations (`speculum velamen`, `forum velamen`,
`mensa logica`, `qr gyrus` via CoreImage) plus `generare`, all of which
run regardless of the filter.

**The lesson is about the reasoning, not the numbers.** I drew a
confident conclusion from a top-5 list, which is a biased sample by
construction — it can tell you what is slowest but nothing about how
the whole distributes. The measurement existed all along
(`TEST_TIMES_FILE`); it was being thrown away.

### Integration shape

- compile is now timed separately (`test_start_time` sat *after* the
  clang invocation, so compile time had never been measured at all)
- `LIBS_COMPILED` becomes `suita.bibliothecae.recompilatae` — the
  cold/warm discriminator, which otherwise makes total time bimodal
- per-test rows go in via **one** `mensor addere -tabula` per table,
  not one process per test (131 tests would have meant 262 spawns)
- emission happens *after* the human-visible summary, so a slow or
  stuck mensor can never delay or hide results

### Never break the suite

Every mensor call carries `|| true`; the session only activates if
`bin/mensor` exists and starts cleanly; `MENSOR_TACET=1` disables it
entirely. A metrics system that can fail the build is worse than no
metrics system, because it makes the gate less trustworthy rather than
more.

### Storage note

`mensura_condere` now strips the trailing newline before storing. An
actum is a *record*, not a line of a file — left in, the terminator
would sit invisibly at the end of the last field and surface later as a
baffling query result.

## 2026-10-13 - runs (cursus) in the journal (analytica T1)

A run = something with a start, an end and an outcome, written as TWO
journal lines in the same journal measurements use (`cursus\tinitium...`
and `cursus\tfinis...`), folded by mensura_condere with genus "cursus".
Two lines, not one written at the end, so a run that never finished
(killed, hung, shell died) is visible as CURSUS_INTERRUPTUS instead of
missing - that is the "did my hook stop firing" class of question.
mensura_cursus_legere pairs end with start by id through a hash table
(the live volume has ~265k events), keeps start order (stable sort),
skips an end without a start. Only argv is escaped (\\ \t \n \r: a
command is data); every other field must be pure or the line is refused
(the house rule of mensura_ad_lineam - a format that lets itself break
corrupts silently). Session context (commit, dirty, branch, tree,
machine, invoker) is just fields here: computing it once and passing it
down by environment needs git, so it belongs to the CLI (T2), keeping
this core pure. Suite probatio_mensura_cursus (credo v2, 5 sections,
mixed order green); plants IV red (fold routing off, exit ignored, no
interruptus, argv unescaped).

## 2026-10-13 - analytica T2: `bin/mensor cursus currere|legere` + chassis `tools/cursus.sh`

The chassis is a C WRAPPER, not a bash `trap EXIT` (spec §III said trap;
this departs). `tools/cursus.sh` is a 6-line sourced prologue that
`exec`s `bin/mensor cursus currere -- "$0" "$@"`; mensor writes the start
line, spawns the script again (CURSUS_INTUS=$0 stops the second wrap),
streams stdout/stderr through (processus_incipere + pulsare +
*_hactenus offsets, 20 ms), reaps, writes the end line. Why C: rusage
(`getrusage(RUSAGE_CHILDREN)`, ru_maxrss in BYTES on macOS), a monotonic
duration, `128+signum` for a signalled child, and one place that stores
the body - a bash trap gets none of those reliably and dies with SIGKILL
anyway (the start line is what makes a killed run visible as
interruptus). Escape hatches: CURSUS_NULLUS=1, or bin/mensor absent
(script runs bare).

Context (commissum, sordidum, ramus, arbor, machina, sessio) is computed
ONCE when MENSOR_COMMISSUM is absent and put into the environment;
children inherit and never call git. Branch is read from the HEAD file
("ref: refs/heads/X" else `detached`), the tree by ascending from cwd to
a `.git` entry, dirty = `git status --porcelain --untracked-files=no`
non-empty. Measured: top-level run +~27 ms, inherited child ~noise.

Bug found by a plant (D: HEAD unreadable): `chorda_ex_literis("")`
gives datum NIHIL (zero-byte allocation), `chorda_ut_cstr` then returns
NULL, and `setenv(name, NULL)` segfaults - every run outside a git repo
would have crashed the wrapper (and so the script). `_litterae_aut_vacuae`
in tools/mensor.c; fumus stage VIII (run from a dir with no repo above)
born red. General trap worth remembering: an empty chorda is not a
valid C string source.

Body seam: `_acta_servare` is the ONE function that writes a run's
output (v1: `<MENSOR_ACTA|~/.rhubarb/cursus/acta>/<id>.log`, fabrica's
"== effusio == / == erratum ==" form, 8 MB tail per stream) - T4 moves
it into volume massae, redaction later goes inside it. `cursus legere`
folds the diary by RENAMING it first (appenders after the rename create a
new file), then reads; 14 TSV columns with -machina.

Gate `mensor-cursus` (tools/mensor_cursus_fumus.sh, VIII stages). Stage V
first raced under the gate (killed before the start line existed: git
context in the big dirty tree > 0.5 s) - it now polls for the start line.
Stage III first could not tell inherited from recomputed context (same
repo, same answer) - it now presets MENSOR_COMMISSUM and requires it to
win. Plants XI red (mensor.c via silva.planta with a rebuild-then-gate
callable, since fabrica does not track bin/mensor; cursus.sh by hand).
Not planted: V (pairing is the reader's, T1 plants), VII (any plant =
unbounded fork recursion).

## 2026-10-13 - analytica T3: producers (porta, commissio, phases, fabrica actions)

Context moved out of tools/mensor.c into `lib/mensura_contextus.c`
(`mensura_contextum_parare`, `mensura_diarium_cursuum`) so bin/fabrica
uses the SAME code; lib/mensura stays pure (no git). Vocator default
is now `agens` when CLAUDECODE is set, else `homo` - the invoker
question (Q5) answered from a real signal instead of guessed.

End line gained `nota` (12th field): how a run ended, in words - a
gate's compendium, fabrica's eventus + causa + stalum ("IUDICIUM - ..."
is what acceptance question 2 will query). Escaped like argv, capped at
CURSUS_NOTA_MAXIMA (256) bytes, retreating to a UTF-8 character start.
The cap's own test was WRONG first: with "aé" from byte 0 the 256 cut
already fell on a boundary, so the retreat loop was never exercised -
plant N2 (loop exits at once) survived. Two leading 'b's put the cut
between C3 and A9; N2 then red. Lesson: a boundary test must assert
WHERE the boundary falls, not just that it exists.

Python producers do not reimplement anything: `bin/mensor cursus
incipere -titulus T` writes the start line and prints KEY=VALUE lines
(MENSOR_PARENS = the new id + context); silva.py's `_cursus` context
manager puts them in os.environ while its body runs (no threads in
silva.py, so this nests correctly through every spawn: gates, fabrica,
chassis scripts), restores after, then `finire -id -exitus
-duratio_ms -nota`. Exception = exitus 1 + first line of the message;
KeyboardInterrupt/SystemExit write NO end -> interruptus, like a killed
process. Wrapped: porta (`porta:<g>[.<filtrum>]`, nota = compendium),
commissio (`commissio`, children get vocator commissio), phases
forma / lint / fabrica / portae debitae / git commit (`phasis:<x>`;
tempora.tsv lines kept, still only on success). commissio_umbra not
wrapped (v1).

fabrica: the run record is written from the hook that already fills
its sqlite `cursus` table (`_cursum_inscribere`, every SANATUM /
FRACTUM / PRAEPARATUM / IUDICIUM event), start and end AFTER the fact
(start = now - duration) - an interrupted action is visible through its
interrupted parent gate, not itself. Written FIRST, so a broken sqlite
statement cannot hide the run. The child environment is deliberately
NOT given MENSOR_PARENS: a step member's environment is in its cache
key, a per-run id there would make every member stale forever.

Tests: mensor fumus IX (incipere/finire, nesting through them, nota in
column 15); fabrica fumus LII (one record per member, inherited parent
and commit, exit from eventus; whole fumus isolated to a temp diary,
read raw with awk so fabrica's gate does not need bin/mensor);
pythonica (commissio tree: phases + gates as children, vocator
commissio, commissum HEAD; _cursus nesting / exception / interrupt /
env restored; suite isolated to build/pythonica). probatio_mensura_cursus
section VI (nota).

Second blind test, same day: the pythonica check "env restored after
the run" compared against the value captured RIGHT BEFORE its block.
With restoration broken (plant Y2), the earlier commissio tests in the
same suite had already leaked MENSOR_PARENS, so "restored to the
leaked value" passed. It now compares every run-context key against
the value at SUITE START. Plants XI red in total (C: N1-N3 lib, M1-M2
mensor, F1-F2 fabrica; Python by hand: Y1 vocator, Y2 restore, Y3
exception end). Cost: forced css 18-22 s with records or with
CURSUS_NULLUS=1 (busy machine; difference inside the noise), 11 records
per run.

Open question found on the way (Fran asked whether fabrica would
benefit from reactor): fabrica's two parallel loops and this wrapper
poll children with a 20 ms sleep - a finished child's slot idles up to
20 ms, and a child printing faster than a pipe-full (16-64 KB) per
20 ms is throttled to ~1-3 MB/s. The right shape for batch loops is a
wait-any in processus (poll on all children's fds, timeout = nearest
deadline), not reactor's callbacks; reactor fits the fabrica resident.
Not measured yet.

## 2026-10-13 - bodies go to diurna (analytica T4 step 2b)

The one seam now ends in diurna: mensor's `_acta_servare` composes the
body ("== effusio ==" / "== erratum ==", each stream tail-capped at 4 MB
so the pair stays under diurna's 8 MB) and `_corpus_ponere` stores it
under the run id; the run's `acta` field is the body's SHA-256 (was a
file name under ~/.rhubarb/cursus/acta - that spool is gone).
`mensura_diurna_aperire` (lib/mensura_contextus) resolves the store
once for every producer: MENSOR_DIURNA, else ~/.rhubarb/diurna.
`cursus finire -titulus T -acta -|via` stores a body for producers
without the wrapper; `cursus acta <id>` prints it (exit 1 + status name
when unknown / deleted / corrupt).

Python: `_CursusApertus.acta`. A gate's body is its whole output
(Porta.acta); an exception's body is the WHOLE message (the nota keeps
its first line) - "what caused the commit to fail" no longer stops at
"git commit rc=1: <tail starting mid-word>". Passed on stdin, no temp
files.

fabrica: `_acta_actionis_ponere` stores build/fabrica/acta/TITULUS.log
(last-run-only until now) under the action's run id for SANATUM /
FRACTUM / PRAEPARATUM; diurna opened once per process (Memoria). Same
guard as the diary: a fixture root writes bodies only to an explicit
MENSOR_DIURNA (fumus LV now checks both stores).

Plants VI red (stderr not captured, -acta ignored, fabrica body guard
off, fractum not passed, Python body = first line only, gate body
dropped). Two notes: M4 went red by a SEGFAULT the plant itself made
(inverted guard passed NULL to strcmp) - weaker evidence than an
assertion, recorded as such. F6 (body guard off) first SURVIVED: in
fumus LV the diary guard returned before the body guard was reached,
and the case the body guard exists for - a fumus isolating only the
diary, as fabrica-fumus did until this morning - was untested. LV now
runs that case (explicit diary, no explicit diurna -> no bodies in the
default store); F6 then red.

## 2026-10-14 - `mensor purgare` (analytica T4 2c)

Reads the retention policy from STML (tools/analytica_retentio.stml by
default, -regula / MENSOR_RETENTIO) - every tier required, a missing
element refused by name (exit 2) - and applies diurna_purgare to the
run-log store with now = wall clock. -siccum reports what would be
compressed / deleted / freed, -machina gives one PURGATIO line. fumus
X. The daily opportunistic trigger is not wired yet (T6).

## 2026-10-14 - named queries (analytica T5)

`lib/mensura_quaestio.c`: pure functions over the run list (and
diurna's index for discus) - an index (by id, children by parent),
one shared filter (titulus, praefixum, arbor, ramus, nota prefix,
status mask, post, n), and the queries: historia (ultimum = n 1, fracta
= status mask), tendentia, filii, summa (iudicia = nota IUDICIUM),
commissiones (culprit = deepest failing descendant), statistica
(sectiones = praefixum '<member>#'), lentescentes, numquam, discus.
`bin/mensor quaestio <nomen> par=valor [-machina]` only parses and
prints; unknown query or parameter -> exit 2 naming it. `silva.cursus`
is a thin wrapper (rows = TSV fields minus the type column; acta =
the body text).

Found by reading REAL output, not by the fixtures:
- tendentia keyed runs by HEAD at start, so a commit attempt's gates
  (testing the NEXT change on a dirty tree) were filed under the
  previous commit: the 415 s radix spike of the credo change showed
  under 2b. Now: runs inside a commissio that PRODUCED a commit are
  keyed by that commit (from its nota, marked '*'); other dirty runs by
  'parent+'; clean by HEAD.
- the human tendentia print cut the key to 11 chars and hid the '+' of
  a 40-char sha + '+' (data was right); marker now separate.
- lentescentes on composite jobs ('commissio' x2.14) is not a
  regression signal - later commits simply owed more gates; it means
  something for fixed jobs (a test, a section, a gate).

Tests strengthened by reasoning before plants: numquam's path-boundary
case (top-level b.sh vs a run of .../tools/ab.sh), lentescentes'
consistency rule (with window 3 and a lower median any ratio pass
already implies 2/3 above - the rule only bites at window >= 4) and
its history rule (a 4-run title), and the CLI's decimal limes (2.5 vs
3.5). My own debugging slip: a trace copy put `echo` right before
`credo $?`, so $? was the echo's and the stage "passed" - the real
failure was my new block feeding data into an older smoke check.
Plants XVII red over two batches (two first did not compile - an unused variable after the planted change; redone with FALSUM &&). The pythonica check 'commissio_umbra: recepta sua deleta' failed once during an unrelated plant run and passed on a clean rerun - its second sighting, recorded on the ledger item (…GKC0).

## 2026-10-14 - pythonica groups as runs (analytica T6a)

The pythonica suite's 57 `print('--- name ---')` headers became
`_grex('name')`: same header, plus the previous group's run is closed
and `pythonica#name` opened (exit 1 if any check in it failed; nota =
"N fracta"; a crash mid-group leaves it interrupted). Group runs go to
the REAL diary/diurna (targets saved BEFORE the suite isolates its
fake gates into T) with the gate run as parent. Self-test points the
helper at a temp diary and restores the open real group. Plant (wrong
title prefix) red on the self-test only - my first attempt ran it with
CURSUS_NULLUS=1, which disables groups entirely (wrong red), and when
I killed it the plant stayed in the file (SIGTERM skips `finally`;
caught by cmp, restored by cp).

First reading (`quaestio sectiones titulus=pythonica`): ~400 s over 57
groups, half in three - pondera.py generator test 76 s, umbra receipts
58 s, commissio owed-gates block 57 s. Recorded on ledger …PRYJ22.

## 2026-10-14 - pilot adoption + censura (analytica T6b)

Chassis on seven scripts: the four converted entry points (toml, css,
md, silva - their own `tee` self-re-exec keeps CURSUS_INTUS, so one
wrap), tools/frigida_probare.sh, tools/vexilla_fumus.sh and the
pre-commit hook (sources via `git rev-parse --show-toplevel`; escape
hatches CURSUS_NULLUS and absent bin/mensor). `bin/mensor cursus
censura -radix D`: per script, a non-comment `source`/`.` line naming
cursus.sh needs a `# <cursus titulus versio/>` line at CURSUS_VERSIO
("I"), an annotation without the source line is dead, a title may
appear once (it is the job's identity); exit 1 naming each. The real
adoption found two gaps in my own lint before any plant: it required
the literal `tools/cursus.sh` (tools scripts source `$(dirname
..)/cursus.sh`), and it enumerated only *.sh, so the extensionless hook
was invisible - one predicate `_scriptum_est` (.sh, or no dot + shell
shebang) now feeds both censura and `numquam`. The mensor-cursus gate
checks the LIVE tree (live-tree invariant) and now `tegit` *.sh and
tools/unci-git/*.

Measured: css entry warm 1.17 -> 1.25 s (+~80 ms per top-level run:
git context, wrap, body write); css -vis 9.3 vs 9.1 s (inside noise) -
but -vis prints only 10 KB, so the 20 ms polling throttle (…KHK441) is
still unmeasured on a truly chatty script. frigida -suitae css under the
chassis: 91 s, nested fabrica runs carry the temp tree's own branch
`frigida-1046becc-24504` and commit (acceptance question 4 answered on
real data). Plants V red (version, twin titles, extensionless blind,
literal source rule, live-tree source line removed).

## 2026-10-14 - maintenance without waiting (analytica T6c)

`bin/mensor curare [-vis]`: folds the run diary into the volume, and
once a day (stamp `<diurna>/curatio.ultima`, or -vis) applies the
retention policy (MENSOR_RETENTIO, else the run's own tree
`tools/analytica_retentio.stml`; absent = not purged, said in the
note - never invented); records itself as run `mensor:curare` with
what it folded / compressed / deleted; a non-waiting lock
(`curatio.sera`) means two never overlap. The stamp is written after
every attempt, so a missing policy retries daily, not on every run.
Trigger: a TOP-LEVEL run ending (`cursus currere` with no parent;
`cursus finire` with no MENSOR_PARENS - an outer commissio) checks two
`stat`s (diary > 256 KB / MENSOR_CURARE_LIMES, stamp older than a day)
and only then forks `mensor curare` detached (setsid, stdio to
/dev/null; the manus `_detrahere` pattern) - the script returns at
once, most runs pay two stats. `mensura_diurna_radix` moved into
lib/mensura_contextus so stamp and lock sit beside the store.
MENSOR_CURARE_NULLUS=1 in the mensor fumus and the pythonica suite:
both create runs in temp diaries, and a detached fold racing their own
reads would be a flake. fumus XIII - my first version tested a body
stage X had already purged, then "diary empty" (curare records itself
after the fold), then a fold the preceding `legere` had already done;
each vacuous, each rewritten. Plants IV red (trigger off, daily gate
off, opt-out ignored, fold skipped).
