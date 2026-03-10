# Group B (Tokenization & Representation) -- Knotapel Relevance Map

Date: 2026-02-26

Cross-referencing H-Net (dynamic chunking), CALM (continuous vectors), and Free Transformer (latent variables) against 109 DKC demos, the five-pillar synthesis, and the reservoir computing interpretation.

---

## Connection 1: H-Net Dynamic Chunking <-> DKC Hierarchical Compression Pipeline

### Paper Idea
H-Net replaces fixed tokenizers with learned, content-aware dynamic chunking via cosine similarity routing. Boundaries emerge where adjacent encoder outputs disagree (low cosine similarity). The architecture is hierarchical (U-Net style): bytes -> sub-words -> phrases, with the bulk of parameters operating on the compressed inner representation.

### Knotapel Result
DKC's pipeline is a concrete instance of hierarchical compression:
- **Braid word -> TL algebra -> bracket value -> Boolean function** is a sequence of compressions (D01-D09 -> D10 -> D29 -> D45-D50).
- The numerical pivot at D10 is literally a 50x compression: `Poly` (808 bytes/entry) replaced by `Cx` (16 bytes/entry), with the expensive computation operating on the compressed representation.
- The depth law (Demo 82, max_xor ~ depth + 6) shows that computational capacity scales linearly with the number of compression stages (generator multiplications / crossings), not with raw vocabulary size. Deep entries with 476 directions outperform strided entries with 512 directions because they share algebraic structure from their common BFS ancestry -- they are "semantically chunked."

### Why This Is Not Surface-Level
The connection is structural, not metaphorical. H-Net's core insight is that learned boundaries should emerge where the content changes (cosine similarity drops). DKC's BFS closure produces natural "boundaries" at each depth round: the value set grows exponentially (~2x per round for 2-strand), but the computational power grows linearly. The depth round IS the natural chunk boundary. Entries born at the same BFS depth share intermediate algebraic products (D90: "algebraic coherence beats raw vocabulary"), exactly as H-Net's chunks share encoder hidden states.

The ratio loss in H-Net (preventing trivial compression solutions) has an analog in DKC: the finite group ceiling (D79-D80). At zeta_8, the binary octahedral group's 24 elements cap the catalog, preventing further compression gains. Moving to zeta_12 (infinite group) removes this ceiling -- analogous to relaxing H-Net's compression ratio constraint.

### Relevant Demos
- **D82** (crossing depth law): the depth = compression stage mapping
- **D90** (axis cancellation mechanism): WHY deeper "chunks" are more computationally coherent
- **D81** (logarithmic scaling law): compression ratio vs capacity tradeoff
- **D10** (numerical engine): the literal 50x compression pivot

### How Existing Code Could Be Leveraged
The BFS closure infrastructure (present in D79, D82, D94, D99-D105) already implements a hierarchical compression where each round produces a new "resolution level" of the catalog. The depth-stratified catalog (D82's `load_up_to_depth(max_d)`) is directly analogous to H-Net's multi-stage encoder output. One could build an H-Net-style routing module that uses the cosine similarity of Z[zeta_8] values (via `cyc8_to_cx()` and the existing float infrastructure) to automatically determine which BFS depth to truncate at for a given computational task. The Raqiya library's `raq_detect_norm_classes()` already partitions values by magnitude -- an algebraic analog of H-Net's learned boundaries.

---

## Connection 2: CALM Continuous Vectors <-> Z[zeta_8] as a Continuous Latent Space

### Paper Idea
CALM replaces discrete next-token prediction with continuous next-vector prediction. K discrete tokens are compressed by a VAE into a continuous vector z in R^l. The language model operates on these continuous vectors, predicting the next vector via an energy-based generative head (single-step, no iterative denoising). The variational regularization (beta=0.001) ensures a smooth, navigable latent space.

### Knotapel Result
Z[zeta_8] = {a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3 : a,b,c,d in Z} is a 4-dimensional integer lattice that already functions as a continuous-like representation space for DKC:
- Each bracket value is a point in R^4 (after embedding via `cyc8_to_cx()`), encoding the complete algebraic content of a braid.
- The 100-value catalog at delta=0 is a discrete codebook, but the additive weight sums S_k = S_{k-1} + w_k (D73) traverse a CONTINUOUS trajectory through the Z[zeta_8] lattice. The 27 distinct partial sums at step 5 (D73) are points in this continuous space.
- The sqrt(n) magnitude quantization (D75: magnitudes take values {0, 1, sqrt(2), sqrt(3), 2, sqrt(6)}) means the continuous trajectory is constrained to lie on specific shells -- exactly the kind of structured latent geometry that CALM's VAE regularization aims to produce.
- The Dual-Channel Theorem (D108): parity requires BOTH phase coherence (product closure, the multiplicative channel) AND magnitude diversity (v_2 connectivity, the additive channel). These two channels are naturally decomposed in Z[zeta_8] as phase (which zeta_8^k axis) and magnitude (integer coefficient). This is the cyclotomic analog of CALM's amplitude/phase decomposition in continuous vectors.

### Why This Is Not Surface-Level
CALM's key challenge is that continuous vector prediction lacks a natural likelihood function (no softmax). DKC faces the identical problem: the bracket value is a point in Z[zeta_8] (continuous, no natural probability), and the computational question is whether a specific combinatorial property (parity) holds. CALM solves this with the energy score; DKC solves it with the activation function (k-sector MVN, Voronoi partition).

The deep parallel is that BOTH systems discover that the "right" representation is NOT the obvious discrete one (tokens for CALM, truth tables for DKC) but rather a continuous embedding where computation becomes geometric. CALM embeds tokens into R^128; DKC embeds Boolean functions into R^4 (Z[zeta_8]) and then onto S^2 (Demo 67-71). The 13=13 theorem (D71: minimum bandwidth l=6 because 2*6+1=13 = number of eigenvector directions) is precisely the statement that the continuous representation has the minimal dimensionality needed for the computation -- the DKC analog of CALM finding l=128 optimal for K=4.

### Relevant Demos
- **D29** (forward DKC): Z[zeta_8] as the weight space where computation lives
- **D73** (eigenvector automaton): additive trajectory through Z[zeta_8] = continuous vector accumulation
- **D75** (angle subsumes cell): the S^1 x S^2 "continuous" activation outperforms discrete Voronoi
- **D108** (dual-channel theorem): multiplicative (phase) + additive (magnitude) = complete representation
- **D71** (13=13 theorem): minimum dimensionality of the continuous representation
- **D107** (Z/4Z axis-alignment): Entry = integer * zeta_8^k, so the "continuous" space is actually Z x Z/4Z -- a hybrid discrete-continuous structure

### How Existing Code Could Be Leveraged
The cyclotomic ring arithmetic (`cyc8_add`, `cyc8_mul`, `cyc8_to_cx`) is already a complete autoencoder in CALM's terms: it encodes braid words into 4-integer vectors and decodes them back to complex numbers for computation. The Raqiya library (`raq_vs_insert`, `raq_analyze`) provides the analysis infrastructure for understanding the latent space geometry. The BrierLM evaluation metric from CALM could be implemented using the existing parity-checking infrastructure: sample bracket values, check parity separability, compute Brier score.

The CALM autoencoder's context-free chunk encoding has a direct implementation path: train a small network that maps sequences of K braid generators to a single Z[zeta_8] value, using the exact bracket computation as ground truth. This would be a "braid autoencoder" -- compressing generator sequences into cyclotomic integers and decompressing back. The variational regularization would ensure smooth interpolation between braid words in the Z[zeta_8] latent space.

---

## Connection 3: CALM Energy Score <-> DKC Activation Function Selection

### Paper Idea
CALM's energy score S(P,y) = E[||x'-x''||^alpha] - 2E[||x-y||^alpha] is a likelihood-free scoring rule that balances diversity (first term: penalizes collapsed predictions) against accuracy (second term: predictions close to truth). It operates purely on samples -- no density evaluation needed.

### Knotapel Result
The DKC activation function selection problem is structurally identical: given a fixed Z[zeta_8] weight catalog, which activation function best separates parity? The project's history is a search through activation space:
- Split-sigmoid: 0/100M parity solutions (D48)
- k-sector MVN at k=6: 906 parity solutions (D50)
- S^2 Voronoi (14 cells): 36 XOR6 solutions (D67)
- S^1 x S^2 product (112 cells): 6 XOR8 solutions (D77)
- K-ladder: 14x better perturbation resilience than Voronoi-only (D88)

The k-ladder activation (D88, D91) is effectively an energy-based evaluation: it tries multiple resolutions {k=6, k=12, k=24} and takes the best, balancing coverage (diversity of sector boundaries) against precision (avoiding pigeonhole collapse). The ratio loss analog: too few cells = pigeonhole (everything in one cell), too many cells = trivialization (everything trivially separated, D69's 90% at 3457 directions).

### Why This Is Not Surface-Level
The energy score's key property is that it is a STRICTLY PROPER scoring rule -- it is uniquely minimized by the true distribution. The DKC activation selection problem has an analogous "strict propriety": the algebraically-derived 13-direction Voronoi is the UNIQUE configuration that maximizes XOR6 solutions while maintaining physical meaning (D72: any geometrically "better" configuration destroys computation). The anti-correlation between geometric quality and computational quality (D72: t=0 design with 36 solutions vs t=1 design with 4 solutions) is precisely the statement that the DKC "scoring rule" is NOT the obvious geometric one (spherical design quality) but rather an algebraic one.

### Relevant Demos
- **D48/D50** (parity wall / resolution): activation as the variable that determines computability
- **D72** (spherical design anti-correlation): the "right" activation is algebraically, not geometrically, motivated
- **D77** (S^1 x S^2 product): combining channels = the multi-sample analog
- **D88** (k-ladder): the multi-resolution evaluation that echoes energy score's multi-sample approach
- **D91** (activation bottleneck): parity is the wall, not the activation -- analogous to CALM's finding that alpha in [1, 1.75] is all that matters

### How Existing Code Could Be Leveraged
The `test_bool_func()` infrastructure from D92 (which tests whether a weight tuple separates truth-table classes under a given activation) is essentially a DKC-specific scoring function. It could be generalized into an energy-score-like framework: sample N weight tuples, compute parity separability for each, and use the energy score to select the activation that best balances diversity (covering multiple parity solutions) and accuracy (correctly classifying all masks). The `sector_of()` / `voronoi_of()` decomposition from D92 provides the channel decomposition needed for multi-channel energy scoring.

---

## Connection 4: Free Transformer Latent Variables <-> DKC Encoding as Latent Structure

### Paper Idea
The Free Transformer conditions autoregressive generation on explicit latent variables Z, sampled from a uniform prior over 2^H values. Z captures global properties (topic, logical mode, structure) that standard autoregressive models must implicitly infer from the token stream. The coin-flip example: once you know the hidden state Z, the tokens follow simply; without Z, you need an increasingly complex posterior calculation. The binary mapper with H bits gives 2^H possible latent values, and kappa controls how much information flows through Z (1/2 to 1 bit per token is the sweet spot).

### Knotapel Result
DKC's encoding scheme is EXACTLY a latent variable that determines the computational mode:

**The Parity-Lock Theorem (D92)** proves that the +/-q paired encoding structurally locks computation to XOR/XNOR -- AND/OR/MAJ are provably impossible under ANY activation. The encoding IS the latent variable Z. Changing the encoding (from +/-q to 1-weight-per-input) changes the accessible Boolean function space from {XOR, XNOR only} to {all 13 NPN classes} (D50). The encoding determines the function; the weights and activation fill in the details.

More precisely:
- **Z = encoding choice** (binary: +/-q vs 1wpi)
- **+/-q encoding -> Z_parity**: the equivalence class structure (3^k classes, D92 Phase 1d) means XOR/XNOR are constant on all classes while AND/OR/MAJ are not. This is the coin-flip example: once you know the encoding (Z), the truth table follows simply.
- **1wpi encoding -> Z_general**: all 13 NPN classes accessible, but the activation geometry now determines which are computable (D50's k=6 for parity, D61-62's 11/13 for half-plane).

The Free Transformer's kappa parameter (controlling information rate through Z) has a direct analog in DKC: the sector count k. At k=2 (maximum compression), 5 NPN classes are accessible. At k=6 (+1 class: parity). At k=8 (+6 more). The information flowing through the "activation channel" is kappa-controlled in the same way.

### Why This Is Not Surface-Level
The Free Transformer's core insight -- that standard autoregressive models waste capacity re-inferring latent structure from the token stream -- maps precisely onto the DKC finding that training wastes capacity re-discovering topological structure. D29's central result is that training is not CREATING computational structure; it is LOCATING pre-existing solutions on the cyclotomic lattice. The Free Transformer makes latent decisions explicit and up-front; DKC makes weight provenance explicit (braid word -> bracket value) instead of discovered (gradient descent -> learned weight). Both eliminate an implicit inference step.

The binary mapper (H independent Bernoulli bits -> one-hot over 2^H values) is structurally parallel to D107's constructive Z/4Z phase formula: Entry(r,c) = (integer path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4}. Three binary variables (writhe parity, nesting of source, nesting of target) determine the cyclotomic phase -- a 3-bit latent code (2^3 = 8 possible phases, but only 4 realized because of mod 4 reduction). This is a Z/4Z latent variable controlling the "logical mode" of each matrix entry.

### Relevant Demos
- **D92** (parity-lock theorem): encoding = latent variable that determines function class
- **D50** (k-sector MVN): sector count = information rate through the "activation channel"
- **D107** (Z/4Z phase formula): 3-variable latent code determines matrix entry phase
- **D93** (complement-blindness): sign-flip symmetry = a second latent constraint
- **D29** (forward DKC): training-as-focusing = latent variables as pre-existing structure
- **D94** (five-pillar synthesis): DKC as reservoir computer where encoding = readout design

### How Existing Code Could Be Leveraged
The truth-table generators from D92 (`make_xor_tt`, `make_and_tt`, `make_or_tt`, `make_maj_tt`) combined with `test_bool_func()` already implement a system where changing a discrete variable (the truth table = the target function = the latent Z) changes the computation. A Free Transformer-style architecture for DKC would:
1. Use the encoder to infer Z from a braid word (which NPN class does this braid compute?)
2. Use the decoder to generate the Boolean truth table conditioned on Z
3. The binary mapper would map (encoding, activation_k, root_of_unity) to a discrete latent code

The infrastructure for this exists across demos: D23's NPN classification code provides the encoder target, D92's equivalence class analysis provides the decoder's ground truth, and the cyclotomic ring arithmetic provides the latent space.

---

## Connection 5: DKC as a Discrete Algebraic Reservoir Computer <-> All Three Papers' Architectural Principles

### Paper Ideas (Combined)
The "Eigenflower Stack" proposed in the Group B synthesis combines:
1. H-Net's hierarchical spatial compression (6x fewer positions)
2. CALM's representational compression (4x fewer autoregressive steps)
3. Free Transformer's structural compression (3% overhead for significant quality gains)

Together: 24x compression, three orthogonal axes.

### Knotapel Result
DKC IS a concrete, exactly-analyzable instance of this three-axis compression, as formalized in the five-pillar synthesis (D94):

1. **Spatial compression (H-Net axis)**: BFS closure compresses the infinite braid group B_n into a finite catalog. At zeta_8, 87,890 enumerated braids produce only 24 quaternionic states (D66) -- a 3,662x compression. The depth law (D82) shows this compression preserves computational capacity linearly.

2. **Representational compression (CALM axis)**: Each bracket value compresses a braid word (variable length, up to 8 generators) into a single Z[zeta_8] integer (4 coefficients). The Z/4Z axis-alignment (D107) further compresses this to Z x Z/4Z per matrix entry -- an integer magnitude and a 2-bit phase code. This is literally CALM's "K tokens -> one vector" but with K = braid word length and one vector = one cyclotomic integer.

3. **Structural compression (Free Transformer axis)**: The encoding (D92) determines the global structure (which Boolean functions are computable) with minimal overhead. The Parity-Lock Theorem shows that a single binary choice (encoding = +/-q or 1wpi) determines the entire function space -- 1 bit of latent information controls which of 2^{2^N} Boolean functions are reachable. This is the Free Transformer's latent Z with kappa at its theoretical minimum.

### The Reservoir Computing Mapping (D94)
The five-pillar synthesis makes this precise:

| Reservoir Computing Component | DKC Realization | Group B Paper Analog |
|------|------|------|
| Fixed reservoir | Z[zeta_8] bracket catalog (24 quaternions) | H-Net encoder (fixed SSM weights) |
| Reservoir connectivity | Cayley graph of binary octahedral group | H-Net U-Net skip connections |
| Linear readout | Signed weight sum S = sum(+/-q_i) | CALM linear projection to latent z |
| Nonlinear output | k-sector / Voronoi activation | CALM energy-based generative head |
| Memory depth | BFS crossing depth (D82: max_xor ~ depth+6) | H-Net recursive nesting depth |
| Reservoir dynamics | Additive, not multiplicative (D73: 82.8% determinism) | CALM autoregressive accumulation |
| Separation property | Null entries maintain S^2 vocabulary (D84) | CALM VAE regularization (smooth manifold) |
| Channel independence | Writhe + depth + lattice are additive (D83) | Free Transformer Z independent of token stream |

### Why This Is Not Surface-Level
This is not an analogy -- it is a mathematical identification. The DKC system satisfies the formal definition of an Echo State Network (Jaeger 2001): a fixed dynamical system (the braid group acting on Z[zeta_8] modules) with a linear readout (additive weight sum, D73) and a nonlinear output (activation function). The fading memory property (D73's 82.8% universal determinism) quantifies the memory-computation tradeoff. The separation property (D84's null entries maintaining S^2 coverage) ensures the reservoir has rich enough dynamics to support diverse readouts.

The three Group B papers each address one limitation of standard reservoir computing:
- H-Net addresses the INPUT problem (how to get variable-length input into fixed-size reservoir): DKC solves this via BFS closure, which maps variable-length braid words to fixed-size catalogs.
- CALM addresses the OUTPUT problem (how to produce variable-length output from fixed-size state): DKC solves this via the activation function, which maps cyclotomic sums to Boolean truth tables.
- Free Transformer addresses the LATENT STRUCTURE problem (how to condition on global properties): DKC solves this via the encoding, which determines the accessible function space.

### Relevant Demos
- **D94** (five-pillar synthesis): formal reservoir computing mapping
- **D73** (eigenvector automaton): 82.8% determinism = fading memory quantification
- **D82** (depth law): memory depth = crossing depth
- **D83** (framing resource): independent channels = independent reservoir dimensions
- **D84** (null states): separation property maintenance
- **D108** (dual-channel theorem): two-channel readout structure

### How Existing Code Could Be Leveraged
The entire knotapel codebase IS a reservoir computer implementation. To build the Group B "Eigenflower Stack" using DKC as the architectural skeleton:
1. **H-Net input layer**: replace BPE tokenizer with BFS closure as the "learned chunking" -- the braid group already provides content-aware hierarchical segmentation
2. **CALM latent space**: use Z[zeta_8] as the continuous latent space (4D integer lattice with smooth S^2 projection via quaternionic embedding)
3. **Free Transformer conditioning**: use the encoding choice (D92's encoding determines function) as the explicit latent variable Z
4. **Energy head**: use the k-ladder activation (D88) as the single-step nonlinear output

---

## Connection 6: H-Net's SSM Encoder Bias <-> DKC's Additive (Not Multiplicative) Nature

### Paper Idea
H-Net finds that SSM (State Space Model) layers dramatically outperform Transformers for the encoder/decoder role because SSMs have a natural "compression bias" -- they compress information into fixed-size states. Even on BPE-tokenized inputs, SSM encoders outperform Transformer ones (Figure 10, p19).

### Knotapel Result
D73 proves that DKC is fundamentally additive, not multiplicative. The multiplicative braid product formulation CANNOT compute XOR6 (every cell is maximally non-separating because complement-all-bits sends M to M^{-1}, which shares the same eigenvector cell). Only the additive weight sum S_k = S_{k-1} + w_k computes parity.

This additive accumulation IS a state space model: the hidden state is the partial sum S_k in Z[zeta_8], and the transition function is S_k = S_{k-1} + w_k where w_k is selected by the input bit. The 82.8% universal determinism (D73) measures the quality of this state compression -- the S^2 Voronoi cell captures 82.8% of the state information while discarding magnitude.

The SSM's "compression bias" corresponds to DKC's magnitude discarding: the Voronoi cell retains direction (13 cells) but loses magnitude, exactly the "fixed-size state compression" that makes SSMs effective. The remaining 17.2% non-determinism IS the compression loss.

### Relevant Demos
- **D73** (additive not multiplicative): DKC's SSM-like accumulation
- **D75** (angle subsumes cell): the S^1 angle channel = the SSM's phase state
- **D107** (Z/4Z phase formula): the 2-bit phase code = the SSM's minimal state

---

## Connection 7: Free Transformer's Split Decoder <-> DKC's Activation-Determines-Capacity

### Paper Idea
The Free Transformer splits the decoder at its midpoint: the first L/2 layers build context-independent representations, then Z is injected at layer L/2+1 and the second half conditions on the latent structure. This means the first half builds representations BEFORE knowing the global mode, and the second half specializes.

### Knotapel Result
DKC has an exact structural parallel:
1. **First half (context-independent)**: BFS closure builds the Z[zeta_8] catalog without knowing what function will be computed. The catalog is the same regardless of encoding, activation, or target function. This is the "representation building" phase.
2. **Z injection (encoding choice)**: The encoding (+/-q or 1wpi) is chosen, restricting the accessible function space (D92: parity-lock theorem). This is the latent variable injection.
3. **Second half (conditioned generation)**: The activation function (k-sector at specific k, or S^1 x S^2 product, or k-ladder) determines which specific Boolean function is computed from the catalog. This is the "conditioned generation" phase.

The critical insight is that the catalog (first half) is REUSABLE across different encodings and activations, just as the Free Transformer's first L/2 layers produce representations usable under any Z value.

### Relevant Demos
- **D92** (parity-lock theorem): encoding injection = Z injection
- **D50** (k-sector activation): activation = conditioned generation
- **D77** (S^1 x S^2 product): combining channels after Z injection
- **D94** (reservoir computing): the fixed reservoir (first half) + readout (second half) decomposition

---

## Connection 8: CALM's Discrete Feedback Loop <-> DKC's Exact Arithmetic Requirement

### Paper Idea
CALM discovers that at inference, the predicted continuous vector must be decoded to K discrete tokens which are then re-embedded as input for the next step. Using continuous vectors directly as input DEGRADES performance (Table 5, p19) because the latent space is too compact/brittle for the Transformer to unpack.

### Knotapel Result
DKC's progression from approximate to exact arithmetic is the same discovery:
- **D10-D28** (floating-point): approximate methods discover phenomena
- **D29+** (Z[zeta_8] exact): exact methods prove them
- **D48**: the 100M-quartet exhaustive search REQUIRES exact equality testing. Floating-point comparison would produce false positives at the sector boundaries.
- **D64**: exact arithmetic eliminates 420 spurious failures from floating-point association-order errors near sector boundaries.

The lesson is identical: working in the "continuous" representation (floating-point for DKC, continuous vectors for CALM) is useful for discovery but fails for rigorous computation. You must "ground" back to the discrete/exact representation (Z[zeta_8] integers for DKC, discrete tokens for CALM) to maintain correctness. The DKC system literally implements CALM's discrete feedback loop: compute in Z[zeta_8] (the "latent space"), decode to Boolean truth table (the "tokens"), use the truth table to verify correctness.

### Relevant Demos
- **D29** (exact Z[zeta_8]): the transition from continuous to exact = CALM's discrete feedback
- **D48** (100M exhaustive search): exact equality as computational necessity
- **D61-D62** (11/13 proof): zero floating-point in the proof = pure discrete verification
- **D64** (matroid minor chain): exact arithmetic eliminates 420 spurious errors

---

## Connection 9: CALM's K as a New Scaling Axis <-> DKC's Sector Count as a Scaling Axis

### Paper Idea
CALM shows that K (tokens per vector) is a new scaling axis orthogonal to model size: increasing K from 1 to 4 provides dramatic efficiency gains. K=8 degrades quality at current model sizes.

### Knotapel Result
DKC's sector count k is an exactly analogous scaling axis:
- k=2: 5/13 NPN classes (D50)
- k=3: +1 class (6/13)
- k=4: +6 classes (12/13)
- k=6: +1 = parity (13/13)
- k=8: solution count DROPS (906 -> 96, D50) -- non-monotonic, anti-correlated with gcd(k,8)
- k=15: XOR5 accessible (3020 solutions, D63)

The non-monotonicity (k=6 is better than k=8 for 3-input parity) parallels CALM's finding that K=8 degrades quality. Both systems have a sweet spot determined by the interaction between the compression granularity (K for CALM, k for DKC) and the underlying algebraic structure (vocabulary size for CALM, Z[zeta_8] lattice alignment for DKC).

### Relevant Demos
- **D50** (k-sector activation): the k scaling axis
- **D63** (parity ceiling): k requirements increase with input arity
- **D65** (algebra-independence): k thresholds are the same across Z[zeta_8] and Z[zeta_16]

---

## Summary Table

| Group B Idea | Best Knotapel Match | Strength | Key Demos |
|------|------|------|------|
| H-Net dynamic chunking | BFS depth rounds as natural chunk boundaries | STRONG (structural) | D82, D90, D81 |
| H-Net hierarchical U-Net | Braid -> TL -> bracket compression pipeline | STRONG (structural) | D01-D10, D29 |
| H-Net SSM compression bias | DKC is additive, not multiplicative (SSM-like) | STRONG (functional) | D73, D75, D107 |
| H-Net ratio loss | Finite group ceiling prevents trivial compression | MODERATE (analogical) | D79, D80, D69 |
| CALM continuous vectors | Z[zeta_8] as continuous latent space | STRONG (mathematical) | D29, D73, D108 |
| CALM energy score | Activation function selection / k-ladder | MODERATE (functional) | D72, D88, D91 |
| CALM discrete feedback | Exact arithmetic requirement (float -> Z[zeta_8]) | STRONG (structural) | D29, D48, D64 |
| CALM K as scaling axis | Sector count k as scaling axis | STRONG (quantitative) | D50, D63, D65 |
| CALM autoencoder | Braid word -> Z[zeta_8] compression | STRONG (mathematical) | D29, D107 |
| Free Transformer latent Z | Encoding choice determines function class | VERY STRONG (proven) | D92, D50, D93 |
| Free Transformer split decoder | Catalog (fixed) + activation (conditioned) | STRONG (structural) | D92, D94 |
| Free Transformer binary mapper | Z/4Z phase formula (3-bit latent code) | STRONG (mathematical) | D107 |
| Free Transformer kappa | Sector count k = information rate control | STRONG (quantitative) | D50, D63 |
| Combined 3-axis compression | Five-pillar RC synthesis matches all 3 axes | VERY STRONG (mathematical) | D94, D73, D82, D83, D84 |
| Speculative decoding analogy | BFS: small encoder (every entry) + big network (deep entries only) | MODERATE (structural) | D82 |

---

## Highest-Value Opportunities

### 1. Use DKC as an Exactly-Analyzable Test Bed for Group B Architectures
DKC provides something no other system can: exact, zero-error computation with fully characterized algebraic structure. The three Group B papers propose architectures that are validated empirically (perplexity, BrierLM). DKC could provide mathematical proofs of WHY these architectures work:
- Why does hierarchical chunking help? Because algebraic coherence (shared BFS ancestry) beats vocabulary diversity (D82 proven, D90 mechanism identified).
- Why does continuous representation help? Because the cyclotomic lattice has the EXACT dimensionality needed (D71: 13=13 theorem, D75: sqrt(n) quantization).
- Why do latent variables help? Because encoding choice provably determines function space (D92: parity-lock theorem, PROVEN).

### 2. Build a "Cyclotomic Autoencoder" Using CALM's Architecture
Train a CALM-style autoencoder where the input is K braid generators and the latent vector is a Z[zeta_8] value. The decoder reconstructs the generators from the cyclotomic integer. Use the exact bracket computation as the ground-truth training signal. The variational regularization should discover the same algebraic structure that DKC already characterized.

### 3. Implement Free Transformer Binary Mapper Using D107's Phase Formula
D107's constructive formula Entry(r,c) = (integer) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4} is a natural 3-bit binary mapper. An encoder that infers (writhe_parity, nest_source, nest_target) from a braid word would be implementing the Free Transformer's encoder using DKC's algebraic structure. The decoder would use these 3 bits to determine the phase and generate the matrix entries.

### 4. DKC Reservoir as a Pre-Trained "Tokenizer Replacement"
H-Net eliminates tokenizers by learning content-aware boundaries. DKC eliminates training by providing topology-aware weights. The synthesis: use the DKC catalog as a pre-computed "tokenizer" for algebraic sequences, where the BFS depth provides the hierarchical structure and the Z[zeta_8] values provide the continuous embedding. This would be a tokenizer that comes from mathematics rather than corpus statistics.
