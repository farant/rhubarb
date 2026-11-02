# compilator worklog

## 2026-10-02 - birth (fabrica plan 2 T4 part I)

`bin/compilator` = drop-in `clang -c` through the thesaurus (store).
Same arguments as clang; runners change one word per compile line.

Keys:
- HEAD = sha(cwd || compiler identity || args minus the -o value ||
  source path || source bytes || include-root listings). Under it: the
  last depfile header list (a blob).
- FULL = sha(head || per header: path || sha(bytes)). Under it: the
  object (a blob).
- cwd is in the key (A5): -g embeds the working directory (spec XII.2).
- Include roots (each -I, then the source's own directory): sorted
  names of .h entries only (a new .c in lib/ must not invalidate every
  lib object). A same-named header earlier on the path shadows; the
  depfile cannot see that, the listing can (fumus V).
- Compiler identity: /usr/bin/clang is a 119 KB trampoline; the real
  binary (clang -print-prog-name=clang, 257 MB in Xcode) is hashed once
  (1.5 s) and memoized in the store under sha(path, size, mtime, inode)
  - a hit costs 0.02 s including the identity call. FABRICA_CLANG
  (test wrappers): the wrapper's own bytes.

Decisions:
- Hit COPIES the object (temp + rename), never hard-links: a shared
  inode would let any later in-place writer of the .o (the old runner
  during T5's oracle period, a plain `clang -o same.o`) corrupt the
  stored blob silently. An identical target is not touched (inode
  unchanged, fumus VIII).
- Not cacheable (no -c / -o, several sources, -E/-S/-M*) -> becomes
  clang (processus_transformare), so it is always safe to substitute.
- Failed compile: clang's exit code and stderr pass through, nothing
  stored (fumus VII).
- Known window: the full key is computed from header bytes read AFTER
  the compile; a header edited during the compile could be stored under
  the wrong key. Not handled (house compiles run with a frozen tree).

Evidence: fumus VIII/VIII (I byte-identical to plain clang on
lib/chorda.c; II hit; III touched-only hit; IV header edit miss,
restored hit; V shadowing miss then original bytes; VI other compiler
miss then hit; VII failed compile; VIII no rewrite). Plants red at the
predicted stage: no include-root listing -> V; constant compiler
identity -> VI; header bytes dropped from the full key -> IV.
All 194 lib/*.c: objects byte-identical to plain clang, cold and warm;
plain 21 s, cold 26 s (+24%: -MD, hashing, identity), warm 5 s.

## 2026-10-06 - fabrica plan 5 T2: shadowing by NAME, not by directory

- Reading (plan 5 T2): the five D rows in porta_toml's trace (include,
  lib, materia/fontes, toml/fontes, toml/probationes) came from HERE, not
  from aedilis (`bin/aedilis <test.c> --enumerare` emits none - probed
  with FABRICA_LECTIONES; aedilis's own listings, `--nexus-purus` and
  `--corpus`, do not run in the toml gate). `_radicem_addere` sealed the
  sorted `.h` names of every -I root and of the source's directory into
  the head key, for shadowing (Review Focus 2), and `directorium_iterator`
  noted each root as D - so any new header anywhere voided every verdict
  whose closure compiled through bin/compilator (the include/ voider of
  plan 5 §0).
- Branch (c) of the plan: membership only. A compile depends on the
  headers it USES and on no same-named file appearing in another root.
  The head key now keeps only the roots' paths (order still matters);
  the full key adds, per header in the depfile index, one existence bit
  for the same relative name in EVERY other root (`filum_existit`, which
  notes A/X precisely). Every other root, not only earlier ones: a
  header living in a later root searches its own directory first.
  Key version "compilator II" (old store entries never match).
- Result: toml trace D 5 -> 0; an unrelated header in include/ leaves
  the verdict RECENS; toml/probationes/latina.h (shadows include/
  latina.h for the tests) -> `STALUM ... lectio transitus mutata:
  toml/probationes/latina.h`. Fumus IX (no D, A for the earlier root)
  and X (unrelated header = hit); plant (probes off) -> V and IX red.
- Before: a shadowing header changed the head key (listing) -> miss.
  Now it changes the FULL key (existence bit) -> miss; same outcome, but
  an unrelated header no longer costs a recompile either.


## 2026-10-23 — `--plures` batch mode + opt-in per-process header memo

**Why.** The radix gate's `--residua-prae` compiled the 352 library objects through `xargs -P FILA -L 1 bin/compilator`, one process per call at ~22 ms each (startup, identity, thesaurus open): 1.8–2.2 s wall, ~7.7 s CPU, all of it cache hits.

**`bin/compilator --plures <lista>`:**
- Each line of the list is one full argument line, split on whitespace, as compile_tests.sh writes `bibliothecae.vocationes`. One `Compilator` (thesaurus + clang identity) serves every line.
- Uncacheable lines run clang as a CHILD (`processus_exsequi`), not via `processus_transformare`.
- Every line runs even after a failure (as xargs did); exit 1 if any failed.
- The tool arena is NOT reset per line: the thesaurus shares it and may allocate into it during a call.

**Memo.** Sampling the batch: `_clavem_plenam` re-read and re-sealed every header in the depfile index for every compile (`filum_legere_totum` + `sigillum_computare`), and `_obumbrationem_addere` probed every other root with `filum_existit` (fopen/fclose) for every header. The same few dozen shared headers, 352 times.

`compilator_memoriam_incipere(Compilator*)` (compilator.h) turns on two per-process tables in their own arena: header path → Sigillum (or absent), and candidate path → present/absent. Only `--plures` calls it.

It is unsafe wherever a per-call read trace matters: a memoized lookup does not note L/A/X in `FABRICA_LECTIONES`, and fabrica compiles the members of a composite in ONE process with a read log per member. That is why it is opt-in. Output objects are never memoized (`_collocare` still seals the destination directly).

**Measured** (352 lines, all hits, warm):

| mode | time |
|---|---|
| xargs -P4 | 1.76–2.18 s |
| `--plures` without memo | 1.14 s |
| `--plures` with memo | 0.28–0.30 s, single-threaded |

All 382 objects untouched (same inode and mtime). Warm radix `--residua-prae`: 4.2 → 2.5 s.

**Fumus XII:** four lines in one process (hit, broken, hit, and a fourth that re-reads line 1's headers through the memo) → exit 1, the broken source named, objects byte-identical to single calls, clang invoked once.

Plants, done by hand:
- a corrupted memoized sigillum → XII red (clang 10 → 12);
- an inverted existence memo → XII red;
- restored → green.

**Lesson: `silva.planta(..., 'compilator')` is MUTE.** The fumus runs the existing `bin/compilator` without rebuilding it, and `bin/fabrica sanare bin/compilator` refuses while fabrica itself (which links lib/compilator.c) is stale ("iudex ipse non recens"). Plant by hand: edit → `./tools/fabrica_struere.sh` → `bin/fabrica sanare bin/compilator` → fumus → restore → rebuild both.

## 2026-10-23 — compilator_compilare_in (fabrica memory, park …T77EH4)

Added `compilator_compilare_in(compilator, argv, n, piscina)`: every per-call allocation (file
reads for the key and cache hit, argv parsing, captured output, error text) goes into the
caller's piscina, so the caller can empty it after the call. `compilator_compilare` is now a
wrapper passing `compilator->piscina` (unchanged behaviour, used by --plures). The thesaurus
allocates in its own piscina only on a store (miss) and in GC. Test:
probationes/probatio_compilator.c (needs lib/compilator.c in compile_tests_fontes_generata.sh,
regenerated). argv is WITHOUT a leading 'clang', or it counts as a second source and the call
is "non cacheabilia".

## 2026-10-24 — compilator_compilare_sine_collocatione

A hit returns the thesaurus blob path in `CompilatorResultus.via` and neither reads nor writes
the `-o` output; a miss compiles into the output as before (via = output). `via` is now also
set by compilare/compilare_in (the output path). fabrica uses it to link members directly from
the cache; bin/compilator (--plures, compile_tests) keeps copying. Test: probatio_compilator
section 'sine collocatione'.

## 2026-10-28 — big vendored sources shared across suites (budget B1 E7)

briar, gesta and ludus_tessera each compiled `vendor/sqlite3.c` (9.5 MB,
~10 s idle) with plain clang into their own build dir; briar and officina
each compiled `silva/amalgama/silva.c` (~5 MB). In a frigida run (fresh
clone, everything cold) that is 3 redundant sqlite compiles + 1 redundant
silva.c on top of radix's. They now call `_vendicatum_compilare`: run
`bin/compilator` FROM THE RADIX with radix's exact argument order and the
relative source path (`-c vendor/sqlite3.c`), so the key (cwd + args minus
-o + source and header bytes) matches compile_tests.sh's line and the
object comes out of the thesaurus. The suites' mtime fast path stays in
front (warm runs never call it). Measured: hit 0.08 s vs ~10 s; all three
sqlite objects byte-identical to radix's build/sqlite3.o; briar's and
officina's silva.c objects identical (one compile served both). Key
lesson: the key includes the source PATH SPELLING and cwd — the old
scripts mixed `"$RADIX_DIR/vendor/sqlite3.c"` (absolute) with relative,
which would never have hit even through compilator.

## 2026-10-29 — compile_tests.sh: --plures in FILA parallel parts (budget B1 E8)

`bin/compilator --plures` (2026-10-23) made the all-hits case fast by
doing everything in one process — and, as a side effect, compiled every
MISS serially (before, `xargs -P FILA` ran one process per line). After a
header change, or in frigida's cold clone, that was ~1–2 min of serial
clang. compile_libraries now deals the vocationes lines round-robin into
FILA parts and runs one `--plures` per part in parallel (shared thesaurus,
as with xargs before); FILA = 1 keeps the single process. Measured on the
414-line radix list with an empty thesaurus and redirected -o: 70.5 s →
29.2 s cold, 0.49 → 0.29 s warm (4 P-cores), objects identical. Error
propagation tested: `#error` in lib/via.c → "FAILED: bibliothecae", rc 2;
plant ignoring the children's exit codes → rc 0 with the broken lib
silently linked from a stale object (so the `wait ... || rc_bib=1` check
is load-bearing).

## 2026-10-30 — ludus_tessera runner through the compilator (budget B1 E3, suite 1)

`ludus_tessera/compile_probationes.sh` compiled ~155 objects (root libs
+ tessera + musivum part + its own sources) with raw clang behind the
"newest header" mtime rule. Touching any header in include/ or
tessera recompiled everything (measured 139 s, 155 objects, after
`touch include/chorda.h` with no content change). Now one vocationes
file (one full clang argument line per object) goes through `bin/compilator
--plures` in FILA parts (same pattern as compile_tests.sh E8). The cache
keys on cwd + args + bytes of the source and its depfile headers, and
identical objects are not rewritten. Measured: compile phase 2.6 s
warm, whole suite 4 s after the same header touch. sqlite stays
on `_vendicatum_compilare` (E7, radix-identical args).

Also removed the trailing `excubitor.sh -tacitus ludus_tessera/build/`
call, as compile_tests.sh did when it moved to the compilator. It
judges objects by mtime, so after a header touch it falsely reported
"STALA 126" (identical objects keep their old mtime) and spent ~40 s
rebuilding its graph.

Plant: changed escape sequence in lib/rivus_terminalis.c → recompiled
via compilator, probatio_ludus_tessera_pons FRACTA (:157).

Remaining E3 suites (tessera, gesta, officina, briar, oratio): not
done in this pass.

## 2026-11-02 — budget B2 E3/J9 suite II: tessera via the compilator + link skip

tessera/compile_probationes.sh now goes through tools/cursor_communis.sh
(H5 helpers): library + tessera sources as ONE `compilator --plures`
(44 lines), each test object via `_cursor_compilare` (per-test failure
semantics kept: one broken test = that test FRACTA, others run), and the
test binary is relinked only when its link signature changes (flags +
`stat -f '%N %i %m %z'` of the test object, every library object and the
binary itself; stored in `<bin>.nexus`, as aedilis E10). The compilator
never rewrites an identical object, so inode/mtime are stable across
hits; a removed source changes the object list and therefore the
signature. Excubitor call at the end removed (mtime verdict lies once
identical objects are not rewritten), as ludus_tessera.

Remember to reset `$CLAUSURAE_DIR/mandata.tsv` when not calling
cursor_clausuras_derivare (it appends forever otherwise).

Measured: warm 4 -> 1 s; after `touch include/chorda.h` 48 s (44
recompiles) -> 1 s. Plant: inverted `tessera_stilus_aequalis` -> 1 object
changed, tests relinked, 10/15 FRACTA; revert green. First plant attempt
(dropping the `ornamenta` comparison) was an equivalent mutant: the test's
a/b styles also differ in colour.

## 2026-11-02 — E3/J9 suite III: gesta + shared tools/nexus_probationis.sh

The test-link skip moved into tools/nexus_probationis.sh
(`probationem_nectere <bin> "<flags>" <objects...>`), sourced by the
runners only — a new file rather than cursor_communis.sh so the change
does not fan out to every suite that sources the cursor. tessera now uses
it too. gesta: sqlite stays on the E7 vendored path (source-newer rule);
32 library + gesta objects through one `--plures`, tests per
`_cursor_compilare`, excubitor call removed. Measured: warm 8 -> 7 s (the
tests themselves, daemons and sqlite, dominate), header touch 54 -> 10 s.
Plant: `!` on the first `_est` of gesta_inventarium_eventus_est -> 1
object changed, 3/7 (probatio_gesta_inventarium and dependents), revert
7/7.

## 2026-11-02 — E3/J9 suite IV: officina

officina had the most hand-written mtime rules: newest-header for most
objects, per-object header lists for the instrumenta units, and the
tessera amalgam on a source-only rule (header changes never rebuilt it).
All 33 objects except the E7 vendored silva amalgam now go through one
`--plures`; a local `_vocatio_addere <src> <obj> <include flags...>`
keeps each unit's own include roots (tessera amalgam: include/ only;
silva_lexicon: silva/fontes + materia/fontes). First run rewrote exactly
1 object — consistent with the old header-blind amalgam rule. Measured:
warm 16 -> 11 s, header touch 68 -> 11 s. Plant: punctum_tollere returns
FALSUM after removing -> 1 object changed, 13/15; revert 15/15.

Gate blind spot (bit the officina commit): the gesta commit ADDED
tools/nexus_probationis.sh, whose comment named the flags array; the
vexilla gate ("every script naming it sources vexilla.sh") scans tracked
files only, and the new file was untracked while that commit's gates ran
— so it passed, and the NEXT commit touching any .sh failed on it. Same
shape as portae-declaratae needing `git add -N`. Fix here: the comment no
longer names the array. Worth a ledger note: gates that enumerate scripts
via git ls-files should include the commit's own new files.

## 2026-11-02 — E3/J9 suite V: briar

82 objects (all but the E7 vendored sqlite and silva amalgam) through one
`--plures`, including the generated build/capsula_facies_briar.c (its old
rule was source-only); silva_lexicon keeps its private -Isilva/fontes.
The old "custodia vetustatis / custodia capitum" blocks (same-second mtime
equality, newest header) are gone — the content-keyed compilator makes
them moot. `--obiecta` mode unchanged (objects at the same paths). Tests
per `_cursor_compilare` + `probationem_nectere`; mensor_suitae timings
kept around them. Measured: warm 12 -> 8 s; after a header touch the
object phase is 6 s with 0 rewrites (old: 82 recompiles).

FINDING (not fixed, separate item): the pre-step `corpus_symbola_generare`
(tools/corpus_infixum.sh, "[symbola] corpus.symbola.tsv (nexus
incrementalis)") took 83 s ALONE after `touch include/chorda.h` (load ~9).
It dominates briar's header-touch time now, old and new alike.

Plant: `_digiti` accepts only 0-8 -> 1 object changed, 19/20
(probatio_briar_dialectus); revert 20/20.

## 2026-11-02 — E3/J9 suite VII: oratio (E3 complete)

76 objects through one `--plures`. The post-build excubitor check was a
real SAFETY gate here (stale -> exit 2, "custodia capitum caput
ignoravit"), guarding the hand-written header index. With content-keyed
compilation there is no header index left to guard, and keeping it would
turn every header touch into a false STALA -> exit 2 (identical objects
are not rewritten), so it is removed with that reasoning in the comment.
Measured: no visible wall-time change — warm 66 -> 66 s (the tests are
~60 s), header touch 112 -> 118 s with 0 recompiles (old: 76). The touch
cost lives elsewhere:

FINDING (not fixed, separate item): `silva/nexus.sh -renovare`, run by
this runner before the tests, took 51 s after `touch include/chorda.h`
(2 s on a second run). With briar's corpus_symbola pre-step (83 s) these
are the two remaining header-touch hogs in the suites.

Plant: initials accepted only A-I -> 1 object changed, 16/19
(probatio_oratio_arbor "J. Smith came."); revert 19/19.

E3/J9 summary (all six raw-clang runners now on the compilator):
ludus_tessera 765f1f2e, tessera 1b7f5edc, gesta f3a54629, officina
85629289, briar c6ab2020, oratio (this commit).
