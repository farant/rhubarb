> Raw analyst report, batch H of budget B2 (2026-10-31), filed verbatim as evidence for ../budget-fusionis.md.

# Batch H — oratio, pythonica, briar (+ the frigida crusta anomaly)

Analyst: Opus 5.5 subagent, 2026-10-30 21:15–22:05 EDT (2026-10-31 01:15–02:05Z). Nothing in the repo was changed except this file. Prototypes, profiles and A/B scripts lived in `scratchpad/b2/H/` (session scratch; will not survive).

## Timing basis

- Gate run `01a18f3f20b8-bb90-1` (quinta, merge 20f603f1, 2686 s, ended FUSIO RUBRA on briar-fumus — see H6) and frigida run `01a18f68da2a-81aa-1` (1423 s, of which phasis:frigida 1261 s).
- Per-test seconds from the suite sessions in `~/.rhubarb/mensurae.volumen` (`silva.mensurae('oratio.'|'crusta.'|'briar.', n)`), which split each suite into compilatio / cursus / praevolatus (praevolatus = everything before and around the tests).
- `mensor quaestio statistica` medians: porta:oratio 137.5 s (n=74), frigida:oratio 108.5; porta:gesta 53.8 vs frigida:gesta 9.4; porta:tessera 51.2 vs frigida:tessera 13.5; porta:officina 53.6 vs frigida:officina 34.3; porta:pythonica 204–438 s (last 10 runs); porta:pythonica-arboris 68–90 s; porta:briar 19–44 s; porta:briar-fumus 30–60 s.
- Machine load while I measured: 6–18 on 8 cores (each timing below gives its own `uptime`). Other analysts ran suites in the same worktree at the same time (see the note at the end). Absolute seconds are therefore pessimistic. **Instruction counts and A/B ratios are the reliable numbers.**
- Multipliers: the warm fusio gates run once per merge. Frigida runs every suite once more in a fresh clone, where `build/` is nearly empty. Commit-tier gates (gesta, tessera, briar, crusta have no `gradus=`) also run in ordinary commits that touch what they cover. None of the ordinary-commit savings is counted below.

## Ranked ideas

| id | Idea | min/merge | eff | Ruling? |
|---|---|---|---|---|
| H2 | `excubitor.sh`: scan only the filter's build directory (and prune `build/` in the freshness check) instead of three whole-tree `/usr/bin/find` walks through 415k entries. Measured **56–60 s → 1 s per call**, byte-identical output. | **2.3–3.7** | S | no |
| H5 | `tools/cursor_communis.sh`: compile an instrument's closure with one `bin/compilator --plures`, not one process per object. Measured **0.79–1.06 s → 0.11–0.18 s per wrapper call** (md/extenta.sh); objects and `mandata.tsv` byte-identical. 28 wrappers use it (oratio/*.sh, md/*.sh, crusta/*.sh, computus.sh ...) and pythonica's Prosa/Oratio groups call them dozens of times. | **0.7–1.2** | S | no |
| H4 | silva wrapper preamble: replace the `$(basename "$src" .c)` fork per source (26 forks per call) with parameter expansion, and give `materia_substratum.sh` one newest-header scan instead of 7 × ~300 `-nt` tests. formator.sh **0.27–0.31 → 0.10–0.12 s**, examen.sh **0.42–0.63 → 0.23–0.25 s** per call; output identical. | **0.5–1.2** | S | no |
| H6 | briar-fumus fails fast ("signum absens") whenever the worktree's `bin/briar` is stale. That is **8 of the last 60 runs**, including this merge's FUSIO RUBRA and rerun. Heal `bin/briar bin/briar-spectator` before the gate (producentes phase, or the gate itself). | **0.3–0.6** (expected) | S | light: may a gate heal its own subject? |
| H1 | `lib/stml_macros.c` laxa matcher: memoize `_liberi_effectivi` per node within one query (each sibling-cursor attempt rebuilt both child lists). oraculum test **417.5 G → 345.8 G instructions (−17%)**, RSS 223 → 133 MB; resolutio −16%; outputs of 7 oratio tests identical. | 0.15–0.2 | M | no (internal) |
| H7 | mensor: record time spent in system sleep for each run (CLOCK_MONOTONIC − CLOCK_UPTIME_RAW) and flag affected runs. This is what the crusta 431 s anomaly was. | 0 (measurement) | S–M | yes (schema) |
| H8 | `probatio_oratio_stml`: the second write→read circuit (s2→relecta→s3) is implied by s1 == s2 plus determinism; ~15% of a 19 s test. | ~0.1 | S | yes (test semantics) |

Sum ≈ **4–7 min per merge**. H2 overlaps E3 (see below), so count H2 *or* E3's excubitor share, not both.

---

### H2. excubitor: narrow the scan to the filter directory — 2.3–3.7 min

- **Where:** `excubitor.sh`, two places.
  - The freshness check (sanatio, l.~68): `find . \( -name '*.c' -o -name '*.h' \) -newer "$GRAPHUS" -not -path './build/*' -not -path '*/build/*' -not -path './.git/*' | head -1`.
  - The object stream (l.~105): `find . -type d -name build | while read d; do find "$d" -name '*.o' ... | xargs stat; done`.
- **Callers:** oratio (l.248, before its tests), gesta (l.200), officina (l.330), tessera (l.211). The last three run it after their tests. Every caller passes a trailing-`/` filter such as `gesta/build/`. Also officina/colloquium.sh and vindex.sh (`officina/build/colloquium/`), which are not in the gates.
- **Evidence:**
  - `PS4`-timestamped trace of `./excubitor.sh oratio/build/ -tacitus` at load 8: `find . -type d -name build` takes **22.4 s** and `find ./build -name '*.o'` takes **22.5 s**.
  - `./build` holds **414,977 entries**, mostly fabrica areas and the aedilis thesaurus. macOS `/usr/bin/find` stats every one: 4–6 s sys, the rest I/O wait.
  - The `-newer` check walks the same tree again: **25.2–31.5 s**. With `-prune` it takes **0.10–0.13 s**.
  - Whole script: real 61 s, user 2 s, sys 13 s.
  - (bfs, which the interactive shell aliases as `find`, does the same walk in 2 s. That alias hid the cost at first.)
  - The gap shows in the gate medians: gesta warm 53.8 s vs cold (frigida, compiling everything) 9.4 s; tessera 51.2 vs 13.5.
- **Mechanism:** the awk judge only inspects objects that `congruit()` with the filter. With a trailing `/`, those are objects *directly* in that directory. Objects elsewhere enter `mtempus[]` but are never read for another object's verdict; the res-fabricae directory loop reads only source directories, and those come from the separate M/fabrica streams. So, when `FILTRUM` ends in `/`, is `sub/build/...`, is not nested in another build directory, and exists, emit only `find "./${FILTRUM%/}" -name '*.o' | xargs stat -f 'O %m %N'`. Otherwise keep the old loop. For the freshness check, use `find . \( -name build -o -path ./.git \) -prune -o \( -name '*.c' -o -name '*.h' \) -newer "$GRAPHUS" -print`. It is the same set in the same order: `-name build` prunes exactly what `-not -path '*/build/*'` filtered, and no `.c`/`.h` file is named `build`.
- **Prototype A/B** (copies `basis.sh` / `novum.sh`, `-tacitus -sine-renovatione`, load 9–13):

  | filter | basis | novum | output |
  |---|---|---|---|
  | `gesta/build/` | 56 s | 1 s | IDENTICAL |
  | `oratio/build/` | 59 s | 1 s | IDENTICAL |
  | `officina/build/` | 56 s | 1 s | IDENTICAL |
  | `tessera/build/` | 60 s | 1 s | IDENTICAL |
  | `officina/build/colloquium/` | — | 1 s | IDENTICAL |

  This was a strong test: `include/latina.h` had just been touched, so each relatio was non-empty (`STALA 65` for gesta, ordered lists of STALUM lines plus the CAUTIO line), and `cmp` was byte-for-byte. The freshness list compared against a fixed reference file: 133 paths in both versions, same order.
- **Estimate:** 4 warm calls per merge × 35–55 s = **2.3–3.7 min**. Frigida gains ~0, because its `build/` is small. Ordinary commits touching gesta or tessera also gain ~45 s per gate.
- **Overlap:** the E3 landing for ludus_tessera (765f1f2e) removed the excubitor call from that runner. If E3 later converts oratio/gesta/officina/tessera the same way and drops the call, this gain is subsumed. H2 is S, lands independently and keeps the safety net. Do H2 first, and let E3 decide later whether the net is still wanted.
- **API:** none. **Risk:** low; the unfiltered and non-`/` paths are unchanged. **Effort:** S (one file, ~15 lines).
- **Verify:** run old vs new on the five caller filters and `cmp` the outputs (my method). Touch a header first so the relatio is non-empty.
- **Plant:** in the narrow branch, use `-name '*.c'` instead of `'*.o'`. Excubitor then sees no objects and always says PURUS. **No existing test would catch this**: no gate asserts an excubitor verdict (diagnostica_fumus only greps its signature). Proposed guard: a fumus case that touches `include/latina.h` in a scratch clone and requires `./excubitor.sh -tacitus gesta/build/` to exit 1 naming `STALUM gesta/build/`.

### H5. cursor_communis: one `compilator --plures` per instrument — 0.7–1.2 min

- **Where:** `tools/cursor_communis.sh` `cursor_instrumentum_struere`. It runs `_cursor_compilare` once for the main source and once for each closure source, i.e. one `bin/compilator` process per object. `cursor_fontes_compilare` (the crusta suite, 48 objects) does the same.
  - Users: crusta/{effectus,facies,mandata_census,oraculum}.sh, css/computus.sh, html/{arbor,computus}.sh, materia/oraculum_silvae_struere.sh, md/{arbor,census,computus,extenta,html}.sh, oratio/{arbor,computus,formae,oraculum,quaere,reticulum,sententiae,verba,vocabula,vocabularium}.sh, silva/lexema_mensura.sh, toml/{arbor,computus}.sh, tools/{iudex_unitatum,runae_corpus}.sh.
- **Evidence:**
  - `./oratio/sententiae.sh` takes 1.04–1.24 s and `./md/extenta.sh` 1.01–1.07 s per call; the binaries themselves take ~0.03 s.
  - 21 cache-hit compilator calls for sententiae's closure: **0.68 s** (32 ms each, mostly process start plus thesaurus/identity setup). The same 21 lines through one `--plures`: **0.06–0.07 s**.
  - Probe of pythonica's "Prosa.sententia: delegatio Orationi" group run on a scratch file: **30 subprocesses, 39.3 of 39.4 s wall**. They were 13 × `oratio/sententiae.sh` and 17 × `md/extenta.sh`; the python side is ~0.
  - The Prosa/Oratio/pondera/citata/clientes/vocabula groups total ~88 s of the ~280 s pythonica + pythonica-arboris pair.
- **Mechanism:** write the vocationes (`<flags> -c <src> -o <obj>`, the same line format compile_tests.sh and ludus_tessera use) to `$dir/vocationes.lst`, append the same `mandata.tsv` lines in the same order, then run `"$COMPILATOR" --plures "$vocationes" || return 1`. The binary freshness check (`bin -nt` every object) is unchanged. compilator does not rewrite identical objects, so a warm call still links nothing.
- **Prototype A/B** (md/extenta.sh copy with build dir in scratch; load 9–14): **basis 0.79 / 0.85 / 1.06 s vs novum 0.11 / 0.16 / 0.18 s per call**. Output identical, all 25 objects identical (`cmp`), `mandata.tsv` identical modulo directory. The two binaries differ only in the debug-map object paths, because they were linked in different directories.
- **Estimate:** pythonica + pythonica-arboris wrapper-heavy groups ~88 s; ~75% is wrapper overhead and ~85% of that is removable → **~55 s per merge**. The crusta suite (`cursor_fontes_compilare`, 48 objects) adds ~1.3 s per warm run. Total ≈ **0.7–1.2 min**, more at higher load.
- **Semantics note:** `--plures` keeps compiling after a failing line, like xargs, and exits 1. The wrapper still fails, with more stderr.
- **API:** none (a sourced shell helper). **Risk:** low. **Effort:** S (two functions).
- **Verify:** run each converted wrapper twice; the second run must relink nothing (binary mtime unchanged). `cmp` objects against the old path. `tools/cursoris_oraculum.sh` still judges `mandata.tsv` determinism.
- **Plant:** drop the `|| return 1` after `--plures` and put a syntax error in a closure source. A warm wrapper then silently links the previous object. **Existing tests catch this only cold** (frigida: no object → link error). Proposed: a cursor_communis case in the nexus-instrumentorum fumus.

### H4. silva wrapper preamble: no `basename` forks, one header scan in materia_substratum — 0.5–1.2 min

- **Where:**
  - `base="$(basename "$src" .c)"` in the `silva/fontes/*.c` loop of 26 silva wrappers (formator, examen, differre, renominare, selecta, origo, censor, auspex, haruspex, impressor, percursus, quaestio, transformare, ...), plus officina/{cursor,fusor,interpretare}.sh, tessera, tools/silex_struere.sh and the oratio/briar/ludus_tessera runners.
  - `silva/materia_substratum.sh` (23 users): for each of 7 objects it loops over every `include/*.h` and `materia/fontes/*.h` with `-nt`.
- **Evidence:** checkpointed copy of formator.sh, warm, load ~12. Each segment's cost: silva/fontes loop 0.12–0.155 s, materia substratum 0.08–0.10 s, everything else ~0.1 s.
- **Prototype A/B:**
  - formator.sh (10 calls each, three rounds): **basis 0.31 / 0.28 / 0.27 s vs novum 0.115 / 0.105 / 0.107 s**.
  - examen.sh: **0.63 / 0.42 vs 0.23 / 0.25 s**.
  - Outputs identical (`cmp`).
- **Mechanism:**
  - `base="${src##*/}"; base="${base%.c}"` — the same string, no fork.
  - In materia_substratum, compute the newest header once and test `! [ "$obj" -nt "$recentissimum" ]`. "obj newer than every header ⟺ obj newer than the newest header"; ties still recompile. With no headers both forms keep `recens=1`, because the glob literal is a nonexistent file.
- **Estimate:**
  - merge commit (forma 8–16 s, unci 5–7 s, git-commit hook 16 s in the 20f603f1 merge): ~10–20 s;
  - porta:nexus-instrumentorum, which re-runs every involucrum's preamble several times with 8 concurrent: CPU saved ~30–40 s, wall less;
  - porta:unci (hook runs);
  - pythonica's formator/examen/differre/renominare calls: ~10 s.
  - Total **≈ 0.5–1.2 min**, and every ordinary commit's hook in every worktree gains too.
- **API:** none. **Risk:** low and mechanical. Use `silva.Editio` or `transformare`, not sed, per house rules. **Effort:** S.
- **Plant:** in materia_substratum, take the *oldest* header (`-ot`) instead of the newest. `touch include/latina.h; ./silva/examen.sh x.c` must still print `[materia]` recompiles; E2's plant protocol shows it. porta:nexus-instrumentorum V ("silva_lexicon.h touched → relink") covers the silva/fontes side. Whether any gate touches a *materia* header is unchecked; if none does, add that case.

### H6. briar-fumus: heal `bin/briar` before the gate — 0.3–0.6 min expected

- **Where:**
  - `tools/briar_fumus.sh` step 0 refuses in a worktree when `bin/briar` is stale ("bin/fabrica sanare bin/briar bin/briar-spectator prius").
  - Its porta tag declares `currit="tools/briar.c tools/briar_spectator.c"`, but nothing heals those binaries before it runs.
  - The "ordo portarum per sumptum" schedule runs this cheap gate early. In this merge it ran at 00:07Z and failed after 5.7 s. Later in the same fusio the silva gate's fabrica healed `briar` and `briar_spectator` (SANATUM lines in its filii). The fusio ended RUBRA and was re-run (01a18f68).
- **Evidence:** `mensor quaestio historia titulus=porta:briar-fumus n=60`: 11 fractus, of which **8 have this exact precondition message** (10-14 ... 10-31; read via `quaestio acta`). The others were 1 missing `bin/fasciculum_instrumentum`, 1 genuine build failure and 1 duplicate.
- **Mechanism:** add `bin/briar` and `bin/briar-spectator` to fusio's producentes phase, i.e. treat `currit=` binaries as producers. Alternatively, let the gate run `bin/fabrica sanare bin/briar bin/briar-spectator` itself in worktrees (it already knows the command). Either way, the gate still tests the tree's binary.
- **Estimate:** ~13% of merges × one avoidable failed fusio. The rerun costs receipt re-checks plus any not-yet-run tail (here ~2.7 min of gates before frigida), plus the human/agent turnaround. Expected ≈ **0.3–0.6 min/merge**; it also removes a source of red merges.
- **Ruling:** may a gate (or fusio) heal its own subject? The gate's purpose ("the tree's binary, not ~/.bin") is kept; only "forgot to rebuild" stops being a failure.
- **Plant:** remove the heal → the next merge that touches briar inputs reproduces "signum absens" (the history above shows that).

### H1. stml_macros laxa matcher: memoize effective children per node — 0.15–0.2 min

- **Where:** `lib/stml_macros.c` `_laxa_liberos_congruere_ab` (l.4076). On every call it creates two fresh Xars and refills them with `_liberi_effectivi(candidatus)` and `_liberi_effectivi(forma)`. The caller `_exemplar_petere` (cursus fratrum, l.4586) calls it once for each `initium` in 0..cnum-1, and `_fratres_congruere` (l.4242/4263) calls it for every nested attempt during backtracking. Only `oratio/partes/resolutio.stml` uses `cursus="fratrum"`.
- **Evidence:**
  - `sample` 1 ms of probatio_oratio_oraculum (30–34 s per run; 58 s and 419 G instructions here): `_exemplar_petere` 65% inclusive.
  - `_liberi_effectivi` reached from l.4106 (candidate) costs 18.5% and from l.4108 (form) 3.9%, with xar_creare another ~2%. 15.6 points of that come through the nested `_fratres_congruere` calls.
- **Mechanism:**
  - Effective-children lists are read-only in `_fratres_congruere` (xar_obtinere/numerus only).
  - Aliases are built once per context (`aliasa_constructa`), and the transparency set is fixed.
  - Nothing mutates the tree inside one top-level `_exemplar_petere` call.
  - So: a node-pointer → Xar map in the context, live only for the duration of a top-level call. Wrap `_exemplar_petere` as begin-memo → intus → end-memo, and pass straight through when a memo is already live. `_laxa_liberos_congruere_ab` takes both lists from it.
- **Prototype** (patched copy, linked against `oratio/build/*.o`):
  - oraculum **417.45 G → 345.84 G instructions (−17.2%)**, max RSS 223 → 133 MB;
  - resolutio 9.20 → 7.76 G (−16%);
  - canon, arbor, partes, totalitas unchanged (±0.2%);
  - stdout of all 7 tests identical after masking timings.
  - Re-profile after the change: the next costs are `xar_obtinere` 16% self and `_exemplar_petere_intus` 13% self (diminishing; C1-type territory).
- **Estimate:** ~5 s per oratio run × 2 (gate + frigida) + small pythonica oratio calls ≈ **0.15–0.2 min**.
- **API:** none; static helpers plus 4 private context fields. **Golden data:** oratio computus does not run resolution, so no re-gold there. *Unchecked:* whether any other computus gold counts allocations of laxa expansions, e.g. silva exemplaria. Fewer `xar_creare` calls would lower such a count → possibly one re-gold (cf. the A4/B3 precedent).
- **Risk:** low–medium (memo lifetime). **Effort:** M (~120 lines in one file; the amalgam picks it up).
- **Verify:** probatio_stml_exemplaria.c (14 cursus-fratrum references), oratio resolutio/oraculum, silva lint/obsoleta/inutilia census lines.
- **Plant:** drop the key-equality check in the probe (return whatever list occupies the slot). Effective children go wrong → stml_exemplaria fratrum cases and oratio resolutio assertions fail.

### H7. Mark runs that spanned system sleep — measurement only

See the crusta anomaly below. At `cursus incipere` and `finire` (and in `mensor_suitae`), store `clock_gettime(CLOCK_MONOTONIC)` and `CLOCK_UPTIME_RAW`. On macOS the first counts sleep and the second does not; both are system-wide, so different processes can compare them. Record `dormitio_ms = Δmono − Δuptime` and keep it out of medians and cohort deltas. This saves no time, but this batch found two suite sessions (10-30 16:31Z and 10-31 01:03Z) and one frigida total inflated 6–18 min by sleep. Schema change → ruling.

### H8. probatio_oratio_stml circuit II — ~0.1 min, needs a ruling

`_circuitum_arboris` writes s1, reads it into lecta, writes s2 and requires s2 == s1. It then reads s2 into relecta and writes s3. Once s2 == s1 byte-for-byte, read(s2) = read(s1) and s3 = s2, unless the reader or writer is nondeterministic, which is the only thing circuit II can still catch. The second read plus third write are ~15% of the 19 s test → ~3 s per run, 2 runs per merge. Fran decides whether "determinism across two cycles" is a property worth 6 s per merge.

---

## The frigida crusta anomaly (431 s vs usual 40–95 s): the laptop slept

- **Where it went:** the crusta suite session for 20f603f1 in frigida (momentum 01:03:06Z) has `suita.tempus.totum` 410 s, of which **`probatio_crusta_effectus` cursus = 375.75 s**. Normally that test takes 2.4–4.4 s; the gate run of the same commit 57 minutes earlier took 3.1 s. Compilation took 6.8 s and every other test was normal.
- **Cause:** `pmset -g log`: `2026-10-30 20:56:36 -0400 Sleep — Entering Sleep state due to 'Clamshell Sleep' … Using Batt (Charge:66%) 371 secs`, then `21:02:47 Wake … HID Activity`. That is 00:56:36–01:02:47Z, entirely inside the crusta suite (00:56:15–01:03:27Z) and inside that one test. 375.75 − 371 = **4.75 s of real work**. The process was frozen, not waiting on a lock or a sleep() call and not contending.
- **Second case found:** the crusta session at 2026-10-30 16:31Z (7b8ee375, not a frigida run): totum 1325 s, effectus 188 s, totalitas 1062 s. The machine was cycling through clamshell/maintenance sleep at 4% battery from 10:12 EDT onward (pmset log 12:02–12:14 EDT shows sleep entries).
- **Can it recur?** Yes, whenever the lid is closed on battery during a fusio or frigida. `caffeinate` (the log shows several instances holding PreventUserIdleSystemSleep) does not prevent *clamshell* sleep, and `caffeinate -s` only works on AC power.
- **Collateral:** this fusio's "frigida 21.0 min" includes 6.2 min of sleep; any cohort or median that includes these runs is polluted. The 10-27 frigida:oratio 508 s outlier cannot be checked, because the pmset log starts 10-28.
- **Remedy:** H7, so such runs are self-describing. No code change saves time here.

## Checked, not worth it (or someone else's lever)

- **Pythonica's python is not the cost.** Subprocess tracing of the pure suite gave 98.3 s of subprocess wall in 98.4 s of suite wall (41 processes) before the run aborted. The Prosa probe gave 39.3 of 39.4 s. cProfile would only show `subprocess.communicate`. The levers are the wrappers it launches (H4, H5).
- **oratio warm praevolatus 59–129 s** (cold frigida: 11–20 s) = the excubitor (~55 s; H2) + up to 76 raw-clang recompiles after any header touch (~15 s at low load, ~40 s at load 12; E3) + nexus -renovare (2 s) + 3 × `git ls-files` (<1 s). After H2 and E3 it should be ~5–10 s.
- **oratio stml test:** interning probe (`tabula_dispersa_invenire` 8.9% self at the probe line) is C-batch territory (C7 skipped as an API addition). `materia_sedes_verificare` re-writes the tree with sedes on (~20%); it is the property under test.
- **briar warm gate (20–44 s):** praevolatus 9–24 s (corpus symbols + facies capsule + header scan); tests ~6 s. Little left after E3.
- **briar frigida (117–316 s, praevolatus 157–163 s):** cold compile including `vendor/sqlite3.c` -O2 and the silva amalgam. That is E4's question (a persistent cache for frigida).
- **briar-fumus runtime (30–60 s):** each .thistle builds through clang in a fresh area. That is the end-to-end property; cross-area build caching would be a briar product change.
- **pythonica-arboris "portae debitae" 23.6 s, "probatio_currere" 16.2 s, "lint latinus praevius" 12.8 s; pure "umbra"/"receptum vivum"/"ordo portarum" 10–17 s each:** these run real gates in photographed clones (`git add -A` into a temp index + clonefile). That is the mechanism under test. Not profiled further.
- **pythonica "legati" 13.7 s:** one persistent legatus process; the cost is its startup over the 2.7M-row nexus index. Not examined further.
- **excubitor awk + fabrica directories:** ~1.5 s per call; not worth it after H2.

## Notes for the lead (not ideas)

- **Concurrent runs in quinta.** While I traced `pythonica/probatio_silva.py` (21:44–21:46 EDT), another process (PID 20302, started ~21:45, cwd quinta; a batch-F binary from `scratchpad/b2/F` was also running) ran the same suite. Both share `build/pythonica/`. My run aborted at "usus + renominare" ("collisio: nomen novum iam registratum"), and theirs may have been disturbed too. I did not re-run the live pythonica suite.
- **Changes I did not make.** `include/latina.h` got a new mtime at 21:31 with no content change. This invalidates every header-keyed object (the next gates in quinta recompile everything). `tools/vexilla.sh` does exactly this when `build/vexilla.sigillum` is missing or differs, which suggests someone ran vexilla.sh against quinta. Also, `git status` now shows `M pythonica/probatio_silva.py`, `M pythonica/silva.py` and untracked `ideae-G.md` / `ideae-J.md`. None of these are mine.
- **`source tools/vexilla.sh` from zsh** (where `BASH_SOURCE` is empty) resolves `_vexilla_radix` to the repo's *parent* and would `touch ../include/latina.h`. A stale `projects/build/vexilla.sigillum` from 2026-09-02 shows this has happened before. Low-priority hardening: fall back to `$0`, or refuse when `BASH_SOURCE` is empty.
