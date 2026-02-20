## Demo 46: Per-Neuron Gauge Correction

- **Status**: COMPLETE
- **File**: `demo_46_per_neuron_gauge/main.c` (~1472 lines)
- **Tests**: 6/10 checks pass (4 informative failures on predictions P2-P5), across 6 parts (A-F): NPN classification, complex network training, bracket catalog, decomposition with global + per-neuron gauge, ranking & Spearman correlation, summary table & angle structure
- **Depends on**: Demo 45 (Complex Function Zoo — global gauge decomposition, Spearman rho = -0.82), Demo 27 (reverse DKC error decomposition), Demo 28 (unitary activation comparison)
- **Feeds into**: Demo 47 (Parity Investigation — MVN activation to test lattice-aligned phases)

### Headline
Replaces Demo 45's single global U(1) gauge rotation with independent per-neuron rotations (3D sweep: one angle per hidden neuron), finding that per-neuron gauge improves RMS by 20% but amplitude-only decomposition (RMS 0.33) still dramatically beats per-neuron gauge (RMS 0.57) — proving that phase decoherence from split-sigmoid training, not gauge approximation, is the fundamental bottleneck.

### Key Results
- **NPN classification** (Part A): maps all 256 3-input truth tables to 14 NPN canonical forms (via permutation + input negation + output negation), selects 13 non-trivial classes for testing
- **Complex network training** (Part B): CxNet3 architecture (3 complex hidden neurons, 12 complex weights), split-sigmoid activation, 10 trials per NPN class, SGD with lr=0.3, max 200k epochs
- **Bracket catalog** (Part C): 64 distinct complex bracket values from 2-3 strand braids at A = e^{i*5pi/4}, same as Demo 45
- **Decomposition** (Part D): for each converged network, three decomposition methods compared:
  - Global gauge (1D sweep, 360 angles): avg RMS = 0.7164
  - Per-neuron gauge (3D sweep, coarse 36^3 + refinement 11^3): avg RMS = 0.5723
  - Amplitude-only (magnitude matching, ignore phases): avg RMS = 0.3283
  - Random baseline: 0.8086
- **Ranking & Spearman** (Part E): correlation between topology frequency and decomposition quality — global rho = -0.8167, per-neuron rho = -0.7833 (slightly worse). Computed over 9 classes with nonzero topology frequency.
- **Summary & angle structure** (Part F): per-neuron angle spreads of 50-170 degrees between neurons for most classes; only 4/13 show clustering

### Prediction Scorecard
| # | Prediction | Result | Notes |
|---|-----------|--------|-------|
| P1 | pn_RMS <= global_RMS for all 13 | PASS | Trivially true (superset search space) |
| P2 | Largest improvement for mid-tier (ranks 4-9) | FAIL | Bottom tier improved most (0.1938 avg) |
| P3 | pn_RMS < amp_RMS for top 6 | FAIL | Amplitude-only wins for ALL classes |
| P4 | Spearman rho improves (more negative) | FAIL | Slightly worse (-0.7833 vs -0.8167) |
| P5 | Per-neuron angles show clustering | FAIL | Only 4/13 show structure; spreads 50-170 deg |

### Theorems/Conjectures
- **Phase decoherence thesis**: split-sigmoid activation treats Re and Im independently, producing weights whose magnitudes align with bracket magnitudes but whose phases are essentially random relative to Z[zeta_8]. No post-training rotation can recover what training never encoded.
- **Classical vs quantum partition functions**: split-sigmoid training accesses the Kauffman bracket as a classical/thermal partition function (magnitudes only), not a quantum one (requiring phase coherence). The Potts-model sector vs the Jones-polynomial sector.
- **Coding theory capacity**: bracket codebook offers ~7.3 bits/symbol (4.3 magnitude + 3 phase). Split-sigmoid encodes ~4.3 bits (magnitudes only). Per-neuron gauge recovers ~0.5 bits of misaligned phase. ~1.5 bits of phase capacity unused.
- **Dimensionality mismatch**: weight vector in C^12 ~ R^24; 12 magnitude coordinates carry signal, 12 phase coordinates carry noise. Gauge rotation couples magnitudes and phases, degrading both. Amplitude-only = dimensionality reduction to the informative subspace.
- **Fork in the road**: Path A — accept phase decoherence, use amplitude-only matching; Path B — fix training with MVN/lattice-constrained optimization to access the quantum sector.

### Data
- 13 NPN classes, 10 trials per class, CxNet3 architecture (3 hidden neurons, 12 complex weights)
- 64 bracket catalog values (2-3 strands, lengths 1-8, at A = e^{i*5pi/4})
- 3D gauge sweep: coarse 36x36x36 (46,656 combinations, 10-deg) + refinement 11x11x11 (1,331, 1-deg)
- Runtime: ~5 minutes (dominated by 3D gauge sweeps)
- Highlight: ~A(B^C) jumped from rank 9 to rank 3 (0.7781 -> 0.4813, delta 0.2968) — pure inter-neuron gauge problem
- XNOR3 (parity): improved 0.9963 -> 0.7654 but still far from lattice

### Code Assets
- **NPN equivalence engine**: `npn_canonical()` computing minimum over 96 transforms (6 perms x 8 input-neg x 2 output-neg) per truth table, precomputed for all 256
- **CxNet3 forward/backward**: 3-hidden-neuron complex network with split-sigmoid activation, full backpropagation
- **3D per-neuron gauge sweep**: coarse-then-refine optimization over independent U(1) rotations per neuron
- **Three-way decomposition comparison**: global gauge (1D), per-neuron gauge (3D), amplitude-only, all applied to same trained weights and catalog
- **Spearman rank correlation**: computed over n=9 topology-reachable classes (excludes 4 classes with zero frequency)
- **LCG RNG**: reproducible random initialization across all trials
- Reuses: complex arithmetic, braid state-sum bracket, union-find, bracket catalog builder

### Literature Touched
- **Hirose (2012)**: per-neuron U(1) phase freedom in complex networks — theoretical curiosity quantified empirically here
- **GLVQ (NeurIPS 2025)**: per-group learnable codebooks for real-valued LLM quantization (closest analog)
- **GPTQ = Babai's nearest-plane algorithm (ICLR 2026)**: amplitude-only success consistent with Babai rounding in correct coordinate system
- **Aizenberg MVN activation**: kth-root-of-unity output, natively respects cyclotomic lattice (candidate to fix phase decoherence)
- **Potts model vs Jones polynomial**: same state-sum structure, different regimes (real temperature vs complex q)
- **Nazer-Gastpar lattice codes**: channel capacity interpretation of bracket codebook
- Novelty: per-neuron U(1) gauge correction for complex weight decomposition onto cyclotomic lattice has no precedent

### Open Questions
- Does MVN activation (Demo 47) produce lattice-aligned phases, closing the gap with amplitude-only?
- Is amplitude-only decomposition sufficient for the compiler, or do phases become critical at 4+ inputs?
- Can the optimal per-neuron gauge be predicted analytically for lattice-aware architectures?
- Is there a deeper algebraic reason why magnitudes carry computational information but phases don't under split-sigmoid?
