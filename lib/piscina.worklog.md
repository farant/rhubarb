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

## 2026-10-06 — VENENUM: a poisoning mode for rollback lifetimes (Fran: yes)

`piscina_reficere` neither frees nor clears, so a value that outlives its
rollback usually still reads correctly — a lifetime bug stays silent and
ASan cannot see it (the memory is still owned). The polynomium review II
planted two such bugs in the scratch-piscina code (rolling back the wrong
scratch in divide's zero-digit branch; rolling back before copying out in
multiply): the normal suite stayed green, a poisoned build caught both.

- `-DPISCINA_VENENUM=1` makes `piscina_reficere` fill every released byte
  (rest of the marked block after the mark, and every later block up to
  its offset) with `PISCINA_OCTETUS_VENENI` (0xA5) before resetting.
  Same pattern as `PISCINA_DEBUG`: a constant defaulting to FALSUM and a
  `si`, so the normal build compiles it out. `piscina_vacare` is
  unchanged (it already zeroes; code may rely on that).
- Gate `venenum` (`tools/venenum_probare.sh`, ~11 s): I. a probe — a byte
  read after reficere is 165 with the mode and 42 without (the mode is
  live AND off by default; a silent poisoning would be a dead gate);
  II. the suites that mark and roll back in hot loops — piscina, magnus,
  fractio, situs, polynomium — built poisoned with ASan/UBSan, outside the
  shared build cache (own temp dir, explicit sources).
- Born red: the two review mutants (each GREEN in the normal suite) turn
  the gate red; switching the poisoning off turns the probe red.
- Header note on `piscina_reficere`: memory is not freed or cleared;
  stale reads look right; use the mode.
- Owed automatically only once the ledger inventory ('suitae
  probationum') gets a lens for it — not possible from a worktree;
  agenda A7.

## 2026-10-21 — piscina_conari_ampliare: grow an allocation without copying

Trigger: STML serialization took a pool 3.6x the text it produced
(materia/fontes/materia_arbor.worklog.md, same date). `chorda_aedificator`
doubles by allocate-and-copy, and an arena never returns the old buffer.

Two paths, else NIHIL with the allocation untouched (the caller falls back
to allocate + copy):
- **I. last allocation of the current block, room left:** extend the
  block's offset. Same pointer.
- **II. sole allocation of the current block (starts at the block's first
  byte, ends at its offset), dynamic pool:** `realloc` the block itself.
  Nothing else can point into that block, so moving it is safe for
  everyone except holders of views into the allocation itself.

Why path I alone is not enough: a request larger than twice the initial
block size gets its own block sized `request + initial`, so a doubling
buffer overflows that slack at every doubling. Path I would have saved
almost nothing on large outputs (1+2+5+9+17 MiB for a 16 MiB buffer in a
1 MiB pool). Path II turns every doubling after the first dedicated block
into a realloc.

Under `-DPISCINA_VENENUM=1` path II always moves (malloc, copy, fill the
old block with 0xA5, free), so a stale view reads poison instead of
plausible old bytes. `numerus_allocationum` does not grow (same
allocation).

Measured (`build/sonda_partes/sonda_scriptura.c`, hilarius STML, 9.3 MB):
serialization pool 32 MiB -> 16.5 MiB (3.6x -> 1.85x). The rest is the
final doubling's slack (16 MiB capacity); untouched pages of it are never
resident.

Plants (`silva.planta`, radix): path I without the offset update -> red at
the usus delta; path II disabled -> red in the builder's pool-size check.

**Verification beyond the suites (same day).**
- ASan sweep, `tegmen_probationum.sh -sanitare -omnes`: 227 library
  suites, zero ASan reports. Reds and UBSan sites all reproduce at HEAD
  or are environmental (park 01M5GFKH01EKR57AEQ3ZQ6TEHK).
- Always-move run: path II temporarily forced to the VENENUM branch
  (move + poison on every growth), then root suites, materia, crusta,
  oratio, briar. No content failure attributable to a moved buffer. The
  first attempt at md/css/html/toml/silva ran NOTHING (exit 2): bin/fabrica
  links piscina, so the judge was stale, and the materia runner healed the
  silva amalgam WITH the plant inside. Sequence that works: plant ->
  fabrica_struere -> sanare amalgams -> fabrica_struere -> runners.
- Computus goldens (all eight clients; silva unchanged): only DOWNWARD
  moves. `allocationes_arboris` in 29 rows (crusta, oratio, briar, css,
  html, toml: -3..-49; md: -29..-212) - growths that are now in place,
  not new allocations. md also drops `usus`/`apex` by 192..17,280 bytes
  and `allocationes` by 2..110 (its builder's old buffers no longer
  stay in the pool). Regenerated with COMPUTUS_SCRIBERE=1. css was found
  only by the commit's css gate: list them with
  `git ls-files '*fixa/computus/basis.tsv'` instead of remembering.
- oratio_canon / oratio_stml over their ceilings: pre-existing on the
  branch (HEAD 885 / 4096 MiB; with this change 755 / 3871). Parks
  01M5GH4FA11PP0GGZ9TZ5E2Y7J and 01M5GKMDGSVZEKPXA54Y0VC87H.

## 2026-10-27 - size overflow near 2^64 (park …1Q6158, found by tertia's contractio review)

`_allocare_interna` computed `necessaria = ordinatus_offset + mensura` with no
check. `piscina_conari_allocare(p, (size_t)-16)` wrapped to a small number, the
"fits?" test passed, the call RETURNED A POINTER and moved the block's offset
BACKWARDS - the next allocation overlapped a live one. Same shape in four more
spots: `necessaria + mensura_alvei_initia` (wrapped to a tiny block -> the grow
loop created tiny blocks forever), `mensura_alvei_initia * II`, the alignment
helper's `ptr + (ordinatio - 1)` (and `% ordinatio` with ordinatio 0 = division
by zero; on arm64 udiv-by-0 returns 0 silently, still UB), and
`piscina_conari_ampliare`'s `offset + incrementum <= capacitas`.

Fix: `_sedem_computare` replaces `_proxima_ordinatio` - computes aligned offset
AND end, refusing ordinatio 0 or any wrap; every check compares against the
HEADROOM (`a > MAXIMUS - b`), never a sum. Overflow = allocation failure
(`_exundatio`: conari -> NIHIL, fatal -> message + exit(1)), arena untouched.
A request whose base-padded block size would wrap is refused BEFORE malloc (it
is unsatisfiable, and a near-2^64 malloc aborts under ASan / venenum).

Test `exundatio mensurae` (probatio_piscina): body runs first in a child under
CREDO_NON_PENDET (the old code hangs or divides by zero - a named failure, not
a dead suite), then in the parent. Four plants, one per guard, each red at its
own line. Lesson from the plants: three cases first passed by ACCIDENT - the
zero-alignment check only matters at offset 0 (elsewhere `ordinatio - 1` =
MAX trips the headroom clause), the base-padding check needs offset + mensura
to fit while + base wraps, and ampliare's wrap needs the allocation to start
past offset 0. Write overflow tests from the arithmetic, then plant each guard.
