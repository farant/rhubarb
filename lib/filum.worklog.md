
## 2026-09-11 — the mode pair: copy drops it, and status couldn't see it

Found while designing `fasciculum` (`project-specs/fasciculum-spec.md`
§7), by reading, before any bundle code existed. Two gaps, one cause —
nothing in this library could set or observe a file's executable bit.

1. **`filum_copiare` does not preserve mode.** It is a plain
   `fopen`/`fread`/`fwrite` loop (`lib/filum.c:814-865`), so the
   destination gets whatever `fopen("wb")` plus umask gives it,
   typically `0644`. The only chmod in the house lived inside
   `filum_directorium_creare_cum_modo`, which handles DIRECTORIES.
   Consequence for the next library up: a binary copied into
   `Foo.app/Contents/MacOS/` lands non-executable, and a bundle whose
   executable is not `+x` fails at double-click **with no diagnosis** —
   Finder simply declines. That is the worst failure shape this arc
   could have shipped, and it would have shipped silently.
2. **`FilumStatus` had no execute flag** — `mensura`,
   `est_directorium`, `est_filum`, `potest_legere`, `potest_scribere`
   and nothing more. So even once the bit could be set, a test could not
   assert the bit it had just set. A guard you cannot observe is not a
   guard.

Added: `filum_modum_ponere(via, modus)` (chmod, refusing a NIHIL path
and a failed call rather than returning quietly) and
`FilumStatus.potest_exsequi`, set in both the `_WIN32` (`_S_IEXEC`) and
POSIX (`S_IXUSR`) arms of `filum_status`. The field is APPENDED;
`filum_status` fills every member explicitly, so growing the struct is
safe here — contrast the house rule about structs that are allocated
without `memset`.

**`chmod` needed no NEW feature-macro work — but not because the dance
is unnecessary.** The plan warned that `-std=c89` defines
`__STRICT_ANSI__` and might hide `chmod` the way it hides `strtoll`. It
did not, and the first draft of this note credited `<sys/stat.h>` for
that. Wrong reason, worth correcting because it would mislead the next
reader: `lib/filum.c:1` already includes `postulata_posix.h` — codex 85
enforces it as the FIRST include — and that header is precisely where
the dance happens (`_DEFAULT_SOURCE`, chosen because glibc under
`-std=c89` hides POSIX declarations while Darwin is permissive). On
macOS `<sys/stat.h>` alone would indeed have sufficed; on Linux glibc it
would not, and the header that saves us was already in place. Nothing
needed adding — which is not the same as nothing being needed.

**The Windows arm is written but unverified.** The neighbouring
`filum_directorium_creare_cum_modo` guards every `chmod` with `#ifndef
_WIN32`, and the first draft of `filum_modum_ponere` called bare `chmod`
unconditionally — which would not even compile under MSVC, where the
spelling is `_chmod` and `mode_t` does not exist. It now mirrors the
file's own convention: `_chmod` under `_WIN32` (from `<io.h>`, already
included in that arm), `chmod` otherwise. Untested; we build only on
macOS. Two Windows caveats recorded rather than papered over — `_chmod`
honours only the write bit, and `_stat` derives `_S_IEXEC` from the file
EXTENSION (.exe/.com/.bat) rather than from any real permission. So
`potest_exsequi` is exact on POSIX and approximate on Windows.

**The test pins the reason the pair exists.** After setting `0755` on a
file it copies it and asserts the copy is NOT executable — so if anyone
later teaches `filum_copiare` to preserve mode, that assertion fails and
names the change rather than letting two behaviours drift apart
silently.

**One ordering hazard worth copying.** The test writes to fixed `/tmp`
paths, and `fopen("w")` does not reset an existing file's mode — so a
leftover `0755` file from an interrupted run would break the very first
assertion for a reason unrelated to the code. It therefore DELETES both
paths at the start, not only at the end. Same shape as the shared-fixture
lesson from plist's arc.

**Calibration, and what the plant refused to do first.** The intended
plant was to short-circuit the call — `FALSUM && chmod(…)`, which keeps
every variable referenced so nothing trips `-Wextra`'s unused-parameter
check. `silva.planta` REFUSED the obvious anchor: the statement
`si (chmod(via, (mode_t)modus) != ZEPHYRUM)` occurs THREE times in this
file — once here, twice in `filum_directorium_creare_cum_modo`, which
happens to name its parameters `via` and `modus` too — and the tool will
not guess which one was meant. The right call: planting into the
directory function would have reddened a different test and taught the
opposite lesson. The `#ifdef _WIN32` arm then supplied a unique anchor
almost by accident, since `_chmod` appears exactly once, making the span
`_chmod` → `#else` → `chmod` unambiguous.

RED on exactly one assertion — `st.potest_exsequi` at
`probatio_filum.c:1017`, the line immediately after 0755 is set — and
green again after the revert. The tightness is itself the evidence: with
`chmod` blinded, the later `CREDO_FALSUM(st.potest_exsequi)` assertions
and the copy assertion all still passed, because absent a working
`chmod` the bit is correctly absent everywhere it is expected to be
absent. A plant that reddens one assertion and only one is aimed at the
thing it claims to test.
