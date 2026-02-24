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

## Connections

- **D48/D50**: 1-weight-per-input encoding (1 bit per weight, ±q selection, no zero state) computes all 13 NPN classes. The parity lock is specific to the ±q PAIRED encoding.
- **D89-D91**: The depth law max_xor ≈ depth + 6 describes the capacity of the ±q encoding for its ONLY achievable function family (parity).
- **Fourier analysis**: The ±q encoding concentrates all computational power on parity — the highest-frequency Boolean function and the one outside AC⁰.
