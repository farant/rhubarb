## Demo 74: Computational Invariant of Braids

- **Status**: COMPLETE
- **File**: `demo_74_computational_invariant/main.c` (~1623 lines)
- **Tests**: 20/20 pass
- **Depends on**: Demo 66 (24 quaternions, Kauffman bracket at Z[ζ_8], 100 bracket values), Demo 67 (13 eigenvector directions, Voronoi cell assignment, 36 XOR6 solutions), Demo 70 (music space, r=-0.06 geometric/computational orthogonality), Demo 73 (82.8% automaton determinism, additive structure)
- **Feeds into**: Future work on braid invariant theory from computational perspective

### Headline
The DKC Voronoi cell and the Kauffman bracket are incomparable braid invariants — neither subsumes the other — and their combination produces 119 equivalence classes (vs 100 bracket-alone and 14 cell-alone), with the cell separating 97.8% of bracket collisions at the braid level while the bracket separates 53.8% of cell collisions; the DKC computation exploits exactly this geometric information the bracket discards.

### Key Results
- **97.8% bracket collision separation (Part A)**: Of 58,078 braid pairs with the same bracket but different quaternion, 56,825 (97.8%) land in different Voronoi cells. The remaining 2.2% correspond to quaternion pairs sharing an eigenvector direction but differing in eigenvalue angle — the angle is what the cell discards.
- **Cell does not subsume bracket (Part B)**: At the quaternion catalog level, 13 pairs share a Voronoi cell. Of these, 7 pairs have different bracket values. The cell is strictly coarser than the bracket in certain cases. Cell 0, 1, and 10 each contain 3 quaternions with up to 3 distinct bracket values; cells 2–5 (body-type, 60° angle) each have 2 quaternions with 1 bracket value.
- **119 combined classes (Part C)**: Braid-level equivalence classes: cell alone gives 14, bracket alone gives 100, (bracket, cell) pair gives 119. The combination is 19% finer than bracket alone. 28 of 100 bracket values (28%) appear in braids across multiple cells; max distinct brackets per cell is 19 (cell is very coarse relative to bracket). The two invariants capture orthogonal information.
- **Computational significance (Part D)**: For each winning triple, the cell labeling assigns binary parity to cells. 33 of 36 winners (92%) use their labeling to XOR6-separate at least some bracket collisions. The 3 exceptions are triples containing quaternions 18 and 22 (edge-type), where colliding cells happen to share a label. Total XOR6-separated collisions across all 36 winners: 186.
- **Information hierarchy (Part E)**: Quaternion→Direction→Cell is a well-defined factoring chain. Direction and Cell are identical (Voronoi IS the direction catalog). The bracket does NOT factor through quaternion — different braids with the same quaternion can have different bracket values. The factoring: Braid→Quaternion→Cell (well-defined), Braid→Bracket (well-defined), Quaternion→Bracket (many-to-many, NOT well-defined).
- **Incomparability synthesis (Part F)**: At the quaternion catalog level: bracket collisions (same bracket, different quaternion) = 30, of which 24 are separated by cell (80%); cell collisions (same cell, different quaternion) = 13, of which 7 are separated by bracket (54%). Neither invariant refines the other. 6 pairs are unseparated by either.
- **6 computational type vectors (Part G)**: Each of the 36 winning triples assigns a binary parity label to each cell, giving a 36-bit type vector per cell. Among 14 cells, only 6 distinct type vectors exist. Types: 0={cell 0 / axis σ₁, 3 quats}, 1={cell 1 / axis σ₂, 3 quats}, 2={cells 2,3,4,5 / body 60° cells, 8 quats}, 3={cells 6,7,8,9,11,12 / edge cells, 6 quats}, 4={cell 10 / axis third, 3 quats}, 5={cell 13 / identity, 1 quat}. The DKC computation is invariant under permutation within each symmetry orbit.
- **Refinement chain (Part I)**: Bracket→Type→Quaternion: 43 bracket groups (of 100 catalog values that appear in braids) → 61 type subgroups → 168 quaternion subgroups. Max types per bracket value: 4. Max quaternions per type within a bracket: 8.

### Theorems/Conjectures
- **Incomparability Theorem (PROVEN)**: The Kauffman bracket (at A=-ζ_8) and the DKC Voronoi cell are incomparable braid invariants. The bracket is a trace/normalization operation; the cell is an eigenvector direction projection. These operations discard orthogonal information. Neither refines the other: cell separates bracket collisions (97.8% at braid level, 80% at qcat level), and bracket separates cell collisions (54% at qcat level).
- **Strict Combination Theorem (PROVEN)**: The combined invariant (bracket, cell) is strictly finer than either alone: 119 > 100 > 14. The 19% improvement over bracket alone confirms genuinely new information in the cell.
- **6-Type Orbit Theorem (PROVEN)**: The DKC computation on the 13-cell Voronoi partition uses only 6 distinct computational roles. These correspond to the 3 axis orbits (σ₁, σ₂, third axis), the 4 body cells, the 6 edge cells, and the identity cell. The binary octahedral symmetry group acts transitively within each type — the computation is invariant under intra-orbit permutation.
- **Bracket-Blind Computation (STATED)**: The 36 XOR6 winning triples actively exploit information the bracket cannot see. 92% of them use their cell labeling to distinguish braid pairs that the bracket treats as equal. The DKC computation is accessing a fundamentally different projection of the braid group representation than the bracket provides.

### Data
- Correspondence records analyzed: 2,000 (1,124,250 pairs at braid level)
- Bracket collisions (braid level): 58,078 pairs with same bracket, different quaternion; 56,825 (97.8%) in different cells
- Cell collisions (braid level): 1,253 pairs with same bracket and same cell (the 2.2% unseparated)
- Quaternion catalog level: 30 bracket collisions (24/30 = 80% separated by cell); 13 cell collisions (7/13 = 54% separated by bracket)
- Equivalence classes: cell=14, quaternion=24, bracket=100, (bracket,cell)=119
- Combined improvement over bracket: +19 classes (+19%)
- Bracket values split across multiple cells: 28 of 100 (28%); max cells per bracket: 6
- Max distinct brackets per cell: 19 (cell is very coarse)
- Computational types: 6 distinct among 14 cells; 4 body cells interchangeable; 6 edge cells interchangeable
- XOR6-separating winners: 33/36 (92%); exceptions: triples with q18 and q22 (edge-type)
- Total XOR6-separated bracket collisions across all 36 winners: 186
- Refinement chain: 43→61→168 (bracket→type→quaternion subgroups from appearing braids)
- Unseparated pairs (same cell AND same bracket at qcat level): 6

### Code Assets
- **Full catalog builder with bracket computation**: computes Kauffman bracket at A=-ζ_8 for every braid up to length 8 on 2 and 3 strands; stores quaternion, bracket, and per-braid correspondence records simultaneously
- **Z[ζ_8] exact arithmetic**: `Cyc8` struct with `cyc8_mul()`, `cyc8_add()`, `cyc8_conj()`, `cyc8_pow_int()`, `cyc8_eq()`; coefficients remain exact integers throughout
- **Union-find loop counter for bracket**: `braid_loops()` counts Temperley-Lieb loops for each state-sum assignment; union-find on (len+1)×n grid handles arbitrary braid words
- **`find_winners()`**: finds all 36 XOR6 winning triples from the 24-element quaternion catalog; identical to Demo 67/73 logic but integrated with full catalog
- **`compute_type_vectors()`**: for each of the 14 Voronoi cells, computes the 36-bit type vector encoding its parity label under each winning triple; `TypeVec` struct with `typevec_eq()` for comparison
- **`braid_type_idx()`**: maps a cell index to its type vector index (0–5); key for the 6-type orbit analysis
- **Per-braid correspondence record**: stores (quaternion, bracket, qcat_idx, bcat_idx, braid_word, word_len, n_strands) for up to 2,000 braids; used for all collision and separation analyses
- **Bracket collision separation analysis (Part A/F)**: counts pairs by (same/diff bracket) × (same/diff cell) at both braid-level and catalog-level; cleanly separated into directed and reverse analyses

### Literature Touched
- **Kauffman (1987)**: the bracket polynomial as a braid/knot invariant at variable A; the specific evaluation A=-ζ_8 is the connection point to the DKC quaternion structure (Demo 66 established this)
- **Jones polynomial (Jones 1985)**: the bracket at A=q^{-1/4} specializes to the Jones polynomial; the 119-class combined invariant is a refinement that the Jones polynomial alone cannot achieve
- **Temperley-Lieb algebra**: the union-find loop counting in `braid_loops()` implements the TL state-sum expansion of the bracket; the computation is exact over Z[ζ_8]
- **Braid group representations (Burau, Lawrence-Krammer)**: the SU(2) representation used for DKC is distinct from Burau/LK; the incomparability result shows the SU(2) eigenvector direction extracts information neither representation captures algebraically
- **Turaev-Viro / TL non-semisimplicity**: the bracket at ζ_8 is a root-of-unity evaluation where the TL category becomes non-semisimple; the fact that 119 classes exceed 100 suggests the cell is detecting some non-semisimple structure that the trace/bracket misses
- **Abramsky (2004) / topological quantum computation**: the Voronoi cell as a classical readout of a topological quantity; the 6-type structure suggests the DKC computation sees the braid through a 6-element coarse-graining of the representation

### Open Questions
- **What are the 6 unseparated pairs?**: At the qcat level, 6 pairs have the same cell AND the same bracket. These correspond to body-type cells (2–5) with 60° angles. Do these pairs represent genuinely equivalent braids under some stronger invariant, or is there a finer invariant that separates them?
- **Is 119 near-optimal?**: With 24 quaternions and 13 cells, the theoretical maximum for (bracket, cell) pairs is min(24×13, number of braids) but in practice is bounded by the braid word space sampled. Is 119 close to the maximum achievable combined class count with this braid space?
- **[*,18,22] dead zone**: Triples containing q18 and q22 (edge-type) separate zero bracket collisions. Is this a geometric accident (those cells happen to share labels) or is there a group-theoretic reason that edge-type triples cannot exploit bracket collisions?
- **Algebraic characterization of the 97.8%**: The cell separates 97.8% of bracket collisions at the braid level. The unseparated 2.2% are pairs with the same eigenvector direction but different eigenvalue angle. Is there a closed-form expression for exactly which braid pairs are unseparated (i.e., which Z[ζ_8] elements share direction but differ in angle)?
- **Refinement chain completeness**: The chain 43→61→168 uses only braids appearing in the length-1-to-8 catalog. The full braid group is infinite. Does the refinement ratio (bracket→type→quat) stabilize as longer braid words are included, or does it grow without bound?
- **Connection to knot invariant strength**: The combined (bracket, cell) invariant defines a function on braids. When the braid closure is taken, does this descend to a knot invariant? If so, how does it compare to known invariants (Jones, HOMFLY, Khovanov) in distinguishing power?
