## Demo 63: Angular Anatomy of DKC

- **Status**: COMPLETE
- **File**: `demo_63_angular_anatomy/main.c` (~1957 lines)
- **Tests**: 29/29 pass
- **Depends on**: Demo 29 (Z[zeta_8] exact arithmetic, bracket catalog), Demo 50 (four-tier NPN hierarchy, parity at k=6, 906 solutions), Demo 35 (TL matrix representations), Demo 39 (Markov RT truncation connection)
- **Feeds into**: Paper 1 (publication strengthener — 4-input and 5-input parity results, parity ceiling); future demos on Z[zeta_16] scaling

### Headline
Establishes that the four-tier NPN reachability structure is determined by discrete octant-sector geometry, derives the complete parity scaling law for Z[zeta_8] (ceiling n=5), extends DKC to 4-input (k=8, 96 solutions) and 5-input (k=15, 3020 solutions) parity, and connects the angular structure to oriented matroid theory.

### Key Results
- **Part A — Octant enumeration**: All 100 catalog values are axial (single nonzero Z[zeta_8] component), lying on 8 discrete rays. Octant distribution: {17,8,17,11,11,11,17,8}. Ray distribution: a=28, b=19, c=34, d=19.
- **Part B — Sector-octant mapping**: Table for k=2..8 mapping k-sector lattices to class-1 octant sets. k=6 uniquely produces class-1 = {2,4,5,7} = the parity octants.
- **Part C — Representative weight triples**: Examples from each tier showing the mechanism by which sector count controls reachable NPN classes.
- **Part D — Minimum-k per NPN class**: Reproduces four-tier structure from pure geometry: k=2 (5 classes), k=3 (+1), k=4 (+6), k=6 (+1=parity). Non-monotonic parity: 906@k=6 > 756@k=7 > 96@k=8.
- **Part E — Parity triskelion anatomy at k=6**: All 906 solutions place one weight per odd sector. Only two octant sets: {2,5,7}=636 (70.2%) and {2,4,7}=270 (29.8%). Octants 2 and 7 are mandatory.
- **Part F — Commensurability analysis**: gcd(k,8) controls the available sector-octant structure. k=6 (gcd=2) maximizes parity-compatible configurations; k=8 (gcd=8) minimizes them.
- **Part G — Oriented matroid classification**: 21 distinct OM types among 512 octant triples. Parity requires the ALTERNATING type only (sign sequence -,+,- or +,-,+ with s12=s23=-s13). Uniform OM types achieve 12/13 NPN classes (everything except parity). 0 degenerate OM types achieve parity.
- **Part H — 4-input XOR (0x6996)**: Reachable only at k=8, with 96 solutions. Tetraskelion structure: 4 weights in all 4 odd octants {1,3,5,7}. OM type: 6-sign encoding (-,+,0,0,-,+).
- **Part I — 5-input XOR**: Reachable at k=15 only, with 3020 solutions. Pentaskelion structure. k=15 has 7 class-1 octants (all non-zero). Exhaustive search over k=2..16.
- **Part J — Complete parity scaling at k=15**: n-input XOR tested for n=3..7. Parity ceiling for Z[zeta_8] is n=5. Table: n=3 min_k=6, sol@k=15=23004; n=4 min_k=8, sol@k=15=16108; n=5 min_k=15, sol@k=15=3020; n=6 impossible (constraint wall); n=7 impossible (constraint wall); n>=8 impossible (pigeonhole, only 7 class-1 octants).

### Theorems/Conjectures
- **Theorem (four-tier from geometry)**: The four-tier NPN reachability structure is reproduced from pure octant-sector geometry alone, without any search. PROVEN.
- **Theorem (parity requires alternating OM type)**: Parity is achievable only by weight triples with alternating oriented matroid type (-,+,- or +,-,+). Exhaustive: 0 of 1M non-alternating triples achieve parity. PROVEN.
- **Theorem (k=6 uniqueness)**: k=6 is the unique sector count in k=2..16 that produces the parity octant set {2,4,5,7} as class-1. PROVEN (exhaustive verification).
- **Theorem (parity ceiling n=5)**: The parity ceiling for Z[zeta_8] MVNs is n=5. n>=6 is impossible by constraint-geometric wall (exhaustive search, 0 solutions); n>=8 is impossible by pigeonhole (only 7 class-1 octants available). PROVEN.
- **Conjecture (universal k theorem)**: k=2M-1 for an M-ray lattice. Proof sketch provided; full proof not yet complete. CONJECTURED.
- **Falsified (k=2n triskelion conjecture)**: n=5 parity requires k=15, not k=10. FALSIFIED.

### Data
- Octant distribution over 100 catalog values: {17,8,17,11,11,11,17,8}
- Ray distribution: a=28, b=19, c=34, d=19
- Sector-octant mapping table: k=2,3,4,5,6,7,8,15,16
- Parity scaling law: n=3 → min_k=6 (906 solutions); n=4 → min_k=8 (96 solutions); n=5 → min_k=15 (3020 solutions); n>=6 → impossible
- Solution counts at k=15: n=3: 23004, n=4: 16108, n=5: 3020, n=6: 0, n=7: 0
- Parity triskelion octant sets at k=6: {2,5,7}=636 (70.2%), {2,4,7}=270 (29.8%); octants 2 and 7 mandatory
- Oriented matroid types: 21 distinct types from 512 octant triples; parity in alternating only

### Code Assets
- **`angle_to_octant()`**: classifies Z[zeta_8] lattice points into the 8 discrete octants
- **Sector-octant mapping**: computes class-1 octant sets for arbitrary k by iterating k-sector lattice
- **`cross_sign()`, `om_encode()`, `om_decode()`**: oriented matroid classification for weight triples (sign of all pairwise cross products)
- **`compute_tt4_at_k()`**: 4-input DKC search with 4-level pruning
- **`collect_class1()`**: collects class-1 sector vectors for 5-input DKC search
- **`pj_search()`**: general recursive n-input parity search with 2^n-1 subset constraints per candidate weight vector
- **NPN classification infrastructure**: reused from Demo 50 (all 13 classes, canonical form)

### Literature Touched
- Cover (1965): 104 linearly separable functions of 3 variables. k=2 tier identified as Cover's threshold functions.
- Reiner (2004, arXiv:math/0402206): cyclotomic matroids mu_N. Framework for Z[zeta_8] as mu_8.
- Bjorner et al. (Cambridge): oriented matroid covector lattices; names the sector-classification construction used here.
- Aizenberg (2008): k-sector MVN activation; direct ancestor of the DKC sector count parameter.

### Open Questions
- Why exactly does n=6 fail? Is there a clean algebraic proof for the constraint-geometric wall (as opposed to exhaustive search)?
- Scaling to Z[zeta_N]: for Z[zeta_16] (16 directions), the pigeonhole ceiling rises to n=15. What is the actual constraint ceiling? Is the gap between constraint ceiling and pigeonhole ceiling always 2?
- Non-monotonic solution counts at k=15 (23004 → 16108 → 3020 → 0): is the decrease a theorem, or could counts increase then decrease for larger n in other lattices?
- Oriented matroid generalization: what OM type characterizes n-input parity for n=4 and n=5?
- Non-parity ceiling: are there NPN classes achievable at n=6 inputs that are not parity functions?
