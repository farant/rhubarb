## Demo 98: 3-Strand DKC Proof of Concept — Readout Bottleneck Discovery

- **Status**: COMPLETE (RESULT)
- **File**: `demo_98_3strand_dkc/main.c` (~1417 lines)
- **Tests**: 10 pass, 0 fail (Phases 1-5)
- **Depends on**: Demo 35 (TL infrastructure, Cyc8 arithmetic, entanglement vanishing, axiality theorem), Demo 67 (Hopf phase inert — wrong readout kills computation), Demo 93 (circuit complexity hierarchy, 1wpi framework), Demo 97 (orthogonal directions = perfection, quaternion readout structure)
- **Feeds into**: Demo 99b (column-vector readout on irreducible Delta_1 module — LANDMARK, first-ever 3-strand XOR), activation design for Cyc8^5 partitioning, multiple trace readout (Tr(M), Tr(M^2), Tr(M^3)), Fran's dimensional thesis testing

### Headline
3-strand algebra is rich (infinite group, 2048 entries at cap, 98.5% interleaving, entanglement vanishing confirmed, column-4 has PERFECT diversity) but the trace readout is catastrophically lossy — ZERO XOR6 at any k_sec. The readout, not the algebra, is the research frontier for multi-strand DKC.

### Key Results

- **Phase 1 — 3-Strand BFS Catalog (2 tests)**: Built via TL_3 matrix representation (5x5 over Z[zeta_8], from Demo 35 infrastructure). BFS with 4 generators (sigma1+, sigma1-, sigma2+, sigma2-) reaches 2048-entry cap in 8 rounds. Growth: 1, 4, 12, 30, 68, 148, 314, 656, 815 (hit cap). Group is INFINITE at zeta_8 (unlike 2-strand's 24-element binary octahedral). Interleaving (uses both sigma1 and sigma2): 2017/2048 (98.5%). Non-interleaving: 31 entries, ALL with zero bracket (entanglement vanishing: 100%). Zero-bracket entries: 113/2048 (5.5%). Writhe range: [-8, 8].
- **Phase 2 — Trace Readout (1 test)**: 105 distinct trace values from 2048 matrices (5.1% retention). Traces are AXIAL (single nonzero Cyc8 component). Trace collapses 25 Cyc8 entries (100 integers) to 1 Cyc8 (4 integers) = 96% information loss.
- **Phase 2b — Column Vector Diversity (1 test)**: Column-0 through column-3 each yield ~1166-1167 distinct vectors (56.9-57.0% retention, 20 integer DOFs each). Column 4 is LOSSLESS: 2048 distinct vectors from 2048 matrices (100% retention). This is 11-20x more diversity than trace.
- **Phase 3 — 3-Strand XOR Capacity (1 test)**: Zero XOR6 at k=4,6,8,10,12,16,20,24,32. Zero even restricting to 1935 non-zero-bracket entries. The readout, not the algebra, is the bottleneck.
- **Phase 4 — Matched Comparison at size 24 (1 test)**: 3-strand XOR6 = 0, 2-strand XOR6 = 1109 (best at k=20). 2-strand wins completely — but this is a readout problem, not an algebraic one. 2-strand uses Voronoi + sector activation (all 4 quaternion DOFs); 3-strand trace readout collapses to 1D.
- **Phase 5 — Interleaving Analysis (1 test)**: Neither interleaving (2017 entries) nor non-interleaving (31) produces XOR6. The trace readout fails uniformly across all subsets.

### Theorems/Conjectures

- **Readout Bottleneck Thesis (DEMONSTRATED)**: Multi-strand TL algebras contain MORE computational structure than 2-strand (infinite group, higher diversity, richer interleaving), but the standard trace readout destroys this information. The 2-strand pipeline succeeds because quaternions ARE the readout — SU(2) is isomorphic to S^3 and the Voronoi+sector activation uses all 4 real DOFs. For 3-strand, trace collapses 20 integers (column vector) to 4 integers (trace), losing 96% of information.
- **Entanglement Vanishing at Scale (CONFIRMED)**: All 31 non-interleaving entries (using only sigma1 or only sigma2) have zero bracket, confirming Demo 35's result at 2048-entry scale. Interleaving (using both generators) is necessary for non-trivial bracket values.
- **Axiality Constraint (CONFIRMED)**: Demo 35's axiality theorem (bracket values have single nonzero Cyc8 component) explains why both trace AND bracket readouts fail for 3-strand: axial = effectively 1D information, insufficient for XOR separation.
- **Column-4 Losslessness (NEW FINDING)**: Column 4 of the TL_3 representation preserves 100% of algebraic diversity (2048 distinct vectors). This directly seeds the activation design question: how to partition the 20-integer Cyc8^5 space into cells that enable XOR separation.

### Data

**3-strand BFS catalog growth:**

| Round | Entries | New |
|-------|---------|-----|
| 0 | 1 | 1 |
| 1 | 5 | 4 |
| 2 | 17 | 12 |
| 3 | 47 | 30 |
| 4 | 115 | 68 |
| 5 | 263 | 148 |
| 6 | 577 | 314 |
| 7 | 1233 | 656 |
| 8 | 2048 | 815 (hit cap) |

**Readout diversity comparison:**

| Readout | Distinct | Retention | DOFs |
|---------|----------|-----------|------|
| Trace | 105 | 5.1% | 4 integers |
| Column 0 | 1166 | 56.9% | 20 integers |
| Column 1 | 1166 | 56.9% | 20 integers |
| Column 2 | 1167 | 57.0% | 20 integers |
| Column 3 | 1167 | 57.0% | 20 integers |
| Column 4 | 2048 | 100.0% | 20 integers |

**Phase 4 — Matched comparison (size 24):**

| Metric | 3-strand | 2-strand |
|--------|----------|----------|
| XOR6 winners | 0 | 1109 |
| Best k_sec | 0 | 20 |

**Phase 5 — Interleaving analysis:**

| Subset | Count | XOR6 | Best k |
|--------|-------|------|--------|
| Interleaving | 2017 | 0 | 0 |
| Non-interleaving | 31 | 0 | 0 |

**2-strand vs 3-strand structural comparison:**

| Property | 2-strand | 3-strand |
|----------|----------|----------|
| TL dimension | 2x2 (TL_2, C_2=2) | 5x5 (TL_3, C_3=5) |
| Group order | 24 (finite, binary octahedral) | Infinite (at zeta_8) |
| Generators | 2 (sigma1+, sigma1-) | 4 (sigma1+/-, sigma2+/-) |
| Interleaving | N/A (1 crossing) | 98.5% (2017/2048) |
| Entanglement vanishing | N/A | 100% (31/31 non-interleaving = zero bracket) |
| Readout DOFs (trace) | 4 integers (Cyc8 = sufficient) | 4 integers (Cyc8 = catastrophically lossy) |
| Readout DOFs (quaternion) | 4 reals (full SU(2)) | N/A |
| Readout DOFs (column) | N/A | 20 integers (Cyc8^5 = lossless at col 4) |
| Fran's dim formula | dim=4 (2*2+0) | dim=6 (2*3+0) |
| XOR6 capacity | 1109 (at k=20, size 24) | 0 (trace readout) |

### Code Assets

- **`Cyc8` (Z[zeta_8] cyclotomic arithmetic)**: Exact integer type with basis {1, zeta_8, zeta_8^2, zeta_8^3}, zeta_8^4 = -1. Operations: `cyc8_make`, `cyc8_zero`, `cyc8_one`, `cyc8_add`, `cyc8_neg`, `cyc8_mul`, `cyc8_eq`, `cyc8_is_zero`, `cyc8_conj`, `cyc8_to_double`. Reused from Demo 35.
- **`TLMat` (5x5 TL_3 matrix)**: Matrix over Cyc8 for the 3-strand Temperley-Lieb representation (C_3=5 basis elements). Operations: `tlm_zero`, `tlm_identity`, `tlm_add`, `tlm_neg`, `tlm_scale`, `tlm_mul`, `tlm_eq`, `tlm_trace`.
- **`PMatch` / diagram composition**: Planar matching representation for TL_3 (6 boundary points). `enumerate_basis()` builds all 5 non-crossing matchings. `compose_diagrams()` computes diagram multiplication with closed-loop counting. `build_gen_matrix()` constructs TL generator matrices e_1, e_2.
- **`build_braid_generators()`**: Constructs 4 braid generators from A = -zeta_8: sigma_i = A*I + A^{-1}*e_i, sigma_i^{-1} = A^{-1}*I + A*e_i. Verifies sigma*sigma^{-1} = I and braid relation sigma1*sigma2*sigma1 = sigma2*sigma1*sigma2.
- **`build_3strand_catalog()`**: BFS closure over 4 generators with bracket computation, depth tracking, writhe tracking, and interleaving bitmask (bit0=uses_sigma1, bit1=uses_sigma2). Caps at MAX_3CAT=2048.
- **`Quat` / `build_2strand_catalog()`**: Quaternionic 2-strand catalog (24 entries, binary octahedral group) for matched comparison. BFS closure with mod-sign identification.
- **`phase_cell_cyc8()`**: Trace-based activation for 3-strand: converts Cyc8 trace to complex via `cyc8_to_double`, uses angle-based sector assignment. k_sec sectors + 1 degenerate cell.
- **`combined_cell_2s()` / `vor_cell_2s()` / `build_2strand_dirs()`**: 1wpi Voronoi+sector activation for 2-strand quaternionic readout. Extracts S2 directions, assigns Voronoi cells, combines with angular sectors. Shared with D93-97.
- **`test_xor_3strand()` / `test_xor_2strand()`**: Exhaustive 1wpi XOR separability tests. 3-strand: matrix sum then trace then phase_cell. 2-strand: quaternion sum then combined_cell. Both use touched-cell optimization for fast cleanup.
- **`count_xor_bf()` / `search_xor()`**: Brute-force XOR6 search over all 3-element subsets, with multi-k_sec sweep (k=4,6,8,10,12,16,20,24,32).

### Connection to Earlier Demos

| Demo | Finding | Relevance to D98 |
|------|---------|-------------------|
| D35 | Entanglement vanishing | CONFIRMED at scale: 31/31 non-interleaving have zero bracket |
| D35 | Axiality theorem | Explains why trace/bracket readouts fail: axial = 1D information |
| D67 | Hopf phase inert | Analogous lesson: wrong readout kills computation even when data is rich |
| D97 | Orthogonal directions = perfection | 2-strand works because quaternion components map to orthogonal axes. 3-strand needs analogous structure in Cyc8^5 |

### Literature Touched

- **Temperley-Lieb algebra TL_3**: 5-dimensional representation (C_3 = Catalan number = 5 non-crossing matchings on 6 boundary points). Standard construction: planar matching composition with closed-loop counting. D98 builds the full representation machinery from scratch.
- **Kauffman bracket at zeta_8**: 3-strand bracket values are axial in Z[zeta_8] (single nonzero component), extending D35's axiality theorem from 2-strand to 3-strand. The axiality constraint is structural, not size-dependent.
- **Braid group B_3**: Infinite group (unlike B_2 which maps to finite subgroups of SU(2)). At the zeta_8 parameter, the TL_3 representation yields an infinite image — 2048 distinct matrices reached without closure. The braid relation sigma1*sigma2*sigma1 = sigma2*sigma1*sigma2 is verified computationally.
- **Reservoir computing (Maass et al. 2002)**: D98 demonstrates the readout bottleneck in reservoir computing terms: the reservoir (3-strand BFS catalog) has high separation capacity (column-4 is lossless), but the linear readout (trace) destroys the information. This motivates D99b's approach of working on the irreducible module directly.

### Open Questions

- **Column-vector activation design**: Column 4 preserves 100% diversity (2048 distinct 20-integer vectors). How to partition Cyc8^5 into cells for XOR separation? This is the key design question for multi-strand DKC.
- **Multiple trace readout**: Using Tr(M), Tr(M^2), Tr(M^3) as characteristic polynomial coefficients could recover more information than single trace. 3 traces x 4 integers = 12 DOFs vs 4 DOFs for single trace.
- **Irreducible module approach**: The 5x5 representation is reducible (TL_3 decomposes into standard modules). Working on irreducible pieces may avoid the lossy trace problem — this is exactly what D99b pursues with the 2x2 Delta_1 module, achieving first-ever 3-strand XOR.
- **Dimensional thesis**: Fran's formula gives dim=6 for 3-strand vs dim=4 for 2-strand, predicting higher capacity. Cannot test until readout problem is solved. The algebra supports more complexity; the question is whether that complexity is computationally accessible.
