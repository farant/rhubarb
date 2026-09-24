# pythonica — worklog

## 2026-09-01/02 — birth: the Python face for structural edits

Why: my edit scripts (python heredocs, literal anchors, immediate
writes) failed three times in one day when formatting moved text, once
pinned a defect from an unfixed binary, and twice committed on a red
gate because the harness shell ignores `set -e` and I misread
`pipestatus`. Fran's read: MCP edit tools are not reached for, scripts
are the reflex, so the primitives went into a Python module over the
house CLIs. Stdlib only, dev-time only, C stays the truth.

What exists (`silva.py`, gate `probare.sh` = 90 assertions):
- `extenta`, `corpus` by name (functions AND types with bodies; the
  formatter's root walk now descends taken conditional arms, so a guard
  after an include or a platform `#ifdef` no longer hides names).
- `Editio`: in-memory edits, ONE write in `applicare`, which is the gate
  point: stale-read guard (disk ≠ what was read → refused), whole-file
  format, examen, differre against the pre-edit text; `Fructus` result;
  `strictum` reverts on REICE. Anchors are TOKEN SEQUENCES by default
  (whitespace anywhere irrelevant, literals exact); `substituere`,
  `inserere_post/ante`, `membrum_addere`, `replace_selecta` (structural
  sub-function anchors via selecta).
- `Refactio`: multi-file transaction, guard on all before any write,
  judge after all; `prototypum_synchronizare` (definition head →
  prototype) — the signature change end to end.
- Judgments: `examen`, `lint`, `forma_delta`, `verdictum`,
  `differre(a, b)` with `-verdictum`, `differre_git`.
- Legati bridge (private legatus over stdio JSON-RPC, ~1.5 s):
  `vocantes`, `vocata`, `inclusiones`, `caput`, `symbolum`,
  `diagnostica`; `usus` (nexus), `renominare` (plan/apply).
- Gates: `PORTAE` table with "it ran" markers; `porta`, `commissio`
  (all gates green or nothing committed; Fran's staging files refused),
  `planta` (plant → must be red and not a build error → revert in
  finally → must be green).
- Umbra: `porta_umbra` runs a gate detached and writes a receipt bound
  to a tree hash (HEAD + tracked diff + untracked files); `exspectare`,
  `portae_pendentes` (orientation after compaction), and `commissio`
  accepts receipt paths — refused if the tree changed since the run.

Lessons: `set -e` and `pipestatus` are not gates here; a plant that
breaks the BUILD proves nothing; a pin generated from the
implementation proves nothing until seen red; `git add <dir>` sweeps
`__pycache__`. Next (desideratum): snapshot-bound umbra (`git stash
create` + temporary worktree) so editing may continue while a suite
runs; LSP format-on-save for Fran.

## 2026-09-02 — the umbra meets the real suite

First receipt-backed full root run: the suite finished green and the
worker died silently. `subprocess.run(text=True)` decodes strictly and
one test (`probatio_eventus_inspector`) prints raw bytes; the
UnicodeDecodeError killed the detached worker after the 3.5-minute run,
no receipt, `.pendens` forever, `exspectare` timed out at 600 s. Three
fixes, each seen red first: `errors='replace'` on every gate output; a
crashed worker now writes a FRACTA receipt with the traceback in
`.acta`; `.pendens` carries the worker PID so `portae_pendentes`
reports 'mortua' and `exspectare` raises at once instead of waiting.
Later the same day `commissio` refused a `git mv` commit because the
old paths no longer exist — it now accepts paths whose deletion is
staged (pass the new paths). Two doctrine notes: the receipt hashes
tracked diffs, and the ledger file is tracked, so file ledger notes
AFTER the commit, never between launch and commit; and `substituere`
gained `genus=` for types.

## 2026-09-02 (later) — what a day of use asked for

Eleven commits and seven shadow receipts in one day, all through the
module. Three gaps it kept exposing, now closed: (1) plain-text files
(runner scripts, the mensor page, markdown) had no Editio, so I wrote
the same count-and-replace helper six times — `Textus` is that helper
with the stale-read guard and all-or-nothing write; (2) the
wait-for-receipt / commit / print-breakdown script was retyped five
times — `commissio_umbra` runs shadow gates one after another (so
suite timings stay honest) and commits against all receipts; (3) the
sqlite reader for the measurement volume — `mensurae` and
`compendium_mensurae`, checked against the real store. Also: a comment
is one anchor token, so tabs or reflowed lines inside it broke exact
matching once; comments now compare whitespace-collapsed. Fran's
longer answer is a trivia-tolerant plain-text/markdown parser next to
the sententiae outline parser, so prose gets structural anchors the
way C has them; not started.

## 2026-09-02 (night) — what the optimization day asked for

Ten shadow-gated commits through commissio_umbra, no half-writes.
Every refusal was an anchor of mine, three of them the same shape:
a block containing comments anchored without them. `replace_inter`
(two short anchors, everything between replaced) is that fallback
made a primitive, for Editio and Textus. The receipt no longer hashes
Fran's staging files (VETITAE) — the tabularium journal is tracked and
every note voided a running gate. `metiri` is the min-of-7 computus
measurement; commissio_umbra reads a suite's wall time from the mensor
session when the runner prints no total; a name refusal now says
whether the name is present as another genus or only as a call.
Still open: the snapshot-bound shadow (edit while gates run) and a
Refactio hook for "write, regenerate, judge".

## 2026-09-02 (late) — a red gate names what went red

The credo NIHIL PROBATUM landing (0c784412) turned the root gate red
for three tests, and the exception said only "porta umbrae radix non
sana: Tests Passed:" — the marker text and the last 1500 characters of
a 4.8 MB output. Three rounds of grep over the receipt's `.acta` to
learn the three names and that each had passed every assertion. The
information was in a file the module had already written. Now:
`fracturae(acta, porta)` reads the failed tests out of a gate's output
by the runner's own shape (root: `Testing: X` … `✗ TEST FAILED: X`;
every sub-suite: `=== X ===` … `FRACTAE: X Y`; anything else: the
whole gate as one fracture) and keeps for each the diagnostic lines of
ITS OWN output — FRACTA/Speratus/Receptus/Totalis/Conditio — not the
tail of the run. `Porta.fracturae` carries them (default None, so the
fake gates in the tests still construct positionally); the umbra
worker writes them into the receipt; `receptum_relatio(via)` reads
them back days later; `commissio` and `commissio_umbra` put them in
the exception instead of the tail; `planta` prints and returns
"fractae: X (Conditio: NIHIL PROBATUM)" so the red testimony survives
the green rerun that overwrites its log. Tests: synthetic root and
sub-suite outputs, then a REAL red gate — a shell script that emits a
canned root run and exits 1, registered through
`PYTHONICA_PORTAE_FICTAE` (JSON in the environment) so the umbra
worker, a separate process with its own PORTAE, sees the same fake.
Proof on the live runner: plant the early return in probatio_credo,
`planta` printed "fractae: probatio_credo (Conditio: NIHIL PROBATUM)"
from the real colored output. Small API note found the same night:
`Refactio.applicare()` returns a list of Fructus, not one.

## 2026-09-02 (later still) — run one test the way the runner does

Profiling the slow silva tests by hand cost two false starts: the
binaries live in different places per suite (root builds into
/tmp/<name>, the sub-suites into <suite>/build), every runner starts
the test from the repo root, and the sub-suite tests find the root
through RHUBARB_RADIX with a fallback of ".." that only works from
inside silva/ — launched bare from the root they print "corpus non
apertum" and exit 1, which looks like a real failure. Then the
sampler: sample.sh only wraps computus, so the profile was `sample
<pid>` plus awk over the call tree, three times. `probatio_currere`
folds all of it: name → suite by source file → binary path → launch
from the root with the environment → Cursus(rc, seconds, output,
fracturae, profilum). `secunda>0` samples the live process after a
grace period and returns the leaf table; `cursus_textus` prints
assertions/failures/condition, the fractures, and the profile.
`aedificare=True` builds through the suite's own runner with the name
as filter (which also runs it once — accepted). A path instead of a
name runs any executable the same way, which is what the tests use
for the red and the sleeping cases. Live check: canon_corpus through
the launcher, sampled 8 s — 26% blocked read, 24% xar accessors — the
same table I built by hand earlier tonight, in one call. Also
`Refactio.applicare` now returns `FructusOmnes`, a list with `.sana`,
so the multi-file assertion reads like the single-file one.

## 2026-09-02 (last stretch) — the profile and the oracle become calls

Four rough edges from the performance evening, all mine to hit
again otherwise. (1) The launcher returned a leaf table, but every
conclusion came from two scratch scripts: an inclusive table with
recursion collapsed and the call paths into a function. The first
inclusive table I built summed every recursion level and inflated
the recursive walkers seven-fold; that mistake is now impossible,
because `profilum_inclusivum` counts a function only where it is not
already on its own stack, and `profilum_viae` compresses recursive
runs to `fn×k` and counts the target once, at the top. Checked
against the saved canon sample: identical numbers and paths to the
hand version. (2) Byte identity was proven three times by shell
loops: snapshot per file, change, compare. `imago_capere` /
`imago_conferre` / `imago_differentia` are that loop with a manifest
(command, files, HEAD), return codes compared too, and named lists
of same / changed / missing / new. Checked on the real thing: 156
aedilis closures captured and re-compared, 156 identical. (3) Anchor
refusals now list the line of each match, so a doubled anchor is
lengthened without a grep. (4) `replace_inter` had eaten its anchors
despite its name and cost one rebuild; it now keeps them, `inclusae=
True` restores the old shape, and the end anchor is searched only
after the start anchor ends (it used to be able to match inside it).
The tests that pinned the old behavior were rewritten, and the first
two rewrites were wrong about the fixture, not about the code — the
line-numbered refusal and the returned text made that a one-look fix.

## 2026-09-02 (last) — the shadow gate binds to a snapshot

The receipt used to hash the whole live tree at launch, so any edit
while a gate ran made it stale: six gates tonight, twenty idle
minutes. Now `porta_umbra` captures the working state as a git tree
(temporary index copied from the real one, `add -A`, `write-tree`;
staging files included, a snapshot is not a commit), materializes it
as a local clone with no checkout under ~/.rhubarb/umbrae (objects
hardlinked, HEAD = the real HEAD, `read-tree --reset -u` of the
snapshot tree, then the ignored build products cloned copy-on-write —
bin, build, silva/build, book_assets: 2 GB apparent, nothing real),
and runs the gate there. Measured: capture 0.08 s, clone plus apply
2.9 s, build products 0.8 s. Why a clone and not a git worktree: a
worktree's `.git` is a pointer file, and the house git library opens
`.git/HEAD` as a path, so the mensor would have silently stopped
recording; the clone has a real `.git` and `bin/mensor sessio` works
inside it. And why a local clone, not a shared one: the first gate
run in a shared clone went red on one assertion — the differ, which
reads HEAD through the house git library, saw an empty HEAD because
the library does not follow git's alternates file, and reported
lib/piscina.c as entirely added. Hardlinked objects cost nothing
and the library reads them. The receipt's seal is the snapshot tree; `commissio` now
verifies per file — each path it commits must equal its blob in that
tree, and HEAD must equal the base — instead of the whole tree, so
edits to anything not being committed no longer matter. Old mode
kept behind `photographica=False`. `receptum_delere` removes the
clone; a failed receipt keeps its clone for inspection until deleted.
Proof of the property: this commit's own pythonica gate ran in a
clone while an untracked file was created in the live tree, and the
commit went through; the same file offered for commit is refused with
the reason named.

Addendum (same night): two guards after walking the mechanism with
Fran. Every name that becomes a path segment — gate, filter, snapshot,
byte-oracle image — is now refused unless it is a single plain
segment, before anything is written or removed; the recursive delete
already could not leave its two roots, but a name with a slash could
have landed inside them somewhere unintended. And clones can be
orphaned when a failed receipt is forgotten: the pending marker now
carries the clone path on its second line, `umbrae_orphanae` lists
clones no receipt or pending gate names, `umbrae_purgare` removes
them, and `portae_pendentes` shows them so a post-compaction glance
sees the cost. The first gate run of that sweep deleted the clone it
was running in: inside the clone the sweep test saw only the clone's
own receipt directory, not the live repository's pending marker, so
the real clone looked orphaned. Two consequences: every clone now
carries `.umbra.pid` (launcher's pid at materialization, the worker's
after spawn) and the sweep never removes a clone whose process is
alive, which holds from any context; and the sweep test runs against
temporary umbrae and receipt directories, never the real ones. The
pending listing also judged snapshot receipts by the live seal and
called every one of them stale; it now compares the base commit. Named but not done: the root runner still compiles
test binaries to /tmp/<name>, a fixed shared path that a clone gate
and a manual run could collide on — same class as the aedilis temp
file, fix when the runner is next touched.

## 2026-09-03 — umbra: tempora plagularum tractarum speculata

The first root gate run in a shadow clone after B1.1 (md) went red on
two natura tests that never touch STML: `probatio_natura_canones` and
`probatio_natura_glossae`, both at their "binary older than a source"
guard, naming `tools/natura_canones.c` / `tools/natura_glossae.c` as
newer than `bin/natura_*`. Mechanism, measured: `git read-tree -u`
stamps every tracked file with the checkout time, while
`_clonare_ignorata` copies bin/ and build/ with `cp -c` (clonefile),
which PRESERVES mtimes (`touch -t 2020… a; cp -c a b; stat` → equal).
So in the clone every source is newer than every binary — the mtime
order is inverted and any mtime-ordered guard lies red. Fix:
`_tempora_speculari(ad, arbor)` after cloning — walk `git ls-tree -r`
of the snapshot and `os.utime` each clone file from the live file's
timestamps (6,730 tracked files, well under a second). A file absent
live (born after capture, or deleted) keeps the checkout time. Gate:
the photographia test sets the temp file to 2020 before materializing
and asserts the clone shows 2020 and README's ns-mtime equals live.
Doctrine reminder that this confirms: "compare OUTPUT, never
timestamps" is the rule for artifacts; binary-staleness guards are
the allowed exception, and they need the clone to be mtime-faithful.

## 2026-09-03 — post B1: five additions from the session review

Fran asked how pythonica had worked over the B1 arc. It carried every C
edit and every landing; the rough edges were all at the seams, and two
of my six complaints were wrong once I read the code: `commissio_umbra`
returns exactly what its docstring says (I had printed the internal
tuple), and `umbrae_purgare` purges only orphans by design — a failed
gate keeps clone and receipt for inspection, `receptum_delere` clears
both. Built: (1) `Editio._extentum_praesens` — when a name is missing
AND the present text yields fewer extents than the original, the parse
died; the error now carries clang's first `error:` line (the
`structura` = `struct` trap cost an hour under the old "non inventum
in hac plagula" message). (2) `Editio.commentum` to replace a leading
comment by name — `substituere` keeps comments by design, so there was
no way to update one. (3) `inserere_ante_vocationem` + `probatio_addere`
for the "test case before the compendium" pattern, hand-anchored four
times in B1. (4) `syntaxis(via, textus)` and a `planta` pre-flight: a
plant that does not compile is refused before the gate runs (the first
B1 plant removed a parameter, -Werror, a full gate cycle wasted). (5)
`expandere(text|path)` around the stml CLI returning the vitium NAME,
slot, fragment, line — I wrote three C probes in B1 for information the
CLI already printed. Test cases for each in probatio_silva.py; the
pythonica gate is the commit gate.

## 2026-09-03 — the formatter was innocent: flat text through tolerant anchors

After B3 I reported a "formatting oddity" in md_arbor.c (an `if` body
collapsed beside its brace). Looking properly: the file carried FIFTEEN
damaged sites (giant continuation columns, `{ redde FALSUM;`, `} m_ad =`,
three declarations after a brace, four `_praefixum(...); redde ...` on one
line) and probatio_md_inlinea.c five ugly wraps. Three experiments cleared
the formatter: (1) formatting the committed file again changes nothing —
it is idempotent and simply ACCEPTS the shape (it has no
one-statement-per-line rule; long-line wrapping then breaks the joined
line by column heuristics, which is where the 85-column `post);` came
from); (2) re-applying the same identifier change well-formed to the
pre-commit version formats cleanly at every site; (3) a minimal probe
with a 73-column call wraps cleanly. The transcript then showed the cause:
my edit scripts passed tolerant anchors as ONE-LINE token series that
matched across three source lines, with the replacement written as the
same flat line. `replace` spliced the replacement over the whole matched
extent, newlines included. The property that made anchors survive the
formatter (whitespace is nothing) is exactly what made flat replacements
destructive.

Law now in `Editio.replace` (tolerant): a flat novus with the same token
count substitutes tokens IN PLACE and keeps the file's whitespace, even
across lines; a flat novus with a different count over a multi-line
extent is REFUSED with the lines named; an empty novus deletes; a novus
carrying newlines is written verbatim (the author's shape). Two suite
expectations changed on purpose (`x = I;` → `x = II;` now keeps the
file's two spaces — the formatter owns spacing, not the anchor).

Also built: the near-miss report (a refused anchor names the longest
matching prefix, the divergent token and its line text — five grep round
trips in B3 were exactly this), comment margins `*` ignored inside a
comment token plus `tolerans='verba'` for prose across a re-flowed
comment, and `silva.oraculum(n)` wrapping the two ORACULUM_* switches
(1 s, structured). `Refactio` already was the all-or-nothing multi-file
transaction I had asked for; the half-applied scripts were my not using
it — a bench line, not code.

Repair lesson: a damaged site and a properly laid-out site have the SAME
token series (`} m_ad = md_scan_citatio(...)` matched at 1937 and 2085),
so the tolerant anchor is ambiguous for layout repair; the damaged bytes
with `tolerans=False` are unique. `differre` judged every repaired unit
cosmetic — that is the proof nothing else moved. A formatter rule
"sententia in linea sua" (family of parametra-singula) would have
repaired flat text by itself; filed as a desideratum.

## 2026-09-04 — Prosa (C1): markdown edited by structure

`silva.Prosa(via)` is Textus plus structural anchors from
`md/extenta.sh`. Three decisions worth recording. (1) BYTES: extents are
byte offsets and the corpus is full of `→`/`§`/em dashes, so Prosa keeps
the file as bytes and decodes only for display; the first probe on the
plan document (heading "Arc C — the consumer") would have been off by
two under str offsets. (2) Extents carry the edit version and any edit
method refuses a stale one — the Editio lesson ("lineae labuntur,
nomina manent") applied to offsets: re-select after each edit. (3) The
judge in `applicare` is the parse of the written file plus the
`documentum` extent covering every byte; anchors used are re-counted
and REPORTED rather than asserted, because replacing a section usually
rewrites its heading and a refusal there would fight the common edit.
`sectio` excludes trailing blank lines so the separator survives a
substitution. Examen's "vocatio implicita" suspecta were right twice
today (`credo_initium`, `chorda_ad_literas` do not exist) — read the
suspecta line before compiling.

## 2026-09-04 — citata: the rot report reads 33,079 code spans in 0.6 s

Fran asked what the markdown tool could extract from the corpus. The
answer that mattered was not statistics but verification: every path
and every function name written in a code span, checked against the
tree and the symbol table. `md/census.sh -citata` dumps the spans with
positions (C, one parse per file), `silva.citata()` classifies them in
Python: `build/nexus.tsv` read once (1.04M rows, 'sedes' = definitions)
and `git ls-files` for paths. Two refinements came from the first run:
citations resolve RELATIVE to the citing document first (`css/CLAUDE.md`
cites `probationes/fixa/...`), and a bare name with several tracked
homonyms is ambiguous, not absent. After them: of 2,736 path citations
350 name a file that exists nowhere and 370 a file that moved; of 2,631
function citations 132 name nothing defined. project-specs holds 330 of
the absent paths — specs cite files before they exist, and some never
came to exist. The report reports; a gate that pins the count "only
falling" is the next step if Fran wants it.

## 2026-09-04 — Prosa after its first six uses

Two edges from writing the closure documents with Prosa, both small:
`inserere_post` on a section lands right after the last content line
(the section excludes trailing blank lines by design), so every note
began with a hand-typed newline; and heading titles had to be typed
exactly, long ones included. Added `paragraphum_addere(x, text,
ubi='finis'|'initium')` — one blank line of separation, the separator
before the next heading untouched, and at the head of a section the
insertion goes after the heading line (after the underline for
setext, carried as `caput_finis` on the section extent) with a blank
line added only if the body follows immediately — and `incipit=` /
`continet=` on `capitulum`/`sectio`, with ambiguity reported as the
list of matching headings. Test lesson: my own ambiguity fixture had
one match, not two — an empty prefix matches everything and makes the
refusal deterministic.

## 2026-09-07 — Live receipts and the runner lock

Two costs from the T18 session, both about gates, both fixed in the
tool rather than in my habits.

**Live receipts.** `commissio` ran its gates in order and a red gate
late in the list (radix after oratio and pythonica, then silva after
both again) discarded the green runs before it: three attempts, about
fifteen minutes of repeated suites. The shadow path already had
receipts bound to a photograph; the live path now has the same shape.
`porta()` in the live tree writes `build/portae/<nomen>.viva.json`
with `sigillum_arboris()` taken BEFORE the run, one per gate key, and
`commissio` consults it: same sigillum now and the receipt is sana →
the gate is not rerun and a line says so. The design question was the
key. A per-gate scope ("oratio reads oratio/ and lib/") would have
saved the second retry today, but it would be a lie: the identifier
lint reads every tracked .c/.h and the corpus gates read every .md, so
an edit to silva's pins can change oratio's verdict. Whole-tree
sigillum, VETITAE excluded (the ledger changes on every note). The
honest consequence is a habit: write the worklog and README first,
run the gates last, commit. The receipt from `planta`'s green run
counts too, since the restored tree has the original sigillum.

Rancid runs: `porta()` compares the sigillum after the run as the
umbra worker does. A rancid gate's verdict still stands for the
current commissio unless one of the files being committed changed
during the run (hashed before the gates, `git hash-object
--stdin-paths`) — that is refused by name; other files changing is
reported and accepted, the photographic law ("quod committitur est
quod probatum est, cetera libera") transposed to the live tree. A
rancid receipt is never reused.

**The lock.** "Oratio runners never concurrently" lived in MEMORY and
in my head. `tools/sera.sh` is one law for every runner: mkdir the
`<build>/cursor.sera` directory, write `radix` then `pid`, wait up to
`SERA_TECTUM` (600 s) with one stderr line, then exit 2 naming the
holder. Stale = holder dead (`ps -p`), or radix differs (a lock copied
into an umbra clone by `cp -c -R build/`: the holder runs in another
tree), or older than an hour. Reentrancy through `SERA_TENTA` in the
environment so `vocabula.sh` calling `compile_probationes.sh registrum`
does not deadlock on itself, and so `probatio_currere` holding the
Python mirror can call the runner. nexus.sh's lock from this morning
became a call into the shared law (120 s, then renovation skipped, as
before). Measured before trusting: bash 3.2 fires an EXIT trap once,
in the main shell, not in `$(...)`, `( )` or `( ) &` — the runners are
full of all three — and stage XI of the fumus pins that. Twenty-one
scripts edited by one regex-anchored Textus loop; `bash -n` on all.

Found by the gate, same afternoon: after the runner sweep a
`oratio/build/cursor.sera` remained with a dead pid. Eight oratio
wrappers ended with `exec "$BIN"`, and exec replaces the shell, so the
EXIT trap that releases the lock never runs. Self-healing in practice
(the next taker sees a dead holder and takes over) but wrong, and the
fumus's live-integration stage found it by failing to mkdir. The
wrappers now run the binary and `exit $?`; stage XII greps for any
`exec "$BIN"` in oratio/*.sh and runs verba.sh to prove the lock is
gone afterwards. Two greps of mine had missed the three wrappers with
an env prefix before the exec; the gate's grep is anchored on the
string, not on the line start.

Also: `planta` now keeps the red run's full output in
`build/portae/<porta>.planta_rubra.acta`. The one-line summary for the
pythonica gate's planted fault named an inner fixture line ("planta
rubra: ficta ... probatio_x"), not the failing assertion, and the green
run had already overwritten the live receipt's acta.

## 2026-09-07 — sample attach flake, and the receipts paying off

The pythonica gate died once under the T19b commit: `sample <pid> 1
-mayDie` returned 255 with no message on the three-second sleeper
probe, right after the oratio suite and while the post-commit index
sweep was running. Alone, green. `_profilum` now retries the attach
up to three times while the pid is alive; a dead pid is still the
real error. The retry with the receipts made the recovery cheap: the
oratio run from the failed attempt and the standalone pythonica run
were both live receipts on an unchanged tree, and the second
commissio ran no gate at all.

## 2026-09-07 — the placement law, and the comment as an object

Five Editio replacements in one oratio step came back with doubled
indentation or a spurious blank line, and each cost a repair round
against the formatter's `-vitia` list. The formatter was innocent
three times over: the same textual insertion run through `-scribere`
alone changed nothing. The cause was the 2026-09-05 note nobody can
keep in their head: a multi-line `novus` is written verbatim from the
anchor's first token, so a `novus` that carries the context's
indentation (the natural way to write one, since the anchor is written
with it too) lands on top of the file's indentation, and the formatter
then aligns the whole declaration block or `=` group to the overlong
line. Same at the tail: an anchor ending in a newline stops at its last
token, the `novus` ends in a newline, and the file's newline follows —
one blank line too many, reported as `intervalla 13 pro 1`. The law
now: first-line indentation dropped when the first token begins its
line, one trailing newline dropped when the file continues with one.
The tolerant mode stays the only one touched; verbatim modes are
verbatim.

The comment refusal (tier I of the ledger item) was cheaper than
expected because `_exemplar_tolerans` already finds the anchor's text
as bytes: if that occurrence starts, ends or lies inside a comment
token, the refusal says so with the comment's line and the three exits.
Tier II is a small object: `Editio.commentum(fragment)` selects one
comment token, exposes its prose without margins, and can rewrite it
reflowed or append a reflowed paragraph while every prior line stays
byte-identical — the dated note in a file header, which today I typed
by hand with a `Textus` anchor on the previous paragraph. Reflow is
words to 72 columns with the `/*` column as the margin; the formatter
judges width but never reflows prose, so this is the only reflow in
the house. Tier III (sentence and paragraph anchors through oratio)
stays open; the plumbing for it is the comment corpus that the
identifier lint already builds.

## 2026-09-07 (night) — the sentence as an anchor

Tier III of the comment-editing item turned out to be mostly
bookkeeping once the comment object kept its raw lines per paragraph:
`sententia(continet=...)` runs `silva.Oratio` over the comment's
prose, which gives sentence extents in bytes of the joined
paragraphs; the paragraph is found by offset, the sentence replaced
inside that paragraph's prose, and only that paragraph is reflowed —
the other paragraphs are re-emitted from their raw lines with the
`/*` and `*/` markers put back, so they are byte-identical. The
sentence reader earned its keep at once: `e.g.` did not split, which
a regex would have. It also showed its gap: `par. III` split, because
oratio's abbreviation table has no `par.`; that is oratio's to fix
and is on the ledger. Two refusals got their missing half from the
residual list: an ambiguous anchor after earlier edits now names the
file's lines beside the in-memory ones, and the staging-file refusal
says whose files they are and what to do.

## 2026-09-16 — planta: 'error:' anywhere was a false build break

`silva.planta` refused a genuine red as "planta AEDIFICATIONEM fregit"
because its build-break check was `\berror:` anywhere in the gate's
output. crusta's differentia gate (P11b) prints bash's own diagnostics
for every disagreement (`bash: -c: line 3: syntax error: unexpected end
of file`), so a sanity plant that turned the gate red was reported as a
compile failure — twice, with two different (compiling) plants, until
the plant was compiled by hand and came back clean. The check now
anchors on diagnostic LINE shapes: `file:line:col: [fatal ]error:`,
`clang|cc|gcc|ld: error:`, or the runners' `FRACTA (compilatio)`.
Test `porta_errata_aliena`: a red gate whose acta carry a foreign
`syntax error:` line is a true red and the file is reverted; the old
pattern matched that line (shown in the session), the existing
`porta_fracta` (`x.c:1:1: error:`) still names the build break. Lesson:
a heuristic over another program's output must match that program's
line format, not a word.

## 2026-09-17 — query tools for materia clients: arbor by suffix, coctum, exemplaria

Asked by Fran after the crusta census (nota 01M2PRPGBC), which ran the
house scripts through silva-style pattern documents by a scratch shell
harness. The harness became `silva.exemplaria(viae, regula)`: projection
per file (`arbor`, routed by suffix like `metiri`), the regula appended
after a sentinel element, `stml expandere`, then `stml vertere` on the
text AFTER the sentinel only and stdlib `html.parser` over the HTML form
(explicit closing tags; STML's pretty form has implicit closes that no
regex should parse). Findings while building:
- `stml vertere` refuses the arbor envelope (`ATTRIBUTUM_VERUM` on the
  valueless `linea-initium`), so the whole expanded document cannot be
  converted: the sentinel `<exemplaria-limes-relatorum/>` splits the
  relata from the tree. The refusal surfaced as "omnes 237 plagulae
  fractae" — the all-fractured refusal paid for itself on its first run.
- `vertere` writes HTML void elements (`br`, `input`, …) without a close
  tag; the relata reader treats that set as depth-neutral.
- The client `arbor.sh` instruments print only a byte count under
  `-tacitus` (silva's prints the tree without a summary): routing must
  not reuse silva's flags.
- For C, `silva/arbor.sh` writes the `<parsura>` forest (`<ambiguus>`
  alternatives repeat a definition per reading: 8 matches for 2
  definitions) WITHOUT examen's system expansion (`NIHIL` never becomes
  `NULL`), so silva's own lint documents find zero there — their gates
  build the document inside examen's recipe. Documented as a limit; C
  lint stays in silva's harnesses (silva is frozen).
- Testing a section of probatio_silva.py by `exec` from stdin imported
  the repo's `silva/` DIRECTORY as a namespace package (`__file__` absent
  → wrong RADIX): give the exec globals a `__file__`.
Result on the house: 237 `.sh` files, 21 rules, 3.9 s, numbers identical
to the scratch census.

## 2026-09-17 — exemplaria rows carry lines (materia-sedes A4)

`silva.exemplaria` now runs every materia client file through the
positioned view (`arbor(via, sedes=True)` → `<cliens>/arbor.sh -sedes`),
so a matched node arrives with `sedes="L:C-L:C" octeti="B-B"`. The relata
reader takes a row's position from the row element itself or from the
first descendant that carries one — in practice the node spliced in by
`<situs>&@n;</situs>`. Each `Congruentia` gains `linea`, `columna`
(1-based, byte columns) and `textus_fontis` (the source slice by the
octets, read once per file); `Exemplaria` gains `sine_sede` per file.
A row without a spliced node (`<situs/>`) has no position — honest,
and counted.

Two consequences of positions being attributes:
- The default prelude for clients is now `tags="ante post"
  attributa="sedes octeti"`. Without attribute transparency a repeated
  capture compares two nodes with different positions (never equal),
  and a node carried across a bridge becomes a literal pattern whose
  `sedes=` pins it to its own place.
- A client rule that declares its own TRANSPARENTIA without
  `attributa="sedes octeti"` is REFUSED with that cause, instead of
  quietly returning zero rows. The scratch census rule met it on the
  first run (it declares `tags="ante post"` only).

Measured over the house (237 `.sh`, 20 rules, the census rule with the
attributes added): 4.6 s (was ~4 s on the plain view); every count
identical to the morning's plain-view census except `exit-2` 263 → 270,
which is the seven `exit 2` lines commit `e12c1f46` added to runners
and oracles. 1,227 of 2,002 rows carry a position; the 775 without are
exactly the rules whose rows are `<situs/>`; every `-nt` row's source
slice reads `-nt`.

The "same command twice" rule, the case A3 was for, is now a gate:
`x=1⏎echo a⏎echo a` gives one row at line 2 column 1 reading `echo a`,
`echo a⏎echo b` gives none. It needs `cursus="fratrum"`: the default
greedy search binds the first `imperium` (`x=1`) and never retries, so
the plain rule returns zero rows — measured, and worth remembering when
a structural rule "finds nothing".

## 2026-09-17 — `diagnostica_materiae`, and a name that was already taken

B5 of the materia-sedes plan gives pythonica the client-diagnostics
face: `silva.diagnostica_materiae(viae)` shells to
`./tools/diagnostica.sh -machina` and returns
`[Diagnosticum(via, linea, columna, linea_finis, columna_finis,
gravitas, codex, causa, textus)]`, with `textus` sliced out of the
source by byte offsets.

**The plan and the spec both specified `silva.diagnostica`, and that
name was already taken** — by `diagnostica(via)`, the legati verdict on
a C89 file, which `probatio_silva.py` calls and `README.md` documents.
Python does not complain about a duplicate `def`: the later definition
silently wins. So the new function was defined, shadowed, and dead, and
the only symptom was that a hand call returned the wrong TYPE — a
verdict string being iterated character by character instead of a list
of records.

Renamed the NEW one, because the old name is documented and has a
caller; the new one had neither. The two are different in kind, not
just in scope — one returns a single verdict for one C file, the other
a located list over many documents — so merging them under one name
and dispatching on suffix would have handed callers two different
return shapes from one call. A test now asserts they are distinct
objects, so a future re-collision goes red instead of silent.

**Reflex to keep:** before adding a name to `silva.py`, grep for it.
`def` is not a declaration here; it is an assignment, and assignments
overwrite.

### `silva.planta` cannot plant in a `.py`

Its anchors are C token sequences (it is built on `silva.Editio`), so
every attempt reported `ancora 0 vicibus inventa` against perfectly
present Python text. Planted by hand instead — read, substitute, run,
restore in a `finally`. Same family as the standing rule **never
`silva.Editio` on a `.sh`**: the C editor is for C.

### The gate that caught a stale build three commits later

The pythonica suite went red at an `oratio/oraculum.sh` call — not
because of anything in this task, but because B1 and B2 changed
`materia/fontes/materia_coctor.c` and `materia_arbor.c` while their
commits ran only the crusta and materia gates. oratio's, md's and
html's build directories were left holding objects older than those
sources, and `oraculum.sh` refused them (the guard added in
`e12c1f46`, doing exactly its job).

The lesson is about the commit's `portae` list, not about the guard:
**a materia-substrate edit invalidates every client's build, so every
client suite belongs in that commit's gates** — the house rule already
says so, and B1/B2 ran the shim plus two suites instead of all of
them. Rebuilt and re-ran: oratio 19/19, md 14/14, html 14/14.

## 2026-09-21 — `commissio(opus=…)`: the commit closes its ledger task

Park 01KY57P2WD. The commit is the one step an agent never forgets, so
task closure rides on it instead of on discipline (measured the same
day: three days of work with nothing filed in the ledger).

**Pre-check BEFORE the gates, closure AFTER the commit.** A typo in the
task id discovered after the commit means a commit with no closure, so
`opus_praeiudicare` runs first, read-only, through `./gesta/frigida.sh
-res`. All preliminary causes are reported together — every forbidden
path plus the task's causes — per Fran's no-drip rule.

**A failed closure must never look like a failed commit.** It raises
`SilvaOpusError` carrying `.hash`; the message opens with "COMMISSUM …
FACTUM EST et stat - NOLI iterare", names the failed step, and prints
only the REMAINING commands, paste-ready. A caller that retried the
whole commit would find nothing to commit; one that re-ran step one
would write the effect twice.

**The launcher is noisy on stdout** (`  [nexus] nota_frigida` when it
relinks). The reader skips lines starting with `[` and takes genus and
status from the END of the first real line, because a title may itself
contain parentheses — pinned with such a title.

**The gate never writes to the live ledger:** `FRIGIDA_IMPERIUM` is
swapped for a stub that logs its arguments.

**I had been doubling the author trailer all day.** `commissio`
appended its own line (copied from recent history) and I also wrote
one in each message: five commits with two trailers, the first naming
two DIFFERENT models. Found only because I read `commissio` to extend
it. Now a message that already carries the line is left alone.

**Plants by hand** (`planta` is C-only): four, each reddening its own
assertion — preliminary causes not collected · trailer always appended
· closure error lists ALL steps, not the residual · pre-check accepts
any genus. The gate takes ~170 s, so five runs outlast one Bash call;
run them in the background and do not touch the tree meanwhile (the
gate seals the working tree). The final "restored" run hit the
previous run's build lock and did NOT run — verify a restore with
`cmp` against the backup, not with a gate that may not have started.

## 2026-09-24 — PORTAE += villa, silex-semen (from the test-suite inventory)

The inventory 'suitae probationum' (tabularium …EH76CR) named 7 runner-shaped
scripts outside PORTAE; two of them are headless and belong in:

- `villa` = `./apps/villa/fumus.sh -sine-facie` (the default run also opens a
  real window for the FACIES leg; `-sine-facie` omits it). Signal
  `FUMUS (PLENUS|FRACTUS)|FRACTUM:` - matches the pass line, the "villa.sh
  failed" line and every shape assertion's failure line. ~3 s.
- `silex-semen` = `./tools/silex_semen_fumus.sh` (no `-agere`, so no window).
  Signal `FUMUS: FACTUM|FUMUS DEFECIT`. Its exit-2 paths (bin/silex absent)
  print neither, so "nothing ran" reads as cucurrit=False, never green. ~13 s.

Born red WITHOUT touching source: villa via `VILLA_STIPES_MODUS=hospes_ignotus`
(the ssh stub fails, rc 1, FUMUS FRACTUS); silex via a `clang` stub that exits 1
shadowed first on PATH (the generated project's build fails, FUMUS DEFECIT);
and `bin/silex` moved aside gave cucurrit=False rc 2. probatio_silva.py pins
the registration, the runners' executability, and that neither command opens a
window (`-sine-facie` present, `-agere` absent). The GUI legs stay by hand.

## 2026-09-24 (later) — the registration went in TWICE; guard added

4f889895 carried both entries twice. My first attempt was a `perl -0pi -e
's|…|…|'` whose `|` delimiter collided with the `\|` alternations in the
pattern; I judged it "not applied" by printing the lines around 'frigida',
where it hadn't landed - it had landed after 'formator-intra'. A Python
replace then added the second copy. Behaviour was right (identical values, a
dict literal keeps the last key), every gate green, and NOTHING noticed - until
tools/inventarium_suitarum.py proposed porta = "villa, villa": the derivation,
reading the source by regex rather than importing the dict, was the only
reader that saw both copies. Fix: first copy removed; probatio_silva.py now
parses silva.py with `ast` and asserts PORTAE's literal keys are unique
(born red on 4f889895 itself: ['silex-semen', 'villa']). Lessons: a
substitution is verified by `git diff`, never by eyeballing the place you
expected it; and a dict literal swallows duplicate keys silently.

## 2026-09-24 — silva.inventarium + silva.portae_debitae (portae debitae T2)

`inventarium(res)` reads `./gesta/frigida.sh -inventarium` (T1's machine
form): lines without a tab are launcher noise and skipped; a line WITH tabs but
not four fields raises (never silently dropped); `\\ \t \n` unescaped. Rows
with no cell do not appear - the machine form prints present cells only.

`portae_debitae(viae)` -> (debita, intecta). Coverage is derived per call:
(a) for each row whose `porta` names a SUITAE gate, `<dir>/probatio_*.c` and
their `bin/aedilis --partes` closures (O/C/V), 4 Popen at a time - the house
has no threads, and none are needed; (b) `currit binaria`: a `.c` entry counts
through its closure, anything else literally; (c) `tegit viae`: fnmatch, whose
`*` already crosses `/` (so `pythonica/*` means the whole tree below). Also:
editing a runner script owes its own gate; a test file owes its suite.

Decisions made while writing it:
- Closures only hold .c/.h/.m, so when no changed path has those suffixes the
  closure sweep is skipped entirely (non-C commits pay nothing).
- A closure that cannot be computed (aedilis rc != 0) makes the suite OWED,
  with that as the causa - over-running beats going silently blind.
- `manu` means "the row has no gate" (GUI script or deliberately separate); I
  first also read `manu tantum`, found it changed nothing, removed it.
- One causa per gate (+N aliae); PORTAE order, then manual rows.

Cost, measured: a C change sweeps all 14 compiled suites' 392 tests, ~18 s
live. Tests: fake inventory via a frigida stub (as opus_praeiudicare's tests);
the dcd516c7 replay both ways - without the `currit binaria` cell radix is NOT
owed, with it radix is owed and the causa names tabulariumd_principale.c.
Plant: the runtime-edge branch disabled -> 3 red. The section's C calls cost a
sweep each, so the manual-row case uses a .sh path (20 s for the section).

First live run (lenses still empty): lib/json.c owes radix, briar, officina,
gesta; pythonica/silva.py is uncovered until T3 fills `tegit viae`.
