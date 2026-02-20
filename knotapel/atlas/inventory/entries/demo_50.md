## Demo 50: Activation Function Zoo

- **Status**: COMPLETE
- **File**: `demo_50_activation_zoo/main.c` (~1094 lines)
- **Tests**: ~6 checks (4 parts: A=1 k=2 impossibility, B=2 k=2 confirm + k=8 parity, C=3 parity-min-k + Aizenberg catalog + Aizenberg parity, D=0 analysis only); 4/6 pass, 2 informative failures (Aizenberg weights not in catalog, Aizenberg computes wrong function under j mod 2)
- **Depends on**: Demo 48 (forward DKC zoo, 100M quartets, XNOR3 zero solutions), Demo 49 (neglecton weights add no new angles)
- **Feeds into**: Understanding that the DKC parity wall is in the activation function, not the lattice; non-semisimplicity reinterpretation; future multi-input parity investigations

### Headline
Proves the DKC parity wall was in the activation function, not the weight lattice: replacing split-sigmoid with k-sector MVN classification makes parity reachable at k=6 with 906 solutions from the same 100-value Z[zeta_8] bracket catalog that Demo 48 exhaustively searched, revealing a four-tier hierarchy of activation complexity across all 13 NPN classes.

### Key Results
- **Part A (k=2 impossibility)**: Exhaustive search of 100^3 = 1M triples confirms zero parity solutions under half-plane classification; computational verification of the complex Minsky-Papert theorem
- **Part B (k-sector search, k=2..8)**: For each k, all 1M triples searched with k-sector MVN (sector j mod 2 = output); parity first appears at k=6 (906 solutions), persists at k=7 (756) and k=8 (96); all 13 NPN classes reachable by k=6
- **Part C (minimum sector analysis)**: Four-tier hierarchy: k=2 (5 classes: AND3', AND2', ~A~(BC), BUF, MAJ'), k=3 (+1: ~A(B^C)), k=4 (+6: EXACT1, ISOLATE, 3v-0x19, 3v-0x1B, A^(B|C), XOR2), k=6 (+1: XNOR3 = parity); Aizenberg's construction (w1=zeta_8, w2=i, w3=-1) verified: zeta_8 is NOT in the bracket catalog (only -zeta_8 is), and under k=8 with j mod 2 mapping it computes 3v-0x19, not parity
- **Part D (parity solution anatomy)**: 41/100 catalog values participate; all 906 solutions use three distinct weights; weights confined to octants {2, 4, 5, 7} (class-1 under k=6 sector mapping); every solution places one weight per odd sector (triskelion pattern at 120° intervals); ~151 distinct unordered triples; non-monotonic solution count: 906@k=6 > 756@k=7 > 96@k=8 anti-correlated with gcd(k,8)

### Theorems/Conjectures
- **Parity wall is in the activation, not the lattice**: The standard 100-value bracket catalog contains parity-capable weights; split-sigmoid destroys angular information needed for parity; k-sector MVN reads angles directly
- **Four-tier hierarchy**: 13 NPN classes partition into 4 tiers by minimum sector count {k=2, k=3, k=4, k=6}, with parity uniquely isolated at the highest tier
- **Triskelion principle**: For 3-input parity, k=6 is minimum because it provides exactly 3 odd sectors (at 120° intervals), matching the 3 inputs; each input requires its own odd sector (single-input constraint forces all weights into class-1 octants)
- **Incommensurability hypothesis**: Parity solutions anti-correlate with gcd(k,8) — sector boundaries that misalign with the lattice's pi/4 angular structure break the symmetry that blocks parity; k=6 (gcd=2) optimal, k=8 (gcd=8, perfect alignment) minimal (96 solutions)
- **Class-1 octant constraint**: Parity's truth table logically forces all three weights into octants {2, 4, 5, 7} (the class-1 angles under k=6); this is a necessary condition derivable from f(1,0,0) = f(0,1,0) = f(0,0,1) = 1

### Data
- 13 NPN classes × 7 k-values = full reachability heatmap
- Per-class minimum sector count with solution counts at min k (range: 127,164 for BUF at k=2 down to 906 for XNOR3 at k=6)
- Parity solution counts by k: 0, 0, 0, 0, 906, 756, 96 for k=2..8
- gcd(k,8) vs parity solutions table
- 41/100 catalog values in parity solutions, top 20 by frequency (|z| range 6-17)
- Octant distribution: 906 in octant 2, 270 in octant 4, 636 in octant 5, 906 in octant 7
- Two octant-set combinations only: {2,5,7} (636 solutions, 70.2%) and {2,4,7} (270 solutions, 29.8%)
- Aizenberg construction trace: per-input z values, angles, sectors, resulting truth table 0x58 (3v-0x19)
- k=6 sector-class map for all 8 powers of zeta_8

### Code Assets
- `Cyc8` struct: exact cyclotomic integer type Z[zeta_8] with basis {1, zeta_8, zeta_8^2, zeta_8^3} where zeta_8^4 = -1
- `cyc8_make()`, `cyc8_zero()`, `cyc8_one()`, `cyc8_add()`, `cyc8_mul()`, `cyc8_eq()`, `cyc8_conj()`, `cyc8_pow_int()`, `cyc8_to_cx()` — full exact cyclotomic arithmetic
- `build_catalog()` — exact bracket catalog builder (standard, loops=1 only); enumerates 2-3 strand braids length 1-8, stores both exact Cyc8 and floating-point Cx representations
- `sector_classify(z, k)` — k-sector MVN classification: angle → sector number → j mod 2 binary output
- `search_for_k(k)` — exhaustive triple search with precomputed w1+w2 sum optimization; tracks per-NPN-class solution counts, truth table histogram, and parity example triples
- `NPNClass` struct, `npn_init()`, `npn_build_classes()`, `npn_class_index()`, `npn_transform()` — NPN classification machinery
- `Triple` struct — stores catalog indices of parity solution triples for post-hoc analysis
- Union-find (`uf_init`, `uf_find`, `uf_union`) and `braid_loops()` for state-sum loop counting
- `bracket_cl()` equivalent via exact Cyc8 arithmetic (loops=1 standard bracket)

### Literature Touched
- Aizenberg's multi-valued neuron (MVN) and k-valued logic — parity construction with k=2^n sectors
- Complex Minsky-Papert theorem — k=2 half-plane cannot compute parity regardless of weights
- NPN equivalence classes as computational complexity hierarchy
- Incommensurability of lattice symmetry (Z[zeta_8], 8-fold) with sector geometry (k-fold)
- Non-semisimplicity of TL_n at delta=0 — reinterpreted: radical limits operations, not values
- Triskelion geometry — 3-fold rotational symmetry in parity weight placement

### Open Questions
- Does the triskelion principle generalize: does n-input parity require k=2n sectors (n odd sectors for n inputs)?
- What is the algebraic structure of the 151 distinct unordered parity triples — do they form a lattice or group?
- Can the incommensurability hypothesis be made rigorous as a theorem about gcd(k,8) and NPN reachability?
- Does the non-monotonicity (906 > 756 > 96) continue: what are the parity solutions for k=9..16?
- How does the four-tier structure change with 4+ inputs and the 222 NPN classes?
