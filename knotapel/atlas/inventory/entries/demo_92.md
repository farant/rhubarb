## Demo 92: Function Scaling — The Parity-Lock Theorem

- **Status**: COMPLETE
- **File**: `demo_92_function_scaling/main.c` (~1399 lines)
- **Tests**: 13/16 pass (3 informative fails — AND/OR/MAJ are impossible under the encoding, so tests expecting them to be achievable correctly fail; these confirm the theorem rather than indicating bugs)
- **Depends on**: Demo 82 (crossing depth law, algebraic coherence), Demo 89 (depth law mechanism, max_xor ~ depth+6), Demo 91 (activation bottleneck, combined_cell framework), Demo 48/50 (forward DKC, NPN class zoo — the 1wpi encoding contrast)
- **Feeds into**: Demo 93 (complement-blindness theorem under 1wpi encoding), encoding design (targeting specific Boolean functions), activation symmetry-breaking research (phase-sensitive activations)

### Headline
The +/-q input encoding is structurally parity-locked — only XOR/XNOR are computable. AND/OR/MAJ/THRESHOLD = 0 winners at ALL depths.

### Key Results

- **Phase 1 — Function comparison across depths (zeta_12 catalog, depths 0-8)**: XOR scales from 8 to 3866 winners (6-input) and 0 to 3080 (8-input) as depth increases. AND, OR, MAJ, THRESHOLD-2 produce ZERO winners at every depth for both 6-input and 8-input arities. The depth law (max_xor ~ depth+6) applies only to parity.
- **Phase 1b — Truth table census (10,000 random 3-weight triples)**: 83.7% of random triples are XOR6-separable. AND, OR, MAJ separability = 0.0% each. This is not a matter of rarity — non-parity functions are structurally impossible.
- **Phase 1c — Mask collision check**: 76 exact quaternion-sum collisions among C(64,2)=2016 mask pairs. ALL 76 are same-parity (0 cross-parity collisions). Same-parity collisions preserve XOR but break non-XOR functions that need to distinguish same-parity masks.
- **Phase 1d — Equivalence class analysis (27 classes for k=3 weights)**: The encoding creates 3^k equivalence classes (per-weight effective states {-q, 0, +q}). XOR is constant on all 27 classes (0 conflicts). AND has 1/27 class conflicts, OR has 1/27, MAJ has 19/27. The critical class is (0,0,0): contains both mask 000000 and mask 111111 (both produce sum=0), but AND(000000)=0 and AND(111111)=1 — impossible to separate. 11 distinct cells for 27 classes at k_sec=12.
- **Phase 1e — Sign-flip symmetry**: All 13 sign-flip pairs (c vs -c) map to identical sector, Voronoi cell, combined cell, and parity. This is provable from the combined_cell code: normalization by |qa| maps S and -S identically. Multi-triple analysis: XOR winners have 11-13 distinct cells (of 27 classes), non-winners have 6-11.

### Theorems/Conjectures

- **Parity-Lock Theorem (PROVED + computationally verified)**: 6-part theorem. (a) The (0,0) equiv (1,1) collision means masks 0...0 and 1...1 always produce identical sums (both zero). (b) Any f where f(0...0) != f(1...1) is impossible under ANY activation. (c) AND, OR, MAJ all have f(0...0) != f(1...1) — all impossible. (d) XOR(0...0) = XOR(1...1) = 0 for even-length inputs — collision harmless. (e) 3^k equivalence classes, parity constant on each (swapping (0,0)<->(1,1) changes popcount by +/-2, preserving parity). (f) XOR/XNOR are the only standard Boolean functions constant on all classes. Full proof in `proofs/parity-lock-theorem.md`.
- **Sign-Flip Symmetry (PROVED computationally)**: combined_cell(S) = combined_cell(-S) for all sums S. The 3^k equivalence classes collapse to at most (3^k + 1)/2 distinguishable groups under the activation. For k=3: 27 classes -> at most 14 groups. Observed: 11 distinct cells.
- **Encoding Concentrates on Hardest Function (DEMONSTRATED)**: The +/-q paired encoding channels all computational power into parity — the single Boolean function outside AC^0 (the class of constant-depth polynomial-size circuits). This is not a limitation to be overcome but a structural feature of the encoding.

### Data

- Catalog: zeta_12, 4096 entries, 9 BFS rounds
- Phase 1 XOR winners (6-input): depth 0 = 8, depth 8 = 3866
- Phase 1 XOR winners (8-input): depth 0 = 0, depth 8 = 3080
- Phase 1 AND/OR/MAJ/THR2 winners: 0 at all depths, both arities
- Phase 1b XOR6-separable: 8370/10000 (83.7%)
- Phase 1b AND/OR/MAJ-separable: 0/10000 each (0.0%)
- Phase 1c mask collisions: 76 total, 76 same-parity, 0 cross-parity
- Phase 1d class conflicts: AND 1/27, OR 1/27, MAJ 19/27, XOR 0/27
- Phase 1d distinct cells: 11 of 27 classes at k_sec=12
- Phase 1e sign-flip pairs: 13/13 same sector, same Voronoi, same cell, same parity
- Phase 1e multi-triple distinct cells: winners 11-13, non-winners 6-11
- K-ladder: K_SHORT = {6, 12, 24} (short ladder for tractable runtime)
- bf_max limits: 30 (6-input), 20 (8-input)

### Code Assets

- **`test_bool_func(indices, n_weights, k_sec, truth_table)`**: generalized Boolean function test — checks whether a weight tuple separates class-0 from class-1 masks in the combined_cell activation space. Returns 1 if separable, 0 if not. Reusable for any truth table.
- **`count_bool_winners(n_weights, bf_limit, truth_table)`**: counts weight tuples that achieve a given Boolean function across the short k-ladder {6,12,24}. Supports 3/4/5-weight tuples.
- **Truth table generators**: `make_xor_tt`, `make_and_tt`, `make_or_tt`, `make_maj_tt`, `make_threshold_tt` — produce truth tables for standard Boolean functions given n_inputs. Reusable.
- **`check_separability(cell_labels, truth_table, n_masks)`**: checks whether a cell-labeling can separate a truth table (all masks in each cell have same truth value). Used in Phase 1b census.
- **`phase1b_truth_table_census()`**: Monte Carlo census — 10,000 random 3-weight triples tested for XOR/XNOR/AND/OR/MAJ separability. Stores distinct cell-label patterns.
- **`phase1c_mask_collisions()`**: exhaustive pairwise collision check (C(64,2) pairs), classifies collisions by parity agreement. Confirms same-parity-only collision structure.
- **`phase1d_class_analysis()`**: enumerates all 27 equivalence classes for 3 weights, checks each standard Boolean function for constancy on each class, prints conflict counts and cell assignments.
- **`phase1e_sign_flip_symmetry()`**: decomposes combined_cell into sector_of and voronoi_of, checks all 13 sign-flip pairs for agreement, plus multi-triple cell count comparison between winners and non-winners.
- **`sector_of()` / `voronoi_of()`**: decomposition of the combined_cell activation into its angular (S^1) and directional (S^2) components. Useful for future activation analysis.
- **`build_closure(verbose)`**: SU(2) group closure with BFS depth tracking and round-by-round reporting. Shared infrastructure from earlier demos.
- **`save_catalog()` / `load_up_to_depth(max_d)`**: snapshot/filter pattern enabling cumulative depth experiments without rebuilding the catalog.
- **LCG RNG**: simple `rng_uniform()` for Monte Carlo sampling (deterministic seed 42).

### Literature Touched

- **Circuit complexity (AC^0)**: Parity is the canonical function outside AC^0 — it cannot be computed by constant-depth polynomial-size circuits of AND/OR/NOT gates. The +/-q encoding naturally selects for the hardest Boolean function, concentrating computational power on parity. Furst-Saxe-Sipser (1984) / Hastad (1987) parity-not-in-AC^0 result is the theoretical backdrop.
- **Habiro (2002) / Z[zeta_8] bracket**: the algebraic substrate underlying the weight catalog; Demo 92 reveals that the encoding structure on top of this substrate is parity-selective.
- **Aizenberg (2008) / MVN neurons**: the k-sector activation framework (k_sec parameter in combined_cell). Demo 92's Phase 1d and 1e decompose the activation into angular sectors (S^1) and Voronoi cells (S^2).
- **D48/D50 (forward DKC NPN classes)**: the critical contrast — 1-weight-per-input encoding with split-sigmoid/MVN activation computes all 13 NPN classes including parity. The +/-q encoding's parity lock is encoding-specific, not a fundamental limit of the DKC framework. This contrast motivated Demo 93.
- **D89-D91 (depth law)**: max_xor ~ depth+6 describes the capacity curve for the +/-q encoding's ONLY achievable function family (parity). Demo 92 reinterprets the depth law as parity-specific, not function-general.

### Open Questions

- **Encoding design**: Can we construct encodings that target specific Boolean functions other than parity? The parity-lock theorem identifies the structural mechanism (per-weight pair cancellation); can this be exploited to engineer encodings for AND, OR, or MAJ?
- **Optimality**: Is the +/-q encoding provably optimal for parity among all 2-bit-per-weight encodings? Among all encodings with the same number of input bits?
- **Phase-sensitive activation**: The sign-flip symmetry (combined_cell(S) = combined_cell(-S)) is identified as a second parity-lock mechanism (exploited in D93). Can a phase-sensitive activation that distinguishes S from -S break the parity lock while preserving the depth law structure?
- **Hybrid encodings**: Can a mixed encoding (some weights +/-q paired, some 1wpi) achieve a richer set of Boolean functions while maintaining some depth-law structure?
- **MAJ conflict count (19/27)**: MAJ has the most class conflicts of any tested function. Is this related to MAJ's computational complexity (MAJ is TC^0-complete, strictly between AC^0 and NC^1)?
