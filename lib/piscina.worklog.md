# piscina — worklog

## 2026-09-02 — the ratchet

The oversized-request branch of `_allocare_interna` set the base block
size to the size it had just computed, permanently. Blocks are meant to
double; after one oversized request the base itself jumped, so the
next doublings were of the jumped base: lib/stml.c committed 1, 2, 3,
6, 9, 18, 27, 54 MB and the final 54 MB block held under 7 MB. One
line deleted; the oversized request still gets its exact-fit block,
the base stays. Handed-out bytes unchanged, committed down (with the
Xar repairs, 635 → 160 MB on stml.c). Measured by
./silva/computus.sh; the block sequence is visible in the pinned
golden of probatio_silva_computus.
