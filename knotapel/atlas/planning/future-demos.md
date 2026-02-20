# DKC Future Demos Backlog

Consolidated 2026-02-20. Sources: future-demos/ proposals, synthesis documents, completed demo results.

50 demos completed (D01-D29, D35, D38-D39, D45-D62). Updated 2026-02-20.
13 proposals remain from future-demos/. 5 new demos added from synthesis open questions.

---

## Obsolete Proposals (completed or superseded)

| # | Title | Status |
|---|-------|--------|
| 35 | TL Matrix Representations | DONE (demo_35_tl_matrices/) |
| 38 | Dimension Reduction at Delta=0 | DONE (demo_38_dimension_reduction/) |
| 42 | Cyclotomic Exact Arithmetic | SUPERSEDED -- Z[zeta_8] exact arithmetic built in D29; Z[zeta_16]/Z[zeta_5]/Z[zeta_24] built in D54/D58/D59. The ring tower exists. |
| 46 | Per-Neuron Gauge Correction | DONE (demo_46_per_neuron_gauge/) |
| 47 | Parity Investigation | DONE (demo_47_parity_investigation/) |
| 57 | Additive Encoding DKC | DONE (demo_57_additive_dkc/) |
| 58 | Fibonacci Anyon DKC | DONE (demo_58_fibonacci_dkc/) |
| 59 | Q=3 Potts DKC | DONE (demo_59_q3_potts_dkc/) |
| 60 | Cubic Wall ell=7 | DONE (demo_60_cubic_wall/) |
| 61 | Linearity Classification | DONE (demo_61_angular_proof/) |
| 62 | Radical CSS Code | DONE (demo_62_analytical_proof/) |
| 39 | Symmetry Decomposition | DONE (demo_39_symmetry_decomposition/) |

---

## Arc 1: TL Matrix Engineering (Track C remnants)

Demo 35 is complete, unblocking this entire chain. These extend the TL matrix method toward practical braid computation at scale.

### D36 -- Long Braid Scaling
- O(k) matrix pipeline for 10,000+ crossing braids; repeated squaring for sigma^k
- Dependencies: D35 (done)
- Priority: **MEDIUM** -- engineering demo, validates practical ceiling
- Addresses: demonstrates DKC at circuit scale (publication credibility)

### D37 -- Diakoptics (Parallel Braid Computation)
- Kron tearing: cut braid into sections, compute in parallel, compose
- Dependencies: D35 (done), D36
- Priority: **LOW** -- performance optimization, not research frontier

### D39 -- Symmetry Decomposition — DONE
- **Completed as demo_39_symmetry_decomposition/**. 18/18 tests pass.
- Verified cellular sector sizes = d(n,j)^2, proved cell filtration, showed semisimple algebras NOT block-diagonal in standard basis, mapped radical dims at n=ell boundary for ell=2..7.
- See atlas/inventory/entries/demo_39.md for full results.

### D40 -- Impedance Tensor
- Transfer matrix as Kron impedance; closure = C^T Z C; eigenvalue spectra
- Dependencies: D35 (done), D38 (done)
- Priority: **LOW** -- Kron analogy is interesting but not on the publication path

### D41 -- Axis Elimination (Schur Complement)
- Eliminate internal strands to reduce wide braids to 2D effective description
- Dependencies: D35 (done), D38 (done), D40
- Priority: **LOW** -- blocked behind D40, engineering benefit only

---

## Arc 2: Foundation / Pipeline (Tracks A and B)

Original proposals from the pre-DKC era. Still valid as pedagogical and infrastructure demos but no longer on the critical research path.

### D30 -- Taxonomy-to-Braid Compiler
- Convert taxonomy (is-a edges) into braid words via linear logic implications
- Dependencies: D01, D03 (done)
- Priority: **LOW** -- interesting for the Abramsky connection (TL as computational calculus) but tangential to current research arc

### D31 -- Execution-as-Traversal
- Run entity state vectors through braid matrix product for taxonomy classification
- Dependencies: D01-D04 (done)
- Priority: **LOW** -- same as D30; the DKC arc has moved far past this

### D32 -- Two-Thread Coordinate System
- Milnor-style frozen coordinate interpolation, 2 threads = 4D coordinates
- Dependencies: none
- Priority: **LOW** -- standalone geometric exploration

### D33 -- Full Pipeline (PD -> Braid -> Spin Chain -> Bracket)
- End-to-end composition of D01+D05+D08 into single pipeline
- Dependencies: D01, D05, D08 (done)
- Priority: **LOW** -- mostly engineering/cleanup; all pieces exist individually

### D34 -- Torus Knots and Connected Sums
- Extend knot table (T(2,5), T(3,4), T(3,5)), test bracket product formula for connected sums
- Dependencies: D01, D05, D07, D08 (done)
- Priority: **LOW** -- nice verification but not advancing the research

---

## Arc 3: Physics Connections (Track D remnants)

### D43 -- Landauer Verification
- Fully reversible Toffoli from Fredkin primitives; measure Landauer tax of current approach
- Dependencies: D21 (done)
- Priority: **LOW** -- interesting physics question but separate from DKC core

### D44 -- Path Integral / Ising Connection
- Bracket = Ising partition function at criticality; verify via Tait graph and transfer matrix
- Dependencies: D18, D20 (done)
- Priority: **MEDIUM** -- connects DKC to statistical mechanics; strengthens the Costello-Francis-Gwilliam (2026) link mentioned in narrative Section 10
- Addresses: physical interpretation open question (narrative "what is next")

---

## Arc 4: Publication Strengthening (NEW -- from synthesis)

These directly address the strengthening actions and open questions identified in novelty.md Section 5 and the narrative frontier.

### D63 -- 4-Input DKC (Triskelion Generalization)
- Search 222 NPN classes of 4-input Boolean functions with bracket catalog; test k=2n conjecture (parity at k=8)
- Dependencies: D50 (done), D29 (done)
- Priority: **HIGH** -- tests the triskelion generalization conjecture; a positive result strengthens Paper 1 enormously; a negative result is equally informative
- Addresses: strengthening action #2 (4-input and 5-input DKC), conjectured triskelion generalization (k=2n for n-input parity)

### D64 -- Catalog Completeness Bound
- Prove or bound that the 100-value Z[zeta_8] catalog at delta=0 contains all distinct bracket values up to braid length L; characterize saturation curve
- Dependencies: D29 (done), D35 (done)
- Priority: **HIGH** -- the 906 parity solutions claim depends on catalog completeness; Paper 1's main weakness
- Addresses: strengthening action #4 (catalog completeness argument)

### D65 -- Sandwich Theorem Formal Proof — PARTIALLY DONE
- **Radical dimension formula PROVEN** (`proofs/radical-dimension-formula.md`): rad(TL_ℓ) = 2ℓ−3 via Graham-Lehrer cellular theory + Chebyshev determinant. Corollary: ℓ²−ℓ−3 at n=ℓ+1.
- **Remaining**: Full sandwich theorem (rad^2(TL_{2k}(0)) ≅ TL_{2k-1}(0) as *algebras*), nilpotency=3 formal proof, Fibonacci rank formal proof.
- Dependencies: D51 (done), D52 (done)
- Priority: **HIGH** -- dimension formula is proven; algebra isomorphism and nilpotency still needed for Paper 3
- Addresses: strengthening action #3 (partially complete)

### D66 -- Nazer-Gastpar Rate Region for DKC
- Apply compute-and-forward capacity bounds to the DKC lattice; how many bits per bracket value can DKC extract?
- Dependencies: D29 (done), D48 (done)
- Priority: **MEDIUM** -- strengthens the Nazer-Gastpar pillar connection; would make a strong section in Paper 1
- Addresses: strengthening action #5 (rate region analysis), two-channel coding conjecture (~5.8 bits/symbol)

### D67 -- Axiality Proof Formalization
- Formalize the proof sketch that bracket values at delta=0 are axial in Z[zeta_8]; determine whether it follows from cyclotomic Galois theory or is genuinely new
- Dependencies: D35 (done), D54 (done)
- Priority: **MEDIUM** -- determines whether Paper 4 (axiality) is a short note or a significant result; consultation with a number theorist recommended
- Addresses: novelty assessment Section 2d (axiality confidence MEDIUM-HIGH); strengthens Paper 4

---

## Priority Summary

### HIGH (do these first -- direct publication impact)
1. **D63** -- 4-Input DKC. Tests triskelion generalization. Paper 1 strengthener.
2. **D64** -- Catalog Completeness Bound. Paper 1 weakness fix.
3. **D65** -- Sandwich Theorem Proof. **Radical dim formula PROVEN**; algebra isomorphism remaining.

### MEDIUM (do next -- strengthen claims or open new connections)
4. **D66** -- Nazer-Gastpar Rate Region. Paper 1 pillar strengthener.
5. **D67** -- Axiality Proof Formalization. Paper 4 gating question.
6. ~~**D39** -- Symmetry Decomposition.~~ **DONE** (demo_39_symmetry_decomposition/)
7. **D44** -- Path Integral / Ising. Physics interpretation.
8. **D36** -- Long Braid Scaling. Practical ceiling validation.

### LOW (backlog -- valid but not urgent)
9. **D37** -- Diakoptics
10. **D40** -- Impedance Tensor
11. **D41** -- Axis Elimination
12. **D30** -- Taxonomy-to-Braid Compiler
13. **D31** -- Execution-as-Traversal
14. **D32** -- Two-Thread Coordinates
15. **D33** -- Full Pipeline
16. **D34** -- Torus Knots
17. **D43** -- Landauer Verification

---

## Open Questions Not Yet Assigned to Demos

These emerged from synthesis but don't have a clear single-demo scope yet:

- **Multi-layer DKC**: cascading bracket-valued neurons for circuit-level compilation (narrative Section 10). Needs design work before scoping.
- **Galois symmetry conjecture**: sandwich duality from Galois automorphisms of Gal(Q(zeta_8)/Q). Highly speculative; may resolve itself during D65.
- **Abramsky depth**: full Geometry of Interaction interpretation to predict which bracket triples compute which functions. Needs significant theoretical development.
- **Costello-Francis-Gwilliam connection**: Witten=RT now proven (2026); DKC compiles QFT transition amplitudes into Boolean logic. Framing question more than a demo.
- **5-input DKC**: would further test triskelion generalization (k=10 for 5-input parity). Scope depends on D63 results.
