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

## 2026-10-05 — ordinatio ordinaria VIII (was 1)

`piscina_allocare` and `piscina_conari_allocare` now align to
`PISCINA_ORDINATIO_ORDINARIA` (8), like malloc; byte-tight packing stays
available as `piscina_allocare_ordinatum(p, n, I)`. Fran's decision (option
A over fixing call sites one by one): the short obvious call was the subtly
wrong one for most uses (973 plain call sites in lib/, 9 aligned ones).

Found by the first math-library review agent: `magnus` stored its 32-bit
limbs at odd addresses after any odd-sized allocation — undefined behaviour
in C that arm64/x86 tolerate. `-fsanitize=alignment`: 71 misaligned accesses
in the magnus suite and 24 in credo itself (house-wide); 0 after. 8 covers
every house type (pointers, i64/s64, f64); 16 would only matter for long
double / SIMD, which the house does not use.

Consequences, all 14 suites run (scratchpad ordinatio/summa.txt):

- 10 suites green unchanged.
- `probatio_piscina`: three exact-usage assertions moved by exactly their
  padding (150 → 156, 150 → 162, 100 → 106); updated with the arithmetic
  in comments, plus a new test pinning the contract (1 byte then 8 → the
  8 lands 8-aligned; `_ordinatum(…, I)` still packs contiguously).
- md and crusta `computus` goldens (arena accounting): only the byte
  columns moved — sabaw.md +8 bytes on 2,041,632; pathologiae.sh +40 on
  204,444 (0.02%); allocation counts identical. Re-recorded with
  `COMPUTUS_SCRIBERE=1`.
- **`probatio_volumen` SIGSEGV — a latent bug the change exposed.** Its
  poison test called `CREDO_NON_NIHIL` between `piscina_notare` and
  `piscina_reficere`; credo allocates its notationes in the SAME piscina,
  so the rollback freed a notatio still linked in credo's list, and the
  next allocations overwrote it (crash in `credo_imprimere_compendium`,
  pointer bytes = "men.c"). Under alignment 1 the later allocations
  happened to stop short of it. Fixed: no credo call inside the window;
  the Volumen struct is now the FIRST allocation after the rollback (so it
  still lands on the poison), checks after. Planted fault (temporarium not
  initialized) still turns it red. General hazard worth remembering: never
  call credo (or anything allocating in the same piscina) between
  `piscina_notare` and `piscina_reficere` when its result must survive.
- `probatio_cliens_tabularii` failed once in the full parallel run
  (portus_int > 0) and passed alone twice with the new code: flaky in the
  worktree, unrelated.
