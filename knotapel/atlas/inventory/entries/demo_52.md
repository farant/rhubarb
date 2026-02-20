## Demo 52: Chebyshev Generalization — Radical Anatomy at delta=1

- **Status**: COMPLETE
- **File**: `demo_52_chebyshev/main.c` (~1954 lines)
- **Tests**: ~30+ pass (inline check() assertions across Parts 0-4)
- **Depends on**: Demo 35 (TL_n matrices, planar matching enumeration, diagram composition), Demo 51 (radical anatomy at delta=0)
- **Feeds into**: Future demos on general-delta DKC, non-semisimplicity landscape mapping

### Headline
Computes the radical structure of TL_n at four delta values (0, 1, sqrt(2), phi=golden ratio, sqrt(3)) for n=2..8, revealing how semisimplicity varies across the Chebyshev parameter space via exact modular arithmetic.

### Key Results
- **Delta=0 cross-checks**: Reproduces Demo 51 radical dimensions for n=2..5
- **Delta=1 (ell=3, percolation)**: Generators are IDEMPOTENT (e_i^2 = e_i), loops contribute factor 1 (no killing). Full analysis for n=2..8 including:
  - TL relation verification (identity, idempotent, adjacent, far commutativity)
  - Gram matrix via trace form, radical dimension via Gaussian elimination
  - Idempotent search (individual generators + length-2 products)
  - Orthogonal decomposition with complement, Peirce block dimensions
  - Radical filtration (rad^1 → rad^2 → ... until nilpotent)
  - Peirce decomposition of rad^2 where available
- **Delta=sqrt(2) (ell=4)**: Analysis for n=2..7 via modular arithmetic (Tonelli-Shanks for sqrt(2) mod p)
- **Delta=phi (ell=5, golden ratio)**: phi = (1+sqrt(5))/2, verified phi^2 = phi+1 mod p. Analysis for n=2..7
- **Delta=sqrt(3) (ell=6)**: Analysis for n=2..7
- **Cross-check**: delta=1 via general-delta pathway matches dedicated delta=1 results for n=3,4,5
- Fixed-point values are Catalan numbers (labeled in output when detected)

### Theorems/Conjectures
- **Idempotency at delta=1**: e_i^2 = e_i for all generators — PROVEN (verified n=2..8)
- **TL relations at delta=1**: Adjacent (e_i*e_{i+1}*e_i = e_i) and far commutativity — PROVEN (verified n=2..8)
- **Semisimplicity pattern**: Varies with delta — delta=0 has alternating odd/even pattern; delta=1 has different radical structure; delta=sqrt(2), phi, sqrt(3) have yet different patterns — COMPUTED (dimensions for n=2..7 or 8)
- **Peirce block sum = algebra dim**: For all orthogonal decompositions computed — PROVEN (verified for applicable n)
- **Nilpotency index at delta=1**: radical filtration terminates; predicted PIM Loewy length up to 2*ell-1 = 5 — TESTED

### Data
- Catalan number dimensions: C_2=2, C_3=5, C_4=14, C_5=42, C_6=132, C_7=429, C_8=1430
- Delta values: 0 (ell=2), 1 (ell=3), sqrt(2) (ell=4), phi=(1+sqrt(5))/2 (ell=5), sqrt(3) (ell=6)
- Modular prime: p = 10^9+7 (default), p = 999999751 (for phi/sqrt(5))
- Multiplication tables: full basis × basis with loop counts for n up to 8
- Gram matrices printed for dim ≤ 14
- Radical filtration sequences (rad^1, rad^2, ... dimensions) for each (n, delta) pair
- Distinct fixed-point values labeled as Catalan numbers where applicable

### Code Assets
- **TLAlgebra struct**: Bundled algebra data (basis, identity, generators, multiplication table with loop counts) for any n up to 8
- **Planar matching enumerator**: Segment-based recursive algorithm (from Demo 35/51), supports up to C_8=1430
- **Diagram composition**: Returns result + loop count, stored in multiplication table
- **AlgElem integer arithmetic**: Zero, basis element, add, sub, multiply (delta=1), multiply (mod p), zero test, equality, idempotent test, print
- **Gram matrix / radical dimension**: Via trace form fixed points + Gaussian elimination (both float and mod-p)
- **Radical basis extraction**: Float path (for small dims, gives integer vectors) and mod-p path (exact for any dim)
- **Radical filtration**: Iterative rad^k computation via product of rad^1 basis with rad^{k-1} basis, echelon tracking
- **Idempotent search**: Individual generators + length-2 products, orthogonality matrix, greedy maximal orthogonal set, complement computation
- **Peirce block dimensions**: Full algebra and subspace-restricted versions
- **Modular arithmetic**: mod_reduce, mod_inv (extended Euclidean), mod_pow, mod_sqrt_ts (Tonelli-Shanks)
- **General delta analysis**: analyze_tl_delta() handles arbitrary delta via precomputed delta^k powers mod p
- **Weighted Gram matrix**: General delta version with delta^loops weighting

### Literature Touched
- Temperley-Lieb algebra at roots of unity (delta = 2*cos(pi/ell))
- Non-semisimple representation theory (radical, radical filtration, nilpotency index)
- Peirce decomposition (orthogonal idempotents, block structure)
- Loewy length / PIM structure (predicted 2*ell-1 for delta=1)
- Percolation theory connection (delta=1, c=0 LCFT)
- Chebyshev polynomials (delta parametrization)
- Tonelli-Shanks algorithm (modular square roots)

### Open Questions
- What is the exact semisimplicity/non-semisimplicity pattern as a function of (n, delta)?
- Do the radical filtration dimensions follow a predictable formula across ell values?
- How does the Peirce block structure at delta=1 relate to the representation theory (which irreducibles appear, with what multiplicity)?
- Can the rad^2 generator elements be connected to neglecton-type objects from Demo 49?
- Does the Loewy length reach the predicted maximum 2*ell-1 = 5 at delta=1 for sufficiently large n?
- How do the general-delta radical dimensions connect to DKC computability boundaries from Demo 48?
