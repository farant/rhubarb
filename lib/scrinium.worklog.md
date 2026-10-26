# scrinium worklog

## 2026-07-14 — born (tabularium-gradus.md phase A)

Vendored SQLite 3.53.3 (vendor/sqlite3.{c,h}, from sqlite.org
amalgamation) + this Latin wrapper + ULID. Vendor object compiled
RELAXED in its own object (compile_tests.sh VENDOR_SOURCES array —
the objc-sources pattern); flags decided at vendor time and recorded
there: SQLITE_ENABLE_FTS5 (the whole point for B'), THREADSAFE=0
(single-writer house law — also faster), DQS=0, DEFAULT_MEMSTATUS=0,
OMIT_LOAD_EXTENSION, OMIT_DEPRECATED, DEFAULT_WAL_SYNCHRONOUS=1.
~11s compile, cached as build/sqlite3.o.

**FIND #1 — Roman numerals eat vendor headers.** sqlite3.h declares
`sqlite3_str_appendchar(sqlite3_str*, int N, char C)` — and latina.h
defines C=100. Preprocessor turned it into `char 100`. SECOND
instance of the class (imago.c's stb note was the first): **vendored
headers must be included BEFORE latina.h**, always. The one-letter
Roman numerals (C, D, I, L, M, V, X) make any vendor header with
one-letter parameter names a landmine.

**FIND #2 — porter stemming is English; Latin gets nothing.** FTS5's
`porter` tokenizer will not fold Latin inflections ('parsuram' !=
'parsura' to it). The working idiom for the Latin corpus is PREFIX
queries ('parsur*'), which the unicode61 tokenizer serves fine.
Design input for tabularium B': either lean on prefix queries (cheap,
works today), or a custom FTS5 tokenizer for Latin stemming someday
(named, distant). English prose in notes stems fine.

Design notes: chorda's (datum, mensura) shape maps EXACTLY onto
sqlite3_bind_text(ptr, len) — the not-null-terminated house string
is native here (mid-string slice binding has a probatio). Column
text is COPIED into a caller piscina (stable across steps).
Statement wrappers allocated via sqlite3_malloc (one allocator world
for ephemeral objects; Scrinium itself is piscina-owned). Migration
runner is libraries.ts sqliteMigrate ported: append-only,
`_migrationes` table, BEGIN/COMMIT around the batch. ULID is
libraries.ts ulid ported: same-ms monotonicity via static state,
randomness from /dev/urandom via plain stdio (LCG mix fallback).

Names scrinium/sigillum are WORKING NAMES, unsealed (gradus doc).

## 2026-10-24 — trace VFS (sqlite reads/writes in fabrica's trace)

sqlite does its own open/read/write below filum, so fabrica's trace (FABRICA_LECTIONES) never
saw database files: a member reading a .db had an incomplete trace, and writes were only caught
by the before/after tree snapshots. scrinium_aperire_cum now opens through a wrapper VFS,
"rhubarb-vestigium" (registered once, non-default; a copy of the default VFS struct with
xOpen/xDelete overridden and pAppData = the real VFS). Each opened file is a FilumVestigii:
our sqlite3_file header, then the real VFS's file right after it (szOsFile = ours + theirs).
Methods forward; xRead and xFetch note L once per file, xWrite and xTruncate note S once,
xDelete notes S. Without FABRICA_LECTIONES lectiones_notare is a no-op, so the cost outside
fabrica is one flag check per call.

Gotchas:
- WAL's -shm file is written through mmap, never xWrite: S for `via + "-shm"` is noted at
  xShmMap (the test plants this away and goes red).
- Only set pMethods when the real xOpen set it: sqlite calls xClose iff pMethods != NULL after
  a failed open.
- sqlite canonicalizes paths (/var -> /private/var), so tests match trace lines by suffix.
- Journal/WAL siblings show up as their own S lines, so three tests that kept dbs under build/
  (mensa, volumen, scrinium) were caught writing -journal/-wal files their debts didn't cover.
  Moved all into credo areas and dropped the three debitum_scripturae lines.
