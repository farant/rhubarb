# Proofs Index

Formal proofs filed in `knotapel/proofs/`. Each entry tracks status, what it proves, and where the result propagates in the atlas.

Generated 2026-02-20.

---

## P01: Radical Dimension Formula for TL at Roots of Unity

- **File**: `proofs/radical-dimension-formula.md`
- **Statement**: rad(TL_ℓ(2cos(π/ℓ))) = 2ℓ − 3 for all ℓ ≥ 2
- **Corollary**: rad(TL_{ℓ+1}(2cos(π/ℓ))) = ℓ² − ℓ − 3 for all ℓ ≥ 3
- **Method**: Graham-Lehrer cellular algebra theory. V_{ℓ-2} is the unique degenerate cell module at n=ℓ, with corank 1 via Chebyshev determinant (U_{ℓ-1}(cos(π/ℓ)) = 0). Radical contribution: 1·(2(ℓ-1)−1) = 2ℓ−3.
- **Status**: PROVEN
- **Previously**: VERIFIED computationally (Demos 51, 52, 60 — six data points ℓ=2..7)
- **Verification**: 23/23 cell module tests pass. Computational cross-check at all ℓ=2..7 including cubic field (ℓ=7).
- **Propagates to**: theorems.md (upgrade status), narrative.md (Section 7, Section 10 frontier), novelty.md (upgrade confidence), next-priorities.md (D65 status)
- **Related demos**: D51 (initial pattern discovery), D52 (multi-ℓ verification), D60 (cubic field confirmation), D39 Part G (cell module verification)
- **Open threads**: Nilpotency=3 from single-kernel structure? Fibonacci rank from kernel vector? Markov trace radical = 2ℓ−2?
