> Raw analyst report, batch J of budget B2 (2026-10-31), filed verbatim as evidence for ../budget-fusionis.md.

# Batch J — fixed per-process / per-call overhead across the pipeline

Analyst: Opus 5.5 subagent, 2026-10-30/31. I changed nothing in the repo except this file. Probes, replicas and A/B copies live in `scratchpad/b2/J/`: a replica tree `wr/` with symlinked sources, a scratch thesaurus, and scratch mensor volumes (`MENSOR_VOLUMEN`/`DIARIUM`/`DIURNA`).

Machine load swung between 6 and 67 while I measured, because other worktrees were committing. Every timing below says "min of N" and gives the load. Use the **counts** and the **ratios**; the absolute ms are pessimistic.

Timing basis:
- fusio 1 `01a18f3f20b8-bb90-1`: 44.8 min, red on briar-fumus.
- fusio 2 `01a18f68da2a-81aa-1`: 23.7 min, including frigida 21.0.
- merge commit 20f603f1: 1.6 min (a small merge).

## Ranked ideas (estimated minutes saved per merge)

| # | Idea | min / merge | eff | Ruling? |
|---|---|---|---|---|
| J1 | fabrica regenerates the same generator outputs in every process. Add a run-scoped regeneration memo shared by the fabrica processes of one commissio / fusio (115 s of *repeat* IUDICIUM regenerations measured in the two fusio runs) | **1.3–1.9** | M | yes |
| J2 | silva wrappers: `$(basename "$src" .c)` subshell per source (38 scripts), and `materia_substratum.sh` scans 408 headers per object (22 wrappers; E2 missed it). The hook should call the binaries directly after one freshness call | **0.8–1.5** | S | no |
| J3 | `/usr/bin/git` and `/usr/bin/clang` are xcrun trampolines costing +5–8 ms per exec, over ~3 000 git and ~4 500 clang execs per merge. Put direct paths first and export `SDKROOT` | **0.6–0.9** | S | yes |
| J4 | content seal (`_sigillum_contenti`, 5 git processes, ~125 ms) recomputed ~260× per merge on a frozen tree. Reuse the previous seal, add a clean-tree fast path, and seal once after a commit instead of 62× | **0.3–0.5** | S–M | no |
| J5 | frigida radix `probatio_una`: each of 450 tests links ALL 419 objects + 4 frameworks (289 ms, 18 MB binary vs 61 ms / 133 KB closure). It also forks 3 perl + 2 bc + `source vexilla.sh` + `print-prog-name` per test | **0.5–0.6** | S / M | (b) yes |
| J9 | briar, oratio, officina, gesta, tessera and ludus_tessera compile **and** link every test file from scratch on every run (84 tests × ~0.3–0.4 s); fold into E3 | **0.4–0.5** | M | no (E10 precedent) |
| J6 | `bin/aedilis` derivation runs `system("git rev-parse --short HEAD > tmp")` **twice** per call (~611 derivations per merge); the result is only a comment | **0.3** | S | no |
| J7 | aedilis-emitted `struere.sh` `vetustum()`: every header against every object, ~1.02 M `-nt` tests per aedilis gate plus 161 programs in instrumenta. Compare against the newest header once | **0.3** | S–M | no |
| J10 | `tools/vexilla.sh` costs ~16 ms per `source` (cksum, cut, cat, dirname subshells) × ~1 300 sources per merge | **0.2–0.3** | S | no |
| J8 | every single-call `bin/compilator` process runs `clang -print-prog-name=clang` (~18–23 ms of its 24 ms hit), ~650 such processes per merge | **0.15–0.25** | S | small |
| J11 | small items: hook per-file `git diff --name-only` (×2 per .c); `mensor_suitae` 4 perl + 2 bc per test; `instrumenta_fumus` grep per file | **~0.15** | S | no |

The sum is 5.0–7.2 min. J3 shaves a little off everything else, so after a ~15 % overlap discount, **≈ 4.5–6 min per merge**.

Load-bearing new facts:
- **(a)** fabrica's per-process regeneration judgments are the largest fixed per-call cost in the pipeline.
- **(b)** the xcrun trampoline tax applies to every git and clang exec in the house.
- **(c)** E2's per-object scan survives in `materia_substratum.sh` and in aedilis-emitted scripts.

---

### J1. Run-scoped regeneration memo across fabrica processes — **~1.3–1.9 min**

**Where:**
- `lib/fabrica.c`: `_regeneratione_iudicare` and the per-run memo `sutura->regenerationes` (worklog 2026-10-23, "generator judgments once per run").
- Callers in `pythonica/silva.py`:
  - `_producentes_fusionis_sanare` (`iudicare -plenus -omnia`, then `_consilium_agere` → more `bin/fabrica` processes);
  - `_porta_per_gradum` (`bin/fabrica probare`);
  - `_porta_per_fabricam` (`iudicare -omnia` + `sanare`);
  - the generata gate (`iudicare -plenus -omnia`).

**Evidence** (`mensor quaestio filii … profunditas=99`, rows with status `IUDICIUM`):

| run | IUDICIUM rows | distinct actions | total | **repeats beyond the first** |
|---|---|---|---|---|
| fusio 1 | 262 | 44 | 114.9 s | **90.0 s** |
| fusio 2 | 140 | 51 | 58.6 s | **25.3 s** |

Worst repeaters in fusio 1:

| action | times judged | total |
|---|---|---|
| speculum_hospes | 7× | 34.4 s (~5 s each) |
| corpus_silicis | 4× | 30.2 s (~7.5 s) |
| tabulae_silvae | 11× | 17.8 s |
| capsula_assets | 6× | 8.1 s |
| fiducia_diffisa | 9× | 5.3 s |
| pecunia_monetae | 8× | 3.7 s |

- `phasis:producentes` alone judged corpus_silicis 5× and speculum_hospes 5× within one phase: 67 s of its 135 s.
- After that, porta:radix, porta:aedilis, porta:generata and porta:silva each re-judge speculum_hospes, tabulae_silvae and capsula_assets.
- Historically (`mensor quaestio iudicia`): tabulae_silvae has 5 105 regenerations, speculum_hospes 1 889, corpus_silicis 1 780 (37 500 s total), bearssl_amalgama 629 at ~74 s each.

**Mechanism:**
- The existing per-process memo is sound because "generator inputs live in the tree, never in a member's area". Each fabrica PROCESS starts it empty, and silva.py spawns many fabrica processes per run.
- Fix: silva.py exports `FABRICA_MEMORIA_REGENERATIONUM=$(mktemp -d)` for the duration of one `commissio` / `fusio_probare` and deletes it at the end. This is the same lifetime rule as aedilis's `MEMORIA_ORACULI`: "arbor intra cursum gelata est".
- fabrica persists `regenerationes` entries there, keyed on: action title + command + fabrica binary digest + the action's declared-input digest (the memorabilis key material, `fabricae_clavem_memoriae`) + the digests of declared `instrumentum_domus` inputs.
- A heal (SANATUM) changes downstream digests, so dependents miss, as they should.
- Outputs are still compared afresh, so only the *regeneration run* is memoized, exactly as in-process today.

**Estimate:** 70–90 % of the 115 s of repeats → **1.3–1.7 min**, plus the merge commit's `phasis:fabrica` repeats (unmeasured).

**What it weakens:**
- Today a generator whose UNDECLARED inputs change mid-run is re-judged by the next fabrica process. Example: a gate rebuilds `bin/x` between two judgments of an action that runs `bin/x` but does not declare it.
- With the memo it would be served stale until the run ends.
- Mitigations:
  - key on declared tool inputs;
  - fusio requires a clean, committed tree;
  - `phasis:instrumenta portarum` rebuilds tools BEFORE the gates.
- The residual hole is the same class as non-memorabilis generators today.

**PLANT:** in `probatio_fabrica_iudicium`, two `bin/fabrica` processes share one memo dir, with a declared input byte changed between them; the second must regenerate. Plant: drop the input digest from the key → that case goes red. **No existing test would catch an unsound cross-process memo**: all current memo tests are single-process.

**Ruling needed:** may a fabrica judgment trust a regeneration done by an EARLIER fabrica process of the same commissio / fusio (run-scoped, tree frozen)?

**Effort:** M (memo storage + key + silva.py env + one test case).

### J2. silva wrapper fixed overhead: basename subshells + materia header scan — **~0.8–1.5 min**

**Where:**
- 38 wrapper scripts (`silva/*.sh`, plus the officina and tessera wrappers) loop `for src in fontes/*.c; do base="$(basename "$src" .c)"`, one fork+exec per source (26 in silva).
- `silva/materia_substratum.sh` l.44–55 is sourced by 22 wrappers (examen, formator, censor, selecta, renominare, transformare, …). Per materia object (7) it runs `for caput in include/*.h materia/fontes/*.h` (408 headers), a `-nt` test each. This is the exact per-object scan E2 removed elsewhere; the caller has already called `caput_recens_directoria` over a SUPERSET of those dirs.
- `examen.sh` l.80–87 rewrites `examen.manifestum` each call with two `ls`.

**Evidence:** examen wrapper replica (`wr/`, real sources symlinked, warm objects) with a `PS4=$EPOCHREALTIME` trace:

| trace line | time | calls |
|---|---|---|
| examen.sh:49 (basename) | 159 ms | 52 |
| materia_substratum:51–52 (2 856 `-nt` tests) | 196 ms | 14 |

A/B at load ~22, min of 8 ×3:
- original wrapper: **217–237 ms**;
- with `${src##*/}` + `${base%.c}` and a single newest-header pass in materia_substratum: **86–107 ms**.
- The examen binary itself, in the replica, takes 13 ms.

Isolated loops at load 40: basename loop 110 ms vs 8 ms expanded; materia scan ~50 ms.

**Call counts:**
- A merge commit with F_ch .c+.h and F_c .c files makes 3·F_ch + 2·F_c wrapper calls:
  - phasis:forma;
  - phasis:unci;
  - the hook's examen, `formare_viae -index` and `formator -delta`.
- For merge 7dc70441 (F_ch 76, F_c 54) that is **336**.
- Fusio gates add some hundreds more (nexus-instrumentorum ~400 per E, formator and differre fumi, hook runs inside unci fumus, pythonica). Count exactly by adding a log line to copies.

**Mechanism:**
- (a) mechanical: `base="${src##*/}"; base="${base%.c}"` in 38 scripts (silva.Editio or a transformare rule).
- (b) materia_substratum: `declare -F caput_recentius && caput_recentius "$obj"`, the same pattern instrumentum_nectere already uses, with a one-pass fallback.
- (c) optional: the manifest is written by glob, not `ls`.
- (d) hook, `formare_viae.sh` and phasis:forma: one `INSTRUMENTUM_NECTERE_SOLUM=1 ./silva/examen.sh` (or formator) per hook, then `silva/build/<tool>` per file, as `silva/examen_vectis.sh` already does. This removes the remaining ~90 ms (bash, vexilla, manifest) per file.

**Estimate:** 130 ms × ~350 calls (commit) + ~400–600 calls (gates) ≈ 1.6–2 min at load 20. Idle is perhaps 60 % of that → **0.8–1.5 min**. (d) adds ~0.3 min on large merges.

**What it weakens:** nothing. Both changes are equivalences: basename ≡ expansion for these paths; "some header newer" ⟺ "newest header newer" (E2's argument; ties are stricter).

**PLANT:**
- materia: drop the `caput_recentius` call → touching a `materia/fontes/*.h` header must still relink. No nexus-instrumentorum case touches a materia header today, so add one (case VII). Without it, **no existing gate catches it**.
- expansion: plant `${src#*/}` → object path with slashes → compile error → nexus fumus red.

**Effort:** S.

### J3. xcrun trampolines on every git and clang exec — **~0.6–0.9 min**

**Where:**
- `/usr/bin/git`, `/usr/bin/clang` and `/usr/bin/python3` are the same 200 KB trampoline (78 hard links).
- PATH resolves `git` and `clang` there for every script, for `system()` in C tools, and for compilator's `processus` "clang".

**Evidence** (min of 20):

| exec | via shim | direct |
|---|---|---|
| `git version` | 9.0 ms | 4.1 ms |
| `git rev-parse HEAD` (load 13) | 15.2 ms | 9.1 ms |
| `clang --version` | 10.6 / 15.4 ms | 5.3 / 7.0 ms |
| tiny `clang -O2 -c` | 33.6 ms | 22.0 ms |

So the shim costs **+5–8 ms per exec**.

**Counts per merge** (script reading, approximate):
- clang ≈ 3 000 in frigida (radix 414 lib + 450×(compile+link+print-prog-name); subsystem libs ~450 + 84 test compile+links; crusta, fabrica suites) + ~1 500 in warm gates (instrumenta 161×2, fumi, briar-family) ≈ **4 500**.
- git ≈ 1 222 (J6) + ~1 200 (seals, J4) + hook per-file + probatio_git oracles + misc ≈ **3 000**.

**Mechanism:** silva.py (and `frigida_probare.sh`, which inherits) creates `build/xcrun_directa/{git,clang,clang++,cc}` → `$(xcrun -f …)`, prepends it to PATH, and exports `SDKROOT="$(xcrun --show-sdk-path)"`.
- Do NOT prepend `Developer/usr/bin` wholesale: it would shadow Homebrew `python3`.
- Verified:
  - objects are byte-identical (`cmp` shim vs direct + SDKROOT);
  - `-print-prog-name=clang` is identical, so the compilator identity is unchanged and every cache key keeps hitting;
  - `git --exec-path` resolves through a symlink.

**What it weakens:**
- (1) Direct clang WITHOUT `SDKROOT` cannot find `<stdio.h>`. The export is mandatory, so a broken export turns every compile red at once. That is loud, not silent.
- (2) The shim adds `-I/usr/local/include` (verified with `-###` diff). Direct + SDKROOT does not. That directory holds only `fuse/`, `fuse.h` and `node/`, and no house file includes them.
- (3) A global `SDKROOT=macOS` could leak into iOS/Android builds that rely on clang's default sysroot. `tools/ios/*` use `xcrun` explicitly, so they are unaffected, but this needs a check with the ios-struere and android-struere gates.

**PLANT:**
- export `SDKROOT=/nonexistent` → every suite red (loud).
- For (2) and (3) no existing test would notice. The ios-struere and android-struere gates are the check.

**Ruling needed:** may the pipeline bypass xcrun, and accept dropping `/usr/local/include` from clang's search path?

**Effort:** S.

### J4. Content seal: reuse it, fast-path the clean tree, seal once per receipt sweep — **~0.3–0.5 min**

**Where** (`pythonica/silva.py`):
- `_sigillum_contenti` l.4312 runs `rev-parse --git-path index`, copies the index, then `git add -u`, `write-tree` and `ls-files --others`. `sigillum_arboris` calls it every time.
- Callers per gate:
  - `receptum_vivum` → `receptum_validum` (1 seal);
  - `_porta_cruda` l.2403 and l.2414 (2 seals: before and after).
- `recepta_viva()` (used by `recepta_viva_rancida_delere` after EVERY commit) seals **once per receipt file**. Quinta has **62** `.viva.json`. `portae_pendentes` already seals once and reuses it.

**Evidence:** one seal costs 118–144 ms (5 runs, load 8): rev-parse 15–19, add -u 30–37, write-tree 22–28, ls-files -o 46–60.

**Counts per merge:**

| where | seals |
|---|---|
| post-commit sweep | 62 |
| fusio 1: receipt checks | ~53 |
| fusio 1: raw gates × 2 | ~80 |
| fusio 2: receipt checks | 53 |
| other | ~10 |
| **total** | **≈ 260 ≈ 32 s** |

**Mechanism:**
- (a) `recepta_viva()` computes the seal once per call and passes it down (`receptum_validum(via, sig=…)`). There is no subprocess between iterations, so this is exactly equivalent.
- (b) `_portas_currere` hands gate i's post-run seal to gate i+1's receipt check and pre-seal. Nothing runs between them, so this is equivalent; concurrent outside edits race today too.
- (c) clean-tree fast path: `git status --porcelain -z` (58 ms). If it is empty, the tree is `HEAD^{tree}` (15 ms). This is the fusio case; a dirty tree takes the full path.
- (d) cache `--git-path index` per process.

Result ≈ 260 → ~60 seals at ~70–125 ms.

**What it weakens:** nothing, as long as reuse never spans a gate or subprocess run.

**PLANT:** reuse a seal ACROSS a gate run → pythonica sections "receptum vivum: porta() sigillo ligata" and "umbra: receptum sigillo ligatum" (rancida cases) must go red.

**Effort:** S–M.

### J5. frigida radix: per-test link-everything and per-test fixed forks — **~0.5–0.6 min**

**Where:** `compile_tests.sh` `probatio_una` (l.~622–650), the FILA path frigida uses. For each of 450 tests it runs:
- `source tools/vexilla.sh` (J10);
- `cat $OBIECTA`;
- 3× `perl -MTime::HiRes` and 2× `bc`;
- a single-call `bin/compilator` (J8);
- `clang … test.o $obj_files -framework Cocoa -framework Security -framework WebKit -framework IOKit`, where `$obj_files` is all **419** objects.

**Evidence** (probatio_xar):

| | link | binary | startup | dylibs |
|---|---|---|---|---|
| all objects | 289 ms min | 18.4 MB | 13.4 ms | 4 frameworks |
| closure (as fabrica links it) | 61 ms | 133 KB | 7.5 ms | — |

- 450 tests × ~228 ms ≈ **100 s CPU** and **~8 GB written** per frigida.
- Fixed forks ≈ 3×10 + 2×9 + 16 + 20 (print-prog-name) ≈ 80 ms × 450 ≈ 36 s CPU.
- FILA = 4, so wall time ≈ ¼.

**Mechanism:**
- (a) **no ruling needed:**
  - time the compile and the run in ONE perl per test (or let credo's plagula time the run);
  - read the vexilla array once in the parent (already exported via `INCLUDE_LINEA`);
  - pass the clang identity once (J8).
- (b) **needs a ruling:**
  - link against `build/libradix.a` (one `ar` of the 419 objects per run, ~1 s) plus frameworks with `-Wl,-dead_strip_dylibs`; or
  - link the per-test closure (aedilis `--enumerare`, as `cursor_communis` does).
  - Keep ONE all-objects link (a dummy main, or `ld -r`) per run to preserve "every lib links together, no duplicate globals".

**What (b) weakens:** frigida would stop proving that each test links against the whole library set. That property is then proven once per run instead of 450×. The warm radix gate already links closures, so frigida would come *closer* to the gate.

**PLANT:** a duplicate global symbol in two unrelated libs → the retained single all-objects link must go red. Without that retained link, the archive variant would hide it.

**Ruling needed:** (b) only.

**Effort:** (a) S, (b) M.

### J9. briar-family runners compile+link every test unconditionally — **~0.4–0.5 min** (scoping for open E3)

**Where:** the per-test loop `clang $GCC_FLAGS $INCLUDE_FLAGS "$test_file" $obj_files -o "$bin"` (compile and link in one step, every run):

| runner | line | tests | lib objects |
|---|---|---|---|
| `briar/compile_probationes.sh` | l.367 | 20 | 84 |
| oratio | l.299 | 19 | 77 |
| officina | l.299 | 15 | 48 |
| gesta | l.170 | 7 | 33 |
| tessera | l.181 | 15 | 57 |
| ludus_tessera | l.346 | 8 | still per-test, after its E3 lib migration |

`oratio` also still calls `excubitor.sh` every run (l.248), which ludus_tessera's E3 removed (~40 s false STALA after a header touch).

**Evidence:**
- briar: compile of probatio_briar_arbor.c 177 ms and probatio_briar_amalgama.c 241 ms; link against 84 objects 74–75 ms.
- 84 tests × ~0.3–0.4 s ≈ **30 s per full run** of these six gates. Fusio 1 ran all of them.

**Mechanism:** while converting each runner to `bin/compilator --plures` (E3), also:
- (1) put the test file's `-c` into the vocationes;
- (2) link only if the link signature (flags + object identity) changed, the `AEDILIS_NEXUS_SERVARE` pattern of E10.

Per runner, E3 still has to convert: lib objects behind "caput recentissimum" (header-touch cost per E: briar 81/84, tessera 44, gesta 32/33, officina 32), the vendored sources already on `_vendicatum_compilare`, and oratio's excubitor call.

**What it weakens:** nothing beyond E10 (linking is deterministic).

**PLANT:**
- E10's plant: same mtime, new inode → relink.
- change an escape in a test file → it must recompile through the compilator key (as in E3 ludus_tessera).

**Effort:** M (one runner at a time).

### J6. aedilis derivation: two `system("git rev-parse --short HEAD")` per call — **~0.3 min**

**Where:**
- `tools/aedilis.c` `_commissum_obtinere` (l.224): `system()` → `/bin/sh` → git, writing `build/aedilis/commissum.<pid>.tmp`.
- It is called at l.1350 (manifest) and again at l.1419 (struere.sh).
- The value lands only in `commissum="…"` in the manifest and `| commissum: …` in the struere.sh comment.

**Evidence:**
- PATH shim log: exactly 2 git execs per `bin/aedilis probationes/probatio_xar.c --exitus …`.
- Derivation 38.8 ms with real git vs 23.5 ms with a fake git exiting immediately (two `sh` forks still paid); without any `system()` it would be ~10 ms.
- So ~29 ms of a 39 ms derivation.
- Counts per merge: aedilis gate 450 derivations + instrumenta_fumus 161 (+ fabrica-fumus etc.) ≈ 611 × 29 ms ≈ **18 s**.

**Mechanism:** compute once per process (static memo). Also prefer `AEDILIS_COMMISSUM` from the environment (gates export it once), or read HEAD in-process via `lib/git` (already has ref resolution: probatio_git "resolutionem refum").

**What it weakens:** nothing (comment-only metadata). The abbreviation length may differ if `lib/git` doesn't do uniqueness: use a fixed 8 hex digits.

**PLANT:** none would catch a wrong hash (metadata, not under test). A cheap guard: aedilis gate compares the struere.sh header's commissum to `git rev-parse --short HEAD` once.

**Effort:** S.

### J7. aedilis-emitted `struere.sh`: `vetustum()` checks every header against every object — **~0.3 min**

**Where:** the generator in `lib/aedilis.c` (`aedilis_scriptum_scribere_ad`) emits `vetustum () { … for c in $CAPITA; do [ "$c" -nt "$obj" ] && return 0; done }`, called once per `compilare` line.

**Evidence:**
- Across the 467 `build/aedilis/probatio_*/struere.sh`: 31.7 objects per test and **1 023 907** header × object `-nt` tests per aedilis gate in the all-fresh case.
- Largest: probatio_vicus_iussa, 153 objects × 163 headers. Its fresh-case replica (scratch objects touched newer) takes **224 ms**, i.e. 9 µs per test.
- ≈ 9 s per aedilis gate, plus instrumenta's 161 programs (E measured ~11 s).

**Mechanism:** emit `_recens=` once (newest of `$CAPITA`), then `vetustum` compares `[ "$_recens" -nt "$obj" ]`. Regenerate the committed GENERATUM scripts.

**What it weakens:** nothing (E2's equivalence).

**PLANT:** add a case to `tools/aedilis_porta.sh`: `cp -p` a closure header with a NEWER mtime → `[obiectum] lib/xar.c` must appear. Plant `-ot` in the emitter → that case goes red. **No existing case touches headers** for aedilis scripts.

**Effort:** S–M (emitter + regeneration of GENERATUM artifacts through the generata gate).

### J10. `tools/vexilla.sh` costs ~16 ms per source — **~0.2–0.3 min**

**Where:** `tools/vexilla.sh` l.51–54:
- `$(cd "$(dirname …)" && pwd)`;
- `$(printf … | cksum | cut …)`;
- `$(cat sigillum)`.

**Evidence:** `bash -c 'source tools/vexilla.sh'` takes 22.6 ms vs 6.9 ms for `bash -c :` (load 12).

**Count:** every wrapper call (J2) + 450 `probatio_una` + runners + hook ≈ 1 300 per merge.

**Mechanism:**
- store the joined flag string itself in `build/vexilla.sigillum`;
- `read -r v < file` (builtin) and compare with `"${VEXILLA_C89[*]}"`;
- `_vexilla_radix="${BASH_SOURCE[0]%/*}/.."`.
- Same semantics (any flag change ⇒ touch latina.h).

**What it weakens:** nothing.

**PLANT:** porta:vexilla ("fumus vexilla") already plants a flag change; it must still touch latina.h.

**Effort:** S.

### J8. bin/compilator: `clang -print-prog-name=clang` once per process — **~0.15–0.25 min**

**Where:** `lib/compilator.c` `_identitas_compilatoris` (l.211). Memoized only within a process ("SEMEL PER PROCESSUM").

**Evidence:**
- Single-call hit: 23.6 ms min, against a `print-prog-name` of 18.1 ms (load 12).
- `--plures` with 20 lines costs 29.7 ms, so the marginal cost is ~0.3 ms per line.

**Single-call processes per merge:**
- frigida radix `probatio_una`: 450;
- `cursor_communis _cursor_compilare` (crusta: 48 sources + 20 tests, × gate + frigida);
- vendored calls;
- hook `iudex_unitatum` paths;
- ≈ 650 × ~20 ms ≈ 13 s.

**Mechanism:**
- (a) runners call `--plures` (compile_tests `probatio_una` can't easily, so (b));
- (b) `COMPILATOR_CLANG_VERUM` exported once per runner (the path from one `print-prog-name`) and honored by `_identitas_compilatoris` (stat + thesaurus memo as today);
- (c) or memo keyed on (`/usr/bin/clang` stat, `/var/db/xcode_select_link` target, `DEVELOPER_DIR`).

**What it weakens:** (b) is run-scoped, so nothing. (c) needs a ruling: does it catch an Xcode switch?

**PLANT:** compilator fumus "compilator alius" (other compiler → no hit) must stay red when the exported path points to a different clang.

**Effort:** S.

### J11. Small items — **~0.15 min total**

- Hook l.~132 and `formare_viae.sh -index`: `git diff --name-only -- "$via"` per file (×2 per .c, 1 per .h ≈ 130 git calls in a 76-file merge ≈ 2.6 s). Fix: one `git diff --name-only` up front, then a set lookup.
- `tools/mensor_suitae.sh`: `mensor_suitae_nunc` (perl) 2× and `_duratio` (perl + bc) 2× per test ≈ 55 ms × 59 tests (briar, crusta, oratio) × 2 runs ≈ 7 s. Fix: one perl per test or credo-reported ms.
- `instrumenta_fumus.sh` l.63: `grep -qE '^(principale|main)'` once per judged .c file (≤ 2 512 files × ~4 ms). Fix: one `grep -lE` over the list.

Effort S, no rulings; plants are unnecessary (pure plumbing).

---

## Counts and costs measured (min ms, load in brackets)

| Per-call thing | Cost | Calls / merge |
|---|---|---|
| credo binary startup over empty main | +2.5 ms (8) | ~650 frigida + few warm |
| credo per section (1 000 empty sections) | 0.037 ms | ~10–20 k |
| `bin/mensor cursus currere` wrap (context inherited / not) | +22 ms / +51 ms (7) | ~30 |
| content seal (`_sigillum_contenti`) | 118–144 ms (8) | ~260 |
| aedilis derivation (git share) | 39 ms (29 ms) | ~611 |
| aedilis `--differentia`, warm memo | 6.6 ms | ~450 |
| `struere.sh` (probatio_xar, all fresh) | 59 ms | 450 + 161 |
| bin/compilator single hit (prog-name share) | 24 ms (18 ms) (12) | ~650 |
| `/usr/bin` trampoline over direct | +5–8 ms | ~7 500 |
| silva wrapper (examen) original vs patched | 217–237 vs 86–107 ms (22) | ~700–950 |
| `source tools/vexilla.sh` | ~16 ms (12) | ~1 300 |
| radix test link: all 419 objects vs closure | 289 vs 61 ms (13) | 450 (frigida) |
| fabrica IUDICIUM regeneration (speculum_hospes / corpus_silicis) | ~5 s / ~7.5 s | 7× / 4× per fusio |

## Checked, not worth it

- **credo startup and teardown.**
  - `CREDO_SECTIO=nulla` refuses after full init in 5.0 ms vs 2.5 ms for an empty main. probatio_xar has only libSystem.
  - Per-section bookkeeping (two `memoria_processus_legere`, area lstat/delete, `opendir` of the suite area, 256 KB piscina) costs 37 µs per section.
  - At the ~650 test processes of a frigida plus warm reruns, that is < 3 s per merge.
- **Framework load in test binaries:** +3–5 ms per process (`-framework Cocoa/WebKit/…`); only the frigida radix path links them. Folded into J5.
- **`bin/mensor cursus currere`:** +22 ms per wrap, and its 20 ms poll loop adds ~10 ms average exit latency. Only ~8 scripts source `tools/cursus.sh` (toml, css, silva, html, saltuarius, materia, md, lexema_mensura, plus the hook), ~30 wraps per merge ≈ 1 s. When `MENSOR_COMMISSUM` is NOT inherited it computes git context (+30 ms); silva.py always passes it.
- **silva.py `_cursus` mensor processes** (incipere + finire per gate/phase): ~4 ms each, ~120 per merge.
- **aedilis `--differentia`** with a warm memo: 6.6 ms; the oracle memo (E6) already did the work.
- **`git check-ignore` per deleted path in commissio:** only for deletions; negligible.
- **fabrica read queries** (`quid` 67 ms, `legentes` 6.8 s, `census` 3.8 s) are not on the pipeline path. The pipeline's fabrica cost is the judgments (J1), not process startup.

## Open questions (incl. rulings)

1. **(J1)** May a fabrica judgment reuse a regeneration done by an earlier fabrica process of the same commissio / fusio, through a run-scoped memo directory deleted at the end (tree frozen, key includes declared inputs and tool digests)? Should it be limited to fusio (clean committed tree) at first?
2. **(J3)** May the pipeline bypass the xcrun trampolines (symlink dir first on PATH + exported `SDKROOT`)? Is dropping xcrun's implicit `-I/usr/local/include` acceptable? Please ask for ios-struere and android-struere runs as plants.
3. **(J5b)** Must frigida link every radix test against ALL objects, or is "closure (or archive) per test + ONE all-objects link per run" enough?
4. **(J8c)** If the clang identity is memoized across processes, what counts as "compiler changed": `xcode-select` link, `DEVELOPER_DIR`, `/usr/bin/clang` stat?
5. **Counts to confirm before landing J2/J3:** log-line copies of `silva/*.sh` and a PATH shim for git/clang during one real fusio would turn my ~350 + 400–600 wrapper calls and ~3 000 git / ~4 500 clang execs into exact numbers. I could not run the pipeline.
6. **(E5 interplay)** With J2(d) the git hook's remaining cost per merge commit drops to ~binary time + `formator -delta`, so E5's value shrinks further (E already noted ~0.5 → ~0.25 min).
