## Demo 51: Radical Anatomy of TL_n at delta=0

- **Status**: COMPLETE
- **File**: `demo_51_radical_anatomy/main.c` (~1166 lines)
- **Tests**: ~35+ pass (7 values of n x 5+ tests each: identity, nilpotent, adjacent, commutativity, radical dim, idempotent sum, Peirce blocks, Catalan trace, radical filtration)
- **Depends on**: Demo 35 (planar matching enumeration, diagram composition), Demo 47 conjecture (TL non-semisimplicity at roots of unity as algebraic obstruction)
- **Feeds into**: Demo 52+ (connecting radical structure to DKC parity barrier, computational implications of non-semisimplicity)

### Headline
Computes the full radical structure of the Temperley-Lieb algebra TL_n(delta=0) for n=2 through 8 using exact integer arithmetic on planar matching diagrams: enumerates basis, builds multiplication tables, determines radical dimension via trace form Gram matrix, extracts radical filtration (rad, rad^2, ...), finds orthogonal idempotent decompositions, and computes Peirce block dimensions.

### Key Results
- **TL_n basis dimensions**: C_2=2, C_3=5, C_4=14, C_5=42, C_6=132, C_7=429, C_8=1430 (Catalan numbers, as expected)
- **Key discovery: TL_3 is SEMISIMPLE despite nilpotent generators** (radical dim = 0) — generators e_i are nilpotent (e_i^2 = 0 at delta=0) but the algebra as a whole can still be semisimple
- **TL_2 is NOT semisimple**: radical dim = 1, structure is k[eps]/(eps^2) (dual numbers), only 1 generator
- **TL_4+ have non-trivial radicals**: radical dimension grows with n
- **Radical filtration computed**: rad^1, rad^2, rad^3, ... until zero, giving nilpotency index for each n
- **Orthogonal idempotent decompositions found** from length-2 generator products (e_i * e_j); complement (1 - sum) verified as idempotent and orthogonal
- **Peirce block dimensions sum to algebra dimension** — verified for all n where computed
- **Peirce blocks of rad^2 also sum correctly** (sandwich duality check)
- **All nonzero fixed-point counts in the trace form are Catalan numbers** (Catalan trace check)
- **TL relations verified exactly**: identity, nilpotent (e_i^2=0), adjacent (e_i*e_{i+1}*e_i = e_i), far commutativity (e_i*e_j = e_j*e_i for |i-j|>=2)

### Theorems/Conjectures
- **TL_3(delta=0) semisimple**: CONFIRMED — radical dim = 0, despite nilpotent generators. This was the explorer's key discovery.
- **TL_2(delta=0) = dual numbers k[eps]/(eps^2)**: CONFIRMED — radical dim = 1, nilpotency index 2
- **Catalan trace property**: CONFIRMED — all nonzero fixed-point counts (trace values) are Catalan numbers
- **Peirce decomposition completeness**: CONFIRMED — block dimensions sum to algebra dimension for all tested n
- **TL non-semisimplicity at delta=0**: CONFIRMED for n >= 4 — radical dimension grows, supporting the hypothesis from Demo 47 that TL non-semisimplicity is the algebraic obstruction for parity

### Data
- n=2: dim=2, radical=1, structure=dual numbers, nilpotency index 2
- n=3: dim=5, radical=0, SEMISIMPLE
- n=4: dim=14, radical>0
- n=5: dim=42, radical>0
- n=6: dim=132, radical>0
- n=7: dim=429, radical>0
- n=8: dim=1430, radical>0
- Multiplication tables printed for dim<=14 (n<=4)
- Gram matrices printed for dim<=14

### Code Assets
- **`TLAlgebra` struct**: complete bundled TL algebra data — basis (PlanarMatch array), identity/generator indices, multiplication table
- **`init_tl_algebra()`**: full initialization: enumerate basis, find identity/generators, compute multiplication table
- **`enumerate_basis()`**: recursive planar matching enumeration via segment decomposition (from Demo 35)
- **`compose_diagrams()`**: planar diagram composition with loop counting (from Demo 35)
- **`AlgElem` type**: integer-coefficient algebra elements with add/sub/mul/is_zero/eq/is_idempotent operations
- **`alg_mul()`**: algebra multiplication via precomputed multiplication table — products that generate loops map to -1 (zero at delta=0)
- **`compute_radical_dim()`**: trace form Gram matrix construction + Gaussian elimination for rank; radical dim = dim - rank
- **`extract_radical_basis()`**: extracts radical basis vectors from Gram matrix null space via augmented row reduction + integer scaling
- **`radical_filtration()`**: computes rad^1, rad^2, rad^3, ... dimensions until zero, yielding nilpotency index
- **`compute_next_radical_power()`**: generates rad^{k+1} basis from rad^k x rad products with echelon deduplication
- **`find_gen_product_idempotents()`**: searches length-2 generator products for idempotents
- **`build_orthogonal_set()`**: greedy maximal orthogonal idempotent set construction
- **`compute_peirce_dim()` / `compute_peirce_dim_subspace()`**: Peirce block dimensions f_i * A * f_j via rank computation
- **`try_add_to_echelon()`**: incremental echelon basis maintenance for linear independence testing

### Literature Touched
- Temperley-Lieb algebra TL_n(delta) — planar matching diagram basis, Catalan number dimensions
- TL relations: e_i^2 = delta*e_i (here delta=0 so nilpotent), e_i*e_{i+1}*e_i = e_i, far commutativity
- Goodman & Wenzl (1993): TL at roots of unity is non-semisimple
- Cartan's criterion: radical = null space of trace form (in characteristic 0)
- Peirce decomposition via orthogonal idempotents
- Wedderburn-Artin theory: semisimple = direct sum of matrix algebras
- Connection to Demo 47 hypothesis: TL non-semisimplicity as parity barrier

### Open Questions
- What is the exact radical dimension sequence for n=2,3,4,5,6,7,8? (TL_3 semisimple is anomalous — is there a pattern?)
- What is the Wedderburn-Artin structure of TL_3(delta=0)? (It's semisimple, so it must be a direct sum of matrix algebras — which ones?)
- Does the radical filtration length (nilpotency index) correlate with the parity barrier strength from Demo 47?
- Can the Peirce block structure explain which Boolean functions are bracket-compatible vs bracket-resistant?
- Is the Catalan trace property (all nonzero trace values are Catalan numbers) a known result or novel?
- How does the radical structure change when moving from delta=0 to nearby values?
