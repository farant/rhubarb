# ANALYTICA — census annalium cursuum (what records a run today)

*Born 2026-10-13, first step of the repo-analytics slice (Fran: "a repo
analytics layer ... a harness for every script or logical job ...
history of how many times it's run, how long it's taken ... tagged with a
git commit/timestamp", plus standard logs with exit codes and rotation).
Vision item: tabularium …QB5SQ (inside the conventions-as-data cluster
…R43654).*

*Discipline note:* this is a census, not a design. Every row is a thing
that exists and was looked at; the design comes from the interview
(`analytica-interview.md`). Rows graduate into the spec only when a
question below needs them.

## I. Recorders that exist (2026-10-13, quinta 6f06cee7)

| # | store | records | home | shape | writers | readers |
|---|---|---|---|---|---|---|
| 1 | **mensura** volume `~/.rhubarb/mensurae.volumen` (44 MB, 264 968 events) | nested measurements: sessio, id, parens, titulus, valor, unitas, momentum; `sessio` events carry machine id, commit, OS, arch | **one home for all trees** | volumen event log (`acta` table: momentum, genus, datum); journal first (`MENSOR_DIARIUM`, default /tmp/mensurae.diarium), folded by `mensor condere` | `bin/mensor` (tools/mensor.c), `tools/mensor_suitae.sh` (gen-1 runners: briar, compile_tests, crusta, html, materia, oratio), crusta/instrumenta/mandata_census.c, materia_excusatio.c, tools/manus_instrumentum.c | `silva.mensurae` (pythonica/silva.py:4238), bin/mensor_ui |
| 2 | `build/portae/tempora.tsv` (1 181 lines) | every `porta()` run and commit phase (forma, lint, fabrica, portae debitae): UTC time, genus, titulus, seconds, sana, rc | per tree | flat TSV append | `silva._tempus_notare` (silva.py:1786) | humans (grep), memory bench line |
| 3 | `build/portae/<g>[.<f>].viva.json` + `*.fracta.acta`, `*.planta_rubra.acta` | live receipts (tree sigil -> pass) and the LAST red / planted log per gate | per tree | JSON + text, overwritten | silva.py (`_receptum_vivum_scribere` :3372, planta :3697, commissio :2872) | commissio (receipt reuse), humans |
| 4 | `build/fabrica.db` → table `cursus` (8 886 rows in ~5 days) | every fabrica action: titulus, initium (epoch s), duratio_ms, eventus (SANATUM / FRACTUM / IUDICIUM ...), causa, stalum (why it ran) | per tree | sqlite (tools/fabrica_memoria.c) | bin/fabrica | `bin/fabrica causae`, `sanare -siccum` estimates, ad-hoc sqlite (this slice) |
| 5 | `build/fabrica/acta/<titulus>.log` (7.7 MB) | command, stdout, stderr of the LAST run of each action | per tree | text, overwritten | bin/fabrica | `bin/fabrica probare` (prints a run member's log), humans |
| 6 | `build/test_logs/<sub>.log` (17 files) | the LAST run of each suite runner / entry point | per tree | tee, overwritten | the 14 runners / entry points, compile_tests.sh | humans |
| 7 | credo `CREDO_VERDICTA` TSV (member area `credo.tsv`) | per section: titulus, exitus, passed/total, fracture | member area | TSV | credo (lib/credo.c) | bin/fabrica (SECTIO lines) |
| 8 | frigida_probare preserved tree + `acta/` | each fresh-clone run, per suite | `$TMPDIR/frigida.*` | dirs | tools/frigida_probare.sh | humans (the run's own summary) |

Not run records, listed so they are not mistaken for them: the ledger
journal `~/.rhubarb/annales` (lifecycle of records), cursor locks
(`<sub>/build/cursor.sera`, pid of the holder, transient).

## II. The acceptance questions against today's stores

| question (asked for real, cursores ad gradus) | answerable today? |
|---|---|
| where did warm radix's 26 s go | partly: #4 per action, #2 the gate total - nothing nests them |
| which upstream generators re-run on every judge | yes, #4 (`eventus = 'IUDICIUM'`) - per tree only |
| did this commit make X slower | only for runners writing mensura (#1); #2 and #4 carry no commit |
| when did the fresh-clone check last pass, in which tree | no (#8 keeps directories, nothing indexes them) |
| which scripts never run | no |
| what did X print the last time it FAILED | only if its last run failed (#3, #5, #6 overwrite) |

## III. Overlaps that will matter to the design

- **Time**: #2 ISO UTC + seconds, #4 epoch seconds + milliseconds, #1
  ISO with ms + valor/unitas. Three clocks, three units.
- **Identity**: #2 gate names (`radix`, `toml (transitus)`), #4 action
  titles (`probationes_toml/probatio_toml_api`), #1 free titles
  (`silva.probatio.cursus.X`). No shared key for "the same job".
- **Home**: #1 is shared across trees; #2-#6 are per tree (a worktree's
  history dies with it).
- **Nesting**: #1 nests (MENSOR_SESSIO / MENSOR_PARENS env, free across
  processes); #2 and #4 are flat.
- **Logs**: #3, #5, #6 are last-run-only; the first run's evidence is
  gone the moment a second happens.

## IV. Coverage universe (the eventual "every script has the chassis")

- 349 tracked `.sh`, 23 of them frozen fixtures -> **~326 scripts**;
- 8 Claude Code hooks (`.claude/hooks/*.sh`), 3 git hooks
  (`tools/unci-git/{pre-commit,post-commit,pre-merge-commit}`);
- 50 binaries in `bin/` (house tools; 21 installed to ~/.bin);
- pythonica entry points (`silva.porta`, `commissio`, ...).

Chokepoints that see most runs without per-script adoption:
`processus_exsequi` (lib/processus*, every C-side spawn: fabrica,
commissio's binaries), `silva._curre` (every pythonica spawn: gates,
tools), fabrica's action runner (already records #4), and the sourced
prologue pattern scripts already use (`tools/sera.sh`, `tools/vexilla.sh`
- a `trap EXIT` closes the record, as sera releases its lock).

## AUDIENDA (not verified)

- Writers that append logs without a `.log` name or via raw `printf >>`
  were found by grep only; one may be missing.
- Residents (tabulariumd, legati, fori) and apps (pictor, vicus, ...)
  log nowhere checked here.
- Whether `mensor condere` runs at the end of every runner that writes
  the journal (an unfolded journal in /tmp loses runs on reboot) - not
  checked per runner.
- Volume size growth rate of #1 (44 MB since 2026-08-13) not projected.
