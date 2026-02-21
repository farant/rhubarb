## Demo 73: Eigenvector Automaton — Can DKC Be a Finite Automaton?

- **Status**: COMPLETE
- **File**: `demo_73_eigenvector_automaton/main.c` (~1042 lines)
- **Tests**: 7/7 pass
- **Depends on**: Demo 67 (13-direction S² Voronoi, 36 XOR6 solutions), Demo 71 (bandwidth l=6, DOF argument), Demo 72 (spherical design test, algebraic structure)
- **Feeds into**: Demo 74 (computational invariant, bracket vs cell comparison)

### Headline
DKC is fundamentally additive (weight vector sum, not braid matrix product), and the 13-cell Voronoi partition is almost but not quite a finite automaton state space: 82.8% of cell transitions are deterministic — a structural constant identical across all 36 winning triples — with non-determinism arising entirely from discarded magnitude information.

### Key Results
- **Multiplicative formulation fails entirely (Part B)**: The 6-step braid product σ₁^±1·σ₂^±1·...·σ₂^±1 assigns exactly 8 even-parity and 8 odd-parity inputs to every final cell. Every cell is maximally non-separating. The product cannot compute XOR6 at all — parity is structurally invisible because flipping all bits (changing parity) maps M→M⁻¹, which shares the same eigenvector cell as M.
- **Additive formulation computes XOR6 (Part D)**: Weight sum S_k = S_{k-1} + w_k with winning triple catalog[0,4,19]. Weights map to only 3 distinct cells (0, 1, 10). All 14 final cell states are parity-pure. State space expands from 2 (step 0) to 14 (step 4) distinct cells as weights accumulate.
- **Additive determinism: 82.8% (Part E)**: Of 58 (prev_cell, bit, step) triples in the additive trajectories, 48 are deterministic (same prev_cell + bit always yields same next_cell). 10 are not. Every non-deterministic transition occurs on bit=1 (adding a weight). Bit=0 transitions are trivially deterministic. Cell 0 is the main split point.
- **Cell is not a sufficient statistic (Part E)**: The cell at step k does not predict the final XOR6 output for any k < 5. Only the final step's cell (the complete sum's eigenvector) determines output.
- **Why 82.8% and not 100% (Part E synthesis)**: The Voronoi cell captures eigenvector direction but discards two pieces of information: (1) eigenvalue angle θ, and (2) magnitude. Two quaternion sums with the same eigenvector direction but different magnitude or angle produce different results when a weight is added. Max split factor is always binary (2-way).
- **27 cells needed for 100% direction-level determinism (Part F)**: Distinct partial sums at each step: 2, 3, 6, 9, 18, 27. The 27-cell signed partition (no antipodal identification) achieves 87.1% determinism vs 82.8% for unsigned. Still not 100% — the remaining non-determinism comes from partial sums with different magnitudes but the same eigenvector direction. Only 17 of 27 cells are actually occupied at step 5.
- **82.8% is a universal structural constant (Part G)**: All 36 winning triples have exactly 82.8% determinism (48/58 transitions). Zero variation. This universality follows from the binary octahedral group acting transitively on the 13 directions — the combinatorial structure of cell collisions is identical across all triples.

### Theorems/Conjectures
- **DKC is Additive, Not Multiplicative (PROVEN)**: The multiplicative braid product formulation is structurally incapable of computing XOR6 via the Voronoi cell mechanism. The reason is algebraic: complement-all-bits maps M to its inverse M⁻¹, which shares the same cell (antipodal eigenvectors are identified), but changes parity. The computation requires additive weight sums.
- **82.8% Universal Determinism (PROVEN)**: The determinism fraction 48/58 is a structural constant of the ζ_8 DKC system, not an artifact of triple selection. It follows from the symmetry of the 13-direction partition: all 36 winning triples are related by the binary octahedral group, so the collision structure is identical.
- **Compass Without Odometer (STATED)**: The Voronoi cell captures the compass heading (eigenvector direction) of the partial sum but not the odometer reading (magnitude). This missing magnitude information is the precise source of the 17% non-determinism. A state machine that also tracked magnitude modulo the weight lattice would achieve 100% determinism but would require infinitely many states.
- **Minimum States Conjecture**: The theoretical minimum cell count for a fully deterministic automaton computing XOR6 via DKC at ζ_8 is 27 (max distinct partial sums). Whether a 27-state automaton actually exists (i.e., whether 27 appropriately-chosen cells achieves 100% determinism) is not yet proven — the signed partition with 27 cells reaches only 87.1%.

### Data
- Multiplicative determinism: 81.2% (26/32 transitions); XOR6: NO (every cell is perfectly balanced)
- Additive determinism: 82.8% (48/58 transitions); XOR6: YES
- Signed 27-cell partition determinism: 87.1%; cells occupied at step 5: 17 of 27
- Number of distinct partial sums per step: 2, 3, 6, 9, 18, 27 (step 0–5)
- Cell collisions (same cell, different partial sum history): 1,296 instances across all steps
- Number of winning triples with exactly 82.8% determinism: 36/36 (100%)
- Cell 0 split count: steps 2, 3, 4, 5; Cell 1 split count: steps 4, 5
- Generator cells: σ₁ and σ₁⁻¹ both → cell 0; σ₂ and σ₂⁻¹ both → cell 1
- Even multiplicative steps are 100% deterministic; odd steps split at 67%

### Code Assets
- **`determinism_test()`**: generic automaton determinism checker for any trajectory array; reports per-step and overall fraction, prints split events, works for both multiplicative and additive formulations
- **`determinism_pct()`**: quiet version returns determinism percentage; used in batch loop over all 36 winners (Part G)
- **`compute_mul_trajectories()`**: computes 64×6 cell trajectory table for the multiplicative braid product formulation
- **`compute_add_trajectories()`**: computes 64×6 cell trajectory table for the additive weight sum formulation given a winning triple
- **`vor_cell_signed()`**: signed Voronoi partition (26 directions, no antipodal identification); returns cell in 0..25 with identity pseudo-cell at 26
- **`quat_to_cell_26()`**: maps quaternion to signed cell index; companion to unsigned `quat_to_cell()`
- **Distinct partial sum counter**: counts unique quaternion vectors at each step to determine theoretical minimum state count; O(N²) exact comparison
- **Sufficient statistic tester**: for each cell at each step, checks whether all inputs reaching that cell have a consistent final output (tests if cell is a Markov blanket for the computation)

### Literature Touched
- **Mealy/Moore automata (classical)**: the 13-cell partition is a near-Mealy machine; the gap from 82.8% to 100% identifies exactly what information a state machine must track beyond cell identity
- **Braid group B_3 (Artin 1947)**: the multiplicative vs additive distinction corresponds to using the group element vs the representation matrix sum; the group element loses parity by the M→M⁻¹ argument
- **Nazer & Gastpar (2011) compute-and-forward**: additive structure is the key feature connecting DKC to lattice computation; the present demo confirms the computation is fundamentally additive at the level of trajectory tracking
- **Compressed sensing / sparse recovery**: the 27-state sufficiency argument connects to the sparsity structure of partial sums — only 27 of 64 possible 6-bit sums are distinct, a 2.4× reduction from the maximum

### Open Questions
- **Exact 27-state automaton**: The signed 27-cell partition reaches 87.1% determinism, not 100%. Is there a different 27-partition of S² that achieves 100% determinism? Or does 100% require tracking more than direction (i.e., some magnitude information)?
- **What encodes the missing 17%?**: The non-determinism comes from magnitude loss. Is there a natural algebraic quantity — perhaps the norm of the partial sum modulo the weight lattice — that, when added to the cell as state, achieves 100% determinism with a finite state count?
- **Automaton for other Boolean functions**: XOR6 at ζ_8 gives 82.8% universal determinism. Does XOR5 at ζ_8 (Demo 50) have a different universal determinism constant? Is the constant always a ratio of integers (48/58 = 24/29)?
- **Relationship to the 13=13 theorem**: The 13 cells and the 13 directions are the same partition. The DOF theorem (Demo 71) explains why 13 directions suffice for XOR6. Does the 82.8% automaton constant have an analogous DOF interpretation?
- **Cellular automaton generalization**: The additive DKC trajectory on cells (with non-determinism) resembles a probabilistic cellular automaton on a 13-cell state space. Is there a stochastic matrix formulation that exactly captures the 82.8% determinism?
