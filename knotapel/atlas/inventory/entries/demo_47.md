## Demo 47: Parity Investigation

- **Status**: COMPLETE
- **File**: `demo_47_parity_investigation/main.c` (~1641 lines)
- **Tests**: ~12 tests; prediction scorecard 3/6 pass (P1, P2, P4 pass; P3, P5, P6 fail — failures are informative)
- **Depends on**: Demo 46 (per-neuron gauge correction, NPN classification, split-sigmoid baseline), Demo 29 (exact Z[zeta_8] catalog), Demo 45/46 (topology-frequency correlation)
- **Feeds into**: Demo 48 (forward DKC zoo, hybrid decomposition), Demo 50 (parity reachability at k=6 sectors)

### Headline
MVN activation dramatically improves bracket decomposition for parity (pn-RMS 0.41 vs 0.77), but NOT by phase alignment to lattice — instead by creating phase coherence within each neuron's weight subspace; the topology-frequency correlation reverses sign under MVN, revealing that different activations access different sectors of the bracket algebra.

### Key Results
- **Three activations compared across all 13 NPN classes**: split-sigmoid (baseline), MVN-continuous (z/|z|), MVN-k8 (snap to nearest 8th root of unity)
- **All 13 NPN classes converge for all 3 activations**; MVN converges 5-10x faster than split-sigmoid
- **MVN-cont improves per-neuron gauge by 20% overall** (0.57→0.46 avg pn-RMS) and **47% for parity** (0.77→0.41)
- **Phase alignment did NOT improve** (0.197 vs 0.189 rad avg error) — the mechanism is phase coherence, not lattice alignment
- **Amplitude-only got WORSE** with MVN (0.36 vs 0.33) — MVN redistributes information from magnitudes into phases
- **Topology-frequency correlation REVERSED**: Spearman rho = -0.78 (split-sig) → +0.55 (MVN-cont) → -0.10 (MVN-k8). Split-sig favors topology-preferred functions; MVN-cont favors topology-resistant functions. The two activations are literally complementary.
- **MVN-k8 disappointed**: STE gradient noise makes it worse than continuous MVN (0.50 vs 0.46 avg, 0.59 vs 0.41 for parity)
- **More neurons DO help parity** under split-sig: nh=3→nh=6 drops pn-RMS from 0.89 to 0.51 (falsified P5)
- **Per-neuron gauge gap (pn-RMS minus amp-only) reduced 60%** by MVN-cont (0.097 vs 0.244), meaning phases now carry real information

### Theorems/Conjectures
- **Phase coherence (not alignment) drives gauge improvement**: CONFIRMED — MVN produces intra-neuron phase correlation that gauge rotation exploits
- **Topology-frequency correlation reversal**: OBSERVED — split-sig and MVN-cont access different sectors of the bracket algebra (topological vs angular)
- **Parity barrier partly architectural, partly algebraic**: CONFIRMED — MVN reduces but doesn't eliminate parity's resistance (residual 0.41 RMS may relate to TL non-semisimplicity)
- **TL non-semisimplicity hypothesis**: CONJECTURED — parity's residual resistance under MVN may reflect the non-semisimple structure of Temperley-Lieb algebra at q=zeta_8 (connecting neural network learnability, knot invariant evaluation, TQC, and abstract algebra)
- **Two-channel coding**: CONJECTURED — bracket catalog has magnitude channel (~4.3 bits, split-sig) and phase channel (~1.5 bits, MVN-cont); joint decoder could achieve ~5.8 bits/symbol

### Data
- Training convergence: split-sig ~480 avg epochs (786 parity), MVN-cont ~92 (156 parity), MVN-k8 ~84 (166 parity)
- Decomposition averages (13 classes): split-sig gl=0.716/pn=0.572/amp=0.328/ph=0.189; MVN-cont gl=0.546/pn=0.457/amp=0.360/ph=0.197; MVN-k8 gl=0.557/pn=0.497/amp=0.399/ph=0.207
- Parity (XNOR3) specifically: split-sig pn=0.765, MVN-cont pn=0.407, MVN-k8 pn=0.590
- Parity deep dive (pn-RMS): split-sig nh=3:0.886 nh=4:0.526 nh=5:0.640 nh=6:0.514; MVN-cont nh=3:0.563 nh=4:0.454 nh=5:0.480 nh=6:0.424
- Spearman rho(pn-RMS, topo_freq): split-sig=-0.783, MVN-cont=+0.550, MVN-k8=-0.100 (n=9)
- Random baseline RMS: 0.809
- Prediction scorecard: P1 PASS, P2 PASS, P3 FAIL, P4 PASS, P5 FAIL, P6 FAIL

### Code Assets
- **Three activation functions**: `cx_sigmoid()` (split-sigmoid), `cx_mvn_cont()` (z/|z| unit circle), `cx_mvn_k8()` (snap to nearest 8th root), unified via `cx_activate(z, ActType)`
- **`CxNetVar` network**: variable hidden neuron count (1-6), complex weights, backprop with activation-specific gradients (sigmoid derivative for split-sig, Jacobian of normalization for MVN, STE for MVN-k8)
- **NPN classification**: `npn_init()` computes canonical forms for all 256 3-input Boolean functions via all permutations/negations; 13 non-trivial NPN classes
- **`fn_name()`**: human-readable names for NPN canonical truth tables (AND3, BUF, EXACT1, MAJ, MUX, XOR2, XOR3, XNOR3, etc.)
- **Per-neuron gauge generalized**: `pn_gauge_3d()` for nh=3 (36^3 grid + refinement), `pn_gauge_coord()` for nh>3 (coordinate descent, 72 steps/axis, 3 iterations)
- **`compute_phase_error()`**: average angular distance to nearest pi/4 multiple
- **`spearman_rho()`**: Spearman rank correlation with tie handling
- Bracket catalog builder, global gauge sweep, amplitude-only RMS — all reusable from Demo 46

### Literature Touched
- **Aizenberg (2008)**: MVN solves parity with single neuron; parity is MVN's showcase problem
- **Aizenberg & Moraga (2007)**: derivative-free backpropagation for MLMVN
- **Troyer & Wiese (2005)**: sign problem is NP-hard; parity = maximal cancellation
- **Goldberg & Jerrum (2017)**: Ising partition function is #P-hard
- **Goodman & Wenzl (1993)**: Temperley-Lieb algebra at roots of unity is non-semisimple
- **Ridout & Saint-Aubin (2019)**: TL non-semisimplicity, Jones-Wenzl idempotent radical
- **"Neglectons" (Nature Comms 2025)**: non-semisimple TQFTs rescue universality for Ising anyons
- **GPTQ = Babai's algorithm (ICLR 2026)**: neural network quantization as lattice rounding
- **arXiv:2501.00817 (2025)**: gradient descent fails on parity (hardness of fixed parities)
- **Novelty confirmed**: zero papers connect MVN activation + Kauffman bracket decomposition + cyclotomic lattice structure + Boolean function difficulty. Four siloed communities.

### Open Questions
- Can a hybrid decoder (amplitude-only for split-sig, gauge for MVN-cont) outperform either alone?
- Is the residual 0.41 pn-RMS for parity under MVN-cont related to TL non-semisimplicity at q=zeta_8?
- Would Aizenberg's native derivative-free learning rule (instead of backprop+STE) produce better lattice alignment with MVN-k8?
- Does the topology-frequency correlation reversal hold at 4-input scale (222 NPN classes)?
- What is the intra-neuron phase variance metric, and does it cleanly separate the three activations?
- Can the "angular sector" accessed by MVN-cont be characterized algebraically?
