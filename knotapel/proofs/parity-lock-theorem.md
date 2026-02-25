# Parity-Lock Theorem

**Demo 92 — Function Scaling**
**Date:** 2026-02-24
**Status:** THEOREM (proved + computationally verified)

## Encoding Definition

The **±q input encoding** with k weights assigns 2 input bits per weight.
For weight i with quaternion q_i:

- bit 2i: if set, adds +q_i to sum; contributes nothing if unset
- bit 2i+1: if set, adds -q_i to sum; contributes nothing if unset

Per-weight contribution by bit pattern:

| (bit 2i, bit 2i+1) | Contribution |
|---------------------|-------------|
| (0, 0)              | 0           |
| (1, 0)              | +q_i        |
| (0, 1)              | -q_i        |
| (1, 1)              | 0           |

Total input: 2k bits. Total masks: 2^{2k}.

## Theorem

**(a)** The collision (0,0) ≡ (1,1) means the all-zeros mask and the all-ones mask always produce identical sums (both zero). More generally, masks produce identical sums iff they agree on the effective state {-q, 0, +q} for each weight.

**(b)** Any Boolean function f where f(0...0) ≠ f(1...1) is structurally impossible under ANY activation, because the sums are identical and no activation can distinguish them.

**(c)** AND(0...0) = 0 ≠ AND(1...1) = 1. OR(0...0) = 0 ≠ OR(1...1) = 1. MAJ(0...0) = 0 ≠ MAJ(1...1) = 1. All impossible.

**(d)** XOR(0...0) = XOR(1...1) = 0 for even-length inputs (both have even Hamming parity). The collision is harmless.

**(e)** More generally: the equivalence classes partition 2^{2k} masks into 3^k classes. Within each class, all masks have the same Hamming parity. Therefore XOR (= Hamming parity) is constant on every class. Any function NOT constant on these classes is impossible.

**(f)** XOR and XNOR are the only standard Boolean functions that are constant on all equivalence classes for arbitrary k.

## Proof

**(a)**: Direct computation. (0,0) contributes nothing. (1,1) contributes +q_i + (-q_i) = 0. So masks 0...0 (all pairs are (0,0)) and 1...1 (all pairs are (1,1)) both produce sum = 0.

**(b)**: If sum(m) = sum(m') then combined_cell(sum(m)) = combined_cell(sum(m')) for any activation. Therefore f(m) = f(m') for any achievable function f.

**(c)**: Truth table evaluation of standard functions at the all-zeros and all-ones masks.

**(d)**: XOR(0...0) = parity(0) = 0. XOR(1...1) = parity(2k) = 0 (even). Both zero. No conflict.

**(e)**: The equivalence classes are defined by agreement on {-q, 0, +q} for each weight. 3 effective states per weight, k weights, so 3^k classes. Within a class, the only variation is (0,0) ↔ (1,1) swaps for weights in the 0-state. Such a swap changes Hamming weight by ±2 (replacing 0 set bits with 2 set bits, or vice versa), preserving parity. The +q and -q states each have exactly 1 set bit. Therefore: parity of a mask = (number of weights in non-zero state) mod 2, which is well-defined per equivalence class.

**(f)**: Among standard Boolean functions, only XOR/XNOR depend solely on Hamming parity. AND, OR, MAJ, NAND, NOR, IMPLIES, and threshold functions all distinguish some pair of masks within the same equivalence class (specifically, the (0,0,0,...) class contains both 0...0 and 1...1 which differ for all of these functions).

**QED.**

## Sign-Flip Symmetry (supplementary)

For sum S = c_1 q_1 + c_2 q_2 + ... + c_k q_k, the negated class produces sum -S. The combined_cell activation normalizes by |scalar part| (flipping if negative), which maps S and -S to identical (sector, Voronoi cell) pairs.

**Proved computationally**: 13/13 sign-flip pairs have identical sector, Voronoi cell, combined cell, and parity.

**Consequence**: The 3^k equivalence classes collapse to at most (3^k + 1)/2 distinguishable groups under the activation. For k=3: 27 classes → at most 14 groups. Observed: 11 distinct cells.

## Numerical Verification

All results from Demo 92 (ζ₁₂ catalog, 4096 entries, 9 BFS rounds):

| Phase | Test | Result |
|-------|------|--------|
| 1 | AND/OR/MAJ/THR2 winners at depths 0-8 | ALL ZERO |
| 1 | XOR winners at depth 0 (6-input) | 8 |
| 1 | XOR winners at depth 8 (6-input) | 3866 |
| 1b | XOR6-separable (10K random triples) | 83.7% |
| 1b | AND/OR/MAJ-separable | 0.0% each |
| 1c | Mask sum collisions (2016 pairs) | 76 total |
| 1c | Same-parity collisions | 76/76 (100%) |
| 1c | Cross-parity collisions | 0/76 (0%) |
| 1d | XOR constant on all 27 classes | YES (0 conflicts) |
| 1d | AND class conflicts | 1/27 |
| 1d | OR class conflicts | 1/27 |
| 1d | MAJ class conflicts | 19/27 |
| 1e | Sign-flip same sector | 13/13 |
| 1e | Sign-flip same Voronoi | 13/13 |
| 1e | Sign-flip same cell | 13/13 |

Multi-triple cell counts (k_sec=12):
- Winners: 11-13 distinct cells (of 27 classes)
- Non-winners: 6-11 distinct cells

---

# Complement-Blindness Theorem

**Demo 93 — Function Scaling (1wpi)**
**Date:** 2026-02-24
**Status:** THEOREM (proved + computationally verified)

## Encoding Definition

The **1-weight-per-input (1wpi) encoding** with N weights assigns 1 input bit per weight.
For weight i with quaternion q_i:

- bit i = 0: contributes -q_i to sum
- bit i = 1: contributes +q_i to sum

Sum = Σ_i (2·bit_i - 1) · q_i. No zero state — all weights always participate.
Total input: N bits. Total masks: 2^N.

## Theorem

**(a)** Under 1wpi encoding, mask m produces sum S(m) = Σ_i s_i q_i where s_i ∈ {-1, +1}. The complement mask ~m produces sum S(~m) = Σ_i (-s_i) q_i = -S(m).

**(b)** The combined_cell activation normalizes by |qa| (flipping sign of all components if the scalar part is negative). Therefore combined_cell(S) = combined_cell(-S) for all S. Masks m and ~m always produce the same cell.

**(c)** Any Boolean function f where f(m) ≠ f(~m) for any mask m is impossible under combined_cell activation, because m and ~m are mapped to the same cell.

**(d)** AND: AND(0...0) = 0, AND(1...1) = 1. Since ~(0...0) = 1...1, AND requires f(m) ≠ f(~m). Impossible for all N.

**(e)** OR: same argument. MAJ: same argument. All impossible for all N.

**(f)** XOR at odd N: XOR(0...0) = 0, XOR(1...1) = parity(N) = 1 (odd). Since ~(0...0) = 1...1, XOR requires f(m) ≠ f(~m). Impossible for odd N.

**(g)** XOR at even N: XOR(0...0) = 0, XOR(1...1) = parity(N) = 0 (even). No conflict at 0/1 endpoints. More generally, for even N, XOR(m) = XOR(~m) for all m because flipping all N bits changes Hamming weight by N - 2·popcount(m), and parity(N - 2k) = parity(N) ⊕ parity(2k) = 0 ⊕ 0 = parity(m) ⊕ 0 ... more directly: popcount(~m) = N - popcount(m), and (N - popcount(m)) mod 2 = popcount(m) mod 2 iff N is even.

## Proof

**(a)**: Direct computation. If bit i of m is b_i, then bit i of ~m is 1-b_i. Contribution of weight i under m: (2b_i - 1)q_i. Under ~m: (2(1-b_i) - 1)q_i = (1 - 2b_i)q_i = -(2b_i - 1)q_i. Sum over all i: S(~m) = -S(m).

**(b)**: In the combined_cell code: normalize q to unit quaternion (a, b, c, d)/|q|. If a < 0, flip: (a, b, c, d) → (-a, -b, -c, -d). This maps -S to the same normalized quaternion as S (since -S has all components negated, and |(-a)| = |a|, the flip condition produces the same result). Therefore sector (from half-angle of a) and Voronoi cell (from normalized axis) are identical.

**(c)**: If combined_cell(S(m)) = combined_cell(S(~m)), then any function realized by the activation must satisfy f(m) = f(~m).

**(d)-(f)**: Truth table evaluation at m = 0...0 and ~m = 1...1.

**(g)**: popcount(~m) = N - popcount(m). Parity(~m) = (N - popcount(m)) mod 2 = N mod 2 ⊕ popcount(m) mod 2. If N even: parity(~m) = parity(m). So XOR(m) = XOR(~m) for all m.

**QED.**

## Numerical Verification

All results from Demo 93 (ζ₁₂ catalog, 4096 entries, 1wpi encoding):

| N | Parity | XOR achievable? | AND | OR | MAJ | Prediction |
|---|--------|-----------------|-----|-----|-----|------------|
| 3 | odd | NO (0 at all depths) | 0 | 0 | 0 | correct |
| 4 | even | YES (5→4608) | 0 | 0 | 0 | correct |
| 5 | odd | NO (0 at all depths) | 0 | 0 | 0 | correct |
| 6 | even | YES (90→630) | 0 | — | — | correct |
| 7 | odd | NO (0 at all depths) | 0 | — | — | correct |
| 8 | even | YES (1→3) | 0 | — | — | correct |

Complement-pair cell sharing:
- N=3: 400/400 (100%)
- N=4: 800/800 (100%)
- N=5: 1600/1600 (100%)
- N=6: 3200/3200 (100%)

XOR depth profile (N=4, bf_max=20):

| Depth | XOR winners |
|-------|-------------|
| 0 | 5 |
| 1 | 2058 |
| 2 | 4500 |
| 3 | 4506 |
| 4 | 4582 |
| 5 | 4600 |
| 6 | 4606 |
| 7 | 4608 |
| 8 | 4608 |

## Unification

D92 (Parity-Lock) and D93 (Complement-Blindness) are structurally parallel:

| | D92: ±q paired | D93: 1wpi |
|---|----------------|-----------|
| **Source of collision** | Encoding: (0,0) ≡ (1,1) | Activation: combined_cell(S) = combined_cell(-S) |
| **Collision pairs** | mask 0...0 ≡ mask 1...1 | mask m ≡ mask ~m |
| **Equivalence classes** | 3^k (per-weight {-q,0,+q}) | 2^{N-1} complement pairs |
| **XOR status** | Always achievable (even input count) | Achievable iff N even |
| **AND/OR/MAJ** | Impossible (all N) | Impossible (all N) |
| **Bottleneck** | Encoding structure | Activation symmetry |

**Common principle**: any symmetry that identifies two masks forces f(m) = f(m'). The encoding and activation each independently impose symmetries. The intersection of achievable functions is those constant on ALL equivalence classes from ALL sources.

---

## Circuit Complexity Hierarchy (phase_cell activation)

**Demo 93, Phases 5-8.** Removing the `if (qa < 0)` sign flip from combined_cell creates phase_cell, which breaks complement-blindness. Under phase_cell, all 13 NPN classes are achievable.

The circuit complexity hierarchy manifests as hit rate (solution density) differences, not onset depth differences. Full table at depth 1 (ζ₁₂, 17 entries, 84 cells, all exhaustive):

| N | masks | XOR% | AND% | MAJ% | AND/XOR ratio |
|---|-------|------|------|------|---------------|
| 3 | 8 | 96.8% | 97.7% | 99.7% | 1.01 |
| 4 | 16 | 85.6% | 92.3% | 91.1% | 1.08 |
| 5 | 32 | 58.3% | 84.1% | 94.3% | 1.44 |
| 6 | 64 | 9.9% | 71.7% | 49.6% | 7.27 |
| 7 | 128 | 0.02% | 56.8% | 68.7% | 2762 |
| 8 | 256 | 0.00% | 41.6% | 2.4% | ∞ |

Three regimes:
- **AND (AC⁰)**: 1-hot → only needs one unique cell → survives pigeonhole
- **MAJ (TC⁰)**: Threshold partition → cliff at N=8 (2.4%)
- **XOR (∉ AC⁰)**: Balanced bisection → killed when 2^N > cells (N≥7)

MAJ/XOR crossover at depth ≈3-4 (N=6): parity harder to start but easier to complete with depth.

---

## Connections

- **D48/D50**: 1wpi encoding with split-sigmoid/MVN activation computes all 13 NPN classes. Those activations do NOT have sign-flip symmetry (angle(z) ≠ angle(-z) in the complex plane). The parity lock is specific to combined_cell.
- **D89-D91**: The depth law max_xor ≈ depth + 6 describes the capacity of the ±q encoding for its ONLY achievable function family (parity).
- **Fourier analysis**: Both encodings concentrate computational power on parity — the highest-frequency Boolean function and the one outside AC⁰.
- **Activation as gatekeeper**: The encoding provides algebraic raw material. The activation determines which functions are extractable. combined_cell's |qa| normalization is the specific mechanism that restricts to complement-safe functions.
- **Håstad (1986)**: Parity requires exp(Ω(n^{1/(d-1)})) gates at bounded depth. The AND/XOR ratio explosion (1→2762→∞) is the DKC manifestation.
- **Barrington (1989)**: Solvable/non-solvable group boundary = finite/infinite capacity boundary. DKC is a hybrid model (multiplicative construction + additive readout) in the group algebra C[G].
