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

