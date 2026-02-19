# Long Braid Scaling

**Track:** C — TL Matrix Compilation
**Status:** Proposed
**Smaragda ID:** 01KHP7P6991F8K2JEXPM9GAW8T
**Dependencies:** Demo 29 (TL_n matrix representations)
**Code path:** `knotapel/demo_30_long_braid_scaling/`

## Hypothesis

Matrix compilation enables braid computations completely intractable for state-sum. For n=2 (2x2 matrices), sigma_1^k for k=10000 is trivial via repeated squaring but cosmically impossible for state-sum (2^10000 states). For n=3, deeply nested Boolean circuits (1000 NOTs = 5000 crossings) and multi-bit adders become routine.

## Predictions

1. Linear scaling in braid length confirmed — time proportional to k, not 2^k.
2. 1000-level nested NOT evaluates correctly (alternating identity/NOT).
3. 16-bit adder (thousands of crossings on 3 strands) computes correctly via matrix multiplication.
4. Practical strand-count ceiling around n=10-12 (C_10=16796, ~280M operations per matrix multiply).
5. Repeated squaring for sigma_1^k gives O(log k) time, enabling million-crossing braids on 2 strands.

## Approach

- **Part A:** Braid length sweep — fix n=2, compute bracket of sigma_1^k for k=1,10,100,1000,10000. Time both matrix and state-sum methods for small k and extrapolate.
- **Part B:** Gate composition depth — build NOT(NOT(...NOT(x)...)) with 100, 1000 nesting levels (5000 crossings). Verify output alternates correctly.
- **Part C:** Complex circuit scaling — full adder via matrix multiplication, then 4-bit, 8-bit, 16-bit adders. Measure time, verify correctness.
- **Part D:** Strand count scaling — fixed braid word sigma_1*sigma_2*...*sigma_{n-1} for n=2..10. Plot time vs n. Find practical ceiling.
