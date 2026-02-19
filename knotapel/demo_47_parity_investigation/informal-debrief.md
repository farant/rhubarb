# Demo 47: Informal Debrief

**Date:** 2026-02-18

## What We Actually Did

Demo 46 left us with a clean fork in the road: split-sigmoid training produces "classical" networks whose magnitudes carry the bracket's computational signature but whose phases are noise. The question was whether MVN activation — Aizenberg's unit-circle activation that's literally designed for roots-of-unity computation — could fix this. Specifically: is parity's terrible decomposition (0.77 pn-RMS, worst of all 13 NPN classes) an artifact of split-sigmoid, or is it baked into the bracket algebra?

Demo 47 runs three activations (split-sigmoid, MVN-continuous z/|z|, MVN-k8 snap-to-root) across all 13 NPN classes, with a parity deep dive using 3-6 hidden neurons. Same bracket catalog, same per-neuron gauge, same everything else.

## The Headline

**MVN-cont dramatically improves parity decomposition (0.41 vs 0.77), but NOT by the mechanism we expected.**

The prediction was straightforward: MVN maps outputs to the unit circle, so phases should be more Z[zeta_8]-aligned, so gauge correction should work better because the phases are closer to lattice points. Simple story.

What actually happened: MVN-cont improves per-neuron gauge from 0.57 to 0.46 average (20% improvement) and from 0.77 to 0.41 for parity (47% improvement). But phase alignment to pi/4 multiples *didn't improve at all* — it got marginally worse (0.197 vs 0.189 rad). And amplitude-only decomposition got WORSE (0.36 vs 0.33).

The mechanism isn't "phases are closer to lattice points." It's "phases within each neuron's weight subspace are more coherent, so per-neuron gauge rotation can exploit them." The z/|z| activation forces the hidden layer outputs onto the unit circle, which means the network can't use hidden output magnitudes as a computational degree of freedom — it has to encode everything in the relative phases of the weights. This produces weight vectors where the 4 complex weights of each neuron are phase-correlated rather than phase-random. The gauge rotation then works because it can rotate all 4 phases of a neuron simultaneously and they all approximately improve.

## The Five Surprises

### 1. The Topology-Frequency Correlation REVERSED

This is the most important result in the whole demo and I didn't predict it at all.

Spearman rho(pn-RMS, topology_freq): split-sig = -0.78, MVN-cont = +0.55, MVN-k8 = -0.10.

Under split-sigmoid, topology-preferred functions (AND, BUF, AND2) decompose best and topology-resistant functions (XNOR3, XOR2, EXACT1) decompose worst. Under MVN-cont, this is *inverted*. The bottom-tier functions improved dramatically (improvement 0.17) while the top-tier functions actually got slightly WORSE (improvement -0.04).

This means the two activations access genuinely different sectors of the bracket algebra. Split-sigmoid's magnitude structure aligns with the bracket's topological sector (where functions that arise naturally from short braids live). MVN-cont's phase coherence aligns with something else — call it the "angular sector" — where functions that resist topological realization nonetheless have bracket-compatible phase structure.

If I'm right about this, it's a very clean result: the bracket algebra has (at least) two decomposition channels, and different activations tune into different channels. A compiler that uses both would be strictly more powerful than either alone.

### 2. Amplitude-Only Got WORSE

Split-sig amp-only: 0.328. MVN-cont amp-only: 0.360. This is the corollary of the correlation reversal: MVN-cont moves information FROM magnitudes INTO phases. The total lattice-compatible information increases (~5 bits vs ~4.3 bits per symbol) but it's redistributed. If you throw away the phases (amplitude-only), you're throwing away the extra information MVN created.

This completely falsifies the simple narrative that "MVN = better phases = better everything." It's an actual tradeoff. You gain phase coherence at the cost of magnitude alignment.

### 3. MVN-k8 Was Worse Than MVN-cont

Predicted that the discrete activation (outputs literally ARE 8th roots of unity) would decompose best. Instead it was worse than continuous MVN for parity (0.59 vs 0.41 pn-RMS) and worse on average (0.50 vs 0.46).

The STE (straight-through estimator) gradient is the problem. In the forward pass, MVN-k8 snaps z/|z| to the nearest root. In the backward pass, it pretends it used the continuous z/|z| gradient. This mismatch between forward and backward introduces training noise — the gradient pushes weights in directions that would improve the continuous output but don't improve the discrete output. The result: worse convergence quality, less coherent weight structure, worse decomposition.

This is a known problem with STE in quantization-aware training, but it's interesting to see it manifest here. The fix would be Aizenberg's native derivative-free learning rule, which doesn't need STE because it directly adjusts weights to rotate the weighted sum into the correct sector.

### 4. More Neurons Help Parity (P5 Failed)

Predicted that more hidden neurons wouldn't help parity under split-sigmoid (based on the idea that parity's resistance is algebraic, not architectural). Wrong — going from nh=3 to nh=6 drops pn-RMS from 0.89 to 0.51.

This makes sense in retrospect. With 3 hidden neurons and split-sigmoid, the network is quite constrained. Each neuron has to contribute a lot to the final output. With 6 neurons, the load is distributed and the gauge has more degrees of freedom (6D torus vs 3D torus). Even if individual weight phases are noisy, the aggregate effect of rotating 6 independent neurons can find better lattice matches.

But even at nh=6, split-sig pn-RMS (0.51) doesn't reach MVN-cont nh=3 (0.56). Actually wait — it does slightly beat MVN-cont nh=3 (0.51 < 0.56). But MVN-cont at nh=6 gets down to 0.42, which split-sig can't match.

### 5. Phase Error Is Not The Right Metric

This is the meta-surprise. The phase error metric (average angular distance to nearest pi/4 multiple) doesn't capture what MVN is doing. MVN produces phase *coherence* (phases within a neuron are correlated), not phase *alignment* (phases are close to lattice multiples). These are different things.

A better metric would be: for each neuron, measure the variance of gauge-corrected phases across its weights. Low variance = coherent (all phases move together when you rotate). High variance = incoherent (rotation helps some weights but hurts others). I bet this metric would show MVN-cont with much lower intra-neuron phase variance than split-sigmoid.

## Cross-Lens Insights

### QM: The Semi-Quantum Regime

Demo 46 established: split-sigmoid = classical (Potts/magnitude sector). Demo 47 adds: MVN-cont = semi-quantum (neither classical nor fully quantum). It's an intermediate regime where phases carry information but not in the lattice-aligned way that would make it a "quantum" computation in the TQC sense.

The three regimes of the bracket:
1. **Classical (split-sig):** Magnitudes carry computation, phases are noise. Topology-preferred functions align well. Like the Ising model at real temperature.
2. **Semi-quantum (MVN-cont):** Phases carry coherent information, not lattice-aligned. Topology-resistant functions now decompose. Like a partially decoherent quantum system.
3. **Quantum (forward DKC):** Exact Z[zeta_8] values. Full phase coherence. All functions decompose perfectly (RMS = 0). Like a fault-tolerant topological quantum computation.

The gap between semi-quantum (0.46) and quantum (0.00) is still large. Closing it probably requires Aizenberg's native learning rule, not backprop with MVN activation hacked on top.

### Coding Theory: Two-Channel Coding

The topology-frequency correlation reversal is a coding theory observation in disguise. The bracket catalog defines a codebook with 64 complex entries. This codebook has two "channels":
1. **Magnitude channel:** ~20 distinct magnitudes, 4.3 bits/symbol. Split-sigmoid transmits well here.
2. **Phase channel:** 8 possible phases (pi/4 multiples), 3 bits/symbol. MVN-cont transmits partially here (~1.5 bits).

The two channels have different "noise profiles" — topology-preferred functions are easier to match in the magnitude channel, topology-resistant functions are easier to match in the phase channel. A joint decoder that uses both channels simultaneously could theoretically achieve ~5.8 bits/symbol, beating either channel alone.

### Approximation Theory: Coherence vs Alignment

In the C^12 approximation framework:
- **Alignment** = individual phases are near lattice points (measured by phase error)
- **Coherence** = phases within a neuron's weight subspace are correlated (measurable by intra-neuron phase variance)

Split-sigmoid has neither alignment nor coherence — phases are random in every sense.
MVN-cont has coherence without alignment — phases are correlated but not at lattice points.
Forward DKC has both — exact lattice values are trivially aligned and coherent.

The gauge rotation exploits coherence, not alignment. This is the key insight: gauge correction is a *rotation*, so it can only help if all the things it rotates move in approximately the same direction. Coherence means they do; alignment means they're already in the right place. You need coherence for gauge to work; you need alignment for gauge to be unnecessary.

### Rep Theory: U(1)^3 → Different Decomposition

Under split-sigmoid, the per-neuron gauge finds angle spreads of 50-170 degrees (Demo 46). Under MVN-cont, the equivalent data isn't reported in the output, but the improvement pattern suggests the spreads are similar in magnitude but the intra-neuron coherence is higher. The U(1)^3 action is more effective when each U(1) acts on a coherent 4-dimensional subspace rather than on 4 independently random phases.

## The Fork Clarified

Demo 46's fork was:
- **Path A:** Accept phase decoherence, use amplitude-only matching
- **Path B:** Fix the training, use MVN to get lattice-aligned phases

Demo 47 shows: **Neither path is exactly right. Path C emerges.**

- **Path A (amplitude-only):** Works well for split-sigmoid (0.33 RMS) but gets *worse* with MVN (0.36). Not universal.
- **Path B (MVN + gauge):** Works well (0.46 RMS) but the mechanism isn't phase alignment — it's phase coherence. The lattice-alignment prediction was wrong.
- **Path C (activation-specific decoding):** Use amplitude-only for split-sig networks. Use per-neuron gauge for MVN-cont networks. The compiler should detect which activation was used and choose the appropriate decomposition strategy. Best of both worlds.

Even more ambitiously: **Path D (dual-activation training).** Train two networks for each function — one split-sig, one MVN-cont. Decompose each optimally. Take the lower RMS. This would capture both the topological sector (split-sig) and the angular sector (MVN-cont).

## Literature Connections

The literature scout found something remarkable: the Temperley-Lieb algebra at q = zeta_8 is **non-semisimple**. The Jones-Wenzl idempotent creates a radical in the algebra. This non-semisimplicity is the algebraic obstruction that limits Ising anyons to Clifford gates.

If parity's residual resistance (0.41 RMS even with MVN) is related to this non-semisimplicity, then no amount of training improvement can fully close the gap for parity. The bracket algebra itself has a structural limitation at the relevant root of unity. This would be a deep result connecting:
1. Neural network learnability (parity is hardest)
2. Knot invariant evaluation (bracket is a state sum)
3. Topological quantum computation (Ising anyons are non-universal)
4. Abstract algebra (TL non-semisimplicity at roots of unity)

Nobody in the literature has proposed this connection. Four siloed communities, one underlying algebraic phenomenon.

## Technical Notes

- Runtime: ~35 minutes total (dominated by 39 3D gauge sweeps + 12 coordinate-descent gauge sweeps)
- The parity deep dive retrains from scratch at each nh, so Part E numbers differ from Part D (which uses the nh=3 networks from Part B)
- Phase error metric (angular distance to nearest pi/4) is a proxy; the more informative metric would be intra-neuron phase variance (not computed in this demo)
- MVN-k8 STE gradient shares the continuous MVN Jacobian — this is the standard approach but is known to introduce bias for large discretization steps (pi/4 is a large step)

## The One-Sentence Version

MVN activation improves bracket decomposition by creating phase coherence rather than phase alignment, and the topology-frequency correlation reverses sign, revealing that different activations access different sectors of the bracket algebra.
