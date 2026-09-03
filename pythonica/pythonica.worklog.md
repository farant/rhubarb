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
