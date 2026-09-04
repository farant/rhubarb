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
