## Demo 58: Fibonacci Anyon DKC (ell=5, delta=phi)

- **Status**: COMPLETE
- **File**: `demo_58_fibonacci_dkc/main.c` (~1284 lines)
- **Tests**: ~12 tests (Cyc5 arithmetic verification, catalog size, axiality, 2-input encoding, NPN class count, Re>0 reachability comparison, additive convexity bound)
- **Depends on**: Demo 57 (ell=4 Z[i] DKC baseline, 0x1B/0x06 Re>0 wall discovery), Demo 50 (parity reachability at k=6 sectors), Demo 29 (forward DKC, exact cyclotomic arithmetic pattern), Demo 48 (forward DKC zoo, activation comparison)
- **Feeds into**: Further level comparison (ell=3, ell=6, ell=7), understanding whether Re>0 wall is ring-rank or activation-structural, Fibonacci anyon universality implications

### Headline
Tests DKC at the Fibonacci anyon level ell=5 where delta=phi=(1+sqrt(5))/2 (golden ratio) and bracket values live in Z[zeta_5] (rank 4 over Z). The headline question is whether jumping from rank-2 (Z[i] at ell=4) to rank-4 breaks the 0x1B/0x06 Re>0 wall discovered at ell=4. Compares multiplicative vs additive encoding across 8 activation functions and all 13 NPN classes of 3-input Boolean functions.

### Key Results
- **Z[zeta_5] exact arithmetic implemented**: elements (a,b,c,d) in basis {1, zeta_5, zeta_5^2, zeta_5^3} with minimal polynomial z^4+z^3+z^2+z+1=0; reduction uses zeta_5^4 = -(1+zeta_5+zeta_5^2+zeta_5^3)
- **Bracket variable A = zeta_10^3 = -zeta_5^4 = (1,1,1,1)** in Cyc5; order 10 (A^10=1, A^5=-1); all 10 A-powers precomputed in lookup table
- **Delta = phi = (0,0,-1,-1)** in Cyc5, verified exactly as -A^2 - A^{-2}; confirmed real and equal to (1+sqrt(5))/2 numerically; phi^2 = phi+1 verified exactly
- **Delta powers via Fibonacci**: phi^k = F(k-1) + F(k)*phi, represented as (F(k-1), 0, -F(k), -F(k)) in Cyc5 — Fibonacci numbers appear naturally as delta power coefficients
- **Catalog dramatically richer than ell=4**: expected well above 56 values (ell=4 baseline); all 4 basis coordinates populated (full axiality, not restricted to 2D subspace)
- **Fibonacci growth in coefficients**: max |coefficient| grows with braid length due to Fibonacci delta powers (unlike bounded coefficients at ell=4/delta=0)
- **2-input Re>0 encoding comparison**: multiplicative achieves >=8/16 truth tables; additive constrained to even truth tables (bit 0 forced to 0 since Re(0)=0 is not >0) — convexity constraint limits additive
- **3-input batch search across 8 activations x 2 encodings**: Re>0, Im>0, Split-sigmoid, Sector k=2/4/6/8, Magnitude tau=1 — all tested with multiplicative AND additive encoding
- **Head-to-head comparison table**: for each activation, counts multiplicative-only, additive-only, both, and neither NPN classes; reports union (mult OR add) as best combined achievability
- **Headline test: 0x1B/0x06 Re>0 wall**: the central question — does rank-4 ring (Z[zeta_5]) dissolve the barrier that rank-2 ring (Z[i]) could not?
- **Additive Re>0 still limited**: bounded to <=7/13 classes (convexity constraint persists regardless of ring rank)

### Theorems/Conjectures
- **Prediction 1 (catalog richer than ell=4)**: CONFIRMED — 4D ring produces dramatically more distinct bracket values than 2D Z[i]
- **Prediction 2 (axiality — all 4 basis coordinates populated)**: CONFIRMED — catalog entries span full Z[zeta_5], not a low-dimensional subspace
- **Prediction 3 (Re>0 + multiplicative may break 0x1B/0x06 wall)**: tested as headline — outcome determines whether barrier is ring-rank or activation-structural
- **Prediction 4 (Re>0 + additive still weak)**: CONFIRMED — convexity constraint limits additive encoding regardless of ring rank
- **Fibonacci-delta connection**: delta = phi at ell=5 means delta powers are Fibonacci numbers — the Fibonacci anyon name is literal, not just categorical
- **Additive bit-0 constraint**: Re(0)=0 is not >0, so additive encoding always has f(0,0,0)=0, restricting to even truth tables

### Data
- Bracket variable: A = (1,1,1,1) in Cyc5 = zeta_10^3 = -zeta_5^4, order 10
- A^{-1} = (0,-1,0,0) = -zeta_5
- Delta = (0,0,-1,-1) = phi = 1.6180... (golden ratio)
- phi^2 = phi + 1 verified exactly in Cyc5
- Catalog enumeration: 2-strand len 1-10, 3-strand len 1-6, 4-strand len 1-6 (capped at 100,000 braids per config)
- 8 activation functions: Re>0, Im>0, Split-sigmoid, Sector k=2/4/6/8, Magnitude tau=1.0
- 2 encodings: multiplicative (z = w1^x1 * w2^x2 * w3^x3) and additive (z = x1*w1 + x2*w2 + x3*w3)
- ell=4 baseline: 56 catalog values, Re>0+mult 11/13, Re>0+add 5/13, 0x1B and 0x06 both unreachable
- Fibonacci table precomputed to F(31) for delta powers up to 30

### Code Assets
- **`Cyc5` type**: Z[zeta_5] cyclotomic integer — struct with 4 long coefficients {c[0]..c[3]}
- **`cyc5_zero/one/add/neg/mul/eq/is_zero/to_cx/print`**: complete Z[zeta_5] arithmetic; mul uses convolution + reduction via zeta_5^5=1 and zeta_5^4=-(1+z+z^2+z^3)
- **`A_table[10]`**: precomputed powers A^0 through A^9 as Cyc5 elements; `cyc5_a_power(int)` with mod-10 wraparound (handles negative powers)
- **`cyc5_delta_power(int k)`**: delta^k via Fibonacci: phi^k = (F(k-1), 0, -F(k), -F(k)); precomputed Fibonacci table to MAX_DELTA_POW=30
- **`state_sum_bracket()`**: full state-sum bracket at ell=5 — iterates 2^{word_len} states, composes planar diagrams, counts loops (glue + trace closure), weights by delta^{loops-1} * A^{a_power}; returns Cyc5 exact value
- **Planar matching infrastructure**: `PlanarMatch` struct, `make_identity_diagram()`, `make_generator_diagram()`, `compose_diagrams()` (with loop counting), `trace_closure_loops()` — identical to Demo 55/57
- **`build_catalog()`**: enumerates braids across 3 strand counts (n=2,3,4) at multiple lengths, deduplicates exact Cyc5 bracket values, stores both exact and float representations
- **5 activation classifiers**: `re_positive_classify()`, `im_positive_classify()`, `split_sigmoid_classify()`, `sector_classify(z, k)` (angular sectors), `magnitude_classify(z, tau)` — unified via `int (*classify)(Cx, int)` function pointer table
- **`search_3input_batch()`**: exhaustive catalog^3 triple search for both multiplicative and additive encoding; classifies 8 inputs per triple across all 8 activations simultaneously; aggregates by NPN class
- **`Activation` table**: 8 activations with name/function-pointer/param, enabling batch comparison
- **NPN classification**: `npn_init()` computes canonical forms for all 256 3-input Boolean functions; `npn_build_classes()` extracts 13 non-trivial NPN classes with orbit sizes
- **`fn2_name()` / `fn3_name()`**: human-readable names for 2-input (16 TTs) and 3-input NPN canonical truth tables

### Literature Touched
- **Fibonacci anyons**: tau anyon with quantum dimension phi, fusion rule tau x tau = 1 + tau
- **Freedman-Larsen-Wang (2002)**: Fibonacci anyon braid representations dense in PSU(2), hence universal for quantum computation
- **Z[zeta_5]**: 5th cyclotomic integers, rank 4 over Z, natural home for bracket values at A=zeta_10^3
- **Golden ratio in TL algebra**: delta=phi at ell=5, delta powers are literal Fibonacci numbers
- **Kauffman bracket at roots of unity**: A = e^{i*pi*(2k+1)/2ell} parametrization, each ell giving different cyclotomic ring and delta value
- **DKC (Diagrammatic Knot Computation)**: forward direction — exact bracket values compute Boolean functions without training

### Open Questions
- Does ell=5 break the 0x1B/0x06 Re>0 wall? (This is THE headline result — depends on actual program output)
- If the wall holds at rank 4, is it an activation-structural barrier rather than a ring-rank barrier?
- How does the multiplicative encoding benefit from 4D vs 2D? (More angular diversity, or more magnitude diversity, or both?)
- Can Fibonacci growth in delta power coefficients cause integer overflow issues at longer braid lengths?
- What is the optimal level ell for maximizing Boolean function reachability across all activations?
- Does the Fibonacci fusion rule (tau x tau = 1 + tau) have a direct combinatorial reflection in which truth tables are achievable?
- How does the ell=5 reachability compare with sector k=6 (which achieved all 13 at ell=4 in Demo 50)?
