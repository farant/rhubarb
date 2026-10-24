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
