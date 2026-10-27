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
