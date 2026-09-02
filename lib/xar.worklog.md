# xar — worklog

## 2026-09-02 — RP Part II: the header was the cost

Measured with computus before touching anything: lib/stml.c parsed
into 457 MB handed out, 635 MB committed, 1.14 M allocations. The Xar
header was 576 bytes, 512 of them a 64-entry segment table inscribed
in every Xar, plus a 32-byte debug name nothing read; with a first
segment of 16 pointers, a one-element pointer Xar cost 704 bytes for
8 bytes of payload, and every token carried two of them for trivia.

Changes: the segment table is now allocated after the header in the
same allocation, sized at creation (`segmenta_maxima`): 24 entries by
default, 64 via `xar_creare_magnum`. Segment k holds first × 2^(k-1),
so the default with first 4 caps at 33.5 M elements and refuses
LOUDLY past it (stderr names xar_creare_magnum; xar_addere returns
NIHIL) — Fran's "big Xar" door, instead of a global shrink. First
segment 16 → 4 (10,395 of 11,010 trivia Xars held one element). The
debug name is gone. Header 576 → 48 bytes.

Same inputs, old headers on disk so the closure text is identical:
stml.c 457 → 150 MB handed out, 635 → 160 MB committed, idle 178 →
10 MB, parse 177 → 152 ms; json.c 127 → 41 MB; piscina.c 29 → 9.7 MB
and 18.5 → 9.6 ms. Allocations rose 3% (smaller first segments split
more often). Output byte-identical on 154/156 lib files via arbor.sh;
the two that differ are xar.c and piscina.c themselves.

Trap worth recording: the differential must hold the INPUT fixed. My
first after-capture ran with the edited headers in every file's
closure and would have flagged most of the corpus for the wrong
reason; the fix was to drive the built binaries directly with the
HEAD header text restored under a trap.

## 2026-09-02 — locate in constant time

`xar_locare` found the segment for an index by walking the doubling
sequence from segment 2, and `xar_obtinere` paid that walk on every
read: 16% of leaf samples on lib/stml.c after the arena fix. Closed
form: with q = index / first, the segment is highest_bit(q) + 1 and
its size is first << highest_bit(q); the bit is found by a five-step
binary shift (no intrinsics in C89). Same boundaries as the loop by
construction, pinned in the test against a reference walk for first
sizes 1, 3, 4, 7, 8 and 16 through segment count and capacity after
4352 appends, plus a value round trip. Measured (min of 7): stml.c
50.3 -> 43.1 ms, json.c 13.4 -> 11.8 ms, allocation counts identical.
