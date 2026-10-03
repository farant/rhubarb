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
