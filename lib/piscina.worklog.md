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

## 2026-09-02 — the peak walk

`_allocare_interna` recomputed total usage by walking every block on
every allocation, to maintain the peak figure: 1.18 M allocations
times up to 17 blocks for lib/stml.c, 13% of leaf samples in the
profile. The total now lives in `usus_currens`: the allocation adds
its delta (new offset minus old, on the block being allocated from),
`vacare` zeroes it, `reficere` recomputes it once. `piscina_summa_usus`
still walks, so the test can hold the incremental figure against the
walked one across notare/reficere. Measured (min of 7): stml.c parse
53.2 -> 49.0 ms, allocation counts identical; json.c unchanged within
noise (half the blocks).
