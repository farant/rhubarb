
## 2026-07-17 — praecidi_laterale: the all-whitespace double bug

The VANA codex-58 adjudication reached chorda's two sites. The
backward trim scan (`per (i = mensura - I; i >= ZEPHYRUM; i--)`)
on unsigned i32 could never terminate for all-whitespace input —
i wraps at zero and the loop reads wild memory. AND independently:
the initium/finis defaults meant an all-space string would have
returned UNTRIMMED (forward scan leaves initium = 0), not empty.
Fixed with the count-down idiom (`per (i = mensura; i > ZEPHYRUM;
i--)` testing datum[i - I]) plus initium = mensura as a not-found
sentinel, so initium >= finis catches the all-space case. Also
removed the dead negative clamp in chorda_ex_bytes_legibilis
(bytes is i64 unsigned). Empty-string was always guarded; only
all-whitespace hit the trap — never observed in practice, which
means no caller has trimmed a pure-whitespace string yet.

## 2026-09-11 — the whole `chorda_ut_*` family accepted NUL-truncated input

Found while designing plist (`project-specs/plist-spec.md` §6.3) by
READING, not by a failing test. Three findings in one family, now
guarded by one helper (`_chorda_nullum_habet`) called at the head of
each converter:

1. **NUL truncation.** `chorda_ut_s32`, `ut_i32` and `ut_f64` each copy
   `mensura` bytes into a malloc'd buffer, append a terminator, then
   accept when `*terminus == '\0'` — which is **true AT an embedded
   NUL**. So `1\0garbage` parsed as 1. A chorda legally contains NUL,
   so the acceptance test could not tell "consumed the whole string"
   from "stopped at a NUL".
2. **Silent truncation on overflow.** `ut_s32` cast `strtol`'s `long` to
   `s32` with no range check, so `"99999999999"` returned VERUM with a
   wrapped value. `ut_i32` uses `strtoul`, which WRAPS a leading `-`:
   `"-1"` returned VERUM carrying 4294967295. Both refuse now.
3. **A stack overread in `chorda_ex_f64`.** `snprintf` returns the
   length it WOULD have written; that value was used as the `memcpy`
   length against a 132-byte buffer. `chorda_ex_f64(1e300, 6, …)` wants
   307 bytes, so the copy read ~175 bytes past the buffer. Guarded now;
   output that does not fit returns an empty chorda.

Added alongside: `chorda_ut_s64` / `chorda_ex_s64` (plist `<integer>` is
signed 64-bit, and s32 truncates it) and `chorda_ex_f64_exacta`
(`%.17g`, so a double survives a round trip, which the fixed-decimal
form cannot do for 1e300 or 1e-20).

**Why s64 parsing is hand-rolled:** `s64` is `longus longus`
(`include/latina.h:412`) and `-std=c89` defines `__STRICT_ANSI__`, so
Apple's `<stdlib.h>` hides `strtoll`. The limit is compared LEXICALLY
against `9223372036854775807` / `…808` after leading zeros are dropped,
and negatives accumulate negatively — `-9223372036854775808` has no
positive counterpart to compare against.

Calibration: two planted faults. Blinding `_chorda_nullum_habet`
reddened the family assertion at `probatio_chorda.c:1501`; blinding the
s64 lexical limit reddened the overflow assertion at `:1482`. Both back
to green after the revert.

Note for readers of the diff: this was `lib/chorda.c`'s first pass under
the formatter, so the commit carries many `cosmetica` rewraps of
pre-existing long lines beside the four substantive functions. The
baseline lint classes it does NOT fix (`vexillum-quinquaginta` banners
at 49 columns, a few comment lines over 72) are older than this work.
