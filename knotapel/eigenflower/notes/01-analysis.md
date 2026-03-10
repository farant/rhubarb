# Eigenflower Analysis — Paper Extractions & Knotapel Crossover Review

Date: 2026-02-26

This is a review of the 10 extraction documents produced by parallel agent sweeps:
- 5 paper extraction files (groups A-E, covering 14 arXiv papers)
- 5 knotapel crossover files (groups A-E, cross-referencing extractions against 109 DKC demos)

---

## Part 1: Paper Extractions Review

### Group D Is the Beating Heart

The Tensor Logic paper (Domingos) and the Hilbert Space paper provide the most direct
theoretical foundation for eigenflower's core goal — compiling syllogisms to ANN weights.

The key equation is almost embarrassingly direct:

    SP[s,p] = H(SM[s,m] * MP[m,p])

That's Barbara. A syllogism compiles to a matrix multiply plus a step function. There is no
gap to bridge between "syllogism" and "ANN weight" — they are the same thing expressed in
two notations.

The Hilbert Space paper doubles down: syllogistic reasoning = operator composition, and you
can solve for the operators in closed form via kernel ridge regression. No iterative training.
You could potentially go from Summa text to compiled reasoning operators in seconds, not
minutes.

### The Practical Backbone: Group A

BitNet ternary weights {-1, 0, +1} mapping to affirm/deny/ignore is almost too perfect for
syllogistic logic. Combined with TRM's recursive refinement (7M params, beats billion-parameter
models on reasoning), you get a concrete architecture: a tiny ternary model that reasons by
iteratively applying the same compiled syllogistic transformation.

The "Ternary Recursive Orchid" proposal at the end of Group A is ambitious but the component
sizes (2-5M params, under 1MB) are realistic for M2.

### The Efficiency Envelope: Group C

This group establishes that the 30-60 minute training constraint is achievable. The key
number: 50-100M params is the sweet spot, with 75% token masking + low-precision training.

The GPTQ-as-CVP insight is sneakily important for goal 3 — it means "compile syllogism to
weights" is literally a Closest Vector Problem with known algorithms and provable error bounds.
That transforms a vague aspiration into a concrete mathematical optimization.

### The Representation Layer: Group B

Strongest for the "how do we handle text" question. Free Transformer's latent Z variable is
the most natural fit for syllogism compilation — Z encodes the logical mode (which syllogistic
figure, what kind of argument), decoder generates conditioned on it. CALM's continuous
autoencoder adds a second path: premises as continuous vectors, conclusion predicted by an
energy head. H-Net's tokenizer-free operation eliminates a dependency and handles Latin
morphology naturally.

The "three axes of compression" observation (spatial, representational, structural = 24x fewer
autoregressive steps) is a useful framing.

### The Training Discipline: Group E

More operational than architectural. The Inverse Knowledge Search paper's key insight: the
Summa is already an "uncompressed reasoning corpus" — it doesn't hide its derivations like
most text does. That's a huge advantage.

The Early Experience paper gives the training recipe: generate invalid syllogism variants as
contrastive examples, train in two stages (logical dynamics first, then theological content).
The 1/8 data efficiency result is encouraging for a small corpus.

### What's Missing / Pushback

1. The agents sometimes rated relevance generously. Orchid's "MEDIUM" for syllogism
   compilation is really a stretch — frequency-domain kernels for logical rules is speculative.

2. No one addressed the extraction problem concretely. Everyone talks about compiling
   syllogisms TO weights, but actually parsing syllogisms FROM the Summa's Latin prose is its
   own hard problem. The Summa is structured (sed contra, respondeo, etc.) but the actual
   logical structure within each article is implicit.

3. The model size estimates don't fully account for vocabulary. TRM works at 7M for grid
   puzzles with tiny vocabularies. Latin theological text has a much larger vocabulary, which
   inflates the embedding layer. A 7M-param model might not have enough capacity for language.

4. None of the agents seriously addressed whether these techniques have been combined before.
   Each paper in isolation is validated; the proposed combinations are novel and therefore
   unvalidated.

### Build Priority (Extractions Only)

1. Tensor logic interpreter (Group D, Paper 1) — parse syllogisms, execute as tensor
   equations, verify correctness. The foundation everything else sits on.
2. Kernel ridge regression reasoning operators (Group D, Paper 2) — closed-form, no training
   loop, instant syllogism compilation. Low risk, fast validation.
3. Ternary recursive backbone (Group A) — BitNet + TRM recursion, the actual neural model.
4. Free Transformer latent conditioning (Group B) — Z as syllogistic mode selector.
5. Everything else — masking, CALM, H-Net, diffusion — optimizations on top of a working core.

---

## Part 2: Knotapel Crossover Review

### Quality Varies — In an Interesting Way

The quality varies significantly across the five groups, and the variation correlates with
how mathematically precise the connections are versus how analogical.

### Group D Crossover Is Extraordinary

The agent who wrote this understood something crucial: the connections aren't analogies,
they're mathematical identities.

"S^2 spectral DKC = Hilbert space operators" isn't a metaphor — D71's spherical harmonic
decomposition of the XOR6 labeling function IS Paper 2's spectral reasoning framework
instantiated on a concrete S^2 function.

"DKC IS tensor logic over Boolean tensors" is similarly precise: the forward DKC computation
`Output[mask] = Activation(Weight[i] * Input[mask, i])` really is a tensor equation in
Domingos's formalism.

The refinement of the Edge of Chaos thesis is genuinely novel: ALGEBRAIC complexity (derived
series depth, solvability) predicts computation where DYNAMICAL complexity (topological
entropy) is completely orthogonal. D106 falsifies the naive application of Paper 3's
complexity measures to algebraic reservoirs. That's a publishable observation.

The synthesis section is the best part of any crossover file: four concrete things knotapel
gains from Group D papers (closed-form kernel regression for optimal activation, Tucker
decomposition for higher-order correlations, Krylov complexity as the right measure, Koopman
operators for the depth law) and four things the papers gain from knotapel (working
implementation, detailed spectral examples, algebraic complexity refinement, exact arithmetic
substrate). Balanced, specific, actionable.

### Group C Crossover Is the Most Actionable

Connection 1 (GPTQ-as-CVP on cyclotomic lattices) identifies a genuine gap in the literature:
"They notice quantization IS a lattice problem but haven't made the leap to non-standard
lattices."

The E8 lattice quantization idea has a concrete implementation path — the icosian ring code
exists, Babai's algorithm is known, what's needed is the bridge (Hessian computation + LDL
decomposition in icosian coordinates). Potentially publishable as an independent result:
"CVP-optimal weight quantization on cyclotomic lattices."

The "3.125 bits as natural information density" parallel with the optimal z8 catalog of 21
(not 24) entries is a nice structural observation — both demonstrate that the natural
resolution is smaller than naively expected, and that removing certain elements actively
improves performance.

### Group A Crossover Is Solid, Occasionally Overstates

Connection 1 (BitNet ternary <-> Z/4Z axis-alignment) is genuinely deep — axis-aligned
entries decompose as integer magnitude + 2-bit phase, structurally isomorphic to ternary on
4 axes.

Connection 5 (TRM recursion <-> depth law) is the strongest: both discover that recursive
application of the same small transformation beats wider/bigger single applications, with
"algebraic coherence beats vocabulary" as the underlying mechanism.

Connection 8 (BitNet+TRM combined <-> DKC as reservoir) is labeled "DEEPEST" and I'd agree —
DKC provides the theoretical framework for why a ternary recursive model should work, via the
five-pillar synthesis. The solvability bottleneck prediction (that DEQ models fail because
fixed-point convergence = solvable dynamics, and solvable groups can't compute parity by
Barrington) is a testable and striking claim.

Where Group A overstates: Connection 3 (Orchid <-> eigenvector automaton) tries to map phase
suppression to Hopf decomposition. Conceptual parallel, not mathematical identification.
Connection 9 (Orchid cross-attention <-> polarity inversion) is reaching.

### Group B Crossover Does the Most Interesting Architectural Synthesis

The "DKC IS the Eigenflower Stack" table mapping reservoir computing components to Group B
paper analogs is clever and structurally illuminating. The identification of DKC as
implementing all three compression axes (spatial via BFS closure, representational via
Z[zeta_8] embedding, structural via encoding choice) is convincing.

The strongest specific connection: Free Transformer latent Z <-> encoding choice. The
parity-lock theorem (D92) proves that the encoding IS a binary latent variable that determines
the entire accessible function space. That's exactly what the Free Transformer's Z does, and
D107's constructive phase formula is literally a 3-bit binary mapper (writhe parity + 2
nesting bits -> Z/4Z phase).

Connection 8 (CALM discrete feedback <-> exact arithmetic requirement) is more insightful
than it looks on first pass. Both CALM and DKC discover that you must "ground" back to the
discrete/exact representation for rigorous computation — continuous is good for exploration,
discrete is necessary for correctness.

### Group E Crossover Is the Weakest

The connections are more conceptual than mathematical. "Forward DKC IS inverse knowledge
search" is a valid framing but doesn't give you new computational tools. "DKC naturally
decomposed into IWM-like and SR-like phases" is a retrospective observation about the
research program's methodology, not a technical connection that enables new work.

The strongest Group E point: Connection 8 (verifiable endpoints + exact arithmetic). DKC's
massive advantage is that verification is trivially exact. Does this weight triple compute
XOR? Compare four integers. Done. No hallucination risk, no cross-model disagreement.

---

## Part 3: What It All Adds Up To

### The Central Observation

DKC is not just "relevant to" these papers — DKC is a concrete, exactly-analyzable instance
of the systems these papers describe theoretically. Tensor logic, Hilbert space operators,
reservoir computing, compressed sensing, edge-of-chaos dynamics — DKC already implements all
of these with exact arithmetic and 109 demos of evidence.

### What This Means for Eigenflower

DKC provides something no one else has: a mathematical PROOF ENVIRONMENT for neural
computation ideas. If you want to know whether ternary recursive models should work for
logical reasoning, DKC can give you a provable answer via the depth law and parity-lock
theorem, rather than an empirical one via training runs.

The papers converge on something real. Domingos tensor logic + Hilbert space operators give
the theory, BitNet + TRM give the architecture, GPTQ-as-CVP gives the compilation mechanism,
and Edge of Chaos tells you the corpus is suitable. The pieces actually fit — this is not a
Frankenstein assembly.

### The Gap That Remains

The bridge from DKC's Boolean-function-on-Z[zeta_8] world to actual natural language
processing on theological text. DKC proves that algebraic structure encodes computation.
Eigenflower needs to show that theological text has the right algebraic structure — or can
be mapped onto one.

Concretely:
- DKC operates on Boolean functions with 3-5 inputs. Theological reasoning operates on
  propositions with complex internal structure.
- DKC's "vocabulary" is 21-100 cyclotomic integers. Natural language vocabulary is 10,000+
  words.
- DKC's "activation" selects a Boolean output. Theological reasoning selects from a space
  of possible conclusions.
- DKC's proofs work because Z[zeta_8] has finite, exact, enumerable structure. Natural
  language does not.

The eigenflower program's central challenge is bridging this gap. The tensor logic framework
(Domingos) is the most promising bridge because it treats propositions as Boolean tensors
and syllogisms as einsum operations — the same formalism DKC already implements. But scaling
from 3-input Boolean functions to the propositional complexity of the Summa is nontrivial.

### Publishable Ideas Identified

1. **CVP-optimal weight quantization on cyclotomic lattices** (Group C crossover).
   Three-community intersection: lattice algorithms + algebraic number theory + neural
   network quantization. Implementation path exists from knotapel code.

2. **Algebraic vs dynamical complexity for reservoir computing** (Group D crossover).
   D106 falsifies topological entropy as a predictor; derived series depth and solvability
   are the correct measures. Refines the Edge of Chaos thesis.

3. **DKC as tensor logic over cyclotomic Boolean tensors** (Group D crossover).
   Domingos's framework has no implementation; DKC provides one with exact arithmetic.
   20,000+ lines of working C89 code.

4. **E8 lattice quantization via the icosian ring** (Group C crossover, Connection 12).
   Provably denser packing than Z^n quantization. Code exists on both sides.
