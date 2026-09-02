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
