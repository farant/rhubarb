# contractio worklog

## 2026-10-27 — T2 of project-specs/contractio-plan.md

New house library: tensor networks, a deterministic greedy contraction
order, and a slab layout with every intermediate's offset fixed before
anything is allocated. Execution (hot i32 tier, anulus reference tier)
is T3; in T2 both `contractio_exsequi_*` refuse.

**Greedy key, exactly.** Among pairs of live tensors sharing at least
one index: minimize (result entries, saturating; smaller id; larger
id). The result takes the next id (inputs 0..n−1, intermediates n,
n+1, ...). If no pair shares an index, the same key over all pairs
(outer product). The key never looks at index ids, so renumbering
indices leaves order, costs and slab unchanged (tested on 200 random
networks); insertion order of tensors can change cost and is not
claimed invariant.

In a closed network the outer-product fallback only ever sees scalars:
if a live tensor kept an index, the index's other end is in some other
live tensor (both ends in one tensor would have been contracted). So
every fallback key is 1 and the fallback always picks the two smallest
ids. Its key comparison is unobservable until open indices exist (not
in v1); the fallback itself is planted (P19).

**Slab.** Intermediates are laid out in step order. The result of step
s is placed first-fit among the intervals live at s — its operands
included, so the kernel never writes over what it reads — and then the
operands are freed. Inputs are never in the slab (they stay in the
caller's `valores`). The test checks the layout without reimplementing
it: intermediates with overlapping lifetimes [born, consumed] have
disjoint intervals, and no smaller candidate offset (0 or the end of an
interval live at birth) would fit.

**Saturation and latitudo.** All i64 costs saturate at 2^64 − 1. An
intermediate whose size equals that value is treated as saturated even
if it is exact (42007935 · 65537 · 6700417 = 2^64 − 1 is in the test),
and its latitudo is recomputed exactly with magnus: ceil(log2 N) =
bits(N − 1). Below saturation it is bits(entries − 1) on the u64.

**Officina rule.** The officina is the slab only (4 bytes per element).
It is refused when the slab or the multiplication count saturates.
That second refusal also gives T3 a free bound: a step whose union has
k indices of dimension ≥ 2 does at least 2^k multiplications, so with
multiplications < 2^64 every step has ≤ 63 nontrivial indices and the
kernel's odometer counters fit a fixed stack array. No counter area in
the officina, no separate rank limit.

**API additions (beyond the plan's header):** three read-only
inspection functions — `contractio_ordo_gradus_numerus`,
`contractio_ordo_gradus` (sinister, dexter, effectus of step k) and
`contractio_ordo_sedes` (offset and size of an intermediate in the
slab) — needed by the tests and by T5's measurements.

**Validation of a tensor in O(numerus):** each index is marked with a
high bit in its use count while the tensor is checked (a repeat sees
the mark), and the marks are cleared on every exit. A refused tensor
leaves the network unchanged.

**Tests** (probationes/probatio_contractio.c, credo v2, 12 sections,
89,235 checks, 0.04 s). Hand-computed orders, costs and slab offsets
(each confirmed with a throwaway Python model before being written
into the suite): chain (and a five-chain exercising both tie-breaks),
ring with dimensions 2, 3, 5, 7, 2×2 torus (pairs share two indices),
star (largest leaf first; slab reuse at offset 0), disconnected
components and zero-index tensors, empty / single / two scalars. 300
random networks (2–12 tensors, 0–20 indices of dimension 1–4) against
an independent count with bitmask index sets. 200 random networks
built twice and with indices renumbered: identical. Saturation: star
over 3^20 (latitudo 96), triangle with 2^80 intermediate (80),
triangle over 3^20 (3^40 fits, latitudo 64, but the slab's bytes
saturate), an intermediate of exactly 2^64 − 1, and 2^90
multiplications with a one-element slab (officina still refused).

**Plants (silva.planta, radix/contractio), 19/19 red:** P1 key ignores
size; P2 larger-id tie-break reversed; P3 smaller-id tie-break
reversed; P4 slab never reuses; P5 result may overlap its operands; P6
shared dimensions left out of multiplications; P7 no saturation; P8
latitudo bits(x) instead of bits(x − 1); P9 saturated latitudo without
magnus; P10 repeat mark never set; P11 third use admitted; P12 marks
left after success; P13 network frozen before the closed check; P14
saturated multiplications admitted by the officina; P15 officina limit
exclusive; P16 slab positions reported when saturated; P17 owner
update always writes slot 0; P18 right operand stays live; P19
outer-product fallback skipped. First forms of P5 (`||` beside `&&`,
-Wlogical-op-parentheses) and P19 (anchor ending inside a comment
token) did not run; replanted as a ternary and on the code line.

Private names prefixed (`IntervallumTabulae`, `CONTRACTIO_NULLUS`,
`CONTRACTIO_USUS_SIGNUM`): `Intervallum` is a type in fasti.h and
`NULLUS` a macro in terminale.h / aemulator.h — harmless today, a
collision in any amalgamation that includes both.

## 2026-10-27 — T2 review (independent, after c1453413) and fixes

Reviewer ported ordering, slab and latitudo to Python and fuzzed the C
build (ASan + UBSan) against the port and against an independent
set-based model: 19,000 networks in five regimes (up to 60 tensors and
90 indices; huge dimensions; all dimension 1), ~40% hitting the
outer-product fallback, ~4,300 saturating elements, ~640 saturating
only the ×4 bytes — 0 mismatches. Intermediate index lists sorted and
equal to the symmetric difference in 3,000 networks. Braid-shaped
networks order fast: 12 strands / 801 crossings 19 ms (width 26, slab
710 MB), 3,000 crossings 0.25 s, 8,000 crossings 2.0 s. No H.

New invariant from the review: slab end ≤ Σ intermediate sizes ≤ Σ
step multiplications, so `octeti_officinae ≤ 4·multiplicationes` and
the slab-end saturation branch fires only when multiplications are
already saturated (officina refused either way). Commented in place;
the branch stays as a guard and is not testable.

Fixed here:
- **M1** `piscina` wraps on sizes near 2^64 (`offset + mensura` in
  `_allocare_interna`; verified by the reviewer: a (size_t)−16 request
  succeeds and the next allocation overlaps a live one). Not fixed in
  piscina here (shared foundation library; filed for Fran). contractio
  now refuses an officina above 2^62 bytes whatever `octeti_maximi`
  says. No known network reaches it (the reviewer's targeted search
  topped out at 7.7e10 bytes), so it is defense in depth, not planted.
- **L1** the outer-product fallback was O(L^2) per step, O(L^3) for a
  tail of L scalars (3,000 scalars: 6 s). When every live tensor is a
  scalar (always, in a closed network) every key is 1, so the pair is
  simply the two smallest live ids; the general scan stays for open
  indices later. New section "scalares multi": the order of 7 scalars
  by hand, and 4,000 scalars under CREDO_NON_PENDET 2 s (P20 red: the
  old scan times out).
- **L3** the intermediate index order (sorted by id, which the header
  promises and open indices will need) was unobservable: deleting the
  insertion sort survived. New accessor `contractio_ordo_indices`
  (inputs in caller order, intermediates ascending); the random section
  now checks each intermediate's list is strictly ascending and equals
  the symmetric difference, and inputs return the caller's list.
- **L4** header: `contractio_ordo_sedes` refuses when slab ELEMENTS
  saturate, not when only the byte count does (the 3^20 triangle).
- **L5** header: `ordinare` allocates network-sized memory with the
  fatal allocator (fatal on a fixed-size arena).
- **L6** header: the slab is one allocation, plus a small struct.

Carried to T3 (in the plan): **M2** the ≤ 63 bound counts only
dimension ≥ 2 indices — 100 shared dimension-1 indices give one
multiplication — so the odometer and strides must skip dimension-1
indices (they never move an offset), with a test of ≥ 64 dimension-1
indices in one step. **M3** `contractio_exsequi_anulo` has no officina
and so none of its guarantees: it must refuse saturated
multiplications or apex itself and size per-step scratch with overflow
checks and `piscina_conari_allocare`; test it on the saturation
networks. **N4** n == 1 → the value is `valores[0][0]`, n == 0 → 1;
inputs keep the caller's index order, so strides are per operand;
check `officina->ordo == ordo` and `ordo->rete == rete`.

Not acted on: L2 (guard branch, see invariant), L7 (`piscina_reficere`
in `_latitudo` only affects memory), N1 equivalent mutants, N2
disconnected components interleave by key (by spec; revisit if #SAT
brings many components), N3 i32 id arithmetic overflows only near 2^31
tensors.

**Plants after the fixes, 4/4 red:** P20 scalar tail by full scan
(CREDO_NON_PENDET), P21 scalar tail picks the wrong pair, P22
intermediate lists not sorted (survived before), P23 `ordo_indices`
one past the end.

Tests now: 13 sections, 106,758 checks, 0.09 s.

## 2026-10-27 — T3: execution (hot tier and reference tier)

**Loop plan built at `ordinare`, not per execution.** Each step stores
its *externi* (result indices of dimension ≥ 2, in the result's order)
and *interni* (shared indices of dimension ≥ 2, in the left operand's
order), with their dimensions and strides in both operands (stride 0 =
index absent from that operand). Dimension-1 indices never enter the
plan (one helper, `_movet`, decides it in all four places), which is
the review's M2: they never move an offset and can be arbitrarily many.
With multiplications unsaturated the plan has ≤ 63 entries, so the
counters are a 64-entry stack array. Strides are per operand: inputs
keep the caller's index order, intermediates are sorted.

**Hot tier.** Externi are walked in the result's own order, so the
result offset is just o = 0, 1, 2, ...; an odometer over externi moves
the two operand offsets. Per result entry, the innermost shared index
runs as a tight loop and the rest by odometer. Accumulation is two
words: `humile += a·b; if (humile < a·b) portatio++` (a·b < 2^64 since
a, b < 2^32), reduced once as ((portatio mod n)·(2^64 mod n) mod n +
humile mod n) mod n — exact for any sum length and any 2 ≤ n < 2^32.
No allocation: intermediates live in the officina's slab at T2's
offsets. n = 0 gives 1, a single scalar gives valores[0][0].

**Reference tier.** The same plan and odometer over any anulus. Two
private dynamic arenas: *servata* holds every intermediate (memory =
sum of intermediates; reference tier is for small networks), and
*scrinium* holds each entry's partial sums and products, reset after
the entry's sum is transcribed into servata. The result is transcribed
into the caller's arena. Refusals before any value is read:
multiplications or apex saturated (the review's M3 — no officina guards
this tier); each intermediate's byte size is checked against overflow
and allocated with `piscina_conari_allocare`.

**Tests** (21 sections, 118,633 checks, 0.07 s): 200 small random
networks (1–7 tensors, 0–9 indices of dimension 1–3) where the hot
tier equals brute-force summation over every index assignment and
equals the reference tier over Z/n, for n = 2, 17, 998244353,
4294967291 and 0xFFFFFFFF; 60 networks over Z and over Z[t, t⁻¹]
(reference result reduced mod p, or evaluated at x = 12345 mod p,
equals the hot tier on reduced values); every value n − 1 with a
shared dimension of 4096, and a 3 × 2048 × 5 chain, at both large
moduli; one officina reused for 100 executions with no growth of the
caller's arena; every refusal leaves `exitus` untouched; 100 shared
dimension-1 indices plus one of dimension 3; n = 0 and n = 1 on both
tiers; the reference tier refuses saturated networks before reading
values.

**Near-2^64 reduction.** Plant Q3 (adding `humile` unreduced) survived
the first suite: it only overflows when humile + (portatio part) ≥
2^64, which random data essentially never hits. Constructed case: n =
3·2^30 (2^64 mod n = 2^30), v·w over dimension 4 with three products
(n − 1)^2 and the fourth (n − 1)·1789569713: sum = 2^65 − 715827886,
portatio 1, humile within 2^30 of 2^64. Expected 1431655762 (Python,
exact). New section "reductio prope 2^64"; Q3 now red.

**Plants (radix/contractio), 15/16 red:** Q1 carry dropped; Q2 2^64
mod n off by one; Q3 low word unreduced (red after the new section);
Q4 odometer reset; Q5 dimension-1 indices in the plan (stack overrun,
red); Q6 inner stride from the wrong operand; Q7 all strides 1; Q8
wrong operand chosen (crash); Q9 result read from the first step; Q10
single tensor; Q11 empty network; Q12 rete check removed; Q13 officina
check removed; Q15 reference sums transcribed into the scratch arena;
Q16 reference tier's multiplication guard removed (crash on 2^90
iterations). Q14 (reference tier's apex guard removed) survives: a
saturated apex also fails the per-intermediate size check. Kept
anyway — with a 1-byte element the size check would pass a size near
2^64 to piscina, which wraps there (tabularium 01M5YW4S...).
First forms of Q6, Q7, Q15 left a variable unused (-Werror) and Q16's
multi-line anchor changed the line count; rewritten.

## 2026-10-27 — T3 review (independent, after 21c44e3d) and fixes

Reviewer built a sanitized scratch copy and fuzzed 62,500 networks
(53,000 small against brute force with 128-bit mulmod; medium; dims up
to 400; up to 120 indices, ~88% of dimension 1, so well over 64
dimension-1 indices per step) at moduli 2, 3, 4, 17, 65537, 998244353,
2^31, 3·2^30, 4294967279, 4294967291, 0xFFFFFFFF and random, with
random / all n−1 / 0 and n−1 values. Four answers per network: hot
tier, brute force, an independent evaluator built from
`contractio_ordo_indices` that bypasses `_gradum_parare` and the
odometer, and the reference tier over Z/n (and Z). Officina reuse
checked. 0 mismatches, ASan/UBSan clean. No H, no correctness M.

Fixed here:
- **L1** the reference tier's size check (`elementa > (2^64−1) /
  mensura`) still passed byte sizes in the last KB below 2^64, where
  piscina wraps; and the T3 note above was wrong about what the apex
  guard (Q14) protects — it only catches apex == 2^64 − 1. Now each
  intermediate is refused above 2^62 bytes, as the officina is. The
  apex guard stays as a cheap explicit refusal; it is not independently
  testable (Q14 survives because the size check also refuses).
  Unreachable in practice per the reviewer (every attempted network
  saturated multiplications first); not planted.
- **L2** the random sections were narrower than the code: dims 1–3
  only, and all-n−1 values only on two fixed chains. Random dims are
  now 1–5 (capped so brute force stays ≤ 2^16 assignments: the largest
  dimension is decremented until it fits) for both "vis bruta" and
  "anuli Z et Z[t]", and every third brute-force network uses all n−1
  values.
- **N** n = 1 now returns valores[0][0] mod n (free; plant Q17 red);
  header: element memory in the reference tier (anulus operations,
  transcribe) is allocated fatally — only the intermediate tables are
  tried allocations.

Deferred to T5 (performance, not correctness): the reviewer measured
1.3–1.8 ns per multiplication on braid-closure networks (8 strands /
400 crossings: 0.15–0.2 s per execution; 10 / 600, width 22: 14.6 s).
Cheaper reduction made no difference; the overhead is the per-entry
odometer (typical interni loop length 2). A prototype with a
precomputed table of operand offsets over all interni assignments
(inner loop `acc += A[oa + ta[q]] * B[ob + tb[q]]`) and the fastest
externus as a plain loop was ~1.65× faster (1.05 vs 1.7 ns on 8/400),
identical results. Tables could be built at `ordinare` (capped, odometer
as fallback) so execution still allocates nothing. Width-22 steps are
partly memory-bound — slicing or a GEMM-style layout, also T5.
Prototype: scratchpad/recensio_contractio_t3/var/contractio_b.c
(session 2026-10-27).

Tests now: 21 sections, 153,949 checks, 0.11 s.
