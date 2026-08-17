# differentia worklog

## 2026-08-17 — natalis: nucleus Myers + involucrum linearum + emissio unificata

Born from silex desideratum 01M08Q43BT ("committed 62 files on
faith"). Design notes worth keeping:

**Core over pointer identity.** differentia_seriei never looks
inside elements — interning (internamentum) turns content-equality
into pointer-equality upstream. Same core will serve silva token
diffs and STML node diffs later.

**Traceback shape** follows the Coglan exposition of Myers O(ND):
snapshot V before each round d (reads during round d only touch
opposite-parity entries, i.e. round d-1 state), then backtrack
replays the same tie-breaking rule. Snapshots are WINDOWED
([-d-1..d+1], not the full 2*MAX array) — memory ~(D+2)^2 * 4B
instead of O((N+M)*D). Common prefix/suffix trimmed before Myers so
D sees only the changed region. D > 2048 bails to trivial
DELETA-all/ADDITA-all (invariant preserved, minimality not).

**Canonical emitter.** Myers paths can interleave -,+,-,+; the
emitter collects pending deletions/additions and flushes DELETA
before ADDITA at each IDEM boundary — GNU's canonical form, which
made the diff -U oracle tests byte-exact. Adjacent same-genus runs
merge, so a change block is at most one DELETA + one ADDITA run.

**Newline-inclusive identity.** "abc" (no final newline) and
"abc\n" are DIFFERENT lines — identity interns the span including
'\n'; display lines stored without it. GNU agrees (shows -abc/+abc
with the no-newline marker). An IDEM final run therefore implies
equal sine-fine status on both sides, which simplifies the marker
logic in the renderer.

**Oracle method.** The @@ hunk numbers and line bodies in
probatio_differentia.c were transcribed from real `diff -U` runs
(2026-08-17, BSD/GNU-compatible output), headers and the Latin
no-newline marker ours. Includes the count-omission rule (",1"
dropped) and the 0-based start for zero-count sides.

**-Wcast-qual dance.** void** → const void** is rejected; the
blessed cast is to const void* const* (all levels qualified). But a
LOCAL of that type can't be assigned after declaration (top-level
const) — so helpers return vacuum** and call sites cast via the
LECTIO() macro (tests) or inline (lib).

**Trap met:** `casus` as a loop variable — latina.h macro (case),
caught by the examen hook at write time.
