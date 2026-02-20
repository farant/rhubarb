## Demo 60: ell=7 Cubic Wall

- **Status**: COMPLETE
- **File**: `demo_60_cubic_wall/main.c` (~1012 lines)
- **Tests**: 15/15 pass
- **Depends on**: Demo 35 (planar matching enumeration, diagram composition), Demo 51 (radical anatomy at delta=0), Demo 52 (Chebyshev generalization, radical anatomy at multiple delta values including ell=3..6)
- **Feeds into**: Demo 61 (angular proof of 11/13 theorem), Demo 62+; extends radical dimension formulas into cubic number fields; confirms universality of radical filtration structure beyond quadratic cases

### Headline
First test of Temperley-Lieb radical anatomy at a cubic number field: at ell=7 (delta=2cos(pi/7), root of x^3-x^2-2x+1=0), all dimension formulas from Demos 51-52 extend universally — semisimple for n<7, radical appears at n=7 with dim=11=2*7-3, grows to dim=39=7^2-7-3 at n=8, rad^2 is always 1-dimensional, nilpotency is uniformly 3, and the Fibonacci rank sum F(6)=8 holds exactly.

### Key Results
- **Cubic number field arithmetic**: delta=2cos(pi/7) satisfies x^3-x^2-2x+1=0, the first non-quadratic case in the Chebyshev parameter hierarchy. Computation done via F_p[x]/(f(x)) polynomial arithmetic over finite fields.
- **Prime selection**: Two independent primes found where the cubic splits — p1=1000000007 (delta mod p1 = 420063799) and p2=1000000009 (delta mod p2 = 913264519). Cubic roots verified at both primes.
- **Semisimplicity boundary confirmed at n=ell**: TL_n is semisimple (rank=dim, rad=0) for n=2,3,4,5,6 (all n < ell=7). Non-trivial radical appears exactly at n=7.
- **Radical dimension formula verified**:
  - n=7: rad=11 matches predicted 2*ell-3 = 2*7-3 = 11
  - n=8: rad=39 matches predicted ell^2-ell-3 = 49-7-3 = 39
- **rad^2 is 1-dimensional** at both n=7 and n=8, consistent with all previous ell values
- **Nilpotency is uniformly 3**: rad^3=0 at both n=7 and n=8 (rad^2 is nonzero, rad^3 is zero)
- **Fibonacci rank test passes**: The 1-dimensional rad^2 generator decomposes by through-strand sectors into bilinear matrices whose total rank sums to F(6)=8 (the 6th Fibonacci number). Sector decomposition: sector 1 (rank 3), sector 3 (rank 4), sector 5 (rank 1), sector 7 (rank 0). Sum = 3+4+1+0 = 8.
- **Cross-validation**: Both primes agree on all ranks and radical dimensions for every n=2..8 — confirms results are characteristic-independent (i.e., genuine properties of TL_n(delta), not artifacts of the chosen prime)

### Theorems/Conjectures
- **Radical dimension formula rad(TL_ell) = 2*ell-3**: CONFIRMED at ell=7 (first cubic case), extending from ell=3,4,5,6 (quadratic cases) — UNIVERSAL across number field degree
- **Radical dimension formula rad(TL_{ell+1}) = ell^2-ell-3**: CONFIRMED at ell=7 (rad(TL_8) = 39 = 49-7-3)
- **rad^2 = 1-dimensional**: CONFIRMED at ell=7 for both n=7 and n=8 — the radical squares down to a single generator regardless of ell or number field degree
- **Nilpotency = 3**: CONFIRMED at ell=7 for both n=7 and n=8 (rad^2 != 0, rad^3 = 0)
- **Fibonacci rank = F(ell-1)**: CONFIRMED — bilinear rank across through-strand sectors sums to F(6)=8
- **Semisimplicity boundary at n=ell**: CONFIRMED — TL_n(delta) is semisimple iff n < ell, non-semisimple iff n >= ell, independent of whether delta generates a quadratic or cubic field
- **Universality conjecture**: All radical structure formulas (dimension, rad^2, nilpotency, Fibonacci rank) appear to hold for ALL ell values regardless of the degree of the number field Q(delta)/Q — STRENGTHENED (now tested at cubic)

### Data
- Minimal polynomial: x^3 - x^2 - 2x + 1 = 0 (irreducible over Q, cubic)
- Reduction rules in F_p[x]/(f): x^3 = x^2 + 2x - 1, x^4 = 3x^2 + x - 1
- Primes: p1 = 1000000007, p2 = 1000000009
- Delta mod p1 = 420063799, delta mod p2 = 913264519

| n | dim (C_n) | rank | rad | rad^2 | nilp | cross-val |
|---|-----------|------|-----|-------|------|-----------|
| 2 |         2 |    2 |   0 |    -1 |   -1 | OK        |
| 3 |         5 |    5 |   0 |    -1 |   -1 | OK        |
| 4 |        14 |   14 |   0 |    -1 |   -1 | OK        |
| 5 |        42 |   42 |   0 |    -1 |   -1 | OK        |
| 6 |       132 |  132 |   0 |    -1 |   -1 | OK        |
| 7 |       429 |  418 |  11 |     1 |    3 | OK        |
| 8 |      1430 | 1391 |  39 |     1 |    3 | OK        |

- Fibonacci rank sector decomposition at n=7:

| Sector | Elements | Matrix size | Rank |
|--------|----------|-------------|------|
| 1      | 196      | 14 x 14    | 3    |
| 3      | 196      | 14 x 14    | 4    |
| 5      | 36       | 6 x 6      | 1    |
| 7      | 1        | 1 x 1      | 0    |
| **Total** |       |             | **8 = F(6)** |

### Code Assets
- **Polynomial arithmetic over F_p mod f(x)**: `poly_mul()`, `poly_pow()` (repeated squaring), `pdeg()`, `pmod()`, `pgcd()` — complete cubic extension field arithmetic in F_p[x]/(x^3-x^2-2x+1)
- **Cantor-Zassenhaus cubic root finder**: `find_cubic_root()` — finds a root of the cubic mod p using x^p mod f(x) followed by polynomial GCD and, if f splits completely, Cantor-Zassenhaus randomized factoring
- **`find_good_prime(start, root_out)`**: Searches for primes where the cubic splits, returning both the prime and a verified root
- **TLAlgebra struct and `init_tl()`**: Full Temperley-Lieb algebra construction (basis enumeration, identity/generator identification, multiplication table with loop counts) — reusable up to n=8 (C_8=1430)
- **`analyze()`**: Complete radical anatomy pipeline — delta power precomputation, weighted fixed-point trace form, augmented Gram matrix, Gaussian elimination for rank, radical basis extraction, rad^2 computation via product echelon, rad^3 computation, nilpotency determination
- **Radical filtration**: rad^2 and rad^3 computed via explicit product of radical basis vectors with echelon deduplication; reports nilpotency index
- **Fibonacci rank test**: `fibonacci_rank_test()` — decomposes rad^2 generator by through-strand count into sector bilinear matrices, computes rank of each sector, sums to verify F(ell-1) prediction
- **`count_through_strands()`**: Counts top-to-bottom matched strands in a planar matching diagram
- **`matrix_rank_modp()`**: Generic Gaussian elimination rank computation on flat matrices mod p
- **Planar matching enumerator**: `enumerate_basis()`, `enum_segments()`, `build_boundary_order()` — segment-based recursive algorithm (from Demo 35/51/52)
- **Diagram composition**: `compose_diagrams()` with loop counting, `find_basis_index()` for lookup

### Literature Touched
- Temperley-Lieb algebra TL_n(delta) at roots of unity (delta = 2cos(pi/ell))
- Cubic number fields: Q(2cos(pi/7)) is a totally real cubic extension of Q with Galois group Z/3Z
- Minimal polynomial x^3-x^2-2x+1 for 2cos(pi/7) — well-known in algebraic number theory
- Non-semisimple representation theory: radical filtration, nilpotency index, Loewy length
- Goodman-Wenzl: TL at roots of unity is non-semisimple for n >= ell
- Fibonacci numbers in TL algebra: sector decomposition and bilinear rank
- Cantor-Zassenhaus algorithm for polynomial factoring over finite fields
- Modular arithmetic approach to exact algebraic number computation (avoiding floating point)
- Chebyshev parametrization of the semisimplicity boundary

### Open Questions
- Do the formulas rad(TL_ell)=2*ell-3 and rad(TL_{ell+1})=ell^2-ell-3 extend to ell=8,9,...? Is there a closed-form for rad(TL_n) at general n >= ell?
- Is rad^2 always exactly 1-dimensional for all ell, or does this break at some higher ell?
- Is nilpotency=3 universal for all ell >= 3, or could higher ell produce deeper radical filtrations?
- Does the Fibonacci rank = F(ell-1) pattern continue? What is the representation-theoretic explanation for this Fibonacci structure?
- What happens at ell values where Q(2cos(pi/ell)) has even higher degree (e.g., ell=11, degree 5)? Does the number field degree ever affect the radical structure?
- Can the sector-by-sector rank decomposition (3+4+1+0=8 at ell=7) be predicted from a formula involving ell and the sector index?
- Is there a uniform proof of the radical dimension formulas that works across all number field degrees, or does each degree require separate verification?
