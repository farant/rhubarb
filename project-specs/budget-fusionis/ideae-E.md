> Raw analyst report, batch E of budget B1 (2026-10-27/28), filed verbatim as evidence for ../budget-fusionis.md. Paths under `scratchpad/b1/` were session scratch (profiles, prototypes) and no longer exist.

# Batch E — hooks, shell-pipeline gates, frigida cold builds

Analyst: Opus 5.5 subagent, 2026-10-27. Nothing in the repo was changed. Probes and replicas are in `scratchpad/b1/E/`.
Machine load was about 11 on 8 cores while I measured (other agents were profiling at the same time), so my absolute timings are pessimistic. Use the ratios.

## 0. First, a correction to the baseline

The frigida numbers in the briefing (oratio 8.5 min, briar 4.5, frigida total 36.5 min) come from an **unusually slow run**. In that run, `frigida` overlapped a `commissio` running in rhubarb-secunda: 77 secunda spans fall inside the frigida window in C.pkl.

`mensor quaestio statistica praefixum=frigida:` gives these medians over 23–24 runs: radix 6.5, silva 4.7, briar 3.4, **oratio 1.9**, saltuarius 1.2, crusta 0.9, md 0.8, officina 0.6, toml 0.6, ludus_tessera 0.6, and the rest under 0.3 min. `phasis:frigida` is typically **18–23 min**, not 36.

Oratio cold is *not* compile-bound. Its frigida median (1.9 min) is lower than its warm gate median (porta:oratio 2.3 min). The time goes to the tests themselves; vocabula alone takes 5.7 s.

**Consequence:** a frigida saving should be quoted against a ~21 min total. Some of the briefing's "cold vs warm" gap is machine contention, not compilation.

---

## Ranked ideas (estimated minutes saved per merge)

### 1. Make gate receipts content-keyed so they survive the merge commit → fusio_probare stops re-running 18 gates that are already green — **~4.1 min**

- **Where:** `pythonica/silva.py`. `sigillum_arboris()` (≈ the definition just after `recepta_viva_delere` area; search `def sigillum_arboris`) hashes `HEAD` + `git diff HEAD` + untracked files. `recepta_viva_delere()` (l.4860) is called after every commit (l.~3913). `fusio_probare(recepta=True)` (l.4043) would reuse receipts, but by then none survive.
- **Evidence:** in the measured merge, these gates passed inside `commissio` and then ran again in `fusio_probare` on the **same content**:
  - vexilla 4.0, unitates 4.8, portae-declaratae 6.9, interrogationes 15.3, **generata 52.0**, css 1.3, materia 0.8, html 1.0, sonda 2.1, toml 2.5, saltuarius 3.3, sera 8.2, mensor-cursus 10.1, **briar 23.1**, **silex-semen 29.3**, md 1.1, **ludus_tessera 58.1**, radix 22.4.
  - Total = **246 s**. The same list comes from the per-merge root listing in C.pkl.
- **Mechanism:** the only thing that changes at commit time is `HEAD`. The content is unchanged: (old HEAD + diff + untracked) == (new HEAD + ∅ + untracked). Change the seal to a **tree hash of the working state**:
  - temp `GIT_INDEX_FILE`, `git add -A` minus VETITAE, `git write-tree`;
  - this is the same idea the photographia shadow already uses ("Receptum sigillum = arbor photographiae");
  - stop deleting still-valid receipts after commit.
- **Estimate:** 246 s measured, so ~4 min per merge. It also helps any commit followed by a re-run of the same gates.
- **What it weakens:** gates whose verdict depends on commit metadata rather than file content, such as provenientia stamping a commit hash, `git log` readers, `formator -delta HEAD`, or the unci fumus. Those need `HEAD` in their key. Mark them in the `<porta>` annotation (e.g. `legit="HEAD"`) and keep the old seal for them. Gates that only read files lose nothing: same bytes, same verdict, except for flakiness, which a re-run is a poor way to detect anyway.
- **Maintainability risk:** low–medium. One function changes, plus a per-gate opt-out. The opt-out list has to be audited once.
- **Effort:** M.
- **Verify:**
  - In a test worktree, `commissio` then `fusio_probare` should print "non iterum cursa / receptum vivum" for those 18 gates.
  - **Plant:** after the commit, edit one byte in a file a gate covers (`tegit`). That gate must re-run.
  - **Plant 2:** a `legit="HEAD"` gate must re-run after the commit even with no file change.
- **Policy question for Fran:** is the fusio-tier re-run of commit-tier gates *meant* as a flake detector or second proof? If so, this idea is "re-run only the flaky ones".
- **Note:** this may overlap with whichever batch owns fusio orchestration.

### 2. Do the header freshness scan once per call instead of once per object in the silva wrappers — **~3 min in the merge commit, plus ~0.5–1 min in other gates**

- **Where:** in `silva/examen.sh` and `silva/formator.sh` (and 37 more scripts), `newest_header()` is `find include silva/fontes materia/fontes [silva/instrumenta] -name '*.h' -newer "$obj" | head -1`. It runs **once per object**: examen has 18 lib objects + 26 `silva/fontes` objects = 44 finds per invocation; formator has 45.
  - Scripts with the per-object find: all `silva/*.sh` tools (examen, formator, censor, nexus, selecta, renominare, transformare, …), `officina/{amalgamare,colloquium,cursor,fusor,interpretare,legatus,vindex}.sh`, `tessera/amalgamare.sh`, `gesta/{fori,tabularium,tabulariumd}.sh`, `materia/coquere.sh`, `saltuarius/saltuarius.sh`, `tools/{aedilis,canon,natura}_struere.sh`.
  - The test runners (oratio, briar, gesta, tessera, officina, ludus_tessera) already use the "CAPUT RECENTISSIMUM SEMEL" pattern ("exemplar radicis 2026-08-25"): find the newest header once, then compare each object with `! [ obj -nt CAPUT_RECENS ]`.
- **Evidence:**
  - A read-only copy of examen.sh's freshness checks (`E/prologus.sh`, 0 stale objects) takes **0.63–0.78 s**. One `find` takes 16 ms, × 44 ≈ 0.7 s. The examen *binary* itself averages **0.22 s per file** (54 merge files = 11.7 s); formator averages 0.12 s.
  - Phase timings of the merge commit:
    - `phasis:unci`: 59 s for 54 examen calls, so ≈0.8 s of wrapper per call.
    - `phasis:forma`: 40 s for ≤76 formator `-scribere` calls.
    - `phasis:git commit`: the hook, 151 s, repeats lint, examen ×54 and formare ×76, then adds `formator -delta` ×76 (two binary runs and one wrapper each).
  - Wrapper calls per merge commit ≈ 76 + 54 + 54 + 76 + 76 ≈ **330**. At ~0.55 s of wasted scanning each, that is **≈3 min**.
- **Mechanism:** a shared helper, e.g. `tools/caput_recens.sh` (one find, newest header, sourced by every wrapper), replacing the per-object find. Two semantics points:
  - It is equivalent ("some header newer than obj ⟺ newest header newer than obj").
  - It is *safer* on ties: `! obj -nt` recompiles on equal mtimes, whereas `find -newer` is strict.

  A further option is for the hook to check freshness once (`INSTRUMENTUM_NECTERE_SOLUM=1 ./silva/examen.sh` already means "build only, exit") and then call `silva/build/examen` directly per file.
- **Estimate:**
  - Merge commit: 330 × ~0.55 s ≈ **3.0 min**.
  - Also helps porta:unci (21 hook runs, 170 s), porta:nexus-instrumentorum (≈400 wrapper calls in `tools/instrumenta_nexus_fumus.sh`, 78 s), and every ordinary commit (cost scales with the number of files).
  - Conservatively **3–4 min per merge**.
- **What it weakens:** nothing, provided the directory list is kept per script. The mtime basis and tie handling are unchanged or stricter.
- **Risk:** low. It is mechanical across 39 scripts (silva.Editio, or the transformare rule machinery). One concern: concurrent-relink correctness, which porta:nexus-instrumentorum is built to test.
- **Effort:** S–M.
- **Verify:**
  - porta:nexus-instrumentorum must stay all ok, especially I "fresh → no relink", II "source touched → relink", IV "8 concurrent stale", and V "silva_lexicon.h touched → relink".
  - **Plant:** `touch include/latina.h` → `./silva/examen.sh x.c` must print `[dep]` recompiles. A plant that drops one directory from the helper should turn V red.

### 3. Raw-clang suites: any header touch rebuilds every object, and nothing is shared across suites or kept between frigida runs — move them onto `bin/compilator --plures` — **~2–4 min (warm gates + frigida)**

- **Where:** these suites compile with plain `clang -c` under "newest header anywhere newer than obj → recompile":
  - briar, oratio, ludus_tessera, tessera, gesta, officina (and md, toml, html, css, materia, saltuarius, silva);
  - see e.g. `oratio/compile_probationes.sh` lines ~120–215 and `briar/compile_probationes.sh` lines ~140–270.
  - Only radix (`compile_tests.sh` compile_libraries, l.181–229), crusta, and the `cursor_communis.sh` instruments already go through the content-keyed thesaurus.
- **Evidence:**
  - Latest logs in `build/test_logs/` show the number of objects recompiled by a *warm* gate after a header-touching commit or merge: ludus_tessera **152/155**, briar **81/84**, tessera 44, gesta 32/33, officina 32.
  - The merge touched 22 headers, so these gates were effectively cold. ludus_tessera took 138 s in commissio and 58 s again in fusio; tessera 72 s, gesta 66 s, officina 71 s.
  - Across the 14 subsystem build dirs: **471 compiles of lib objects for 172 distinct libs**. chorda, piscina, xar, credo, friatio, tabula_dispersa, lectiones and chorda_aedificator are each compiled 14×.
  - Typical lib compile is 0.1–0.4 s (measured under load: mean 0.40 s, stml.c 1.46 s).
- **Mechanism:** each suite writes a `vocationes` list and calls `$COMPILATOR --plures`, exactly as `compile_libraries` does.
  - Warm benefit: only translation units whose actual depfile headers changed get recompiled.
  - Frigida benefit inside one run: identical (cwd, args, source) triples hit, for example the same sqlite3.o, chorda.o, and so on. This only happens where flags and `-I` lists are identical. They differ per suite today, and the key includes the arguments.
- **Estimate:**
  - Warm: perhaps half of the ~6–7 min of suite compile work in a merge is header-coarse rebuilding, so **~1.5–3 min**.
  - Frigida within one run: ~300 redundant lib compiles × ~0.15 s unloaded ≈ **0.75 min** (more if `-I` lists are unified).
- **What it weakens:** nothing. The key is stricter than mtimes: the compilator worklog records that an absent header is a miss, and that shadowing headers are keyed by listing.
- **Risk:** medium. It is mechanical but touches 13 runners, and their per-suite `-I` lists and odd objects (vendor, amalgam) need care.
- **Effort:** L (but repetitive; it can be done one suite at a time).
- **Verify:** per suite, "Bibliothecae: N per bin/compilator, M mutata" lines. **Plant:** change a header that only one lib includes → only that object (plus its includers) is rebuilt. A plant in the thesaurus key (the compilator fumus) must stay red.

### 4. Frigida: a persistent thesaurus for the compiles that already go through the compilator — **~1.5–2.5 min per frigida (more once #3 lands)**

- **Where:**
  - `tools/frigida_probare.sh` makes `AREA=$(mktemp -d …)` and `git worktree add`.
  - `cursor_communis.sh` l.49: `THESAURUS="${FABRICA_THESAURUS:-$RADIX_DIR/build/aedilis/obiecta}"`. Inside the fresh tree this is an empty, run-local store.
- **Evidence:**
  - Radix cold compiles ~352 lib objects plus vendor (sqlite3.c alone took 27 s at `-O2` under load), then the 440 test objects. `porta:radix` warm is 1.5 min; frigida:radix is 6.5 min.
  - The compilator key includes **cwd**, because `-g` embeds the working directory. Since the frigida path is random per run, a shared store would never hit today.
- **Mechanism:** either:
  - (a) use a fixed frigida path per source worktree (e.g. `$TMPDIR/frigida-<worktree>/arbor`, guarded by `tools/sera.sh`) and export `FABRICA_THESAURUS=$RADIX/build/frigida_thesaurus` (gitignored, persistent); or
  - (b) add `-fdebug-compilation-dir=.` to `VEXILLA_C89` and drop cwd from the key. That would let all worktrees share one store.
- **Estimate:** radix lib phase (~1–1.5 min, serial, see #8) + radix test objects (~0.5 min at FILA=4) + oratio/silva instrument objects ≈ **1.5–2.5 min**. After #3 is done, almost all compile time in frigida (~5–7 min) becomes cacheable.
- **What it weakens — the key question:** frigida's purpose (…QY4) is "the suite doesn't read gitignored or uncommitted input". A content-keyed cache **keeps** that guarantee:
  - Every key covers the source and header bytes *read from the fresh tree*.
  - A header missing from the clone is a miss (`_caput_sigillare` FALSUM → recompile → clang fails), exactly as today.

  What frigida stops proving is "clang compiles these bytes again today". Residual holes:
  - (1) SDK or system-header changes, if the depfile is `-MMD`-style (to check);
  - (2) environment variables clang reads (SDKROOT, CPATH, MACOSX_DEPLOYMENT_TARGET);
  - (3) a corrupted blob in the store.

  Mitigation: a monthly or `FRIGIDA_GELIDA=1` truly-cold run, and/or a full truly-cold run whenever `clang --version` or the SDK path changes.
- **Effort:** S for (a), M for (b).
- **Verify:** a second frigida on the same SHA should show radix "Bibliothecae: N … 0 mutata"-style hits. **Plant:** a commit with a `.c` file that includes an untracked header present only in the dev tree. Frigida must still go red.
- **Policy question for Fran:** is "a fresh clone builds and passes" satisfied by "fresh clone **content** + trusted object cache", or must frigida exercise the compiler?

### 5. Hook after `silva.commissio`: skip lint, examen, diagnostica and formare when the praevius already judged the same blobs — **~1.6 min alone; ~0.5 min if #2 lands first**

- **Where:**
  - `tools/unci-git/pre-commit` (whole file). `silva.py` `commissio()` l.3843–3910 runs `phasis:forma` (formare_viae), `phasis:lint` (lint_latinus), `phasis:unci` (the same hook with `UNCUS_VIAE` + `UNCUS_CRIBRUM=1`: diagnostica + examen), then `git commit`, which runs the hook *again in full*: lint, diagnostica, examen, `formare_viae -index`, and `formator -delta`.
  - The hook comment itself says "plerumque nihil mutatur (plagulae iam formatae)".
- **Evidence:** this confirms ledger idea B5, with numbers. The merge's hook took 151 s, versus 59 s for the praevius copy. Everything except `formator -delta` (~50–60 s) is a repeat of work done minutes earlier on the same bytes, because `_portas_currere` refuses commits whose viae changed during the gates.
- **Mechanism:** after `git add`, `commissio` exports a token, e.g. `UNCUS_PROBATUM=<sha256 of "path blob-sha" lines for the viae, as judged in phasis:unci>`. The hook recomputes it from `git diff --cached --name-only` + `git ls-files -s`. If it matches, skip to `formator -delta`; otherwise run everything as today. A bare `git commit` and `git merge` are unaffected.
- **Estimate:** without #2, it removes ≈ 4 + 3 + 54×0.95 + 76×0.6 ≈ **95 s**. With #2, ≈ 4 + 12 + 6 + overhead ≈ **25–30 s**.
- **What it weakens:** nothing, if the token binds to the index blobs (a mismatch means the full hook runs). The tool binaries could only change by being rebuilt from the same sources.
- **Effort:** S–M.
- **Verify / plant:**
  - (1) After the praevius, alter a staged blob (e.g. `git add` a modified file) before the commit. The hook must run the full examen and block a planted REICE.
  - (2) A forged token value must cause a full run.
  - `tools/unci-git/fumus.sh` needs a new rung for this.

### 6. aedilis: the `clang -MM` oracle for `.m` files runs once per closure because the derivation calls don't pass `--memoria-oraculi` — **~0.8 min**

- **Where:**
  - `tools/aedilis_porta.sh`: the first `bin/aedilis "$f" --thesaurus …` call has no memo. Only the `--differentia` call passes `--memoria-oraculi "$MEMORIA_ORACULI"`.
  - `tools/instrumenta_fumus.sh`: the `bin/aedilis "$f" --exitus …` call has no memo at all.
  - `.m` extractions are never stored in the thesaurus by design (`lib/aedilis_silva.c` ~l.840: "Fontes .m … numquam conduntur").
- **Evidence:**
  - `sample` of `bin/aedilis apps/mensor/mensor_ui.c --enumerare --thesaurus …` (1.65 s): **417/504 samples in `system()` → `_extractor_oraculi`**.
  - `probatio_admissio` (2 `.m` files): 0.24–0.29 s without the memo, **0.02 s** with a warm memo; output byte-identical.
  - porta_aedilis generates `.m` compiles 190 times across 89 tests, for only **8 distinct** `.m` files.
  - porta:instrumenta has 64 `.m` occurrences (tls_securetransport_macos and fiducia_macos 17× each, fenestra_macos 12×). Cocoa `-MM` takes ~0.5 s each.
- **Mechanism:** pass the run-local memo (mktemp + trap, same lifetime rules as the existing one) to the derivation calls in both gates.
- **Estimate:** porta_aedilis ≈ 89 × 0.23 ≈ 20 s; porta:instrumenta ≈ 56 repeats × ~0.4 ≈ 22 s. Total **≈0.7–0.8 min**.
- **What it weakens:** nothing. It uses the same memo the gate already trusts for `--differentia`, with the tree frozen during the gate.
- **Effort:** S (one argument in each file).
- **Verify:** both gates are unchanged in verdict and output. **Plant:** the worklog's existing plant (a wrong memo entry for lib/xar.c → "ORACULUM SOLUM") should still surface.

### 7. Frigida: sqlite3.c and the silva amalgam are compiled over and over — **~0.5–1 min per frigida** (folds into #3/#4)

- **Where:** `vendor/sqlite3.c` (9.5 MB) is compiled at `-O2` with *identical* `VENDOR_FLAGS` by:
  - compile_tests.sh (radix), briar (l.~225), gesta (l.108), ludus_tessera (l.283);
  - also the generated struere.sh files of porta:instrumenta and porta_aedilis into their own `obiecta/`.

  `silva/amalgama/silva.c` (4.9 MB) is compiled by briar (l.244) and officina (l.136).
- **Evidence:** measured under load: sqlite3.c **27 s**, silva.c **29 s** per compile. The gesta frigida median (10.7 s total, including sqlite3) suggests ~8–10 s on an idle P-core. So expect ~10 s each.
- **Mechanism:** route these compiles through `bin/compilator`, using one normalized source path (the scripts mix `"$RADIX_DIR/vendor/sqlite3.c"`, which is absolute, with `vendor/sqlite3.c`, which is relative, so the keys differ).
- **Estimate:** 3 redundant sqlite × 10 s + 1 redundant silva.c × 11 s ≈ **40 s per frigida**. More if #4 lands, since then the first compile is gone too.
- **Weakens:** nothing. **Effort:** S per call site.

### 8. `bin/compilator --plures` handles misses serially (a cold-path regression introduced 2026-10-23) — **~0.75 min per frigida, plus warm radix after header churn**

- **Where:** `tools/compilator.c` `_plures` (l.107–185) loops over the lines in order. Before c3b2abcb, compile_tests.sh used `xargs -P FILA` (FILA = 4 P-cores here).
- **Evidence:** the hit path went from 1.8 s to 0.3 s, which is what the change was for. On a cold or header-churned tree, though, ~352 misses at ~0.15–0.4 s now run one after another, ≈1–2 min. The merge's own commissio porta:radix took 398 s after touching 22 headers. frigida:radix history is too noisy to confirm the regression (276–582 s).
- **Mechanism:** inside `--plures`, compute hits first, then fork up to FILA children for the misses, each with its own Compilator. Or keep the processes as they are and split the list into FILA chunks.
- **Estimate:** ~0.75 min per frigida; several tens of seconds on any radix gate after a header-touching commit.
- **Note:** this is intra-suite parallelism, not B7/B8. Mentioned because it is a specific regression.
- **Weakens:** nothing. **Effort:** S–M.
- **Verify:** compilator fumus XII; cold radix "Bibliothecae: … mutata" lines are identical.

### 9. In a merge commit, `formator -delta HEAD` compares main's files against the wrong base — **~0.3–1 min per merge**

- **Where:** pre-commit, last section (the `formator -delta HEAD` loop).
- **Evidence:** the merge put 54 `.c` and 22 `.h` files into `git diff --cached`. Each runs `formator -delta` (wrapper + 2 binary runs + `git show`, ≈0.6–0.9 s each before #2). It only warns and never blocks. For files brought in from main, "new divergences vs HEAD" are main's edits, which were already warned about when committed on main.
- **Mechanism:** when `MERGE_HEAD` exists, limit `-delta` to paths whose index blob differs from **both** parents (that is, conflict resolutions and edits made during the merge).
- **Estimate:** ~50 s per merge today, ~15 s after #2.
- **Weakens:** a warning nobody reads during an automated merge. **Effort:** S. **Policy:** Fran's call (lex basis).

### 10. porta:instrumenta: provenientia stamped with the commit forces recompile and relink of every program on every commit — **~0.3–0.5 min**

- **Where:** in the generated `build/instrumenta_fumus/*/struere.sh`, each program compiles `build/fabrica/provenientia/<prog>.c`. Its header says "commissum: 7dc70441", so it is regenerated per commit. The final `clang $VEXILLA objs… -o` always relinks; there is no up-to-date check.
- **Evidence:** the latest log for `apps__mensor__mensor_ui` shows lib/provenientia.c and build/fabrica/provenientia/mensor_ui.c recompiled. 161 programs; link ≈0.05–0.35 s each (mensor_ui, the biggest, measured 0.34 s); the gate median is 108 s.
- **Mechanism:** a link-skip when the output is newer than all objects (and the flags are unchanged), plus a constant provenientia stamp in fumus mode, since the gate proves "links", not "stamped". Or let fabrica own these as artifacts.
- **Estimate:** ~161 × (0.1 compile + 0.15 link) ≈ **40 s** saved on runs where nothing else changed.
- **Weakens:** a skipped link would no longer re-prove that linking works for unchanged inputs. Since linking is deterministic, that adds nothing. **Effort:** M (an aedilis generator change). **Plant:** delete one object → that program must relink; break a symbol in one lib → the programs that depend on it must go red.

---

## Checked, not worth it

- **`vetustum` bash loops in the generated struere.sh files** (headers × objects `-nt` tests): replica `E/vetustum_replica.sh` = 70 ms per program × 161 ≈ 11 s per run. Small.
- **Test-binary links in porta_aedilis:** 0.04–0.07 s each (probatio_xar, vicus, stml). Not the cost driver; aedilis derivation is (#6, and the per-test process startup).
- **Sleeps and polling:** no `sleep` in aedilis_porta.sh, credo_fumus.sh, instrumenta_nexus_fumus.sh, iudicium_fumus.sh, unci-git/fumus.sh, or the ludus_tessera, tessera, gesta, officina and crusta runners.
- **Oratio cold (8.5 min):** an outlier caused by contention. Median 1.9 min, and it is run-bound (warm ≥ cold).
- **ludus_tessera cold** (frigida median 35 s) is *faster* than its warm gate (40 s median, 138 s in commissio). The warm gate is the problem (#3), not frigida.
- **`iudex_unitatum.sh` calls in the hook:** ~5 per hook at a few hundred ms each. Negligible.
- **`lint_latinus` in the hook:** incremental `nexus.sh -renovare` (~1 s) + vocabula. 4.4 s per phase. Only relevant as part of #5.
- **Hook process count** (examen/formator taking one file per process): batching would need binary changes. With #2 in place, a process costs ~0.05 s plus the binary, so batching would save another ~10–15 s per merge at most.

## Open questions (incl. policy for Fran)

1. **(#1)** Is re-running commit-tier gates in `fusio_probare` deliberate, as a second proof or flake hunting? If so: keep it for a named flaky list only?
2. **(#4)** Does frigida have to exercise the compiler, or is "fresh-clone content + a content-keyed object cache, plus a periodic truly-cold run" acceptable? How often should the truly-cold run happen: weekly, every Nth merge, on a compiler or SDK change?
3. **(#4)** Should the cache be per-worktree (fixed frigida path, simplest) or one shared store across worktrees (needs `-fdebug-compilation-dir`, which changes DWARF comp_dir in every object, debugger impact to check)?
4. **(#5)** Is an env-var token from `silva.commissio` an acceptable "already proven" channel for the git hook, or must the hook always judge independently (the "porta quae praeteriri potest" principle)? The token binds to index blobs, so a bare `git commit` is unaffected.
5. **(#9)** For merges, should `formator -delta` warn only on blobs that differ from both parents?
6. **(#3)** When migrating suites to the compilator, should `-I` lists be unified to `vexilla_radices` (which would make cross-suite sharing possible), or kept per suite (safer about header shadowing, e.g. duplicate `latina.h` in fixtures)?
7. Does the depfile the compilator records include system and SDK headers (`-MD`) or not (`-MMD`)? This decides hole (1) in #4.
8. Measure under no load before triaging: my absolute timings were taken at load ~11, so the per-call numbers (0.6–0.8 s wrapper overhead) may be ~1.5–2.5× lower on an idle machine. The *counts* (≈330 wrapper calls, 18 doubled gates, 190 oracle calls for 8 files) are exact.
