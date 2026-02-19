# Demo 45: Informal Debrief

**Date:** 2026-02-18

## What We Actually Did

Demo 27 showed that if you train a complex-valued neuron to compute XOR, the trained weights decompose almost perfectly into Kauffman bracket values — RMS < 0.1 with gauge correction. That was exciting but it was one function. The obvious question was: is XOR special, or does the bracket algebra have a deeper relationship to computation?

Demo 45 answers this by running the same decomposition on all 13 non-trivial NPN equivalence classes of 3-input Boolean functions. Every function a 3-input Boolean circuit can compute, modulo symmetry. Train a network for each one, decompose the weights, rank by quality.

## The Headline

**Spearman rho = -0.8167 between topology frequency and decomposition RMS.**

This is the result that matters most, and it took two separate demos measured from opposite directions to produce it. Demo 23 asked the *forward* question: if you enumerate braids and compute their truth tables, which Boolean functions appear most often? Demo 45 asked the *reverse* question: if you train a network to compute a Boolean function, how well do the weights decompose back into bracket values?

These are completely independent measurements. One enumerates braids. The other trains neural networks. They have no shared code, no shared methodology, no reason to agree — unless the bracket algebra genuinely has intrinsic computational structure. And they agree almost perfectly. The functions that topology "likes" (produces frequently) are exactly the functions whose trained weights decompose cleanly back into bracket values.

This is probably the strongest single piece of evidence for the realist thesis across the whole demo arc. Bracket values don't just *happen* to compute — computation is an algebraic property of the bracket, measurable from either direction.

## The Two Failures That Taught More Than The Successes

**P3 (braid length)** predicted that well-decomposing functions would match short braids. Wrong — average matched braid length was 5.58 for the top tier, not <= 4. But this failure is illuminating: what matters is the bracket *value*, not the braid that produces it. Many different braids of different lengths produce the same algebraic invariant. The bracket abstracts away braid complexity. In retrospect this is obvious — the whole point of a knot invariant is that it's invariant under deformation. But it's nice to see it confirmed empirically: the catalog has 8192 braids producing only 64 distinct bracket values. The algebra is doing heavy compression.

**P5 (phase richness)** was the real surprise. The prediction was that functions using more distinct phases in their bracket decomposition would decompose better — richer topological structure, richer decomposition. The opposite is true, with r = +0.87. Fewer phases = better decomposition.

This one took a minute to understand, but once it clicked it became arguably the deepest insight from the whole experiment. Here's why: a function whose trained weights cluster around only 2-3 lattice directions has found a *low-dimensional* solution. It's using a small subspace of the Z[zeta_8] lattice. That's easier to match because there are fewer degrees of freedom to go wrong. A function whose weights sprawl across 8+ lattice directions is trying to use the full complexity of the lattice, and the nearest-neighbor matching degrades.

The implication is that topologically natural functions have *simpler* algebraic representations. They don't need the whole lattice — they live in a subspace. Topologically unnatural functions sprawl.

## Parity: The Anti-Function

XNOR3 (parity) is the only function whose decomposition is genuinely *worse* than random. RMS = 0.9963 vs the random baseline of 0.8086. It's also the only function with topology frequency exactly 0 — braids never produce it.

This is striking because parity has maximum symmetry (smallest NPN orbit, size 2). The orbit-stabilizer prediction (P6) said more symmetry = better decomposition. Parity violates this completely. Symmetry is necessary but not sufficient — what matters is *topological* naturalness, and parity is the canonical topologically unnatural function.

The QM lens explains why. The bracket is a partition function — a sum over states. Partition functions naturally produce threshold-like behavior: smooth boundaries, majority-vote decisions. That's why BUF, AND, and MAJ decompose best. Parity is the anti-threshold function. It requires exact cancellation across all configurations — every path must contribute with precise phases to get the destructive interference pattern that parity demands. The bracket's sum-over-states produces constructive interference naturally but struggles with the exact zeros parity requires.

This connects to a classical result in neural network theory: parity is the hardest function for perceptrons, the canonical example requiring hidden layers. Demo 45 suggests it's hard for a much deeper algebraic reason — it's hard for *partition functions generally*.

## The Scaling Challenge

One thing that was counterintuitive: the amplitude-only RMS (just matching magnitudes, ignoring phase) is often *better* than the full gauge-corrected complex RMS. For BUF: amp_RMS = 0.40 vs cx_RMS = 0.53.

In Demo 27 (2-input XOR, 3 complex weights), the gauge sweep worked beautifully because one global rotation could align all weights simultaneously. In Demo 45 (3-input, 12 complex weights), different neurons' weights may need different rotations. The single global U(1) sweep can't satisfy all of them at once.

This is the clearest signal pointing toward per-neuron gauge correction as the next step. Instead of one angle for all 12 weights, use 3 angles — one per hidden neuron's weight group. This turns a 1D optimization into a 3D one, which is still cheap computationally but should dramatically improve mid-ranking functions where the mismatch is coming from different neurons sitting at different lattice orientations.

## Cross-Lens Insights

The lenses produced some genuinely interesting crossover:

**Coding theory + approximation theory:** The decomposition is literally the Closest Vector Problem (CVP) in Z[zeta_8], with the bracket constellation as a fixed codebook. But unlike classical vector quantization where you design the codebook for your source, here the codebook is *algebraically determined* — it's whatever the bracket polynomial produces. The reversed P5 finding is a *sparsity* result in compressed sensing terms: weight vectors that are sparse in the bracket basis are easier to recover. The topology-preference boundary is interpretable as a channel capacity threshold — functions on the good side are "below capacity" for the bracket channel, functions on the bad side exceed it.

**QM lens:** The sharp RMS gap between rank 8 and rank 9 (the biggest gap in the ranking) cleanly separates functions with nonzero topology frequency from those with zero. This looks like a phase transition — there's a qualitative boundary, not just a gradual degradation. On one side the partition function can express the computation, on the other it can't.

**The gauge angle clustering (P7):** 5/9 optimal gauge angles fall near multiples of pi/4. This is exactly what you'd expect if the trained weights are "trying" to align with Z[zeta_8] = Z[e^{i*pi/4}] — the lattice has 8-fold rotational structure and the gauge sweep finds it. For badly-decomposing functions the optimal angles are arbitrary, meaning there's no lattice structure to find.

## What This Means Going Forward

Five natural next demos fell out:

1. **Per-neuron gauge** (Demo 46) — the most obvious improvement, should be quick to implement
2. **Parity investigation** (Demo 47) — is parity's resistance architectural (split-sigmoid) or algebraic (bracket can't express it)? Test with Aizenberg's MVN activation. If parity still resists with MVN, that's a deep algebraic fact.
3. **4-input function zoo** (Demo 48) — 222 NPN classes. Does the correlation hold at scale? Blocked on Demo 35 (TL matrices).
4. **Expanded catalog** (Demo 49) — more bracket values from more braids. Do topology-invisible functions improve, or are they fundamentally resistant?
5. **Multi-layer DKC** (Demo 50) — two hidden layers. This connects to the "compiler" vision — compiling multi-layer computations into braid words.

The big open question is scaling. Demo 29 showed the forward direction works exactly. Demo 45 shows the reverse direction works approximately but with strong structure. The gap between "approximate with structure" and "exact" is where the compiler lives — and per-neuron gauge, expanded catalogs, and multi-layer decomposition are all paths toward closing that gap.

The realist thesis keeps getting stronger: bracket values compute because of what they *are*, not because of how we use them. Demo 45 adds the strongest evidence yet — the same structure, measured from opposite ends, gives the same answer.
