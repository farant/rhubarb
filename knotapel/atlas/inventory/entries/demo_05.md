## Demo 05: Scaling the Spin Chain

- **Status**: COMPLETE
- **File**: `demo_05_scaling/main.c` (~678 lines)
- **Tests**: ~30 pass (6 quantum dimension, Yang-Baxter per n, far commutativity, R*Rinv=I per generator, quantum trace per braid per n)
- **Depends on**: Demo 03 (Yang-Baxter / R-matrix), Demo 04 (quantum trace formula for n=3)
- **Feeds into**: later demos needing multi-strand braid representations; establishes that the trace formula is universal across strand counts

### Headline
The quantum trace formula `tr_q = (-1)^n * delta * bracket` generalizes from n=3 to arbitrary strand count (verified n=2 through n=5, matrices up to 32x32).

### Key Results
- Quantum dimension recurrence [n+1]_q = [2]_q * [n]_q - [n-1]_q verified for n=1..6
- Explicit formula [n]_q * (A^2 - A^{-2}) = A^{2n} - A^{-2n} confirmed for all n
- Yang-Baxter relation R_i R_{i+1} R_i = R_{i+1} R_i R_{i+1} holds at every scale (4x4 through 32x32)
- Far commutativity R_i R_j = R_j R_i for |i-j| >= 2 verified (first testable at n=4)
- R_i * R_i^{-1} = I for all generators at all scales
- **Key discovery**: sign depends on strand count — `tr_q = (-1)^n * delta * oracle` — even n gives +delta, odd n gives -delta (Demo 04 only saw odd case)

### Theorems/Conjectures
- PROVEN (computationally, n=2..5): Quantum trace formula generalizes with (-1)^n sign factor
- PROVEN: Sign factor arises from tr_q(I) = (-delta)^n vs oracle(identity) = delta^{n-1}
- VERIFIED: Yang-Baxter + far commutativity hold for all tested tensor-product generators

### Data
- n=2: 4x4 matrices, 1 generator
- n=3: 8x8 matrices, 2 generators (reproduces Demo 04)
- n=4: 16x16 matrices, 3 generators (first far commutativity test)
- n=5: 32x32 matrices, 4 generators (1024 Poly entries per matrix, ~800KB each)
- [1]_q = 1, [2]_q = A^2 + A^{-2}, [3]_q = A^4 + 1 + A^{-4}, etc.

### Code Assets
- Heap-allocated `Mat` struct with `mat_alloc`/`mat_free` (needed for n>=4, stack would overflow)
- `mat_tensor(C, A, B)`: tensor product of polynomial matrices — key for building R_i from base R4
- `build_Ri(result, R4, i, n)`: constructs I^{x(i-1)} x R x I^{x(n-i-1)} for arbitrary strand count
- `build_Kn(w, n)`: builds K^{xn} diagonal weights for quantum trace at any n
- `braid_product_n()`: evaluates braid word as matrix product in 2^n x 2^n space
- `mat_qtrace()`: quantum trace with diagonal weights
- State-sum oracle (`braid_bracket`, `braid_loops` via union-find) carried forward from Demo 03/04
- `MAT(m, i, j)` macro for row-major access into heap-allocated polynomial matrices

### Literature Touched
- Quantum dimension / quantum integers [n]_q (standard in quantum groups)
- Chebyshev recurrence for quantum dimensions
- Temperley-Lieb representation theory at general n
- Yang-Baxter equation in tensor-product form

### Open Questions
- Can the formula be pushed to n=6 (64x64) or beyond? Memory is the constraint (~50MB per matrix at n=6)
- The (-1)^n sign factor — is there a cleaner normalization that absorbs it?
- Connection to Fibonacci TQFT mentioned in header ("n=5 is Fibonacci TQFT boundary") but not explored
