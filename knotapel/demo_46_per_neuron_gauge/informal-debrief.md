# Demo 46: Informal Debrief

**Date:** 2026-02-18

## What We Actually Did

Demo 45 left a puzzle. The decomposition of trained complex network weights into Kauffman bracket values worked — Spearman rho = -0.82 between topology frequency and decomposition quality across all 13 NPN classes. But there was this weird thing: if you just matched magnitudes and threw away all phase information, you got *better* results than the best gauge-corrected complex decomposition. For BUF it was 0.40 vs 0.53.

The natural explanation: Demo 27 (2-input XOR, 3 complex weights) found a single global rotation that aligned everything beautifully. But Demo 45 has 12 complex weights spread across 3 hidden neurons. One rotation can't serve all masters — different neurons learned at different lattice orientations. Give each neuron its own rotation and the problem should go away.

Demo 46 tests this by replacing the 1D gauge sweep (one angle for all 12 weights) with a 3D sweep (one angle per hidden neuron's 4 weights). Coarse grid at 10-degree resolution (36^3 = 46,656 combinations), then 1-degree refinement around the best. Same training, same catalog, same everything else.

## The Headline

**The hypothesis was wrong in an informative way.**

Per-neuron gauge helped — average RMS dropped from 0.72 to 0.57, a 20% improvement. Every single class improved or stayed the same. All 13 now beat the random baseline (vs 11 with global gauge). Even XNOR3 (parity) came down from 0.9963 to 0.7654.

But amplitude-only RMS is 0.33. It still wins by a mile. Not even close.

The error isn't gauge approximation. It's something deeper. The trained weights' phases are genuinely not Z[zeta_8]-aligned, and no amount of rotation can fix it.

## Why Phases Are Noise

The split-sigmoid activation treats real and imaginary parts independently — sigmoid(Re(z)) and sigmoid(Im(z)) applied separately. There's nothing in that activation or in backprop that would push phases toward multiples of pi/4. The optimizer finds weight magnitudes that work (because magnitude structure carries the Boolean function's computational signature) but the phases are just... whatever gradient descent happened to land on.

Magnitudes carry the computation. Phases carry training-procedure artifacts. When you do gauge correction (1D or 3D), you're trying to align all 24 real dimensions (12 magnitudes + 12 phases) simultaneously, but only 12 of them contain signal. Worse, gauge rotation *couples* magnitudes and phases — rotating to improve a phase can degrade a magnitude. Amplitude-only projection just drops the 12 noise dimensions. Cleaner, simpler, better.

## The Four Surprises

### Bottom Tier Benefited Most

Predicted mid-tier would improve most (they're close enough to the lattice that better gauge might push them over). Instead the worst functions improved most (avg 0.1938 vs middle 0.1167). In retrospect this makes sense — the worst functions had the most *inter-neuron* phase disagreement, which is exactly what per-neuron gauge fixes. But they also have the most *intra-neuron* phase noise, which per-neuron gauge can't touch.

### ~A(B^C) Jumped From Rank 9 to Rank 3

Single biggest improvement: 0.7781 to 0.4813, delta 0.2968. This function's neurons were internally well-organized but badly misaligned *with each other*. A pure inter-neuron gauge problem. Once you give each neuron its own rotation, the decomposition reveals that ~A(B^C) was actually a decent lattice citizen all along — it was just wearing a disguise.

### Spearman Rho Got Worse

-0.7833 vs -0.8167. You'd think better decomposition = better correlation. But the ranking reshuffle from per-neuron gauge doesn't respect the topology-frequency ordering. The topology-frequency correlation is fundamentally a magnitude-level phenomenon. Adding phase correction introduces noise into the ranking — it helps functions for reasons that have nothing to do with topological naturalness.

### Angle Spreads Are Huge

Most classes show 50-170 degree spreads between their three neuron angles. Only 4/13 show any clustering. The three neurons are genuinely in very different U(1) orbits. The global gauge was making a brutal compromise.

## Cross-Lens Insights

### QM: Classical vs Quantum Partition Functions

The Kauffman bracket is a partition function — a sum over states where each state contributes amplitude AND phase. But there are two kinds of partition functions in physics:

- **Classical/thermal** (Potts model, Ising model): only magnitudes matter, phases wash out, you get thermal equilibrium
- **Quantum** (Jones polynomial, path integrals): phases ARE the computation, interference is the mechanism, you get quantum amplitudes

Demo 46 shows that split-sigmoid training produces networks that access the bracket as a *classical* partition function. The magnitude structure is there — that's what gives us the 0.33 RMS and the topology-frequency correlation. But the quantum phase structure is absent. The training procedure is a "decoherence channel" in the literal QM sense.

This maps to a real mathematical distinction. The Potts model at real temperature and the Jones polynomial at complex q are both state-sum models on the same combinatorial structure. But the Jones polynomial requires phase coherence to produce its invariant. We're currently in the Potts sector. To get to the Jones sector, you need training that preserves phase — which is exactly what Aizenberg's MVN activation does (it maps to kth roots of unity, natively respecting the lattice's rotational structure).

### Coding Theory: Channel Capacity Measurement

The bracket catalog is a codebook with 64 complex values. Each trained weight is a "received symbol." The gauge is a channel equalizer.

The magnitude channel has capacity ~4.3 bits/symbol (log2 of ~20 distinct magnitudes). The phase channel has capacity ~3 bits/symbol (log2(8) for 8th roots). Total potential: ~7.3 bits/symbol.

But the split-sigmoid network only *encodes* ~4.3 bits/symbol of lattice-compatible information — all in magnitudes, zero in phases. The per-neuron gauge recovers about 0.5 bits of phase information that was present but misaligned. The remaining ~1.5 bits of phase capacity were simply never used by the training procedure.

Clean prediction for Demo 47: MVN activation should encode 2-3 bits/symbol of lattice-compatible phase information. If it does, per-neuron gauge should become dramatically more effective and might close the gap with amplitude-only.

### Approximation Theory: Dimensionality Mismatch

The weight vector lives in C^12 ~ R^24. Amplitude-only projection maps to R^12. The gauge orbit is a 1D curve (global) or 3D torus (per-neuron) in R^24.

Amplitude-only wins because it's doing dimensionality reduction to the informative subspace. The 12 noise dimensions (phases) aren't just uninformative — they actively degrade approximation quality because gauge rotation couples them to the signal dimensions. It's like trying to approximate f(x,y) when the signal is only in x — working in (x,y) space is strictly harder than projecting to x first.

### Rep Theory: U(1) -> U(1)^3

The global gauge is a single U(1) action on C^12. Per-neuron gauge is U(1)^3 acting on C^4 x C^4 x C^4. The improvement from global to per-neuron directly measures how much the network's internal representation decomposes into independent U(1) sectors. Answer: a lot — the neurons need very different rotations — but it's still not enough because the phases within each neuron's 4-weight subspace are also disordered.

## The Fork in the Road

Demo 46 creates a clear choice for the DKC program:

**Path A — Accept phase decoherence.** Use amplitude-only matching as the default reverse pathway. Accept that you're in the "classical" sector of the bracket algebra. This works now, gives RMS ~0.33, and is simple. The compiler would match magnitudes and ignore phases.

**Path B — Fix the training.** Use MVN activation or lattice-constrained optimization to produce weights whose phases are lattice-aligned. Then per-neuron gauge (or even global gauge) should work properly, and you'd access the full "quantum" sector.

Demo 47 (Parity Investigation) is designed to test whether Path B is even possible, starting with parity — the hardest case. If MVN activation produces lattice-aligned phases, per-neuron gauge should dramatically improve. If parity *still* resists with MVN, that's a deep algebraic fact about the bracket, not an activation function problem.

There's also a deeper question that Demo 46 raises but can't answer: is amplitude-only decomposition *good enough* for the compiler? If magnitudes carry all the computational information, maybe phases are genuinely irrelevant for Boolean function decomposition. Or maybe phases become critical at 4+ inputs where the function space is much richer and magnitude-only matching runs out of resolution.

## Literature Context

The four-way connection (Aizenberg CVNNs + Habiro/Kauffman cyclotomic brackets + Nazer-Gastpar lattice codes + Abramsky categorical computation) remains unconnected in the literature. Per-neuron gauge correction for complex weight decomposition onto a cyclotomic lattice is novel. Closest match is GLVQ (NeurIPS 2025) — per-group learnable lattice codebooks for real-valued LLM quantization. Also relevant: GPTQ proven equivalent to Babai's nearest-plane algorithm (ICLR 2026), which means our amplitude-only approach is essentially Babai rounding in the right coordinate system.

Hirose (2012) identifies the per-neuron U(1) phase freedom in complex networks as a theoretical curiosity. Demo 46 empirically quantifies its impact on lattice decomposition — it's not just a curiosity, it's the dominant source of error.

## Technical Note: Spearman Implementation

The Spearman correlation is computed only over the 9 classes with nonzero topology frequency (excluding ISOLATE, A^(B|C), XOR2, XNOR3). The function silently returns 0.0 for n < 3, which doesn't affect our results (n=9) but could mask problems if someone later calls it on a per-tier subset. The filtering to n=9 is inherited from Demo 45 and is defensible — you can't meaningfully rank things all tied at 0 — but the reported rho describes the topology-reachable subset, not the full 13.

## The One-Sentence Version

Split-sigmoid training produces classical networks that use the bracket's magnitude structure but not its phase structure, and no post-training rotation can recover what training never encoded.
