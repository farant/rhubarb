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
