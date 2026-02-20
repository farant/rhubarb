# Future Demos

Unfinished knotapel demos, extracted from Smaragda entities (Feb 2026). Each file = one demo.

23 demos total (30-44, 46-47, 57-62). 10 original ready now, 7 blocked behind keystone, 6 new from DKC research arc.

## Ready Now

All dependencies (Demos 01-29) are completed with code on disk.

### Track A: Early Taxonomy/Traversal (original proposal)
- `30-taxonomy-to-braid-compiler.md` — Convert taxonomy into braid words via linear logic *(needs Demo 1, 3)*
- `31-execution-as-traversal.md` — Run entities through braid matrix product for classification *(needs Demos 1-4)*
- `32-two-thread-coordinates.md` — Milnor-style frozen coordinate interpolation *(no dependencies)*

### Track B: Pipeline Composition
- `33-full-pipeline.md` — End-to-end PD -> Braid -> Spin Chain -> Bracket *(needs Demo 01, 05, 08)*
- `34-torus-knots-connected-sums.md` — Extend knot table, test bracket product formula *(needs Demo 01, 05, 07, 08)*

### Track C Keystone
- `35-tl-matrix-representations.md` — Bracket via C_n x C_n matrix products *(needs Demo 19, 24)* **<-- unlocks 7 more demos**

### Track D: DKC Extensions
- `43-landauer-verification.md` — Fully reversible Toffoli, zero Landauer cost *(needs Demo 21)*
- `44-path-integral-ising.md` — Bracket = Ising partition function at criticality *(needs Demo 18, 20)*
- `46-per-neuron-gauge.md` — Per-neuron U(1) gauge correction, 3D optimization *(needs Demo 45)*
- `47-parity-investigation.md` — Is parity's resistance architectural or algebraic? MVN activation test *(needs Demo 45)*

## Blocked (all waiting on Demo 35: TL Matrix Representations)

These form a dependency chain within Track C. Completing Demo 35 unblocks the whole series.

```
35 tl-matrix-representations
├── 36 long-braid-scaling
│   └── 37 diakoptics
├── 38 dimension-reduction-delta-zero
│   ├── 39 symmetry-decomposition
│   ├── 40 impedance-tensor
│   │   └── 41 axis-elimination
│   └── (also feeds into 39, 40)
└── 42 cyclotomic-exact-arithmetic
```

- `36-long-braid-scaling.md` — 10,000+ crossing braids, O(k) not O(2^k)
- `37-diakoptics.md` — Parallel braid computation via tearing
- `38-dimension-reduction-delta-zero.md` — Effective algebra at delta=0
- `39-symmetry-decomposition.md` — Irreducible blocks, NPN reachability
- `40-impedance-tensor.md` — Transfer matrix as Kron impedance
- `41-axis-elimination.md` — Schur complement strand elimination
- `42-cyclotomic-exact-arithmetic.md` — Z[i,sqrt(2)] integer arithmetic

### Track D (continued): DKC Research Arc (Demos 57-62)

Added Feb 2026 after Demos 53-56 results. Ordering: 57 first (prediction test), 58 second (highest theory value), rest in any order. Demo 61 builds on 57-59 results.

- `57-additive-encoding-dkc.md` — Test coding theory prediction: additive encoding dissolves 0x1B/0x06 Re>0 wall *(needs Demo 55)*
- `58-fibonacci-anyon-dkc.md` — ℓ=5 (δ=φ) axiality and DKC. Does TQC universality → DKC universality? *(needs Demos 54, 55)*
- `59-q3-potts-dkc.md` — Q=3 Potts (ℓ=6, δ=√3). Extend Potts hierarchy. *(needs Demos 52, 56)*
- `60-cubic-wall-l7.md` — ℓ=7 first cubic number field. Sharp test of radical dimension universality. *(needs Demos 51, 52)*
- `61-linearity-classification.md` — Complete GF(2) linearity classification of NPN classes vs DKC achievability. *(needs Demos 55-59)*
- `62-radical-css-code.md` — Radical → CSS quantum error-correcting code → computable functions pipeline. *(needs Demos 51, 52, 55-59)*

### Track E: Radical Anatomy

- `60-cubic-wall-l7.md` — Also in Track D. First non-quadratic number field test.
- `62-radical-css-code.md` — Also in Track D. Bridges anatomy and computation.

## Completed Demos (for reference)

Demos 01-29 all have working code on disk in `knotapel/demo_*/` directories. The Smaragda entities for Demos 10-16 and 26 still show "proposed" status but are actually completed (code exists, tests pass). Those should be updated in Smaragda at some point.

- **Demo 45** (Complex Function Zoo) — Completed 2026-02-18. Bracket decomposition across all 13 NPN classes of 3-input Boolean functions. Spearman rho = -0.8167 between topology frequency and decomposition RMS. 9/11 predictions confirmed. See `demo_45_complex_function_zoo/findings.md`.
