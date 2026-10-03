# thesaurus worklog

## 2026-10-02 - birth (fabrica plan 2 T3)

Content-addressed store, filum + sigillum only (no sqlite), so any
house tool can use it. Layout under the root: `blobi/<2>/<62>` (bytes,
named by their SHA-256), `actiones/<2>/<62>` (key -> one output digest
per line), `generationes/<THESAURUS_GENERATIO>.lst` (keys a run used).

Decisions:
- Atomic writes: temp name in the same directory (`.temporarium.<pid>.<n>`)
  then rename. A present blob is never rewritten (name = content); an
  identical action entry is never rewritten. Tests check inode identity
  (rename = new inode), not mtime.
- Read verification by sample: `thesaurus_verificationem_ponere` (0 none,
  I every read, default XVI). A failed verification deletes the blob and
  reports absent - the caller recomputes. Counter per handle, not by
  digest byte (a digest rule would never check some blobs).
- GENERATION = the orchestrating run, not the process. First version
  wrote one list per process; a -plenus runs ~200 aedilis processes, so
  "keep 5 generations" would have kept 5 processes' worth. Now the run
  names itself (THESAURUS_GENERATIO, UTC-timestamp prefix so name order
  = time order): bin/fabrica sets it once per invocation unless an outer
  run did, tools/aedilis_porta.sh sets it per gate run. No variable ->
  no list (a standalone process can't push real runs out of the window;
  its keys just aren't protected). The variable is read with raw getenv
  on purpose: the store is a cache keyed by inputs already traced, so it
  is not an input (lib/thesaurus.c exempt in tools/lectiones_lint.sh;
  fabrica's judge drops blobi/ actiones/ generationes/ from traces).
- Purge is not safe against a concurrent writer (could delete a blob
  between put and action entry) - it only ever costs a cache miss,
  never a wrong result. bin/fabrica purgare takes the fabrica lock.

Concurrency test (two forked writers, 200 rounds of a fresh 64 KiB
blob, each put immediately read back with full verification) found a
real bug in filum's mkdir -p (see lib/filum.worklog.md), not in the
store. Plant (write in place instead of temp+rename): torn reads, red
3/3, only that section. First two plant attempts were invalid (read of
an uninitialized buffer; one didn't compile under -Werror) - a static
flag made a clean one.

Live: first purge of the real store (726 per-process lists from the
measurement runs, pre-fix) -> 2269 files deleted, 5 lists kept, the 488
shared .o files in the same root untouched.
